#include "core.h"

/* A `gStaticData_0817A6B8` stride-8 trampoline-record dispatcher:
 * `{s16 baseOff; s16 count; s16 subOffset}` records, indexed by
 * `self+0x28`'s state. When `count > 0`, indexes a per-instance list
 * pointer at `self+subOffset` and reads its last entry's `{s32 delta;
 * void *fn}` pair, added to `baseOff` for the trampoline address;
 * otherwise falls back to the record's own inline `{..; void *fn}`
 * pair at `+4` with just `baseOff` for the address. Fires
 * `sub_803AD84(self+addr, baseOff, count, fn)`.
 *
 * Semantics are fully understood, but this is transcribed as NAKED
 * asm rather than plain C: the ROM's own build keeps `gStaticData_
 * 0817A6B8`'s base address alive in `r7` for the whole function (the
 * plain `adds r7, r1, #0` variant of this project's documented
 * "explicit `register T x asm(\"r7\")` compiles correct instructions
 * but silently drops r7 from the prologue/epilogue push/pop list"
 * gcc-2.9 bug - see docs/matching.md's `sub_8007DBC`/`sub_8006600`
 * entries, and `src/system/link_cable.c`'s `sub_8001CB8` for another
 * instance of this same NAKED-transcription workaround). This
 * compiler's own *unforced* register allocator never reaches r7 here
 * either (confirmed: r7 only enters this compiler's natural push/pop
 * set via the separate `mov r7, sb` high-register relay idiom, which
 * doesn't apply to this function - it has no r8-r11 pressure to relay
 * from), so no C-level rephrasing/pinning can reach the ROM's exact
 * register allocation; every other `gStaticData_*` stride-8 trampoline
 * dispatcher sharing this shape (`sub_802F748`, `sub_8033B44`,
 * `sub_8033C84`, `sub_8033E80`, `sub_8033FE4`) hits the identical r7
 * hazard and is transcribed the same way. */
NAKED void sub_802C208(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, 1f\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r3, r0, #3\n\t"
        "add r0, r3, r1\n\t"
        "mov r7, #2\n\t"
        "ldrsh r2, [r0, r7]\n\t"
        "add r7, r1, #0\n\t"
        "cmp r2, #0\n\t"
        "ble 2f\n\t"
        "mov r1, #4\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r1\n\t"
        "sub r0, #8\n\t"
        "ldr r5, [r0]\n\t"
        "ldr r6, [r0, #4]\n\t"
        "add r3, r6, #0\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_0817A6B8\n"
    "2:\n\t"
        "add r0, r7, #4\n\t"
        "add r0, r3, r0\n\t"
        "ldr r3, [r0]\n\t"
    "3:\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r0, r7\n\t"
        "mov r7, #0\n\t"
        "ldrsh r1, [r0, r7]\n\t"
        "cmp r2, #0\n\t"
        "ble 4f\n\t"
        "lsl r0, r5, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "add r0, r0, r1\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "add r0, r1, #0\n\t"
    "5:\n\t"
        "add r0, r4, r0\n\t"
        "bl sub_803AD84\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}
