#include "core.h"
#include "vram_pool.h"
#include "phys_obj.h"

/* GitHub issue #12 Phase 2: 0x0800E560-0x0800EEF0, the lower-address
 * half of the remaining tail of the physics/collision subsystem's
 * per-edge handler family (see docs/matching/issue-12-physics-collision.md's
 * "Phase 1" appendix for the confirmed dispatch map both
 * sub_0800D18C/sub_800E08C, src/system/game_loop47.c, dispatch into).
 * `self` throughout is the same "collision box" object every other
 * function in this subsystem operates on (`struct crate`,
 * include/phys_obj.h). Compiled with old_agbcc (the Makefile's
 * OLD_AGBCC_OBJS) - see docs/matching/issue-12-physics-collision.md's
 * NAKED-retry section. */

extern void sub_8009150(struct phys_obj_list *list, struct crate *obj);
extern struct phys_obj_list *gCrateList;
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *gAudioContext;
extern void *gEntitySpawner;
extern u8 gCrateKindCounted[];
extern u8 gCrateKindUnbreakable[];
extern u8 gCrateKindExplosive[];
extern u16 rand(void);
extern void AddBrokenCrate(void *self);
extern void SetCheckpointAtPlayer(void *self, u8 arg1);
extern void FreezeLevelClock(void *arg, s32 n);
extern void sub_80259D4(void *self, s32 n);
extern s32 sub_802599C(void *self, s32 n);
extern struct crate *GetCrateBelow(struct crate *obj);
extern struct crate *GetCrateAbove(struct crate *obj);
extern void OpenAkuAkuCrate(struct crate *self);
extern void OpenLifeCrate(struct crate *self, u32 arg1);
extern void BreakCrateInStack(struct crate *self, u32 arg1, u32 arg2, u32 arg3);
extern void BreakCrate(struct crate *self, u32 arg1);
extern void OpenMysteryCrate(struct crate *self, u32 arg1);
extern void sub_800ED08(struct crate *self, u32 arg1);
extern void DropCratesAbove(struct crate *self);
extern void ExplodeCrate(struct crate *self, u8 arg1);
extern void ActivateIronSwitchCrate(struct crate *self);
extern void ActivateNitroSwitchCrate(struct crate *self);

/* The effect object sub_8025BAC spawns (only the fields set here). */
struct phys_puff
{
    u8 unk_00[0xC];
    u8 unk_0C_0:2;      // 0x0C
    u8 hidden:1;
    u8 unk_0C_3:5;
    u8 unk_0D[0x1B];
    u8 unk_28_0:4;      // 0x28
    u8 flipX:1;
    u8 unk_28_5:3;
    u8 unk_29[0x2B];
    s32 velX;           // 0x54
    s32 accelX;         // 0x58
    s32 accelY;         // 0x5C
    u8 unk_60[4];
    s32 velY;           // 0x64
};

extern struct phys_puff *sub_8025BAC(void *pool, s32 arg1, s32 kind, s32 x, s32 y, s32 arg5);
extern void *DropWumpa(void *pool, u16 x, u16 y, u8 p3, u8 p4, u8 p5);
extern void *DropExtraLife(void *pool, u16 x, u16 y, u8 p3, u32 p4, u8 p5);

/* DropWumpa/DropExtraLife take a stack-passed word (p4) and byte (p5).
 * The ROM stores the byte with `add rX, sp, #4; strb`, but this compiler
 * widens a stack-passed u8 to a word `str` (see mover_new.h), so callers
 * store both by hand into two locals declared first in the function
 * (`s32 argP4; u32 argP5;`, landing at sp+0/sp+4) and call through a
 * 4-argument view of the function. */
typedef void *(*SpawnCall4)(void *pool, s32 x, s32 y, u8 p3);
#define SPAWN_CALL(pool, x, y, p3) ((SpawnCall4)DropWumpa)((pool), (x), (y), (p3))
#define BONUS_CALL(pool, x, y, p3) ((SpawnCall4)DropExtraLife)((pool), (x), (y), (p3))

static inline void PhysArgByte(u8 *p, u8 v)
{
    *(volatile u8 *)p = v;
}

/* DropWumpa(gEntitySpawner, x, y, p3, p4, p5), x/y evaluated
 * before the pool pointer as in the ROM. */
#define PHYS_SPAWN(x, y, p3, p4, p5)                                           \
    {                                                                          \
        s32 _x = (x);                                                          \
        s32 _y = (y);                                                          \
        SPAWN_CALL(gEntitySpawner, _x, _y,                                  \
                   (*(volatile s32 *)&argP4 = (p4),                            \
                    PhysArgByte((u8 *)&argP5, (p5)), (p3)));                   \
    }

/* The same for DropExtraLife. */
#define PHYS_BONUS(x, y, p3, p4, p5)                                           \
    {                                                                          \
        s32 _x = (x);                                                          \
        s32 _y = (y);                                                          \
        BONUS_CALL(gEntitySpawner, _x, _y,                                  \
                   (*(volatile s32 *)&argP4 = (p4),                            \
                    PhysArgByte((u8 *)&argP5, (p5)), (p3)));                   \
    }

/* Dispatch-id-5 handler. Both `sub_0800D18C`'s and `sub_800E08C`'s
 * per-edge jump tables' case 3 eventually reach this handler
 * transitively (via `BreakCrateInStack`), see
 * docs/matching/issue-12-physics-collision.md's dispatch map.
 *
 * Arms `self`'s `+0x48` frame-countdown timer to `0x168` (360) the
 * first time it's seen at its sentinel value (`-0x2a`), clearing
 * `+0x51`'s retry counter alongside it. While that countdown is
 * still running and `self`'s own `+0x50` byte is zero, bumps `+0x51`
 * each call; once it passes 4, calls `BreakCrateInStack(self, 0, 0, 0)`
 * (the "give up, hand off" case). Otherwise (still under the retry
 * cap), sets `self+0x4d` bit `0x80`, marks
 * `gPlayer+0x80 = 1`, arms a fresh `+0x4f = 6` sub-timer,
 * and spawns a pair of particle effects (`DropWumpa`, effect kind
 * `0xe`) at `self`'s position, offset `-6`/`+3` pixels on Y/X. Once
 * the `+0x48` countdown itself expires (`<= 0`), calls
 * `BreakCrateInStack(self, 0, 0, 0)` unconditionally instead.
 *
 * The two spawns' stack byte argument is stored by hand (see
 * SPAWN_CALL above); the ROM keeps its slot address in r5 and the
 * constant 1 in r4 across both calls, pinned here. */
void BounceWumpaCrate(struct crate *self)
{
    s32 argP4;
    u32 argP5;

    if (self->u48.n == -0x2a) {
        self->u48.n = 0x168;
        self->unk_51 = 0;
    }
    if (self->u48.n > 0) {
        if (self->unk_50 == 0) {
            if (++self->unk_51 > 4) {
                BreakCrateInStack(self, 0, 0, 0);
            } else {
                self->state |= 0x80;
                {
                    struct phys_player *p = PHYS_PLAYER;
                    u8 one = 1;

                    p->busy = one;
                }
                self->timer = 6;
                self->unk_50 = 1;
            }
            {
                register u8 *p5 asm("r5");
                register u8 one asm("r4");

                {
                    s32 x = self->x >> 8;
                    s32 y = (self->y >> 8) - 6;

                    SPAWN_CALL(gEntitySpawner, x, y, (*(volatile s32 *)&argP4 = 0xe, ({
                        p5 = (u8 *)&argP5;
                        one = 1;
                        *p5 = one;
                        0;
                    }), 0));
                }
                {
                    s32 x = (self->x >> 8) + 3;
                    s32 y = self->y >> 8;

                    SPAWN_CALL(gEntitySpawner, x, y, (*(volatile s32 *)&argP4 = 0, ({
                        *p5 = one;
                        0;
                    }), 0));
                }
            }
        }
    } else {
        BreakCrateInStack(self, 0, 0, 0);
    }
}

/* Case-2 handler ("dispatch id 0xe") both `sub_0800D18C`'s and
 * `sub_800E08C`'s per-edge jump tables select - see
 * docs/matching/issue-12-physics-collision.md's dispatch map. Switches
 * `self` into a fresh sub-state (`+0x4e = 0x15`, hitbox tag `+0x2d =
 * 0x14`), rebuilds its hitbox record (`sub_80087C0`/`sub_80087B4`/
 * `sub_800872C`, the same trio every hitbox-rebuild call in this
 * subsystem uses), registers it with the object-pool grid
 * (`sub_8009150`), re-derives a low-nibble sub-animation value from
 * the freshly selected hitbox record's `+0x14` byte via
 * `GetPaletteSlot`'s tile-asset-cache lookup, plays SFX `0x11`, and
 * arms a `+0x4f` countdown of `0x3c` (60) frames. */
void LightTntCrate(void *selfArg)
{
    u8 *self = selfArg;
    u8 *entry;
    u8 lo;

    self[0x4e] = 0x15;
    {
        /* Anchored: the ROM loads the `0x14` immediate before computing
         * `&self[0x2d]` for this store (the opposite order from the
         * previous `self[0x4e] = 0x15` store just above, which computes
         * its address first) - a plain C `self[0x2d] = 0x14;` here
         * always picks the address-first order for both stores. `addr2d`
         * is kept as the live `&self[0x2d]` pointer (matching the ROM's
         * own r5) rather than recomputed, since the ROM's later tag
         * read reuses this same register. */
        register u8 *addr2d asm("r5");

        asm volatile(
            "mov r0, #0x14\n\t"
            "add r5, %1, #0\n\t"
            "add r5, r5, #0x2d\n\t"
            "strb r0, [r5]\n\t"
            : "=r"(addr2d)
            : "r"(self)
            : "r0", "cc", "memory"
        );

        sub_80087C0(self);
        sub_80087B4(self);
        sub_800872C(self, 0);
        /* Anchored: the ROM materializes the `0x10` immediate before
         * loading `self[0xc]`, not after - a plain C `self[0xc] |=
         * 0x10;` (in either operand order) always loads the field
         * first here. */
        {
            register u32 flagsResult asm("r0");

            asm volatile(
                "mov r0, #0x10\n\t"
                "ldrb r1, [%1, #0xc]\n\t"
                "orr r0, r1\n\t"
                "strb r0, [%1, #0xc]\n\t"
                : "=r"(flagsResult)
                : "r"(self)
                : "r1", "cc", "memory"
            );
        }
        sub_8009150(gCrateList, (struct crate *)self);

        {
            register u8 **p2 asm("r0") = *(u8 ***)(self + 0x20);
            register u8 *table2 asm("r1") = *p2;
            register u8 tag2 asm("r2") = *addr2d;
            register u8 *entry2 asm("r1");

        asm volatile(
            "lsl r0, %2, #3\n\t"
            "sub r0, r0, %2\n\t"
            "lsl r0, r0, #2\n\t"
            "add %0, %1, r0\n\t"
            : "=r"(entry2)
            : "r"(table2), "r"(tag2)
            : "r0", "cc"
        );
            entry = entry2;
        }
    }
    lo = GetPaletteSlot(gPaletteCache, entry[0x14]);
    /* Empty compiler barrier: forces the u8->u32 zero-extend implied by
     * `lo`'s use below to happen as its own step (matching the ROM's
     * `lsls r0,r0,0x18; lsrs r0,r0,0x18`), rather than letting the
     * optimizer fuse it into the `& 0xf` mask below into a single
     * shift-mask-shift sequence. */
    asm volatile("" : "+r"(lo));
    /* Anchored: the ROM computes `&self[0x29]` *before* masking `lo`
     * down to its low nibble (a plain C `self[0x29] = (self[0x29] &
     * ~0xf) | (lo & 0xf);` here always computes the mask first
     * regardless of source statement order), and materializes the
     * `~0xf` clear-mask at runtime (`movs r1,#0x10; rsbs r1,r1,#0`,
     * the negative-constant register-pinned mask idiom - see
     * matching_decomp_register_pinning and DrawCrate's own use of
     * it, game_loop35.c) rather than folding it into an 8-bit AND
     * immediate, ORing into the mask register (not the freshly-
     * extracted low-nibble register) before storing - transcribed as
     * one block to pin the whole sequence's order and registers at
     * once. */
    {
        register u8 rawLo asm("r0") = lo;

        asm volatile(
            "add r2, %1, #0\n\t"
            "add r2, r2, #0x29\n\t"
            "mov r1, #0xf\n\t"
            "and r0, r1\n\t"
            "mov r1, #0x10\n\t"
            "neg r1, r1\n\t"
            "ldrb r3, [r2]\n\t"
            "and r1, r3\n\t"
            "orr r1, r0\n\t"
            "strb r1, [r2]\n\t"
            : "+r"(rawLo)
            : "r"(self)
            : "r1", "r2", "r3", "cc", "memory"
        );
    }

    PlaySfx(gAudioContext, 0x11, 0x100);
    self[0x4f] = 0x3c;
}

/* Case-5 handler both `sub_0800D18C`'s and `sub_800E08C`'s per-edge
 * jump tables select unconditionally - see
 * docs/matching/issue-12-physics-collision.md's dispatch map. Spawns
 * a particle-effect object (`sub_8025BAC`, kind `0x2a`) at `self`'s
 * position (minus 10 pixels on X), initializes it (clearing flag bits
 * `+0xc`/`+0x28`, arming `+0x64`/`+0x54`/`+0x58`/`+0x5c` with a fixed
 * "settle" trajectory), then switches `self` itself into sub-state
 * `+0x2d = 0x1b`, rebuilds its own hitbox record, plays SFX `0x17`,
 * notifies `sub_80259D4` unless `self`'s `+8` id field is the
 * sentinel `0xffff`, conditionally reactivates the viewport
 * (`AddBrokenCrate`, gated on `gCrateKindCounted[self+0x4e]`), and
 * ends by telling `SetCheckpointAtPlayer` whether `self`'s `+0x50` byte is
 * nonzero before resetting `self`'s own `+0x4d` state byte to `1`. */
static inline void PuffSetMotion(struct phys_puff *puff, s32 vel, s32 ax, s32 ay)
{
    puff->velY = vel;
    puff->velX = vel;
    puff->accelX = ax;
    puff->accelY = ay;
}

void OpenCheckpointCrate(struct crate *self)
{
    struct phys_puff *puff;
    u8 one;

    {
        s32 x = (self->x >> 8) - 10;
        s32 y = self->y >> 8;

        puff = sub_8025BAC(gEntitySpawner, 0x2a, 0, x, y, 0);
    }
    puff->hidden = 0;
    puff->flipX = 0;
    PuffSetMotion(puff, -0x180, 8, -0x10);
    PhysSetTag(self, 0x1b);
    PlaySfx(gAudioContext, 0x17, 0x100);
    {
        u16 id = self->id;

        if (id != 0xffff)
            sub_80259D4(gEntityFlags, id);
    }
    if (gCrateKindCounted[self->kind])
        AddBrokenCrate(gLevelState);
    SetCheckpointAtPlayer(gLevelState, self->unk_50 != 0);
    self->state &= 0x7f;
    PHYS_PLAYER->busy = 0;
    one = 1;
    self->state = (self->state & 0x80) | one;
}

/* Case-3 handler both `sub_0800D18C`'s and `sub_800E08C`'s per-edge
 * jump tables select (see docs/matching/issue-12-physics-collision.md's
 * dispatch map): counts `self` into `gPlayer+0x91`'s
 * "objects handled this frame" tally (saturating at a nonzero value -
 * only the very first caller of the frame actually increments it,
 * gated on `arg2`), then walks `self`'s "get prev" (`arg3 == 4`) or
 * "get next" (`arg3 == 8`) neighbor chain past every node whose
 * `+0x4d & 0x7f` state is already `1`, stopping at the first node
 * that isn't (or the last reachable node if the whole chain is state
 * `1`). Neither `arg3` value falls back to `self` itself as the
 * target. Finally, unless `gCrateKindUnbreakable[target+0x4e]` is
 * nonzero, dispatches to `BreakCrate(target, arg1)` - the shared
 * tail every one of this handler's paths converges on.
 *
 * The walk is written as the ROM's goto loops: the natural `for`
 * loops get rotated and their exit blocks laid out differently. */
void BreakCrateInStack(struct crate *self, u32 arg1, u32 arg2, u32 dir)
{
    u8 flag = arg1;
    struct crate *p;
    struct crate *q;

    if ((u8)arg2) {
        if (PHYS_PLAYER->handled != 0)
            return;
        PHYS_PLAYER->handled = 1;
        PHYS_PLAYER->handled++;
    }
    if (dir == 4) {
        p = GetCrateBelow(self);
        if (p == NULL || (p->state & 0x7f) == 1)
            goto none;
    prev:
        q = GetCrateBelow(p);
        if (q == NULL || (q->state & 0x7f) == 1)
            goto last;
        p = q;
        goto prev;
    }
    if (dir != 8)
        goto other;
    p = GetCrateAbove(self);
    if (p != NULL && (p->state & 0x7f) != 1)
        goto next;
none:
    q = self;
    goto found;
last:
    q = p;
    goto found;
next:
    q = GetCrateAbove(p);
    if (q == NULL || (q->state & 0x7f) == 1)
        goto last;
    p = q;
    goto next;
found:
    if (gCrateKindUnbreakable[q->kind] == 0)
        BreakCrate(q, flag);
    return;
other:
    BreakCrate(self, flag);
}

/* `BreakCrateInStack`'s (and, transitively, both of the subsystem's
 * top-level dispatchers') shared "actually apply the collision
 * response" landing point - see
 * docs/matching/issue-12-physics-collision.md's dispatch map. Early-
 * outs when `self+0x4d & 0x7f == 1` (already fully handled this
 * frame). Otherwise: registers `self` with the object-pool grid,
 * resets its `+0x4d` state byte to `0x81` and clears
 * `gPlayer+0x80`, switches `self` into hitbox tag `0x1d`
 * and rebuilds its hitbox record, re-derives its `+0x29` low-nibble
 * sub-animation value (same `GetPaletteSlot` tile-asset-cache lookup
 * `LightTntCrate` uses) and clamps `self+0x30`'s index to the newly
 * selected hitbox record's own `+0x16` count, conditionally
 * reactivates the viewport (`AddBrokenCrate`, gated on
 * `gCrateKindCounted[self+0x4e]`), flips one bit of
 * `gEntityFlags`'s bit-grid keyed by `self+8`, calls
 * `DropCratesAbove` (neighbor "impact spread" propagation), then
 * dispatches a 23-case jump table on `self`'s freshly-cached
 * `+0x4e` state id to one of this subsystem's other per-state leaf
 * handlers (`OpenAkuAkuCrate`/`OpenLifeCrate`/`ActivateIronSwitchCrate`/`ActivateNitroSwitchCrate`/
 * `OpenMysteryCrate`/`sub_800ED08`/`ExplodeCrate`/`FreezeLevelClock`, or a
 * SFX-3-plus-particle-spawn fallback) before converging on a shared
 * epilogue.
 *
 * Real C under old_agbcc (the old "r8/sb accumulators" note was wrong;
 * see docs/matching/issue-12-13-25-naked-retry.md): the tag switch goes
 * through PhysSetTag (constant loaded before the tag address), the
 * frame clamp through PhysSetFrame(self, 3), bit 4 of `flags` is set as
 * a bitfield (a plain `|= 0x10` leaves a zero pseudo that CSE shares
 * with the `busy` store), the constant 1 of the state store is a local
 * `one` that the bitmap shift reuses (the ROM's r8), and the switch
 * cases are in the ROM's block order with an explicit empty case 22. */
void BreakCrate(struct crate *self, u32 arg1)
{
    s32 argP4;
    u32 argP5;
    u8 flag = arg1;
    u8 chained;
    u8 one;

    if ((self->state & 0x7f) == 1)
        return;
    chained = 0;
    if (GetCrateAbove(self) != NULL && flag == 0)
        chained = 1;
    PHYS_FLAG4(self) = 1;
    sub_8009150(gCrateList, self);
    self->state &= 0x7f;
    PHYS_PLAYER->busy = 0;
    one = 1;
    self->state = (self->state & 0x80) | one;
    PhysSetTag(self, 0x1d);
    {
        struct anim_rec *recs = self->anim->records;
        struct anim_rec *rec = &recs[self->tag];

        self->slot = GetPaletteSlot(gPaletteCache, rec->unk_14);
    }
    PhysSetFrame(self, 3);
    if (gCrateKindCounted[self->kind])
        AddBrokenCrate(gLevelState);
    {
        s32 id = self->id;
        u8 *base = gEntityFlags;
        s32 word = id / 32;
        s32 off = word * 4;
        u32 *slot = (u32 *)(base + 0x108);

        slot = (u32 *)((u8 *)slot + off);
        *slot |= one << (id - word * 32);
    }
    DropCratesAbove(self);
    switch (self->kind)
    {
    case 2:
        if (flag == 0)
            OpenAkuAkuCrate(self);
        break;
    case 9:
        if (flag == 0)
            OpenLifeCrate(self, chained);
        break;
    case 3:
        ActivateIronSwitchCrate(self);
        break;
    case 6:
        ActivateNitroSwitchCrate(self);
        break;
    case 11:
        if (flag == 0)
            OpenMysteryCrate(self, chained);
        break;
    case 4:
    case 12:
    case 13:
        PlaySfx(gAudioContext, 3, 0x100);
        break;
    case 10:
    case 14:
    case 19:
    case 20:
    case 21:
        ExplodeCrate(self, 0);
        break;
    case 15:
        if (flag == 0)
            sub_800ED08(self, chained);
        break;
    case 16:
        FreezeLevelClock(gLevelState, 1);
        break;
    case 17:
        FreezeLevelClock(gLevelState, 2);
        break;
    case 18:
        FreezeLevelClock(gLevelState, 3);
        break;
    case 0:
        PHYS_SPAWN(self->x >> 8, (self->y >> 8) + 3, 0, 3, chained);
        if (flag == 0)
            PlaySfx(gAudioContext, 3, 0x100);
        break;
    case 22:
        break;
    }
}

/* Case-11 handler of `BreakCrate`'s own 23-case jump table (dispatch
 * id `0xb`) - see docs/matching/issue-12-physics-collision.md's
 * dispatch map. Plays SFX 3, then (the first time `self`'s `+0x51`
 * retry counter is exactly `9`) rolls a random "escalation level"
 * (`1`/`4`/`7`/`8`, weighted via three `rand()` thresholds) into that
 * same byte. Dispatches a second, 10-case jump table on
 * `(self+0x51 - 1)` (clamped, values above 10 fall to the same
 * "final" case as 0): cases 5 down through 0 deliberately
 * *fall through* into each other without their own return, cascading
 * multiple `DropWumpa` particle spawns at slightly different
 * offsets around `self` the further the level counted down (a
 * escalating "more debris" burst); case 6 fires a screen-shake
 * (`_call_via_r4`, effect `0x1a`) plus SFX; case 7 spawns a
 * `DropExtraLife` bonus object and notifies `sub_80259D4`; case 9 spawns
 * one final small `DropWumpa` puff. All paths converge on a shared
 * epilogue.
 *
 * Cases 7 and 8 are OpenAkuAkuCrate/OpenLifeCrate inlined; their SFX calls
 * go through a static inline wrapper so the id is loaded before the
 * volume, as in the ROM. */
static inline void PhysSfx(s32 id)
{
    PlaySfx(gAudioContext, id, 0x100);
}

void OpenMysteryCrate(struct crate *self, u32 arg1)
{
    s32 argP4;
    u32 argP5;
    u8 flag = arg1;

    PlaySfx(gAudioContext, 3, 0x100);
    if (self->unk_51 == 9) {
        u8 r = (u16)rand() >> 8;

        if (r <= 0x56)
            self->unk_51 = 1;
        else if (r <= 0xd3)
            self->unk_51 = 4;
        else if (r <= 0xec)
            self->unk_51 = 7;
        else
            self->unk_51 = 8;
    }
    switch (self->unk_51) {
    case 10:
        {
            s32 x = self->x >> 8;
            s32 y = self->y >> 8;

            SPAWN_CALL(gEntitySpawner, x, y, (*(volatile s32 *)&argP4 = 0xff, ({
                register u8 *p asm("r4") = (u8 *)&argP5;
                register u8 v asm("r3") = 0;
                *p = v;
                0;
            }), 0));
        }
        break;
    case 8:
        PhysSfx(3);
        {
            u16 id = self->id;

            if (id != 0xffff) {
                if ((u8)sub_802599C(gEntityFlags, id) == 0)
                    sub_80259D4(gEntityFlags, self->id);
            }
        }
        PHYS_BONUS(self->x >> 8, (self->y >> 8) + 3, 0, 3, flag);
        break;
    case 7:
        {
            struct gobj *p = gPlayer;

            if (p->flags >> 7) {
                PhysCall3(p, &p->vtable->m68, 0, 0x1a, 0);
                PhysSfx(1);
            }
        }
        break;
    case 6:
        PHYS_SPAWN((self->x >> 8) - 1, (self->y >> 8) + 3, 1, 3, flag);
    case 5:
        PHYS_SPAWN((self->x >> 8) + 1, (self->y >> 8) + 1, 0, 3, flag);
    case 4:
        PHYS_SPAWN((self->x >> 8) - 3, (self->y >> 8) + 3, 1, 1, flag);
    case 3:
        PHYS_SPAWN((self->x >> 8) + 3, (self->y >> 8) + 2, 0, 1, flag);
    case 2:
        PHYS_SPAWN((self->x >> 8) + 5, (self->y >> 8) + 2, 0, 2, flag);
    case 1:
    default:
        PHYS_SPAWN((self->x >> 8) - 5, (self->y >> 8) + 3, 1, 2, flag);
        break;
    }
}

/* Case-15 handler of `BreakCrate`'s own 23-case jump table (dispatch
 * id `0xf`) - see docs/matching/issue-12-physics-collision.md's
 * dispatch map. Plays SFX 3, then switches on `self+0x48 & 7`: `1`
 * plays SFX 3 again, notifies `sub_80259D4` unless `self`'s `+8` id
 * is the sentinel `0xffff` (or is already scheduled per
 * `sub_802599C`), and spawns a `DropExtraLife` bonus object 3 pixels
 * below `self`; `2` forwards to `OpenMysteryCrate` (the escalating-debris
 * handler above); `3` clears `self+0x4d` bit `0x7f` and calls
 * `ExplodeCrate(self, 1)`; any other value (including `0`) does
 * nothing further.
 *
 * The empty `case 0` gives the ROM's `==1`/`<=1`/`==2`/`==3` compare
 * order. */
static inline void PhysBonus(s32 *p4, u8 *p5, s32 x, s32 y, u8 flag)
{
    BONUS_CALL(gEntitySpawner, x, y,
               (*(volatile s32 *)p4 = 3, *(volatile u8 *)p5 = flag, 0));
}

void sub_800ED08(struct crate *self, u32 arg1)
{
    s32 argP4;
    u32 argP5;
    u8 flag = arg1;

    PlaySfx(gAudioContext, 3, 0x100);
    switch (self->u48.n & 7) {
    case 0:
        break;
    case 1:
        PlaySfx(gAudioContext, 3, 0x100);
        {
            u16 id = self->id;

            if (id != 0xffff) {
                if ((u8)sub_802599C(gEntityFlags, id) == 0)
                    sub_80259D4(gEntityFlags, self->id);
            }
        }
        PhysBonus(&argP4, (u8 *)&argP5, self->x >> 8, (self->y >> 8) + 3, flag);
        break;
    case 2:
        OpenMysteryCrate(self, flag);
        break;
    case 3:
        self->state &= 0x80;
        ExplodeCrate(self, 1);
        break;
    }
}

/* Neighbor "impact spread" propagation, called once from
 * `BreakCrate`'s own body (not through either jump table) - see
 * docs/matching/issue-12-physics-collision.md's dispatch map. Derives
 * a base spread budget from `self`'s hitbox record's own `+9` byte
 * (`+1`, scaled by 256), then walks `self`'s "get next" neighbor
 * chain (`GetCrateAbove`), redistributing that budget across each
 * visited node's `+0x40`/`+0x44` "remaining spread" fields (first
 * node gets the whole thing computed from `self`'s own `+4`/`+0x44`
 * state, every node after that gets a running remainder carried
 * forward via `sb`), nudging each node's `+0x4c` byte toward 0 by the
 * caller-supplied `arg1`-derived step, re-registering it with the
 * object-pool grid, and - for any node whose `gCrateKindExplosive`
 * row is set, `self`'s own `+0x48` is clear, and its accumulated
 * `+0x44` spread exceeds `0x1600` - "graduating" it into state `0x48
 * = 1` (unless a neighbor-adjacency/`+0x4d` gate blocks it). Stops
 * when the walk runs out of neighbors.
 *
 * Matching notes (old_agbcc): the gCrateKindExplosive pointer is a
 * local set before the loop (only then does the ROM's reload-register
 * choice come out), the record lookup takes the anim table
 * first and the byte offset second, `spread` is built in two steps, the
 * step delta is widened into its own int before the add, and
 * `n->unk_40` is written in both arms of an if/else. */
void DropCratesAbove(struct crate *self)
{
    s8 delta = -2;
    struct anim_table *anim = self->anim;
    u32 off = self->tag * sizeof(struct anim_rec);
    struct anim_rec *rec = (struct anim_rec *)((u8 *)anim->records + off);
    s32 base = (rec->padY + 1) << 8;
    struct crate *n = GetCrateAbove(self);
    s32 spread;
    s32 carry;
    u8 *tbl = gCrateKindExplosive;

    if (self->unk_44 != 0)
        delta = -4;
    if (n == NULL || self == NULL)
        return;
    if (self->unk_44 != 0)
        n->unk_40 = self->unk_40;
    else
        n->unk_40 = self->y;
    spread = n->unk_40;
    spread -= n->y;
    if (spread < 0)
        spread = 0;
    carry = 0;
    while (n != NULL)
    {
        s32 t;

        if (n->unk_44 != 0)
        {
            n->unk_44 = spread + carry;
            n->unk_40 = n->unk_40 + carry;
        }
        else
        {
            n->unk_44 = spread;
            n->unk_40 = n->y + base;
        }
        t = n->unk_4C;
        if (t > 0)
            t = 0;
        {
            s32 d = delta;

            n->unk_4C = t + d;
        }
        n->flags |= 0x10;
        sub_8009150(gCrateList, n);
        if (tbl[n->kind] && self->u48.n == 0 && n->unk_44 > 0x1600)
        {
            struct crate *next = GetCrateAbove(n);
            struct crate *prev = GetCrateBelow(n);

            if (next == NULL && prev != NULL)
            {
                if (n->kind != 10)
                    goto advance;
                if (n->state & 0x7f)
                    goto advance;
            }
            n->timer = 0;
            n->u48.n = 1;
        }
    advance:
        n = GetCrateAbove(n);
        if (carry == 0)
            carry = base;
    }
}
