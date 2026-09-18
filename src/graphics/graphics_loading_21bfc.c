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
 * Written as NAKED asm, not plain C: the bit-test/mask-write tail
 * matches the ROM exactly (the same explicit-register-pin technique as
 * `UPDATE_ICON_FRAME_NIBBLE` in src/graphics/settings_menu6.c for the
 * negative-mask idiom), but the middle "resolve the per-record flags
 * byte" section's register allocation never converged - see
 * docs/matching/issue-30-graphics-loading.md - so it's transcribed
 * instruction-for-instruction from the ROM disassembly instead. */
NAKED void sub_8021D04(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "sub sp, #4\n\t"
        "add r4, r3, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "lsl r4, r4, #0x10\n\t"
        "lsr r4, r4, #0x10\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r3, #0\n\t"
        "str r3, [sp]\n\t"
        "add r3, r4, #0\n\t"
        "bl sub_800FF0C\n\t"
        "add r5, r0, #0\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "lsl r4, r4, #1\n\t"
        "add r4, r4, r0\n\t"
        "ldr r0, [r1, #0xc]\n\t"
        "ldrh r4, [r4]\n\t"
        "add r0, r4, r0\n\t"
        "add r3, r0, #0\n\t"
        "mov r0, #2\n\t"
        "ldrb r1, [r3]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x28\n\t"
        "mov r1, #0x11\n\t"
        "neg r1, r1\n\t"
        "ldrb r2, [r0]\n\t"
        "and r1, r2\n\t"
        "mov r2, #0x10\n\t"
        "orr r1, r2\n\t"
        "strb r1, [r0]\n\t"
    "2:\n\t"
        "mov r0, #4\n\t"
        "ldrb r3, [r3]\n\t"
        "and r0, r3\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x28\n\t"
        "mov r1, #0x21\n\t"
        "neg r1, r1\n\t"
        "ldrb r2, [r0]\n\t"
        "and r1, r2\n\t"
        "mov r2, #0x20\n\t"
        "orr r1, r2\n\t"
        "strb r1, [r0]\n\t"
    "3:\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012B4\n"
    );
}
