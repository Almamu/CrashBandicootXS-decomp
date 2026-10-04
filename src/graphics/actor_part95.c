#include "core.h"
#include "actor_anim.h"
#include "gba/io_reg.h"

asm(".set _call_via_r4, sub_803AD88");

extern s32 gUnknown_03001384;
extern s32 gUnknown_03001388;
extern s32 gUnknown_0300138C;
extern s32 gUnknown_03001390;
extern s32 gUnknown_03000878;
extern void *gLevelState;

extern s32 sub_802A4E0(void);
extern void sub_8022CA0(void *arg0);
extern void sub_802AB34(void);

/* Re-bases the category's secondary tick counter from `arg0` (net of
 * `sub_802A4E0`'s current Q8.8 offset), resets the active-instance
 * counters, and re-syncs the frame-tick snapshot for a freshly
 * (re)selected category. */
void sub_8029748(s32 arg0)
{
    gUnknown_03000878 = arg0 - sub_802A4E0();
    gUnknown_03001384 = 0;
    gUnknown_03001388 = 0;
    sub_8022CA0(gLevelState);
    gUnknown_03001390 = gUnknown_0300138C;
    sub_802AB34();
}

extern s32 gUnknown_03001380;

/* True once the running active-instance count reaches the current
 * category's `unknown_20` threshold. The cast to `s32` matches the
 * ROM's own signed comparison (`blt`) - `unknown_20` is declared `u32`
 * in actor_anim.h (its sign isn't otherwise pinned down), and the
 * unsigned usual-arithmetic-conversion comparison that produces
 * compiles to the unsigned `bcc` instead (see docs/workflow.md
 * step 3). */
s32 sub_8029794(void)
{
    return gUnknown_03001384 >= (s32)gStaticData_08175558[gUnknown_03001380].unknown_20;
}

extern s32 gUnknown_030013B0;
extern s32 gUnknown_030013A4;
extern void *gUnknown_03001394;
extern u8 gUnknown_030013B8;
extern u8 gUnknown_030013B9;
extern s32 gUnknown_030013A0;
extern void (*gUnknown_0300087C)(void *src, s32 arg1, s32 arg2, s32 arg3);
extern s32 gUnknown_03001398;
extern s32 gUnknown_0300139C;
extern u8 gUnknown_030013BA;

/* Kicks off a DMA copy of `gUnknown_030013A0` bytes from the current
 * "console"/text-plane cursor cell into VRAM (one of three fixed
 * destination strategies depending on the `gUnknown_030013B8`/
 * `gUnknown_030013B9` mode bytes), then arms `gUnknown_030013BA` so a
 * caller can poll for completion. `gUnknown_0300087C` is a function
 * pointer (called through `_call_via_r4`).
 *
 * The `0x204` header offset is added to the row product before the
 * base pointer, the destination is a ternary, and the callback's first
 * argument goes through its own local; each of those fixes one piece of
 * the ROM's instruction order. Matches under both compilers. */
void sub_80297C8(void)
{
    u8 *src = (u8 *)gUnknown_03001394 + ((gUnknown_030013B0 >> 8) * gUnknown_030013A4 + 0x204);
    u32 dst;

    if (gUnknown_030013B8 != 0) {
        u8 *next;

        dst = gUnknown_030013B9 != 0 ? VRAM : VRAM + 0x2000;
        next = src + gUnknown_030013A0;
        gUnknown_0300087C(next, gUnknown_030013B9, gUnknown_03001398, gUnknown_0300139C);
    } else if (gUnknown_030013B9 != 0) {
        dst = VRAM + 0x20;
    } else {
        dst = gUnknown_030013A0 + (VRAM + 0x20);
    }
    DmaSet(3, src, dst, 0x80000000 | (gUnknown_030013A0 / 2));
    gUnknown_030013BA = 1;
}

extern s32 gUnknown_030013AC;
extern s32 gUnknown_030013C8;
extern s32 gUnknown_030013A8;
extern s32 gUnknown_030013B4;
extern s32 gUnknown_030013BC;
extern void sub_802996C(void);
extern s32 sub_803ADB4(s32 a, s32 b);

/* (Re)configures the console/text-plane cell geometry from a fresh
 * cell record at `arg1` (a `struct cell_anim_header`: its `cols`/`rows`) - cell pixel area, its DMA-scroll-wrap threshold, and the
 * initial X/Y scroll accumulators - then rebuilds both VRAM screen
 * blocks via `sub_802996C`.
 *
 * Matched in the second near-miss sweep. The ROM stores
 * `gUnknown_030013A4` once, after the `if`, then reloads it for the
 * division through a *copy* of its address taken before the branch
 * (`ldr r4, =A4; ...; add r1, r4, #0`). The copy is
 * `asm("" : "=r"(reload) : "0"(a4))`, which emits no code but gives
 * gcc a second pointer it can't merge back into `a4`. Evaluation order
 * fixes the rest: the flag goes through a pointer to
 * `gUnknown_030013B8` loaded first, `area` is assigned inside the
 * `gUnknown_030013A0` store so that global's address loads before the
 * multiply, and `size` is read back from `gUnknown_030013A0` between
 * taking the address and copying it. Matches under both compilers. */
void sub_8029890(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct cell_anim_header *cell = (struct cell_anim_header *)arg1;
    s32 area;
    s32 flag;
    s32 size;
    u8 *pFlag = &gUnknown_030013B8;

    flag = (arg0 == 0);
    *pFlag = flag;
    gUnknown_03001394 = cell;
    gUnknown_03001398 = cell->cols;
    gUnknown_0300139C = cell->rows;
    gUnknown_030013A0 = (area = gUnknown_03001398 * gUnknown_0300139C) << 5;
    {
        s32 *a4 = &gUnknown_030013A4;
        s32 *reload;

        size = gUnknown_030013A0;
        asm("" : "=r"(reload) : "0"(a4));
        if (flag)
            size += (area + 7) / 8 * 4;
        *a4 = size;
        gUnknown_030013AC = sub_803ADB4(arg2 - 0x204, *reload) << 8;
    }
    gUnknown_030013B0 = 0;
    sub_802996C();
    gUnknown_030013B4 = 0;
    if (gUnknown_030013B8 == 0)
        gUnknown_030013A8 = (arg3 - gUnknown_030013C8) >> 8;
    else
        gUnknown_030013A8 = (arg3 + gUnknown_030013C8) >> 8;
    gUnknown_030013BC = 0;
}

/* `sub_8029BC4` (actor_part98.c), inlined here twice: fills screen
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
 * DMA from the cell record, and (unless `gUnknown_030013B8` is set)
 * clears the first tile and the 0x0600E000 screen block and fills both
 * tile maps. The clear loop needs `vram` as a local and an upward `i`
 * (gcc reverses it into the ROM's pointer loop). Matches under both
 * compilers. */
void sub_802996C(void)
{
    REG_DISPCNT = 0x1141;
    REG_BG0CNT = 0x5c02;
    DmaSet(3, gUnknown_03001394, PLTT, 0x80000100);
    if (gUnknown_030013B8 == 0) {
        u32 *vram = (u32 *)VRAM;
        s32 i;

        for (i = 0; i < 8; i++)
            vram[i] = 0;
        DmaFill16(3, 0, VRAM + 0xE000, 0x2000);
        FillTileMap(0, gUnknown_03001398, gUnknown_0300139C);
        FillTileMap(1, gUnknown_03001398, gUnknown_0300139C);
    }
    gUnknown_030013B9 = 1;
    sub_80297C8();
}
