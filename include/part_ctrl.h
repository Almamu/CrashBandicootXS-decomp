#ifndef GUARD_PART_CTRL_H
#define GUARD_PART_CTRL_H

#include "core.h"
#include "box_part.h"
#include "constants/entities.h"

/* The enemy controller of the 0x0800B8DC-0x0800CA60 cluster
 * (src/enemies/, UpdateEnemyCtrl's own `self`,
 * docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md) and the part it steers
 * (`self->target`, "owner" in the older docs). CreateEnemyCtrl constructs
 * the controller in a 0x8C-byte block, and the enemy spawners
 * (src/level/spawn_enemies.cpp) attach it to the sprite part they create. Only
 * the fields the code touches are named.
 *
 * The controller is the C++ class EnemyCtrl (include/enemy_ctrl.hpp),
 * which reads the part through `struct ctrl_target`. */

/* The steered part. Same object as include/box_part.h's
 * `struct box_part`, with the fields past 0x38 this cluster uses. */
struct ctrl_target {
    s32 x;   // 0x00 - Q8 fixed-point
    s32 y;   // 0x04 - Q8 fixed-point
    u16 id;  // 0x08 - bit index in the "gone" bitmap, 0xFFFF for none
    u8 kind; // 0x0A
    u8 unk_0B;
    u8 gone:1; // 0x0C
    u8 unk_0C_1:1;
    u8 visible:1;
    u8 hit:1;
    u8 flag4:1;
    u8 unk_0C_5:1;
    u8 flag6:1;
    u8 flag7:1;
    u8 unk_0D_0:3; // 0x0D
    u8 solid:1;
    u8 unk_0D_4:4;
    u8 unk_0E[0xA];
    u8 *vtable; // 0x18 - the vtable (Entity's, include/entity.hpp)
    u8 unk_1C[4];
    struct keyframe **keyframes; // 0x20
    u8 unk_24[4];
    struct {
        u32 layer:2;
        u32 unk_2:2;
        u32 x:1; // X mirrored
        u32 y:1; // Y mirrored
        u32 unk_6:2;
    } mirror;     // 0x28
    u8 animating; // 0x2C - nonzero while the keyframe timer runs
    u8 frame;     // 0x2D - current keyframe index
    u8 unk_2E[2];
    s32 tick;    // 0x30
    s32 timer;   // 0x34
    u8 animDone; // 0x38
    u8 unk_39[0xB];
    void *ctrl;   // 0x44 - its controller (struct gobj.mover; HitEnemy attaches the knocked one)
    s32 rampX[3]; // 0x48 - speedX's ramp: start, step, target (struct gobj.rampX)
    s32 rampY[3]; // 0x54 - speedY's ramp
    s32 speedX;   // 0x60
    s32 speedY;   // 0x64
    u8 hitAxes;   // 0x68 - collision axes the terrain probe resolved (8: Y)
};

#endif /* GUARD_PART_CTRL_H */
