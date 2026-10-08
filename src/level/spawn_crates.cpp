#include "crate.hpp"
#include "spawners.hpp"
#include "level_state.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
}

/* Dispatches to the `CreateCrate` entity-constructor trampoline family
 * (docs/rom_map.md, "already-documented `CreateCrate` entity-constructor
 * trampoline family") with type `7` or `6` depending on
 * `gLevelState->IsSwitchPressed()`. */
void SpawnNitroSwitchCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (gLevelState->IsSwitchPressed())
        Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_IRON);
    else
        Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_NITRO_SWITCH);
}

/* Plain `CreateCrate` trampoline, type `5`. */
void SpawnOutlineCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_OUTLINE);
}

/* Plain `CreateCrate` trampoline, type `4`. */
void SpawnArrowCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_ARROW);
}

/* Plain `CreateCrate` trampoline, type `3`. */
void SpawnIronSwitchCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_IRON_SWITCH);
}

/* Plain `CreateCrate` trampoline, type `2`. */
void SpawnAkuAkuCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_AKU_AKU);
}

/* Plain `CreateCrate` trampoline, type `1`. */
void SpawnCheckpointCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_CHECKPOINT);
}

/* The mirror bits stored from a parameter: inlined at the RTL level, the
 * store is a general bitfield insert (the field cleared with `~0x10`, the
 * value ORed in), as in the ROM, where a literal 1 is a plain OR. */
static inline void SetFlipX(Crate *c, u32 v)
{
    c->mirrorFlags.mirrorX = v;
}

static inline void SetFlipY(Crate *c, u32 v)
{
    c->mirrorFlags.mirrorY = v;
}

/* A basic crate (type 0), mirrored in X and Y by bits 1 and 2 of its
 * parameter record's flags.
 *
 * old_agbcp (Makefile OLD_AGBCC_OBJS; agbcc before): the masks are
 * loaded before the bytes they apply to. The C held agbcc to that order
 * with 16 pins and 3 `asm` statements. */
void SpawnBasicCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate *obj = Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_BASIC);
    const struct entity_params *rec = EntityParams(arg3);

    if (rec->flags & 2)
        SetFlipX(obj, 1);
    if (rec->flags & 4)
        SetFlipY(obj, 1);
}
