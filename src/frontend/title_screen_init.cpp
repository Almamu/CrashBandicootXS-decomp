#include "sprite_obj.hpp"
#include "frontend.hpp"

extern "C" {
#include "match.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"
#include "gba/io_reg.h"
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* GitHub issue #65's chunk (0x080354E0-0x08037110) starts here: the title
 * screen's constructor, its graphics loaders and its logo pieces' update
 * and draw (TitleScreen, #664 part 10c-2, include/frontend.hpp). Its
 * other methods are in title_screen.cpp. See
 * docs/matching/archive/issue-65-graphics-loading.md.
 *
 * old_agbcp (OLD_AGBCC_OBJS), as its C was old_agbcc, with the default
 * -O2 strength reduction: DrawLogoPieces's up-counting inner loop is
 * reversed by it, as in the ROM (title_screen.cpp is built without it).
 * See docs/matching/archive/issue-64-65-naked-retry-2.md. */

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

    *(vu32 *)REG_ADDR_BLDCNT = 0xff;
    REG_BLDY = 0x10;
    REG_DISPCNT = 0;

    font->SetPalette(0xe);
    font->SetTileBase(0x200);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)gTitleMenuPalette;
    dma->dst = OBJ_PLTT + 13 * 0x20;
    dma->cnt = 0x80000010;
    dma->cnt;
    dma->src = (u32)gTitleMenuSelectedPalette;
    dma->dst = OBJ_PLTT + 14 * 0x20;
    dma->cnt = 0x80000010;
    dma->cnt;
    dma->src = (u32)gTitleMenuBlinkPalette;
    dma->dst = OBJ_PLTT + 15 * 0x20;
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

    StartSong(gAudioContext, SONG_MAIN_MENU_EUROPE);
}

/* Loads BG2's palette, tiles and map from gTitleScreenBg, packing the
 * map's 16-bit entries (the tile number in the low byte) into the bytes
 * of the 8-bit affine screen at 0x0600F000, and sets up BG2.
 *
 * `bg2cnt` is deliberately left uninitialized: the ROM builds the
 * register value with an `& 0xFFFF0000` against whatever register it
 * got. The indexed `map[i]`/`map[i + 1]` reads are strength-reduced into
 * the ROM's walk pointer, with `map` kept in r8 for the free. */
void TitleScreen::LoadBg()
{
    const struct bg_package *pkg = &gTitleScreenBg;
    u16 *map;
    u16 *dest;
    s32 i;
    /* self-init: deliberately unset (see above); silences -Wuninitialized (#577) */
    u32 bg2cnt = bg2cnt;

    LoadTaggedAsset(pkg->paletteAsset, (void *)BG_PLTT);
    LoadTaggedAsset(pkg->tileAsset, (void *)BG_CHAR_ADDR(2));
    map = new u16[(s32)pkg->height * (s32)pkg->width];
    LoadTaggedAsset(pkg->mapAsset, map);
    dest = (u16 *)BG_SCREEN_ADDR(30);
    for (i = 0; i < (s32)pkg->height * (s32)pkg->width; i += 2) {
        *dest = (map[i] & 0xff) | ((map[i + 1] & 0xff) << 8);
        dest++;
    }
    bg2cnt &= 0xFFFF0000;
    bg2cnt |= 8;
    bg2cnt |= 0xf0 << 5;
    bg2cnt |= 0x80;
    bg2cnt |= 1;
    REG_BG2CNT = bg2cnt;
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
    /* The next package, from the map load to the end of the pass: with
     * the copy, the ROM's r7 holds `pkg` only until then and the inner
     * loop reuses it. Pinned (register allocation): unpinned, `next` and
     * `pass` swap r8 and r9. */
    MATCH_HOLD_REG(const struct bg_package *const *, next, r8);

    for (pass = 0; pass <= 3; pass++) {
        u8 *palette;
        u8 *tiles;
        u16 *map;
        s32 count;

        palette = new u8[*(u32 *)(*pkg)->paletteAsset >> 8];
        LoadTaggedAsset((*pkg)->paletteAsset, palette);
        {
            struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;

            dma->src = (u32)palette;
            dma->dst = (u32)paletteDest;
            dma->cnt = 0x80000010;
            dma->cnt;
        }
        paletteDest += 0x20;
        delete[] palette;

        tiles = new u8[*(u32 *)(*pkg)->tileAsset >> 8];
        LoadTaggedAsset((*pkg)->tileAsset, tiles);

        count = (*pkg)->height * (*pkg)->width;
        map = new u16[count];
        LoadTaggedAsset((*pkg++)->mapAsset, map);
        next = pkg;
        {
            s32 i;

            for (i = 0; i < count; i++) {
                struct dma_regs *dma2 = (struct dma_regs *)REG_ADDR_DMA3SAD;

                dma2->src = (u32)(tiles + ((map[i] & 0xff) << 5));
                dma2->dst = (u32)tileDest;
                dma2->cnt = 0x80000010;
                dma2->cnt;
                tileDest += 0x20;
            }
        }

        delete[] map;
        delete[] tiles;
        pkg = next;
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
                    PlaySfx(gAudioContext, SFX_UNKNOWN_4A, 0x100);
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
                        PlaySfx(gAudioContext, SFX_UNKNOWN_3D, 0x100);
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
