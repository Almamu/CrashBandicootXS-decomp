#include "sprite_obj.hpp"
#include "font.hpp"

extern "C" {
#include "core.h"
#include "text.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
}

/* Sits right after InitBresenhamLine (ROM 0x08000E6C, in src/util/line.cpp) and
 * before FormatCentiseconds (still raw in asm/code_3_1_3.s). C++ since the
 * #664 cleanup: the font draws are Font's virtual methods (font.hpp),
 * which the C spelled out as `_call_via_r2`/`_call_via_r3` slot calls.
 *
 * Built with old_agbcc, now old_agbcp (Makefile OLD_AGBCC_OBJS): current
 * agbcc gets the `/b` handler's first hoisted-address reload in r0 where
 * the ROM has r2; the old compiler reproduces it. See
 * docs/matching/archive/strag3-naked-retry.md. */

/* A text-layout/word-wrap renderer: walks a NUL-terminated string one
 * "token" at a time (GetWordLength returns each token's byte length, up
 * to and including a space), measuring each token (MeasureChars) and
 * drawing it (DrawChars) while accumulating a running pixel width
 * against a per-line budget (`box->w`). When the running width would
 * overflow, it advances to a new line (PutChar('\n'), then re-draws the
 * just-measured token at the line's start) and optionally flushes
 * (WaitForVBlank then CommitOamBuffer) depending on `mode` (0 = never
 * flush per-token, 1 = flush after every token, 2 = only flush after a
 * line wrap) - and flushes once more after the whole string is consumed
 * if `mode != 0`. Recognizes two escape sequences, `/b` (nudge the
 * position down by 4, a half-line break) and `/n` (newline). Returns the
 * number of bytes consumed.
 *
 * The `/b` handler reads the position through GetX/GetY: their inline
 * `this` makes the reads use the same loop-hoisted `&posX`/`&posY` as
 * the stores (the ROM's two spill slots), where a plain `posX` read
 * recomputes the address.
 *
 * Each branch of the measured token ends with its own flush test and
 * `posAccum += len` (#662 round 4). jump2's cross-jumping merges the two
 * identical tails after reload (the ROM's `bne skip; b flush` from the
 * fitting token into the wrapped one's flush), but before that `len` has
 * the extra use: 16 loop-weighted references over 97 insns, so
 * global-alloc's floor_log2(refs) * refs / live length (0.66) ranks it
 * above `self` (17 over 141, 0.48) and it takes r7, as in the ROM. The
 * draft's shared tail (`goto skip`/`goto flush`) left `len` at 14 over
 * 91 (0.46) against `self`'s 0.51, and needed an extra `MATCH_USE(len)`
 * reference and an r1 hold at the wrap's `lineCount++` for the reload
 * rotation that followed. */
s32 DrawWrappedText(u8 *text, Font *self, struct aabb *box, s32 limit, s32 mode)
{
    s32 widthAccum;
    s32 lineCount;
    s32 posAccum;
    u8 *token;
    s32 len;
    s32 charWidth;
    s32 combined;

    posAccum = 0;
    if (mode != 0) {
        gOamBuffer->Reset();
        gOamBuffer->HideUnused();
    }
    self->SetPos(box->x, box->y);
    widthAccum = 0;
    lineCount = 0;
    token = text;
    while (*token != 0 && lineCount < limit) {
        len = GetWordLength(text);
        token = text;
        text = token + len;
        if (*token == '/') {
            token++;
            switch (*token) {
            case 'b':
                self->SetPos(self->GetX(), self->GetY() + 4);
                /* fallthrough */
            case 'n':
                self->PutChar('\n');
                widthAccum = 0;
                lineCount++;
            }
            posAccum += len;
        } else {
            charWidth = self->MeasureChars(token, len);
            combined = widthAccum + charWidth;
            if (combined <= box->w) {
                self->DrawChars(token, len);
                widthAccum = combined;
                if (mode == 1) {
                    WaitForVBlank();
                    gOamBuffer->Commit();
                }
                posAccum += len;
            } else {
                lineCount++;
                if (lineCount >= limit)
                    continue;
                self->PutChar('\n');
                self->DrawChars(token, len);
                widthAccum = charWidth;
                if (mode == 1 || mode == 2) {
                    WaitForVBlank();
                    gOamBuffer->Commit();
                }
                posAccum += len;
            }
        }
    }
    if (mode != 0) {
        WaitForVBlank();
        gOamBuffer->Commit();
    }
    return posAccum;
}
