#include "action_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* ActionCtrl's KillPlayer, skid animation and facing (include/
 * action_ctrl.hpp; #664, docs/cplusplus.md). Built by old_agbcp (the
 * Makefile's OLD_AGBCC_OBJS): KillPlayer has old_agbcc's
 * constant-before-`ldrb`, which the C, built by agbcc, got with 44 pins
 * and an asm. */

/* UpdateFacing's two turns. Each takes the constant the ROM materializes
 * first as a parameter (the turbo run's 0, the pending X motion's 1): an
 * inline parameter is computed before the body, as there; the right turn
 * keeps the mask in SImode (`& -0x11`, then `| 0x10`). */
static inline void FaceLeft(ActionCtrl *ctrl, Player *p, s32 turboRun)
{
    p->mirrorFlags.mirrorX = 0;
    ctrl->motionXPending = 1;
    ctrl->turboRun = turboRun;
}

static inline void FaceRight(ActionCtrl *ctrl, Player *p, s32 pending)
{
    u8 *mirror = &p->mirror;
    s32 m = ~0x10;

    m &= *mirror;
    m |= 0x10;
    *mirror = m;
    ctrl->motionXPending = pending;
    ctrl->turboRun = 0;
}

/* The player is hit: plays the hurt sound, switches to the dying state
 * on death animation `anim` (0x1C, 0x2A-0x2F from HandleEvent), cancels
 * both motion entries, makes the player intangible and dead, takes a life
 * and reloads the player's palette. */
void ActionCtrl::KillPlayer(s32 anim)
{
    PlaySfx(gAudioContext, SFX_PLAYER_HURT, 0x100);
    SetTargetAnim(part, anim);
    SetMode(ACTION_STATE_DYING);
    QueueNowX(0);
    QueueNowY(0);
    ApplyMotion();
    part->StoreSlippery(0);
    part->pushLeft = 0;
    part->pushRight = 0;
    part->f.b.collides = 0;
    part->f.b.vulnerable = 0;
    part->dead = 1;
    LoseLife(gLevelState);
    gPaletteCache->LoadSlot(part->palette, part->bank->anims[part->tag].paletteId);
}

/* Update calls this when the player's `slippery` changed. On slippery
 * ground the idle animation (0x12, while still moving) becomes the skid
 * 0x25, the run and turbo run animations (0xD, 0x18) the skid 0x26 (as
 * SetTargetAnim does), restarted; off it, a skid animation stops the skid
 * sound and goes back to idle. */
void ActionCtrl::UpdateSkidAnim()
{
    Player *player = gPlayer;
    s32 slippery = player->slippery;

    if (slippery) {
        u8 *tagp = &player->tag;
        s32 tag = *tagp;
        /* The ROM tests 0x18 on a copy of the animation, in r2; a switch
         * or an if chain tests all four on the loaded register. */
        MATCH_HOLD_REG(s32, tagCopy, r2) = tag;

        if (tag == 0x12)
            goto idle;
        if (tag > 0x12)
            goto above;
        if (tag == 0xD)
            goto run;
        return;
    above:
        if (tagCopy == 0x18)
            goto run;
        return;
    idle:
        if (player->speedX == 0)
            return;
        *tagp = 0x25;
        goto restart;
    run:
        player = gPlayer;
        tag = 0x26;
        player->tag = tag;
    restart:
        player->ResetFrameTimer();
        player->ResetFrameIndex();
        player->SetAnimDone(0);
    } else {
        s32 tag = player->tag;

        /* Two tests: `tag == 0x25 || tag == 0x26` folds into one range
         * check. */
        if (tag == 0x25)
            goto unskid;
        if (tag == 0x26) {
        unskid:
            StopSfx(gAudioContext, SFX_SKID);
            SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, slippery);
        }
    }
}

/* Turns the player to face the D-pad's direction (directions 3/5/7 face
 * right, 4/6/8 left) in the states that steer; turning queues the X
 * motion again and ends the turbo run. Also clears the player's `mirror`
 * bit 5 (Y mirrored) in those states. Returns whether he turned. */
s32 ActionCtrl::UpdateFacing()
{
    s32 dir = GetDpadDirection(gInput);
    s32 turned = 0;

    switch (state) {
    case ACTION_STATE_IDLE:
    case ACTION_STATE_RUN:
    case ACTION_STATE_TURBO_RUN:
    case ACTION_STATE_JUMP:
    case ACTION_STATE_AIRBORNE_JUMP:
    case ACTION_STATE_AIRBORNE_FLIP_JUMP:
    case ACTION_STATE_AIRBORNE_HIGH_JUMP:
    case ACTION_STATE_SPIN:
    case ACTION_STATE_AIR_SPIN:
    case ACTION_STATE_TORNADO_SPIN:
    case ACTION_STATE_CRAWL:
    case ACTION_STATE_AIRBORNE_FALL:
    case ACTION_STATE_HANG:
    case ACTION_STATE_HANG_SPIN:
    case ACTION_STATE_HANG_MOVE_START:
    case ACTION_STATE_HANG_MOVE:
        break;
    default:
        goto done;
    }
    part->mirrorFlags.mirrorY = 0;
    if (part->mirrorBits.flipX < 0 && (dir == 4 || dir == 6 || dir == 8)) {
        FaceLeft(this, part, 0);
    } else if ((s32)(part->mirror << 27) >= 0 && (dir == 3 || dir == 5 || dir == 7)) {
        FaceRight(this, part, 1);
    } else {
        goto done;
    }
    turned = 1;
done:
    return turned;
}
