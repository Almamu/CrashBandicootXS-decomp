#include "vehicle.hpp"
#include "cell_anim.hpp"
#include "actor_palette_cycle.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "actor_anim.h"
#include "gba/io_reg.h"
#include <libgcc.h>
#include "actor.h"
#include "bosses.h"
#include "level.h"
#include "globals.h"
}

using Engine::Actor::ActorPaletteCycle;
using Engine::Gfx::CellAnim;

/* Kicks off a DMA copy of `tileBytes` bytes from the current
 * "console"/text-plane cursor cell into VRAM (one of three fixed
 * destination strategies depending on the `hasBanks`/
 * `page` mode bytes), then arms `uploaded` so a
 * caller can poll for completion. `gDrawMirroredTilemapFunc` is a function
 * pointer, called through the pointer with four arguments.
 *
 * The `0x204` header offset is added to the row product before the
 * base pointer, the destination is a ternary, and the callback's first
 * argument goes through its own local; each of those fixes one piece of
 * the ROM's instruction order. Matches under both compilers. */
void CellAnim::UploadFrame()
{
    u8 *src = (u8 *)data + (Q8_TO_INT(time) * frameSize + 0x204);
    u32 dst;

    if (hasBanks != 0) {
        u8 *next;

        dst = page != 0 ? VRAM : VRAM + 0x2000;
        next = src + tileBytes;
        gDrawMirroredTilemapFunc(next, page, cols, rows);
    } else if (page != 0) {
        dst = VRAM + 0x20;
    } else {
        dst = tileBytes + (VRAM + 0x20);
    }
    DmaCopy16(3, src, dst, tileBytes);
    uploaded = 1;
}

/* (Re)configures the console/text-plane cell geometry from a fresh
 * cell record at `cellAnim` (a `struct cell_anim_header`: its `cols`/`rows`) - cell pixel area, its DMA-scroll-wrap threshold, and the
 * initial X/Y scroll accumulators - then rebuilds both VRAM screen
 * blocks via `ResetBg`.
 *
 * Matched in the second near-miss sweep. The ROM stores
 * `frameSize` once, after the `if`, then reloads it for the
 * division through a *copy* of its address taken before the branch
 * (`ldr r4, =A4; ...; add r1, r4, #0`). The copy is a second pointer
 * local, `reload` (dividing through `*pFrameSize` itself loses it). Evaluation order
 * fixes the rest: the flag goes through a pointer to
 * `hasBanks` loaded first, `area` is assigned inside the
 * `tileBytes` store so that global's address loads before the
 * multiply, and `size` is read back from `tileBytes` between
 * taking the address and copying it. Matches under both compilers. */
void CellAnim::Init(s32 type, void *cellAnim, u32 animSize, s32 checkpoint)
{
    struct cell_anim_header *cell = (struct cell_anim_header *)cellAnim;
    s32 area;
    s32 flag;
    s32 size;
    u8 *pFlag = &hasBanks;

    flag = (type == CATEGORY_TYPE_POLAR);
    *pFlag = flag;
    data = cell;
    cols = cell->cols;
    rows = cell->rows;
    tileBytes = (area = cols * rows) << 5;

    s32 *pFrameSize = &frameSize;
    s32 *reload;

    size = tileBytes;
    reload = pFrameSize;
    if (flag)
        size += (area + 7) / 8 * 4;
    *pFrameSize = size;
    length = INT_TO_Q8(__divsi3(animSize - 0x204, *reload));
    time = 0;
    ResetBg();
    speed = 0;
    if (hasBanks == 0)
        distance = Q8_TO_INT(checkpoint - gActorFocalLength);
    else
        distance = Q8_TO_INT(checkpoint + gActorFocalLength);
    frameStep = 0;
}

/* `FillTilemap` (below), inlined here twice: fills screen
 * block 0x0600E400 (or 0x0600F400 for `page` 1, numbering on
 * from `w * h + 1`) with consecutive tile numbers for a `w` x `h` cell
 * grid; columns past 31 go to the next screen block (+0x7c0 bytes).
 * `tile++` sits in each branch: with one increment after the `if`,
 * `base` and `tile` swap registers. Unlike the standalone copy, both
 * branches assign `base` here; the default-initializer form is 5
 * halfwords off once inlined with a constant `page`. */
static inline void FillTileMap(s32 page, s32 w, s32 h)
{
    u16 *base;
    s32 tile;
    s32 row, col;

    if (page != 0) {
        base = (u16 *)(BG_SCREEN_ADDR(30) + 0x400);
        tile = h * w + 1;
    } else {
        base = (u16 *)(BG_SCREEN_ADDR(28) + 0x400);
        tile = 1;
    }
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            if (col <= BG_SCREEN_BLOCK_TILES - 1)
                base[col] = tile++;
            else
                base[col + 0x3e0] = tile++;
        }
        base += 0x20;
    }
}

/* Resets the console/text plane: display mode, BG0 control, palette
 * DMA from the cell record, and (unless `hasBanks` is set)
 * clears the first tile and the 0x0600E000 screen block and fills both
 * tile maps. The clear loop needs `vram` as a local and an upward `i`
 * (gcc reverses it into the ROM's pointer loop). Matches under both
 * compilers. */
void CellAnim::ResetBg()
{
    REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
    REG_BG0CNT = BGCNT_PRIORITY(2) | BGCNT_SCREENBASE(28) | BGCNT_TXT512x256;
    DmaCopy16(3, data, PLTT, BG_PLTT_SIZE);
    if (hasBanks == 0) {
        u32 *vram = (u32 *)VRAM;
        s32 i;

        for (i = 0; i < 8; i++)
            vram[i] = 0;
        DmaFill16(3, 0, BG_SCREEN_ADDR(28), 4 * BG_SCREEN_SIZE);
        FillTileMap(0, cols, rows);
        FillTileMap(1, cols, rows);
    }
    page = 1;
    UploadFrame();
}

/* Empty hook ActorCategory::Init calls at the end of each attempt (after
 * ActorCategory::DestroyActors), named after that call site. ActorCategoryEndStub
 * (actor_bg.cpp) is its counterpart at the end of the whole category. */
void ActorCategoryAttemptEndStub(void)
{
}

/* `cols`/`rows` are a tile width/height pair
 * (set by Init, still raw) for this same scroll-effect subsystem;
 * this computes their product doubled plus one - likely a tile-count-
 * to-byte-count-ish conversion for a buffer this subsystem allocates,
 * not traced further. */
s32 CellAnim::GetFreeTile()
{
    return cols * rows * 2 + 1;
}

/* Toggles the BG0 tile-set half used for the console/text plane
 * (`page`), committing the choice to `REG_BG0CNT`, once
 * `uploaded` signals the previous DMA is done. Written as two
 * full, duplicated `REG_BG0CNT = ...` branches rather than a ternary -
 * this compiler emits a separate `REG_BG0CNT` literal-pool reference per
 * branch for the duplicated-store form (matching the ROM's own two
 * `.4byte 0x04000008` pool entries), while a ternary/single-store
 * collapses it to one shared pool entry and a shorter, differently
 * shaped sequence (see docs/workflow.md step 3/7). The trailing toggle
 * goes through a `toggled` local set to `1` first - this compiler
 * otherwise canonicalizes `1 ^ page`/`page ^= 1` the
 * same way regardless of source operand order, loading the memory
 * operand first, where the ROM loads the constant `1` first (see
 * docs/workflow.md step 3). */
void CellAnim::FlipPage()
{
    if (uploaded != 0) {
        u8 toggled;

        if (page != 0) {
            REG_BG0CNT = BGCNT_PRIORITY(2) | BGCNT_SCREENBASE(28) | BGCNT_TXT512x256;
        } else {
            REG_BG0CNT = BGCNT_PRIORITY(2) | BGCNT_SCREENBASE(30) | BGCNT_TXT512x256;
        }
        uploaded = 0;
        toggled = 1;
        toggled ^= page;
        page = toggled;
    }
}

/* Trivial getter for this scroll-effect subsystem's accumulated X
 * offset, set by Advance (still raw). */

s32 CellAnim::GetDistance()
{
    return distance;
}

/* Advances the console/text-plane's horizontal scroll accumulator by
 * `speed` (a Q8.8 per-frame velocity), wrapping it against
 * `length`, and - whenever the whole-tile column actually
 * changed - shifts the visible-column counter and re-triggers the
 * pending-cell DMA/palette-cursor pair. The ROM keeps the updated
 * position `pos` in a separate register from the old value `prev`.
 * `bcPtr` is materialized *before* the shift/subtract that becomes
 * `delta` - the ROM loads the store destination's address ahead of
 * computing the value, not right before the store (see
 * docs/workflow.md step 3 for this family of fixes). */
void CellAnim::Advance()
{
    s32 *b0ptr = &time;
    s32 prev = *b0ptr;
    /* `prevShifted` is needed: inlined, the instructions differ */
    s32 prevShifted = Q8_TO_INT(prev);
    s32 pos = prev + speed;
    s32 *bcPtr;
    s32 delta;

    *b0ptr = pos;
    bcPtr = &frameStep;
    delta = Q8_TO_INT(pos) - prevShifted;
    *bcPtr = delta;

    if (pos >= length) {
        *b0ptr = pos - length;
    }

    if (delta != 0) {
        distance += delta;
        UploadFrame();
        ActorPaletteCycle::Update();
    }
}

/* Trivial getter for this scroll-effect subsystem's per-tick delta,
 * set by Advance (still raw). */
s32 CellAnim::GetFrameStep()
{
    return frameStep;
}

/* `(v*15) << 2 >> 8` on speed (a Q8.8-ish speed/step value
 * set by SetSpeed, still raw) - written as the ROM's own
 * shift-subtract-shift idiom (`(v<<4) - v`, i.e. `v*15`) rather than a
 * plain `* 15` to match its exact instruction sequence. */
s32 CellAnim::GetSpeed()
{
    s32 v = speed;
    return Q8_TO_INT(((v << 4) - v) << 2);
}

/* `dest` is materialized before the `__divsi3` call, not after (a
 * plain `speed = ...` loads it after) - the ROM loads the store
 * destination's address ahead of the call so it survives across it in
 * a register `bl` doesn't clobber, rather than recomputing it from the
 * return value's position afterward (see docs/workflow.md step 3). */
void CellAnim::SetSpeed(s32 value)
{
    s32 *dest = &speed;

    *dest = __divsi3(INT_TO_Q8(value), 0x3c);
}

/* Fills screen block 28 from row 16 on (block 30 for `screen` 1,
 * numbering on from `w * h + 1`) with consecutive tile numbers for a
 * `w` x `h` cell grid; columns past 31 go to the next screen block
 * (+0x7c0 bytes). `ResetBg` above inlines the same body
 * twice.
 *
 * The old NAKED note blamed an unreachable register permutation. The
 * fix is `tile++` inside each branch of the column test: with one
 * increment after the `if`, `tile` has fewer references than `base`
 * and the two swap registers (the `r8`/`ip` roles of the 0x7c0 offset
 * and `h` follow from that). Matches under both compilers. */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void CellAnim::FillTilemap(s32 screen, s32 w, s32 h)
{
    u16 *base = (u16 *)(BG_SCREEN_ADDR(28) + 0x400);
    s32 tile;
    s32 row, col;

    if (screen != 0) {
        base = (u16 *)(BG_SCREEN_ADDR(30) + 0x400);
        tile = h * w + 1;
    } else {
        tile = 1;
    }
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            if (col <= BG_SCREEN_BLOCK_TILES - 1)
                base[col] = tile++;
            else
                base[col + 0x3e0] = tile++;
        }
        base += 0x20;
    }
}
