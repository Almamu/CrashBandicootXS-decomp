#include "core.h"
#include "actor_self.h"
#include "audio.h"

/* Same "self" object family as hovercraft_launcher.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

/* Constructor: health defaults to `0x10`, or `0x18` if the
 * `gHovercraft` singleton hasn't been constructed yet
 * (`GetHovercraftLevel() == 0`). Forwards to `InitActorPart`, sets the event
 * table (`+0x50=&gHovercraftSideGunVtable`), caches the constructor's 6th
 * (byte, stack-passed) argument at `+0x59`, selects table-index 0 or 1
 * depending on whether that byte is set, resets the usual state/frame-
 * counter/anim fields, clears the death flag (`+0x58=0`), seeds the
 * "spawn/orbit" record (`+0x5c` from the `+0x59` byte, `+0x60=0xa00`,
 * `+0x64=-1`), clears the one-shot flag (`+0x2c=0`), caches the
 * singleton table's `+4` field at `+0x68`, and clears `+0x6c`. Returns
 * `self`.
 *
 * Two gaps closed to get this byte-exact, both `asm volatile` anchors
 * (this compiler's own C-driven codegen can't reproduce either shape):
 * - The 6th (stack-passed, byte-sized) constructor argument: a plain
 *   `u8 eByte` parameter always reads the full word and narrows it
 *   with two shifts, where the ROM computes the stack slot's address
 *   (`add r0, sp, #0x24`) and does a genuine `ldrb` - the same
 *   pattern already closed for other stack-passed byte arguments (see
 *   `docs/matching/issue-5-overlay-ui-sync.md`'s `DrawSaveMenuMain` entry
 *   and `issue-45-hud-stat-widget-dispatcher.md`'s "6th-argument"
 *   entry). Materialized via a two-instruction `asm volatile` anchor
 *   into an `"=l"`-constrained (lo-register) temp, then copied into
 *   the `register u32 eByteVal asm("r9")` pin that mirrors the ROM's
 *   own `sb`/r9 cache (needed since it survives the following
 *   `GetHovercraftLevel()` call).
 * - The `+0x5c` spawn-record ternary (`(self[0x59] != 0) ? 0xFFFFBF00
 *   : 0x8400`): the ROM computes this as a genuine two-way diamond (a
 *   forward `beq`/`ldr`/`b` skipping a `movs`/`lsls` false-branch,
 *   with `gHovercraftSideGunVtable`'s pending literal and `0xFFFFBF00`
 *   pooled together right after the skip branch). A plain-C ternary
 *   or if/else always collapses this into an eager "compute one value,
 *   conditionally overwrite" shape instead (4 bytes short - missing
 *   the `b` that skips the false block), and - separately - as soon as
 *   the diamond is forced through any register-pinned if/else, this
 *   compiler's parameter-homing pass stops copying `self` first into
 *   its `r5` home in the prologue (still correct address, but
 *   reordered relative to `part`/`b`/`cParam` - a non-matching
 *   permutation of the same four `adds`/`mov` instructions). Neither
 *   glitch responds to any C-level restructuring tried (ternary
 *   polarity flips, if/else, goto-linearized if/else, precomputed
 *   addresses, or pinning any subset of `self`/`part`/`cParam` - and
 *   pinning `b` itself to its needed `r7` hits the already-documented
 *   r7-pin-hazard, dropping r7 from the prologue's push/pop list
 *   entirely). Fixed by moving the whole diamond into one opaque
 *   `asm volatile` block (referencing `self`'s known `r5` home
 *   directly by name rather than as an operand, which - unlike the
 *   hazard above - does not perturb the surrounding C's own register
 *   allocation) with a real `ldr r0, =0xFFFFBF00` assembler pseudo-op
 *   and a manual `.pool` directive right after the skip branch; the
 *   preceding `gHovercraftSideGunVtable` store (originally plain C) also
 *   had to move into its own tiny `asm volatile` island using the same
 *   `=symbol` pseudo-op, because a real, respected `.pool` split only
 *   works for symbols whose literal load is itself opaque assembler
 *   text - this compiler's own C-driven pool placement always defers
 *   a plain `extern` global access to the function's very end and
 *   ignores an `asm(".pool")` marker around it, exactly the gap
 *   already documented in `actor.c`'s `UpdateActorPaletteCycle`. The
 *   `self[0x28]=0`/self[0x59] reload pair and the `zeroByte`/`zero2`
 *   register splits below needed the same "which register holds
 *   which cached zero" register-pinning treatment, each in its own
 *   narrowly-scoped block so the pin doesn't widen past where the ROM
 *   actually needs that register reserved. */
extern void *GetHovercraftLevel(void);
extern void *InitActorPart(void *selfArg, void *part, s32 b, s32 c, s32 d);
extern u8 gHovercraftSideGunVtable[];
extern struct orbit_table *GetHovercraftAttack(void);

void *CreateHovercraftSideGun(void *selfArg, void *part, s32 b, s32 cParam, s32 d, u8 eByte)
{
    u8 *self;
    register u32 eByteVal asm("r9");
    register s32 health asm("r4");
    s32 idx;
    u8 byte59;
    s32 zero;
    u8 *p59;
    u32 t;

    self = selfArg;
    asm volatile ("add %0, sp, #0x24\n\tldrb %0, [%0]" : "=l" (t));
    eByteVal = t;

    health = (GetHovercraftLevel() == 0) ? 0x18 : 0x10;

    InitActorPart(self, part, b, cParam, d);
    *(s32 *)(self + 0x54) = health;
    asm volatile (
        "ldr r0, =gHovercraftSideGunVtable\n\t"
        "str r0, [r5, #0x50]\n\t"
        :
        :
        : "r0", "memory"
    );
    p59 = self + 0x59;
    zero = 0;
    *p59 = (u8)eByteVal;
    *(s32 *)(self + 0x28) = zero;
    byte59 = *p59;
    idx = 1;
    if (byte59 != 0) {
        idx = 0;
    }
    *(s32 *)(self + 0x28) = zero;
    *(s32 *)(self + 0x44) = zero;
    *(s32 *)(self + 0xc) = idx;
    {
        register u8 zeroByte asm("r1");
        u16 tmp16 = *(u16 *)(*(u8 **)self + idx * 12);
        zeroByte = 0;
        *(u16 *)(self + 0x10) = tmp16;
        self[0x12] = zeroByte;
        *(s32 *)(self + 8) = zero;
        self[0x58] = zeroByte;
    }
    asm volatile (
        "ldrb r0, [%0]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r0, =0xFFFFBF00\n\t"
        "b 3f\n\t"
        ".pool\n"
        "1:\n\t"
        "mov r0, #0x84\n\t"
        "lsl r0, r0, #8\n"
        "3:\n\t"
        "str r0, [r5, #0x5c]\n\t"
        :
        : "r" (p59)
        : "r0", "memory"
    );
    *(s32 *)(self + 0x60) = 0xa00;
    *(s32 *)(self + 0x64) = -1;
    {
        register s32 zero2 asm("r4");
        u8 *p2c = self + 0x2c;
        zero2 = 0;
        *p2c = (u8)zero2;
        *(s32 *)(self + 0x68) = *(s32 *)((u8 *)GetHovercraftAttack() + 4);
        *(s32 *)(self + 0x6c) = zero2;
    }

    return self;
}

/* Same `InitActorPart`-rooted per-instance "self" object family
 * documented in action_ctrl.c/hovercraft_parts.c/hovercraft_cannon.c: a "part
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
 * a hit sound. Same shape as `DamageHovercraftCannon` (hovercraft_cannon.c). */
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
