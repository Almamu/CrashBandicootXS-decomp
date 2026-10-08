extern "C" {
#include "core.h"
#include "math_util.h"
#include "actor_anim.h"
#include "gba/io_reg.h"
#include <libgcc.h>
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "level.h"
#include "globals.h"
}

/* codegen: SetCheckpointAtPlayer takes (state, flag) (level.h); this
 * caller passes the state only and leaves r1 as it is. docs/headers_plan.md */
extern void SetCheckpointAtPlayer_1(void *self) asm("SetCheckpointAtPlayer");

/* Re-bases the category's secondary tick counter from `arg0` (net of
 * `GetActorSpawnOffset`'s current Q8.8 offset), resets the active-instance
 * counters, and re-syncs the frame-tick snapshot for a freshly
 * (re)selected category. */
void SetActorCheckpoint(s32 arg0)
{
    gActorCheckpoint = arg0 - GetActorSpawnOffset();
    gActorCategoryDeaths = 0;
    gActorCategoryBossDeaths = 0;
    SetCheckpointAtPlayer_1(gLevelState);
    gActorCheckpointMissedNitros = gActorMissedNitros;
    SaveActorPaletteCycle();
}

/* True once the running active-instance count reaches the current
 * category's `maskAssistDeaths` threshold. The cast to `s32` matches the
 * ROM's own signed comparison (`blt`) - `maskAssistDeaths` is declared `u32`
 * in actor_anim.h (its sign isn't otherwise pinned down), and the
 * unsigned usual-arithmetic-conversion comparison that produces
 * compiles to the unsigned `bcc` instead (see docs/workflow.md
 * step 3). */
s32 IsActorMaskAssistDue(void)
{
    return gActorCategoryDeaths >= (s32)gActorCategories[gActorCategory].maskAssistDeaths;
}

/* Kicks off a DMA copy of `gCellAnimTileBytes` bytes from the current
 * "console"/text-plane cursor cell into VRAM (one of three fixed
 * destination strategies depending on the `gCellAnimHasBanks`/
 * `gCellAnimPage` mode bytes), then arms `gCellAnimUploaded` so a
 * caller can poll for completion. `gDrawMirroredTilemapFunc` is a function
 * pointer (called through `_call_via_r4`).
 *
 * The `0x204` header offset is added to the row product before the
 * base pointer, the destination is a ternary, and the callback's first
 * argument goes through its own local; each of those fixes one piece of
 * the ROM's instruction order. Matches under both compilers. */
void UploadCellAnimFrame(void)
{
    u8 *src = (u8 *)gCellAnim + (Q8_TO_INT(gCellAnimTime) * gCellAnimFrameSize + 0x204);
    u32 dst;

    if (gCellAnimHasBanks != 0) {
        u8 *next;

        dst = gCellAnimPage != 0 ? VRAM : VRAM + 0x2000;
        next = src + gCellAnimTileBytes;
        gDrawMirroredTilemapFunc(next, gCellAnimPage, gCellAnimCols, gCellAnimRows);
    } else if (gCellAnimPage != 0) {
        dst = VRAM + 0x20;
    } else {
        dst = gCellAnimTileBytes + (VRAM + 0x20);
    }
    DmaSet(3, src, dst, 0x80000000 | (gCellAnimTileBytes / 2));
    gCellAnimUploaded = 1;
}

/* (Re)configures the console/text-plane cell geometry from a fresh
 * cell record at `arg1` (a `struct cell_anim_header`: its `cols`/`rows`) - cell pixel area, its DMA-scroll-wrap threshold, and the
 * initial X/Y scroll accumulators - then rebuilds both VRAM screen
 * blocks via `ResetCellAnimBg`.
 *
 * Matched in the second near-miss sweep. The ROM stores
 * `gCellAnimFrameSize` once, after the `if`, then reloads it for the
 * division through a *copy* of its address taken before the branch
 * (`ldr r4, =A4; ...; add r1, r4, #0`). The copy is a second pointer
 * local, `reload` (dividing through `*a4` itself loses it). Evaluation order
 * fixes the rest: the flag goes through a pointer to
 * `gCellAnimHasBanks` loaded first, `area` is assigned inside the
 * `gCellAnimTileBytes` store so that global's address loads before the
 * multiply, and `size` is read back from `gCellAnimTileBytes` between
 * taking the address and copying it. Matches under both compilers. */
void InitCellAnim(s32 arg0, void *cellAnim, u32 animSize, s32 arg3)
{
    struct cell_anim_header *cell = (struct cell_anim_header *)cellAnim;
    s32 area;
    s32 flag;
    s32 size;
    u8 *pFlag = &gCellAnimHasBanks;

    flag = (arg0 == CATEGORY_TYPE_POLAR);
    *pFlag = flag;
    gCellAnim = cell;
    gCellAnimCols = cell->cols;
    gCellAnimRows = cell->rows;
    gCellAnimTileBytes = (area = gCellAnimCols * gCellAnimRows) << 5;
    {
        s32 *a4 = &gCellAnimFrameSize;
        s32 *reload;

        size = gCellAnimTileBytes;
        reload = a4;
        if (flag)
            size += (area + 7) / 8 * 4;
        *a4 = size;
        gCellAnimLength = INT_TO_Q8(__divsi3(animSize - 0x204, *reload));
    }
    gCellAnimTime = 0;
    ResetCellAnimBg();
    gCellAnimSpeed = 0;
    if (gCellAnimHasBanks == 0)
        gCellAnimDistance = Q8_TO_INT(arg3 - gActorFocalLength);
    else
        gCellAnimDistance = Q8_TO_INT(arg3 + gActorFocalLength);
    gCellAnimFrameStep = 0;
}

/* `FillCellAnimTilemap` (below), inlined here twice: fills screen
 * block 0x0600E400 (or 0x0600F400 when `arg0` is set, numbering on
 * from `w * h + 1`) with consecutive tile numbers for a `w` x `h` cell
 * grid; columns past 31 go to the next screen block (+0x7c0 bytes).
 * `tile++` sits in each branch: with one increment after the `if`,
 * `base` and `tile` swap registers. Unlike the standalone copy, both
 * branches assign `base` here; the default-initializer form is 5
 * halfwords off once inlined with a constant `arg0`. */
static inline void FillTileMap(s32 arg0, s32 w, s32 h)
{
    u16 *base;
    s32 tile;
    s32 row, col;

    if (arg0 != 0) {
        base = (u16 *)(VRAM + 0xF400);
        tile = h * w + 1;
    } else {
        base = (u16 *)(VRAM + 0xE400);
        tile = 1;
    }
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            if (col <= 0x1f)
                base[col] = tile++;
            else
                base[col + 0x3e0] = tile++;
        }
        base += 0x20;
    }
}

/* Resets the console/text plane: display mode, BG0 control, palette
 * DMA from the cell record, and (unless `gCellAnimHasBanks` is set)
 * clears the first tile and the 0x0600E000 screen block and fills both
 * tile maps. The clear loop needs `vram` as a local and an upward `i`
 * (gcc reverses it into the ROM's pointer loop). Matches under both
 * compilers. */
void ResetCellAnimBg(void)
{
    REG_DISPCNT = 0x1141;
    REG_BG0CNT = 0x5c02;
    DmaSet(3, gCellAnim, PLTT, 0x80000100);
    if (gCellAnimHasBanks == 0) {
        u32 *vram = (u32 *)VRAM;
        s32 i;

        for (i = 0; i < 8; i++)
            vram[i] = 0;
        DmaFill16(3, 0, VRAM + 0xE000, 0x2000);
        FillTileMap(0, gCellAnimCols, gCellAnimRows);
        FillTileMap(1, gCellAnimCols, gCellAnimRows);
    }
    gCellAnimPage = 1;
    UploadCellAnimFrame();
}

/* Empty hook InitActorCategory calls at the end of each attempt (after
 * DestroyAllActors), named after that call site. ActorCategoryEndStub
 * (actor_bg.cpp) is its counterpart at the end of the whole category. */
void ActorCategoryAttemptEndStub(void)
{
}

/* `gCellAnimCols`/`gCellAnimRows` are a tile width/height pair
 * (set by InitCellAnim, still raw) for this same scroll-effect subsystem;
 * this computes their product doubled plus one - likely a tile-count-
 * to-byte-count-ish conversion for a buffer this subsystem allocates,
 * not traced further. */
s32 GetCellAnimFreeTile(void)
{
    return gCellAnimCols * gCellAnimRows * 2 + 1;
}

/* Toggles the BG0 tile-set half used for the console/text plane
 * (`gCellAnimPage`), committing the choice to `REG_BG0CNT`, once
 * `gCellAnimUploaded` signals the previous DMA is done. Written as two
 * full, duplicated `REG_BG0CNT = ...` branches rather than a ternary -
 * this compiler emits a separate `REG_BG0CNT` literal-pool reference per
 * branch for the duplicated-store form (matching the ROM's own two
 * `.4byte 0x04000008` pool entries), while a ternary/single-store
 * collapses it to one shared pool entry and a shorter, differently
 * shaped sequence (see docs/workflow.md step 3/7). The trailing toggle
 * goes through a `toggled` local set to `1` first - this compiler
 * otherwise canonicalizes `1 ^ gCellAnimPage`/`gCellAnimPage ^= 1` the
 * same way regardless of source operand order, loading the memory
 * operand first, where the ROM loads the constant `1` first (see
 * docs/workflow.md step 3). */
void FlipCellAnimPage(void)
{
    if (gCellAnimUploaded != 0) {
        u8 toggled;

        if (gCellAnimPage != 0) {
            REG_BG0CNT = 0x5C02;
        } else {
            REG_BG0CNT = 0x5E02;
        }
        gCellAnimUploaded = 0;
        toggled = 1;
        toggled ^= gCellAnimPage;
        gCellAnimPage = toggled;
    }
}

/* Trivial getter for this scroll-effect subsystem's accumulated X
 * offset, set by AdvanceCellAnim (still raw). */

s32 GetCellAnimDistance(void)
{
    return gCellAnimDistance;
}

/* Advances the console/text-plane's horizontal scroll accumulator by
 * `gCellAnimSpeed` (a Q8.8 per-frame velocity), wrapping it against
 * `gCellAnimLength`, and - whenever the whole-tile column actually
 * changed - shifts the visible-column counter and re-triggers the
 * pending-cell DMA/palette-cursor pair. The ROM keeps the updated
 * position `pos` in a separate register from the old value `prev`.
 * `bcPtr` is materialized *before* the shift/subtract that becomes
 * `delta` - the ROM loads the store destination's address ahead of
 * computing the value, not right before the store (see
 * docs/workflow.md step 3 for this family of fixes). */
void AdvanceCellAnim(void)
{
    s32 *b0ptr = &gCellAnimTime;
    s32 prev = *b0ptr;
    s32 prevShifted = Q8_TO_INT(prev);
    s32 pos = prev + gCellAnimSpeed;
    s32 *bcPtr;
    s32 delta;

    *b0ptr = pos;
    bcPtr = &gCellAnimFrameStep;
    delta = Q8_TO_INT(pos) - prevShifted;
    *bcPtr = delta;

    if (pos >= gCellAnimLength) {
        *b0ptr = pos - gCellAnimLength;
    }

    if (delta != 0) {
        gCellAnimDistance += delta;
        UploadCellAnimFrame();
        UpdateActorPaletteCycle();
    }
}

/* Trivial getter for this scroll-effect subsystem's per-tick delta,
 * set by AdvanceCellAnim (still raw). */
s32 GetCellAnimFrameStep(void)
{
    return gCellAnimFrameStep;
}

/* `(v*15) << 2 >> 8` on gCellAnimSpeed (a Q8.8-ish speed/step value
 * set by SetCellAnimSpeed, still raw) - written as the ROM's own
 * shift-subtract-shift idiom (`(v<<4) - v`, i.e. `v*15`) rather than a
 * plain `* 15` to match its exact instruction sequence. */
s32 GetCellAnimSpeed(void)
{
    s32 v = gCellAnimSpeed;
    return Q8_TO_INT(((v << 4) - v) << 2);
}

/* `dest` is materialized before the `__divsi3` call, not after (a
 * plain `gCellAnimSpeed = ...` loads it after) - the ROM loads the store
 * destination's address ahead of the call so it survives across it in
 * a register `bl` doesn't clobber, rather than recomputing it from the
 * return value's position afterward (see docs/workflow.md step 3). */
void SetCellAnimSpeed(s32 arg0)
{
    s32 *dest = &gCellAnimSpeed;

    *dest = __divsi3(INT_TO_Q8(arg0), 0x3c);
}

/* Fills screen block 28 from row 16 on (block 30 when `arg0` is set,
 * numbering on from `w * h + 1`) with consecutive tile numbers for a
 * `w` x `h` cell grid; columns past 31 go to the next screen block
 * (+0x7c0 bytes). `ResetCellAnimBg` above inlines the same body
 * twice.
 *
 * The old NAKED note blamed an unreachable register permutation. The
 * fix is `tile++` inside each branch of the column test: with one
 * increment after the `if`, `tile` has fewer references than `base`
 * and the two swap registers (the `r8`/`ip` roles of the 0x7c0 offset
 * and `h` follow from that). Matches under both compilers. */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void FillCellAnimTilemap(s32 arg0, s32 w, s32 h)
{
    u16 *base = (u16 *)(BG_SCREEN_ADDR(28) + 0x400);
    s32 tile;
    s32 row, col;

    if (arg0 != 0) {
        base = (u16 *)(BG_SCREEN_ADDR(30) + 0x400);
        tile = h * w + 1;
    } else {
        tile = 1;
    }
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            if (col <= 0x1f)
                base[col] = tile++;
            else
                base[col + 0x3e0] = tile++;
        }
        base += 0x20;
    }
}

/* Selects one of two fixed BG0/BG1 scroll-effect parameter sets
 * (arg0 == 0 vs nonzero), then derives the shared initial scroll
 * position/register state from them. */
void InitActorBgScroll(s32 arg0)
{
    s32 v;

    gActorBgScrollType = arg0;

    if (arg0 == CATEGORY_TYPE_POLAR) {
        gActorNearClipDepth = 0x88 << 5;
        gActorFarClipDepth = 0xa0 << 8;
        gActorFocalLength = 0xbc << 6;
        gActorBgWidth = 0x98 << 9;
        gActorBgHeight = 0xd0 << 8;
        gActorBgScrollEaseShift = 2;
        gActorBgScrollRangeX = 0x64;
        gActorBgScrollRangeY = 0x51;
        gActorBg0VOffset = arg0;
    } else {
        gActorNearClipDepth = 0xd0 << 5;
        gActorFarClipDepth = 0xaa << 8;
        gActorFocalLength = 0xe0 << 5;
        gActorBgWidth = 0x98 << 9;
        gActorBgHeight = 0xce << 8;
        gActorBgScrollEaseShift = 3;
        gActorBgScrollRangeX = 3 + 0xfd;
        gActorBgScrollRangeY = 0x96;
        gActorBg0VOffset = 2;
    }

    /* `v` stays in `r1` from the moment it's first computed
     * (as `gActorBgScrollMaxX`'s new value) through the sign-rounded
     * `/2`/`>>9`/`>>9` triple below, all reusing that same register in
     * place rather than reloading `gActorBgScrollMaxX` fresh - matching
     * the ROM's own single, unbroken chain of `r1` uses (see
     * docs/workflow.md step 3). `REG_BG0VOFS` is just
     * `gActorBg0VOffset` alone here, NOT
     * `gActorBg0VOffset + (v >> 9)` - there's no addition in the ROM's
     * own instructions for this store. */
    {
        s32 *ecPtr = &gActorBgScrollMaxX;

        v = gActorBgWidth + (s32)0xFFFF1000;
        *ecPtr = v;
    }
    gActorBgScrollMaxY = gActorBgHeight + (s32)0xFFFF6000;

    {
        s32 *d0Ptr = &gActorBgScrollX;

        v = v + (s32)((u32)v >> 31);
        *d0Ptr = v >> 1;
    }
    gActorBgScrollY = 0;

    {
        vu16 *bg0hofsPtr = &REG_BG0HOFS;

        v >>= 9;
        *bg0hofsPtr = v;
    }
    REG_BG0VOFS = gActorBg0VOffset;
    REG_BG1HOFS = v;
    REG_BG1VOFS = 0;

    gActorBgShake = 0;
    gActorBgLayerDepth = gActorFarClipDepth;
}

/* Eases the BG0/BG1 scroll accumulators (gActorBgScrollX/gActorBgScrollY)
 * toward their per-axis target/scale-derived offsets
 * (gActorBgScrollMaxX/gActorBgScrollMaxY), clamping each to
 * [0, target]. arg0/arg1 are the two axes' own driving values. Each
 * axis has its own target and delta locals (shared ones put the clamps in
 * other registers). */
void UpdateActorBgScroll(s32 arg0, s32 arg1)
{
    s32 targetX, targetY;
    s32 deltaX, deltaY;
    s32 cur;
    s32 shift;

    targetX = gActorBgScrollMaxX;
    deltaX = __divsi3(arg0 * Q8_TO_INT(targetX), gActorBgScrollRangeX);
    deltaX += targetX / 2;
    cur = gActorBgScrollX;
    deltaX -= cur;
    shift = gActorBgScrollEaseShift;
    deltaX >>= shift;
    cur += deltaX;
    gActorBgScrollX = cur;
    LIMIT_MIN(cur, 0);
    cur = MIN(cur, targetX);
    gActorBgScrollX = cur;

    targetY = gActorBgScrollMaxY;
    deltaY = __divsi3(arg1 * Q8_TO_INT(targetY), gActorBgScrollRangeY);
    deltaY += targetY / 2;
    cur = gActorBgScrollY;
    deltaY -= cur;
    deltaY >>= shift;
    deltaY -= gActorBgShake;
    cur += deltaY;
    gActorBgScrollY = cur;
    LIMIT_MIN(cur, 0);
    cur = MIN(cur, targetY);
    gActorBgScrollY = cur;
}
