#include "core.h"
#include "settings_sync.h"

extern void sub_8002B70(void *arg0);

void sub_8002D28(struct settings_sync_record *self, u8 flags)
{
    register u8 loaded asm("r3");
    register u8 v asm("r1");

    loaded = self->flags;
    v = loaded | flags;
    self->flags = v;
    sub_8002B70(self);
}
/* Trailing byte-padding mismatch fix: GAS's default Thumb code
 * alignment filler is the `mov r8, r8` NOP (0x46c0), but the ROM pads
 * this function's tail with a zero halfword instead - force zero
 * padding to match (see docs/matching.md's alignment-padding gotcha /
 * the matching_decomp_alignment_fix convention). */
asm(".align 2, 0");

extern void *gUnknown_03000804;

/* sub_8002EFC (below) is the third of a trio with sub_8002D44/
 * sub_8002E20 - see docs/matching/issue-5-overlay-ui-sync.md for the
 * whole SIO send/receive-pump write-up, and its "NAKED-transcription
 * pass" section for how sub_8002D44/sub_8002E20 below went from parked
 * to matched. All three functions in this trio touch `r7`-class
 * scratch in a way this exact agbcc build's automatic callee-save
 * push/pop can never reproduce from plain C (confirmed by direct
 * reproduction - a function that only ever touches r7, whether via a
 * plain asm clobber, an explicit `register T x asm("r7")` pin used
 * across a real call, or a real C-level variable forced into r7 under
 * heavy register pressure, still comes back with no r7 in the push/pop
 * list every single time). This is the same categorical r7-pin
 * limitation already documented for `sub_8007DBC` (actor_part2.c),
 * `sub_802D3A8` (actor_part62.c) and others - see
 * matching_decomp_register_pinning memory point 10. sub_8002EFC itself
 * doesn't touch r7 at all, so it was matched first via plain
 * instruction-for-instruction asm transcription; sub_8002D44/
 * sub_8002E20 below need `r7` (and `sb`/`r8`) as genuine scratch
 * registers across their fill loops, so they're matched the same way,
 * as NAKED functions. */

/* Drains up to 0x60 bytes per call from `self->cursor` (streaming a
 * settings_sync_record out of `self->tmpl`) into the SIO session
 * object's outgoing ring buffer, tracked by that session's own
 * still-uncharacterized fields at +0xc4 (pending-byte count) and +0xcc
 * (ring write position, wrapping a 0x80-byte ring at +0x44). Marks
 * `field_214` once `remaining` is fully drained.
 *
 * Written as NAKED asm rather than plain C: `self` lives in r8 and
 * `sendLen` lives in r7 across the whole function, including the
 * fill-loop pair (wrap vs. no-wrap around the 0x80-byte ring) that
 * also needs `ip`/`sb` as extra scratch - exactly the r7-as-genuine-
 * scratch case described in the doc comment above. Every instruction
 * below is a direct, byte-verified transcription of the ROM's own
 * disassembly (that comment, plus this one, is the semantic
 * derivation for it). */
NAKED void sub_8002D44(struct settings_sync_pump *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "mov r8, r0\n\t"
        "ldr r1, [r0]\n\t"
        "cmp r1, #0\n\t"
        "beq 8f\n\t"
        "ldr r0, 3f\n\t"
        "ldr r0, [r0]\n\t"
        "mov ip, r0\n\t"
        "add r0, #0xc4\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 10f\n\t"
        "add r7, r1, #0\n\t"
        "cmp r7, #0x60\n\t"
        "ble 1f\n\t"
        "mov r7, #0x60\n\t"
    "1:\n\t"
        "mov r0, r8\n\t"
        "ldr r4, [r0, #0xc]\n\t"
        "mov r3, ip\n\t"
        "add r3, #0xcc\n\t"
        "mov r0, #0x80\n\t"
        "sub r0, r0, r7\n\t"
        "ldr r1, [r3]\n\t"
        "cmp r1, r0\n\t"
        "bge 4f\n\t"
        "sub r2, r7, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r2, r0\n\t"
        "beq 7f\n\t"
        "mov r5, ip\n\t"
        "add r5, #0xc4\n\t"
        "mov r6, ip\n\t"
        "add r6, #0x44\n\t"
        "mov ip, r0\n\t"
    "2:\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, #1\n\t"
        "str r0, [r3]\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, #1\n\t"
        "str r0, [r5]\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, r6, r0\n\t"
        "ldrb r1, [r4]\n\t"
        "strb r1, [r0]\n\t"
        "add r4, #1\n\t"
        "sub r2, #1\n\t"
        "cmp r2, ip\n\t"
        "bne 2b\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "3: .4byte gUnknown_03000804\n"
    "4:\n\t"
        "sub r2, r7, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r2, r0\n\t"
        "beq 7f\n\t"
        "add r1, r3, #0\n\t"
        "mov r6, ip\n\t"
        "add r6, #0xc4\n\t"
        "mov r3, #0x44\n\t"
        "add ip, r3\n\t"
        "mov sb, r0\n\t"
    "5:\n\t"
        "ldrb r5, [r4]\n\t"
        "add r4, #1\n\t"
        "ldr r0, [r1]\n\t"
        "mov r3, #0\n\t"
        "cmp r0, #0x7f\n\t"
        "beq 6f\n\t"
        "add r3, r0, #1\n\t"
    "6:\n\t"
        "str r3, [r1]\n\t"
        "ldr r0, [r6]\n\t"
        "add r0, #1\n\t"
        "str r0, [r6]\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, ip\n\t"
        "strb r5, [r0]\n\t"
        "sub r2, #1\n\t"
        "cmp r2, sb\n\t"
        "bne 5b\n\t"
    "7:\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1, #0xc]\n\t"
        "add r0, r0, r7\n\t"
        "str r0, [r1, #0xc]\n\t"
        "ldr r0, [r1, #0]\n\t"
        "sub r0, r0, r7\n\t"
        "b 9f\n\t"
    "8:\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0xc4\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 10f\n\t"
        "mov r1, #0x85\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r8\n\t"
        "mov r0, #1\n\t"
    "9:\n\t"
        "str r0, [r1]\n\t"
    "10:\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "11: .4byte gUnknown_03000804\n"
    );
}

/* Counterpart to sub_8002D44 above: drains whatever's available from
 * `playerIndex`'s per-player incoming ring buffer (session base +
 * playerIndex*0xc8, a still-uncharacterized per-player sub-record) into
 * `self->data` via `self->writePtr`. Marks `field_218` once
 * `totalReceived` reaches a full record's worth.
 *
 * Written as NAKED asm for the same reason as sub_8002D44 above: `self`
 * lives in ip across the whole function, and the fill-loop pair needs
 * r8 as genuine extra scratch. Every instruction below is a direct,
 * byte-verified transcription of the ROM's own disassembly. */
NAKED void sub_8002E20(struct settings_sync_pump *self, s32 playerIndex)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "mov ip, r0\n\t"
        "ldr r0, 2f\n\t"
        "ldr r3, [r0]\n\t"
        "mov r2, #0xc8\n\t"
        "add r0, r1, #0\n\t"
        "mul r0, r2, r0\n\t"
        "add r0, r0, r3\n\t"
        "mov r4, #0xc6\n\t"
        "lsl r4, r4, #1\n\t"
        "add r0, r0, r4\n\t"
        "ldr r6, [r0]\n\t"
        "cmp r6, #0\n\t"
        "beq 7f\n\t"
        "mov r0, #0x84\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, ip\n\t"
        "mul r2, r1, r2\n\t"
        "add r2, r2, r3\n\t"
        "mov r1, #0x84\n\t"
        "lsl r1, r1, #1\n\t"
        "add r2, r2, r1\n\t"
        "ldr r4, [r0]\n\t"
        "add r5, r2, #0\n\t"
        "add r5, #0x88\n\t"
        "mov r0, #0x80\n\t"
        "sub r0, r0, r6\n\t"
        "ldr r1, [r5]\n\t"
        "cmp r1, r0\n\t"
        "bge 3f\n\t"
        "sub r3, r6, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r3, r0\n\t"
        "beq 6f\n\t"
        "add r1, r5, #0\n\t"
        "add r5, r2, #4\n\t"
        "add r2, #0x84\n\t"
        "add r7, r0, #0\n\t"
    "1:\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, r5, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "strb r0, [r4]\n\t"
        "add r4, #1\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #1\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, [r2]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r2]\n\t"
        "sub r3, #1\n\t"
        "cmp r3, r7\n\t"
        "bne 1b\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "2: .4byte gUnknown_03000804\n"
    "3:\n\t"
        "sub r3, r6, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r3, r0\n\t"
        "beq 6f\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x84\n\t"
        "add r7, r2, #4\n\t"
        "mov r8, r0\n\t"
    "4:\n\t"
        "ldr r2, [r5]\n\t"
        "mov r0, #0\n\t"
        "cmp r2, #0x7f\n\t"
        "beq 5f\n\t"
        "add r0, r2, #1\n\t"
    "5:\n\t"
        "str r0, [r5]\n\t"
        "ldr r0, [r1]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r1]\n\t"
        "add r0, r7, r2\n\t"
        "ldrb r0, [r0]\n\t"
        "strb r0, [r4]\n\t"
        "add r4, #1\n\t"
        "sub r3, #1\n\t"
        "cmp r3, r8\n\t"
        "bne 4b\n\t"
    "6:\n\t"
        "mov r0, #0x84\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, ip\n\t"
        "ldr r1, [r0]\n\t"
        "add r1, r1, r6\n\t"
        "str r1, [r0]\n\t"
        "mov r4, ip\n\t"
        "ldr r0, [r4, #4]\n\t"
        "add r0, r0, r6\n\t"
        "str r0, [r4, #4]\n\t"
        "b 8f\n\t"
    "7:\n\t"
        "mov r0, ip\n\t"
        "ldr r1, [r0, #4]\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #2\n\t"
        "cmp r1, r0\n\t"
        "bne 8f\n\t"
        "mov r1, #0x86\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, ip\n\t"
        "mov r0, #1\n\t"
        "str r0, [r1]\n\t"
    "8:\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}
