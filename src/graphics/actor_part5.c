#include "core.h"
#include "actor.h"

extern s32 sub_8007114(struct actor *self, void *box);

/* `part+0x25 == 1` is the same fast override seen in
 * sub_8007F78/sub_8007FD8; otherwise defers to `sub_8007114` (already
 * matched in graphics.c), forwarding `box` straight through
 * unmodified. */
s32 sub_8008304(struct actor *part, void *box)
{
    s32 result = 0;
    register u8 *addr asm("r2") = (u8 *)part + 0x25;
    register u8 byteVal asm("r2");

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if ((u8)sub_8007114(part, box)) {
        result = 1;
    }
    return result;
}

extern u8 sub_8006FE4(struct actor *self);

/* Same `part+0x25` fast-override shape as `sub_8008304` above,
 * deferring to `sub_8006FE4` (already matched in `graphics.c`)
 * instead - a single-argument sibling, so the address scratch
 * naturally lands in `r1` instead of `r2` (no second call argument to
 * keep out of the way). */
s32 sub_8008328(struct actor *part)
{
    s32 result = 0;
    register u8 *addr asm("r1") = (u8 *)part + 0x25;
    register u8 byteVal asm("r1");

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if (sub_8006FE4(part)) {
        result = 1;
    }
    return result;
}

/* Always-true stub. */
s32 sub_800834C(void)
{
    return 1;
}

extern void sub_8007A84(void *self, void *part);
extern void *gUnknown_030012CC;

/* Tail-calls `sub_8007A84` (already matched in `actor_part.c`) with
 * the global `gUnknown_030012CC` as `self`. */
void sub_8008350(void *part)
{
    sub_8007A84(gUnknown_030012CC, part);
}

extern void sub_8008044(struct actor *part);
extern void *sub_803AD7C(void *arg0, void *arg1);

/* Advances `part`'s animation timer (`sub_8008044`), then resolves two
 * `table+N`/`table+N+4` offset/pointer slot pairs (the same convention
 * documented for `sub_8006FE4`/`sub_8007F78`) into `sub_803AD7C` calls
 * - table+0x60/+0x64 first, then table+8/+0xc. */
void sub_8008364(struct actor *part)
{
    sub_8008044(part);

    {
        void *table = part->table;
        void *slot = (u8 *)table + 0x60;
        s32 offset = *(s16 *)slot;
        void *addr = (u8 *)part + offset;
        void *ptr = *(void **)((u8 *)slot + 4);

        sub_803AD7C(addr, ptr);
    }
    {
        void *table = part->table;
        s32 offset = *(s16 *)((u8 *)table + 8);
        void *addr = (u8 *)part + offset;
        void *ptr = *(void **)((u8 *)table + 0xc);

        sub_803AD7C(addr, ptr);
    }
}

/* Returns a pointer to `part`'s current keyframe record's `+4` field -
 * the same keyframe-table lookup used throughout this ROM region. */
void *sub_8008394(struct actor *part)
{
    register void **tablePtr asm("r2") = *(void ***)((u8 *)part + 0x20);
    register u8 idx asm("r3") = *((u8 *)part + 0x2d);
    s32 offset = idx * 0x1c;
    void *table = *tablePtr;
    void *rec = (u8 *)table + offset;
    return (u8 *)rec + 4;
}

extern void *gUnknown_030012D0;

/* Ignores its `part` argument entirely (the ROM never reads r0 before
 * overwriting it) - already declared with this signature at its
 * `sub_80073DC` call site in graphics.c. Returns
 * `(*(void **)gUnknown_030012D0)+4`'s value. */
s32 sub_80083A8(void *part)
{
    void *p2 = *(void **)gUnknown_030012D0;
    return *(s32 *)((u8 *)p2 + 4);
}

/* Looks up `part`'s current keyframe record (same convention as
 * elsewhere in this ROM region). If `part+0x38` ("done", set by
 * `sub_8008044`) is set and the record's `+0x17` flags byte bit 1 is
 * clear (not looping), clamps `part`'s frame index (`+0x30`) to the
 * last frame (`record+0x16 - 1`) and resets the sub-counter
 * (`+0x34`) to the record's duration (`record+0x15`). Either way,
 * then resolves a final pointer: the record's own `+0` field is
 * itself a pointer (`recPtr`) to a per-frame `u16` array, indexed by
 * the (possibly just-clamped) frame index; that `u16` in turn indexes
 * a pointer array at `table+4`, and the result is that array's
 * pointer at the looked-up index.
 *
 * Matched in a later session than the original NAKED transcription -
 * see docs/matching.md's "Parked, not matched: sub_80083B8" for the
 * original account. Unlike `sub_8008188`/`sub_8008200`/`sub_8008278`'s
 * shared-switch-case gap, this function's resistant
 * `adds r0, r1, r0`-vs-`adds r0, r0, r1` add sits in genuinely
 * straight-line code (no switch, no case merging to protect), so a
 * plain inline-asm anchor on just that one instruction - the same
 * technique that had backfired inside those functions' shared case
 * blocks - works here with no caveats. Needed a trailing
 * `asm(".align 2, 0")` since it's the last function in this file (the
 * ROM has 2 bytes of zero padding here before `sub_8008408` in
 * `actor_part6.c`, and a plain compiled function's own natural
 * alignment produces a `0x46c0` nop-fill instead - the standard
 * `matching_decomp_alignment_fix` gotcha). */
void *sub_80083B8(struct actor *part)
{
    register void *rec asm("r1") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r2") = (u8 *)part + 0x2d;
    register u8 idx asm("r4") = *idxAddr;
    s32 offset = idx * 0x1c;

    rec = *(void **)rec;
    rec = (u8 *)rec + offset;

    if (*((u8 *)part + 0x38) != 0) {
        register s32 mask asm("r0") = 2;
        register s32 flags asm("r2") = *((u8 *)rec + 0x17);
        register s32 test asm("r0");

        test = mask & flags;
        if (!test) {
            *(s32 *)((u8 *)part + 0x30) = *((u8 *)rec + 0x16) - 1;
            *(s32 *)((u8 *)part + 0x34) = *((u8 *)rec + 0x15);
        }
    }

    {
        void **tablePtr2 = *(void ***)((u8 *)part + 0x20);
        s32 frameIdx = *(s32 *)((u8 *)part + 0x30);
        register void *recPtr asm("r1") = *(void **)rec;
        register s32 byteOffset asm("r0") = frameIdx * 2;
        register u16 *arr asm("r0");
        register void **ptrArray asm("r1");
        u16 idx2;

        /* The ROM's `adds r0, r0, r1` (byteOffset-then-recPtr operand
         * order) versus this compiler's always-canonicalized
         * `adds r0, r1, r0` - see the doc comment above. */
        asm volatile(
            "add r0, r0, r1\n\t"
            : "=r"(arr)
            : "0"(byteOffset), "r"(recPtr)
        );

        ptrArray = *(void ***)((u8 *)tablePtr2 + 4);
        idx2 = *arr;
        return ptrArray[idx2];
    }
}
asm(".align 2, 0");
