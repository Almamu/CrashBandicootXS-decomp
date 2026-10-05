#include "core.h"

/* GitHub issue #9/#10: foundational investigation of the large, fully
 * raw 0x0800B8DC-0x0800D040 cluster (43 functions, ~5988 bytes) sitting
 * right after `actor_part17.c`'s span and right before the already-
 * documented physics/collision subsystem (`sub_800D040`,
 * game_loop6.c). See docs/matching/issue-9-10-0x0800b8dc-graphics.md
 * for the full semantic map this pass produced - the 18-case dispatch
 * table, `HitEnemy`'s own 22-case table, and field-layout notes for
 * whoever picks up the cluster's other ~41 functions next.
 *
 * Both functions here are entity-vtable slots (their own thumb-bit-set
 * addresses were found as raw pointer values inside the documented
 * 93-entry `gStaticData_087Exxx` family - `UpdateEnemyCtrl` sits at
 * `gEnemyCtrlVtable+0xC`, per docs/rom_map.md's "Found it" section)
 * - NOT caller/callee: `HitEnemy` is never `bl`'d from `UpdateEnemyCtrl`,
 * confirmed by reading `UpdateEnemyCtrl`'s own bytes in full. They're two
 * independent per-object-type behavior slots that merely sit next to
 * each other in ROM address order. */

#include "part_ctrl.h"

struct bg_scroll_layer {
    u8 unk_00[0x14];
    s32 heightPx;       // 0x14 - the level's height, in pixels
};

struct level_layers {
    u8 unk_00[0x10];
    struct bg_scroll_layer *layer0; // 0x10
};

/* A one-byte by-value argument: the ROM stores it into its stack slot
 * with `strb` (as in actor_part128.c). */
struct byte_arg {
    u8 v;
} __attribute__((packed));

extern void *gPlayer;
extern void *gEntityFlags;
extern void *gAudioContext;
extern void *gEntitySpawner;
extern struct level_layers *gLevelLayers;
extern s32 gUnknown_030012A0;
extern s32 gUnknown_030012A4;
extern s32 gUnknown_030012A8;
extern s32 gUnknown_030012AC;

/* All of the following are still-raw siblings in this same cluster
 * (asm/code_3_2_17.s) - treated as opaque callees for this pass, per
 * this investigation's own scope (dispatch shape first, callee
 * semantics are the next phase's job). Signatures are inferred purely
 * from the registers each call site sets. */
extern void UpdateEnemyPatrol(void *self);
extern void UpdateEnemyHomingX(void *self);
extern void UpdateEnemyHomingY(void *self);
extern void UpdateEnemyHop(void *self);
extern void sub_800C314(void *self);
extern void UpdateEnemyAttackCycle(void *self);
extern void sub_800C5D4(void *self);
extern void sub_800C8F8(void *self);
extern void UpdateEnemyBob(void *self);
extern void sub_800C97C(void *self);
extern void *sub_800C9C8(s32 a, s32 b, s32 c, s32 d, s32 e, void *f);
extern void sub_800BFA8(void *self);
extern void *CreateKnockedEnemyCtrl(void *mem); /* constructor: resets the fresh object and points its +0xC table at gKnockedEnemyCtrlVtable (actor_part117.c) */
extern void *OperatorNew(s32 size);
extern void *_call_via_r1(void *arg0, void *fn);
extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);
extern void PlayAmbientSfx(void *ctx, s32 id, s32 frame, s32 vol, struct byte_arg force);
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern s32 RandRange(s32 max);
extern void *sub_8025BAC(void *pool, s32 arg1, s32 kind, s32 x, s32 y, s32 arg5);

typedef void (*ctrl_bounce_fn)(void *self, s32 a, s32 b, s32 c);

/* Sets `t`'s `gone` bit and, if it has an id, its bit in the "gone"
 * bitmap at gEntityFlags+0x108. */
static inline void MarkGone(struct ctrl_target *t)
{
    t->gone = 1;
    if (t->id != 0xFFFF)
        do {
            s32 id = t->id;
            u8 *base = gEntityFlags;
            s32 word = id / 32;
            s32 off = word * 4;
            u32 *slot = (u32 *)(base + 0x108);

            slot = (u32 *)((u8 *)slot + off);
            *slot |= 1 << (id - word * 32);
        } while (0);
}

static inline void SetVelX(struct ctrl_target *t, s32 a, s32 b, s32 c)
{
    t->speedX = a;
    t->velA[0] = a;
    t->velA[1] = b;
    t->velA[2] = c;
}

static inline void SetVelY(struct ctrl_target *t, s32 a, s32 b, s32 c)
{
    t->speedY = a;
    t->velB[0] = a;
    t->velB[1] = b;
    t->velB[2] = c;
}

/* Both values are evaluated before either store, as in the ROM. */
static inline void SetPos(struct ctrl_target *t, s32 x, s32 y)
{
    t->x = x;
    t->y = y;
}

/* The target's method at +0x28 (nonzero once its animation is over). */
static inline u8 AnimQuery(struct ctrl_target *t)
{
    struct part_method *m = PART_METHOD(t, 0x28);

    return (u32)_call_via_r1((u8 *)t + m->thisOffset, m->fn);
}

/* Branchless `abs()` (`asrs`/`eors`/`subs`), as the ROM computes it. */
static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

/* The target's `hit` bit, read the way the ROM does (`lsrs #3; ands
 * #1` on the flags byte; the bitfield would be tested with `movs #8;
 * ands`). */
static inline u32 TargetHit(struct ctrl_target *t)
{
    return (((struct box_part *)t)->flags >> 3) & 1;
}

/* An 18-state dispatcher keyed off `self->state` (1-18; 0 or > 18 is
 * a no-op, the ROM's `subs r0, #1` / `cmp r0, #0x11` / `bls`). Reached
 * only through the entity-vtable slot noted at the top of this file:
 * a per-object-type "update" callback.
 *
 * Real C since the #10 big NAKED retry (old_agbcc; this file moved to
 * `OLD_AGBCC_OBJS`). The case bodies are in the ROM's block order;
 * 1 and 12 are explicit empty cases so the table is indexed by
 * `state - 1`. What the match needed (see
 * docs/matching/big-naked-retry-2.md):
 * - state 18's second `animDone` test reads the byte through a
 *   `vu8`, so jump threading can't fold it into the first test (the
 *   ROM reloads the target and tests again);
 * - `zero.v = 0` is stored before the distance math, and the volume
 *   is a separate local;
 * - state 5's height tests read `t->y` into a local first, and the
 *   second test goes through its own `t2`;
 * - state 9 reads each position into a local before storing it. */
void UpdateEnemyCtrl(struct part_ctrl *self)
{
    switch (self->state) {
    case 1:
    case 12:
        break;
    case 17:
        if (self->target->y >= self->baseY) {
            if (self->target->speedX == 0 && self->target->speedY != 0) {
                sub_800C8AC(self, 0);
            } else if (self->target->speedX < 0) {
                u8 done;

                if (self->target->animDone)
                    SetEnemyAnimMode(self, 0);
                done = AnimQuery(self->target);
                if (done == 0) {
                    struct ctrl_target *t;

                    SetPos(self->target, self->baseX, self->baseY - 0x6400);
                    sub_800C8BC(self, 0);
                    t = self->target;
                    SetVelY(t, 0x80, 0, 0x80);
                    t->flag4 = 1;
                }
            } else {
                sub_800C5D4(self);
            }
        }
        {
            struct ctrl_target *t = self->target;

            if (t->tick == 0 && t->timer == 0 && AnimQuery(t))
                PlaySfx(gAudioContext, 0x13, 0x100);
        }
        break;
    case 5:
        {
            struct ctrl_target *t = self->target;
            s32 y = t->y;

            if (y > (gLevelLayers->layer0->heightPx << 8) - 0x1E00) {
                t->flag7 = 0;
                {
                    struct ctrl_target *t2 = self->target;

                    y = t2->y;
                    if (y > (gLevelLayers->layer0->heightPx << 8) + 0x1E00)
                        MarkGone(t2);
                }
            } else if (t->unk_68 == 8) {
                SetVelY(t, 0, 0, 0);
            } else {
                SetVelY(t, 0x400, 0, 0x400);
            }
        }
        break;
    case 18:
        {
            struct ctrl_target *t = self->target;
            s32 x = t->x >> 8;
            s32 y = t->y >> 8;
            struct ctrl_target *p = gPlayer;
            s32 dx = Abs(x - (p->x >> 8));
            s32 d = Abs(y - (p->y >> 8));
            struct byte_arg zero;
            s32 vol;

            zero.v = 0;
            if (d < dx)
                d = dx;
            d = d < 0x20 ? 0x20 : d;
            if (d > 0xa0)
                d = 0xa0;
            vol = 0x100 - (d - 0x20) * 2;
            PlayAmbientSfx(gAudioContext, 0x2b, 8, vol, zero);
        }
        if (self->target->animDone && self->mode == 3) {
            struct ctrl_target *pop = sub_800C9C8(0x1d, 0, 0, 0x2b, 0, self->target);

            self->popup = pop;
            pop->kind = 3;
            PlaySfx(gAudioContext, 0x12, 0x100);
        } else if (*(vu8 *)&self->target->animDone && self->mode == 5) {
            MarkGone(self->popup);
            self->popup = 0;
        }
        UpdateEnemyPatrol(self);
        UpdateEnemyAttackCycle(self);
        if (self->mode == 1) {
            struct ctrl_target *t = self->target;
            u32 m = t->mirror.u.x;

            t->mirror.u.x = !m;
            SetEnemyAnimMode(self, 0);
            sub_800C8BC(self, 1);
        } else if (self->mode == 6) {
            struct ctrl_target *t = self->target;
            u32 m = t->mirror.u.x;

            t->mirror.u.x = !m;
            SetEnemyAnimMode(self, 4);
            {
                struct ctrl_target *t2 = self->target;

                t2->tick = (*t2->keyframes)[t2->frame].steps - 1;
            }
            sub_800C8BC(self, 1);
        }
        if (self->popup)
            self->popup->x = self->target->x;
        break;
    case 3:
        sub_800C5D4(self);
        break;
    case 2:
        UpdateEnemyPatrol(self);
        break;
    case 13:
        UpdateEnemyPatrol(self);
        /* fallthrough */
    case 4:
        UpdateEnemyAttackCycle(self);
        break;
    case 14:
        UpdateEnemyAttackCycle(self);
        sub_800C97C(self);
        break;
    case 6:
        UpdateEnemyBob(self);
        break;
    case 7:
        sub_800C314(self);
        break;
    case 8:
        UpdateEnemyHop(self);
        break;
    case 9:
        if (!gUnknown_030012A4) {
            s32 x = self->target->x;

            gUnknown_030012A0 = x;
            gUnknown_030012A4 = 1;
        }
        if (!gUnknown_030012AC) {
            s32 y = self->target->y;

            gUnknown_030012A8 = y;
            gUnknown_030012AC = 1;
        }
        UpdateEnemyHomingX(self);
        sub_800C8F8(self);
        {
            struct ctrl_target *t = self->target;
            s32 x, y;

            x = t->x;
            gUnknown_030012A0 = x;
            y = t->y;
            gUnknown_030012A8 = y;
        }
        break;
    case 11:
        UpdateEnemyHomingX(self);
        UpdateEnemyHomingY(self);
        if (TargetHit(self->target) && self->kind == 6) {
            struct part_method *m = &self->anchor->bounce;

            ((ctrl_bounce_fn)m->fn)((u8 *)self + m->thisOffset, 0, 1, 0);
            PlaySfx(gAudioContext, 4, 0x100);
        }
        break;
    case 15:
        UpdateEnemyPatrol(self);
        /* fallthrough */
    case 10:
        UpdateEnemyHomingY(self);
        break;
    case 16:
        UpdateEnemyAttackCycle(self);
        sub_800BFA8(self);
        break;
    }
}

/* `HitEnemy`'s own state selector (`arg2`, values 1-22) dispatches
 * through a *second*, independent jump table after a shared prelude
 * (a `gPlayer+0x88` state-object bit-flip + bitmap-set, or
 * the same on `self+0x88` if that global gate is off). Case values
 * 1-17 and 20-21 collapse to the same shared "camera-anchored ambient
 * sound + bitmap-flag" tail (case 0's own code, reused); only 18/19
 * (a spawn-and-launch-a-child-object handler) and 0/20/21 (the ambient
 * sound tail) do real, distinct work - 17 of the 22 declared states are
 * pure no-ops sharing one target, the same "mostly-empty dense switch"
 * shape `UpdateEnemyCtrl` itself has for states 1/12.
 *
 * `gPlayer+0x88`'s object (when non-null and `state==1`) or
 * `self+0x88`'s own object get the same "flip `+0xc` bit0, bitmap-set
 * `+8`'s halfword id into `gEntityFlags`" treatment already
 * documented in `UpdateEnemyCtrl`'s doc comment above and in several
 * matched sibling functions - a widely-reused "flag a nearby collision
 * bucket active" idiom, not specific to either function.
 *
 * Real C since the late NAKED retry 3 (old_agbcc,
 * docs/matching/late-naked-retry-3.md). The draft was off only in
 * reload registers and in where the layer's `1` is loaded:
 * - The first MarkGone's id compare is a reload. The ROM uses r3 for
 *   it; reload would spill r2, the lowest free register.
 *   `MarkGoneHeld` keeps r2 live across the compare with a register
 *   variable that only empty asms set and use (no code). That puts r3 in reload's spill-register set, and
 *   every later reload then rotates through the same registers as the
 *   ROM's.
 * - States 1/21/22 store the layer from an `s32 one` local, so the `1`
 *   is loaded before the `-4` mask and shared with the `gone` OR.
 *   `MarkGoneFreshBit` builds its bitmap `1` with the constant-init
 *   asm after the shift count, so it doesn't reuse `one`. */
struct player_ring {
    u8 unk_00[0x88];
    u8 ringLocked;      // 0x88
};

struct launch_obj {
    u8 unk_00[0xC];
    u8 *vtable;         // 0x0C
};

typedef void (*bd48_method_fn)(void *self, void *arg);
typedef void (*bd48_method_i_fn)(void *self, s32 arg);

static inline struct ctrl_target *SpawnAt(s32 kind, s32 x, s32 y)
{
    return sub_8025BAC(gEntitySpawner, kind, 2, x, y, 0);
}

/* MarkGone with r2 held live across the id compare (see above). */
static inline void MarkGoneHeld(struct ctrl_target *t)
{
    register s32 hold asm("r2");

    t->gone = 1;
    asm("" : "=r"(hold)); /* no code: r2 live from here */
    if (t->id != 0xFFFF)
        do {
            s32 id;
            u8 *base;
            s32 word;
            s32 off;
            u32 *slot;

            asm("" : : "r"(hold)); /* no code: ...to here */
            id = t->id;
            base = gEntityFlags;
            word = id / 32;
            off = word * 4;
            slot = (u32 *)(base + 0x108);
            slot = (u32 *)((u8 *)slot + off);
            *slot |= 1 << (id - word * 32);
        } while (0);
}

/* MarkGone whose bitmap `1` is loaded after the shift count and isn't
 * shared with an earlier 1 (see above). */
static inline void MarkGoneFreshBit(struct ctrl_target *t)
{
    t->gone = 1;
    if (t->id != 0xFFFF)
        do {
            s32 id = t->id;
            u8 *base = gEntityFlags;
            s32 word = id / 32;
            s32 off = word * 4;
            u32 *slot = (u32 *)(base + 0x108);
            s32 bit;
            s32 sh;

            slot = (u32 *)((u8 *)slot + off);
            sh = id - word * 32;
            asm("" : "=r"(bit) : "0"(1)); /* movs #1 here, not CSE'd */
            *slot |= bit << sh;
        } while (0);
}

void HitEnemy(struct part_ctrl *self, s32 unused, s32 state)
{
    if (((struct player_ring *)gPlayer)->ringLocked == 1) {
        MarkGoneHeld(self->target);
        SpawnAt(0x28, self->target->x >> 8, self->target->y >> 8);
        PlaySfx(gAudioContext, 0x5a, 0x80);
        return;
    }
    if (self->popup)
        MarkGone(self->popup);
    switch (state) {
    case 19:
    case 20:
        {
            struct launch_obj *obj = CreateKnockedEnemyCtrl(OperatorNew(0x10));
            struct part_method *m;
            struct ctrl_target *t;
            s32 a, v;

            *(struct launch_obj **)((u8 *)self->target + 0x44) = obj;
            m = (struct part_method *)(obj->vtable + 0x18);
            ((bd48_method_fn)m->fn)((u8 *)obj + m->thisOffset, self->target);
            self->target->flag7 = 0;
            t = self->target;
            if ((a = t->x) > ((struct ctrl_target *)gPlayer)->x)
                SetVelX(t, 0x1000, 0, 0x1800);
            else
                SetVelX(t, -0x1000, 0, -0x1800);
            v = ((u16)RandRange(3) << 9) - 0x200;
            SetVelY(self->target, v, 0, v);
            self->target->visible = 0;
            PlaySfx(gAudioContext, 5, 0x80);
            if (self) {
                struct part_method *m2 = &self->anchor->launch;
                ((bd48_method_i_fn)m2->fn)((u8 *)self + m2->thisOffset, 3);
            }
        }
        break;
    case 1:
    case 21:
    case 22:
        {
            struct ctrl_target *obj = SpawnAt(0x29, self->target->x >> 8, self->target->y >> 8);
            s32 one = 1;

            obj->visible = 0;
            obj->mirror.u.layer = one;
            MarkGoneFreshBit(self->target);
        }
        break;
    }
}
