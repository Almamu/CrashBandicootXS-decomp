#ifndef GUARD_PART_CTRL_H
#define GUARD_PART_CTRL_H

#include "core.h"
#include "box_part.h"

/* The enemy controller of the 0x0800B8DC-0x0800CA60 cluster
 * (src/enemies/, UpdateEnemyCtrl's own `self`,
 * docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md) and the part it steers
 * (`self->target`, "owner" in the older docs). CreateEnemyCtrl constructs
 * the controller in a 0x8C-byte block, and the level spawners
 * (include/text_popup.h) attach it to the sprite part they create. Only
 * the fields the code touches are named.
 *
 * Built with old_agbcc: see docs/matching/archive/issue-10-naked-retry.md. */

/* The steered part. Same object as include/box_part.h's
 * `struct box_part`, with the fields past 0x38 this cluster uses. */
struct ctrl_target {
    s32 x;              // 0x00 - Q8 fixed-point
    s32 y;              // 0x04 - Q8 fixed-point
    u16 id;             // 0x08 - bit index in the "gone" bitmap, 0xFFFF for none
    u8 kind;            // 0x0A
    u8 unk_0B;
    u8 gone:1;          // 0x0C
    u8 unk_0C_1:1;
    u8 visible:1;
    u8 hit:1;
    u8 flag4:1;
    u8 unk_0C_5:1;
    u8 flag6:1;
    u8 flag7:1;
    u8 unk_0D_0:3;      // 0x0D
    u8 solid:1;
    u8 unk_0D_4:4;
    u8 unk_0E[0xA];
    u8 *vtable;         // 0x18 - method table, see PART_METHOD
    u8 unk_1C[4];
    struct keyframe **keyframes; // 0x20
    u8 unk_24[4];
    /* The mirror bits. `u` is the usual view; UpdateEnemyPatrol's position
     * gate reads bit 4 once through each view, which is what keeps the
     * ROM's two sign tests of one `lsl #27` (see that function). */
    union {
        struct {
            u32 layer:2;
            u32 unk_2:2;
            u32 x:1;
            u32 y:1;
            u32 unk_6:2;
        } u;
        struct {
            s32 layer:2;
            s32 unk_2:2;
            s32 x:1;
            s32 y:1;
            s32 unk_6:2;
        } s;
    } mirror;           // 0x28
    u8 animating;       // 0x2C - nonzero while the keyframe timer runs
    u8 frame;           // 0x2D - current keyframe index
    u8 unk_2E[2];
    s32 tick;           // 0x30
    s32 timer;          // 0x34
    u8 animDone;        // 0x38
    u8 unk_39[0xF];
    s32 rampX[3];       // 0x48 - speedX's ramp: start, step, target (struct gobj.rampX)
    s32 rampY[3];       // 0x54 - speedY's ramp
    s32 speedX;         // 0x60
    s32 speedY;         // 0x64
    u8 hitAxes;         // 0x68 - collision axes the terrain probe resolved (8: Y)
};

/* The controller's method table (`self->anchor`, gEnemyCtrlVtable for
 * CreateEnemyCtrl's controllers); +0x50 is the method the mode trigger
 * (SetEnemyAnimMode) calls. */
struct ctrl_anchor {
    u8 unk_00[0x10];
    struct part_method bounce;  // 0x10
    struct part_method attach;  // 0x18 - AttachEnemyCtrl: hands the controller its part
    u8 unk_20[0x28];
    struct part_method launch;  // 0x48
    struct part_method trigger; // 0x50
};

struct part_ctrl {
    u8 unk_00[4];
    void *manager;      // 0x04 - the motion entry set StartCtrlTargetMotionXFromSet/
                        //        StartCtrlTargetMotionYFromSet read (gEnemyCtrlMotionSet)
    u8 unk_08[4];
    struct ctrl_anchor *anchor; // 0x0C
    s32 rangeX[2];      // 0x10 - homing bounds
    s32 rangeY[2];      // 0x18
    s32 boxL;           // 0x20 - hit/trigger box, relative to the target
                        //        (UpdateEnemyTriggerBox, SetEnemyTriggerBox)
    s32 boxT;           // 0x24
    s32 boxR;           // 0x28
    s32 boxB;           // 0x2C
    s32 idleTime;       // 0x30 - attack cycle (UpdateEnemyAttackCycle): frames in mode 0
                        //        before the attack (mode 3/4) starts
    s32 attackTime;     // 0x34 - frames in the attack before it ends (mode 5); the cycle
                        //        repeats every idleTime + attackTime frames of gRoomFrameCount
    s32 cycleOffset;    // 0x38 - where in the cycle the enemy starts (SetEnemyState starts it
                        //        attacking when cycleOffset >= idleTime)
    s32 period;         // 0x3C - oscillator (SetEnemyOscillator, UpdateEnemyOscillateX)
    s32 phase;          // 0x40
    s32 amplitude;      // 0x44
    s32 shotPeriod;     // 0x48 - UpdateEnemyShooter fires every shotPeriod
    s32 shotPhase;      // 0x4C   frames, offset by shotPhase
    u8 unk_50[8];
    s32 speed;          // 0x58 - homing
    s32 accel;          // 0x5C
    s32 baseX;          // 0x60 - oscillator base / last target x
    s32 baseY;          // 0x64 - oscillator base / last target y
    s32 mode;           // 0x68 - see SetEnemyAnimMode
    s32 kind;           // 0x6C - the enemy kind (its sprite bank)
    struct ctrl_target *target; // 0x70
    s32 state;          // 0x74 - UpdateEnemyCtrl's state
    s32 modeB;          // 0x78 - see SetEnemyMotionX
    s32 modeA;          // 0x7C - see SetEnemyMotionY
    s32 counter;        // 0x80
    const s32 *anims;   // 0x84 - per-mode argument of the trigger: anim mode ->
                        //        bank anim (gEnemyDefaultAnimMap..., SetEnemyModeTable)
    struct ctrl_target *popup; // 0x88 - floating popup spawned in state 18
};

COMPILE_TIME_ASSERT(part_ctrl_h, sizeof(struct part_ctrl) == 0x8C);

#endif /* GUARD_PART_CTRL_H */
