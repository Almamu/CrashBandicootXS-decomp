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

#if NON_MATCHING
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
 * PARKED, NOT BYTE-MATCHING: every field offset, mask and call is
 * confirmed correct against the ROM (cross-checked against `+0xac`,
 * `+0x88`, `+0x8c`, `+0x28` bit 4, and the `+0xb0` child-pointer
 * convention independently confirmed by `sub_800A810`/`sub_800AFF4`
 * below and in the same neighborhood). The ROM builds several of the
 * field addresses as a single running pointer incremented by small
 * relative offsets (`adds r0, #0x6c`, then `#0x1c`, then `subs r0,
 * #0x2c`, then `#8`) reusing one register across a long stretch of
 * unrelated-looking field writes, while this compiler always
 * recomputes each address fresh from `self` (or partially reuses an
 * adjacent computation via CSE in a different combination) - no source
 * restructuring tried (grouping writes by nearby offset, an explicit
 * running `u8 *cursor` incremented by hand) reproduced the ROM's exact
 * increment sequence. Parked rather than chase the address-recompute
 * pattern field-by-field. See docs/matching/issue-9-0x08007634-actor.md. */
void sub_800A734(void *selfArg)
{
    u8 *self = selfArg;
    u8 flags = self[0xc];
    void *child;
    s32 ret;
    u8 nibble;

    flags = (flags | 0x80 | 0x40 | 2) & ~4;
    self[0xc] = flags;
    self[0xd] |= 1;

    *(s32 *)(self + 0x60) = 0;
    *(s32 *)(self + 0x64) = 0;
    *(s32 *)(self + 0x48) = 0;
    *(s32 *)(self + 0x4c) = 0;
    *(s32 *)(self + 0x50) = 0;
    *(s32 *)(self + 0x54) = 0;
    *(s32 *)(self + 0x58) = 0;
    *(s32 *)(self + 0x5c) = 0;

    self[0x28] &= ~0x10;
    self[0x68] = 8;
    self[0x24] = 0;
    *(s32 *)(self + 0x44) = 0;
    *(s32 *)(self + 0x78) = 0;
    *(s32 *)(self + 0x1c) = 0;
    self[0x90] = 0;
    *(s32 *)(self + 0xac) = 0;
    self[0x80] = 0;
    self[0x88] = 0;
    *(s32 *)(self + 0x8c) = gUnknown_0300082C;

    self[0x91] = 0;
    self[0x94] = 0;
    self[0x92] = 0;
    self[0xa] = 1;

    child = *(void **)(self + 0xb0);
    ret = sub_800815C(child);
    nibble = *((u8 *)child + 0x29);
    *((u8 *)child + 0x29) = (nibble & ~0xF) | (ret & 0xF);

    self[0x100] = 0;
    self[0x101] = 0;
    self[0x102] = 0;
    self[0x103] = 0;
    self[0x104] = 0;
    self[0xc] &= ~1;
    self[0x105] = 0;
}
#endif /* NON_MATCHING */
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
