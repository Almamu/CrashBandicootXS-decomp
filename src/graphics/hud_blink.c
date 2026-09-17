#include "core.h"

/* A small 3-slot icon "blink" animation timer, shared with `sub_8028568`/
 * `sub_802856C` (src/system/game_loop.c, still raw asm here) via the
 * `gUnknown_03001318` instance - each slot is a `{state, timer}` s32
 * pair: state 0 idle, 1 counting up to a threshold then -> 2, 2 counting
 * down 0x14 frames then -> 3, 3 counting down its own timer then back to
 * 0. Kept as raw offsets (matching the existing `void *state` convention
 * already used for `gUnknown_03001318` in src/system/game_loop.c/
 * game_loop2.c) rather than a named struct, since the object extends
 * past this file's own fields (at least to +0x28, per `sub_8028568`). */

extern void *gUnknown_030012C0;

extern void sub_8028520(void *self, s32 *state, s32 *timer, s32 threshold);

/* Per-frame tick, gated on the central state object's `+0x8c` flag:
 * force-advances slots 0 and 1 out of a stuck 1/2 state (state 1 -> 3
 * directly; state 2 -> 3, refreshing its timer to 0x14 first), then
 * runs the generic `sub_8028520` advance on all three slots
 * unconditionally. Slot 2 (self+0x10/+0x14) doesn't get the manual
 * force-advance the other two do - reproduced as-is. */
void sub_8028400(void *state)
{
    /* Pointer arithmetic is kept inline (not cached into locals) at each
     * use site, matching the ROM's own redundant recomputation - a
     * plain cached-pointer version pulls the four field addresses into
     * extra callee-saved registers up front (see docs/workflow.md
     * step 7). Each of the two "force out of state 1/2" checks below
     * also needs the same explicit `goto`-forced extra `<=`
     * range-check branch as `sub_8028520`'s dispatch (see its own
     * comment) to reproduce the ROM's exact compare chain - a plain
     * `if (v==1) {...} else if (v==2) {...}` collapses that redundant
     * middle branch away. */
    if (*(u8 *)((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
        s32 v;

        v = *(s32 *)state;
        if (v == 1) {
            goto set0;
        }
        if (v <= 1) {
            goto skip0;
        }
        if (v != 2) {
            goto skip0;
        }
        *(s32 *)((u8 *)state + 4) = 0x14;
    set0:
        *(s32 *)state = 3;
    skip0:

        v = *(s32 *)((u8 *)state + 8);
        if (v == 1) {
            goto set1;
        }
        if (v <= 1) {
            goto skip1;
        }
        if (v != 2) {
            goto skip1;
        }
        *(s32 *)((u8 *)state + 0xc) = 0x14;
    set1:
        *(s32 *)((u8 *)state + 8) = 3;
    skip1:
        ;
    }

    sub_8028520(state, (s32 *)((u8 *)state + 0x10), (s32 *)((u8 *)state + 0x14), 0x78);
    sub_8028520(state, (s32 *)state, (s32 *)((u8 *)state + 4), 0x78);
    sub_8028520(state, (s32 *)((u8 *)state + 8), (s32 *)((u8 *)state + 0xc), 0x78);
}

/* Trigger for slot 2 (self+0x10/+0x14): only runs while the central
 * state object's `+0x8c` flag is clear. Idle (0) or finished (3) starts
 * a fresh blink (state -> 1); already counting down the "on" phase (2)
 * instead just refreshes its timer back to the full 0x78-frame hold.
 * Already counting up (1) is left alone. */
void sub_8028474(void *state)
{
    s32 slotState;

    if (*(u8 *)((u8 *)gUnknown_030012C0 + 0x8c) == 0) {
        slotState = *(s32 *)((u8 *)state + 0x10);

        if (slotState == 0 || slotState == 3) {
            *(s32 *)((u8 *)state + 0x10) = 1;
        } else if (slotState == 2) {
            *(s32 *)((u8 *)state + 0x14) = 0x78;
        }
    }
}

/* Same trigger as `sub_8028474`, for slot 0 (self+0/+4). */
void sub_80284A4(void *state)
{
    s32 slotState;

    if (*(u8 *)((u8 *)gUnknown_030012C0 + 0x8c) == 0) {
        slotState = *(s32 *)state;

        if (slotState == 0 || slotState == 3) {
            *(s32 *)state = 1;
        } else if (slotState == 2) {
            *(s32 *)((u8 *)state + 4) = 0x78;
        }
    }
}

/* Same trigger as `sub_8028474`, for slot 1 (self+8/+0xc). */
void sub_80284D4(void *state)
{
    s32 slotState;

    if (*(u8 *)((u8 *)gUnknown_030012C0 + 0x8c) == 0) {
        slotState = *(s32 *)((u8 *)state + 8);

        if (slotState == 0 || slotState == 3) {
            *(s32 *)((u8 *)state + 8) = 1;
        } else if (slotState == 2) {
            *(s32 *)((u8 *)state + 0xc) = 0x78;
        }
    }
}

/* Fires all three slots' triggers at once. */
void sub_8028504(void *state)
{
    sub_8028474(state);
    sub_80284A4(state);
    sub_80284D4(state);
}

/* Generic single-slot blink advance, called once per slot by
 * `sub_8028400` above. `self` is unused - forwarded through purely for
 * calling-convention parity with the trigger functions above.
 *
 * The explicit (never-reached, since callers only ever pass 0-3 and
 * state 0 is otherwise idle/inert) `case 0` below is load-bearing, not
 * dead code: without it this compiler lowers the switch as a balanced
 * comparison tree pivoting on the middle case value (2) instead of the
 * ROM's plain ascending compare chain (1, then an early-out for
 * anything <= 1, then 2, then 3) - see docs/workflow.md step 7. */
void sub_8028520(void *self, s32 *state, s32 *timer, s32 threshold)
{
    switch (*state) {
    case 1:
        *timer += 1;
        if (*timer > 0x13) {
            *state = 2;
            *timer = threshold;
        }
        break;
    case 0:
        break;
    case 3:
        *timer -= 1;
        if (*timer == 0) {
            *state = *timer;
        }
        break;
    case 2:
        *timer -= 1;
        if (*timer == 0) {
            *state = 3;
            *timer = 0x14;
        }
        break;
    }
}
