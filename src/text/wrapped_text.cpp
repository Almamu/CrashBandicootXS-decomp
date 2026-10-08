#include "font.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "text.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
}

/* Sits right after InitBresenhamLine (ROM 0x08000E6C, in src/util/line.c) and
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
 * recomputes the address. */
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
        ResetOamBuffer(gOamBuffer);
        HideUnusedOamEntries(gOamBuffer);
    }
    self->SetPos(box->x, box->y);
    widthAccum = 0;
    lineCount = 0;
    token = text;
    while (*token != 0 && lineCount < limit) {
        len = GetWordLength(text);
        /* Emits nothing; the extra reference raises `len`'s allocation
         * priority so it gets r7 ahead of `self` (r8) and `charWidth`
         * (r9), as in the ROM. */
        MATCH_USE(len);
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
                /* ROM order: `bne skip; b flush`. */
                if (mode != 1)
                    goto skip;
                goto flush;
            } else {
                {
                    /* Emits nothing; keeping r1 live here moves the
                     * spilled lineCount's reload to r2 and the limit's
                     * to r0, as in the ROM (hard-register hold, #489). */
                    MATCH_HOLD_REG(s32, hold, r1);
                    MATCH_HOLD(hold);
                    lineCount++;
                    MATCH_USE(hold);
                }
                if (lineCount >= limit)
                    continue;
                self->PutChar('\n');
                self->DrawChars(token, len);
                widthAccum = charWidth;
                if (mode == 1 || mode == 2) {
                flush:
                    WaitForVBlank();
                    CommitOamBuffer(gOamBuffer);
                }
            }
        skip:
            posAccum += len;
        }
    }
    if (mode != 0) {
        WaitForVBlank();
        CommitOamBuffer(gOamBuffer);
    }
    return posAccum;
}
