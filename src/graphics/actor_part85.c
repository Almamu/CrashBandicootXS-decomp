#include "core.h"
#include "gba/dma_macros.h"

/* Same "self" object family as actor_part61.c/actor_part66.c/actor_part72.c/
 * actor_part73.c - see docs/matching/issue-63-0x08033ef4-actor.md. This is
 * the 0x14-byte constructor (`sub_8034374`, called by `LoadLevelGraphics` as
 * `sub_8034374(sub_8026EDC(0x14))`, see `src/graphics/level_graphics.c`) and
 * its companion per-frame updater (`sub_8034480`, called by
 * `sub_8034688`/actor_part73.c) for a BG0 "raw bitmap" particle-trail
 * effect: the whole 240x160 screen is set up as one contiguous run of 8x8
 * tiles on BG0 (tile index == screen position, palette bank 15), and a
 * shadow 4-bit-per-pixel buffer (`tileBuffer`, exactly 240*160/2 = 0x4B00
 * bytes) is drawn into with plain nibble writes each frame, then DMA'd
 * wholesale into the real tile graphics VRAM - the classic "abuse the BG
 * tile grid as a raw indexed bitmap" GBA trick. */
struct particle_bg {
    /* Always 0x06000000 - the BG tile *graphics* VRAM this object's whole
     * `tileBuffer` gets DMA'd into every frame. */
    u32 tileVramBase;
    /* Always 0x0600F800 - BG0's screen/tilemap base (screen base block 31,
     * see `sub_8034374`'s `REG_BG0CNT` setup below), laid out once at
     * construction time as one sequential tile index per 8x8 cell. */
    u32 mapVramBase;
    /* 128-slot particle array (`sub_8026EC0(0x800)`, 16-byte stride - see
     * `struct particle_slot`, actor_part72.c). */
    void *particles;
    /* Active particle count (0-0x80). */
    s32 count;
    /* 240x160, 4-bit-per-pixel shadow tile-graphics buffer
     * (`sub_8026EC0(0x4B00)`) - `sub_8034480` draws each active particle's
     * trail into this every frame, then DMAs it wholesale into
     * `tileVramBase`. */
    void *tileBuffer;
};

struct particle_slot {
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
};

extern void *sub_8026EC0(u32 size);
extern void sub_8001524(s32 val);
extern void sub_80015D0(void);
extern void sub_8001614(void);
extern void sub_80345B0(void *mgrArg, s32 idx);
extern u8 gUnknown_03001288[2];

/* Constructs the particle-trail BG0 object. Fully matched as real C.
 *
 * The ROM builds the 4-bit-palette-bank tile-index mask (0xFFFFF000)
 * by loading the 32-bit literal into `r1` first and then copying it
 * into `r5` (`ldr r1,=0xFFFFF000; adds r5,r1,#0`), rather than
 * materializing it directly into `r5` in one `ldr` the way a plain
 * `mask = -0x1000;` compiles - closed by pinning an intermediate local
 * to `r1`, letting the compiler's own literal-pool codegen place the
 * constant (an inline-asm immediate instead produces a *second*,
 * separately-pooled literal, appended after the compiler's own pool
 * rather than interleaved into it in ROM's actual order, so the
 * intermediate must stay a plain C initializer, not an asm-embedded
 * one) and then forcing the r1->r5 copy via `asm volatile("add %0,
 * %1, #0" ...)`. Also needed the `mapBase + (row << 6)` addition's
 * operand order pinned (`add r1, r0, r7`, not gcc's default `r7, r0`)
 * via the same technique, and the `col = 0x1d` initializer moved after
 * that computation in the C source (a trivial immediate move that
 * gcc otherwise schedules ahead of the pinned-register asm block,
 * unlike the ROM's own ordering, since the source's original textual
 * placement determines scheduling once a hard asm barrier is
 * introduced nearby). See docs/matching/issue-63-0x08033ef4-actor.md. */
void *sub_8034374(void *selfArg)
{
    struct particle_bg *self = selfArg;
    struct dma_regs *dma;
    /* Deliberately left uninitialized - the ROM builds REG_BG0CNT's value
     * with an `ands r5, =0xFFFF0000` against whatever was already in the
     * register, then fills in every bit the halfword write actually reads
     * via the ORs below (negative-constant bit-clear idiom, see
     * LoadBg2Background's `bg2cnt`, src/graphics/level_graphics.c). */
    u32 bg0cnt;
    s32 gradIdx;
    s32 gradCount;
    s32 row;
    s32 tileIdx;
    u32 tileBase;
    u32 mapBase;
    u32 mask;
    u16 *gradDst;
    u16 dmaFillSrc16;
    u32 dmaFillSrc32;
    u32 *dma2Src;
    u32 zero;

    self->particles = sub_8026EC0(0x800);

    {
        u16 *dispcntShadow = (u16 *)gUnknown_03001288;
        zero = 0;
        *dispcntShadow = 0x40;
    }
    sub_8001524(0);

    /* Clears BG0HOFS/BG0VOFS together via one word store. */
    *(vu32 *)REG_ADDR_BG0HOFS = zero;

    bg0cnt &= -0x10000;
    bg0cnt |= 3;                /* priority 3 */
    bg0cnt |= 0xf8 << 5;        /* screen base block 31 (0x0600F800) */
    REG_BG0CNT = bg0cnt;

    self->tileVramBase = 0x06000000;
    self->mapVramBase = 0x0600F800;

    sub_80015D0();

    /* Clears BG palette entry 0 (the backdrop color). */
    *(vu16 *)0x05000000 = zero;

    /* A 3-step white-to-black grayscale gradient into BG palette bank 15's
     * last 3 entries (0x050001E0 = palette index 240), used by the
     * tilemap-fill loop below's palette-bank-15 tile entries. */
    gradDst = (u16 *)0x050001E0;
    dma2Src = &dmaFillSrc32;
    gradIdx = 0;
    gradCount = 2;
    do {
        s32 half = gradIdx / 2;
        u16 color = half | (half << 5) | (half << 10);
        *gradDst = color;
        gradDst++;
        gradIdx += 0x1f;
        gradCount--;
    } while (gradCount >= 0);

    /* Lays out BG0's whole 240x160 (30x20 tiles) screen as one sequential
     * run of tile indices (palette bank 15) - tile index N at screen
     * position N, so `tileBuffer`'s raw nibble data below lines up 1:1
     * with BG tile graphics VRAM once DMA'd there. */
    tileIdx = 0;
    row = 0;
    tileBase = self->tileVramBase;
    mapBase = self->mapVramBase;
    {
        register u32 maskTmp asm("r1") = -0x1000;
        register u32 maskReg asm("r5");
        asm volatile("add %0, %1, #0" : "=r"(maskReg) : "r"(maskTmp));
        mask = maskReg;
    }
    do {
        s32 nextRow = row + 1;
        register u32 shifted asm("r0") = row << 6;
        register u32 rowPtrVal asm("r1");
        u16 *rowPtr;
        s32 col;

        asm volatile("add %0, %1, %2" : "=r"(rowPtrVal) : "r"(shifted), "r"(mapBase));
        rowPtr = (u16 *)rowPtrVal;
        col = 0x1d;

        do {
            *rowPtr = tileIdx | mask;
            tileIdx++;
            rowPtr++;
            col--;
        } while (col >= 0);
        row = nextRow;
    } while (row <= 0x13);

    /* Zero-fills the 19200-byte tile *graphics* VRAM region this object
     * owns (fixed-source 16-bit fill from one stack halfword). */
    dmaFillSrc16 = 0;
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)&dmaFillSrc16;
    dma->dst = tileBase;
    dma->cnt = 0x81002580;
    dma->cnt;

    REG_BLDCNT = 0;
    sub_8001614();

    self->count = 0;

    self->tileBuffer = sub_8026EC0(0x4B00);

    /* Zero-fills the freshly-allocated 19200-byte `tileBuffer` (fixed-
     * source 32-bit fill from one stack word). */
    dmaFillSrc32 = 0;
    dma->src = (u32)dma2Src;
    dma->dst = (u32)self->tileBuffer;
    dma->cnt = 0x850012C0;
    dma->cnt;

    return self;
}

/* Fully matched as real C. Every frame: commits last frame's `tileBuffer`
 * to the real tile VRAM (DMA3, 32-bit), clears `tileBuffer` back to zero
 * (DMA3 fill), then for each active particle draws a 2-value trail (nibble
 * `1` at the pre-movement position, nibble `2` at the post-movement
 * position - the same `(x>>3)<<6 + ((y>>3)*15)<<7 + (x&7) + (y&7)<<3`
 * nibble-address formula as the matched general-purpose `sub_8034634`,
 * actor_part72.c, just inlined twice instead of called), applies the
 * particle's `dx`/`dy` in between, and respawns it via `sub_80345B0` if it
 * drifted outside the `[0, 0xEFFF]`x`[0, 0x9FFF]` (24.8 fixed-point,
 * 240x160 pixel) box.
 *
 * Closing the residual register-allocation gap here turned out to be a
 * mix of `sub_8034634`'s own three fixes plus two more ordering fixes
 * specific to this larger, twice-inlined function:
 *   1. Both `x` bounds checks (`x <= 0xef` and, for the post-move copy,
 *      `newX > 0xEFFF`) want an unsigned comparison (the ROM's `bhi`),
 *      modeled via `(u32)xPix <= 0xef` / `(u32)newX > 0xEFFF` casts,
 *      while the `x >> 11` (`blockX`) shift stays a plain signed
 *      arithmetic shift on the already-`s32` raw value.
 *   2. `addr`'s two halves need computing as separate statements
 *      (`addr = blockX << 6; addr += ...;`), not folded into one `a + b`
 *      expression, same as `sub_8034634`.
 *   3. The final opaque `asm volatile` reproduces the ROM's own tail
 *      sequence for `cell &= ~mask; cell |= val << shift; *entry = cell;`
 *      - simpler than `sub_8034634`'s own tail since this function's ROM
 *      build never needs the `r4`-materialize-then-copy-back step, just
 *      `mask` (`r0`) computed, `cell` loaded straight into `r2` and
 *      `bic`'d in place, then `r0` reused to shift `val` in before the
 *      final `orr`/`strh`.
 *   4. Two more scheduling-only orderings this compiler doesn't infer on
 *      its own from a natural declaration block: `x`'s raw value and its
 *      `>>8` pixel value must be computed immediately, before loading
 *      `y` (`xRaw = slot->x; xPix = xRaw >> 8; yRaw = slot->y; ...`), and
 *      likewise `blockX`'s `<<6` term must be fully computed before
 *      `blockY` is even loaded. Similarly, the post-move `slot->x = newX`
 *      store happens immediately after computing `newX`, before `newY`
 *      is even loaded - not batched together at the end the way the two
 *      stores read in the source's natural top-to-bottom order.
 *   5. The second inlined copy's `oldVal` (the second nibble value,
 *      always 2, pinned to `ip`) must be assigned via a plain statement
 *      *after* `xPix2`/`yPix2` are computed, not as its `register`
 *      declaration's own initializer - an initializer schedules the
 *      `movs #2`/`mov ip` pair too early (before the position reload),
 *      unlike the ROM's own ordering. */
void sub_8034480(void *selfArg)
{
    struct particle_bg *self = selfArg;
    struct dma_regs *dma;
    struct particle_slot *slot;
    s32 i;
    s32 count;
    u32 fillZero;

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)self->tileBuffer;
    dma->dst = self->tileVramBase;
    dma->cnt = 0x840012C0;
    dma->cnt;

    fillZero = 0;
    dma->src = (u32)&fillZero;
    dma->dst = (u32)self->tileBuffer;
    dma->cnt = 0x850012C0;
    dma->cnt;

    slot = self->particles;
    i = 0;
    count = self->count;
    if (i < count) {
        /* Register-pinned to match the ROM's own choices - `trailVal`
         * (the first nibble value, always 1) lives in `sb` for the whole
         * loop, `sevenMask` (the `&7` pixel-within-tile mask) in `r8`. */
        register s32 trailVal asm("sb") = 1;
        register s32 sevenMask asm("r8") = 7;

        do {
            s32 xRaw, yRaw, xPix, yPix;

            xRaw = slot->x;
            xPix = xRaw >> 8;
            yRaw = slot->y;
            yPix = yRaw >> 8;

            if ((u32)xPix <= 0xef && yPix >= 0 && yPix <= 0x9f) {
                s32 blockX, blockY;
                s32 addr;
                u16 *entry;
                s32 shift;

                blockX = xRaw >> 11;
                addr = blockX << 6;
                blockY = yRaw >> 11;
                addr += ((blockY << 4) - blockY) << 7;
                addr += xPix & sevenMask;
                addr += (yPix & sevenMask) << 3;
                entry = (u16 *)((u8 *)self->tileBuffer + ((addr >> 2) << 1));
                shift = (addr & 3) << 2;
                /* Opaque tail reproducing the ROM's own
                 * `mask`(r0)/`cell`(r2) `bic`/`orr`/`strh` sequence - see
                 * point 3 above. */
                asm volatile(
                    "mov r0, #0xf\n"
                    "lsl r0, %0\n"
                    "ldrh r2, [%1, #0]\n"
                    "bic r2, r0\n"
                    "mov r0, %2\n"
                    "lsl r0, %0\n"
                    "orr r2, r0\n"
                    "strh r2, [%1, #0]\n"
                    :
                    : "r"(shift), "r"(entry), "r"(trailVal)
                    : "r0", "r2", "memory"
                );
            }

            {
                s32 newX, newY;

                newX = slot->x + slot->dx;
                slot->x = newX;
                newY = slot->y + slot->dy;
                slot->y = newY;

                if ((u32)newX > 0xEFFF || newY < 0 || newY > 0x9FFF) {
                    sub_80345B0(self, i);
                }
            }

            {
                s32 xRaw2, yRaw2, xPix2, yPix2;
                /* The second nibble value (always 2) is recomputed fresh
                 * into `ip` every iteration, matching the ROM's own
                 * build - unlike `trailVal`/`sevenMask` above, it isn't
                 * hoisted above the loop, and it's assigned by a plain
                 * statement after the position reload (see point 5
                 * above), not as this declaration's own initializer. */
                register s32 oldVal asm("ip");

                xRaw2 = slot->x;
                xPix2 = xRaw2 >> 8;
                yRaw2 = slot->y;
                yPix2 = yRaw2 >> 8;
                oldVal = 2;

                if ((u32)xPix2 <= 0xef && yPix2 >= 0 && yPix2 <= 0x9f) {
                    s32 blockX, blockY;
                    s32 addr;
                    u16 *entry;
                    s32 shift;

                    blockX = xRaw2 >> 11;
                    addr = blockX << 6;
                    blockY = yRaw2 >> 11;
                    addr += ((blockY << 4) - blockY) << 7;
                    addr += xPix2 & sevenMask;
                    addr += (yPix2 & sevenMask) << 3;
                    entry = (u16 *)((u8 *)self->tileBuffer + ((addr >> 2) << 1));
                    shift = (addr & 3) << 2;
                    asm volatile(
                        "mov r0, #0xf\n"
                        "lsl r0, %0\n"
                        "ldrh r2, [%1, #0]\n"
                        "bic r2, r0\n"
                        "mov r0, %2\n"
                        "lsl r0, %0\n"
                        "orr r2, r0\n"
                        "strh r2, [%1, #0]\n"
                        :
                        : "r"(shift), "r"(entry), "r"(oldVal)
                        : "r0", "r2", "memory"
                    );
                }
            }

            slot++;
            i++;
            count = self->count;
        } while (i < count);
    }
}

asm(".align 2, 0");
