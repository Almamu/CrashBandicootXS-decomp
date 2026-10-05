#include "core.h"
#include "bitmap_font.h"

/* Sits right after LoadBackgroundTileAndPalette (ROM 0x080011C0, in src/system/asset_util.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

/* Returns the length of the next "word" starting at `s`: the number of
 * characters up to and including the first space, or up to (but not
 * including) the NUL terminator if no space is found first. Used by
 * the still-parked DrawWrappedText (src/graphics/text_layout.c) to walk text one
 * token at a time. */
s32 GetWordLength(u8 *s)
{
    s32 len = 0;
    u8 c;

    c = *s;
    if (c == 0) {
        goto done;
    }
    len = 1;
    if (c == ' ') {
        goto done;
    }
    for (;;) {
        s++;
        c = *s;
        if (c == 0) {
            goto done;
        }
        len++;
        if (c == ' ') {
            goto done;
        }
    }
done:
    return len;
}

extern s32 __udivsi3(s32 value, s32 divisor);
extern s32 DrawWrappedText(u8 *text, struct bitmap_font *self, void *box, s32 limit, s32 mode);

struct wrapped_text_box_params {
    s32 field_0;
    u8 unused_04[8];
    s32 field_c;
};

/* Thin wrapper around the still-parked DrawWrappedText (src/graphics/text_layout.c):
 * stashes `params->field_0` into `self`'s own `marginX`, computes a
 * line-count limit as `params->field_c / self->lineHeight`, then
 * forwards to DrawWrappedText with that limit and returns its result
 * (unused by the one call site matched so far, in `src/graphics/oam_count.c`'s
 * still-parked `DrawPowerDialog`, but the ROM does actually propagate it -
 * confirmed by the epilogue needing r1, not r0, to restore the return
 * address, since r0 holds the forwarded value at that point). */
s32 DrawWrappedTextInBox(u8 *text, struct bitmap_font *self, struct wrapped_text_box_params *params, s32 mode)
{
    s32 limit;

    {
        s32 v = params->field_0;
        self->marginX = v;
    }
    limit = __udivsi3(params->field_c, self->lineHeight);
    return DrawWrappedText(text, self, params, limit, mode);
}
