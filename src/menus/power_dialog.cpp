#include "menus.hpp"

extern "C" {
#include "bitmap_font.h"
#include "vram_pool.h"
#include "system.h"
#include "text.h"
#include "audio.h"
#include "globals.h"
}

/* The power dialog (GitHub issue #8; PowerDialog, menus.hpp): a power's
 * name and description over the scrolling sky, shown by the four
 * Show*Dialog wrappers (power_dialog_draw.cpp) when a boss gives Crash a
 * power.
 *
 * gSmallFont and gLargeFont are still C (src/text/): their virtual calls
 * are spelled out through the record's slots. */

/* The fonts' set-up: the font's tile base, then its slot-6 method (which
 * uploads its glyphs), and its tiles reserved in OBJ VRAM. The same
 * helpers as RunLevelSelect's (level_select.cpp). */
static inline void IconSetup(struct bitmap_font *m, u32 v)
{
    struct icon_slot *slot;

    m->tileBase = v;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserve(struct bitmap_font **m)
{
    struct vram_upload_cursor *c = gObjVramCursor;

    ReserveObjVram(c, (*m)->tileCount << 5);
}

/* Runs the dialog to completion: resets the display, the palette cache
 * and both fonts, builds the dialog with the two texts, runs it (fade in,
 * wait for START, fade out) and deletes it. Once a NAKED transcription;
 * see docs/matching/archive/issue-4-6-8-naked-retry.md. */
void PowerDialog::Show(s32 title, s32 desc, s32 type)
{
    PowerDialog *dialog;

    mem_free_bytes(0xC0000000);
    WaitForVBlank();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;
    FreeUnlockedPaletteSlots(gPaletteCache);
    FontResetPalette(gSmallFont);
    FontResetPalette(gLargeFont);
    gObjVramCursor->baseTile = 0;
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);
    IconSetup(gSmallFont, 0);
    IconReserve(&gSmallFont);
    {
        u32 v = gSmallFont->tileCount;

        IconSetup(gLargeFont, v);
    }
    IconReserve(&gLargeFont);
    MarkObjVram(gObjVramCursor);
    dialog = new PowerDialog(GetUiText(title), GetUiText(desc), type);
    dialog->Loop();
    delete dialog;
    FreeUnlockedPaletteSlots(gPaletteCache);
    mem_free_bytes(0xC0000000);
}

/* The constructor: alpha blending at full fade (BLDY 16, so Loop fades
 * it in), BG0 and the OBJs on, the two texts, the sky background, and the
 * power's icon (animation `type` of sprite bank 0x1C8) at (240, 160) / 2,
 * then BG0's registers and the intro song. */
PowerDialog::PowerDialog(s32 titleText, s32 descText, s32 type)
{
    InitBgSetup(&bg, 0, 0x1f, 0, 3);
    blend.raw = 0;
    blend.bits.effect = 3;
    blend.bits.bdFirst = 1;
    blend.bits.bg0First = 1;
    blend.bits.bg1First = 1;
    blend.bits.bg2First = 1;
    blend.bits.bg3First = 1;
    blend.bits.objFirst = 1;
    bldy.evy = 16;
    *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
    *(vu16 *)REG_ADDR_BLDY = bldy.evy;
    dispcnt.raw = 0;
    dispcnt.bits.objMap1D = 1;
    dispcnt.bits.mode = 0;
    dispcnt.bits.bg0 = 1;
    dispcnt.bits.obj = 1;
    this->titleText = titleText;
    this->descText = descText;
    LoadGraphicsPackage(&bg, &gMenuSkyBg);
    frame = 0;
    {
        UiSprite *s = new UiSprite;

        icon = s;
        SetIconBank(s, 0xe4 << 1);
        s->tag = type;
        s->ResetFrameTimer();
        s->ResetFrameIndex();
        s->SetAnimDone(0);
    }
    icon->x = 0xf0 << 7;
    icon->y = 0xa0 << 7;
    icon->palette = icon->GetAnimPaletteSlot();
    REG_BG0CNT = GetBgSetupControl(&bg);
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    PlaySong(gAudioContext, SONG_INTRO);
}
