#include "enemy_ctrl.hpp"

extern "C" {
#include "match.h"
#include <libgcc.h>
#include "globals.h"
}

/* GitHub issue #9/#10: EnemyCtrl's shooter (include/enemy_ctrl.hpp),
 * called only from Update's state 16, after UpdateAttackCycle
 * (docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md).
 *
 * Once every `shotPeriod` frames of gRoomFrameCount (offset by
 * `shotPhase`, the gate PeriodicSpawner::Update uses too), a walking
 * shooter (mode 0 or 4) starts its shooting animation (mode 2 or 7).
 * On the other frames, a shooting one goes back to walking when the
 * animation is done, and otherwise fires its shot (a harmful effect
 * part, kind 8) at keyframe 10 (mode 2) or 8 (mode 7).
 *
 * The C pinned the controller to r4 and read gRoomFrameCount in a
 * statement of its own; g++ gives the ROM's code as written. */
void EnemyCtrl::UpdateShooter()
{
    MovingSprite *part;
    struct ctrl_target *shot;

    if (__modsi3(gRoomFrameCount + shotPeriod - shotPhase, shotPeriod) == 0) {
        switch (mode) {
        case 0:
            SetAnimMode(2);
            break;
        case 4:
            SetAnimMode(7);
            break;
        }
        return;
    }

    part = sprite;
    if (part->animDone != 0) {
        switch (mode) {
        case 2:
            SetAnimMode(0);
            break;
        case 7:
            SetAnimMode(4);
            break;
        }
        return;
    }

    shot = 0;
    switch (mode) {
    case 2:
        if (part->frame == 0xa && part->stepTimer == 0)
            shot = (struct ctrl_target *)LaunchHarmfulEffectPart(0xc, 6, 0, -0xa, 0x400, part);
        break;
    case 7:
        if (part->frame == 8 && part->stepTimer == 0)
            shot = (struct ctrl_target *)LaunchHarmfulEffectPart(0xc, 6, 0, 8, 0x400, part);
        break;
    }
    if (shot != 0)
        shot->kind = 8;
}
