#include "core.h"
#include "actor.h"
#include "bitmap_font.h"
#include "pause_menu.h"
#include "text.h"

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Draws `label1`/`label2` (a small "N/M" fraction readout - a row's
 * count over its fixed total, e.g. the icon-row helpers in
 * DrawPauseGemsPage/DrawPauseRelicsPage pass each row's formatted count/total
 * scratch buffers) on the composite pause/options screen's results
 * icons: draws `label1` at `gSmallFont`'s current position
 * (slot 2), copies that position (x-2, y unchanged) into
 * `gLargeFont` and draws a literal `/` there (slot 4), then
 * repositions `gSmallFont` to (that x-5, that y+8) and draws
 * `label2` there (slot 2). `self` is unused - the ROM never reads it
 * either.
 *
 * Matched in the last-ten pass (docs/matching/last-ten-naked-retry.md).
 * The ROM's r7 is never a pseudo's register here (the function is one
 * basic block, and local-alloc never uses the frame pointer): it is
 * reload's register for the 0x110 posX offset. Every posX/posY access is
 * a plain field access, so each offset reaches reload as a constant: the
 * second half's 0x110 goes to r6 (reload_cse copies it from r7) and
 * 0x114 to r7 (move2add's `adds r7, #4`). The second half's reads go
 * through inline getters and its new position is passed straight to the
 * inline setter, which puts both loads ahead of the `*pdc` load. */
static inline u32 get_icon_mgr_posx(struct bitmap_font *m)
{
    return m->posX;
}

static inline u32 get_icon_mgr_posy(struct bitmap_font *m)
{
    return m->posY;
}

static inline void set_icon_mgr_pos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Calls the icon manager's `record->slots[slot]` method on `label` (a
 * gcc 2.x virtual call through libgcc's `_call_via_r2`). */
#define DRAW_ICON_SLOT(mgrExpr, slot, label)                                          \
    {                                                                                 \
        struct bitmap_font *_m = (mgrExpr);                                          \
        struct icon_record *_r = _m->record;                                          \
        _call_via_r2((u8 *)_m + _r->slots[slot].offset, (label), _r->slots[slot].ptr); \
    }

void DrawPauseFraction(struct pause_menu *self, void *label1, void *label2)
{
    struct bitmap_font **pdc = &gSmallFont;
    struct bitmap_font **pe0;

    DRAW_ICON_SLOT(*pdc, 2, label1);
    {
        struct bitmap_font *d = *pdc;
        u32 x = d->posX;
        u32 y = d->posY;

        /* Assigned here, not at the top: that keeps the
         * &gLargeFont load after the posX/posY loads. */
        pe0 = &gLargeFont;
        set_icon_mgr_pos(*pe0, x - 2, y);
    }
    DRAW_ICON_SLOT(*pe0, 4, (void *)0x2f);
    {
        struct bitmap_font *e = *pe0;

        set_icon_mgr_pos(*pdc, get_icon_mgr_posx(e) - 5, get_icon_mgr_posy(e) + 8);
    }
    DRAW_ICON_SLOT(*pdc, 2, label2);
}

extern s32 FormatDecimal(s32 value, u8 *dest);
extern void *gAudioContext;
extern void SetMusicVolume(void *self, u32 value);
extern void SetSfxVolume(void *self, u32 value);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

/* One entry of the per-row record array at `field_14` (8-byte stride;
 * `type` 4/5 are the two editable-percentage rows). */
struct pause_row {
    void *label;
    s32 type;
};

#define ROW_TYPE(self) (((struct pause_row *)(self)->field_14)[(self)->field_18].type)

/* Writes " <NN%>" into `buf`: the digits of `value` land at `buf + 2`
 * via FormatDecimal, which returns how many it wrote. */
static inline void format_pct(u8 *buf, s32 value)
{
    s32 len;

    buf[0] = ' ';
    buf[1] = '<';
    len = FormatDecimal(value, &buf[2]);
    buf[len + 2] = '%';
    buf[len + 3] = '>';
    buf[len + 4] = '\0';
}

/* One of a matched pair (PauseMenuVolumeUp increments the other way): if the
 * current row is an "editable count" row (type 4 or 5) and its count
 * (`musicVolume`/`soundVolume` respectively, 0-20 in steps of 5%) is
 * non-zero, decrements it, formats the " <NN%>" scratch string into
 * `musicVolumeText`/`soundVolumeText`, and pushes the new level (`(count << 8 | 1) / 20`)
 * through the matching AudioContext setter (`SetMusicVolume`/`SetSfxVolume`
 * - see src/audio/audio.c). Only the type-5/`soundVolume` branch
 * also plays the standard SFX cue. */
void PauseMenuVolumeDown(struct pause_menu *self)
{
    s32 count;

    switch (ROW_TYPE(self)) {
    case 4:
        count = self->musicVolume;
        if (count != 0) {
            count--;
            self->musicVolume = count;
            format_pct(self->musicVolumeText, count * 5);
            SetMusicVolume(gAudioContext, ((self->musicVolume << 8) + 1) / 20);
        }
        break;
    case 5:
        count = self->soundVolume;
        if (count != 0) {
            count--;
            self->soundVolume = count;
            format_pct(self->soundVolumeText, count * 5);
            SetSfxVolume(gAudioContext, ((self->soundVolume << 8) + 1) / 20);
            PlaySfx(gAudioContext, 0xe, 0x100);
        }
        break;
    }
}

/* Counterpart to PauseMenuVolumeDown above: increments (capped at 0x14)
 * instead of decrementing. */
void PauseMenuVolumeUp(struct pause_menu *self)
{
    s32 count;

    switch (ROW_TYPE(self)) {
    case 4:
        count = self->musicVolume;
        if (count <= 0x13) {
            count++;
            self->musicVolume = count;
            format_pct(self->musicVolumeText, count * 5);
            SetMusicVolume(gAudioContext, ((self->musicVolume << 8) + 1) / 20);
        }
        break;
    case 5:
        count = self->soundVolume;
        if (count <= 0x13) {
            count++;
            self->soundVolume = count;
            format_pct(self->soundVolumeText, count * 5);
            SetSfxVolume(gAudioContext, ((self->soundVolume << 8) + 1) / 20);
            PlaySfx(gAudioContext, 0xe, 0x100);
        }
        break;
    }
}

/* A small counter/threshold pair on the composite pause/options screen's
 * settings-row sub-widget (the big object InitPauseMenu constructs,
 * fields 0x88 upward - still raw as a whole; see docs/matching.md's
 * write-up for this chunk). Only the two fields these two functions
 * touch are named, the same minimal-local-type convention
 * src/save/save_menu_ui.c's `struct bg_widget` uses. */
struct row_counter_widget {
    u8 unused_00[0x18];
    s32 field_18;
    s32 field_1c;
};

extern s32 __modsi3(s32 arg0, s32 arg1);

/* Bumps `field_18` by one, then re-clamps it against `field_1c` via
 * __modsi3 (still raw - reads like a generic "wrap/clamp counter"
 * helper, seen throughout this chunk). */
void PauseMenuCursorDown(struct row_counter_widget *self)
{
    self->field_18 = self->field_18 + 1;
    self->field_18 = __modsi3(self->field_18, self->field_1c);
}

/* Counterpart to PauseMenuCursorDown above: decrements `field_18`, wrapping
 * around to `field_1c` first when it's already at zero. */
s32 PauseMenuCursorUp(struct row_counter_widget *self)
{
    s32 v = self->field_18;
    if (v == 0) {
        v = self->field_1c;
    }
    v -= 1;
    self->field_18 = v;
    return v;
}

extern s32 __modsi3(s32 dividend, s32 divisor);
extern s32 __divsi3(s32 dividend, s32 divisor);

/* Decimal `itoa`: writes `value`'s decimal digits (unsigned, most
 * significant first) to `dest`, NUL-terminated, and returns the digit
 * count. Shared by every settings-row/results-widget number label in
 * this ROM region (`src/menus/pause_menu_pages_init.c`/`pause_menu_widgets.c`
 * already call it as an `extern`). Builds the digits least-significant
 * first into a small stack buffer via the div/mod library primitives
 * (`lib/libgcc/lib1funcs.s`), then reverses them into `dest`.
 *
 * `val`'s explicit `r5` pin (initialized from the `value` parameter,
 * rather than just using `value` directly) is required to reproduce
 * the ROM's parameter-home order: with a plain unpinned `value`, this
 * compiler always copies argument registers to their home pseudo-regs
 * in ascending source-register order (r0 before r1), but the ROM
 * copies r1 (`dest` -> r7) first, r0 (`value` -> r5) second - pinning
 * `val`'s initializer as a separate reg-var assignment defers the r0
 * copy until just before the loop that needs it, matching the ROM's
 * order, while leaving `dest` to the natural allocator (an *explicit*
 * pin on `dest` would also have to include r7 in the push/pop list by
 * hand - a genuine ABI hazard in this toolchain, see
 * matching_decomp_register_pinning memory - whereas the natural
 * allocator gets the push/pop list right on its own once it reaches
 * for r7 by itself). */
s32 FormatDecimal(s32 value, u8 *dest)
{
    register s32 val asm("r5") = value;
    u8 buf[0xc];
    s32 count;
    s32 i;

    count = 0;
    do {
        u8 *p = &buf[count];
        *p = (u8)__modsi3(val, 10) + '0';
        val = __divsi3(val, 10);
        count++;
    } while (val != 0);

    i = 0;
    do {
        count--;
        dest[count] = buf[i];
        i++;
    } while (count != 0);
    dest[i] = 0;

    return i;
}

/* Formats a `" <NN%>"`-shaped scratch string (space, `<`, decimal
 * digits of `arg1 * 5`, `%`, `>`, NUL) into `out` via FormatDecimal
 * above. `arg0` is read by nothing in this function - a genuinely
 * unused parameter (the ROM's own r0 -> r0 first instruction discards
 * it before ever reading it). */
void FormatVolumePercent(s32 arg0, s32 arg1, u8 *out)
{
    s32 value = arg1 * 5;
    s32 count;

    out[0] = ' ';
    out[1] = '<';
    count = FormatDecimal(value, out + 2);
    out[count + 2] = '%';
    out[count + 3] = '>';
    out[count + 4] = 0;
}
/* Trailing byte-padding gotcha (see docs/matching.md/
 * matching_decomp_alignment_fix memory): the ROM pads the gap before
 * the next function (DrawPauseTimeTrialPage, still-raw at this point) with zero
 * bytes (an explicit `.align 2, 0` in the original assembly), but this
 * compiler's own default inter-function padding is a `mov r8, r8`
 * NOP-equivalent instead. */
asm(".align 2, 0");
