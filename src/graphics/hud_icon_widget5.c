#include "core.h"
#include "icon_manager.h"

/* Sits between the parked InitFont (asm/code_3_2_20_8a78.s) and the
 * rest of the still-raw HUD text/icon-widget driver code
 * (asm/code_3_2_20_8b7c.s, out of GitHub issue #46's chunk scope) -
 * FontHeightToLines through FontSetTileBase, GitHub issue #46. Same
 * `struct icon_manager` as hud_icon_widget.c/hud_icon_widget2.c/
 * hud_icon_widget3.c/hud_icon_widget4.c.
 *
 * DestroyFont (0x08028B7C, no tracked issue - just the next
 * function in ROM order, immediately after FontSetTileBase above) is
 * folded in here too rather than getting its own object file, per
 * docs/workflow.md's "one .c file per contiguous ROM region"
 * convention: same `struct icon_manager`/`record` field this whole
 * file already documents. It is the base font's destructor (vtable
 * slot 1 of gFontVtable; it was once read as a constructor):
 * (`record = &gFontVtable`, then the same conditional
 * `OperatorDelete(self)` teardown-registration idiom
 * DestroyLargeFont/DestroySmallFont (src/graphics/actor_aabb_setup.c) and
 * InitFont above use elsewhere for the same table). Matched
 * byte-exact via plain struct field access - unlike InitFont's own
 * record write, this one didn't need the inline-asm address anchor
 * those functions use (confirmed by a full clean `make compare`): with
 * only one address computation total in the whole function (nothing
 * else contends for it), gcc's own codegen already lands the `self+
 * offsetof(record)` add in r2 exactly like the ROM. */
extern u8 gFontVtable[];
extern void OperatorDelete(void *manager);

extern void *_call_via_r1(void *arg0, void *arg1);
extern s32 __udivsi3(s32 value, s32 divisor);

/* Divides `value` by the widget's own line height (`lineHeight`) - see
 * src/util/word_util.c's sub_8001214, which uses this same field as a
 * divisor for a line-count limit. */
s32 FontHeightToLines(struct icon_manager *self, s32 value)
{
    return __udivsi3(value, self->lineHeight);
}

/* Trivial getter/setter pairs around `struct icon_manager`'s fields -
 * used by callers elsewhere in the still-raw HUD text/icon-widget
 * driver code. */
u32 FontGetTileCount(struct icon_manager *self)
{
    return self->tileCount;
}

void FontSetPos(struct icon_manager *self, u32 x, u32 y)
{
    self->posX = x;
    self->posY = y;
}

void FontNewLineAt(struct icon_manager *self, u32 y)
{
    self->posX = self->marginX;
    self->posY = y;
}

u32 FontGetMargin(struct icon_manager *self)
{
    return self->marginX;
}

void FontSetMargin(struct icon_manager *self, u32 val)
{
    self->marginX = val;
}

u32 FontGetY(struct icon_manager *self)
{
    return self->posY;
}

u32 FontGetX(struct icon_manager *self)
{
    return self->posX;
}

/* Sets `tileBase`, then forwards to `record`'s slot-6 trampoline (see
 * include/icon_manager.h's `struct icon_record`) via `_call_via_r1`,
 * discarding its result. */
void FontSetTileBase(struct icon_manager *self, u32 val)
{
    struct icon_slot *slot;

    self->tileBase = val;
    slot = &self->record->slots[6];
    _call_via_r1((u8 *)self + slot->offset, slot->ptr);
}

void DestroyFont(struct icon_manager *self, u32 flags)
{
    self->record = (struct icon_record *)gFontVtable;
    if (flags & 1) {
        OperatorDelete(self);
    }
}
