#include "core.h"
#include "actor.h"
#include "text_popup.h"

extern void *gLevelState;

extern u8 IsSwitchPressed(void *self);
extern void *CreateCrate(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u8 type);

/* Dispatches to the `CreateCrate` entity-constructor trampoline family
 * (docs/rom_map.md, "already-documented `CreateCrate` entity-constructor
 * trampoline family") with type `7` or `6` depending on
 * `IsSwitchPressed(gLevelState)`. */
void SpawnNitroSwitchCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    void *result;

    if (IsSwitchPressed(gLevelState)) {
        result = CreateCrate(arg0, arg1, arg2, arg3, 7);
    } else {
        result = CreateCrate(arg0, arg1, arg2, arg3, 6);
    }
    (void)result;
}

/* Plain `CreateCrate` trampoline, type `5`. */
void SpawnOutlineCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 5);
}

/* Plain `CreateCrate` trampoline, type `4`. */
void SpawnArrowCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 4);
}

/* Plain `CreateCrate` trampoline, type `3`. */
void SpawnIronSwitchCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 3);
}

/* Plain `CreateCrate` trampoline, type `2`. */
void SpawnAkuAkuCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 2);
}

/* Plain `CreateCrate` trampoline, type `1`. */
void SpawnCheckpointCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 1);
}

/* `CreateCrate` trampoline (type `0`), then indexes a small per-record
 * flags byte via `gEntityFlags`'s own table (same
 * `gEntityFlags -> *P -> {+8 array, +0xc base}` shape as
 * `UpdateStompedHopPad`'s table read in actor_part27c.c, indexed here by
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
    register void *obj asm("r5");
    register struct level_record_table *rec asm("r1");
    register u16 *arrayBase asm("r0");
    register s32 loaded asm("r4");
    register u8 *tmp asm("r0");
    register u8 *flagsAddr asm("r3");

    obj = CreateCrate(arg0, arg1, arg2, arg3, 0);

    rec = *gEntityFlags;
    arrayBase = rec->offsets;
    loaded = (arg3 << 1) + (s32)arrayBase;
    {
        s32 base = (s32)rec->bytes;
        loaded = *(u16 *)loaded;
        tmp = (u8 *)(loaded + base);
    }
    asm volatile("add %0, %1, #0" : "=r"(flagsAddr) : "r"(tmp));

    {
        register s32 two asm("r0") = 2;
        register u8 flagByte asm("r1");
        flagByte = *flagsAddr;
        two &= flagByte;
        if (two) {
            register u8 *addr asm("r0") = (u8 *)obj + 0x28;
            register s32 mask asm("r1");
            register u8 byte asm("r2");
            asm volatile("mov %0, #0x11\n\tneg %0, %0" : "=r"(mask));
            byte = *addr;
            mask &= byte;
            mask |= 0x10;
            *addr = mask;
        }
    }
    {
        register s32 four asm("r0") = 4;
        register u8 flagByte asm("r3");
        flagByte = *flagsAddr;
        four &= flagByte;
        if (four) {
            register u8 *addr asm("r0") = (u8 *)obj + 0x28;
            register s32 mask asm("r1");
            register u8 byte asm("r2");
            asm volatile("mov %0, #0x21\n\tneg %0, %0" : "=r"(mask));
            byte = *addr;
            mask &= byte;
            mask |= 0x20;
            *addr = mask;
        }
    }
}
