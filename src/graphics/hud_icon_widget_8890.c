#include "core.h"
#include "icon_manager.h"

/* NOT YET BYTE-MATCHING - see docs/matching/issue-46-hud-icon-widget.md
 * for the full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_2_20_8890.s) is used otherwise. */
#if NON_MATCHING

extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);

/* Same per-character logic as sub_8028808 (asm/code_3_2_20_85c4.s),
 * inlined into a loop over a NUL-terminated string instead of
 * dispatching through the trampoline for each character - this widget
 * family's "draw this whole string" entry point.
 *
 * Residual gap: same if/else-if block-layout shape sub_8028808 hits -
 * see that function's own comment for what was tried. */
void sub_8028890(struct icon_manager *self, u8 *str)
{
    u8 c;

    for (c = *str; c != 0; c = *++str) {
        if (c == '\n') {
            self->posX = self->field_118;
            self->posY += self->field_11c;
        } else if (c == ' ') {
            self->posX += self->spaceWidth;
        } else {
            struct icon_slot *slot = &self->record->slots[4];
            sub_803AD80((u8 *)self + slot->offset, c, slot->ptr);
        }
    }
}

/* Sums the advance width of `count` characters starting at `str`
 * (`MeasureText`'s fixed-count sibling): newline contributes nothing,
 * space contributes `spaceWidth`, everything else contributes
 * `glyphRecords[charLookup[c]].width`.
 *
 * Residual gap: the ROM pins `str` and `self+0x10c` (`&glyphRecords`)
 * into `r8`/`sb`, spilling them across the loop's own `bl` calls; this
 * compiler keeps everything in r4-r7 instead - same class of gap as
 * sub_8006600/sub_8037388 elsewhere in this codebase (see
 * src/audio/counter_selector_setup.c's comment on the latter). */
s32 sub_8028900(struct icon_manager *self, u8 *str, s32 count)
{
    s32 total = 0;
    s32 i;

    for (i = 0; i < count; i++) {
        u8 c = str[i];

        if (c == '\n') {
            continue;
        }
        if (c == ' ') {
            total += self->spaceWidth;
        } else {
            u8 glyphIndex = self->charLookup[c];
            struct icon_glyph_metrics *rec =
                (struct icon_glyph_metrics *)((u8 *)self->glyphRecords + glyphIndex * 12);
            total += rec->width;
        }
    }
    return total;
}

#endif /* NON_MATCHING */
