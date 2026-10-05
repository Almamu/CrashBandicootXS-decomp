#include "core.h"
#include "text.h"

/* GitHub issue #46: whole-string draw and fixed-count measure for the HUD
 * icon/text widget. Built with old_agbcc, which FontMeasureChars needs. */

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);

/* Same per-character logic as FontPutChar (asm/code_3_2_20_85c4.s),
 * inlined into a loop over a NUL-terminated string instead of
 * dispatching through the trampoline for each character - this widget
 * family's "draw this whole string" entry point. Unlike FontPutChar,
 * `&posX`/`&posY` are cached once outside the loop (`r6`/`r7` in the
 * ROM) rather than recomputed per character, and there's no shared
 * "dest += offset" tail - each arm does its own complete
 * address/load/add/store.
 *
 * Needed explicit register pins (`self`=r4, `str`=r5, the cached
 * `&posX`/`&posY`=r6/r7) and a `goto`-based rewrite of the
 * if/else-if/else to reproduce the ROM's exact block order (newline
 * tested first with a forward `beq`, space inline as the fallthrough,
 * dispatch last) - the same techniques already established for
 * FontPutChar's identical-shaped dispatcher. */
void FontDrawText(struct bitmap_font *selfArg, u8 *strArg)
{
    register struct bitmap_font *self asm("r4") = selfArg;
    register u8 *str asm("r5") = strArg;
    u8 c = *str;

    if (c == 0) {
        return;
    }

    {
        register u32 *posXAddr asm("r6") = &self->posX;
        /* NOT pinned to r7 - see docs/matching.md's "Why not just pin
         * r7" (an explicit r7 pin silently drops it from push/pop,
         * corrupting the caller's r7). gcc's own unforced allocator
         * picks r7 for this anyway, since it's the only register left. */
        u32 *posYAddr = &self->posY;

    loop:
        if (c == '\n') {
            goto newline;
        }
        if (c != ' ') {
            goto dispatch;
        }
        *posXAddr += self->spaceWidth;
        goto tail;
    newline:
        *posXAddr = self->marginX;
        *posYAddr += self->lineHeight;
        goto tail;
    dispatch:
        {
            struct icon_slot *slot = &self->record->slots[4];
            _call_via_r2((u8 *)self + slot->offset, c, slot->ptr);
        }
    tail:
        str++;
        c = *str;
        if (c != 0) {
            goto loop;
        }
    }
}
/* Ends 2 bytes short of a 4-byte boundary; the ROM zero-pads the gap,
 * this compiler's own trailing alignment fill doesn't - see
 * docs/matching.md's alignment-padding gotcha (also documented in
 * docs/matching/issue-46-hud-icon-widget.md's "Real gotchas" section). */
asm(".align 2, 0");

/* Sums the advance width of `count` characters starting at `str`
 * (`FontMeasureText`'s fixed-count sibling): newline contributes nothing,
 * space contributes `spaceWidth`, everything else contributes
 * `glyphRecords[charLookup[c]].width`. */
s32 FontMeasureChars(struct bitmap_font *self, u8 *str, s32 count)
{
    s32 total = 0;
    s32 i;

    for (i = 0; i < count; i++)
    {
        u32 c = str[i];

        switch (c)
        {
        case ' ':
            total += self->spaceWidth;
            break;
        case '\n':
            break;
        default:
            total += self->glyphRecords[self->charLookup[c]].width;
            break;
        }
    }
    return total;
}
/* Zero-fill the trailing halfword, as the ROM does. */
asm(".align 2, 0");
