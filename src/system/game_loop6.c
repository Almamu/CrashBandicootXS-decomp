#include "core.h"
#include "actor.h"

extern void *gUnknown_030012D8;
extern void *gUnknown_030012B4;
extern void *gUnknown_030012BC;
extern void *gUnknown_03001318;

extern s32 sub_8023414(void *self);
extern void sub_80232EC(void *self);
extern void sub_80232FC(void *self);
extern void sub_8023298(void *self);
extern void sub_8023288(void *self);
extern u8 sub_8023290(void *self);
extern u8 sub_80232B8(void *self);
extern void sub_80231D4(void *self);
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void sub_803A94C(const void *src, void *dst, u32 cnt);
extern void sub_8007398(struct actor *self, s32 arg1, s32 arg2);
extern void sub_8028568(void *state, s32 arg1);
extern void sub_8022CA0(void *self, u8 arg1);
extern void sub_8022468(void *self, s32 mode);
extern void sub_80019A8(struct AudioContext *self, u32 id);
extern void *sub_8026EDC(s32 size);
extern void nullsub_7(void);
extern s32 sub_80361B0(void);
extern void sub_8037154(void *self, u32 flags);

/* Sets `self->0x1bc` (a Q-format camera/position field paired with the
 * `sub_8023500` two-word setter below). */
void sub_80234E8(void *self, s32 value)
{
    *(s32 *)((u8 *)self + 0x1bc) = value;
}

/* Sets `self->0x1b8`, the companion field to `sub_80234E8` above. */
void sub_80234F4(void *self, s32 value)
{
    *(s32 *)((u8 *)self + 0x1b8) = value;
}

/* Copies a `{x, y}` pair into `self->0x1c0`/`0x1c4`. */
void sub_8023500(void *self, s32 *point)
{
    s32 *dst = (s32 *)((u8 *)self + 0x1c0);
    s32 y = point[1];
    s32 x = point[0];

    dst[0] = x;
    dst[1] = y;
}

/* Sets `self->0xa6` to 1 unless `sub_8023290` says it's already set. */
void sub_8023510(void *self)
{
    if (!sub_8023290(self)) {
        *((u8 *)self + 0xa6) = 1;
    }
}

/* Sets `self->0xa4` to 1 unless `sub_80232B8` says it's already set. */
void sub_802352C(void *self)
{
    if (!sub_80232B8(self)) {
        *((u8 *)self + 0xa4) = 1;
    }
}

/* Restores `self->0x70`/`0xa9` from their `0xcc`/`0xd0` shadow copies,
 * then copies the `0xe4`-`0x14b` snapshot block back over `self`'s own
 * first `0x68` bytes - the inverse direction of `sub_802356C`/
 * `sub_8022CA0`'s "stash a snapshot at +0xe4" below. */
void sub_8023548(void *self)
{
    u8 tmp;

    *(s32 *)((u8 *)self + 0x70) = *(s32 *)((u8 *)self + 0xcc);
    tmp = *((u8 *)self + 0xd0);
    *((u8 *)self + 0xa9) = tmp;
    sub_800014C(self, (u8 *)self + 0xe4, 0x68);
}

/* Re-arms a level/checkpoint transition: stores `flag` at `self->0xe0`,
 * refreshes the `0xcc`/`0xd0` shadow pair, resets the `sub_8023414`
 * animation-state pair (`sub_80232FC`/`sub_80232EC`), snapshots `pair`
 * into `self->0xd4`/`0xd8`, flushes two spans of the
 * `gUnknown_030012B4` bitmap via the `CpuSet` wrapper, then stashes the
 * `0xe4`-byte snapshot block (see `sub_8023548` above). */
void sub_802356C(void *selfArg, u8 flag, s32 *pairArg)
{
    register void *self asm("r5") = selfArg;
    register s32 *pair asm("r4") = pairArg;
    u8 tmp;

    *((u8 *)self + 0xe0) = flag;
    *(s32 *)((u8 *)self + 0xcc) = sub_8023414(self);
    tmp = *((u8 *)self + 0xa9);
    *((u8 *)self + 0xd0) = tmp;
    sub_80232FC(self);
    sub_80232EC(self);
    {
        register s32 *dst asm("r2") = (s32 *)((u8 *)self + 0xd4);
        register s32 px asm("r0") = pair[0];
        register s32 py asm("r1") = pair[1];
        dst[0] = px;
        dst[1] = py;
    }

    pair = gUnknown_030012B4;
    {
        void *a = (u8 *)pair + 0x108;
        void *b = (u8 *)pair + 8;
        register u32 ctrl asm("r2") = 0x04000040;
        sub_803A94C(a, b, ctrl);
    }
    {
        void *a = (u8 *)pair + 0x308;
        void *b = (u8 *)pair + 0x208;
        register u32 ctrl asm("r2") = 0x04000040;
        sub_803A94C(a, b, ctrl);
    }

    sub_800014C((u8 *)self + 0xe4, self, 0x68);
}

/* When `flag` is set, accumulates `self->0xb4` into `self->0x70`,
 * refreshes the animation-state pair, flushes the tile record cache
 * (`gUnknown_03001318`) using `self->0xbc`, re-syncs the player's
 * stored position (`gUnknown_030012D8`) from `self->0xd4`/`0xd8`, and
 * re-runs `sub_8022CA0`; otherwise just calls `sub_80231D4`. */
void sub_80235E4(void *self, u8 flag)
{
    if (flag != 0) {
        *(s32 *)((u8 *)self + 0x70) += *(s32 *)((u8 *)self + 0xb4);
        sub_8023298(self);
        sub_80232FC(self);
        sub_8023288(self);
        sub_8028568(gUnknown_03001318, *(s32 *)((u8 *)self + 0xbc));
        {
            struct actor *player = (struct actor *)gUnknown_030012D8;
            s32 *p = (s32 *)((u8 *)self + 0xd4);
            sub_8007398(player, p[0], p[1]);
        }
        sub_8022CA0(self, *((u8 *)self + 0xe0));
    } else {
        sub_80231D4(self);
    }
}

void sub_802364C(void *self)
{
    sub_8022468(self, 2);
}

void sub_8023658(void *self)
{
    sub_8022468(self, 1);
    sub_80019A8(gUnknown_030012BC, 0x5d);
}

/* Allocates a `0x44c`-byte block, fires an (empty) `nullsub_7` hook and
 * `sub_80361B0`, then hands the block to `sub_8037154` with flags `3`
 * if the allocation succeeded. */
void sub_8023674(void)
{
    /* `nullsub_7` is a real no-op (`bx lr`) but, split into its own
     * translation unit (src/audio/counter_selector.c), an ordinary call
     * forces the allocated block's pointer into a callee-saved register
     * *before* the call, one instruction earlier than the ROM (which
     * keeps it in r0 across the call and only moves it afterward - only
     * possible because the two functions were compiled together
     * originally). Spelling the call as inline asm that doesn't clobber
     * r0 reproduces the ROM's exact (and, here, still safe) delayed
     * move. */
    register void *tmp asm("r0") = sub_8026EDC(0x44c);
    void *block;

    asm volatile("bl nullsub_7" : "+r"(tmp) :: "r1", "r2", "r3", "lr", "cc");
    block = tmp;
    sub_80361B0();
    if (block != NULL) {
        sub_8037154(block, 3);
    }
}

void sub_802369C(void *self)
{
    sub_8022468(self, 0);
}

void nullsub_24(void)
{
}

/* Unpacks the packed halfword at `self->0x14c`/`0x14d` (see
 * `sub_80236EC`'s inverse below) into `self->0x74`/`0x6c`/`0x78`, but
 * first refreshes the snapshot itself: copies `src` into `self`'s first
 * `0x68` bytes, then re-copies `self` into the `0x14c`-based snapshot
 * block. */
void sub_80236AC(void *self, void *src)
{
    register u8 *snap asm("r5") = (u8 *)self + 0x14c;
    /* Register pins reproduce the ROM's exact "freshly loaded value in
     * one register, shifted result in another" shape for both the byte
     * and halfword extracts below (see docs/workflow.md step 7). */
    register u8 raw asm("r1");
    register s32 val asm("r0");
    register u16 packed asm("r5");

    sub_800014C(self, src, 0x68);
    sub_800014C(snap, self, 0x68);

    raw = *snap;
    val = (u32)(raw << 25) >> 25;
    *(s32 *)((u8 *)self + 0x74) = val;

    *(s32 *)((u8 *)self + 0x6c) = *((u8 *)self + 0x14d) >> 1;

    packed = *(u16 *)snap;
    val = (u32)(packed << 23) >> 30;
    *(s32 *)((u8 *)self + 0x78) = val;
}

#if NON_MATCHING
/* Packs `self->0x74`/`0x6c`/`0x78` back into the halfword at
 * `self->0x14c`/`0x14d` - the inverse of `sub_80236AC` above. Real
 * bytes for the default build in `asm/code_3_2_17_236ec.s`.
 *
 * NOT YET BYTE-MATCHING: every field/mask/shift is confirmed correct,
 * but the ROM keeps `self` itself alive in r3 the whole function (this
 * compiler instead folds `self` straight into each field access), and
 * addresses `self->0x14d` via register+register indexing (a literal
 * `0x14d` loaded once into r5, added to r3 at the `ldrb`/`strb`
 * themselves) rather than a precomputed pointer - a different
 * addressing-mode encoding no plain-C phrasing tried here reproduces,
 * while `self->0x14c` (used both early and at the final halfword
 * access) does get its own dedicated pointer register either way. */
void sub_80236EC(void *self)
{
    u8 *snap = (u8 *)self + 0x14c;
    u8 byte0, byte1;
    u16 packed;

    byte0 = (*snap & ~0x7f) | (*(s32 *)((u8 *)self + 0x74) & 0x7f);
    *snap = byte0;

    byte1 = (*(u8 *)(snap + 1) & 1) | ((*(s32 *)((u8 *)self + 0x6c) << 1) & 0xff);
    *(u8 *)(snap + 1) = byte1;

    packed = *(u16 *)snap;
    packed = (packed & 0xfe7f) | ((*(s32 *)((u8 *)self + 0x78) & 3) << 7);
    *(u16 *)snap = packed;
}
#endif
