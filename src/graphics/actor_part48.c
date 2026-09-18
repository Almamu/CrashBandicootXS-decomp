#include "core.h"

/* GitHub issue #9: 0x08007634-0x0800B3F0, game_loop-labeled chunk that
 * turned out to be part of the `actor` category's "part" object family
 * (see docs/matching/issue-9-0x08007634-actor.md). `sub_800A734`/
 * `sub_800A810` sit right after already-matched actor_part14.c
 * (`sub_800A5F4`-`sub_800A730`). */

extern s32 sub_800815C(void *part);
extern void sub_8015840(s32 arg0);
extern void sub_80159A4(s32 arg0);
extern void sub_8017994(s32 arg0);
extern s32 gUnknown_0300082C;

/* `part`-object constructor/reset: clears the velocity/accel fields
 * `sub_800A590`/`sub_8009DF4` consume, resets state (`+0x68`) to 8,
 * snapshots the current frame counter (`gUnknown_0300082C`) into
 * `+0x8c` (the same "periodic check" field documented elsewhere in
 * this ROM), zeroes the `+0x100`-`+0x105` per-phase flag bytes
 * `actor_part15.c`'s doc comment already describes, and hooks up a
 * child/"owner" object at `+0xb0`: calls `sub_800815C(child)` and packs
 * its low nibble into `child+0x29`'s own low nibble (preserving the
 * high nibble) - the same field `sub_800A734`'s sibling constructors in
 * actor_part14.c already touch at a different bit.
 *
 * Matched after a second pass closed the gap an earlier session parked
 * on (see docs/matching/issue-9-0x08007634-actor.md's original entry
 * for that history). The ROM builds most of these field addresses via
 * two running-pointer cursors (`p1`/`p0` here, matching its own r1/r0)
 * incremented/decremented by the literal relative offset between each
 * field, rather than recomputing `self + N` fresh each time - the same
 * cursor idiom `sub_800A810` right below already needed for its own,
 * shorter `self+0x68`-to-`self+0x28` chain. Getting the remaining gap
 * closed needed a cluster of further techniques, all documented inline
 * at their exact spot below: separate statements (not one folded
 * expression) for `self+0xc`'s three ORs, since the ROM never combines
 * those constants at compile time; an inline-asm-anchored first OR so
 * the mask constant loads into r0 *before* the byte load, matching the
 * ROM's own instruction order; the negative-constant register-pinned
 * mask idiom for every `& ~N` clear; forcing a pointer to actually
 * materialize in a register (`self+0x8c`'s write) where this compiler
 * would otherwise fold a `+= N` straight into the next store's
 * addressing mode; keeping `self+0xb0`'s *address* (not the loaded
 * child pointer) live across the `bl sub_800815C` and reloading the
 * pointer fresh afterward, since the ROM does the same rather than
 * spend an extra register caching it across the call; and, for the
 * trailing `self+0x100`-`self+0x105` writes, three independent
 * one-shot/reused offset registers (not one shared cursor) with
 * deliberately narrow scopes so this compiler's allocator doesn't pick
 * a different (but equally "free") register than the ROM's own choice.
 * See docs/matching/issue-9-0x08007634-actor.md for the full write-up. */
void sub_800A734(void *selfArg)
{
    u8 *self = selfArg;
    u8 flags;
    s32 ret;
    u8 *p1, *p0;

    /* Separate statements, not one folded expression: the ROM performs
     * each OR as its own instruction against the freshly-loaded byte
     * rather than combining the constants at compile time. Register-
     * pinned for the first OR: the ROM loads the mask constant into r0
     * *before* the byte load (`movs r0,#0x80; ldrb r1,[r6,#12]`), while
     * a plain `flags |= 0x80` (flags already loaded into a register)
     * puts the byte load first instead. */
    {
        register u8 result asm("r0");

        asm volatile(
            "mov r0, #0x80\n\t"
            "ldrb r1, [%1, #0xc]\n\t"
            "orr r0, r0, r1\n\t"
            : "=r"(result) : "l"(self) : "r1"
        );
        result |= 0x40;
        result |= 2;
        {
            /* Register-pinned negative-constant idiom: the ROM
             * computes ~4 at runtime (`movs r1,#5; rsbs r1,r1,#0`) via
             * the `-N == ~(N-1)` identity instead of folding the mask
             * at compile time - see matching_decomp_register_pinning
             * memory. Kept in the same `result` (r0)-anchored block as
             * the ORs above it so this compiler doesn't re-derive the
             * byte's live range through a plain `u8 flags` round-trip
             * and reinsert a redundant 32-bit mask/shift pair. */
            register s32 clearMask asm("r1") = -5;
            result &= clearMask;
        }
        flags = result;
    }
    self[0xc] = flags;

    {
        register u32 m asm("r0") = 1;
        register u8 b asm("r2") = self[0xd];
        self[0xd] = m | b;
    }

    *(s32 *)(self + 0x60) = 0;
    *(s32 *)(self + 0x64) = 0;
    *(s32 *)(self + 0x48) = 0;
    *(s32 *)(self + 0x4c) = 0;
    *(s32 *)(self + 0x50) = 0;
    *(s32 *)(self + 0x54) = 0;
    *(s32 *)(self + 0x58) = 0;
    *(s32 *)(self + 0x5c) = 0;

    /* The ROM builds these field addresses as two running-pointer
     * cursors (r1/r0) incremented/decremented by the literal relative
     * offset between each field, rather than recomputing `self + N`
     * fresh each time - the same idiom `sub_800A810` needed a single
     * `u8 *p` cursor for, just with two cursors interleaved here. */
    {
        register u8 *pinnedP1 asm("r1") = self + 0x28;
        register s32 clearMask asm("r0") = -0x11;
        register u8 byte asm("r3") = *pinnedP1;
        register s32 result asm("r0");

        result = clearMask & byte;
        *pinnedP1 = result;
        p1 = pinnedP1;
    }
    p1 += 0x40; /* self+0x68 */
    *p1 = 8;

    p0 = self + 0x24;
    *p0 = 0;
    *(s32 *)(self + 0x44) = 0;
    *(s32 *)(self + 0x78) = 0;
    *(s32 *)(self + 0x1c) = 0;
    p0 += 0x6c; /* self+0x90 */
    *p0 = 0;
    p0 += 0x1c; /* self+0xac */
    *(s32 *)p0 = 0;
    p0 -= 0x2c; /* self+0x80 */
    *p0 = 0;
    p0 += 8; /* self+0x88 */
    *p0 = 0;

    {
        /* Register-pinned: without forcing p1 to materialize in a
         * register, this compiler folds the `+0x24` straight into the
         * store's addressing mode (`str r0, [r1, #0x24]`) instead of
         * reproducing the ROM's separate `adds r1, #0x24` pointer
         * update followed by an offset-0 store. */
        register u8 *pinnedP1 asm("r1") = p1;
        asm volatile("add %0, %0, #0x24" : "+l"(pinnedP1)); /* self+0x8c */
        *(s32 *)pinnedP1 = gUnknown_0300082C;
        p1 = pinnedP1;
    }

    self[0x91] = 0;
    self[0x94] = 0;
    self[0x92] = 0;
    self[0xa] = 1;

    {
        /* Register-pinned: the ROM keeps the *address* self+0xb0 (not
         * the loaded child pointer) live across the `bl` in r5, then
         * reloads the child pointer fresh afterward - r0 gets
         * clobbered by the call's return value, so caching `child`
         * across the call (this compiler's natural choice) needs an
         * extra register/copy the ROM doesn't have. */
        register void **childAddr asm("r5") = (void **)(self + 0xb0);

        ret = sub_800815C(*childAddr);

        {
            /* Register-pinned to mirror the ROM's exact instruction
             * order: `ret & 0xf` computed first (r0/r1), then the
             * negative-constant `~0xf` mask (r1, re-used) ANDed
             * against the freshly reloaded field byte (r3), before
             * the two halves are OR'd together into r1 and stored. */
            register u8 *fieldAddr asm("r2") = (u8 *)*childAddr + 0x29;
            register u32 retMasked asm("r0") = ret & 0xf;
            register s32 notMask asm("r1");

            /* Forced via inline asm: r1 already holds the just-used
             * `0xf` mask here, and a plain `-0x10` constant lets this
             * compiler derive it as `r1 - 0x1f` (algebraically valid,
             * since 0xf - 0x1f == -0x10) instead of the ROM's fresh
             * `movs r1, #0x10; negs r1, r1` pair. */
            asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=l"(notMask));
            {
                register u8 byte asm("r3") = *fieldAddr;
                register u32 result asm("r1");

                result = notMask & byte;
                result |= retMasked;
                *fieldAddr = result;
            }
        }
    }

    /* Three independent running offsets (r1/r2/r3 in the ROM), not one
     * shared cursor: 0x100 (r1, built via `movs #0x80; lsls #1`) and
     * 0x102 (r3, `movs #0x81; lsls #1`) are one-shot, while 0x101 (r2,
     * a literal-pool load - it doesn't fit an 8-bit `movs` immediate)
     * and 0x100 each get reused a second time (+3) for 0x103/0x104,
     * and r1 a third time (+2 more) for 0x105. */
    {
        register s32 off1 asm("r1");
        register s32 off2 asm("r2");
        u8 *p;

        /* Each offset is initialized right where the ROM first
         * computes it, not all three up front - otherwise this
         * compiler's scheduler hoists the (data-independent) r2
         * literal-pool load ahead of r1's `movs`/`lsls` pair. */
        off1 = 0x80;
        off1 = off1 << 1; /* 0x100 */
        p = self + off1;
        *p = 0;

        off2 = 0x101; /* literal-pool load - doesn't fit an 8-bit movs */
        p = self + off2;
        *p = 0;

        {
            /* off3 (r3) gets its own narrow scope, ending right after
             * its one use - a pinned register variable claims its
             * register for its whole *lexical* scope even past its
             * last real use (see the "Real gotchas" note in
             * docs/matching/issue-9-0x08007634-actor.md), so keeping
             * it declared alongside off1/off2 would leave r3
             * unavailable for the clearMask block below, same as the
             * ROM's own re-use of it there. */
            register s32 off3 asm("r3") = 0x81;
            off3 = off3 << 1; /* 0x102 */
            p = self + off3;
            *p = 0;
        }

        off1 += 3; /* 0x103 */
        p = self + off1;
        *p = 0;

        off2 += 3; /* 0x104 */
        p = self + off2;
        *p = 0;

        {
            /* Forced via inline asm: with off1/off2 both free by this
             * point, this compiler's allocator always picks r2 (the
             * lowest free register) for the freshly-loaded byte here,
             * never r3 like the ROM, no matter how the surrounding
             * scopes are narrowed. */
            register u8 result asm("r0");
            asm volatile(
                "mov r0, #0x2\n\t"
                "neg r0, r0\n\t"
                "ldrb r3, [%1, #0xc]\n\t"
                "and r0, r0, r3\n\t"
                : "=r"(result) : "l"(self) : "r3"
            );
            self[0xc] = result;
        }

        off1 += 2; /* 0x105 */
        p = self + off1;
        *p = 0;
    }
}
asm(".align 2, 0");

/* Resets `part`'s velocity/state fields (`+0x60`/`+0x64` cleared,
 * `+0x68` state set to 8, `+0x24` cleared, `+0x28` bit 5 cleared,
 * `+0x2d` cleared, `+0xc` bit 3 cleared/bit 6 set), then dispatches on
 * a sub-state byte at `+0x88` to one of three per-state teardown
 * helpers (each called with the same single `+0x44` "record" argument
 * `sub_800A730`/`sub_8009F1C` already established): state 0 ->
 * `sub_8015840`, state 1 -> `sub_80159A4`, state 3 -> `sub_8017994`;
 * state 2 and anything else is a no-op.
 *
 * Needed `self` pinned to `r3` (kept live across the three `bl` calls),
 * the two AND-mask field writes (`self+0x28`, `self+0xc`) built via the
 * "negative-constant register-pinned mask" idiom (`register s32 mask
 * asm("r0") = -0x21`/`-9`, matching the ROM's own runtime `mov`+`neg`
 * pair instead of a folded 8-bit AND immediate), the `self+0x68`-to-
 * `self+0x28` field-address chain expressed as a single decremented
 * `u8 *p` cursor (reproducing the ROM's own `subs r1, #0x40` reuse
 * instead of a fresh address computation), and the whole dispatch
 * rewritten as an explicit `goto` chain - `if (state == 1) goto do1;
 * if (state > 1) goto gt1; if (state == 0) goto do0;` - with a second
 * register-pinned copy of `state` (`state2`, `r1`) read once up front
 * for the inner `state2 == 2`/`state2 == 3` checks, matching the ROM's
 * own `adds r1, r0, #0` copy and its `beq`/`bgt`/`beq` comparison chain
 * (an `if`/`else if` chain, even restructured with an empty `case 2`
 * arm, always collapsed the branch polarity to `bne`-skip instead of
 * this `beq`-take shape and let the CSE pass drop the `state`/`state2`
 * copy entirely). See docs/matching/issue-9-0x08007634-actor.md. */
void sub_800A810(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;
    s32 state;

    {
        register s32 zero asm("r2") = 0;

        *(s32 *)(self + 0x60) = zero;
        *(s32 *)(self + 0x64) = zero;
        {
            u8 *p = self + 0x68;
            *p = 8;
            self[0x24] = zero;
            p -= 0x40;
            {
                register s32 mask asm("r0") = -0x21;
                register u8 byte asm("r4") = *p;
                register s32 result asm("r0");

                result = mask & byte;
                *p = result;
            }
        }
        self[0x2d] = zero;
    }
    {
        register s32 mask asm("r0") = -9;
        register s32 byte asm("r1") = self[0xc];
        register s32 result asm("r0");
        register s32 orMask asm("r1");

        result = mask & byte;
        orMask = 0x40;
        result = result | orMask;
        self[0xc] = result;
    }

    state = self[0x88];
    {
        register s32 state2 asm("r1") = state;

        if (state == 1) goto do1;
        if (state > 1) goto gt1;
        if (state == 0) goto do0;
        goto endDispatch;
    gt1:
        if (state2 == 2) goto endDispatch;
        if (state2 == 3) goto do3;
        goto endDispatch;
    do0:
        sub_8015840(*(s32 *)(self + 0x44));
        goto endDispatch;
    do1:
        sub_80159A4(*(s32 *)(self + 0x44));
        goto endDispatch;
    do3:
        sub_8017994(*(s32 *)(self + 0x44));
    endDispatch:
        ;
    }
}
asm(".align 2, 0");
