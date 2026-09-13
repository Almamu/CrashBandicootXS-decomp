#include "core.h"

/* Sits right after sub_800132C (ROM 0x0800132C, in src/fade_util.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

extern u16 gUnknown_03000A80[512];
extern u16 gUnknown_03000E80[512];

/* Blends the whole 512-entry palette at `gUnknown_03000A80` toward
 * black by `factor`/16 per channel (5 bits each, GBA BGR555), writing
 * the result to `gUnknown_03000E80`. Each channel is extracted via an
 * explicit shift-left-then-shift-right pair (not a plain `&`/`>>`) and
 * re-inserted via a "clear those bits, then OR the new value in"
 * sequence - matching the ROM's own instruction shapes, which use this
 * shape even for pulling the initial raw 16-bit pixel into the
 * (reused, never explicitly zeroed) `color` accumulator register.
 * Several inline-asm-anchored temporaries (`tmp`/`diff`, both pinned to
 * r0) are needed to reproduce exact ROM register/instruction choices
 * that gcc's own optimizer would otherwise collapse into shorter but
 * differently-shaped code: the extraction's two shifts naturally
 * collapse into one register when written as a single C expression;
 * the post-subtract `(u16)` truncate before the final 5-bit mask gets
 * optimized away entirely (correct result, but ROM has the redundant
 * 16-bit truncate first); and the channel-2/3 insert's mask-then-shift
 * vs shift-then-mask ordering matters for exact instruction order even
 * though both compute the same value. */
void sub_80013FC(s32 factor)
{
    s32 i;

    for (i = 0; i <= 0x1FF; i++) {
        s32 color;
        register s32 ch asm("r1");
        s32 scaled;
        register s32 raw asm("r1");
        register u16 *addr asm("r1");

        addr = &gUnknown_03000A80[i];
        color &= ~0xFFFF;
        raw = *addr;
        color |= raw;

        {
            register s32 tmp asm("r0");
            tmp = color << 27;
            ch = (s32)((u32)tmp >> 27);
        }
        scaled = ch * factor;
        if (scaled < 0) scaled += 15;
        scaled >>= 4;
        {
            register s32 diff asm("r0");
            diff = ch - scaled;
            asm("lsl %0, %0, #0x10\n\tlsr %0, %0, #0x10" : "+r"(diff));
            diff &= 0x1F;
            color = (color & ~0x1F) | diff;
        }

        {
            register s32 tmp asm("r0");
            tmp = color << 22;
            ch = (s32)((u32)tmp >> 27);
        }
        scaled = ch * factor;
        if (scaled < 0) scaled += 15;
        scaled >>= 4;
        {
            register s32 diff asm("r0");
            diff = ch - scaled;
            asm("lsl %0, %0, #0x10\n\tlsr %0, %0, #0x10" : "+r"(diff));
            diff &= 0x1F;
            diff <<= 5;
            color = (color & ~(0x1F << 5)) | diff;
        }

        {
            register s32 tmp asm("r0");
            tmp = color << 17;
            ch = (s32)((u32)tmp >> 27);
        }
        scaled = ch * factor;
        if (scaled < 0) scaled += 15;
        scaled >>= 4;
        {
            register s32 diff asm("r0");
            diff = ch - scaled;
            asm("lsl %0, %0, #0x10\n\tlsr %0, %0, #0x10" : "+r"(diff));
            diff &= 0x1F;
            diff <<= 10;
            color = (color & ~(0x1F << 10)) | diff;
        }

        gUnknown_03000E80[i] = color;
    }
}
