#include "core.h"
#include "match.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "util.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "level.h"
#include "globals.h"

/* First half of the `0x08031A6C`-`0x08032858` remainder issue #59's
 * foundational pass (docs/matching/archive/issue-59-0x08031784-actor.md) left
 * for "Phase 2" - the first 30 of the 60 still-raw functions in
 * `asm/code_3_2_20_28568_c99c_31784_31a6c.s`, `UpdateJetpackBalloonCrate` through
 * `UpdateJetpackRing` inclusive. Same shared "self" object family documented
 * for the boss-weapon cluster (issues #58/#62) and confirmed again on
 * first read here: `struct actor_self` (actor_self.h) - `state`, the
 * table-index/"kind" `animIndex`, the `animTimer`/`animDone` pair, the
 * `animTime` accumulator, the `anims` part table and the `vtable`
 * event/trampoline table.
 *
 * `UpdateJetpackQuestionCrate`/`DamageJetpackQuestionCrate`/`UpdateJetpackHealthCrate`/`UpdateJetpackTimeCrate`/`DamageJetpackTimeCrate`
 * are the "type-byte event dispatch" family already characterized by
 * `docs/rom_map.md`: a proximity check (`IsTouchingPlayer`) or a countdown
 * timer at `self+0x54` gates the transition, `PlaySfx(3, 0x100)` always
 * plays first, then the record's kind byte (`record->index`) selects between
 * `FreezeLevelClock`/`QueueJetpackWumpa` calls - written as `goto`-chained `if`
 * blocks (not a plain `switch`) to match this family's already-matched
 * sibling `UpdatePolarQuestionCrate` (`polar_pickups.c`), whose last case does
 * something structurally different from the others and resists a plain
 * `switch`'s uniform codegen.
 *
 * `UpdateJetpackRocket` is the already-flagged orbital-motion consumer of the
 * shared trig table `gSineTable`; `JetpackBalloonCrateStateHang` turned out to
 * be a second, closely-related consumer of the same table feeding the
 * same `x`/`y` position pair.
 *
 * `UpdateJetpackBalloonCrate`/`RunJetpackBalloonCrateState` are the per-state member-pointer
 * dispatches through `gJetpackBalloonCrateStateFuncs` (`ACTOR_PMF_CALL`,
 * include/actor_self.h) - once parked NAKED as an "r7 table-base-pin"
 * hazard, see docs/matching/archive/pmf-dispatch-retry.md. */

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);


/* The derived classes in this file, each the common `actor_self` prefix
 * plus its own fields. The animation-reset blocks store through
 * `*(T *)&self->field` casts: plain member stores let gcc move the
 * zero loads (docs/workflow.md step 7). */

/* `UpdateJetpackQuestionCrate`-`IsJetpackBalloonCrateUnshootable`: orbits `center` and carries a child
 * object (`SpawnJetpackBalloon`, released with `ReleaseJetpackBalloon`). */
struct orbit_actor {
    struct actor_self base;
    s32 health;  // 0x54
    void *child; // 0x58
    u8 done;     // 0x5c
    u8 unk_5d[3];
    s32 centerX;   // 0x60
    s32 centerY;   // 0x64
    s32 phase;     // 0x68 - random, added to stateTime
    s32 fallSpeed; // 0x6c - JetpackBalloonCrateStateFall, capped at 0x4c0
    void *spawn;   // 0x70 - the level spawn record, handed to MarkSpawnCollected
};

/* `UpdateJetpackParachuteNitro`-`IsJetpackParachuteNitroUnshootable`: climbs until it reaches `limitY`. */
struct rising_actor {
    struct actor_self base;
    s32 health; // 0x54
    u8 dead;    // 0x58
    u8 unk_59[3];
    s32 limitY; // 0x5c
};

/* `UpdateJetpackRocket`-`IsJetpackRocketUnshootable`: swings around `originX` while moving
 * down by `stepY` until `limitY`. */
struct swing_actor {
    struct actor_self base;
    s32 health;   // 0x54
    s32 originX;  // 0x58
    s32 limitY;   // 0x5c
    s32 stepY;    // 0x60
    u8 triggered; // 0x64 - set by the LaunchJetpackRocket transition
    u8 hit;       // 0x65
};

/* `UpdateJetpackRing`'s object is `struct jetpack_ring` (vehicle.h). */

/* Per-state member-pointer dispatch, `(this->*gJetpackBalloonCrateStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`), then "destroy" once state 1
 * has risen past a height or the state-2 animation has played through,
 * else the standard UpdateActor step. */
void UpdateJetpackBalloonCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    ACTOR_PMF_CALL(self, gJetpackBalloonCrateStateFuncs);

    if (self->state == 1 && self->y > 0xE100) {
        if (self != NULL) {
            ACTOR_VCALL(self, destroy, 3);
        }
    } else if (self->state == 2 && self->animDone != 0) {
        if (self != NULL) {
            ACTOR_VCALL(self, destroy, 3);
        }
    } else {
        UpdateActor(self);
    }
}

/* Proximity-triggered member of the shared "type-byte event dispatch"
 * family: on trigger, transitions to state 2/table-index 1 (anim frame
 * from `self`'s own part table at `+0xc`), then dispatches on a
 * `self+0x30` type byte (`0x14`-`0x16` into `QueueJetpackWumpa` at
 * increasing tiers, `0x17` into a fixed sound cue plus
 * `MarkSpawnCollected`/`AddLife`), flushes a pending trampoline call at
 * `self+0x58`, marks `self+0x5c`, and tail-calls `UpdateJetpackBalloonCrate`. */
void UpdateJetpackQuestionCrate(void *selfArg)
{
    struct orbit_actor *self = selfArg;
    s32 kind = self->base.animIndex;

    if (kind == 0 && (u8)IsTouchingPlayer(self)) {
        MATCH_HOLD_REG(s32, state, r0) = 2;
        MATCH_HOLD_REG(s32, one, r1) = 1;
        s32 typeByte;

        self->base.state = state;
        self->base.stateTime = kind;
        self->base.animIndex = one;
        {
            MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
            MATCH_HOLD_REG(u8, zero1, r1) = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
        }
        self->base.animTime = kind;

        typeByte = *(u8 *)&self->base.record->index;

        if (typeByte == 0x15) {
            goto case_15;
        }
        if (typeByte > 0x15) {
            goto gt_15;
        }
        if (typeByte == 0x14) {
            goto case_14;
        }
        goto after_dispatch;

    gt_15:
        if (typeByte == 0x16) {
            goto case_16;
        }
        if (typeByte == 0x17) {
            goto case_17;
        }
        goto after_dispatch;

    case_14:
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        QueueJetpackWumpa(gActorList, 1);
        goto after_dispatch;

    case_15:
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        QueueJetpackWumpa(gActorList, 3);
        goto after_dispatch;

    case_16:
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        QueueJetpackWumpa(gActorList, 5);
        goto after_dispatch;

    case_17:
        PlaySfx(gAudioContext, SFX_EXTRA_LIFE, 0x100);
        MarkSpawnCollected(self->spawn);
        AddLife(gLevelState);

    after_dispatch:
        if (self->child != NULL) {
            AddBrokenCrate(gLevelState);
            ReleaseJetpackBalloon(self->child);
            self->child = NULL;
        }
        self->done = 1;
    }

    UpdateJetpackBalloonCrate(self);
}

/* Countdown twin of `UpdateJetpackQuestionCrate`: gated by `self+0x54`'s health-style
 * timer instead of proximity, same type-byte dispatch, no tail call
 * (the caller drives whatever comes after directly). */
void DamageJetpackQuestionCrate(void *selfArg, s32 delta)
{
    struct orbit_actor *self = selfArg;
    s32 health = self->health - delta;
    s32 typeByte;

    self->health = health;
    if (health > 0) {
        return;
    }

    {
        MATCH_HOLD_REG(s32, state, r0) = 2;
        MATCH_HOLD_REG(s32, one, r1) = 1;

        self->base.state = state;
        {
            MATCH_HOLD_REG(s32, zero2, r2) = 0;

            self->base.stateTime = zero2;
            self->base.animIndex = one;
            {
                MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
                MATCH_HOLD_REG(u8, zero1, r1) = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero1;
            }
            *(s32 *)&self->base.animTime = zero2;
        }
    }

    typeByte = *(u8 *)&self->base.record->index;

    if (typeByte == 0x15) {
        goto case_15;
    }
    if (typeByte > 0x15) {
        goto gt_15;
    }
    if (typeByte == 0x14) {
        goto case_14;
    }
    goto after_dispatch;

gt_15:
    if (typeByte == 0x16) {
        goto case_16;
    }
    if (typeByte == 0x17) {
        goto case_17;
    }
    goto after_dispatch;

case_14:
    PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
    QueueJetpackWumpa(gActorList, 1);
    goto after_dispatch;

case_15:
    PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
    QueueJetpackWumpa(gActorList, 3);
    goto after_dispatch;

case_16:
    PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
    QueueJetpackWumpa(gActorList, 5);
    goto after_dispatch;

case_17:
    PlaySfx(gAudioContext, SFX_EXTRA_LIFE, 0x100);
    MarkSpawnCollected(self->spawn);
    AddLife(gLevelState);

after_dispatch:
    if (self->child != NULL) {
        AddBrokenCrate(gLevelState);
        ReleaseJetpackBalloon(self->child);
        self->child = NULL;
    }
    self->done = 1;
}

/* Proximity-triggered transition with a single fixed downstream call
 * (`HealJetpackPlayer(player, 0x14)`) rather than a type-byte dispatch, then
 * flushes `self+0x58` and tail-calls `UpdateJetpackBalloonCrate`. */
void UpdateJetpackHealthCrate(void *selfArg)
{
    struct orbit_actor *self = selfArg;
    s32 kind = self->base.animIndex;

    if (kind == 0 && (u8)IsTouchingPlayer(self)) {
        MATCH_HOLD_REG(s32, state, r0) = 2;
        MATCH_HOLD_REG(s32, one, r6) = 1;

        self->base.state = state;
        self->base.stateTime = kind;
        self->base.animIndex = one;
        {
            MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
            MATCH_HOLD_REG(u8, zero1, r1) = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
        }
        self->base.animTime = kind;

        HealJetpackPlayer(gActorList, 0x14);
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);

        if (self->child != NULL) {
            AddBrokenCrate(gLevelState);
            ReleaseJetpackBalloon(self->child);
            self->child = (void *)kind;
        }
        self->done = one;
    }

    UpdateJetpackBalloonCrate(self);
}

/* Proximity-triggered member of the `FreezeLevelClock` half of the type-byte
 * dispatch family (values `0x18`/`0x19`/`0x1a`/`0x1d`); the `0x1d` case
 * plays a different cue and calls `StartTimeTrial` instead, and the
 * trailing flush re-reads the type byte fresh to skip the lap-counter
 * tie (`AddBrokenCrate`) specifically for that case. Tail-calls
 * `UpdateJetpackBalloonCrate`. */
void UpdateJetpackTimeCrate(void *selfArg)
{
    struct orbit_actor *self = selfArg;
    s32 kind = self->base.animIndex;

    if (kind == 0 && (u8)IsTouchingPlayer(self)) {
        MATCH_HOLD_REG(s32, state, r0) = 2;
        MATCH_HOLD_REG(s32, one, r1) = 1;
        s32 typeByte;

        self->base.state = state;
        self->base.stateTime = kind;
        self->base.animIndex = one;
        {
            MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
            MATCH_HOLD_REG(u8, zero1, r1) = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
        }
        self->base.animTime = kind;

        typeByte = *(u8 *)&self->base.record->index;

        if (typeByte == 0x19) {
            goto case_19;
        }
        if (typeByte > 0x19) {
            goto gt_19;
        }
        if (typeByte == 0x18) {
            goto case_18;
        }
        goto after_dispatch;

    gt_19:
        if (typeByte == 0x1a) {
            goto case_1a;
        }
        if (typeByte == 0x1d) {
            goto case_1d;
        }
        goto after_dispatch;

    case_18:
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        FreezeLevelClock(gLevelState, 1);
        goto after_dispatch;

    case_19:
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        FreezeLevelClock(gLevelState, 2);
        goto after_dispatch;

    case_1a:
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        FreezeLevelClock(gLevelState, 3);
        goto after_dispatch;

    case_1d:
        PlaySfx(gAudioContext, SFX_CLOCK, 0x100);
        StartTimeTrial(gLevelState);

    after_dispatch:
        if (self->child != NULL) {
            if (*(u8 *)&self->base.record->index != 0x1d) {
                AddBrokenCrate(gLevelState);
            }
            ReleaseJetpackBalloon(self->child);
            self->child = NULL;
        }
        self->done = 1;
    }

    UpdateJetpackBalloonCrate(self);
}

/* Countdown twin of `UpdateJetpackTimeCrate`: gated by `self+0x54`'s timer instead
 * of proximity, same `FreezeLevelClock` dispatch, no tail call. */
void DamageJetpackTimeCrate(void *selfArg, s32 delta)
{
    struct orbit_actor *self = selfArg;
    s32 health = self->health - delta;
    s32 typeByte;

    self->health = health;
    if (health > 0) {
        return;
    }

    {
        MATCH_HOLD_REG(s32, state, r0) = 2;
        MATCH_HOLD_REG(s32, one, r1) = 1;

        self->base.state = state;
        {
            MATCH_HOLD_REG(s32, zero2, r2) = 0;

            self->base.stateTime = zero2;
            self->base.animIndex = one;
            {
                MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
                MATCH_HOLD_REG(u8, zero1, r1) = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero1;
            }
            *(s32 *)&self->base.animTime = zero2;
        }
    }

    typeByte = *(u8 *)&self->base.record->index;

    if (typeByte == 0x19) {
        goto case_19;
    }
    if (typeByte > 0x19) {
        goto gt_19;
    }
    if (typeByte == 0x18) {
        goto case_18;
    }
    goto after_dispatch;

gt_19:
    if (typeByte == 0x1a) {
        goto case_1a;
    }
    if (typeByte == 0x1d) {
        goto case_1d;
    }
    goto after_dispatch;

case_18:
    PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
    FreezeLevelClock(gLevelState, 1);
    goto after_dispatch;

case_19:
    PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
    FreezeLevelClock(gLevelState, 2);
    goto after_dispatch;

case_1a:
    PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
    FreezeLevelClock(gLevelState, 3);
    goto after_dispatch;

case_1d:
    PlaySfx(gAudioContext, SFX_CLOCK, 0x100);
    StartTimeTrial(gLevelState);

after_dispatch:
    if (self->child != NULL) {
        if (*(u8 *)&self->base.record->index != 0x1d) {
            AddBrokenCrate(gLevelState);
        }
        ReleaseJetpackBalloon(self->child);
        self->child = NULL;
    }
    self->done = 1;
}

/* `InitActorPart`-based constructor: forwards `a`/`b`/`c`/`d` straight
 * through, marks health `2`, stashes `b`/`c` into `self+0x60`/`0x64`, a
 * random 16-bit seed into `self+0x68`, then forwards to `SpawnJetpackBalloon`
 * (kind `0x28`) with `c` biased by `-15798` - one of the "spawn effect
 * type N" family's own per-kind constructors (docs/rom_map.md). */
void *CreateJetpackTimeCrate(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct orbit_actor *self = selfArg;
    MATCH_HOLD_REG(s32, health, r8) = 2;

    InitActorPart(self, part, b, c, d);
    self->health = health;
    self->base.vtable = (struct actor_vtable *)gJetpackBalloonCrateVtable;
    self->done = 0;
    self->centerX = b;
    self->centerY = c;
    self->phase = (u16)RandRange(0xff);

    self->child = (void *)SpawnJetpackBalloon(0x28, b, c + (s32)0xFFFFC24A, d, (s32)self);
    self->base.vtable = (struct actor_vtable *)gJetpackTimeCrateVtable;

    return self;
}

/* Countdown twin of `DamageJetpackQuestionCrate`'s shape applied to a fixed-cue,
 * single-downstream-call proximity/countdown transition (same body as
 * `DamageJetpackHealthCrate` below except gated by `self+0x54`, see there). */
void DamageJetpackHealthCrate(void *selfArg, s32 delta)
{
    struct orbit_actor *self = selfArg;
    s32 health = self->health - delta;

    self->health = health;
    if (health > 0) {
        return;
    }

    {
        MATCH_HOLD_REG(s32, state, r0) = 2;
        MATCH_HOLD_REG(s32, one, r6) = 1;

        self->base.state = state;
        {
            MATCH_HOLD_REG(s32, zero2, r5) = 0;

            self->base.stateTime = zero2;
            self->base.animIndex = one;
            {
                MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
                MATCH_HOLD_REG(u8, zero1, r1) = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero1;
            }
            *(s32 *)&self->base.animTime = zero2;

            HealJetpackPlayer(gActorList, 0x14);
            PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);

            if (self->child != NULL) {
                AddBrokenCrate(gLevelState);
                ReleaseJetpackBalloon(self->child);
                self->child = (void *)zero2;
            }
            self->done = one;
        }
    }
}

/* Same `SpawnJetpackBalloon`-based constructor shape as `CreateJetpackTimeCrate`, kind
 * `0x2a`, final event table `gJetpackHealthCrateVtable`. */
void *CreateJetpackHealthCrate(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct orbit_actor *self = selfArg;
    MATCH_HOLD_REG(s32, health, r8) = 2;

    InitActorPart(self, part, b, c, d);
    self->health = health;
    self->base.vtable = (struct actor_vtable *)gJetpackBalloonCrateVtable;
    self->done = 0;
    self->centerX = b;
    self->centerY = c;
    self->phase = (u16)RandRange(0xff);

    self->child = (void *)SpawnJetpackBalloon(0x2a, b, c + (s32)0xFFFFC24A, d, (s32)self);
    self->base.vtable = (struct actor_vtable *)gJetpackHealthCrateVtable;

    return self;
}

/* Same `SpawnJetpackBalloon`-based constructor shape again, kind `0x29`, final
 * event table `gJetpackQuestionCrateVtable`, plus a 6th argument stashed
 * verbatim into `self+0x70`. */
void *CreateJetpackQuestionCrate(void *selfArg, void *part, s32 b, s32 c, s32 d, s32 e)
{
    struct orbit_actor *self = selfArg;
    MATCH_HOLD_REG(s32, health, r8) = 2;

    InitActorPart(self, part, b, c, d);
    self->health = health;
    self->base.vtable = (struct actor_vtable *)gJetpackBalloonCrateVtable;
    self->done = 0;
    self->centerX = b;
    self->centerY = c;
    self->phase = (u16)RandRange(0xff);

    self->child = (void *)SpawnJetpackBalloon(0x29, b, c + (s32)0xFFFFC24A, d, (s32)self);
    self->base.vtable = (struct actor_vtable *)gJetpackQuestionCrateVtable;
    self->spawn = (void *)e;

    return self;
}

/* Trivial `self+0x58` clearing setter. */
void ClearJetpackCrateBalloon(void *selfArg)
{
    struct orbit_actor *self = selfArg;
    self->child = NULL;
}

/* Full reset idiom variant: `self+0x6c`/`0x44`/`0xc`/`8` cleared, state
 * set to 1, anim frame re-synced from `self`'s own part table at `+0`
 * (not `+0xc`, unlike the boss cluster's usual reset block), lap-counter
 * tie (`AddBrokenCrate`), `self+0x58` cleared. */
void BreakJetpackBalloonCrate(void *selfArg)
{
    struct orbit_actor *self = selfArg;
    MATCH_HOLD_REG(s32, zero, r5) = 0;

    self->fallSpeed = zero;
    self->base.state = 1;
    self->base.stateTime = zero;
    self->base.animIndex = zero;
    {
        MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[0].duration;
        MATCH_HOLD_REG(u8, zero1, r1) = 0;

        *(u16 *)&self->base.animTimer = anim;
        *(u8 *)&self->base.animDone = zero1;
    }
    self->base.animTime = zero;
    AddBrokenCrate(gLevelState);
    self->child = (void *)zero;
}

/* Countdown-gated state-2 transition with a `self+0x58` trampoline
 * flush (sound cue plus lap-counter tie only fire when there's a
 * pending object to flush), no type-byte dispatch. */
void DamageJetpackBalloonCrate(void *selfArg, s32 delta)
{
    struct orbit_actor *self = selfArg;
    s32 health = self->health - delta;

    self->health = health;
    if (health > 0) {
        return;
    }

    {
        MATCH_HOLD_REG(s32, state, r0) = 2;
        MATCH_HOLD_REG(s32, one, r6) = 1;

        self->base.state = state;
        {
            MATCH_HOLD_REG(s32, zero2, r5) = 0;

            self->base.stateTime = zero2;
            self->base.animIndex = one;
            {
                MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
                MATCH_HOLD_REG(u8, zero1, r1) = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero1;
            }
            *(s32 *)&self->base.animTime = zero2;

            if (self->child != NULL) {
                PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
                AddBrokenCrate(gLevelState);
                ReleaseJetpackBalloon(self->child);
                self->child = (void *)zero2;
            }
            self->done = one;
        }
    }
}

/* Doubly-linked-list unlink (`self+0x48`=prev, `self+0x4c`=next, cross-
 * links `next->prev`/`prev->next` around `self`), resets `vtable`'s
 * event table to `gActorVtable`, then conditionally `mem_free`s
 * `self` if the caller's flag bit 0 is set - a destructor/detach helper
 * for this object family. */
void DestroyJetpackBalloonCrate(void *selfArg, s32 flags)
{
    struct orbit_actor *self = selfArg;
    struct actor_self *next;
    struct actor_self *prev;

    self->base.vtable = (struct actor_vtable *)gActorVtable;

    {
        MATCH_HOLD_REG(struct actor_self *, nextReg, r2) = self->base.next;
        MATCH_HOLD_REG(struct actor_self *, prevReg, r0) = self->base.prev;

        nextReg->prev = prevReg;
    }

    prev = self->base.prev;
    next = self->base.next;
    prev->next = next;

    if ((flags & 1) != 0) {
        mem_free(self);
    }
}

/* Same `SpawnJetpackBalloon`-based constructor shape as `CreateJetpackTimeCrate`, but
 * fully parameterized: the "kind" (`0x28`/`0x29`/`0x2a`/etc there) is a
 * 6th caller-supplied byte argument here rather than a fixed literal,
 * and this one doesn't reassign `vtable`'s event table afterward.
 *
 * Once parked NAKED over three claimed gaps (`c` re-materialized from
 * r6, a late `kind` truncation and a b/c/d parameter-save order the
 * compiler "couldn't reproduce"); none of them exists for this plain
 * form - `kind` declared `u8` and `health` an ordinary local - which
 * matches under both agbcc and old_agbcc (see
 * docs/matching/archive/issue-59-60-m-operand-scheduling.md). */
void *InitJetpackBalloonCrate(void *selfArg, void *part, s32 b, s32 c, s32 d, u8 kind)
{
    struct orbit_actor *self = selfArg;
    s32 health = 2;

    InitActorPart(self, part, b, c, d);
    self->health = health;
    self->base.vtable = (struct actor_vtable *)gJetpackBalloonCrateVtable;
    self->done = 0;
    self->centerX = b;
    self->centerY = c;
    self->phase = (u16)RandRange(0xff);

    self->child = (void *)SpawnJetpackBalloon(kind, b, c + (s32)0xFFFFC24A, d, (s32)self);

    return self;
}

/* gJetpackBalloonCrateStateFuncs[2]: the crate was shot
 * (DamageJetpackBalloonCrate); UpdateJetpackBalloonCrate destroys it
 * once the state's animation has played through. Empty. */
void JetpackBalloonCrateStateDestroyed(void *selfArg)
{
}

/* Trivial accumulator: `y` advances by `self+0x6c`'s current
 * step, then the step itself advances by `0x12`/frame, clamped to
 * `0x4c0`. */
void JetpackBalloonCrateStateFall(void *selfArg)
{
    struct orbit_actor *self = selfArg;
    s32 pos = self->base.y;
    s32 delta = self->fallSpeed;

    self->base.y = pos + delta;
    delta += 0x12;
    self->fallSpeed = delta;
    if (delta <= 0x4c0) {
        return;
    }
    self->fallSpeed = 0x4c0;
}

/* A second, independent consumer of the shared trig table
 * `gSineTable` (the orbital-motion convention already
 * documented for `UpdateJetpackRocket`): computes an `x`/`y`
 * position pair from two phase-shifted table lookups around
 * `self+0x68 + self+0x44`, then - while `self+0x58` holds another
 * object - forwards the result into that object's own anim-frame-
 * advance-and-clamp step (`MoveJetpackBalloon`). */
void JetpackBalloonCrateStateHang(void *selfArg)
{
    struct orbit_actor *self = selfArg;
    const s16 *trig = gSineTable;
    s32 phase = self->phase + self->base.stateTime;
    s32 idx1 = ((phase * 5) >> 4) & 0xff;
    s32 v1 = trig[idx1];
    s32 x = self->centerX + v1 * 17;
    s32 idx2;
    s32 v2;
    s32 y;

    self->base.x = x;

    idx2 = ((phase * 8) >> 4) & 0xff;
    v2 = trig[idx2];
    y = self->centerY + v2 * 30;
    self->base.y = y;

    if (self->child != NULL) {
        MoveJetpackBalloon(self->child, x, y + (s32)0xFFFFC24A, self->base.z);
    }
}

/* `UpdateJetpackBalloonCrate`'s dispatch without its tail: `(this->*gStaticData_
 * 0817C42C[this->state])()` (see `ACTOR_PMF_CALL`). */
void RunJetpackBalloonCrateState(void *selfArg)
{
    struct actor_self *self = selfArg;

    ACTOR_PMF_CALL(self, gJetpackBalloonCrateStateFuncs);
}

/* Trivial `self+0x5c` byte getter. */
u8 IsJetpackBalloonCrateUnshootable(void *selfArg)
{
    struct orbit_actor *self = selfArg;
    return self->done;
}

/* State-1 trampoline flush, or (otherwise) a proximity-triggered
 * transition that fires an event-table call on the *player* object
 * (`gActorList`) before its own state-1/table-index-1
 * transition; either way clamps `y` forward by `0x140` once it
 * falls behind `self+0x5c`, then tail-calls `UpdateActor`. */
void UpdateJetpackParachuteNitro(void *selfArg)
{
    struct rising_actor *self = selfArg;

    if (self->base.animIndex == 1) {
        if (self->base.animDone == 0) {
            goto tail;
        }
        if (self != 0) {
            struct actor_vtable *table = self->base.vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, 3, table->destroy.fn);
        }
        return;
    }

    if ((u8)IsTouchingPlayer(self)) {
        struct actor_self *player = gActorList;
        struct actor_vtable *ptable = player->vtable;

        _call_via_r2((u8 *)player + ptable->m20.thisOffset, 0x14, ptable->m20.fn);
        AddBrokenCrate(gLevelState);
        PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
        self->base.animIndex = 1;
        {
            MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
            MATCH_HOLD_REG(u8, zero1, r1) = 0;
            MATCH_HOLD_REG(s32, zero2, r2) = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
            *(s32 *)&self->base.animTime = zero2;
        }
        self->dead = 1;
    }

    if (self->base.y < self->limitY) {
        self->base.y += 0x140;
    }

tail:
    UpdateActor(self);
}

/* Countdown-gated `self+0x58` byte transition into state 1 (anim frame
 * from `self`'s own part table at `+0xc`), then ties the lap counter. */
void DamageJetpackParachuteNitro(void *selfArg, s32 delta)
{
    MATCH_HOLD_REG(struct rising_actor *, self, r6) = selfArg;
    s32 health = self->health - delta;

    self->health = health;
    if (health > 0) {
        return;
    }

    {
        u8 *deathPtr = &self->dead;
        MATCH_HOLD_REG(s32, zero2, r5) = 0;
        MATCH_HOLD_REG(s32, one, r4) = 1;

        *deathPtr = one;
        PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
        self->base.animIndex = one;
        {
            MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
            MATCH_HOLD_REG(u8, zero1, r1) = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
        }
        *(s32 *)&self->base.animTime = zero2;
        AddBrokenCrate(gLevelState);
    }
}

/* Thin `InitActorPart`-based constructor: forwards `a`/`b` straight
 * through but replaces its own `c` with a fixed bias constant
 * (`0xFFFF0600`) for `InitActorPart`'s own 4th argument, stashing the
 * caller's real `c` into `self+0x5c` instead; `d` still forwards to
 * `InitActorPart` untouched.
 *
 * The blocker (issue #59/#60 gap write-up) was pure instruction
 * scheduling, not register allocation: whatever C expression carries
 * the `0xFFFF0600` constant, this compiler's call-argument evaluation
 * always computes it right next to `d`'s own stack load, while only
 * ever deferring the plain reg-to-reg copy of an *already-resident*
 * value (`self`, sitting in r4 the whole function) to just before the
 * call - a `d`-occupies-r0-until-its-own-store hazard, not a
 * scheduling hint, is what actually delays that copy. Manually writing
 * the call's last few instructions - the outgoing stack slot for `d`,
 * `self`-into-r0, the constant, and the `bl` itself - reproduces the
 * same hazard for the constant too and pins the ROM's exact order;
 * `d`'s own address is still entirely the compiler's choice via the
 * outgoing slot's "m" operand, and `a`/`b` pass through r1/r2
 * untouched. The `0xFFFF0600`/`gJetpackParachuteNitroVtable` literal pool
 * needed manual placement too (a trailing file-scope `asm` right after
 * the function) since inline asm's own `=constant` load syntax dumps
 * its literal in the assembler's default pool location instead of
 * immediately after the function like this compiler's own `-fhex-asm`
 * literals. */
void *CreateJetpackParachuteNitro(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct rising_actor *self = selfArg;
    MATCH_HOLD_REG(void *, aReg, r1) = part;
    MATCH_HOLD_REG(s32, bReg, r2) = b;
    MATCH_HOLD_REG(s32, dReg, r0) = d;
    MATCH_HOLD_REG(s32, health, r5) = 2;
    s32 outSlot;

    /* A plain call `InitActorPart(self, a, b, (s32)0xFFFF0600, d)` (or
     * any C expression/inline-asm value merely *passed* as its 4th
     * argument) always gets evaluated right next to `d`'s own stack
     * load - this compiler's call-argument evaluation puts every
     * "compute a fresh value" argument first, regardless of source
     * order, and only defers the plain reg-to-reg copy of an
     * already-resident value (`self`, sitting in r4 the whole function)
     * to just before the call. Manually writing the call's last few
     * instructions - the outgoing stack slot for `d`, `self`-into-r0,
     * the `0xFFFF0600` constant, and the `bl` itself - is what actually
     * pins their position, matching the ROM's own order; `part`/`b` are
     * passed through untouched via r1/r2, and the outgoing slot for `d`
     * is a plain local ("m" operand) so the compiler still owns its own
     * single stack-frame reservation instead of a hand-managed sp
     * adjustment local to this block. */
    // clang-format off
    asm volatile(
        "str %1, %0\n\t"
        "add r0, %2, #0\n\t"
        "ldr r3, 1f\n\t"
        "bl InitActorPart\n\t"
        : "=m"(outSlot)
        : "r"(dReg), "l"(self), "r"(aReg), "r"(bReg)
        : "r0", "r3", "r12", "lr", "memory", "cc");
    // clang-format on

    self->health = health;
    asm("ldr r0, 2f\n\tstr r0, [%0, #0x50]" : : "l"(self) : "r0", "memory");
    self->limitY = c;
    self->dead = 0;

    return self;
}
asm(".align 2, 0\n1: .4byte 0xFFFF0600\n2: .4byte gJetpackParachuteNitroVtable\n");

/* Trivial `self+0x58` byte getter. */
u8 IsJetpackParachuteNitroUnshootable(void *selfArg)
{
    struct rising_actor *self = selfArg;
    return self->dead;
}

/* The already-flagged orbital-motion consumer of the shared trig table
 * `gSineTable` (docs/rom_map.md): while idle (state 0),
 * checks proximity to fire an event-table call on the player plus a
 * state transition through `LaunchJetpackRocket`, then drives the orbit itself
 * (`x`) and either lets `y` coast forward by
 * `self+0x60` or, once it catches up to `self+0x5c`, re-seeds
 * `self+0x38`'s 3-word block from `gJetpackRocketBox` and re-fires
 * `LaunchJetpackRocket`. Once no longer idle, either flushes a pending
 * `vtable` trampoline call (state-1/table-index-1 shape) or repeats
 * the same player-proximity event once (latched via `self+0x65`).
 * Falls back to `UpdateActor` in both non-idle paths. */
void UpdateJetpackRocket(void *selfArg)
{
    struct swing_actor *self = selfArg;

    if (self->base.animIndex != 0) {
        goto state_nonzero;
    }

    if ((u8)IsTouchingPlayer(self)) {
        struct actor_self *player = gActorList;
        struct actor_vtable *ptable = player->vtable;

        _call_via_r2((u8 *)player + ptable->m20.thisOffset, 0xe, ptable->m20.fn);
        self->hit = 1;
        LaunchJetpackRocket(self);
    }

    /* `LaunchJetpackRocket` may have just transitioned the state away from 0 -
     * the ROM re-checks and, if so, joins the state-nonzero handling
     * below instead of running the orbital-motion step on stale state. */
    if (self->base.animIndex != 0) {
        goto state_nonzero;
    }

    {
        const s16 *trig = gSineTable;
        s32 idx = (self->base.stateTime) << 6;
        s32 v;

        idx = ((idx >> 4) & 0xff) + 0x40;
        idx &= 0xff;
        v = trig[idx];

        self->base.x = self->originX + v * 16;

        if (self->base.y > self->limitY) {
            self->base.y += self->stepY;
        } else {
            *(struct vec3_words *)&self->base.box = *(const struct vec3_words *)&gJetpackRocketBox;
            LaunchJetpackRocket(self);
        }
    }

    goto tail;

state_nonzero:
    if (self->base.animDone != 0) {
        if (self != 0) {
            struct actor_vtable *table = self->base.vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, 3, table->destroy.fn);
        }
        return;
    }

    if (self->hit == 0 && (u8)IsTouchingPlayer(self)) {
        struct actor_self *player = gActorList;
        struct actor_vtable *ptable = player->vtable;

        _call_via_r2((u8 *)player + ptable->m20.thisOffset, 0xe, ptable->m20.fn);
        self->hit = 1;
    }

tail:
    UpdateActor(self);
}

/* State transition setter: marks `self+0x64`, plays a fixed cue, sets
 * `self+0x18`, and the usual state-1/anim-reset block (anim frame from
 * `self`'s own part table at `+0xc`). */
void LaunchJetpackRocket(void *selfArg)
{
    struct swing_actor *self = selfArg;
    u8 *statePtr = &self->triggered;
    MATCH_HOLD_REG(s32, zero, r6) = 0;
    MATCH_HOLD_REG(s32, one, r5) = 1;

    *statePtr = one;
    PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
    self->base.palette = 7;
    self->base.animIndex = one;
    {
        MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
        MATCH_HOLD_REG(u8, zero1, r1) = 0;

        *(u16 *)&self->base.animTimer = anim;
        *(u8 *)&self->base.animDone = zero1;
    }
    self->base.animTime = zero;
}

/* Countdown-gated double-byte state transition (`self+0x64`/`0x65`),
 * anim frame taken from `self`'s own part table at `+0x18` this time
 * (not the usual `+0xc`). */
void DamageJetpackRocket(void *selfArg, s32 delta)
{
    MATCH_HOLD_REG(struct swing_actor *, self, r5) = selfArg;
    s32 health = self->health - delta;

    self->health = health;
    if (health > 0) {
        return;
    }

    {
        MATCH_HOLD_REG(u8 *, statePtr, r1) = &self->triggered;
        MATCH_HOLD_REG(s32, zero, r4) = 0;
        MATCH_HOLD_REG(s32, one, r0) = 1;

        *statePtr = one;
        asm volatile("add %0, %0, #1" : "+r"(statePtr));
        *statePtr = one;
        PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
        self->base.palette = 4;
        self->base.animIndex = 2;
        {
            MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[2].duration;
            MATCH_HOLD_REG(u8, zero1, r1) = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
        }
        self->base.animTime = zero;
    }
}

/* The base-class constructor call, as an inline wrapper. Passing the
 * constant through an inline's parameter is what places its load after
 * the outgoing stack store and the `self` copy, as in the ROM: a constant
 * written directly as a call argument is precomputed into a register
 * before the stack arguments are stored, while an inline's parameter is
 * a register when the call is expanded and only becomes the constant
 * when the inline is integrated. */
static inline void InitActorPartInline(void *self, void *part, s32 b, s32 c, s32 d)
{
    InitActorPart(self, part, b, c, d);
}

/* `InitActorPart`-based constructor (kind `1`, `InitActorPart`'s own 4th
 * argument replaced with a fixed `0xfa00` bias); clamps the caller's
 * `c` into `self+0x5c` (+-0x3f00), mirrors a clamped `x` into
 * `self+0x58` (+-0x8000), and derives `self+0x60` from
 * `__divsi3(self+0x5c - 0xfa00, 0xc6)`. Once parked NAKED over the
 * `0xfa00` load's position (see InitActorPartInline above). */
void *CreateJetpackRocket(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct swing_actor *self = selfArg;
    s32 health = 1;

    InitActorPartInline(self, part, b, 0xfa00, d);
    self->health = health;
    self->base.vtable = (struct actor_vtable *)gJetpackRocketVtable;
    if (c > 0x3f00)
        c = 0x3f00;
    if (c < -0x3f00)
        c = -0x3f00;
    self->limitY = c;
    if (self->base.x > 0x8000)
        self->base.x = 0x8000;
    if (self->base.x < -0x8000)
        self->base.x = -0x8000;
    self->originX = self->base.x;
    self->stepY = __divsi3(self->limitY - 0xfa00, 0xc6);
    self->hit = 0;
    self->triggered = 0;
    PlaySfx(gAudioContext, 0x2d, 0x100);

    return self;
}

/* Trivial `self+0x64` byte getter. */
u8 IsJetpackRocketUnshootable(void *selfArg)
{
    struct swing_actor *self = selfArg;
    return self->triggered;
}

/* Kind-gated (the low byte of `record->index` `== 0x1f`) proximity check:
 * on trigger, feeds the offset between `x` and the record's
 * `spawnX`, plus `y`, into `PassJetpackRing`, then
 * latches the one-shot `cued`. Tail-calls `UpdateActor`
 * unconditionally. */
void UpdateJetpackRing(void *selfArg)
{
    struct jetpack_ring *self = selfArg;

    if (*(u8 *)&self->base.record->index == 0x1f && (u8)IsTouchingPlayer(self)) {
        struct actor_self *player = gActorList;
        struct anim_table_record *record = self->base.record;
        s32 x = self->base.x - record->spawnX;
        s32 y = self->base.y;

        PassJetpackRing(player, x, y);

        if (self->cued == 0) {
            self->cued = 1;
            PlaySfx(gAudioContext, 0x3e, 0x100);
        }
    }

    UpdateActor(self);
}
