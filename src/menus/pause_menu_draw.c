#include "core.h"
#include "actor.h"
#include "bitmap_font.h"
#include "pause_menu.h"
#include "audio.h"
#include "vram_pool.h"
#include "memory.h"

extern void AdvanceSpriteAnim(struct actor *part);
extern s32 __modsi3(s32 dividend, s32 divisor);
extern void ResetSpriteFrameTimer(struct actor *part);
extern void ResetSpriteFrameIndex(struct actor *part);
extern void SetSpriteAnimDone(struct actor *part, u8 val);
extern s32 RandRange(s32 max);

/* A slow reveal/cycle animation over the results screen's icon groups:
 * `field_24` (0-4) selects which group to hide this call (a plain
 * `AdvanceSpriteAnim` per icon, no fade), advancing to the next group every
 * `field_28` (180) calls, wrapping mod 5. Independently, `field_c0`
 * (the row-cursor icon) blinks on its own countdown (`field_c4`):
 * while it's ticking down, just decrement it; once it hits 0, either
 * re-show the icon with a fresh random countdown (0x78-0xef) if it's
 * currently "armed" (`field_38`), or hide it otherwise. */
void AnimatePauseMenu(struct pause_menu *self)
{
    switch (self->field_24) {
    case 0:
        AdvanceSpriteAnim((struct actor *)self->field_88);
        break;
    case 1: {
        struct settings_icon_actor **p = self->icons8c;
        s32 i;
        for (i = 3; i >= 0; i--) {
            AdvanceSpriteAnim((struct actor *)*p);
            p++;
        }
        break;
    }
    case 2: {
        struct settings_icon_actor **p = self->icons9c;
        s32 i;
        for (i = 4; i >= 0; i--) {
            AdvanceSpriteAnim((struct actor *)*p);
            p++;
        }
        break;
    }
    case 3: {
        struct settings_icon_actor **p = self->iconsB0;
        s32 i;
        for (i = 2; i >= 0; i--) {
            AdvanceSpriteAnim((struct actor *)*p);
            p++;
        }
        break;
    }
    case 4:
        AdvanceSpriteAnim((struct actor *)self->field_bc);
        break;
    }

    self->field_28--;
    if (self->field_28 == 0) {
        self->field_24++;
        self->field_24 = __modsi3(self->field_24, 5);
        self->field_28 = 0xb4;
    }

    {
        s32 *countAddr = &self->field_c4;
        s32 result;

        if (*countAddr != 0) {
            goto decrement;
        }
        {
            struct settings_icon_actor *icon = self->field_c0;

            if (icon->field_38 != 0) {
                icon->frameIndex = 0;
                ResetSpriteFrameTimer((struct actor *)icon);
                ResetSpriteFrameIndex((struct actor *)icon);
                SetSpriteAnimDone((struct actor *)icon, 0);
                result = (u16)RandRange(0x78) + 0x78;
                goto store;
            } else {
                AdvanceSpriteAnim((struct actor *)icon);
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
 * docs/matching/issue-7-0x08004d74-overlay-ui.md. */

extern void ResetOamBuffer(void *arg0);
extern void RewindObjVram(struct vram_upload_cursor *self);
extern struct oam_shadow_buffer *gOamBuffer;
extern void HideUnusedOamEntries(struct oam_shadow_buffer *arg0);
extern void DrawPauseMenuRows(struct pause_menu *self);
extern void DrawPauseMenuPageTitle(struct pause_menu *self);
extern void DrawPauseCrystalsPage(struct pause_menu *self);
extern void DrawPauseTimeTrialPage(struct pause_menu *self);
extern void DrawPausePowersPage(struct pause_menu *self);
extern void DrawPauseGemsPage(struct pause_menu *self);
extern void DrawPauseRelicsPage(struct pause_menu *self);
extern void DrawSpriteWithOffset(void *icon, s32 dx, s32 dy);
extern u32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern struct vram_upload_cursor *gObjVramCursor;
extern struct bitmap_font *gSmallFont;
extern struct bitmap_font *gLargeFont;
extern void *GetUiText(s32 id);
extern void FontResetPalette(struct bitmap_font *self);

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
 * settings row" step: draws `self->field_70` (the current level's name
 * label) centered into `gLargeFont`'s slot pair, then - only
 * when `self->field_74` is set (levels 0-0x13, see InitPauseMenuInfo) -
 * draws `field_74` followed immediately by `self->buf78` (" N") at a
 * fixed position, forming a "LEVEL N"-shaped composite label.
 * Unconditionally right-aligns `self->buf41` (the completion
 * percentage string) at a fixed row. Calls the per-row list renderer
 * (`DrawPauseMenuRows`) and an unread sibling (`DrawPauseMenuPageTitle`), then
 * dispatches on `self->field_24` (the same state AnimatePauseMenu cycles -
 * cases 0-4 map to `DrawPauseCrystalsPage`/`DrawPausePowersPage`/`DrawPauseGemsPage`/
 * `DrawPauseRelicsPage`/`DrawPauseTimeTrialPage`, one per icon-row group), and finally
 * hides `self->field_c0` (the row-cursor icon) if its blink countdown
 * (`field_c4`) has reached 0.
 *
 * Was NAKED; matches as plain C under both compilers since the
 * hard-register hold pass (docs/matching/hard-register-hold-retry.md).
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
        u32 w = ICON_SLOT_CALL(gLargeFont, 0, self->field_70);
        u32 x, t;
        register s32 hold asm("r2");

        /* Hard-register hold (no code): r2 stays live across the x
         * computation, so y takes it afterwards. */
        asm("" : "=r"(hold));
        t = 0xf0 - w;
        x = t >> 1;
        /* Extra reference (no code): keeps `t` (r1) from being tied
         * to `x` (r3). */
        asm("" : : "r"(t));
        /* End of the hold. */
        asm("" : : "r"(hold));
        set_icon_mgr_pos(gLargeFont, x, 0xe);
    }
    ICON_SLOT_CALL(gLargeFont, 2, self->field_70);
    label = self->field_74;
    if (label != NULL) {
        set_icon_mgr_pos(gLargeFont, 0x20, 0x26);
        ICON_SLOT_CALL(gLargeFont, 2, label);
        ICON_SLOT_CALL(gLargeFont, 2, self->buf78);
    }
    width = ICON_SLOT_CALL(gLargeFont, 0, self->buf41);
    {
        u32 x, t;
        register s32 hold asm("r2");

        /* Hard-register hold (no code), as above. */
        asm("" : "=r"(hold));
        t = 0x8c;
        x = t - width;
        /* Extra references (no code): neither the 0x8c (r1) nor
         * `width` (r0) is tied to `x` (r3). */
        asm("" : : "r"(t), "r"(width));
        /* End of the hold. */
        asm("" : : "r"(hold));
        set_icon_mgr_pos(gLargeFont, x, 0x88);
    }
    ICON_SLOT_CALL(gLargeFont, 2, self->buf41);
    DrawPauseMenuRows(self);
    DrawPauseMenuPageTitle(self);
    switch (self->field_24) {
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
    if (self->field_c4 == 0)
        DrawSpriteWithOffset(self->field_c0, 0, 0);
    HideUnusedOamEntries(gOamBuffer);
}

extern s32 FontSetPalette(struct bitmap_font *self, s32 val);

/* One 8-byte record of `self->field_14`'s per-row array: a runtime
 * string-table label id, then a type tag (`DrawPauseMenuRows` branches on
 * `==4`/`==5`/else; `PauseMenuLoop`'s confirm check uses the same tag). */
struct pause_screen_row_record {
    s32 labelId;
    s32 typeTag;
};

/* The composite pause/options screen's per-row list renderer - draws
 * `self->field_1c` rows (from `self->field_14`'s record array),
 * highlighting whichever matches `self->field_18` (the selected
 * index), each centered horizontally and stacked vertically by
 * `self->field_20` pixels starting at y=0x4a. Three layout variants
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

    for (i = 0; i < self->field_1c; i++) {
        void *label;
        s32 x;

        if (i == self->field_18)
            FontSetPalette(gSmallFont, 0xf);
        else
            FontResetPalette(gSmallFont);
        label = GetUiText(((struct pause_screen_row_record *)self->field_14)[i].labelId);
        x = 0x32 - (ICON_SLOT_CALL(gSmallFont, 0, label) >> 1);
        switch (((struct pause_screen_row_record *)self->field_14)[i].typeTag) {
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
        y += self->field_20;
    }
    FontResetPalette(gSmallFont);
}
