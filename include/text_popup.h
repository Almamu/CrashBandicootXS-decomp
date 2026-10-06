#ifndef GUARD_TEXT_POPUP_H
#define GUARD_TEXT_POPUP_H

/* Shared by the enemy spawners in
 * src/level/spawn_enemies.c, spawn_bosses.c and
 * spawn_objects.c (ROM 0x0801EF0C-0x08021BFC; they were read as
 * "two-line text popup" spawners at first, hence the file name). Each
 * builds a sprite part with CreateMovingSprite, attaches a freshly constructed
 * enemy controller (CreateEnemyCtrl) to it, and fills the part's two "collected" bits
 * from the level's record table. This ROM region was built with
 * old_agbcc (see docs/matching/archive/old-agbcc-retry.md). */

#include "actor.h"
#include "enemies.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"

/* The sprite part CreateMovingSprite returns. Same layout as cortex.c's
 * `struct gfx_part`. The +0x28 bits are declared on a 32-bit base type:
 * with `u8` bitfields the shared `1` constant is a QImode pseudo that CSE
 * merges with `kind = 1`, and the allocator no longer matches. */
struct popup_part {
    struct actor base; // 0x00
    u8 unk_1C[4];
    void *anim; // 0x20
    u8 unk_24[4];
    u32 unk_28_0:4; // 0x28
    u32 flipX:1;
    u32 flipY:1;
    u32 unk_28_6:2;
    u8 frameNibble:4; // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[2];
    u8 animating; // 0x2C - nonzero while the keyframe timer runs
    u8 tag;       // 0x2D
    u8 unk_2E[0x16];
    struct part_ctrl *hdr; // 0x44
};

/* One level record, at `params + paramOffsets[id]` (bytes) in the
 * room's entity list (gEntityFlags->list). After the flags come
 * per-type parameter words; `p` has one view per layout, named after
 * the part_ctrl setters spawn_enemies.c hands them to (SetEnemyRangeX:
 * `rangeX`/`rangeY` are pixels either side of the spawn point;
 * SetEnemyRangeXSpeed/SetEnemyRangeYSpeed: `speed`/`accel`;
 * SetEnemyAttackCycle: `idleTime`/`attackTime`/`cycleOffset`;
 * SetEnemyWave: `period`/`phase`/`amplitude`). */
struct level_record {
    u8 flags; // bit 1: clear = X-mirrored (popup_part.flipX), bit 2: Y-mirrored
    u8 unk_01[3];
    union {
        // lizard, patrolling jungle enemy, polar bear, patrolling sewer enemy, rat
        struct {
            s32 rangeX; // 0x04
        } patrol;
        // blowgun tribesman, pufferfish (with the wave), piston crusher, wooden
        // crusher; the saucer lab assistant reads only `cycleOffset`
        struct {
            s32 idleTime;      // 0x04
            s32 attackTime;    // 0x08
            s32 cycleOffset;   // 0x0C
            s32 waveAmplitude; // 0x10
            s32 wavePeriod;    // 0x14
            s32 wavePhase;     // 0x18
        } cycle;
        // stationary space enemy, flamethrower lab assistant
        struct {
            s32 attackTime;  // 0x04
            s32 idleTime;    // 0x08
            s32 cycleOffset; // 0x0C
        } attackFirst;
        // electric eel, patrolling space enemy
        struct {
            s32 rangeX;      // 0x04
            s32 idleTime;    // 0x08
            s32 attackTime;  // 0x0C
            s32 cycleOffset; // 0x10
        } rangeCycle;
        // penguin
        struct {
            s32 rangeX;      // 0x04
            s32 attackTime;  // 0x08
            s32 idleTime;    // 0x0C
            s32 cycleOffset; // 0x10
        } rangeAttackFirst;
        // shark: X range, Y homing
        struct {
            s32 rangeX; // 0x04
            s32 rangeY; // 0x08
            s32 speedY; // 0x0C
            s32 accelY; // 0x10
        } rangeHomingY;
        // jellyfish
        struct {
            s32 amplitude; // 0x04
            s32 period;    // 0x08
            s32 phase;     // 0x0C
        } wave;
        // homing sewer enemy
        struct {
            s32 rangeX; // 0x04
            s32 speedX; // 0x08
            s32 accelX; // 0x0C
        } homingX;
        // sea mine
        struct {
            s32 rangeY; // 0x04
            s32 speedY; // 0x08
            s32 accelY; // 0x0C
            s32 rangeX; // 0x10
            s32 speedX; // 0x14
            s32 accelX; // 0x18
        } homingXY;
    } p;
};


extern s32 _call_via_r2(void *self, void *arg, void *fn);

#define POPUP_ANIM(offset) ((void *)(SPRITE_BANK_BASE + (offset)))

/* hdr->attach(part) (AttachEnemyCtrl), through _call_via_r2. */
#define POPUP_ATTACH(hdr, part)                                                \
    _call_via_r2((u8 *)(hdr) + (hdr)->anchor->attach.thisOffset, (part),       \
                (hdr)->anchor->attach.fn)

#define LEVEL_RECORD(id)                                                       \
    ((struct level_record *)((const u8 *)gEntityFlags->list->params +  \
                             gEntityFlags->list->paramOffsets[id]))

/* The setters below are inline because old_agbcc schedules a store's
 * value before its address only when the value arrives as an inline
 * helper's parameter; written in place, the order flips. */
static inline void SetPartKind(struct popup_part *part, s32 value)
{
    part->base.kind = value;
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

static inline void SetEnemyAttackCycle(struct part_ctrl *hdr, s32 idleTime, s32 attackTime,
                                       s32 cycleOffset)
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
