#include "core.h"
#include "memory.h"

/* One entry of `anim_part_instance.frameTable`, stride 12 - only the
 * five fields actually read by this file's functions are named; the
 * rest (0xa-0xb) isn't exercised by any function matched so far. */
struct anim_frame_record {
    u16 duration;   // 0x00 - copied into the owning self's +0x10 halfword by sub_803B0A8
    s16 frameIndex; // 0x02 - added to GetAnimFrameBaseOffset()'s result, indexes frameOffsets
    s16 loopThreshold; // 0x04 - sub_803B4EC wraps self->field_08 back once the frame base
                        // offset reaches this value
    s16 loopBase;       // 0x06 - subtracted from loopThreshold (then <<8) as the wrap amount
    u16 attr;       // 0x08 - packed into the high halfword of sub_803B060's return value
    u8 unknown_0a[2];
};

/* Partial view of the same large per-instance "self" object documented
 * at length in actor_part19.c (state at +0x28, table-index at +0xc,
 * anim-frame halfword/byte pair at +0x10/+0x12, +0x48/+0x4c circular
 * list, +0x50 trampoline record, etc.) - only the first 0x10 bytes this
 * file's functions actually touch are named here, per that file's own
 * "none of these objects' full shapes are pinned down yet" convention;
 * every other actor_part*.c file keeps using raw offsets into the same
 * bigger object rather than this struct. */
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
 * pre-shifted into the high 16 bits - `sub_803B46C` ORs this straight
 * into an OAM attribute word it builds itself. */
s32 sub_803B060(struct anim_part_instance *self)
{
    s32 idx = self->frameIndex;
    struct anim_frame_record *table = self->frameTable;

    return (s32)table[idx].attr << 16;
}

asm(".align 2, 0");

extern void *gUnknown_0300137C;

/* Resolves the current keyframe's tile-graphics pointer: looks up
 * `frameTable[frameIndex].frameIndex`, adds `GetAnimFrameBaseOffset()`'s
 * result, and uses that as an index into `frameOffsets` (an array of
 * byte offsets) to get a pointer relative to the `gUnknown_0300137C`
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
    g = &gUnknown_0300137C;
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
void sub_803B0A8(struct anim_part_instance *self, s32 idx)
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
 * src/audio/counter_selector.c's `sub_803716C` (`+0x48`=prev,
 * `+0x4c`=next, `+0x50`=state/vtable pointer) - duplicated here rather
 * than shared, matching this project's existing per-file convention for
 * small locally-scoped structs (see `struct aabb`). */
struct linked_node {
    u8 unused_00[0x48];
    struct linked_node *prev;
    struct linked_node *next;
    void *field_50;
};

extern u8 gStaticData_087E4DF4[];

/* Twenty near-identical "kind" teardown handlers: set `self+0x50`'s
 * state/vtable pointer to the shared "dead" table `gStaticData_087E4DF4`,
 * unlink `self` from its `+0x48`(prev)/`+0x4c`(next) circular list, and
 * free `self` when `flags & 1`. All twenty compile to byte-identical
 * bodies in the ROM (confirmed - every one of their embedded literal
 * pointers resolves to the same `gStaticData_087E4DF4` symbol) - almost
 * certainly one shared per-"kind" destructor template that just wasn't
 * deduplicated by the original build, the same way this project's other
 * per-"kind"/per-slot dispatch tables aren't. */
void sub_803B0C4(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_802A7B8(void *self);

/* Advances `self+0x20` (a Q8 fixed-point accumulator, likely a
 * fall/scroll speed) by a fixed `-0x180`/256 per call, then either
 * fires the `self+0x50` trampoline record (arg `3`) if `self+0x12` is
 * set, or tail-calls `sub_802A7B8(self)` otherwise - the same
 * `+0x50`-rooted `{s16 offset; void *fn}` trampoline convention
 * documented in actor_part19.c. */
void sub_803B0F0(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x20) += -0x180;

    if (self[0x12] != 0) {
        if (self != NULL) {
            u8 *mgr = *(u8 **)(self + 0x50);
            sub_803AD80(self + *(s16 *)(mgr + 8), (void *)3, *(void **)(mgr + 0xc));
        }
    } else {
        sub_802A7B8(self);
    }
}

asm(".align 2, 0");

void sub_803B128(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B154(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B180(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B1AC(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B1D8(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B204(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B230(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B25C(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B288(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B2B4(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B2E0(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B30C(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B338(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B364(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B390(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B3BC(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B3E8(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B414(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B440(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
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
 * words (masked position, `sub_803B060`'s attr flag, and a priority/
 * palette nibble from `self+0x18`/`self+0x14`) and calls
 * `SetupSpriteFrameOam`. See docs/matching/issue-71-0x0803b060-actor.md
 * for the full semantic account.
 *
 * Written as NAKED asm, not plain C: `sub_802C2FC` (src/graphics/
 * actor_part19b.c) is the near-identical twin of this function (same
 * shape, self-relative position instead of a fixed one), parked on two
 * compiler gaps a plain-C reconstruction hit here too - a `| 0`-with-a-
 * zero-valued-local term the ROM keeps as a real materialize-and-OR
 * pair but this compiler's dead-store elimination always removes
 * regardless of phrasing, and a register-budget difference (this
 * compiler needs an extra spilled/high register to keep `frame` alive
 * across both calls where the ROM fits entirely in r4-r7). Every
 * load/store, branch and call was already confirmed correct against
 * the ROM, so this is a mechanical, byte-verified transcription of the
 * ROM's own instructions (translated from the disassembler's unified
 * syntax to this project's established NAKED plain/divided syntax,
 * local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess. */
NAKED void sub_803B46C(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r5, r0, #0\n\t"
        "mov r4, #120\n\t"
        "mov r6, #106\n\t"
        "bl GetAnimFrameData\n\t"
        "add r7, r0, #0\n\t"
        "ldrb r0, [r7]\n\t"
        "lsl r2, r0, #2\n\t"
        "ldrb r1, [r7, #1]\n\t"
        "lsl r0, r1, #2\n\t"
        "sub r4, r4, r2\n\t"
        "sub r6, r6, r0\n\t"
        "cmp r6, #159\n\t"
        "bgt 7f\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r6, r0\n\t"
        "cmp r0, #0\n\t"
        "blt 7f\n\t"
        "cmp r4, #239\n\t"
        "bgt 7f\n\t"
        "lsl r0, r2, #1\n\t"
        "add r0, r4, r0\n\t"
        "cmp r0, #0\n\t"
        "blt 7f\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_803B060\n\t"
        "mov r3, #255\n\t"
        "and r3, r6\n\t"
        "ldr r1, 4f\n\t"
        "and r4, r1\n\t"
        "lsl r1, r4, #16\n\t"
        "orr r3, r1\n\t"
        "orr r3, r0\n\t"
        "mov r0, #0\n\t"
        "orr r3, r0\n\t"
        "ldr r4, [r5, #24]\n\t"
        "lsl r2, r4, #12\n\t"
        "ldr r0, [r5, #20]\n\t"
        "mov r1, #128\n\t"
        "lsl r1, r1, #8\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "mov r0, #128\n\t"
        "lsl r0, r0, #4\n\t"
        "orr r2, r0\n\t"
        "lsl r0, r2, #16\n\t"
        "b 6f\n\t"
    "4: .4byte 0x1ff\n"
    "5:\n\t"
        "lsl r0, r4, #28\n\t"
    "6:\n\t"
        "lsr r2, r0, #16\n\t"
        "add r0, r7, #0\n\t"
        "add r1, r3, #0\n\t"
        "mov r3, #128\n\t"
        "lsl r3, r3, #1\n\t"
        "bl SetupSpriteFrameOam\n\t"
    "7:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
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
 * set (mirroring `sub_803B0A8`'s use of the same halfword/byte pair). */
void sub_803B4EC(void *selfArg)
{
    /* A single raw-offset `self`, not also a `struct anim_part_instance *`
     * local - keeping both alive at once costs this compiler an extra
     * register and a spurious `mov` the ROM doesn't have (the struct type
     * is only needed transiently, for the GetAnimFrameBaseOffset() call
     * itself). Pinned to r4: without the pin, this compiler puts `self`
     * in a scratch register for the early-return branch (its last use
     * there is right before a call, so nothing forces a callee-saved
     * home) but still needs r4 for the other branch (used again after
     * the GetAnimFrameBaseOffset() call) - the ROM picks r4 for both
     * branches uniformly instead of branch-locally optimizing. */
    register u8 *self asm("r4") = selfArg;

    *(s32 *)(self + 0x14) = 1;

    if (self[0x12] != 0) {
        if (self != NULL) {
            u8 *mgr = *(u8 **)(self + 0x50);
            sub_803AD80(self + *(s16 *)(mgr + 8), (void *)3, *(void **)(mgr + 0xc));
        }
    } else {
        s32 base;
        s32 idx;
        struct anim_frame_record *table;
        struct anim_frame_record *rec;

        register s32 delta asm("r1");
        asm("mov r3, #0x10\n\tldrsh r1, [r4, r3]" : "=r"(delta) : : "r3");
        *(s32 *)(self + 0x08) += delta;
        self[0x12] = 0;
        base = GetAnimFrameBaseOffset((struct anim_part_instance *)self);
        {
            s32 off;

            idx = *(s32 *)(self + 0x0c);
            table = *(struct anim_frame_record **)self;
            off = idx * (s32)sizeof(struct anim_frame_record);
            off += (s32)table;
            rec = (struct anim_frame_record *)off;
        }
        if (base >= rec->loopThreshold) {
            *(s32 *)(self + 0x08) -= (rec->loopThreshold - rec->loopBase) << 8;
            self[0x12] = 1;
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
 * bx lr) squeezed between sub_803B4EC's real return and the next
 * labelled function, sub_803B550 below (same pattern documented for
 * sub_800039C/strlen in docs/decomp_dev.md - see the matching split
 * entry in expected/corrections.txt for this address). A trivial
 * "return true" stub, plausibly a vtable slot default. */
s32 sub_803B54C(void)
{
    return 1;
}

asm(".align 2, 0");

/* Another hidden function with no `thumb_func_start` label of its own
 * (see `sub_803B54C` above) - the standard "kind" teardown handler
 * shape already matched 20 times over in this file (`sub_803B0C4`
 * onward) and again below (`sub_803B5B0` onward): set `self->field_50`
 * to the shared "dead" table, unlink `self` from its `+0x48`/`+0x4c`
 * circular list, and free `self` when `flags & 1`. */
void sub_803B550(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

/* A third hidden, unlabelled function (see `sub_803B54C` above) -
 * `sub_803B0F0`'s near-twin: advances `self+0x24` (a Q8 fixed-point
 * accumulator, `+170`/256 per call this time instead of `-0x180`/256)
 * and either fires the `+0x50` trampoline record if `self+0x12` is set,
 * or tail-calls `sub_802A7B8(self)` otherwise. */
void sub_803B57C(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x24) += 170;

    if (self[0x12] != 0) {
        if (self != NULL) {
            u8 *mgr = *(u8 **)(self + 0x50);
            sub_803AD80(self + *(s16 *)(mgr + 8), (void *)3, *(void **)(mgr + 0xc));
        }
    } else {
        sub_802A7B8(self);
    }
}

asm(".align 2, 0");

/* Byte-identical to sub_803B54C above (see its doc comment for the
 * "no direct reference found, but grep-invisible callers are normal for
 * this vtable-dispatched family" caveat), and hidden the same way - see
 * this address's own split entry in expected/corrections.txt. */
s32 sub_803B5AC(void)
{
    return 1;
}

asm(".align 2, 0");

void sub_803B5B0(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

/* A fourth hidden function with no thumb_func_start of its own (see
 * sub_803B54C above, including its "no direct reference found" caveat)
 * - a plain self->field_54 getter. */
s32 sub_803B5DC(void *self)
{
    return *(s32 *)((u8 *)self + 0x54);
}

asm(".align 2, 0");

/* A fifth hidden function (see sub_803B54C above) - a genuinely empty
 * stub, same shape as nullsub_16 (src/graphics/actor_part39.c). */
void nullsub_44(void *self)
{
}

asm(".align 2, 0");

/* A sixth hidden function (see sub_803B54C above) - a trivial "return
 * 0" stub. */
s32 sub_803B5E4(void)
{
    return 0;
}

asm(".align 2, 0");

void sub_803B5E8(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B614(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B640(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B66C(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B698(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B6C4(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

extern void sub_80321D0(void *arg0, s32 arg1);

/* A different teardown shape from the `linked_node` handlers above -
 * tears down via `sub_80321D0(self, 0)` (still unmatched itself)
 * instead of the inline list-unlink, then frees `self` when
 * `flags & 1`, same as every other handler in this file. */
void sub_803B6F0(void *self, u32 flags)
{
    sub_80321D0(self, 0);
    if (flags & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");

void sub_803B710(void *self, u32 flags)
{
    sub_80321D0(self, 0);
    if (flags & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");

void sub_803B730(void *self, u32 flags)
{
    sub_80321D0(self, 0);
    if (flags & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");

void sub_803B750(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B77C(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B7A8(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B7D4(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B800(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B82C(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B858(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");

void sub_803B884(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

asm(".align 2, 0");
