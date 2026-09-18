#include "core.h"
#include "memory.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Constructor for the small tracker object (`gUnknown_03001534`):
 * stashes the caller's own object into `gUnknown_03001564`, and - if
 * `sub_802973C` (a level-index/mode query) returns nonzero - clears
 * `gUnknown_0300157C`'s spawn-budget counter. Seeds the row/column
 * dimensions (`gUnknown_03001528`/`gUnknown_0300152C`) from
 * `gStaticData_08167CD4`'s first two halfwords, allocates the 0x1c-byte
 * tracker object, wires its event table (`gStaticData_0817C3E4`) and
 * part table (`gUnknown_03001580`) pointers plus a fixed `+0x18` flag,
 * registers it via `sub_803B0A8`, and stores it into
 * `gUnknown_03001534`. Resets both boss-weapon state globals
 * (`gUnknown_03001538`/`gUnknown_0300153C`) and fires the tracker's own
 * state-0/table-index-0 transition (anim frame from its own part-table
 * pointer at `+0`). Finishes by running `sub_8031504` once (the DMA/
 * tile-cache setup + palette fade, actor_part26b.c) and clearing
 * `gUnknown_03001524`'s "apply now" latch.
 *
 * Semantics are fully understood (every load/store, branch and call
 * accounted for above - the tail's state-0/table-index-0 transition is
 * the exact same idiom already matched via plain C in
 * `sub_80306AC`/actor_part21c.c), but a first plain-C attempt
 * compiled noticeably shorter/differently-structured code (this
 * compiler proved several of the ROM's own address/value reloads
 * redundant and CSE'd them away). Given the size of this constructor
 * and the number of independent register-order choices the ROM's own
 * ~50-instruction prologue makes, reproducing it register-for-register
 * wasn't pursued further this pass. Mechanical, byte-verified
 * transcription. */
extern s32 gUnknown_0300157C;
extern s32 gUnknown_03001528;
extern s32 gUnknown_0300152C;
extern void *gUnknown_03001534;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern u8 gUnknown_03001524;
extern void *gUnknown_03001564;

NAKED void sub_8030F88(void *selfArg)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "ldr r1, 1f\n\t"
        "str r0, [r1]\n\t"
        "bl sub_802973C\n\t"
        "add r1, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "bne 2f\n\t"
        "ldr r0, 3f\n\t"
        "str r1, [r0]\n\t"
    "2:\n\t"
        "ldr r1, 4f\n\t"
        "ldr r2, 5f\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r2, r3]\n\t"
        "str r0, [r1]\n\t"
        "ldr r1, 6f\n\t"
        "mov r3, #2\n\t"
        "ldrsh r0, [r2, r3]\n\t"
        "str r0, [r1]\n\t"
        "ldr r4, 7f\n\t"
        "mov r0, #0x1c\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #0x18\n\t"
        "bl mem_alloc\n\t"
        "add r5, r0, #0\n\t"
        "ldr r0, 8f\n\t"
        "ldr r1, 9f\n\t"
        "mov r2, #1\n\t"
        "str r0, [r5]\n\t"
        "str r1, [r5, #4]\n\t"
        "str r2, [r5, #0x18]\n\t"
        "add r0, r5, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_803B0A8\n\t"
        "str r5, [r4]\n\t"
        "mov r4, #0\n\t"
        "ldr r0, 10f\n\t"
        "str r4, [r0]\n\t"
        "ldr r0, 11f\n\t"
        "str r4, [r0]\n\t"
        "str r4, [r5, #0xc]\n\t"
        "ldr r0, [r5]\n\t"
        "ldrh r0, [r0]\n\t"
        "mov r6, #0\n\t"
        "strh r0, [r5, #0x10]\n\t"
        "strb r6, [r5, #0x12]\n\t"
        "add r0, r5, #0\n\t"
        "bl GetAnimFrameBaseOffset\n\t"
        "ldr r2, [r5, #0xc]\n\t"
        "ldr r3, [r5]\n\t"
        "lsl r1, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r1, r3\n\t"
        "mov r2, #4\n\t"
        "ldrsh r1, [r1, r2]\n\t"
        "cmp r0, r1\n\t"
        "blt 12f\n\t"
        "str r4, [r5, #8]\n\t"
    "12:\n\t"
        "bl sub_8031504\n\t"
        "ldr r0, 13f\n\t"
        "strb r6, [r0]\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001564\n"
    "3: .4byte gUnknown_0300157C\n"
    "4: .4byte gUnknown_03001528\n"
    "5: .4byte gStaticData_08167CD4\n"
    "6: .4byte gUnknown_0300152C\n"
    "7: .4byte gUnknown_03001534\n"
    "8: .4byte gStaticData_0817C3E4\n"
    "9: .4byte gUnknown_03001580\n"
    "10: .4byte gUnknown_03001538\n"
    "11: .4byte gUnknown_0300153C\n"
    "13: .4byte gUnknown_03001524\n"
    );
}
