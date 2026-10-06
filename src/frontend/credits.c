#include "core.h"
#include "gba/io_reg.h"
#include "bitmap_font.h"
#include "vram_pool.h"
#include "audio.h"
#include "gba/dma_macros.h"
#include <agb_syscall.h>
#include "text.h"
#include "frontend.h"
#include "system.h"
#include "menus.h"
#include "gfx.h"
#include "globals.h"

/* GitHub issue #64 (0x08034AA4-0x080354E0, 13 functions). Continues
 * straight on from issue #63's fade-overlay cluster (continue_prompt_init.c/
 * continue_prompt.c) - the first five functions here
 * (DrawContinuePrompt/GetContinuePromptBlink/CommitContinuePromptFrame/DestroyContinuePrompt/RunContinuePrompt) are more
 * methods on that same `struct continue_prompt` "self" object, then the
 * chunk moves on to the credits screen (RunCredits, read at first as
 * a "between-level map/progress screen") (see docs/rom_map.md's
 * "RunContinuePrompt turns out to be a separate screen trigger"/"A fourth
 * thing in this file" sections) - see
 * docs/matching/archive/issue-64-0x08034aa4-actor.md for the full write-up. */

/* The credits screen (RunCredits; docs/rom_map.md read it as a
 * "between-level map/progress screen"), allocated `OperatorNew(0x98)` by
 * `RunCredits`: `struct credits_screen` is in frontend.h. */

/* Another instance of the by-now-familiar "refresh OAM + center text"
 * pattern (docs/rom_map.md's "DrawContinuePrompt is just another instance of
 * ..." note): syncs the OAM shadow buffer and VRAM upload cursor, then
 * draws the Yes/No dialog's three labels (icon-manager mode ids
 * 0x28/0x29/0x2a) via `self->icons->record->slots[6]`'s position
 * (mode 0x28 twice - once for the plain label, once conditionally for
 * a highlighted "cursor" redraw keyed on `self->selection`), applying
 * `GetContinuePromptBlink`'s blink mask to each option's own OAM-hide byte via
 * `FontSetPalette` in between, and finally re-commits the OAM shadow
 * buffer.
 *
 * Was a NAKED transcription until the issue #64/#65 NAKED retry: each
 * label draw is a gcc 2.x virtual call through the icon manager's
 * method record (`record->slots[n]`, `_call_via_r2`),
 * the same `ICON_TEXT_CALL` shape save_menu_draw.c already matches, and
 * with that the "many live values across calls" allocation falls out
 * of plain C. */
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* `record->slots[n]` on an icon manager, called with `label` (slot 0
 * measures and returns the pixel width, slot 2 draws). */
#define ICON_TEXT_CALL(mgrExpr, n, label)                                       \
    ({                                                                          \
        struct bitmap_font *_m = (mgrExpr);                                    \
        struct icon_slot *_s = &_m->record->slots[n];                           \
        _call_via_r2((u8 *)_m + _s->offset, (void *)(label), _s->ptr);           \
    })

static inline void set_icon_mgr_pos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

void DrawContinuePrompt(struct continue_prompt *self)
{
    s32 w;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    w = ICON_TEXT_CALL(self->icons, 0, GetUiText(0x28));
    FontSetPalette(self->icons, 0);
    set_icon_mgr_pos(self->icons, 0x88 - w, 0x87);
    ICON_TEXT_CALL(self->icons, 2, GetUiText(0x28));
    FontSetPalette(self->icons, GetContinuePromptBlink(self, 0));
    if (self->selection == 0)
    {
        set_icon_mgr_pos(self->icons, 0x90, 0x87);
        ICON_TEXT_CALL(self->icons, 2, gContinuePromptCursorText);
    }
    set_icon_mgr_pos(self->icons, 0x98, 0x87);
    ICON_TEXT_CALL(self->icons, 2, GetUiText(0x29));
    FontSetPalette(self->icons, GetContinuePromptBlink(self, 1));
    if (self->selection == 1)
    {
        set_icon_mgr_pos(self->icons, 0x90, 0x91);
        ICON_TEXT_CALL(self->icons, 2, gContinuePromptCursorText);
    }
    set_icon_mgr_pos(self->icons, 0x98, 0x91);
    ICON_TEXT_CALL(self->icons, 2, GetUiText(0x2a));
    HideUnusedOamEntries(gOamBuffer);
}

asm(".align 2, 0");

/* --------------------------------------------------------------------
 * GetContinuePromptBlink - blink/toggle helper: returns 1 immediately if `mode`
 * isn't the dialog's current selection; otherwise advances the
 * selected item's blink counter and returns bit 1 of its pre-advance
 * value (a 0/2 flicker mask consumed by DrawContinuePrompt to hide the label
 * every other frame-pair).
 * ------------------------------------------------------------------ */
s32 GetContinuePromptBlink(struct continue_prompt *self, s32 mode)
{
    register s32 result asm("r0");
    s32 counter;

    if (mode != self->selection) {
        result = 1;
    } else {
        counter = self->blinkCounter;
        result = (counter >> 1) & 2;
        self->blinkCounter = counter + 1;
    }
    return result;
}

/* --------------------------------------------------------------------
 * CommitContinuePromptFrame - one frame's "yield" helper for the continue prompt: syncs
 * the shared OAM shadow buffer, flushes the VRAM upload queue, then
 * re-applies the overlay's own DISPCNT mirror.
 * ------------------------------------------------------------------ */
void CommitContinuePromptFrame(struct continue_prompt *self)
{
    WaitForVBlank();
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
    REG_DISPCNT = self->dispcnt;
}

/* --------------------------------------------------------------------
 * DestroyContinuePrompt - the continue prompt's teardown: frees its three BG scratch
 * buffers, then frees `self` too when `mode` bit 0 is set.
 * ------------------------------------------------------------------ */
void DestroyContinuePrompt(struct continue_prompt *self, s32 mode)
{
    OperatorDelete(self->bg0Buf);
    OperatorDelete(self->bg1Buf);
    OperatorDelete(self->bg2Buf);
    if (mode & 1)
        OperatorDelete(self);
}

/* --------------------------------------------------------------------
 * RunContinuePrompt - the "Are you sure?" confirmation-dialog trigger
 * (docs/rom_map.md's "RunContinuePrompt drives a Yes/No confirmation prompt"
 * section): allocates and builds the continue prompt (InitContinuePrompt), runs
 * the Yes/No dialog to completion (ContinuePromptLoop), tears the overlay down
 * (DestroyContinuePrompt) if it was actually built, and returns which option was
 * selected.
 * ------------------------------------------------------------------ */
u8 RunContinuePrompt(void)
{
    struct continue_prompt *self;
    u8 result;

    mem_free_bytes(0xc0000000);
    self = InitContinuePrompt(OperatorNew(0x24));
    result = ContinuePromptLoop(self);
    if (self != NULL)
        DestroyContinuePrompt(self, 3);
    mem_free_bytes(0xc0000000);
    return result;
}

asm(".align 2, 0");

/* The credits screen's constructor (docs/rom_map.md's "InitCredits is a
 * combined constructor" note): builds the starfield
 * (`InitStarfield(OperatorNew(0x14))` -> `self->starfield`), syncs the OAM
 * shadow buffer, hooks both text-icon managers
 * (`gSmallFont`/`gLargeFont`) up for this screen (firing
 * each one's slot-6 OAM trampoline via `_call_via_r1`, and copying
 * `gSmallFont->tileCount` into `gLargeFont->tileBase` -
 * a new, previously-unexplained cross-wiring between the two icon
 * managers), loads the popup-text glyph assets (`LoadCreditsLogos`), resets
 * the shared tile cache and VRAM upload cursor, initializes the
 * popup-text opcode-stream fields to `gCreditsText`, sets the
 * DISPCNT "OBJ enable"-adjacent bit in `gDispcnt`, and ducks
 * the audio context (`PlaySong(gAudioContext, 0x11)`). Returns
 * `self`.
 *
 * Was a NAKED transcription until the issue #64/#65 NAKED retry. The
 * "high registers rebound across calls" shape is just cse keeping each
 * global's address live; what it took was the ROM's own evaluation
 * order - the two icon-manager steps (`IconSetBase`, `IconReserveVram`)
 * as inline helpers taking the manager as a parameter (so each keeps
 * its own rematerialized 0x108/0x12c/0x130 offsets and the E0 base
 * value is read before E0 itself), and old_agbcc (the DISPCNT byte OR
 * loads the 0x10 constant before the `ldrb`; this whole object matches
 * under old_agbcc, so it moved to the Makefile's OLD_AGBCC_OBJS). The
 * empty `asm("")` after the E0 reset produces no code; it only
 * lengthens the live ranges crossing it by one insn, which is what tips
 * the allocator into giving `&gPaletteCache`/`&gObjVramCursor`
 * r4 and `&gSmallFont` r6 as the ROM does. */
extern void _call_via_r1(void *self, void *fn);

/* Sets the manager's glyph tile base and fires its slot-6 method. */
static inline void IconSetBase(struct bitmap_font *m, u32 base)
{
    struct icon_slot *slot;

    m->tileBase = base;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

/* Reserves `m`'s glyph tiles (`tileCount` tiles) from the VRAM upload
 * cursor `c`. */
static inline void IconReserveVram(struct vram_upload_cursor *c, struct bitmap_font *m)
{
    ReserveObjVram(c, m->tileCount << 5);
}

struct credits_screen *InitCredits(struct credits_screen *self)
{
    self->starfield = InitStarfield(OperatorNew(0x14));
    ResetOamBuffer(gOamBuffer);
    HideUnusedOamEntries(gOamBuffer);
    WaitForVBlank();
    CommitOamBuffer(gOamBuffer);
    FreeUnlockedPaletteSlots(gPaletteCache);
    FontResetPalette(gSmallFont);
    FontSetPalette(gLargeFont, 0);
    asm("");
    LoadCreditsLogos(self);
    UploadPaletteCache(gPaletteCache);
    gObjVramCursor->baseTile = 0;
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);
    IconSetBase(gSmallFont, 0);
    IconReserveVram(gObjVramCursor, gSmallFont);
    {
        u32 base = gSmallFont->tileCount;

        IconSetBase(gLargeFont, base);
    }
    IconReserveVram(gObjVramCursor, gLargeFont);
    MarkObjVram(gObjVramCursor);
    self->popupListHead = NULL;
    self->drawMode = 0;
    self->streamBase = gCreditsText;
    self->streamCursor = gCreditsText;
    self->suppressCounter = 0;
    gDispcnt[1] |= 0x10;
    CommitDispcnt();
    self->frameParity = 0;
    PlaySong(gAudioContext, 0x11);
    return self;
}

asm(".align 2, 0");


/* --------------------------------------------------------------------
 * CreditsLoop - the credits screen's per-frame driver: an input-gated busy
 * loop toggling `frameParity` every iteration (driving the popup-text
 * system, UpdateCreditsText, every other frame) alongside the starfield
 * (UpdateStarfield) every frame, until confirm or D-pad-down+L is pressed;
 * then a fixed 17-frame wipe/transition effect poking the window-blend
 * hardware registers directly; then frees every remaining popup-text
 * list node.
 * ------------------------------------------------------------------ */
void CreditsLoop(struct credits_screen *self)
{
    s32 i;

    while (1) {
        UpdateKeys(gInput);
        {
            register struct held_pressed_pair *p asm("r1") = &gKeys.half;
            register s32 mask asm("r0") = 9;

            mask &= p->pressed;
            if (mask)
                break;
        }

        self->frameParity = (self->frameParity + 1) & 1;
        if (self->frameParity != 0)
            UpdateCreditsText(self);

        DrawCreditsText(self);
        WaitForVBlank();
        CommitCreditsFrame(self);
        UpdateStarfield(self->starfield);
    }

    FadeOutMusic(gAudioContext, 0);

    for (i = 0; i <= 0x10; i++) {
        self->frameParity = (self->frameParity + 1) & 1;
        if (self->frameParity != 0)
            UpdateCreditsText(self);

        DrawCreditsText(self);
        WaitForVBlank();
        REG_BLDY = i;
        REG_BLDCNT = 0xff;
        CommitCreditsFrame(self);
        UpdateStarfield(self->starfield);
    }

    if (self->popupListHead != NULL) {
        void *node = self->popupListHead;
        do {
            void *next = *(void **)node;
            OperatorDelete(node);
            node = next;
        } while (node != NULL);
    }
    self->popupListHead = NULL;
}

asm(".align 2, 0");

/* The credits screen's per-frame OAM-icon draw dispatcher for the starfield
 * object (`self->starfield`, the `sp[0xc]`-cached argument throughout):
 * for `starfield->drawMode` (see `struct credits_screen` above) 0/1/2 draws a
 * single centered label via `_call_via_r2` against
 * `gSmallFont`/`gLargeFont`; for any other drawMode value
 * (docs/rom_map.md's starfield/radar-dot description) DMA3-transfers a
 * procedurally-built tile buffer and iterates a per-tile record array,
 * building each dot's OAM attribute halfwords in place and applying
 * them via `AddOamEntry`, before advancing to the next linked object in
 * `starfield`'s list and repeating.
 *
 * Was a NAKED transcription until the issue #64/#65 NAKED retry: the
 * "six running values across a call in a nested loop" allocation is
 * plain gcc output under old_agbcc once the source order matches (the
 * icon position set through `set_icon_mgr_pos` so x/y are loaded before
 * the stores, `h * w` for the tile count, the OAM request's size and
 * palette as bitfields of a stack `struct popup_oam`). */
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

void DrawCreditsText(struct credits_screen *self)
{
    struct popup_node *node;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    for (node = self->popupListHead; node != NULL; node = node->next)
    {
        struct bitmap_font *m;

        switch (node->mode)
        {
        case 0:
            m = gSmallFont;
            goto draw;
        case 1:
            m = gLargeFont;
        draw:
            set_icon_mgr_pos(m, node->x, node->y);
            _call_via_r2((u8 *)m + m->record->slots[4].offset, (void *)(u32)node->index, m->record->slots[4].ptr);
            break;
        case 2:
        {
            /* `node->index` is read twice, as the ROM does. */
            struct popup_glyph *glyph = (struct popup_glyph *)((u8 *)self + 0x1c + (node->index * 2 + node->index) * 8);
            s32 tile;
            u32 zero;
            struct popup_oam oam;
            s32 y;
            s32 i;

            tile = GetObjVramTile(gObjVramCursor);
            UploadObjVram(gObjVramCursor, glyph->tiles, (glyph->rows * glyph->cols) << 9);
            zero = 0;
            CpuSet(&zero, &oam, CPU_SET_SRC_FIXED | CPU_SET_32BIT | 2);
            oam.size = 2;
            oam.palette = glyph->palette;
            y = node->y;
            for (i = 0; i < glyph->rows; i++)
            {
                s32 x;
                s32 j;

                oam.y = y;
                x = node->x;
                for (j = 0; j < glyph->cols; j++)
                {
                    if ((u32)(y + 0x1f) <= 0xbe)
                    {
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
    }
    HideUnusedOamEntries(gOamBuffer);
}

asm(".align 2, 0");

/* The credits screen's floating-text popup driver (docs/rom_map.md's "A
 * floating-text/glyph popup system" note): first walks `self`'s
 * `popupListHead` linked list, decrementing each node's countdown pair
 * (`+8`/`+0xc`) and unlinking/freeing (`OperatorDelete`) any node whose
 * sum has expired; then, unless `self->suppressCounter` is still
 * counting down, parses `self`'s byte-opcode stream
 * (`streamCursor`/`streamBase`, `struct credits_screen`) - opcodes 0/1
 * allocate and link a new 0x18-byte popup node (mode 0/1 respectively),
 * 2/3 set `self->drawMode`, 0xA terminates a line (measuring both
 * `gSmallFont`/`gLargeFont`'s text width via `FontTextHeight` first)
 * - drawing the assembled line centered via `_call_via_r3` once `self`'s
 * `drawMode` is known, then finally re-derives `self->suppressCounter`
 * from the measured line width and walks the popup list one more time
 * shifting each node horizontally into position.
 *
 * Was a NAKED transcription until the issue #64/#65 NAKED retry; under
 * old_agbcc the "many high registers across calls" allocation is plain
 * gcc output once the source order matches. The glyph height/width
 * reads go through `GlyphHeightAt`/`GlyphWidthAt` (base field address
 * first, then the `index * 0x18` offset) so loop.c hoists
 * `&glyphs[0].height` the way the ROM does; the first height read spells
 * the index as `index * 2 + index`, re-reading the byte it just stored,
 * as the ROM does; the cursor advance and the popup y placement keep
 * their own temporaries so the old/new cursor and the `y + 0xa0` term are
 * formed in the ROM's order. */
extern s32 _call_via_r3(void *self, const void *a, s32 b, void *fn);

#define ICON_TEXT_CALL3(mgrExpr, n, a, b)                                      \
    ({                                                                          \
        struct bitmap_font *_m = (mgrExpr);                                    \
        struct icon_slot *_s = &_m->record->slots[n];                           \
        _call_via_r3((u8 *)_m + _s->offset, (a), (b), _s->ptr);                  \
    })

static inline s32 *GlyphHeightAt(struct credits_screen *self, s32 off)
{
    u8 *base = (u8 *)&self->glyphs[0].height;
    return (s32 *)(base + off);
}

static inline s32 *GlyphWidthAt(struct credits_screen *self, s32 off)
{
    u8 *base = (u8 *)&self->glyphs[0].width;
    return (s32 *)(base + off);
}

void UpdateCreditsText(struct credits_screen *self)
{
    struct popup_node **link;
    struct popup_node *lineStart;
    struct popup_node *tail;
    s32 widthA;
    s32 widthB;
    s32 maxHeight;
    s32 penX;
    const u8 *p;

    link = (struct popup_node **)&self->popupListHead;
    while (*link != NULL)
    {
        struct popup_node *n = *link;

        if (--n->y + n->timer <= 0)
        {
            *link = n->next;
            OperatorDelete(n);
        }
        else
        {
            link = &n->next;
        }
    }

    if (self->suppressCounter != 0)
    {
        self->suppressCounter--;
        return;
    }

    lineStart = (struct popup_node *)self;
    while (lineStart->next != NULL)
        lineStart = lineStart->next;
    tail = lineStart;

    widthA = FontTextHeight(gSmallFont, (u8 *)gCreditsEmptyText);
    widthB = FontTextHeight(gLargeFont, (u8 *)gCreditsEmptyText);
    maxHeight = widthA;
    penX = 0;
    if (*(const u8 *)self->streamCursor == 0)
        self->streamCursor = self->streamBase;
    p = self->streamCursor;
    if (*p != '\n')
    {
        if (*p != 0)
        {
            u8 c;

            do
            {
                s32 advance = 0;
                s32 height = 0;

                if (*p == 2)
                {
                    self->drawMode = height;
                }
                else if (*p == 3)
                {
                    self->drawMode = 1;
                }
                else if (*p == 1)
                {
                    struct popup_node *n;

                    self->streamCursor = p + 1;
                    n = OperatorNew(0x18);
                    tail->next = n;
                    n->mode = 2;
                    n->y = height;
                    n->x = penX;
                    n->index = *(const u8 *)self->streamCursor;
                    n->timer = *GlyphHeightAt(self, (n->index * 2 + n->index) * 8);
                    n->next = NULL;
                    tail = n;
                    {
                        s32 off = n->index * sizeof(struct popup_glyph);

                        height = *GlyphHeightAt(self, off);
                        advance = *GlyphWidthAt(self, off);
                    }
                }
                else
                {
                    if (self->drawMode == 0)
                    {
                        advance = ICON_TEXT_CALL3(gSmallFont, 1, p, 1);
                        height = widthA;
                    }
                    else
                    {
                        advance = ICON_TEXT_CALL3(gLargeFont, 1, p, 1);
                        height = widthB;
                    }
                    if (*(const u8 *)self->streamCursor != ' ')
                    {
                        struct popup_node *n = OperatorNew(0x18);

                        tail->next = n;
                        n->mode = self->drawMode;
                        n->y = 0;
                        n->x = penX;
                        n->index = *(const u8 *)self->streamCursor;
                        tail->next->timer = height;
                        tail = tail->next;
                        tail->next = NULL;
                    }
                }
                penX += advance;
                if (maxHeight < height)
                    maxHeight = height;
                {
                    const u8 *q = self->streamCursor;

                    self->streamCursor = q + 1;
                    c = q[1];
                    if (c == '\n')
                        goto newline;
                    p = q + 1;
                }
            } while (c != 0);
        }
        if (*(const u8 *)self->streamCursor != '\n')
            goto place;
    }
newline:
    self->streamCursor = (const u8 *)self->streamCursor + 1;
place:
    {
        struct popup_node *n = lineStart->next;
        s32 counter = maxHeight + 6;

        if (n != NULL)
        {
            s32 dx = (0xf0 - penX) / 2;

            do
            {
                s32 y = n->y;
                s32 top = y + 0xa0;

                n->y = top + (maxHeight - (y + n->timer)) / 2;
                n->x += dx;
                n = n->next;
            } while (n != NULL);
        }
        self->suppressCounter = counter;
    }
}

asm(".align 2, 0");

/* The credits screen's popup-text asset loader (docs/rom_map.md's
 * `LoadCreditsLogos` note): iterates `gCreditsLogos`'s 5 records
 * (stride 0x14) into `self->asset0`-`asset4` (`struct credits_screen` above
 * - each `sp[4]+0x1c+i*0x18`-relative in the ROM's own indexing),
 * converting each record's raw width/height into rounded Q-something
 * runtime units, DMA3-transferring custom glyph tile data
 * (`OperatorNewArray`/`LoadTaggedAsset`) into a freshly-decoded buffer and
 * building each glyph cell's OAM tile index via a nested nibble/row
 * loop, then loading the shared 15-color palette tail
 * (`gPaletteCache+0x2c`) the same way and pinning the freshly-built
 * asset into the shared tile cache (`ClaimPaletteSlot`).
 *
 * Built with old_agbcc. GCSE's PRE hoists any `slot << 5` (and even a
 * plain `asm("" : "+r")` copy, since a non-volatile asm with outputs is
 * an ordinary hashed expression) to the y loop's pre-test, next to the
 * `slot + 1` and `i + 1` it also hoists there, and spills it; the ROM
 * computes the palette address at the copy. The palette index is
 * therefore a copy `ps` passed through `asm volatile` (volatile asms are
 * never entered in GCSE's table), so `slot` itself and its `slot + 1`
 * hoist are untouched. `ps` is an r1 register variable (the ROM reloads
 * `slot` into r1) and gets one extra `asm("" : : "r")` reference after
 * the shift, so the shift result goes to r0 instead of reusing r1. */
/* One `gCreditsLogos` record (0x14 bytes), read through its own view of
 * `struct bg_package`: a popup glyph's size in 8-px tiles (signed here;
 * the loops compare them signed) and its tagged palette/tile assets. */
struct popup_glyph_src {
    s32 w;               /* 0x00 */
    s32 h;               /* 0x04 */
    const u32 *palette;  /* 0x08 - tagged asset, size in the header's bits 9+ */
    const u32 *tiles;    /* 0x0c - tagged asset, size in the header's bits 8+ */
    u32 unk_10;
};

void LoadCreditsLogos(struct credits_screen *self)
{
    u8 (*palSlots)[TILE_SIZE_4BPP] = gPaletteCache->slots;
    s32 slot = 1;
    s32 i;

    for (i = 0; i <= 4; i++)
    {
        struct popup_glyph_src *src = (struct popup_glyph_src *)&gCreditsLogos[i];
        struct popup_glyph *glyph = (struct popup_glyph *)((u8 *)self + 0x1c + (i * 2 + i) * 8);
        u8 *tiles;
        u16 *pal;
        s32 size;
        s32 y;

        {
            s32 w = src->w;
            s32 h = src->h;

            glyph->height = h << 3;
            glyph->width = w << 3;
            glyph->cols = (w + 3) / 4;
            glyph->rows = (h + 3) / 4;
        }
        tiles = OperatorNewArray(*src->tiles >> 8);
        LoadTaggedAsset(src->tiles, tiles);
        size = (glyph->cols * glyph->rows) << 9;
        glyph->tiles = OperatorNewArray(size);
        {
            u32 zero = 0;
            struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;

            dma->src = (u32)&zero;
            dma->dst = (u32)glyph->tiles;
            dma->cnt = (size / 4) | 0x85000000;
            dma->cnt;
        }
        for (y = 0; y < src->h; y++)
        {
            s32 x;

            for (x = 0; x < src->w; x++)
            {
                s32 cell = (y >> 2) * glyph->cols + (x >> 2);
                s32 sub = (x & 3) + ((y & 3) << 2);
                struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;

                dma->src = (u32)(tiles + (y * src->w + x) * 32);
                dma->dst = (u32)((u8 *)glyph->tiles + (((cell << 4) + sub) << 5));
                dma->cnt = 0x84000008;
                dma->cnt;
            }
        }
        if (tiles != NULL)
            OperatorDeleteArray(tiles);
        pal = OperatorNewArray((*src->palette >> 9) << 1);
        LoadTaggedAsset(src->palette, pal);
        {
            u16 *s = pal;
            /* Escaped copy of `slot`: see the note above. */
            register s32 ps asm("r1") = slot;
            s32 sh;
            u16 *d;
            s32 k;

            asm volatile("" : "+r"(ps)); /* new pseudo GCSE can't hoist */
            sh = ps << 5;
            asm("" : : "r"(ps)); /* keeps ps live so sh doesn't reuse r1 */
            d = (u16 *)(sh + (u32)palSlots);

            for (k = 15; k >= 0; k--)
                *d++ = *s++;
        }
        if (pal != NULL)
            OperatorDeleteArray(pal);
        ClaimPaletteSlot(gPaletteCache, slot);
        *(s32 *)&glyph->palette = slot;
        slot++;
    }
}

asm(".align 2, 0");

/* --------------------------------------------------------------------
 * CommitCreditsFrame - end-of-frame commit for the credits screen: resets BG0's
 * scroll registers, flushes the shared tile cache and OAM shadow
 * buffer, and flushes the VRAM upload queue. Takes (and ignores) an
 * unused `self` argument - both of CreditsLoop's call sites pass it
 * anyway (leftover from a shared call-site shape with its neighbors),
 * so the parameter is kept here rather than dropped, to match the
 * ROM's own call sites byte-for-byte. */
void CommitCreditsFrame(void *unused)
{
    CommitDispcnt();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
}

/* --------------------------------------------------------------------
 * DestroyCredits - credits screen teardown: kicks the starfield's own
 * teardown (`DestroyStarfield(starfield, 3)`) if one was ever built, frees each
 * of the five popup-asset buffers still allocated, then frees `self`
 * too when `mode` bit 0 is set.
 * ------------------------------------------------------------------ */
void DestroyCredits(struct credits_screen *self, s32 mode)
{
    u8 *slot;
    s32 i;

    if (self->starfield != NULL)
        DestroyStarfield(self->starfield, 3);

    slot = (u8 *)&self->glyphs[0].tiles;
    i = 4;
    do {
        if (*(void **)slot != NULL)
            OperatorDeleteArray(*(void **)slot);
        slot += 0x18;
        i--;
    } while (i >= 0);

    if (mode & 1)
        OperatorDelete(self);
}

/* --------------------------------------------------------------------
 * RunCredits - the credits screen's top-level
 * entry point (docs/rom_map.md's "A fourth thing in this file"
 * section): allocates and constructs the screen (InitCredits), runs it
 * to completion (CreditsLoop), and tears it down (DestroyCredits).
 * ------------------------------------------------------------------ */
void RunCredits(void)
{
    struct credits_screen *self = InitCredits(OperatorNew(0x98));

    CreditsLoop(self);
    if (self != NULL)
        DestroyCredits(self, 3);
}

asm(".align 2, 0");
