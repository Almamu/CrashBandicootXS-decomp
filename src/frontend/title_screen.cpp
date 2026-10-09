#include "sprite_obj.hpp"
#include "frontend.hpp"
#include "audio.hpp"
#include "key_input.hpp"

extern "C" {
#include "gba/dma_macros.h"
#include "graphics_package.h"
#include "gba/io_reg.h"
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "system.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* The title screen (TitleScreen, #664 part 10c-2, include/frontend.hpp):
 * its constructor, graphics loaders and logo pieces' update and draw,
 * then the cheat input, the run loop, the menu and the destructor. The
 * first half was title_screen_init.cpp until #771.
 *
 * GitHub issue #65's chunk (0x080354E0-0x08037110) starts here. See
 * docs/matching/archive/issue-65-graphics-loading.md.
 *
 * old_agbcp (OLD_AGBCC_OBJS), as its C was old_agbcc, with the default
 * -O2 strength reduction: DrawLogoPieces's up-counting inner loop is
 * reversed by it, as in the ROM (the second half's C was built without
 * it, see below). See docs/matching/archive/issue-64-65-naked-retry-2.md. */

/* codegen: RandRange returns u16 (util.h), but DrawLogoPieces was matched
 * against an s32 return: with the u16 prototype its two stack slots
 * swap. docs/headers_plan.md */
extern "C" s32 RandRange_s32(s32 max) asm("RandRange");

/* Blanks the screen, loads the menu font's palettes and the logo's
 * graphics, starts the starfield and the title music. */
TitleScreen::TitleScreen()
{
    struct dma_regs *dma;

    font = gSmallFont;
    gOamBuffer->Reset();
    gOamBuffer->HideUnused();
    WaitForVBlank();
    gOamBuffer->Commit();

    *(vu32 *)REG_ADDR_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_BLDY = 0x10;
    REG_DISPCNT = 0;

    font->SetPalette(0xe);
    font->SetTileBase(0x200);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)gTitleMenuPalette;
    dma->dst = OBJ_PLTT + 13 * PALETTE_SIZE_16;
    dma->cnt = 0x80000010;
    dma->cnt;
    dma->src = (u32)gTitleMenuSelectedPalette;
    dma->dst = OBJ_PLTT + 14 * PALETTE_SIZE_16;
    dma->cnt = 0x80000010;
    dma->cnt;
    dma->src = (u32)gTitleMenuBlinkPalette;
    dma->dst = OBJ_PLTT + 15 * PALETTE_SIZE_16;
    dma->cnt = 0x80000010;
    dma->cnt;

    LoadBg();
    LoadObjTiles();
    starfield = new Starfield;

    SetObjMapping1D();
    ShowObj();
    SetDispcntMode(1);
    CommitDispcnt();

    selection = 0;
    blinkCounter = 0;

    gAudioContext->StartSong(SONG_MAIN_MENU_EUROPE);
}

/* Loads BG2's palette, tiles and map from gTitleScreenBg, packing the
 * map's 16-bit entries (the tile number in the low byte) into the bytes
 * of the 8-bit affine screen at 0x0600F000, and sets up BG2.
 *
 * `bg2cnt` is a BGnCNT union (a word on the stack): clearing its raw
 * halfword is the ROM's `& 0xFFFF0000`. The indexed `map[i]`/`map[i + 1]` reads are strength-reduced into
 * the ROM's walk pointer, with `map` kept in r8 for the free. */
void TitleScreen::LoadBg()
{
    const struct bg_package *pkg = &gTitleScreenBg;
    u16 *map;
    u16 *dest;
    s32 i;
    union bgcnt bg2cnt;

    LoadTaggedAsset(pkg->paletteAsset, (void *)BG_PLTT);
    LoadTaggedAsset(pkg->tileAsset, (void *)BG_CHAR_ADDR(2));
    map = new u16[(s32)pkg->height * (s32)pkg->width];
    LoadTaggedAsset(pkg->mapAsset, map);
    dest = (u16 *)BG_SCREEN_ADDR(30);
    for (i = 0; i < (s32)pkg->height * (s32)pkg->width; i += 2) {
        *dest = (map[i] & 0xff) | ((map[i + 1] & 0xff) << 8);
        dest++;
    }
    bg2cnt.raw = 0;
    bg2cnt.bits.charBase = 2;
    bg2cnt.bits.screenBase = 30;
    bg2cnt.bits.colorMode = 1;
    bg2cnt.bits.priority = 1;
    REG_BG2CNT = bg2cnt.raw;
    delete[] map;
}

/* Loads the four OBJ packages of gTitleObjPackages (CRASH, the two
 * arrows, BANDICOOT): each one's palette to the next OBJ palette, and
 * its tiles to OBJ VRAM in map order, a DMA per map entry (the entry's
 * low byte picks the tile). */
void TitleScreen::LoadObjTiles()
{
    const struct bg_package *const *pkg = (const struct bg_package *const *)gTitleObjPackages;
    u8 *tileDest = (u8 *)OBJ_VRAM0;
    u8 *paletteDest = (u8 *)OBJ_PLTT;
    s32 pass;

    for (pass = 0; pass <= 3; pass++) {
        u8 *palette;
        u8 *tiles;
        u16 *map;
        s32 count;

        palette = new u8[*(u32 *)(*pkg)->paletteAsset >> 8];
        LoadTaggedAsset((*pkg)->paletteAsset, palette);
        DmaCopy16(3, palette, paletteDest, PALETTE_SIZE_16);
        paletteDest += PALETTE_SIZE_16;
        delete[] palette;

        tiles = new u8[*(u32 *)(*pkg)->tileAsset >> 8];
        LoadTaggedAsset((*pkg)->tileAsset, tiles);

        count = (*pkg)->height * (*pkg)->width;
        map = new u16[count];
        LoadTaggedAsset((*pkg)->mapAsset, map);
        {
            s32 i;

            for (i = 0; i < count; i++) {
                DmaCopy16(3, tiles + ((map[i] & 0xff) << 5), tileDest, TILE_SIZE_4BPP);
                tileDest += TILE_SIZE_4BPP;
            }
        }

        delete[] map;
        delete[] tiles;
        pkg++;
    }
}

/* Moves the nine pieces one frame: a piece whose step has frames left
 * adds its deltas; one whose step just ran out loads the next step
 * (activating the piece), until a step with a 0 hold ends its motion.
 * The ROM recomputes `this + field + i * 0x34` at every access. */
void TitleScreen::UpdateLogoPieces()
{
    s32 i;

    for (i = 0; i <= 8; i++) {
        if (pieces[i].countdown != 0) {
            s32 countdown = pieces[i].countdown - 1;

            pieces[i].countdown = countdown;
            if (countdown == 0) {
                const struct delta_record *record = pieces[i].record++;

                pieces[i].active = 1;
                countdown = record->hold;
                pieces[i].countdown = countdown;
                if (countdown != 0) {
                    pieces[i].posC = INT_TO_Q16(record->dPosC);
                    pieces[i].deltaC = record->deltaC;
                    pieces[i].velA = INT_TO_Q8(record->dVelA);
                    pieces[i].deltaD = record->deltaD;
                    pieces[i].velB = INT_TO_Q8(record->dVelB);
                    pieces[i].deltaE = record->deltaE;
                    pieces[i].posA.q = INT_TO_Q16(record->dPosA);
                    pieces[i].deltaA = record->deltaA;
                    pieces[i].posB.q = INT_TO_Q16(record->dPosB);
                    pieces[i].deltaB = record->deltaB;
                }
            } else {
                pieces[i].posC += pieces[i].deltaC;
                pieces[i].velA += pieces[i].deltaD;
                pieces[i].velB += pieces[i].deltaE;
                pieces[i].posA.q += pieces[i].deltaA;
                pieces[i].posB.q += pieces[i].deltaB;
            }
        }
    }
}

static inline void SetAffine(OamBuffer *buf, s32 m, u16 pa, u16 pb, u16 pc, u16 pd)
{
    s32 idx = m * 4;

    buf->table[idx].attr[3] = pa;
    buf->table[idx + 3].attr[3] = pd;
    buf->table[idx + 1].attr[3] = pb;
    buf->table[idx + 2].attr[3] = pc;
}

/* Draws the logo: piece 8 (the TM, three 32x16 entries, affine when
 * scaled), the CRASH and BANDICOOT letters (pieces 7 down to 3, 64x64
 * each, affine while scaled, a sound when each lands), the two arrows
 * (pieces 0 and 1, eight 32x32 entries each from
 * gTitleArrowPieceOffsets, a sound and the BG2 shake when they land), and
 * piece 2, the BG2 logo, whose scale and (shaken) position it computes
 * for CommitFrame.
 *
 * Closed in the issues #64/#65 second NAKED retry (old_agbcc, strength
 * reduction on):
 * - The offset-table loop is written up-counting (`j = 0; j < 4`).
 *   Strength reduction reverses it (check_dbra_loop) and emits the
 *   `j = 3` start after the hoisted invariants, as in the ROM.
 * - CLEAR_OAM is a macro that stores `zero` before loading the DMA base.
 *   The second slot loop does its DMA through `dma2`, set before the
 *   loop, so loop.c hoists it into r9 and the table pointer is spilled.
 * - The counter addresses compute the scaled index before
 *   `this + 0x1e4`. `px + dx` locals stop fold from reassociating the
 *   -0x20. The random shake keeps `a - 5` as its own local after the
 *   call.
 * - The second loop has its own counter `k`, and `matrixLo = matrix`
 *   lets the bitfield store do the `& 7`. */
void TitleScreen::DrawLogoPieces()
{
    u16 zero;
    s32 matrix = 0;
    s32 i;
    s32 k;
    struct oam_attrs oamA;
    struct oam_attrs oamB;
    struct oam_attrs oamC;

    {
        LogoPiece *slot = &pieces[8];

        if (slot->active) {
            u16 scale = 0x1000000 / slot->velA;
            SetAffine(gOamBuffer, matrix, scale, 0, 0, scale);
            CLEAR_OAM(&oamA);
            oamA.affineMode = 3;
            oamA.matrixLo = 0;
            oamA.palette = 3;
            oamA.size = 2;
            oamA.shape = 1;
            oamA.y = Q16_TO_INT(slot->posB.q) - 16;
            oamA.tileNum = 0x1c0;
            oamA.x = Q16_TO_INT(slot->posA.q) - 0x20 - Q16_TO_INT(slot->velA << 5);
            gOamBuffer->Add(&oamA);
            oamA.tileNum += 8;
            oamA.x = Q16_TO_INT(slot->posA.q) - 0x20;
            gOamBuffer->Add(&oamA);
            oamA.tileNum += 8;
            {
                s32 px = Q16_TO_INT(slot->posA.q);
                s32 dx = Q16_TO_INT(slot->velA << 5) - 0x20;

                oamA.x = px + dx;
            }
            gOamBuffer->Add(&oamA);
            matrix = 2;
        }
    }
    for (i = 0; i <= 4; i++) {
        LogoPiece *slot = &pieces[7] - i;

        if (slot->active) {
            s32 *cnt;
            s32 d;
            u16 scale;
            s32 off;

            {
                u32 idx = (7 - i) << 2;
                u32 base = (u32)landTimer;
                cnt = (s32 *)(base + idx);
            }
            if (*cnt != 0) {
                if (*cnt == -1)
                    *cnt = 10;
                if (--*cnt == 0)
                    gAudioContext->PlaySfx(SFX_UNKNOWN_4A, 0x100);
            }
            d = 0x1000000 / slot->velA;
            scale = d;
            SetAffine(gOamBuffer, matrix, scale, 0, 0, scale);
            CLEAR_OAM(&oamB);
            off = 0;
            if (d != 0x100) {
                off = -32;
                oamB.affineMode = 3;
            } else {
                oamB.affineMode = 1;
            }
            oamB.matrixLo = matrix;
            matrix++;
            oamB.palette = 0;
            oamB.tileNum = i << 6;
            oamB.size = 3;
            {
                s32 px = Q16_TO_INT(slot->posA.q);
                s32 dx = off - 32;

                oamB.x = px + dx;
            }
            oamB.y = Q16_TO_INT(slot->posB.q) - 32 + off;
            gOamBuffer->Add(&oamB);
        }
    }
    {
        const s32 *tbl = &gTitleArrowPieceOffsets[0][0];
        struct dma_regs *dma2;

        k = 0;
        dma2 = (struct dma_regs *)REG_ADDR_DMA3SAD;
        for (; k <= 1; k++) {
            LogoPiece *slot = &pieces[k];
            s32 j;

            if (slot->active) {
                s32 *cnt;
                {
                    u32 off = k << 2;
                    u32 base = (u32)landTimer;
                    cnt = (s32 *)(base + off);
                }

                if (*cnt != 0) {
                    if (*cnt == -1) {
                        *cnt = 8;
                        gAudioContext->PlaySfx(SFX_UNKNOWN_3D, 0x100);
                    } else if (--*cnt == 0) {
                        shake = 30;
                    }
                }
            }
            zero = 0;
            dma2->src = (u32)&zero;
            dma2->dst = (u32)&oamC;
            dma2->cnt = 0x81000004;
            dma2->cnt;
            oamC.palette = k + 1;
            oamC.tileNum = (k << 6) + 0x140;
            oamC.size = 2;
            oamC.priority = 2;
            for (j = 0; j < 4; j++) {
                s32 x = Q16_TO_INT(slot->posA.q) + *tbl++;
                s32 y = Q16_TO_INT(slot->posB.q) + *tbl++;

                if (y <= 0x8b) {
                    oamC.x = x;
                    oamC.y = y;
                    if (slot->active)
                        gOamBuffer->Add(&oamC);
                }
                oamC.tileNum += 0x10;
            }
        }
    }
    {
        LogoPiece *rec = &pieces[2];

        if (rec->active) {
            s32 a, b;

            ShowBg2();
            a = rec->posA.q;
            b = rec->posB.q;
            if (shake != 0) {
                --shake;
                {
                    s32 r = RandRange_s32(10);
                    s32 t = a - 5;
                    a = t + (u16)r;
                }
                {
                    s32 r = RandRange_s32(10);
                    s32 t = b - 5;
                    b = t + (u16)r;
                }
            }
            bgScale = 0x1000000 / rec->velA;
            bgX = bgScale * Q16_TO_INT(-a) + 0x4000;
            bgY = Q16_TO_INT(-b) * bgScale + 0x4000;
        }
    }
}

/* Middle part of GitHub issue #65's chunk (0x08035D1C-0x080361B0): the
 * title screen's other methods (TitleScreen: the cheat input, the run
 * loop, the menu, the destructor), C++ since #664 part 10c-2
 * (include/frontend.hpp). Split off title_screen_init.cpp at
 * `TitleScreenCheatInput` in the issues #64/#65 second NAKED retry, and
 * merged back into one file in #771 once both halves had the same flags.
 * The four company-logo methods that follow it in the ROM are at the head
 * of company_logos.cpp (here until #770).
 *
 * old_agbcp (OLD_AGBCC_OBJS), as its C was old_agbcc. Its C was built
 * with -fno-strength-reduce, for InitVvLogoPieces's up-counting loop
 * (then in this file); as C++ the plain indexed loop matches with
 * strength reduction on, and so does the rest of this half, so the flag
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
        gInput->Update();
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
    REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
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
    REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_BLDY = 0x10;
}
