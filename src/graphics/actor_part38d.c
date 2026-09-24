#include "core.h"

/* Continuation of actor_part28c.c (issue #18's chunk, the last one) -
 * covers `nullsub_17` through `sub_80157C4` (all matched); non-adjacent
 * to actor_part28c.c since the parked `sub_80156EC` sits raw between
 * them (asm/code_3_2_17_156ec.s). Same "self" object family documented
 * at the top of actor_part18.c/actor_part28.c. */

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
 * `sub_800B86C(arg0, arg1, mode)`.
 *
 * The control flow below is written as explicit `goto`s matching the
 * ROM's own block layout exactly (one label per ROM branch target, no
 * `if`/`else` restructuring at all): an `if`/`else if` chain testing
 * `mode==0xd || mode==0x18` compiles to a different (non-matching)
 * decision tree than the ROM's own `cmp #0x12/beq`, `cmp #0x12/bgt`,
 * `cmp #0xd/beq`, (fallthrough) `cmp #0x18/beq` triangle - this
 * project's usual "translate the disassembly's control flow directly,
 * don't re-infer it as structured C" convention applies to branch
 * *shape* just as much as to instruction *choice*.
 *
 * The tail call is declared to return `s32` (reinterpreting
 * `sub_800B86C`'s real `u8` return through a function-pointer cast)
 * purely so the value is considered live in `r0` at the return point:
 * a genuinely `void` tail call leaves `r0` free, and gcc then reuses
 * it as the epilogue's `pop`/`bx` scratch register, where the ROM uses
 * `r1`. Returning the call's result normally (matching its real `u8`
 * type) also frees `r0` for `r1`, but pulls in a spurious zero-
 * extension pair (`lsl`/`lsr #0x18`) the ROM doesn't have, since gcc
 * always widens a `char`-returning call's result before propagating
 * it further; the raw-`s32` reinterpretation sidesteps that widening
 * entirely since the value is never treated as narrower than a full
 * register. See docs/matching/naked-sub_80157c4-matched.md for the
 * full derivation (this was the sole remaining residual after a
 * 99.8%-matching pass). */
s32 sub_80157C4(void *arg0, void *other, s32 mode)
{
    void *player = gUnknown_030012D8;

    if (*(u8 *)((u8 *)player + 0x100) == 0) {
        goto tail;
    }
    if (mode == 0x12) {
        goto case12;
    }
    if (mode > 0x12) {
        goto checkC18;
    }
    if (mode == 0xd) {
        goto setC26;
    }
    goto rearm;

checkC18:
    if (mode == 0x18) {
        goto setC26;
    }
    goto rearm;

case12:
    if (*(s32 *)((u8 *)player + 0x60) == 0) {
        goto tail;
    }
    mode = 0x25;
    goto playCue;

setC26:
    mode = 0x26;
playCue:
    sub_80019A8(gUnknown_030012BC, 0x36);
    PlaySfx(gUnknown_030012BC, 0x36, 0x100);
    goto tail;

rearm:
    sub_80019A8(gUnknown_030012BC, 0x36);

tail:
    return ((s32 (*)(void *, void *, s32))sub_800B86C)(arg0, other, mode);
}
