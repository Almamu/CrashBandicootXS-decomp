#include "core.h"
#include "match.h"
#include "text.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"

/* Sits right after InitBresenhamLine (ROM 0x08000E6C, in src/util/line.c) and
 * before FormatCentiseconds (still raw in asm/code_3_1_3.s).
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): current agbcc gets the
 * `/b` handler's first hoisted-address reload in r0 where the ROM has r2;
 * old_agbcc reproduces it. See docs/matching/archive/strag3-naked-retry.md. */

/* A text-layout/word-wrap renderer: walks a NUL-terminated string one
 * "token" at a time (`GetWordLength` returns each token's byte length -
 * looks like it splits on word boundaries), measuring each token with
 * the render target's `record->slots[1]` method and drawing it with
 * `slots[3]` (gcc 2.x virtual calls through `_call_via_r3` =
 * `_call_via_r3`) while accumulating a running pixel width against a
 * per-line budget (`box->w`). When the running width would
 * overflow, it advances to a new line (`slots[5]` with a '\n', then
 * re-draws the just-measured token at the line's start) and optionally
 * flushes (`WaitForVBlank` then `CommitOamBuffer(gOamBuffer)`)
 * depending on `mode` (0 = never flush per-token, 1 = flush after every
 * token, 2 = only flush after a line wrap) - and flushes once more
 * after the whole string is consumed if `mode != 0`. Recognizes two
 * escape sequences, `/b` (nudge the render Y position down by 4, a
 * half-line break) and `/n` (newline). Returns the number of bytes
 * consumed. */
extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, u8 *arg1, s32 arg2, void *arg3);

static inline void set_pos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Address accessors for the `/b` handler's reads: going through a
 * returned pointer makes the reads use the same loop-hoisted
 * `&self->posX`/`&self->posY` as the stores (the ROM's two spill
 * slots), where a plain `self->posX` read recomputes the address. */
static inline u32 *pos_x(struct bitmap_font *m) { return &m->posX; }
static inline u32 *pos_y(struct bitmap_font *m) { return &m->posY; }

s32 DrawWrappedText(u8 *text, struct bitmap_font *self, struct aabb *box, s32 limit, s32 mode)
{
    s32 widthAccum;
    s32 lineCount;
    s32 posAccum;
    u8 *token;
    s32 len;
    s32 charWidth;
    s32 combined;
    struct icon_record *r;

    posAccum = 0;
    if (mode != 0) {
        ResetOamBuffer(gOamBuffer);
        HideUnusedOamEntries(gOamBuffer);
    }
    set_pos(self, box->x, box->y);
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
                set_pos(self, *pos_x(self), *pos_y(self) + 4);
                /* fallthrough */
            case 'n':
                r = self->record;
                _call_via_r2((u8 *)self + r->slots[5].offset, '\n', r->slots[5].ptr);
                widthAccum = 0;
                lineCount++;
            }
            posAccum += len;
        } else {
            r = self->record;
            charWidth = _call_via_r3((u8 *)self + r->slots[1].offset, token, len, r->slots[1].ptr);
            combined = widthAccum + charWidth;
            if (combined <= box->w) {
                r = self->record;
                _call_via_r3((u8 *)self + r->slots[3].offset, token, len, r->slots[3].ptr);
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
                r = self->record;
                _call_via_r2((u8 *)self + r->slots[5].offset, '\n', r->slots[5].ptr);
                r = self->record;
                _call_via_r3((u8 *)self + r->slots[3].offset, token, len, r->slots[3].ptr);
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
