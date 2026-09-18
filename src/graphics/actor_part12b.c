#include "core.h"
#include "actor.h"

extern s32 sub_8008278(void *posQ8, void *arg1);
extern s32 sub_8026628(void *player, void *arg1, void *posInt, s32 arg3, void *outY);
extern void *gUnknown_03001308;

/* A physics/collision "step probe": makes a working copy of `self`'s
 * position (`self->x`/`self->y`), runs it through `sub_8008278`
 * (still unexamined - some kind of movement/gravity step, taking the
 * position pointer and `arg1` alongside it), converts the result from
 * Q8 fixed-point to plain integers, resets `self+0x69` (an attempt
 * counter) to 0, then probes the position via `sub_8026628` (also
 * still unexamined - takes `gUnknown_03001308`, `arg1`, the working
 * integer position, `arg2` - `*(u8 *)(arg2+4)` - and a pointer to
 * `self`'s original Q8 `y`).
 *
 * If the first probe succeeds: restores `self->y` to its original
 * value (undoing whatever `sub_8008278` mutated) and returns `1`.
 *
 * Otherwise: clears `gUnknown_03001308`'s `+0x2a` flag byte (saving
 * its old value) and retries the probe up to 3 more times, nudging the
 * working Y position down by `8` (Q8, i.e. `1/32` of a pixel-ish unit)
 * each attempt and incrementing `self+0x69`'s attempt counter; whether
 * a retry succeeds or all 4 attempts are exhausted, restores
 * `gUnknown_03001308`'s `+0x2a` byte to its saved value and returns
 * `0` either way - only the very first, un-nudged probe returning
 * success is distinguished by this function's return value.
 *
 * Written as NAKED asm, not plain C: fully understood (every load,
 * store, and branch confirmed against the ROM disassembly, including
 * both `sub_8008278`/`sub_8026628` call signatures traced from their
 * own call sites here), but a real-C reconstruction following this
 * shape couldn't reproduce the ROM's choice to keep `self+0x69`'s
 * address in `r6` for the whole retry loop while also reusing that
 * same value as the loop's own termination test operand - gcc
 * consistently reloaded the address a second time instead. Parked as a
 * direct transcription of the ROM's own confirmed-correct instructions
 * instead. */
NAKED s32 sub_8009BE0(void *self, void *arg1, void *arg2)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x10\n\t"
        "add r5, r0, #0\n\t"
        "mov sb, r1\n\t"
        "ldrb r0, [r2, #4]\n\t"
        "mov sl, r0\n\t"
        "ldr r0, [r5, #4]\n\t"
        "str r0, [sp, #0xc]\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r1, [r5, #4]\n\t"
        "str r0, [sp, #4]\n\t"
        "str r1, [sp, #8]\n\t"
        "add r0, sp, #4\n\t"
        "mov r1, sb\n\t"
        "bl sub_8008278\n\t"
        "ldr r0, [sp, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r0, [sp, #8]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [sp, #8]\n\t"
        "add r6, r5, #0\n\t"
        "add r6, #0x69\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r6]\n\t"
        "ldr r1, 1f\n\t"
        "mov r8, r1\n\t"
        "ldr r0, [r1]\n\t"
        "add r4, sp, #0xc\n\t"
        "str r4, [sp]\n\t"
        "mov r1, sb\n\t"
        "add r2, sp, #4\n\t"
        "mov r3, sl\n\t"
        "bl sub_8026628\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r1, r0, #0x18\n\t"
        "cmp r1, #0\n\t"
        "beq 2f\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "str r0, [r5, #4]\n\t"
        "mov r0, #1\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001308\n"
    "2:\n\t"
        "mov r2, r8\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #0x2a\n\t"
        "ldrb r7, [r0]\n\t"
        "strb r1, [r0]\n\t"
        "add r5, r6, #0\n\t"
        "add r4, sp, #4\n\t"
    "3:\n\t"
        "ldrb r0, [r5]\n\t"
        "add r0, #1\n\t"
        "strb r0, [r5]\n\t"
        "ldr r0, [r4, #4]\n\t"
        "add r0, #8\n\t"
        "str r0, [r4, #4]\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1]\n\t"
        "add r2, sp, #0xc\n\t"
        "str r2, [sp]\n\t"
        "mov r1, sb\n\t"
        "add r2, sp, #4\n\t"
        "mov r3, sl\n\t"
        "bl sub_8026628\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #2\n\t"
        "bls 3b\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1]\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
    "5:\n\t"
        "add r0, #0x2a\n\t"
        "strb r7, [r0]\n\t"
        "mov r0, #0\n\t"
    "6:\n\t"
        "add sp, #0x10\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "7: .4byte gUnknown_03001308\n"
    );
}
