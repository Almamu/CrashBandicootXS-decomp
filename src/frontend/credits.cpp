#include "sprite_obj.hpp"
#include "frontend.hpp"
#include "audio.hpp"
#include "key_input.hpp"

extern "C" {
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include <agb_syscall.h>
#include "text.h"
#include "system.h"
#include "menus.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* GitHub issue #64 (0x08034AA4-0x080354E0, 13 functions), C++ since
 * #664 part 10c (include/frontend.hpp): the credits screen (Credits). The
 * file started with the last five methods of the continue prompt
 * (ContinuePrompt: Draw, Blink, CommitFrame, the destructor and Run),
 * which are at the end of src/menus/continue_prompt.cpp since #767. See
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
    WaitForVBlank();
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
    {
        u32 base = gSmallFont->tileCount;

        gLargeFont->SetTileBase(base);
    }
    ReserveFontVram(gObjVramCursor, gLargeFont);
    gObjVramCursor->Mark();
    popups = 0;
    largeFont = 0;
    streamBase = gCreditsText;
    stream = gCreditsText;
    lineDelay = 0;
    gDispcnt[1] |= 0x10;
    CommitDispcnt();
    frameParity = 0;
    gAudioContext->PlaySong(SONG_CREDITS);
}

/* Runs the credits until A or START, then fades out over 17 frames and
 * frees the lines left. */
void Credits::Loop()
{
    s32 i;

    for (;;) {
        gInput->Update();
        if (gKeys.half.pressed & 9) {
            break;
        }

        frameParity = (frameParity + 1) & 1;
        if (frameParity != 0) {
            UpdateText();
        }

        DrawText();
        WaitForVBlank();
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
        WaitForVBlank();
        REG_BLDY = i;
        REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
        CommitFrame();
        starfield->Update();
    }

    if (popups != 0) {
        CreditsPopup *node = popups;

        do {
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
        case 0:
            m = gSmallFont;
            goto draw;
        case 1:
            m = gLargeFont;
        draw:
            m->SetPos(node->x, node->y);
            m->DrawGlyph(node->index);
            break;
        case 2:
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
                    if ((u32)(y + 0x1f) <= 0xbe) {
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
    s32 largeHeight;
    s32 maxHeight;
    s32 penX;
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

    smallHeight = gSmallFont->TextHeight((u8 *)gCreditsEmptyText);
    largeHeight = gLargeFont->TextHeight((u8 *)gCreditsEmptyText);
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

                if (*p == 2) {
                    largeFont = height;
                } else if (*p == 3) {
                    largeFont = 1;
                } else if (*p == 1) {
                    CreditsPopup *n;

                    stream = p + 1;
                    n = new CreditsPopup;
                    tail->next = n;
                    n->mode = 2;
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
                        advance = gSmallFont->MeasureChars((u8 *)p, 1);
                        height = smallHeight;
                    } else {
                        advance = gLargeFont->MeasureChars((u8 *)p, 1);
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
                {
                    const u8 *q = stream;

                    stream = q + 1;
                    c = q[1];
                    if (c == '\n') {
                        goto newline;
                    }
                    p = q + 1;
                }
            } while (c != 0);
        }
        if (*stream != '\n') {
            goto place;
        }
    }
newline:
    stream++;
place:
    {
        CreditsPopup *n = lineStart->next;
        s32 delay = maxHeight + 6;

        if (n != 0) {
            s32 dx = (0xf0 - penX) / 2;

            do {
                s32 y = n->y;
                s32 top = y + 0xa0;

                n->y = top + (maxHeight - (y + n->height)) / 2;
                n->x += dx;
                n = n->next;
            } while (n != 0);
        }
        lineDelay = delay;
    }
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
static inline void CopyPaletteToSlot(u8 (*slots)[TILE_SIZE_4BPP], const u32 &slot, const u16 *s)
{
    u16 *d = (u16 *)(slot * TILE_SIZE_4BPP + (u32)slots);
    s32 k;

    for (k = 15; k >= 0; k--) {
        *d++ = *s++;
    }
}

/* Loads the five logos: each one's tiles rearranged into 32x32 OAM cells
 * and its palette into palette-cache slots 1-5. */
void Credits::LoadLogos()
{
    u8 (*palSlots)[TILE_SIZE_4BPP] = gPaletteCache->slots;
    s32 slot = 1;
    s32 i;

    for (i = 0; i <= 4; i++) {
        struct popup_glyph_src *src = (struct popup_glyph_src *)&gCreditsLogos[i];
        CreditsLogo *logo = &logos[i];
        u8 *tiles;
        u16 *pal;
        s32 size;
        s32 y;

        {
            s32 w = src->w;
            s32 h = src->h;

            logo->height = h << 3;
            logo->width = w << 3;
            logo->cols = (w + 3) / 4;
            logo->rows = (h + 3) / 4;
        }
        tiles = new u8[*src->tiles >> 8];
        LoadTaggedAsset(src->tiles, tiles);
        size = (logo->cols * logo->rows) << 9;
        logo->tiles = new u8[size];
        {
            u32 zero = 0;
            struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;

            dma->src = (u32)&zero;
            dma->dst = (u32)logo->tiles;
            dma->cnt = (size / 4) | 0x85000000;
            dma->cnt;
        }
        for (y = 0; y < src->h; y++) {
            s32 x;

            for (x = 0; x < src->w; x++) {
                s32 cell = (y >> 2) * logo->cols + (x >> 2);
                s32 sub = (x & 3) + ((y & 3) << 2);
                struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;

                dma->src = (u32)(tiles + (y * src->w + x) * 32);
                dma->dst = (u32)(logo->tiles + (((cell << 4) + sub) << 5));
                dma->cnt = 0x84000008;
                dma->cnt;
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
    CommitDispcnt();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    gPaletteCache->Upload();
    gOamBuffer->Commit();
    FlushVramDmaQueue();
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
