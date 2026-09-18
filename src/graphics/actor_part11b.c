#include "core.h"
#include "actor.h"

/* The spatial-hash-grid removal primitive `sub_8009A30`/`sub_8009AA0`
 * (`actor_part12.c`) call before compacting `manager`'s active-object
 * array - unlinks `item`'s pool node(s) from `manager`'s grid
 * (the `struct pool_manager` documented in `actor_part12.c`) and
 * returns them to the free list.
 *
 * Two-phase search, matching the two places `sub_8009B3C` (insert side)
 * can register an object:
 *  - Phase 1 walks `item`'s own *primary* bucket
 *    (`gridHead[*(s16 *)(item+2)]`, the same bucket index `sub_8009B3C`
 *    computes on insert) looking for a node whose data pointer equals
 *    `item`; on the first match, unlinks it from that bucket's
 *    singly-linked list (head/tail/prev-next patched as needed, mirroring
 *    `sub_8009AF0`'s own insert-side bookkeeping) and pushes the node's
 *    wrapper entry back onto `manager->freeListHead`.
 *  - Phase 2 always runs unless phase 1 found a match *and* neither of
 *    two escape conditions hold (`*(u16 *)(item+8) == 0xFFFF`, or
 *    `item`'s flags byte bit 4 - the "large object" flag `sub_8009150`/
 *    `sub_8009B3C` also test - is clear): it then walks every bucket
 *    from 255 down to 0 (the special "large object" bucket first,
 *    matching where `sub_8009B3C` links a second node for large objects),
 *    removing any further node matching `item` the same way, but stops
 *    scanning deeper into a bucket's list once the combined removal
 *    count across both phases exceeds 1 (an object can only ever be
 *    registered in at most two buckets - its primary one and, for large
 *    objects, bucket 255).
 *
 * Written as NAKED asm, not plain C: real-C attempts following this
 * shape reproduce every branch and field access correctly, but one
 * detail of phase 2's "found a second match, stop searching this
 * bucket's list" path doesn't translate - after unlinking the node, the
 * ROM discards the just-used bucket-index scratch register entirely
 * (reloading it with an unrelated literal, `0x100`, immediately before
 * the `bucket--` that terminates the *outer* loop rather than the
 * inner one) instead of preserving the bucket index across the
 * `removedCount > 1` test the way any C-level loop naturally would.
 * This is the same category of "gcc keeps a value the ROM discards, or
 * vice versa" gap already documented for this cluster's siblings
 * (`sub_8008F20`, `sub_8009150`, `sub_800944C` in `actor_part11.c`) -
 * parked as a direct asm transcription instead of chasing a single-
 * instruction register-reuse quirk. Every load, store, and branch below
 * is a byte-for-byte transcription of the ROM's own confirmed-correct
 * instructions. */
NAKED void sub_8009008(void *manager, void *item)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "mov r8, r0\n\t"
        "mov sl, r1\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "lsl r1, r2, #2\n\t"
        "mov r0, r8\n\t"
        "add r0, #0x10\n\t"
        "add r0, r0, r1\n\t"
        "ldr r5, [r0]\n\t"
        "mov r6, #0\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp]\n\t"
        "cmp r5, #0\n\t"
        "beq 8f\n\t"
        "mov r4, #0x82\n\t"
        "lsl r4, r4, #3\n\t"
        "add r4, r8\n\t"
        "ldr r7, 2f\n\t"
        "add r7, r8\n\t"
    "1:\n\t"
        "ldr r0, [r5]\n\t"
        "cmp r0, sl\n\t"
        "bne 7f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r5]\n\t"
        "ldr r0, [sp]\n\t"
        "add r0, #1\n\t"
        "str r0, [sp]\n\t"
        "lsl r1, r2, #2\n\t"
        "mov r0, r8\n\t"
        "add r0, #0x10\n\t"
        "add r3, r0, r1\n\t"
        "ldr r0, [r3]\n\t"
        "cmp r5, r0\n\t"
        "bne 4f\n\t"
        "ldr r2, [r5, #4]\n\t"
        "cmp r2, #0\n\t"
        "beq 3f\n\t"
        "str r2, [r3]\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "2: .4byte 0x00000814\n"
    "3:\n\t"
        "str r2, [r3]\n\t"
        "add r0, r4, r1\n\t"
        "str r2, [r0]\n\t"
        "b 6f\n\t"
    "4:\n\t"
        "cmp r6, #0\n\t"
        "beq 6f\n\t"
        "add r1, r4, r1\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r5, r0\n\t"
        "bne 5f\n\t"
        "str r6, [r1]\n\t"
    "5:\n\t"
        "ldr r0, [r5, #4]\n\t"
        "str r0, [r6, #4]\n\t"
    "6:\n\t"
        "ldr r1, [r7]\n\t"
        "ldr r0, [r5, #8]\n\t"
        "str r1, [r0, #4]\n\t"
        "ldr r0, [r5, #8]\n\t"
        "str r0, [r7]\n\t"
        "b 8f\n\t"
    "7:\n\t"
        "add r6, r5, #0\n\t"
        "ldr r5, [r6, #4]\n\t"
        "cmp r5, #0\n\t"
        "bne 1b\n\t"
    "8:\n\t"
        "mov r2, sl\n\t"
        "ldrh r1, [r2, #8]\n\t"
        "cmp r5, #0\n\t"
        "beq 11f\n\t"
        "ldr r0, 9f\n\t"
        "cmp r1, r0\n\t"
        "beq 11f\n\t"
        "ldrb r1, [r2, #0xc]\n\t"
        "lsr r0, r1, #4\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 20f\n\t"
    "11:\n\t"
        "mov r1, #0xff\n\t"
        "mov r2, #0x10\n\t"
        "add r2, r8\n\t"
        "mov sb, r2\n\t"
    "12:\n\t"
        "mov r6, #0\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, sb\n\t"
        "ldr r3, [r0]\n\t"
        "cmp r3, #0\n\t"
        "beq 19f\n\t"
        "mov r0, #0x82\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r8\n\t"
        "mov ip, r0\n\t"
        "ldr r7, 10f\n\t"
        "add r7, r8\n\t"
    "13:\n\t"
        "ldr r0, [r3]\n\t"
        "cmp r0, sl\n\t"
        "bne 18f\n\t"
        "add r5, r3, #0\n\t"
        "mov r0, #0\n\t"
        "str r0, [r3]\n\t"
        "ldr r2, [sp]\n\t"
        "add r2, #1\n\t"
        "str r2, [sp]\n\t"
        "lsl r4, r1, #2\n\t"
        "mov r0, sb\n\t"
        "add r2, r0, r4\n\t"
        "ldr r0, [r2]\n\t"
        "cmp r3, r0\n\t"
        "bne 15f\n\t"
        "ldr r1, [r3, #4]\n\t"
        "cmp r1, #0\n\t"
        "beq 14f\n\t"
        "str r1, [r2]\n\t"
        "b 17f\n\t"
        ".align 2, 0\n"
    "9: .4byte 0x0000FFFF\n"
    "10: .4byte 0x00000814\n"
    "14:\n\t"
        "str r1, [r2]\n\t"
        "mov r2, ip\n\t"
        "add r0, r2, r4\n\t"
        "str r1, [r0]\n\t"
        "b 17f\n\t"
    "15:\n\t"
        "cmp r6, #0\n\t"
        "beq 17f\n\t"
        "mov r0, ip\n\t"
        "add r1, r0, r4\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r3, r0\n\t"
        "bne 16f\n\t"
        "str r6, [r1]\n\t"
    "16:\n\t"
        "ldr r0, [r3, #4]\n\t"
        "str r0, [r6, #4]\n\t"
    "17:\n\t"
        "ldr r1, [r7]\n\t"
        "ldr r0, [r5, #8]\n\t"
        "str r1, [r0, #4]\n\t"
        "ldr r0, [r5, #8]\n\t"
        "str r0, [r7]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #1\n\t"
        "ldr r2, [sp]\n\t"
        "cmp r2, #1\n\t"
        "bgt 19f\n\t"
    "18:\n\t"
        "add r6, r3, #0\n\t"
        "ldr r3, [r3, #4]\n\t"
        "cmp r3, #0\n\t"
        "bne 13b\n\t"
    "19:\n\t"
        "sub r1, #1\n\t"
        "cmp r1, #0\n\t"
        "bge 12b\n\t"
    "20:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
asm(".align 2, 0");
