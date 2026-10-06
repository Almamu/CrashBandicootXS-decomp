#include "core.h"
#include "actor_self.h"
#include "memory.h"
#include <libgcc.h>
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

/* Continuation of polar_player_actions.c's player/action-object family, right
 * after `RunPolarPlayerState` (matched C, see polar_player_dispatch.c) - same `self`
 * object and conventions documented there. */

/* `self` with the velocity pair this class adds after the common
 * prefix. */
struct moving_actor {
    struct actor_self base;
    s32 velX;                   // 0x54
    s32 velY;                   // 0x58
};

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Constant getter - returns `gPolarPlayerInactive`. */
u8 IsPolarPlayerInactive(void)
{
    return gPolarPlayerInactive;
}

/* Sets `sortKey`, advances `x`/`y` by the velocity pair, and once both
 * exceed `0x1000`: adds the (signed) `animTimer` into the `animTime`
 * accumulator and, once the frame counter reaches the current anim
 * record's `loopThreshold` (the same test `BoostPolarPlayer` uses), backs
 * the accumulator off by `loopThreshold - loopBase` and marks
 * `animDone`. Otherwise (the common per-frame case) just plays a sound
 * cue and fires the vtable's `destroy` method with 3. */
void UpdatePolarCollectedWumpa(void *selfArg)
{
    struct moving_actor *self = selfArg;
    s32 x, y;

    self->base.sortKey = 1;

    x = self->base.x + self->velX;
    self->base.x = x;
    y = self->base.y + self->velY;
    self->base.y = y;

    if (x > 0x1000 && y > 0x1000) {
        goto frameBlock;
    }

    PlaySfx(gAudioContext, 0xe, 0x100);
    if (self != 0) {
        struct actor_vtable *table = self->base.vtable;
        _call_via_r2((u8 *)self + table->destroy.thisOffset, (void *)3, table->destroy.fn);
    }
    return;

frameBlock:
    {
        /* `animTimer` read as an s16 through a register offset, as the
         * ROM does. */
        register s32 sixteenConst asm("r3") = 0x10;
        register s32 delta asm("r1") = *(s16 *)((u8 *)self + sixteenConst);

        self->base.animTime += delta;
        self->base.animDone = 0;
    }
    {
        s32 frame = GetAnimFrameBaseOffset((struct actor_self *)self);
        register s32 idx asm("r2") = self->base.animIndex;
        register u8 *table asm("r3") = (u8 *)self->base.anims;
        register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
        register s32 four asm("r3");
        register s32 e4 asm("r2");

        asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
        four = 4;
        e4 = *(s16 *)(entryPtr + four);

        if (frame >= e4) {
            register s32 six asm("r0") = 6;
            register s32 e6 asm("r1") = *(s16 *)(entryPtr + six);
            register s32 diff asm("r1") = e4 - e6;

            diff <<= 8;
            self->base.animTime -= diff;
            self->base.animDone = 1;
        }
    }
}

asm(".align 2, 0");

/* Screen-space visibility test and OAM setup for one sprite frame:
 * derives the top-left corner from `self->x`/`self->y` minus half
 * the frame's tile size, culls if fully off-screen, then builds the
 * OAM attribute words (position, `GetAnimFrameAttr`'s flag byte, and a
 * priority/palette nibble from `self->palette`/`self->sortKey`) and calls
 * `SetupSpriteFrameOam`.
 *
 * The dead `flag = 0` initializer (materialized by the ROM as `movs
 * r0, #0` / `mov r8, r0` right after `frame` is obtained, even though
 * every reachable path to `flag`'s use overwrites it with `0x100`
 * first) closes via an opaque `asm volatile("mov r0, #0\n\tmov %0,
 * r0" : "=r"(flag) :: "r0")` two-instruction materialization - a
 * single-instruction `mov r8, #0` isn't valid Thumb (only lo registers
 * take an immediate `mov`), caught by the mandatory
 * `arm-none-eabi-as` assemble-verification step. Every other register
 * choice (the `w`/`wShift`/`h`/`hShift` load/shift order, the
 * `x`/`y`-position-word pack, the `self+0x18` priority-nibble unpack)
 * matches the ROM's own register roles exactly once pinned to match. */

void DrawPolarCollectedWumpa(void *selfArg)
{
    register struct actor_self *self asm("r6") = selfArg;
    register s32 rawX asm("r0") = self->x;
    register s32 rawY asm("r1") = self->y;
    register s32 x asm("r4") = rawX >> 8;
    register s32 y asm("r5") = rawY >> 8;
    u8 *frame;
    register u32 flag asm("r8");
    register u32 packed asm("r3");

    frame = GetAnimFrameData(self);
    asm volatile("mov r0, #0\n\tmov %0, r0" : "=r"(flag) :: "r0");

    {
        register s32 w asm("r0");
        register s32 wShift asm("r2");
        register s32 h asm("r1");
        register s32 hShift asm("r0");

        w = frame[0];
        wShift = w << 2;
        h = frame[1];
        hShift = h << 2;
        x -= wShift;
        y -= hShift;

        if (y > 0x9f) return;
        {
            register s32 hCheck asm("r0") = h << 3;
            if (y + hCheck < 0) return;
        }
        if (x > 0xef) return;
        {
            register s32 wCheck asm("r0") = wShift << 1;
            if (x + wCheck < 0) return;
        }

        flag = 0x100;
        {
            register s32 attrFlag asm("r0") = GetAnimFrameAttr(self);
            register s32 a0 asm("r3") = 0xff;
            register s32 xm asm("r4") = x;

            a0 &= y;
            {
                register s32 mask asm("r1") = 0x1ff;
                register s32 shifted asm("r1");

                xm &= mask;
                shifted = xm << 16;
                a0 |= shifted;
            }
            a0 |= attrFlag;
            a0 |= flag;
            packed = a0;
        }
    }

    {
        register s32 field24 asm("r4") = self->palette;
        s32 a2 = field24 << 12;
        s32 field20 = self->sortKey;
        register u32 attr2 asm("r2");

        if (field20 & 0x8000) {
            a2 |= 0x800;
            {
                register s32 shifted asm("r0") = a2 << 16;
                attr2 = (u32)shifted >> 16;
            }
        } else {
            register s32 shifted asm("r0") = field24 << 28;
            attr2 = (u32)shifted >> 16;
        }

        {
            register u8 *argFrame asm("r0") = frame;
            register u32 argPacked asm("r1") = packed;
            register s32 argPriority asm("r3") = 0x140;

            SetupSpriteFrameOam(argFrame, argPacked, attr2, argPriority);
        }
    }
}

asm(".align 2, 0");

/* Continuation of polar_player_actions.c's player/action-object family, right
 * after `DrawPolarCollectedWumpa` (above) - same `self`
 * object and conventions documented there. */

/* The derived-class field `DestroyPolarCollectedWumpa` reads: how many fruit the
 * object hands out (one `CollectWumpa` call each). */
struct fruit_actor {
    struct actor_self base;
    u8 unk_54[8];
    s32 fruit;                  // 0x5c
};


/* Same "award `fruit` fruit via `CollectWumpa(gLevelState)`,
 * retarget the vtable to the 'dead' state, unlink from the circular
 * `+0x48`/`+0x4c` list, free on `arg1 & 1`" teardown shape as
 * `DestroyPolarPlayer` above, but with a plain iteration count instead of a
 * `gPolarQueuedWumpa` global drain. */
void DestroyPolarCollectedWumpa(void *selfArg, u32 arg1)
{
    register u8 *self asm("r4") = selfArg;
    u32 arg1r = arg1;
    s32 i;

    *(u8 **)(self + 0x50) = (u8 *)gPolarCollectedWumpaVtable;

    for (i = 0; i < ((struct fruit_actor *)self)->fruit; i++) {
        CollectWumpa(gLevelState);
    }

    *(u8 **)(self + 0x50) = (u8 *)gActorVtable;

    {
        u8 *next = *(u8 **)(self + 0x4c);
        u8 *prev = *(u8 **)(self + 0x48);
        *(u8 **)(next + 0x48) = prev;
    }
    {
        u8 *prev = *(u8 **)(self + 0x48);
        u8 *next = *(u8 **)(self + 0x4c);
        *(u8 **)(prev + 0x4c) = next;
    }

    if (arg1r & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");

/* Thin `InitActorPart`-based constructor (constant last-arg `1`,
 * unlike `CreatePolarWumpa`'s forwarded one), then computes a velocity
 * vector aiming toward a fixed offset point via the screen-projection
 * helpers `GetActorBgCenterY`/`GetActorBgCenterX` plus `__divsi3` division -
 * the "homing/seek-toward-point effect" `CreateJetpackCollectedWumpa` byte-for-byte
 * twins, per docs/rom_map.md.
 *
 * The Manhattan-distance/abs-value computation uses the ROM's own
 * branchless abs idiom (`(x ^ (x >> 31)) - (x >> 31)`, compiling to
 * `asr`/`eor`/`sub`) rather than a `(x < 0) ? -x : x` ternary, which
 * this compiler instead turns into a `cmp`/`bge`/`neg` branch. The
 * `x` (+0x1c) reload also needs pinning to `r1` and reading *after*
 * the `GetActorBgCenterX()` call (not before) - pinning it before the call
 * let this compiler's optimizer silently skip the reload and reuse a
 * stale register value from the unrelated `y` (+0x20) computation two
 * statements earlier, a genuine correctness bug caught by a direct
 * byte compare against the ROM, not just a register-choice cosmetic
 * mismatch. */

/* The seek effect (method table gPolarCollectedWumpaVtable). */
struct polar_collected_wumpa {
    struct actor_self base;
    s32 velX;           // 0x54
    s32 velY;           // 0x58
    s32 count;          // 0x5C - the spawn parameter; DestroyPolarCollectedWumpa (below)
                        // repeats its teardown drain this many times
};

void *CreatePolarCollectedWumpa(void *selfArg, void *part, s32 b, s32 c, s32 spawnParam)
{
    struct polar_collected_wumpa *self = selfArg;
    register s32 dy asm("r3");
    s32 sum;
    s32 q;

    InitActorPart(self, part, b, c, 1);
    self->base.vtable = (struct actor_vtable *)gPolarCollectedWumpaVtable;
    self->count = spawnParam;

    self->base.y += GetActorBgCenterY();

    {
        register s32 ebResult asm("r0") = GetActorBgCenterX();
        register s32 old asm("r1") = self->base.x;
        dy = old + ebResult;
    }
    self->base.x = dy;

    {
        s32 a1 = dy - 0x1000;
        s32 a2 = (a1 ^ (a1 >> 31)) - (a1 >> 31);
        s32 dx = self->base.y;
        s32 b1 = dx - 0x1000;
        s32 b2 = (b1 ^ (b1 >> 31)) - (b1 >> 31);

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

asm(".align 2, 0");

/* Continuation of polar_player_actions.c's player/action-object family, right
 * after `CreatePolarCollectedWumpa` (above) - same `self`
 * object (`struct actor_self`) and conventions documented there. The
 * animation-reset blocks (the `anim`/`zero1`/`zero2` register trios)
 * store through `*(T *)&self->field` casts: plain member stores let
 * gcc move the zero loads (docs/workflow.md step 7). */

/* `UpdatePolarLifeCrate`'s class adds one field after the common prefix: an
 * object it hands to `MarkSpawnCollected`'s 15-entry list. */
struct listed_actor {
    struct actor_self base;
    void *unk_54;               // 0x54
};

/* On proximity (`IsTouchingPlayer`), accumulates `1` into the shared
 * `gActorList`-targeted accumulator via `QueuePolarWumpa` then fires
 * the `vtable` trampoline (behind this family's `if (self)` guard);
 * otherwise tail-calls `UpdateActor(self)`. */
void UpdatePolarWumpa(void *selfArg)
{
    struct actor_self *self = selfArg;

    if ((u8)IsTouchingPlayer(self)) {
        QueuePolarWumpa(gActorList, 1);
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, (void *)3, table->destroy.fn);
        }
    } else {
        UpdateActor(self);
    }
}

/* Thin `InitActorPart`-based constructor: forwards its own `a`/`b`/`c`
 * parameters straight through (untouched, same registers) plus the
 * caller's last stack argument, then sets `vtable` to
 * `gPolarWumpaVtable` - one of the "spawn effect type N" family
 * documented in docs/rom_map.md. */
void *CreatePolarWumpa(void *selfArg, void *part, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitActorPart(self, part, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarWumpaVtable;
    return self;
}

/* Shared cleanup/tail step for this actor family (per docs/rom_map.md):
 * once (state != 0x12 and `IsTouchingYeti`'s overlap test passes),
 * transitions to the shared "used" state 0x12 (anim frame from
 * `self`'s part-table pointer at `+0xd8`) with a sound cue and the
 * lap-counter tie `AddBrokenCrate`. Either way, fires the `vtable`
 * trampoline once state is (already, or now) 0x12 and `animDone` is
 * set; otherwise tail-calls `UpdateActor`. */
void UpdatePolarCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12) {
        if (IsTouchingYeti(self)) {
            PlaySfx(gAudioContext, 3, 0x100);
            AddBrokenCrate(gLevelState);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
        }
    }

    if (self->animIndex == 0x12 && self->animDone != 0) {
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, (void *)3, table->destroy.fn);
        }
        return;
    }

    UpdateActor(self);
}

/* Extends the shared "type-byte event dispatch" family
 * (`UpdateJetpackTimeCrate`/etc., per docs/rom_map.md) to value range `0x1c`-
 * `0x1f`, reading the type byte through one extra pointer indirection
 * (`self+0x30`). Ties into the wraparound-lap-counter system via
 * `AddBrokenCrate` and dispatches accumulator/lock-timer calls
 * (`QueuePolarWumpa`/`GivePolarPlayerMask`) before tail-calling the shared cleanup
 * `UpdatePolarCrate`. */
void UpdatePolarQuestionCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12 && (u8)IsTouchingPlayer(self)) {
        s32 typeByte;

        AddBrokenCrate(gLevelState);
        typeByte = **(u8 **)((u8 *)self + 0x30);

        if (typeByte == 0x1d) {
            goto case_1d;
        }
        if (typeByte > 0x1d) {
            goto gt_1d_dispatch;
        }
        if (typeByte == 0x1c) {
            goto case_1c;
        }
        goto state_block;

    gt_1d_dispatch:
        if (typeByte == 0x1e) {
            goto case_1e;
        }
        if (typeByte == 0x1f) {
            goto case_1f;
        }
        goto state_block;

    case_1c:
        PlaySfx(gAudioContext, 3, 0x100);
        QueuePolarWumpa(gActorList, 1);
        goto state_block;

    case_1d:
        PlaySfx(gAudioContext, 3, 0x100);
        QueuePolarWumpa(gActorList, 3);
        goto state_block;

    case_1e:
        PlaySfx(gAudioContext, 3, 0x100);
        QueuePolarWumpa(gActorList, 5);
        goto state_block;

    case_1f:
        GivePolarPlayerMask(gActorList);

    state_block:
        self->animIndex = 0x12;
        {
            register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            *(s32 *)&self->animTime = zero2;
        }
    }

    UpdatePolarCrate(self);
}

/* Extends the lap-counter/proximity-dispatch family: on proximity
 * (`IsTouchingPlayer`), plays a sound, ties the lap counter, forwards the
 * global player pointer to `GivePolarPlayerLife` and `unk_54` to
 * `MarkSpawnCollected`, then (whether or not that first branch fired) on
 * `IsTouchingYeti`'s overlap test transitions to the shared "used" state
 * a second time with its own sound cue - both branches finish with the
 * same state-0x12 transition block before tail-calling `UpdatePolarCrate`. */
void UpdatePolarLifeCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12) {
        if ((u8)IsTouchingPlayer(self)) {
            PlaySfx(gAudioContext, 7, 0x100);
            AddBrokenCrate(gLevelState);
            GivePolarPlayerLife(gActorList);
            MarkSpawnCollected(((struct listed_actor *)self)->unk_54);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
            self->palette = 1;
        }

        if (self->animIndex != 0x12 && IsTouchingYeti(self)) {
            PlaySfx(gAudioContext, 3, 0x100);
            AddBrokenCrate(gLevelState);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
            self->palette = 1;
        }
    }

    UpdatePolarCrate(self);
}

/* Once already in the "used" state (0x12): refreshes `self+0x38`
 * (a 12-byte AABB, from `gPolarNitroCrateBox`) and, once
 * `stateTime` reaches `0x14`, calls `DetonateNearbyPolarNitros` (still raw - see
 * docs/matching.md). Otherwise, while `depth` (a lap/lifetime
 * counter) exceeds `0xa000`, calls `AddActorMissedNitro` and fires the
 * `vtable` trampoline; else on proximity or overlap, plays a sound,
 * ties the lap counter and the homing-chase helper `HurtPolarPlayer`,
 * and transitions to the "used" state. Tail-calls `UpdatePolarCrate`. */
void UpdatePolarNitroCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex == 0x12) {
        goto usedState;
    }

    if (self->depth > 0xa000) {
        AddActorMissedNitro();
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, (void *)3, table->destroy.fn);
        }
        return;
    } else {
        register u32 raw asm("r0") = (u8)IsTouchingPlayer(self);
        register u32 found asm("r5");

        raw = raw << 24;
        found = raw >> 24;

        if (found) {
            PlaySfx(gAudioContext, 4, 0x100);
            AddBrokenCrate(gLevelState);
            HurtPolarPlayer(gActorList);
            {
                register s32 zero2 asm("r2") = 0;

                self->stateTime = zero2;
                self->animIndex = 0x12;
                {
                    register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero1;
                }
                *(s32 *)&self->animTime = zero2;
            }
        } else if (IsTouchingYeti(self)) {
            PlaySfx(gAudioContext, 4, 0x100);
            AddBrokenCrate(gLevelState);
            {
                register s32 zero2 asm("r5") = found;

                self->stateTime = zero2;
                self->animIndex = 0x12;
                {
                    register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero1;
                }
                *(s32 *)&self->animTime = zero2;
            }
        }
    }
    goto tail;

usedState:
    *(struct vec3_words *)((u8 *)self + 0x38) = *(const struct vec3_words *)&gPolarNitroCrateBox;

    if (self->stateTime == 0x14) {
        DetonateNearbyPolarNitros(self);
    }

tail:
    UpdatePolarCrate(self);
}

asm(".align 2, 0");
