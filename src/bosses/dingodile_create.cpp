#include "boss_ctrl.hpp"

extern "C" {
#include "match.h"
}

/* GitHub issue #25, ROM 0x0801A794-0x0801A878 (see
 * docs/matching/archive/issue-25-level-objects.md): DingodileShieldCtrl's
 * constructor and DingodileCtrl's StartMotion, destructor, constructor
 * and two setters (include/boss_ctrl.hpp). StartMotion is the same
 * mirror-gated speed-ramp copy as MegaMixCtrl::SetMotionXFromSet
 * (mega_mix.cpp), indexed straight into gDingodileMotionEntries.
 *
 * UNUSED - no caller anywhere in the ROM (checked asm/ .s files, src/ .c files
 * and the ROM for Thumb pointers): SetStep, SetNextState. */

DingodileShieldCtrl::DingodileShieldCtrl()
{
}

/* Starts motion entry `index`: its X record (negated when `part` faces
 * left) and its Y record, with the speed set to the record's start. */
void DingodileCtrl::StartMotion(MovingSprite *part, s32 index)
{
    const struct speed_ramp *e = &gDingodileMotionRecords[gDingodileMotionEntries[index][0]];

    if (part->mirrorBits.flipX < 0) {
        s32 x = -e->start;
        s32 z = -e->target;
        s32 y = e->step;

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    } else {
        s32 x = e->start;
        s32 y = e->step;
        s32 z = e->target;

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
    {
        const struct speed_ramp *e2 = &gDingodileMotionRecords[gDingodileMotionEntries[index][1]];
        s32 x = e2->start;
        s32 y = e2->step;
        s32 z = e2->target;

        part->speedY = x;
        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
}

DingodileCtrl::~DingodileCtrl()
{
}

/* Spawns the shield at (x, y). */
DingodileCtrl::DingodileCtrl(u32 x, u32 y)
{
    SpawnShieldOrRocket(0, (u16)x, (u16)y, 0);
}

void DingodileCtrl::SetStep(s32 value)
{
    step = value;
}

void DingodileCtrl::SetNextState(s32 value)
{
    nextState = value;
}
