#include "core.h"
#include "icon_manager.h"
#include "pause_options_screen.h"
#include "actor.h"
#include "pause_screen_results.h"
#include "vram_pool.h"

extern s32 FontSetPalette(void *mgr, s32 arg1);
extern s32 GetUiText(s32 arg0);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern struct icon_manager *gLargeFont;
extern struct icon_manager *gSmallFont;

static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Calls `record->slots[n]` on an icon manager with `label` (slot 0
 * measures and returns the pixel width, slot 2 draws) - a gcc 2.x
 * virtual call through libgcc's `_call_via_r2`. A statement macro so
 * `this` is computed before the label argument, as in the ROM. */
#define ICON_TEXT_CALL(mgrExpr, n, label)                                       \
    ({                                                                          \
        struct icon_manager *_m = (mgrExpr);                                    \
        struct icon_slot *_s = &_m->record->slots[n];                           \
        _call_via_r2((u8 *)_m + _s->offset, (void *)(label), _s->ptr);           \
    })

/* The functions below (0x08003B40-0x080041BC) were NAKED
 * transcriptions until the issue #4/#6/#8 retry
 * (docs/matching/issue-4-6-8-naked-retry.md); sub_8003D3C followed in
 * docs/matching/early-rom-naked-retry.md, and the raw `sub_800450C` at
 * the end of the file in docs/matching/hard-register-hold-retry.md.
 * All now match as plain C; the file is built with old_agbcc (Makefile
 * OLD_AGBCC_OBJS) because `sub_800450C` only matches under it - every
 * other function here compiles identically under both compilers. Their
 * siblings `sub_8004914`/`sub_80049CC` live in
 * `src/graphics/settings_menu23.c`. */

extern void *sub_8026EDC(s32 size);
extern void sub_8002FCC(void *newObj, void *tmpl);
extern void sub_8002FD8(void *newObj);
extern void WaitForVBlank(void);
extern void UpdateKeys(void *arg0);
extern void *gUnknown_03001304;
extern u32 gKeys;
extern u8 gUnknown_03000800;
extern void *gLinkSession;
extern s32 sub_8001F50(void *arg0);
extern s32 sub_8002EFC(void *newObj);
extern s32 sub_8002FD4(void *newObj);
extern void MemCopy32(void *arg0, s32 arg1, s32 arg2);
extern void sub_8026ED0(void *newObj);

/* A "connecting..." SIO-handshake spinner dialog: allocates a small
 * icon object from self->field_8c's template, then loops VBlank-
 * waiting while polling input (cancel -> state 3), the link-active
 * flag gUnknown_03000800, and sub_8001F50 (the link-connection/
 * handshake driver documented in docs/rom_map.md's SIO/link-cable
 * section) until the spinner object's own state (sub_8002EFC) settles.
 * Returns that state; when it settles at 0, also feeds a result value
 * through self->field_90 via MemCopy32.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The cancel test is `(u16)(keys & 2)`, whose known-zero
 * value the ROM reuses to clear `gUnknown_03000800`, and the
 * `sub_8002FD4` result is taken before `self->field_90` is loaded. */
s32 sub_8003B40(struct pause_options_screen *self)
{
    void *spinner = sub_8026EDC(0x220);
    s32 state;

    sub_8002FCC(spinner, self->field_8c);
    sub_8002FD8(spinner);
    do {
        WaitForVBlank();
        UpdateKeys(gUnknown_03001304);
        if ((u16)(gKeys & 2)) {
            state = 3;
        } else {
            if (gUnknown_03000800) {
                gUnknown_03000800 = 0;
                sub_8002FD8(spinner);
            }
            sub_8001F50(gLinkSession);
            state = sub_8002EFC(spinner);
        }
    } while (state == 1);
    if (state == 0) {
        s32 result = sub_8002FD4(spinner);

        MemCopy32(self->field_90, result, 0x200);
    }
    sub_8026ED0(spinner);
    return state;
}

/* Draws `label1` (if non-zero) centered at Y=0x87, then `label2` (if
 * non-zero) centered at Y=0x91, both into gSmallFont.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers once the centre X gets its own local (`x = (0xf0 - w) >> 1`),
 * which is what puts it in r3 and the Y constant in ip. */
void sub_8003BDC(struct pause_options_screen *self, s32 label1, s32 label2)
{
    s32 w, x;

    FontSetPalette(gSmallFont, 0);
    if (label1) {
        w = ICON_TEXT_CALL(gSmallFont, 0, label1);
        x = (0xf0 - w) >> 1;
        set_icon_mgr_pos(gSmallFont, x, 0x87);
        ICON_TEXT_CALL(gSmallFont, 2, label1);
    }
    if (label2) {
        w = ICON_TEXT_CALL(gSmallFont, 0, label2);
        x = (0xf0 - w) >> 1;
        set_icon_mgr_pos(gSmallFont, x, 0x91);
        ICON_TEXT_CALL(gSmallFont, 2, label2);
    }
}

/* Same centered-label shape as sub_80049CC (src/graphics/settings_menu20.c),
 * but always label 0x23, drawn into gSmallFont (not E0) at
 * fixed Y=0x87, and with a highlight-dependent initial visibility call.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers (same shape as sub_8004914, src/graphics/settings_menu23.c). */
void sub_8003C90(struct pause_options_screen *self, u8 highlight)
{
    s32 w;

    if (highlight)
        FontSetPalette(gSmallFont, ((self->flags >> 2) & 1) ? 1 : 2);
    else
        FontSetPalette(gSmallFont, 0);
    w = ICON_TEXT_CALL(gSmallFont, 0, GetUiText(0x23));
    set_icon_mgr_pos(gSmallFont, (0xf0 - w) >> 1, 0x87);
    ICON_TEXT_CALL(gSmallFont, 2, GetUiText(0x23));
}

extern u8 gStaticData_0816B138[];


/* Draws `value`'s label centered at Y=0x87, then draws a
 * highlighted/plain pair of fixed labels (0x29/0x2a, purpose
 * unconfirmed) swapping Y=0x87 vs Y=0x91 depending on `self->field_10`
 * - each pair member's slot gets a `gStaticData_0816B138` draw at its
 * *previous* position right before the real label, which reads as a
 * clear/overwrite step rather than a width probe (the return value is
 * never used).
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The ROM keeps the record offset 0x130 in r8 and the Y
 * constant 0x87 in sb: `y` is pinned to r9 and set after the
 * manager pointer is loaded (the unpinned draft swapped the two, as
 * global-alloc ranks 0x87 slightly above 0x130). */
void sub_8003D3C(struct pause_options_screen *self, s32 value)
{
    s32 w;
    register s32 y asm("r9");

    FontSetPalette(gSmallFont, 0);
    w = ICON_TEXT_CALL(gSmallFont, 0, GetUiText(value));
    {
        s32 x = 0xa0 - w;
        struct icon_manager *m = gSmallFont;
        y = 0x87;
        set_icon_mgr_pos(m, x, y);
    }
    ICON_TEXT_CALL(gSmallFont, 2, GetUiText(value));
    FontSetPalette(gSmallFont, ((self->flags >> 2) & 1) ? 1 : 2);
    if (!self->field_10) {
        set_icon_mgr_pos(gSmallFont, 0xa8, y);
        ICON_TEXT_CALL(gSmallFont, 2, gStaticData_0816B138);
        set_icon_mgr_pos(gSmallFont, 0xb0, y);
        ICON_TEXT_CALL(gSmallFont, 2, GetUiText(0x29));
    } else {
        set_icon_mgr_pos(gSmallFont, 0xa8, 0x91);
        ICON_TEXT_CALL(gSmallFont, 2, gStaticData_0816B138);
        set_icon_mgr_pos(gSmallFont, 0xb0, 0x91);
        ICON_TEXT_CALL(gSmallFont, 2, GetUiText(0x2a));
    }
    FontSetPalette(gSmallFont, 0);
    if (!self->field_10) {
        set_icon_mgr_pos(gSmallFont, 0xb0, 0x91);
        ICON_TEXT_CALL(gSmallFont, 2, GetUiText(0x2a));
    } else {
        set_icon_mgr_pos(gSmallFont, 0xb0, 0x87);
        ICON_TEXT_CALL(gSmallFont, 2, GetUiText(0x29));
    }
}

extern u8 IsSaveSlotEmpty(void *handle, s32 rowIndex);
extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern s32 itoa(s32 value, u8 *buffer, s32 base);

/* `rowObjA`/`rowObjB`/`rowObjC` entries (see pause_options_screen.h)
 * are small on-screen objects with just a Q8 `x`/`y` position at their
 * front - `sub_800450C` (this chunk's other remaining function,
 * currently still fully raw) allocates and positions them. */
struct row_obj {
    s32 x;
    s32 y;
};

/* A one-byte by-value argument: the caller stores it into its stack
 * slot with `strb` and the callee reads it back with `ldrb` (a promoted
 * `u8` parameter is stored with `str` and read as a whole word). */
struct byte_arg {
    u8 v;
} __attribute__((packed));

static inline void place_row_obj(void *p, s32 x, s32 y)
{
    struct row_obj *o = p;

    o->x = x << 8;
    o->y = y << 8;
    sub_8008890(o, 0, 0);
}

/* The shared highlight/dim state call: selected rows draw in the
 * palette `self->flags` bit 2 picks (1 or 2), others in palette 0. */
#define SET_HIGHLIGHT(mgrExpr, flag)                                            \
    if (flag)                                                                   \
        FontSetPalette((mgrExpr), ((self->flags >> 2) & 1) ? 1 : 2);               \
    else                                                                        \
        FontSetPalette((mgrExpr), 0)

/* Draws this settings row's three numeric stat values -
 * `statPtr->gems`/`field_10`/`field_8` of the row's own `struct
 * settings_row_stats` (`statPtr` is `(&self->currentStats)[rowIdx]`,
 * i.e. `currentStats` and `rowStats[0..3]` read as one contiguous
 * 5-element array - `RefreshSaveSlotSummaries`/`SummarizeProgress`,
 * `src/graphics/settings_menu2.c`, already establish `rowStats` as
 * this same array shape) - as plain decimal strings into
 * `self->rowObjA[rowIdx]`/`rowObjC[rowIdx]`/`rowObjB[rowIdx]`
 * respectively (each drawn via `gSmallFont`'s `record->slots[2]`
 * trampoline, and each preceded by the same highlight/dim
 * `FontSetPalette` call this chunk's other row-label functions already
 * establish - `sub_80041BC`'s own `flag` parameter selects which row
 * is "selected", matching that shared idiom). A fourth value
 * (`statPtr->percent`) is formatted as `"NN%"` by `itoa`-ing then
 * manually scanning for the NUL terminator and overwriting it with a
 * literal `%` byte (re-terminating one byte later) - measured once via
 * `gLargeFont`'s `slots[0]` trampoline to get its pixel width,
 * then drawn a second time via that same manager's `slots[2]`
 * trampoline, right-aligned against `label1` using the measured
 * width (`posX = label1 - width + 0x1f`) - the standard
 * "measure, then right-align" idiom this ROM region uses throughout.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The fifth argument is a packed one-byte struct
 * (`struct byte_arg`): the callee reads it with `ldrb` and the caller
 * stores it with `strb`. Each block keeps running `x`/`y` locals, and
 * the third block re-derives `y` the same way the second does, which
 * reproduces the ROM spilling it. See docs/matching/issue-4-6-8-naked-retry.md. */
void sub_8003F30(struct pause_options_screen *self, s32 label1, s32 label2, s32 rowIdx, struct byte_arg flagArg)
{
    u8 flag = flagArg.v;
    u8 buf[8];
    struct settings_row_stats *stats = &(&self->currentStats)[rowIdx];
    s32 x, y, i;
    s32 w;

    x = label1 + 0x2b;
    y = label2 + 5;
    place_row_obj(self->rowObjA[rowIdx], x, y);
    x += 0xd;
    y = label2;
    itoa(stats->gems, buf, 10);
    SET_HIGHLIGHT(gSmallFont, flag);
    set_icon_mgr_pos(gSmallFont, x, y);
    ICON_TEXT_CALL(gSmallFont, 2, buf);

    x = label1 + 7;
    y = label2 + 0x1e;
    place_row_obj(self->rowObjC[rowIdx], x, y);
    x += 9;
    y -= 7;
    itoa(stats->crystals, buf, 10);
    SET_HIGHLIGHT(gSmallFont, flag);
    set_icon_mgr_pos(gSmallFont, x, y);
    ICON_TEXT_CALL(gSmallFont, 2, buf);

    x = label1 + 0x2b;
    y = label2 + 0x1e;
    place_row_obj(self->rowObjB[rowIdx], x, y);
    x += 0xd;
    y -= 7;
    itoa(stats->relics, buf, 10);
    SET_HIGHLIGHT(gSmallFont, flag);
    set_icon_mgr_pos(gSmallFont, x, y);
    ICON_TEXT_CALL(gSmallFont, 2, buf);

    itoa(stats->percent, buf, 10);
    i = 0;
    y = label2 - 2;
    for (; i < 7; i++) {
        if (buf[i] == 0) {
            buf[i] = '%';
            buf[i + 1] = 0;
            break;
        }
    }
    w = ICON_TEXT_CALL(gLargeFont, 0, buf);
    SET_HIGHLIGHT(gLargeFont, flag);
    set_icon_mgr_pos(gLargeFont, label1 - w + 0x1f, y);
    ICON_TEXT_CALL(gLargeFont, 2, buf);
}

/* An inlined copy of sub_8004914 (src/graphics/settings_menu23.c): the
 * row's highlighted/dimmed 0x25 glyph centred at (arg1 + 0x1d, arg2 + 0xc). */
static inline void draw_row_mark(struct pause_options_screen *self, s32 arg1, s32 arg2, u8 arg3)
{
    s32 x = arg1 + 0x1d;
    s32 y = arg2 + 0xc;
    s32 w;

    if (arg3)
        FontSetPalette(gSmallFont, ((self->flags >> 2) & 1) ? 1 : 2);
    else
        FontSetPalette(gSmallFont, 0);
    w = ICON_TEXT_CALL(gSmallFont, 0, GetUiText(0x25));
    set_icon_mgr_pos(gSmallFont, x - w / 2, y);
    ICON_TEXT_CALL(gSmallFont, 2, GetUiText(0x25));
}

#define DRAW_ROW(i, labelX, labelY)                                             \
    if (IsSaveSlotEmpty(handle, (i))) {                                             \
        draw_row_mark(self, (labelX), (labelY), selectedIndex == (i));          \
    } else {                                                                    \
        struct byte_arg sel;                                                    \
        sel.v = selectedIndex == (i);                                           \
        sub_8003F30(self, (labelX), (labelY), (i) + 1, sel);                    \
    }

/* Per docs/rom_map.md's "narrowed down which screen overlay_ui is"
 * section: one of 4 settings rows, `handle`/`selectedIndex` from the
 * 6-wrapper-caller family (DrawSaveMenuConfirmDelete etc.,
 * src/graphics/settings_menu3.c). When `IsSaveSlotEmpty(handle, i)`
 * reports row `i` selected, draws a highlighted numeric glyph
 * (label 0x25) centered at the row's fixed position; otherwise draws
 * the row's normal label pair via sub_8003F30 (above in this file),
 * flagged if `selectedIndex == i`. The four rows' fixed anchors: row 0
 * = (0x43,0x2d)/labels(0x26,0x21)/idx 1; row 1 = (0x43,0x5f)/
 * (0x26,0x53)/idx 2; row 2 = (0xa3,0x2d)/(0x86,0x21)/idx 3; row 3 =
 * (0xa3,0x5f)/(0x86,0x53)/idx 4.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The "selected" branch is an inlined copy of sub_8004914
 * (src/graphics/settings_menu23.c), `draw_row_mark` above. */
void sub_80041BC(struct pause_options_screen *self, void *handle, s32 selectedIndex)
{
    DRAW_ROW(0, 0x26, 0x21);
    DRAW_ROW(1, 0x26, 0x53);
    DRAW_ROW(2, 0x86, 0x21);
    DRAW_ROW(3, 0x86, 0x53);
}
asm(".align 2, 0");

extern struct oam_shadow_buffer *gOamBuffer;
extern void ResetOamBuffer(struct oam_shadow_buffer *arg0);
extern void HideUnusedOamEntries(struct oam_shadow_buffer *arg0);
extern void CommitOamBuffer(struct oam_shadow_buffer *arg0);
extern struct palette_cache *gPaletteCache;
extern void FreeUnlockedPaletteSlots(struct palette_cache *self);
extern s32 ClaimPaletteSlot(struct palette_cache *self, s32 index);
extern struct vram_upload_cursor *gObjVramCursor;
extern void ResetObjVram(struct vram_upload_cursor *self);
extern s32 ReserveObjVram(struct vram_upload_cursor *self, s32 size);
extern void MarkObjVram(struct vram_upload_cursor *self);
extern void _call_via_r1(void *addr, void *fn);
extern struct actor *sub_8008904(struct actor *part);
extern void sub_80087C0(struct actor *part);
extern void sub_80087B4(struct actor *part);
extern void sub_800872C(struct actor *part, u8 val);
extern void ***gUnknown_030012D0;
extern u16 gStaticData_0816B13A[16];
extern u16 gStaticData_0816B15A[16];
extern u16 gStaticData_0816B17A[16];
extern u16 gStaticData_0816B19A[16];

struct icon_frame_nibble {
    u8 lo:4;
    u8 hi:4;
};

#define SET_ICON_FRAME_NIBBLE(iconExpr) \
    (((struct icon_frame_nibble *)&(iconExpr)->field_29)->lo = sub_800815C(&(iconExpr)->base))

static inline void IconSetup(struct icon_manager *m, u32 v)
{
    struct icon_slot *slot;

    m->tileBase = v;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserve(struct icon_manager **m)
{
    struct vram_upload_cursor *c = gObjVramCursor;

    ReserveObjVram(c, (*m)->tileCount << 5);
}

#define SET_ROW_OBJ_POS(objExpr, px, py)                                        \
    {                                                                           \
        struct row_obj *_o = (objExpr);                                         \
        _o->x = (px) << 8;                                                      \
        _o->y = (py) << 8;                                                      \
    }

static inline s32 Opaque(s32 v)
{
    return v;
}

static inline void new_row_icon(struct settings_icon_actor **slot, u32 tblOff, u32 frame)
{
    struct settings_icon_actor *icon;

    icon = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
    *slot = icon;
    icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + tblOff);
    /* Plain `u8 *` store: old_agbcc's read-modify-write struct store
     * leaves a dead zero mask that the loop pass counts as a movable,
     * which kept 0x80 out of the loop pre-header. */
    if (frame)
        *(u8 *)&icon->frameIndex = Opaque(frame);
    else {
        /* The ROM computes the address first, in r0, and reloads the 0
         * into r1 after it; this pin reproduces that for frame 0. */
        register u8 *fp asm("r0") = &icon->frameIndex;
        *fp = 0;
    }
    sub_80087C0(&icon->base);
    sub_80087B4(&icon->base);
    sub_800872C(&icon->base, 0);
    {
        s32 lo = sub_800815C(&(*slot)->base);
        u8 *p = &(*slot)->field_29;
        s32 m = -16;

        if (frame == 0) {
            /* Hard-register hold (no code): keeping r1 live here makes
             * reload skip it when it copies the 15 from sl, so the third
             * icon takes r3 as in the ROM. The ROM reloaded the 0 above,
             * which moved the round-robin on; the pinned store doesn't. */
            register s32 hold asm("r1");
            asm("" : "=r"(hold));
            lo &= 15;
            asm("" : : "r"(hold));
        } else
            lo &= 15;
        *p = (*p & m) | lo;
    }
    *(u16 *)&(*slot)->field_3c = 0x80;
}

/* sub_800450C, the screen's init routine: resets the OAM shadow buffer
 * and the tile cache, loads four 16-colour palettes into cache slots
 * 0-3, re-initialises both icon managers (the same IconSetup/
 * IconReserve sequence as ShowPowerDialog), builds the three 5-entry icon
 * arrays `rowObjA/B/C` (keyframe-table bases 0x180/0x18c/0x1bc, frames
 * 1/2/0, `field_3c` = 0x80) and places five of them.
 *
 * Was raw asm (asm/code_3_1_10_4.s) with a NON_MATCHING draft; closed
 * in docs/matching/hard-register-hold-retry.md. The plain-pointer
 * stores in new_row_icon fix the loop pre-header (see
 * docs/matching/early-rom-naked-retry-2.md); the frame-0 address pin,
 * the r1 hold and the padding below fix the last 6 halfwords. */
void sub_800450C(struct pause_options_screen *self)
{
    u16 (*pal)[16];
    struct settings_icon_actor **a, **b, **c;
    s32 i;

    /* Instruction-count padding (no code): the hold's asm statements in
     * new_row_icon shift gcc's temporary numbering, which swaps the
     * rowObj pointer stack slots; three bare asm("") restore the ROM's
     * slot order. */
    asm("");
    asm("");
    asm("");
    ResetOamBuffer(gOamBuffer);
    HideUnusedOamEntries(gOamBuffer);
    WaitForVBlank();
    CommitOamBuffer(gOamBuffer);
    FreeUnlockedPaletteSlots(gPaletteCache);
    ClaimPaletteSlot(gPaletteCache, 0);
    ClaimPaletteSlot(gPaletteCache, 1);
    ClaimPaletteSlot(gPaletteCache, 2);
    ClaimPaletteSlot(gPaletteCache, 3);
    pal = (u16 (*)[16])gPaletteCache->slots;
    for (i = 0; i < 16; i++) {
        pal[0][i] = gStaticData_0816B13A[i];
        pal[1][i] = gStaticData_0816B15A[i];
        pal[2][i] = gStaticData_0816B17A[i];
        pal[3][i] = gStaticData_0816B19A[i];
    }
    FontSetPalette(gSmallFont, 0);
    FontSetPalette(gLargeFont, 0);
    gObjVramCursor->baseTile = 0;
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);
    IconSetup(gSmallFont, 0);
    IconReserve(&gSmallFont);
    {
        u32 v = gSmallFont->tileCount;

        IconSetup(gLargeFont, v);
    }
    IconReserve(&gLargeFont);
    MarkObjVram(gObjVramCursor);

    a = (struct settings_icon_actor **)self->rowObjA;
    b = (struct settings_icon_actor **)self->rowObjB;
    c = (struct settings_icon_actor **)self->rowObjC;
    for (i = 0; i < 5; i++) {
        new_row_icon(&a[i], 0xc0 << 1, 1);
        new_row_icon(&b[i], 0xc6 << 1, 2);
        new_row_icon(&c[i], 0xde << 1, 0);
    }
    SET_ROW_OBJ_POS(self->rowObjA[0], 0x14, 0x28);
    SET_ROW_OBJ_POS(self->rowObjB[0], 0x14, 0x50);
    SET_ROW_OBJ_POS(self->rowObjB[1], 0x14, 0x3c);
    SET_ROW_OBJ_POS(self->rowObjB[2], 0x14, 0x4b);
    SET_ROW_OBJ_POS(self->rowObjC[0], 0x78, 0x50);
}
