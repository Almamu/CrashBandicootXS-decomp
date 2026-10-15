#include "engine/sys/display.hpp"
#include "engine/sys/vram_dma_queue.hpp"
#include "game/entities/sprite_obj.hpp"
#include "game/ui/frontend.hpp"
#include "engine/audio/audio.hpp"
#include "engine/sys/key_input.hpp"
#include "game/ui/menus.hpp"
#include "engine/sys/frame_limiter.hpp"

extern "C" {
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include <agb_syscall.h>
#include "engine/text/text.h"
#include "engine/sys/system.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
#include "engine/util/math_util.h"
}

using Engine::Gfx::ObjVramCursor;
using Engine::Sys::Display;
using Engine::Sys::FrameLimiter;
using Engine::Sys::VramDmaQueue;
using Engine::Text::Font;
using Game::Ui::Credits;
using Game::Ui::Starfield;

/* GitHub issue #64 (0x08034AA4-0x080354E0, 13 functions), C++ since
 * #664 part 10c (include/game/ui/frontend.hpp): the credits screen (Credits). The
 * file started with the last five methods of the continue prompt
 * (ContinuePrompt: Draw, Blink, CommitFrame, the destructor and Run),
 * which are at the end of src/game/ui/continue_prompt.cpp since #767. See
 * docs/matching/archive/issue-64-0x08034aa4-actor.md.
 *
 * old_agbcp (OLD_AGBCC_OBJS), as its C was old_agbcc. */

/* Reserves `m`'s glyph tiles (`tileCount` tiles) from the VRAM upload
 * cursor `c`. */
static inline void ReserveFontVram(ObjVramCursor *c, Font *m)
{
    c->Reserve(m->tileCount << 5);
}

/* Starts the starfield, loads the logos and the two fonts' tiles, and
 * starts the credits music. The font steps are inline functions taking
 * the font as a parameter, which gives the ROM's evaluation order. */
Credits::Credits()
{
    starfield = new Starfield;
    gOamBuffer->Reset();
    gOamBuffer->HideUnused();
    FrameLimiter::WaitForVBlank();
    gOamBuffer->Commit();
    gPaletteCache->FreeUnlockedSlots();
    gSmallFont->ResetPalette();
    gLargeFont->SetPalette(0);
    LoadLogos();
    gPaletteCache->Upload();
    gObjVramCursor->baseTile = 0;
    gObjVramCursor->Reset();
    gObjVramCursor->Reset();
    gSmallFont->SetTileBase(0);
    ReserveFontVram(gObjVramCursor, gSmallFont);

    /* `base` is needed: inlined, the code is 1 instruction longer */
    u32 base = gSmallFont->tileCount;

    gLargeFont->SetTileBase(base);
    ReserveFontVram(gObjVramCursor, gLargeFont);
    gObjVramCursor->Mark();
    popups = 0;
    largeFont = 0;
    streamBase = gCreditsText;
    stream = gCreditsText;
    lineDelay = 0;
    Display::dispcnt.bits.obj = 1;
    Display::CommitDispcnt();
    frameParity = 0;
    gAudioContext->PlaySong(SCREEN_SONG_CREDITS);
}

/* Runs the credits until A or START, then fades out over 17 frames and
 * frees the lines left. */
void Credits::Loop()
{
    s32 i;

    for (;;) {
        gInput->Update();
        if (gKeys.half.pressed & (A_BUTTON | START_BUTTON)) {
            break;
        }

        frameParity = (frameParity + 1) & 1;
        if (frameParity != 0) {
            UpdateText();
        }

        DrawText();
        FrameLimiter::WaitForVBlank();
        CommitFrame();
        starfield->Update();
    }

    gAudioContext->FadeOutMusic(0);

    for (i = 0; i <= 0x10; i++) {
        frameParity = (frameParity + 1) & 1;
        if (frameParity != 0) {
            UpdateText();
        }

        DrawText();
        FrameLimiter::WaitForVBlank();
        REG_BLDY = i;
        REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
        CommitFrame();
        starfield->Update();
    }

    if (popups != 0) {
        CreditsPopup *node = popups;

        do {
            /* `next` is needed: inlined, the instructions differ */
            CreditsPopup *next = node->next;

            delete node;
            node = next;
        } while (node != 0);
    }
    popups = 0;
}

/* Draws the lines on screen: the characters with their font's slot 4,
 * the logos as 32x32 OAM cells (those on screen). */
void Credits::DrawText()
{
    CreditsPopup *node;

    gOamBuffer->Reset();
    gObjVramCursor->Rewind();
    for (node = popups; node != 0; node = node->next) {
        Font *m;
        CreditsLogo *logo;
        s32 tile;
        u32 zero;
        struct oam_attrs oam; // the logo's 32x32 cells
        s32 y;
        s32 i;

        switch (node->mode) {
        case CREDITS_POPUP_MODE_SMALL_CHAR:
            m = gSmallFont;
            goto draw;
        case CREDITS_POPUP_MODE_LARGE_CHAR:
            m = gLargeFont;
        draw:
            m->SetPos(node->x, node->y);
            m->DrawGlyph(node->index);
            break;
        case CREDITS_POPUP_MODE_LOGO:
            logo = &logos[node->index];
            tile = gObjVramCursor->GetTile();
            gObjVramCursor->Upload(logo->tiles, (logo->rows * logo->cols) << 9);
            zero = 0;
            CpuSet(&zero, &oam, CPU_SET_SRC_FIXED | CPU_SET_32BIT | sizeof(oam) / sizeof(u32));
            oam.size = 2;
            oam.palette = logo->palette;
            y = node->y;
            for (i = 0; i < logo->rows; i++) {
                s32 x;
                s32 j;

                oam.y = y;
                x = node->x;
                for (j = 0; j < logo->cols; j++) {
                    /* on screen: -0x1f <= y <= DISPLAY_HEIGHT - 1 (a 32x32
                     * OBJ row) */
                    if ((u32)(y + 0x1f) <= DISPLAY_HEIGHT - 1 + 0x1f) {
                        oam.x = x;
                        oam.tileNum = tile;
                        gOamBuffer->Add(&oam);
                    }
                    tile += 0x10;
                    x += 0x20;
                }
                y += 0x20;
            }
            break;
        }
    }
    gOamBuffer->HideUnused();
}

/* Moves the lines up, freeing those that have left the screen; then,
 * unless the last line's delay is still running, reads the next line of
 * gCreditsText (wrapping at its end): its characters and logos become
 * popups, laid out left to right, then centred and placed below the
 * screen, and the delay is set from the line's height. */
void Credits::UpdateText()
{
    CreditsPopup **link;
    CreditsPopup *lineStart;
    CreditsPopup *tail;
    s32 smallHeight;
    /* `largeHeight` is needed: inlined, the code is 3 instructions shorter */
    s32 largeHeight;
    screen_h_t maxHeight;
    screen_x_t penX;
    const u8 *p;

    link = &popups;
    while (*link != 0) {
        CreditsPopup *n = *link;

        if (--n->y + n->height <= 0) {
            *link = n->next;
            delete n;
        } else {
            link = &n->next;
        }
    }

    if (lineDelay != 0) {
        lineDelay--;
        return;
    }

    lineStart = (CreditsPopup *)this;
    while (lineStart->next != 0) {
        lineStart = lineStart->next;
    }
    tail = lineStart;

    smallHeight = gSmallFont->TextHeight(gCreditsEmptyText);
    largeHeight = gLargeFont->TextHeight(gCreditsEmptyText);
    maxHeight = smallHeight;
    penX = 0;
    if (*stream == 0) {
        stream = streamBase;
    }
    p = stream;
    if (*p != '\n') {
        if (*p != 0) {
            u8 c;

            do {
                s32 advance = 0;
                s32 height = 0;

                if (*p == CREDITS_OP_SMALL) {
                    largeFont = height; // height is still 0 here
                } else if (*p == CREDITS_OP_LARGE) {
                    largeFont = 1;
                } else if (*p == CREDITS_OP_PICTURE) {
                    CreditsPopup *n;

                    stream = p + 1;
                    n = new CreditsPopup;
                    tail->next = n;
                    n->mode = CREDITS_POPUP_MODE_LOGO;
                    n->y = height;
                    n->x = penX;
                    n->index = *stream;
                    n->height = logos[n->index].height;
                    n->next = 0;
                    tail = n;
                    height = logos[n->index].height;
                    advance = logos[n->index].width;
                } else {
                    if (largeFont == 0) {
                        advance = gSmallFont->MeasureChars(p, 1);
                        height = smallHeight;
                    } else {
                        advance = gLargeFont->MeasureChars(p, 1);
                        height = largeHeight;
                    }
                    if (*stream != ' ') {
                        CreditsPopup *n = new CreditsPopup;

                        tail->next = n;
                        n->mode = largeFont;
                        n->y = 0;
                        n->x = penX;
                        n->index = *stream;
                        tail->next->height = height;
                        tail = tail->next;
                        tail->next = 0;
                    }
                }
                penX += advance;
                LIMIT_MIN(maxHeight, height);

                const u8 *q = stream;

                stream = q + 1;
                c = q[1];
                if (c == '\n') {
                    goto newline;
                }
                p = q + 1;
            } while (c != 0);
        }
        if (*stream != '\n') {
            goto place;
        }
    }
newline:
    stream++;
place:
    CreditsPopup *n = lineStart->next;
    s32 delay = maxHeight + 6;

    if (n != 0) {
        do {
            s32 y = n->y;
            /* `top` is needed: inlined, the instructions differ */
            s32 top = y + DISPLAY_HEIGHT;

            n->y = top + (maxHeight - (y + n->height)) / 2;
            n->x += (DISPLAY_WIDTH - penX) / 2;
            n = n->next;
        } while (n != 0);
    }
    lineDelay = delay;
}

/* One `gCreditsLogos` record (0x14 bytes), read through its own view of
 * `struct bg_package`: a logo's size in 8-px tiles (signed here; the
 * loops compare them signed) and its tagged palette/tile assets. */
struct popup_glyph_src {
    s32 w;              /* 0x00 */
    s32 h;              /* 0x04 */
    const u32 *palette; /* 0x08 - tagged asset, size in the header's bits 9+ */
    const u32 *tiles;   /* 0x0c - tagged asset, size in the header's bits 8+ */
    u32 map;            /* 0x10 - bg_package's mapAsset, NULL for every logo; not read */
};

/* Copies a 16-colour palette into palette-cache bank `slot`.
 *
 * `slot` is a const reference. LoadLogos passes its s32 counter, so the
 * call binds the reference to a u32 temporary: a copy of the counter whose
 * address is taken. Until the addressof pass (after cse1) that copy is
 * memory, so cse1 can't fold it into the counter, and gcse's PRE sees the
 * shift of a register set here, after the tile loops. Of the shift by
 * itself (`slot << 5` with `slot` the counter) PRE would compute a copy
 * ahead of the tile loops, with the `slot + 1` and `i + 1` it does hoist
 * there in the ROM (#662 rounds 2-10). cse2 then folds the copy back
 * into the counter, which leaves the ROM's reload of `slot` at the copy.
 * With a `const s32 &` the reference binds to the counter itself, which
 * then lives in memory until the addressof pass and loses the `slot + 1`
 * hoist (55 lines off); by value the copy is folded and the shift
 * hoisted (50). */
static inline void CopyPaletteToSlot(u16 (*slots)[16], const u32 &slot, const u16 *s)
{
    u16 *d = (u16 *)(slot * TILE_SIZE_4BPP + (uintptr_t)slots);
    s32 k;

    for (k = 15; k >= 0; k--) {
        *d++ = *s++;
    }
}

/* Loads the five logos: each one's tiles rearranged into 32x32 OAM cells
 * and its palette into palette-cache slots 1-5. */
void Credits::LoadLogos()
{
    u16 (*palSlots)[16] = gPaletteCache->slots;
    s32 slot = 1;
    s32 i;

    for (i = 0; i <= 4; i++) {
        struct popup_glyph_src *src = (struct popup_glyph_src *)&gCreditsLogos[i];
        CreditsLogo *logo = &logos[i];
        u8 *tiles;
        u16 *pal;
        s32 size;
        s32 y;

        s32 w = src->w;
        s32 h = src->h;

        logo->height = h << 3;
        logo->width = w << 3;
        logo->cols = (w + 3) / 4;
        logo->rows = (h + 3) / 4;
        tiles = new u8[*src->tiles >> 8];
        LoadTaggedAsset(src->tiles, tiles);
        size = (logo->cols * logo->rows) << 9;
        logo->tiles = new u8[size];
        DmaFill32(3, 0, logo->tiles, size);
        for (y = 0; y < src->h; y++) {
            s32 x;

            for (x = 0; x < src->w; x++) {
                s32 cell = (y >> 2) * logo->cols + (x >> 2);
                s32 sub = (x & 3) + ((y & 3) << 2);

                DmaCopy32(3, tiles + (y * src->w + x) * 32,
                          logo->tiles + (((cell << 4) + sub) << 5), TILE_SIZE_4BPP);
            }
        }
        delete[] tiles;
        pal = new u16[*src->palette >> 9];
        LoadTaggedAsset(src->palette, pal);
        CopyPaletteToSlot(palSlots, slot, pal);
        delete[] pal;
        gPaletteCache->ClaimSlot(slot);
        logo->palette = slot;
        slot++;
    }
}

void Credits::CommitFrame()
{
    Display::CommitDispcnt();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    gPaletteCache->Upload();
    gOamBuffer->Commit();
    VramDmaQueue::Flush();
}

/* Frees the starfield and the logos' tiles. */
Credits::~Credits()
{
    s32 i;

    delete starfield;
    for (i = 0; i < 5; i++) {
        delete[] logos[i].tiles;
    }
}

void Credits::Run()
{
    Credits *self = new Credits;

    self->Loop();
    delete self;
}
