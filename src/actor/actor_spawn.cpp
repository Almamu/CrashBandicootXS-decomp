#include "actor_self.hpp"
#include "vehicle.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "globals.h"
}

/* gActorSpawnTable[] - the category's "spawnTable" runtime array,
 * category_descriptor.spawnTable (include/actor_anim.h, offset
 * 0x14), stride 0x14 bytes - see docs/rom_map.md's "sub_802A5xx
 * siblings pin down spawnTable's runtime shape" finding (a real
 * ROM data dump confirmed record 0 doubles as a combined header+entry,
 * its own `link` holding the entry count). Set up by
 * SelectActorCategory.
 *
 * GetActorSpawnNextTarget/GetActorSpawnZ both read one record *past* the index they're
 * given (their address math works out to base+idx*0x14+0x18 and
 * +0x14 respectively, i.e. `&record[idx+1].link`/`&record[idx+1].depth`
 * - not a distinct field of record[idx] itself, confirmed by
 * matching the exact ROM instruction order: this compiler computes the
 * base+constant step before the base+idx*stride step for these two
 * specifically, unlike every plain record[idx].field access elsewhere
 * in this file, which folds its constant straight into the load).
 * The record is actor_anim.h's `struct sub_effect_record` (this file had
 * a copy).
 *
 * The per-spawn accessors below are what the homing actors (AimJetpackPlane,
 * AimPolarPenguin) use to fly to a target spawn: its X/Y/Z, its kind (which
 * picks their speed) and its next target. */

/* Trivial getter - the big loading-loop call counter set by
 * InitActorCategory's own loop tail (still raw). */
s32 GetActorCategoryFrameCount(void)
{
    return gActorCategoryFrameCount;
}

/* Trivial getter, Q8.8-converted. */
s32 GetActorSpawnOffset(void)
{
    return INT_TO_Q8(gActorSpawnOffset);
}

void ResumeActorSpawns(void)
{
    gActorSpawnsPaused = 0;
}

void PauseActorSpawns(void)
{
    gActorSpawnsPaused = 1;
}

/* Spawn `idx`'s next target (the *next* record's `link`, see the struct
 * comment above): the spawn index a homing actor goes to after this one,
 * -1 at the end of the chain. Written
 * as "cache base pointer, then compute idx*stride, then add the
 * constant record-boundary offset to the base before combining" to
 * match this compiler's exact instruction order for this specific
 * shape - see struct comment. */
s32 GetActorSpawnNextTarget(s32 idx)
{
    u8 *base = (u8 *)gActorSpawnTable;
    s32 off = idx * 0x14;

    base = base + 0x18;
    return *(s32 *)(base + off);
}

/* Spawn `idx`'s Z: the *next* record's `depth`, offset by
 * gActorSpawnOffset, Q8.8-converted. Same instruction-order shape as
 * GetActorSpawnNextTarget. */
s32 GetActorSpawnZ(s32 idx)
{
    u8 *base = (u8 *)gActorSpawnTable;
    s32 off = idx * 0x14;

    base = base + 0x14;
    return INT_TO_Q8(*(s32 *)(base + off) + gActorSpawnOffset);
}

/* Spawn `idx`'s Y and X (`offsetY`/`offsetX`), Q8.8-converted. */
s32 GetActorSpawnY(s32 idx)
{
    u8 *base = (u8 *)gActorSpawnTable;
    s32 off = idx * 0x14;

    base = base + 0x10;
    return INT_TO_Q8(*(s32 *)(base + off));
}

s32 GetActorSpawnX(s32 idx)
{
    u8 *base = (u8 *)gActorSpawnTable;
    s32 off = idx * 0x14;

    base = base + 0xc;
    return INT_TO_Q8(*(s32 *)(base + off));
}

/* Spawn `idx`'s kind as SpawnActor picks it (`kind`, `altKind` in a time
 * trial (gLevelState+0x8c), `bonusKind` while gActorSpawnUseBonus is set),
 * less 0x20: the index into the homing actors' speed tables
 * (iwram_data.cpp) - see docs/rom_map.md's "sub_802A5xx siblings" entry.
 * This one *is* record[idx] itself (not idx+1), with the table pointer
 * loaded before the index is scaled; its fields fold straight into the
 * `ldrb` offsets. */
s32 GetActorSpawnKindIndex(s32 idx)
{
    struct sub_effect_record *base = gActorSpawnTable;
    struct sub_effect_record *record = &base[idx];
    u8 v;

    v = record->kind;
    if (gLevelState->timeTrial != 0) {
        v = record->altKind;
    } else if (gActorSpawnUseBonus != 0) {
        v = record->bonusKind;
    }
    return v - 0x20;
}
