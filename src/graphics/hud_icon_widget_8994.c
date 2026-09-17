#include "core.h"
#include "icon_manager.h"

/* NOT YET BYTE-MATCHING - see docs/matching/issue-46-hud-icon-widget.md
 * for the full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_2_20_8994.s) is used otherwise. */
#if NON_MATCHING

/* Widest single line of `text`: like sub_8028900 but walks a
 * NUL-terminated string and tracks a running per-line width, resetting
 * on each newline and keeping the running maximum; the final result is
 * whichever of the last line's width or the running maximum is larger
 * (covers un-terminated final lines).
 *
 * Residual gap: the ROM pins `self` into `ip` and the space-width field
 * address into `r8`, same class of register-pressure gap as
 * sub_8028900 (asm/code_3_2_20_8890.s) - see that function's own
 * comment. */
s32 MeasureText(struct icon_manager *self, u8 *text)
{
    s32 curWidth = 0;
    s32 maxWidth = 0;
    u8 c;

    for (c = *text; c != 0; c = *++text) {
        if (c == '\n') {
            if (curWidth > maxWidth) {
                maxWidth = curWidth;
            }
            curWidth = 0;
            continue;
        }
        if (c == ' ') {
            curWidth += self->spaceWidth;
        } else {
            u8 glyphIndex = self->charLookup[c];
            struct icon_glyph_metrics *rec =
                (struct icon_glyph_metrics *)((u8 *)self->glyphRecords + glyphIndex * 12);
            curWidth += rec->width;
        }
    }
    return curWidth >= maxWidth ? curWidth : maxWidth;
}

#endif /* NON_MATCHING */
