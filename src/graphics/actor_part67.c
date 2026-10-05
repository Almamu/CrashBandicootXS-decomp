#include "core.h"
#include "actor_self.h"

/* Same `InitActorPart`-rooted per-instance "self" object family
 * documented in action_ctrl.c/actor_part28.c/actor_part32.c: a "part
 * table" pointer at `self+0`, a table-index/"kind" field at `self+0xc`,
 * an anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 * accumulator at `self+8`, state at `self+0x28`, a frame counter at
 * `self+0x44`, and a `+0x50`-rooted event/trampoline table fed through
 * `_call_via_r2`. This is the second object kind (constructed by the
 * parked `CreateHovercraftSideGun`, vtable `gHovercraftSideGunVtable`), with a health-
 * like countdown at `self+0x54`, a "dead" byte flag at `self+0x58`, a
 * `visible` byte at `self+0x2c`, the constructor's cached
 * gate byte at `self+0x59`, and a little "spawn/orbit" record at
 * `self+0x5c`/`self+0x60`/`self+0x64`/`self+0x68`/`self+0x6c` driving
 * `UpdateHovercraftSideGun`'s position-plus-effect-spawn step. See
 * docs/matching/issue-63-0x08033ef4-actor.md. */

extern void StartHovercraftHitFlash(void);
extern void LoseHovercraftPart(void);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *gAudioContext;

/* The second object kind (vtable gHovercraftSideGunVtable). */
struct actor_orbiter {
    struct actor_self base;
    s32 hp;             // 0x54
    u8 dead;            // 0x58
    u8 gate;            // 0x59 - the constructor's cached gate byte
    u8 unk_5A[2];
    s32 offX;           // 0x5C - added to the singleton's position
    s32 offY;           // 0x60
    s32 offZ;           // 0x64
    s32 orbitTimer;     // 0x68 - frames until the next effect spawn
    s32 lap;            // 0x6C
};

/* The singleton table GetHovercraftAttack returns. */
struct orbit_table {
    s32 unk_00;
    s32 period;         // 0x04 - orbitTimer reload within a lap cycle
    s32 laps;           // 0x08 - lap count that ends a cycle
    s32 cyclePeriod;    // 0x0C - orbitTimer reload once a cycle ends
};

/* Applies `dmg` damage to `self+0x54` and once it drops to zero (or
 * below): marks `self` dead (`+0x58=1`), sets `visible`
 * (`+0x2c=1`), fires the singleton's own death transition
 * (`LoseHovercraftPart`), and switches `self` to state 1, table-index 0 or 1
 * depending on the constructor's cached gate byte (`+0x59`), resetting
 * the anim-frame pair and playing the death sound; otherwise just plays
 * a hit sound. Same shape as `DamageHovercraftCannon` (actor_part30.c). */
void DamageHovercraftSideGun(void *selfArg, s32 dmg)
{
    struct actor_orbiter *self = selfArg;

    StartHovercraftHitFlash();
    self->hp -= dmg;

    if (self->hp <= 0) {
        u8 *flag;
        register s32 zero asm("r3");
        register s32 one asm("r1");
        s32 idx;

        LoseHovercraftPart();
        flag = &self->dead;
        zero = 0;
        one = 1;
        *flag = one;
        flag -= 0x2c;
        *flag = one;
        {
            u8 gate = flag[0x2d];

            idx = 1;
            if (gate != 0) {
                idx = 0;
            }
        }
        self->base.state = one;
        self->base.stateTime = zero;
        self->base.animIndex = idx;
        {
            register u16 anim asm("r0") = self->base.anims[idx].duration;
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

extern void UpdateActor(void *self);
extern s32 GetHovercraftX(void);
extern s32 GetHovercraftY(void);
extern s32 GetHovercraftZ(void);
extern void SpawnHovercraftFireball(s32 x, s32 y);
extern struct orbit_table *GetHovercraftAttack(void);

/* Per-frame position sync (`+0x1c`/`+0x20`/`+0x24` from the singleton's
 * position plus `self`'s own `+0x5c`/`+0x60`/`+0x64` offsets), calling
 * `UpdateActor(self)` first for the frame's regular update. While `self`
 * is still in state 0 and `+0x34` is over its `0x2800` threshold, drives
 * an "orbit" counter at `+0x68`: at zero, spawns an effect at the synced
 * position (`SpawnHovercraftFireball`) and advances a lap counter (`+0x6c`),
 * reseeding `+0x68` from the singleton table's `+4`/`+8`/`+0xc` fields
 * depending on whether the lap counter just reached the table's `+8`
 * entry; otherwise just decrements the orbit counter. */
void UpdateHovercraftSideGun(void *selfArg)
{
    register struct actor_orbiter *self asm("r5") = selfArg;

    UpdateActor(self);
    self->base.x = GetHovercraftX() + self->offX;
    self->base.y = GetHovercraftY() + self->offY;
    {
        s32 base = GetHovercraftZ();
        register s32 field asm("r1") = self->offZ;
        register s32 z asm("r2") = base + field;
        self->base.z = z;
    }

    if (self->base.state == 0 && self->base.depth > 0x2800) {
        register s32 origCounter asm("r6") = self->orbitTimer;
        register s32 result asm("r0");

        if (origCounter == 0) {
            register s32 lap asm("r4");

            SpawnHovercraftFireball(self->base.x, self->base.y);
            lap = self->lap + 1;
            self->lap = lap;

            if (lap == GetHovercraftAttack()->laps) {
                self->lap = origCounter;
                result = GetHovercraftAttack()->cyclePeriod;
            } else {
                result = GetHovercraftAttack()->period;
            }
        } else {
            result = origCounter - 1;
        }

        self->orbitTimer = result;
    }
}

/* Near-twin of `UpdateHovercraftSideGun` (same position-sync/orbit-effect shape),
 * but does not call `UpdateActor(self)` first - this object's regular
 * per-frame update is driven elsewhere. */
void sub_80341F8(void *selfArg)
{
    register struct actor_orbiter *self asm("r5") = selfArg;

    self->base.x = GetHovercraftX() + self->offX;
    self->base.y = GetHovercraftY() + self->offY;
    {
        s32 base = GetHovercraftZ();
        register s32 field asm("r1") = self->offZ;
        register s32 z asm("r2") = base + field;
        self->base.z = z;
    }

    if (self->base.state == 0 && self->base.depth > 0x2800) {
        register s32 origCounter asm("r6") = self->orbitTimer;
        register s32 result asm("r0");

        if (origCounter == 0) {
            register s32 lap asm("r4");

            SpawnHovercraftFireball(self->base.x, self->base.y);
            lap = self->lap + 1;
            self->lap = lap;

            if (lap == GetHovercraftAttack()->laps) {
                self->lap = origCounter;
                result = GetHovercraftAttack()->cyclePeriod;
            } else {
                result = GetHovercraftAttack()->period;
            }
        } else {
            result = origCounter - 1;
        }

        self->orbitTimer = result;
    }
}

/* Constant getter - returns `self`'s death flag (`self+0x58`) for this
 * object kind. */
u8 IsHovercraftSideGunUnshootable(void *selfArg)
{
    struct actor_orbiter *self = selfArg;

    return self->dead;
}

/* gHovercraftCannonFlashVtable slot 4, the damage handler: empty (the
 * cannon's muzzle flash can't be hurt). */
void DamageHovercraftCannonFlash(void)
{
}

asm(".align 2, 0");
