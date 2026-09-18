#include "core.h"
#include "audio.h"

extern struct GaxPlayerState *gUnknown_03001630;

/* This file's three functions (issue #67's `sub_8038C88`/`sub_8038DC0`/
 * `sub_8038E74` - docs/status/audio.md's "more GAX2 mixer-tick/voice-
 * stealing internals") walk the same nested, still only partly-modeled
 * object chain the rest of this GAX2_SoundHandler cluster does:
 * `gUnknown_03001630->channels[curChannelIdx]` (a "channel slot") holds
 * a pointer at `+0` to a "handler" object (itself holding, at `+0`
 * again, a THIRD object whose `+0xc` supplies a base array index, and
 * at its own `+8`/`+0x14` an array-of-pointers base/count pair) and a
 * pointer at `+4` to the per-voice state object `sub_803A104`
 * (`gax_channel_init.c`) initializes. None of these three nested
 * objects have confident names yet - kept as raw offsets throughout,
 * same as every other function in this cluster (`gax_channel_pool_
 * alloc.c`, `gax_channel_table_alloc.c`, etc).
 *
 * All three are NAKED transcriptions, not real C, for the reasons
 * explained on each function individually below - unlike
 * `src/util/math_div64_util.c`'s NAKED functions (a hard, confirmed
 * compiler-capability gap), these are the more ordinary "the C
 * reconstruction's *logic* is right but this compiler's register
 * allocation for a long chain of nested-pointer dereferences doesn't
 * land on the ROM's exact choice of scratch registers" gap already
 * documented throughout this project (`docs/matching.md`, `docs/status/
 * audio.md`'s many-register entries) - mechanical, byte-verified
 * transcriptions of the ROM's own instructions (translated from the
 * disassembler's unified syntax to this project's established NAKED
 * plain/divided syntax, local labels renumbered per docs/matching/
 * issue-4-sio-settings-sync.md's convention), not inferred control
 * flow. */

/* Per-frame mixer tick (docs/status/audio.md): once a song is loaded
 * and playing (`state != 0`), zero-fills a 0x40-byte scratch buffer
 * pointed to by `songPtr->+0x34` whenever that pointer is non-null,
 * clamps `songPtr->+0x10` to a byte-sized maximum (0xFF), mirrors that
 * clamped value into the current channel's voice object (`+0x1f`) and
 * a `songPtr->+0xa` halfword out to `gUnknown_03001630->+0x180`,
 * flags the current voice `+0x1a`, computes a `sub_803A5A8` argument
 * from `gUnknown_03001630->+0x18`/`+0x2c` and a handler sub-object
 * field, toggles `gUnknown_03001630->+0x2c` bit 0, copies the current
 * voice's `+0x21` byte into `songPtr->+0x39`, and - specifically when
 * `curChannelIdx == 1` and that just-copied byte is non-zero - resets
 * `curChannelIdx` to 0, records the old index at `songPtr->+0x3a`, and
 * (if `songPtr->+0x2c` is set) re-links every one of `songPtr->+0xe`
 * channel-row entries' `+8` voice-pointer slot to the (now-current)
 * channel 0's voice object.
 *
 * NAKED, not attempted as real C this pass: substantially larger and
 * more deeply-nested (the `p->channels[curChannelIdx]` chase alone
 * repeats 5 times, several through three further pointer hops) than
 * `sub_8038DC0` below, which itself already resisted an extensive real-
 * C attempt (see that function's own comment) on a much simpler shape -
 * pursuing this one was deprioritized in favor of documenting a
 * confident semantic understanding and landing a correct byte-verified
 * transcription instead. */
NAKED void sub_8038C88(void)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "ldr r6, 7f\n\t"
        "ldr r1, [r6]\n\t"
        "ldr r0, [r1, #0x30]\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "b 6f\n\t"
    "1:\n\t"
        "ldr r0, [r1, #4]\n\t"
        "ldr r0, [r0, #0x34]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "mov r1, #0x40\n\t"
        "bl sub_8037F3C\n\t"
    "2:\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "ldrh r0, [r1, #0x10]\n\t"
        "cmp r0, #0xff\n\t"
        "bls 3f\n\t"
        "mov r0, #0xff\n\t"
        "strh r0, [r1, #0x10]\n\t"
    "3:\n\t"
        "ldr r2, [r6]\n\t"
        "ldr r1, [r2, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "ldr r0, [r2, #4]\n\t"
        "ldrh r0, [r0, #0x10]\n\t"
        "strb r0, [r1, #0x1f]\n\t"
        "ldr r0, [r6]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #1\n\t"
        "add r2, r0, r1\n\t"
        "ldr r1, [r0, #4]\n\t"
        "ldrh r1, [r1, #0xa]\n\t"
        "str r1, [r2]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "mov r4, #1\n\t"
        "strb r4, [r0, #0x1a]\n\t"
        "ldr r3, [r6]\n\t"
        "ldr r1, [r3, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "ldrh r2, [r1, #4]\n\t"
        "ldr r1, [r3, #0x2c]\n\t"
        "mul r2, r1, r2\n\t"
        "ldr r1, [r3, #0x18]\n\t"
        "add r1, r1, r2\n\t"
        "bl sub_803A5A8\n\t"
        "ldr r1, [r6]\n\t"
        "ldr r0, [r1, #0x2c]\n\t"
        "eor r0, r4\n\t"
        "str r0, [r1, #0x2c]\n\t"
        "ldr r2, [r1, #4]\n\t"
        "ldr r0, [r1, #0x10]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, #8\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "add r0, #0x21\n\t"
        "ldrb r0, [r0]\n\t"
        "add r2, #0x39\n\t"
        "strb r0, [r2]\n\t"
        "ldr r1, [r6]\n\t"
        "ldr r3, [r1, #0x10]\n\t"
        "cmp r3, #1\n\t"
        "bne 5f\n\t"
        "ldr r2, [r1, #4]\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x39\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r1, #0x10]\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x3a\n\t"
        "strb r3, [r0]\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "ldr r0, [r1, #0x2c]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "mov r5, #0\n\t"
        "ldrh r1, [r1, #0xe]\n\t"
        "cmp r5, r1\n\t"
        "bge 5f\n\t"
    "4:\n\t"
        "ldr r1, [r6]\n\t"
        "ldr r0, [r1, #0x10]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r2, r1, #0\n\t"
        "add r2, #8\n\t"
        "add r2, r2, r0\n\t"
        "ldr r0, [r1, #4]\n\t"
        "ldr r0, [r0, #0x30]\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r2, [r2]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r2\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #8]\n\t"
        "ldr r0, [r2, #4]\n\t"
        "str r0, [r1]\n\t"
        "ldr r3, [r6]\n\t"
        "ldr r1, [r3, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r2, [r4]\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r1, [r0, #0xc]\n\t"
        "add r1, r1, r5\n\t"
        "ldr r0, [r2, #8]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r1, r0\n\t"
        "ldr r2, [r3, #4]\n\t"
        "ldr r0, [r2, #0x30]\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r5\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r4\n\t"
        "ldr r0, [r0]\n\t"
        "str r0, [r1]\n\t"
        "add r5, #1\n\t"
        "ldrh r2, [r2, #0xe]\n\t"
        "cmp r5, r2\n\t"
        "blt 4b\n\t"
    "5:\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x43\n\t"
        "mov r1, #1\n\t"
        "strb r1, [r0]\n\t"
    "6:\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "7: .4byte gUnknown_03001630\n"
    );
}

/* UNUSED - no caller anywhere in the ROM (checked every `asm/*.s`,
 * `expected/code_3.s`, `expected/legacy.s`, and every matched `.c` file
 * under `src/` for a `bl sub_8038DC0` / `.4byte sub_8038DC0` reference;
 * none exist). Otherwise a near-twin of `sub_8038E74`'s (`PlaySfx`'s
 * voice-stealing allocator, docs/audio.md) unconditional "scan every
 * row for the lowest `+0x4c` priority" path: walks the current
 * channel's `count` (`handler->+0x14`) array entries starting at
 * `handler->+8 + (subObj->+0xc << 2)`, tracks the lowest `+0x4c`
 * value's index, then writes `+0x24 = 8`, `+0x25 = chanArg` (this
 * function's own argument), and `+0x4c = 0` on the winning entry
 * before returning its index. Reads like a simpler, standalone sibling
 * of `sub_8038E74` that never got wired up to a real call site - dead
 * weight left over from development, or a superseded earlier version
 * of the same allocator.
 *
 * NAKED, not because of a many-register ceiling (only `r4`-`r7` are
 * used) but a genuine, extensively-attempted real-C gap: several C
 * reconstructions (bare loop, `do`/`while`, explicit register pins on
 * `chanArg`/`best`/the loop limit) got very close - matching the
 * `lsl #9`/mask-free body, the loop shape, and even most individual
 * register choices - but none reproduced the ROM's exact combination
 * (`r4`=best, `r5`=bestIdx, `r6`=the saved `&gUnknown_03001630`
 * copy kept alive across the loop, `r7`=`chanArg` held for the whole
 * function with no bleed into the loop's scratch registers) at once;
 * every attempt either left `chanArg` homed to `r12` (a plausible but
 * ROM-inequivalent choice for a value used only once, near the end) or
 * pinning `chanArg` to `r7` caused the allocator to reuse `r7` for the
 * post-loop saved-address value instead, changing the actual runtime
 * value read at the final `strb` (confirmed by direct byte comparison
 * against the real ROM, not assumed - see the isolated-compile
 * transcript this pass produced). Given this function is provably
 * unreachable, further register archaeology wasn't worth chasing
 * indefinitely; the semantics above are fully understood and this is a
 * mechanical, byte-verified transcription of the ROM's own
 * instructions. */
NAKED s32 sub_8038DC0(s32 chanArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r7, r0, #0\n\t"
        "ldr r4, 4f\n\t"
        "mov r3, #0\n\t"
        "ldr r2, 5f\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #0x14]\n\t"
        "add r6, r2, #0\n\t"
        "cmp r3, r0\n\t"
        "bhs 3f\n\t"
        "add r2, r0, #0\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, r0, r1\n\t"
    "1:\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0x4c]\n\t"
        "cmp r0, r4\n\t"
        "bgt 2f\n\t"
        "add r5, r3, #0\n\t"
        "add r4, r0, #0\n\t"
    "2:\n\t"
        "add r1, #4\n\t"
        "add r3, #1\n\t"
        "cmp r3, r2\n\t"
        "blo 1b\n\t"
    "3:\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x24\n\t"
        "mov r2, #0\n\t"
        "mov r1, #8\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x25\n\t"
        "strb r7, [r0]\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "str r2, [r0, #0x4c]\n\t"
        "add r0, r5, #0\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "4: .4byte 0x7FFFFFFF\n"
    "5: .4byte gUnknown_03001630\n"
    );
}

/* The voice-stealing mixer allocator (docs/audio.md, one of `PlaySfx`'s
 * two direct callees): resolves `channel` to a specific channel-row
 * index when it isn't `-1`, clamping it to the current channel's row
 * count and falling back to the lowest-`+0x4c`-priority row already
 * claimed by the requested `handle` if the exact index isn't free (an
 * `!= 0` sentinel check); when `channel == -1`, scans every row the
 * same way `sub_8038DC0` above does, unconditionally picking the
 * lowest-priority one. Unless the scan bails out entirely (index ==
 * `0x0FFFFFFF`, a sentinel), claims the winning row: `+0x24` gets `8`
 * or (`pitchOffset == -1`) `(pitchOffset >> 5) + 2`, `+0x25` gets
 * `handle`, and `+0x4c` gets `priority`.
 *
 * NAKED for the same many-register (`r8`/`ip`) gcc-2.9 allocation
 * ceiling already documented throughout this ROM region
 * (`sub_8006600`/`sub_80372BC`/`sub_8038240`/`sub_8038538`/
 * `sub_8038A1C` - docs/status/audio.md) - `self` is pinned to `r8` and
 * the saved `&gUnknown_03001630` copy to `ip`, both live across the
 * whole function including two separate scan loops, well past what
 * this compiler's register allocator can be steered into reproducing
 * from plain C. */
NAKED s32 sub_8038E74(u32 handle, s32 channel, s32 pitchOffset, s32 priority)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "mov r8, r0\n\t"
        "add r6, r2, #0\n\t"
        "add r7, r3, #0\n\t"
        "mov r5, #1\n\t"
        "neg r5, r5\n\t"
        "add r4, r6, #0\n\t"
        "add r3, r5, #0\n\t"
        "cmp r1, r5\n\t"
        "bne 4f\n\t"
        "mov r3, #0\n\t"
        "ldr r2, 3f\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #0x14]\n\t"
        "mov ip, r2\n\t"
        "cmp r3, r0\n\t"
        "bhs 6f\n\t"
        "add r2, r0, #0\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, r0, r1\n\t"
    "1:\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0x4c]\n\t"
        "cmp r0, r4\n\t"
        "bgt 2f\n\t"
        "add r5, r3, #0\n\t"
        "add r4, r0, #0\n\t"
    "2:\n\t"
        "add r1, #4\n\t"
        "add r3, #1\n\t"
        "cmp r3, r2\n\t"
        "blo 1b\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "3: .4byte gUnknown_03001630\n"
    "4:\n\t"
        "add r5, r1, #0\n\t"
        "ldr r2, 7f\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #0x14]\n\t"
        "mov ip, r2\n\t"
        "cmp r5, r0\n\t"
        "blo 5f\n\t"
        "add r5, r3, #0\n\t"
    "5:\n\t"
        "cmp r5, r3\n\t"
        "beq 6f\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x4c]\n\t"
        "cmp r6, r0\n\t"
        "bge 6f\n\t"
        "add r5, r3, #0\n\t"
    "6:\n\t"
        "ldr r0, 8f\n\t"
        "cmp r5, r0\n\t"
        "beq 11f\n\t"
        "mov r1, ip\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r4, [r0]\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r7, r0\n\t"
        "beq 9f\n\t"
        "asr r0, r7, #5\n\t"
        "add r1, r0, #2\n\t"
        "b 10f\n\t"
        ".align 2, 0\n"
    "7: .4byte gUnknown_03001630\n"
    "8: .4byte 0x0FFFFFFF\n"
    "9:\n\t"
        "mov r1, #8\n\t"
    "10:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x24\n\t"
        "strb r1, [r0]\n\t"
        "mov r1, ip\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x25\n\t"
        "mov r1, r8\n\t"
        "strb r1, [r0]\n\t"
        "mov r1, ip\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r1, #8]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "str r6, [r0, #0x4c]\n\t"
    "11:\n\t"
        "add r0, r5, #0\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}
