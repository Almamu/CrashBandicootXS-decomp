#include "menus.hpp"
#include "font.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "system.h"
#include <agb_syscall.h>
#include "text.h"
#include "util.h"
#include "level.h"
#include "globals.h"
#include "core.h"
}

/* The pause menu (PauseMenu, menus.hpp; docs/rom_map.md's "overlay_ui"
 * section). Run runs the whole screen to completion: frees the pending
 * heap bytes, stops the ambient sounds, clears palette color 0 and
 * DISPCNT, swaps gPaletteCache for a new palette cache with the menu's
 * palette in slot 15, sets up both fonts (each one's tiles uploaded, the
 * two fonts' tiles reserved in OBJ VRAM), builds the menu, runs its loop
 * (Loop, below) and deletes it, then restores the old palette
 * cache and returns the loop's result.
 *
 * The ROM builds the font's 0x12c offset again for each `tileCount` read:
 * reading it through the inline Font::GetTileCount stops CSE from sharing
 * it. The
 * two reads for the VRAM reservation are taken into locals before
 * `gObjVramCursor` is loaded, and the palette cache into a local before
 * `gPauseMenuPalette`'s address, for the ROM's load order. */

static inline void reserve_icon_vram(u32 n)
{
    gObjVramCursor->baseTile = n;
    gObjVramCursor->Reset();
}

s32 PauseMenu::Run()
{
    PaletteCache *oldCache;
    PauseMenu *screen;
    s32 result;

    mem_free_bytes(MEM_HEAP_BOTH);
    gAudioContext->StopAmbientSfx();
    WaitForVBlank();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;

    oldCache = gPaletteCache;
    gPaletteCache = (PaletteCache *)new PaletteCache;
    gPaletteCache->SetSource(gSpriteBankTable.paletteCount, gSpriteBankTable.palettes);
    gPaletteCache->ClaimSlot(0xf);
    {
        PaletteCache *cache = gPaletteCache;

        CpuSet(gPauseMenuPalette, cache->slots[15], 0x10);
    }

    gSmallFont->ResetPalette();
    gLargeFont->ResetPalette();
    gSmallFont->SetTileBase(0);
    gLargeFont->SetTileBase(gSmallFont->GetTileCount());
    {
        u32 a = gSmallFont->GetTileCount();
        u32 b = gLargeFont->GetTileCount();

        gObjVramCursor->baseTile = a + b;
        gObjVramCursor->Reset();
    }

    screen = new PauseMenu;
    result = screen->Loop();
    delete screen;

    reserve_icon_vram(0);
    delete gPaletteCache;
    gPaletteCache = oldCache;
    mem_free_bytes(MEM_HEAP_BOTH);
    return result;
}

/* The constructor: alpha blending at full fade (BLDY 16), BG0 and the
 * OBJs on, the background, the save block, the info pages (InitInfo),
 * the blinking eyes (sprite bank 0x228, at (238, 188) / 2), then the
 * rows (five in a time trial: "restart trial") and BG0's registers. */
PauseMenu::PauseMenu() : bg(0, 0x1f, 0, 3)
{
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
    bg.Load(&gPauseMenuBg);
    progress = gLevelState->PackSaveData();
    InitInfo();
    {
        UiSprite *s;

        blinkEyes = new UiSprite;
        s = blinkEyes;
        SetIconBank(s, 0x8a << 2);
        blinkEyes->palette = s->GetAnimPaletteSlot();
    }
    blinkEyes->SetPixelPos(119, 94);
    blinkTimer = (u16)RandRange(0x78) + 0x78;
    rows = gPauseMenuRows;
    cursor = 0;
    if (gLevelState->timeTrial != 0)
        rowCount = 5;
    else
        rowCount = 4;
    rowSpacing = 0x10;
    page = 0;
    pageTimer = 0xb4;
    REG_BG0CNT = bg.GetControl();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
}

/* The destructor deletes the icons: the eyes, the time trial's medal, the
 * relics', gems' and powers' icons and the crystal. Each loop has its own
 * counter: with one shared counter, the counter and the strength-reduced
 * pointer swap r4 and r5. */
PauseMenu::~PauseMenu()
{
    delete blinkEyes;
    delete trialIcon;
    for (s32 i = 0; i < 3; i++)
        delete relicIcons[i];
    for (s32 j = 0; j < 5; j++)
        delete gemIcons[j];
    for (s32 k = 0; k < 4; k++)
        delete powerIcons[k];
    delete crystalIcon;
}

/* PauseMenu::Loop (menus.hpp; C++ since the #664 cleanup), after the
 * destructor and before Animate (pause_menu_draw.cpp) in the ROM; it was
 * pause_menu_loop.cpp until #771. See docs/matching/archive/issue-7-0x08004d74-overlay-ui.md.
 *
 * The pause menu's input loop, redrawing every frame (Draw, CommitFrame,
 * Animate): fades in (BLDY's level down to 0), then up/down move the
 * cursor and left/right turn the selected row's volume down/up (once on
 * the press, then every 5 frames while held; `flashTimer`), A confirms a
 * row (the volume rows only play a cue) and START resumes (0). Then it
 * fades out (the level back up to 0x10), sets REG_DISPCNT's shadow to
 * just bit 6, commits it and returns the confirmed row's type (or 0).
 *
 * Matches under old_agbcp only (Makefile OLD_AGBCC_OBJS, old_agbcc as C;
 * agbcc is 4 bytes longer). The left/right tests read gKeys once, as a
 * held_pressed_pair copy `k`: the word load plus `lsr #16` for
 * `k.pressed`, and the ROM's two `mov #K` (the key's constant is
 * reloaded for the held test, the press test's copy being consumed by
 * its `and`). Reading KEYS.pressed directly gives `ldrh [keys+2]`
 * (#662 round 2; the C pinned `pressed` and `key` to r1/r3 for this).
 * The input loop is a plain `for (;;)` with the
 * START test at the bottom: the old compiler's rotation puts that test at
 * the loop top and enters at the body, as in the ROM. The fade pointer
 * (this+0xcc) that GCSE carries past the input loop into the fade-in
 * loop is inserted at the end of the block after the fade-out loop, so
 * `disp` is only taken after the fade-in loop; taken before the input
 * loop it lands ahead of that insertion
 * (docs/matching/archive/early-rom-naked-retry-2.md). */

/* gKeys as the {held, newly pressed} key-state pair (globals.h). */
#define KEYS (gKeys.half)

s32 PauseMenu::Loop()
{
    s32 result;
    union dispcnt *disp;

    while (bldy.evy != 0) {
        bldy.evy--;
        Draw();
        CommitFrame();
        Animate();
    }

    for (;;) {
        Draw();
        CommitFrame();
        Animate();
        UpdateKeys(gInput);
        if (KEYS.pressed & DPAD_UP) {
            CursorUp();
            flashTimer = 0x1e;
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        }
        if (KEYS.pressed & DPAD_DOWN) {
            CursorDown();
            flashTimer = 0x1e;
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        }
        {
            struct held_pressed_pair k = KEYS;
            if (k.pressed & DPAD_LEFT) {
                VolumeDown();
                flashTimer = 0x1e;
            } else if (k.held & DPAD_LEFT) {
                if (flashTimer == 0) {
                    VolumeDown();
                    flashTimer = 5;
                } else {
                    flashTimer--;
                }
            }
        }
        {
            struct held_pressed_pair k = KEYS;
            if (k.pressed & DPAD_RIGHT) {
                VolumeUp();
                flashTimer = 0x1e;
            } else if (k.held & DPAD_RIGHT) {
                if (flashTimer == 0) {
                    VolumeUp();
                    flashTimer = 5;
                } else {
                    flashTimer--;
                }
            }
        }
        if (KEYS.pressed & A_BUTTON) {
            result = rows[cursor].type;
            if ((u32)(result - 4) <= 1) {
                gAudioContext->PlaySfx(SFX_MENU_ERROR, 0x100);
            } else {
                gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
                break;
            }
        }
        if (KEYS.pressed & START_BUTTON) {
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
            result = 0;
            break;
        }
    }
    while (bldy.evy != 0x10) {
        bldy.evy++;
        Draw();
        CommitFrame();
        Animate();
    }
    disp = &dispcnt;
    disp->raw = 0;
    disp->bits.objMap1D = 1;
    CommitFrame();
    return result;
}
