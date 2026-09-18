#include "core.h"

/* GAX2_SoundHandler "Channel" type's play_fn (ROM 0x080395A5, see
 * docs/audio.md's per-type function-pointer table). `self` is this
 * channel's own handler object; `info` (`*(void**)(self+8)`) is the
 * shared Info handler every channel's `children_ptr` points at
 * (docs/audio.md's "one shared 4-byte array... containing the Info
 * handler's address"). First forwards this call straight into `info`'s
 * own play_fn slot (`info->0->0x8`, the same three-function-pointer-
 * per-type table this function itself is a member of, via the
 * `sub_803AD84` "call through r3" trampoline), passing this function's
 * own `(arg1, chanArg)` through unchanged - the same "run this
 * handler's assigned function" dispatch pattern `sub_803A22C`
 * (UnknownC type's init_fn) uses via `sub_803AD7C`, just through the
 * `r3`-argument trampoline instead since a return value's expected
 * here. Then: if `info` armed a "retrigger" flag (`info->0x1b`), clears
 * this channel's own `field_0x3c`; if `info` is armed at all
 * (`info->0x1a`), advances this channel's own countdown (`field_0x34`,
 * a signed halfword) and fires `sub_8039658(self, info, 1)` once it
 * hits zero, or fires `sub_8039658(self, info, 0)` whenever
 * `info->0x18` is set and `info->0x1d` is set; drives a
 * `field_0x1f`/`field_0x1e` "note-cut countdown" pair (calling
 * `sub_80398DC` each time it's re-armed from `field_0x1e`); always runs
 * `sub_8039AA4`; and finally, only when `self->0xc == 0`, forwards into
 * `sub_8039B44(self, info, arg1, chanArg, info->0->0x18, 0)` and
 * returns its low byte - otherwise returns 0. Object shape not
 * confidently modeled yet (same situation as the other
 * GAX2_SoundHandler functions in this cluster) - kept as raw offsets
 * throughout.
 *
 * Written as NAKED asm, not plain C: a real C reconstruction (every
 * field access/branch above independently confirmed against this
 * exact disassembly first) got every single instruction byte-identical
 * except the 3-instruction `self`/`arg1`/`chanArg` parameter-homing
 * sequence right after the prologue, which the ROM orders
 * `r4, r6, r7` (`self` first) but this compiler's register allocator -
 * pinned or not, and in every combination of which of the three gets
 * an explicit `register T x asm("rN")` pin - only ever produces `r6,
 * r7, r4` or `r7, r4, r6`. `chanArg` can't be pinned to force the issue
 * further: `r7` hits this toolchain's confirmed pin bug (an explicit
 * `register T x asm("r7")` compiles with no push/pop of `r7` at all -
 * see hud_icon_widget_85c4.c's `InitHudIconWidgetA`/`B` for the same
 * conclusion on an unrelated function), so only fully-unpinned
 * allocation can safely put a value in `r7`, and that path's own
 * ordering choice never matches the ROM's either. Mechanical,
 * byte-verified transcription of the ROM's own instructions
 * (translated from the disassembler's unified syntax to this project's
 * established NAKED plain/divided syntax, local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess. */
NAKED u32 sub_80395A4(void *self, void *arg1, u32 chanArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add sp, sp, #-0x8\n\t"
        "add r4, r0, #0\n\t"
        "add r6, r1, #0\n\t"
        "add r7, r2, #0\n\t"
        "ldr r0, [r4, #8]\n\t"
        "ldr r5, [r0]\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r3, [r0, #8]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_803AD84\n\t"
        "ldrb r0, [r5, #0x1b]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r4, #0x3c]\n\t"
    "1:\n\t"
        "ldrb r0, [r5, #0x1a]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "ldrh r1, [r4, #0x34]\n\t"
        "mov r2, #0x34\n\t"
        "ldrsh r0, [r4, r2]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "sub r0, r1, #1\n\t"
        "strh r0, [r4, #0x34]\n\t"
        "lsl r0, r0, #0x10\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r5, #0\n\t"
        "mov r2, #1\n\t"
        "bl sub_8039658\n\t"
    "2:\n\t"
        "ldrh r0, [r5, #0x18]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "ldrb r0, [r5, #0x1d]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r5, #0\n\t"
        "mov r2, #0\n\t"
        "bl sub_8039658\n\t"
    "3:\n\t"
        "ldr r0, [r4, #0x3c]\n\t"
        "ldrb r1, [r4, #0x1f]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "cmp r1, #0\n\t"
        "bne 5f\n\t"
        "ldrb r0, [r4, #0x1e]\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_80398DC\n\t"
        "ldrb r0, [r4, #0x1e]\n\t"
        "sub r0, r0, #1\n\t"
        "b 4f\n\t"
    "5:\n\t"
        "sub r0, r1, #1\n\t"
    "4:\n\t"
        "strb r0, [r4, #0x1f]\n\t"
    "6:\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_8039AA4\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "cmp r1, #0\n\t"
        "bne 8f\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r0, [r0, #0x18]\n\t"
        "str r0, [sp]\n\t"
        "str r1, [sp, #4]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r5, #0\n\t"
        "add r2, r6, #0\n\t"
        "add r3, r7, #0\n\t"
        "bl sub_8039B44\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "b 9f\n\t"
    "8:\n\t"
        "mov r0, #0\n\t"
    "9:\n\t"
        "add sp, sp, #0x8\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}

/* `sub_80395A4`'s direct callee - a per-note-event command dispatcher
 * for this channel: `self+0x40` is a rolling cursor into the current
 * pattern-row byte stream (docs/audio.md's per-song pattern data); this
 * decodes one "packed row" record (a leading control byte whose top two
 * bits select how many of note/instrument/volume/effect follow, per
 * the `0x80`/`0x7f` masks below) unless `self+0xe`/`self+0xf` (a
 * pending-rows/hold countdown pair) says to skip decoding this tick,
 * dispatches the decoded command byte (`sb`, an effect-command index)
 * through the 15-entry jump table starting at case 0 (most cases just
 * store the low nibble - `r5` - into a channel field, cases 6/14 also
 * poke the shared `info` handler's own state), and always re-primes
 * `self+0x1a`/`0x28`/`0x34` to 0 up front and calls
 * `sub_8039818`/`sub_803985C` (per-channel note-cut dispatch and voice
 * trigger) unless the decoded effect index is exactly 3. `flag`
 * (function argument 3, the caller's `1`/`0` from `sub_80395A4` above)
 * selects between reading a *new* record from the pattern stream
 * (`flag == 0`) or replaying the *same* `self+0x50`/`0x51` "last
 * command" bytes again (`flag != 0`, used for the retrigger case).
 * Object shape not confidently modeled yet - kept as raw offsets.
 *
 * Written as NAKED asm, not plain C: this function's prologue alone
 * (`push {r4-r7,lr}; mov r7,sb; mov r6,r8; push {r6,r7}`) needs both
 * `r8`/`sb` as genuine scratch across the packed-row decode - the same
 * many-register gcc-2.9 allocation ceiling already documented
 * throughout this ROM region for `sub_8038538`'s cluster
 * (docs/status/audio.md), which a NAKED function sidesteps entirely
 * since nothing asks gcc's allocator to decide anything. Mechanical,
 * byte-verified transcription of the ROM's own instructions
 * (translated from the disassembler's unified syntax to this project's
 * established NAKED plain/divided syntax; the hand-placed 15-entry
 * jump table uses named local labels, the same "hand-placed local
 * labels shared across a single literal pool" idea as
 * actor_part38b.c's `sub_80151C8` jump table, rather than numbered
 * ones, given how many branch targets this function has), not an
 * inferred control-flow guess. */
NAKED void sub_8039658(void *self, void *info, u32 flag)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "add r4, r0, #0\n\t"
        "add r7, r1, #0\n\t"
        "lsl r2, r2, #0x18\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r4, #0x1a]\n\t"
        "strh r0, [r4, #0x28]\n\t"
        "strh r0, [r4, #0x34]\n\t"
        "cmp r2, #0\n\t"
        "bne L9658_newrow\n\t"
        "ldrb r0, [r7, #0x1e]\n\t"
        "cmp r0, #0\n\t"
        "beq L9658_afterload\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r2, [r0, #0x18]\n\t"
        "ldr r1, [r4]\n\t"
        "mov r3, #0x14\n\t"
        "ldrsh r0, [r7, r3]\n\t"
        "ldr r1, [r1, #0x18]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrh r1, [r0]\n\t"
        "ldr r0, [r2, #0xc]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #0x40]\n\t"
        "mov r1, #0\n\t"
        "strb r1, [r4, #0xf]\n\t"
        "ldrb r1, [r0]\n\t"
        "strb r1, [r4, #0xe]\n\t"
        "add r0, r0, #1\n\t"
        "str r0, [r4, #0x40]\n\t"
    "L9658_afterload:\n\t"
        "ldrb r0, [r4, #0xe]\n\t"
        "cmp r0, #0\n\t"
        "beq L9658_checkf\n\t"
        "b L9658_end\n\t"
    "L9658_checkf:\n\t"
        "ldrb r0, [r4, #0xf]\n\t"
        "cmp r0, #0\n\t"
        "beq L9658_decode\n\t"
        "sub r0, r0, #1\n\t"
        "strb r0, [r4, #0xf]\n\t"
        "b L9658_end\n\t"
    "L9658_decode:\n\t"
        "ldr r2, [r4, #0x40]\n\t"
        "ldrb r3, [r2]\n\t"
        "add r0, r3, #0\n\t"
        "cmp r0, #0xff\n\t"
        "bne L9658_notff\n\t"
        "ldrb r0, [r2, #1]\n\t"
        "sub r0, r0, #1\n\t"
        "strb r0, [r4, #0xf]\n\t"
        "add r0, r2, #2\n\t"
        "str r0, [r4, #0x40]\n\t"
        "b L9658_end\n\t"
    "L9658_notff:\n\t"
        "mov r0, #0x80\n\t"
        "and r0, r3\n\t"
        "cmp r0, #0\n\t"
        "beq L9658_bit80clear\n\t"
        "mov r1, #0x7f\n\t"
        "and r1, r3\n\t"
        "cmp r1, #0\n\t"
        "bne L9658_bit7fset\n\t"
        "add r0, r2, #1\n\t"
        "str r0, [r4, #0x40]\n\t"
        "b L9658_end\n\t"
    "L9658_bit7fset:\n\t"
        "cmp r1, #0x79\n\t"
        "bhi L9658_hibranch\n\t"
        "mov r8, r1\n\t"
        "ldrb r6, [r2, #1]\n\t"
        "mov r5, #0\n\t"
        "mov sb, r5\n\t"
        "add r0, r2, #2\n\t"
        "b L9658_gotcmd\n\t"
    "L9658_hibranch:\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "mov r6, #0\n\t"
        "ldrb r1, [r2, #1]\n\t"
        "mov sb, r1\n\t"
        "ldrb r5, [r2, #2]\n\t"
        "add r0, r2, #3\n\t"
        "b L9658_gotcmd\n\t"
    "L9658_bit80clear:\n\t"
        "ldrb r3, [r2]\n\t"
        "mov r8, r3\n\t"
        "ldrb r6, [r2, #1]\n\t"
        "ldrb r0, [r2, #2]\n\t"
        "mov sb, r0\n\t"
        "ldrb r5, [r2, #3]\n\t"
        "add r0, r2, #4\n\t"
    "L9658_gotcmd:\n\t"
        "str r0, [r4, #0x40]\n\t"
        "mov r1, sb\n\t"
        "cmp r1, #0xe\n\t"
        "bne L9658_dispatch\n\t"
        "lsr r0, r5, #4\n\t"
        "cmp r0, #0xd\n\t"
        "bne L9658_dispatch\n\t"
        "mov r0, #0xf\n\t"
        "and r5, r0\n\t"
        "strh r5, [r4, #0x34]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, r0, #0x50\n\t"
        "mov r2, r8\n\t"
        "strb r2, [r0]\n\t"
        "add r0, r0, #1\n\t"
        "strb r6, [r0]\n\t"
        "b L9658_end\n\t"
    "L9658_newrow:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, r0, #0x50\n\t"
        "ldrb r0, [r0]\n\t"
        "mov r8, r0\n\t"
        "add r0, r4, #0\n\t"
        "add r0, r0, #0x51\n\t"
        "ldrb r6, [r0]\n\t"
        "mov r5, #0\n\t"
        "mov sb, r5\n\t"
    "L9658_dispatch:\n\t"
        "mov r3, sb\n\t"
        "cmp r3, #3\n\t"
        "beq L9658_afterdispatch\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, r8\n\t"
        "bl sub_8039818\n\t"
    "L9658_afterdispatch:\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r3, [r0, #0x18]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "add r2, r6, #0\n\t"
        "bl sub_803985C\n\t"
        "mov r0, sb\n\t"
        "sub r0, r0, #1\n\t"
        "cmp r0, #0xe\n\t"
        "bhi L9658_end\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, L9658_tbl_ptr\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "L9658_tbl_ptr: .4byte L9658_tbl\n"
    "L9658_tbl:\n\t"
        ".4byte L9658_case0\n\t"
        ".4byte L9658_case1\n\t"
        ".4byte L9658_case2\n\t"
        ".4byte L9658_end\n\t"
        ".4byte L9658_end\n\t"
        ".4byte L9658_end\n\t"
        ".4byte L9658_case6\n\t"
        ".4byte L9658_end\n\t"
        ".4byte L9658_end\n\t"
        ".4byte L9658_case9\n\t"
        ".4byte L9658_case10\n\t"
        ".4byte L9658_case11\n\t"
        ".4byte L9658_case12\n\t"
        ".4byte L9658_end\n\t"
        ".4byte L9658_case14\n"
    "L9658_case0:\n\t"
        "strh r5, [r4, #0x28]\n\t"
        "b L9658_end\n\t"
    "L9658_case1:\n\t"
        "neg r0, r5\n\t"
        "strh r0, [r4, #0x28]\n\t"
        "b L9658_end\n\t"
    "L9658_case2:\n\t"
        "cmp r5, #0\n\t"
        "beq L9658_end\n\t"
        "mov r0, r8\n\t"
        "sub r0, r0, #2\n\t"
        "lsl r0, r0, #5\n\t"
        "strh r0, [r4, #0x30]\n\t"
        "mov r1, #0x30\n\t"
        "ldrsh r0, [r4, r1]\n\t"
        "mov r2, #0x26\n\t"
        "ldrsh r1, [r4, r2]\n\t"
        "sub r0, r0, r1\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_803ADB4\n\t"
        "strh r0, [r4, #0x32]\n\t"
        "b L9658_end\n\t"
    "L9658_case6:\n\t"
        "lsr r0, r5, #4\n\t"
        "lsl r1, r5, #8\n\t"
        "mov r3, #0xf0\n\t"
        "lsl r3, r3, #4\n\t"
        "add r2, r3, #0\n\t"
        "and r1, r2\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r7, #0x18]\n\t"
        "sub r0, r0, #1\n\t"
        "b L9658_setflag\n\t"
    "L9658_case9:\n\t"
        "strh r5, [r4, #0x1a]\n\t"
        "b L9658_end\n\t"
    "L9658_case10:\n\t"
        "neg r0, r5\n\t"
        "strh r0, [r4, #0x1a]\n\t"
        "b L9658_end\n\t"
    "L9658_case11:\n\t"
        "strb r5, [r4, #0x15]\n\t"
        "b L9658_end\n\t"
    "L9658_case12:\n\t"
        "add r1, r7, #0\n\t"
        "add r1, r1, #0x22\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "strh r5, [r7, #0x24]\n\t"
        "b L9658_end\n\t"
    "L9658_case14:\n\t"
        "strh r5, [r7, #0x18]\n\t"
        "sub r0, r5, #1\n\t"
    "L9658_setflag:\n\t"
        "strb r0, [r7, #0x1c]\n\t"
    "L9658_end:\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n\t"
    );
}
