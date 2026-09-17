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

#if NON_MATCHING
/* `sub_800FF0C` trampoline (type `0`), then indexes a small per-record
 * flags byte via `gUnknown_030012B4`'s own table (same
 * `gUnknown_030012B4 -> *P -> {+8 array, +0xc base}` shape as
 * `sub_80187FC`'s table read in actor_part27c.c, indexed here by
 * `arg3`) and folds two of its bits into the constructed object's
 * `+0x28` bitfield. Uses the same explicit-register-pin technique as
 * `UPDATE_ICON_FRAME_NIBBLE` (src/graphics/settings_menu6.c) for the
 * negative-mask idiom (confirmed correct, matches byte-for-byte) - gcc
 * otherwise materializes the negative mask as a single-instruction
 * bitwise-complement immediate instead of the ROM's actual two's-
 * complement `movs`/`rsbs` pair (`-0x11` == `0xEF`, one more than the
 * plain bitwise-NOT `~0x11` == `0xEE`).
 *
 * NOT YET BYTE-MATCHING: every field/mask/branch is confirmed correct
 * and the bit-test/mask-write tail matches the ROM exactly, but the
 * middle "resolve the per-record flags byte" section doesn't - the ROM
 * computes the array-base read, the index-shifted address, and the
 * `+0xc` base-pointer read in a specific order that keeps the record's
 * base object (`S = *(void **)gUnknown_030012B4`) live in `r1` across
 * both field reads and only copies the final resolved address into a
 * third register (`adds r3, r0, #0`) right at the end, whereas this
 * compiler resolves the same value one register short - every ordering
 * of the reads/locals tried here still collapses that last copy away.
 * Parked rather than fight gcc's CSE for a 4-byte gap. */
void sub_8021D04(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register void *obj asm("r5") = sub_800FF0C(arg0, arg1, arg2, arg3, 0);
    void *s = *(void **)gUnknown_030012B4;
    u16 *offsets = *(u16 **)((u8 *)s + 8);
    u16 offset = offsets[(u16)arg3];
    u8 *addr = *(u8 **)((u8 *)s + 0xc) + offset;

    if (*addr & 2) {
        register u8 *_addr asm("r0") = (u8 *)obj + 0x28;
        register s32 _mask asm("r1");
        register u8 _byte asm("r2");

        asm volatile("mov %0, #0x11\n\tneg %0, %0" : "=r" (_mask));
        _byte = *_addr;
        _mask &= _byte;
        _mask |= 0x10;
        *_addr = _mask;
    }
    if (*addr & 4) {
        register u8 *_addr asm("r0") = (u8 *)obj + 0x28;
        register s32 _mask asm("r1");
        register u8 _byte asm("r2");

        asm volatile("mov %0, #0x21\n\tneg %0, %0" : "=r" (_mask));
        _byte = *_addr;
        _mask &= _byte;
        _mask |= 0x20;
        *_addr = _mask;
    }
}
asm(".align 2, 0");
#endif /* NON_MATCHING */
