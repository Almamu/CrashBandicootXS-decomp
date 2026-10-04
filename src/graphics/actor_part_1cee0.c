#include "core.h"
#include "level_menu.h"

/* GitHub issue #27: 0x0801CEE0-0x0801DA38, the whole of the former
 * asm/code_3_2_17_188d0_1cee0.s. The rest of the level-select screen
 * (`struct level_menu`, issue #26's actor_part_1b85c.c) and its two
 * background layers:
 *
 * - LevelSelectTurnPage-sub_801D730: level_menu methods - the page-turn
 *   animation (LevelSelectTurnPage, driven by the Down/Up handlers LevelSelectPrevWorld/
 *   LevelSelectNextWorld), the frame loops of the A (LevelSelectConfirm) and Start
 *   (LevelSelectExit) exits, the "previous/next page open" tests, and the
 *   per-page entry refresh (sub_801D5CC/sub_801D638/sub_801D668, also
 *   inlined into LevelSelectTurnPage).
 * - sub_801D77C-sub_801D7F8: `struct page_bg`, BG1 - the page strip whose
 *   vertical scroll eases 8 per frame toward a Q8 target (0x100 = one
 *   page).
 * - InitZoomBg: `struct icon_bg`'s constructor, BG2 - clears its screen
 *   block, writes an 8x4 tile block and spawns four corner sprites
 *   (mirrored per corner).
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/ and no Thumb
 * pointer anywhere in the ROM: sub_801D698. Matched anyway.
 *
 * Compiled with old_agbcc (see OLD_AGBCC_OBJS in the Makefile and
 * docs/matching/issue-27-level-select-pages.md): it builds the constant
 * operand of a byte read-modify-write before the load, as the ROM does,
 * so the bitfield stores here are plain C. */

extern void *gPaletteCache;
extern void *gAudioContext;
extern void ***gUnknown_030012D0;
extern void *gOamBuffer;
extern void *gUnknown_03001304;
extern u8 gNewWorldOpened;
extern u32 gKeys;     // held keys (low half), newly pressed (high half)
extern struct xy_pair gStaticData_0816C4D8[];
extern struct xy_pair gStaticData_0816C508[];
extern u32 gStaticData_0816C538[];
extern u32 gStaticData_0816C548[];
extern u8 gStaticData_0816C58C[];
extern struct xy_pair gStaticData_0816C5F0[];

extern void WaitForVBlank(void);
extern void UpdateKeys(void *p);
extern void UploadPaletteCache(void *p);
extern void CommitOamBuffer(void *p);
extern void ClaimPaletteSlot(void *cache, s32 arg);
extern void LockPalette(void *cache, s32 record);
extern void FlushVramDmaQueue(void);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void LoadGraphicsPackage(void *dst, void *pkg);
extern void *sub_8026EDC(u32 size);
extern void sub_8026ED0(void *p);
extern struct sprite *sub_8008904(void *mem);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, s32 arg);
extern void sub_80088D8(void *part, s32 value);
extern s32 sub_800815C(void *part);
extern void InitBgSetup(void *self, s32 a, s32 b, s32 c, s32 d);

extern void UpdateLevelSelect(struct level_menu *self);
extern void sub_801CCF8(struct level_menu *self);
extern void UpdateZoomBg(struct icon_bg *p);
extern void CommitZoomBg(struct icon_bg *p);
extern u8 IsZoomBgGone(struct icon_bg *p);
extern u8 IsZoomBgShown(struct icon_bg *p);
extern u8 IsZoomBgWaiting(struct icon_bg *p);
extern void StartZoomBgExit(struct icon_bg *p);
extern void ClearZoomBgPicture(struct icon_bg *p);
extern void RandomizeZoomBgTwinkle(struct icon_bg *p, struct icon_slot *slot);
extern u16 GetZoomBgControl(struct icon_bg *p);
extern void sub_801DEA0(struct item *it, s32 arg);
extern void sub_801DF0C(struct item *it, u32 arg);
extern void sub_801E190(void *panel);
extern void sub_801E3F4(void *panel);
extern u8 sub_801E464(void *panel);
extern void sub_801E480(void *panel, s32 x, s32 y);
extern u16 GetBgSetupControl(void *p);

s32 sub_801D77C(struct page_bg *p);
u8 sub_801D780(struct page_bg *p);
void sub_801D790(struct page_bg *p);
void sub_801D79C(struct page_bg *p);
void sub_801D7AC(struct page_bg *p);
u32 sub_801D7D0(struct page_bg *p);
void sub_801D05C(struct level_menu *self);
u8 LevelSelectHasPrevWorld(struct level_menu *self);
u8 LevelSelectIsNextWorldOpen(struct level_menu *self);
void sub_801D470(struct level_menu *self);

typedef void (*item_load_fn)(void *self, s32 world, s32 slot);
typedef void (*item_place_fn)(void *self, struct xy_pair *pos);

/* The level-select screen's per-frame register commit (as in
 * actor_part_1b85c.c). */
static inline void CommitDisplay(struct level_menu *self)
{
    FlushVramDmaQueue();
    CommitZoomBg(self->bg2);
    self->scroll++;
    *(vu16 *)REG_ADDR_BG0HOFS = self->scroll >> 3;
    *(vu32 *)REG_ADDR_BG1HOFS = sub_801D7D0(self->bg1);
    *(vu16 *)REG_ADDR_BG1CNT = GetBgSetupControl(self->bg1);
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

/* sub_801D638: load every entry of the current page. */
static inline void LoadItems(struct level_menu *self)
{
    s32 i;

    for (i = 0; i <= 5; i++)
    {
        struct item *it = self->items[i];
        struct method *m = &it->vtable->m10;

        ((item_load_fn)m->fn)((u8 *)it + m->thisOffset, self->world, i);
    }
}

/* sub_801D5CC: pick the cursor layout (six slots once all five levels
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

        for (j = 0; j <= 4; j++)
        {
            s32 k = self->world * 5 + j;

            n += self->save->levels[k].b.cleared;
        }
    }
    if (n == 5)
    {
        self->positions = gStaticData_0816C508;
        self->lastIndex = n;
    }
    else
    {
        self->positions = gStaticData_0816C4D8;
        self->lastIndex = 4;
    }
    for (i = 0; i <= 5; i++)
    {
        struct item *it = self->items[i];
        struct method *m = &it->vtable->m18;

        ((item_place_fn)m->fn)((u8 *)it + m->thisOffset, &self->positions[i]);
    }
}

/* sub_801D668. */
static inline void SkinItems(struct level_menu *self)
{
    s32 i;

    for (i = 0; i <= 5; i++)
        sub_801DF0C(self->items[i], gStaticData_0816C538[self->world]);
}

/* Runs the page-turn animation: steps BG1's scroll toward its target one
 * frame at a time, and halfway through (scroll 0xA0) swaps the page's
 * entries over to the new page. */
void LevelSelectTurnPage(struct level_menu *self)
{
    while (!sub_801D780(self->bg1))
    {
        BeginFrame(self);
        sub_801D7AC(self->bg1);
        sub_801E190(self->panel);
        if ((sub_801D77C(self->bg1) & 0xFF) == 0xA0)
        {
            LoadItems(self);
            PlaceItems(self);
            SkinItems(self);
        }
    }
}

/* Runs frames until the cursor panel settles. */
void sub_801D05C(struct level_menu *self)
{
    while (!sub_801E464(self->panel))
    {
        BeginFrame(self);
        sub_801E190(self->panel);
        UpdateZoomBg(self->bg2);
    }
}

/* A pressed on an open entry: sound 0x52, move the panel to the
 * middle, let the icon layer play its selection, then fade out. */
void LevelSelectConfirm(struct level_menu *self)
{
    s32 t;

    PlaySfx(gAudioContext, 0x52, 0x100);
    sub_801DEA0(self->items[self->index], 0);
    sub_801E480(self->panel, 0x78, 0x35);
    sub_801E3F4(self->panel);
    sub_801D05C(self);
    while (!IsZoomBgShown(self->bg2))
    {
        BeginFrame(self);
        sub_801E190(self->panel);
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
    while (!IsZoomBgGone(self->bg2))
    {
        BeginFrame(self);
        sub_801E190(self->panel);
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
    while (!IsZoomBgWaiting(self->bg2))
    {
        BeginFrame(self);
        sub_801E190(self->panel);
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

    switch (self->world)
    {
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

/* sub_80087D0, inlined: select animation `idx` and restart it. */
static inline void SetAnim(struct sprite *s, u32 idx)
{
    s->animIndex = idx;
    sub_80087C0(s);
    sub_80087B4(s);
    sub_800872C(s, 0);
}

/* Page changed: switch the page title sprite's animation and put the
 * cursor panel back on the (clamped) cursor. */
void sub_801D470(struct level_menu *self)
{
    SetAnim(self->sprites[0], gStaticData_0816C548[self->world]);
    if (self->index > self->lastIndex)
        self->index = self->lastIndex;
    {
        struct xy_pair *pos = &self->positions[self->index];

        sub_801E480(self->panel, pos->x, pos->y - 0x18);
    }
}

/* Down: turn back one page (repeating while Down is held); sound 0x48 on
 * the first page. The loop is written with gotos to keep the ROM's
 * block order (the test sits after the body, entered by a jump). */
void LevelSelectPrevWorld(struct level_menu *self)
{
    if (LevelSelectHasPrevWorld(self))
    {
        sub_801CCF8(self);
        PlaySfx(gAudioContext, 0x56, 0x100);
        goto check;
    loop:
        self->world--;
        sub_801D790(self->bg1);
        LevelSelectTurnPage(self);
        UpdateKeys(gUnknown_03001304);
        if (!(gKeys & DPAD_DOWN))
            goto done;
    check:
        if (LevelSelectHasPrevWorld(self))
            goto loop;
    done:
        sub_801D470(self);
    }
    else
    {
        PlaySfx(gAudioContext, 0x48, 0x100);
    }
}

/* Up: turn forward one page (repeating while Up is held) while the next
 * page is open; sound 0x48 otherwise. */
void LevelSelectNextWorld(struct level_menu *self)
{
    if (LevelSelectIsNextWorldOpen(self))
    {
        sub_801CCF8(self);
        PlaySfx(gAudioContext, 0x55, 0x100);
        goto check;
    loop:
        self->world++;
        sub_801D79C(self->bg1);
        LevelSelectTurnPage(self);
        UpdateKeys(gUnknown_03001304);
        if (!(gKeys & DPAD_UP))
            goto done;
    check:
        if (LevelSelectIsNextWorldOpen(self))
            goto loop;
    done:
        sub_801D470(self);
    }
    else
    {
        PlaySfx(gAudioContext, 0x48, 0x100);
    }
}

void sub_801D5CC(struct level_menu *self)
{
    PlaceItems(self);
}

void sub_801D638(struct level_menu *self)
{
    LoadItems(self);
}

void sub_801D668(struct level_menu *self)
{
    SkinItems(self);
}

/* UNUSED - no caller anywhere in the ROM. One frame of the screen
 * without the menu's own update. */
void sub_801D698(struct level_menu *self)
{
    WaitForVBlank();
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    CommitDisplay(self);
}

/* Reloads the palette and re-applies it to the eight sprites and the
 * page entries. */
void sub_801D730(struct level_menu *self)
{
    s32 i;

    ClaimPaletteSlot(gPaletteCache, 0xF);
    for (i = 0; i <= 7; i++)
        self->sprites[i]->palette = sub_800815C(self->sprites[i]);
    sub_801D668(self);
}

s32 sub_801D77C(struct page_bg *p)
{
    return p->scroll;
}

u8 sub_801D780(struct page_bg *p)
{
    u8 r = 0;

    if (p->scroll == p->target)
        r = 1;
    return r;
}

void sub_801D790(struct page_bg *p)
{
    p->target += 0x100;
}

void sub_801D79C(struct page_bg *p)
{
    p->target -= 0x100;
}

/* Eases `scroll` 8 per frame toward `target` and scrolls BG1 with it. */
void sub_801D7AC(struct page_bg *p)
{
    if (p->scroll < p->target)
        p->scroll += 8;
    if (p->scroll > p->target)
        p->scroll -= 8;
    p->vofs = p->scroll;
}

/* BG1HOFS and BG1VOFS as one word. */
u32 sub_801D7D0(struct page_bg *p)
{
    return *(u32 *)&p->hofs;
}

void sub_801D7D4(struct page_bg *p)
{
    p->hofs = 8;
    p->vofs = p->scroll + 0x30;
}

void sub_801D7E0(struct page_bg *p, s32 flags)
{
    if (flags & 1)
        sub_8026ED0(p);
}

struct page_bg *sub_801D7F8(struct page_bg *self, s32 charBlock, s32 screenBlock)
{
    InitBgSetup(self, charBlock, screenBlock, 0, 2);
    self->scroll = self->target = 0x300;
    LoadGraphicsPackage(self, gStaticData_0816C58C);
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
 * by gStaticData_0816C5F0. */
struct icon_bg *InitZoomBg(struct icon_bg *self, s32 charBlock, s32 screenBlock)
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
        for (row = 0; row <= 7; row++)
        {
            s32 j;

            for (j = 0; j <= 3; j++)
            {
                dst[j] = v;
                v += 0x202;
            }
            dst += 8;
        }
    }
    self->unk_10 = 0xB;
    self->unk_0C = 2;
    self->unk_14 = 8;
    self->unk_28 = self->unk_2C = self->unk_30 = 0;
    self->unk_3C = self->unk_38 = 0x2000;
    self->x16 = self->x;
    self->y16 = self->y;
    self->unk_48 = 0;
    for (i = 0; i <= 3; i++)
    {
        self->slots[i].sprite = sub_8008904(sub_8026EDC(0x40));
        self->slots[i].sprite->anim = (struct anim_table *)((u8 *)**gUnknown_030012D0 + 0x258);
        SetMode(self->slots[i].sprite, 1);
        SetPos(self->slots[i].sprite, self->x + gStaticData_0816C5F0[i].x, self->y + gStaticData_0816C5F0[i].y);
        sub_80088D8(self->slots[i].sprite, 1);
        SetPalette(self->slots[i].sprite, sub_800815C(self->slots[0].sprite));
        RandomizeZoomBgTwinkle(self, &self->slots[i]);
    }
    LockPalette(gPaletteCache, self->slots[0].sprite->anim->records[self->slots[0].sprite->animIndex].tileRecord);
    SetFlipX(self->slots[1].sprite, 1);
    SetFlipY(self->slots[2].sprite, 1);
    SetFlipX(self->slots[3].sprite, 1);
    SetFlipY(self->slots[3].sprite, 1);
    return self;
}
