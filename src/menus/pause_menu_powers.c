#include "core.h"
#include "match.h"
#include "actor.h"
#include "bitmap_font.h"
#include "pause_menu.h"
#include "text.h"
#include "system.h"
#include "menus.h"
#include "level_menu.h"
#include "objects.h"

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Shows (`DrawSpriteWithOffset(icon, 0, 0)`) whichever of `icons8c[0..3]` has a
 * matching power bit set in `self->progress->flags` (bits
 * 0x20/0x80/0x40/0x10, one per slot). If *none* of the four bits were set, draws a fallback
 * centered label (text id 0x3a) at a fixed position instead. */
void DrawPausePowersPage(struct pause_menu *self)
{
    s32 none = 1;

    {
        MATCH_HOLD_REG(u8 *, p, r1) = &self->progress->flags;
        MATCH_HOLD_REG(s32, mask, r0) = 0x20;
        MATCH_HOLD_REG(u8, byte, r1);
        byte = *p;
        mask &= byte;
        if (mask) {
            DrawSpriteWithOffset((struct actor *)self->icons8c[0], 0, 0);
            none = 0;
        }
    }
    {
        MATCH_HOLD_REG(u8 *, p, r1) = &self->progress->flags;
        MATCH_HOLD_REG(s32, mask, r0) = 0x80;
        MATCH_HOLD_REG(u8, byte, r1);
        byte = *p;
        mask &= byte;
        if (mask) {
            DrawSpriteWithOffset((struct actor *)self->icons8c[1], 0, 0);
            none = 0;
        }
    }
    {
        MATCH_HOLD_REG(u8 *, p, r1) = &self->progress->flags;
        MATCH_HOLD_REG(s32, mask, r0) = 0x40;
        MATCH_HOLD_REG(u8, byte, r1);
        byte = *p;
        mask &= byte;
        if (mask) {
            DrawSpriteWithOffset((struct actor *)self->icons8c[2], 0, 0);
            none = 0;
        }
    }
    {
        MATCH_HOLD_REG(u8 *, p, r1) = &self->progress->flags;
        MATCH_HOLD_REG(s32, mask, r0) = 0x10;
        MATCH_HOLD_REG(u8, byte, r1);
        byte = *p;
        mask &= byte;
        if (mask) {
            DrawSpriteWithOffset((struct actor *)self->icons8c[3], 0, 0);
            none = 0;
        }
    }

    if (none) {
        s32 label = GetUiText(0x3a);
        struct icon_record *rec = gSmallFont->record;
        u32 width =
            _call_via_r2((u8 *)gSmallFont + rec->slots[0].offset, (void *)label, rec->slots[0].ptr);
        s32 halfX = 0xc2 - (width >> 1);
        struct bitmap_font *mgr = gSmallFont;
        s32 y = 0x64;

        mgr->posX = halfX;
        mgr->posY = y;

        rec = gSmallFont->record;
        _call_via_r2((u8 *)gSmallFont + rec->slots[2].offset, (void *)label, rec->slots[2].ptr);
    }
}
