#include "core.h"

/* Continuation of actor_part28c.c (issue #18's chunk, the last one) -
 * covers `nullsub_17` through `sub_8015780` (matched) and `sub_80157C4`
 * (parked, NON_MATCHING); non-adjacent to actor_part28c.c since the
 * parked `sub_80156EC` sits raw between them (asm/code_3_2_17_156ec.s).
 * Same "self" object family documented at the top of actor_part18.c/
 * actor_part28.c. */

extern void sub_8015460(void *selfArg);
extern void sub_8012FBC(void *self);
extern void sub_8012D24(void *self);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);

void nullsub_17(void)
{
}

/* While `self+0x29` is clear: tail-calls `sub_8015460` first. Always
 * tail-calls `sub_8012FBC` afterward. */
void sub_8015750(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;

    if (self[0x29] == 0) {
        sub_8015460(self);
    }
    sub_8012FBC(self);
}

void nullsub_18(void)
{
}

/* Trivial tail-call. */
void sub_8015774(void *selfArg)
{
    sub_8012D24(selfArg);
}

/* Fires the mgr trampoline pair with `a`/`b` as the two action
 * arguments, then conditionally latches `self+0x18`/`self+0x1c` from
 * `c`/`d` unless either is the `0x7FFFFFFF` sentinel. */
void sub_8015780(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    u8 *mgr = *(u8 **)(self + 0xc);
    u8 *off;

    sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)a, *(void **)(mgr + 0x24));
    off = *(u8 **)(self + 0xc) + 0x50;
    sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)b, *(void **)(off + 4));

    if (c != 0x7FFFFFFF) {
        *(s32 *)(self + 0x18) = c;
    }
    if (d != 0x7FFFFFFF) {
        *(s32 *)(self + 0x1c) = d;
    }
}

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-18-0x08014f8c-actor.md,
 * "Parked, not matched: sub_80157C4" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_17_157c4.s) is used otherwise. Every load/store, branch
 * and call is understood and semantically correct - the residual gap
 * is register allocation across the 3-way `mode` dispatch (this
 * compiler wants `r8` for the running "unused" register the ROM keeps
 * `mode` copies in instead, and merges the `0xd`/`0x18` case pair into
 * two sequential compares sharing a target rather than the ROM's own
 * `cmp/bgt/cmp/beq` triangle) - tried the `switch`-based anti-
 * canonicalization technique that fixed `sub_8015238`'s range check
 * (this dispatch isn't a contiguous range, so it didn't apply the same
 * way) and various `register asm("rN")` pins; none converged within
 * the effort budget for this pass. */
extern void *gUnknown_030012BC;
extern void *gUnknown_030012D8;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern void sub_80019A8(void *self, u32 id);
extern u8 sub_800B86C(void *unused, void *partArg, s32 newVal);

/* If the player's `+0x100` flag is set: picks a replacement `mode` for
 * a handful of special values (`0x12` when the player's `+0x60` is
 * nonzero -> `0x25`; `0xd`/`0x18` -> `0x26`, both playing a fixed cue
 * via `sub_80019A8`/`PlaySfx`) and otherwise just re-arms the cue via
 * `sub_80019A8` with the original `mode`. Always tail-calls
 * `sub_800B86C(arg0, arg1, mode)`. */
void sub_80157C4(void *arg0, void *arg1, s32 mode)
{
    u8 *player = gUnknown_030012D8;

    if (player[0x100] != 0) {
        if (mode == 0x12) {
            if (*(s32 *)(player + 0x60) != 0) {
                mode = 0x25;
                sub_80019A8(gUnknown_030012BC, 0x36);
                PlaySfx(gUnknown_030012BC, 0x36, 0x100);
            }
        } else if (mode == 0xd || mode == 0x18) {
            mode = 0x26;
            sub_80019A8(gUnknown_030012BC, 0x36);
            PlaySfx(gUnknown_030012BC, 0x36, 0x100);
        } else {
            sub_80019A8(gUnknown_030012BC, 0x36);
        }
    }

    sub_800B86C(arg0, arg1, mode);
}
#endif /* NON_MATCHING */
