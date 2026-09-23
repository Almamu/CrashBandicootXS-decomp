#include "core.h"

/* Sits right after the still-raw remainder of asm/code_3_1_6.s'
 * SIO/link-cable and overlay_ui functions and before the small
 * fade/screen-mode utility cluster this file documents - see
 * docs/rom_map.md "A fourth thing in this file". This cluster's
 * parked functions interleave with the matched ones - see
 * docs/matching.md for the full split. */

extern void sub_80013FC(s32 factor);
extern void sub_80006A8(void);
extern u16 gUnknown_03000A80[512];
extern u16 gUnknown_03000E80[512];

/* Backs the real palette (0x05000000) up into `gUnknown_03000A80`,
 * then, for each factor 0/2/4/.../16, blends it toward black via the
 * already-matched `sub_80013FC` into `gUnknown_03000E80` and DMAs
 * that result into the real palette, waiting one VBlank between each
 * step - a textbook fade-to-black animation. Once fully faded, sets
 * up the hardware blend registers (`REG_BLDCNT`/`REG_BLDY`) and
 * restores the original backed-up palette.
 *
 * Was NAKED asm, not plain C - see
 * docs/matching/naked-sub_80014a4-matched.md for the derivation of how
 * this was finally matched as real C. The gap: the ROM caches the
 * blended-buffer address (`gUnknown_03000E80`) in a register across
 * the loop while recomputing the other two DMA fields (the 0x05000000
 * destination and the 0x80000200 control word) fresh every iteration,
 * plus an extra "rename" copy of the DMA register pointer right before
 * the loop, and reuses the loop's last-iteration register values
 * (rather than recomputing) for the final post-loop DMA setup too -
 * this compiler's loop-invariant hoisting pass never reproduces any of
 * that from plain C. Closed with register-pinned locals matching the
 * ROM's own register roles (including the "rename" copy) plus
 * inline-asm-materialized DMA-field writes (opaque to the hoisting
 * pass) for the fields the ROM keeps fresh, with the loop's own
 * asm-computed values threaded through as real operands so the
 * post-loop block reuses them exactly like the ROM does instead of
 * recomputing. */
void sub_80014A4(void)
{
    register struct dma_regs *dma asm("r1");
    register struct dma_regs *dma2 asm("r4");
    register s32 factor asm("r5");
    u32 val;
    register u32 *bufAddr asm("r6");
    register u32 dstVal asm("r3");
    register u32 cntVal asm("r2");

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = 0x05000000;
    dma->dst = (u32)gUnknown_03000A80;
    dma->cnt = 0x80000200;
    val = dma->cnt;

    factor = 0;
    dma2 = dma;
    bufAddr = (u32 *)gUnknown_03000E80;

    /* The 0x80000200 reload below deliberately references the literal
     * pool slot (`.L8+0x8`) this function's own compiler-generated
     * pool already holds it in (shared with the plain-C uses above and
     * `dstVal`/`cntVal`'s reuse below), rather than materializing its
     * own literal, to stay byte-identical to the ROM's single shared
     * pool entry - see the derivation doc for why. `REG_BLDCNT`/
     * `REG_BLDY` below are deliberately left as plain C (not also
     * folded into asm) specifically so this function's own literal
     * pool keeps a real, compiler-tracked entry for
     * `REG_ADDR_DMA3SAD`+16 (0x04000050) at `.L8+0x10` for this block
     * to reference - if a future edit to this function changes what
     * agbcc names its pool or how many words are in it (check a
     * `make NON_MATCHING=1` build's generated .s), update every
     * `.L8+`-prefixed reference in this function to match. */
    do {
        sub_80013FC(factor);
        sub_80006A8();
        dma2->src = (u32)bufAddr;
        asm volatile(
            "mov %0, #0xa0\n\t"
            "lsl %0, %0, #0x13\n\t"
            "str %0, [%2, #4]\n\t"
            "ldr %1, .L8+0x8\n\t"
            "str %1, [%2, #8]\n\t"
            "ldr r0, [%2, #8]\n\t"
            : "=r"(dstVal), "=r"(cntVal) : "r"(dma2) : "r0", "memory");
        factor += 2;
    } while (factor <= 0x10);

    REG_BLDCNT = 0xff;
    REG_BLDY = 0x10;

    /* Reload `dma` fresh from the pool (matching the ROM) instead of
     * letting the compiler notice it can cheaply derive
     * `REG_ADDR_DMA3SAD` from the `REG_ADDR_BLDY` value still live in
     * a register from the two writes above (`REG_ADDR_DMA3SAD` is
     * `REG_ADDR_BLDY + 0x80`) - a real, shorter instruction sequence
     * this compiler prefers, but not what the ROM does. */
    {
        register struct dma_regs *dma3 asm("r0");
        asm volatile(
            "ldr %0, .L8\n\t"
            "ldr r1, .L8+0x4\n\t"
            "str r1, [%0, #0]\n\t"
            "str %1, [%0, #4]\n\t"
            "str %2, [%0, #8]\n\t"
            "ldr %0, [%0, #8]\n\t"
            : "=r"(dma3), "+r"(dstVal), "+r"(cntVal) :: "r1", "memory");
    }
}

/* `gUnknown_030007E8.field_0 != -1` - the same "idle" sentinel
 * documented on the struct in fade_util.c, exposed here as a plain
 * s32 read (this file doesn't share that struct definition, per this
 * project's per-file raw-offset convention). */
extern s32 gUnknown_030007E8;

s32 sub_8001510(void)
{
    return gUnknown_030007E8 != -1;
}
