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

/* Resets `part`'s velocity/state fields (`+0x60`/`+0x64` cleared,
 * `+0x68` state set to 8, `+0x24` cleared, `+0x28` bit 5 cleared,
 * `+0x2d` cleared, `+0xc` bit 3 cleared/bit 6 set), then dispatches on
 * a sub-state byte at `+0x88` to one of three per-state teardown
 * helpers (each called with the same single `+0x44` "record" argument
 * `sub_800A730`/`sub_8009F1C` already established): state 0 ->
 * `sub_8015840`, state 1 -> `sub_80159A4`, state 3 -> `sub_8017994`;
 * state 2 and anything else is a no-op.
 *
 * PARKED, NOT BYTE-MATCHING: the field writes and dispatch semantics
 * are confirmed correct one-for-one against the ROM. The remaining gap
 * is register allocation around the dispatch itself: the ROM keeps
 * `self` in r3 across the three `bl` calls (safe since nothing between
 * dispatch and each call clobbers it) and reaches the three targets via
 * a `beq`/`bgt`/`ble`/`beq` comparison chain that groups state 1 first,
 * then state>1 (splitting 2 from 3), then state 0 last; every C
 * phrasing tried here (a plain `switch`, an explicit `if`/`else if`
 * chain ordered to match, with an empty `case 2`/`else if (state==2)`
 * arm to force the redundant compare) reproduces the *dispatch value*
 * correctly but not this exact compare/branch ordering or `self`'s
 * register letter at the same time - fixing one via restructuring moved
 * the other. Parked rather than keep chasing the exact chain shape. See
 * docs/matching/issue-9-0x08007634-actor.md. */
void sub_800A810(void *selfArg)
{
    u8 *self = selfArg;
    u8 state;

    *(s32 *)(self + 0x60) = 0;
    *(s32 *)(self + 0x64) = 0;
    self[0x68] = 8;
    self[0x24] = 0;
    self[0x28] &= ~0x20;
    self[0x2d] = 0;
    self[0xc] = (self[0xc] & ~8) | 0x40;

    state = self[0x88];
    switch (state) {
    case 0:
        sub_8015840(*(s32 *)(self + 0x44));
        break;
    case 1:
        sub_80159A4(*(s32 *)(self + 0x44));
        break;
    case 3:
        sub_8017994(*(s32 *)(self + 0x44));
        break;
    default:
        break;
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
