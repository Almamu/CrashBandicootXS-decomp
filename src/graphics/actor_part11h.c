#include "core.h"
#include "actor.h"

extern void *sub_803AD7C(void *arg0, void *fn);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void *gUnknown_03001308;

/* The spatial-hash-grid pool manager struct `sub_8008F20` initializes
 * and `actor_part12.c` operates on - see that file (and
 * `actor_part11.c`) for the full field writeup. */
struct pool_manager {
    s32 activeCount;
    s32 capacity;
    void **slotArray;
    void *nodeArray;
    void *gridHead[256];
    void *gridTail[256];
    void *freeListArray;
    void *freeListHead;
};

/* Same "extended screen box" filter shape as `sub_8008C80` (the plain
 * 240x160 GBA screen region, in Q8, at the `gUnknown_03001308`
 * sub-object's own position), but instead of filtering into a second
 * array, iterates `manager`'s spatial hash grid buckets directly (from
 * `baseIdx+2` down to `baseIdx` inclusive - a fixed 3-bucket window,
 * where `baseIdx` is the screen-box's own X position clamped to
 * non-negative - note this is NOT "down to 0": the ROM's own loop-end
 * compare is against `baseIdx` itself, not a literal 0) and, for every
 * node whose `table+0x30/0x34`-driven trampoline passes the box test,
 * fires its `table+0x20/0x24`-driven trampoline and marks it
 * (`node+0x11 = 1`) so the second pass - over the special "large
 * object" bucket 255 - knows to skip nodes already handled via their
 * primary bucket (clearing the mark instead) rather than
 * double-processing them, while still running the same
 * box-test-then-trampoline logic for any bucket-255 node that wasn't
 * already marked.
 *
 * Byte-matches. Getting there took three fixes past the first "obvious"
 * C transcription: (1) the outer bucket loop is a `do`/`while` (the ROM
 * never emits an upfront bounds check before the first iteration,
 * since `baseIdx+2 >= baseIdx` always holds - a `for` loop's redundant
 * entry test doesn't get optimized away by this compiler even though
 * it's always true), with the next-bucket index computed *before* the
 * inner per-node chain is walked (mirroring the ROM's own
 * `subs r6, r1, #1` placement); (2) the bucket-255 pass's `if` is
 * ordered "box-test-and-fire on the *unmarked* case, clear the mark on
 * the marked case" (`if (mark == 0) {...} else {mark = 0;}`), not the
 * reverse - this is what makes the compiler's own literal-pool dump
 * land at the same point in the instruction stream as the ROM's; (3)
 * each grid-bucket dereference (`table+0x30/0x34` for the box test,
 * then `table+0x20/0x24` for the fire) re-reads `part = *node` fresh
 * rather than reusing a cached value across the `sub_803AD80` call,
 * matching the ROM's own redundant reload. The one genuine compiler
 * gap that's left, even after all of the above: computing the grid
 * slot's address (`(u8 *)gridHeadBase + bucket*4`) as
 * `ADD Rd, Rbase, Roffset` (`adds r0, r7, r0`, base first) - this
 * compiler's natural array-indexing codegen instead emits
 * `ADD Rd, Roffset, Rbase` (`adds r0, r0, r7`, offset first), which
 * encodes to different bytes despite computing the same value. A tied
 * single-instruction `asm` (matching the ROM's own operand order
 * exactly, `"add %0, %1, %2"` with the base as `%1`) closes it without
 * disturbing anything else. */
void sub_800944C(void *managerArg)
{
    register struct pool_manager *manager asm("r3") = managerArg;
    s32 box[4];
    register void *P asm("r0") = gUnknown_03001308;
    register void *subObj asm("r2") = *(void **)((u8 *)P + 0x10);
    register s32 v0 asm("r1") = *(s32 *)subObj << 8;
    register s32 v1 asm("r0") = *(s32 *)((u8 *)subObj + 4) << 8;
    s32 v2, v3;
    register s32 baseIdx asm("r5");
    s32 bucket;

    box[0] = v0;
    box[1] = v1;
    v2 = 0xf0 << 8;
    v3 = 0xa0 << 8;
    box[2] = v2;
    box[3] = v3;

    baseIdx = *(s32 *)subObj >> 8;
    if (baseIdx < 0) {
        baseIdx = 0;
    }

    bucket = baseIdx + 2;
    {
        void **gridHeadBase = manager->gridHead;
        register void **gridHead255 asm("r8") = &manager->gridHead[255];

        do {
            s32 off = bucket << 2;
            u8 *slot;
            void *node;
            s32 nextBucket;

            /* Same value as `(u8 *)gridHeadBase + off`, but forces the
             * ROM's own base-then-offset operand order
             * (`adds r0, r7, r0`) instead of this compiler's natural
             * offset-then-base order (`adds r0, r0, r7`) - both encode
             * the identical addition, just as different bytes. */
            asm("add %0, %1, %2" : "=r"(slot) : "r"((u8 *)gridHeadBase), "r"(off));
            node = *(void **)slot;
            nextBucket = bucket - 1;

            if (node != 0) {
                do {
                    void *part = *(void **)node;
                    u8 *tbl = *(u8 **)((u8 *)part + 0x18);
                    s16 offset = *(s16 *)(tbl + 0x30);
                    void *addr = (u8 *)part + offset;
                    void *fn = *(void **)(tbl + 0x34);

                    if ((u8)sub_803AD80(addr, box, fn)) {
                        void *part2 = *(void **)node;
                        u8 *tbl2 = *(u8 **)((u8 *)part2 + 0x18);
                        s16 offset2 = *(s16 *)(tbl2 + 0x20);
                        void *addr2 = (u8 *)part2 + offset2;
                        void *fn2 = *(void **)(tbl2 + 0x24);

                        sub_803AD7C(addr2, fn2);
                        *((u8 *)node + 0x11) = 1;
                    }

                    node = *(void **)((u8 *)node + 4);
                } while (node != 0);
            }

            bucket = nextBucket;
        } while (bucket >= baseIdx);

        {
            void *node = *gridHead255;

            while (node != 0) {
                void *node2 = *(void **)((u8 *)node + 0xc);

                if (*((u8 *)node2 + 0x11) == 0) {
                    void *part = *(void **)node;
                    u8 *tbl = *(u8 **)((u8 *)part + 0x18);
                    s16 offset = *(s16 *)(tbl + 0x30);
                    void *addr = (u8 *)part + offset;
                    void *fn = *(void **)(tbl + 0x34);

                    if ((u8)sub_803AD80(addr, box, fn)) {
                        void *part2 = *(void **)node;
                        u8 *tbl2 = *(u8 **)((u8 *)part2 + 0x18);
                        s16 offset2 = *(s16 *)(tbl2 + 0x20);
                        void *addr2 = (u8 *)part2 + offset2;
                        void *fn2 = *(void **)(tbl2 + 0x24);

                        sub_803AD7C(addr2, fn2);
                    }
                } else {
                    *((u8 *)node2 + 0x11) = 0;
                }

                node = *(void **)((u8 *)node + 4);
            }
        }
    }
}

/* Matches the ROM's own trailing zero-fill padding out to the 4-byte
 * boundary that `sub_8009528` (`actor_part11f.c`, immediately following
 * in `ldscript.txt` link order) needs for its own alignment. Without this,
 * the linker bridges the same 2-byte gap with its default Thumb NOP
 * fill (`mov r8, r8` / `0x46c0`) instead of the ROM's zero bytes - same
 * "trailing byte-padding mismatch" fix already used elsewhere in this
 * project. */
asm(".align 2, 0");
