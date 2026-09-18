#include "core.h"
#include "actor.h"

extern s32 sub_8009FF4(void *part, void *region);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* `sub_8008AD8`'s sibling: resolves the same collision-hit logic when
 * the "compare viewport" doesn't match the current one (see
 * `sub_8008A40` above) - `otherViewport` here plays the role
 * `gUnknown_030012D8` (the player) plays in `sub_8008AD8`. Tests
 * `part` against the incoming box via `sub_8009FF4`; on a hit, fires
 * a `part->table+0x68`-driven trampoline (same "dead read" idiom as
 * `sub_8008AD8`) with `otherViewport->field_0A` as the third argument,
 * then sets `otherViewport->flags` bit 3. See the (now removed)
 * NON_MATCHING C draft in git history for the full commented C
 * reconstruction - same structural gap as `sub_8008AD8`/`sub_8008A40`
 * above stopped it closing (this compiler has no way to leave one
 * scalar parameter untouched in its own incoming stack slot while
 * still building a 4-word AABB pointer that includes it - see
 * docs/matching.md's "Parked, not matched: sub_8008D80"). Written as
 * NAKED asm here instead, same technique as the other functions in
 * `actor_part7.c`. Kept in its own translation unit (not appended to
 * `actor_part7.c`) since its real ROM address, 0x08008D80, isn't
 * adjacent to that file's other functions - `actor_part10.c`'s
 * `sub_8008C80`/`sub_8008CEC`/`sub_8008D30` sit between `sub_8008AD8`
 * and this function in ROM order (see docs/workflow.md step 4's "needs
 * its own new .c file" case). */
NAKED void sub_8008D80(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, void *partArg, void *otherViewportArg)
{
    asm(
        "sub sp, #0xc\n\t"
        "push {r4, r5, lr}\n\t"
        "str r1, [sp, #0xc]\n\t"
        "str r2, [sp, #0x10]\n\t"
        "str r3, [sp, #0x14]\n\t"
        "ldr r4, [sp, #0x1c]\n\t"
        "ldr r5, [sp, #0x20]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, sp, #0xc\n\t"
        "bl sub_8009FF4\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldrb r2, [r5, #0xa]\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #1\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "mov r0, #8\n\t"
        "ldrb r1, [r5, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r5, #0xc]\n\t"
    "1:\n\t"
        "pop {r4, r5}\n\t"
        "pop {r3}\n\t"
        "add sp, #0xc\n\t"
        "bx r3\n\t"
    );
}
asm(".align 2, 0");
