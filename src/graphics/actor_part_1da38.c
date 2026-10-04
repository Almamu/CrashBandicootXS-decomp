#include "core.h"
#include "level_select_parts.h"

/* GitHub issue #28: 0x0801DA38-0x0801DFEC, the whole of the former
 * asm/code_3_2_17_188d0_1da38.s. Two of the level-select screen's
 * (actor_part_1b85c.c) sub-objects:
 *
 * - sub_801DA38-sub_801DE04: `struct zoom_bg`, the level picture on the
 *   affine BG2 layer (`level_menu.bg2`, constructor sub_801D828 in the
 *   issue #27 range). It zooms out to swap in the selected level's
 *   picture (states 1 -> 2 -> 0), wobbles on the sine table while shown
 *   (state 3) and zooms away on exit (states 4 -> 5). Four sprite parts
 *   ("twinkles") sit on top of it, each showing a random frame for a
 *   random time and blinking along with the wobble.
 * - sub_801DE24-sub_801DF98: `struct level_item`'s methods (method table
 *   gStaticData_087E4BAC: +0x08 sub_801DE30 bob, +0x10 sub_801DEA4
 *   set level, +0x18 sub_801DF70 set position, +0x20 nullsub_20, +0x28
 *   sub_801DF98 destructor; the constructor sub_801DFEC starts issue #29).
 *
 * This file is compiled with tools/agbcc/bin/old_agbcc (see Makefile and
 * docs/matching/issue-24-boss-actor.md). Every function is real C; see
 * docs/matching/issue-28-29-level-select-parts.md. */

struct twinkle
{
    s32 timer;             // 0x00 - frames until a new frame is picked
    s32 blink;             // 0x04 - blink window, counts down with timer
    struct sprite *part;   // 0x08
};

/* REG_BG2CNT's layout. `packed` keeps the union 2 bytes: this ABI rounds
 * every struct up to a word. */
struct bgcnt_bits
{
    u16 priority:2;
    u16 charBase:2;
    u16 unk_4:2;
    u16 mosaic:1;
    u16 colors256:1;
    u16 screenBase:5;
    u16 wrap:1;
    u16 size:2;
} __attribute__((packed));

union bgcnt
{
    u16 raw;
    struct bgcnt_bits bits;
} __attribute__((packed));

/* The level-select screen's BG2 picture (sub_801D828, 0x8C bytes). */
struct zoom_bg
{
    u8 unk_00[0x0C];
    s32 state;             // 0x0C - see sub_801DAD8
    s32 image;             // 0x10 - gStaticData_0816C5A0 index, 11 = none
    s32 scale;             // 0x14 - zoom, 0x100 = 1:1, 8 = smallest
    s32 charBase;          // 0x18
    s32 screenBase;        // 0x1C
    s32 x;                 // 0x20 - screen centre
    s32 y;                 // 0x24
    s32 dx;                // 0x28 - wobble offset
    s32 dy;                // 0x2C
    u32 phase;             // 0x30 - wobble phase, 0-0xFF
    union bgcnt bgcnt;     // 0x34 - REG_BG2CNT value (sub_801DE24)
    u8 unk_36[2];
    /* BgAffineSet source, 0x38-0x49 */
    s32 texX;              // 0x38
    s32 texY;              // 0x3C
    s16 scrX;              // 0x40
    s16 scrY;              // 0x42
    s16 sx;                // 0x44
    s16 sy;                // 0x46
    u16 alpha;             // 0x48 - rotation
    u8 unk_4A[2];
    /* BgAffineSet destination, committed by sub_801DCBC */
    s16 pa;                // 0x4C
    s16 pb;                // 0x4E
    s16 pc;                // 0x50
    s16 pd;                // 0x52
    s32 bgx;               // 0x54
    s32 bgy;               // 0x58
    struct twinkle twinkles[4]; // 0x5C
};

COMPILE_TIME_ASSERT(sizeof(struct zoom_bg) == 0x8C);

/* A level picture: 32-colour palette and 8bpp tiles (LoadTaggedAsset). */
struct image_pair
{
    void *palette;
    void *tiles;
};

extern void *gAudioContext;
extern s16 gStaticData_0816A820[];
extern struct image_pair gStaticData_0816C5A0[];
extern u32 gStaticData_0816C610[];
extern u32 gStaticData_0816C624[];

extern void PlaySfx(void *ctx, s32 sfx, s32 volume);
extern void BgAffineSet(void *src, void *dst, s32 count);

void sub_801DE04(struct zoom_bg *self, struct twinkle *t);
u8 sub_801DD18(struct zoom_bg *self);
void sub_801DD90(struct zoom_bg *self, struct twinkle *t);
void sub_801DDB4(struct zoom_bg *self, struct twinkle *t);

static inline void SetPosQ8(struct sprite *p, s32 x, s32 y)
{
    p->x = x << 8;
    p->y = y << 8;
}

/* Destructor (`level_menu.bg2`, called from sub_801C040). */
void sub_801DA38(struct zoom_bg *self, s32 flags)
{
    sub_8006D68(gUnknown_030012B8, PART_RECORD(self->twinkles[0].part).tileRecord);
    DELETE_PART(self->twinkles[3].part);
    DELETE_PART(self->twinkles[2].part);
    DELETE_PART(self->twinkles[1].part);
    DELETE_PART(self->twinkles[0].part);
    if (flags & 1)
        sub_8026ED0(self);
}

/* Per-frame state machine:
 * 0 zoom in to 1:1, then 3; 1 zoom out, then 2; 2 load the requested
 * picture (if any) and go to 0; 3 shown, wobbling; 4 zoom out while
 * rotating, then 5 (gone). The twinkles advance in every state. */
void sub_801DAD8(struct zoom_bg *self)
{
    u8 buf[0x200];

    switch (self->state)
    {
    case 0:
        if (self->scale <= 0xFF)
        {
            self->scale += 8;
            break;
        }
        self->scale = 0x100;
        self->state = 3;
        break;
    case 1:
        if (self->scale > 8)
        {
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
        self->dx = (gStaticData_0816A820[self->phase & 0xFF] * 4) >> 8;
        self->dy = (gStaticData_0816A820[(self->phase << 1) & 0xFF] * 4) >> 8;
        break;
    case 2:
        if (self->image == 11)
            break;
        PlaySfx(gAudioContext, 0x53, 0x100);
        LoadTaggedAsset(gStaticData_0816C5A0[self->image].palette, buf);
        DmaCopy16(3, buf, BG_PLTT, 0x40);
        LoadTaggedAsset(gStaticData_0816C5A0[self->image].tiles,
                        (void *)(BG_VRAM + (self->charBase << 14)));
        self->state = 0;
        break;
    case 4:
        if (self->scale > 8)
        {
            self->scale -= 8;
            self->alpha += 0x100;
            break;
        }
        self->scale = 8;
        self->state = 5;
        break;
    }
    sub_801DE04(self, &self->twinkles[0]);
    sub_801DE04(self, &self->twinkles[1]);
    sub_801DE04(self, &self->twinkles[2]);
    sub_801DE04(self, &self->twinkles[3]);
}

/* Draw step: moves the twinkles with the wobble, then rebuilds the BG2
 * affine matrix from the current zoom (rotation only in states 4-5). */
void sub_801DC28(struct zoom_bg *self)
{
    if (sub_801DD18(self))
    {
        sub_801DD90(self, &self->twinkles[0]);
        sub_801DD90(self, &self->twinkles[1]);
        sub_801DD90(self, &self->twinkles[2]);
        sub_801DD90(self, &self->twinkles[3]);
    }
    switch (self->state)
    {
    case 0 ... 3:
    {
        u16 s;

        self->scrX = self->x + self->dx;
        self->scrY = self->y + self->dy;
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
void sub_801DCBC(struct zoom_bg *self)
{
    REG_BG2PA = self->pa;
    REG_BG2PB = self->pb;
    REG_BG2PC = self->pc;
    REG_BG2PD = self->pd;
    REG_BG2X = self->bgx;
    REG_BG2Y = self->bgy;
}

/* Zooming away (state 4). */
u8 sub_801DCF8(struct zoom_bg *self)
{
    return self->state == 4;
}

/* Gone (state 5). */
u8 sub_801DD08(struct zoom_bg *self)
{
    return self->state == 5;
}

/* Shown (state 3). */
u8 sub_801DD18(struct zoom_bg *self)
{
    return self->state == 3;
}

/* Waiting for a picture (state 2). */
u8 sub_801DD28(struct zoom_bg *self)
{
    return self->state == 2;
}

/* Zooming out (state 1). */
u8 sub_801DD38(struct zoom_bg *self)
{
    return self->state == 1;
}

/* Starts the exit zoom, with BG2 at the front. */
void sub_801DD48(struct zoom_bg *self)
{
    self->bgcnt.bits.priority = 0;
    self->state = 4;
}

/* Zooms the picture out with no follow-up picture (page turn). */
void sub_801DD5C(struct zoom_bg *self)
{
    PlaySfx(gAudioContext, 0x54, 0x100);
    self->state = 1;
    self->image = 11;
}

/* Requests picture `image`, taken once the zoom-out finishes. */
void sub_801DD80(struct zoom_bg *self, s32 image)
{
    switch (self->state)
    {
    case 1 ... 2:
        self->image = image;
        break;
    }
}

/* Moves a blinking twinkle by the wobble offset. */
void sub_801DD90(struct zoom_bg *self, struct twinkle *t)
{
    if (t->blink > 0 && (t->timer & 4))
        sub_8008890(t->part, self->dx, self->dy);
}

/* Picks a new random frame, duration and blink window for a twinkle. */
void sub_801DDB4(struct zoom_bg *self, struct twinkle *t)
{
    SetFrame(t->part, (u16)RandRange(8));
    t->timer = (u16)RandRange(0x3C);
    if (t->timer != 0)
        t->blink = (u16)RandRange(t->timer / 2 + 1);
}

void sub_801DE04(struct zoom_bg *self, struct twinkle *t)
{
    if (t->timer != 0)
    {
        t->timer--;
        t->blink--;
    }
    else
    {
        sub_801DDB4(self, t);
    }
}

u16 sub_801DE24(struct zoom_bg *self)
{
    return self->bgcnt.raw;
}

u8 sub_801DE28(struct level_item *self)
{
    return self->selected;
}

s32 sub_801DE2C(struct level_item *self)
{
    return self->id;
}

/* Method +0x08: bobs the entry by `phase` (a byte; 0xA0-0xFF wrap to a
 * small upward offset), the icon 2 px lower while selected, and shows the
 * box's selected/unselected frame. */
void sub_801DE30(struct level_item *self, s32 phase)
{
    s32 dy;

    phase &= 0xFF;
    if (phase <= 0x9F)
        dy = -phase;
    else
        dy = 0x100 - phase;
    if (self->selected)
        sub_8008890(self->icon, 0, dy + 2);
    else
        sub_8008890(self->icon, 0, dy);
    if (self->selected)
        SetFrame(self->frame, 1);
    else
        SetFrame(self->frame, 0);
    sub_8008890(self->frame, 0, dy);
}

void sub_801DEA0(struct level_item *self, u8 selected)
{
    self->selected = selected;
}

/* Method +0x10: entry `index` of world `world`. Indices 0-4 are levels
 * (icon frame = level id); anything past that is the world's extra entry
 * (level id 0x14 + world, its own icon animation). */
void sub_801DEA4(struct level_item *self, s32 world, s32 index)
{
    if (index <= 4)
    {
        self->id = world * 5 + index;
        SetFrame(self->icon, self->id);
    }
    else
    {
        self->id = world + 0x14;
        SetAnim(self->icon, gStaticData_0816C624[world]);
    }
}

/* Sets the box's animation (gStaticData_0816C610[kind]) and refreshes
 * both parts' palettes. */
void sub_801DF0C(struct level_item *self, s32 kind)
{
    SetAnim(self->frame, gStaticData_0816C610[kind]);
    self->frame->palette = sub_800815C(self->frame);
    self->icon->palette = sub_800815C(self->icon);
}

/* Method +0x18: places the entry at pixel `pos` (x, y). */
void sub_801DF70(struct level_item *self, s32 *pos)
{
    SetPosQ8(self->icon, pos[0], pos[1] - 3);
    sub_800737C(self->frame, pos[0], pos[1]);
}

/* Method +0x20. */
void nullsub_20(void)
{
}

/* Method +0x28: destructor. */
void sub_801DF98(struct level_item *self, s32 flags)
{
    self->vtable = gStaticData_087E4BAC;
    DELETE_PART(self->icon);
    DELETE_PART(self->frame);
    if (flags & 1)
        sub_8026ED0(self);
}
