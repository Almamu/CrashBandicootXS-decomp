#include "core.h"
#include "actor.h"
#include "hud.h"

/* Fixed 3-entry queue object for a particle/projectile-trajectory
 * effect - docs/rom_map.md's "fx" investigation (the "Correcting `hud`"
 * section) documents how the consumer (`sub_8026F54`) and producer
 * (`sub_8027018`) use these fields. Allocated at `gUnknown_030012C8` via
 * `sub_8026EDC(0x48)`, matching this struct's size.
 *
 * Reading both functions in full (docs/matching/
 * issue-45-hud-stat-widget-dispatcher.md's "Third pass" section) settled
 * the remaining fields: each of the 3 slots pairs a `targets`/`lists`
 * pointer pair with a `periods`/`counts` scalar pair, and `sub_8026F54`
 * (the per-frame consumer) rotates `targets[i]` by one position, once
 * every `periods[i]` frames (`gUnknown_0300082C % periods[i] == 0`),
 * walking the permutation order given by `lists[i]` - forwards or
 * backwards depending on `direction`. `sub_8027018` (the producer) only
 * ever appends at `count` (no wraparound seen in either function - the
 * caller resets the queue via `sub_8027088`/`sub_80270C0` between
 * bursts rather than this pair enforcing the 3-slot cap itself). */
struct hud_fx_queue {
    u8 active;              /* +0x00 */
    u8 unknown_01[3];       /* +0x01 */
    s32 fields_e[3];        /* +0x04 - only ever written (to 0) by
                              * sub_8027018; never read by either function
                              * matched here. Purpose unconfirmed. */
    u16 *targets[3];        /* +0x10 - array sub_8026F54 rotates. */
    u16 *lists[3];           /* +0x1c - permutation order (as u16 indices
                               * into `targets[i]`), `counts[i]` long. */
    s32 periods[3];           /* +0x28 - sub_8027018 sets this from
                                * sub_803ADB4(0x3C, angle_arg); sub_8026F54
                                * rotates slot i once every `periods[i]`
                                * frames. */
    s32 counts[3];             /* +0x34 - `lists[i]`'s element count. */
    s32 count;                  /* +0x40 - number of active slots (0-3). */
    u8 direction;                 /* +0x44 - 0/1 selects which end of
                                    * `lists[i]` the rotation starts from. */
    u8 unknown_45[3];
};

COMPILE_TIME_ASSERT(sizeof(struct hud_fx_queue) == 0x48);

extern void sub_8026ED0(void *ptr);
extern s32 gUnknown_0300086C;
extern void sub_8008890(struct actor *part, s32 arg1, s32 arg2);
extern void sub_80088F0(struct actor *part, u32 arg1);
extern struct actor *sub_8008904(struct actor *part);
extern u8 gStaticData_087E4CB4[];

extern u32 gUnknown_0300082C;
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern u32 sub_803AF1C(u32 a, u32 b);

/* Per-frame consumer: for each active slot whose period has elapsed
 * this frame, rotates `targets[i]` by one position along the order
 * `lists[i]` gives - see the struct's doc comment above. */
void sub_8026F54(struct hud_fx_queue *self)
{
    register s32 i asm("r4");
    s32 count;
    /* `next_i` is computed right after the `sub_803AF1C` call below
     * (regardless of its result) and stashed in `ip` - a register with
     * no other role in this loop - freeing r4 (which still holds the
     * *old* `i`, since r4 is callee-saved and survives the call) for
     * `target` to reuse for the rest of this iteration's body, exactly
     * like the ROM. Used as the `for` loop's own increment expression
     * below so both the normal and `continue`d paths fall through the
     * same single bottom-of-loop test - matching the ROM's one shared
     * `SKIP` label - rather than each getting their own copy of it. */
    register s32 next_i asm("ip");

    if (!self->active) {
        return;
    }

    i = 0;
    count = self->count;
    for (; i < count; i = next_i) {
        /* `offset` is `i*4`, computed once here and kept live (in r5, a
         * callee-saved register) across the `sub_803AF1C` call below, so
         * every per-slot field access this iteration reuses it instead
         * of recomputing `i*4` fresh. Plain `self->arr[i]` struct access
         * recomputes `i*4` after the call instead (its register gets
         * clobbered by the callee) - tried and confirmed to change the
         * generated code, so the field accesses below stay raw pointer
         * arithmetic off this cached offset; see docs/workflow.md
         * step 7. */
        register s32 offset asm("r5");
        u16 *target;
        u16 *list;
        s32 mod_result;

        {
            /* ROM fetches this global's *address* first, then computes
             * `offset`/builds the periods-slot address, and only
             * dereferences the global right before the call - not
             * immediately after taking its address. A plain
             * `u32 global_val = gUnknown_0300082C;` local dereferences
             * it immediately instead (tried and confirmed to change the
             * generated code); see docs/workflow.md step 7. */
            u32 *global_addr = &gUnknown_0300082C;
            u8 *p;

            offset = i << 2;
            p = (u8 *)self;
            p += 0x28;
            p += offset;
            mod_result = sub_803AF1C(*global_addr, *(s32 *)p);
        }
        next_i = i + 1;
        if (mod_result != 0) {
            continue;
        }

        {
            u8 *p = (u8 *)self;
            p += 0x10;
            p += offset;
            target = *(u16 **)p;
        }

        {
            u8 *p = (u8 *)self;
            p += 0x1c;
            p += offset;
            list = *(u16 **)p;
        }

        if (self->direction) {
            u16 carry;
            register s32 n asm("r1");

            {
                u8 *p = (u8 *)self;
                p += 0x34;
                p += offset;
                n = *(s32 *)p;
            }
            carry = target[list[n - 1]];
            if (n > 0) {
                s32 j = n;
                do {
                    /* Plain C reverses this commutative ADD's operands -
                     * see docs/workflow.md step 7 and hud_counter.c's
                     * matching note on the same idiom. */
                    u16 idx_val = *list;
                    u32 addr_val;
                    u16 *addr;
                    asm volatile("lsl %0, %1, #1" : "=r"(addr_val) : "r"(idx_val));
                    asm volatile("add %0, %0, %1" : "+r"(addr_val) : "r"(target));
                    addr = (u16 *)addr_val;
                    {
                        u16 old = *addr;
                        *addr = carry;
                        carry = old;
                    }
                    list++;
                    j--;
                } while (j != 0);
            }
        } else {
            u16 carry;
            s32 n;

            {
                /* Split so the index-read and the shifted address land in
                 * different registers, matching the ROM's `ldrh r1,.../
                 * lsls r0,r1,...` pair - see docs/workflow.md step 7. */
                register u16 idx_val asm("r1") = list[0];
                register u32 shifted asm("r0");
                asm volatile("lsl %0, %1, #1" : "=r"(shifted) : "r"(idx_val));
                asm volatile("add %0, %0, %1" : "+r"(shifted) : "r"(target));
                carry = *(u16 *)shifted;
            }
            {
                /* ROM fuses the `- 1` into the same `subs` that produces
                 * the loop counter, never keeping the un-decremented
                 * `counts[i]` value in its own register the way path_a
                 * does (there it's compared against 0 before the
                 * decrement) - see docs/workflow.md step 7. */
                u8 *p = (u8 *)self;
                p += 0x34;
                p += offset;
                n = *(s32 *)p - 1;
            }
            if (n >= 0) {
                /* Plain C reverses this commutative ADD's operands - see
                 * docs/workflow.md step 7. */
                {
                    register u32 byte_off asm("r0");
                    asm volatile("lsl %0, %1, #1" : "=r"(byte_off) : "r"(n));
                    asm volatile("add %0, %1, %0" : "+r"(list) : "r"(byte_off));
                }
                do {
                    /* Plain C reverses this commutative ADD's operands -
                     * see docs/workflow.md step 7 and hud_counter.c's
                     * matching note on the same idiom. */
                    u16 idx_val = *list;
                    u32 addr_val;
                    u16 *addr;
                    asm volatile("lsl %0, %1, #1" : "=r"(addr_val) : "r"(idx_val));
                    asm volatile("add %0, %0, %1" : "+r"(addr_val) : "r"(target));
                    addr = (u16 *)addr_val;
                    {
                        u16 old = *addr;
                        *addr = carry;
                        carry = old;
                    }
                    list--;
                    n--;
                } while (n >= 0);
            }
        }
    }
}

/* Producer: appends a new slot at `count` (no wraparound - see the
 * struct's doc comment), computing `periods[count]` from `angle` via the
 * atan2-style `sub_803ADB4` helper. */
void sub_8027018(struct hud_fx_queue *self, u16 *targets_arg, u16 *lists, s32 angle, s32 list_count, u8 direction_arg)
{
    /* ROM reads this 6th (stack-passed) `u8` argument as a genuine
     * `ldrb` off a computed stack address, right at function entry.
     * Plain `u8` parameter access here instead reads the full stack
     * word and narrows it with two shifts, scheduled later - the same
     * gcc-2.9 stack-argument codegen gap documented in
     * docs/matching/issue-3-overlay-ui-audio-wrapper.md's `sub_80019F8`
     * entry. Same fix: bypass the parameter read with a literal
     * inline-asm anchor reproducing the ROM's exact instruction pair. */
    u32 direction;
    /* ROM copies `targets` out of its natural r1 parameter register
     * into r4 immediately (used for the null check), later reusing r4
     * (once `targets` has been stored through) for `&self->periods[idx]`
     * - the address that stays live across the `sub_803ADB4` call below.
     * Plain `u16 *targets = targets_arg;` leaves it in r1 instead and
     * picks a different register for the periods address - tried and
     * confirmed to change the generated code, so this stays a register
     * pin; see docs/workflow.md step 7. */
    register u16 *targets asm("r4") = targets_arg;

    {
        register u32 addr_scratch asm("r0");
        asm volatile("add %1, sp, #0x18\n\tldrb %0, [%1]" : "=r"(direction), "=r"(addr_scratch));
    }

    if (targets == 0 || lists == 0 || &self->counts[0] == 0) {
        self->active = 0;
        return;
    }

    {
        /* Likewise pinned to r1: the ROM computes `idx*4` once here,
         * uses it to place `targets`/`lists`, then lets r1 die (reused
         * for `angle` right before the call) once `&self->periods[idx]`
         * has been computed from it into r4. */
        register s32 offset asm("r1") = self->count << 2;

        {
            u8 *p = (u8 *)self;
            p += 0x10;
            p += offset;
            *(u16 **)p = targets;
        }
        {
            u8 *p = (u8 *)self;
            p += 0x1c;
            p += offset;
            *(u16 **)p = lists;
        }
        {
            register s32 *periods_addr asm("r4");
            u8 *p = (u8 *)self;
            p += 0x28;
            p += offset;
            periods_addr = (s32 *)p;
            *periods_addr = sub_803ADB4(0x3c, angle);
        }
    }
    self->active = 1;
    self->fields_e[self->count] = 0;
    self->counts[self->count] = list_count;
    self->count += 1;
    self->direction = direction;
}

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
        self->targets[i] = 0;
        self->lists[i] = 0;
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
        self->targets[i] = 0;
        self->lists[i] = 0;
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
