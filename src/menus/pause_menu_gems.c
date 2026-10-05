#include "core.h"
#include "audio.h"
#include "actor.h"
#include "bitmap_font.h"
#include "vram_pool.h"
#include "pause_menu.h"
#include "memory.h"
#include "text.h"
#include "menus.h"

/* DrawPauseGemsPage + DrawPauseRelicsPage: mutually address-adjacent, bracketed by
 * the already-matched DrawPausePowersPage (pause_menu_powers.c) before and
 * InitPauseMenuInfo (pause_menu_info.c) after - own object file for the
 * same reason pause_menu_loop.c documents. See
 * docs/matching/issue-7-0x08004d74-overlay-ui.md.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): its mask-before-ldrb
 * order shows in DrawPauseGemsPage's flag tests. */

extern void DrawSpriteWithOffset(void *arg0, s32 arg1, s32 arg2);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

static inline void set_icon_mgr_pos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Draws `label` at the icon manager's current position through its
 * `record->slots[2]` method (a gcc 2.x virtual call; _call_via_r2 is
 * `_call_via_r2`). Kept as a block macro rather than an inline
 * function: the method's `this` must be computed before the label
 * argument, as in the ROM. */
#define DRAW_ICON_TEXT(mgrExpr, label)                                          \
    {                                                                           \
        struct bitmap_font *_m = (mgrExpr);                                    \
        struct icon_record *_r = _m->record;                                    \
        _call_via_r2((u8 *)_m + _r->slots[2].offset, (label), _r->slots[2].ptr); \
    }

/* Shows whichever of `icons9c[1..4]` has a matching bit set in
 * `self->field_10`'s flag byte (bits 1/4/8/2 - a different bit set
 * than DrawPausePowersPage's, same handle), always shows `icons9c[0]`
 * unconditionally, then draws a fixed "x/28"-shaped fraction readout:
 * first `gPauseGemIconPos[0]`'s position (offset by -0x14/-4) with
 * `self->buf2f` at a fixed slot, then repositions to (0xb4, 0x80) and
 * calls `DrawPauseFraction` with `self->buf32`/`self->buf49` (the count/total
 * buffers `InitPauseGemsPage` - src/menus/pause_menu_pages_init.c - already fills
 * for this same icon row). */
void DrawPauseGemsPage(struct pause_menu *self)
{
    if (((u8 *)self->field_10)[2] & 1)
        DrawSpriteWithOffset(self->icons9c[1], 0, 0);
    if (((u8 *)self->field_10)[2] & 4)
        DrawSpriteWithOffset(self->icons9c[2], 0, 0);
    if (((u8 *)self->field_10)[2] & 8)
        DrawSpriteWithOffset(self->icons9c[3], 0, 0);
    if (((u8 *)self->field_10)[2] & 2)
        DrawSpriteWithOffset(self->icons9c[4], 0, 0);
    DrawSpriteWithOffset(self->icons9c[0], 0, 0);

    set_icon_mgr_pos(gSmallFont, gPauseGemIconPos[0].x - 0x14, gPauseGemIconPos[0].y - 4);
    DRAW_ICON_TEXT(gSmallFont, self->buf2f);
    set_icon_mgr_pos(gSmallFont, 0xb4, 0x80);
    DrawPauseFraction(self, self->buf32, self->buf49);
}

/* Same shape as DrawPauseGemsPage above for the `iconsB0[3]` row: shows all
 * three icons unconditionally (no per-bit gating this time), then
 * draws three fixed "x/20"-shaped fraction readouts at
 * `gPauseRelicIconPos[2]/[1]/[0]`'s positions (offset -4/+0xe, same
 * pattern as DrawPauseGemsPage's single readout) with `self->buf38`/
 * `buf3b`/`buf3e`, then a final one at (0xb4, 0x80) via `DrawPauseFraction`
 * with `self->buf35`/`buf4c` (the total/threshold buffers
 * `InitPauseRelicsPage` - src/menus/pause_menu_pages_init.c - fills for this
 * row). */
void DrawPauseRelicsPage(struct pause_menu *self)
{
    s32 i;

    for (i = 0; i <= 2; i++)
        DrawSpriteWithOffset(self->iconsB0[i], 0, 0);

    set_icon_mgr_pos(gSmallFont, gPauseRelicIconPos[2].x - 4, gPauseRelicIconPos[2].y + 0xe);
    DRAW_ICON_TEXT(gSmallFont, self->buf38);
    set_icon_mgr_pos(gSmallFont, gPauseRelicIconPos[1].x - 4, gPauseRelicIconPos[1].y + 0xe);
    DRAW_ICON_TEXT(gSmallFont, self->buf3b);
    set_icon_mgr_pos(gSmallFont, gPauseRelicIconPos[0].x - 4, gPauseRelicIconPos[0].y + 0xe);
    DRAW_ICON_TEXT(gSmallFont, self->buf3e);
    set_icon_mgr_pos(gSmallFont, 0xb4, 0x80);
    DrawPauseFraction(self, self->buf35, self->buf4c);
}
