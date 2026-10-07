#include "core.h"
#include "match.h"
#include "actor_self.h"
#include "actor_anim.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"

/* Second half of issue #59's Phase 2 gap (`CreateJetpackRing`-`nullsub_35`,
 * the tail of `asm/code_3_2_20_28568_c99c_31784_31a6c.s`) - see
 * docs/matching/archive/issue-59-0x08031784-actor.md and
 * docs/matching/archive/issue-60-61-gap-31a6c-part2.md for the full writeup. A
 * sibling pass (`src/vehicle/jetpack_crates.c`) covers the first half
 * of the same file (`UpdateJetpackBalloonCrate`-`UpdateJetpackRing`).
 *
 * Two threads converge in this range:
 *
 * 1. The same per-instance boss-weapon/tracker "self" object family
 *    documented since issue #58 (`airship_fireball.c`-`airship_graphics.c`):
 *    state at `self+0x28`, table-index/"kind" at `self+0xc`, an
 *    anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 *    accumulator at `self+8`, a "part table" pointer at `self+0`, and
 *    an event/trampoline table pointer at `self+0x50` - the common
 *    `struct actor_self` prefix, with each class's own fields from
 *    +0x54 on in a small per-class struct here.
 *
 * 2. The `gHovercraft` singleton system first constructed by
 *    `CreateHovercraft` (this file) and already partially characterized by
 *    the LATER `hovercraft_parts.c`-`hovercraft_launcher.c` (issue #62,
 *    `0x08033804`+): a second, independent "unique object" cluster,
 *    structurally parallel to the boss's own patrol/BG2-affine/tile
 *    machinery (issue #58) but on a completely separate global family
 *    (`0x030015A0`-`0x030015FF`, plus the P1/P2-mirror pair
 *    `gFlashBgPalette`/`gFlashObjPalette` and the row-pointer array
 *    `gHovercraftMapFrames`). Every global in that family already has a
 *    real name from `hovercraft_parts.c`'s own extern block where this
 *    file's functions are the ones that *first* reference it in ROM
 *    order - reused verbatim here for consistency rather than
 *    reinvented. Per that file's own precedent (flat, independently
 *    linked BSS symbols, not fields of one struct reached through a
 *    common base pointer), this file does the same rather than
 *    introducing a struct wrapper that wouldn't match the actual link
 *    layout - see the writeup doc for the fuller rationale. */

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);
extern void _call_via_r0(void *fn);

/* Shared inlines of the singleton system (see CreateHovercraft). */
static inline struct actor_self *AllocActor(u32 size)
{
    return (struct actor_self *)mem_alloc(size, 0x80000000);
}

static inline void InitAnimPart(struct actor_self *self, struct anim_frame_record *anims,
                                u32 *offsets, s32 flag)
{
    self->anims = anims;
    self->frameOffsets = offsets;
    self->palette = flag;
    SetActorAnim(self, 0);
}

static inline void SingletonSetKind(s32 kind, s32 idx)
{
    struct actor_self *self;

    gHovercraftState = kind;
    self = gHovercraft;
    self->animIndex = idx;
    self->animTimer = self->anims[idx].duration;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold)
        self->animTime = 0;
}

/* The homing spawn effect advanced by UpdateJetpackCollectedWumpa. */
struct actor_2718 {
    struct actor_self base;
    s32 hp;   // 0x54
    s32 velX; // 0x58
    s32 velY; // 0x5C
};

/* The seek effect built by CreateJetpackCollectedWumpa (method table
 * gJetpackCollectedWumpaVtable; DestroyJetpackCollectedWumpa is its destructor). */
struct actor_2890 {
    struct actor_self base;
    s32 hp;     // 0x54
    s32 velX;   // 0x58
    s32 velY;   // 0x5C
    s32 reward; // 0x60 - the spawn parameter, see DestroyJetpackCollectedWumpa
};

/* The patrol object built by CreateHovercraftFireball (method table
 * gHovercraftFireballVtable) is the airship fireball's class, `struct
 * actor_orbit` (bosses.h): it sets up the orbit fields but never reads
 * them. */

/* An `InitActorPart`-based constructor: forwards its 4 real arguments
 * straight through (the 5th, `d`, is itself stack-passed), sets
 * `hp = 1` and the method table, and clears `cued`. Same shape as the
 * already-matched `CreateAirshipFireball` (issue #58, `airship_fireball.c`), minus
 * that function's extra `b`/`c` re-stash into `centerX`/`centerY`. */
void *CreateJetpackRing(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct jetpack_ring *self = selfArg;
    MATCH_HOLD_REG(s32, dReg, r0) = d;
    MATCH_HOLD_REG(s32, one, r5) = 1;

    InitActorPart(self, part, b, c, dReg);
    self->hp = one;
    self->base.vtable = (struct actor_vtable *)gJetpackRingVtable;
    self->cued = 0;

    return self;
}

/* Trivial constant getter - always "true"/"ready". */
s32 IsJetpackRingUnshootable(void *self)
{
    return 1;
}

/* Homing spawn effect step: advances the position by the velocity at
 * `+0x58`/`+0x5c`; once it leaves the positive quadrant past 0x1000,
 * plays sound 0xE and destroys itself, otherwise runs the shared
 * anim-frame-advance-and-clamp idiom. The idiom's `#4`/`#6` scheduling
 * gap closes by re-indexing `anims[animIndex]` for each field instead
 * of through a record pointer (docs/matching/archive/pmf-dispatch-retry.md). */
void UpdateJetpackCollectedWumpa(void *selfArg)
{
    struct actor_2718 *self = selfArg;
    s32 one = 1;
    s32 x, y;
    s32 base;

    self->base.sortKey = one;
    x = self->base.x += self->velX;
    y = self->base.y += self->velY;
    if (x <= 0x1000 || y <= 0x1000) {
        PlaySfx(gAudioContext, SFX_HUD_COLLECT, 0x100);
        if (self != NULL) {
            ACTOR_VCALL(&self->base, destroy, 3);
        }
        return;
    }
    self->base.animTime += *(s16 *)&self->base.animTimer;
    self->base.animDone = 0;
    base = GetAnimFrameBaseOffset((struct actor_self *)self);
    if (base >= self->base.anims[self->base.animIndex].loopThreshold) {
        ANIM_REWIND(self->base.animTime, self->base.anims[self->base.animIndex]);
        self->base.animDone = one;
    }
}

/* Draws `self`'s current anim frame at its Q8 position, centered on the
 * frame's width/height bytes, unless it is entirely off screen. Same
 * shape as DrawActor (actor.c) with the scale
 * doubling fixed off: `scaled` starts at 0 (halving nothing) and becomes
 * the 0x100 OBJ-affine bit once the sprite is known to be visible. The
 * third OAM word takes `self->palette` as its priority nibble, plus 0x800
 * when bit 15 of `self->sortKey` is set. */
void DrawJetpackCollectedWumpa(void *selfArg)
{
    struct actor_self *self = selfArg;
    s32 x;
    s32 y;
    u8 *frame;
    u32 scaled;
    s32 halfW;
    s32 halfH;
    u32 attr;
    u32 attr2;
    u16 attr2Out;
    s32 oamPriority = 0x180;
    u32 highBit = 0x800;

    {
        s32 qx = self->x, qy = self->y;

        x = Q8_TO_INT(qx);
        y = Q8_TO_INT(qy);
    }
    frame = GetAnimFrameData(self);
    scaled = 0;
    halfW = scaled ? frame[0] << 3 : frame[0] << 2;
    halfH = scaled ? frame[1] << 3 : frame[1] << 2;
    x -= halfW;
    y -= halfH;
    if (y > 0x9f)
        return;
    if (y + halfH * 2 < 0)
        return;
    if (x > 0xef)
        return;
    if (x + halfW * 2 < 0)
        return;

    scaled |= 0x100;
    attr = (y & 0xff) | ((x & 0x1ff) << 16) | GetAnimFrameAttr(self) | scaled;
    x = self->palette;
    attr2 = x << 12;
    if (self->sortKey & SORT_KEY_FLAG_BEHIND_BG)
        attr2Out = attr2 | highBit;
    else
        attr2Out = attr2;
    SetupSpriteFrameOam(frame, attr, attr2Out, oamPriority);
}

/* Destructor: dispenses `reward` score increments via `CollectWumpa`
 * while temporarily wearing `gJetpackCollectedWumpaVtable` (`CreateJetpackCollectedWumpa`'s
 * constructor table), then runs the same teardown as `DestroyRiderlessPolar`
 * (actor_anim.c): retag to the shared "dead" table, unlink from the
 * `+0x48`/`+0x4c` circular list, and free `self` if `flags & 1`. The
 * parameter-copy order the earlier NAKED note blamed on the compiler
 * comes out as in the ROM from this plain form
 * (docs/matching/archive/pmf-dispatch-retry.md). */
struct actor_283c {
    u8 unk_00[0x48];
    struct actor_283c *l48;
    struct actor_283c *l4c;
    void *vtable;
    u8 unk_54[0xc];
    s32 reward;
};

void DestroyJetpackCollectedWumpa(struct actor_283c *self, u32 flags)
{
    s32 i;

    self->vtable = (void *)gJetpackCollectedWumpaVtable;
    for (i = 0; i < self->reward; i++) {
        CollectWumpa(gLevelState);
    }
    self->vtable = (void *)gActorVtable;
    self->l4c->l48 = self->l48;
    self->l48->l4c = self->l4c;
    if (flags & 1) {
        mem_free(self);
    }
}

/* "Spawn effect type N" homing/seek-toward-point constructor - a
 * byte-for-byte twin of the already-matched `CreatePolarCollectedWumpa` (issue #52,
 * `polar_pickups.c`), per `docs/rom_map.md`'s
 * "Two small follow-ups round out the picture further" finding. Field
 * offsets differ slightly from that twin: this object keeps a fixed
 * `self+0x54 = 1` health field separate from the computed velocity
 * (stored at `self+0x58`/`self+0x5c` here, vs. `self+0x54`/`self+0x58`
 * there), and stashes `spawnParam` at `self+0x60` rather than `0x5c`. */
void *CreateJetpackCollectedWumpa(void *selfArg, void *part, s32 b, s32 c, s32 spawnParam)
{
    struct actor_2890 *self = selfArg;
    MATCH_HOLD_REG(s32, dy, r3);
    s32 sum;
    s32 q;

    InitActorPart(self, part, b, c, 1);
    self->hp = 1;
    self->base.vtable = (struct actor_vtable *)gJetpackCollectedWumpaVtable;
    self->reward = spawnParam;

    self->base.y += GetActorBgCenterY();

    {
        MATCH_HOLD_REG(s32, ebResult, r0) = GetActorBgCenterX();
        MATCH_HOLD_REG(s32, old, r1) = self->base.x;
        dy = old + ebResult;
    }
    self->base.x = dy;

    {
        s32 a1 = dy - 0x1000;
        s32 a2 = ABS_BRANCHLESS(a1);
        s32 dx = self->base.y;
        s32 b1 = dx - 0x1000;
        s32 b2 = ABS_BRANCHLESS(b1);

        sum = a2 + b2;
        if (sum < 0) {
            sum += 0x7ff;
        }
        q = sum >> 0xb;

        self->velX = __divsi3(0x1000 - dy, q);
        self->velY = __divsi3(0x1000 - dx, q);
    }

    return self;
}

/* Trivial constant getter - always "true"/"ready", same shape as
 * `IsJetpackRingUnshootable` above. */
s32 IsJetpackCollectedWumpaUnshootable(void *self)
{
    return 1;
}

/* Damage/health countdown: subtracts `delta` from `self+0x54`, and once
 * it drops to zero (or below) plays the death sound and runs the full
 * state/accumulator/anim-frame reset idiom already matched for
 * `ReleaseJetpackBalloon` (issue #59, `jetpack_balloon.c`)/`DamageHovercraftCannon` (issue
 * #62, `hovercraft_cannon.c`). */
void DamageHovercraftFireball(void *selfArg, s32 delta)
{
    struct actor_2890 *self = selfArg;
    s32 health = self->hp - delta;

    self->hp = health;
    if (health > 0) {
        return;
    }

    self->base.palette = 4;
    PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
    {
        MATCH_HOLD_REG(s32, one, r0) = 1;

        self->base.state = one;
        {
            MATCH_HOLD_REG(s32, zero, r2) = 0;

            self->base.stateTime = zero;
            self->base.animIndex = one;
            {
                u16 anim = self->base.anims[1].duration;
                MATCH_HOLD_REG(u8, zero2, r1) = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero2;
            }
            self->base.animTime = zero;
        }
    }
}

/* Per-state member-pointer dispatch, `(this->*gHovercraftFireballStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`), then "destroy" once the
 * state-1 animation has played through, else the standard UpdateActor
 * step. */
void UpdateHovercraftFireball(void *selfArg)
{
    struct actor_self *self = selfArg;

    ACTOR_PMF_CALL(self, gHovercraftFireballStateFuncs);

    if (self->state == 1 && self->animDone != 0) {
        if (self != NULL) {
            ACTOR_VCALL(self, destroy, 3);
        }
    } else {
        UpdateActor(self);
    }
}

/* Same shared shape as `CreateHovercraftFireball`/`CreateHovercraftCannon` (issue #62,
 * `hovercraft_cannon.c`): forwards `a`/`b`/`c` (the last two re-stashed into
 * `self+0x58`/`self+0x5c`, `c` pinned to the high register `r8`
 * matching `CreateHovercraftCannon`'s own documented gap - this compiler's
 * allocator always prefers the low registers when they fit, so `c`
 * needs the explicit pin to reproduce the ROM's choice), `d` stack-
 * passed straight through to `InitActorPart`, plus the fixed
 * `self+0x54 = 2`/`self+0x50` event table/`self+0x64 = 0`/
 * `self+0x60 = 0x95`/`self+0x68 (byte) = 0` initialization already
 * matched verbatim for `CreateAirshipFireball` (issue #58, `airship_fireball.c`). */
void *CreateHovercraftFireball(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct actor_orbit *self = selfArg;
    MATCH_HOLD_REG(s32, bReg, r6) = b;
    MATCH_HOLD_REG(s32, cReg, r8) = c;
    MATCH_HOLD_REG(s32, dReg, r0) = d;
    MATCH_HOLD_REG(s32, health, r5) = 2;

    InitActorPart(self, part, b, c, dReg);
    self->hp = health;
    self->base.vtable = (struct actor_vtable *)gHovercraftFireballVtable;
    self->centerX = bReg;
    self->centerY = cReg;
    self->radius = 0;
    self->velZ = 0x95;
    self->exploding = 0;

    return self;
}

/* Trivial `exploding` setter. */
void HovercraftFireballStateExplode(void *selfArg)
{
    struct actor_orbit *self = selfArg;
    self->exploding = 1;
}

/* Patrol-speed decay plus a death transition: advances `self+0x24` by
 * `self+0x60`, decays `self+0x60` by 5 (floored at 0x14). If
 * `IsTouchingPlayer(self)` fires, draws a text popup on the orbiting
 * companion object (`gActorList`, via its own `+0x50`
 * trampoline-table pointer, same `{s16 offset; ...; void *arg}`
 * convention already documented at `self+0x50` elsewhere in this
 * cluster), then runs the exact same state/anim-frame reset tail as
 * `DamageHovercraftFireball` above. */
void HovercraftFireballStateFly(void *selfArg)
{
    MATCH_HOLD_REG(struct actor_orbit *, self, r4) = selfArg;
    s32 sum = self->base.z;
    s32 delta = self->velZ;

    sum += delta;
    self->base.z = sum;
    delta -= 5;
    self->velZ = delta;
    if (delta <= 0x13) {
        self->velZ = 0x14;
    }

    if ((u8)IsTouchingPlayer(self)) {
        struct actor_self *player = gActorList;
        struct actor_vtable *table = player->vtable;

        _call_via_r2((u8 *)player + table->m20.thisOffset, 6, table->m20.fn);

        self->base.palette = 4;
        PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
        {
            MATCH_HOLD_REG(s32, one, r0) = 1;

            self->base.state = one;
            {
                MATCH_HOLD_REG(s32, zero, r2) = 0;

                self->base.stateTime = zero;
                self->base.animIndex = one;
                {
                    u16 anim = self->base.anims[1].duration;
                    MATCH_HOLD_REG(u8, zero2, r1) = 0;

                    *(u16 *)&self->base.animTimer = anim;
                    *(u8 *)&self->base.animDone = zero2;
                }
                self->base.animTime = zero;
            }
        }
    }
}

/* `UpdateHovercraftFireball`'s dispatch without its tail: `(this->*gStaticData_
 * 0817C450[this->state])()` (see `ACTOR_PMF_CALL`). */
void RunHovercraftFireballState(void *selfArg)
{
    struct actor_self *self = selfArg;

    ACTOR_PMF_CALL(self, gHovercraftFireballStateFuncs);
}

/* Trivial `exploding` getter. */
u8 IsHovercraftFireballUnshootable(void *selfArg)
{
    struct actor_orbit *self = selfArg;
    return self->exploding;
}

/* Palette blink/flash effect for the P2 VRAM fill-level meter, gated by
 * the one-shot latch `StartHovercraftHitFlash` (issue #62, `hovercraft_parts.c`) arms
 * (`gHovercraftHitFlashTimer`/`gHovercraftHitFlashOn`): once armed, advances the counter
 * every call, flips the toggle byte every 4th call, wraps the counter
 * past 0xb, then rewrites the meter's 16-halfword palette strip
 * (`0x05000020`) either solid white (`0x7fff`, while the toggle is set)
 * or restored from `gHovercraftPalette`'s own first 16 halfwords -
 * the same per-level table `ConvertHovercraftTiles` below reads for the meter's
 * fill-level heights. */
void UpdateHovercraftHitFlash(void)
{
    if (gHovercraftHitFlashTimer == 0) {
        return;
    }

    gHovercraftHitFlashTimer += 1;
    {
        MATCH_HOLD_REG(s32, three, r0) = 3;
        MATCH_HOLD_REG(s32, cur, r1) = *(vu16 *)&gHovercraftHitFlashTimer;
        MATCH_HOLD_REG(s32, result, r0);

        result = three & cur;
        if (result == 0) {
            MATCH_HOLD_REG(u8 *, addr, r1) = &gHovercraftHitFlashOn;
            MATCH_HOLD_REG(u8, one, r0) = 1;
            MATCH_HOLD_REG(u8, old, r3) = *addr;

            one ^= old;
            *addr = one;
        }
    }

    if (gHovercraftHitFlashTimer > 0xb) {
        gHovercraftHitFlashTimer = 0;
    }

    {
        MATCH_HOLD_REG(u8 *, flagAddr, r5) = &gHovercraftHitFlashOn;
        MATCH_HOLD_REG(u16, white, r4) = 0x7fff;
        const u16 *src = gHovercraftPalette;
        vu16 *dst = (vu16 *)(PLTT + 0x20);
        vu16 *end = dst + 15;

        do {
            if (*flagAddr != 0) {
                *dst = white;
            } else {
                *dst = *src;
            }
            src++;
            dst++;
        } while ((s32)dst <= (s32)end);
    }
}

/* A frame counter (`gHovercraftFrameCount`) drives the same P1/P2-mirrored
 * speed-override toggle already matched for `SetHovercraftFlashColor` (issue #62,
 * `hovercraft_parts.c`) - inlined twice here (once forcing "max speed"
 * every 16th frame, once restoring the cached normal speed every
 * 8th-but-not-16th frame) rather than calling that function, matching
 * the ROM exactly (the ROM's own build never emits a `bl SetHovercraftFlashColor`
 * here, so the original source duplicated the logic rather than
 * sharing it). `SetHovercraftFlashColor`'s own body confirms the same "reload
 * `gFlashBgPalette`'s pointer value through a register-pinned `p`,
 * assign the register-pinned `val` in its own statement" idiom closes
 * the exact register split this compiler otherwise collapses (loading
 * a >255 constant like `0x7FFF` straight into a pre-existing
 * register-pinned variable forces this compiler to materialize it in
 * a fresh register first, then copy - the `MATCH_KEEP(p)` barrier
 * between the pointer reload and the value assignment keeps that
 * reload from being folded into the shared tail the two call sites
 * below happen to converge on). Tail: runs `UpdateHovercraftHitFlash` (the meter
 * blink above) then dispatches the singleton's current animation
 * "kind" through the third table family (`gHovercraftStateFuncs`), the
 * same convention already documented for the entity/actor category
 * vtables. */
static inline void CommitSpeed(u16 *p, u16 val)
{
    p[15] = val;
    gFlashObjPalette[15] = val;
}

void RunHovercraftState(void)
{
    s32 counter = gHovercraftFrameCount + 1;
    gHovercraftFrameCount = counter;

    if ((counter & 0xf) == 0) {
        if (gHovercraftFlashColorSaved == 0) {
            gHovercraftFlashSavedColor = gFlashBgPalette[15];
            gHovercraftFlashColorSaved = 1;
        }
        {
            MATCH_HOLD_REG(u16 *, p, r0) = gFlashBgPalette;
            MATCH_HOLD_REG(u16, val, r1);

            MATCH_KEEP(p);
            val = 0x7FFF;
            CommitSpeed(p, val);
        }
    } else if ((counter & 7) == 0) {
        if (gHovercraftFlashColorSaved == 0) {
            gHovercraftFlashSavedColor = gFlashBgPalette[15];
            gHovercraftFlashColorSaved = 1;
        }
        {
            MATCH_HOLD_REG(u16 *, p, r0) = gFlashBgPalette;
            MATCH_HOLD_REG(u16, val, r1);

            MATCH_KEEP(p);
            val = gHovercraftFlashSavedColor;
            CommitSpeed(p, val);
        }
    }

    UpdateHovercraftHitFlash();
    _call_via_r0(gHovercraftStateFuncs[gHovercraftState]);
}

/* Opens the singleton's own camera-follow/scroll-velocity smoothing
 * computation (`gHovercraftX`-`gHovercraftPhase`) - the twin of the boss
 * cluster's `SteerAirship` (airship.c). Ramps the Z velocity
 * toward a per-phase target, then by patrol phase: phase 0 steers the
 * X/Y velocities toward the player (`gActorList`) relative to a
 * camera-offset target box (`gHovercraftBox`), clamped to +-0x200
 * and kept inside fixed bounds; phase 1 bounces X at +-0x10000; later
 * phases orbit on the trig table `gSineTable` with a growing
 * radius. Once close enough (`gHovercraftDistance <= 0x27ff`), resets to
 * kind 3 / phase 0. Plain C - the documented "four live high registers"
 * blocker was not real; what mattered was keeping the player/camera
 * reads as separate locals (so `(a - b) - c` isn't reassociated),
 * updating the velocities with `-=` and taking the orbit's destination
 * and table pointers before the angle is computed. */
void HovercraftStateCloseIn(void)
{
    gHovercraftZ += gHovercraftVelZ;
    if (gHovercraftPhase == 0) {
        if (gHovercraftVelZ <= 0x98)
            gHovercraftVelZ = gHovercraftVelZ + 1;
        else
            gHovercraftVelZ = gHovercraftVelZ - 1;
    } else if (gHovercraftPhase == 1) {
        if (gHovercraftVelZ <= 0x3f)
            gHovercraftVelZ = gHovercraftVelZ + 1;
        else if (gHovercraftVelZ > 0x40)
            gHovercraftVelZ = gHovercraftVelZ - 1;
    } else {
        if (gHovercraftVelZ <= 0x69)
            gHovercraftVelZ = gHovercraftVelZ + 1;
        else if (gHovercraftVelZ > 0x6a)
            gHovercraftVelZ = gHovercraftVelZ - 1;
    }

    if (gHovercraftPhase == 0) {
        struct actor_self *pl;
        s32 vx, vy, px, py, cx, cy;

        gHovercraftX += gHovercraftVelX;
        gHovercraftY += gHovercraftVelY;
        pl = gActorList;
        px = pl->x;
        cx = gHovercraftScreenX - 0x1200;
        gHovercraftVelX -= (px - cx - (gHovercraftBox.x + gHovercraftBox.w / 2)) >> 12;
        vx = gHovercraftVelX;
        py = pl->y;
        cy = gHovercraftScreenY + 0x1800;
        vy = gHovercraftVelY - ((py - cy - (gHovercraftBox.y + gHovercraftBox.h / 2)) >> 12);
        gHovercraftVelY = vy;

        LIMIT_MAX(vx, 0x200);
        gHovercraftVelX = vx;
        LIMIT_MIN(vx, -0x200);
        gHovercraftVelX = vx;
        LIMIT_MAX(vy, 0x200);
        gHovercraftVelY = vy;
        LIMIT_MIN(vy, -0x200);
        gHovercraftVelY = vy;

        if (gHovercraftScreenX <= 0)
            gHovercraftVelX = 0x200;
        if (gHovercraftScreenX > 0x63ff)
            gHovercraftVelX = -0x200;
        if (gHovercraftScreenY <= -0x3c00)
            gHovercraftVelY = 0x200;
        if (gHovercraftScreenY > 0x2bff)
            gHovercraftVelY = -0x200;
    } else if (gHovercraftPhase == 1) {
        gHovercraftX += gHovercraftVelX;
        if (gHovercraftX > 0xffff && gHovercraftVelX > 0)
            gHovercraftVelX = -0x400;
        else if (gHovercraftX <= -0x10000 && gHovercraftVelX < 0)
            gHovercraftVelX = 0x400;
    } else {
        s32 a;

        if ((gHovercraftOrbitRadius += 0x180) > 0x8000)
            gHovercraftOrbitRadius = 0x8000;
        {
            s32 *px = &gHovercraftX;
            const s16 *tbl = gSineTable;

            a = ((gHovercraftFrameCount * 30) >> 4) & 0xff;
            *px = Q8_MUL(tbl[(a + 0x40) & 0xff], gHovercraftOrbitRadius);
            gHovercraftY = Q8_MUL(tbl[a], gHovercraftOrbitRadius);
        }
    }

    if (gHovercraftDistance <= 0x27ff) {
        gHovercraftFireTimer = gHovercraftAttack->timing[1].delay;
        gHovercraftVolleyCount = 0;
        SingletonSetKind(3, 0);
        gHovercraftPhase = 0;
        gHovercraftVelZ = 0xae;
    }
}

/* `HovercraftStateCloseIn`'s sibling half of the same camera-follow/scroll-
 * velocity smoothing computation: integrates the singleton's position
 * (`gHovercraftX`/`B8`/`BC`) by its velocities, damps the Y
 * velocity toward 0, and - while the patrol phase `gHovercraftPhase`
 * is still in its first legs (<= 3) - ramps the Z velocity toward
 * 0xae and bounces the X velocity at +-0x8000, counting legs; later
 * legs ramp Z toward 0x1d4 and steer X back to 0. Once past leg 3 and
 * far enough away (`gHovercraftDistance > 0x8000`), resets the timers,
 * reloads `gHovercraftFireTimer` from the owner, switches the singleton to
 * kind 2 and re-arms the next patrol phase from the lifetime counter.
 * Plain C (the documented "register gap" was never real). */
void HovercraftStateFallBack(void)
{
    s32 y;

    gHovercraftX += gHovercraftVelX;
    y = gHovercraftY += gHovercraftVelY;
    gHovercraftZ += gHovercraftVelZ;

    if (y > 0)
        gHovercraftVelY = -0x100;
    else if (y < 0)
        gHovercraftVelY = 0x100;
    else
        gHovercraftVelY = 0;

    if (gHovercraftPhase <= 3) {
        s32 v = gHovercraftVelZ;

        if (v <= 0xad)
            gHovercraftVelZ = v + 1;
        else if (v > 0xae)
            gHovercraftVelZ = v - 1;

        if (gHovercraftX > 0x7fff && gHovercraftVelX > 0) {
            gHovercraftVelX = -0x200;
            gHovercraftPhase++;
        } else if (gHovercraftX <= -0x8000 && gHovercraftVelX < 0) {
            gHovercraftVelX = 0x200;
            gHovercraftPhase++;
        }
    } else {
        s32 v = gHovercraftVelZ;

        if (v <= 0x1d4)
            gHovercraftVelZ = v + 1;
        else
            gHovercraftVelZ = v - 1;

        if (gHovercraftX > 0)
            gHovercraftVelX = -0x200;
        else if (gHovercraftX < 0)
            gHovercraftVelX = 0x200;
        else
            gHovercraftVelX = 0;
    }

    if (gHovercraftPhase > 3 && gHovercraftDistance > 0x8000) {
        gHovercraftFrameCount = 0;
        gHovercraftOrbitRadius = 0;
        gHovercraftFireTimer = gHovercraftAttack->timing[1].delay;
        gHovercraftVolleyCount = 0;
        SingletonSetKind(2, 0);
        if (gHovercraftPartsLeft > 2) {
            gHovercraftPhase = 1;
            gHovercraftVelZ = 0x40;
        } else {
            gHovercraftPhase = 2;
            gHovercraftVelZ = 0x6a;
        }
        gHovercraftVelX = -0xa00;
    }
}

/* Patrol/oscillation driver, structurally parallel to the boss
 * cluster's own `AirshipStateFireballs` (issue #58) - a bounded oscillator on
 * `gHovercraftVelZ` (converging on 0x99) and `gHovercraftVelY`
 * (bouncing 0-0x100), applied to the singleton's own position
 * (`gHovercraftZ`/`gHovercraftY`), with a reward trigger
 * (`ResumeActorSpawns`/`FinishJetpackRun`) once `gHovercraftY` crosses
 * 0x4b00, and a DISPCNT window/mosaic-bit clear once
 * `gHovercraftDistance` drops below 0x1500 (setting the "dead" flag
 * `gHovercraftGone`). */
void HovercraftStateFall(void)
{
    if (gHovercraftGone == 0) {
        s32 v = gHovercraftVelZ;
        s32 d;

        if (v <= 0x98) {
            gHovercraftVelZ = v + 1;
        } else if (v > 0x99) {
            gHovercraftVelZ = v - 1;
        }

        d = gHovercraftVelY;
        if (d <= 0xff) {
            gHovercraftVelY = d + 0x100;
        } else if (d > 0x100) {
            gHovercraftVelY = d - 0x100;
        }

        gHovercraftZ += gHovercraftVelZ;
        gHovercraftY += gHovercraftVelY;

        if (gHovercraftY > 0x4b00) {
            ResumeActorSpawns();
            FinishJetpackRun(gActorList);
        }
    }

    if (gHovercraftDistance <= 0x14ff) {
        REG_DISPCNT &= 0xfbff;
        gHovercraftGone = 1;
    }
}

/* The singleton's own BG-tilemap-blit tile consumer, confirmed by
 * `docs/rom_map.md` as the same mechanics as the boss cluster's
 * `DrawAirshipMap` (airship_map.c, issue #58) but on the singleton's own
 * separate global cluster (`gHovercraftMapCols`-family, not
 * `gAirshipBg2Page`-family). Same source as that twin: the bias is a
 * plain `u8` narrowing of the `s32` global, and `row` is declared before
 * `i` so `i + 1` wins the r7/ip tie. Needs old_agbcc, which is why this
 * file is on OLD_AGBCC_OBJS (docs/matching/archive/issue-58-61-naked-retry.md). */
void DrawHovercraftMap(void *tileRow)
{
    u16 *src = tileRow;
    u8 *row = (u8 *)((gHovercraftBg2Page + 0x18) << 11) +
              (VRAM + (0x20 - gHovercraftMapCols) / 4 * 2) +
              ((0x20 - gHovercraftMapRows) / 2 * 32 + 2);
    s32 i, j;

    for (i = 0; i < gHovercraftMapRows; i++) {
        for (j = 0; j < gHovercraftMapCols / 2; j++) {
            u8 bias = gHovercraftMapTileBase;
            u16 lo = *src++ + bias;
            u16 hi = *src++ + bias;
            ((u16 *)row)[j] = lo | (hi << 8);
        }
        row += 0x20;
    }
}

/* The missing constructor for the whole singleton system - see
 * `docs/rom_map.md`'s "`CreateHovercraft` closes a long-open question"
 * section. Caches its own incoming argument into `gHovercraftLevel`,
 * seeds the P2-meter-shaped row/column counts (`gHovercraftMapCols`/
 * `gHovercraftMapRows`) from a per-level table (`gHovercraftPicture`), allocates
 * the singleton object itself (part table `gHovercraftKeyframes`,
 * "frame offsets" field re-using the row-pointer array
 * `gHovercraftMapFrames`), stores it into `gHovercraft` - the pointer
 * everything else in this thread reads - resets its kind/anim-frame
 * fields, fires the per-frame update driver (`LoadHovercraftGraphics`) once, and
 * finishes by clearing the "apply now" BG2 latch and setting the
 * lifetime counter `gHovercraftPartsLeft = 4` (the exact counter
 * `LoseHovercraftPart`, issue #62, decrements toward "dead").
 *
 * The boss tracker's constructor `CreateAirship` (airship.c) is its
 * twin and matched the same way: an inlined C++ `new` - destination
 * address taken before the allocation, an `operator new`-style size
 * wrapper, and an inlined base constructor taking its values as
 * arguments - followed by an inlined "set kind" helper (the source of
 * the ROM's two separate zero registers). */
void CreateHovercraft(s32 level)
{
    struct actor_self *t;
    struct actor_self **slot;

    gHovercraftLevel = level;
    gHovercraftMapCols = BOSS_PICTURE_SIZE(gHovercraftPicture)->cols;
    gHovercraftMapRows = BOSS_PICTURE_SIZE(gHovercraftPicture)->rows;
    slot = &gHovercraft;
    t = AllocActor(0x1c);
    InitAnimPart(t, (struct anim_frame_record *)gHovercraftKeyframes, (u32 *)gHovercraftMapFrames,
                 1);
    *slot = t;
    SingletonSetKind(0, 0);
    LoadHovercraftGraphics();
    gHovercraftBg2PageFlip = 0;
    gHovercraftPartsLeft = 4;
}

/* The animation-system-wired spawn/init step for the singleton - the
 * twin of the boss cluster's `SpawnAirship` (airship.c): resets
 * the patrol oscillator (`gHovercraftVelZ = 0x66`), selects animation
 * "kind" 1 with the standard anim-frame reset, seeds position from its
 * arguments, looks up the per-kind record (`gHovercraftAttacks`,
 * stride 0x28, indexed by the incoming kind plus the level index
 * `gHovercraftLevel` the constructor cached; `gHovercraftAttack` keeps
 * it for later accessors), sets DISPCNT's window bit, resets every
 * timing/lifetime field for a fresh spawn, recomputes the BG2 zoom, blits
 * the current tile row, and finishes by spawning two pairs of small
 * effect objects (`SpawnHovercraftSideGun` x2, `SpawnHovercraftCannon`, `SpawnHovercraftLauncher`)
 * around the singleton - the "burst spawn... clustered around the
 * singleton" `docs/rom_map.md` documents. Plain C: the "six live
 * scratch values" blocker wasn't real; the zoom divide is an explicit
 * `__divsi3` call and the record lookup needs the `- -` form below. */
void SpawnHovercraft(s32 kind, s32 x, s32 y, s32 z)
{
    s32 scale;

    gHovercraftVelZ = 0x66;
    SingletonSetKind(1, 0);
    gHovercraftX = x * 5;
    gHovercraftY = y * 3;
    gHovercraftZ = z;
    /* `a - -b` rather than `a + b`: the latter lets fold reassociate the
     * constant table base out of `&table[kind]`, while the ROM adds the
     * level offset to the finished record address. */
    gHovercraftAttack =
        (const struct hovercraft_attack *)(gHovercraftLevel *
                                               (s32)sizeof(struct hovercraft_attack) -
                                           -(s32)&gHovercraftAttacks[kind]);
    gHovercraftFireTimer = gHovercraftAttack->timing[0].burstDelay;
    gHovercraftHp = gHovercraftAttack->hp;
    gHovercraftVolleyCount = 0;
    REG_DISPCNT |= DISPCNT_BG2_ON;
    gHovercraftBg2PageFlip = 1;
    gHovercraftBg2Page = 0;
    gHovercraftPhase = 0;
    gHovercraftFrameCount = 0;
    gHovercraftOrbitRadius = 0;
    gHovercraftPartsLeft = 4;
    gHovercraftHitFlashTimer = 0;
    gHovercraftHitFlashOn = 0;
    gHovercraftDistance = gHovercraftZ - INT_TO_Q8(GetCellAnimDistance());
    scale = __divsi3(0x1C00000, gHovercraftDistance);
    gHovercraftScreenX = Q12_MUL(gHovercraftX, scale);
    gHovercraftScreenY = Q12_MUL(scale, gHovercraftY);
    SetActorBgLayerDepth(gHovercraftDistance);
    {
        struct actor_self *self = gHovercraft;
        s32 t = Q8_TO_INT(self->animTime);

        DrawHovercraftMap((void *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
    }
    SpawnHovercraftCannon(gHovercraftX + 0x2000, gHovercraftY + 0x3000, gHovercraftZ - 0x100);
    SpawnHovercraftLauncher(gHovercraftX + 0x1e00, gHovercraftY - 0x3000, gHovercraftZ - 0x100);
    SpawnHovercraftSideGun(gHovercraftX - 0x4100, gHovercraftY + 0xa00, gHovercraftZ - 1, 1);
    SpawnHovercraftSideGun(gHovercraftX + 0x8400, gHovercraftY + 0xa00, gHovercraftZ - 1, 0);
    PauseActorSpawns();
}

/* Per-frame animate+project+tile-stream update driver, the singleton's
 * twin of the boss cluster's `UpdateAirship` (airship.c) and
 * matched the same way: runs the P1/P2 speed-toggle dispatcher
 * (`RunHovercraftState`), and while the singleton's animation "kind"
 * (`gHovercraftState`) is active, the usual anim-frame-advance-and-
 * clamp idiom; then recomputes the projection scale and BG2-space
 * offsets (`gHovercraftScreenX`/`gHovercraftScreenY`) from the current position
 * and `gHovercraftDistance`, calling `SetActorBgLayerDepth` on the result;
 * finally, if the (Q8.8-truncated) frame index changed this tick,
 * streams the new tile row through `DrawHovercraftMap` and arms the "apply
 * now" BG2 latch (`gHovercraftBg2PageFlip`). The divide is an explicit call
 * to `__divsi3` (the ROM reloads `gHovercraftDistance` after it, which
 * `/`'s const libcall wouldn't), and the tail reads the singleton
 * through a fresh local - the "4 extra bytes" of the earlier attempt. */
void UpdateHovercraft(void)
{
    s32 prev = Q8_TO_INT(gHovercraft->animTime);
    struct actor_self *self;

    RunHovercraftState();
    if (gHovercraftState != 0) {
        s32 scale;

        self = gHovercraft;
        self->animTime += (s16)self->animTimer;
        self->animDone = 0;
        if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold) {
            ANIM_REWIND(self->animTime, self->anims[self->animIndex]);
            self->animDone = 1;
        }
        gHovercraftDistance = gHovercraftZ - INT_TO_Q8(GetCellAnimDistance());
        scale = __divsi3(0x1C00000, gHovercraftDistance);
        gHovercraftScreenX = Q12_MUL(gHovercraftX, scale);
        gHovercraftScreenY = Q12_MUL(scale, gHovercraftY);
        SetActorBgLayerDepth(gHovercraftDistance);
        {
            struct actor_self *cur = gHovercraft;
            s32 t = Q8_TO_INT(cur->animTime);

            if (prev != t) {
                DrawHovercraftMap(
                    (void *)cur->frameOffsets[cur->anims[cur->animIndex].frameIndex + t]);
                gHovercraftBg2PageFlip = 1;
            }
        }
    }
}

/* The singleton's own BG2 affine-matrix committer, structurally
 * parallel to the boss cluster's `UpdateAirshipBg2` (issue #58) - pure
 * scale, no rotation (`BG2PB`/`BG2PC = 0`), per `docs/rom_map.md`'s
 * confirmation of this exact reuse pattern. */
void UpdateHovercraftBg2(void)
{
    s32 scale;
    s32 dy;
    s32 dx;

    if (gHovercraftBg2PageFlip != 0) {
        if (gHovercraftBg2Page == 0) {
            REG_BG2CNT = 0x5809;
        } else {
            REG_BG2CNT = 0x5909;
        }
        gHovercraftBg2PageFlip = 0;
        gHovercraftBg2Page ^= 1;
    }

    scale = __divsi3(gHovercraftDistance << 8, 0x3c00);
    dy = gHovercraftScreenX + GetActorBgCenterX();
    dx = gHovercraftScreenY + GetActorBgCenterY();

    REG_BG2X = 0x8000 - Q8_MUL(dy, scale);
    REG_BG2Y = 0x8000 - Q8_MUL(dx, scale);
    REG_BG2PA = scale;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = scale;
}

/* Top-level per-frame driver for the whole singleton system - fired
 * once by the constructor (`CreateHovercraft`) and, per `docs/rom_map.md`,
 * confirmed as the per-frame step `HovercraftStateFall`/`UpdateHovercraftBg2` connect
 * to. Copies the palette strip into BG palette bank 1, clears the tile
 * just before BG char block 3 and fills block 3 itself with a blank/
 * transparent tile (the exact same idiom the boss cluster's
 * `LoadAirshipGraphics`, airship_load_graphics.c, uses ahead of its own meter-generator
 * call), runs the P2 VRAM fill-level meter (`ConvertHovercraftTiles`), and - while
 * the singleton's animation "kind" is active - re-arms the "apply now"
 * BG2 latch, streams the current tile row through `DrawHovercraftMap`, sets
 * DISPCNT's window/mosaic bit, and commits the BG2 affine matrix
 * (`UpdateHovercraftBg2`). Matched with the same pieces as `LoadAirshipGraphics`: the
 * standard DMA macros, and the tile clear as a signed-address loop with
 * its zero hoisted into a local. */
void LoadHovercraftGraphics(void)
{
    s32 i;
    s32 base;
    u32 zero;

    DmaCopy16(3, gHovercraftPalette, (void *)(PLTT + 0x20), 0x20);
    base = VRAM + 0xBFC0;
    zero = 0;
    for (i = base + 0x3c; i >= base; i -= 4)
        *(u32 *)i = zero;
    DmaFill16(3, 0xFFFF, (void *)(VRAM + 0xC000), 0x1000);
    ConvertHovercraftTiles();
    if (gHovercraftState != 0) {
        struct actor_self *self;

        gHovercraftBg2PageFlip = 1;
        gHovercraftBg2Page = 0;
        self = gHovercraft;
        {
            s32 t = Q8_TO_INT(self->animTime);

            DrawHovercraftMap(
                (void *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
        }
        REG_DISPCNT |= DISPCNT_BG2_ON;
        UpdateHovercraftBg2();
    }
}

/* The P2-side VRAM fill-level meter, a near-identical twin of
 * `ConvertAirshipTiles` (issue #58, `airship_graphics.c`), applied to this
 * cluster's own per-level table (`gHovercraftPalette`) and row-pointer
 * array (`gHovercraftMapFrames`). */
/* The one-row twin of `ConvertAirshipTiles` (airship_graphics.c). The height is
 * re-read after the row-pointer store (the `MATCH_KEEP_MEM`) and the second
 * loop has its own counter (sharing `k` makes the first loop's reversed
 * counter start from a constant instead of `sum`'s zero register). The
 * row header is written out step by step in the ROM's order, `d` being
 * a copy of `dst`. In the nibble loop the 0xf mask is an opaque value
 * ANDed with each byte (`m & b`), so gcc copies the mask rather than the
 * byte, as the ROM does, and the second byte gets its own local. */
static inline u32 MeterPx(u32 v)
{
    u32 r = 0;
    if (v != 0)
        r = 0x10 | v;
    return r;
}

void ConvertHovercraftTiles(void)
{
    s32 heights[1];
    u32 stride;
    s32 sum = 0;
    s32 off = 0x204;
    s32 k;
    s32 row_i;
    u32 *dst;
    u8 **rows = (u8 **)gHovercraftMapFrames;
    u32 m;

    stride = (u32)(gHovercraftMapCols * gHovercraftMapRows + 1) >> 1 << 2;
    for (k = 0; k < 1; k++) {
        s32 x = *(s32 *)(((u8 *)gHovercraftPalette) + off);
        heights[k] = x;
        sum += x;
        off += 4;
        rows[k] = ((u8 *)gHovercraftPalette) + off;
        /* forces the height to be re-read (the ROM's `ldm r1!`) */
        MATCH_KEEP_MEM(heights[k]);
        off += stride;
        off += heights[k] << 5;
    }
    gHovercraftMapTileBase = 0xFF - sum;
    dst = (u32 *)(((0xFF - sum) << 6) + (VRAM + 0x8000));
    for (row_i = 0; row_i <= 0; row_i++) {
        u8 *src;
        u8 *row;
        s32 *hp;
        s32 n;
        s32 j;
        u32 *d;

        row = (u8 *)gHovercraftMapFrames[row_i];
        hp = &heights[row_i];
        d = dst;
        src = row + stride;
        n = *hp;

        for (j = 0; j < n << 4; j++) {
            u32 b, c, p0, p1, p2, p3;

            /* the 0xf mask without a constant-set register: the mask is
             * the AND's first operand, as in the ROM */
            MATCH_CONST(m, 0xf);
            b = *src;
            p0 = m & b;
            p0 = MeterPx(p0);
            p1 = (b >> 4) & m;
            src++;
            p1 = MeterPx(p1);
            c = *src;
            p2 = m & c;
            p2 = MeterPx(p2);
            p3 = (c >> 4) & m;
            src++;
            p3 = MeterPx(p3);
            *d++ = p0 | (p1 << 8) | (p2 << 16) | (p3 << 24);
        }
        dst = d;
    }
}

/* Destructor: frees the singleton object. */
void DestroyHovercraft(void)
{
    mem_free(gHovercraft);
}

/* UNUSED - no caller anywhere in the ROM (checked every src/ .c file, the
 * category vtables and every word-aligned Thumb pointer in baserom.gba).
 * Empty. With sub_80337FC and nullsub_35 it trails DestroyHovercraft the
 * way nullsub_30 trails DestroyAirship; no table slot names them. */
void nullsub_34(void)
{
}

/* UNUSED - no caller anywhere in the ROM (same checks as nullsub_34).
 * Returns 0. */
s32 sub_80337FC(void)
{
    return 0;
}

/* UNUSED - no caller anywhere in the ROM (same checks as nullsub_34).
 * Empty. */
void nullsub_35(void)
{
}
