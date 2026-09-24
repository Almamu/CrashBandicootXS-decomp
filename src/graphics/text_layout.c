#include "core.h"
#include "icon_manager.h"

/* Sits right after InitBresenhamLine (ROM 0x08000E6C, in src/util/line_util.c) and
 * before FormatCentiseconds (still raw in asm/code_3_1_3.s). */

/* A text-layout/word-wrap renderer: walks a NUL-terminated string one
 * "token" at a time (`GetWordLength` returns each token's byte length -
 * looks like it splits on word boundaries), drawing each token through
 * the OAM-icon system (`sub_803AD84`, returning the token's pixel
 * width; a second call at a different record slot appears to draw a
 * cursor/highlight) while accumulating a running pixel width against a
 * per-line budget (`box->field_8`). When the running width would
 * overflow, it advances to a new "line" (drawing a newline marker via
 * `sub_803AD80` at a third record slot, and re-drawing the
 * just-measured token at the line's start) and optionally "flushes"
 * (`sub_80006A8` then `sub_8006AAC(gUnknown_03001300)` - the same
 * OAM-shadow-buffer flush pattern used elsewhere) depending on a
 * `mode` parameter (0 = never flush per-token, 1 = flush after every
 * token, 2 = only flush after a line wrap) - and unconditionally
 * flushes once more after the whole string is consumed if `mode != 0`.
 * Recognizes two escape sequences, `/b` (nudge the render Y position
 * down by 4, a half-line break) and `/n` (full newline - same drawing
 * as an overflow-driven line advance), both introduced by a literal
 * `/` byte in the text.
 *
 * `box` (3rd parameter) has only field_0/field_4 (the starting X/Y,
 * copied into the render-target object's 0x110/0x114 fields before the
 * loop starts) and field_8 (the per-line pixel-width budget) used; the
 * render-target object (2nd parameter, `self`) reads 3 different
 * 8-byte record-array entries (+0x18/+0x28/+0x38, 0x10 bytes apart,
 * `{s16, pad, void *}`) off `self->0x130` depending on what it's
 * drawing - see `sub_8006770` (`src/graphics/oam_count.c`) for the
 * same record-array shape.
 *
 * Written as NAKED asm, not plain C. A close (99.89% instruction match)
 * C reconstruction is kept below under `#if NON_MATCHING` - see
 * docs/matching/naked-sub_8000ee4-progress.md for the full derivation
 * of everything it gets right (including two gaps this function's
 * original NAKED write-up believed were unfixable: the `self`/`cursor`
 * vs `box`/`limit` argument-spill ordering, and a pair of pointer
 * locals the ROM keeps live in a register and spills to the stack only
 * at the last possible moment, both closed with techniques documented
 * there) and the two small, isolated register-choice residuals still
 * open (a `/b`-handler pointer reload, and the final `mode != 0`
 * epilogue check) - every attempt to pin either one directly regressed
 * *other*, already-matching code elsewhere in the function, suggesting
 * they're downstream of the same local-allocator pass rather than
 * independently fixable. Every instruction in the NAKED block below is
 * confirmed byte-identical to the ROM - full NAKED transcription, like
 * this project's other hard-compiler-limitation cases
 * (`src/util/math_div_util.c`'s `nullsub_8`, `src/system/link_cable.c`'s
 * `sub_8001CB8`/`sub_8001DB4`, `src/util/printf_util.c`'s
 * `sub_8000CBC`), remains the byte-exact answer while the last two
 * gaps stay open. */
struct sub_8000EE4_box {
    s32 field_0;
    s32 field_4;
    s32 field_8;
};

extern void sub_8006A90(void *arg0);
extern void sub_8006A48(void *arg0);
extern s32 GetWordLength(u8 *cursor);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, u8 *arg1, s32 arg2, void *arg3);
extern void sub_80006A8(void);
extern void sub_8006AAC(void *arg0);
extern void *gUnknown_03001300;

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - 99.89% instruction match (only two small,
 * isolated register-choice residuals left - see the doc comment above
 * and docs/matching/naked-sub_8000ee4-progress.md); compiled only
 * under `make NON_MATCHING=1`, the NAKED version below is used
 * otherwise. */
s32 sub_8000EE4(u8 *textParam, struct icon_manager *selfParam, struct sub_8000EE4_box *boxParam, s32 limitParam, s32 mode)
{
    register u8 *cursor asm("r10");
    register struct icon_manager *self asm("r8");
    struct sub_8000EE4_box * volatile box;
    s32 volatile limit;
    s32 volatile widthAccum;
    s32 volatile lineCount;
    s32 volatile posAccum;
    register u32 *xAddrEarly asm("r3");
    register u32 *yAddrEarly asm("r1");
    u32 * volatile xAddr;
    u32 * volatile yAddr;
    register u8 *token asm("r6");
    s32 tokenLen;
    register struct icon_record **fieldAddr asm("r4");
    struct icon_record *record;
    register s32 charWidth asm("r9");
    s32 combined;

    asm volatile("mov %0, %1" : "=r"(cursor) : "r"(textParam));
    asm volatile("mov %0, %1" : "=r"(self) : "r"(selfParam));
    box = boxParam;
    limit = limitParam;
    { register s32 zero asm("r0") = 0; posAccum = zero; }

    { register s32 modeCheck asm("r1");
      asm volatile("ldr %0, %1" : "=r"(modeCheck) : "m"(mode));
      if (modeCheck != 0) {
        sub_8006A90(gUnknown_03001300);
        sub_8006A48(gUnknown_03001300);
    } }

    {
        register struct sub_8000EE4_box *boxPtr asm("r2") = box;
        s32 boxX = boxPtr->field_0;
        s32 boxY = boxPtr->field_4;
        xAddrEarly = &self->posX;
        *xAddrEarly = boxX;
        yAddrEarly = &self->posY;
        *yAddrEarly = boxY;
    }

    widthAccum = 0;
    { register s32 zero2 asm("r2") = 0; lineCount = zero2; }

    {
        register u8 *cursorCopy asm("r2") = cursor;
        u8 firstChar = *cursorCopy;
        yAddr = yAddrEarly;
        if (firstChar == 0) {
            goto end;
        }
    }
    { register s32 posAccumCk asm("r0") = posAccum;
      register s32 limitCk asm("r1") = limit;
      if (posAccumCk >= limitCk) {
          goto end;
      }
    }
    xAddr = xAddrEarly;

loopTop:
    tokenLen = GetWordLength(cursor);
    token = cursor;
    { register u8 *cursorTmp asm("r2");
      asm volatile("add %0, %1, %2" : "=r"(cursorTmp) : "r"(token), "r"(tokenLen));
      cursor = cursorTmp; }

    if (*token != '/') {
        goto normalChar;
    }

    {
        u8 c;
        asm volatile("add %0, %0, #1" : "+r"(token));
        c = *token;
        if (c == 'b') {
            goto handleB;
        } else if (c == 'n') {
            goto handleN;
        } else {
            goto incrementTail;
        }
    handleB:
        {
            u32 *xAddr2, *yAddr2;
            s32 x, y;
            asm volatile("ldr %0, %1" : "=r"(xAddr2) : "m"(xAddr));
            x = *xAddr2;
            asm volatile("ldr %0, %1" : "=r"(yAddr2) : "m"(yAddr));
            y = *yAddr2;
            y += 4;
            asm volatile("ldr %0, %1" : "=r"(xAddr2) : "m"(xAddr));
            *xAddr2 = x;
            asm volatile("ldr %0, %1" : "=r"(yAddr2) : "m"(yAddr));
            *yAddr2 = y;
        }
    handleN:
        ;
    }

    record = self->record;
    sub_803AD80((u8 *)self + record->slots[5].offset, 10, record->slots[5].ptr);
    widthAccum = 0;
    { register s32 lcN asm("r1") = lineCount;
      asm volatile("add %0, %0, #1" : "+r"(lcN));
      lineCount = lcN; }
incrementTail:
    { register s32 posAccumR asm("r2") = posAccum;
      asm volatile("add %0, %0, %1" : "+r"(posAccumR) : "r"(tokenLen));
      posAccum = posAccumR; }
    goto bottom1;

normalChar:
    fieldAddr = &self->record;
    record = *fieldAddr;
    charWidth = sub_803AD84((u8 *)self + record->slots[1].offset, token, tokenLen, record->slots[1].ptr);
    combined = widthAccum;
    asm volatile("add %0, %1" : "+r"(combined) : "r"(charWidth));
    { register struct sub_8000EE4_box *boxPtr2 asm("r1") = box;
    if (combined <= boxPtr2->field_8) {
        record = *fieldAddr;
        sub_803AD84((u8 *)self + record->slots[3].offset, token, tokenLen, record->slots[3].ptr);
        widthAccum = combined;
        if (mode != 1) {
            goto bottom2;
        }
        goto flush;
    } else {
        { register s32 lc asm("r2") = lineCount;
          asm volatile("add %0, %0, #1" : "+r"(lc));
          lineCount = lc;
          if (lc >= limit) {
              goto bottom1;
          }
        }
        record = *fieldAddr;
        sub_803AD80((u8 *)self + record->slots[5].offset, 10, record->slots[5].ptr);
        record = *fieldAddr;
        sub_803AD84((u8 *)self + record->slots[3].offset, token, tokenLen, record->slots[3].ptr);
        widthAccum = charWidth;
        if (mode == 1 || mode == 2) {
            goto flush;
        }
        goto bottom2;
    } }

flush:
    sub_80006A8();
    sub_8006AAC(gUnknown_03001300);

bottom2:
    { register s32 posAccumR asm("r1") = posAccum;
      asm volatile("add %0, %0, %1" : "+r"(posAccumR) : "r"(tokenLen));
      posAccum = posAccumR; }

bottom1:
    if (*token == 0) {
        goto end;
    }
    { register s32 lineCountCk asm("r2") = lineCount;
      register s32 limitCk2 asm("r0") = limit;
      if (lineCountCk < limitCk2) {
          goto loopTop;
      }
    }

end:
    if (mode != 0) {
        sub_80006A8();
        sub_8006AAC(gUnknown_03001300);
    }
    return posAccum;
}
#else /* !NON_MATCHING */
NAKED s32 sub_8000EE4(u8 *textParam, struct icon_manager *selfParam, void *box, s32 limit, s32 mode)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x1c\n\t"
        "mov sl, r0\n\t"
        "mov r8, r1\n\t"
        "str r2, [sp]\n\t"
        "str r3, [sp, #4]\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #0x10]\n\t"
        "ldr r1, [sp, #0x3c]\n\t"
        "cmp r1, #0\n\t"
        "beq 1f\n\t"
        "ldr r4, 5f\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A90\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A48\n\t"
    "1:\n\t"
        "ldr r2, [sp]\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r2, [r2, #4]\n\t"
        "mov r3, #0x88\n\t"
        "lsl r3, r3, #1\n\t"
        "add r3, r8\n\t"
        "str r0, [r3]\n\t"
        "mov r1, #0x8a\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r8\n\t"
        "str r2, [r1]\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #8]\n\t"
        "mov r2, #0\n\t"
        "str r2, [sp, #0xc]\n\t"
        "mov r2, sl\n\t"
        "ldrb r0, [r2]\n\t"
        "str r1, [sp, #0x18]\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "b 14f\n\t"
    "2:\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "ldr r1, [sp, #4]\n\t"
        "cmp r0, r1\n\t"
        "blt 3f\n\t"
        "b 14f\n\t"
    "3:\n\t"
        "str r3, [sp, #0x14]\n\t"
    "4:\n\t"
        "mov r0, sl\n\t"
        "bl GetWordLength\n\t"
        "add r7, r0, #0\n\t"
        "mov r6, sl\n\t"
        "add r2, r6, r7\n\t"
        "mov sl, r2\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #0x2f\n\t"
        "bne 9f\n\t"
        "add r6, #1\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #0x62\n\t"
        "beq 6f\n\t"
        "cmp r0, #0x6e\n\t"
        "beq 7f\n\t"
        "b 8f\n\t"
        ".align 2, 0\n\t"
    "5: .4byte gUnknown_03001300\n\t"
    "6:\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #4\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "str r1, [r2]\n\t"
        "ldr r1, [sp, #0x18]\n\t"
        "str r0, [r1]\n\t"
    "7:\n\t"
        "mov r0, #0x98\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r8\n\t"
        "ldr r1, [r0]\n\t"
        "mov r2, #0x38\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r2, [r1, #0x3c]\n\t"
        "mov r1, #0xa\n\t"
        "bl sub_803AD80\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "add r1, #1\n\t"
        "str r1, [sp, #0xc]\n\t"
    "8:\n\t"
        "ldr r2, [sp, #0x10]\n\t"
        "add r2, r2, r7\n\t"
        "str r2, [sp, #0x10]\n\t"
        "b 13f\n\t"
    "9:\n\t"
        "mov r4, #0x98\n\t"
        "lsl r4, r4, #1\n\t"
        "add r4, r8\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x18\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r3, [r1, #0x1c]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_803AD84\n\t"
        "mov sb, r0\n\t"
        "ldr r5, [sp, #8]\n\t"
        "add r5, sb\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "cmp r5, r0\n\t"
        "bgt 10f\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x28\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r3, [r1, #0x2c]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_803AD84\n\t"
        "str r5, [sp, #8]\n\t"
        "ldr r0, [sp, #0x3c]\n\t"
        "cmp r0, #1\n\t"
        "bne 12f\n\t"
        "b 11f\n\t"
    "10:\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r2, #1\n\t"
        "str r2, [sp, #0xc]\n\t"
        "ldr r0, [sp, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 13f\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x38\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r2, [r1, #0x3c]\n\t"
        "mov r1, #0xa\n\t"
        "bl sub_803AD80\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x28\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r3, [r1, #0x2c]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_803AD84\n\t"
        "mov r0, sb\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [sp, #0x3c]\n\t"
        "sub r0, #1\n\t"
        "cmp r0, #1\n\t"
        "bhi 12f\n\t"
    "11:\n\t"
        "bl sub_80006A8\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006AAC\n\t"
    "12:\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "add r1, r1, r7\n\t"
        "str r1, [sp, #0x10]\n\t"
    "13:\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #0\n\t"
        "beq 14f\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "ldr r0, [sp, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 14f\n\t"
        "b 4b\n\t"
    "14:\n\t"
        "ldr r1, [sp, #0x3c]\n\t"
        "cmp r1, #0\n\t"
        "beq 15f\n\t"
        "bl sub_80006A8\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006AAC\n\t"
    "15:\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "add sp, #0x1c\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n\t"
    "16: .4byte gUnknown_03001300\n\t"
    );
}
#endif /* NON_MATCHING */
