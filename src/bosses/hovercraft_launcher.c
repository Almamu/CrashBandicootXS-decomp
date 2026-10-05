#include "core.h"
#include "actor_self.h"
#include "util.h"

/* Same "self" object family as hovercraft_parts.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern s32 GetHovercraftX(void);
extern s32 GetHovercraftY(void);
extern s32 GetHovercraftZ(void);
extern struct spawn_timing_table *GetHovercraftAttack(void);
extern s32 GetHovercraftState(void);
extern void CreateJetpackActor(s32 kind, s32 x, s32 y, s32 z, s32 arg4);
extern struct actor_self *gActorList;

/* The singleton's per-spawner timing table (`GetHovercraftAttack`): after each
 * spawn a spawner waits `delay` frames, except every `burst`-th spawn,
 * which resets its count and waits `burstDelay` instead. One record per
 * spawner kind (hovercraft_side_gun.c reads [0], hovercraft_cannon.c [1],
 * this file [2]). */
struct spawn_timing {
    s32 delay;
    s32 burst;
    s32 burstDelay;
};

struct spawn_timing_table {
    s32 unk_00;
    struct spawn_timing timing[3];  // 0x04
};

/* A spawner object of the singleton system (hovercraft_parts.c):
 * `actor_self` plus a hit-point word, its spawn cooldown/count and a
 * "dead" flag. */
struct spawner {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 spawnX;         // 0x58 - the constructor's `b`/`c` (CreateHovercraftLauncher)
    s32 spawnY;         // 0x5C
    u8 unk_60[4];
    s32 cooldown;       // 0x64
    s32 count;          // 0x68
    u8 dead;            // 0x6C
};

/* HovercraftLauncherStateLaunch: `HovercraftCannonStateFire`'s sibling. Sets `self`'s position fields
 * from the singleton's own position plus a different fixed offset,
 * and - while `cooldown` is zero - measures `self`'s
 * distance to the player the same way; in range, it picks one of three
 * spawn "kinds" (5/6/8, via `RandRange(3)`) and calls `CreateJetpackActor`
 * at `self`'s position, then cycles `count` against a threshold
 * from `GetHovercraftAttack`'s table. Once `base.depth` passes `0x4B00` and the
 * singleton's own "kind" (`GetHovercraftState`) is 3, resets `self` back to
 * its idle animation state.
 *
 * Matched in a later pass with the same shape as `HovercraftCannonStateFire` (see
 * docs/matching/issue-62-0x08033804-actor.md, "Later pass: strag2 retry"): no
 * register pins at all - the old `r7` blocker came from a wrong
 * source shape, not from a register the allocator couldn't reach. */
void HovercraftLauncherStateLaunch(struct spawner *self)
{
    s32 slot;
    s32 next;

    self->base.x = GetHovercraftX() + 0x1E00;
    self->base.y = GetHovercraftY() - 0x3000;
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
                s32 kind = (u16)RandRange(3);
                struct spawn_timing_table *table;
                s32 count;

                if (kind == 0) {
                    CreateJetpackActor(5, self->base.x, self->base.y, self->base.z, slot);
                } else if (kind == 1) {
                    CreateJetpackActor(6, self->base.x, self->base.y, self->base.z, slot);
                } else {
                    CreateJetpackActor(8, self->base.x, self->base.y, self->base.z, slot);
                }

                count = self->count + 1;
                self->count = count;
                table = GetHovercraftAttack();
                if (count == table->timing[2].burst) {
                    self->count = 0;
                    table = GetHovercraftAttack();
                    next = table->timing[2].burstDelay;
                } else {
                    table = GetHovercraftAttack();
                    next = table->timing[2].delay;
                }
                goto store;
            }
        }
    } else {
        next = slot - 1;
    store:
        self->cooldown = next;
    }

    if (self->base.depth > 0x4B00 && GetHovercraftState() == 3) {
        s32 zero32;
        s32 state;

        /* The ROM materializes the 0 and then the 2 before the stores
         * (`movs r2, #0; movs r0, #2`); the "=r"/"0" escapes keep both
         * as registers, and the volatile one stops the 2 from being
         * sunk to its store. */
        asm("" : "=r"(zero32) : "0"(0));
        asm volatile("" : "=r"(state) : "0"(2));
        self->base.state = zero32;
        self->base.stateTime = zero32;
        self->base.animIndex = state;
        {
            u16 anim = self->base.anims[2].duration;
            u8 zero;

            /* separate byte zero: the ROM materializes its own movs for it */
            asm("" : "=r"(zero) : "0"(0));
            self->base.animTimer = anim;
            self->base.animDone = zero;
        }
        self->base.animTime = zero32;
    }
}

asm(".align 2, 0");

/* Same "self" object family as hovercraft_parts.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern void *gAudioContext;

/* A spawner object of the singleton system (hovercraft_parts.c):
 * `actor_self` plus a hit-point word, its spawn cooldown/count and a
 * "dead" flag. */
extern void StartHovercraftHitFlash(void);
extern void LoseHovercraftPart(void);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);

/* `DamageHovercraftCannon`'s gated twin: only applies damage while `self` is in
 * state 1. On death, uses table-index 3 and the anim frame from
 * `self`'s part table `+0x24` field (instead of `DamageHovercraftCannon`'s
 * table-index 2/`+0x18`), and reuses the just-checked `state` value
 * (always 1 here) for the death-flag store, matching the ROM's literal
 * register reuse. The `*(T *)&self->...` stores keep gcc from treating them
 * as struct-member accesses, which changes where the byte zero is built. */
void DamageHovercraftLauncher(struct spawner *self, s32 dmg)
{
    s32 state = self->base.state;

    if (state == 1) {
        StartHovercraftHitFlash();
        self->hp -= dmg;

        if (self->hp <= 0) {
            u8 *deadFlag = &self->dead;
            register s32 zero asm("r4") = 0;

            *deadFlag = state;
            LoseHovercraftPart();
            {
                register s32 stateVal asm("r0") = 2;
                register s32 three asm("r1") = 3;

                self->base.state = stateVal;
                self->base.stateTime = zero;
                self->base.animIndex = three;
            }
            {
                register u16 anim asm("r0") = self->base.anims[3].duration;
                register u8 zero2 asm("r1") = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero2;
            }
            self->base.animTime = zero;
            PlaySfx(gAudioContext, 4, 0x100);
        } else {
            PlaySfx(gAudioContext, 0x45, 0x100);
        }
    }
}

/* Same "self" object family as hovercraft_parts.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern struct actor_pmf gHovercraftLauncherStateFuncs[];
extern void UpdateActor(void *self);

/* Per-state member-pointer dispatch, `(this->*gHovercraftLauncherStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`), then the standard
 * UpdateActor step unless the state-2 animation has played through. */
void UpdateHovercraftLauncher(struct actor_self *self)
{
    s32 state;
    s32 step;

    ACTOR_PMF_CALL(self, gHovercraftLauncherStateFuncs);

    state = self->state;
    step = 1;
    if (state == 2 && self->animDone != 0) {
        step = 0;
    }
    if (step) {
        UpdateActor(self);
    }
}

/* Same `InitActorPart`-rooted per-instance "self" object family already
 * documented in hovercraft_parts.c/hovercraft_cannon.c/actor.c: a "part
 * table" pointer at `self+0`, a table-index/"kind" field at `self+0xc`,
 * an anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 * accumulator at `self+8`, state at `self+0x28`, a frame counter at
 * `self+0x44`, and a `+0x50`-rooted event/trampoline table fed through
 * `_call_via_r2`. This particular object kind (constructed here by
 * `CreateHovercraftLauncher`, vtable `gHovercraftLauncherVtable`) additionally caches its
 * own constructor `b`/`c` arguments at `self+0x58`/`self+0x5c` and has a
 * death/"dead" byte flag at `self+0x6c` - see
 * docs/matching/issue-63-0x08033ef4-actor.md. */

extern void *InitActorPart(void *selfArg, void *part, s32 b, s32 c, s32 d);
extern u8 gHovercraftLauncherVtable[];

/* A spawner object of the singleton system (hovercraft_parts.c):
 * `actor_self` plus a hit-point word, its spawn cooldown/count and a
 * "dead" flag.
 *
 * The `*(T *)&self->...` byte/halfword stores below are deliberate: as plain
 * struct-member stores gcc moves the anim load and rebuilds the byte
 * zero instead of storing the pinned register. */
/* Constructor: forwards straight through to `InitActorPart`, then sets
 * `self`'s health (`+0x54=0x19`), event/trampoline table
 * (`+0x50=&gHovercraftLauncherVtable`), caches its own `b`/`c` constructor
 * args at `+0x58`/`+0x5c`, and resets state/frame-counter/table-index
 * (`+0x28`/`+0x44`/`+0xc=0`), the anim-frame pair from the part table's
 * first entry, the accumulator (`+8=0`) and the death flag
 * (`+0x6c=0`). Returns `self`. */
void *CreateHovercraftLauncher(void *selfArg, void *part, s32 b, s32 cParam, s32 d)
{
    struct spawner *self = selfArg;
    register s32 bReg asm("r6") = b;
    register s32 c asm("r8") = cParam;
    register s32 dReg asm("r0") = d;
    register s32 health asm("r5") = 0x19;

    InitActorPart(self, part, b, cParam, dReg);
    self->hp = health;
    self->base.vtable = (struct actor_vtable *)gHovercraftLauncherVtable;
    self->spawnX = bReg;
    self->spawnY = c;
    {
        register s32 zero asm("r1") = 0;

        self->base.state = zero;
        self->base.stateTime = zero;
        self->base.animIndex = zero;
        {
            register u16 anim asm("r0") = self->base.anims[0].duration;
            register u8 zero2 asm("r2") = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero2;
            self->base.animTime = zero;
            *(u8 *)&self->dead = zero2;
        }
    }

    return self;
}

extern void ShakeActorBg(s32 arg0);
extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);

/* Plays a fixed sound cue (`ShakeActorBg(0x400)`), then - if `self` is
 * non-NULL and its `+0x12` flag is set - fires the `self+0x50` event
 * table's slot-3 trampoline at `self` offset by the table's `+8`
 * halfword. Same shape as `HovercraftCannonStateDestroyed` (hovercraft_cannon.c). */
void HovercraftLauncherStateDestroyed(void *selfArg)
{
    struct actor_self *self = selfArg;

    ShakeActorBg(0x400);

    if (self->animDone != 0 && self != NULL) {
        register struct actor_vtable *table asm("r1") = self->vtable;
        register u8 *addr asm("r0");
        void *fn;

        {
            register s32 eight asm("r2") = 8;

            /* &table->destroy.thisOffset, with the 8 in its own register */
            addr = (u8 *)self + *(s16 *)((u8 *)table + eight);
        }
        fn = table->destroy.fn;

        _call_via_r2(addr, (void *)3, fn);
    }
}

extern s32 GetHovercraftPartsLeft(void);

/* Syncs `self`'s position fields (`+0x1c`/`+0x20`/`+0x24`) from the
 * `gHovercraft` singleton's own position plus a fixed offset, and
 * - while the singleton's lifetime counter (`GetHovercraftPartsLeft`) is still
 * under 3, and the singleton's own animation "kind" (`GetHovercraftState`) is
 * either 2, or 3 with `self+0x34` still under its `0x4AFF` threshold -
 * switches `self` to state 1/table-index 1, resetting the anim-frame
 * pair from the part table's `+0xc` entry and clearing the
 * accumulator/frame counter. */
void HovercraftLauncherStateWait(void *selfArg)
{
    register struct spawner *self asm("r4") = selfArg;

    self->base.x = GetHovercraftX() + 0x1E00;
    self->base.y = GetHovercraftY() - 0x3000;
    self->base.z = GetHovercraftZ() - 0x100;

    if (GetHovercraftPartsLeft() <= 2
     && (GetHovercraftState() == 2
      || (GetHovercraftState() == 3 && self->base.depth <= 0x4AFF))) {
        register s32 zero asm("r2") = 0;
        register s32 one asm("r0");

        self->cooldown = zero;
        self->count = zero;
        one = 1;
        self->base.state = one;
        self->base.stateTime = zero;
        self->base.animIndex = one;
        {
            register u16 anim asm("r0") = self->base.anims[1].duration;
            register u8 zero2 asm("r1") = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero2;
        }
        self->base.animTime = zero;
    }
}

/* Same "self" object family as above (constructed by
 * `CreateHovercraftLauncher`) - see docs/matching/issue-63-0x08033ef4-actor.md. */

/* Per-state member-pointer dispatch, `(this->*gHovercraftLauncherStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`); returns 0 once the
 * state-2 animation has played through, 1 otherwise. */
s32 RunHovercraftLauncherState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gHovercraftLauncherStateFuncs);

    if (self->state == 2 && self->animDone != 0) {
        return 0;
    }
    return 1;
}

/* Same `InitActorPart`-rooted per-instance "self" object family
 * documented in action_ctrl.c/hovercraft_parts.c/hovercraft_cannon.c. A second
 * object kind (constructed by the parked `CreateHovercraftSideGun`, vtable
 * `gHovercraftSideGunVtable`) reuses a death/"dead" byte flag at
 * `self+0x6c`. See docs/matching/issue-63-0x08033ef4-actor.md. */

/* Constant getter - returns `self`'s death flag (`self+0x6c`), the same
 * shape as `IsHovercraftCannonUnshootable` (hovercraft_cannon.c). */
u8 IsHovercraftLauncherUnshootable(void *selfArg)
{
    u8 *self = selfArg;

    return self[0x6c];
}

asm(".align 2, 0");
