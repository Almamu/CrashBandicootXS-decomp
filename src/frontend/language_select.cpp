#include "sprite_obj.hpp"
#include "frontend.hpp"

extern "C" {
#include "gba/dma_macros.h"
#include "system.h"
#include "vram_pool.h"
#include "text.h"
#include "audio.h"
#include "gfx.h"
#include "globals.h"
}

/* The language menu shown at boot (OpenLanguageSelect/RunLanguageSelect/
 * CloseLanguageSelect, called from MainLoop): up/down cycles `language`
 * through the six entries of gLanguageNames ("english", "français",
 * "deutsch", "español", "italiano", "nederlands", drawn with gSmallFont
 * over a starfield), A or START confirms, and MainLoop stores the
 * result in gLanguage, which picks the gUiText<Lang>/cutscene tables.
 * Sits at the very start of the address range docs/audio.md calls the
 * GAX2 engine, but is game-side code that merely uses PlaySfx.
 * LanguageSelect is in frontend.hpp (#664, part 10b); the file starts
 * with the last of CompanyLogos's and LogoActor's methods
 * (company_logos.cpp). */

/* Loads a "tagged" asset (see LoadTaggedAsset, src/system/asset.c)
 * into a freshly allocated buffer, then DMAs it to `dest`. A method of
 * the logo screen (LoadVvLogoGraphics's), which it doesn't use. */
void CompanyLogos::LoadAssetBuffered(const void *asset, void *dest)
{
    u32 val = *(const u32 *)asset;
    struct dma_regs *dma;
    u8 *buf;

    val >>= 8;
    buf = new u8[val];
    LoadTaggedAsset(asset, buf);
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)buf;
    dma->dst = (u32)dest;
    val >>= 1;
    dma->cnt = val | 0x80000000;
    dma->cnt;
    delete[] buf;
}

/* The company-logo screen's constructor and destructor, both empty
 * (ShowCompanyLogos, level_state.cpp, allocates the screen, runs it and
 * deletes it). */
CompanyLogos::CompanyLogos()
{
}

CompanyLogos::~CompanyLogos()
{
}

/* The company-logo actor's destructor (slot 1; RunCompanyLogos deletes
 * it): frees the two VRAM tile blocks the constructor allocated. g++ adds
 * ActorSelf's inline destructor (the unlink) and the class's operator
 * delete (mem_free). */
LogoActor::~LogoActor()
{
    FreeVramTileBlock(gLogoActorTiles[0]);
    FreeVramTileBlock(gLogoActorTiles[1]);
}

/* Runs the widget: resets it, draws/flushes once, then polls input each
 * frame (dispatching newly-pressed keys to Input) until it signals
 * `done`, returning the selected `language`. */
s32 LanguageSelect::Run()
{
    gLanguageSelect->language = 0;
    gLanguageSelect->frame = 0;
    gLanguageSelect->done = 0;
    gLanguageSelect->Draw();
    WaitForVBlank();
    gLanguageSelect->CommitFrame();

    while (gLanguageSelect->done == 0) {
        u16 keys;

        UpdateKeys(gInput);
        /* Read before gLanguageSelect, as the ROM loads them. */
        keys = gKeys.half.pressed;
        gLanguageSelect->Input(keys);
        gLanguageSelect->Draw();
        WaitForVBlank();
        gLanguageSelect->CommitFrame();
        gLanguageSelect->starfield->Update();
    }

    return gLanguageSelect->language;
}

/* Dispatches one frame's newly-pressed `flags` for the widget above:
 * bit 3 or bit 0 confirms/cancels (sets `done` to end the loop, sfx
 * 0x49); bit 6/bit 7 decrement/increment the 0-5 `language` value
 * (wrapping around, sfx 0x46). `frame` is a free-running frame
 * counter, incremented every call regardless. */
void LanguageSelect::Input(u32 flags)
{
    if (flags & START_BUTTON) {
        done = 1;
        PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
    } else if (flags & A_BUTTON) {
        done = 1;
        PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
    } else if (flags & DPAD_UP) {
        language--;
        if (language < 0) {
            language = 5;
        }
        PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
    } else if (flags & DPAD_DOWN) {
        language++;
        if (language > 5) {
            language = 0;
        }
        PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
    }
    frame = (frame + 1) & 0xff;
}

/* The language menu's per-frame draw
 * loop: for each of the six language names (0-5) it sets the shared
 * overlay frame (`gSmallFont`: 1 or 2 from `Blink`'s blink
 * state on the currently selected entry `language`, 0 elsewhere), measures
 * that name (MeasureText), centers it horizontally, and draws it
 * (DrawText) at a Y stepping by 0xa from 0x32.
 *
 * Was NAKED ("many-register allocation ceiling"); the glyph is assigned
 * inside the first call's argument list, as the ROM loads it there - see
 * docs/matching/archive/gax-toolchain-retry.md. */
void LanguageSelect::Draw()
{
    s32 y;
    s32 i;

    gOamBuffer->Reset();
    gObjVramCursor->Rewind();
    y = 0x32;
    for (i = 0; i <= 5; i++) {
        const u8 *glyph;
        s32 x;

        if (i == language)
            gSmallFont->SetPalette(Blink());
        else
            gSmallFont->SetPalette(0);
        x = (240 - gSmallFont->MeasureText((u8 *)(glyph = gLanguageNames[i]))) >> 1;
        gSmallFont->SetPos(x, y);
        gSmallFont->DrawText((u8 *)glyph);
        y += 10;
    }
    gOamBuffer->HideUnused();
}

/* Resets several OAM-manager globals, then hand-fills
 * `gPaletteCache`'s (`struct palette_cache`, include/vram_pool.h)
 * `slots[0]`-`slots[3]` with 4 fixed 32-byte OBJ tiles copied from
 * `gLanguageSelectPalette0`..`gLanguageSelectPalette3`, and finally sets
 * `gSmallFont`'s/`gLargeFont`'s tile base (Font::SetTileBase, which
 * uploads the tiles) plus a VRAM reserve (`ReserveObjVram`) for each,
 * copying `tileCount` into the other font's `tileBase`.
 *
 * Was NAKED: the ROM rematerializes the 0x108/0x12c/0x130 field-offset
 * constants after every call instead of keeping them in callee-saved
 * registers. Matched with the idiom from credits.cpp's
 * `InitCredits`: the two font steps as inline functions taking the font
 * as a parameter (each expansion recomputes its own
 * offsets; the E0 base is read from DC before E0 itself), plus one
 * `zero` local shared by the `gObjVramCursor` word 2/`tileBase` stores - the 0 the
 * ROM keeps in r8. Matches under both compilers. */
static inline void IconReserveVram(ObjVramCursor *c, Font *m)
{
    c->Reserve(m->tileCount << 5);
}

void LanguageSelect::InitGraphics()
{
    s32 i;

    gOamBuffer->Reset();
    gOamBuffer->HideUnused();
    WaitForVBlank();
    gOamBuffer->Commit();
    gPaletteCache->FreeUnlockedSlots();
    gPaletteCache->ClaimSlot(0);
    gPaletteCache->ClaimSlot(1);
    gPaletteCache->ClaimSlot(2);
    gPaletteCache->ClaimSlot(3);
    {
        PaletteCache *cache = gPaletteCache;
        u16 *destA = (u16 *)cache->slots[0];
        u16 *destB = (u16 *)cache->slots[2];

        for (i = 0; i < 16; i++) {
            destA[i] = gLanguageSelectPalette0[i];
            destA[i + 0x10] = gLanguageSelectPalette1[i];
            destB[i] = gLanguageSelectPalette2[i];
            destB[i + 0x10] = gLanguageSelectPalette3[i];
        }
    }
    {
        u32 zero = 0;

        gSmallFont->SetPalette(0);
        gLargeFont->SetPalette(0);
        gObjVramCursor->baseTile = zero;
        gObjVramCursor->Reset();
        gObjVramCursor->Reset();
        gSmallFont->SetTileBase(zero);
        IconReserveVram(gObjVramCursor, gSmallFont);
        {
            u32 base = gSmallFont->tileCount;

            gLargeFont->SetTileBase(base);
        }
        IconReserveVram(gObjVramCursor, gLargeFont);
    }
    gObjVramCursor->Mark();
}
