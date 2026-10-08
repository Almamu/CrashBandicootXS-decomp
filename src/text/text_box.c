#include "core.h"
#include "text.h"
#include <libgcc.h>

/* Sits right after LoadBackgroundTileAndPalette (ROM 0x080011C0, in src/system/asset.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

/* Returns the length of the next "word" starting at `s`: the number of
 * characters up to and including the first space, or up to (but not
 * including) the NUL terminator if no space is found first. Used by
 * DrawWrappedText (src/text/wrapped_text.cpp) to walk text one
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

/* Thin wrapper around DrawWrappedText (src/text/wrapped_text.cpp):
 * stashes `params->x` into `self`'s own `marginX`, computes a
 * line-count limit as `params->h / self->lineHeight`, then
 * forwards to DrawWrappedText with that limit and returns its result
 * (unused by the one call site matched so far, in `src/menus/power_dialog_draw.cpp`'s
 * still-parked `DrawPowerDialog`, but the ROM does actually propagate it -
 * confirmed by the epilogue needing r1, not r0, to restore the return
 * address, since r0 holds the forwarded value at that point). */
s32 DrawWrappedTextInBox(u8 *text, struct bitmap_font *self, struct aabb *params, s32 mode)
{
    s32 limit;

    {
        s32 v = params->x;
        self->marginX = v;
    }
    limit = __udivsi3(params->h, self->lineHeight);
    return DrawWrappedText(text, self, params, limit, mode);
}
