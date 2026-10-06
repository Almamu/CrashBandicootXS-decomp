#include "core.h"
#include "match.h"
#include "system.h"
#include "gfx.h"

/* Sits right after the still-raw remainder of asm/code_3_1_6.s'
 * SIO/link-cable and overlay_ui functions and before the small
 * fade/screen-mode utility cluster this file documents - see
 * docs/rom_map.md "A fourth thing in this file". This cluster's
 * parked functions interleave with the matched ones - see
 * docs/matching.md for the full split. */

/* Backs the real palette (`PLTT`) up into `gPaletteBackup`,
 * then, for each factor 0/2/4/.../16, blends it toward black via the
 * already-matched `DarkenPalette` into `gPaletteFadeBuffer` and DMAs
 * that result into the real palette, waiting one VBlank between each
 * step - a textbook fade-to-black animation. Once fully faded, sets
 * up the hardware blend registers (`REG_BLDCNT`/`REG_BLDY`) and
 * restores the original backed-up palette.
 *
 * Was NAKED asm, not plain C - see
 * docs/matching/archive/naked-sub_80014a4-matched.md for the derivation of how
 * this was finally matched as real C. The gap: the ROM caches the
 * blended-buffer address (`gPaletteFadeBuffer`) in a register across
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
void FadePaletteToBlack(void)
{
    MATCH_HOLD_REG(struct dma_regs *, dma, r1);
    MATCH_HOLD_REG(struct dma_regs *, dma2, r4);
    MATCH_HOLD_REG(s32, factor, r5);
    u32 val;
    MATCH_HOLD_REG(u32 *, bufAddr, r6);
    MATCH_HOLD_REG(u32, dstVal, r3);
    MATCH_HOLD_REG(u32, cntVal, r2);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = PLTT;
    dma->dst = (u32)gPaletteBackup;
    dma->cnt = 0x80000200;
    val = dma->cnt;

    factor = 0;
    dma2 = dma;
    bufAddr = (u32 *)gPaletteFadeBuffer;

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
        DarkenPalette(factor);
        WaitForVBlank();
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
        MATCH_HOLD_REG(struct dma_regs *, dma3, r0);
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

/* `gBrightnessFade.field_0 != -1`: the "idle" sentinel (gfx.h). */

s32 IsBrightnessFadeActive(void)
{
    return gBrightnessFade.field_0 != -1;
}
