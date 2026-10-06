#include "core.h"
#include "match.h"
#include "actor_self.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "globals.h"

/* Start of the boss-weapon/singleton-object cluster's next raw range
 * (issue #58/#62's shared "self" object family continues here - state
 * at `self+0x28`, table-index/"kind" at `self+0xc`, anim-frame
 * halfword/byte pair at `self+0x10`/`self+0x12`, an accumulator at
 * `self+8`, a "part table" pointer at `self+0`, and an event/trampoline
 * table pointer at `self+0x50`). See docs/matching/archive/issue-58-0x08030334-actor.md,
 * docs/matching/archive/issue-62-0x08033804-actor.md and this range's own
 * write-up in docs/matching/. */

/* The gJetpackBalloonVtable class built by CreateJetpackBalloon. */
struct jetpack_balloon {
    struct actor_self base;
    s32 hp;                     // 0x54
    struct actor_self *pending; // 0x58
    u8 dying;                   // 0x5C
    u8 unk_5D[3];
    s32 velY; // 0x60
};

/* Gates the boss-weapon tracker's own "ready" check: while the tracker
 * is inactive (`gAirshipState == 0`), reports "not ready" (-1).
 * Otherwise scales the countdown `gAirshipHp` by 100 through
 * `__divsi3` against the weapon table's own first field
 * (`*gAirshipAttack`, a `void *` pointing at the small weapon-kind
 * table already characterized in airship_states.c), and reports "ready"
 * (1) once that scaled ratio is exactly zero and the countdown is still
 * running (`> 0`); otherwise passes the scaled ratio straight through. */
s32 GetAirshipHpPercent(void)
{
    MATCH_HOLD_REG(s32, countdown, r4);
    s32 result;

    if (gAirshipState == 0) {
        return -1;
    }

    countdown = gAirshipHp;
    result = __divsi3(countdown * 100, gAirshipAttack->hp);
    if (result == 0 && countdown > 0) {
        result = 1;
    }
    return result;
}

/* Destructor for the small tracker object (`gAirship`) -
 * `mem_free`'s it directly, the counterpart to its constructor
 * `CreateAirship` (airship.c). */
void DestroyAirship(void)
{
    mem_free(gAirship);
}

/* UNUSED - no caller anywhere in the ROM (checked every src/ .c file, the
 * category vtables and every word-aligned Thumb pointer in baserom.gba).
 * Empty; it has no table slot to name it after. */
void nullsub_30(void)
{
}

/* gAirshipStateFuncs[0]: no airship is active (AirshipStateFall goes back
 * to state 0). Empty. */
void AirshipStateInactive(void)
{
}

/* Per-frame update: syncs via `UpdateActorDepth`; once `self` has fallen
 * behind the camera (`depth` below `gActorNearClipDepth - 0x200`) it
 * releases its pending linked object (`ClearJetpackCrateBalloon`) and destroys
 * itself, as it also does once the state-2 animation has played through
 * or state 1 has sunk past a height; otherwise runs the member-pointer
 * dispatch `RunJetpackBalloonState`. The shared destroy tail is a `goto` target, as
 * the ROM's branch layout shares it between both paths. */
void UpdateJetpackBalloon(struct jetpack_balloon *self)
{
    UpdateActorDepth((struct actor_self *)self);
    if (self->base.depth < gActorNearClipDepth - 0x200) {
        if (self->pending != NULL) {
            ClearJetpackCrateBalloon(self->pending);
            self->pending = NULL;
        }
        goto destroy;
    }
    if ((self->base.state == 2 && self->base.animDone != 0) ||
        (self->base.state == 1 && self->base.y < -0xE100)) {
    destroy:
        if (self != NULL) {
            ACTOR_VCALL(&self->base, destroy, 3);
        }
    } else {
        RunJetpackBalloonState(&self->base);
    }
}

/* Trivial setter: forgets the attached crate (`pending`). */
void ClearJetpackBalloonCrate(void *selfArg)
{
    struct jetpack_balloon *self = selfArg;
    self->pending = NULL;
}

/* Damage handler: once hit points run out, marks `self` dying,
 * releases the pending linked object through its method table's `m38`
 * slot, plays the death cue and enters state 2 with animation 1. Same
 * overall shape as the boss cluster's `DamageAirshipFireball` (airship_fireball.c). */
void DamageJetpackBalloon(struct jetpack_balloon *self, s32 damage)
{
    if ((self->hp -= damage) > 0) {
        return;
    }
    self->dying = 1;
    if (self->pending != NULL) {
        struct actor_self *pending = self->pending;
        struct actor_vtable *vt = pending->vtable;

        ((void (*)(void *))vt->m38.fn)((u8 *)pending + vt->m38.thisOffset);
        self->pending = NULL;
    }
    PlaySfx(gAudioContext, 0x2E, 0x100);
    ACTOR_SET_STATE(&self->base, 2, 1);
}

/* Drops the attached crate and restarts state 1 (floating away) with
 * animation 0 - same shape as the boss cluster's reset blocks
 * (`AirshipStateApproach`, airship_states.c). The `animTimer`/`animDone`
 * stores go through `*(T *)&` casts: as plain member stores gcc drops
 * the r3 zero and reuses r1. */
void ReleaseJetpackBalloon(void *selfArg)
{
    struct jetpack_balloon *self = selfArg;
    MATCH_HOLD_REG(s32, zero, r2) = 0;

    self->pending = (struct actor_self *)zero;
    self->velY = zero;
    {
        MATCH_HOLD_REG(s32, one, r1) = 1;
        self->base.state = one;
    }
    self->base.stateTime = zero;
    self->base.animIndex = zero;
    {
        MATCH_HOLD_REG(u16, anim, r1) = self->base.anims[0].duration;
        MATCH_HOLD_REG(u8, zero3, r3) = 0;

        *(u16 *)&self->base.animTimer = anim;
        *(u8 *)&self->base.animDone = zero3;
    }
    self->base.animTime = zero;
}

/* Moves `self` to (x, y, z), then runs the shared
 * anim-frame-advance-and-clamp idiom. Each `anims[animIndex]` field is
 * re-indexed rather than read through a record pointer - that is what
 * gives the ROM's `#4`/`#6` constant scheduling
 * (docs/matching/archive/pmf-dispatch-retry.md). */
void MoveJetpackBalloon(struct actor_self *self, s32 x, s32 y, s32 z)
{
    s32 base;

    self->x = x;
    self->y = y;
    self->z = z;
    self->stateTime++;
    self->animTime += *(s16 *)&self->animTimer;
    self->animDone = 0;
    base = GetAnimFrameBaseOffset(self);
    if (base >= self->anims[self->animIndex].loopThreshold) {
        // clang-format off
        self->animTime -= (self->anims[self->animIndex].loopThreshold -
                           self->anims[self->animIndex].loopBase) << 8;
        // clang-format on
        self->animDone = 1;
    }
}

/* An `InitActorPart`-based constructor: forwards its first 4 real
 * arguments straight to `InitActorPart` (the last, `d`, stack-passed),
 * then sets `hp = 2` and the `gJetpackBalloonVtable` vtable, stores the
 * 6th argument (`e`, the attached crate, also stack-passed) in `pending`,
 * and clears `dying`. Same
 * shape as the already-matched `CreateAirshipFireball` (airship_fireball.c), except
 * with a 6th argument instead of a second stash of `c`. */
void *CreateJetpackBalloon(void *selfArg, void *part, s32 b, s32 c, s32 d, s32 e)
{
    struct jetpack_balloon *self = selfArg;
    MATCH_HOLD_REG(s32, eReg, r6) = e;
    MATCH_HOLD_REG(s32, health, r5) = 2;

    InitActorPart(self, part, b, c, d);
    self->hp = health;
    self->base.vtable = (struct actor_vtable *)gJetpackBalloonVtable;
    self->pending = (struct actor_self *)eReg;
    self->dying = 0;

    return self;
}

/* The shared anim-frame-advance-and-clamp idiom on its own (see
 * `MoveJetpackBalloon`). */
void JetpackBalloonStatePop(struct actor_self *self)
{
    s32 base;

    self->stateTime++;
    self->animTime += *(s16 *)&self->animTimer;
    self->animDone = 0;
    base = GetAnimFrameBaseOffset(self);
    if (base >= self->anims[self->animIndex].loopThreshold) {
        // clang-format off
        self->animTime -= (self->anims[self->animIndex].loopThreshold -
                           self->anims[self->animIndex].loopBase) << 8;
        // clang-format on
        self->animDone = 1;
    }
}

/* Falls under a decaying vertical velocity (`velY` drops by 6 per
 * frame, floored at -0x12C), then the shared anim-frame-advance-and-
 * clamp idiom (see `MoveJetpackBalloon`). */
void JetpackBalloonStateFloatAway(struct jetpack_balloon *self)
{
    s32 base;

    self->base.y += self->velY;
    self->velY -= 6;
    if (self->velY > -0x12C) {
        self->velY = -0x12C;
    }
    self->base.stateTime++;
    self->base.animTime += *(s16 *)&self->base.animTimer;
    self->base.animDone = 0;
    base = GetAnimFrameBaseOffset((struct actor_self *)self);
    if (base >= self->base.anims[self->base.animIndex].loopThreshold) {
        // clang-format off
        self->base.animTime -= (self->base.anims[self->base.animIndex].loopThreshold -
                                self->base.anims[self->base.animIndex].loopBase) << 8;
        // clang-format on
        self->base.animDone = 1;
    }
}

/* gJetpackBalloonStateFuncs[0]: the balloon is still tied to its crate,
 * which moves it (MoveJetpackBalloon) until ReleaseJetpackBalloon starts
 * state 1. Empty. */
void JetpackBalloonStateAttached(void)
{
}

/* Per-state member-pointer dispatch, `(this->*gJetpackBalloonStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`). */
void RunJetpackBalloonState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gJetpackBalloonStateFuncs);
}

/* Trivial `self+0x5c` byte getter. Needs a trailing `asm(".align 2, 0")`
 * - the lone-function-at-end-of-translation-unit padding gap already
 * documented for `IsAirshipFireballUnshootable`/`IsHovercraftCannonUnshootable` (issues #58/#62). */
u8 IsJetpackBalloonUnshootable(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x5c];
}

asm(".align 2, 0");
