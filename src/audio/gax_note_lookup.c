#include "core.h"

/* Resolves a pattern-note index into an interpolated pitch/volume byte
 * from a small sorted breakpoint table (`table`, 8-byte records:
 * `+0/+1/+2/+3` control bytes, `+4` a u16 breakpoint index, `+6` a signed
 * slope, `+8` a value byte). `*out` is both read (the previous note
 * index) and advanced to the new one on entry. Two "snap to a fixed
 * envelope" side effects run first: a portamento retrigger snap (using
 * `table[1]`) and an end-of-table "note off" arm (using the last
 * breakpoint, `table[table[0]-1]`, which - if its value byte is 0 and no
 * higher-priority override table entry (`table[3]`) applies - arms
 * `self->0x2a/0x2c/0x4c` with the fixed `0x8AD0` "no note" envelope), and
 * a loop-point remap via `table[2]`/`table[3]`. Finally scans the table
 * for the first breakpoint whose index is `>= *out`'s (new) note index
 * and returns either that breakpoint's value directly (exact match) or a
 * linearly-interpolated value between it and the previous breakpoint
 * (using the previous breakpoint's slope, `>> 8`, Q8 scale). Voice/table
 * object shapes aren't modeled - kept as raw offsets, same as the
 * neighboring GAX2 engine internals in this directory.
 *
 * Written as NAKED asm, not plain C: a near-matching C reconstruction is
 * kept below under `#if NON_MATCHING` - see
 * docs/matching/issue-68-channel-bind-envelope-note.md for the
 * derivation and the one-instruction residual (gcc-2.9's own constant-
 * pool materialization for the `0x8AD0` sentinel picks a different
 * scratch register, `r2`, than the ROM's `r0`, requiring one extra
 * `adds r0, r2, #0` copy - confirmed at the compiler's own generated-
 * assembly level, not just the final object bytes, so this is a genuine
 * codegen limitation rather than an unexplored C phrasing). Mechanical,
 * byte-verified transcription of the ROM's own instructions in the
 * meantime. */
#if NON_MATCHING
/* NOT YET BYTE-MATCHING - every instruction's operation, operand, and
 * (with one exception) register matches the ROM; compiled only under
 * `make NON_MATCHING=1`, the NAKED version below is used otherwise. See
 * the doc comment above and
 * docs/matching/issue-68-channel-bind-envelope-note.md for the residual. */
u8 sub_8039F30(void *self, void *table, u16 *out)
{
    register u8 *selfR asm("r5") = (u8 *)self;
    register u8 *tableR asm("r3") = (u8 *)table;
    register u16 *outR asm("r6") = out;
    register u32 idx asm("r4");

    {
        register s32 rawOut asm("r0") = *outR;
        register s32 nextOut asm("r1") = rawOut + 1;
        *outR = nextOut;
        asm volatile("lsl %0, %0, #0x10\n\tlsr %1, %0, #0x10" : "+r"(rawOut), "=r"(idx));
    }

    if (*(u8 *)(tableR + 1) != 0xff) {
        if (*(u8 *)(selfR + 0x22) == 0) {
            u8 b1 = *(u8 *)(tableR + 1);
            u16 *rec = (u16 *)(tableR + (b1 << 3) + 4);
            if (idx == *rec) {
                *outR = idx;
            }
        }
    }

    {
        u8 lastIdx = *tableR;
        s32 last = lastIdx - 1;
        u8 *lastRec = tableR + (last << 3);
        u16 lastBp = *(u16 *)(lastRec + 4);
        if (idx >= lastBp) {
            register u8 lastVal asm("r1") = *(u8 *)(lastRec + 8);
            if (lastVal == 0) {
                u8 override = *(u8 *)(tableR + 3);
                if (override == 0xff || override < last) {
                    *(u16 *)(selfR + 0x2a) = 0x8ad0;
                    asm volatile("strh %0, [%1, #0x2c]" :: "r"(lastVal), "r"(selfR));
                    *(u32 *)(selfR + 0x4c) = 0x80000000;
                }
            }
            *outR = idx;
        }
    }

    {
        register u8 *s0 asm("r0") = selfR + 0x22;
        if (*s0 == 0) {
            if (*(u8 *)(tableR + 2) != 0xff) {
                u8 override2 = *(u8 *)(tableR + 3);
                if (override2 != 0xff) {
                    u16 *rec2 = (u16 *)(tableR + (override2 << 3) + 4);
                    if (idx == *rec2) {
                        u8 loopIdx = *(u8 *)(tableR + 2);
                        u16 *rec3 = (u16 *)(tableR + (loopIdx << 3) + 4);
                        *outR = *rec3;
                    }
                }
            }
        }
    }

    {
        register s32 i asm("r1") = 0;
        {
            register u16 bp0 asm("r0") = *(u16 *)(tableR + 4);
            if (bp0 < idx) {
                register u8 *scan asm("r2") = tableR;
                do {
                    scan += 8;
                    i += 1;
                    bp0 = *(u16 *)(scan + 4);
                } while (bp0 < idx);
            }
        }

        {
            register u8 *found asm("r0") = tableR + (i << 3);
            register u16 bp asm("r2") = *(u16 *)(found + 4);
            if (idx == bp) {
                return *(u8 *)(found + 8);
            } else {
                register s32 slope asm("r2");
                {
                    register u8 six asm("r5") = 6;
                    asm volatile("ldrsh %0, [%1, %2]" : "=r"(slope) : "r"(found), "r"(six));
                }
                i -= 1;
                {
                    register u8 *prevRec asm("r1") = tableR + (i << 3);
                    register u16 prevBp asm("r0") = *(u16 *)(prevRec + 4);
                    register s32 delta asm("r0") = idx - prevBp;
                    register s32 val asm("r0") = slope * delta;
                    val >>= 8;
                    {
                        register u8 prevVal asm("r1") = *(u8 *)(prevRec + 8);
                        val += prevVal;
                    }
                    return (u8)val;
                }
            }
        }
    }
}
#else /* !NON_MATCHING */
NAKED u8 sub_8039F30(void *self, void *table, u16 *out)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "add r5, r0, #0\n\t"
        "add r3, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "ldrh r0, [r6]\n\t"
        "add r1, r0, #1\n\t"
        "strh r1, [r6]\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r4, r0, #0x10\n\t"
        "ldrb r0, [r3, #1]\n\t"
        "cmp r0, #0xff\n\t"
        "beq L9F30_0\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x22\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne L9F30_0\n\t"
        "ldrb r0, [r3, #1]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r3, r0\n\t"
        "ldrh r0, [r0, #4]\n\t"
        "cmp r4, r0\n\t"
        "bne L9F30_0\n\t"
        "strh r4, [r6]\n\t"
        "L9F30_0:\n\t"
        "ldrb r0, [r3]\n\t"
        "sub r2, r0, #1\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r3, r0\n\t"
        "ldrh r1, [r0, #4]\n\t"
        "cmp r4, r1\n\t"
        "blo L9F30_1\n\t"
        "ldrb r1, [r0, #8]\n\t"
        "cmp r1, #0\n\t"
        "bne L9F30_2\n\t"
        "ldrb r0, [r3, #3]\n\t"
        "cmp r0, #0xff\n\t"
        "beq L9F30_3\n\t"
        "cmp r0, r2\n\t"
        "bge L9F30_2\n\t"
        "L9F30_3:\n\t"
        "ldr r0, L9F30_9\n\t"
        "strh r0, [r5, #0x2a]\n\t"
        "strh r1, [r5, #0x2c]\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #0x18\n\t"
        "str r0, [r5, #0x4c]\n\t"
        "L9F30_2:\n\t"
        "strh r4, [r6]\n\t"
        "L9F30_1:\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x22\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne L9F30_4\n\t"
        "ldrb r0, [r3, #2]\n\t"
        "cmp r0, #0xff\n\t"
        "beq L9F30_4\n\t"
        "ldrb r0, [r3, #3]\n\t"
        "cmp r0, #0xff\n\t"
        "beq L9F30_4\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r3, r0\n\t"
        "ldrh r0, [r0, #4]\n\t"
        "cmp r4, r0\n\t"
        "bne L9F30_4\n\t"
        "ldrb r0, [r3, #2]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r3, r0\n\t"
        "ldrh r0, [r0, #4]\n\t"
        "strh r0, [r6]\n\t"
        "L9F30_4:\n\t"
        "mov r1, #0\n\t"
        "ldrh r0, [r3, #4]\n\t"
        "cmp r0, r4\n\t"
        "bhs L9F30_5\n\t"
        "add r2, r3, #0\n\t"
        "L9F30_6:\n\t"
        "add r2, #8\n\t"
        "add r1, #1\n\t"
        "ldrh r0, [r2, #4]\n\t"
        "cmp r0, r4\n\t"
        "blo L9F30_6\n\t"
        "L9F30_5:\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r3, r0\n\t"
        "ldrh r2, [r0, #4]\n\t"
        "cmp r4, r2\n\t"
        "beq L9F30_8\n\t"
        "mov r5, #6\n\t"
        "ldrsh r2, [r0, r5]\n\t"
        "sub r1, #1\n\t"
        "lsl r1, r1, #3\n\t"
        "add r1, r3, r1\n\t"
        "ldrh r0, [r1, #4]\n\t"
        "sub r0, r4, r0\n\t"
        "mul r0, r2, r0\n\t"
        "asr r0, r0, #8\n\t"
        "ldrb r1, [r1, #8]\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "b L9F30_7\n\t"
        ".align 2, 0\n\t"
        "L9F30_9: .4byte 0x8ad0\n\t"
        "L9F30_8:\n\t"
        "ldrb r0, [r0, #8]\n\t"
        "L9F30_7:\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}
#endif /* NON_MATCHING */
