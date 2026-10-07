#include "boss_ctrl.hpp"

/* GitHub issue #22, ROM 0x08017ECC-0x08017FE8 - non-adjacent to
 * airship_fireball.c since UpdateMegaMix (mega_mix_update.cpp) sits between
 * them. MegaMixCtrl (include/boss_ctrl.hpp) looks motion records up like
 * Ctrl's ...FromSet methods (ctrl.cpp): `animSet->entries[index]` is an
 * {X, Y} pair of indexes, here into gMegaMixMotionRecords. `part->mirror`
 * bit 4/bit 5 negate the record's `start`/`target` like Ctrl's
 * SetTargetMotionX/StartTargetMotionX. */

/* Copies the pair's Y record into `part->rampY` (the speed is kept),
 * negated when `part` is Y-mirrored. */
void MegaMixCtrl::SetMotionYFromSet(MovingSprite *part, s32 index)
{
    const speed_ramp *rec = &gMegaMixMotionRecords[animSet->entries[index][1]];

    if ((s32)(part->mirror << 26) < 0) {
        s32 x = -rec->start;
        s32 z = -rec->target;
        s32 y = rec->step;

        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    } else {
        s32 x = rec->start;
        s32 y = rec->step;
        s32 z = rec->target;

        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
}

/* The same with the pair's X record, `part->rampX` and the X mirror bit. */
void MegaMixCtrl::SetMotionXFromSet(MovingSprite *part, s32 index)
{
    const speed_ramp *rec = &gMegaMixMotionRecords[animSet->entries[index][0]];

    if ((s32)(part->mirror << 27) < 0) {
        s32 x = -rec->start;
        s32 z = -rec->target;
        s32 y = rec->step;

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    } else {
        s32 x = rec->start;
        s32 y = rec->step;
        s32 z = rec->target;

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
}

/* Starts the pair's Y record: Ctrl::StartTargetMotionY (player_flags.cpp),
 * called directly, not through the vtable. */
void MegaMixCtrl::StartTargetMotionYFromSet(MovingSprite *part, s32 index)
{
    Ctrl::StartTargetMotionY(part, &gMegaMixMotionRecords[animSet->entries[index][1]]);
}

/* Starts the pair's X record: Ctrl::StartTargetMotionX (ctrl.cpp). */
void MegaMixCtrl::StartTargetMotionXFromSet(MovingSprite *part, s32 index)
{
    Ctrl::StartTargetMotionX(part, &gMegaMixMotionRecords[animSet->entries[index][0]].start);
}

/* Mode 1, the latch cleared, no frame stamp, and the Mega Mix motion
 * set. */
void MegaMixCtrl::Reset()
{
    SetMode(1);
    latch = 0;
    stamp = -1;
    animSet = &gMegaMixMotionSet;
}

MegaMixCtrl::~MegaMixCtrl()
{
}

MegaMixCtrl::MegaMixCtrl()
{
    Reset();
}
