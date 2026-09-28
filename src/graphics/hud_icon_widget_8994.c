#include "core.h"
#include "icon_manager.h"

/* GitHub issue #46. Built with old_agbcc, which MeasureText needs. */

/* Width of the widest line of `text`: sums each line's advance widths
 * (space = `spaceWidth`, other characters = their glyph's width),
 * resetting at each newline and keeping the running maximum. */
s32 MeasureText(struct icon_manager *self, u8 *text)
{
    u32 maxWidth = 0;
    u32 cur = 0;
    u8 c;

    for (; (c = *text) != 0; text++)
    {
        switch (c)
        {
        case ' ':
            cur += self->spaceWidth;
            break;
        case '\n':
            if (cur > maxWidth)
                maxWidth = cur;
            cur = 0;
            break;
        default:
            cur += self->glyphRecords[self->charLookup[c]].width;
            break;
        }
    }
    return cur < maxWidth ? maxWidth : cur;
}
