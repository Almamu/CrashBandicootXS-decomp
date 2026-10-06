#include "core.h"
#include "gba/dma_macros.h"
#include "frontend.h"
#include "util.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"

/* Same "self" object family as hovercraft_side_gun.c - see docs/matching/issue-63-0x08033ef4-actor.md. This is
 * the 0x14-byte constructor (`InitStarfield`, called by `InitTitleScreen` as
 * `InitStarfield(OperatorNew(0x14))`, see `src/frontend/title_screen_init.c`) and
 * its companion per-frame updater (`DrawStarfield`, called by
 * `UpdateStarfield`, below) for a BG0 "raw bitmap" particle-trail
 * effect: the whole 240x160 screen is set up as one contiguous run of 8x8
 * tiles on BG0 (tile index == screen position, palette bank 15), and a
 * shadow 4-bit-per-pixel buffer (`tileBuffer`, exactly 240*160/2 = 0x4B00
 * bytes) is drawn into with plain nibble writes each frame, then DMA'd
 * wholesale into the real tile graphics VRAM - the classic "abuse the BG
 * tile grid as a raw indexed bitmap" GBA trick.
 *
 * The effect is a starfield: SpawnStar (starfield.c) starts each of
 * up to 128 stars at the screen centre with a random direction and
 * speed, and DrawStarfield moves them outwards, plotting a short trail.
 * The language menu, the credits and the level-loading screens run it
 * behind their text. */
struct particle_bg {
    /* Always `VRAM` - the BG tile *graphics* VRAM this object's whole
     * `tileBuffer` gets DMA'd into every frame. */
    u32 tileVramBase;
    /* Always `BG_SCREEN_ADDR(31)` - BG0's screen/tilemap base (see
     * `InitStarfield`'s `REG_BG0CNT` setup below), laid out once at
     * construction time as one sequential tile index per 8x8 cell. */
    u32 mapVramBase;
    /* 128-slot particle array (`OperatorNewArray(0x800)`, 16-byte stride - see
     * `struct particle_slot`, starfield.c). */
    void *particles;
    /* Active particle count (0-0x80). */
    s32 count;
    /* 240x160, 4-bit-per-pixel shadow tile-graphics buffer
     * (`OperatorNewArray(0x4B00)`) - `DrawStarfield` draws each active particle's
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
void *InitStarfield(void *selfArg)
{
    struct particle_bg *self = selfArg;
    struct dma_regs *dma;
    /* Deliberately left uninitialized - the ROM builds REG_BG0CNT's value
     * with an `ands r5, =0xFFFF0000` against whatever was already in the
     * register, then fills in every bit the halfword write actually reads
     * via the ORs below (negative-constant bit-clear idiom, see
     * LoadTitleScreenBg's `bg2cnt`, src/frontend/title_screen_init.c). */
    u32 bg0cnt = bg0cnt; /* self-init: deliberately unset (see above); silences -Wuninitialized (#577) */
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

    self->particles = OperatorNewArray(0x800);

    {
        u16 *dispcntShadow = (u16 *)gDispcnt;
        zero = 0;
        *dispcntShadow = 0x40;
    }
    SetDispcntMode(0);

    /* Clears BG0HOFS/BG0VOFS together via one word store. */
    *(vu32 *)REG_ADDR_BG0HOFS = zero;

    bg0cnt &= -0x10000;
    bg0cnt |= 3;                /* priority 3 */
    bg0cnt |= 0xf8 << 5;        /* screen base block 31 */
    REG_BG0CNT = bg0cnt;

    self->tileVramBase = VRAM;
    self->mapVramBase = BG_SCREEN_ADDR(31);

    ShowBg0();

    /* Clears BG palette entry 0 (the backdrop color). */
    *(vu16 *)BG_PLTT = zero;

    /* A 3-step white-to-black grayscale gradient into BG palette bank 15's
     * last 3 entries (from palette index 240), used by the
     * tilemap-fill loop below's palette-bank-15 tile entries. */
    gradDst = &((u16 *)BG_PLTT)[240];
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
    CommitDispcnt();

    self->count = 0;

    self->tileBuffer = OperatorNewArray(0x4B00);

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
 * nibble-address formula as the matched general-purpose `PlotStarfieldPixel`,
 * starfield.c, just inlined twice instead of called), applies the
 * particle's `dx`/`dy` in between, and respawns it via `SpawnStar` if it
 * drifted outside the `[0, 0xEFFF]`x`[0, 0x9FFF]` (24.8 fixed-point,
 * 240x160 pixel) box.
 *
 * Closing the residual register-allocation gap here turned out to be a
 * mix of `PlotStarfieldPixel`'s own three fixes plus two more ordering fixes
 * specific to this larger, twice-inlined function:
 *   1. Both `x` bounds checks (`x <= 0xef` and, for the post-move copy,
 *      `newX > 0xEFFF`) want an unsigned comparison (the ROM's `bhi`),
 *      modeled via `(u32)xPix <= 0xef` / `(u32)newX > 0xEFFF` casts,
 *      while the `x >> 11` (`blockX`) shift stays a plain signed
 *      arithmetic shift on the already-`s32` raw value.
 *   2. `addr`'s two halves need computing as separate statements
 *      (`addr = blockX << 6; addr += ...;`), not folded into one `a + b`
 *      expression, same as `PlotStarfieldPixel`.
 *   3. The final opaque `asm volatile` reproduces the ROM's own tail
 *      sequence for `cell &= ~mask; cell |= val << shift; *entry = cell;`
 *      - simpler than `PlotStarfieldPixel`'s own tail since this function's ROM
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
void DrawStarfield(void *selfArg)
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
                    SpawnStar(self, i);
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

/* Same "self" object family as hovercraft_side_gun.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. These two
 * functions drive a small 128-slot particle-like effect array at
 * `self+8` (16-byte stride: `{s32 x; s32 y; s32 dx; s32 dy;}`) and a
 * 4-bit-per-cell tilemap at `self+0x10`. */

/* Seeds particle slot `idx` at a fixed starting position, then rolls two
 * random values (`RandRange`) to pick a direction out of the 256-entry
 * `gSineTable` sin-ish table and a speed, and applies the
 * resulting `dx`/`dy` to the slot's position (including the `idx > 0x7f`
 * infinite-loop trap the ROM itself has). Closed the previous
 * register-allocation gap - this compiler chose the opposite multiply
 * operand order (`speed * table[...]`, copying `speed` into the
 * destination register before `muls`) from the ROM's own build (`table[...]
 * * speed`, copying the table lookup instead) - by simply writing the C
 * multiplication with the table lookup as the left operand; both are the
 * same value mathematically, but this compiler's instruction selection for
 * `dest = a * b` always materializes/copies the *left* operand into the
 * destination register before the `muls`, so which source expression is
 * written first decides which value gets that copy - matching the ROM's
 * choice here needed no register pins or opaque asm at all. */
void SpawnStar(void *mgrArg, s32 idx)
{
    u8 *mgr = mgrArg;
    struct particle_slot *slot;
    s32 rng1;
    s32 speed;

    if (idx > 0x7f) {
        for (;;) {
        }
    }

    slot = (struct particle_slot *)(*(u8 **)(mgr + 8) + (idx << 4));
    slot->x = 0x7800;
    slot->y = 0x5000;

    rng1 = (u16)RandRange(0x100);
    speed = (u16)RandRange(0x200) + 0x100;

    slot->dx = (gSineTable[(rng1 + 0x40) & 0xff] * speed) >> 8;
    slot->dy = (gSineTable[rng1 & 0xff] * speed) >> 8;

    slot->x += slot->dx * 5;
    slot->y += slot->dy * 5;
}

/* Writes a 4-bit nibble `val` into the `self+0x10` tilemap at pixel
 * `(x, y)`, once bounds-checked against the `0xf0x0xa0` screen (silently
 * doing nothing out of range). This compiler originally spilled all four
 * parameters into callee-saved registers at entry even though `val` is
 * never touched until the function's tail with no intervening call (a leaf
 * function, so the ROM's own build simply leaves it in `r3` the whole
 * time) - pinning `val` to `register s32 val asm("r3")` (assigned from a
 * plain, unpinned `valArg` parameter; a pinned parameter itself doesn't
 * parse on this compiler) fixed that, dropping the `push`/`pop` back down
 * to the ROM's `{r4, r5, r6}`.
 *
 * Closing the residual `addr`/`blockY` register-role gap turned out to be
 * three separate, independently-discovered issues rather than one:
 *   1. `x`'s bounds check (`x <= 0xef`) wants an unsigned comparison (the
 *      ROM's `bhi`), but `x >> 3` wants a *signed* arithmetic shift
 *      (`asrs`) - i.e. the ROM's own source treated `x` as signed for the
 *      shift while still using an unsigned-style bounds check. Modeled
 *      here by computing `addr`'s x-derived half via `((s32)x >> 3) << 6`
 *      rather than a plain unsigned `x >> 3`.
 *   2. `addr`'s two halves (the x-derived `<< 6` term and the
 *      blockY-derived `<< 7` term) need to be computed as two separate
 *      statements (`addr = ...; addr += ...;`), not folded into one `a + b`
 *      additive C expression - gcc doesn't evaluate `+`'s operands
 *      left-to-right, so the combined-expression form let it pick blockY's
 *      half first (opposite the ROM's x-half-first order); splitting into
 *      statements pins the evaluation order to match.
 *   3. `mask` (`0xf << shift`) needs to be a plain 32-bit type (`u32`), not
 *      `u16` - declaring it `u16` makes this compiler insert a defensive
 *      32-bit-to-16-bit truncation sequence (`0xf0 << 12`, shift, `>> 16`)
 *      around the shift, 4 extra instructions the ROM's own build never
 *      has (its `mask` never actually needs truncating: the low 16 bits
 *      are all `bics`/`orrs` ever reads). Once `mask` reverted to a plain
 *      32-bit-computing type, the two-instruction `movs`/`lsls` from the
 *      ROM's own build fell out on its own.
 * The final residual gap - the ROM's own "materialize `cell` into `r4`,
 * `bics` it against `mask` in `r0`, then copy `r4` back into `r0` before
 * `orrs`/`strh`" idiom, the same class of redundant-copy-after-a-binary-op
 * gcc-2.9 quirk already seen for `AllocJetpackPlayerTiles`'s multiply - is closed with
 * one opaque `asm volatile` block emitting that exact instruction sequence
 * verbatim, taking `shift` and `tileMapEntry` (itself pinned to `r2` via a
 * nested `register` local, matching the ROM's own choice) as inputs and
 * `val` (already pinned to `r3`) as an in/out operand. */
void PlotStarfieldPixel(void *mgrArg, u32 x, s32 y, s32 valArg)
{
    u8 *mgr = mgrArg;
    register s32 val asm("r3") = valArg;

    if (x <= 0xef && y >= 0 && y <= 0x9f) {
        s32 addr = ((s32)x >> 3) << 6;
        s32 blockY = y >> 3;
        s32 shift;

        addr += ((blockY << 4) - blockY) << 7;
        addr += (x & 7);
        addr += (y & 7) << 3;
        {
            register s32 off asm("r0") = (addr >> 2) << 1;
            register u16 *tileMapEntry asm("r2") = (u16 *)(*(u8 **)(mgr + 0x10) + off);

            shift = (addr & 3) << 2;
            asm volatile(
                "mov r0, #0xf\n"
                "lsl r0, %1\n"
                "ldrh r4, [%2, #0]\n"
                "bic r4, r0\n"
                "add r0, r4, #0\n"
                "lsl %0, %1\n"
                "orr r0, %0\n"
                "strh r0, [%2, #0]\n"
                : "+r"(val)
                : "r"(shift), "r"(tileMapEntry)
                : "r0", "r4", "memory"
            );
        }
    }
}

asm(".align 2, 0");

/* Same "self" object family as hovercraft_side_gun.c - see
 * docs/matching/issue-63-0x08033ef4-actor.md. */

/* Calls `DrawStarfield(mgr)` (the OAM/tile-scan update this object's part
 * table drives), then - while the particle `count` is still under
 * 0x80 - spawns up to 8 more particles via `SpawnStar`, incrementing
 * `count` for each one spawned. */
void UpdateStarfield(void *mgrArg)
{
    struct particle_bg *mgr = mgrArg;

    DrawStarfield(mgr);

    if (mgr->count <= 0x7f) {
        s32 count = 0x80 - mgr->count;

        if (count > 8) {
            count = 8;
        }
        count -= 1;

        if (count != -1) {
            s32 end = -1;

            do {
                register s32 idxR0 asm("r0") = mgr->count;
                register s32 idx asm("r1") = idxR0;

                mgr->count = idxR0 + 1;
                SpawnStar(mgr, idx);
                count -= 1;
            } while (count != end);
        }
    }
}

/* Busy-waits (yielding a frame via `WaitForVBlank`/`UpdateStarfield` each
 * time) until the input-poll result from `UpdateKeys(gInput)`
 * has either of bits 0/3 set in `gKeys`'s `+2` halfword. */
void StarfieldWaitForButton(void *mgrArg)
{
    u8 *mgr = mgrArg;
    s32 result;

    goto check;
body:
    WaitForVBlank();
    UpdateStarfield(mgr);
check:
    UpdateKeys(gInput);
    {
        register u8 *addr asm("r1") = (u8 *)&gKeys;
        register s32 nine asm("r0") = 9;
        register s32 flag asm("r1");
        register s32 r asm("r0");

        flag = *(u16 *)(addr + 2);
        r = nine & flag;
        result = r;
    }
    if (result == 0) {
        goto body;
    }
}

/* Releases `self+0x10`/`self+8`'s dynamically-allocated buffers (each,
 * if non-NULL, via `OperatorDeleteArray`) and, if bit 0 of `flags` is set, also
 * releases `self` itself via `OperatorDelete`. */
void DestroyStarfield(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    if (*(void **)(self + 0x10) != NULL) {
        OperatorDeleteArray(*(void **)(self + 0x10));
    }
    if (*(void **)(self + 8) != NULL) {
        OperatorDeleteArray(*(void **)(self + 8));
    }
    if ((flags & 1) != 0) {
        OperatorDelete(self);
    }
}

asm(".align 2, 0");
