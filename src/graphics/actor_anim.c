#include "core.h"
#include "memory.h"
#include "actor_self.h" /* struct anim_frame_record */

/* Partial view of the same large per-instance "self" object documented
 * at length in actor_part19.c (state at +0x28, table-index at +0xc,
 * anim-frame halfword/byte pair at +0x10/+0x12, +0x48/+0x4c circular
 * list, +0x50 trampoline record, etc.) - only the first 0x10 bytes this
 * file's functions actually touch are named here, per that file's own
 * "none of these objects' full shapes are pinned down yet" convention;
 * most other actor_part*.c files keep using raw offsets into the same
 * bigger object; include/actor_self.h's `struct actor_self` is the
 * fuller view used from actor_part_2fbf0.c on. */
struct anim_part_instance {
    struct anim_frame_record *frameTable; // 0x00
    u32 *frameOffsets;                    // 0x04 - stride 4, indexed by frameTable[idx].frameIndex + GetAnimFrameBaseOffset()
    s32 field_08;                         // 0x08
    s32 frameIndex;                       // 0x0c - current index into frameTable
};

s32 GetAnimFrameBaseOffset(struct anim_part_instance *self)
{
    return self->field_08 >> 8;
}

asm(".align 2, 0");

/* Reads the current keyframe record's `attr` halfword and returns it
 * pre-shifted into the high 16 bits - `DrawJetpackCheckpointText` ORs this straight
 * into an OAM attribute word it builds itself. */
s32 GetAnimFrameAttr(struct anim_part_instance *self)
{
    s32 idx = self->frameIndex;
    struct anim_frame_record *table = self->frameTable;

    return (s32)table[idx].attr << 16;
}

asm(".align 2, 0");

extern void *gCategorySpriteSheet;

/* Resolves the current keyframe's tile-graphics pointer: looks up
 * `frameTable[frameIndex].frameIndex`, adds `GetAnimFrameBaseOffset()`'s
 * result, and uses that as an index into `frameOffsets` (an array of
 * byte offsets) to get a pointer relative to the `gCategorySpriteSheet`
 * tile-graphics base. */
u8 *GetAnimFrameData(struct anim_part_instance *self)
{
    s32 base;
    void **g;
    s32 idx;
    struct anim_frame_record *table;
    s32 val;
    u32 *offsets;

    base = GetAnimFrameBaseOffset(self);
    g = &gCategorySpriteSheet;
    idx = self->frameIndex;
    table = self->frameTable;
    val = table[idx].frameIndex;
    val += base;
    offsets = self->frameOffsets;
    return (u8 *)*g + offsets[val];
}

asm(".align 2, 0");

/* Selects a new keyframe: sets `frameIndex` to `idx`, copies that
 * record's `duration` into `self+0x10`, and resets the `+0x12` flag
 * byte and the `field_08` playback accumulator. */
void SetActorAnim(struct anim_part_instance *self, s32 idx)
{
    struct anim_frame_record *table;
    u16 duration;
    register u8 zero1 asm("r2");
    register s32 zero2 asm("r3");

    self->frameIndex = idx;
    table = self->frameTable;
    duration = table[idx].duration;
    zero1 = 0;
    zero2 = 0;
    *(u16 *)((u8 *)self + 0x10) = duration;
    *((u8 *)self + 0x12) = zero1;
    self->field_08 = zero2;
}

asm(".align 2, 0");

/* Shared shape for the 20 near-identical teardown functions below: same
 * doubly-linked-list unlink convention already named in
 * src/audio/counter_selector.c's `DestroyLogoActor` (`+0x48`=prev,
 * `+0x4c`=next, `+0x50`=state/vtable pointer) - duplicated here rather
 * than shared, matching this project's existing per-file convention for
 * small locally-scoped structs (see `struct aabb`). */
struct linked_node {
    u8 unused_00[0x48];
    struct linked_node *prev;
    struct linked_node *next;
    void *field_50;
};

extern u8 gActorVtable[];

/* Twenty near-identical "kind" teardown handlers: set `self+0x50`'s
 * state/vtable pointer to the shared "dead" table `gActorVtable`,
 * unlink `self` from its `+0x48`(prev)/`+0x4c`(next) circular list, and
 * free `self` when `flags & 1`. All twenty compile to byte-identical
 * bodies in the ROM (confirmed - every one of their embedded literal
 * pointers resolves to the same `gActorVtable` symbol) - almost
 * certainly one shared per-"kind" destructor template that just wasn't
 * deduplicated by the original build, the same way this project's other
 * per-"kind"/per-slot dispatch tables aren't. */
void sub_803B0C4(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void UpdateActor(void *self);

/* Advances `self+0x20` (a Q8 fixed-point accumulator, likely a
 * fall/scroll speed) by a fixed `-0x180`/256 per call, then either
 * fires the `self+0x50` trampoline record (arg `3`) if `self+0x12` is
 * set, or tail-calls `UpdateActor(self)` otherwise - the same
 * `+0x50`-rooted `{s16 offset; void *fn}` trampoline convention
 * documented in actor_part19.c. */
void UpdatePolarCheckpointText(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x20) += -0x180;

    if (self[0x12] != 0) {
        if (self != NULL) {
            u8 *mgr = *(u8 **)(self + 0x50);
            _call_via_r2(self + *(s16 *)(mgr + 8), (void *)3, *(void **)(mgr + 0xc));
        }
    } else {
        UpdateActor(self);
    }
}

asm(".align 2, 0");

void DestroyPolarCheckpointText(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarWumpa(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarTimeCrate(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarQuestionCrate(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarAkuAkuCrate(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarNitroCrate(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarLifeCrate(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B25C(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarBasicCrate(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarCrate(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarElectricFence(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B30C(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B338(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarPenguin(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarIcicle(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarAkuAku(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B3E8(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B414(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyPolarCheckpointCrate(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

extern void SetupSpriteFrameOam(u8 *frame, u32 arg1, u32 arg2, s32 priority);

/* Screen-space visibility test and OAM setup for one sprite frame drawn
 * at the fixed screen position (120, 106): builds the OAM attribute
 * words (masked position, `GetAnimFrameAttr`'s attr flag, and a priority/
 * palette nibble from `self+0x18`/`self+0x14`) and calls
 * `SetupSpriteFrameOam`. See docs/matching/issue-71-0x0803b060-actor.md
 * for the full semantic account.
 *
 * The dead `flag`-equivalent `| 0` closes via the established opaque
 * `asm volatile("orr ...")` idiom; every other register choice (the
 * `w`/`wShift`/`h`/`hShift` load/shift order, the position-word pack,
 * the `self+0x18` priority-nibble unpack, and the final
 * `SetupSpriteFrameOam` argument shuffle) matches the ROM's own
 * register roles once pinned to match, the same techniques worked out
 * for the near-identical twin `DrawPolarCollectedWumpa` (`actor_part19b.c`). Note
 * `DrawJetpackCheckpointText`'s own priority constant is `0x100`, not
 * `DrawPolarCollectedWumpa`'s `0x140` - the two twins differ here. */
void DrawJetpackCheckpointText(void *selfArg)
{
    register u8 *self asm("r5") = selfArg;
    register s32 x asm("r4") = 120;
    register s32 y asm("r6") = 106;
    u8 *frame;
    register u32 packed asm("r3");
    register s32 w asm("r0");
    register s32 wShift asm("r2");
    register s32 h asm("r1");
    register s32 hShift asm("r0");

    frame = GetAnimFrameData((struct anim_part_instance *)self);
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
        register s32 hCheck asm("r0") = h << 3;
        if (y + hCheck < 0) {
            return;
        }
    }
    if (x > 239) {
        return;
    }
    {
        register s32 wCheck asm("r0") = wShift << 1;
        if (x + wCheck < 0) {
            return;
        }
    }

    {
        register s32 attrFlag asm("r0") = GetAnimFrameAttr((struct anim_part_instance *)self);
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
        {
            register s32 zero asm("r0") = 0;
            asm volatile("orr %0, %0, %1" : "+r"(a0) : "r"(zero));
        }
        packed = a0;
    }

    {
        register s32 field24 asm("r4") = *(s32 *)(self + 24);
        s32 a2 = field24 << 12;
        s32 field20 = *(s32 *)(self + 20);
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
            register s32 argPriority asm("r3") = 0x100;

            SetupSpriteFrameOam(argFrame, argPacked, attr2, argPriority);
        }
    }
}

asm(".align 2, 0");

/* Advances the animation frame accumulator, or fires the `+0x50`
 * trampoline record instead when the "held" flag (`+0x12`) is set - the
 * `+0x14` flag written unconditionally at the top looks like a per-call
 * "ticked this frame" marker read elsewhere (no reader matched yet).
 * `self->frameTable[self->frameIndex]`'s `loopThreshold`/`loopBase`
 * (offsets 0x4/0x6, newly named on `struct anim_frame_record` here -
 * previously an opaque `unknown_04[4]`) implement a loop-back: once the
 * frame base offset reaches `loopThreshold`, `field_08` is stepped back
 * by `(loopThreshold - loopBase) << 8` and the `+0x12` "held" flag is
 * set (mirroring `SetActorAnim`'s use of the same halfword/byte pair). */
void UpdateJetpackCheckpointText(void *selfArg)
{
    /* A single `self` pointer, not also a `struct anim_part_instance *`
     * local - keeping both alive at once costs this compiler an extra
     * register and a spurious `mov` the ROM doesn't have (the struct type
     * is only needed transiently, for the GetAnimFrameBaseOffset() call
     * itself). Pinned to r4: without the pin, this compiler puts `self`
     * in a scratch register for the early-return branch (its last use
     * there is right before a call, so nothing forces a callee-saved
     * home) but still needs r4 for the other branch (used again after
     * the GetAnimFrameBaseOffset() call) - the ROM picks r4 for both
     * branches uniformly instead of branch-locally optimizing. */
    register struct actor_self *self asm("r4") = selfArg;

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

        register s32 delta asm("r1");
        asm("mov r3, #0x10\n\tldrsh r1, [r4, r3]" : "=r"(delta) : : "r3");
        self->animTime += delta;
        self->animDone = 0;
        base = GetAnimFrameBaseOffset((struct anim_part_instance *)self);
        {
            s32 off;

            idx = self->animIndex;
            table = self->anims;
            off = idx * (s32)sizeof(struct anim_frame_record);
            off += (s32)table;
            rec = (struct anim_frame_record *)off;
        }
        if (base >= rec->loopThreshold) {
            self->animTime -= (rec->loopThreshold - rec->loopBase) << 8;
            self->animDone = 1;
        }
    }
}

asm(".align 2, 0");

/* No direct `bl`/`.4byte` reference found in any asm/*.s, expected/*.s
 * or src/*.c file, but - like the 20 "kind" teardown handlers already
 * matched in this file (sub_803B0C4 onward) - that doesn't mean
 * unreachable: those are also grep-invisible, since whatever installs
 * them as a "kind"'s destroy/vtable slot does so from a still-raw data
 * table this project hasn't symbolized yet, not a readable `bl`. The
 * original disassembly never gave this address its own function label -
 * it sits as four bytes of real, coherent Thumb code (movs r0, #1;
 * bx lr) squeezed between UpdateJetpackCheckpointText's real return and the next
 * labelled function, DestroyJetpackCheckpointText below (same pattern documented for
 * sub_800039C/strlen in docs/decomp_dev.md - see the matching split
 * entry in expected/corrections.txt for this address). A trivial
 * "return true" stub, plausibly a vtable slot default. */
s32 IsJetpackCheckpointTextUnshootable(void)
{
    return 1;
}

asm(".align 2, 0");

/* Another hidden function with no `thumb_func_start` label of its own
 * (see `IsJetpackCheckpointTextUnshootable` above) - the standard "kind" teardown handler
 * shape already matched 20 times over in this file (`sub_803B0C4`
 * onward) and again below (`DestroyJetpackExplosion` onward): set `self->field_50`
 * to the shared "dead" table, unlink `self` from its `+0x48`/`+0x4c`
 * circular list, and free `self` when `flags & 1`. */
void DestroyJetpackCheckpointText(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

/* A third hidden, unlabelled function (see `IsJetpackCheckpointTextUnshootable` above) -
 * `UpdatePolarCheckpointText`'s near-twin: advances `self+0x24` (a Q8 fixed-point
 * accumulator, `+170`/256 per call this time instead of `-0x180`/256)
 * and either fires the `+0x50` trampoline record if `self+0x12` is set,
 * or tail-calls `UpdateActor(self)` otherwise. */
void UpdateJetpackExplosion(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x24) += 170;

    if (self[0x12] != 0) {
        if (self != NULL) {
            u8 *mgr = *(u8 **)(self + 0x50);
            _call_via_r2(self + *(s16 *)(mgr + 8), (void *)3, *(void **)(mgr + 0xc));
        }
    } else {
        UpdateActor(self);
    }
}

asm(".align 2, 0");

/* Byte-identical to IsJetpackCheckpointTextUnshootable above (see its doc comment for the
 * "no direct reference found, but grep-invisible callers are normal for
 * this vtable-dispatched family" caveat), and hidden the same way - see
 * this address's own split entry in expected/corrections.txt. */
s32 IsJetpackExplosionUnshootable(void)
{
    return 1;
}

asm(".align 2, 0");

void DestroyJetpackExplosion(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

/* `actor_self` plus the first derived-class word at +0x54. */
struct actor_self_54 {
    struct actor_self base;
    s32 unk_54;         // 0x54
};

/* A fourth hidden function with no thumb_func_start of its own (see
 * IsJetpackCheckpointTextUnshootable above, including its "no direct reference found" caveat)
 * - a plain self->unk_54 getter. */
s32 GetActorHp(struct actor_self_54 *self)
{
    return self->unk_54;
}

asm(".align 2, 0");

/* A fifth hidden function (see IsJetpackCheckpointTextUnshootable above) - a genuinely empty
 * stub, same shape as nullsub_16 (src/graphics/actor_part39.c). */
void nullsub_44(void *self)
{
}

asm(".align 2, 0");

/* A sixth hidden function (see IsJetpackCheckpointTextUnshootable above) - a trivial "return
 * 0" stub. */
s32 IsJetpackPlayerUnshootable(void)
{
    return 0;
}

asm(".align 2, 0");

void DestroyJetpackShot(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyJetpackPlane(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyJetpackBomber(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyJetpackCannonball(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyAirshipFireball(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyJetpackBalloon(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

extern void DestroyJetpackBalloonCrate(void *arg0, s32 arg1);

/* A different teardown shape from the `linked_node` handlers above -
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

asm(".align 2, 0");

void DestroyJetpackTimeCrate(void *self, u32 flags)
{
    DestroyJetpackBalloonCrate(self, 0);
    if (flags & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");

void DestroyJetpackQuestionCrate(void *self, u32 flags)
{
    DestroyJetpackBalloonCrate(self, 0);
    if (flags & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");

void DestroyJetpackParachuteNitro(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyJetpackRocket(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyJetpackRing(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyHovercraftFireball(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyHovercraftCannon(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyHovercraftLauncher(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyHovercraftSideGun(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void DestroyHovercraftCannonFlash(struct linked_node *self, u32 flags)
{
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");
