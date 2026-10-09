#include "action_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "key_input.hpp"

extern "C" {
#include "match.h"
#include "system.h"
#include "crates.h"
#include "gfx.h"
#include "level.h"
#include "sprite_bank.h"
#include "box_part.h"
#include "globals.h"
#include "math_util.h"
}

/* ActionCtrl's state methods for leaving the ground, dying, warping in
 * and hanging, the super body slam's shockwave and the tornado spin's
 * turns (include/action_ctrl.hpp; #664, docs/cplusplus.md). Built by
 * old_agbcp (the Makefile's OLD_AGBCC_OBJS), like the old_agbcc C it
 * replaces. */

/* The player's animation set to `tag`, restarted. */
static inline void SetTag(Player *part, s32 tag)
{
    part->tag = tag;
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
}

/* The state CheckLeftGround enters when the player walks off the ground.
 * Back on it (`hitAxes` bit 3): the run or turbo run resumes from its
 * animation (0xD, 0x18), the air spin goes on as the ground spin,
 * anything else goes idle. Still off it: A jumps (animation 0x13, Y
 * entry 7), B spins, R crouches down; with the D-pad idle the X motion
 * stops.
 *
 * `hit` is assigned in the test: initialised at its declaration, the
 * test reads a copy of it (an extra `adds`). */
void ActionCtrl::StateLeftGround()
{
    u8 hit;

    if ((hit = part->hitAxes & 8) != 0) {
        u8 tag;
        /* the ROM's r5 zero, reused by the air spin's trio below */
        u8 z;

        /* flags2 through a pointer to it, as in HandleAirInput:
         * `part->flags2 |= 1` and ActOrFlags0D allocate differently. The
         * 0 is stored through a pointer so it is loaded after the
         * address. */
        {
            u8 *flags2 = &part->f.bytes.flags2;

            *flags2 |= 1;
        }
        {
            u8 *blocked = &slamBlocked;

            z = 0;
            *blocked = z;
        }
        switch (tag = part->tag) {
        case 0xD:
        case 0x18:
            if (tag == 0xD) {
                if (turboRun) {
                    frame = 0;
                    SetTargetAnim(part, 0x18);
                    SetMode(ACTION_STATE_TURBO_RUN);
                    QueuePendingX(0, 0x1B);
                } else {
                    frame = 0;
                    SetMode(ACTION_STATE_RUN);
                    if (motionX != 1)
                        QueueX(0, 1, 1);
                }
                QueueNowY(0);
            } else if (tag == 0x18) {
                SetMode(ACTION_STATE_TURBO_RUN);
                motionYKeepSpeed = 0;
                motionYPending = 1;
                motionY = 0;
                turboRun = 1;
                QueuePendingX(0, 0x1B);
            }
            break;
        default:
            if (state == ACTION_STATE_AIR_SPIN) {
                if (gKeys.all & DPAD_SIDEWAYS) {
                    QueuePendingX(0, 1);
                } else {
                    motionXKeepSpeed = z;
                    motionXPending = 1;
                    motionX = z;
                }
                QueueNowY(0);
                SetMode(ACTION_STATE_SPIN);
            } else {
                SetMode(ACTION_STATE_IDLE);
                motionYKeepSpeed = z;
                motionYPending = 1;
                motionY = 0;
                motionXKeepSpeed = 0;
                motionXPending = 1;
                motionX = 0;
            }
        }
        return;
    }
    {
        u32 in = gKeys.all;
        u16 p = INPUT_PRESSED(in);
        s32 fire = p & 1;

        if (fire) {
            SetMode(ACTION_STATE_JUMP);
            SetTargetAnim(part, 0x13);
            frame = hit;
            QueueY(hit, 1, 7);
        } else {
            s32 t = 2;
            u16 alt;

            t &= p;
            alt = t;

            if (alt) {
                ActOrFlags0D(part, 1);
                slamBlocked = fire;
                if (gKeys.all & DPAD_SIDEWAYS)
                    QueueX(fire, 1, 1);
                else
                    QueueX(0, 1, 0);
                if ((u32)(state - ACTION_STATE_SPIN) > 1)
                    StartSpin();
                else
                    SetMode(ACTION_STATE_SPIN);
            } else if (INPUT_HELD(in) & R_BUTTON) {
                ActOrFlags0D(part, 1);
                slamBlocked = alt;
                SetMode(ACTION_STATE_CROUCH_DOWN);
                SetTargetAnim(part, 3);
                frames = alt;
                QueueX(alt, 1, alt);
            }
        }
    }
    {
        u8 dir = gInput->GetDpadDirection();

        if (dir == 0) {
            motionXKeepSpeed = dir;
            motionXPending = 1;
            motionX = dir;
        }
    }
}

/* Dying: the death animation 0x2F's sound on its frame 3; once the
 * animation is done, the player is marked gone. */
void ActionCtrl::StateDying()
{
    Player *p = part;

    if (p->tag == 0x2F && p->frame == 3 && p->stepTimer == 0)
        gAudioContext->PlaySfx(SFX_UNKNOWN_2E, 0x100);
    p = part;
    if (p->animDone)
        p->MarkGone();
}

/* Warping in: once the animation is done, the player's collision is
 * switched on, idle, and the player's palette reloaded. */
void ActionCtrl::StateWarpIn()
{
    if (part->animDone) {
        gPlayer->f.b.collides = 1;
        SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, 0);
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = 0;
        gPaletteCache->LoadSlot(part->palette, part->bank->anims[part->tag].paletteId);
    }
}

/* Hanging: the D-pad sideways starts moving along (animation 0x20, X
 * entry 0x20); A drops (ReleaseHang), B spins (StartHangSpin). Then the
 * facing. */
void ActionCtrl::StateHang()
{
    u8 dir = gInput->GetDpadDirection();
    u32 in = gKeys.all;

    if (dir != 0)
        switch (dir) {
        case 3 ... 8:
            QueueNowX(0x20);
            SetMode(ACTION_STATE_HANG_MOVE_START);
            SetTargetAnim(part, 0x20);
            break;
        }
    if (INPUT_PRESSED(in) & 1) {
        gAudioContext->PlaySfx(SFX_JUMP, 0x100);
        ReleaseHang();
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartHangSpin();
        UpdateFacing();
    } else {
        UpdateFacing();
    }
}

/* gActionCtrlStateTable's slot 0x22, a state nothing sets: StateHang
 * without the D-pad move (A drops, B starts the hang spin). */
void ActionCtrl::StateUnusedHang()
{
    u32 in = gKeys.all;
    s32 fire = INPUT_PRESSED(in) & 1;

    if (fire) {
        gAudioContext->PlaySfx(SFX_JUMP, 0x100);
        ReleaseHang();
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartHangSpin();
        UpdateFacing();
        motionXKeepSpeed = fire;
        motionXPending = 1;
        motionX = fire;
    }
}

/* Letting go: the player drops 6 px (0x600 in Q8) into the fall
 * (animation 0x1B, held on its last frame), Y entry 4.
 *
 * Kept from the C: reload loads the 0x600 into a spill register, and the
 * ROM's is r3. With r2 and r3 both free it takes r2 (the only difference
 * left in C++: the later reloads are the ROM's). `hold`, pinned to r2 and
 * never assigned, keeps r2 live from the start of the function to its
 * `MATCH_USE` after the add (an empty asm, no code). Round 2 of #662:
 * `part->y = part->y + 0x600`, a Player copy, `0x600u`, `-= -0x600` and
 * the 0x600 in a local all take r2. #662 round 3 (-dg dump): the 0x600 is
 * a reload of the add's constant operand, and reload hands out spill
 * registers round-robin: the reload of the `hanging` field's 0x101
 * offset just before takes r1, so the next one gets r2. The ROM's r3
 * means r2 was not free for it there (as the hold makes it); the flag
 * sweep (-fno-gcse ... -O1) changes nothing at this site. */
void ActionCtrl::ReleaseHang()
{
    Player *p;
    s32 count;
    MATCH_HOLD_REG(s32, hold, r2);

    part->hanging = 0;
    part->y += 0x600;
    MATCH_USE(hold); /* r2 held to here, so the 0x600 reload takes r3 */
    SetMode(ACTION_STATE_AIRBORNE_FALL);
    SetTargetAnim(part, 0x1B);
    p = part;
    count = p->bank->anims[p->tag].frameCount;
    p->frame = count - 1;
    QueueNowY(4);
}

/* Starting to move along while hanging: A drops, B spins; the D-pad idle
 * stops (animation 0x22); sideways queues X entry 0x20, and once the
 * start animation is done the move itself (animation 0x21 from frame
 * 5). Then the facing. */
void ActionCtrl::StateHangMoveStart()
{
    KeyInput *pad = gInput;
    u32 in = gKeys.all;
    s32 v = INPUT_PRESSED(in) & 1;

    if (v) {
        gAudioContext->PlaySfx(SFX_JUMP, 0x100);
        QueuePendingX(0, 0);
        ReleaseHang();
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartHangSpin();
        UpdateFacing();
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        return;
    }
    v = pad->GetDpadDirection();
    if (v == 0) {
        SetMode(ACTION_STATE_HANG_STOP);
        SetTargetAnim(part, 0x22);
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        return;
    }
    {
        u8 cur = motionX;

        if (cur == 0) {
            switch (v) {
            case 3 ... 8:
                QueuePendingX(cur, 0x20);
            }
        }
        if (part->animDone) {
            Player *p;
            s32 zero = 0;
            s32 frame;
            s32 count;

            SetMode(ACTION_STATE_HANG_MOVE);
            SetTargetAnim(part, 0x21);
            this->frame = zero;
            p = part;
            frame = 5;
            count = p->bank->anims[p->tag].frameCount;
            CLAMP_INDEX(frame, count);
            p->frame = frame;
            QueuePendingX(zero, 0x20);
        }
    }
    UpdateFacing();
}

/* Moving along while hanging (animation 0x21, restarted when done): A
 * drops, B spins. With the D-pad idle, every 4 frames the stop animation
 * (0x22 or 0x23, by the move's frame) unless mid-step; sideways keeps X
 * entry 0x20. While the facing reports a step, the player moves by the
 * frame's anchor X (mirrored). */
void ActionCtrl::StateHangMove()
{
    u8 dir = gInput->GetDpadDirection();
    u32 in = gKeys.all;
    Player *p = part;
    s32 fire;
    u16 alt;

    if (p->animDone)
        SetTag(p, 0x21);
    fire = INPUT_PRESSED(in) & 1;
    if (fire) {
        gAudioContext->PlaySfx(SFX_JUMP, 0x100);
        QueuePendingX(0, 0);
        ReleaseHang();
        return;
    }
    alt = INPUT_PRESSED(in) & 2;
    if (alt) {
        StartHangSpin();
        UpdateFacing();
        motionXKeepSpeed = fire;
        motionXPending = 1;
        motionX = fire;
        return;
    }
    if (dir == 0) {
        if (++frame > 3) {
            s32 f;

            frame = alt;
            f = part->frame;
            if (f == 0) {
                SetMode(ACTION_STATE_HANG_STOP);
                SetTargetAnim(part, 0x22);
                motionXKeepSpeed = alt;
                motionXPending = 1;
                motionX = alt;
            } else if (f <= 4) {
                SetMode(ACTION_STATE_HANG_STOP);
                SetTargetAnim(part, 0x23);
                motionXKeepSpeed = alt;
                motionXPending = 1;
                motionX = alt;
            } else if (f > 9) {
                SetMode(ACTION_STATE_HANG_STOP);
                SetTargetAnim(part, 0x22);
                motionXKeepSpeed = alt;
                motionXPending = 1;
                motionX = alt;
            } else {
                motionXKeepSpeed = alt;
                motionXPending = 1;
                motionX = alt;
            }
        }
    } else {
        frame = alt;
        QueuePendingX(alt, 0x20);
    }
    if ((u8)UpdateFacing()) {
        const struct sprite_frame *sf = part->GetFrame();
        const struct sprite_point *info;
        s32 x;
        s32 y;

        switch (sf->pieces[0] >> 4) {
        case 0:
            info = &((const struct sprite_frame_3box_anchor *)sf)->anchor;
            break;
        case 1:
            info = &gEmptySpritePoint;
            break;
        case 2:
            info = &gEmptySpritePoint;
            break;
        case 3:
            info = &gEmptySpritePoint;
            break;
        case 4:
            info = &gEmptySpritePoint;
            break;
        case 5:
            info = &gEmptySpritePoint;
            break;
        case 6:
            info = &((const struct sprite_frame_1box_anchor *)sf)->anchor;
            break;
        default:
            info = &gEmptySpritePoint;
            break;
        }
        x = Q8_TO_INT(part->x);
        y = part->y;
        if ((s8)(part->mirror << 3) < 0)
            x += info->x;
        else
            x -= info->x;
        part->x = INT_TO_Q8(x);
        part->y = y;
    }
}

/* Stopping while hanging: A drops, B spins; once the animation is done,
 * hanging (animation 0x1F). */
void ActionCtrl::StateHangStop()
{
    u32 in = gKeys.all;
    s32 fire = INPUT_PRESSED(in) & 1;
    u16 alt;

    if (fire) {
        gAudioContext->PlaySfx(SFX_JUMP, 0x100);
        QueueNowX(0);
        ReleaseHang();
        return;
    }
    alt = INPUT_PRESSED(in) & 2;
    if (alt) {
        StartHangSpin();
        UpdateFacing();
        motionXKeepSpeed = fire;
        motionXPending = 1;
        motionX = fire;
        return;
    }
    if (part->animDone) {
        SetMode(ACTION_STATE_HANG);
        SetTargetAnim(part, 0x1F);
        frame = alt;
        frames = alt;
    }
}

/* gCollidableList's length. As an inline function (a block of its own),
 * the loop's exit test isn't copied in front of the loop, so the loop
 * body reloads the list's address as the ROM does. */
static inline s32 CollidableCount()
{
    return gCollidableList->count;
}

/* The super body slam's landing: breaks the crates within 0x40 px
 * (BreakCratesInArea), then sends event 0x16 (EVENT_ATTACK_SUPER_BODY_SLAM)
 * to each object of gCollidableList with a class id above 4, within 0x40
 * px (Manhattan distance) and 0x11 px vertically, that can be hit
 * (`flags` bit 6). */
void ActionCtrl::DoSuperBodySlamShockwave()
{
    Player *p = part;
    s32 range;
    s32 px;
    s32 py;
    s32 i;

    BreakCratesInArea(Q8_TO_INT(p->x), Q8_TO_INT(p->y), 0x40, 0x12);
    range = 0x40;
    p = part;
    px = Q8_TO_INT(p->x);
    py = Q8_TO_INT(p->y);
    i = 0;
    while (i < CollidableCount()) {
        MovingSprite *other = (MovingSprite *)gCollidableList->items[i];
        s32 dx;
        s32 dy;

        if (other->GetClassId() > 4) {
            dx = ABS_BRANCHLESS(Q8_TO_INT(other->x) - px);
            dy = ABS_BRANCHLESS(Q8_TO_INT(other->y) - py);
            if (dx + dy <= range && ((other->f.flags >> 6) & 1) != 0 && dy <= 0x11)
                other->HandleEvent(0, EVENT_ATTACK_SUPER_BODY_SLAM, 0);
        }
        i++;
    }
}

/* The tornado spin's next turn (from the ground, air and tornado spins).
 * Winding up (`tornadoUnwinding` clear): enters state `id` with the turn's
 * animation (0x17, 0x28, 0x27 for turns 0-2) for 0x14 frames and its
 * sound; once `tornadoTurn` reaches `charge`, the turns count back down.
 * Unwinding: the same, counting down, until the count wraps; then state
 * `param2` with the spin animation (0x10, 0x18 frames), and the spin
 * cooldown at 0x63. */
void ActionCtrl::StartTornadoSpin(s32 id, s32 param2)
{
    if (tornadoUnwinding == 0) {
        s32 idx = 0x17;
        s32 zero;
        s32 wait;

        tornadoVariant = 0;
        if (tornadoTurn == 1) {
            idx = 0x28;
            tornadoVariant = 1;
        } else if (tornadoTurn == 2) {
            idx = 0x27;
            tornadoVariant = 2;
        }
        zero = 0;
        wait = 0x14;
        SetMode(id);
        SetTargetAnim(part, idx);
        frame = zero;
        frames = wait;
        gAudioContext->PlaySfx(tornadoVariant + SFX_TORNADO_SPIN, 0x100);
        if (++tornadoTurn >= charge) {
            tornadoUnwinding = 1;
            if (tornadoTurn > 1)
                tornadoTurn = 1;
            else
                tornadoTurn = zero;
        }
    } else {
        if (tornadoTurn > 0xf0) {
            s32 idx;
            s32 zero;
            s32 wait;

            idx = 0x17;
            tornadoVariant = 0;
            if (tornadoTurn == 1) {
                idx = 0x28;
                tornadoVariant = 1;
            } else if (tornadoTurn == 2) {
                idx = 0x27;
                tornadoVariant = 2;
            }
            zero = 0;
            wait = 0x14;
            SetMode(id);
            SetTargetAnim(part, idx);
            frame = zero;
            frames = wait;
            gAudioContext->PlaySfx(tornadoVariant + SFX_TORNADO_SPIN, 0x100);
        } else {
            u8 *p21 = &tornadoVariant;
            s32 zero = 0;
            s32 wait;

            *p21 = zero;
            charge = zero;
            wait = 0x18;
            SetMode(param2);
            SetTargetAnim(part, 0x10);
            frame = zero;
            frames = wait;
            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            spinCooldown = 0x63;
        }
        tornadoTurn--;
    }
    tornadoFallQueued = 0;
}

/* ActionCtrl's moves (include/action_ctrl.hpp; #664, docs/cplusplus.md):
 * the tornado fall, the end and steering of a spin, SetMode, the starts
 * of the spin, hang spin, run, high jump and mask-hit jump, Attach, and
 * the short state methods (hang grab, hang spin, warp out, crawl stop,
 * body slam start). Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS),
 * like the old_agbcc C it replaces. */

/* The tornado spin's slow descent, once per spin (`tornadoFallQueued`):
 * called once the spinning player leaves the ground (StateTornadoSpin)
 * or, in the air spin, starts falling (HandleAirInput). Queues Y motion
 * entry 0x18/0x19/0x1A by `tornadoTurn` (0-1, 2, 3-4; nothing above 4).
 * Those entries are gCtrlMotionRecords 29-31, {8, 18/14/6, 1280}: the
 * more turns, the slower the fall speeds up. Then sets the player's
 * `flags2` bit 0 and clears `slamBlocked`. */
void ActionCtrl::StartTornadoFall()
{
    if (tornadoFallQueued != 0)
        return;
    tornadoFallQueued = 1;
    {
        /* Pinned (the C had seven pins and a hand-written jump table):
         * unpinned, `this` and `entry` swap r2 and r3. Global-alloc
         * takes `this` first (priority 8 references over 38 insns
         * against entry's 4 over 26), and `this` conflicts only with
         * r0/r1, so it gets r2 (#662 round 3, from the -dg dump; u8 and
         * s32 entries, QueueNowY and a local copy of the turn count all
         * allocate the same). */
        MATCH_HOLD_REG(s32, entry, r2);

        switch (tornadoTurn) {
        case 0:
        case 1:
            entry = 0x18;
            break;
        case 2:
            entry = 0x19;
            break;
        case 3:
        case 4:
            entry = 0x1A;
            break;
        default:
            goto queued;
        }
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = entry;
    }
queued:
    /* Through ActOrFlags0D's pointer: as a member `|=` the expansion's
     * dead `& 0` leaves a 0 that cse reuses for slamBlocked's store,
     * loaded before the `ldrb` (an r0 pin on the 0 before; #662 round
     * 3). */
    ActOrFlags0D(part, 1);
    slamBlocked = 0;
}

/* The end of a ground spin: the spin cooldown starts (12 frames). With
 * the D-pad sideways (`mode` 3/4) the run restarts: the turbo run if L
 * is held (`flags` is the input word) and HasTurboRun allows it,
 * otherwise StartRun. Any other direction goes back to idle. */
void ActionCtrl::EndSpin(u8 mode, s32 flags)
{
    spinCooldown = 0xC;
    switch (mode) {
    case 3:
    case 4:
        {
            s32 m = L_BUTTON;
            s32 m2;

            /* The ROM builds 0x200 in r1 and ANDs through a copy in r0,
             * into flags' own r2. Kept from the C: the MATCH_CONST escape
             * keeps the copy (m2) apart from m, and the volatile use of m
             * and flags right after the `and` stops combine from sinking
             * it into the test and regmove from retargeting it onto m2.
             * The natural `flags & L_BUTTON` ANDs into the constant's
             * register instead: both inputs die there and local-alloc
             * ties the output to the constant's (block-local) pseudo,
             * not to `flags` (live from the entry). #662 round 3: with
             * `s32 m = L_BUTTON;` declared at the top of the function,
             * `flags &= m` keeps the result in r2, but the constant is
             * then built in r0 directly; the ROM's `adds r0, r1, #0`
             * means two pseudos for it, the first still live after the
             * AND, which no spelling tried (inline mask helpers, u32
             * types, the mask as the AND's target) gives. */
            MATCH_CONST(m2, m);
            flags &= m2;
            asm volatile("" : "+r"(flags) : "r"(m));
        }
        if (flags != 0 && (u8)gLevelState->HasTurboRun()) {
            turboRun = 1;
            SetMode(ACTION_STATE_TURBO_RUN);
            SetTargetAnim(part, 0x18);
            QueueNowX(0x1B);
        } else {
            StartRun();
        }
        break;
    default:
        SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, 0);
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = 0;
        break;
    }
}

/* Steering during a spin: with no X motion queued and no crate bump, the
 * D-pad sideways (`mode` 3/4) queues X entry 0x17; `mode` 0-2 queues
 * entry 0. Then the facing (UpdateFacing). */
void ActionCtrl::SteerSpin(u8 mode)
{
    if (motionX == 0 && bumpTimer == 0) {
        switch (mode) {
        case 3:
        case 4:
            QueueNowX(0x17);
            break;
        }
    }
    if (mode <= 2)
        QueueNowX(0);
    UpdateFacing();
}

/* Enters state `mode`, keeping the old one in `prevState`, and cancels a
 * crate bump. Unless the new state is a spin (0xD, 0xE), the player's
 * `bumped`, `bounce` and `listCount` are cleared (the last one twice). */
void ActionCtrl::SetMode(s32 mode)
{
    idleFidget = 0;
    prevState = state;
    state = mode;
    bumpedMotionX = 0;
    bumpTimer = 0;
    if ((u32)(mode - ACTION_STATE_SPIN) > 1) {
        part->bumped = 0;
        gPlayer->bounce = 0;
        gPlayer->listCount = 0;
        gPlayer->listCount = 0;
    }
}

/* Starts a ground spin unless the cooldown runs: animation 0x10 for 0x18
 * frames, the tornado turns reset. */
void ActionCtrl::StartSpin()
{
    if (spinCooldown == 0) {
        gAudioContext->PlaySfx(SFX_SPIN, 0x100);
        frame = 0;
        frames = 0x18;
        SetTargetAnim(part, 0x10);
        SetMode(ACTION_STATE_SPIN);
        tornadoVariant = 0;
        charge = 0;
        tornadoTurn = 0;
        tornadoFallQueued = 0;
        tornadoUnwinding = 0;
    }
}

/* StartSpin while hanging: animation 0x1E, state 0x21. */
void ActionCtrl::StartHangSpin()
{
    if (spinCooldown == 0) {
        gAudioContext->PlaySfx(SFX_SPIN, 0x100);
        frame = 0;
        frames = 0x18;
        tornadoVariant = 0;
        charge = 0;
        tornadoTurn = 0;
        tornadoFallQueued = 0;
        tornadoUnwinding = 0;
        SetTargetAnim(part, 0x1E);
        SetMode(ACTION_STATE_HANG_SPIN);
    }
}

/* Starts the run: the turbo run (animation 0x18, X entry 0x1B) if
 * `turboRun` is set, else the plain run (animation 0xD, X entry 1). On
 * slippery ground the speed is kept. */
void ActionCtrl::StartRun()
{
    if (turboRun != 0) {
        frame = 0;
        SetTargetAnim(part, 0x18);
        SetMode(ACTION_STATE_TURBO_RUN);
        QueueNowX(0x1B);
        if (part->slippery != 0)
            motionXKeepSpeed = 1;
    } else {
        SetTargetAnim(part, 0xD);
        frame = 0;
        SetMode(ACTION_STATE_RUN);
        QueueNowX(1);
        if (part->slippery != 0)
            motionXKeepSpeed = 1;
    }
}

/* The high jump: state 0xB, animation 0xB, Y entry 0xB. */
void ActionCtrl::StartHighJump()
{
    SetModeAnimNow(ACTION_STATE_AIRBORNE_HIGH_JUMP, 0xB, 0);
    QueueNowY(0xB);
    part->hitAxes = 0;
}

/* StartHighJump with Y entry 7 (the plain jump the A button queues in
 * StateRun) instead of 0xB. The hop Crash makes when the Aku Aku mask
 * absorbs a hit: HandleEvent's event 11 calls it, and the only sender of
 * event 11 is PlayerHandleEvent's hit cases (1-10), right after they drop
 * the mask level by one (mask level 1 or 2). */
void ActionCtrl::StartMaskHitJump()
{
    SetModeAnimNow(ACTION_STATE_AIRBORNE_HIGH_JUMP, 0xB, 0);
    QueueNowY(7);
    part->hitAxes = 0;
}

void ActionCtrl::Attach(MovingSprite *owner)
{
    part = (Player *)owner;
}

/* gActionCtrlStateTable's slot 0x27, a state nothing sets: runs
 * ReleaseHang. */
void ActionCtrl::StateUnusedHangRelease()
{
    ReleaseHang();
}

/* gActionCtrlStateTable's slot 0x23, a state nothing sets; the same code
 * as StateHangGrab (slot 0x1F). */
void ActionCtrl::StateUnusedHangGrab()
{
    if (part->animDone != 0) {
        SetModeAnimNow(ACTION_STATE_HANG, 0x1F, 0, 0);
    }
}

/* The spin while hanging: after `frames` frames, or once the animation is
 * done, back to hanging (animation 0x1F) with the spin cooldown started.
 * Then the facing. */
void ActionCtrl::StateHangSpin()
{
    frame += 1;
    if (frame >= frames || part->animDone != 0) {
        spinCooldown = 0xC;
        SetMode(ACTION_STATE_HANG);
        SetTargetAnim(part, 0x1F);
        frame = 0;
        frames = 0;
    }
    UpdateFacing();
}

/* Grabbing a ledge: once the grab animation is done, hanging (animation
 * 0x1F). */
void ActionCtrl::StateHangGrab()
{
    if (part->animDone != 0) {
        SetModeAnimNow(ACTION_STATE_HANG, 0x1F, 0, 0);
    }
}

/* Warping out: once the animation is done, the player's collision is
 * switched on and the room ends. */
void ActionCtrl::StateWarpOut()
{
    if (part->animDone != 0) {
        gPlayer->f.flags |= 0x80;
        RequestRoomExit();
    }
}

/* Once the animation is done: crouching (animation 4). */
void ActionCtrl::StateCrawlStop()
{
    if (part->animDone != 0) {
        SetMode(ACTION_STATE_CROUCH);
        SetTargetAnim(part, 4);
    }
}

/* Once the start animation is done: the super body slam (animation 7)
 * if HasSuperBodySlam allows it, else the body slam. */
void ActionCtrl::StateBodySlamStart()
{
    if (part->animDone != 0) {
        if ((u8)gLevelState->HasSuperBodySlam()) {
            SetMode(ACTION_STATE_AIRBORNE_SUPER_BODY_SLAM);
            SetTargetAnim(part, 7);
        } else {
            SetMode(ACTION_STATE_AIRBORNE_BODY_SLAM);
        }
    }
}
