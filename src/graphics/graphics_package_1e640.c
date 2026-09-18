#include "core.h"

/* Reads back the packed-BG-control halfword (bits later written by
 * `sub_801E644`'s `self+0xc`/`self+0xd` pair) from the small graphics-
 * package "self" scratch buffer that `LoadGraphicsPackage`'s callers
 * build up before calling it (see `sub_80374D0` in
 * counter_selector_setup.c: `u8 buf[0x10]; sub_801E644(buf, ...);
 * LoadGraphicsPackage(buf, ...); REG_BG0CNT = sub_801E640(buf);` - the
 * buffer is treated as a raw byte scratch area by every caller, never a
 * named struct, so it stays `u8 *` here too). */
u16 sub_801E640(u8 *self)
{
    return *(u16 *)(self + 0xc);
}

/* Writes the same "self" scratch buffer's first four fields: three raw
 * 32-bit values at +0x00/+0x04/+0x08 (`arg1`/`arg2`/`arg3` verbatim -
 * `sub_80374D0` calls this as `sub_801E644(buf, 2, 0x1e, 1, 3)`, so
 * these look like a level/category index, a BG control value, and a
 * priority/slot index, though the exact meaning isn't pinned down
 * beyond their offsets) plus two packed sub-byte fields: byte +0xc's
 * low nibble is `(arg5 & 3) | ((arg1 & 3) << 2)`, byte +0xd is
 * `arg2 & 0x1f`.
 *
 * Written as NAKED asm, not plain C: every instruction's operation was
 * already confirmed against the ROM, but gcc 2.9's register allocation
 * around the repeated `& 3`/`-0xd` masks (and one extra callee-saved
 * `r7` in the ROM's push/pop list) never reproduced exactly - see
 * docs/matching/issue-30-graphics-loading.md. Transcribed
 * instruction-for-instruction from the ROM disassembly instead, the
 * same escape hatch used for `sub_8001CB8`/`sub_8001DB4`
 * (src/system/link_cable.c). */
NAKED void sub_801E644(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg5)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "ldr r5, [sp, #0x14]\n\t"
        "mov r4, #0\n\t"
        "strh r4, [r0, #0xc]\n\t"
        "mov r6, #3\n\t"
        "and r5, r6\n\t"
        "sub r4, #4\n\t"
        "ldrb r7, [r0, #0xc]\n\t"
        "and r4, r7\n\t"
        "orr r4, r5\n\t"
        "str r1, [r0]\n\t"
        "and r1, r6\n\t"
        "lsl r1, r1, #2\n\t"
        "mov r5, #0xd\n\t"
        "neg r5, r5\n\t"
        "and r4, r5\n\t"
        "orr r4, r1\n\t"
        "strb r4, [r0, #0xc]\n\t"
        "mov r1, #0x3f\n\t"
        "ldrb r4, [r0, #0xd]\n\t"
        "and r1, r4\n\t"
        "str r2, [r0, #4]\n\t"
        "mov r4, #0x1f\n\t"
        "and r2, r4\n\t"
        "mov r4, #0x20\n\t"
        "neg r4, r4\n\t"
        "and r1, r4\n\t"
        "orr r1, r2\n\t"
        "strb r1, [r0, #0xd]\n\t"
        "str r3, [r0, #8]\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
