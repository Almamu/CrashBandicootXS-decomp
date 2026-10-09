#include "crate.hpp"
#include "spawners.hpp"
#include "level_state.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
}

/* The crate spawners (#664, include/spawners.hpp), ROM
 * 0x08021A4C-0x08021D80. Built with old_agbcp. */

/* Plain `CreateCrate` entity-constructor trampoline (docs/rom_map.md;
 * same dispatch family as types 1-7 below), type `0x12`. */
void SpawnTimeCrate3(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_TIME_3);
}

/* Plain `CreateCrate` trampoline, type `0x11`. */
void SpawnTimeCrate2(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_TIME_2);
}

/* Plain `CreateCrate` trampoline, type `0x10`. */
void SpawnTimeCrate1(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_TIME_1);
}

/* Plain `CreateCrate` trampoline, type `0xf`. */
void SpawnSlotCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_SLOT);
}

/* Plain `CreateCrate` trampoline, type `0xe`. */
void SpawnTntCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_TNT);
}

/* Plain `CreateCrate` trampoline, type `0xd`: the reinforced crate (bank
 * 31 animation 6, a wooden crate with metal-banded edges). Only a body
 * slam (attack kind 5, gActionCtrlStateAttackKinds) or the invincibility
 * mask breaks it: gCrateHitResponse row 13 bounces every other attack,
 * and QueueCratePlayerCollision turns a body slam moving up into a bounce
 * too. */
void SpawnReinforcedCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_REINFORCED);
}

/* Plain `CreateCrate` trampoline, type `0xc`. */
void SpawnBouncyWumpaCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_BOUNCY_WUMPA);
}

/* Plain `CreateCrate` trampoline, type `0xb`. */
void SpawnMysteryCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_MYSTERY);
}

/* Plain `CreateCrate` trampoline, type `0xa`. */
void SpawnNitroCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_NITRO);
}

/* Plain `CreateCrate` trampoline, type `9`. */
void SpawnLifeCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_LIFE);
}

/* Plain `CreateCrate` trampoline, type `8`. */
void SpawnIronArrowCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_IRON_ARROW);
}

/* Plain `CreateCrate` trampoline, type `7`. */
void SpawnIronCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_IRON);
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
        obj->SetFlipX(1);
    if (rec->flags & 4)
        obj->SetFlipY(1);
}
