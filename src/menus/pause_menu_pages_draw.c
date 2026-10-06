#include "core.h"
#include "match.h"
#include "bitmap_font.h"
#include "gba/defines.h"
#include "actor.h"
#include "pause_menu.h"
#include "text.h"
#include "system.h"
#include "menus.h"
#include "gfx.h"
#include "objects.h"
#include "globals.h"

/* The three functions below are companion "draw a label centered on an
 * icon widget" steps. Once NAKED transcriptions; they match as plain C
 * under both compilers once the icon-manager draws are written as the
 * gcc 2.x virtual calls they are (through libgcc's `_call_via_r2`),
 * with `this` computed before the label argument - see
 * docs/matching/archive/issue-4-6-8-naked-retry.md. */

/* DrawPauseTimeTrialPage, below, takes the `struct pause_menu`
 * (include/pause_menu.h; `field_6c`/`field_bc`/`timeBuf`) - the medal-icon-widget's (`InitPauseTimeTrialPage`)
 * companion label draw: formats `self->timeBuf` (already filled in by
 * InitPauseTimeTrialPage) centered on the medal icon via the shared
 * `gSmallFont` icon manager, using the same fixed
 * `gPauseTimeTrialIconPos` position pair InitPauseTimeTrialPage itself positions
 * the icon with. */

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);

static inline void set_icon_mgr_pos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* `record->slots[0]` measures `label` (returns its pixel width) and
 * `record->slots[2]` draws it at the manager's position - gcc 2.x
 * virtual calls (through libgcc's `_call_via_r2`). Block/statement macros
 * so `this` is computed before the label argument, as in the ROM. */
#define MEASURE_ICON_TEXT(mgrExpr, label)                                       \
    ({                                                                          \
        struct bitmap_font *_m = (mgrExpr);                                    \
        struct icon_record *_r = _m->record;                                    \
        _call_via_r2((u8 *)_m + _r->slots[0].offset, (s32)(label), _r->slots[0].ptr); \
    })
#define DRAW_ICON_TEXT(mgrExpr, label)                                          \
    {                                                                           \
        struct bitmap_font *_m = (mgrExpr);                                    \
        struct icon_record *_r = _m->record;                                    \
        _call_via_r2((u8 *)_m + _r->slots[2].offset, (s32)(label), _r->slots[2].ptr); \
    }

void DrawPauseTimeTrialPage(struct pause_menu *self)
{
    u32 w;

    if (self->field_6c)
        DrawSpriteWithOffset((struct actor *)self->field_bc, 0, 0);
    w = MEASURE_ICON_TEXT(gSmallFont, self->timeBuf);
    set_icon_mgr_pos(gSmallFont, gPauseTimeTrialIconPos.x - (w >> 1) - 2, gPauseTimeTrialIconPos.y - 0x23);
    DRAW_ICON_TEXT(gSmallFont, self->timeBuf);
}

/* Same self object, `InitPauseCrystalsPage`'s (the `field_88` icon widget)
 * companion label draw - the "results count" pair (`buf2c`/`buf46`,
 * already formatted by `InitPauseCrystalsPage` itself) centered on that icon at
 * the fixed `gPauseCrystalIconPos` position, via `DrawPauseFraction`
 * (src/menus/pause_menu_widgets.c) that actually draws the two small
 * strings. */
void DrawPauseCrystalsPage(struct pause_menu *self)
{
    DrawSpriteWithOffset((struct actor *)self->field_88, 0, 0);
    set_icon_mgr_pos(gSmallFont, gPauseCrystalIconPos.x - 0x2c, gPauseCrystalIconPos.y - 8);
    DrawPauseFraction(self, self->buf2c, self->buf46);
}

/* Draws the current info page's title (`field_24`, AnimatePauseMenu's
 * page index) at (0xc2, 0x2c) via the same icon manager,
 * picking its text from a lookup table
 * (`gPauseMenuPageTitles[self->field_24]`) fed through `GetUiText`
 * (the same "char code -> something _call_via_r2 can draw" conversion
 * `DrawPowerDialog`/`InitPauseCrystalsPage` already use for fixed digits like
 * `0x2e`/`0x14`). */
void DrawPauseMenuPageTitle(struct pause_menu *self)
{
    s32 label = GetUiText(gPauseMenuPageTitles[self->field_24]);
    u32 w = MEASURE_ICON_TEXT(gSmallFont, label);

    set_icon_mgr_pos(gSmallFont, 0xc2 - (w >> 1), 0x2c);
    DRAW_ICON_TEXT(gSmallFont, label);
}

/* The composite pause/options screen's "apply display registers" step
 * for its own top-level object - see include/pause_menu.h for
 * the full reconciled struct (this function only touches field_c8/
 * field_cc/field_d0). Distinct from - and much larger than -
 * `struct sub_8006700_actor` (src/menus/power_dialog_draw.c/power_dialog_loop.c),
 * which is the smaller per-widget object `src/menus/power_dialog_draw.c`'s
 * already-matched `CommitPowerDialogFrame` uses for the same job at different
 * offsets. */

/* `self->field_d0`'s read+store is deliberately routed through an
 * inline-asm-computed address pinned to `r0` rather than a plain
 * `self->field_d0` field access: with the latter, this compiler
 * recognizes `self` (r4) is dead after this point and folds the
 * address computation directly into r4 (saving a `mov`), one
 * instruction shorter than the ROM's fresh r0 computation - the ROM
 * never performs this particular reuse here (though it does for the
 * `field_c8`/`field_cc` accesses just above, which this reconstruction
 * gets for free from plain field access). */
void CommitPauseMenuFrame(struct pause_menu *self)
{
    WaitForVBlank();
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
    *(vu16 *)PLTT = 0;
    *(vu32 *)REG_ADDR_BLDCNT = self->field_c8;
    *(vu16 *)REG_ADDR_BLDY = (u32)(self->field_cc << 27) >> 27;
    {
        MATCH_HOLD_REG(u16 *, p, r0);
        vu16 *dst = (vu16 *)REG_ADDR_DISPCNT;
        asm("add %0, %1, #0\n\tadd %0, %0, #0xd0" : "=r" (p) : "r" (self));
        *dst = *p;
    }
}
