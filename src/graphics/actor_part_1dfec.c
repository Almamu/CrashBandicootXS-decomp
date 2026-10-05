#include "core.h"
#include "line_util.h"
#include "level_select_parts.h"

/* GitHub issue #29: 0x0801DFEC-0x0801E578, the whole of the former
 * asm/code_3_2_17_188d0_1dfec.s.
 *
 * - CreateLevelSelectEntry: `struct level_item`'s constructor (its methods end
 *   issue #28, actor_part_1da38.c).
 * - CreateLevelSelectCursor-DestroyLevelSelectCursor: `struct cursor_panel`, the level-select
 *   screen's cursor (`level_menu.panel`, 0x54 bytes). It glides between
 *   entries along a Bresenham line (two steps per frame), plays an idle
 *   animation cycle at random intervals, and grows in (state 4) / shrinks
 *   away (state 5) as an affine OBJ drawn straight into the OAM shadow
 *   buffer.
 *
 * This file is compiled with tools/agbcc/bin/old_agbcc (see Makefile and
 * docs/matching/issue-24-boss-actor.md). Every function is real C; see
 * docs/matching/issue-28-29-level-select-parts.md.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/, and no
 * Thumb pointer anywhere in the ROM: sub_801E3D4, sub_801E3E4,
 * sub_801E4E4 (ParkLevelSelectCursor has it inlined). Matched anyway. */

/* One hardware OAM entry. The matrix number is split in three because
 * the ROM writes it three bits + one + one (DrawLevelSelectCursor). */
struct oam_attrs
{
    u32 y:8;            // 0x00
    u32 affineMode:2;   // 0x01
    u32 objMode:2;
    u32 mosaic:1;
    u32 bpp:1;
    u32 shape:2;
    u32 x:9;            // 0x02
    u32 matrixLo:3;
    u32 matrixBit3:1;
    u32 matrixBit4:1;
    u32 size:2;
    u16 tileNum:10;     // 0x04
    u16 priority:2;
    u16 palette:4;
    u16 affineParam;    // 0x06
};

struct oam_entry
{
    u16 attr0;
    u16 attr1;
    u16 attr2;
    s16 affineParam;
};

/* Same 0x40C-byte OAM shadow buffer src/graphics/graphics.c names
 * `struct oam_shadow_buffer`; `matrixCount` counts the affine matrices
 * handed out this frame. */
struct oam_shadow_buffer
{
    s32 count;
    s32 base;
    s32 matrixCount;
    struct oam_entry entries[128];
};

/* The level-select cursor (CreateLevelSelectCursor, 0x54 bytes). */
struct cursor_panel
{
    struct bresenham_line line; // 0x00 - (x0, y0) is the position
    struct sprite *part;        // 0x28
    s32 timer;                  // 0x2C - frames to the next idle cycle
    s32 state;                  // 0x30 - see UpdateLevelSelectCursor
    struct oam_attrs oam;       // 0x34
    s32 speed;                  // 0x3C - zoom step (MoveLevelSelectCursor)
    s32 scale;                  // 0x40 - 0x100 = 1:1
    /* ObjAffineSet source */
    s16 sx;                     // 0x44
    s16 sy;                     // 0x46
    u16 angle;                  // 0x48
    u8 unk_4A[2];
    s16 matrix[4];              // 0x4C - pa, pb, pc, pd
};

COMPILE_TIME_ASSERT(sizeof(struct cursor_panel) == 0x54);

struct xy
{
    s32 x;
    s32 y;
};

extern struct oam_shadow_buffer *gOamBuffer;
extern u32 gStaticData_0816C634[];
extern u8 gStaticData_086377C0[];

extern void AddOamEntry(struct oam_shadow_buffer *buf, struct oam_attrs *oam);
extern void ObjAffineSet(void *src, void *dst, s32 count, s32 stride);

void UpdateLevelSelectCursor(struct cursor_panel *self);
void SetLevelSelectCursorMatrix(struct cursor_panel *self);
void GlideLevelSelectCursor(struct cursor_panel *self);
void MoveLevelSelectCursor(struct cursor_panel *self, s32 x, s32 y);
void SetLevelSelectCursorPos(struct cursor_panel *self, s32 x, s32 y);
void sub_801E504(struct cursor_panel *self);

static inline void ResetIdleTimer(struct cursor_panel *self)
{
    self->timer = (u16)RandRange(300) + 600;
}

static inline void MoveToPos(struct cursor_panel *self, struct xy *pos)
{
    MoveLevelSelectCursor(self, pos->x, pos->y);
}

/* One affine parameter; OBJ matrix `m` is entries 4m..4m+3. An inline
 * so that the value is loaded before the address is computed. */
static inline void SetAffineParam(struct oam_shadow_buffer *buf, s32 n, u16 v)
{
    buf->entries[n].affineParam = v;
}

struct level_item *CreateLevelSelectEntry(struct level_item *self)
{
    self->vtable = gLevelSelectEntryVtable;
    self->selected = 0;
    self->frame = InitUiSpriteObj(OperatorNew(0x40));
    self->frame->anim = AnimTable(0x24C);
    SetSpritePriority(self->frame, 1);
    self->icon = InitUiSpriteObj(OperatorNew(0x40));
    self->icon->anim = AnimTable(0x264);
    SetSpritePriority(self->icon, 1);
    return self;
}

struct cursor_panel *CreateLevelSelectCursor(struct cursor_panel *self)
{
    struct sprite *p = InitUiSpriteObj(OperatorNew(0x40));

    self->part = p;
    p->anim = AnimTable(0x240);
    SetAnim(p, 0);
    self->part->palette = GetSpriteAnimPaletteSlot(self->part);
    LockPalette(gPaletteCache, PART_RECORD(self->part).tileRecord);
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
    LoadTaggedAsset(gStaticData_086377C0, OBJ_VRAM0 + 0x7800);
    self->angle = 0;
    self->scale = 8;
    self->state = 4;
    return self;
}

/* Per-frame update. Steps the glide; states 0-3 are the idle animation
 * cycle (0 waits for `timer`, then plays gStaticData_0816C634[1..3] and
 * back to [0]), 4 grows the cursor to 1:1, 5 shrinks it away. */
void UpdateLevelSelectCursor(struct cursor_panel *self)
{
    GlideLevelSelectCursor(self);
    switch (self->state)
    {
    case 0:
        AdvanceSpriteAnim(self->part);
        if (self->timer != 0)
        {
            self->timer--;
            break;
        }
        if (self->part->frame == 0)
        {
            self->state = 1;
            SetAnim(self->part, gStaticData_0816C634[1]);
        }
        break;
    case 1:
        AdvanceSpriteAnim(self->part);
        if (self->part->animDone)
        {
            self->state = 2;
            SetAnim(self->part, gStaticData_0816C634[2]);
        }
        break;
    case 2:
        AdvanceSpriteAnim(self->part);
        if (self->part->animDone)
        {
            self->state = 3;
            SetAnim(self->part, gStaticData_0816C634[3]);
        }
        break;
    case 3:
        AdvanceSpriteAnim(self->part);
        if (self->part->animDone)
        {
            self->state = 0;
            SetAnim(self->part, gStaticData_0816C634[0]);
            sub_801E504(self);
        }
        break;
    case 4:
        if (self->scale <= 0xFF)
        {
            self->scale += self->speed;
        }
        else
        {
            self->scale = 0x100;
            self->state = 0;
            sub_801E504(self);
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
    sub_800737C(self->part, self->line.x0, self->line.y0);
    switch (self->state)
    {
    case 4 ... 5:
        if (self->scale > 8)
        {
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
        DrawSpriteWithOffset(self->part, 0, 0);
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
u8 sub_801E3D4(struct cursor_panel *self)
{
    return self->state == 5;
}

/* UNUSED. Growing (state 4). */
u8 sub_801E3E4(struct cursor_panel *self)
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
    struct xy pos;

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

    for (i = 1; i >= 0; i--)
    {
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
    if (self->line.x0 != x || self->line.y0 != y)
    {
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
void sub_801E4E4(struct cursor_panel *self, struct xy *pos)
{
    MoveToPos(self, pos);
}

/* Places the cursor at (x, y) at once. */
void SetLevelSelectCursorPos(struct cursor_panel *self, s32 x, s32 y)
{
    self->line.x0 = x;
    self->line.y0 = y;
    sub_800737C(self->part, x, y);
}

void sub_801E504(struct cursor_panel *self)
{
    ResetIdleTimer(self);
}

/* Destructor (`level_menu.panel`, called from DestroyLevelSelect). */
void DestroyLevelSelectCursor(struct cursor_panel *self, s32 flags)
{
    UnlockPalette(gPaletteCache, PART_RECORD(self->part).tileRecord);
    DELETE_PART(self->part);
    if (flags & 1)
        OperatorDelete(self);
}
