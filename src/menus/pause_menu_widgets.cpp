#include "menus.hpp"
#include "font.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "text.h"
#include <libgcc.h>
#include "audio.h"
#include "globals.h"
}

/* PauseMenu's fraction readout, volume and cursor steps (menus.hpp; C++
 * since the #664 cleanup), with FormatDecimal (C linkage). */

/* Draws a "count/total" readout: `count` at gSmallFont's position, a
 * large `/` (gLargeFont, moved to 2 pixels left of that position), then
 * `total` with gSmallFont moved to 5 left of and 8 below the large
 * font's position. `this` is unused.
 *
 * Matched in the last-ten pass (docs/matching/archive/last-ten-naked-retry.md).
 * The ROM's r7 is never a pseudo's register here (the function is one
 * basic block, and local-alloc never uses the frame pointer): it is
 * reload's register for the 0x110 posX offset. Every posX/posY access is
 * a plain field access, so each offset reaches reload as a constant: the
 * second half's 0x110 goes to r6 (reload_cse copies it from r7) and
 * 0x114 to r7 (move2add's `adds r7, #4`). The second half's reads go
 * through the inline getters and its new position is passed straight to
 * the inline setter, which puts both loads ahead of the `*pdc` load. */
void PauseMenu::DrawFraction(void *count, void *total)
{
    Font **pdc = &gSmallFont;
    Font **pe0;

    (*pdc)->DrawText((u8 *)count);
    {
        Font *d = *pdc;
        u32 x = d->posX;
        u32 y = d->posY;

        /* Assigned here, not at the top: that keeps the
         * &gLargeFont load after the posX/posY loads. */
        pe0 = &gLargeFont;
        (*pe0)->SetPos(x - 2, y);
    }
    (*pe0)->DrawGlyph('/');
    {
        Font *e = *pe0;

        (*pdc)->SetPos(e->GetX() - 5, e->GetY() + 8);
    }
    (*pdc)->DrawText((u8 *)total);
}

/* Writes " <NN%>" into `buf`: the digits of `value` land at `buf + 2`
 * via FormatDecimal, which returns how many it wrote. */
static inline void FormatPercent(u8 *buf, s32 value)
{
    s32 len;

    buf[0] = ' ';
    buf[1] = '<';
    len = FormatDecimal(value, &buf[2]);
    buf[len + 2] = '%';
    buf[len + 3] = '>';
    buf[len + 4] = '\0';
}

/* On the music row (type 4) or the sound row (type 5), turns that volume
 * down one 5% step, unless it is 0: the new " <NN%>" text, and the
 * volume `((v << 8) + 1) / 20` set on the audio context. The sound row
 * also plays a cue. */
void PauseMenu::VolumeDown()
{
    s32 count;

    switch (rows[cursor].type) {
    case 4:
        count = musicVolume;
        if (count != 0) {
            count--;
            musicVolume = count;
            FormatPercent(musicVolumeText, count * 5);
            SetMusicVolume(gAudioContext, ((musicVolume << 8) + 1) / 20);
        }
        break;
    case 5:
        count = soundVolume;
        if (count != 0) {
            count--;
            soundVolume = count;
            FormatPercent(soundVolumeText, count * 5);
            SetSfxVolume(gAudioContext, ((soundVolume << 8) + 1) / 20);
            PlaySfx(gAudioContext, SFX_HUD_COLLECT, 0x100);
        }
        break;
    }
}

/* VolumeDown's counterpart: one step up, up to 20 (100%). */
void PauseMenu::VolumeUp()
{
    s32 count;

    switch (rows[cursor].type) {
    case 4:
        count = musicVolume;
        if (count <= 0x13) {
            count++;
            musicVolume = count;
            FormatPercent(musicVolumeText, count * 5);
            SetMusicVolume(gAudioContext, ((musicVolume << 8) + 1) / 20);
        }
        break;
    case 5:
        count = soundVolume;
        if (count <= 0x13) {
            count++;
            soundVolume = count;
            FormatPercent(soundVolumeText, count * 5);
            SetSfxVolume(gAudioContext, ((soundVolume << 8) + 1) / 20);
            PlaySfx(gAudioContext, SFX_HUD_COLLECT, 0x100);
        }
        break;
    }
}

/* Moves the cursor down one row, wrapping at the row count. */
void PauseMenu::CursorDown()
{
    cursor = cursor + 1;
    cursor = __modsi3(cursor, rowCount);
}

/* Moves the cursor up one row, wrapping to the last row. */
s32 PauseMenu::CursorUp()
{
    s32 v = cursor;
    if (v == 0) {
        v = rowCount;
    }
    v -= 1;
    cursor = v;
    return v;
}

/* Decimal `itoa`: writes `value`'s decimal digits (unsigned, most
 * significant first) to `dest`, NUL-terminated, and returns the digit
 * count. Shared by every number label of the pause menu, the level
 * select and the HUD. Builds the digits least-significant first into a
 * small stack buffer via the div/mod library primitives
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
 * order, while leaving `dest` to the natural allocator. */
s32 FormatDecimal(s32 value, u8 *dest)
{
    MATCH_HOLD_REG(s32, val, r5) = value;
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

/* Formats " <NN%>" (the digits of `volume * 5`) into `out`. `this` is
 * unused (the ROM's first instruction discards r0). */
void PauseMenu::FormatVolume(s32 volume, u8 *out)
{
    s32 value = volume * 5;
    s32 count;

    out[0] = ' ';
    out[1] = '<';
    count = FormatDecimal(value, out + 2);
    out[count + 2] = '%';
    out[count + 3] = '>';
    out[count + 4] = 0;
}
