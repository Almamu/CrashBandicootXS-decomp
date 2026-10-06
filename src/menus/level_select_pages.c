#include "core.h"
#include "level_menu.h"
#include "system.h"
#include "audio.h"
#include "menus.h"
#include "gfx.h"
#include "objects.h"
#include "globals.h"

/* GitHub issue #27: 0x0801CEE0-0x0801DA38, the whole of the former
 * asm/code_3_2_17_188d0_1cee0.s. The rest of the level-select screen
 * (`struct level_menu`, issue #26's level_select.c) and its two
 * background layers:
 *
 * - LevelSelectTurnPage-ReloadLevelSelectPalette: level_menu methods - the page-turn
 *   animation (LevelSelectTurnPage, driven by the Down/Up handlers LevelSelectPrevWorld/
 *   LevelSelectNextWorld), the frame loops of the A (LevelSelectConfirm) and Start
 *   (LevelSelectExit) exits, the "previous/next page open" tests, and the
 *   per-page entry refresh (PlaceLevelSelectEntries/LoadLevelSelectEntries/SetLevelSelectEntryBoxes, also
 *   inlined into LevelSelectTurnPage).
 * - GetLevelSelectPageBgScroll-CreateLevelSelectPageBg: `struct page_bg`, BG1 - the page strip whose
 *   vertical scroll eases 8 per frame toward a Q8 target (0x100 = one
 *   page).
 * - InitZoomBg: `struct zoom_bg`'s constructor, BG2 - clears its screen
 *   block, writes an 8x4 tile block and spawns four corner sprites
 *   (mirrored per corner).
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/ and no Thumb
 * pointer anywhere in the ROM: CommitLevelSelectFrame. Matched anyway.
 *
 * Compiled with old_agbcc (see OLD_AGBCC_OBJS in the Makefile and
 * docs/matching/archive/issue-27-level-select-pages.md): it builds the constant
 * operand of a byte read-modify-write before the load, as the ROM does,
 * so the bitfield stores here are plain C. */

typedef void (*item_load_fn)(void *self, s32 world, s32 slot);
typedef void (*item_place_fn)(void *self, const struct xy_pair *pos);

/* The level-select screen's per-frame register commit (as in
 * level_select.c). */
static inline void CommitDisplay(struct level_menu *self)
{
    FlushVramDmaQueue();
    CommitZoomBg(self->bg2);
    self->scroll++;
    *(vu16 *)REG_ADDR_BG0HOFS = self->scroll >> 3;
    *(vu32 *)REG_ADDR_BG1HOFS = GetLevelSelectPageBgOffsets(self->bg1);
    *(vu16 *)REG_ADDR_BG1CNT = GetBgSetupControl(&self->bg1->bg);
    *(vu16 *)REG_ADDR_BG2CNT = GetZoomBgControl(self->bg2);
    *(vu16 *)PLTT = 0;
    *(vu32 *)REG_ADDR_BLDCNT = self->blend.raw;
    *(vu16 *)REG_ADDR_BLDY = self->bldy.evy;
    *(vu16 *)REG_ADDR_DISPCNT = self->dispcnt.raw;
}

static inline void BeginFrame(struct level_menu *self)
{
    UpdateLevelSelect(self);
    WaitForVBlank();
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    CommitDisplay(self);
}

/* LoadLevelSelectEntries: load every entry of the current page. */
static inline void LoadItems(struct level_menu *self)
{
    s32 i;

    for (i = 0; i <= 5; i++) {
        struct level_item *it = self->items[i];
        struct actor_method *m = &it->vtable->m10;

        ((item_load_fn)m->fn)((u8 *)it + m->thisOffset, self->world, i);
    }
}

/* PlaceLevelSelectEntries: pick the cursor layout (six slots once all five levels
 * of the page are cleared) and place every entry. */
static inline void PlaceItems(struct level_menu *self)
{
    s32 n = 0;
    s32 i;

    /* A counter separate from `i`, and the index taken into a local
     * before the lookup: both are what the ROM's register use and
     * address-computation order need (docs/workflow.md step 7). */
    {
        s32 j;

        for (j = 0; j <= 4; j++) {
            s32 k = self->world * 5 + j;

            n += self->save->levels[k].b.cleared;
        }
    }
    if (n == 5) {
        self->positions = gLevelSelectEntryPositionsAllCleared;
        self->lastIndex = n;
    } else {
        self->positions = gLevelSelectEntryPositions;
        self->lastIndex = 4;
    }
    for (i = 0; i <= 5; i++) {
        struct level_item *it = self->items[i];
        struct actor_method *m = &it->vtable->m18;

        ((item_place_fn)m->fn)((u8 *)it + m->thisOffset, &self->positions[i]);
    }
}

/* SetLevelSelectEntryBoxes. */
static inline void SkinItems(struct level_menu *self)
{
    s32 i;

    for (i = 0; i <= 5; i++)
        SetLevelSelectEntryBox((struct level_item *)self->items[i],
                               gLevelSelectWorldEntryBoxAnims[self->world]);
}

/* Runs the page-turn animation: steps BG1's scroll toward its target one
 * frame at a time, and halfway through (scroll 0xA0) swaps the page's
 * entries over to the new page. */
void LevelSelectTurnPage(struct level_menu *self)
{
    while (!IsLevelSelectPageBgSettled(self->bg1)) {
        BeginFrame(self);
        ScrollLevelSelectPageBg(self->bg1);
        UpdateLevelSelectCursor(self->panel);
        if ((GetLevelSelectPageBgScroll(self->bg1) & 0xFF) == 0xA0) {
            LoadItems(self);
            PlaceItems(self);
            SkinItems(self);
        }
    }
}

/* Runs frames until the cursor panel settles. */
void WaitLevelSelectCursor(struct level_menu *self)
{
    while (!HasLevelSelectCursorArrived(self->panel)) {
        BeginFrame(self);
        UpdateLevelSelectCursor(self->panel);
        UpdateZoomBg(self->bg2);
    }
}

/* A pressed on an open entry: sound 0x52, move the panel to the
 * middle, let the icon layer play its selection, then fade out. */
void LevelSelectConfirm(struct level_menu *self)
{
    s32 t;

    PlaySfx(gAudioContext, 0x52, 0x100);
    SetLevelSelectEntrySelected((struct level_item *)self->items[self->index], 0);
    MoveLevelSelectCursor(self->panel, 0x78, 0x35);
    HideLevelSelectCursor(self->panel);
    WaitLevelSelectCursor(self);
    while (!IsZoomBgShown(self->bg2)) {
        BeginFrame(self);
        UpdateLevelSelectCursor(self->panel);
        UpdateZoomBg(self->bg2);
    }
    self->blend.bits.effect = 3;
    self->blend.bits.bdFirst = 1;
    self->blend.bits.bg0First = 1;
    self->blend.bits.bg1First = 1;
    self->blend.bits.bg2First = 1;
    self->blend.bits.bg3First = 1;
    self->blend.bits.objFirst = 1;
    self->bldy.evy = 0;
    t = 0;
    StartZoomBgExit(self->bg2);
    while (!IsZoomBgGone(self->bg2)) {
        BeginFrame(self);
        UpdateLevelSelectCursor(self->panel);
        UpdateZoomBg(self->bg2);
        t++;
        self->bldy.evy = t / 2;
    }
}

/* Start: sound 0x49, fade to black, and leave with `result` 1. */
void LevelSelectExit(struct level_menu *self)
{
    s32 t;

    PlaySfx(gAudioContext, 0x49, 0x100);
    self->blend.bits.effect = 3;
    self->blend.bits.bdFirst = 1;
    self->blend.bits.bg0First = 1;
    self->blend.bits.bg1First = 1;
    self->blend.bits.bg2First = 1;
    self->blend.bits.bg3First = 1;
    self->blend.bits.objFirst = 1;
    self->bldy.evy = 0;
    t = 0;
    ClearZoomBgPicture(self->bg2);
    while (!IsZoomBgWaiting(self->bg2)) {
        BeginFrame(self);
        UpdateLevelSelectCursor(self->panel);
        UpdateZoomBg(self->bg2);
        t++;
        self->bldy.evy = t / 2;
    }
    self->result = 1;
}

void SetNewWorldOpened(void)
{
    gNewWorldOpened = 1;
}

/* Whether there is a previous page (`world != 0`, spelled as the ROM's
 * branchless neg/orr/lsr; a plain comparison compiles to a branch). */
u8 LevelSelectHasPrevWorld(struct level_menu *self)
{
    u32 w = self->world;

    return (-w | w) >> 31;
}

/* Whether the next page has been opened (save byte 2, bits 5/7/6). */
u8 LevelSelectIsNextWorldOpen(struct level_menu *self)
{
    u8 r = 0;

    switch (self->world) {
    case 0:
        r = (self->save->open >> 5) & 1;
        break;
    case 1:
        r = (self->save->open >> 7) & 1;
        break;
    case 2:
        r = (self->save->open >> 6) & 1;
        break;
    }
    return r;
}

/* SetSpriteAnim, inlined: select animation `idx` and restart it. */
static inline void SetAnim(struct sprite *s, u32 idx)
{
    s->animIndex = idx;
    ResetSpriteFrameTimer(s);
    ResetSpriteFrameIndex(s);
    SetSpriteAnimDone(s, 0);
}

/* Page changed: switch the page title sprite's animation and put the
 * cursor panel back on the (clamped) cursor. */
void RefreshLevelSelectPage(struct level_menu *self)
{
    SetAnim(self->sprites[0], gLevelSelectWorldAnims[self->world]);
    if (self->index > self->lastIndex)
        self->index = self->lastIndex;
    {
        const struct xy_pair *pos = &self->positions[self->index];

        MoveLevelSelectCursor(self->panel, pos->x, pos->y - 0x18);
    }
}

/* Down: turn back one page (repeating while Down is held); sound 0x48 on
 * the first page. The loop is written with gotos to keep the ROM's
 * block order (the test sits after the body, entered by a jump). */
void LevelSelectPrevWorld(struct level_menu *self)
{
    if (LevelSelectHasPrevWorld(self)) {
        SettleLevelSelectPage(self);
        PlaySfx(gAudioContext, 0x56, 0x100);
        goto check;
    loop:
        self->world--;
        TurnLevelSelectPageBgBack(self->bg1);
        LevelSelectTurnPage(self);
        UpdateKeys(gInput);
        if (!(gKeys.all & DPAD_DOWN))
            goto done;
    check:
        if (LevelSelectHasPrevWorld(self))
            goto loop;
    done:
        RefreshLevelSelectPage(self);
    } else {
        PlaySfx(gAudioContext, 0x48, 0x100);
    }
}

/* Up: turn forward one page (repeating while Up is held) while the next
 * page is open; sound 0x48 otherwise. */
void LevelSelectNextWorld(struct level_menu *self)
{
    if (LevelSelectIsNextWorldOpen(self)) {
        SettleLevelSelectPage(self);
        PlaySfx(gAudioContext, 0x55, 0x100);
        goto check;
    loop:
        self->world++;
        TurnLevelSelectPageBgForward(self->bg1);
        LevelSelectTurnPage(self);
        UpdateKeys(gInput);
        if (!(gKeys.all & DPAD_UP))
            goto done;
    check:
        if (LevelSelectIsNextWorldOpen(self))
            goto loop;
    done:
        RefreshLevelSelectPage(self);
    } else {
        PlaySfx(gAudioContext, 0x48, 0x100);
    }
}

void PlaceLevelSelectEntries(struct level_menu *self)
{
    PlaceItems(self);
}

void LoadLevelSelectEntries(struct level_menu *self)
{
    LoadItems(self);
}

void SetLevelSelectEntryBoxes(struct level_menu *self)
{
    SkinItems(self);
}

/* UNUSED - no caller anywhere in the ROM. One frame of the screen
 * without the menu's own update. */
void CommitLevelSelectFrame(struct level_menu *self)
{
    WaitForVBlank();
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    CommitDisplay(self);
}

/* Reloads the palette and re-applies it to the eight sprites and the
 * page entries. */
void ReloadLevelSelectPalette(struct level_menu *self)
{
    s32 i;

    ClaimPaletteSlot(gPaletteCache, 0xF);
    for (i = 0; i <= 7; i++)
        self->sprites[i]->palette = GetSpriteAnimPaletteSlot((struct actor *)self->sprites[i]);
    SetLevelSelectEntryBoxes(self);
}

s32 GetLevelSelectPageBgScroll(struct page_bg *p)
{
    return p->scroll;
}

u8 IsLevelSelectPageBgSettled(struct page_bg *p)
{
    u8 r = 0;

    if (p->scroll == p->target)
        r = 1;
    return r;
}

void TurnLevelSelectPageBgBack(struct page_bg *p)
{
    p->target += 0x100;
}

void TurnLevelSelectPageBgForward(struct page_bg *p)
{
    p->target -= 0x100;
}

/* Eases `scroll` 8 per frame toward `target` and scrolls BG1 with it. */
void ScrollLevelSelectPageBg(struct page_bg *p)
{
    if (p->scroll < p->target)
        p->scroll += 8;
    if (p->scroll > p->target)
        p->scroll -= 8;
    p->vofs = p->scroll;
}

/* BG1HOFS and BG1VOFS as one word. */
u32 GetLevelSelectPageBgOffsets(struct page_bg *p)
{
    return *(u32 *)&p->hofs;
}

void SetLevelSelectPageBgOffsets(struct page_bg *p)
{
    p->hofs = 8;
    p->vofs = p->scroll + 0x30;
}

void DestroyLevelSelectPageBg(struct page_bg *p, s32 flags)
{
    if (flags & 1)
        OperatorDelete(p);
}

struct page_bg *CreateLevelSelectPageBg(struct page_bg *self, s32 charBlock, s32 screenBlock)
{
    InitBgSetup(&self->bg, charBlock, screenBlock, 0, 2);
    self->scroll = self->target = 0x300;
    LoadGraphicsPackage(&self->bg, (void *)&gLevelSelectPageBg);
    return self;
}

/* Inline setters, as the C++ original's member functions: storing a
 * parameter (rather than a literal) into the bitfield keeps the ROM's
 * full clear-then-or sequence, and lets the loop hoist the `1` for
 * SetMode into a register. */
static inline void SetMode(struct sprite *s, s32 mode)
{
    s->f28.mode = mode;
}

static inline void SetFlipX(struct sprite *s, u8 on)
{
    s->f28.flipX = on;
}

static inline void SetFlipY(struct sprite *s, u8 on)
{
    s->f28.flipY = on;
}

static inline void SetPalette(struct sprite *s, s32 pal)
{
    s->palette = pal;
}

static inline void SetPos(struct sprite *s, s32 x, s32 y)
{
    s->x = x << 8;
    s->y = y << 8;
}

/* BG2 constructor: the BGCNT fields (priority 1, 256 colours), a
 * cleared screen block with an 8x4 block of tile entries at its top
 * left, and four corner sprites (animation bank `+0x258` of the level
 * graphics, mirrored per corner) positioned around (x, y) = (0x78, 0x35)
 * by gZoomBgSlotOffsets. */
struct zoom_bg *InitZoomBg(struct zoom_bg *self, s32 charBlock, s32 screenBlock)
{
    s32 i;

    self->x = 0x78;
    self->y = 0x35;
    self->bgcnt.raw = 0;
    self->bgcnt.bits.priority = 1;
    self->charBlock = charBlock;
    self->bgcnt.bits.charBase = charBlock;
    self->bgcnt.bits.screenSize = 0;
    self->screenBlock = screenBlock;
    self->bgcnt.bits.screenBase = screenBlock;
    self->bgcnt.bits.color256 = 1;
    {
        u16 *dst = (u16 *)(VRAM + (self->screenBlock << 11));
        u32 v;
        s32 row;

        DmaFill16(3, 0, dst, 0x100);
        v = 0x100;
        for (row = 0; row <= 7; row++) {
            s32 j;

            for (j = 0; j <= 3; j++) {
                dst[j] = v;
                v += 0x202;
            }
            dst += 8;
        }
    }
    self->image = 0xB;
    self->state = 2;
    self->scale = 8;
    self->dx = self->dy = self->phase = 0;
    self->texY = self->texX = 0x2000;
    self->x16 = self->x;
    self->y16 = self->y;
    self->alpha = 0;
    for (i = 0; i <= 3; i++) {
        self->twinkles[i].part = (struct sprite *)InitUiSpriteObj(OperatorNew(0x40));
        self->twinkles[i].part->anim = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x258);
        SetMode(self->twinkles[i].part, 1);
        SetPos(self->twinkles[i].part, self->x + gZoomBgSlotOffsets[i].x,
               self->y + gZoomBgSlotOffsets[i].y);
        SetSpritePriority(self->twinkles[i].part, 1);
        SetPalette(self->twinkles[i].part,
                   GetSpriteAnimPaletteSlot((struct actor *)self->twinkles[0].part));
        RandomizeZoomBgTwinkle(self, &self->twinkles[i]);
    }
    LockPalette(gPaletteCache,
                self->twinkles[0].part->anim->anims[self->twinkles[0].part->animIndex].paletteId);
    SetFlipX(self->twinkles[1].part, 1);
    SetFlipY(self->twinkles[2].part, 1);
    SetFlipX(self->twinkles[3].part, 1);
    SetFlipY(self->twinkles[3].part, 1);
    return self;
}
