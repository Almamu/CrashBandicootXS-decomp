#include "core.h"
#include "actor.h"

extern void *gUnknown_030012C0;
extern void *gUnknown_030012B4;
extern struct actor *gUnknown_030012D8;
extern void *gUnknown_030012BC;

extern u8 sub_80232F4(void *self);
extern s32 sub_80232E0(void *self);
extern s32 sub_8023130(void *self);
extern s32 sub_803AFEC(void *self);
extern u8 sub_80232B8(void *self);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void PlaySfx(void *bank, s32 arg1, s32 sfxId);

/* Sound-trigger dispatch/position writer - the last of the
 * `LoadGraphicsPackage` cluster's scratch-buffer-style helper family
 * (issue #30). Two independent, unrelated halves:
 *
 * 1. If `sub_80232F4(gUnknown_030012C0)` (the player's `+0xa8` flag)
 *    is set: looks up a per-`z` flags byte via the same
 *    `gUnknown_030012B4 -> *rec -> {+8 offsets[], +0xc base}` table
 *    `sub_8021D04` (graphics_loading_21bfc.c) already reads, folds
 *    its bit 1 into the player's `+0x28` bitfield's bit 4, then
 *    unconditionally writes the incoming `x`/`y` (Q8.8, shifted from
 *    the raw `u16` args) into the player's own `x`/`y` fields - the
 *    same unconditional write `sub_80221A4`/`sub_80221D4`
 *    (graphics_loading_21d80.c) already do elsewhere in this cluster.
 *
 * 2. Unless the player's `+0x8c` "paused" flag is set: fires the
 *    player's `table+0x68` trampoline (via `sub_803AD88`, action
 *    `0x1a`) and plays SFX `0x100` through `gUnknown_030012BC`,
 *    unless a budget/reentrancy guard trips first - either the
 *    player's spawn counter (`sub_80232E0`, `+0x7c`) has room against
 *    its cap (`sub_8023130`, `+0x84`), or (when it doesn't) all three
 *    of `sub_803AFEC` (`+0x74`), `sub_80232B8` (`+0xa4`) and the
 *    player's `+0x78` mode field agree it's still safe to fire.
 *
 * Written as NAKED asm, not plain C: semantics fully understood (a
 * plain-C reconstruction with this exact meaning compiles cleanly and
 * every operation/field/register was confirmed correct against the
 * ROM in isolation - see the removed NON_MATCHING draft in git
 * history), but the first half's table-resolution/bitfield-pack
 * section never converged on the ROM's own register choices (`byte`
 * in r0, the shifted bit in r2, the mask constant reusing r0 via a
 * `movs r0,#1`/`subs r0,#0x12` derivation rather than a fresh
 * literal) no matter how the C was restructured - the exact same
 * `gUnknown_030012B4 -> *rec -> {+8, +0xc}` resolution shape already
 * documented as unmatchable via plain C for `sub_8021D04`
 * (graphics_loading_21bfc.c, issue #33) for the same reason.
 * Transcribed instruction-for-instruction from the ROM disassembly
 * instead, the same escape hatch used there and for
 * `sub_8001CB8`/`sub_8001DB4` (src/system/link_cable.c). The
 * `sub_803AD88` call's function-pointer half (`table+0x68+4`, loaded
 * into r4) is a genuine "dead read" - loaded but never actually
 * passed to `sub_803AD88` (a plain 4-argument function, not itself a
 * trampoline) - the same idiom documented for `sub_8009FD4`
 * (actor_part9.c) and `sub_8007DBC`. */
NAKED void sub_801E990(u32 arg0, u16 x, u16 y, u16 z)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r6, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r7, r2, #0x10\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r4, r3, #0x10\n\t"
        "ldr r5, 10f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80232F4\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r2, #8]\n\t"
        "lsl r1, r4, #1\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r2, #0xc]\n\t"
        "ldrh r1, [r1]\n\t"
        "add r0, r1, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "lsr r2, r0, #1\n\t"
        "mov r0, #1\n\t"
        "ldr r3, 12f\n\t"
        "ldr r1, [r3]\n\t"
        "add r1, #0x28\n\t"
        "and r2, r0\n\t"
        "lsl r2, r2, #4\n\t"
        "sub r0, #0x12\n\t"
        "ldrb r4, [r1]\n\t"
        "and r0, r4\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r1]\n\t"
        "ldr r1, [r3]\n\t"
        "lsl r0, r6, #8\n\t"
        "str r0, [r1]\n\t"
        "lsl r0, r7, #8\n\t"
        "str r0, [r1, #4]\n\t"
    "1:\n\t"
        "ldr r1, [r5]\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "add r0, r1, #0\n\t"
        "bl sub_80232E0\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8023130\n\t"
        "cmp r4, r0\n\t"
        "bge 2f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_803AFEC\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80232B8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
    "2:\n\t"
        "ldr r0, 12f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0x1a\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "ldr r0, 13f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #1\n\t"
        "bl PlaySfx\n\t"
    "3:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "10: .4byte gUnknown_030012C0\n"
    "11: .4byte gUnknown_030012B4\n"
    "12: .4byte gUnknown_030012D8\n"
    "13: .4byte gUnknown_030012BC\n"
    );
}
