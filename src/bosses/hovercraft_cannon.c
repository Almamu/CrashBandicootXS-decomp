#include "core.h"
#include "match.h"
#include "actor_self.h"
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "bosses.h"
#include "globals.h"

/* Same singleton system as hovercraft_parts.c - see that file's header
 * comment and docs/matching/archive/issue-62-0x08033804-actor.md. */

/* HovercraftCannonStateFire: a proximity-triggered effect/hazard detector. Syncs
 * `self`'s position fields to the singleton's current position (plus a
 * fixed offset), and - while the `cooldown` slot is zero -
 * measures `self`'s distance to the player; in range, it spawns a pair
 * of effects at `self`'s position and cycles `count` against a
 * threshold from `GetHovercraftAttack`'s table. Once `base.depth` passes
 * `0x4B00` it resets `self` to its idle animation state.
 *
 * Matched in a later pass (see docs/matching/archive/issue-62-0x08033804-actor.md,
 * "Later pass: strag2 retry"): both divisions are plain `/` through the ROM's own
 * `__divsi3` - as a libcall they don't clobber memory, so
 * `base.z` stays CSE'd in `r6` across them - the divisor is -0x1AA
 * (not -0xAA), and the new cooldown value is stored at one shared
 * `store:` label from all three paths. */
void HovercraftCannonStateFire(struct spawner *self)
{
    s32 slot;
    s32 next;

    self->base.x = GetHovercraftX() + 0x2000;
    self->base.y = GetHovercraftY() + 0x3000;
    self->base.z = GetHovercraftZ() - 0x100;

    slot = self->cooldown;
    if (slot == 0) {
        struct actor_self *player = gActorList;
        s32 angle = (player->z - self->base.z) / -0x1AA;

        if (angle > 0) {
            s32 scale = 0x1000 / angle;
            s32 rawDx = (player->x - self->base.x) * scale;
            s32 dx = rawDx >> 12;
            s32 rawDy = (player->y - self->base.y) * scale;
            s32 dy = rawDy >> 12;
            s32 signDx = rawDx >> 31;
            s32 absDx = (dx ^ signDx) - signDx;
            s32 signDy = rawDy >> 31;
            s32 absDy = (dy ^ signDy) - signDy;

            if (absDx + absDy <= 0xFFF) {
                const struct hovercraft_attack *table;
                s32 count;

                SpawnJetpackCannonball(self->base.x, self->base.y, self->base.z, dx, dy);
                SpawnHovercraftCannonFlash(self->base.x, self->base.y, self->base.z);

                count = self->count + 1;
                self->count = count;
                table = GetHovercraftAttack();
                if (count == table->timing[1].burst) {
                    self->count = slot;
                    table = GetHovercraftAttack();
                    next = table->timing[1].burstDelay;
                } else {
                    table = GetHovercraftAttack();
                    next = table->timing[1].delay;
                }
                goto store;
            }
        }
    } else {
        next = slot - 1;
    store:
        self->cooldown = next;
    }

    if (self->base.depth > 0x4B00) {
        self->base.state = 0;
        self->base.stateTime = 0;
        self->base.animIndex = 0;
        {
            u16 anim = self->base.anims[0].duration;
            u8 zero;

            /* separate byte zero: the ROM materializes its own movs for it */
            MATCH_CONST(zero, 0);
            self->base.animTimer = anim;
            self->base.animDone = zero;
        }
        self->base.animTime = 0;
    }
}

/* Same "self" object family as hovercraft_parts.c - see that file's header
 * comment and docs/matching/archive/issue-62-0x08033804-actor.md. */

/* Applies `dmg` damage to `hp`, and once it drops to zero (or
 * below), marks `self` dead (`dead = 1`), fires the singleton's own
 * death transition (`LoseHovercraftPart`), switches `self` to its death
 * state/anim (state 2, table-index 2, anim frame from `self`'s part
 * table at `+0x18`), and plays the death sound; otherwise just plays a
 * hit sound. The `*(T *)&self->...` stores keep gcc from treating them as
 * struct-member accesses, which changes where the byte zero is built. */
void DamageHovercraftCannon(struct spawner *self, s32 dmg)
{

    StartHovercraftHitFlash();
    self->hp -= dmg;

    if (self->hp <= 0) {
        u8 *deadFlag = &self->dead;
        MATCH_HOLD_REG(s32, zero, r4) = 0;

        *deadFlag = 1;
        LoseHovercraftPart();
        {
            MATCH_HOLD_REG(s32, stateVal, r0) = 2;

            self->base.state = stateVal;
            self->base.stateTime = zero;
            self->base.animIndex = stateVal;
            {
                MATCH_HOLD_REG(u16, anim, r0) = self->base.anims[2].duration;
                MATCH_HOLD_REG(u8, zero2, r1) = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero2;
            }
            self->base.animTime = zero;
        }
        PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
    } else {
        PlaySfx(gAudioContext, SFX_HOVERCRAFT_PART_HIT, 0x100);
    }
}

/* Same "self" object family as hovercraft_parts.c - see that file's header
 * comment and docs/matching/archive/issue-62-0x08033804-actor.md. */

/* Per-state member-pointer dispatch, `(this->*gHovercraftCannonStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`), then the standard
 * UpdateActor step unless the state-2 animation has played through. */
void UpdateHovercraftCannon(struct actor_self *self)
{
    s32 state;
    s32 step;

    ACTOR_PMF_CALL(self, gHovercraftCannonStateFuncs);

    state = self->state;
    step = 1;
    if (state == 2 && self->animDone != 0) {
        step = 0;
    }
    if (step) {
        UpdateActor(self);
    }
}

/* Same "self" object family as hovercraft_parts.c - see that file's header
 * comment and docs/matching/archive/issue-62-0x08033804-actor.md. */

/* The animation-reset blocks below store through `*(T *)&self->field`
 * casts: plain member stores let gcc move the zero load
 * (docs/workflow.md step 7). */

extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);

/* Constructor: forwards to `InitActorPart`, then sets `self`'s health
 * (`+0x54=15`), event/trampoline table (`+0x50=&gHovercraftCannonVtable`),
 * and caches its own `b`/`c` constructor args at `+0x58`/`+0x5c`;
 * finally resets state (`+0x28=0`) and the death flag (`+0x6c=0`).
 * Returns `self`. */
void *CreateHovercraftCannon(void *selfArg, void *part, s32 b, s32 cParam, s32 d)
{
    struct spawner *self = selfArg;
    MATCH_HOLD_REG(s32, bReg, r6) = b;
    MATCH_HOLD_REG(s32, c, r8) = cParam;
    MATCH_HOLD_REG(s32, dReg, r0) = d;
    MATCH_HOLD_REG(s32, health, r5) = 15;

    InitActorPart(self, part, b, cParam, dReg);
    self->hp = health;
    self->base.vtable = (struct actor_vtable *)gHovercraftCannonVtable;
    self->spawnX = bReg;
    self->spawnY = c;
    self->base.state = 0;
    self->dead = 0;

    return self;
}

/* Plays a fixed sound cue (`ShakeActorBg(0x400)`), then - if `self`'s
 * `+0x12` flag is set - fires the `vtable` event table's slot-3
 * trampoline at `self` offset by the table's `+8` halfword. */
void HovercraftCannonStateDestroyed(void *selfArg)
{
    struct spawner *self = selfArg;

    ShakeActorBg(0x400);

    if (self->base.animDone != 0 && self != NULL) {
        MATCH_HOLD_REG(struct actor_vtable *, table, r1) = self->base.vtable;
        MATCH_HOLD_REG(u8 *, addr, r0);
        void *fn;

        {
            MATCH_HOLD_REG(s32, eight, r2) = 8;

            /* table->destroy.thisOffset, with the 8 built in r2 */
            addr = (u8 *)self + *(s16 *)((u8 *)table + eight);
        }
        fn = table->destroy.fn;

        _call_via_r2(addr, (void *)3, fn);
    }
}

/* Sets `self`'s position fields (`+0x1c`/`+0x20`/`+0x24`) from the
 * singleton's own position plus a fixed offset, and - while `depth`
 * is still under its `0x4AFF` threshold - switches `self` to state 1/
 * table-index 1, seeding `+0x64`/`+0x68` from `gHovercraftAttack`'s
 * table and resetting the anim-frame pair from `self`'s part table's
 * `+0xc` field. */
void HovercraftCannonStateWait(void *selfArg)
{
    struct spawner *self = selfArg;

    self->base.x = GetHovercraftX() + 0x2000;
    self->base.y = GetHovercraftY() + 0x3000;
    self->base.z = GetHovercraftZ() - 0x100;

    if (self->base.depth <= 0x4AFF) {
        const struct hovercraft_attack *table = GetHovercraftAttack();

        self->cooldown = table->timing[1].delay;
        {
            MATCH_HOLD_REG(s32, zero, r2) = 0;

            self->count = zero;
            {
                MATCH_HOLD_REG(s32, one, r0) = 1;

                self->base.state = one;
                self->base.stateTime = zero;
                self->base.animIndex = one;
                {
                    MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->base.anims[1].duration;
                    MATCH_HOLD_REG(u8, zero2, r1) = 0;

                    *(u16 *)&self->base.animTimer = anim;
                    *(u8 *)&self->base.animDone = zero2;
                }
                self->base.animTime = zero;
            }
        }
    }
}

/* Same "self" object family as hovercraft_parts.c - see that file's header
 * comment and docs/matching/archive/issue-62-0x08033804-actor.md. */

/* Per-state member-pointer dispatch, `(this->*gHovercraftCannonStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`); returns 0 once the
 * state-2 animation has played through, 1 otherwise. */
s32 RunHovercraftCannonState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gHovercraftCannonStateFuncs);

    if (self->state == 2 && self->animDone != 0) {
        return 0;
    }
    return 1;
}

/* Same "self" object family as hovercraft_parts.c - see that file's header
 * comment and docs/matching/archive/issue-62-0x08033804-actor.md. */

/* Constant getter - returns `self`'s death flag (`self+0x6c`). */
u8 IsHovercraftCannonUnshootable(void *selfArg)
{
    u8 *self = selfArg;

    return self[0x6c];
}
