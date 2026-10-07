#include "core.h"
#include "match.h"
#include "actor.h"
#include "box_part.h"
#include "crates.h"
#include "globals.h"
#include "level.h"

extern void *_call_via_r1(void *arg0, void *fn);
extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);

/* The spatial-hash-grid pool manager struct `InitCrateList` initializes
 * and `crate_list.c` operates on - see that file (and
 * `part_list.cpp`) for the full field writeup. */
/* Same "extended screen box" filter shape as `CullPartList` (the plain
 * 240x160 GBA screen region, in Q8, at `gLevelLayers->layer0`'s
 * scroll position), but instead of filtering into a second
 * array, iterates `manager`'s spatial hash grid buckets directly (from
 * `baseIdx+2` down to `baseIdx` inclusive - a fixed 3-bucket window,
 * where `baseIdx` is the screen-box's own X position clamped to
 * non-negative - note this is NOT "down to 0": the ROM's own loop-end
 * compare is against `baseIdx` itself, not a literal 0) and, for every
 * node whose `table+0x30/0x34`-driven trampoline passes the box test,
 * fires its `table+0x20/0x24`-driven trampoline and marks it
 * (`node->mark2 = 1`) so the second pass - over the special "large
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
 * rather than reusing a cached value across the `_call_via_r2` call,
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
void DrawCrateList(struct pool_manager *managerArg)
{
    MATCH_HOLD_REG(struct pool_manager *, manager, r3) = managerArg;
    s32 box[4];
    MATCH_HOLD_REG(struct level_layers *, P, r0) = gLevelLayers;
    MATCH_HOLD_REG(struct bg_scroll_layer *, subObj, r2) = P->layer0;
    MATCH_HOLD_REG(s32, v0, r1) = INT_TO_Q8(subObj->x);
    MATCH_HOLD_REG(s32, v1, r0) = INT_TO_Q8(subObj->y);
    s32 v2, v3;
    MATCH_HOLD_REG(s32, baseIdx, r5);
    s32 bucket;

    box[0] = v0;
    box[1] = v1;
    v2 = INT_TO_Q8(0xf0);
    v3 = INT_TO_Q8(0xa0);
    box[2] = v2;
    box[3] = v3;

    baseIdx = subObj->x >> 8;
    LIMIT_MIN(baseIdx, 0);

    bucket = baseIdx + 2;
    {
        void **gridHeadBase = (void **)manager->gridHead;
        MATCH_HOLD_REG(struct pool_node **, gridHead255, r8) = &manager->gridHead[255];

        do {
            s32 off = bucket << 2;
            u8 *slot;
            struct pool_node *node;
            s32 nextBucket;

            /* Same value as `(u8 *)gridHeadBase + off`, but forces the
             * ROM's own base-then-offset operand order
             * (`adds r0, r7, r0`) instead of this compiler's natural
             * offset-then-base order (`adds r0, r0, r7`) - both encode
             * the identical addition, just as different bytes. */
            asm("add %0, %1, %2" : "=r"(slot) : "r"((u8 *)gridHeadBase), "r"(off));
            node = *(struct pool_node **)slot;
            nextBucket = bucket - 1;

            if (node != 0) {
                do {
                    struct box_part *part = node->data;
                    struct part_method *tbl = PART_METHOD(part, 0x30);
                    s16 offset = tbl->thisOffset;
                    void *addr = (u8 *)part + offset;
                    void *fn = tbl->fn;

                    if ((u8)_call_via_r2(addr, box, fn)) {
                        struct box_part *part2 = node->data;
                        struct part_method *tbl2 = PART_METHOD(part2, 0x20);
                        s16 offset2 = tbl2->thisOffset;
                        void *addr2 = (u8 *)part2 + offset2;
                        void *fn2 = tbl2->fn;

                        _call_via_r1(addr2, fn2);
                        node->mark2 = 1;
                    }

                    node = node->next;
                } while (node != 0);
            }

            bucket = nextBucket;
        } while (bucket >= baseIdx);

        {
            struct pool_node *node = *gridHead255;

            while (node != 0) {
                struct pool_node *node2 = node->link;

                if (node2->mark2 == 0) {
                    struct box_part *part = node->data;
                    struct part_method *tbl = PART_METHOD(part, 0x30);
                    s16 offset = tbl->thisOffset;
                    void *addr = (u8 *)part + offset;
                    void *fn = tbl->fn;

                    if ((u8)_call_via_r2(addr, box, fn)) {
                        struct box_part *part2 = node->data;
                        struct part_method *tbl2 = PART_METHOD(part2, 0x20);
                        s16 offset2 = tbl2->thisOffset;
                        void *addr2 = (u8 *)part2 + offset2;
                        void *fn2 = tbl2->fn;

                        _call_via_r1(addr2, fn2);
                    }
                } else {
                    node2->mark2 = 0;
                }

                node = node->next;
            }
        }
    }
}
