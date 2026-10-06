#include "core.h"
#include "match.h"
#include "actor_self.h"
#include "audio.h"
#include "actor.h"
#include "bosses.h"
#include "globals.h"

/* Same large per-instance "self" object as polar_player_actions.c/polar_pickups.c
 * (`struct actor_self`: state, anim index/timer/done flag, state timer,
 * anim accumulator, anim table pointer and method table), part of a
 * boss-weapon effect state machine - see
 * docs/matching/archive/issue-58-0x08030334-actor.md and docs/status/actor.md. */

struct actor_timed {
    struct actor_self base;
    s32 timer;          // 0x54 - countdown until the transition fires
};

/* Countdown `timer`: once it reaches zero, plays a sound, sets the
 * "table-index 4" tag at `palette`, and fires the state-2/anim-1
 * transition (ACTOR_SET_STATE's stores, with the ROM's registers
 * pinned). The `animTimer`/`animDone` stores go through a cast of the
 * field's address: a plain member store is marked as a struct access,
 * which lets the scheduler move the zero into a different register. */
void DamageAirshipFireball(void *selfArg, s32 delta)
{
    struct actor_timed *self = selfArg;

    self->timer -= delta;
    if (self->timer <= 0) {
        self->base.palette = 4;
        PlaySfx(gAudioContext, 4, 0x100);
        {
            MATCH_HOLD_REG(s32, stateVal, r0) = 2;
            MATCH_HOLD_REG(s32, idxVal, r1) = 1;

            self->base.state = stateVal;
            {
                MATCH_HOLD_REG(s32, zero, r2) = 0;

                self->base.stateTime = zero;
                self->base.animIndex = idxVal;
                {
                    MATCH_HOLD_REG(u16, anim, r0) = self->base.anims[1].duration;
                    MATCH_HOLD_REG(u8, zero2, r1) = 0;

                    *(u16 *)&self->base.animTimer = anim;
                    *(u8 *)&self->base.animDone = zero2;
                }
                self->base.animTime = zero;
            }
        }
    }
}

/* Same large per-instance "self" object family as above - see
 * this file's header comment and docs/matching/archive/issue-58-0x08030334-actor.md. */

/* Per-state member-pointer dispatch, `(this->*gAirshipFireballStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`), then either the "destroy"
 * virtual call once the state-2 animation has played through, or the
 * standard UpdateActor step. */
void UpdateAirshipFireball(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gAirshipFireballStateFuncs);

    if (self->state == 2 && self->animDone != 0) {
        if (self != NULL) {
            ACTOR_VCALL(self, destroy, 3);
        }
    } else {
        UpdateActor(self);
    }
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");

/* Same large per-instance "self" object family as above - see this file's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md. */

/* An `InitActorPart`-based constructor: forwards all 4 of its own real
 * arguments (the last stack-passed) straight to `InitActorPart`, then
 * marks `self+0x54 = 2`, sets `self+0x50`'s event/trampoline table to
 * `gAirshipFireballVtable`, and stashes its own `b`/`c` arguments a
 * second time into `self+0x58`/`self+0x5c`, `self+0x64 = 0`,
 * `self+0x60 = 0x95`, `self+0x68 (byte) = 0`. Returns `self` - the same
 * shape as the already-matched `CreateHovercraftCannon` (hovercraft_cannon.c) and the
 * still-parked `CreateJetpackShot` (jetpack_shot.c), except this one's `d`
 * argument is itself stack-passed (a 5th real argument total) rather
 * than the 4th register argument. Pinning `d` to `r0` *after* the other
 * register pins (rather than alongside them) is what gets this
 * compiler to fetch the stack argument in the same position the ROM's
 * own build does - declaring it earlier reorders the fetch ahead of the
 * `r6`/`r8` parameter homes, which is the "4-instruction scheduling
 * permutation" this function previously resisted. */
void *CreateAirshipFireball(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    MATCH_HOLD_REG(s32, bReg, r6) = b;
    MATCH_HOLD_REG(s32, cReg, r8) = c;
    MATCH_HOLD_REG(s32, dReg, r0) = d;
    MATCH_HOLD_REG(s32, health, r5) = 2;

    InitActorPart(self, part, b, c, dReg);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = (void *)gAirshipFireballVtable;
    *(s32 *)(self + 0x58) = bReg;
    *(s32 *)(self + 0x5c) = cReg;
    *(s32 *)(self + 0x64) = 0;
    *(s32 *)(self + 0x60) = 0x95;
    self[0x68] = 0;

    return self;
}

asm(".align 2, 0");

/* Same boss-weapon "self" object family as above - see this
 * file's header comment and docs/matching/archive/issue-58-0x08030334-actor.md. */

/* Trivial setter: marks `self+0x68` (a small state/flag byte, meaning
 * not yet understood beyond its offset). */
void AirshipFireballStateExplode(void *selfArg)
{
    u8 *self = selfArg;
    self[0x68] = 1;
}

/* Same "self" object family as above - see
 * docs/matching/archive/issue-58-0x08030334-actor.md. */

/* `UpdateAirshipFireball`'s (above) per-state member-pointer dispatch
 * without its tail: `(this->*gAirshipFireballStateFuncs[this->state])()`
 * (see `ACTOR_PMF_CALL`). */
void RunAirshipFireballState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gAirshipFireballStateFuncs);
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");

/* Same boss-weapon "self" object family as above - see this
 * file's header comment and docs/matching/archive/issue-58-0x08030334-actor.md. */

/* Trivial getter counterpart to `AirshipFireballStateExplode` (above): reads
 * `self+0x68`. */
u8 IsAirshipFireballUnshootable(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x68];
}

asm(".align 2, 0");
