#include "frontend.hpp"

extern "C" {
#include "match.h"
#include "gba/io_reg.h"
#include "bitmap_font.h"
#include "vram_pool.h"
#include "audio.h"
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
 * #664 part 10c (include/frontend.hpp). The file starts with the last
 * methods of the continue prompt (ContinuePrompt: Draw, Blink,
 * CommitFrame, the destructor and Run; its constructor and loop are
 * still C, src/menus/continue_prompt*.c), then the credits screen
 * (Credits). See docs/matching/archive/issue-64-0x08034aa4-actor.md.
 *
 * gSmallFont and gLargeFont are still C (src/text/): their virtual calls
 * are spelled out through the record's slots. old_agbcp (OLD_AGBCC_OBJS),
 * as its C was old_agbcc. */

/* `_call_via_rN`: calls `fn(self, ...)` (a bitmap_font method). */
extern "C" s32 _call_via_r1(void *self, void *fn);
extern "C" s32 _call_via_r2(void *self, void *arg, void *fn);
extern "C" s32 _call_via_r3(void *self, const void *a, s32 b, void *fn);

static inline void SetFontPos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Draws the Yes/No labels (UI texts 0x28-0x2a) with gSmallFont, the
 * selected one blinking (Blink) and marked with the cursor. */
void ContinuePrompt::Draw()
{
    s32 w;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    w = ICON_TEXT_CALL(icons, 0, GetUiText(0x28));
    FontSetPalette(icons, 0);
    SetFontPos(icons, 0x88 - w, 0x87);
    ICON_TEXT_CALL(icons, 2, GetUiText(0x28));
    FontSetPalette(icons, Blink(0));
    if (selection == 0) {
        SetFontPos(icons, 0x90, 0x87);
        ICON_TEXT_CALL(icons, 2, gContinuePromptCursorText);
    }
    SetFontPos(icons, 0x98, 0x87);
    ICON_TEXT_CALL(icons, 2, GetUiText(0x29));
    FontSetPalette(icons, Blink(1));
    if (selection == 1) {
        SetFontPos(icons, 0x90, 0x91);
        ICON_TEXT_CALL(icons, 2, gContinuePromptCursorText);
    }
    SetFontPos(icons, 0x98, 0x91);
    ICON_TEXT_CALL(icons, 2, GetUiText(0x2a));
    HideUnusedOamEntries(gOamBuffer);
}

/* The palette of `option`'s label: 1 when it isn't selected, else 0 or 2
 * from the blink counter, which it advances. */
s32 ContinuePrompt::Blink(s32 option)
{
    if (option == selection) {
        return (blinkCounter++ >> 1) & 2;
    }
    return 1;
}

void ContinuePrompt::CommitFrame()
{
    WaitForVBlank();
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
    REG_DISPCNT = dispcnt;
}

/* Frees the three BG buffers. */
ContinuePrompt::~ContinuePrompt()
{
    delete bg0Buf;
    delete bg1Buf;
    delete bg2Buf;
}

/* Runs the prompt and returns the choice (0 yes, 1 no). */
u8 ContinuePrompt::Run()
{
    ContinuePrompt *self;
    u8 result;

    mem_free_bytes(0xc0000000);
    self = new ContinuePrompt;
    result = self->Loop();
    delete self;
    mem_free_bytes(0xc0000000);
    return result;
}

/* Sets the font's glyph tile base and calls its slot 6. */
static inline void SetFontTileBase(struct bitmap_font *m, u32 base)
{
    struct icon_slot *slot;

    m->tileBase = base;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

/* Reserves `m`'s glyph tiles (`tileCount` tiles) from the VRAM upload
 * cursor `c`. */
static inline void ReserveFontVram(struct vram_upload_cursor *c, struct bitmap_font *m)
{
    ReserveObjVram(c, m->tileCount << 5);
}

/* Starts the starfield, loads the logos and the two fonts' tiles, and
 * starts the credits music. The font steps are inline functions taking
 * the font as a parameter, which gives the ROM's evaluation order. */
Credits::Credits()
{
    starfield = new Starfield;
    ResetOamBuffer(gOamBuffer);
    HideUnusedOamEntries(gOamBuffer);
    WaitForVBlank();
    CommitOamBuffer(gOamBuffer);
    FreeUnlockedPaletteSlots(gPaletteCache);
    FontResetPalette(gSmallFont);
    FontSetPalette(gLargeFont, 0);
    /* No code: it lengthens the live ranges across it, which gives
     * &gPaletteCache and &gObjVramCursor r4 and &gSmallFont r6. */
    MATCH_BARRIER();
    LoadLogos();
    UploadPaletteCache(gPaletteCache);
    gObjVramCursor->baseTile = 0;
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);
    SetFontTileBase(gSmallFont, 0);
    ReserveFontVram(gObjVramCursor, gSmallFont);
    {
        u32 base = gSmallFont->tileCount;

        SetFontTileBase(gLargeFont, base);
    }
    ReserveFontVram(gObjVramCursor, gLargeFont);
    MarkObjVram(gObjVramCursor);
    popups = 0;
    largeFont = 0;
    streamBase = gCreditsText;
    stream = gCreditsText;
    lineDelay = 0;
    gDispcnt[1] |= 0x10;
    CommitDispcnt();
    frameParity = 0;
    PlaySong(gAudioContext, SONG_CREDITS);
}

/* Runs the credits until A or START, then fades out over 17 frames and
 * frees the lines left. */
void Credits::Loop()
{
    s32 i;

    for (;;) {
        UpdateKeys(gInput);
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

    FadeOutMusic(gAudioContext, 0);

    for (i = 0; i <= 0x10; i++) {
        frameParity = (frameParity + 1) & 1;
        if (frameParity != 0) {
            UpdateText();
        }

        DrawText();
        WaitForVBlank();
        REG_BLDY = i;
        REG_BLDCNT = 0xff;
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

/* The OAM request of a logo's 32x32 cells. */
struct popup_oam {
    u8 y;
    u8 unk_1;
    u16 x:9;
    u16 unk_2:5;
    u16 size:2;
    u16 tile:10;
    u16 unk_4:2;
    u16 palette:4;
};

/* Draws the lines on screen: the characters with their font's slot 4,
 * the logos as 32x32 OAM cells (those on screen). */
void Credits::DrawText()
{
    CreditsPopup *node;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    for (node = popups; node != 0; node = node->next) {
        struct bitmap_font *m;
        CreditsLogo *logo;
        s32 tile;
        u32 zero;
        struct popup_oam oam;
        s32 y;
        s32 i;

        switch (node->mode) {
        case 0:
            m = gSmallFont;
            goto draw;
        case 1:
            m = gLargeFont;
        draw:
            SetFontPos(m, node->x, node->y);
            _call_via_r2((u8 *)m + m->record->slots[4].offset, (void *)(u32)node->index,
                         m->record->slots[4].ptr);
            break;
        case 2:
            logo = &logos[node->index];
            tile = GetObjVramTile(gObjVramCursor);
            UploadObjVram(gObjVramCursor, logo->tiles, (logo->rows * logo->cols) << 9);
            zero = 0;
            CpuSet(&zero, &oam, CPU_SET_SRC_FIXED | CPU_SET_32BIT | 2);
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
                        oam.tile = tile;
                        AddOamEntry(gOamBuffer, &oam);
                    }
                    tile += 0x10;
                    x += 0x20;
                }
                y += 0x20;
            }
            break;
        }
    }
    HideUnusedOamEntries(gOamBuffer);
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

    smallHeight = FontTextHeight(gSmallFont, (u8 *)gCreditsEmptyText);
    largeHeight = FontTextHeight(gLargeFont, (u8 *)gCreditsEmptyText);
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
                        struct icon_slot *s = &gSmallFont->record->slots[1];

                        advance = _call_via_r3((u8 *)gSmallFont + s->offset, p, 1, s->ptr);
                        height = smallHeight;
                    } else {
                        struct icon_slot *s = &gLargeFont->record->slots[1];

                        advance = _call_via_r3((u8 *)gLargeFont + s->offset, p, 1, s->ptr);
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
    u32 unk_10;
};

/* Loads the five logos: each one's tiles rearranged into 32x32 OAM cells
 * and its palette into palette-cache slots 1-5.
 *
 * GCSE's PRE hoists any `slot << 5` (even through a plain MATCH_KEEP: a
 * non-volatile asm with outputs is an ordinary hashed expression) to the
 * y loop's pre-test, next to the `slot + 1` and `i + 1` it also hoists
 * there, and spills it; the ROM computes the palette address at the
 * copy. So the palette index is a copy `ps` passed through
 * MATCH_KEEP_VOLATILE (volatile asms are never entered in GCSE's table).
 * `ps` is an r1 register variable (the ROM reloads `slot` into r1) and
 * has one more MATCH_USE after the shift, so the shift result goes to r0
 * instead of reusing r1. The same as the C's, which needed them too. */
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
        {
            u16 *s = pal;
            /* Escaped copy of `slot`: see the note above. */
            MATCH_HOLD_REG(s32, ps, r1) = slot;
            s32 sh;
            u16 *d;
            s32 k;

            MATCH_KEEP_VOLATILE(ps); /* new pseudo GCSE can't hoist */
            sh = ps << 5;
            MATCH_USE(ps); /* keeps ps live so sh doesn't reuse r1 */
            d = (u16 *)(sh + (u32)palSlots);
            for (k = 15; k >= 0; k--) {
                *d++ = *s++;
            }
        }
        delete[] pal;
        ClaimPaletteSlot(gPaletteCache, slot);
        logo->palette = slot;
        slot++;
    }
}

void Credits::CommitFrame()
{
    CommitDispcnt();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
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
