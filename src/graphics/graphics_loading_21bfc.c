#include "core.h"
#include "actor.h"

extern void *gUnknown_030012C0;
extern void *gUnknown_030012B4;

extern u8 sub_80232C8(void *self);
extern void *sub_800FF0C(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u8 type);

/* Dispatches to the `sub_800FF0C` entity-constructor trampoline family
 * (docs/rom_map.md, "already-documented `sub_800FF0C` entity-constructor
 * trampoline family") with type `7` or `6` depending on
 * `sub_80232C8(gUnknown_030012C0)`. */
void sub_8021BFC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    void *result;

    if (sub_80232C8(gUnknown_030012C0)) {
        result = sub_800FF0C(arg0, arg1, arg2, arg3, 7);
    } else {
        result = sub_800FF0C(arg0, arg1, arg2, arg3, 6);
    }
    (void)result;
}

/* Plain `sub_800FF0C` trampoline, type `5`. */
void sub_8021C50(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 5);
}

/* Plain `sub_800FF0C` trampoline, type `4`. */
void sub_8021C74(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 4);
}

/* Plain `sub_800FF0C` trampoline, type `3`. */
void sub_8021C98(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 3);
}

/* Plain `sub_800FF0C` trampoline, type `2`. */
void sub_8021CBC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 2);
}

/* Plain `sub_800FF0C` trampoline, type `1`. */
void sub_8021CE0(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 1);
}

/* `sub_800FF0C` trampoline (type `0`), then indexes a small per-record
 * flags byte via `gUnknown_030012B4`'s own table (same
 * `gUnknown_030012B4 -> *P -> {+8 array, +0xc base}` shape as
 * `sub_80187FC`'s table read in actor_part27c.c, indexed here by
 * `arg3`) and folds two of its bits into the constructed object's
 * `+0x28` bitfield.
 *
 * Register-pinning the table-resolution chain to the ROM's own
 * registers (`S` in r1, the array-base/loaded-value pair reusing r0/r4
 * in the ROM's own load order, an opaque `asm volatile` copy for the
 * final `adds r3,r0,#0` - a plain C copy always got optimized away
 * here) and using the same `mov #N; neg` negative-mask idiom as
 * `UPDATE_ICON_FRAME_NIBBLE` (src/graphics/settings_menu6.c) for both
 * bitfield writes closes the whole function. */
void sub_8021D04(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register void *obj asm("r5");
    register u8 *rec asm("r1");
    register u16 *arrayBase asm("r0");
    register s32 loaded asm("r4");
    register u8 *tmp asm("r0");
    register u8 *flagsAddr asm("r3");

    obj = sub_800FF0C(arg0, arg1, arg2, arg3, 0);

    rec = *(u8 **)gUnknown_030012B4;
    arrayBase = *(u16 **)(rec + 8);
    loaded = (arg3 << 1) + (s32)arrayBase;
    {
        s32 base = *(s32 *)(rec + 0xc);
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
