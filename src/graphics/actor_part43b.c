#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md.
 *
 * Computes two `self`-keyframe-driven sizes (byte0*byte1, scaled by
 * 32) via `AllocVramTileBlock`, storing them into the `gUnknown_03001518`
 * pair, then arms `gUnknown_03001510`/clears `gUnknown_03001514`. Both
 * keyframe-size sub-blocks are literally identical computations,
 * matching the ROM's own duplication.
 *
 * The ROM computes `byte0*byte1` into one register, copies it to a
 * second, *then* shifts (`adds r2,r3,#0; muls r2,r1,r2; adds r0,r2,#0;
 * lsls r0,r0,#5`) - this compiler's dead-store elimination always
 * collapses a plain `(rec[1] * rec[0]) << 5` into a shorter
 * compute-and-shift-in-place sequence, so the extra copy is
 * materialized via an opaque `asm volatile` matching the ROM's exact
 * register roles (same class of gap as `sub_802C2FC`/`sub_803B46C`,
 * issue #52/#71). The two blocks' index/address computation
 * (`table + idx*3*4 + 2`) also needed its own register roles pinned to
 * match: `table` loaded early into r3, the `+2` index constant
 * materialized via an opaque `mov #2` immediately before the `ldrsh`
 * (a bare C-level `register`-pinned local has no effect here, since
 * gcc constant-folds the literal and freely picks its own register),
 * and the final byte-load pair (`rec[0]`/`rec[1]`) pinned per-block to
 * the exact registers the ROM's `ldrb` pair uses. */
extern void *AllocVramTileBlock(s32 size);
extern void *gUnknown_03001518[2];
extern s32 gUnknown_03001510;
extern s32 gUnknown_03001514;

void sub_802F338(void *selfArg)
{
    u8 *self = selfArg;

    {
        s32 accum = *(s32 *)(self + 8) >> 8;
        s32 idx = *(s32 *)(self + 0xc);
        u8 *table = *(u8 **)self;
        s16 off = *(s16 *)(table + idx * 3 * 4 + 2);
        s32 pos = off + accum;
        u8 **table2 = *(u8 ***)(self + 4);
        u8 *rec = table2[pos];
        register s32 b0 asm("r3") = rec[0];
        register s32 b1 asm("r1") = rec[1];
        register s32 temp asm("r2");
        register s32 size asm("r0");

        asm volatile(
            "add %0, %2, #0\n\t"
            "mul %0, %1, %0\n\t"
            "add %3, %0, #0\n\t"
            "lsl %3, %3, #5\n\t"
            : "=r"(temp), "+r"(b1), "+r"(b0), "=r"(size)
        );
        gUnknown_03001518[0] = AllocVramTileBlock(size);
    }
    {
        s32 accum = *(s32 *)(self + 8) >> 8;
        s32 idx = *(s32 *)(self + 0xc);
        register u8 *table asm("r3") = *(u8 **)self;
        register s32 shiftResult asm("r0") = idx * 3 * 4;
        register u8 *addr2 asm("r0");
        register s32 twoIdx asm("r3");
        register s32 off asm("r0");
        s32 pos;
        u8 **table2;
        u8 *rec;

        asm volatile("add %0, %0, %1" : "+r"(shiftResult) : "r"(table));
        addr2 = (u8 *)shiftResult;
        asm volatile("mov %0, #2\n\tldrsh %1, [%2, %0]" : "=r"(twoIdx), "=r"(off) : "r"(addr2));
        pos = off + accum;
        table2 = *(u8 ***)(self + 4);
        rec = table2[pos];
        {
            register s32 b0 asm("r2") = rec[0];
            register s32 b1 asm("r3") = rec[1];
            register s32 temp asm("r1");
            register s32 size asm("r0");

            asm volatile(
                "add %0, %2, #0\n\t"
                "mul %0, %1, %0\n\t"
                "add %3, %0, #0\n\t"
                "lsl %3, %3, #5\n\t"
                : "=r"(temp), "+r"(b1), "+r"(b0), "=r"(size)
            );
            gUnknown_03001518[1] = AllocVramTileBlock(size);
        }
    }

    gUnknown_03001510 = 1;
    gUnknown_03001514 = 0;
}

asm(".align 2, 0");
