#include "core.h"
#include "math_util.h"
#include "match.h"
#include "memory.h"
#include "actor_self.h" /* struct anim_frame_record */
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "globals.h"

s32 GetAnimFrameBaseOffset(struct actor_self *self)
{
    return Q8_TO_INT(self->animTime);
}

/* Reads the current keyframe record's `attr` halfword and returns it
 * pre-shifted into the high 16 bits - `DrawJetpackCheckpointText` ORs this straight
 * into an OAM attribute word it builds itself. */
s32 GetAnimFrameAttr(struct actor_self *self)
{
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;

    return (s32)table[idx].attr << 16;
}

/* Resolves the current keyframe's tile-graphics pointer: looks up
 * `anims[animIndex].frameIndex`, adds `GetAnimFrameBaseOffset()`'s
 * result, and uses that as an index into `frameOffsets` (an array of
 * byte offsets) to get a pointer relative to the `gCategorySpriteSheet`
 * tile-graphics base. */
u8 *GetAnimFrameData(struct actor_self *self)
{
    s32 base;
    void **g;
    s32 idx;
    struct anim_frame_record *table;
    s32 val;
    u32 *offsets;

    base = GetAnimFrameBaseOffset(self);
    g = &gCategorySpriteSheet;
    idx = self->animIndex;
    table = self->anims;
    val = table[idx].frameIndex;
    val += base;
    offsets = self->frameOffsets;
    return (u8 *)*g + offsets[val];
}

/* Selects a new keyframe: sets `animIndex` to `idx`, copies that
 * record's `duration` into `animTimer`, and resets the `animDone` flag
 * byte and the `animTime` playback accumulator. */
void SetActorAnim(struct actor_self *self, s32 idx)
{
    struct anim_frame_record *table;
    u16 duration;
    MATCH_HOLD_REG(u8, zero1, r2);
    MATCH_HOLD_REG(s32, zero2, r3);

    self->animIndex = idx;
    table = self->anims;
    duration = table[idx].duration;
    zero1 = 0;
    zero2 = 0;
    self->animTimer = duration;
    /* animDone: through the field, the pinned zero in r2 is dropped and
     * a fresh `mov r1, #0` is emitted. */
    *(u8 *)&self->animDone = zero1;
    self->animTime = zero2;
}

/* Twenty near-identical "kind" teardown handlers: set `self+0x50`'s
 * state/vtable pointer to the shared "dead" table `gActorVtable`,
 * unlink `self` from its `+0x48`(prev)/`+0x4c`(next) circular list, and
 * free `self` when `flags & 1`. All twenty compile to byte-identical
 * bodies in the ROM (confirmed - every one of their embedded literal
 * pointers resolves to the same `gActorVtable` symbol) - almost
 * certainly one shared per-"kind" destructor template that just wasn't
 * deduplicated by the original build, the same way this project's other
 * per-"kind"/per-slot dispatch tables aren't. */
void DestroyRiderlessPolar(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Moves `self->y` up by `0x180`/256 per call, then either calls the
 * `destroy` method (arg `3`) once `animDone` is set, or tail-calls
 * `UpdateActor(self)` otherwise. */
void UpdatePolarCheckpointText(void *selfArg)
{
    struct actor_self *self = selfArg;

    self->y += -0x180;

    if (self->animDone != 0) {
        if (self != NULL) {
            struct actor_vtable *mgr = self->vtable;
            _call_via_r2((u8 *)self + mgr->destroy.thisOffset, (void *)3, mgr->destroy.fn);
        }
    } else {
        UpdateActor(self);
    }
}

void DestroyPolarCheckpointText(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarWumpa(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarTimeCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarQuestionCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarAkuAkuCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarNitroCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarLifeCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarFourWumpaCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarBasicCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarElectricFence(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarObstacle(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarLauncher(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarPenguin(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarIcicle(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarAkuAku(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarGoal(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarBoostPad(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyPolarCheckpointCrate(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

/* Screen-space visibility test and OAM setup for one sprite frame drawn
 * at the fixed screen position (120, 106): builds the OAM attribute
 * words (masked position, `GetAnimFrameAttr`'s attr flag, and a priority/
 * palette nibble from `self+0x18`/`self+0x14`) and calls
 * `SetupSpriteFrameOam`. See docs/matching/archive/issue-71-0x0803b060-actor.md
 * for the full semantic account.
 *
 * The dead `flag`-equivalent `| 0` closes via the established opaque
 * `asm volatile("orr ...")` idiom; every other register choice (the
 * `w`/`wShift`/`h`/`hShift` load/shift order, the position-word pack,
 * the `self+0x18` priority-nibble unpack, and the final
 * `SetupSpriteFrameOam` argument shuffle) matches the ROM's own
 * register roles once pinned to match, the same techniques worked out
 * for the near-identical twin `DrawPolarCollectedWumpa` (`polar_pickups.c`). Note
 * `DrawJetpackCheckpointText`'s own priority constant is `0x100`, not
 * `DrawPolarCollectedWumpa`'s `0x140` - the two twins differ here. */
void DrawJetpackCheckpointText(void *selfArg)
{
    MATCH_HOLD_REG(struct actor_self *, self, r5) = selfArg;
    MATCH_HOLD_REG(s32, x, r4) = 120;
    MATCH_HOLD_REG(s32, y, r6) = 106;
    u8 *frame;
    MATCH_HOLD_REG(u32, packed, r3);
    MATCH_HOLD_REG(s32, w, r0);
    MATCH_HOLD_REG(s32, wShift, r2);
    MATCH_HOLD_REG(s32, h, r1);
    MATCH_HOLD_REG(s32, hShift, r0);

    frame = GetAnimFrameData(self);
    w = frame[0];
    wShift = w << 2;
    h = frame[1];
    hShift = h << 2;
    x -= wShift;
    y -= hShift;
    if (y > 159) {
        return;
    }
    {
        MATCH_HOLD_REG(s32, hCheck, r0) = h << 3;
        if (y + hCheck < 0) {
            return;
        }
    }
    if (x > 239) {
        return;
    }
    {
        MATCH_HOLD_REG(s32, wCheck, r0) = wShift << 1;
        if (x + wCheck < 0) {
            return;
        }
    }

    {
        MATCH_HOLD_REG(s32, attrFlag, r0) = GetAnimFrameAttr(self);
        MATCH_HOLD_REG(s32, a0, r3) = 0xff;
        MATCH_HOLD_REG(s32, xm, r4) = x;

        a0 &= y;
        {
            MATCH_HOLD_REG(s32, mask, r1) = 0x1ff;
            MATCH_HOLD_REG(s32, shifted, r1);

            xm &= mask;
            shifted = xm << 16;
            a0 |= shifted;
        }
        a0 |= attrFlag;
        {
            MATCH_HOLD_REG(s32, zero, r0) = 0;
            asm volatile("orr %0, %0, %1" : "+r"(a0) : "r"(zero));
        }
        packed = a0;
    }

    {
        MATCH_HOLD_REG(s32, palette, r4) = self->palette;
        s32 a2 = palette << 12;
        s32 sortKey = self->sortKey;
        MATCH_HOLD_REG(u32, attr2, r2);

        if (sortKey & SORT_KEY_FLAG_BEHIND_BG) {
            a2 |= 0x800;
            {
                MATCH_HOLD_REG(s32, shifted, r0) = a2 << 16;
                attr2 = (u32)shifted >> 16;
            }
        } else {
            MATCH_HOLD_REG(s32, shifted, r0) = palette << 28;
            attr2 = (u32)shifted >> 16;
        }
        {
            MATCH_HOLD_REG(u8 *, argFrame, r0) = frame;
            MATCH_HOLD_REG(u32, argPacked, r1) = packed;
            MATCH_HOLD_REG(s32, argPriority, r3) = 0x100;

            SetupSpriteFrameOam(argFrame, argPacked, attr2, argPriority);
        }
    }
}

/* Advances the animation frame accumulator, or fires the `+0x50`
 * trampoline record instead when the "held" flag (`+0x12`) is set - the
 * `+0x14` flag written unconditionally at the top looks like a per-call
 * "ticked this frame" marker read elsewhere (no reader matched yet).
 * `self->anims[self->animIndex]`'s `loopThreshold`/`loopBase`
 * (offsets 0x4/0x6, newly named on `struct anim_frame_record` here -
 * previously an opaque `unknown_04[4]`) implement a loop-back: once the
 * frame base offset reaches `loopThreshold`, `animTime` is stepped back
 * by `(loopThreshold - loopBase) << 8` and the `+0x12` "held" flag is
 * set (mirroring `SetActorAnim`'s use of the same halfword/byte pair). */
void UpdateJetpackCheckpointText(void *selfArg)
{
    /* A single `self` pointer: keeping a second typed copy alive costs
     * this compiler an extra register and a spurious `mov` the ROM
     * doesn't have. Pinned to r4: without the pin, this compiler puts `self`
     * in a scratch register for the early-return branch (its last use
     * there is right before a call, so nothing forces a callee-saved
     * home) but still needs r4 for the other branch (used again after
     * the GetAnimFrameBaseOffset() call) - the ROM picks r4 for both
     * branches uniformly instead of branch-locally optimizing. */
    MATCH_HOLD_REG(struct actor_self *, self, r4) = selfArg;

    self->sortKey = 1;

    if (self->animDone != 0) {
        if (self != NULL) {
            struct actor_vtable *mgr = self->vtable;
            _call_via_r2((u8 *)self + mgr->destroy.thisOffset, (void *)3, mgr->destroy.fn);
        }
    } else {
        s32 base;
        s32 idx;
        struct anim_frame_record *table;
        struct anim_frame_record *rec;

        MATCH_HOLD_REG(s32, delta, r1);
        asm("mov r3, #0x10\n\tldrsh r1, [r4, r3]" : "=r"(delta) : : "r3");
        self->animTime += delta;
        self->animDone = 0;
        base = GetAnimFrameBaseOffset(self);
        {
            s32 off;

            idx = self->animIndex;
            table = self->anims;
            off = idx * (s32)sizeof(struct anim_frame_record);
            off += (s32)table;
            rec = (struct anim_frame_record *)off;
        }
        if (base >= rec->loopThreshold) {
            self->animTime -= INT_TO_Q8(rec->loopThreshold - rec->loopBase);
            self->animDone = 1;
        }
    }
}

/* No direct `bl`/`.4byte` reference found in any asm/*.s, expected/*.s
 * or src/*.c file, but - like the 20 "kind" teardown handlers already
 * matched in this file (DestroyRiderlessPolar onward) - that doesn't mean
 * unreachable: those are also grep-invisible, since whatever installs
 * them as a "kind"'s destroy/vtable slot does so from a still-raw data
 * table this project hasn't symbolized yet, not a readable `bl`. The
 * original disassembly never gave this address its own function label -
 * it sits as four bytes of real, coherent Thumb code (movs r0, #1;
 * bx lr) squeezed between UpdateJetpackCheckpointText's real return and the next
 * labelled function, DestroyJetpackCheckpointText below (same pattern documented for
 * mem_walk_heaps/strlen in docs/decomp_dev.md - see the matching split
 * entry in expected/corrections.txt for this address). A trivial
 * "return true" stub, plausibly a vtable slot default. */
s32 IsJetpackCheckpointTextUnshootable(void)
{
    return 1;
}

/* Another hidden function with no `thumb_func_start` label of its own
 * (see `IsJetpackCheckpointTextUnshootable` above) - the standard "kind" teardown handler
 * shape already matched 20 times over in this file (`DestroyRiderlessPolar`
 * onward) and again below (`DestroyJetpackExplosion` onward): set `self->field_50`
 * to the shared "dead" table, unlink `self` from its `+0x48`/`+0x4c`
 * circular list, and free `self` when `flags & 1`. */
void DestroyJetpackCheckpointText(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

/* A third hidden, unlabelled function (see `IsJetpackCheckpointTextUnshootable` above) -
 * `UpdatePolarCheckpointText`'s near-twin: advances `self->z` by 170/256
 * per call instead, and either calls the `destroy` method once `animDone`
 * is set, or tail-calls `UpdateActor(self)` otherwise. */
void UpdateJetpackExplosion(void *selfArg)
{
    struct actor_self *self = selfArg;

    self->z += 170;

    if (self->animDone != 0) {
        if (self != NULL) {
            struct actor_vtable *mgr = self->vtable;
            _call_via_r2((u8 *)self + mgr->destroy.thisOffset, (void *)3, mgr->destroy.fn);
        }
    } else {
        UpdateActor(self);
    }
}

/* Byte-identical to IsJetpackCheckpointTextUnshootable above (see its doc comment for the
 * "no direct reference found, but grep-invisible callers are normal for
 * this vtable-dispatched family" caveat), and hidden the same way - see
 * this address's own split entry in expected/corrections.txt. */
s32 IsJetpackExplosionUnshootable(void)
{
    return 1;
}

void DestroyJetpackExplosion(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

/* `actor_self` plus the first derived-class word at +0x54. */
struct actor_self_54 {
    struct actor_self base;
    s32 hp; // 0x54 - hit points, where the derived class has them (vehicle.h, bosses.h)
};

/* A fourth hidden function with no thumb_func_start of its own (see
 * IsJetpackCheckpointTextUnshootable above, including its "no direct reference found" caveat)
 * - a plain self->hp getter. */
s32 GetActorHp(struct actor_self_54 *self)
{
    return self->hp;
}

/* A fifth hidden function (see IsJetpackCheckpointTextUnshootable above).
 * The jetpack actors' damage slot (vtable slot 4, beside
 * DrawActor and GetActorHp) for the classes that take no damage; the
 * damageable ones override it (DamageJetpackPlayer, DamageJetpackPlane,
 * ...). Empty. */
void DamageActor(void *self)
{
}

/* A sixth hidden function (see IsJetpackCheckpointTextUnshootable above) - a trivial "return
 * 0" stub. */
s32 IsJetpackPlayerUnshootable(void)
{
    return 0;
}

void DestroyJetpackShot(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyJetpackPlane(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyJetpackBomber(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyJetpackCannonball(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyAirshipFireball(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyJetpackBalloon(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

/* A different teardown shape from the list-unlinking handlers above -
 * tears down via `DestroyJetpackBalloonCrate(self, 0)` (still unmatched itself)
 * instead of the inline list-unlink, then frees `self` when
 * `flags & 1`, same as every other handler in this file. */
void DestroyJetpackHealthCrate(void *self, u32 flags)
{
    DestroyJetpackBalloonCrate(self, 0);
    if (flags & 1) {
        mem_free(self);
    }
}

void DestroyJetpackTimeCrate(void *self, u32 flags)
{
    DestroyJetpackBalloonCrate(self, 0);
    if (flags & 1) {
        mem_free(self);
    }
}

void DestroyJetpackQuestionCrate(void *self, u32 flags)
{
    DestroyJetpackBalloonCrate(self, 0);
    if (flags & 1) {
        mem_free(self);
    }
}

void DestroyJetpackParachuteNitro(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyJetpackRocket(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyJetpackRing(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyHovercraftFireball(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyHovercraftCannon(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyHovercraftLauncher(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyHovercraftSideGun(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

void DestroyHovercraftCannonFlash(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}
