#include "sprite_obj.hpp"
#include "font.hpp"

extern "C" {
#include "core.h"
#include "match.h"
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
        gOamBuffer->Reset();
        gOamBuffer->HideUnused();
    }
    self->SetPos(box->x, box->y);
    widthAccum = 0;
    lineCount = 0;
    token = text;
    while (*token != 0 && lineCount < limit) {
        len = GetWordLength(text);
        /* Emits nothing; the extra reference raises `len`'s allocation
         * priority so it gets r7 ahead of `self` (r8) and `charWidth`
         * (r9), as in the ROM. #662 round 3: global-alloc's priority is
         * floor_log2(refs) * refs / live length; plain, `len` has 14
         * refs over 93 insns (3 * 14 / 93 = 0.45) against `self`'s 17
         * over 136 (4 * 17 / 136 = 0.50), and the asm's operand makes it
         * 16 (4 * 16 / 94 = 0.68). The ROM needs two more `len` refs or
         * two fewer `self` refs, which no spelling of the loop tried
         * (a `self` copy for the /b handler, GetX/GetY into locals,
         * `text += len`, local reorderings, -fno-* flags, agbcp) gives. */
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
                     * to r0, as in the ROM (hard-register hold, #489).
                     * #662 round 2: `++lineCount >= limit`, `limit <=
                     * lineCount`, moving the setup stores and the
                     * permuter (best C wraps every `*token` read in an
                     * inline function) don't replace it.
                     * #662 round 3: it is reload's spill-register
                     * rotation. lineCount and limit are spilled, and
                     * reload hands out r0-r3 round-robin: plain, the
                     * increment gets r1 and the limit r2; with r1 live
                     * the rotation starts one register on (r2, then r0
                     * with r3 busy), the ROM's. Without the hold the
                     * rest of the function also moves (the setup
                     * SetPos's &posY spill); under -fno-rerun-loop-opt
                     * only these two reloads differ, and no spelling of
                     * the wrap (`++lineCount`, `limit <= lineCount`, the
                     * draw nested in `if (lineCount < limit)`, an inline
                     * `*token` read, local orders) moves the rotation. */
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
                    gOamBuffer->Commit();
                }
            }
        skip:
            posAccum += len;
        }
    }
    if (mode != 0) {
        WaitForVBlank();
        gOamBuffer->Commit();
    }
    return posAccum;
}
