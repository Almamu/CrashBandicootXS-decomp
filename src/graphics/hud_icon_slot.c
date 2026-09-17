#include "core.h"
#include "actor.h"
#include "hud.h"

/* Fixed 3-entry queue object for a particle/projectile-trajectory
 * effect - docs/rom_map.md's "fx" investigation (the "Correcting `hud`"
 * section) documents how the consumer (`sub_8026F54`) and producer
 * (`sub_8027018`) use `fields_a`/`fields_b`/two more parallel arrays at
 * +0x28/+0x34 (not touched by this reset pair, so left as padding) and
 * `count`; both of those are left raw. Allocated at `gUnknown_030012C8`
 * via `sub_8026EDC(0x48)`, matching this struct's size. */
struct hud_fx_queue {
    u8 active;
    u8 unknown_01[0xF];
    s32 fields_a[3];    /* +0x10 */
    s32 fields_b[3];    /* +0x1c */
    u8 unknown_28[0x18]; /* +0x28 - fields_c/fields_d, unused here */
    s32 count;           /* +0x40 */
    u8 unknown_44[4];
};

COMPILE_TIME_ASSERT(sizeof(struct hud_fx_queue) == 0x48);

extern void sub_8026ED0(void *ptr);
extern s32 gUnknown_0300086C;
extern void sub_8008890(struct actor *part, s32 arg1, s32 arg2);
extern void sub_80088F0(struct actor *part, u32 arg1);
extern struct actor *sub_8008904(struct actor *part);
extern u8 gStaticData_087E4CB4[];

/* Resets an `hud_fx_queue` to empty - clears the "active" flag, the
 * first three slots of its two touched parallel arrays, and the entry
 * count. Called right before `sub_8027018` (the raw producer) queues a
 * fresh entry - see the call sites in the still-raw game_loop chunk
 * (e.g. `asm/code_3_2_17_231cc.s` around `_08023AF4`). */
void sub_8027088(struct hud_fx_queue *self)
{
    s32 i;

    self->active = 0;
    for (i = 0; i < 3; i++) {
        self->fields_a[i] = 0;
        self->fields_b[i] = 0;
    }
    self->count = 0;
}

/* Teardown counterpart to `sub_80270C0` below: frees `self` via
 * `sub_8026ED0` when bit 0 of `flags` is set. */
void sub_80270A8(void *self, s32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Same reset as `sub_8027088` (minus the entry-count clear - freshly
 * `mem_alloc`'d memory doesn't need it) but returns `self` - this is
 * the queue's constructor, called right after its `sub_8026EDC(0x48)`
 * allocation. */
struct hud_fx_queue *sub_80270C0(struct hud_fx_queue *self)
{
    s32 i;

    self->active = 0;
    for (i = 0; i < 3; i++) {
        self->fields_a[i] = 0;
        self->fields_b[i] = 0;
    }
    return self;
}

/* Draws one HUD digit/icon slot's current frame, unless it's been
 * hidden (`frame_index == -1`, the single-digit case `sub_8027838`
 * sets on the second digit). `gUnknown_0300086C` is the shared HUD
 * layout offset `sub_8027838` derives from a counter's `layout_value`,
 * folded into the vertical position here. */
void sub_80270E0(struct hud_digit_part *part, s32 arg1, s32 arg2)
{
    if (part->frame_index != -1) {
        sub_8008890((struct actor *)part, arg1, arg2 + gUnknown_0300086C);
    }
}

/* UNUSED - no caller anywhere in the ROM (checked asm/*.s,
 * expected/*.s, every src/*.c file). A `struct actor`-table-swap constructor
 * variant of `sub_8027120` below: sets `table` directly instead of
 * going through `sub_8008904`, then forwards to `sub_80088F0` (which
 * immediately overwrites `table` again as part of its own two-step
 * table swap - see actor_part7.c). */
void sub_802710C(struct actor *part, u32 arg1)
{
    part->table = gStaticData_087E4CB4;
    sub_80088F0(part, arg1);
}

/* Constructs one `struct hud_digit_part` slot as a `struct actor`
 * (the two share the same first 0x18 bytes plus `table` at +0x18 - see
 * include/hud.h): re-initializes it via `sub_8008904`, then overwrites
 * `table` with this widget family's own `gStaticData_087E4CB4` in
 * place of whatever `sub_8008904` set it to. */
struct actor *sub_8027120(struct actor *part)
{
    sub_8008904(part);
    part->table = gStaticData_087E4CB4;
    return part;
}
