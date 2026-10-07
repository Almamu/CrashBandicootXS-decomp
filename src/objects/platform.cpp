#include "platform.hpp"
#include "player.hpp"

/* Platform's and PlatformMover's methods (#664, include/platform.hpp), ROM
 * 0x0801B208-0x0801B85C.
 *
 * UNUSED - no caller anywhere in the ROM: the out-of-line constructor
 * InitPlatform (CreatePlatform inlines it), SetTargetMotionYFromSet,
 * SetTargetMotionXFromSet and ClearActive. */

/* While near the camera, steps the animation; then the velocity, and the
 * mover. A Neo Cortex platform that has crumbled (type 6 past frame 0x12)
 * drops the player. */
void Platform::Update()
{
    if (IsNearCamera()) {
        AdvanceAnim();
        ApplyVelocity();
        if (type == 6 && frame > 0x12) {
            Sprite **c = &gPlayer->carried;

            if (*c == this)
                *c = 0;
        }
        if (mover)
            mover->Update(this);
    } else {
        ApplyVelocity();
        if (mover)
            mover->Update(this);
    }
}

/* The bonus platform's exit facing (flags2 bit 4), set by CreatePlatform
 * from bit 0 of its entity's parameter flags (SetExitMirror). RunRoom
 * passes it to SetCheckpoint as the checkpoint flags, so the player comes
 * back from the bonus round on the platform X-mirrored when it is set. */
s32 Platform::GetExitMirror()
{
    return (f.bytes.flags2 >> 4) & 1;
}

/* Sets the exit facing GetExitMirror reads (CreatePlatform, type 3: the
 * active bonus platform). */
void Platform::SetExitMirror(u8 value)
{
    u32 one = 1;
    u32 bit;
    s32 mask;

    /* Emits nothing: hides the 1 from reload, which would otherwise
     * build the mask below from it, as `1 - 0x12`. */
    MATCH_KEEP(one);
    bit = (value & one) << 4;
    mask = ~0x10;
    f.bytes.flags2 = (mask & f.bytes.flags2) | bit;
}

s32 Platform::GetClassId()
{
    return 4;
}

Platform::~Platform()
{
}

/* Clears `vulnerable`, as the constructor does. */
void Platform::ClearVulnerable()
{
    f.b.vulnerable = 0;
}

Platform::Platform()
{
    ClearVulnerable();
}

/* The motion record of the set's entry 1, X or Y. */
static inline const struct speed_ramp *MoverRamp(PlatformMover *self)
{
    return &gPlatformMoverMotionRecords[self->animSet->entries[1][0]];
}

/* Holds the animation at its first frame (its last, if it has none). */
static inline void HoldFirstFrame(MovingSprite *part)
{
    s32 f = 0;
    s32 n = part->anim->records[part->tag].frames;

    CLAMP_INDEX(f, n);
    part->frame = f;
}

/* The mover's per-frame step (slot 1). On the first frame each axis with
 * a range starts moving (motion record 1 of the set, its sign flipped
 * unless dirX/dirY); the distance travelled accumulates in distX/distY,
 * and once it passes rangeX/rangeY the direction flips. Types 5, 6 and 7
 * add timed behaviour: 5 falls (slot 12, record 3) 60 frames after the
 * player lands and wobbles the platform +-3 px meanwhile; 6 and 7 hold
 * their animation at its first frames until the player lands on a 7 or
 * a 6's timer runs out, and a 7 is gone (MarkGone) once its animation
 * ends. Then MovePlayer carries the player along. */
void PlatformMover::Update(MovingSprite *part)
{
    const struct speed_ramp *e;

    if (lastX == 0 && rangeX > 0) {
        lastX = Q8_TO_INT(part->x);
        e = MoverRamp(this);

        if (dirX) {
            s32 start = e->start;
            s32 step = e->step;
            s32 target = e->target;

            part->speedX = start;
            part->rampX.start = start;
            part->rampX.step = step;
            part->rampX.target = target;
        } else {
            s32 start = -e->start;
            s32 target = -e->target;
            s32 step = e->step;

            part->speedX = start;
            part->rampX.start = start;
            part->rampX.step = step;
            part->rampX.target = target;
        }
    }
    if (lastY == 0 && rangeY > 0) {
        lastY = Q8_TO_INT(part->y);
        e = MoverRamp(this);

        if (dirY) {
            s32 start = e->start;
            s32 step = e->step;
            s32 target = e->target;

            part->speedY = start;
            part->rampY.start = start;
            part->rampY.step = step;
            part->rampY.target = target;
        } else {
            s32 start = -e->start;
            s32 target = -e->target;
            s32 step = e->step;

            part->speedY = start;
            part->rampY.start = start;
            part->rampY.step = step;
            part->rampY.target = target;
        }
    }

    {
        s32 d = distX;

        if (d != -1) {
            s32 v = Q8_TO_INT(part->x) - lastX;
            s32 sign;

            MAKE_ABS_BRANCHLESS(v, sign);
            distX = d + v;
        } else {
            s32 v = part->speedX;
            s32 sign;

            MAKE_ABS_BRANCHLESS(v, sign);
            if (v >= gPlatformMoverMotionRecords[animSet->entries[1][0]].target)
                distX = 0;
        }
    }
    {
        s32 d = distY;

        if (d != -1) {
            s32 v = Q8_TO_INT(part->y) - lastY;
            s32 sign;

            MAKE_ABS_BRANCHLESS(v, sign);
            distY = d + v;
        } else {
            s32 v = part->speedY;
            s32 sign;

            MAKE_ABS_BRANCHLESS(v, sign);
            if (v >= gPlatformMoverMotionRecords[animSet->entries[1][0]].target)
                distY = 0;
        }
    }

    if (distX > rangeX && rangeX != 0) {
        dirX ^= 1;
        e = MoverRamp(this);
        if (dirX) {
            s32 start = e->start;
            s32 step = e->step;
            s32 target = e->target;

            part->rampX.start = start;
            part->rampX.step = step;
            part->rampX.target = target;
        } else {
            s32 start = -e->start;
            s32 target = -e->target;
            s32 step = e->step;

            part->rampX.start = start;
            part->rampX.step = step;
            part->rampX.target = target;
        }
        distX = -1;
    }
    if (distY > rangeY && rangeY != 0) {
        dirY ^= 1;
        e = MoverRamp(this);
        if (dirY) {
            s32 start = e->start;
            s32 step = e->step;
            s32 target = e->target;

            part->rampY.start = start;
            part->rampY.step = step;
            part->rampY.target = target;
        } else {
            s32 start = -e->start;
            s32 target = -e->target;
            s32 step = e->step;

            part->rampY.start = start;
            part->rampY.step = step;
            part->rampY.target = target;
        }
        distY = -1;
    }

    s32 k = kind;
    /* The frame count crosses the __umodsi3 call. Unpinned, old_agbcp's
     * global allocator ranks it above `part` and gives it r4, so `part`
     * and every temporary after it swap r4 and r5 (the C pinned `part`). */
    MATCH_HOLD_REG(u32, now, r5);

    if (k == 5 && timer > 0 && gRoomFrameCount - timer == 60) {
        StartTargetMotionYFromSet(part, 3);
        timer = -1;
    } else if (k == 5 && timer > 0 && ((now = gRoomFrameCount) - timer) % 30 <= 4) {
        now &= 1;
        if (now == 0)
            part->y += -0x300;
        else {
            s32 y = part->y;
            /* The 0x300 in r2 (reload picks r1 for a plain constant), as
             * the C had it. */
            MATCH_HOLD_REG(s32, up, r2) = 0x300;

            MATCH_KEEP(up);
            part->y = y + up;
        }
    } else if ((k == 7 && part->frame <= 1 && !active) ||
               (k == 6 && part->frame <= 1 && gRoomFrameCount < time)) {
        HoldFirstFrame(part);
    } else if (k == 7 && part->animDone) {
        part->MarkGone();
    } else if (k == 6 && part->animDone) {
        time = gRoomFrameCount + 120;
        HoldFirstFrame(part);
    }
    MovePlayer(part);
    lastX = Q8_TO_INT(part->x);
    lastY = Q8_TO_INT(part->y);
}

/* While active (the player stands on the platform) and the player
 * collides: moves the player by the platform's displacement since the
 * last frame, makes the platform the one the player stands on, and folds
 * the platform's velocity signs into the player's `dir`. A type 5
 * platform starts its timer. */
void PlatformMover::MovePlayer(MovingSprite *part)
{
    if (active && kind != 6) {
        Player *p = gPlayer;
        u32 f = p->f.flags;

        if (f >> 7) {
            u8 dir;

            p->carried = part;
            /* the 8 as a variable set here: the ROM forms hitAxes'
             * address from carried's (`subs r0, #0x44`) */
            {
                u8 m = 8;

                p->hitAxes = m;
            }
            {
                Player *q = gPlayer;
                s32 px = Q8_TO_INT(q->x);
                s32 dx = Q8_TO_INT(part->x) - lastX;
                s32 py = Q8_TO_INT(q->y);
                s32 dy = Q8_TO_INT(part->y) - lastY;

                px += dx;
                py += dy;
                q->SetPrevPos(q->x = INT_TO_Q8(px), q->y = INT_TO_Q8(py));
            }
            dir = gPlayer->dir;
            if (part->speedX > 0)
                dir |= 1;
            else if (part->speedX < 0)
                dir |= 2;
            if (part->speedY > 0)
                dir |= 8;
            else if (part->speedY < 0)
                dir |= 4;
            gPlayer->dir = dir;
            if (kind == 5 && timer == 0)
                timer = gRoomFrameCount;
        }
    }
}

void PlatformMover::SetTargetMotionYFromSet(MovingSprite *part, s32 index)
{
    const struct speed_ramp *e = &gPlatformMoverMotionRecords[animSet->entries[index][1]];

    if ((s32)(part->mirror << 26) < 0) {
        s32 start = -e->start;
        s32 target = -e->target;
        s32 step = e->step;

        part->rampY.start = start;
        part->rampY.step = step;
        part->rampY.target = target;
    } else {
        s32 start = e->start;
        s32 step = e->step;
        s32 target = e->target;

        part->rampY.start = start;
        part->rampY.step = step;
        part->rampY.target = target;
    }
}

void PlatformMover::SetTargetMotionXFromSet(MovingSprite *part, s32 index)
{
    const struct speed_ramp *e = &gPlatformMoverMotionRecords[animSet->entries[index][0]];

    if ((s32)(part->mirror << 27) < 0) {
        s32 start = -e->start;
        s32 target = -e->target;
        s32 step = e->step;

        part->rampX.start = start;
        part->rampX.step = step;
        part->rampX.target = target;
    } else {
        s32 start = e->start;
        s32 step = e->step;
        s32 target = e->target;

        part->rampX.start = start;
        part->rampX.step = step;
        part->rampX.target = target;
    }
}

void PlatformMover::StartTargetMotionYFromSet(MovingSprite *part, s32 index)
{
    Ctrl::StartTargetMotionY(part, &gPlatformMoverMotionRecords[animSet->entries[index][1]]);
}

void PlatformMover::StartTargetMotionXFromSet(MovingSprite *part, s32 index)
{
    Ctrl::StartTargetMotionX(part, &gPlatformMoverMotionRecords[animSet->entries[index][0]].start);
}

PlatformMover::~PlatformMover()
{
}

/* Moves `distX`/`distY` pixels each way (none for types 6 and 7), from
 * the side dirX/dirY give. */
PlatformMover::PlatformMover(s32 distX, s32 distY, bool dirX, bool dirY, s32 kind)
{
    if ((u32)(kind - 6) <= 1) {
        distY = 0;
        distX = 0;
    }
    lastX = 0;
    this->distX = distX;
    lastY = 0;
    this->distY = distY;
    animSet = &gPlatformMoverMotionSet;
    active = 0;
    this->kind = kind;
    timer = 0;
    rangeX = distX * 2;
    rangeY = distY * 2;
    this->dirX = dirX;
    this->dirY = dirY;
    time = gRoomFrameCount + 0x78;
}

void PlatformMover::ClearActive()
{
    active = 0;
}
