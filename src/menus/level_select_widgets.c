#include "core.h"
#include "level_select_parts.h"
#include <agb_syscall.h>
#include "util.h"
#include "audio.h"
#include "menus.h"
#include "gfx.h"
#include "globals.h"

/* GitHub issue #28: 0x0801DA38-0x0801DFEC, the whole of the former
 * asm/code_3_2_17_188d0_1da38.s. Two of the level-select screen's
 * (level_select.c) sub-objects:
 *
 * - DestroyZoomBg-TickZoomBgTwinkle: `struct zoom_bg`, the level picture on the
 *   affine BG2 layer (`level_menu.bg2`, constructor InitZoomBg in the
 *   issue #27 range). It zooms out to swap in the selected level's
 *   picture (states 1 -> 2 -> 0), wobbles on the sine table while shown
 *   (state 3) and zooms away on exit (states 4 -> 5). Four sprite parts
 *   ("twinkles") sit on top of it, each showing a random frame for a
 *   random time and blinking along with the wobble.
 * - GetZoomBgControl-DestroyLevelSelectEntry: `struct level_item`'s methods (method table
 *   gLevelSelectEntryVtable: +0x08 AnimateLevelSelectEntry bob, +0x10 SetLevelSelectEntryLevel
 *   set level, +0x18 SetLevelSelectEntryPos set position, +0x20 nullsub_20, +0x28
 *   DestroyLevelSelectEntry destructor; the constructor CreateLevelSelectEntry starts issue #29).
 *
 * This file is compiled with tools/agbcc/bin/old_agbcc (see Makefile and
 * docs/matching/archive/issue-24-boss-actor.md). Every function is real C; see
 * docs/matching/archive/issue-28-29-level-select-parts.md. */

/* `struct twinkle`, `struct zoom_bg` (with its BG2CNT views) and `struct
 * level_item` are level_menu.h's. */

static inline void SetPosQ8(struct sprite *p, s32 x, s32 y)
{
    p->x = x << 8;
    p->y = y << 8;
}

/* Destructor (`level_menu.bg2`, called from DestroyLevelSelect). */
void DestroyZoomBg(struct zoom_bg *self, s32 flags)
{
    UnlockPalette(gPaletteCache, PART_RECORD(self->twinkles[0].part).paletteId);
    DELETE_PART(self->twinkles[3].part);
    DELETE_PART(self->twinkles[2].part);
    DELETE_PART(self->twinkles[1].part);
    DELETE_PART(self->twinkles[0].part);
    if (flags & 1)
        OperatorDelete(self);
}

/* Per-frame state machine:
 * 0 zoom in to 1:1, then 3; 1 zoom out, then 2; 2 load the requested
 * picture (if any) and go to 0; 3 shown, wobbling; 4 zoom out while
 * rotating, then 5 (gone). The twinkles advance in every state. */
void UpdateZoomBg(struct zoom_bg *self)
{
    u8 buf[0x200];

    switch (self->state) {
    case 0:
        if (self->scale <= 0xFF) {
            self->scale += 8;
            break;
        }
        self->scale = 0x100;
        self->state = 3;
        break;
    case 1:
        if (self->scale > 8) {
            self->scale -= 8;
            break;
        }
        self->scale = 8;
        self->state = 2;
        break;
    case 3:
        self->phase = (self->phase + 1) & 0xFF;
        /* `* 4 >> 8`, not `>> 6`: gcc's combiner turns the latter into
         * ldrh+lsl+asr instead of the ROM's ldrsh+asr. */
        self->dx = (gSineTable[self->phase & 0xFF] * 4) >> 8;
        self->dy = (gSineTable[(self->phase << 1) & 0xFF] * 4) >> 8;
        break;
    case 2:
        if (self->image == 11)
            break;
        PlaySfx(gAudioContext, 0x53, 0x100);
        LoadTaggedAsset(gLevelSelectPictures[self->image].palette, buf);
        DmaCopy16(3, buf, BG_PLTT, 0x40);
        LoadTaggedAsset(gLevelSelectPictures[self->image].tiles,
                        (void *)(BG_VRAM + (self->charBlock << 14)));
        self->state = 0;
        break;
    case 4:
        if (self->scale > 8) {
            self->scale -= 8;
            self->alpha += 0x100;
            break;
        }
        self->scale = 8;
        self->state = 5;
        break;
    }
    TickZoomBgTwinkle(self, &self->twinkles[0]);
    TickZoomBgTwinkle(self, &self->twinkles[1]);
    TickZoomBgTwinkle(self, &self->twinkles[2]);
    TickZoomBgTwinkle(self, &self->twinkles[3]);
}

/* Draw step: moves the twinkles with the wobble, then rebuilds the BG2
 * affine matrix from the current zoom (rotation only in states 4-5). */
void DrawZoomBg(struct zoom_bg *self)
{
    if (IsZoomBgShown(self)) {
        MoveZoomBgTwinkle(self, &self->twinkles[0]);
        MoveZoomBgTwinkle(self, &self->twinkles[1]);
        MoveZoomBgTwinkle(self, &self->twinkles[2]);
        MoveZoomBgTwinkle(self, &self->twinkles[3]);
    }
    switch (self->state) {
    case 0 ... 3:
        {
            u16 s;

            self->x16 = self->x + self->dx;
            self->y16 = self->y + self->dy;
            s = 0x10000 / self->scale;
            self->sx = s;
            self->sy = s;
            break;
        }
    case 4 ... 5:
        self->sx = self->scale;
        self->sy = self->scale;
        break;
    }
    BgAffineSet(&self->texX, &self->pa, 1);
}

/* Commits the affine matrix to BG2PA-BG2Y. */
void CommitZoomBg(struct zoom_bg *self)
{
    REG_BG2PA = self->pa;
    REG_BG2PB = self->pb;
    REG_BG2PC = self->pc;
    REG_BG2PD = self->pd;
    REG_BG2X = self->bgx;
    REG_BG2Y = self->bgy;
}

/* Zooming away (state 4). */
u8 IsZoomBgExiting(struct zoom_bg *self)
{
    return self->state == 4;
}

/* Gone (state 5). */
u8 IsZoomBgGone(struct zoom_bg *self)
{
    return self->state == 5;
}

/* Shown (state 3). */
u8 IsZoomBgShown(struct zoom_bg *self)
{
    return self->state == 3;
}

/* Waiting for a picture (state 2). */
u8 IsZoomBgWaiting(struct zoom_bg *self)
{
    return self->state == 2;
}

/* Zooming out (state 1). */
u8 IsZoomBgZoomingOut(struct zoom_bg *self)
{
    return self->state == 1;
}

/* Starts the exit zoom, with BG2 at the front. */
void StartZoomBgExit(struct zoom_bg *self)
{
    self->bgcnt.bits16.priority = 0;
    self->state = 4;
}

/* Zooms the picture out with no follow-up picture (page turn). */
void ClearZoomBgPicture(struct zoom_bg *self)
{
    PlaySfx(gAudioContext, 0x54, 0x100);
    self->state = 1;
    self->image = 11;
}

/* Requests picture `image`, taken once the zoom-out finishes. */
void SetZoomBgPicture(struct zoom_bg *self, s32 image)
{
    switch (self->state) {
    case 1 ... 2:
        self->image = image;
        break;
    }
}

/* Moves a blinking twinkle by the wobble offset. */
void MoveZoomBgTwinkle(struct zoom_bg *self, struct twinkle *t)
{
    if (t->blink > 0 && (t->timer & 4))
        DrawSpriteWithOffset((struct actor *)t->part, self->dx, self->dy);
}

/* Picks a new random frame, duration and blink window for a twinkle. */
void RandomizeZoomBgTwinkle(struct zoom_bg *self, struct twinkle *t)
{
    SetFrame(t->part, (u16)RandRange(8));
    t->timer = (u16)RandRange(0x3C);
    if (t->timer != 0)
        t->blink = (u16)RandRange(t->timer / 2 + 1);
}

void TickZoomBgTwinkle(struct zoom_bg *self, struct twinkle *t)
{
    if (t->timer != 0) {
        t->timer--;
        t->blink--;
    } else {
        RandomizeZoomBgTwinkle(self, t);
    }
}

u16 GetZoomBgControl(struct zoom_bg *self)
{
    return self->bgcnt.raw;
}

u8 IsLevelSelectEntrySelected(struct level_item *self)
{
    return self->selected;
}

s32 GetLevelSelectEntryLevel(struct level_item *self)
{
    return self->id;
}

/* Method +0x08: bobs the entry by `phase` (a byte; 0xA0-0xFF wrap to a
 * small upward offset), the icon 2 px lower while selected, and shows the
 * box's selected/unselected frame. */
void AnimateLevelSelectEntry(struct level_item *self, s32 phase)
{
    s32 dy;

    phase &= 0xFF;
    if (phase <= 0x9F)
        dy = -phase;
    else
        dy = 0x100 - phase;
    if (self->selected)
        DrawSpriteWithOffset((struct actor *)self->icon, 0, dy + 2);
    else
        DrawSpriteWithOffset((struct actor *)self->icon, 0, dy);
    if (self->selected)
        SetFrame(self->frame, 1);
    else
        SetFrame(self->frame, 0);
    DrawSpriteWithOffset((struct actor *)self->frame, 0, dy);
}

void SetLevelSelectEntrySelected(struct level_item *self, u8 selected)
{
    self->selected = selected;
}

/* Method +0x10: entry `index` of world `world`. Indices 0-4 are levels
 * (icon frame = level id); anything past that is the world's extra entry
 * (level id 0x14 + world, its own icon animation). */
void SetLevelSelectEntryLevel(struct level_item *self, s32 world, s32 index)
{
    if (index <= 4) {
        self->id = world * 5 + index;
        SetFrame(self->icon, self->id);
    } else {
        self->id = world + 0x14;
        SetAnim(self->icon, gLevelSelectEntryWorldAnims[world]);
    }
}

/* Sets the box's animation (gLevelSelectEntryBoxAnims[kind]) and refreshes
 * both parts' palettes. */
void SetLevelSelectEntryBox(struct level_item *self, s32 kind)
{
    SetAnim(self->frame, gLevelSelectEntryBoxAnims[kind]);
    self->frame->palette = GetSpriteAnimPaletteSlot((struct actor *)self->frame);
    self->icon->palette = GetSpriteAnimPaletteSlot((struct actor *)self->icon);
}

/* Method +0x18: places the entry at pixel `pos` (x, y). */
void SetLevelSelectEntryPos(struct level_item *self, s32 *pos)
{
    SetPosQ8(self->icon, pos[0], pos[1] - 3);
    SetEntityPixelPos((struct actor *)self->frame, pos[0], pos[1]);
}

/* Method +0x20. */
void nullsub_20(void)
{
}

/* Method +0x28: destructor. */
void DestroyLevelSelectEntry(struct level_item *self, s32 flags)
{
    self->vtable = (struct item_vtable *)gLevelSelectEntryVtable;
    DELETE_PART(self->icon);
    DELETE_PART(self->frame);
    if (flags & 1)
        OperatorDelete(self);
}

/* GitHub issue #29: 0x0801DFEC-0x0801E578, the whole of the former
 * asm/code_3_2_17_188d0_1dfec.s.
 *
 * - CreateLevelSelectEntry: `struct level_item`'s constructor (its methods end
 *   issue #28, level_select_widgets.c).
 * - CreateLevelSelectCursor-DestroyLevelSelectCursor: `struct cursor_panel`, the level-select
 *   screen's cursor (`level_menu.panel`, 0x54 bytes). It glides between
 *   entries along a Bresenham line (two steps per frame), plays an idle
 *   animation cycle at random intervals, and grows in (state 4) / shrinks
 *   away (state 5) as an affine OBJ drawn straight into the OAM shadow
 *   buffer.
 *
 * This file is compiled with tools/agbcc/bin/old_agbcc (see Makefile and
 * docs/matching/archive/issue-24-boss-actor.md). Every function is real C; see
 * docs/matching/archive/issue-28-29-level-select-parts.md.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/, and no
 * Thumb pointer anywhere in the ROM: IsLevelSelectCursorHidden, IsLevelSelectCursorGrowing,
 * MoveLevelSelectCursorTo (ParkLevelSelectCursor has it inlined). Matched anyway. */

/* One hardware OAM entry. The matrix number is split in three because
 * the ROM writes it three bits + one + one (DrawLevelSelectCursor). */
/* The level-select cursor (CreateLevelSelectCursor, 0x54 bytes). */
struct cursor_panel {
    struct bresenham_line line; // 0x00 - (x0, y0) is the position
    struct sprite *part;        // 0x28
    s32 timer;                  // 0x2C - frames to the next idle cycle
    s32 state;                  // 0x30 - see UpdateLevelSelectCursor
    struct oam_attrs oam;       // 0x34
    s32 speed;                  // 0x3C - zoom step (MoveLevelSelectCursor)
    s32 scale;                  // 0x40 - 0x100 = 1:1
    /* ObjAffineSet source */
    s16 sx;    // 0x44
    s16 sy;    // 0x46
    u16 angle; // 0x48
    u8 unk_4A[2];
    s16 matrix[4]; // 0x4C - pa, pb, pc, pd
};

COMPILE_TIME_ASSERT(level_select_widgets_c, sizeof(struct cursor_panel) == 0x54);

static inline void ResetIdleTimer(struct cursor_panel *self)
{
    self->timer = (u16)RandRange(300) + 600;
}

static inline void MoveToPos(struct cursor_panel *self, struct xy_pair *pos)
{
    MoveLevelSelectCursor(self, pos->x, pos->y);
}

/* One affine parameter; OBJ matrix `m` is entries 4m..4m+3. An inline
 * so that the value is loaded before the address is computed. */
static inline void SetAffineParam(struct oam_shadow_buffer *buf, s32 n, u16 v)
{
    buf->table[n].attr[3] = v;
}

struct level_item *CreateLevelSelectEntry(struct level_item *self)
{
    self->vtable = (struct item_vtable *)gLevelSelectEntryVtable;
    self->selected = 0;
    self->frame = (struct sprite *)InitUiSpriteObj(OperatorNew(0x40));
    self->frame->anim = AnimTable(0x24C);
    SetSpritePriority(self->frame, 1);
    self->icon = (struct sprite *)InitUiSpriteObj(OperatorNew(0x40));
    self->icon->anim = AnimTable(0x264);
    SetSpritePriority(self->icon, 1);
    return self;
}

struct cursor_panel *CreateLevelSelectCursor(struct cursor_panel *self)
{
    struct sprite *p = (struct sprite *)InitUiSpriteObj(OperatorNew(0x40));

    self->part = p;
    p->anim = AnimTable(0x240);
    SetAnim(p, 0);
    self->part->palette = GetSpriteAnimPaletteSlot((struct actor *)self->part);
    LockPalette(gPaletteCache, PART_RECORD(self->part).paletteId);
    SetLevelSelectCursorPos(self, 0x78, 0x35);
    self->oam.y = self->line.y0 - 0x20;
    self->oam.affineMode = 1;
    self->oam.objMode = 0;
    self->oam.mosaic = 0;
    self->oam.bpp = 0;
    self->oam.shape = 0;
    self->oam.x = self->line.x0 - 0x20;
    self->oam.matrixLo = 0;
    self->oam.matrixBit3 = 0;
    self->oam.matrixBit4 = 0;
    self->oam.size = 3;
    self->oam.tileNum = 0x3C0;
    self->oam.priority = 0;
    self->oam.palette = self->part->palette;
    LoadTaggedAsset(gLevelSelectCursorZoomTiles, OBJ_VRAM0 + 0x7800);
    self->angle = 0;
    self->scale = 8;
    self->state = 4;
    return self;
}

/* Per-frame update. Steps the glide; states 0-3 are the idle animation
 * cycle (0 waits for `timer`, then plays gLevelSelectCursorAnims[1..3] and
 * back to [0]), 4 grows the cursor to 1:1, 5 shrinks it away. */
void UpdateLevelSelectCursor(struct cursor_panel *self)
{
    GlideLevelSelectCursor(self);
    switch (self->state) {
    case 0:
        AdvanceSpriteAnim((struct box_part *)self->part);
        if (self->timer != 0) {
            self->timer--;
            break;
        }
        if (self->part->frame == 0) {
            self->state = 1;
            SetAnim(self->part, gLevelSelectCursorAnims[1]);
        }
        break;
    case 1:
        AdvanceSpriteAnim((struct box_part *)self->part);
        if (self->part->animDone) {
            self->state = 2;
            SetAnim(self->part, gLevelSelectCursorAnims[2]);
        }
        break;
    case 2:
        AdvanceSpriteAnim((struct box_part *)self->part);
        if (self->part->animDone) {
            self->state = 3;
            SetAnim(self->part, gLevelSelectCursorAnims[3]);
        }
        break;
    case 3:
        AdvanceSpriteAnim((struct box_part *)self->part);
        if (self->part->animDone) {
            self->state = 0;
            SetAnim(self->part, gLevelSelectCursorAnims[0]);
            ResetLevelSelectCursorIdleTimer(self);
        }
        break;
    case 4:
        if (self->scale <= 0xFF) {
            self->scale += self->speed;
        } else {
            self->scale = 0x100;
            self->state = 0;
            ResetLevelSelectCursorIdleTimer(self);
        }
        break;
    case 5:
        if (self->scale > 8)
            self->scale -= self->speed;
        else
            self->scale = 8;
        break;
    }
}

/* Draw step. While growing/shrinking the cursor is drawn by hand as an
 * affine OBJ with its own matrix; otherwise the sprite part draws it. */
void DrawLevelSelectCursor(struct cursor_panel *self)
{
    SetEntityPixelPos((struct actor *)self->part, self->line.x0, self->line.y0);
    switch (self->state) {
    case 4 ... 5:
        if (self->scale > 8) {
            s32 idx;

            self->oam.x = self->line.x0 - 0x20;
            self->oam.y = self->line.y0 - 0x20;
            idx = gOamBuffer->matrixCount++;
            self->oam.matrixLo = idx;
            self->oam.matrixBit3 = (idx >> 3) & 1;
            self->oam.matrixBit4 = (idx >> 4) & 1;
            SetLevelSelectCursorMatrix(self);
            SetAffineParam(gOamBuffer, idx * 4 + 0, self->matrix[0]);
            SetAffineParam(gOamBuffer, idx * 4 + 1, self->matrix[1]);
            SetAffineParam(gOamBuffer, idx * 4 + 2, self->matrix[2]);
            SetAffineParam(gOamBuffer, idx * 4 + 3, self->matrix[3]);
            AddOamEntry(gOamBuffer, &self->oam);
        }
        break;
    default:
        DrawSpriteWithOffset((struct actor *)self->part, 0, 0);
        break;
    }
}

/* Builds the OBJ affine matrix for the current zoom. */
void SetLevelSelectCursorMatrix(struct cursor_panel *self)
{
    u16 s = 0x10000 / self->scale;

    self->sx = s;
    self->sy = s;
    ObjAffineSet(&self->sx, self->matrix, 1, 2);
}

/* UNUSED. Shrunk away (state 5). */
u8 IsLevelSelectCursorHidden(struct cursor_panel *self)
{
    return self->state == 5;
}

/* UNUSED. Growing (state 4). */
u8 IsLevelSelectCursorGrowing(struct cursor_panel *self)
{
    return self->state == 4;
}

/* Starts the shrink-away. */
void HideLevelSelectCursor(struct cursor_panel *self)
{
    self->state = 5;
    self->scale = 0x100;
    UpdateLevelSelectCursor(self);
}

/* Parks the cursor at the bottom-left or bottom-right corner, whichever
 * side of the screen it is on (page turn). */
void ParkLevelSelectCursor(struct cursor_panel *self)
{
    struct xy_pair pos;

    if (self->part->x >> 8 <= 0x78)
        pos.x = 0x14;
    else
        pos.x = 0xDC;
    pos.y = 0x88;
    MoveToPos(self, &pos);
    self->timer = 0;
}

/* Glide: two Bresenham steps per frame towards (x1, y1). */
void GlideLevelSelectCursor(struct cursor_panel *self)
{
    s32 i;

    for (i = 1; i >= 0; i--) {
        if (self->line.x0 != self->line.x1 || self->line.y0 != self->line.y1)
            StepBresenhamLine(&self->line);
    }
}

/* Arrived at the glide target. */
u8 HasLevelSelectCursorArrived(struct cursor_panel *self)
{
    u8 done = FALSE;

    if (self->line.x0 == self->line.x1 && self->line.y0 == self->line.y1)
        done = TRUE;
    return done;
}

/* Starts a glide to (x, y). The zoom step is derived from half the
 * distance along the major axis. */
void MoveLevelSelectCursor(struct cursor_panel *self, s32 x, s32 y)
{
    self->line.x1 = x;
    self->line.y1 = y;
    if (self->line.x0 != x || self->line.y0 != y) {
        s32 d;

        InitBresenhamLine(&self->line);
        if (self->line.flag)
            d = (self->line.x1 - self->line.x0) / 2;
        else
            d = (self->line.y1 - self->line.y0) / 2;
        if (d < 0)
            d = -d;
        self->speed = 0xF8 / d;
    }
    if (self->state == 0)
        ResetIdleTimer(self);
}

/* UNUSED (inlined into ParkLevelSelectCursor). */
void MoveLevelSelectCursorTo(struct cursor_panel *self, struct xy_pair *pos)
{
    MoveToPos(self, pos);
}

/* Places the cursor at (x, y) at once. */
void SetLevelSelectCursorPos(struct cursor_panel *self, s32 x, s32 y)
{
    self->line.x0 = x;
    self->line.y0 = y;
    SetEntityPixelPos((struct actor *)self->part, x, y);
}

void ResetLevelSelectCursorIdleTimer(struct cursor_panel *self)
{
    ResetIdleTimer(self);
}

/* Destructor (`level_menu.panel`, called from DestroyLevelSelect). */
void DestroyLevelSelectCursor(struct cursor_panel *self, s32 flags)
{
    UnlockPalette(gPaletteCache, PART_RECORD(self->part).paletteId);
    DELETE_PART(self->part);
    if (flags & 1)
        OperatorDelete(self);
}
