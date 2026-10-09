#include "sprite_obj.hpp"
#include "frontend.hpp"
#include "audio.hpp"

extern "C" {
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "system.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* Middle part of GitHub issue #65's chunk (0x08035D1C-0x080361B0): the
 * title screen's other methods (TitleScreen: the cheat input, the run
 * loop, the menu, the destructor), C++ since #664 part 10c-2
 * (include/frontend.hpp). Split off title_screen_init.cpp at
 * `TitleScreenCheatInput` in the issues #64/#65 second NAKED retry. The
 * four company-logo methods that follow it in the ROM are at the head of
 * company_logos.cpp (here until #770).
 *
 * old_agbcp (OLD_AGBCC_OBJS), as its C was old_agbcc. Its C was built
 * with -fno-strength-reduce, for InitVvLogoPieces's up-counting loop
 * (then in this file); as C++ the plain indexed loop matches with
 * strength reduction on, and so does the rest of the file, so the flag
 * went (#664 part 10c-2; see
 * docs/matching/per-file-flags-investigation.md). */

/* Folds `val` into the cheat hash: XOR, rotate left by one, times 521
 * (CheatInput's, and HashCheatInput's body). The rotate's left shift is
 * `v * 2`: as `v << 1`, combine makes the pair a `ror`, which the ROM
 * doesn't have (the C pinned four registers and kept one with asm). */
inline void TitleScreen::HashInput(u32 val)
{
    u32 *slot = &cheatHash;
    u32 v = *slot ^ val;
    u32 rotated = (v * 2) | (v >> 31);

    *slot = (((rotated << 6) + rotated) << 3) + rotated;
}

/* The cheat code: while R is held, each press of a direction, B, A or
 * START folds a constant into `cheatHash`, and the sequence whose hash
 * is 0x3034AF3B plays the Japanese title music. Returns `pressed`
 * without R, and nothing with it (the menu ignores the keys).
 *
 * Copying the whole held/pressed pair into a local struct first is what
 * makes the ROM build the 0x100 mask in r4 and copy it to r1 (the issue
 * #64/#65 NAKED retry). HashInput is an inline method, so each branch
 * loads its constant before `&cheatHash`, and cross-jumping shares the
 * rest. */
u32 TitleScreen::CheatInput(u32 pressed)
{
    struct held_pressed_pair input = gKeys.half;

    if (!(input.held & R_BUTTON)) {
        cheatHash = 0;
        return pressed;
    }
    if (pressed & DPAD_LEFT)
        HashInput(0x12345678);
    else if (pressed & DPAD_RIGHT)
        HashInput(0x31415926);
    else if (pressed & DPAD_UP)
        HashInput(0xC0DEBA1D);
    else if (pressed & DPAD_DOWN)
        HashInput(0xDEADBEEF);
    else if (pressed & B_BUTTON)
        HashInput(0xB1E4B1E4);
    else if (pressed & A_BUTTON)
        HashInput(0x71839406);
    else if (pressed & START_BUTTON)
        HashInput(0x828A048B);
    if (cheatHash == 0x3034AF3B) {
        gAudioContext->PlaySong(SONG_MAIN_MENU_JAPAN);
        cheatHash = 0;
    }
    return 0;
}

/* Starts the nine pieces' motions from gTitleLogoPieceSeeds and clears
 * `shake` (Run's first step, inlined there).
 *
 * #662 round 3: inlined, the loop's giv choices differ from the same
 * loop compiled on its own (ResetLogoPieces): loop.c reduces the
 * piece pointer, the 0x34 stride and the seed pointer but leaves
 * `landTimer[i]` as `this + 0x1e4 + (i << 2)`, which is Run's ROM code.
 * Written out in Run, the `-1` store's address is strength-reduced too
 * (r9), as in ResetLogoPieces; the C had a hand-written `goto` loop with
 * two `MATCH_USE(i)`s in its place. */
inline void TitleScreen::SeedLogoPieces()
{
    s32 i;

    for (i = 0; i <= 8; i++) {
        pieces[i].active = 0;
        pieces[i].countdown = gTitleLogoPieceSeeds[i].hold + 1;
        pieces[i].record = gTitleLogoPieceSeeds[i].record;
        landTimer[i] = -1;
    }
    shake = 0;
}

/* Runs the title screen: starts the nine pieces' motions and moves them
 * until the first one's ends, then shows the menu (up/down, A or START
 * to choose), fades out and returns the choice: 0 new game, 1 load, 2
 * the credits.
 *
 * Closed in the issues #64/#65 second NAKED retry. Three loop shapes:
 * - The seed loop is SeedLogoPieces, inlined (see there).
 * - The menu loop is a real `for (;;)`, so `&gAudioContext` is
 *   hoisted into r6. Leaving it with `goto fadeLoop` instead of `break`
 *   keeps jump.c from rotating it around the `pressed & 9` exit.
 * - The fade loop is a `goto` loop (its register addresses are
 *   reloaded each pass) with its own counter.
 * `pressed` is loaded into its own variable first (the ROM's
 * `ldrh r5` / `add r1, r5, #0`). The `cheatHash` zero is a local, so it is
 * materialized before the `1`. */
s32 TitleScreen::Run()
{
    u32 pressed;
    s32 fade;

    SeedLogoPieces();
    menuShown = 0;
    while (pieces[0].countdown != 0) {
        UpdateLogoPieces();
        Draw();
        starfield->Update();
        WaitForVBlank();
        *(vu32 *)REG_ADDR_BLDCNT = 0;
        CommitFrame();
    }
    {
        u32 z = 0;
        menuShown = 1;
        cheatHash = z;
    }
    for (;;) {
        Draw();
        starfield->Update();
        UpdateKeys(gInput);
        pressed = gKeys.half.pressed;
        pressed = CheatInput(pressed);
        if (pressed & (A_BUTTON | START_BUTTON)) {
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
            fade = 0;
            goto fadeLoop;
        }
        if (pressed & DPAD_UP) {
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
            if (selection != 0)
                selection--;
            else
                selection = 2;
        }
        if (pressed & DPAD_DOWN) {
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
            selection++;
            selection = selection % 3;
        }
        WaitForVBlank();
        CommitFrame();
    }
fadeLoop:
    Draw();
    starfield->Update();
    WaitForVBlank();
    REG_BLDY = fade;
    REG_BLDCNT = 0xff;
    CommitFrame();
    fade++;
    if (fade <= 0x10)
        goto fadeLoop;
    return selection;
}

/* Writes the BG2 logo's affine scroll and scale (DrawLogoPieces's),
 * then commits DISPCNT and the OAM buffer. The ROM derives
 * `REG_BG2PA`'s address from `REG_BG2Y`'s (`-0xc`); that falls out of
 * writing the PA store as a chained assignment (`REG_BG2PA = scale =
 * ...`), which makes the address get computed before the load. */
void TitleScreen::CommitFrame()
{
    s32 scale;

    REG_BG2X = bgX;
    REG_BG2Y = bgY;
    REG_BG2PA = scale = bgScale;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = scale;
    CommitDispcnt();
    gOamBuffer->Commit();
}

/* Draws menu item `item` (`text`, a GetUiText string) centred at row
 * 0x80 + 10 * item: blinking between palettes 14 and 15 when it is
 * selected, palette 13 otherwise. */
void TitleScreen::DrawMenuItem(s32 text, s32 item)
{
    Font *m;
    s32 x;

    if (item == selection) {
        s32 count = blinkCounter + 1;

        blinkCounter = count;
        font->SetPalette(((count >> 2) & 1) + 0xe);
    } else {
        font->SetPalette(0xd);
    }
    x = (0xf0 - font->MeasureText((u8 *)text)) >> 1;
    m = font;
    m->SetPos(x, item * 10 + 0x80);
    m->DrawText((u8 *)text);
}

/* Builds the frame's OAM: the logo pieces, and the menu once it is
 * shown. */
void TitleScreen::Draw()
{
    gOamBuffer->Reset();
    DrawLogoPieces();
    if (menuShown != 0) {
        DrawMenuItem(GetUiText(0x1a), 0);
        DrawMenuItem(GetUiText(0x1b), 1);
        DrawMenuItem(GetUiText(0x3b), 2);
    }
    gOamBuffer->HideUnused();
}

/* CheatInput's hash step on its own. */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void TitleScreen::HashCheatInput(u32 val)
{
    HashInput(val);
}

/* Run's seed loop on its own: starts the nine pieces' motions from
 * gTitleLogoPieceSeeds and clears `shake`. SeedLogoPieces's body, but
 * compiled on its own rather than inlined, where loop.c also
 * strength-reduces the `landTimer[i]` store (the ROM's `stmia` on r9).
 *
 * #662 round 3: the plain indexed loop. The C was a `goto` loop over
 * hand-kept byte offsets with a `MATCH_CONST` and three `MATCH_USE`s for
 * the allocation. */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void TitleScreen::ResetLogoPieces()
{
    s32 i;

    for (i = 0; i <= 8; i++) {
        pieces[i].active = 0;
        pieces[i].countdown = gTitleLogoPieceSeeds[i].hold + 1;
        pieces[i].record = gTitleLogoPieceSeeds[i].record;
        landTimer[i] = -1;
    }
    shake = 0;
}

/* Deletes the starfield, blanks DISPCNT and the BG palettes and fades
 * the screen to black. The palette clear needs its zero in a local
 * assigned before the pointer (the ROM materializes it first). */
TitleScreen::~TitleScreen()
{
    s32 i;
    u16 *pal;
    s32 zero;

    delete starfield;
    *(u16 *)gDispcnt = 0;
    CommitDispcnt();
    zero = 0;
    pal = (u16 *)PLTT;
    for (i = 0xff; i >= 0; i--)
        *pal++ = zero;
    REG_BLDCNT = 0xff;
    REG_BLDY = 0x10;
}
