#include "core.h"
#include "icon_manager.h"

/* Sits right after InitBresenhamLine (ROM 0x08000E6C, in src/util/line_util.c) and
 * before FormatCentiseconds (still raw in asm/code_3_1_3.s).
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): current agbcc gets the
 * `/b` handler's first hoisted-address reload in r0 where the ROM has r2;
 * old_agbcc reproduces it. See docs/matching/strag3-naked-retry.md. */

/* A text-layout/word-wrap renderer: walks a NUL-terminated string one
 * "token" at a time (`GetWordLength` returns each token's byte length -
 * looks like it splits on word boundaries), measuring each token with
 * the render target's `record->slots[1]` method and drawing it with
 * `slots[3]` (gcc 2.x virtual calls through `_call_via_r3` =
 * `_call_via_r3`) while accumulating a running pixel width against a
 * per-line budget (`box->field_8`). When the running width would
 * overflow, it advances to a new line (`slots[5]` with a '\n', then
 * re-draws the just-measured token at the line's start) and optionally
 * flushes (`WaitForVBlank` then `sub_8006AAC(gUnknown_03001300)`)
 * depending on `mode` (0 = never flush per-token, 1 = flush after every
 * token, 2 = only flush after a line wrap) - and flushes once more
 * after the whole string is consumed if `mode != 0`. Recognizes two
 * escape sequences, `/b` (nudge the render Y position down by 4, a
 * half-line break) and `/n` (newline). Returns the number of bytes
 * consumed. */
struct sub_8000EE4_box {
    s32 field_0;
    s32 field_4;
    s32 field_8;
};

extern void sub_8006A90(void *arg0);
extern void sub_8006A48(void *arg0);
extern s32 GetWordLength(u8 *cursor);
extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, u8 *arg1, s32 arg2, void *arg3);
extern void WaitForVBlank(void);
extern void sub_8006AAC(void *arg0);
extern void *gUnknown_03001300;

static inline void set_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Address accessors for the `/b` handler's reads: going through a
 * returned pointer makes the reads use the same loop-hoisted
 * `&self->posX`/`&self->posY` as the stores (the ROM's two spill
 * slots), where a plain `self->posX` read recomputes the address. */
static inline u32 *pos_x(struct icon_manager *m) { return &m->posX; }
static inline u32 *pos_y(struct icon_manager *m) { return &m->posY; }

s32 sub_8000EE4(u8 *text, struct icon_manager *self, struct sub_8000EE4_box *box, s32 limit, s32 mode)
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
        sub_8006A90(gUnknown_03001300);
        sub_8006A48(gUnknown_03001300);
    }
    set_pos(self, box->field_0, box->field_4);
    widthAccum = 0;
    lineCount = 0;
    token = text;
    while (*token != 0 && lineCount < limit) {
        len = GetWordLength(text);
        /* Emits nothing; the extra reference raises `len`'s allocation
         * priority so it gets r7 ahead of `self` (r8) and `charWidth`
         * (r9), as in the ROM. */
        asm("" : : "r"(len));
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
            if (combined <= box->field_8) {
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
                    register s32 hold asm("r1");
                    asm("" : "=r"(hold));
                    lineCount++;
                    asm("" : : "r"(hold));
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
                    sub_8006AAC(gUnknown_03001300);
                }
            }
        skip:
            posAccum += len;
        }
    }
    if (mode != 0) {
        WaitForVBlank();
        sub_8006AAC(gUnknown_03001300);
    }
    return posAccum;
}
