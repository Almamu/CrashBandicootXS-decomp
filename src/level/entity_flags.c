#include "core.h"
#include "match.h"
#include "memory.h"
#include "level.h"

/* GitHub issue #41: 0x08025894-0x08025FC8. Counts, across every group
 * in `list` and every entity in each group,
 * how many items have an "effective type" that falls in
 * `[0x15, 0x27]` but is not one of `{0x18, 0x1a, 0x1b, 0x1c, 0x1d}`
 * (the ROM's jump table sends those five cases to the no-increment
 * path, everything else in range to the increment path, anything
 * outside the range skips the table read entirely via the `bhi`
 * short-circuit). An item's `type` is used directly unless it's `0x1a`,
 * in which case the effective type is instead looked up indirectly:
 * `list->paramOffsets[item->param]` gives a byte offset into
 * `list->params`, and the effective type is the `s16` eight bytes past
 * that.
 *
 * The `item->type == 0x1a` lookup does its whole four-load chain
 * using only `r0`/`r1` as scratch in the ROM, aggressively overwriting
 * each value the instant it's dead (item's own address is destroyed
 * by the very read that uses it, the table address is destroyed by
 * the read that dereferences it, and so on) - every plain-C shape
 * tried here kept at least one of those values alive in a third
 * register, colliding with the outer loop's `i` counter (pinned to
 * `r2` by the surrounding loop structure) and forcing an extra `r7`
 * push/pop the ROM does not have. Matched by emitting that one block
 * as an opaque `asm volatile` computing the effective type directly
 * from `l`/`item`, with `r0`/`r1` named explicitly in the asm text -
 * this keeps the block's own internal register churn invisible to the
 * surrounding function-level allocator, so `i` stays cleanly in `r2`
 * and the `r7` push/pop disappears. Splitting `i`'s own init
 * (`l->groupCount` then `- 1`) into two statements was also needed:
 * as one combined expression this compiler loads the count into a
 * scratch register before subtracting into `i`'s register, instead of
 * the ROM's direct load-then-decrement-in-place into the same
 * register - see docs/matching/archive/issue-41-game-loop-25894.md. */
s32 CountCrateEntities(void *self, const struct level_entity_list *list)
{
    const struct level_entity_list *l = list;
    s32 count = 0;
    s32 i;

    i = l->groupCount;
    i -= 1;

    for (; i >= 0; i--) {
        const struct level_entity_group *group = &l->groups[i];
        s32 j;

        for (j = 0; j < group->count; j++) {
            const struct level_entity *item = &group->entities[j];
            s32 type = item->type;

            if (type == 0x1a) {
                MATCH_HOLD_REG(const void *, itemReg, r1) = item;
                MATCH_HOLD_REG(s32, result, r0);

                // clang-format off
                asm volatile (
                    "ldr r0, [%1, #8]\n\t"
                    "ldrh r1, [r1, #6]\n\t"
                    "lsl r1, r1, #1\n\t"
                    "add r1, r1, r0\n\t"
                    "ldr r0, [%1, #0xc]\n\t"
                    "ldrh r1, [r1]\n\t"
                    "add r0, r1, r0\n\t"
                    "mov r1, #8\n\t"
                    "ldrsh r0, [r0, r1]\n\t"
                    : "=r" (result)
                    : "r" (l), "r" (itemReg)
                );
                // clang-format on
                type = result;
            }

            // clang-format off
            switch (type) {
            case 0x18: case 0x1a: case 0x1b: case 0x1c: case 0x1d:
                break;
            case 0x15: case 0x16: case 0x17: case 0x19:
            case 0x1e: case 0x1f: case 0x20: case 0x21: case 0x22:
            case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
                count++;
                break;
            }
            // clang-format on
        }
    }
    return count;
}
asm(".align 2, 0");

/* The bit accessors of struct entity_flags (level.h), by entity id `n`:
 * `bits0` is the committed "gone" set (collected, broken or killed;
 * SpawnRoomEntities skips those) and `bits1` the committed "activated"
 * set (a checkpoint, life, "?" or slot crate opened, an iron switch
 * crate pressed and the outline crates it made solid; CreateCrate builds
 * those in their used state). `bits0Copy`/`bits1Copy` are the live copies, committed back at a
 * checkpoint: "Set" writes the committed set and "Mark" only the live
 * copy (like MarkEntityGone's bits0Copy write).
 *
 * UNUSED - no caller anywhere in the ROM (checked src/ and asm/). Sets
 * bit `n` of `bits0` (floor-divided into a 32-bit-word row, same idiom as
 * `SetBitmapBit` in collision_map.c). */
void SetEntityIdGone(void *self, s32 n)
{
    u8 *base = (u8 *)self;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 8;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    *word |= mask;
}

/* Tests bit `n` of `bits0`, the committed "gone" set `SetEntityIdGone`
 * sets. */
s32 IsEntityIdGone(void *self, s32 n)
{
    u8 *base = (u8 *)self;
    s32 result = 0;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 8;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    if (*word & mask) {
        result = 1;
    }
    return result;
}

/* Tests bit `n` of `bits1` (`self+0x208`), the committed "activated"
 * set. */
s32 IsEntityIdActivated(void *self, s32 n)
{
    u8 *base = (u8 *)self;
    s32 result = 0;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 0x208;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    if (*word & mask) {
        result = 1;
    }
    return result;
}

asm(".align 2, 0");

/* Sets bit `n` in *both* `bits1` (`self+0x208`) and `bits1Copy`
 * (`self+0x308`): the activation is committed at once, so it survives a
 * death before the next checkpoint (the checkpoint, life, "?" and slot
 * crates).
 *
 * Was NAKED asm, not plain C - see
 * docs/matching/archive/naked-sub_80259d4-matched.md for the derivation of how
 * this was finally matched. This is a true leaf function in the ROM
 * (no `push`/`pop` at all - `self` lives in `ip`/`r12` for the whole
 * function). The gap: the ROM does `mov ip, r0` (stash `self`) before
 * `adds r2, r1, #0` (copy `n` into its own working register `t`), and
 * this compiler always emits the `n`-copy first regardless of C
 * source order - fixed by materializing both moves as one opaque
 * inline-asm block. The ROM also keeps `n`'s pristine copy (`t`, r2)
 * untouched by the "clamp negative indices" adjustment (which lands in
 * a *separate* register, r0), reusing the untouched `t` again later
 * for `bitIndex` - a second local (`adjusted`) instead of adjusting
 * `t` in place reproduces that split. */
void SetEntityIdActivated(void *self, s32 n)
{
    MATCH_HOLD_REG(u8 *, base, ip);
    MATCH_HOLD_REG(s32, t, r2);
    s32 adjusted, wordIndex;
    MATCH_HOLD_REG(s32, bitIndex, r0);
    MATCH_HOLD_REG(s32, mask, r2);
    MATCH_HOLD_REG(s32, shifted, r3);
    MATCH_HOLD_REG(s32, addr, r1);

    asm volatile("mov %0, %2\n\tadd %1, %3, #0" : "=r"(base), "=r"(t) : "r"(self), "r"(n));

    adjusted = t;
    if (t < 0) {
        adjusted += 0x1f;
    }
    wordIndex = adjusted >> 5;
    shifted = wordIndex << 2;

    addr = 0x208;
    addr += (s32)base;
    addr += shifted;
    bitIndex = t - (wordIndex << 5);
    mask = 1 << bitIndex;
    *(s32 *)addr |= mask;

    addr = 0x308;
    addr += (s32)base;
    addr += shifted;
    *(s32 *)addr |= mask;
}

/* Sets bit `n` of `bits1Copy` (`self+0x308`) only: the live "activated"
 * set, committed at the next checkpoint (ActivateIronSwitchCrate: the
 * switch itself and the outline crates it makes solid). */
void MarkEntityIdActivated(void *self, s32 n)
{
    u8 *base = (u8 *)self;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 0x308;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    *word |= mask;
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/). Stores
 * the Q8 `val` as pixels in `pos` (`self+4`), as SpawnRoomEntities does. */
void SetEntityFlagsPos(void *self, s32 val)
{
    ((struct entity_flags *)self)->pos = val >> 8;
}

/* If bit 0 of `flags` is set, forwards to `OperatorDelete` - same
 * conditional-destroy shape as entity_spawner.c's
 * near-identical `DestroyEntitySpawnerObj`. */
void DestroyEntityFlags(void *self, s32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Clears `list` and `pos`. */
void *InitEntityFlags(void *self)
{
    ((struct entity_flags *)self)->list = NULL;
    ((struct entity_flags *)self)->pos = 0;
    return self;
}
