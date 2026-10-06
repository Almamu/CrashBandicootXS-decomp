#include "core.h"
#include "match.h"
#include "actor_self.h"
#include <libgcc.h>
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "bosses.h"
#include "globals.h"

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment, docs/matching/archive/issue-56-0x0802f0dc-actor.md and
 * docs/matching/archive/pmf-dispatch-retry.md. */

struct actor_fa38 {
    struct actor_self base;
    s32 hp;       // 0x54
    s32 cooldown; // 0x58
    s32 hits;     // 0x5C
    s32 velX;     // 0x60
    s32 velY;     // 0x64
    s32 velZ;     // 0x68
    u8 unk_6C[0x10];
    u8 unk_7C; // 0x7C
};

/* Per-frame update: flags `self` as "deep" past a depth threshold,
 * integrates its velocity (Q4), runs the per-state member-pointer
 * dispatch `(this->*gJetpackPlaneStateFuncs[this->state])()`, and while
 * animation 3 plays and the cooldown has run out, pushes the player
 * away (SpawnJetpackCannonball) when it is close in front - every third hit takes
 * a long cooldown. Then the usual player-contact damage exchange, and
 * finally "destroy" once state 3 rises past a height, else the
 * standard UpdateActor step. */
void UpdateJetpackPlane(struct actor_fa38 *self)
{
    if (self->base.depth > 0x1B00) {
        self->base.visible = 1;
    } else {
        self->base.visible = 0;
    }
    self->base.x += self->velX >> 4;
    self->base.y += self->velY >> 4;
    self->base.z += self->velZ >> 4;

    ACTOR_PMF_CALL(&self->base, gJetpackPlaneStateFuncs);

    if (self->base.animIndex == 3) {
        s32 cooldown = self->cooldown;

        if (cooldown == 0) {
            struct actor_self *player = gActorList;
            s32 angle = (player->z - (self->base.z - 10)) / -0x1AA;

            if (angle > 0 && self->base.depth <= 0x8BFF) {
                s32 scale = 0x1000 / angle;
                s32 rawDx = (player->x - self->base.x) * scale;
                s32 dx = rawDx >> 12;
                s32 rawDy = (player->y - self->base.y) * scale;
                s32 dy = rawDy >> 12;
                s32 signDx = rawDx >> 31;
                s32 absDx = (dx ^ signDx) - signDx;
                s32 signDy = rawDy >> 31;
                s32 absDy = (dy ^ signDy) - signDy;

                if (absDx + absDy <= 0x5FF) {
                    SpawnJetpackCannonball(self->base.x, self->base.y, self->base.z - 10, dx, dy);
                    if (++self->hits == 3) {
                        self->hits = cooldown;
                        self->cooldown = 0x3C;
                    } else {
                        self->cooldown = 0x14;
                    }
                }
            }
        } else {
            self->cooldown = cooldown - 1;
        }
    }

    if (self->unk_7C == 0 && (u8)IsTouchingPlayer(self)) {
        ACTOR_VCALL(gActorList, m20, 6);
        ACTOR_VCALL(&self->base, m20, 4);
    }

    if (self->base.state == 3 && self->base.y > 0xE100) {
        if (self != NULL) {
            ACTOR_VCALL(&self->base, destroy, 3);
        }
    } else {
        UpdateActor(self);
    }
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");

/* 0x0802FBF0-0x08030530 (issue #57, plus issue #58's AirshipFireballStateOrbit/
 * AirshipFireballStateSpiralIn): the methods of three small C++ actor classes built on
 * the shared `struct actor_self` object (include/actor_self.h), plus the
 * orbiting-companion position updaters that open issue #58's
 * boss-weapon cluster. Each class is identified by the method table its
 * constructor installs at +0x50:
 *
 * - gJetpackPlaneVtable (`struct jetpack_plane`, AimJetpackPlane-IsJetpackPlaneUnshootable):
 *   a hopping pickup/hazard that ballistically jumps between the
 *   level's sub-effect target points (`sub_802A5xx` accessors - see
 *   docs/rom_map.md), with a "dying" flag and 4 hit points.
 * - gJetpackBomberVtable (`struct jetpack_bomber`, CreateJetpackBomber-IsJetpackBomberUnshootable):
 *   a 2-hit-point object whose spawn kind (4-9) picks its initial
 *   state; its per-state movers circle a home point on the shared
 *   sine table gSineTable or drift toward the player.
 * - gJetpackCannonballVtable (`struct jetpack_cannonball`, UpdateJetpackCannonball-IsJetpackCannonballUnshootable):
 *   a straight-line projectile that damages the player on contact.
 *
 * Method-table and member-pointer calls are real indirect calls
 * (ACTOR_VCALL/ACTOR_PMF_CALL), which Thumb gcc emits as
 * `bl _call_via_rN` (lib/libgcc/lib1funcs.s). */

struct jetpack_plane {
    struct actor_self base;
    s32 hp;     // 0x54
    s32 unk_58; // 0x58
    s32 unk_5C; // 0x5C
    s32 velX;   // 0x60
    s32 velY;   // 0x64
    s32 speed;  // 0x68
    s32 accX;   // 0x6C
    s32 accY;   // 0x70
    s32 steps;  // 0x74
    s32 next;   // 0x78
    u8 dying;   // 0x7C
};

struct jetpack_bomber {
    struct actor_self base;
    s32 hp;    // 0x54
    s32 homeX; // 0x58
    s32 homeY; // 0x5C
    u8 unk_60; // 0x60
};

struct jetpack_cannonball {
    struct actor_self base;
    s32 hp;   // 0x54
    s32 velX; // 0x58
    s32 velY; // 0x5C
};

struct actor_orbit {
    struct actor_self base;
    s32 hp;      // 0x54
    s32 centerX; // 0x58
    s32 centerY; // 0x5C
    s32 velZ;    // 0x60
    s32 radius;  // 0x64
};

/* Aims the next hop at sub-effect target `target`: looks up the hop
 * speed for that target's kind, derives the step count from the height
 * difference, and solves for per-step accelerations that land on the
 * target's X/Y after that many steps. A negative target ends the chain
 * (idle state 1 if the hop speed is low, else a practically endless
 * glide). Finally restarts the "low" (3) or "high" (0) animation. */
void AimJetpackPlane(struct jetpack_plane *self, s32 target)
{
    if (target < 0) {
        if (self->speed <= 0x955) {
            ACTOR_SET_STATE(&self->base, 1, 3);
        } else {
            self->accX = 0;
            self->accY = 0;
            self->steps = 0x40000000;
        }
    } else {
        s32 scale;
        s32 scale2;

        self->speed = gUnknown_0300089C[sub_802A570(target)];
        self->steps = __divsi3((sub_802A51C(target) - self->base.z) << 8, self->speed) >> 4;
        if (self->steps == 0) {
            self->steps = 1;
        }
        {
            s32 steps = self->steps;
            scale = __divsi3(0x8000, steps);
        }
        // clang-format off
        self->accX = ((((sub_802A558(target) - self->base.x) -
                        ((self->velX * self->steps) >> 4)) * scale >> 13) *
                      (scale2 = scale * 2)) >> 13;
        self->accY = ((((sub_802A540(target) - self->base.y) -
                        ((self->velY * self->steps) >> 4)) * scale >> 13) *
                      scale2) >> 13;
        // clang-format on
        self->next = sub_802A504(target);
    }

    if (self->speed <= 0x955) {
        self->base.animIndex = 3;
        self->base.animTimer = self->base.anims[3].duration;
        self->base.animDone = 0;
        if (GetAnimFrameBaseOffset((struct actor_self *)self) >=
            self->base.anims[self->base.animIndex].loopThreshold) {
            self->base.animTime = 0;
        }
    } else {
        self->base.animIndex = 0;
        self->base.animTimer = self->base.anims[0].duration;
        self->base.animDone = 0;
        if (GetAnimFrameBaseOffset((struct actor_self *)self) >=
            self->base.anims[self->base.animIndex].loopThreshold) {
            self->base.animTime = 0;
        }
    }
}

/* Damage handler: once hit points run out, marks the object dying,
 * halves its velocity (only an upward Y velocity) and plays the
 * knock-out animation (1 or 4, matching the current pose) in state 2. */
void DamageJetpackPlane(struct jetpack_plane *self, s32 damage)
{
    s32 idx;

    if ((self->hp -= damage) > 0) {
        return;
    }
    self->dying = 1;
    self->velX /= 2;
    if (self->velY < 0) {
        self->velY /= 2;
    }
    if (self->base.animIndex == 0) {
        idx = 1;
    } else {
        idx = 4;
    }
    ACTOR_SET_STATE(&self->base, 2, idx);
    PlaySfx(gAudioContext, 0x25, 0x100);
}

/* Constructor: 4 hit points; a spawn whose first target needs a fast
 * hop starts higher up and with the faster speed. The constant 4 is
 * pinned to r4 so it is loaded before the InitActorPart call like the
 * ROM does (docs/workflow.md). */
void *CreateJetpackPlane(struct jetpack_plane *self, void *part, s32 b, s32 c, s32 d,
                         struct spawn_arg *arg)
{
    MATCH_HOLD_REG(s32, four, r4) = 4;

    InitActorPart(self, part, b, c, d);
    self->hp = four;
    self->base.vtable = (struct actor_vtable *)gJetpackPlaneVtable;
    self->unk_58 = 0x3c;
    self->unk_5C = 0;
    self->dying = 0;
    self->velY = 0;
    self->velX = 0;
    self->speed = 0x955;
    if (arg->target >= 0 && gUnknown_0300089C[sub_802A570(arg->target)] > 0x955) {
        self->base.z += -0x8e00;
        self->speed = 0xd55;
    }
    AimJetpackPlane(self, arg->target);
    return self;
}

/* Applies Y acceleration, capped at 0x1400. */
void JetpackPlaneStateFall(struct jetpack_plane *self)
{
    self->velY += self->accY;
    if (self->velY > 0x1400) {
        self->velY = 0x1400;
    }
}

/* When the current animation finishes, switches to the landing
 * animation (2 or 5) in state 3 with a fixed Y acceleration. */
void sub_802FE1C(struct jetpack_plane *self)
{
    s32 idx;

    if (self->base.animDone == 0) {
        return;
    }
    self->accY = 0xa0;
    if (self->base.animIndex == 1) {
        idx = 2;
    } else {
        idx = 5;
    }
    ACTOR_SET_STATE(&self->base, 3, idx);
}

/* Sets the velocity to 1/16 of the offset to the player. */
void sub_802FE58(struct jetpack_plane *self)
{
    struct actor_self *player = gActorList;

    self->velX = (player->x - self->base.x) >> 4;
    self->velY = (player->y - self->base.y) >> 4;
}

/* Integrates acceleration into velocity; aims the next hop once the
 * step count runs out. */
void JetpackPlaneStateFly(struct jetpack_plane *self)
{
    self->velX += self->accX;
    self->velY += self->accY;
    if (--self->steps <= 0) {
        AimJetpackPlane(self, self->next);
    }
}

/* Per-frame update: calls this state's gJetpackPlaneStateFuncs handler. */
void RunJetpackPlaneState(struct jetpack_plane *self)
{
    ACTOR_PMF_CALL(&self->base, gJetpackPlaneStateFuncs);
}

/* Getter for the dying flag. */
u8 IsJetpackPlaneUnshootable(struct jetpack_plane *self)
{
    return self->dying;
}

/* Constructor: 2 hit points, home point = (b, c); the spawn record's
 * kind byte (4-9) selects the starting state (0-5). `part`/`b`/`c` and
 * the constant 2 are pinned to the ROM's r8/r5/r6/r4, the stack
 * argument to r0 after them (same fix as CreateAirshipFireball), and the kind
 * byte is read through r1 as in the ROM (docs/workflow.md). */
void *CreateJetpackBomber(struct jetpack_bomber *self, u8 *part, s32 b, s32 c, s32 d)
{
    MATCH_HOLD_REG(u8 *, partReg, r8) = part;
    MATCH_HOLD_REG(s32, bReg, r5) = b;
    MATCH_HOLD_REG(s32, cReg, r6) = c;
    MATCH_HOLD_REG(s32, dReg, r0) = d;
    MATCH_HOLD_REG(s32, two, r4) = 2;
    s32 kind;

    InitActorPart(self, part, b, c, dReg);
    self->hp = two;
    self->base.vtable = (struct actor_vtable *)gJetpackBomberVtable;
    self->homeX = bReg;
    self->homeY = cReg;
    self->unk_60 = 0;
    {
        MATCH_HOLD_REG(u8 *, kindPtr, r1) = partReg;
        kind = *kindPtr;
    }
    switch (kind) {
    case 4:
        ACTOR_SET_STATE(&self->base, 0, 0);
        break;
    case 5:
        ACTOR_SET_STATE(&self->base, 1, 0);
        break;
    case 6:
        ACTOR_SET_STATE(&self->base, 2, 0);
        break;
    case 7:
        ACTOR_SET_STATE(&self->base, 3, 0);
        break;
    case 8:
        ACTOR_SET_STATE(&self->base, 4, 0);
        break;
    case 9:
        ACTOR_SET_STATE(&self->base, 5, 0);
        break;
    }
    return self;
}

/* Per-frame update: while alive, ticks CountJetpackBomber and on player
 * contact damages the player (strength 10) and switches to the dying
 * state 6. Then runs this state's gJetpackBomberStateFuncs handler; once
 * the dying animation is done, calls its own "destroy" method,
 * otherwise sinks slightly and runs the standard UpdateActor step. */
void UpdateJetpackBomber(struct jetpack_bomber *self)
{
    if (self->base.state != 6) {
        CountJetpackBomber(gActorList);
        if (self->base.state != 6 && (u8)IsTouchingPlayer(self)) {
            ACTOR_VCALL(gActorList, m20, 10);
            self->base.palette = 4;
            PlaySfx(gAudioContext, 4, 0x100);
            ACTOR_SET_STATE(&self->base, 6, 1);
        }
    }

    ACTOR_PMF_CALL(&self->base, gJetpackBomberStateFuncs);

    if (self->base.state == 6 && self->base.animDone != 0) {
        if (self != NULL) {
            ACTOR_VCALL(&self->base, destroy, 3);
        }
    } else {
        self->base.z += 0x60;
        UpdateActor(self);
    }
}

/* Eases `self` 1/32 of the way toward the player. */
void HomeJetpackBomber(struct jetpack_bomber *self)
{
    struct actor_self *player = gActorList;

    self->base.x += (player->x - self->base.x) >> 5;
    self->base.y += (player->y - self->base.y) >> 5;
}

/* gJetpackBomberStateFuncs[6]: the dying state UpdateJetpackBomber and
 * DamageJetpackBomber enter; UpdateJetpackBomber destroys the bomber once
 * its animation is done. Empty. */
void JetpackBomberStateDying(struct jetpack_bomber *self)
{
}

/* Moves `self` down by 0x88. */
void JetpackBomberStateDrop(struct jetpack_bomber *self)
{
    self->base.z -= 0x88;
}

/* Circles `self` around its home point (radius 60) while above a depth
 * threshold, otherwise homes in on the player. */
void JetpackBomberStateCircle(struct jetpack_bomber *self)
{
    if (self->base.depth > 0x35ff) {
        const s16 *sine = gSineTable;
        s32 angle = ((self->base.stateTime << 4) >> 4) & 0xff;

        self->base.x = self->homeX + sine[(angle + 0x40) & 0xff] * 60;
        self->base.y = self->homeY + sine[angle] * 60;
    } else {
        HomeJetpackBomber(self);
    }
}

/* Horizontal-only variant of JetpackBomberStateCircle (radius 80, slower phase). */
void JetpackBomberStateSwingHorizontal(struct jetpack_bomber *self)
{
    if (self->base.depth > 0x35ff) {
        self->base.x = self->homeX +
                       gSineTable[((((self->base.stateTime * 10) >> 4) & 0xff) + 0x40) & 0xff] * 80;
    } else {
        HomeJetpackBomber(self);
    }
}

/* Vertical-only variant of JetpackBomberStateCircle. */
void JetpackBomberStateBobVertical(struct jetpack_bomber *self)
{
    if (self->base.depth > 0x35ff) {
        self->base.y = self->homeY + gSineTable[((self->base.stateTime << 4) >> 4) & 0xff] * 60;
    } else {
        HomeJetpackBomber(self);
    }
}

/* Homes in on the player only while at or below the depth threshold. */
void JetpackBomberStateHome(struct jetpack_bomber *self)
{
    if (self->base.depth <= 0x35ff) {
        HomeJetpackBomber(self);
    }
}

/* gJetpackBomberStateFuncs[0], the state of kind-4 bombers: no movement
 * of its own (UpdateJetpackBomber's common step still applies). Empty. */
void JetpackBomberStateIdle(struct jetpack_bomber *self)
{
}

/* Damage handler: when hit points run out, enters the dying state 6. */
void DamageJetpackBomber(struct jetpack_bomber *self, s32 damage)
{
    if (self->base.state != 6 && (self->hp -= damage) <= 0) {
        self->base.palette = 4;
        PlaySfx(gAudioContext, 4, 0x100);
        ACTOR_SET_STATE(&self->base, 6, 1);
    }
}

/* Calls this state's gJetpackBomberStateFuncs handler. */
void RunJetpackBomberState(struct jetpack_bomber *self)
{
    ACTOR_PMF_CALL(&self->base, gJetpackBomberStateFuncs);
}

/* Getter for the +0x60 flag byte. */
u8 IsJetpackBomberUnshootable(struct jetpack_bomber *self)
{
    return self->unk_60;
}

/* Projectile step: moves by its velocity and falls; on player contact
 * damages the player (strength 2) and destroys itself, otherwise runs
 * the standard UpdateActor step. */
void UpdateJetpackCannonball(struct jetpack_cannonball *self)
{
    self->base.x += self->velX;
    self->base.y += self->velY;
    self->base.z += -0x100;
    if ((u8)IsTouchingPlayer(self)) {
        ACTOR_VCALL(gActorList, m20, 2);
        if (self != NULL) {
            ACTOR_VCALL(&self->base, destroy, 3);
        }
    } else {
        UpdateActor(self);
    }
}

/* Constructor: 1 hit point and the given velocity - the same shape as
 * CreateJetpackShot (jetpack_shot.c), matched with the same register
 * arrangement (the constant pinned to r5, the two stack arguments left
 * to the allocator). */
void *CreateJetpackCannonball(struct jetpack_cannonball *self, void *part, s32 b, s32 c, s32 d,
                              s32 velX, s32 velY)
{
    MATCH_HOLD_REG(s32, one, r5) = 1;
    register s32 vx = velX;
    register s32 vy = velY;

    InitActorPart(self, part, b, c, d);
    self->hp = one;
    self->base.vtable = (struct actor_vtable *)gJetpackCannonballVtable;
    self->velX = vx;
    self->velY = vy;
    return self;
}

/* Constant-true predicate. */
s32 IsJetpackCannonballUnshootable(struct jetpack_cannonball *self)
{
    return 1;
}

/* Orbiting-companion update (issue #58): rises with a decaying Z
 * velocity, grows its orbit radius up to 0x2A00, eases its orbit
 * centre 1/32 of the way toward an offset from the player, and places
 * itself on the circle at an angle driven by `stateTime`. Below a
 * depth threshold it returns to state 1 (keeping its angle); on player
 * contact it damages the player (strength 6) and enters state 2.
 * The centre/target are split into separate locals and the centre and
 * sine-table pointer pinned to r3/r4/r5 so that gcc neither folds
 * `px - (cx - 0x600)` into `(px + 0x600) - cx` nor moves the loads
 * (docs/workflow.md). */
void AirshipFireballStateOrbit(struct actor_orbit *self)
{
    s32 angle;
    s32 t;

    self->base.z += self->velZ;
    if ((self->velZ -= 5) <= 0x13) {
        self->velZ = 0x14;
    }
    if ((self->radius += 0x100) > 0x2a00) {
        self->radius = 0x2a00;
    }
    {
        struct actor_self *player = gActorList;
        s32 px, py, tx, ty;
        MATCH_HOLD_REG(s32, cx, r3);
        MATCH_HOLD_REG(s32, cy, r4);
        MATCH_HOLD_REG(const s16 *, sine, r5);

        px = player->x;
        cx = self->centerX;
        tx = cx + -0x600;
        cx += (px - tx) >> 5;
        self->centerX = cx;
        py = player->y;
        cy = self->centerY;
        ty = cy + 0x800;
        cy += (py - ty) >> 5;
        self->centerY = cy;
        sine = gSineTable;
        t = self->base.stateTime;
        angle = ((t << 5) >> 4) & 0xff;
        self->base.x = cx + ((sine[(angle + 0x40) & 0xff] * self->radius) >> 8);
        self->base.y = cy + ((sine[angle] * self->radius) >> 8);
    }
    if (self->base.depth <= 0x2bff) {
        ACTOR_SET_STATE(&self->base, 1, 0);
        self->base.stateTime = t;
    }
    if ((u8)IsTouchingPlayer(self)) {
        ACTOR_VCALL(gActorList, m20, 6);
        self->base.palette = 4;
        PlaySfx(gAudioContext, 4, 0x100);
        ACTOR_SET_STATE(&self->base, 2, 1);
    }
}

/* Spiral-in variant of AirshipFireballStateOrbit (issue #58): the radius shrinks by
 * 0x100 per frame down to 0, the centre eases 1/16 of the way, and
 * there is no depth check. Same register pins as AirshipFireballStateOrbit. */
void AirshipFireballStateSpiralIn(struct actor_orbit *self)
{
    s32 angle;

    self->base.z += self->velZ;
    if ((self->velZ -= 5) <= 0x13) {
        self->velZ = 0x14;
    }
    if ((self->radius += -0x100) < 0) {
        self->radius = 0;
    }
    {
        struct actor_self *player = gActorList;
        s32 px, py, tx, ty;
        MATCH_HOLD_REG(s32, cx, r3);
        MATCH_HOLD_REG(s32, cy, r4);
        MATCH_HOLD_REG(const s16 *, sine, r5);

        px = player->x;
        cx = self->centerX;
        tx = cx + -0x600;
        cx += (px - tx) >> 4;
        self->centerX = cx;
        py = player->y;
        cy = self->centerY;
        ty = cy + 0x800;
        cy += (py - ty) >> 4;
        self->centerY = cy;
        sine = gSineTable;
        angle = ((self->base.stateTime << 5) >> 4) & 0xff;
        self->base.x = cx + ((sine[(angle + 0x40) & 0xff] * self->radius) >> 8);
        self->base.y = cy + ((sine[angle] * self->radius) >> 8);
    }
    if ((u8)IsTouchingPlayer(self)) {
        ACTOR_VCALL(gActorList, m20, 6);
        self->base.palette = 4;
        PlaySfx(gAudioContext, 4, 0x100);
        ACTOR_SET_STATE(&self->base, 2, 1);
    }
}

asm(".align 2, 0");
