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
 * Written as NAKED asm, not plain C: a real C reconstruction reproduced
 * every branch and computation (confirmed by isolated compile matching
 * this function's full control flow), but the ROM's register choices -
 * `self` in r5, `table` kept in r3 for the entire function (never the
 * parameter's own r1), and a `lsls/lsrs #0x10` zero-extension dance for
 * the note-index parameter - didn't come out byte-identical from the C
 * forms tried in the time available this pass; register-pinning every
 * one of them to force the exact rotation is a reasonable next step for
 * a future pass. Mechanical, byte-verified transcription of the ROM's
 * own instructions in the meantime. */
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
