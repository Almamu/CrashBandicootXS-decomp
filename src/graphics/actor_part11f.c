#include "core.h"
#include "actor.h"

extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void *sub_803AD7C(void *arg0, void *fn);
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void sub_80096C0(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, void *part);
extern void sub_80099F0(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, void *part, void *otherViewport);
extern void *gUnknown_03001308;
extern void *gUnknown_030012D8;

/* The spatial-hash-grid-cluster analog of `sub_8008A40`: the same
 * "extended screen box" filter shape as `sub_800944C` (iterating
 * `manager`'s grid buckets from `baseIdx+2` down to 0, then the
 * special "large object" bucket 255), but instead of firing a simple
 * action trampoline on a box hit, additionally tests `part->flags`
 * bit 2 and fires a `table+0x48/0x4c`-driven trampoline via
 * `sub_803AD7C`; if that result is greater than 4, reconstructs the
 * caller's original `{boxX, boxY, boxW, boxH}` box (via
 * `sub_800014C`, the same "unavoidable extra `boxH` load" idiom
 * established for `sub_8008A40`) and dispatches to `sub_80096C0`
 * (when `compareViewport` is the player, `gUnknown_030012D8`) or
 * `sub_80099F0` (otherwise) - the exact same dispatch `sub_8008A40`
 * makes to `sub_8008AD8`/`sub_8008D80`. See `sub_8008A40`'s own
 * writeup (`actor_part7.c`) for the full branch-by-branch semantics,
 * identical here.
 *
 * PARKED AS NAKED: the semantics above were fully confirmed (every
 * branch, field offset, and call argument traced against the ROM
 * disassembly) as a `#if NON_MATCHING` C reconstruction, but that
 * reconstruction's compiled size stayed noticeably larger than the
 * real ROM function - a bigger gap than the single `boxH`-reuse idiom
 * alone accounts for, involving both the incoming 7-argument stack
 * frame (3 words spilled below the callee-saved registers before the
 * `sl`/`sb`/`r8` triple is even pushed) and register allocation across
 * the two nested grid loops (each containing a duplicated inline
 * dispatch, mirroring `sub_800944C`'s own two-pass shape). Given this
 * session's proven track record hand-transcribing even large,
 * register-pressure-heavy functions (`sub_8010B6C`, `sub_80096C0`,
 * `sub_8008AD8`/`sub_8008D80`/`sub_80099F0`), it's hand-transcribed as
 * literal Thumb asm instead: the ROM's own ldr/str/lsl/asr sequence,
 * one-to-one, both grid passes byte-identical to each other. Kept in
 * its own translation unit (not appended to `actor_part11.c`) since
 * its real ROM address, 0x08009528, sits between `sub_800944C` (still
 * raw asm, `asm/code_3_2_13_944c.s`) and `sub_80096C0`
 * (`actor_part11e.c`) in ROM order - see docs/workflow.md step 4's
 * "needs its own new .c file" case. See docs/matching.md, "Parked, not
 * matched: `sub_8009528`". */
NAKED void sub_8009528(void *managerArg, s32 boxX, s32 boxY, s32 boxW, s32 boxH, s32 unused, void *compareViewport)
{
    asm(
        "sub sp, #0xc\n\t"
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x30\n\t"
        "add r7, r0, #0\n\t"
        "str r1, [sp, #0x50]\n\t"
        "str r2, [sp, #0x54]\n\t"
        "str r3, [sp, #0x58]\n\t"
        "ldr r0, [sp, #0x64]\n\t"
        "mov sb, r0\n\t"
        "ldr r0, 4f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r2, [r0, #0x10]\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r1, r1, #8\n\t"
        "ldr r0, [r2, #4]\n\t"
        "lsl r0, r0, #8\n\t"
        "str r1, [sp, #0xc]\n\t"
        "str r0, [sp, #0x10]\n\t"
        "mov r0, #0xf0\n\t"
        "lsl r0, r0, #8\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #8\n\t"
        "str r0, [sp, #0x14]\n\t"
        "str r1, [sp, #0x18]\n\t"
        "ldr r6, [r2]\n\t"
        "asr r6, r6, #8\n\t"
        "cmp r6, #0\n\t"
        "bge 1f\n\t"
        "mov r6, #0\n\t"
    "1:\n\t"
        "add r1, r6, #2\n\t"
        "mov r2, #0x10\n\t"
        "add r2, r2, r7\n\t"
        "mov sl, r2\n\t"
        "ldr r0, 5f\n\t"
        "add r0, r7, r0\n\t"
        "str r0, [sp, #0x2c]\n\t"
    "2:\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, sl\n\t"
        "ldr r5, [r0]\n\t"
        "sub r1, #1\n\t"
        "mov r8, r1\n\t"
        "cmp r5, #0\n\t"
        "beq 9f\n\t"
    "3:\n\t"
        "ldr r4, [r5]\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "mov r2, #0x30\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x34]\n\t"
        "add r1, sp, #0xc\n\t"
        "bl sub_803AD80\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "lsr r0, r1, #2\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x48\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "cmp r0, #4\n\t"
        "ble 8f\n\t"
        "ldr r0, 6f\n\t"
        "ldr r0, [r0]\n\t"
        "cmp sb, r0\n\t"
        "bne 7f\n\t"
        "add r0, sp, #0x1c\n\t"
        "add r1, sp, #0x50\n\t"
        "mov r2, #0x10\n\t"
        "bl sub_800014C\n\t"
        "str r4, [sp, #4]\n\t"
        "ldr r0, [sp, #0x28]\n\t"
        "str r0, [sp]\n\t"
        "ldr r1, [sp, #0x1c]\n\t"
        "ldr r2, [sp, #0x20]\n\t"
        "ldr r3, [sp, #0x24]\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_80096C0\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "4: .4byte gUnknown_03001308\n"
    "5: .4byte 0x0000040C\n"
    "6: .4byte gUnknown_030012D8\n"
    "7:\n\t"
        "add r0, sp, #0x1c\n\t"
        "add r1, sp, #0x50\n\t"
        "mov r2, #0x10\n\t"
        "bl sub_800014C\n\t"
        "str r4, [sp, #4]\n\t"
        "mov r0, sb\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [sp, #0x28]\n\t"
        "str r0, [sp]\n\t"
        "ldr r1, [sp, #0x1c]\n\t"
        "ldr r2, [sp, #0x20]\n\t"
        "ldr r3, [sp, #0x24]\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_80099F0\n\t"
    "8:\n\t"
        "ldr r5, [r5, #4]\n\t"
        "cmp r5, #0\n\t"
        "bne 3b\n\t"
    "9:\n\t"
        "mov r1, r8\n\t"
        "cmp r1, r6\n\t"
        "bge 2b\n\t"
        "ldr r1, [sp, #0x2c]\n\t"
        "ldr r5, [r1]\n\t"
        "cmp r5, #0\n\t"
        "beq 14f\n\t"
    "10:\n\t"
        "ldr r4, [r5]\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "mov r2, #0x30\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x34]\n\t"
        "add r1, sp, #0xc\n\t"
        "bl sub_803AD80\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 13f\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "lsr r0, r1, #2\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 13f\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x48\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "cmp r0, #4\n\t"
        "ble 13f\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "cmp sb, r0\n\t"
        "bne 12f\n\t"
        "add r0, sp, #0x1c\n\t"
        "add r1, sp, #0x50\n\t"
        "mov r2, #0x10\n\t"
        "bl sub_800014C\n\t"
        "str r4, [sp, #4]\n\t"
        "ldr r0, [sp, #0x28]\n\t"
        "str r0, [sp]\n\t"
        "ldr r1, [sp, #0x1c]\n\t"
        "ldr r2, [sp, #0x20]\n\t"
        "ldr r3, [sp, #0x24]\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_80096C0\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
    "11: .4byte gUnknown_030012D8\n"
    "12:\n\t"
        "add r0, sp, #0x1c\n\t"
        "add r1, sp, #0x50\n\t"
        "mov r2, #0x10\n\t"
        "bl sub_800014C\n\t"
        "str r4, [sp, #4]\n\t"
        "mov r0, sb\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [sp, #0x28]\n\t"
        "str r0, [sp]\n\t"
        "ldr r1, [sp, #0x1c]\n\t"
        "ldr r2, [sp, #0x20]\n\t"
        "ldr r3, [sp, #0x24]\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_80099F0\n\t"
    "13:\n\t"
        "ldr r5, [r5, #4]\n\t"
        "cmp r5, #0\n\t"
        "bne 10b\n\t"
    "14:\n\t"
        "add sp, #0x30\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r3}\n\t"
        "add sp, #0xc\n\t"
        "bx r3\n\t"
    );
}
asm(".align 2, 0");
