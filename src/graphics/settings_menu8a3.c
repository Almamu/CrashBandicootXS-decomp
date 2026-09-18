#include "core.h"
#include "settings_sync.h"

extern void *gUnknown_03000804;
extern void sub_8002E20(struct settings_sync_pump *self, s32 playerIndex);
extern void sub_8002D44(struct settings_sync_pump *self);

/* Polls the SIO-handshake spinner's transfer state once per frame: if
 * the session (*gUnknown_03000804, byte +7 = "connected") isn't
 * connected, just tracks completion/reset of `self` and returns
 * 1 (reset)/0 (still finishing). If connected, picks a player slot from
 * the session's +0x3fc negotiation value, pumps RX (sub_8002E20) and TX
 * (sub_8002D44) at most once each per call, and once both sides report
 * complete, waits ~0x1e extra polls before finally returning 0
 * ("settled"). Returns 2 if the session's +0x3fc value is neither 0 nor
 * 1 (unrecognised role).
 *
 * Its two siblings (sub_8002D44/sub_8002E20, src/graphics/
 * settings_menu8a2.c) stay parked - both need r7 as genuine scratch,
 * and this exact agbcc build never includes r7 in a function's
 * automatic callee-save push/pop (see the doc comment above their
 * parked C for the full explanation and repro). This function doesn't
 * touch r7, so it isn't affected, and is spelled out as a direct
 * instruction-for-instruction transcription of the ROM's own
 * disassembly like the rest of this chunk's r7-affected neighbours -
 * see docs/matching/issue-5-overlay-ui-sync.md. */
s32 sub_8002EFC(struct settings_sync_pump *self)
{
    register s32 result asm("r0");

    asm volatile(
        "add r4, %1, #0\n"
        "ldr r0, =gUnknown_03000804\n"
        "ldr r1, [r0]\n"
        "ldrb r0, [r1, #7]\n"
        "cmp r0, #0\n"
        "bne 1f\n"
        "movs r1, #0x86\n"
        "lsl r1, r1, #2\n"
        "add r0, r4, r1\n"
        "ldr r0, [r0]\n"
        "cmp r0, #0\n"
        "beq 2f\n"
        "sub r1, #4\n"
        "add r0, r4, r1\n"
        "ldr r0, [r0]\n"
        "cmp r0, #0\n"
        "bne 9f\n"
        "2:\n"
        "movs r0, #0x80\n"
        "lsl r0, r0, #2\n"
        "str r0, [r4]\n"
        "movs r2, #0\n"
        "str r2, [r4, #4]\n"
        "ldr r0, [r4, #8]\n"
        "str r0, [r4, #0xc]\n"
        "movs r0, #0x84\n"
        "lsl r0, r0, #2\n"
        "add r1, r4, r0\n"
        "add r0, r4, #0\n"
        "add r0, #0x10\n"
        "str r0, [r1]\n"
        "movs r1, #0x85\n"
        "lsl r1, r1, #2\n"
        "add r0, r4, r1\n"
        "str r2, [r0]\n"
        "add r1, #4\n"
        "add r0, r4, r1\n"
        "str r2, [r0]\n"
        "add r1, #4\n"
        "add r0, r4, r1\n"
        "str r2, [r0]\n"
        "movs %0, #1\n"
        "b 10f\n"
        ".pool\n"
        "1:\n"
        "movs r0, #0xff\n"
        "lsl r0, r0, #2\n"
        "add r1, r1, r0\n"
        "ldr r0, [r1]\n"
        "cmp r0, #0\n"
        "bne 3f\n"
        "movs r1, #1\n"
        "b 4f\n"
        "3:\n"
        "ldr r0, [r1]\n"
        "cmp r0, #1\n"
        "beq 5f\n"
        "movs %0, #2\n"
        "b 10f\n"
        "5:\n"
        "movs r1, #0\n"
        "4:\n"
        "movs r0, #0x86\n"
        "lsl r0, r0, #2\n"
        "add r6, r4, r0\n"
        "ldr r0, [r6]\n"
        "cmp r0, #0\n"
        "bne 6f\n"
        "add r0, r4, #0\n"
        "bl sub_8002E20\n"
        "6:\n"
        "movs r1, #0x85\n"
        "lsl r1, r1, #2\n"
        "add r5, r4, r1\n"
        "ldr r0, [r5]\n"
        "cmp r0, #0\n"
        "bne 7f\n"
        "add r0, r4, #0\n"
        "bl sub_8002D44\n"
        "7:\n"
        "movs r3, #0\n"
        "ldr r0, [r6]\n"
        "cmp r0, #0\n"
        "beq 11f\n"
        "ldr r0, [r5]\n"
        "cmp r0, #0\n"
        "beq 11f\n"
        "movs r1, #0x87\n"
        "lsl r1, r1, #2\n"
        "add r0, r4, r1\n"
        "ldr r1, [r0]\n"
        "add r2, r1, #0\n"
        "add r1, #1\n"
        "str r1, [r0]\n"
        "cmp r2, #0x1e\n"
        "ble 11f\n"
        "movs r3, #1\n"
        "11:\n"
        "cmp r3, #0\n"
        "bne 9f\n"
        "movs %0, #1\n"
        "b 10f\n"
        "9:\n"
        "movs %0, #0\n"
        "10:\n"
        : "=r" (result)
        : "r" (self)
        : "r1", "r2", "r3", "r4", "r5", "r6", "cc", "memory"
    );

    return result;
}
/* Trailing byte-padding mismatch fix: GAS's default Thumb code
 * alignment filler is the `mov r8, r8` NOP (0x46c0), but the ROM pads
 * this function's tail with a zero halfword instead (see
 * docs/matching.md's alignment-padding gotcha / the
 * matching_decomp_alignment_fix convention). */
asm(".align 2, 0");
