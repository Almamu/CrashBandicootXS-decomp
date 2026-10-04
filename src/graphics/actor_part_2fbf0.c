#include "core.h"
#include "actor_self.h"

/* 0x0802FBF0-0x08030530 (issue #57, plus issue #58's sub_8030334/
 * sub_803044C): the methods of three small C++ actor classes built on
 * the shared `struct actor_self` object (include/actor_self.h), plus the
 * orbiting-companion position updaters that open issue #58's
 * boss-weapon cluster. Each class is identified by the method table its
 * constructor installs at +0x50:
 *
 * - gStaticData_087E51B4 (`struct actor_51b4`, sub_802FBF0-sub_802FF00):
 *   a hopping pickup/hazard that ballistically jumps between the
 *   level's sub-effect target points (`sub_802A5xx` accessors - see
 *   docs/rom_map.md), with a "dying" flag and 4 hit points.
 * - gStaticData_087E51EC (`struct actor_51ec`, sub_802FF08-sub_8030290):
 *   a 2-hit-point object whose spawn kind (4-9) picks its initial
 *   state; its per-state movers circle a home point on the shared
 *   sine table gStaticData_0816A820 or drift toward the player.
 * - gStaticData_087E5224 (`struct actor_5224`, sub_8030298-sub_8030330):
 *   a straight-line projectile that damages the player on contact.
 *
 * Method-table and member-pointer calls are real indirect calls
 * (ACTOR_VCALL/ACTOR_PMF_CALL), which Thumb gcc emits as
 * `bl _call_via_rN` (src/system/reg_trampolines.c). */

extern s32 sub_802A504(s32 idx);
extern s32 sub_802A51C(s32 idx);
extern s32 sub_802A540(s32 idx);
extern s32 sub_802A558(s32 idx);
extern s32 sub_802A570(s32 idx);
extern s32 __divsi3(s32 a, s32 b);
extern s32 GetAnimFrameBaseOffset(void *self);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void InitActorPart(void *self, void *part, s32 b, s32 c, s32 d);
extern u8 sub_802A6EC(void *self);
extern void sub_802A7B8(void *self);
/* Defined as a no-argument counter in actor_part44.c, but the ROM passes
 * the player object here (a C++ method ignoring its `this`). */
extern s32 sub_802F46C(void *player);

extern s32 gUnknown_0300089C[];
extern void *gUnknown_030012BC;
extern struct actor_self *gUnknown_03000884;
extern s16 gStaticData_0816A820[];
extern struct actor_pmf gStaticData_0817C260[];
extern struct actor_pmf gStaticData_0817C280[];
extern u8 gStaticData_087E51B4[];
extern u8 gStaticData_087E51EC[];
extern u8 gStaticData_087E5224[];

struct actor_51b4 {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 unk_58;         // 0x58
    s32 unk_5C;         // 0x5C
    s32 velX;           // 0x60
    s32 velY;           // 0x64
    s32 speed;          // 0x68
    s32 accX;           // 0x6C
    s32 accY;           // 0x70
    s32 steps;          // 0x74
    s32 next;           // 0x78
    u8 dying;           // 0x7C
};

struct actor_51ec {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 homeX;          // 0x58
    s32 homeY;          // 0x5C
    u8 unk_60;          // 0x60
};

struct actor_5224 {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 velX;           // 0x58
    s32 velY;           // 0x5C
};

struct actor_orbit {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 centerX;        // 0x58
    s32 centerY;        // 0x5C
    s32 velZ;           // 0x60
    s32 radius;         // 0x64
};

struct spawn_arg {
    u8 unk_00[0x10];
    s32 target;         // 0x10
};

/* Aims the next hop at sub-effect target `target`: looks up the hop
 * speed for that target's kind, derives the step count from the height
 * difference, and solves for per-step accelerations that land on the
 * target's X/Y after that many steps. A negative target ends the chain
 * (idle state 1 if the hop speed is low, else a practically endless
 * glide). Finally restarts the "low" (3) or "high" (0) animation. */
void sub_802FBF0(struct actor_51b4 *self, s32 target)
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
        self->accX = ((((sub_802A558(target) - self->base.x) - ((self->velX * self->steps) >> 4)) * scale >> 13) * (scale2 = scale * 2)) >> 13;
        self->accY = ((((sub_802A540(target) - self->base.y) - ((self->velY * self->steps) >> 4)) * scale >> 13) * scale2) >> 13;
        self->next = sub_802A504(target);
    }

    if (self->speed <= 0x955) {
        self->base.animIndex = 3;
        self->base.animTimer = self->base.anims[3].duration;
        self->base.animDone = 0;
        if (GetAnimFrameBaseOffset(self) >= self->base.anims[self->base.animIndex].loopThreshold) {
            self->base.animTime = 0;
        }
    } else {
        self->base.animIndex = 0;
        self->base.animTimer = self->base.anims[0].duration;
        self->base.animDone = 0;
        if (GetAnimFrameBaseOffset(self) >= self->base.anims[self->base.animIndex].loopThreshold) {
            self->base.animTime = 0;
        }
    }
}

/* Damage handler: once hit points run out, marks the object dying,
 * halves its velocity (only an upward Y velocity) and plays the
 * knock-out animation (1 or 4, matching the current pose) in state 2. */
void sub_802FD1C(struct actor_51b4 *self, s32 damage)
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
    PlaySfx(gUnknown_030012BC, 0x25, 0x100);
}

/* Constructor: 4 hit points; a spawn whose first target needs a fast
 * hop starts higher up and with the faster speed. The constant 4 is
 * pinned to r4 so it is loaded before the InitActorPart call like the
 * ROM does (docs/workflow.md). */
void *sub_802FD8C(struct actor_51b4 *self, void *part, s32 b, s32 c, s32 d, struct spawn_arg *arg)
{
    register s32 four asm("r4") = 4;

    InitActorPart(self, part, b, c, d);
    self->hp = four;
    self->base.vtable = (struct actor_vtable *)gStaticData_087E51B4;
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
    sub_802FBF0(self, arg->target);
    return self;
}

/* Applies Y acceleration, capped at 0x1400. */
void sub_802FE04(struct actor_51b4 *self)
{
    self->velY += self->accY;
    if (self->velY > 0x1400) {
        self->velY = 0x1400;
    }
}

/* When the current animation finishes, switches to the landing
 * animation (2 or 5) in state 3 with a fixed Y acceleration. */
void sub_802FE1C(struct actor_51b4 *self)
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
void sub_802FE58(struct actor_51b4 *self)
{
    struct actor_self *player = gUnknown_03000884;

    self->velX = (player->x - self->base.x) >> 4;
    self->velY = (player->y - self->base.y) >> 4;
}

/* Integrates acceleration into velocity; aims the next hop once the
 * step count runs out. */
void sub_802FE78(struct actor_51b4 *self)
{
    self->velX += self->accX;
    self->velY += self->accY;
    if (--self->steps <= 0) {
        sub_802FBF0(self, self->next);
    }
}

/* Per-frame update: calls this state's gStaticData_0817C260 handler. */
void sub_802FEA4(struct actor_51b4 *self)
{
    ACTOR_PMF_CALL(&self->base, gStaticData_0817C260);
}

/* Getter for the dying flag. */
u8 sub_802FF00(struct actor_51b4 *self)
{
    return self->dying;
}

/* Constructor: 2 hit points, home point = (b, c); the spawn record's
 * kind byte (4-9) selects the starting state (0-5). `part`/`b`/`c` and
 * the constant 2 are pinned to the ROM's r8/r5/r6/r4, the stack
 * argument to r0 after them (same fix as sub_80305F8), and the kind
 * byte is read through r1 as in the ROM (docs/workflow.md). */
void *sub_802FF08(struct actor_51ec *self, u8 *part, s32 b, s32 c, s32 d)
{
    register u8 *partReg asm("r8") = part;
    register s32 bReg asm("r5") = b;
    register s32 cReg asm("r6") = c;
    register s32 dReg asm("r0") = d;
    register s32 two asm("r4") = 2;
    s32 kind;

    InitActorPart(self, part, b, c, dReg);
    self->hp = two;
    self->base.vtable = (struct actor_vtable *)gStaticData_087E51EC;
    self->homeX = bReg;
    self->homeY = cReg;
    self->unk_60 = 0;
    {
        register u8 *kindPtr asm("r1") = partReg;
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

/* Per-frame update: while alive, ticks sub_802F46C and on player
 * contact damages the player (strength 10) and switches to the dying
 * state 6. Then runs this state's gStaticData_0817C280 handler; once
 * the dying animation is done, calls its own "destroy" method,
 * otherwise sinks slightly and runs the standard sub_802A7B8 step. */
void sub_802FFB8(struct actor_51ec *self)
{
    if (self->base.state != 6) {
        sub_802F46C(gUnknown_03000884);
        if (self->base.state != 6 && sub_802A6EC(self)) {
            ACTOR_VCALL(gUnknown_03000884, m20, 10);
            self->base.unk_18 = 4;
            PlaySfx(gUnknown_030012BC, 4, 0x100);
            ACTOR_SET_STATE(&self->base, 6, 1);
        }
    }

    ACTOR_PMF_CALL(&self->base, gStaticData_0817C280);

    if (self->base.state == 6 && self->base.animDone != 0) {
        if (self != NULL) {
            ACTOR_VCALL(&self->base, m08, 3);
        }
    } else {
        self->base.z += 0x60;
        sub_802A7B8(self);
    }
}

/* Eases `self` 1/32 of the way toward the player. */
void sub_80300B0(struct actor_51ec *self)
{
    struct actor_self *player = gUnknown_03000884;

    self->base.x += (player->x - self->base.x) >> 5;
    self->base.y += (player->y - self->base.y) >> 5;
}

void nullsub_28(struct actor_51ec *self)
{
}

/* Moves `self` down by 0x88. */
void sub_80300D8(struct actor_51ec *self)
{
    self->base.z -= 0x88;
}

/* Circles `self` around its home point (radius 60) while above a depth
 * threshold, otherwise homes in on the player. */
void sub_80300E0(struct actor_51ec *self)
{
    if (self->base.depth > 0x35ff) {
        s16 *sine = gStaticData_0816A820;
        s32 angle = ((self->base.stateTime << 4) >> 4) & 0xff;

        self->base.x = self->homeX + sine[(angle + 0x40) & 0xff] * 60;
        self->base.y = self->homeY + sine[angle] * 60;
    } else {
        sub_80300B0(self);
    }
}

/* Horizontal-only variant of sub_80300E0 (radius 80, slower phase). */
void sub_803013C(struct actor_51ec *self)
{
    if (self->base.depth > 0x35ff) {
        self->base.x = self->homeX + gStaticData_0816A820[((((self->base.stateTime * 10) >> 4) & 0xff) + 0x40) & 0xff] * 80;
    } else {
        sub_80300B0(self);
    }
}

/* Vertical-only variant of sub_80300E0. */
void sub_8030188(struct actor_51ec *self)
{
    if (self->base.depth > 0x35ff) {
        self->base.y = self->homeY + gStaticData_0816A820[((self->base.stateTime << 4) >> 4) & 0xff] * 60;
    } else {
        sub_80300B0(self);
    }
}

/* Homes in on the player only while at or below the depth threshold. */
void sub_80301CC(struct actor_51ec *self)
{
    if (self->base.depth <= 0x35ff) {
        sub_80300B0(self);
    }
}

void nullsub_29(struct actor_51ec *self)
{
}

/* Damage handler: when hit points run out, enters the dying state 6. */
void sub_80301EC(struct actor_51ec *self, s32 damage)
{
    if (self->base.state != 6 && (self->hp -= damage) <= 0) {
        self->base.unk_18 = 4;
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        ACTOR_SET_STATE(&self->base, 6, 1);
    }
}

/* Calls this state's gStaticData_0817C280 handler. */
void sub_8030234(struct actor_51ec *self)
{
    ACTOR_PMF_CALL(&self->base, gStaticData_0817C280);
}

/* Getter for the +0x60 flag byte. */
u8 sub_8030290(struct actor_51ec *self)
{
    return self->unk_60;
}

/* Projectile step: moves by its velocity and falls; on player contact
 * damages the player (strength 2) and destroys itself, otherwise runs
 * the standard sub_802A7B8 step. */
void sub_8030298(struct actor_5224 *self)
{
    self->base.x += self->velX;
    self->base.y += self->velY;
    self->base.z += -0x100;
    if (sub_802A6EC(self)) {
        ACTOR_VCALL(gUnknown_03000884, m20, 2);
        if (self != NULL) {
            ACTOR_VCALL(&self->base, m08, 3);
        }
    } else {
        sub_802A7B8(self);
    }
}

/* Constructor: 1 hit point and the given velocity - the same shape as
 * sub_802FA04 (actor_part45c.c), matched with the same register
 * arrangement (the constant pinned to r5, the two stack arguments left
 * to the allocator). */
void *sub_8030300(struct actor_5224 *self, void *part, s32 b, s32 c, s32 d, s32 velX, s32 velY)
{
    register s32 one asm("r5") = 1;
    register s32 vx = velX;
    register s32 vy = velY;

    InitActorPart(self, part, b, c, d);
    self->hp = one;
    self->base.vtable = (struct actor_vtable *)gStaticData_087E5224;
    self->velX = vx;
    self->velY = vy;
    return self;
}

/* Constant-true predicate. */
s32 sub_8030330(struct actor_5224 *self)
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
void sub_8030334(struct actor_orbit *self)
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
        struct actor_self *player = gUnknown_03000884;
        s32 px, py, tx, ty;
        register s32 cx asm("r3");
        register s32 cy asm("r4");
        register s16 *sine asm("r5");

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
        sine = gStaticData_0816A820;
        t = self->base.stateTime;
        angle = ((t << 5) >> 4) & 0xff;
        self->base.x = cx + ((sine[(angle + 0x40) & 0xff] * self->radius) >> 8);
        self->base.y = cy + ((sine[angle] * self->radius) >> 8);
    }
    if (self->base.depth <= 0x2bff) {
        ACTOR_SET_STATE(&self->base, 1, 0);
        self->base.stateTime = t;
    }
    if (sub_802A6EC(self)) {
        ACTOR_VCALL(gUnknown_03000884, m20, 6);
        self->base.unk_18 = 4;
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        ACTOR_SET_STATE(&self->base, 2, 1);
    }
}

/* Spiral-in variant of sub_8030334 (issue #58): the radius shrinks by
 * 0x100 per frame down to 0, the centre eases 1/16 of the way, and
 * there is no depth check. Same register pins as sub_8030334. */
void sub_803044C(struct actor_orbit *self)
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
        struct actor_self *player = gUnknown_03000884;
        s32 px, py, tx, ty;
        register s32 cx asm("r3");
        register s32 cy asm("r4");
        register s16 *sine asm("r5");

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
        sine = gStaticData_0816A820;
        angle = ((self->base.stateTime << 5) >> 4) & 0xff;
        self->base.x = cx + ((sine[(angle + 0x40) & 0xff] * self->radius) >> 8);
        self->base.y = cy + ((sine[angle] * self->radius) >> 8);
    }
    if (sub_802A6EC(self)) {
        ACTOR_VCALL(gUnknown_03000884, m20, 6);
        self->base.unk_18 = 4;
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        ACTOR_SET_STATE(&self->base, 2, 1);
    }
}

asm(".align 2, 0");
