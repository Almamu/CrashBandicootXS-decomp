#include "core.h"
#include "gba/io_reg.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "audio.h"
#include "gba/dma_macros.h"

/* GitHub issue #64 (0x08034AA4-0x080354E0, 13 functions). Continues
 * straight on from issue #63's fade-overlay cluster (actor_part87.c/
 * actor_part88.c/actor_part89.c) - the first five functions here
 * (sub_8034AA4/sub_8034C40/sub_8034C5C/sub_8034C84/sub_8034CB0) are more
 * methods on that same `struct fade_overlay` "self" object, then the
 * chunk moves on to an unrelated "between-level map/progress screen"
 * object driven directly from `game_loop` (see docs/rom_map.md's
 * "sub_8034CB0 turns out to be a separate screen trigger"/"A fourth
 * thing in this file" sections) - see
 * docs/matching/issue-64-0x08034aa4-actor.md for the full write-up. */

/* Same `struct fade_overlay` as actor_part87.c/88.c/89.c, redeclared
 * locally per this project's minimal-local-type convention. This
 * chunk's functions pin down real meanings for two fields actor_part87.c
 * left vague: `unused_1c` is a per-item blink/flash toggle counter
 * (kept the same field name there since that file never touches it),
 * and `flag_20` - guessed there as "which of two alternating cue sfx
 * last fired" - turns out, in this sibling function set, to hold the
 * Yes/No dialog's currently-selected option index (0/1; any other value
 * means neither option is highlighted) instead. Same field, a related
 * but distinct use by this file's functions. */
struct fade_overlay {
    u8 *bg1Buf;   /* 0x00 */
    u8 *bg0Buf;   /* 0x04 */
    u8 *bg2Buf;   /* 0x08 */
    u16 dispcnt;  /* 0x0c */
    u8 unused_0e[0xa];
    struct icon_manager *icons; /* 0x18 */
    s32 blinkCounter; /* 0x1c */
    s32 selection;     /* 0x20 */
};

/* The "between-level map/progress screen" object (docs/rom_map.md's "A
 * fourth thing in this file" section) - a combined minimap-reveal +
 * floating-text-popup screen driven from `UpdateGameFrame`'s level-load
 * state machine, allocated `sub_8026EDC(0x98)` by `sub_80354BC`. Only
 * the fields this file's functions actually touch are named. */

/* One timed text-popup node (0x18 bytes, `sub_8026EDC`-allocated by
 * sub_80350A4, drawn by sub_8034EF0). */
struct popup_node {
    struct popup_node *next; /* 0x00 */
    s32 x;                   /* 0x04 */
    s32 y;                   /* 0x08 - counts down while alive */
    s32 timer;               /* 0x0c - node dies once y + timer <= 0 */
    s32 mode;                /* 0x10 - 0/1: text via icon manager DC/E0, 2: glyph */
    u8 index;                /* 0x14 - glyph index / character */
};

/* One of the five custom popup glyphs `sub_80352AC` loads (0x18 bytes
 * each, at `map_screen+0x1c`). */
struct popup_glyph {
    s32 cols;    /* 0x00 - width in 32-px OAM cells */
    s32 rows;    /* 0x04 - height in 32-px OAM cells */
    s32 height;  /* 0x08 - pixel height */
    s32 width;   /* 0x0c - pixel advance */
    u8 palette;  /* 0x10 */
    void *tiles; /* 0x14 - heap buffer, freed by sub_803547C */
};

struct map_screen {
    struct popup_node *popupListHead; /* 0x00 - timed text-popup node list, see sub_80350A4 */
    const void *streamBase;   /* 0x04 - popup byte-opcode stream base */
    const void *streamCursor; /* 0x08 - popup byte-opcode stream cursor */
    void *mapObj;             /* 0x0c - the minimap object, sub_8034374 */
    s32 drawMode;              /* 0x10 */
    s32 suppressCounter;        /* 0x14 */
    u8 unused_18[4];
    struct popup_glyph glyphs[5]; /* 0x1c */
    u32 frameParity; /* 0x94 */
};


COMPILE_TIME_ASSERT(sizeof(struct map_screen) == 0x98);

extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void sub_80006A8(void);
extern void FlushVramDmaQueue(void);

/* Another instance of the by-now-familiar "refresh OAM + center text"
 * pattern (docs/rom_map.md's "sub_8034AA4 is just another instance of
 * ..." note): syncs the OAM shadow buffer and VRAM upload cursor, then
 * draws the Yes/No dialog's three labels (icon-manager mode ids
 * 0x28/0x29/0x2a) via `self->icons->record->slots[6]`'s position
 * (mode 0x28 twice - once for the plain label, once conditionally for
 * a highlighted "cursor" redraw keyed on `self->selection`), applying
 * `sub_8034C40`'s blink mask to each option's own OAM-hide byte via
 * `sub_8028A30` in between, and finally re-commits the OAM shadow
 * buffer.
 *
 * Was a NAKED transcription until the issue #64/#65 NAKED retry: each
 * label draw is a gcc 2.x virtual call through the icon manager's
 * method record (`record->slots[n]`, `_call_via_r2` = `sub_803AD80`),
 * the same `ICON_TEXT_CALL` shape settings_menu.c already matches, and
 * with that the "many live values across calls" allocation falls out
 * of plain C. */
extern struct vram_upload_cursor *gUnknown_030012FC;
extern u8 gStaticData_0817C510[];
extern void sub_8006A90(struct oam_shadow_buffer *arg0);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_8006C28(struct vram_upload_cursor *arg0);
extern s32 sub_8028A30(void *mgr, u8 arg1);
extern s32 sub_8026F38(s32 arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
s32 sub_8034C40(struct fade_overlay *self, s32 mode);

/* `record->slots[n]` on an icon manager, called with `label` (slot 0
 * measures and returns the pixel width, slot 2 draws). */
#define ICON_TEXT_CALL(mgrExpr, n, label)                                       \
    ({                                                                          \
        struct icon_manager *_m = (mgrExpr);                                    \
        struct icon_slot *_s = &_m->record->slots[n];                           \
        sub_803AD80((u8 *)_m + _s->offset, (void *)(label), _s->ptr);           \
    })

static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

void sub_8034AA4(struct fade_overlay *self)
{
    s32 w;

    sub_8006A90(gUnknown_03001300);
    sub_8006C28(gUnknown_030012FC);
    w = ICON_TEXT_CALL(self->icons, 0, sub_8026F38(0x28));
    sub_8028A30(self->icons, 0);
    set_icon_mgr_pos(self->icons, 0x88 - w, 0x87);
    ICON_TEXT_CALL(self->icons, 2, sub_8026F38(0x28));
    sub_8028A30(self->icons, sub_8034C40(self, 0));
    if (self->selection == 0)
    {
        set_icon_mgr_pos(self->icons, 0x90, 0x87);
        ICON_TEXT_CALL(self->icons, 2, gStaticData_0817C510);
    }
    set_icon_mgr_pos(self->icons, 0x98, 0x87);
    ICON_TEXT_CALL(self->icons, 2, sub_8026F38(0x29));
    sub_8028A30(self->icons, sub_8034C40(self, 1));
    if (self->selection == 1)
    {
        set_icon_mgr_pos(self->icons, 0x90, 0x91);
        ICON_TEXT_CALL(self->icons, 2, gStaticData_0817C510);
    }
    set_icon_mgr_pos(self->icons, 0x98, 0x91);
    ICON_TEXT_CALL(self->icons, 2, sub_8026F38(0x2a));
    sub_8006A48(gUnknown_03001300);
}

asm(".align 2, 0");

/* --------------------------------------------------------------------
 * sub_8034C40 - blink/toggle helper: returns 1 immediately if `mode`
 * isn't the dialog's current selection; otherwise advances the
 * selected item's blink counter and returns bit 1 of its pre-advance
 * value (a 0/2 flicker mask consumed by sub_8034AA4 to hide the label
 * every other frame-pair).
 * ------------------------------------------------------------------ */
s32 sub_8034C40(struct fade_overlay *self, s32 mode)
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
 * sub_8034C5C - one frame's "yield" helper for the fade overlay: syncs
 * the shared OAM shadow buffer, flushes the VRAM upload queue, then
 * re-applies the overlay's own DISPCNT mirror.
 * ------------------------------------------------------------------ */
void sub_8034C5C(struct fade_overlay *self)
{
    sub_80006A8();
    sub_8006AAC(gUnknown_03001300);
    FlushVramDmaQueue();
    REG_DISPCNT = self->dispcnt;
}

extern void sub_8026ED0(void *self);

/* --------------------------------------------------------------------
 * sub_8034C84 - the fade overlay's teardown: frees its three BG scratch
 * buffers, then frees `self` too when `mode` bit 0 is set.
 * ------------------------------------------------------------------ */
void sub_8034C84(struct fade_overlay *self, s32 mode)
{
    sub_8026ED0(self->bg0Buf);
    sub_8026ED0(self->bg1Buf);
    sub_8026ED0(self->bg2Buf);
    if (mode & 1)
        sub_8026ED0(self);
}

extern void *sub_8026EDC(s32 size);
extern s32 mem_free_bytes(s32 flags);
extern void *sub_803472C(void *selfArg);
extern s32 sub_8034994(void *selfArg);

/* --------------------------------------------------------------------
 * sub_8034CB0 - the "Are you sure?" confirmation-dialog trigger
 * (docs/rom_map.md's "sub_8034CB0 drives a Yes/No confirmation prompt"
 * section): allocates and builds the fade overlay (sub_803472C), runs
 * the Yes/No dialog to completion (sub_8034994), tears the overlay down
 * (sub_8034C84) if it was actually built, and returns which option was
 * selected.
 * ------------------------------------------------------------------ */
s32 sub_8034CB0(void)
{
    struct fade_overlay *self;
    u8 result;

    mem_free_bytes(0xc0000000);
    self = sub_803472C(sub_8026EDC(0x24));
    result = sub_8034994(self);
    if (self != NULL)
        sub_8034C84(self, 3);
    mem_free_bytes(0xc0000000);
    return result;
}

asm(".align 2, 0");

/* The map screen's constructor (docs/rom_map.md's "sub_8034CEC is a
 * combined constructor" note): builds the minimap sub-object
 * (`sub_8034374(sub_8026EDC(0x14))` -> `self->mapObj`), syncs the OAM
 * shadow buffer, hooks both text-icon managers
 * (`gUnknown_030012DC`/`gUnknown_030012E0`) up for this screen (firing
 * each one's slot-6 OAM trampoline via `sub_803AD7C`, and copying
 * `gUnknown_030012DC->field_12c` into `gUnknown_030012E0->field_108` -
 * a new, previously-unexplained cross-wiring between the two icon
 * managers), loads the popup-text glyph assets (`sub_80352AC`), resets
 * the shared tile cache and VRAM upload cursor, initializes the
 * popup-text opcode-stream fields to `gStaticData_0817C5D0`, sets the
 * DISPCNT "OBJ enable"-adjacent bit in `gUnknown_03001288`, and ducks
 * the audio context (`sub_8001B54(gUnknown_030012BC, 0x11)`). Returns
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
 * the allocator into giving `&gUnknown_030012B8`/`&gUnknown_030012FC`
 * r4 and `&gUnknown_030012DC` r6 as the ROM does. */
extern void *sub_8034374(void *arg0);
extern void sub_8006EA8(struct tile_asset_cache *cache);
extern void sub_8028A40(struct icon_manager *mgr);
extern void sub_80352AC(struct map_screen *self);
extern void sub_8006DC8(struct tile_asset_cache *arg0);
extern void sub_8006C4C(struct vram_upload_cursor *self);
extern s32 sub_8006C58(struct vram_upload_cursor *self, s32 size);
extern void sub_8006C30(struct vram_upload_cursor *self);
extern void sub_803AD7C(void *self, void *fn);
extern void sub_8001614(void);
extern void sub_8001B54(struct AudioContext *self, u32 id);
extern struct tile_asset_cache *gUnknown_030012B8;
extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern u8 gUnknown_03001288[2];
extern struct AudioContext *gUnknown_030012BC;
extern u8 gStaticData_0817C5D0[];
extern void *sub_8026EDC(s32 size);

/* Sets the manager's glyph tile base and fires its slot-6 method. */
static inline void IconSetBase(struct icon_manager *m, u32 base)
{
    struct icon_slot *slot;

    m->field_108 = base;
    slot = &m->record->slots[6];
    sub_803AD7C((u8 *)m + slot->offset, slot->ptr);
}

/* Reserves `m`'s glyph tiles (`field_12c` tiles) from the VRAM upload
 * cursor `c`. */
static inline void IconReserveVram(struct vram_upload_cursor *c, struct icon_manager *m)
{
    sub_8006C58(c, m->field_12c << 5);
}

struct map_screen *sub_8034CEC(struct map_screen *self)
{
    self->mapObj = sub_8034374(sub_8026EDC(0x14));
    sub_8006A90(gUnknown_03001300);
    sub_8006A48(gUnknown_03001300);
    sub_80006A8();
    sub_8006AAC(gUnknown_03001300);
    sub_8006EA8(gUnknown_030012B8);
    sub_8028A40(gUnknown_030012DC);
    sub_8028A30(gUnknown_030012E0, 0);
    asm("");
    sub_80352AC(self);
    sub_8006DC8(gUnknown_030012B8);
    gUnknown_030012FC->field_08 = 0;
    sub_8006C4C(gUnknown_030012FC);
    sub_8006C4C(gUnknown_030012FC);
    IconSetBase(gUnknown_030012DC, 0);
    IconReserveVram(gUnknown_030012FC, gUnknown_030012DC);
    {
        u32 base = gUnknown_030012DC->field_12c;

        IconSetBase(gUnknown_030012E0, base);
    }
    IconReserveVram(gUnknown_030012FC, gUnknown_030012E0);
    sub_8006C30(gUnknown_030012FC);
    self->popupListHead = NULL;
    self->drawMode = 0;
    self->streamBase = gStaticData_0817C5D0;
    self->streamCursor = gStaticData_0817C5D0;
    self->suppressCounter = 0;
    gUnknown_03001288[1] |= 0x10;
    sub_8001614();
    self->frameParity = 0;
    sub_8001B54(gUnknown_030012BC, 0x11);
    return self;
}

asm(".align 2, 0");

extern void sub_80007AC(void *arg0);
extern void sub_80350A4(struct map_screen *self);
extern void sub_8034EF0(struct map_screen *self);
extern void sub_803544C(void *unused);
extern void sub_8034688(void *mapObj);
extern void sub_8001AC4(struct AudioContext *self, u32 value);
extern struct AudioContext *gUnknown_030012BC;
extern void *gUnknown_03001304;

struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gUnknown_030007E0;

/* --------------------------------------------------------------------
 * sub_8034E2C - the map screen's per-frame driver: an input-gated busy
 * loop toggling `frameParity` every iteration (driving the popup-text
 * system, sub_80350A4, every other frame) alongside the minimap reveal
 * (sub_8034688) every frame, until confirm or D-pad-down+L is pressed;
 * then a fixed 17-frame wipe/transition effect poking the window-blend
 * hardware registers directly; then frees every remaining popup-text
 * list node.
 * ------------------------------------------------------------------ */
void sub_8034E2C(struct map_screen *self)
{
    s32 i;

    while (1) {
        sub_80007AC(gUnknown_03001304);
        {
            register struct held_pressed_pair *p asm("r1") = &gUnknown_030007E0;
            register s32 mask asm("r0") = 9;

            mask &= p->pressed;
            if (mask)
                break;
        }

        self->frameParity = (self->frameParity + 1) & 1;
        if (self->frameParity != 0)
            sub_80350A4(self);

        sub_8034EF0(self);
        sub_80006A8();
        sub_803544C(self);
        sub_8034688(self->mapObj);
    }

    sub_8001AC4(gUnknown_030012BC, 0);

    for (i = 0; i <= 0x10; i++) {
        self->frameParity = (self->frameParity + 1) & 1;
        if (self->frameParity != 0)
            sub_80350A4(self);

        sub_8034EF0(self);
        sub_80006A8();
        REG_BLDY = i;
        REG_BLDCNT = 0xff;
        sub_803544C(self);
        sub_8034688(self->mapObj);
    }

    if (self->popupListHead != NULL) {
        void *node = self->popupListHead;
        do {
            void *next = *(void **)node;
            sub_8026ED0(node);
            node = next;
        } while (node != NULL);
    }
    self->popupListHead = NULL;
}

asm(".align 2, 0");

/* The map screen's per-frame OAM-icon draw dispatcher for the minimap
 * object (`self->mapObj`, the `sp[0xc]`-cached argument throughout):
 * for `mapObj->drawMode` (see `struct map_screen` above) 0/1/2 draws a
 * single centered label via `sub_803AD80` against
 * `gUnknown_030012DC`/`gUnknown_030012E0`; for any other drawMode value
 * (docs/rom_map.md's minimap/radar-dot description) DMA3-transfers a
 * procedurally-built tile buffer and iterates a per-tile record array,
 * building each dot's OAM attribute halfwords in place and applying
 * them via `sub_8006AC8`, before advancing to the next linked object in
 * `mapObj`'s list and repeating.
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

extern s32 sub_8006C44(struct vram_upload_cursor *self);
extern s32 sub_8006C84(struct vram_upload_cursor *self, void *src, s32 size);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_8006AC8(struct oam_shadow_buffer *self, void *record);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);

void sub_8034EF0(struct map_screen *self)
{
    struct popup_node *node;

    sub_8006A90(gUnknown_03001300);
    sub_8006C28(gUnknown_030012FC);
    for (node = self->popupListHead; node != NULL; node = node->next)
    {
        struct icon_manager *m;

        switch (node->mode)
        {
        case 0:
            m = gUnknown_030012DC;
            goto draw;
        case 1:
            m = gUnknown_030012E0;
        draw:
            set_icon_mgr_pos(m, node->x, node->y);
            sub_803AD80((u8 *)m + m->record->slots[4].offset, (void *)(u32)node->index, m->record->slots[4].ptr);
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

            tile = sub_8006C44(gUnknown_030012FC);
            sub_8006C84(gUnknown_030012FC, glyph->tiles, (glyph->rows * glyph->cols) << 9);
            zero = 0;
            sub_803A94C(&zero, &oam, 0x05000002);
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
                        sub_8006AC8(gUnknown_03001300, &oam);
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
    sub_8006A48(gUnknown_03001300);
}

asm(".align 2, 0");

/* The map screen's floating-text popup driver (docs/rom_map.md's "A
 * floating-text/glyph popup system" note): first walks `self`'s
 * `popupListHead` linked list, decrementing each node's countdown pair
 * (`+8`/`+0xc`) and unlinking/freeing (`sub_8026ED0`) any node whose
 * sum has expired; then, unless `self->suppressCounter` is still
 * counting down, parses `self`'s byte-opcode stream
 * (`streamCursor`/`streamBase`, `struct map_screen`) - opcodes 0/1
 * allocate and link a new 0x18-byte popup node (mode 0/1 respectively),
 * 2/3 set `self->drawMode`, 0xA terminates a line (measuring both
 * `gUnknown_030012DC`/`030012E0`'s text width via `sub_8028968` first)
 * - drawing the assembled line centered via `sub_803AD84` once `self`'s
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
extern s32 sub_8028968(struct icon_manager *mgr, const u8 *text);
extern s32 sub_803AD84(void *self, const void *a, s32 b, void *fn);
extern void *sub_8026EDC(s32 size);
extern u8 gStaticData_0817CF3C[];

#define ICON_TEXT_CALL3(mgrExpr, n, a, b)                                      \
    ({                                                                          \
        struct icon_manager *_m = (mgrExpr);                                    \
        struct icon_slot *_s = &_m->record->slots[n];                           \
        sub_803AD84((u8 *)_m + _s->offset, (a), (b), _s->ptr);                  \
    })

static inline s32 *GlyphHeightAt(struct map_screen *self, s32 off)
{
    u8 *base = (u8 *)&self->glyphs[0].height;
    return (s32 *)(base + off);
}

static inline s32 *GlyphWidthAt(struct map_screen *self, s32 off)
{
    u8 *base = (u8 *)&self->glyphs[0].width;
    return (s32 *)(base + off);
}

void sub_80350A4(struct map_screen *self)
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
            sub_8026ED0(n);
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

    widthA = sub_8028968(gUnknown_030012DC, gStaticData_0817CF3C);
    widthB = sub_8028968(gUnknown_030012E0, gStaticData_0817CF3C);
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
                    n = sub_8026EDC(0x18);
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
                        advance = ICON_TEXT_CALL3(gUnknown_030012DC, 1, p, 1);
                        height = widthA;
                    }
                    else
                    {
                        advance = ICON_TEXT_CALL3(gUnknown_030012E0, 1, p, 1);
                        height = widthB;
                    }
                    if (*(const u8 *)self->streamCursor != ' ')
                    {
                        struct popup_node *n = sub_8026EDC(0x18);

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

/* The map screen's popup-text asset loader (docs/rom_map.md's
 * `sub_80352AC` note): iterates `gStaticData_0817CF40`'s 5 records
 * (stride 0x14) into `self->asset0`-`asset4` (`struct map_screen` above
 * - each `sp[4]+0x1c+i*0x18`-relative in the ROM's own indexing),
 * converting each record's raw width/height into rounded Q-something
 * runtime units, DMA3-transferring custom glyph tile data
 * (`sub_8026EC0`/`LoadTaggedAsset`) into a freshly-decoded buffer and
 * building each glyph cell's OAM tile index via a nested nibble/row
 * loop, then loading the shared 15-color palette tail
 * (`gUnknown_030012B8+0x2c`) the same way and pinning the freshly-built
 * asset into the shared tile cache (`sub_8006D50`).
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
/* One `gStaticData_0817CF40` record (0x14 bytes): a popup glyph's size
 * in 8-px tiles and its tagged palette/tile assets. */
struct popup_glyph_src {
    s32 w;               /* 0x00 */
    s32 h;               /* 0x04 */
    const u32 *palette;  /* 0x08 - tagged asset, size in the header's bits 9+ */
    const u32 *tiles;    /* 0x0c - tagged asset, size in the header's bits 8+ */
    u32 unk_10;
};

extern struct popup_glyph_src gStaticData_0817CF40[];
extern void *sub_8026EC0(u32 size);
extern void sub_8026EB4(void *ptr);
extern void LoadTaggedAsset(const void *asset, void *dest);
extern s32 sub_8006D50(struct tile_asset_cache *cache, s32 index);

void sub_80352AC(struct map_screen *self)
{
    u8 (*palSlots)[TILE_SIZE_4BPP] = gUnknown_030012B8->slots;
    s32 slot = 1;
    s32 i;

    for (i = 0; i <= 4; i++)
    {
        struct popup_glyph_src *src = &gStaticData_0817CF40[i];
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
        tiles = sub_8026EC0(*src->tiles >> 8);
        LoadTaggedAsset(src->tiles, tiles);
        size = (glyph->cols * glyph->rows) << 9;
        glyph->tiles = sub_8026EC0(size);
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
            sub_8026EB4(tiles);
        pal = sub_8026EC0((*src->palette >> 9) << 1);
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
            sub_8026EB4(pal);
        sub_8006D50(gUnknown_030012B8, slot);
        *(s32 *)&glyph->palette = slot;
        slot++;
    }
}

asm(".align 2, 0");

extern void sub_8001614(void);
extern void sub_8006DC8(struct tile_asset_cache *arg0);
extern struct tile_asset_cache *gUnknown_030012B8;

/* --------------------------------------------------------------------
 * sub_803544C - end-of-frame commit for the map screen: resets BG0's
 * scroll registers, flushes the shared tile cache and OAM shadow
 * buffer, and flushes the VRAM upload queue. Takes (and ignores) an
 * unused `self` argument - both of sub_8034E2C's call sites pass it
 * anyway (leftover from a shared call-site shape with its neighbors),
 * so the parameter is kept here rather than dropped, to match the
 * ROM's own call sites byte-for-byte. */
void sub_803544C(void *unused)
{
    sub_8001614();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    sub_8006DC8(gUnknown_030012B8);
    sub_8006AAC(gUnknown_03001300);
    FlushVramDmaQueue();
}

extern void sub_80346FC(void *self, s32 arg1);
extern void sub_8026EB4(void *ptr);

/* --------------------------------------------------------------------
 * sub_803547C - map screen teardown: kicks the minimap object's own
 * teardown (`sub_80346FC(mapObj, 3)`) if one was ever built, frees each
 * of the five popup-asset buffers still allocated, then frees `self`
 * too when `mode` bit 0 is set.
 * ------------------------------------------------------------------ */
void sub_803547C(struct map_screen *self, s32 mode)
{
    u8 *slot;
    s32 i;

    if (self->mapObj != NULL)
        sub_80346FC(self->mapObj, 3);

    slot = (u8 *)&self->glyphs[0].tiles;
    i = 4;
    do {
        if (*(void **)slot != NULL)
            sub_8026EB4(*(void **)slot);
        slot += 0x18;
        i--;
    } while (i >= 0);

    if (mode & 1)
        sub_8026ED0(self);
}

/* --------------------------------------------------------------------
 * sub_80354BC - the between-level map/progress screen's top-level
 * entry point (docs/rom_map.md's "A fourth thing in this file"
 * section): allocates and constructs the screen (sub_8034CEC), runs it
 * to completion (sub_8034E2C), and tears it down (sub_803547C).
 * ------------------------------------------------------------------ */
void sub_80354BC(void)
{
    struct map_screen *self = sub_8034CEC(sub_8026EDC(0x98));

    sub_8034E2C(self);
    if (self != NULL)
        sub_803547C(self, 3);
}

asm(".align 2, 0");
