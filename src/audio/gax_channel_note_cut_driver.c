#include "core.h"

/* GAX2 per-channel per-tick note-cut/retrigger driver (issue #68,
 * docs/status/audio.md's "many-register r8/sb/sl" group) - a wider
 * near-twin of the matched-but-NAKED `sub_80395A4`
 * (gax_sound_handler_channel_play.c)'s calling pattern. `self` is the
 * channel object (also used by `sub_8039818`/`sub_803985C`/
 * `sub_80398DC`/`sub_8039AA4`/`sub_8039B44`); `self+8`'s own `+0` is
 * the shared "info" handler object (`r6` throughout). First: if
 * `info+0x1b` (retrigger flag) is set, clears `self+0x3c`. Then, when
 * `info+0x1a` is armed and `self+0x24`/`self+0x25` (a small state
 * pair) allow it and `info+0x1b` is clear, fires
 * `sub_8039818(self, self[0x24])` then
 * `sub_803985C(self, info, self[0]->0x18, self[0x25])`, zeroing
 * `self+0x1a`/`0x28`/`0x24`/`0x25` afterward. Next, drives a
 * `self+0x1f`/`0x1e` "note-cut countdown" pair exactly like
 * `sub_80395A4` does (calling `sub_80398DC` each time it re-arms from
 * `self+0x1e`), always runs `sub_8039AA4`, and finally - only when
 * `self+0xc == 0` - forwards into
 * `sub_8039B44(self, info, arg1, arg2, self[0]->0x18, 1)` and returns
 * its low byte, or 0 otherwise. Channel/info object shape not
 * confidently modeled yet (same situation as the rest of this GAX2
 * cluster) - kept as raw offsets throughout.
 *
 * Written as NAKED asm, not plain C: needs `r8` (caching `&self[0x24]`
 * across several branches), `sb`/`sl` (preserving the incoming
 * `arg1`/`arg2` parameters across five separate `bl` calls that clobber
 * `r0-r3`) all three simultaneously live for most of the function - the
 * same many-register gcc-2.9 allocation ceiling already documented
 * throughout this ROM region (`sub_8006600`/`sub_8038538`/
 * `sub_80395A4`'s wider sibling `sub_8039658`). Mechanical,
 * byte-verified transcription of the ROM's own instructions
 * (translated from the disassembler's unified syntax to this project's
 * established NAKED plain/divided syntax, local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess. */
NAKED u32 sub_803A158(void *self, void *arg1, void *arg2)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, sp, #8\n\t"
        "add r4, r0, #0\n\t"
        "mov sb, r1\n\t"
        "mov sl, r2\n\t"
        "ldr r0, [r4, #8]\n\t"
        "ldr r6, [r0]\n\t"
        "ldrb r0, [r6, #0x1b]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r4, #0x3c]\n\t"
    "1:\n\t"
        "ldrb r0, [r6, #0x1a]\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "mov r0, #0x24\n\t"
        "add r0, r0, r4\n\t"
        "mov r8, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #1\n\t"
        "beq 2f\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "add r0, r4, #0\n\t"
        "add r0, r0, #0x25\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
    "2:\n\t"
        "ldrb r5, [r6, #0x1b]\n\t"
        "cmp r5, #0\n\t"
        "bne 6f\n\t"
        "mov r0, r8\n\t"
        "ldrb r1, [r0]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8039818\n\t"
        "add r7, r4, #0\n\t"
        "add r7, r7, #0x25\n\t"
        "ldrb r2, [r7]\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r3, [r0, #0x18]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_803985C\n\t"
        "mov r0, #0\n\t"
        "strh r5, [r4, #0x1a]\n\t"
        "strh r5, [r4, #0x28]\n\t"
        "strb r0, [r7]\n\t"
        "mov r1, r8\n\t"
        "strb r0, [r1]\n\t"
    "6:\n\t"
        "ldr r0, [r4, #0x3c]\n\t"
        "ldrb r1, [r4, #0x1f]\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "cmp r1, #0\n\t"
        "bne 4f\n\t"
        "ldrb r0, [r4, #0x1e]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_80398DC\n\t"
        "ldrb r0, [r4, #0x1e]\n\t"
        "sub r0, r0, #1\n\t"
        "b 3f\n\t"
    "4:\n\t"
        "sub r0, r1, #1\n\t"
    "3:\n\t"
        "strb r0, [r4, #0x1f]\n\t"
    "5:\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_8039AA4\n\t"
        "ldrb r0, [r4, #0xc]\n\t"
        "cmp r0, #0\n\t"
        "bne 8f\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r0, [r0, #0x18]\n\t"
        "str r0, [sp]\n\t"
        "mov r0, #1\n\t"
        "str r0, [sp, #4]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
        "mov r2, sb\n\t"
        "mov r3, sl\n\t"
        "bl sub_8039B44\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "b 9f\n\t"
    "8:\n\t"
        "mov r0, #0\n\t"
    "9:\n\t"
        "add sp, sp, #8\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n\t"
    );
}
