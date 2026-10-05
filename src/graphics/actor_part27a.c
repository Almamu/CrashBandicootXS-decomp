#include "core.h"

/* UpdateChaser: the update of the chaser that entity type 0x49 spawns
 * (sub_8021668, sprite bank 30: a running, grabbing character; only
 * room 37 places one, at its left end among 34 nitro and 21 TNT
 * crates). State 0 waits while the player is dead, state 1 runs after
 * the player, turning to face him and blowing up or breaking every
 * crate it reaches, and within range state 2 grabs him (player event 1
 * on frame 8 of anim 1).
 *
 * GitHub issue #22, ROM 0x08017AB0-0x08017ECC - the raw span between
 * actor_part27.c (ends 0x08017AAC) and actor_part27b.c (starts
 * 0x08017ECC) that docs/matching/issue-22-0x08017a44-actor.md's first
 * pass left completely untouched ("out of scope... given their size").
 * `self` (r4) is the same large per-level "player/action" object this
 * whole object family shares (`self+0xc` per-category table pointer,
 * pairs of `{s16 offset; u8 pad[2]; void *fn}` records read at fixed
 * offsets - 0x20, 0x50, 0x58 here - and fired through `_call_via_r2`/
 * `_call_via_r3`); `other` (r5) is the "part" object passed alongside
 * it, with its own analogous table at `other+0x18`. `gPlayer`
 * is the player/camera-viewport object (see docs/rom_map.md) - its
 * `+0x104` byte is the "player busy" gate this function's state 0/2
 * paths both check, and its `+0x18`-table feeds two more trampoline
 * calls (a `_call_via_r1` busy check at STATE1 entry, a `_call_via_r4`
 * call at STATE2's tail).
 *
 * **3-state dispatch on `self+8`** (anything outside 0/1/2 returns
 * immediately, matching docs/rom_map.md's existing read):
 *
 * - Prelude (every state): if `self+0x1c` (a signed timestamp/flag
 *   word) is the sentinel `-1`, fires `SetChaserMotionXFromSet(self, other, 1)`
 *   and resets it to `0`.
 * - **State 0**: bails if the player's `+0x104` busy gate is set;
 *   otherwise falls into the same "activate/deactivate table entry 3"
 *   block state 2 also reaches (see below).
 * - **State 1**: a "busy-toggle" gate on `other+0x18`'s own table
 *   (skipped - reaching the trampoline pairs directly - unless the
 *   player's `+0x104` gate is set AND its `+0x44`-object's `+8` field
 *   is `!=0x1e`), fires the `0x50`/`0x20`/`0x58`-indexed trampoline
 *   trio, then runs a `_call_via_r1` "occupied" probe (twice, toggling
 *   `self+0x20`'s latch and re-stamping `self+0x1c` from the tick
 *   counter `gRoomFrameCount` on a transition), a timeout check
 *   (`gRoomFrameCount - self+0x1c > 0x3c` ticks re-fires
 *   `SetChaserMotionXFromSet` with a mode selected by `other+0x28` bit 4 and
 *   `self+0x20`), then a screen-relative "reset entry 3's flags"
 *   double gate (X `< `/`>` viewport, mirroring `other+0x28` bits
 *   `0x10`/`0x11`) and finally an in-bounds check (`abs(dx) <=
 *   0x27FF && abs(dy) <= 0x31FF` in the player/other Q8 delta) that
 *   fires `SetChaserMotionXFromSet(self, other, 0)` plus two more trampoline
 *   calls, or - out of bounds - falls into a "spawn + scan" cluster:
 *   builds an AABB via `GetSpriteHitbox(&box, other)`, unpacks it into
 *   `CollidePartList(gCollidableList, box.x, box.y, box.w, box.h, 0,
 *   other)`, then walks the whole `gCrateList` object list -
 *   for each entry whose own table (`+0x18`, offset `0x48`) probes
 *   `==3` and is within a `0x27`/`0x3b` Q8>>8 box of `other` with
 *   `entry+0x4d` bit-`0x7f`-clear: a `entry+0x4e` tag of `0xe`/`0x13`/
 *   `0x14`/`0x15`/`0xa` calls `ExplodeCrate(entry, 0)`, otherwise
 *   `IsCrateKindBreakable(entry, tag)` gates a `BreakCrate(entry, 1)`.
 * - **State 2**: if `other+0x30==8` and `other+0x34==0` and the same
 *   in-bounds Q8 check passes, fires `_call_via_r4` against the
 *   player's own `+0x18`-table (offset `0x68`) and returns; otherwise
 *   falls through to the shared "activate/deactivate table entry 3"
 *   tail state 0 also reaches.
 * - **Shared tail** (state 0 direct, or state 2's two "otherwise"
 *   exits): gated on `other+0x38` (state-2-only) and the player's
 *   `+0x104` busy bit, either fires the `0x58`-indexed trampoline with
 *   mode 3 then the `0x50`/`0x20` pair with modes 0/1 (the "activate"
 *   shape, also `SetChaserMotionXFromSet(self, other, 1)` in place of the `0x58`
 *   call when the busy bit was never set), or - only reachable from
 *   state 2's busy-bit-set path - the `0x50`/`0x20`/`0x58` trio with
 *   modes 2/0/0 (the "deactivate" shape).
 *
 * Real C, built with old_agbcc (Makefile OLD_AGBCC_OBJS) - the same
 * compiler as actor_part_18008.c/actor_part_188d0.c after it. This used to
 * be a NAKED transcription: the two "unclosable" agbcc gaps recorded for
 * it both close under old_agbcc - the list walk's per-iteration pointer
 * reload is a guarded do-while, and the CollidePartList stack-argument order
 * comes from passing the box by value. See
 * docs/matching/issue-22-0x08018008-hopper.md. */

struct ab_method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct ab_vtable
{
    u8 unk_00[0x20];
    struct ab_method m20; // 0x20
    struct ab_method m28; // 0x28
    u8 unk_30[0x18];
    struct ab_method m48; // 0x48
    struct ab_method m50; // 0x50
    struct ab_method m58; // 0x58
    u8 unk_60[8];
    struct ab_method m68; // 0x68
};

struct ab_self
{
    u8 unk_00[8];
    s32 state;              // 0x08
    struct ab_vtable *vt;   // 0x0C
    u8 unk_10[0xC];
    s32 stamp;              // 0x1C
    u8 latch;               // 0x20
};

struct ab_ctrl
{
    u8 unk_00[8];
    s32 state;              // 0x08
};

struct ab_player
{
    s32 x;                  // 0x00
    s32 y;                  // 0x04
    u8 unk_08[0x10];
    struct ab_vtable *vt;   // 0x18
    u8 unk_1C[0x28];
    struct ab_ctrl *ctrl;   // 0x44
    u8 unk_48[0xBC];
    u8 busy;                // 0x104
};

struct ab_part
{
    s32 x;                  // 0x00
    s32 y;                  // 0x04
    u8 unk_08[0x10];
    struct ab_vtable *vt;   // 0x18
    u8 unk_1C[0xC];
    u8 flags28;             // 0x28 - bit 4: X mirror
    u8 unk_29[7];
    s32 frame;              // 0x30
    s32 unk_34;             // 0x34
    u8 animDone;            // 0x38
    u8 unk_39[0x14];
    u8 unk_4D;              // 0x4D
    u8 kind;                // 0x4E
};

struct ab_list
{
    s32 count;              // 0x00
    u8 unk_04[4];
    struct ab_part **items; // 0x08
};

struct ab_box
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

typedef void (*ab_fn1)(void *self, s32 a);
typedef void (*ab_fn2)(void *self, void *a, s32 b);
typedef void (*ab_fn3)(void *self, s32 a, s32 b, s32 c);
typedef u8 (*ab_probe)(void *self);
typedef s32 (*ab_probe_s)(void *self);

#define VCALL1(obj, m, a)                                                      \
    do                                                                         \
    {                                                                          \
        struct ab_method *_m = &(obj)->vt->m;                                  \
        ((ab_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));              \
    } while (0)
#define VCALL2(obj, m, a, b)                                                   \
    do                                                                         \
    {                                                                          \
        struct ab_method *_m = &(obj)->vt->m;                                  \
        ((ab_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } while (0)

/* virtual queries on `part`'s own method table (+0x18) */
static inline u8 Probe28(struct ab_part *p)
{
    struct ab_method *m = &p->vt->m28;

    return ((ab_probe)m->fn)((u8 *)p + m->thisOffset);
}

static inline s32 Probe48(struct ab_part *p)
{
    struct ab_method *m = &p->vt->m48;

    return ((ab_probe_s)m->fn)((u8 *)p + m->thisOffset);
}

/* The player's busy byte (+0x104). Written in place, the 0x104 offset is
 * a reload whose register rotates through r1-r3; the ROM's rotation is one
 * step off from what gcc picks for this C, so two of the three reads spell
 * out the ROM's registers (r3 for the offset, r0 for the address). */
static inline u8 Busy(struct ab_player *pl)
{
    register s32 off asm("r3") = 0x104;
    register u8 *p asm("r0");

    asm("" : "+r"(off));
    p = (u8 *)pl + off;
    return *p;
}

static inline s32 Abs(s32 v)
{
    s32 sign = v >> 31;

    return (v ^ sign) - sign;
}

extern u32 gRoomFrameCount;
extern struct ab_player *gPlayer;
extern void *gCollidableList;
extern struct ab_list *gCrateList;
extern void SetChaserMotionXFromSet(void *self, void *part, s32 index);
extern struct ab_box GetSpriteHitbox(void *obj);
extern void CollidePartList(void *manager, struct ab_box box, s32 unused, void *compareViewport);
extern void ExplodeCrate(struct ab_part *p, s32 arg);
extern u8 IsCrateKindBreakable(struct ab_part *p, s32 kind);
extern void BreakCrate(struct ab_part *p, s32 arg);

void UpdateChaser(struct ab_self *self, struct ab_part *other)
{
    struct ab_box box;

    if (self->stamp == -1)
    {
        SetChaserMotionXFromSet(self, other, 1);
        self->stamp = 0;
    }

    switch (self->state)
    {
    case 0:
        if (gPlayer->busy != 0)
            return;
    test:
        if ((s8)(other->flags28 << 3) < 0)
            goto mirrored;
        goto plain;
    case 1:
    {
        s32 i;
        s32 px;

        if (Busy(gPlayer) != 0 || gPlayer->ctrl->state == 0x1E)
        {
            VCALL2(self, m50, other, 2);
            VCALL1(self, m20, 0);
            VCALL2(self, m58, other, 0);
        }
        if (Probe28(other) && self->latch == 0)
        {
            self->stamp = gRoomFrameCount;
            self->latch = 1;
        }
        else
        {
            u8 hit = Probe28(other);

            if (hit == 0 && self->latch != 0)
            {
                self->stamp = gRoomFrameCount;
                self->latch = hit;
            }
        }
        if (self->stamp != 0 && gRoomFrameCount - self->stamp > 0x3C)
        {
            self->stamp = 0;
            if ((s8)(other->flags28 << 3) >= 0)
            {
                if (self->latch != 0)
                    SetChaserMotionXFromSet(self, other, 1);
                else
                    SetChaserMotionXFromSet(self, other, 2);
            }
            else
                SetChaserMotionXFromSet(self, other, 3);
        }
        if (gPlayer->x < other->x)
        {
            u8 f = other->flags28;

            if ((s8)(f << 3) >= 0)
            {
                s32 m = -0x11;

                m &= f;
                m |= 0x10;
                other->flags28 = m;
                VCALL2(self, m58, other, 3);
            }
        }
        /* read into a local first: in place, gcc loads it after the sum */
        px = gPlayer->x;
        if (px > other->x + 0xA00)
        {
            u8 f = other->flags28;

            if ((s8)(f << 3) < 0)
            {
                s32 m = -0x11;

                m &= f;
                other->flags28 = m;
                VCALL2(self, m58, other, 1);
            }
        }
        {
            struct ab_player *pl = gPlayer;

            if (Abs(pl->x - other->x) <= 0x27FF && Abs(pl->y - other->y) <= 0x31FF)
            {
                SetChaserMotionXFromSet(self, other, 0);
                VCALL1(self, m20, 2);
                VCALL2(self, m50, other, 1);
                return;
            }
        }
        /* The box goes to CollidePartList by value (three words in r1-r3, the
         * fourth on the stack): that is what gives the ROM's stack-argument
         * order (6th, 7th, then the box's last word). */
        box = GetSpriteHitbox(other);
        CollidePartList(gCollidableList, box, 0, other);
        /* a guarded do-while: a `for` shares the list pointer between the
         * entry test and the body, where the ROM reloads it */
        i = 0;
        if (i < gCrateList->count)
        {
            do
            {
                struct ab_part *e = gCrateList->items[i];

                if (Probe48(e) == 3)
                {
                    /* the ROM passes ExplodeCrate a copy of `e` made here,
                     * and loads `kind` straight into r1 (IsCrateKindBreakable's
                     * second argument) */
                    struct ab_part *t = e;

                    asm("" : "+r"(t));
                    if (Abs((e->x >> 8) - (other->x >> 8)) <= 0x27
                        && Abs((e->y >> 8) - (other->y >> 8)) <= 0x3B
                        && (e->unk_4D & 0x7F) == 0)
                    {
                        register s32 kind asm("r1") = e->kind;

                        if (kind == 0xE || kind == 0x13 || kind == 0x14
                            || kind == 0x15 || kind == 0xA)
                            ExplodeCrate(t, 0);
                        else if (IsCrateKindBreakable(e, kind))
                            BreakCrate(e, 1);
                    }
                }
                i++;
            } while (i < gCrateList->count);
        }
        return;
    }
    case 2:
        if (other->frame == 8 && other->unk_34 == 0)
        {
            struct ab_player *pl = gPlayer;

            if (Abs(pl->x - other->x) > 0x27FF || Abs(pl->y - other->y) > 0x31FF)
                goto test;
            {
                struct ab_method *m = &pl->vt->m68;
                void *t = (u8 *)pl + m->thisOffset;

                ((ab_fn3)m->fn)(t, 0, 1, 0);
            }
            return;
        }
        if (!other->animDone)
            return;
        if (Busy(gPlayer) == 0)
        {
            if ((s8)(other->flags28 << 3) < 0)
            {
            mirrored:
                VCALL2(self, m58, other, 3);
            }
            else
            {
            plain:
                SetChaserMotionXFromSet(self, other, 1);
            }
            VCALL2(self, m50, other, 0);
            VCALL1(self, m20, 1);
        }
        else
        {
            VCALL2(self, m50, other, 2);
            VCALL1(self, m20, 0);
            VCALL2(self, m58, other, 0);
        }
        return;
    }
}
