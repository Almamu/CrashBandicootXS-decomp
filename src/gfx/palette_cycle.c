#include "core.h"
#include "match.h"
#include "gfx.h"
#include "actor.h"
#include "hud.h"
#include <libgcc.h>
#include "objects.h"
#include "memory.h"
#include "globals.h"


/* Per-frame consumer: for each active slot whose period has elapsed
 * this frame, rotates `targets[i]` by one position along the order
 * `lists[i]` gives - see the struct's doc comment above. */
void TickPaletteCycles(struct palette_cycler *self)
{
    MATCH_HOLD_REG(s32, i, r4);
    s32 count;
    /* `next_i` is computed right after the `__umodsi3` call below
     * (regardless of its result) and stashed in `ip` - a register with
     * no other role in this loop - freeing r4 (which still holds the
     * *old* `i`, since r4 is callee-saved and survives the call) for
     * `target` to reuse for the rest of this iteration's body, exactly
     * like the ROM. Used as the `for` loop's own increment expression
     * below so both the normal and `continue`d paths fall through the
     * same single bottom-of-loop test - matching the ROM's one shared
     * `SKIP` label - rather than each getting their own copy of it. */
    MATCH_HOLD_REG(s32, next_i, ip);

    if (!self->active) {
        return;
    }

    i = 0;
    count = self->count;
    for (; i < count; i = next_i) {
        /* `offset` is `i*4`, computed once here and kept live (in r5, a
         * callee-saved register) across the `__umodsi3` call below, so
         * every per-slot field access this iteration reuses it instead
         * of recomputing `i*4` fresh. Plain `self->arr[i]` struct access
         * recomputes `i*4` after the call instead (its register gets
         * clobbered by the callee) - tried and confirmed to change the
         * generated code, so the field accesses below stay raw pointer
         * arithmetic off this cached offset; see docs/workflow.md
         * step 7. */
        MATCH_HOLD_REG(s32, offset, r5);
        u16 *target;
        u16 *list;
        s32 mod_result;

        {
            /* ROM fetches this global's *address* first, then computes
             * `offset`/builds the periods-slot address, and only
             * dereferences the global right before the call - not
             * immediately after taking its address. A plain
             * `u32 global_val = gRoomFrameCount;` local dereferences
             * it immediately instead (tried and confirmed to change the
             * generated code); see docs/workflow.md step 7. */
            u32 *global_addr = &gRoomFrameCount;
            u8 *p;

            offset = i << 2;
            p = (u8 *)self;
            p += 0x28;
            p += offset;
            mod_result = __umodsi3(*global_addr, *(s32 *)p);
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
            MATCH_HOLD_REG(s32, n, r1);

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
                     * see docs/workflow.md step 7 and hud_lives.c's
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
                MATCH_HOLD_REG(u16, idx_val, r1) = list[0];
                MATCH_HOLD_REG(u32, shifted, r0);
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
                    MATCH_HOLD_REG(u32, byte_off, r0);
                    asm volatile("lsl %0, %1, #1" : "=r"(byte_off) : "r"(n));
                    asm volatile("add %0, %1, %0" : "+r"(list) : "r"(byte_off));
                }
                do {
                    /* Plain C reverses this commutative ADD's operands -
                     * see docs/workflow.md step 7 and hud_lives.c's
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
 * struct's doc comment), computing `periods[count]` as 60 / `rate` (`__divsi3`). */
void AddPaletteCycle(struct palette_cycler *self, u16 *targets_arg, u16 *lists, s32 rate,
                     s32 list_count, u8 direction_arg)
{
    /* ROM reads this 6th (stack-passed) `u8` argument as a genuine
     * `ldrb` off a computed stack address, right at function entry.
     * Plain `u8` parameter access here instead reads the full stack
     * word and narrows it with two shifts, scheduled later - the same
     * gcc-2.9 stack-argument codegen gap documented in
     * docs/matching/archive/issue-3-overlay-ui-audio-wrapper.md's `PlayAmbientSfx`
     * entry. Same fix: bypass the parameter read with a literal
     * inline-asm anchor reproducing the ROM's exact instruction pair. */
    u32 direction;
    /* ROM copies `targets` out of its natural r1 parameter register
     * into r4 immediately (used for the null check), later reusing r4
     * (once `targets` has been stored through) for `&self->periods[idx]`
     * - the address that stays live across the `__divsi3` call below.
     * Plain `u16 *targets = targets_arg;` leaves it in r1 instead and
     * picks a different register for the periods address - tried and
     * confirmed to change the generated code, so this stays a register
     * pin; see docs/workflow.md step 7. */
    MATCH_HOLD_REG(u16 *, targets, r4) = targets_arg;

    {
        MATCH_HOLD_REG(u32, addr_scratch, r0);
        asm volatile("add %1, sp, #0x18\n\tldrb %0, [%1]" : "=r"(direction), "=r"(addr_scratch));
    }

    if (targets == 0 || lists == 0 || &self->counts[0] == 0) {
        self->active = 0;
        return;
    }

    {
        /* Likewise pinned to r1: the ROM computes `idx*4` once here,
         * uses it to place `targets`/`lists`, then lets r1 die (reused
         * for `rate` right before the call) once `&self->periods[idx]`
         * has been computed from it into r4. */
        MATCH_HOLD_REG(s32, offset, r1) = self->count << 2;

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
            MATCH_HOLD_REG(s32 *, periods_addr, r4);
            u8 *p = (u8 *)self;
            p += 0x28;
            p += offset;
            periods_addr = (s32 *)p;
            *periods_addr = __divsi3(0x3c, rate);
        }
    }
    self->active = 1;
    self->fields_e[self->count] = 0;
    self->counts[self->count] = list_count;
    self->count += 1;
    self->direction = direction;
}

/* Resets an `palette_cycler` to empty - clears the "active" flag, the
 * first three slots of its two touched parallel arrays, and the entry
 * count. Called right before `AddPaletteCycle` (the raw producer) queues a
 * fresh entry - see the call sites in the still-raw game_loop chunk
 * (e.g. `asm/code_3_2_17_231cc.s` around `_08023AF4`). */
void ClearPaletteCycles(struct palette_cycler *self)
{
    s32 i;

    self->active = 0;
    for (i = 0; i < 3; i++) {
        self->targets[i] = 0;
        self->lists[i] = 0;
    }
    self->count = 0;
}

/* Teardown counterpart to `InitPaletteCycles` below: frees `self` via
 * `OperatorDelete` when bit 0 of `flags` is set. */
void DestroyPaletteCycles(struct palette_cycler *self, s32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Same reset as `ClearPaletteCycles` (minus the entry-count clear - freshly
 * `mem_alloc`'d memory doesn't need it) but returns `self` - this is
 * the queue's constructor, called right after its `OperatorNew(0x48)`
 * allocation. */
struct palette_cycler *InitPaletteCycles(struct palette_cycler *self)
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
 * hidden (`frame_index == -1`, the single-digit case `UpdateHudLives`
 * sets on the second digit). `gHudSlideOffset` is the shared HUD
 * layout offset `UpdateHudLives` derives from a counter's `layout_value`,
 * folded into the vertical position here. */
void DrawHudPart(struct hud_digit_part *part, s32 arg1, s32 arg2)
{
    if (part->frame_index != -1) {
        DrawSpriteWithOffset((struct actor *)part, arg1, arg2 + gHudSlideOffset);
    }
}

/* UNUSED - no caller anywhere in the ROM (checked asm/*.s,
 * expected/*.s, every src/*.c file). A `struct actor`-table-swap constructor
 * variant of `InitHudPart` below: sets `table` directly instead of
 * going through `InitUiSpriteObj`, then forwards to `DestroyUiSpriteObj` (which
 * immediately overwrites `table` again as part of its own two-step
 * table swap - see sprite_anim.c). */
void sub_802710C(struct actor *part, u32 arg1)
{
    part->table = (void *)gHudPartVtable;
    DestroyUiSpriteObj(part, arg1);
}

/* Constructs one `struct hud_digit_part` slot as a `struct actor`
 * (the two share the same first 0x18 bytes plus `table` at +0x18 - see
 * include/hud.h): re-initializes it via `InitUiSpriteObj`, then overwrites
 * `table` with this widget family's own `gHudPartVtable` in
 * place of whatever `InitUiSpriteObj` set it to. */
struct hud_digit_part *InitHudPart(struct hud_digit_part *part)
{
    InitUiSpriteObj((struct actor *)part);
    part->table = (void *)gHudPartVtable;
    return part;
}
