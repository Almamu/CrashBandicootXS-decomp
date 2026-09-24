#include "core.h"

extern void sub_8026ED0(void *self);

/* If bit 0 of `flags` is set, forwards to `sub_8026ED0` - identical
 * shape to `sub_8006FC8` (src/graphics/graphics.c). */
void sub_8025444(void *self, u32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

void nullsub_4(void)
{
}
asm(".align 2, 0");

/* See game_loop3.c for the full `tile_cache` doc comment - duplicated
 * here (not shared via a header) since it's only ever accessed through
 * a raw pointer parameter in this cluster of files. */
struct tile_cache {
    void *source;      /* 0x000 */
    void *decodeBase;  /* 0x004 */
    s32 unk008;         /* 0x008 */
    s32 unk00c;          /* 0x00c */
    s32 unk010;           /* 0x010 */
    s32 unk014;            /* 0x014 */
    s32 width;               /* 0x018 */
    s32 height;                /* 0x01c */
    u8 buf[16][0x100];           /* 0x020 - 0x1020 */
    s32 id[16];                    /* 0x1020 - 0x105c */
    s32 nextSlot;                    /* 0x1060 */
};

extern void *sub_8024F24(struct tile_cache *self, s32 recordId);

/* Same lookup as `sub_80250BC`, but returns the raw decoded halfword
 * directly (no bounds check, no terrain-table lookup) and also writes
 * the cell's top nibble out through `hiOut`.
 *
 * Same register-allocation-permutation gap `sub_8025130`/`sub_8025228`
 * (game_loop3.c) had - closed the same way: hand-transcribed
 * instruction-for-instruction from the ROM disassembly (formerly
 * `asm/code_3_2_17_25460.s`). This was the last unclosed member of the
 * issue #40 terrain-tile-cache cluster - see
 * docs/matching/issue-40-terrain-tile-cache.md. */
NAKED u16 sub_8025460(struct tile_cache *self, s32 x, s32 y, u8 *flagsOut, s32 *hiOut)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r5, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "add r7, r3, #0\n\t"
        "cmp r4, #0\n\t"
        "blt 1f\n\t"
        "cmp r6, #0\n\t"
        "bge 2f\n\t"
        "1:\n\t"
        "mov r0, #0\n\t"
        "b 4f\n\t"
        "2:\n\t"
        "asr r3, r4, #4\n\t"
        "asr r1, r6, #3\n\t"
        "ldr r2, [r5]\n\t"
        "ldr r0, [r5, #0x18]\n\t"
        "mul r0, r1, r0\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrh r1, [r0]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8024F24\n\t"
        "mov r1, #7\n\t"
        "and r1, r6\n\t"
        "mov r3, #0xf\n\t"
        "and r4, r3\n\t"
        "lsl r1, r1, #4\n\t"
        "add r1, r1, r4\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r0\n\t"
        "ldrh r1, [r1]\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r4, r1, #0x10\n\t"
        "lsr r2, r1, #0x1c\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "str r2, [r0]\n\t"
        "lsr r1, r1, #0x18\n\t"
        "and r1, r3\n\t"
        "cmp r1, #0\n\t"
        "beq 3f\n\t"
        "strb r1, [r7]\n\t"
        "3:\n\t"
        "mov r0, #0xff\n\t"
        "and r0, r4\n\t"
        "4:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}
