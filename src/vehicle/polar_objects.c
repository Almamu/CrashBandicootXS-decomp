#include "core.h"
#include "actor_self.h"
#include "util.h"
#include <libgcc.h>
#include "audio.h"
#include "actor.h"
#include "vehicle.h"

/* Continues the `InitActorPart`/`gActorList`-rooted "self" object
 * family (state at `self+0x28`, table-index/"kind" at `self+0xc`, an
 * anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 * accumulator at `self+8`, a `self+0x50`-rooted event/trampoline
 * table, and the position triple at `self+0x1c`/`self+0x20`/`self+0x24`)
 * already documented for `ctrl.c`-`polar_crates.c` and
 * `polar_aku_aku.c`. Sits between `polar_crates.c` (issue #53, ending
 * at `CreatePolarBasicCrate`) and issue #54's code (starting at
 * `MovePolarAkuAku`, at the end of this file) - the whole `0x0802CC9C`-`0x0802D3A8` gap
 * docs/matching/issue-53-actor-c7a8.md's "What's left" section
 * described as "a larger, IsTouchingYeti/IsTouchingPlayer/ShockPolarPlayer-calling
 * state machine ... not attempted this pass". */

extern void *gActorList;
extern void *gAudioContext;

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);
extern s32 SetMaskLevel(void *arg0, s32 arg1);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);

/* The gLevelState fields read here. */
struct game_state {
    u8 unk_00[0x78];
    s32 maskLevel;      // 0x78 - the Aku Aku mask level (0-3)
};

extern struct game_state *gLevelState;

/* The homing projectile (method table gPolarPenguinVtable). */
struct polar_penguin {
    struct actor_self base;
    s32 velX;           // 0x54
    s32 velY;           // 0x58
    s32 velZ;           // 0x5C
    s32 countdown;      // 0x60 - frames until the next retarget
    s32 targetZ;        // 0x64 - passed back to AimPolarPenguin on retarget
};

struct hazard_part {
    u8 unk_00[0x14];
    struct vec3_words box;           // 0x14
};

/* actor_self with this class's own fields over its unk_ areas. */
struct hazard {
    struct anim_frame_record *anims; // 0x00
    u32 *frameOffsets;          // 0x04
    s32 animTime;               // 0x08
    s32 animIndex;              // 0x0C
    u16 animTimer;              // 0x10
    u8 animDone;                // 0x12
    u8 unk_13;
    s32 sortKey;                // 0x14
    s32 palette;                // 0x18
    s32 x;                      // 0x1C
    s32 y;                      // 0x20
    s32 z;                      // 0x24
    s32 state;                  // 0x28
    u8 deep;                    // 0x2C
    u8 unk_2D[3];
    struct hazard_part *part;   // 0x30
    s32 depth;                  // 0x34
    struct vec3_words box;           // 0x38
    s32 stateTime;              // 0x44
    u8 unk_48[8];
    struct actor_vtable *vtable; // 0x50
};

/* Plays the pickup sound and restarts animation sequence 1 ("used").
 * Wrapped in `if (1)` rather than `do { } while (0)` or an inline: both
 * of those change the block layout gcc emits. */
#define HAZARD_HIT(self)                                                       \
    if (1)                                                                     \
    {                                                                          \
        PlaySfx(gAudioContext, 4, 0x100);                                  \
        (self)->animIndex = 1;                                                 \
        (self)->animTimer = (self)->anims[1].duration;                         \
        (self)->animDone = 0;                                                  \
        (self)->animTime = 0;                                                  \
    } else (void)0

/* Once-per-frame hazard/proximity update. Latches `deep` once `depth`
 * passes 0x15FF. While unused (sequence 0) it tests the part table's own
 * box with IsTouchingYeti, then gPolarElectricFenceWireBox's box with IsTouchingPlayer
 * (a hit there only counts if ShockPolarPlayer agrees), or failing that the
 * 0817A774 and 0817A780 boxes; any hit switches to sequence 1. Once used,
 * fires method 0x08 with 3 when the sequence has played through. */
void UpdatePolarElectricFence(void *selfArg)
{
    struct hazard *self = selfArg;

    if (self->depth > 0x15ff)
        self->deep = 1;

    if (self->animIndex == 0) {
        self->box = self->part->box;
        if (IsTouchingYeti((struct actor_self *)self)) {
            HAZARD_HIT(self);
        }
        self->box = *(const struct vec3_words *)&gPolarElectricFenceWireBox;
        if ((u8)IsTouchingPlayer(self)) {
            if ((u8)ShockPolarPlayer(gActorList)) {
                HAZARD_HIT(self);
            }
        } else {
            self->box = *(const struct vec3_words *)&gPolarElectricFenceLeftPostBox;
            if ((u8)IsTouchingPlayer(self)) {
                HAZARD_HIT(self);
            }
            self->box = *(const struct vec3_words *)&gPolarElectricFenceRightPostBox;
            if ((u8)IsTouchingPlayer(self)) {
                HAZARD_HIT(self);
            }
        }
    } else if (self->animDone) {
        if (self) {
            ACTOR_VCALL(self, destroy, 3);
        }
        return;
    }
    UpdateActor(self);
}


/* `InitActorPart`-based constructor: forwards `a`/`b`/`c`/`d` straight
 * through, installs `self+0x50 = gPolarElectricFenceVtable`, and clears the
 * `self+0x2c` one-shot flag. */
void *CreatePolarElectricFence(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct actor_self *self = selfArg;

    InitActorPart(self, part, b, c, d);
    self->vtable = (struct actor_vtable *)gPolarElectricFenceVtable;
    self->visible = 0;
    return self;
}

/* On the trampoline-fire edge (`IsTouchingPlayer`), forwards to
 * `HurtPolarPlayer(gActorList)` (the player object), discarding its
 * result; always tail-calls `UpdateActor`. */
void sub_802CE10(void *selfArg)
{
    u8 *self = selfArg;

    if ((u8)IsTouchingPlayer(self)) {
        (u8)HurtPolarPlayer(gActorList);
    }
    UpdateActor(self);
}

/* Same `InitActorPart`-based constructor shape as `CreatePolarElectricFence`, minus
 * the `self+0x2c` clear, `self+0x50 = gStaticData_087E4FD4`. */
void *sub_802CE38(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct actor_self *self = selfArg;

    InitActorPart(self, part, b, c, d);
    self->vtable = (struct actor_vtable *)gStaticData_087E4FD4;
    return self;
}

/* 3-way `self+0x28` state dispatch. State 0: on `IsTouchingPlayer`'s
 * trampoline-fire edge, calls `LaunchPolarPlayer(gActorList)` (the
 * player object), plays a cue, and transitions to state 1/table-index
 * 1; otherwise, on `IsTouchingYeti`'s player-overlap test, transitions the
 * same way. State 1: once `self+0x12` fires, dispatches the
 * `self+0x50` trampoline (index 3) instead of the usual
 * `UpdateActor` fallback. Any other state (and state 0/1's own
 * non-transition paths) falls through to `UpdateActor`. */

void UpdatePolarLauncher(void *selfArg)
{
    struct actor_self *self = selfArg;
    register s32 state asm("r5") = self->state;

    if (state == 0) {
        goto case0;
    }
    if (state == 1) {
        goto case1;
    }
    goto done;

case0:
    {
        register s32 fired asm("r6") = (u8)IsTouchingPlayer(self);

        if (fired) {
            LaunchPolarPlayer(gActorList);
            PlaySfx(gAudioContext, 4, 0x100);
            self->state = 1;
            self->stateTime = state;
            self->animIndex = 1;
            {
                register u16 anim asm("r0") = self->anims[1].duration;
                register u8 zero1 asm("r1") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
            }
            self->animTime = state;
            goto done;
        }
        if (IsTouchingYeti(self)) {
            PlaySfx(gAudioContext, 4, 0x100);
            self->state = 1;
            self->stateTime = fired;
            self->animIndex = 1;
            {
                register u16 anim asm("r0") = self->anims[1].duration;
                register u8 zero1 asm("r1") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
            }
            self->animTime = fired;
        }
    }
    goto done;

case1:
    if (self->animDone != 0) {
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, 3, table->destroy.fn);
        }
        return;
    }

done:
    UpdateActor(self);
}

/* Same `InitActorPart`-based constructor shape as `sub_802CE38`,
 * `self+0x50 = gPolarLauncherVtable`. */
void *CreatePolarLauncher(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct actor_self *self = selfArg;

    InitActorPart(self, part, b, c, d);
    self->vtable = (struct actor_vtable *)gPolarLauncherVtable;
    return self;
}

/* Applies `self`'s own velocity (`self+0x54`/`0x58`/`0x5c`) to its
 * position, and while idle (`self+0x28 == 0`) counts down
 * `self+0x60`, re-deriving a fresh velocity/homing target via
 * `AimPolarPenguin` once it expires. While idle, also probes
 * `IsTouchingPlayer`'s trampoline-fire edge against the player
 * (`HurtPolarPlayer`) or, failing that, `IsTouchingYeti`'s player-overlap
 * test - either hit re-arms a fixed outward velocity (`self+0x54`
 * biased by `self+0x1c`'s sign), a random negative Y kick
 * (`self+0x58`), bumps `self+0x5c`, plays a cue, and transitions to
 * state 1/table-index 0. Always tail-calls `UpdateActor`. */

void UpdatePolarPenguin(void *selfArg)
{
    struct polar_penguin *self = selfArg;
    register s32 state asm("r6");

    self->base.x += self->velX;
    self->base.y += self->velY;
    self->base.z += self->velZ;

    state = self->base.state;
    if (state == 0) {
        s32 remain = self->countdown - 1;
        self->countdown = remain;
        if (remain <= 0) {
            AimPolarPenguin(self, self->targetZ);
        }

        {
            register s32 fired asm("r5") = (u8)IsTouchingPlayer(self);

            if (fired) {
                if ((u8)HurtPolarPlayer(gActorList)) {
                    s32 velX = (self->base.x > 0) ? 0x600 : 0xFFFFFA00;

                    self->velX = velX;
                    self->velY = -(s32)(u16)RandRange(0x300);
                    self->velZ += 0x200;
                    PlaySfx(gAudioContext, 5, 0x100);
                    self->base.state = 1;
                    self->base.stateTime = state;
                    self->base.animIndex = state;
                    {
                        register u16 anim asm("r0") = self->base.anims[0].duration;
                        register u8 zero1 asm("r1") = 0;

                        *(u16 *)&self->base.animTimer = anim;
                        *(u8 *)&self->base.animDone = zero1;
                    }
                    self->base.animTime = state;
                }
            } else if (IsTouchingYeti((struct actor_self *)self)) {
                s32 velX = (self->base.x > 0) ? 0x600 : 0xFFFFFA00;

                self->velX = velX;
                self->velY = -(s32)(u16)RandRange(0x300);
                self->velZ += 0x200;
                PlaySfx(gAudioContext, 5, 0x100);
                self->base.state = 1;
                self->base.stateTime = fired;
                self->base.animIndex = fired;
                {
                    register u16 anim asm("r0") = self->base.anims[0].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->base.animTimer = anim;
                    *(u8 *)&self->base.animDone = zero1;
                }
                self->base.animTime = fired;
            }
        }
    }

    UpdateActor(self);
}

/* Homing-velocity (re)initializer: with a negative `target` index,
 * arms a fixed slow downward drift (`self+0x54/0x58/0x5c/0x60` set to
 * constants). Otherwise derives a per-frame speed factor
 * (`__divsi3` of `target`'s own "speed" record,
 * `gUnknown_0300088C[sub_802A570(target)]`, against the remaining
 * distance in Z) and scales the X/Y deltas toward `target`'s own
 * tracked position (`sub_802A558`/`sub_802A540`) by that factor,
 * caching the new countdown in `self+0x60` (floored at 1) and
 * `target`'s own Z record in `self+0x64`. */
void AimPolarPenguin(void *selfArg, s32 target)
{
    struct polar_penguin *self = selfArg;

    if (target < 0) {
        self->velY = 0;
        self->velX = 0;
        self->velZ = 0x62;
        self->countdown = 0x40000000;
    } else {
        s32 idx = sub_802A570(target);
        s32 speed = gUnknown_0300088C[idx];
        s32 factor;
        s32 countdown;

        self->velZ = speed;
        countdown = __divsi3(sub_802A51C(target) - self->base.z, self->velZ);
        self->countdown = countdown;
        if (countdown == 0) {
            self->countdown = 1;
        }

        {
            register s32 countdown2 asm("r1") = self->countdown;
            register s32 lit asm("r0") = 0x1000;

            factor = __divsi3(lit, countdown2);
        }
        self->velX = factor * (sub_802A558(target) - self->base.x) >> 0xc;
        self->velY = factor * (sub_802A540(target) - self->base.y) >> 0xc;
        self->targetZ = sub_802A504(target);
    }
}

/* `InitActorPart`-based constructor, forwarding `a`/`b`/`c`/`d`
 * straight through plus a 6th argument `e` (a pointer whose `+0x10`
 * field feeds `AimPolarPenguin`'s homing target): installs
 * `self+0x50 = gPolarPenguinVtable`, then calls
 * `AimPolarPenguin(self, e->0x10)`. */
void *CreatePolarPenguin(void *selfArg, void *part, s32 b, s32 c, s32 d, struct spawn_arg *e)
{
    struct actor_self *self = selfArg;

    InitActorPart(self, part, b, c, d);
    self->vtable = (struct actor_vtable *)gPolarPenguinVtable;
    AimPolarPenguin(self, e->target);
    return self;
}

/* On the trampoline-fire edge, forwards to `HurtPolarPlayer` on the player
 * object (discarding its result), then, gated on `self+0x34`'s cached
 * depth crossing one of two thresholds paired with `self+0x28`'s
 * current tier, advances `self+0xc`'s table index and, once
 * `GetAnimFrameBaseOffset` reaches the new record's own threshold,
 * clears `self+8` (deep-tier variant clears it unconditionally with
 * the pre-increment tier value instead) and bumps `self+0x28`. */
void UpdatePolarIcicle(void *selfArg)
{
    struct actor_self *self = selfArg;
    register s32 threshold1 asm("r0");
    register s32 depth asm("r1");

    if ((u8)IsTouchingPlayer(self)) {
        (u8)HurtPolarPlayer(gActorList);
    }

    threshold1 = 0x6400;
    depth = self->depth;

    if ((depth > threshold1 && self->state == 2)
        || (depth > 0x5A00 && self->state == 1)) {
        s32 frame;
        register s32 idx asm("r1") = self->animIndex + 1;

        self->animIndex = idx;
        {
            register u16 anim asm("r0") = self->anims[idx].duration;
            register u8 zero1 asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
        }
        frame = GetAnimFrameBaseOffset(self);
        {
            register s32 idx2 asm("r2") = self->animIndex;
            register u8 *table2 asm("r3") = (u8 *)self->anims;
            register s32 threshold asm("r1") = *(s16 *)(table2 + idx2 * 0xc + 4);

            if (frame >= threshold) {
                self->animTime = 0;
            }
        }
        goto increment;
    } else if (depth > 0x5000) {
        register s32 zero asm("r5") = self->state;

        if (zero == 0) {
            s32 frame;
            register s32 idx asm("r1") = self->animIndex + 1;

            self->animIndex = idx;
            {
                register u16 anim asm("r0") = self->anims[idx].duration;
                register u8 zero1 asm("r1") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
            }
            frame = GetAnimFrameBaseOffset(self);
            {
                register s32 idx2 asm("r2") = self->animIndex;
                register u8 *table2 asm("r3") = (u8 *)self->anims;
                register s32 threshold asm("r1") = *(s16 *)(table2 + idx2 * 0xc + 4);

                if (frame >= threshold) {
                    self->animTime = zero;
                }
            }
            goto increment;
        }
    }

    goto tail;

increment:
    self->state = self->state + 1;

tail:
    UpdateActor(self);
}

/* `InitActorPart`-based constructor: forwards `self`/`d` straight
 * through, passing `b` (a `u8 *`, cast to `s32` for `InitActorPart`'s
 * own generic third argument) and `c` unchanged; installs
 * `self+0x50 = gPolarIcicleVtable`, then classifies a "kind"
 * (`self+0xc`) from `b`'s own first byte (bumped by 1 if `c > 0`),
 * scaled `*4 - 0x40`, to seed `self+0x10`/`self+0x12`/`self+8` from the
 * part table. */
void *CreatePolarIcicle(void *selfArg, u8 *b, s32 c, s32 d, s32 e)
{
    struct actor_self *self = selfArg;
    register s32 kind asm("r1");

    InitActorPart(self, b, c, d, e);
    self->vtable = (struct actor_vtable *)gPolarIcicleVtable;

    kind = *b;
    if (c > 0) {
        kind += 1;
    }
    kind = kind * 4 - 0x40;
    self->animIndex = kind;
    {
        register u16 anim asm("r0") = self->anims[kind].duration;
        register u8 zero1 asm("r1") = 0;
        register s32 zero2 asm("r2") = 0;

        *(u16 *)&self->animTimer = anim;
        *(u8 *)&self->animDone = zero1;
        self->animTime = zero2;
    }
    return self;
}

/* VRAM-gauge/state-transition driver for a `gPolarAkuAkuInvincibleTimer`-counted
 * effect: while the current mask level (`maskLevel`) (`gLevelState->0x78`)
 * and the `retrigger` flag are both zero, just clears `self+0x2c`;
 * otherwise DMAs one of four `gPolarAkuAkuPalette1`-indexed gauge
 * strips and resets `self`'s table index/anim, arming `self+0x2c`.
 * Then: tier 3 arms a long `gPolarAkuAkuInvincibleTimer` countdown and
 * transitions to state 1; tier 0 with `retrigger` set transitions to
 * state 2/table-index 1 instead; any other combination just clears
 * `gPolarAkuAkuInvincibleTimer` and, if `self+0x28` was already non-zero, resets
 * `self` back to state 0/table-index 0. */
void RefreshPolarAkuAku(void *selfArg, s32 retriggerParam)
{
    struct actor_self *self = selfArg;
    u8 retrigger = (u8)retriggerParam;
    register s32 tier asm("r5") = gLevelState->maskLevel;

    if (tier == 0 && retrigger == 0) {
        self->visible = tier;
    } else {
        register s32 zero asm("r6");

        QueueVramDmaTransfer((u8 *)gPolarAkuAkuPalette1 + (tier - 1) * 0x20, (void *)(PLTT + 0x3C0), 0x20, 0x10);
        {
            u8 *addr = &self->visible;

            zero = 0;
            *addr = 1;
        }
        self->animIndex = zero;
        {
            register u16 anim asm("r0") = self->anims[0].duration;
            register u8 zero1 asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
        }
        {
            s32 frame = GetAnimFrameBaseOffset(self);
            s32 idx = self->animIndex;
            u8 *table = (u8 *)self->anims;

            if (frame >= *(s16 *)(table + idx * 0xc + 4)) {
                self->animTime = zero;
            }
        }
    }

    if (tier == 3) {
        {
            register s32 *addr asm("r1") = &gPolarAkuAkuInvincibleTimer;
            register s32 val asm("r0") = 0x1F4;

            *addr = val;
        }
        {
            register s32 state asm("r0") = 1;
            register s32 zero asm("r2") = 0;

            self->state = state;
            self->stateTime = zero;
            self->animIndex = zero;
            {
                register u16 anim asm("r0") = self->anims[0].duration;
                register u8 zero1 asm("r1") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
            }
            self->animTime = zero;
        }
        return;
    } else if (tier == 0 && retrigger != 0) {
        register s32 two asm("r0");
        register s32 one asm("r1");

        gPolarAkuAkuInvincibleTimer = tier;
        two = 2;
        one = 1;
        self->state = two;
        self->stateTime = tier;
        self->animIndex = one;
        {
            register u16 anim asm("r0") = self->anims[1].duration;
            register u8 zero1 asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
        }
        self->animTime = tier;
        return;
    } else {
        register s32 *addr asm("r0") = &gPolarAkuAkuInvincibleTimer;
        register s32 zero asm("r2") = 0;

        *addr = zero;
        if (self->state == 0) {
            return;
        }
        self->state = zero;

        self->stateTime = zero;
        self->animIndex = zero;
        {
            register u16 anim asm("r0") = self->anims[0].duration;
            register u8 zero1 asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
        }
        self->animTime = zero;
    }
}

/* Drives `gPolarAkuAkuInvincibleTimer`'s countdown, DMAing one of two gauge
 * strips per frame (`gPolarAkuAkuPalette3` on the low bit set,
 * `gPolarAkuAkuPalette2` otherwise) and, once it expires, resetting
 * the mask level (`maskLevel`) via `SetMaskLevel(gLevelState, 2)` then
 * `RefreshPolarAkuAku(self, 0)`. Independently re-fires `RefreshPolarAkuAku` once
 * state 2's own `self+0x12` edge trips. Always advances `self`'s own
 * anim frame (`UpdateActorDepth`, frame-counter bump, and the usual
 * wrap-around `GetAnimFrameBaseOffset` check). */
void UpdatePolarAkuAku(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (gPolarAkuAkuInvincibleTimer != 0) {
        if (gPolarAkuAkuInvincibleTimer & 4) {
            QueueVramDmaTransfer((void *)gPolarAkuAkuPalette3, (void *)(PLTT + 0x3C0), 0x20, 0x10);
        } else {
            QueueVramDmaTransfer((void *)gPolarAkuAkuPalette2, (void *)(PLTT + 0x3C0), 0x20, 0x10);
        }

        gPolarAkuAkuInvincibleTimer -= 1;
        if (gPolarAkuAkuInvincibleTimer == 0) {
            SetMaskLevel(gLevelState, 2);
            RefreshPolarAkuAku(self, 0);
        }
    }

    if (self->state == 2 && self->animDone != 0) {
        RefreshPolarAkuAku(self, 0);
    }

    UpdateActorDepth(self);
    self->stateTime += 1;
    self->animTime += *(s16 *)&self->animTimer;
    *(u8 *)&self->animDone = 0;

    {
        s32 frame = GetAnimFrameBaseOffset(self);
        register s32 idx2 asm("r2") = self->animIndex;
        register u8 *table2 asm("r3") = (u8 *)self->anims;
        register u8 *record asm("r1") = (u8 *)(idx2 * 0xc);

        asm("add %0, %0, %1" : "+r" (record) : "r" (table2));
        {
            register s32 threshold asm("r2") = *(s16 *)(record + 4);

            if (frame >= threshold) {
                register s32 diff asm("r0") = (threshold - *(s16 *)(record + 6)) << 8;

                self->animTime -= diff;
                *(u8 *)&self->animDone = 1;
            }
        }
    }
}

asm(".align 2, 0");

extern s16 gSineTable[];

/* Eases `self`'s cached position (`self+0x1c`/`0x20`/`0x24`, the same
 * fields `InitActorPart` caches its `b`/`c`/`d` constructor arguments
 * into, per actor.c) toward a caller-supplied target, with the
 * exact target/mode selected by `self+0x28` ("state"):
 *   - state 0: eases toward `posX`/`posY` offset by a per-frame-counter
 *     (`self+0x44`) lookup into `gSineTable` (two different
 *     index strides for the X/Y offsets, producing a scatter/orbit-style
 *     curve), and toward `posZ-0x200`.
 *   - state 1: snaps (no easing) directly to `posX` for the X axis, to
 *     `posY` plus a different table-driven offset for Y, and to
 *     `posZ+0x200` for Z.
 *   - any other state: eases toward `posX`/`posY` directly (no table
 *     offset), and toward `posZ+0x200`.
 * "Easing" is a round-toward-zero divide (by 16 for X/Y, by 4 for Z) of
 * the remaining delta, added back onto the cached position - the ROM's
 * own rsb/lsr/add/asr rounding idiom, same shape as `SetEntitySize` in
 * docs/matching.md.
 *
 * No register pins needed: the table offsets go through their own locals
 * (`ox`/`oy`) so they are summed before the position is added, as the
 * ROM does, and the explicit `goto ease_y` reproduces the ROM sharing
 * one copy of the Y/Z easing between state 0 and the default case. */
void MovePolarAkuAku(struct actor_self *self, s32 posX, s32 posY, s32 posZ)
{
    s32 tx, ty, tz;
    s32 cur, d;

    if (self->state == 0) {
        s32 ox = gSineTable[(self->stateTime * 4) & 0xff] * 24 - 0x1000;
        s32 oy;

        tx = posX + ox;
        oy = gSineTable[(self->stateTime * 2) & 0xff] * 10 - 0x1e00;
        ty = posY + oy;
        tz = posZ - 0x200;
        self->x += (tx - self->x) / 16;
        cur = self->y;
        d = ty - cur;
        goto ease_y;
    } else if (self->state == 1) {
        s32 oy;

        self->x = posX;
        oy = gSineTable[(self->stateTime * 9) & 0xff] * 4 - 0xa00;
        self->y = oy + posY;
        self->z = posZ + 0x200;
        return;
    }
    tz = posZ + 0x200;
    self->x += (posX - self->x) / 16;
    cur = self->y;
    d = posY - cur;
ease_y:
    self->y = cur + d / 16;
    self->z += (tz - self->z) / 4;
}

asm(".align 2, 0");
