#include "core.h"
#include "hud.h"

/* Sits right after hud_blink.c's blink-timer trio (ROM 0x08028568) and
 * before FontDrawGlyph/InitSmallFont/InitLargeFont/FontPutChar
 * (src/graphics/hud_icon_widget_85c4.c) - see GitHub issue #46. Just
 * `DestroyHud` here: a `struct hud_counter`-parts destructor, unrelated
 * to the `struct bitmap_font` text/icon-glyph renderer the rest of this
 * chunk's functions operate on (see include/bitmap_font.h and the other
 * hud_icon_widget*.c files). */

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);
extern void OperatorDeleteArray(void *ptr);
extern void OperatorDelete(void *manager);

/* Minimal local copy of `struct icon_slot` (see include/bitmap_font.h)
 * - not itself an `bitmap_font`/`icon_record` object, but the same
 * generic {offset, ptr} trampoline-dispatch pair shape reused for a
 * `hud_digit_part`'s own per-type descriptor table. */
struct icon_slot {
    s16 offset;
    u8 unused_2[2];
    void *ptr;
};

/* Destructor for a `struct hud_counter`'s `parts` array (see
 * include/hud.h): walks the array back to front, invoking each
 * `hud_digit_part`'s own per-type descriptor's slot-8 (`table+0x50`)
 * teardown trampoline via `_call_via_r2`, frees the array itself
 * (`self->parts`, allocated with a leading element-count word per the
 * `[-4]` read below - see the same convention in src/graphics/
 * hud_icon_slot.c/actor files), then optionally frees `self` when
 * `flags` bit 0 is set (same "free-self" convention as
 * DestroyPaletteCycles/DestroyLanguageSelect elsewhere in this codebase). The per-type
 * descriptor's own shape past its first 0x18 bytes (the `struct actor`-
 * shared prefix) isn't established, so `table+0x50/+0x54` stay raw
 * offsets rather than a named slot index. */
void DestroyHud(struct hud_counter *self, s32 flags)
{
    struct hud_digit_part *parts;
    struct hud_digit_part *end;

    parts = self->parts;
    if (parts != NULL) {
        s32 count = *(s32 *)((u8 *)parts - 4);

        end = (struct hud_digit_part *)((u8 *)parts + (count << 6));
        if (parts != end) {
            do {
                struct icon_slot *slot;

                end--;
                slot = (struct icon_slot *)((u8 *)end->table + 0x50);
                _call_via_r2((u8 *)end + slot->offset, 0, slot->ptr);
            } while (self->parts != end);
        }
        OperatorDeleteArray((u8 *)self->parts - 4);
    }
    if (flags & 1) {
        OperatorDelete(self);
    }
}
asm(".align 2, 0");
