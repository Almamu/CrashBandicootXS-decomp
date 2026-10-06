#include "core.h"
#include "match.h"
#include "actor.h"
#include "bitmap_font.h"
#include "pause_menu.h"
#include "audio.h"
#include "vram_pool.h"
#include "system.h"
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "menus.h"
#include "gfx.h"
#include "objects.h"
#include "globals.h"

/* A slow reveal/cycle animation over the results screen's icon groups:
 * `page` (0-4) selects which group to hide this call (a plain
 * `AdvanceSpriteAnim` per icon, no fade), advancing to the next group every
 * `pageTimer` (180) calls, wrapping mod 5. Independently, `blinkEyes`
 * (a pair of eyelids, sprite bank 46, drawn over the background's
 * eyes) blinks on its own countdown (`blinkTimer`): while it's ticking
 * down, just decrement it; at 0 the blink animation plays, and once it
 * is done (`animDone`) it is rewound and a fresh random countdown
 * (0x78-0xef) starts. */
void AnimatePauseMenu(struct pause_menu *self)
{
    switch (self->page) {
    case 0:
        AdvanceSpriteAnim((struct box_part *)(struct actor *)self->crystalIcon);
        break;
    case 1:
        {
            struct settings_icon_actor **p = self->icons8c;
            s32 i;
            for (i = 3; i >= 0; i--) {
                AdvanceSpriteAnim((struct box_part *)*p);
                p++;
            }
            break;
        }
    case 2:
        {
            struct settings_icon_actor **p = self->icons9c;
            s32 i;
            for (i = 4; i >= 0; i--) {
                AdvanceSpriteAnim((struct box_part *)*p);
                p++;
            }
            break;
        }
    case 3:
        {
            struct settings_icon_actor **p = self->iconsB0;
            s32 i;
            for (i = 2; i >= 0; i--) {
                AdvanceSpriteAnim((struct box_part *)(struct actor *)*p);
                p++;
            }
            break;
        }
    case 4:
        AdvanceSpriteAnim((struct box_part *)(struct actor *)self->trialIcon);
        break;
    }

    self->pageTimer--;
    if (self->pageTimer == 0) {
        self->page++;
        self->page = __modsi3(self->page, 5);
        self->pageTimer = 0xb4;
    }

    {
        s32 *countAddr = &self->blinkTimer;
        s32 result;

        if (*countAddr != 0) {
            goto decrement;
        }
        {
            struct settings_icon_actor *icon = self->blinkEyes;

            if (icon->animDone != 0) {
                icon->frameIndex = 0;
                ResetSpriteFrameTimer((struct actor *)icon);
                ResetSpriteFrameIndex((struct actor *)icon);
                SetSpriteAnimDone((struct actor *)icon, 0);
                result = (u16)RandRange(0x78) + 0x78;
                goto store;
            } else {
                AdvanceSpriteAnim((struct box_part *)(struct actor *)icon);
                goto done;
            }
        }
    decrement:
        result = *countAddr - 1;
    store:
        *countAddr = result;
    done:;
    }
}
/* Trailing byte-padding gotcha (see docs/matching.md/
 * matching_decomp_alignment_fix memory): the ROM pads the gap before
 * the next function (DrawPauseMenu) with zero bytes (an explicit
 * `.align 2, 0` in the original assembly), but this compiler's own
 * default inter-function padding is a `mov r8, r8` NOP-equivalent
 * instead. */
asm(".align 2, 0");

/* DrawPauseMenu + DrawPauseMenuRows: mutually address-adjacent (nothing real
 * sits between them), but bracketed by the already-matched
 * AnimatePauseMenu (pause_menu_draw.c) before and DrawPausePowersPage
 * (pause_menu_powers.c) after - own object file for the same reason
 * pause_menu_loop.c documents. See
 * docs/matching/archive/issue-7-0x08004d74-overlay-ui.md. */

extern u32 _call_via_r2(void *arg0, void *arg1, void *arg2);

static inline void set_icon_mgr_pos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* slot 0: measure a label's width; slot 2: draw it. */
#define ICON_SLOT_CALL(mgrExpr, slot, label)                                         \
    ({                                                                               \
        struct bitmap_font *_m = (mgrExpr);                                         \
        struct icon_record *_r = _m->record;                                         \
        _call_via_r2((u8 *)_m + _r->slots[slot].offset, (label), _r->slots[slot].ptr); \
    })

/* The composite pause/options screen's per-frame "draw the current
 * settings row" step: draws `self->levelName` (the current level's name
 * label) centered into `gLargeFont`'s slot pair, then - only
 * when `self->levelLabel` is set (levels 0-0x13, see InitPauseMenuInfo) -
 * draws `levelLabel` followed immediately by `self->buf78` (" N") at a
 * fixed position, forming a "LEVEL N"-shaped composite label.
 * Unconditionally right-aligns `self->buf41` (the completion
 * percentage string) at a fixed row. Calls the per-row list renderer
 * (`DrawPauseMenuRows`) and an unread sibling (`DrawPauseMenuPageTitle`), then
 * dispatches on `self->page` (the same state AnimatePauseMenu cycles -
 * cases 0-4 map to `DrawPauseCrystalsPage`/`DrawPausePowersPage`/`DrawPauseGemsPage`/
 * `DrawPauseRelicsPage`/`DrawPauseTimeTrialPage`, one per icon-row group), and finally
 * draws `self->blinkEyes` (the blinking eyelids) while its blink
 * countdown (`blinkTimer`) is at 0.
 *
 * Was NAKED; matches as plain C under both compilers since the
 * hard-register hold pass (docs/matching/archive/hard-register-hold-retry.md).
 * In both computed-x `set_icon_mgr_pos` calls the ROM keeps r2 free
 * while it computes x (r3), and never ties x to the value it is
 * computed from (r1). An r2 hold over the x computation puts y in r2,
 * and the address reloads that follow rotate as in the ROM (r4, r3,
 * then r4/r6 for the `ldrsh` offset). The extra references on the
 * temporaries stop local-alloc tying them to x. */
void DrawPauseMenu(struct pause_menu *self)
{
    void *label;
    u32 width;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    {
        u32 w = ICON_SLOT_CALL(gLargeFont, 0, self->levelName);
        u32 x, t;
        MATCH_HOLD_REG(s32, hold, r2);

        /* Hard-register hold (no code): r2 stays live across the x
         * computation, so y takes it afterwards. */
        MATCH_HOLD(hold);
        t = 0xf0 - w;
        x = t >> 1;
        /* Extra reference (no code): keeps `t` (r1) from being tied
         * to `x` (r3). */
        MATCH_USE(t);
        /* End of the hold. */
        MATCH_USE(hold);
        set_icon_mgr_pos(gLargeFont, x, 0xe);
    }
    ICON_SLOT_CALL(gLargeFont, 2, self->levelName);
    label = self->levelLabel;
    if (label != NULL) {
        set_icon_mgr_pos(gLargeFont, 0x20, 0x26);
        ICON_SLOT_CALL(gLargeFont, 2, label);
        ICON_SLOT_CALL(gLargeFont, 2, self->buf78);
    }
    width = ICON_SLOT_CALL(gLargeFont, 0, self->buf41);
    {
        u32 x, t;
        MATCH_HOLD_REG(s32, hold, r2);

        /* Hard-register hold (no code), as above. */
        MATCH_HOLD(hold);
        t = 0x8c;
        x = t - width;
        /* Extra references (no code): neither the 0x8c (r1) nor
         * `width` (r0) is tied to `x` (r3). */
        MATCH_USE2(t, width);
        /* End of the hold. */
        MATCH_USE(hold);
        set_icon_mgr_pos(gLargeFont, x, 0x88);
    }
    ICON_SLOT_CALL(gLargeFont, 2, self->buf41);
    DrawPauseMenuRows(self);
    DrawPauseMenuPageTitle(self);
    switch (self->page) {
    case 0:
        DrawPauseCrystalsPage(self);
        break;
    case 1:
        DrawPausePowersPage(self);
        break;
    case 2:
        DrawPauseGemsPage(self);
        break;
    case 3:
        DrawPauseRelicsPage(self);
        break;
    case 4:
        DrawPauseTimeTrialPage(self);
        break;
    }
    if (self->blinkTimer == 0)
        DrawSpriteWithOffset((struct actor *)self->blinkEyes, 0, 0);
    HideUnusedOamEntries(gOamBuffer);
}

/* The composite pause/options screen's per-row list renderer - draws
 * `self->rowCount` rows (from `self->rows`'s record array),
 * highlighting whichever matches `self->cursor` (the selected
 * index), each centered horizontally and stacked vertically by
 * `self->rowSpacing` pixels starting at y=0x4a. Three layout variants
 * per row, keyed by the record's type tag (docs/rom_map.md's
 * overlay_ui section, "DrawPauseMenuRows branches on a per-row type tag"):
 * a plain centered label (any other tag), or - for tags 4/5 - the
 * label additionally offset left by half of a second string's width
 * (`self->musicVolumeText` for tag 4, `self->soundVolumeText` for tag 5 - the " <NN%>"
 * scratch buffers InitPauseMenuInfo/PauseMenuVolumeDown/PauseMenuVolumeUp fill), with that second
 * string drawn immediately after at the same position (auto-advancing
 * - "label <NN%>" on one line).
 */
void DrawPauseMenuRows(struct pause_menu *self)
{
    s32 y = 0x4a;
    s32 i;

    for (i = 0; i < self->rowCount; i++) {
        void *label;
        s32 x;

        if (i == self->cursor)
            FontSetPalette(gSmallFont, 0xf);
        else
            FontResetPalette(gSmallFont);
        label = (void *)GetUiText(self->rows[i].labelId);
        x = 0x32 - (ICON_SLOT_CALL(gSmallFont, 0, label) >> 1);
        switch (self->rows[i].type) {
        case 4:
            x -= ICON_SLOT_CALL(gSmallFont, 0, self->musicVolumeText) >> 1;
            set_icon_mgr_pos(gSmallFont, x, y);
            ICON_SLOT_CALL(gSmallFont, 2, label);
            ICON_SLOT_CALL(gSmallFont, 2, self->musicVolumeText);
            break;
        case 5:
            x -= ICON_SLOT_CALL(gSmallFont, 0, self->soundVolumeText) >> 1;
            set_icon_mgr_pos(gSmallFont, x, y);
            ICON_SLOT_CALL(gSmallFont, 2, label);
            ICON_SLOT_CALL(gSmallFont, 2, self->soundVolumeText);
            break;
        default:
            set_icon_mgr_pos(gSmallFont, x, y);
            ICON_SLOT_CALL(gSmallFont, 2, label);
            break;
        }
        y += self->rowSpacing;
    }
    FontResetPalette(gSmallFont);
}
