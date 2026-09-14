#include "core.h"
#include "icon_manager.h"

/* Sits right after InitBresenhamLine (ROM 0x08000E6C, in src/util/line_util.c) and
 * before FormatCentiseconds (still raw in asm/code_3_1_3.s). */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching.md, "Parked, not matched:
 * sub_8000EE4" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly (asm/code_3_1_3.s) is
 * used otherwise. Logic and almost all register allocation match the
 * ROM exactly (verified instruction-by-instruction against a direct
 * baserom.gba objdump); the residual ~8 bytes are two things: (1) the
 * ROM moves `self`/`cursor` into their pinned registers *before*
 * `box`/`limit` get spilled to their stack homes, but this reconstruction
 * always gets the opposite order regardless of statement order (`box`/
 * `limit` spill before any explicit register-pinned move runs) - looks
 * like a fixed agbcc behavior for incoming stack-homed arguments, not
 * something under the C source's control; (2) two loop-bound checks
 * (`posAccum >= limit` near the top, `lineCount < limit` at the bottom)
 * compile to a single inverted conditional branch here, but the ROM has
 * a redundant two-instruction "correct-sense compare, branch on true,
 * fall to an unconditional far branch" pair at the same spots - most
 * likely a Thumb conditional-branch-range (+-256 byte) artifact given
 * how far the "end" label sits, but not conclusively reproduced. A
 * `asm volatile("" : "+r"(posAccum))` barrier after zeroing `posAccum`
 * stops gcc from noticing `posAccum`/`lineCount` are both providably 0
 * at the top check and cross-jump-merging it into the bottom check's
 * code (a bigger, wrong-shape mismatch this barrier fixes) - the same
 * trick applied to `lineCount` at the bottom check reduces the byte
 * count further but introduces a new spurious store, so it's omitted
 * here pending a better fix. */
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

s32 sub_8000EE4(u8 *textParam, struct icon_manager *selfParam, struct sub_8000EE4_box *box, s32 limit, s32 mode)
{
    register struct icon_manager *self asm("r8");
    register u8 *cursor asm("r10");
    s32 widthAccum;
    s32 lineCount;
    s32 posAccum;
    u32 *xAddr;
    u32 *yAddr;
    register u8 *token asm("r6");
    s32 tokenLen;
    struct icon_record **fieldAddr;
    struct icon_record *record;
    register s32 charWidth asm("r9");
    s32 combined;

    cursor = textParam;
    self = selfParam;

    if (mode != 0) {
        sub_8006A90(gUnknown_03001300);
        sub_8006A48(gUnknown_03001300);
    }

    xAddr = &self->posX;
    yAddr = &self->posY;
    *xAddr = box->field_0;
    *yAddr = box->field_4;

    posAccum = 0;
    widthAccum = 0;
    lineCount = 0;
    asm volatile("" : "+r"(posAccum));

    if (*cursor == 0) {
        goto end;
    }
    if (posAccum >= limit) {
        goto end;
    }

loopTop:
    token = cursor;
    tokenLen = GetWordLength(cursor);
    cursor = token + tokenLen;

    if (*token != '/') {
        goto normalChar;
    }

    {
        u8 c;
        token++;
        c = *token;
        if (c == 'b') {
            s32 x = *xAddr;
            s32 y = *yAddr;
            y += 4;
            *xAddr = x;
            *yAddr = y;
        } else if (c != 'n') {
            goto bottom1;
        }
    }

    record = self->record;
    sub_803AD80((u8 *)self + record->slots[5].offset, 10, record->slots[5].ptr);
    widthAccum = 0;
    lineCount++;
    goto bottom1;

normalChar:
    fieldAddr = &self->record;
    record = *fieldAddr;
    charWidth = sub_803AD84((u8 *)self + record->slots[1].offset, token, tokenLen, record->slots[1].ptr);
    combined = widthAccum + charWidth;
    if (combined <= box->field_8) {
        record = *fieldAddr;
        sub_803AD84((u8 *)self + record->slots[3].offset, token, tokenLen, record->slots[3].ptr);
        widthAccum = combined;
        if (mode == 1) {
            goto flush;
        }
        goto bottom2;
    } else {
        lineCount++;
        if (lineCount >= limit) {
            goto bottom1;
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
    }

flush:
    sub_80006A8();
    sub_8006AAC(gUnknown_03001300);

bottom2:
    posAccum += tokenLen;

bottom1:
    if (*cursor == 0) {
        goto end;
    }
    if (lineCount < limit) {
        goto loopTop;
    }

end:
    if (mode != 0) {
        sub_80006A8();
        sub_8006AAC(gUnknown_03001300);
    }
    return posAccum;
}
#endif /* NON_MATCHING */
