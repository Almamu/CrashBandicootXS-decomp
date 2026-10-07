#include "core.h"
#include "match.h"
#include "actor.h"
#include "text_popup.h"
#include "crates.h"
#include "level.h"
#include "globals.h"

/* Dispatches to the `CreateCrate` entity-constructor trampoline family
 * (docs/rom_map.md, "already-documented `CreateCrate` entity-constructor
 * trampoline family") with type `7` or `6` depending on
 * `IsSwitchPressed(gLevelState)`. */
void SpawnNitroSwitchCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    void *result;

    if (IsSwitchPressed(gLevelState)) {
        result = CreateCrate(arg0, arg1, arg2, arg3, CRATE_KIND_IRON);
    } else {
        result = CreateCrate(arg0, arg1, arg2, arg3, CRATE_KIND_NITRO_SWITCH);
    }
    (void)result;
}

/* Plain `CreateCrate` trampoline, type `5`. */
void SpawnOutlineCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, CRATE_KIND_OUTLINE);
}

/* Plain `CreateCrate` trampoline, type `4`. */
void SpawnArrowCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, CRATE_KIND_ARROW);
}

/* Plain `CreateCrate` trampoline, type `3`. */
void SpawnIronSwitchCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, CRATE_KIND_IRON_SWITCH);
}

/* Plain `CreateCrate` trampoline, type `2`. */
void SpawnAkuAkuCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, CRATE_KIND_AKU_AKU);
}

/* Plain `CreateCrate` trampoline, type `1`. */
void SpawnCheckpointCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, CRATE_KIND_CHECKPOINT);
}

/* `CreateCrate` trampoline (type `0`), then indexes a small per-record
 * flags byte via `gEntityFlags`'s own table (same
 * `gEntityFlags -> *P -> {+8 array, +0xc base}` shape as
 * `UpdateStompedHopPad`'s table read in tiny_hop_pad.c, indexed here by
 * `arg3`) and folds two of its bits into the constructed object's
 * `+0x28` bitfield.
 *
 * Register-pinning the table-resolution chain to the ROM's own
 * registers (`S` in r1, the array-base/loaded-value pair reusing r0/r4
 * in the ROM's own load order, an opaque `asm volatile` copy for the
 * final `adds r3,r0,#0` - a plain C copy always got optimized away
 * here) and using the same `mov #N; neg` negative-mask idiom as
 * `UPDATE_ICON_FRAME_NIBBLE` (src/menus/pause_menu_pages_init.c) for both
 * bitfield writes closes the whole function. */
void SpawnBasicCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MATCH_HOLD_REG(void *, obj, r5);
    MATCH_HOLD_REG(const struct level_entity_list *, rec, r1);
    MATCH_HOLD_REG(u16 *, arrayBase, r0);
    MATCH_HOLD_REG(s32, loaded, r4);
    MATCH_HOLD_REG(u8 *, tmp, r0);
    MATCH_HOLD_REG(u8 *, flagsAddr, r3);

    obj = CreateCrate(arg0, arg1, arg2, arg3, CRATE_KIND_BASIC);

    rec = gEntityFlags->list;
    arrayBase = (u16 *)rec->paramOffsets;
    loaded = (arg3 << 1) + (s32)arrayBase;
    {
        s32 base = (s32)rec->params;
        loaded = *(u16 *)loaded;
        tmp = (u8 *)(loaded + base);
    }
    asm volatile("add %0, %1, #0" : "=r"(flagsAddr) : "r"(tmp));

    {
        MATCH_HOLD_REG(s32, two, r0) = 2;
        MATCH_HOLD_REG(u8, flagByte, r1);
        flagByte = *flagsAddr;
        two &= flagByte;
        if (two) {
            MATCH_HOLD_REG(u8 *, addr, r0) = (u8 *)obj + 0x28;
            MATCH_HOLD_REG(s32, mask, r1);
            MATCH_HOLD_REG(u8, byte, r2);
            asm volatile("mov %0, #0x11\n\tneg %0, %0" : "=r"(mask));
            byte = *addr;
            mask &= byte;
            mask |= 0x10;
            *addr = mask;
        }
    }
    {
        MATCH_HOLD_REG(s32, four, r0) = 4;
        MATCH_HOLD_REG(u8, flagByte, r3);
        flagByte = *flagsAddr;
        four &= flagByte;
        if (four) {
            MATCH_HOLD_REG(u8 *, addr, r0) = (u8 *)obj + 0x28;
            MATCH_HOLD_REG(s32, mask, r1);
            MATCH_HOLD_REG(u8, byte, r2);
            asm volatile("mov %0, #0x21\n\tneg %0, %0" : "=r"(mask));
            byte = *addr;
            mask &= byte;
            mask |= 0x20;
            *addr = mask;
        }
    }
}
