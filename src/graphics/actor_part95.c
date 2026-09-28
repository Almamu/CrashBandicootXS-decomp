#include "core.h"
#include "actor_anim.h"
#include "gba/io_reg.h"

asm(".set _call_via_r4, sub_803AD88");

extern s32 gUnknown_03001384;
extern s32 gUnknown_03001388;
extern s32 gUnknown_0300138C;
extern s32 gUnknown_03001390;
extern s32 gUnknown_03000878;
extern void *gUnknown_030012C0;

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
    sub_8022CA0(gUnknown_030012C0);
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

        dst = gUnknown_030013B9 != 0 ? 0x06000000 : 0x06002000;
        next = src + gUnknown_030013A0;
        gUnknown_0300087C(next, gUnknown_030013B9, gUnknown_03001398, gUnknown_0300139C);
    } else if (gUnknown_030013B9 != 0) {
        dst = 0x06000020;
    } else {
        dst = gUnknown_030013A0 + 0x06000020;
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

/* NAKED - (re)configures the console/text-plane cell geometry from a
 * fresh cell record at `arg1` (a `{..., s16 width @0x200,
 * s16 height @0x202}` layout) - cell pixel area, its DMA-scroll-wrap
 * threshold, and the initial X/Y scroll accumulators - then rebuilds
 * both VRAM screen blocks via `sub_802996C`.
 *
 * The draft below is 37 halfwords off under both compilers, all in the
 * `gUnknown_030013A4` block: the ROM stores A4 once, after the `if`,
 * and then reloads it for the division; the draft stores it twice.
 * Storing a `size` local once instead makes gcc forward the value into
 * the division (no reload), and the flag byte loses `r5`. */
#if NON_MATCHING
void sub_8029890(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *cell = (u8 *)arg1;
    s32 area;

    gUnknown_030013B8 = (arg0 == 0);
    gUnknown_03001394 = cell;
    gUnknown_03001398 = *(s16 *)(cell + 0x200);
    gUnknown_0300139C = *(s16 *)(cell + 0x202);
    area = gUnknown_03001398 * gUnknown_0300139C;
    gUnknown_030013A4 = gUnknown_030013A0 = area << 5;
    if (gUnknown_030013B8)
        gUnknown_030013A4 += (area + 7) / 8 * 4;
    gUnknown_030013AC = sub_803ADB4(arg2 - 0x204, gUnknown_030013A4) << 8;
    gUnknown_030013B0 = 0;
    sub_802996C();
    gUnknown_030013B4 = 0;
    if (gUnknown_030013B8 == 0)
        gUnknown_030013A8 = (arg3 - gUnknown_030013C8) >> 8;
    else
        gUnknown_030013A8 = (arg3 + gUnknown_030013C8) >> 8;
    gUnknown_030013BC = 0;
}
#else
NAKED void sub_8029890(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "add r7, r3, #0\n\t"
        "ldr r1, 1f\n\t"
        "mov r5, #0\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "mov r5, #1\n\t"
        "2:\n\t"
        "strb r5, [r1]\n\t"
        "ldr r0, 3f\n\t"
        "str r4, [r0]\n\t"
        "ldr r1, 4f\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #2\n\t"
        "add r0, r4, r2\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r0, r3]\n\t"
        "str r2, [r1]\n\t"
        "ldr r1, 5f\n\t"
        "ldr r3, 6f\n\t"
        "add r0, r4, r3\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r0, r3]\n\t"
        "str r0, [r1]\n\t"
        "ldr r1, 7f\n\t"
        "mul r2, r0, r2\n\t"
        "lsl r0, r2, #5\n\t"
        "str r0, [r1]\n\t"
        "ldr r4, 8f\n\t"
        "add r3, r0, #0\n\t"
        "add r1, r4, #0\n\t"
        "cmp r5, #0\n\t"
        "beq 9f\n\t"
        "add r0, r2, #7\n\t"
        "cmp r0, #0\n\t"
        "bge 10f\n\t"
        "add r0, #7\n\t"
        "10:\n\t"
        "asr r0, r0, #3\n\t"
        "lsl r0, r0, #2\n\t"
        "add r3, r3, r0\n\t"
        "9:\n\t"
        "str r3, [r4]\n\t"
        "ldr r4, 11f\n\t"
        "ldr r2, 12f\n\t"
        "add r0, r6, r2\n\t"
        "ldr r1, [r1]\n\t"
        "bl sub_803ADB4\n\t"
        "lsl r0, r0, #8\n\t"
        "str r0, [r4]\n\t"
        "ldr r0, 13f\n\t"
        "mov r4, #0\n\t"
        "str r4, [r0]\n\t"
        "bl sub_802996C\n\t"
        "ldr r0, 14f\n\t"
        "str r4, [r0]\n\t"
        "ldr r0, 1f\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 15f\n\t"
        "ldr r1, 16f\n\t"
        "ldr r0, 17f\n\t"
        "ldr r0, [r0]\n\t"
        "sub r0, r7, r0\n\t"
        "b 18f\n\t"
        ".align 2, 0\n"
        "1: .4byte gUnknown_030013B8\n"
        "3: .4byte gUnknown_03001394\n"
        "4: .4byte gUnknown_03001398\n"
        "5: .4byte gUnknown_0300139C\n"
        "6: .4byte 0x00000202\n"
        "7: .4byte gUnknown_030013A0\n"
        "8: .4byte gUnknown_030013A4\n"
        "11: .4byte gUnknown_030013AC\n"
        "12: .4byte 0xFFFFFDFC\n"
        "13: .4byte gUnknown_030013B0\n"
        "14: .4byte gUnknown_030013B4\n"
        "16: .4byte gUnknown_030013A8\n"
        "17: .4byte gUnknown_030013C8\n"
        "15:\n\t"
        "ldr r1, 19f\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r7, r0\n\t"
        "18:\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [r1]\n\t"
        "ldr r1, 21f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r1]\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "19: .4byte gUnknown_030013A8\n"
        "20: .4byte gUnknown_030013C8\n"
        "21: .4byte gUnknown_030013BC\n"
    );
}
#endif

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
        base = (u16 *)0x0600F400;
        tile = h * w + 1;
    } else {
        base = (u16 *)0x0600E400;
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
    DmaSet(3, gUnknown_03001394, 0x05000000, 0x80000100);
    if (gUnknown_030013B8 == 0) {
        u32 *vram = (u32 *)0x06000000;
        s32 i;

        for (i = 0; i < 8; i++)
            vram[i] = 0;
        DmaFill16(3, 0, 0x0600E000, 0x2000);
        FillTileMap(0, gUnknown_03001398, gUnknown_0300139C);
        FillTileMap(1, gUnknown_03001398, gUnknown_0300139C);
    }
    gUnknown_030013B9 = 1;
    sub_80297C8();
}
