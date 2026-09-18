#include "core.h"

extern void sub_8024E68(void *self, void *vec2);
extern void sub_803AD84(void *addr, s32 a1, s32 a2, void *fn);
extern void sub_8024AA0(void *arg0, void *self);
extern void sub_8025E2C(void *self, s32 lo, s32 hi);
extern void sub_8025DE8(void *self, s32 lo, s32 hi);

/* Computes the four screen-edge tile coordinates from self's Q8
 * position (self+0/self+4) against the 240x160 GBA screen (0xef/0x9f
 * are one pixel short of the full extent, keeping the edge tile
 * inclusive; each coordinate uses the same floor-divide-by-8 "add 7
 * before the arithmetic shift when negative" idiom), re-derives the
 * streaming layer via `sub_8024E68`/`sub_8024AA0`, fires the two
 * `self->0x30`-table-driven trampolines (`+0x48`/`+0x40` records) with
 * the tile bounds via `sub_803AD84` (whose function-pointer argument
 * naturally lands in `r3`, the 4th AAPCS register, so - unlike
 * `sub_8025D28`'s `sub_803AD8C`/"bx r5" case - no register pin is
 * needed there), then clamps the streamed range via
 * `sub_8025E2C`/`sub_8025DE8` (game_loop15.c). The ROM pairs
 * `self->0x30+0x48`/`sub_8025E2C` with the Y-derived tiles and
 * `self->0x30+0x40`/`sub_8025DE8` with the X-derived ones.
 *
 * NAKED, not plain C: every field/offset/call is confirmed against the
 * ROM (see the plain-C reconstruction this replaced, still visible in
 * git history), but this compiler keeps `r8` live for the whole
 * function (the X-axis max tile, computed first but not consumed until
 * the very end) and a plain C version was not iterated to an exact
 * register allocation - transcribed straight from the confirmed-correct
 * ROM disassembly. See docs/matching/issue-41-game-loop-25894.md. */
NAKED void sub_8025E98(void *self, void *vec2)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "add r5, r0, #0\n\t"
        "bl sub_8024E68\n\t"
        "ldr r0, [r5]\n\t"
        "add r1, r0, #0\n\t"
        "cmp r0, #0\n\t"
        "bge 1f\n\t"
        "add r1, r0, #7\n\t"
    "1:\n\t"
        "asr r1, r1, #3\n\t"
        "mov r8, r1\n\t"
        "add r2, r0, #0\n\t"
        "add r2, r2, #0xef\n\t"
        "cmp r2, #0\n\t"
        "bge 2f\n\t"
        "add r2, r2, #7\n\t"
    "2:\n\t"
        "asr r7, r2, #3\n\t"
        "ldr r0, [r5, #4]\n\t"
        "add r1, r0, #0\n\t"
        "cmp r0, #0\n\t"
        "bge 3f\n\t"
        "add r1, r0, #7\n\t"
    "3:\n\t"
        "asr r6, r1, #3\n\t"
        "add r4, r0, #0\n\t"
        "add r4, r4, #0x9f\n\t"
        "cmp r4, #0\n\t"
        "bge 4f\n\t"
        "add r4, r4, #7\n\t"
    "4:\n\t"
        "asr r4, r4, #3\n\t"
        "ldr r1, [r5, #0x30]\n\t"
        "add r1, r1, #0x48\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r3, [r1, #4]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r4, #0\n\t"
        "bl sub_803AD84\n\t"
        "ldr r1, [r5, #0x30]\n\t"
        "add r1, r1, #0x40\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r3, [r1, #4]\n\t"
        "mov r1, r8\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_803AD84\n\t"
        "ldr r0, [r5, #0x2c]\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_8024AA0\n\t"
        "add r0, r5, #0\n\t"
        "mov r1, r8\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_8025E2C\n\t"
        "add r0, r5, #0\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r4, #0\n\t"
        "bl sub_8025DE8\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}

/* Truncates the Q8 X/Y position (self+0/self+4) to plain tile-scroll
 * halfwords at self+0x54/self+0x56 (read back together as one 32-bit
 * word), then writes that packed pair through the pointer at
 * self+0x58 - the `BGnHOFS`/`BGnVOFS` register pair address
 * `sub_8025D74` (game_loop15.c) caches there. */
void sub_8025F24(void *self)
{
    s32 x = *(s32 *)self;
    u8 *dst1 = (u8 *)self + 0x54;

    *(s16 *)dst1 = x;
    {
        s32 y = *(s32 *)((u8 *)self + 4);
        u8 *dst2 = (u8 *)self + 0x56;

        *(s16 *)dst2 = y;
    }
    *(s32 *)(*(void **)((u8 *)self + 0x58)) = *(s32 *)((u8 *)self + 0x54);
}

extern void *sub_8024B18(void *arg0, s32 arg1, s32 *outCol);

/* Streams decoded tile data into the circular row buffer at self+0x4c,
 * for rows self->0x3c..self->0x40 inclusive: `sub_8024B18` resolves
 * the decode table (self->0x2c, self->0x3c, and a column-cursor output
 * slot on the stack) once, then each row copies a halfword from the
 * decode table (indexed by the column cursor, wrapping every 32
 * columns via a 128-byte-stride column bank) into the circular buffer
 * at a position derived from `self`'s/`arg1`'s combined 32x32-tile
 * offset (`(rowStart mod 32) * 32 + (arg1 mod 32)`, each mod computed
 * with the same floor-divide-by-32 idiom as the bitmap-grid family in
 * game_loop12.c/game_loop13.c), advancing that combined offset by 1
 * row each time. The wraparound at the end of each row is a genuine
 * floor-divide/mod by 0x400 (32*32) - not a simple "subtract once if
 * over" clamp - using the same negative-adjust-then-shift idiom as the
 * two initial row/col mods, just at shift 10 instead of 5.
 *
 * NAKED, not plain C: the control flow, the floor-mod computations, the
 * 128-byte column stride and the floor-mod-0x400 wraparound are all
 * confirmed against the ROM's raw operations (see the plain-C
 * reconstruction this replaced, still visible in git history), but
 * this is a dense, register-heavy loop where the ROM keeps `r8` (and a
 * stack slot for the column cursor) live across the whole function; a
 * plain C version was not iterated to an exact register allocation.
 * Transcribed straight from the confirmed-correct ROM disassembly. See
 * docs/matching/issue-41-game-loop-25894.md. */
NAKED void sub_8025F3C(void *self, s32 arg1)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #4\n\t"
        "add r5, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "ldr r0, [r5, #0x2c]\n\t"
        "ldr r2, [r5, #0x3c]\n\t"
        "mov r3, sp\n\t"
        "bl sub_8024B18\n\t"
        "mov ip, r0\n\t"
        "ldr r2, [r5, #0x3c]\n\t"
        "add r0, r2, #0\n\t"
        "cmp r2, #0\n\t"
        "bge 1f\n\t"
        "add r0, r0, #0x1f\n\t"
    "1:\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r3, r2, r0\n\t"
        "add r0, r4, #0\n\t"
        "cmp r4, #0\n\t"
        "bge 2f\n\t"
        "add r0, r0, #0x1f\n\t"
    "2:\n\t"
        "asr r1, r0, #5\n\t"
        "lsl r0, r1, #5\n\t"
        "sub r1, r4, r0\n\t"
        "lsl r0, r3, #5\n\t"
        "add r3, r0, r1\n\t"
        "add r4, r2, #0\n\t"
        "ldr r0, [r5, #0x40]\n\t"
        "cmp r4, r0\n\t"
        "bgt 4f\n\t"
        "ldr r6, [r5, #0x4c]\n\t"
        "ldr r2, [sp]\n\t"
        "mov r1, #0x1f\n\t"
        "mov r8, r1\n\t"
        "add r5, r0, #0\n\t"
    "3:\n\t"
        "lsl r0, r3, #1\n\t"
        "add r0, r0, r6\n\t"
        "lsl r1, r2, #7\n\t"
        "add r1, ip\n\t"
        "ldrh r1, [r1]\n\t"
        "strh r1, [r0]\n\t"
        "add r2, r2, #1\n\t"
        "mov r7, r8\n\t"
        "and r2, r7\n\t"
        "add r1, r3, #0\n\t"
        "add r1, r1, #0x20\n\t"
        "add r0, r1, #0\n\t"
        "cmp r1, #0\n\t"
        "bge 5f\n\t"
        "ldr r7, 6f\n\t"
        "add r0, r3, r7\n\t"
    "5:\n\t"
        "asr r3, r0, #0xa\n\t"
        "lsl r0, r3, #0xa\n\t"
        "sub r3, r1, r0\n\t"
        "add r4, r4, #1\n\t"
        "cmp r4, r5\n\t"
        "ble 3b\n\t"
        "str r2, [sp]\n\t"
    "4:\n\t"
        "add sp, #4\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "6: .4byte 0x0000041f\n"
    );
}
