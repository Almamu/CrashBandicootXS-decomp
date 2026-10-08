#include "sprite_obj.hpp"
#include "frontend.hpp"

extern "C" {
#include "match.h"
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* Middle part of GitHub issue #65's chunk (0x08035D1C-0x0803686C): the
 * title screen's other methods (TitleScreen: the cheat input, the run
 * loop, the menu, the destructor) and four of the company logos'
 * (CompanyLogos: Run, the VV logo's graphics and pieces), C++ since #664
 * part 10c-2 (include/frontend.hpp). Split off title_screen_init.cpp at
 * `TitleScreenCheatInput` in the issues #64/#65 second NAKED retry.
 *
 * old_agbcp (OLD_AGBCC_OBJS), as its C was old_agbcc. Its C was built
 * with -fno-strength-reduce, for InitVvLogoPieces's up-counting loop; as
 * C++ the plain indexed loop matches with strength reduction on, and so
 * does the rest of the file, so the flag went (#664 part 10c-2; see
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
        PlaySong(gAudioContext, SONG_MAIN_MENU_JAPAN);
        cheatHash = 0;
    }
    return 0;
}

/* Runs the title screen: starts the nine pieces' motions and moves them
 * until the first one's ends, then shows the menu (up/down, A or START
 * to choose), fades out and returns the choice: 0 new game, 1 load, 2
 * the credits.
 *
 * Closed in the issues #64/#65 second NAKED retry. Three loop shapes:
 * - The seed loop is a `goto` loop (nothing hoisted), as in
 *   ResetLogoPieces. Two extra `i` references (empty asms) give `i` the
 *   first free low register (r3) ahead of `slot`/`stride`. Both
 *   address sums compute the scaled index first (`off`), and the
 *   `-1` store adds it second (`base + off`).
 * - The menu loop is a real `for (;;)`, so `&gAudioContext` is
 *   hoisted into r6. Leaving it with `goto fadeLoop` instead of `break`
 *   keeps jump.c from rotating it around the `pressed & 9` exit.
 * - The fade loop is a `goto` loop again (its register addresses are
 *   reloaded each pass) with its own counter, so the seed loop's `i`
 *   does not cross calls.
 * `pressed` is loaded into its own variable first (the ROM's
 * `ldrh r5` / `add r1, r5, #0`). The `cheatHash` zero is a local, so it is
 * materialized before the `1`. */
s32 TitleScreen::Run()
{
    s32 i;
    const struct slot_seed *seedBase;
    const struct slot_seed *seed;
    u8 *slot;
    s32 stride;
    s32 zero;
    u32 pressed;
    s32 fade;

    i = 0;
    seedBase = gTitleLogoPieceSeeds;
    seed = seedBase;
    slot = (u8 *)this;
    stride = 0;
seedLoop:
    zero = 0;
    slot[offsetof(TitleScreen, pieces[0].active)] = zero;
    {
        u8 *countdownBase = (u8 *)&pieces[0].countdown;
        s32 *dst = (s32 *)(countdownBase + stride);
        s32 off = i << 3;
        u32 holdBase = (u32)&seedBase->hold;

        *dst = *(s32 *)(off + holdBase) + 1;
    }
    {
        u8 *recordBase = (u8 *)&pieces[0].record;

        *(const struct delta_record **)(recordBase + stride) = seed->record;
    }
    {
        s32 off = i << 2;
        u32 base = (u32)landTimer;

        *(s32 *)(base + off) = -1;
    }
    seed++;
    slot += sizeof(LogoPiece);
    stride += sizeof(LogoPiece);
    i++;
    MATCH_USE(i);
    MATCH_USE(i);
    if (i <= 8)
        goto seedLoop;
    shake = zero;
    menuShown = zero;
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
            PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
            fade = 0;
            goto fadeLoop;
        }
        if (pressed & DPAD_UP) {
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
            if (selection != 0)
                selection--;
            else
                selection = 2;
        }
        if (pressed & DPAD_DOWN) {
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
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
 * gTitleLogoPieceSeeds and clears `shake`.
 *
 * Closed in the issues #64/#65 second NAKED retry. The loop is a
 * hand-written `goto` loop (no loop notes), so nothing is hoisted, as in
 * the ROM. The rest is global-alloc priority:
 * - `stride` starts from a constant-init (`MATCH_CONST(stride, 0)`). A plain
 *   `stride = 0` makes local-alloc double its live length, which drops
 *   it below `slot` (r5/r4 swapped).
 * - One extra `this` reference in the loop and `seedBase`/`zero`
 *   references after it (empty asms, no code) lift those three to the
 *   ROM's r3/r8/ip.
 * - `off = i << 3` computed before `holdBase` (as an integer) gives the
 *   ROM's `lsl` first, `add r0, r0, r1` order. */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void TitleScreen::ResetLogoPieces()
{
    s32 i;
    const struct slot_seed *seedBase;
    s32 *counter;
    const struct slot_seed *seed;
    u8 *slot;
    s32 stride;
    s32 zero;

    i = 0;
    seedBase = gTitleLogoPieceSeeds;
    counter = landTimer;
    seed = seedBase;
    slot = (u8 *)this;
    MATCH_CONST(stride, 0);
loop:
    zero = 0;
    slot[offsetof(TitleScreen, pieces[0].active)] = zero;
    {
        u8 *countdownBase = (u8 *)&pieces[0].countdown;
        s32 *dst = (s32 *)(countdownBase + stride);
        s32 off = i << 3;
        u32 holdBase = (u32)&seedBase->hold;

        *dst = *(s32 *)(off + holdBase) + 1;
    }
    {
        u8 *recordBase = (u8 *)&pieces[0].record;

        *(const struct delta_record **)(recordBase + stride) = seed->record;
    }
    *counter++ = -1;
    seed++;
    slot += sizeof(LogoPiece);
    stride += sizeof(LogoPiece);
    i++;
    MATCH_USE(this);
    if (i <= 8)
        goto loop;
    MATCH_USE(seedBase);
    MATCH_USE(zero);
    shake = zero;
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

/* Runs the company logos: the logo actor (Crash), the VV logo's pieces
 * over the starfield with a 60-frame fade-in, the Universal logo zooming
 * in on BG2 (A or START skips ahead) and fading out, then the VV logo
 * until its pieces have flown off, and frees everything.
 *
 * The zoom-in's decrement/grow/shrink is written as "step the counter,
 * then test it again" (which gives the ROM's block order), and the
 * affine X/Y values are computed before either register store. The
 * fade-out value is pinned to r1 (register allocation, as in the C: the
 * ROM's choice; unpinned it lands in r2 and costs a copy). */
void CompanyLogos::Run()
{
    LogoActor *part;
    Starfield *bg;
    s32 i;
    s32 scale;

    InitObjTileFreeList(OBJ_VRAM0);
    InitSpriteFrameOamQueue();
    InitSpriteFrameCache();
    part = new LogoActor(&gLogoActorAnim);
    {
        struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)gPolarCategoryPalette;
        dma->dst = OBJ_PLTT;
        dma->cnt = 0x80000100;
        dma->cnt;
    }
    LoadVvLogoGraphics();
    InitVvLogoPieces();
    bg = new Starfield;
    LoadUniversalLogoBg();
    for (i = 0; i <= 0x3b; i++) {
        if (i <= 0x10) {
            REG_BLDCNT = 0xff;
            REG_BLDY = 0x10 - i;
        } else {
            *(vu32 *)REG_ADDR_BLDCNT = 0;
        }
        WaitForVBlank();
        bg->Update();
    }
    PlaySfx(gAudioContext, SFX_UNIVERSAL_LOGO_IN, 0x100);
    scale = 0x2000;
    fade = -1;
    do {
        s32 v;
        s32 q;

        UpdateKeys(gInput);
        if (gKeys.half.pressed & (A_BUTTON | START_BUTTON)) {
            if (fade > 0x40)
                fade = 0x40;
        }
        WaitForVBlank();
        CommitDispcnt();
        if (fade != -1) {
            if (fade == 0x40)
                PlaySfx(gAudioContext, SFX_UNIVERSAL_LOGO_OUT, 0x100);
            v = fade;
            if (v <= 0x40) {
                s32 a = v >> 2;
                REG_BLDCNT = 0x3f7f;
                REG_BLDALPHA = a | ((0x10 - a) << 8);
            }
            fade = v - 1;
        }
        if (fade == -1) {
            if (scale > 0xffff || (scale += 0x600) > 0xffff) {
                scale = 0x10000;
                if (fade == -1)
                    fade = 0xf4;
            }
        } else if (fade <= 0x40) {
            scale = Q8_MUL(scale, 0x118);
        }
        q = 0x1000000 / scale;
        {
            s32 x = -(q * 120) + 0x7800;
            s32 y = -(q * 80) + 0x5000;
            REG_BG2X = x;
            REG_BG2Y = y;
        }
        REG_BG2PA = q;
        REG_BG2PD = q;
        REG_BG2PB = 0;
        REG_BG2PC = 0;
        bg->Update();
    } while (fade != 0);
    ((struct dispcnt_bits *)gDispcnt)->bg2 = 0;
    ((struct dispcnt_bits *)gDispcnt)->obj = 1;
    ((struct dispcnt_bits *)gDispcnt)->objMap1D = 1;
    CommitDispcnt();
    *(vu32 *)REG_ADDR_BLDCNT = 0;
    fade = -1;
    timer = -1;
    while (fade != 0) {
        MATCH_HOLD_REG(s32, v, r1);

        UpdateKeys(gInput);
        if (gKeys.half.pressed & (A_BUTTON | START_BUTTON)) {
            if (timer > 0)
                timer = 1;
        }
        part->Update();
        part->Draw();
        gOamBuffer->Rewind();
        FlushSpriteFrameOamQueue();
        UpdateVvLogoPieces();
        DrawVvLogoPieces();
        bg->Update();
        WaitForVBlank();
        v = fade;
        if (v > 0x10) {
            s32 n = v - 1;
            s32 a;

            fade = n;
            a = v - 0x12;
            REG_BLDCNT = 0x3f7f;
            REG_BLDALPHA = (0x10 - a) | (a << 8);
            if (n == 0x11) {
                fade = -1;
                *(vu32 *)REG_ADDR_BLDCNT = 0;
            }
        } else if (v >= 0) {
            v--;
            fade = v;
            REG_BLDY = 0x10 - v;
            REG_BLDCNT = 0xff;
        }
        gOamBuffer->Commit();
        FlushVramDmaQueue();
        AgeSpriteFrameCache();
    }
    delete part;
    delete bg;
    delete[] scratch;
    delete[] frames;
    FreeSpriteFrameCache();
    FreeSpriteFrameOamQueue();
    FreeObjTileFreeList();
    FreeCategorySpriteSheet();
}

/* Reserves the three OBJ VRAM tile blocks and loads the VV logo's three
 * palettes (gVvLogoEmblemObj's, gVvLogoLettersObj's, gVvLogoUrlObj's)
 * to OBJ palettes 15, 14 and 13, the letters' and the URL's tiles to
 * the first two blocks, and the emblem's frames to `frames`; allocates
 * `scratch`. The frame strip's size is a local, computed before the
 * allocation (the ROM's order). */
void CompanyLogos::LoadVvLogoGraphics()
{
    tilesA = (u32)AllocVramTileBlock(0x1200);
    tilesB = (u32)AllocVramTileBlock(0x400);
    tilesC = (u32)AllocVramTileBlock(0x1000);
    LoadAssetBuffered(gVvLogoEmblemObj.paletteAsset, (void *)(PLTT + 0x3E0));
    LoadAssetBuffered(gVvLogoLettersObj.paletteAsset, (void *)(PLTT + 0x3C0));
    LoadAssetBuffered(gVvLogoUrlObj.paletteAsset, (void *)(PLTT + 0x3A0));
    LoadAssetBuffered(gVvLogoLettersObj.tileAsset, (void *)tilesA);
    LoadAssetBuffered(gVvLogoUrlObj.tileAsset, (void *)tilesB);
    {
        u32 size = *(u32 *)gVvLogoEmblemObj.tileAsset >> 8;
        u8 *buf;

        frames = buf = new u8[size];
        LoadTaggedAsset(gVvLogoEmblemObj.tileAsset, buf);
    }
    scratch = new u8[0x1000];
}

/* Starts the 20 pieces' motions from gVvLogoPieceSeeds, sets every
 * sound-cue flag and rewinds the frame strip.
 *
 * Matches only because this object is built with -fno-strength-reduce
 * (see NO_STRENGTH_REDUCE_OBJS in the Makefile and
 * docs/matching/per-file-flags-investigation.md): with strength
 * reduction on, gcc's loop optimizer reverses the first loop into a
 * count-down (its counter is only used by the exit test) while the ROM
 * keeps `i` counting up. The pointer walks are the source's own - with
 * strength reduction off nothing would have produced them. The second
 * loop's `1` lives in a local assigned before its counter and pointer
 * (the ROM materializes it first). */
void CompanyLogos::InitVvLogoPieces()
{
    s32 i;

    for (i = 0; i <= 0x13; i++) {
        slots[i].active = 0;
        slots[i].countdown = gVvLogoPieceSeeds[i].hold + 1;
        slots[i].record = gVvLogoPieceSeeds[i].record;
    }
    {
        u8 one = 1;
        s32 j = 0x11;
        u8 *flags = &sfxPending[0x11];

        for (; j >= 0; j--)
            *flags-- = one;
    }
    frame = 0;
    loops = 0;
    frameTick = 0;
}

/* Moves the 20 pieces (as TitleScreen::UpdateLogoPieces, without the
 * header) while `timer` is -1, setting it to -2 when a piece's motion
 * has ended, and advances the emblem's frame strip (4 ticks a frame, 10
 * frames, twice). Then, 240 frames later (a sound), flies every piece
 * off upwards; once all are gone it starts the fade (`fade` 0x10). */
void CompanyLogos::UpdateVvLogoPieces()
{
    s32 i;

    if (timer == -1) {
        for (i = 0; i <= 0x13; i++) {
            if (slots[i].countdown != 0) {
                s32 countdown = slots[i].countdown - 1;

                slots[i].countdown = countdown;
                if (countdown == 0) {
                    const struct delta_record *record = slots[i].record++;

                    slots[i].active = 1;
                    countdown = record->hold;
                    slots[i].countdown = countdown;
                    if (countdown != 0) {
                        slots[i].posC = INT_TO_Q16(record->dPosC);
                        slots[i].deltaC = record->deltaC;
                        slots[i].velA = INT_TO_Q8(record->dVelA);
                        slots[i].deltaD = record->deltaD;
                        slots[i].velB = INT_TO_Q8(record->dVelB);
                        slots[i].deltaE = record->deltaE;
                        slots[i].posA.q = INT_TO_Q16(record->dPosA);
                        slots[i].deltaA = record->deltaA;
                        slots[i].posB.q = INT_TO_Q16(record->dPosB);
                        slots[i].deltaB = record->deltaB;
                    }
                } else {
                    slots[i].posC += slots[i].deltaC;
                    slots[i].velA += slots[i].deltaD;
                    slots[i].velB += slots[i].deltaE;
                    slots[i].posA.q += slots[i].deltaA;
                    slots[i].posB.q += slots[i].deltaB;
                }
            } else {
                timer = -2;
            }
        }
        if (loops <= 1) {
            if (++frameTick > 3) {
                frameTick = 0;
                if (++frame > 9) {
                    frame = 0;
                    ++loops;
                }
            }
        }
    }
    if (timer == -2)
        timer = 0xf0;
    if (timer > 0) {
        if (--timer != 0)
            return;
        PlaySfx(gAudioContext, SFX_UNKNOWN_50, 0x100);
    }
    if (timer == 0) {
        s32 allDone = 1;
        LogoPiece *slot;
        LogoPiece *end;

        slot = slots;
        end = &slots[19];
        do {
            if (slot->active != 0) {
                s32 y;

                allDone = 0;
                y = slot->posA.q - 0x80000;
                slot->posA.q = y;
                if (y < -0x7f0000)
                    slot->active = allDone;
            }
            slot++;
        } while ((s32)slot <= (s32)end);
        if (allDone) {
            fade = 0x10;
            timer = -3;
        }
    }
}
