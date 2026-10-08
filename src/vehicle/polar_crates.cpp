#define POLAR_CRATE_CONSTRUCTORS_OUT_OF_LINE
#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "audio.h"
#include "actor.h"
#include "level.h"
#include "globals.h"
}

/* The polar crate kinds' updates, PolarCrate's constructor and the kinds'
 * out-of-line constructors (#664 part 11d, include/vehicle.hpp), ROM
 * 0x0802CA60-0x0802CC9C, between polar_nitro.cpp and polar_objects.cpp.
 * Each kind breaks (PolarCrate::Break) when the player touches it, then
 * hands the rest to PolarCrate::Update (polar_pickups.cpp), which also
 * breaks it on the yeti and deletes it once broken. */

/* A mask for the player; the yeti breaks it for nothing. */
void PolarAkuAkuCrate::Update()
{
    if (animIndex != 0x12 && (u8)IsTouchingPlayer(this)) {
        AddBrokenCrate(gLevelState);
        static_cast<PolarPlayer *>(gActorList)->GiveMask();
        Break();
        palette = 1;
    }

    if (animIndex != 0x12 && IsTouchingYeti(this)) {
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        AddBrokenCrate(gLevelState);
        Break();
        palette = 1;
    }

    PolarCrate::Update();
}

/* Freezes the clock: 1, 2 or 3 seconds for kinds 5, 6 and 7. */
void PolarTimeCrate::Update()
{
    if (animIndex != 0x12 && (u8)IsTouchingPlayer(this)) {
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        AddBrokenCrate(gLevelState);

        switch ((u8)record->index) {
        case 5:
            FreezeLevelClock(gLevelState, 1);
            break;
        case 6:
            FreezeLevelClock(gLevelState, 2);
            break;
        case 7:
            FreezeLevelClock(gLevelState, 3);
            break;
        }

        Break();
    }

    PolarCrate::Update();
}

/* Explodes, as each nitro DetonateNearby sets off. UNUSED: no caller
 * anywhere in the ROM (checked every src/ file and every word-aligned
 * Thumb pointer in baserom.gba). */
void PolarNitroCrate::Detonate()
{
    if (animIndex != 0x12) {
        PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
        AddBrokenCrate(gLevelState);
        Explode();
    }
}

/* The CreateActor kind-10 crate: a plain wooden crate worth 4 wumpa
 * (PolarBasicCrate gives 1). Its animation record,
 * gCategoryFamily0AnimTable[10], is kind 1's with only the index changed
 * (gPolarBasicCrateKeyframes/Frames, palette 1), so in game it looks just
 * like a basic crate. Only the first polar stage (gCategory0SpawnTable)
 * places it: four times, in two side-by-side pairs; in time trials one
 * pair becomes time crates (kind 7). */
void PolarFourWumpaCrate::Update()
{
    if (animIndex != 0x12 && (u8)IsTouchingPlayer(this)) {
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        AddBrokenCrate(gLevelState);
        static_cast<PolarPlayer *>(gActorList)->QueueWumpa(4);
        Break();
    }

    PolarCrate::Update();
}

/* A wumpa. */
void PolarBasicCrate::Update()
{
    if (animIndex != 0x12 && (u8)IsTouchingPlayer(this)) {
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        AddBrokenCrate(gLevelState);
        static_cast<PolarPlayer *>(gActorList)->QueueWumpa(1);
        Break();
    }

    PolarCrate::Update();
}

/* The look, from the crate's place: six columns by x ((x + 60) / 20,
 * clamped to 0-5), in three rows by height (+6 at y <= 43, +6 more at
 * y <= 6). */
PolarCrate::PolarCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
    s32 idx = (Q8_TO_INT(x) + 0x3c) / 0x14;
    s32 row;

    LIMIT_MIN(idx, 0);
    LIMIT_MAX(idx, 5);

    row = Q8_TO_INT(y);
    if (row <= 0x2b)
        idx += 6;
    if (row <= 6)
        idx += 6;

    RestartAnim(idx);
}

/* The kinds' constructors, out of line (polar_crate_ctors.hpp). */
#define POLAR_CRATE_CTOR
#include "polar_crate_ctors.hpp"
