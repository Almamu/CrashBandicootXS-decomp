#include "core.h"

/* Part of GitHub issue #16's remainder (0x08011BD4-0x08012D24) - see
 * kill_player.c's top-of-file comment for the shared field-offset
 * conventions (`self+0xc`/`self+0x10`/`+0x27`..`+0x32`) this "child
 * object" family uses. Not ROM-adjacent to wumpa.c's matched
 * span before it or kill_player.c's after it (see
 * docs/matching/issue-16-actor-11b0c.md/issue-16-actor-12160.md), so a
 * new file.
 *
 * Built with old_agbcc (the file is on `OLD_AGBCC_OBJS`): under the
 * current agbcc the same C is 36 halfwords off. */

#include "action_obj.h"
#include "audio.h"
#include "player.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* A keyframe record's `{s16 x, s16 y}` offset (see sprite.c). */
struct part_offset {
    s16 x;
    s16 y;
};

extern struct act_part *gPlayer;
extern u8 gEmptySpritePoint[];

/* Queues action `next` on the +0x31/+0x2F/+0x27 trio. */
static inline void ActSetNext27(struct act *self, s32 next)
{
    self->motionXKeepSpeed = 0;
    self->motionXPending = 1;
    self->motionX = next;
}

/* GetSpriteFrameAnchor inlined: points `dst` at the part's current keyframe
 * offset record. A macro so each case assigns `dst` itself. */
#define PART_OFFSET(dst, part)                                                 \
    if (1) {                                                                   \
        u8 *_info = GetSpriteFrame((struct gfx_part *)(part));                    \
                                                                               \
        switch (**(u8 **)(_info + 4) >> 4) {                                   \
        case 0:                                                                \
            (dst) = (struct part_offset *)(_info + 0x24);                      \
            break;                                                             \
        case 1:                                                                \
            (dst) = (struct part_offset *)gEmptySpritePoint;                \
            break;                                                             \
        case 2:                                                                \
            (dst) = (struct part_offset *)gEmptySpritePoint;                \
            break;                                                             \
        case 3:                                                                \
            (dst) = (struct part_offset *)gEmptySpritePoint;                \
            break;                                                             \
        case 4:                                                                \
            (dst) = (struct part_offset *)gEmptySpritePoint;                \
            break;                                                             \
        case 5:                                                                \
            (dst) = (struct part_offset *)gEmptySpritePoint;                \
            break;                                                             \
        case 6:                                                                \
            (dst) = (struct part_offset *)(_info + 0x14);                      \
            break;                                                             \
        default:                                                               \
            (dst) = (struct part_offset *)gEmptySpritePoint;                \
            break;                                                             \
        }                                                                      \
    } else (void)0

static inline void PartSetHanging(struct act_part *p, s32 v)
{
    p->hanging = v;
}

static inline void PartSetBumped(struct act_part *p, s32 v)
{
    p->bumped = v;
}

static inline void ActTrio28(struct act *self, s32 a, s32 b, s32 c)
{
    self->motionYKeepSpeed = a;
    self->motionYPending = b;
    self->motionY = c;
}

static inline void PartSetVelY(struct act_part *p, s32 a, s32 b, s32 c)
{
    p->speedY = a;
    p->rampYStart = a;
    p->rampYStep = b;
    p->rampYTarget = c;
}

/* docs/rom_map.md's "25-case jump table on a second parameter, with a
 * further 7-case sub-dispatch on a nibble of a child object's `+4`
 * byte": the companion of `UpdatePlayerCtrl`. Does nothing for an object in
 * state 0x1D; otherwise dispatches on `arg2` (1-25):
 * - 2/3/7/8/9/10 call `KillPlayer` with a fixed id; 1/4/6 do the same
 *   with 0x1C and also reset the part's velocities and
 *   `gCamera->mode`;
 * - 11 calls `sub_8015558` once the player is in contact and
 *   `PlayerHasRoomForAnim(part, 0xB)` reports 1;
 * - 12 applies the contact bits `arg3` (and, for `arg3 & 3` == 1/2,
 *   the player's X mirror decides whether the queued action moves to
 *   +0x2C); in state 0xC with the player mid-keyframe it rewinds the
 *   player's step counter instead;
 * - 13/14/25 queue the next action from the fire button
 *   (`gKeys` bit 0); 15-17 play a sound and re-select the
 *   part's palette; 23/24 queue actions 0x1D/0x1B, 23 also moving the
 *   part by the change in its keyframe Y offset (`PART_OFFSET`).
 * 5 and 18-22 do nothing.
 *
 * What the match needed (docs/matching/big-naked-retry-2.md):
 * - the case bodies in the ROM's block order;
 * - `PART_OFFSET` as a macro that assigns the destination in each case
 *   (an inline's return value was copied into it);
 * - in 13/14/25, the `1` the trio stores is its own `asm`-initialised
 *   local (so it isn't shared with the `& 1` fire test), and 13/14 read
 *   the input through a second local (`held`), which gives the ROM's
 *   register copy;
 * - in 12, a volatile read of `self->state` where the ROM has a dead
 *   `ldr` of it; without it the whole case is laid out differently;
 * - the `hanging`/`bumped` stores through inline setters (the value is
 *   materialized before the offset), the part's Y read into a local
 *   before the subtraction, and 1/4/6's zero as a local assigned
 *   before the `slippery` test. */
void ActionCtrlHandleEvent(struct act *self, s32 arg1, s32 arg2, s32 arg3)
{
    if (self->state == 0x1d)
        return;
    switch (arg2) {
    case 23:
        {
            struct part_offset *from;
            struct part_offset *to;

            PART_OFFSET(from, self->part);

            PartSetHanging(self->part, 1);
            SetActionCtrlModeAnim(self, 0x1f, 0x1d, 0, 0);
            ActSetNext27(self, 0);
            ActSetNext(self, 0);
            PART_OFFSET(to, self->part);
            {
                s32 d = to->y - from->y;
                s32 y = self->part->y;

                self->part->y = y - (d << 8);
            }
        }
        break;
    case 24:
        self->part->hanging = 0;
        SetActionCtrlModeAnim(self, 0x1a, 0x1b, 0x7FFFFFFF, 0x7FFFFFFF);
        ActSetNext(self, 4);
        break;
    case 12:
        if (self->state == 0) {
            ActSetNext27(self, 0);
            self->bumpedMotionX = 0;
            self->part->contact |= arg3;
            self->part->speedX = 0;
            break;
        }
        {
            s32 m = arg3 & 3;

            if (m == 2) {
                if (self->motionX != 0 && (s8)(gPlayer->flags28 << 3) < 0) {
                    self->bumpedMotionX = self->motionX;
                    ActSetNext27(self, 0);
                    self->part->speedX = 0;
                }
            } else if (m == 1) {
                if (self->motionX != 0 && !((u32)(gPlayer->flags28 << 27) >> 31)) {
                    self->bumpedMotionX = self->motionX;
                    self->motionXKeepSpeed = 0;
                    self->motionXPending = m;
                    self->motionX = 0;
                    self->part->speedX = 0;
                }
            } else {
                goto check_c;
            }
            /* The ROM has a dead load of the state here. */
            *(vs32 *)&self->state;
            self->bumpTimer = 3;
            PartSetBumped(self->part, 1);
        }
    check_c:
        if (self->state == 0xc && gPlayer->frame != 0) {
            struct act_part *pl = gPlayer;

            self->frame = self->frames;
            self->bumpTimer = 0;
            pl->frame = pl->bank->records[pl->tag].frameCount - 1;
            break;
        }
        self->part->contact |= arg3;
        self->part->speedX = 0;
        break;
    case 13:
        {
            u32 in = gKeys.all;
            u32 held = in;
            s32 fire;
            s32 one;

            if (held & 0x100)
                self->slamBlocked = 1;
            /* one = 1, kept apart from the fire test's 1 (see above). */
            asm("" : "=r"(one) : "0"(1));
            fire = held & 1;
            if (fire) {
                SetActionCtrlModeAnim(self, 5, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                self->part->speedY = 0;
                ActTrio28(self, 0, one, 0x10);
            } else {
                SetActionCtrlModeAnim(self, 5, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                self->part->speedY = fire;
                ActTrio28(self, 0, one, 0xf);
            }
        }
        self->frame = 0;
        break;
    case 14:
        {
            u32 in = gKeys.all;
            u32 held = in;
            s32 fire;
            s32 one;

            if (held & 0x100)
                self->slamBlocked = 1;
            /* one = 1, kept apart from the fire test's 1 (see above). */
            asm("" : "=r"(one) : "0"(1));
            fire = held & 1;
            if (fire) {
                SetActionCtrlModeAnim(self, 5, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                self->part->speedY = 0;
                ActTrio28(self, 0, one, 0x12);
            } else {
                SetActionCtrlModeAnim(self, 5, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                self->part->speedY = fire;
                ActTrio28(self, 0, one, 0x11);
            }
        }
        self->frame = 0;
        break;
    case 25:
        {
            u32 in = gKeys.all;
            s32 fire;
            s32 one;

            PlaySfx(gAudioContext, 0xa, 0x100);
            /* one = 1, kept apart from the fire test's 1 (see above). */
            asm("" : "=r"(one) : "0"(1));
            fire = in & 1;
            if (fire) {
                SetActionCtrlModeAnim(self, 0xe, 0x10, 0, 0x18);
                self->unk_22 = 0;
                self->unk_23 = 0;
                self->unk_24[0] = 0;
                self->charge = 3;
                self->part->speedY = 0;
                ActTrio28(self, 0, one, 0x14);
            } else {
                SetActionCtrlModeAnim(self, 0xe, 0x10, 0, 0x18);
                self->unk_22 = fire;
                self->unk_23 = fire;
                self->unk_24[0] = fire;
                self->charge = 3;
                self->part->speedY = fire;
                ActTrio28(self, 0, one, 0x13);
            }
            ApplyActionCtrlMotion(self);
        }
        self->frame = 0;
        break;
    case 15:
        FadeOutMusic(gAudioContext, 0);
        /* fallthrough */
    case 16:
    case 17:
        PlaySfx(gAudioContext, 0x2c, 0x100);
        {
            u8 *f = &gPlayer->flags0C;

            *f &= 0x7f;
        }
        SetActionCtrlModeAnim(self, 0x1e, 0x24, 0x7FFFFFFF, 0x7FFFFFFF);
        LoadPaletteSlot(gPaletteCache, self->part->slotNibble,
                    self->part->bank->records[self->part->tag].paletteId);
        ActSetNext27(self, 0);
        ActSetNext(self, 0);
        break;
    case 2:
        KillPlayer(self, 0x2e);
        break;
    case 3:
        KillPlayer(self, 0x2c);
        break;
    case 7:
        KillPlayer(self, 0x2b);
        break;
    case 8:
        KillPlayer(self, 0x2f);
        break;
    case 9:
        KillPlayer(self, 0x2d);
        break;
    case 1:
    case 4:
    case 6:
        {
            struct act_part *p;
            s32 z;

            KillPlayer(self, 0x1c);
            p = self->part;
            z = 0;
            if (p->slippery == 0)
                p->speedX = z;
            p->rampXStart = z;
            p->rampXStep = z;
            p->rampXTarget = z;
            PartSetVelY(self->part, -0x100, 0, -0x100);
            gCamera->mode = 3;
        }
        break;
    case 10:
        KillPlayer(self, 0x2a);
        break;
    case 11:
        if ((gPlayer->contact & 8) && PlayerHasRoomForAnim((struct box_part *)self->part, 0xb) == 1) {
            ActAndFlags0D(self->part, -2);
            ActAndFlags0D(self->part, -3);
            sub_8015558(self);
        }
        break;
    }
}
