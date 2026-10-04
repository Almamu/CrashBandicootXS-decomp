#include "core.h"
#include "actor.h"
#include "level_state.h"

extern void *gUnknown_030012D8;
extern void *gEntityFlags;
extern void *gUnknown_030012BC;
extern void *gUnknown_03001318;

extern s32 sub_8023414(struct level_state *self);
extern void sub_80232EC(struct level_state *self);
extern void sub_80232FC(struct level_state *self);
extern void sub_8023298(struct level_state *self);
extern void sub_8023288(struct level_state *self);
extern u8 sub_8023290(struct level_state *self);
extern u8 sub_80232B8(struct level_state *self);
extern void sub_80231D4(struct level_state *self);
extern void *MemCopy32(void *dest, void *src, s32 size);
extern void CpuSet(const void *src, void *dst, u32 cnt);
extern void sub_8007398(struct actor *self, s32 arg1, s32 arg2);
extern void sub_8028568(void *state, s32 arg1);
extern void sub_8022CA0(void *self, u8 arg1);
extern void PlayCutscene(void *self, s32 mode);
struct AudioContext;
extern void StopSfx(struct AudioContext *self, u32 id);
extern void *sub_8026EDC(s32 size);
extern void nullsub_7(void);
extern s32 sub_80361B0(void);
extern void sub_8037154(void *self, u32 flags);

/* Sets `self->0x1bc` (a Q-format camera/position field paired with the
 * `sub_8023500` two-word setter below). */
void sub_80234E8(struct level_state *self, s32 value)
{
    self->unk_1bc = value;
}

/* Sets `self->0x1b8`, the companion field to `sub_80234E8` above. */
void sub_80234F4(struct level_state *self, s32 value)
{
    self->unk_1b8 = value;
}

/* Copies a `{x, y}` pair into `self->0x1c0`/`0x1c4`. */
void sub_8023500(struct level_state *self, s32 *point)
{
    s32 *dst = &self->unk_1c0;
    s32 y = point[1];
    s32 x = point[0];

    dst[0] = x;
    dst[1] = y;
}

/* Sets `self->0xa6` to 1 unless `sub_8023290` says it's already set. */
void sub_8023510(struct level_state *self)
{
    if (!sub_8023290(self)) {
        self->unk_a6 = 1;
    }
}

/* Sets `self->0xa4` to 1 unless `sub_80232B8` says it's already set. */
void sub_802352C(struct level_state *self)
{
    if (!sub_80232B8(self)) {
        self->unk_a4 = 1;
    }
}

/* Restores `self->0x70`/`0xa9` from their `0xcc`/`0xd0` shadow copies,
 * then copies the `0xe4`-`0x14b` snapshot block back over `self`'s own
 * first `0x68` bytes - the inverse direction of `SetCheckpoint`/
 * `sub_8022CA0`'s "stash a snapshot at +0xe4" below. */
void RestoreCheckpoint(struct level_state *self)
{
    u8 tmp;

    self->unk_70 = self->unk_cc;
    tmp = self->unk_d0;
    self->unk_a9 = tmp;
    MemCopy32(self, self->snapE4, 0x68);
}

/* Re-arms a level/checkpoint transition: stores `flag` at `self->0xe0`,
 * refreshes the `0xcc`/`0xd0` shadow pair, resets the `sub_8023414`
 * animation-state pair (`sub_80232FC`/`sub_80232EC`), snapshots `pair`
 * into `self->0xd4`/`0xd8`, flushes two spans of the
 * `gEntityFlags` bitmap via the `CpuSet` wrapper, then stashes the
 * `0xe4`-byte snapshot block (see `RestoreCheckpoint` above). */
void SetCheckpoint(void *selfArg, u8 flag, s32 *pairArg)
{
    register struct level_state *self asm("r5") = selfArg;
    register s32 *pair asm("r4") = pairArg;
    u8 tmp;

    self->unk_e0 = flag;
    self->unk_cc = sub_8023414(self);
    tmp = self->unk_a9;
    self->unk_d0 = tmp;
    sub_80232FC(self);
    sub_80232EC(self);
    {
        register s32 *dst asm("r2") = &self->checkpointX;
        register s32 px asm("r0") = pair[0];
        register s32 py asm("r1") = pair[1];
        dst[0] = px;
        dst[1] = py;
    }

    pair = gEntityFlags;
    {
        void *a = (u8 *)pair + 0x108;
        void *b = (u8 *)pair + 8;
        register u32 ctrl asm("r2") = CPU_SET_32BIT | 0x40;
        CpuSet(a, b, ctrl);
    }
    {
        void *a = (u8 *)pair + 0x308;
        void *b = (u8 *)pair + 0x208;
        register u32 ctrl asm("r2") = CPU_SET_32BIT | 0x40;
        CpuSet(a, b, ctrl);
    }

    MemCopy32(self->snapE4, self, 0x68);
}

/* When `flag` is set, accumulates `self->0xb4` into `self->0x70`,
 * refreshes the animation-state pair, flushes the tile record cache
 * (`gUnknown_03001318`) using `self->0xbc`, re-syncs the player's
 * stored position (`gUnknown_030012D8`) from `self->0xd4`/`0xd8`, and
 * re-runs `sub_8022CA0`; otherwise just calls `sub_80231D4`. */
void sub_80235E4(struct level_state *self, u8 flag)
{
    if (flag != 0) {
        self->unk_70 += self->unk_b4;
        sub_8023298(self);
        sub_80232FC(self);
        sub_8023288(self);
        sub_8028568(gUnknown_03001318, self->unk_bc);
        {
            struct actor *player = (struct actor *)gUnknown_030012D8;
            s32 *p = &self->checkpointX;
            sub_8007398(player, p[0], p[1]);
        }
        sub_8022CA0(self, self->unk_e0);
    } else {
        sub_80231D4(self);
    }
}

void sub_802364C(void *self)
{
    PlayCutscene(self, 2);
}

void sub_8023658(void *self)
{
    PlayCutscene(self, 1);
    StopSfx(gUnknown_030012BC, 0x5d);
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
    PlayCutscene(self, 0);
}

void nullsub_24(void)
{
}

/* Unpacks the packed halfword at `self->0x14c`/`0x14d` (see
 * `sub_80236EC`'s inverse below) into `self->0x74`/`0x6c`/`0x78`, but
 * first refreshes the snapshot itself: copies `src` into `self`'s first
 * `0x68` bytes, then re-copies `self` into the `0x14c`-based snapshot
 * block. */
void sub_80236AC(struct level_state *self, void *src)
{
    register u8 *snap asm("r5") = self->snap14C;
    /* Register pins reproduce the ROM's exact "freshly loaded value in
     * one register, shifted result in another" shape for both the byte
     * and halfword extracts below (see docs/workflow.md step 7). */
    register u8 raw asm("r1");
    register s32 val asm("r0");
    register u16 packed asm("r5");

    MemCopy32(self, src, 0x68);
    MemCopy32(snap, self, 0x68);

    raw = *snap;
    val = (u32)(raw << 25) >> 25;
    self->lives = val;

    self->wumpa = self->snap14C[1] >> 1;

    packed = *(u16 *)snap;
    val = (u32)(packed << 23) >> 30;
    self->maskLevel = val;
}

/* Packs `self->0x74`/`0x6c`/`0x78` back into the halfword at
 * `self->0x14c`/`0x14d` - the inverse of `sub_80236AC` above - and
 * returns the `self->0x14c` snapshot pointer (matching the `void *`
 * externs used at its call sites in settings_menu15.c/settings_menu8b.c).
 *
 * Register-pinned to reproduce three ROM-specific shapes plain C
 * phrasing alone didn't reach (see docs/workflow.md step 7):
 *  - `self` stays live in r3 across the whole function (natural
 *    codegen instead folds `self` into each field access as an
 *    immediate-offset addressing mode).
 *  - `self->0x14d` is addressed via register+register indexing (a
 *    literal `0x14D` loaded once into r5, added to r3 right at the
 *    `ldrb`/`strb`) rather than through a precomputed pointer -
 *    reproduced with two opaque `asm volatile` accesses.
 *  - the first field's `~0x7f` mask is materialized as a full 32-bit
 *    `0x80; neg` pair (the same "freshly loaded value and its
 *    transformed result in different registers" idiom as
 *    `sub_80236AC`) rather than narrowed to an 8-bit `#0x80` AND the
 *    way this compiler's optimizer does when it can prove the masked
 *    operand is byte-ranged - reproduced with an opaque `asm volatile`
 *    for just that mask. Because the return value (`self+0x14c`) ends
 *    up already sitting in r0 at the end, the epilogue's LR-restore
 *    register naturally lands on r1 instead of r0, matching the ROM's
 *    `pop {r1}; bx r1` without any extra hint. */
void *sub_80236EC(void *selfArg)
{
    register struct level_state *self asm("r3") = selfArg;
    register u8 *snap asm("r0");
    u8 byte0;
    u16 packed;
    register s32 t asm("r2");

    t = self->lives;
    snap = self->snap14C;
    t &= 0x7f;
    {
        register s32 mask asm("r1");
        asm volatile("mov %0, #0x80\n\tneg %0, %0" : "=r"(mask));
        mask &= *snap;
        byte0 = mask | t;
    }
    *snap = byte0;

    {
        s32 field6c = self->wumpa;
        register u32 off asm("r5") = offsetof(struct level_state, snap14C[1]);
        s32 shifted = field6c << 1;
        register s32 one asm("r1") = 1;
        register u32 raw asm("r4");
        asm volatile("ldrb %0, [%1, %2]" : "=r"(raw) : "r"(off), "r"(self));
        one &= raw;
        one |= shifted;
        asm volatile("strb %0, [%1, %2]" :: "r"(one), "r"(off), "r"(self));
    }

    {
        register s32 shifted asm("r2") = (self->maskLevel & 3) << 7;
        register s32 mask asm("r1") = 0xFFFFFE7F;
        register u16 loaded asm("r5") = *(u16 *)snap;
        mask &= loaded;
        packed = mask | shifted;
    }
    *(u16 *)snap = packed;

    return snap;
}
