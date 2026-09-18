#include "core.h"

/* GAX2_SoundHandler "Channel" type's init_fn (ROM 0x08039519, see
 * docs/audio.md's per-type function-pointer table and
 * gax_sound_handler_channel.c's neighboring unknown_fn/play_fn note).
 * Resets the same kind of field-set sub_803A104's per-channel voice
 * constructor does (accumulator/envelope/priority/portamento defaults),
 * then computes a fixed-point reciprocal (`sub_8037A7C((s64)1 << 32,
 * (s64)self->field_4's 0x2 halfword)`, a generic 64-bit software
 * division helper - not GAX2-specific, see docs/audio.md) into the
 * 8-byte global `gUnknown_03001618`, and finally loops `self->8`'s
 * child-pointer array (count `self->0->0xc`) firing each child's own
 * function pointer through the `sub_803AD7C` trampoline - the same
 * "run an init/reset callback on every child" pattern
 * `sub_803A22C` (UnknownC type's init_fn, gax_sound_handler_unknownc.c)
 * uses. The object shape isn't confidently modeled yet (same situation
 * as the other GAX2_SoundHandler functions in this cluster) - kept as
 * raw offsets throughout.
 *
 * Written as NAKED asm, not plain C: a real C reconstruction (kept
 * in-tree in an earlier draft of this file, now discarded once this
 * NAKED transcription verified byte-exact) got every field-reset store
 * byte-identical on its own, including reproducing the ROM's otherwise
 * unreproducible `ldr rX,=0`/`ldr rX,=1` literal-pool loads for the
 * division call's first two arguments (by passing them as one real
 * `(s64)1 << 32` C constant rather than two separate int arguments -
 * previously undocumented as a solution, see
 * docs/matching/issue-67-0x08038538-audio.md's original note on this
 * call site) - but two separate compiler limitations kept the division
 * call and its surrounding literal pool from matching:
 * 1. `sub_8037A7C`'s second argument (the halfword field read through a
 *    pointer chain) has to sit in a register pinned to `r2` to match
 *    the ROM's register choreography around the call, but gcc 2.9's
 *    explicit-register-variable support always emits one extra
 *    defensive copy (`adds r6, r2, #0`) between computing a pinned
 *    register's value and consuming it at a call site, even when both
 *    already agree on the same physical register - confirmed by trying
 *    every combination of pinned/unpinned intermediates for both the
 *    temporary field-pointer and the halfword result.
 * 2. The ROM groups all 4 of this function's literal-pool words
 *    (0x8AD0, `gUnknown_03001618`'s address, and the division call's 0
 *    and 1) into one pool sitting right after the `b` that skips over
 *    it. A plain-C reconstruction naturally produces exactly this
 *    grouping (gcc pools every compiler-visible constant used by one
 *    function at the same flush point) - but as soon as any instruction
 *    in that stretch has to become a hand-placed inline-asm anchor (per
 *    point 1), `ldr rX,=value`'s automatic pool placement inside that
 *    anchor text no longer shares gcc's own flush point, splitting the
 *    4 words across two separate pools and shifting every following
 *    instruction's address away from the ROM's. Merging them back
 *    requires manually hand-placing the whole stretch from the first
 *    pool-pinned constant onward as one block - at which point it's a
 *    mechanical, byte-verified transcription of the ROM's own
 *    instructions (translated from the disassembler's unified syntax to
 *    this project's established NAKED plain/divided syntax, local
 *    labels renumbered per
 *    docs/matching/issue-4-sio-settings-sync.md's convention), not an
 *    inferred control-flow guess - every field offset and the loop
 *    bound below were independently understood first, the same as the
 *    project's other NAKED transcriptions (timer_util.c's
 *    `sub_803AA08`, audio_context.c's `sub_80019F8`). */
NAKED void sub_8039518(void *self)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "add r5, r0, #0\n\t"
        "mov r2, #0\n\t"
        "str r2, [r5, #0x44]\n\t"
        "strb r2, [r5, #0x10]\n\t"
        "str r2, [r5, #0x3c]\n\t"
        "mov r1, #0\n\t"
        "ldr r0, 1f\n\t"
        "strh r0, [r5, #0x2a]\n\t"
        "mov r3, #1\n\t"
        "strb r3, [r5, #0x11]\n\t"
        "mov r0, #0xff\n\t"
        "strb r0, [r5, #0x15]\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "strb r0, [r5, #0x18]\n\t"
        "strb r1, [r5, #0xc]\n\t"
        "strb r1, [r5, #0x12]\n\t"
        "strb r1, [r5, #0xd]\n\t"
        "strb r1, [r5, #0xf]\n\t"
        "strb r1, [r5, #0xe]\n\t"
        "add r0, r5, #0\n\t"
        "add r0, r0, #0x24\n\t"
        "strb r1, [r0]\n\t"
        "add r0, r0, #1\n\t"
        "strb r1, [r0]\n\t"
        "strh r2, [r5, #0x34]\n\t"
        "strh r2, [r5, #0x32]\n\t"
        "strh r2, [r5, #0x30]\n\t"
        "add r0, r0, #0x2d\n\t"
        "strb r3, [r0]\n\t"
        "ldr r4, 2f\n\t"
        "ldr r0, [r5, #4]\n\t"
        "ldrh r2, [r0, #2]\n\t"
        "mov r3, #0\n\t"
        "ldr r0, 3f\n\t"
        "ldr r1, 4f\n\t"
        "bl sub_8037A7C\n\t"
        "str r0, [r4]\n\t"
        "str r1, [r4, #4]\n\t"
        "mov r4, #0\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "1: .4byte 0x00008AD0\n"
    "2: .4byte gUnknown_03001618\n"
    "3: .4byte 0x00000000\n"
    "4: .4byte 0x00000001\n"
    "5:\n\t"
        "ldr r1, [r5, #8]\n\t"
        "lsl r0, r4, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r1, [r1]\n\t"
        "bl sub_803AD7C\n\t"
        "add r4, r4, #1\n\t"
    "6:\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "cmp r4, r0\n\t"
        "blo 5b\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
