#ifndef GUARD_TEXT_POPUP_H
#define GUARD_TEXT_POPUP_H

/* Shared by the enemy spawners in
 * src/level/spawn_enemies.c, spawn_bosses.c and
 * spawn_objects.c (ROM 0x0801EF0C-0x08021BFC; they were read as
 * "two-line text popup" spawners at first, hence the file name). Each
 * builds a sprite part with CreateMovingSprite, attaches a freshly constructed
 * enemy controller (CreateEnemyCtrl) to it, and fills the part's two "collected" bits
 * from the level's record table. This ROM region was built with
 * old_agbcc (see docs/matching/old-agbcc-retry.md). */

#include "actor.h"
#include "enemies.h"

/* The sprite part CreateMovingSprite returns. Same layout as cortex.c's
 * `struct gfx_part`. The +0x28 bits are declared on a 32-bit base type:
 * with `u8` bitfields the shared `1` constant is a QImode pseudo that CSE
 * merges with `field_0A = 1`, and the allocator no longer matches. */
struct popup_part
{
    struct actor base;          // 0x00
    u8 unk_1C[4];
    void *anim;                 // 0x20
    u8 unk_24[4];
    u32 unk_28_0:4;             // 0x28
    u32 flipX:1;
    u32 flipY:1;
    u32 unk_28_6:2;
    u8 frameNibble:4;           // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[2];
    u8 animating;               // 0x2C - nonzero while the keyframe timer runs
    u8 tag;                     // 0x2D
    u8 unk_2E[0x16];
    struct part_ctrl *hdr;      // 0x44
};

/* One level record, at `bytes + offsets[id]` in gEntityFlags's
 * table. */
struct level_record
{
    u8 flags;                   // bit 1: clear = X-mirrored (popup_part.flipX), bit 2: Y-mirrored
    u8 unk_01[3];
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
};

struct level_record_table
{
    u8 unk_00[8];
    u16 *offsets;
    u8 *bytes;
};

extern void ***gSpriteBankSet;
extern struct level_record_table **gEntityFlags;
extern void *gCollidableList;

extern struct popup_part *CreateMovingSprite(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern s32 GetSpriteAnimPaletteSlot(struct popup_part *part);
extern void *OperatorNew(s32 size);
extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern void AddToPartList(void *manager, void *value);
extern void ResetSpriteFrameTimer(struct popup_part *part);
extern void ResetSpriteFrameIndex(struct popup_part *part);
extern void SetSpriteAnimDone(struct popup_part *part, s32 arg);

#define POPUP_ANIM(offset) ((void *)((u8 *)**gSpriteBankSet + (offset)))

/* hdr->attach(part) (AttachEnemyCtrl), through _call_via_r2. */
#define POPUP_ATTACH(hdr, part)                                                \
    _call_via_r2((u8 *)(hdr) + (hdr)->anchor->attach.thisOffset, (part),       \
                (hdr)->anchor->attach.fn)

#define LEVEL_RECORD(id)                                                       \
    ((struct level_record *)((*gEntityFlags)->bytes +                     \
                             (*gEntityFlags)->offsets[id]))

/* The setters below are inline because old_agbcc schedules a store's
 * value before its address only when the value arrives as an inline
 * helper's parameter; written in place, the order flips. */
static inline void SetPartField0A(struct popup_part *part, s32 value)
{
    part->base.field_0A = value;
}

static inline void SetEnemyAnimMap(struct part_ctrl *hdr, const s32 *gfx)
{
    hdr->anims = gfx;
}

static inline void SetPartTag(struct popup_part *part, s32 tag)
{
    part->tag = tag;
}

/* The mask arrives as an `s32` so old_agbcc derives it from a still-live
 * constant (`subs r0, #0x43`) the way the ROM does. */
static inline void AndPartFlags(struct popup_part *part, s32 mask)
{
    part->base.flags &= mask;
}

/* Hands the part animation `anim` and restarts it. */
static inline void SetPartAnim(struct popup_part *part, s32 anim)
{
    part->tag = anim;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
}

/* The multi-field setters load every value before storing any, as the ROM
 * does; written as separate statements, each load/store pair interleaves. */
static inline void SetEnemyHitBox(struct part_ctrl *hdr, s32 l, s32 t, s32 r, s32 b)
{
    hdr->boxL = l;
    hdr->boxR = r;
    hdr->boxT = t;
    hdr->boxB = b;
}

static inline void SetEnemyAttackCycle(struct part_ctrl *hdr, s32 idleTime, s32 attackTime, s32 cycleOffset)
{
    hdr->idleTime = idleTime;
    hdr->attackTime = attackTime;
    hdr->cycleOffset = cycleOffset;
}

static inline void SetEnemyWave(struct part_ctrl *hdr, s32 period, s32 phase, s32 amplitude)
{
    hdr->period = period;
    hdr->phase = phase;
    hdr->amplitude = amplitude;
}

#endif /* GUARD_TEXT_POPUP_H */
