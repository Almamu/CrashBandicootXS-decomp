#include "core.h"

/* Per-tick ping-pong position sweep (issue #68, docs/status/audio.md's
 * "many-register r8" group). `self+0x12` gates on "armed"; `self+0x14`
 * is a per-tick countdown that, once it hits 0, reloads from a
 * columnar table (`self+0x3c`, indexed by `self+0x10` with a 28-byte
 * stride - the table isn't one packed struct: the five columns this
 * function touches sit at fixed offsets 0x14/0x18/0x1c/0x20/0x24 from
 * the table base, each strided 28 bytes per index, so what occupies
 * the other bytes of each 28-byte row isn't touched here and stays
 * unmodeled, same as the rest of this GAX2 cluster) and recomputes
 * `self+0x48` (a signed position accumulator) by stepping it toward
 * one of two bounds from the table row, flipping `self+0x13`'s sign
 * once a bound is crossed (to 0xff/1, i.e. reverse direction), then
 * folds the position delta (shifted by 11) into `self+0x44`.
 *
 * Written as NAKED asm, not plain C: an 81.3%-matching C
 * reconstruction is kept below under `#if NON_MATCHING` - see
 * docs/matching/naked-sub_803a03c-matched.md for the derivation and
 * the residual gaps (register-choice-only diffs, plus one genuine
 * instruction-selection difference in the repeated `tablePtr+0x20`
 * columnar-table address that no C phrasing tried reproduced without
 * regressing elsewhere - consistent with the many-register GAX2
 * allocation-heuristic sensitivity this ROM region already documents,
 * not an easy phrasing gap). Mechanical, byte-verified transcription
 * of the ROM's own instructions (translated from the disassembler's
 * unified syntax to this project's established NAKED plain/divided
 * syntax, local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess. */
#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/naked-sub_803a03c-matched.md
 * for the instruction match and the residual register-choice gaps;
 * compiled only under `make NON_MATCHING=1`, the NAKED version below is
 * used otherwise. */
void sub_803A03C(void *self)
{
    register u8 *s asm("r2") = (u8 *)self;
    register s32 zero8 asm("r8");
    register u8 *tablePtr asm("ip");
    s32 posAccum;
    s32 newPos;
    s32 accum44;
    s32 countdown;

    if (!s[0x12]) {
        return;
    }

    countdown = s[0x14] - 1;
    s[0x14] = countdown;
    zero8 = 0xff;
    if ((u8)countdown != 0) {
        return;
    }

    posAccum = *(s32 *)(s + 0x48);
    tablePtr = *(u8 **)(s + 0x3c);
    *(u8 *)(s + 0x14) = *(u16 *)(tablePtr + s[0x10] * 28 + 0x24);

    {
        u8 oldDir = s[0x13];
        s8 dirSigned = (s8)s[0x13];

        if (dirSigned > 0) {
            newPos = posAccum + *(s32 *)((tablePtr + 0x20) + s[0x10] * 28);
            *(s32 *)(s + 0x48) = newPos;
            if (newPos + *(s32 *)((tablePtr + 0x1c) + s[0x10] * 28) > *(s32 *)((tablePtr + 0x18) + s[0x10] * 28)) {
                newPos = newPos - *(s32 *)((tablePtr + 0x20) + s[0x10] * 28) * 2;
                *(s32 *)(s + 0x48) = newPos;
                s[0x13] = (u8)(zero8 | oldDir);
            }
        } else {
            newPos = posAccum - *(s32 *)((tablePtr + 0x20) + s[0x10] * 28);
            *(s32 *)(s + 0x48) = newPos;
            if (newPos < *(s32 *)((tablePtr + 0x14) + s[0x10] * 28)) {
                newPos = newPos + *(s32 *)((tablePtr + 0x20) + s[0x10] * 28) * 2;
                *(s32 *)(s + 0x48) = newPos;
                s[0x13] = 1;
            }
        }
    }

    accum44 = *(s32 *)(s + 0x44) - (posAccum << 11);
    accum44 = accum44 + (*(s32 *)(s + 0x48) << 11);
    *(s32 *)(s + 0x44) = accum44;
}
#else /* !NON_MATCHING */
NAKED void sub_803A03C(void *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "add r2, r0, #0\n\t"
        "ldrb r0, [r2, #0x12]\n\t"
        "cmp r0, #0\n\t"
        "beq 9f\n\t"
        "ldrb r0, [r2, #0x14]\n\t"
        "sub r0, r0, #1\n\t"
        "strb r0, [r2, #0x14]\n\t"
        "mov r1, #0xff\n\t"
        "mov r8, r1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 9f\n\t"
        "ldr r6, [r2, #0x48]\n\t"
        "ldr r0, [r2, #0x3c]\n\t"
        "mov ip, r0\n\t"
        "ldrb r1, [r2, #0x10]\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, ip\n\t"
        "ldrh r0, [r0, #0x24]\n\t"
        "strb r0, [r2, #0x14]\n\t"
        "ldrb r7, [r2, #0x13]\n\t"
        "mov r0, #0x13\n\t"
        "ldrsb r0, [r2, r0]\n\t"
        "cmp r0, #0\n\t"
        "ble 2f\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "mov r5, ip\n\t"
        "add r5, r5, #0x20\n\t"
        "add r0, r5, r0\n\t"
        "ldr r0, [r0]\n\t"
        "add r4, r6, r0\n\t"
        "str r4, [r2, #0x48]\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r3, r0, #2\n\t"
        "mov r0, ip\n\t"
        "add r0, r0, #0x1c\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r0]\n\t"
        "add r1, r4, r1\n\t"
        "mov r0, ip\n\t"
        "add r0, r0, #0x18\n\t"
        "add r0, r0, r3\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r1, r0\n\t"
        "ble 4f\n\t"
        "add r0, r5, r3\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r0, r0, #1\n\t"
        "sub r0, r4, r0\n\t"
        "str r0, [r2, #0x48]\n\t"
        "mov r0, r8\n\t"
        "orr r0, r0, r7\n\t"
        "b 3f\n\t"
    "2:\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "mov r4, ip\n\t"
        "add r4, r4, #0x20\n\t"
        "add r0, r4, r0\n\t"
        "ldr r0, [r0]\n\t"
        "sub r3, r6, r0\n\t"
        "str r3, [r2, #0x48]\n\t"
        "ldrb r1, [r2, #0x10]\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r1, r0, #2\n\t"
        "mov r0, ip\n\t"
        "add r0, r0, #0x14\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bge 4f\n\t"
        "add r0, r4, r1\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r3, r0\n\t"
        "str r0, [r2, #0x48]\n\t"
        "mov r0, #1\n\t"
    "3:\n\t"
        "strb r0, [r2, #0x13]\n\t"
    "4:\n\t"
        "lsl r0, r6, #0xb\n\t"
        "ldr r1, [r2, #0x44]\n\t"
        "sub r1, r1, r0\n\t"
        "ldr r0, [r2, #0x48]\n\t"
        "lsl r0, r0, #0xb\n\t"
        "add r1, r1, r0\n\t"
        "str r1, [r2, #0x44]\n\t"
    "9:\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n\t"
    );
}
#endif /* NON_MATCHING */
