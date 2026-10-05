#include "core.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem (see game_loop6.c's header comment and
 * docs/matching/issue-12-physics-collision.md). These are the two
 * functions left untouched between game_loop6.c's `sub_800D040` and
 * game_loop7.c's `sub_800E494` - the subsystem's largest, most tangled
 * dispatchers. Real bytes formerly in asm/code_3_2_17_d18c.s (now
 * deleted, fully consumed). */

/* The physics/collision subsystem's **collision-response commit**
 * function (~3840 B, not the ~1960 B this issue's write-up originally
 * estimated - see docs/matching/issue-12-physics-collision.md's
 * "Phase 1" appendix for the correction). Builds `self`'s and the
 * player's (`gPlayer`) AABBs via the shared
 * `self+0x20`-table/`self+0x2d`-tag/28-byte-stride hitbox-record
 * convention (`sub_800D040`'s own "AABB1" shape), then:
 *
 * - Looks up a per-state jump-table id from `self`'s hitbox tag (a
 *   7-case table selecting either the just-built self AABB or a
 *   fallback `gStaticData_0816B2F8` box), tests it for overlap with the
 *   player's own hitbox-record box (`AabbOverlaps`), and if it overlaps,
 *   dispatches to one of `ActivateNitroSwitchCrate`/`ActivateIronSwitchCrate`/`BreakCrateInStack`/
 *   `ExplodeCrate`/`OpenCheckpointCrate`/a flag-only case, keyed by an edge-code
 *   value looked up from `gCrateHitResponse` (`self+0x4e` row,
 *   dispatch-id column) - the 6-case jump table
 *   docs/matching/issue-12-physics-collision.md already documented from
 *   `docs/rom_map.md`'s read-only pass, now confirmed byte-for-byte
 *   (see this issue doc's dispatch-map appendix for the exact
 *   case-to-target mapping).
 * - Walks `self`'s neighbor list both directions
 *   (`GetCrateBelow`/`GetCrateAbove`) maintaining `gPlayer`'s
 *   5-slot "recently touched" object ring buffer (`+0x94` counter,
 *   `+0x98`+ array).
 * - Runs a second, larger 9-case jump table (case ids 0-8, most cases
 *   falling through to a shared tail at old ROM offset `0x0800E00C`)
 *   that further classifies the collision via `GetBottomCrate`/
 *   `GetTopCrate` ("get next"/"get prev" neighbor-list-walk-and-filter
 *   helpers, matched in game_loop30.c) and `gCrateHitResponse`,
 *   computing a final corrected offset and calling `SetEntityPos`
 *   (apply the offset) plus `_call_via_r4` (a `bx r4`
 *   register-indirect-call trampoline - see docs/rom_map.md's
 *   trampoline-table correction - used here to play a sound/particle
 *   effect through a caller-supplied function pointer).
 * - Ends by handing an ~8-argument packed position/rect off to
 *   `AddCollisionCandidate` (the apply/commit step), unless the dispatch id was
 *   6 (case skips the commit entirely) or `self+0x58` is set and
 *   `self+0x44` is clear (a very early return).
 *
 * Early-outs entirely when `self+0x4d & 0x7f == 1` or `self+0x44 != 0`.
 *
 * Real C under old_agbcc (this file is on OLD_AGBCC_OBJS; it does not
 * match under current agbcc). It was parked as NAKED asm for a long
 * time; it closed over three passes, see docs/matching/huge-naked-retry.md,
 * docs/matching/huge-naked-retry-2.md and docs/matching/huge-naked-retry-3.md
 * for what each step fixed. */
#include "phys_obj.h"

/* The player (gPlayer) as this function reads it. */
struct d18c_player
{
    s32 x;              // 0x00
    s32 y;              // 0x04
    u8 unk_08[4];
    u8 flags;           // 0x0C - bit 6
    u8 unk_0D[0xB];
    struct crate_vtable *vtable; // 0x18
    u8 unk_1C[4];
    struct anim_table *anim; // 0x20
    u8 dir;             // 0x24
    u8 unk_25[3];
    u32 unk_28_0:4;     // 0x28
    s32 flipX:1;
    s32 flipY:1;
    u32 unk_28_6:2;
    u32 unk_29:24;
    u8 unk_2C;
    u8 tag;             // 0x2D
    u8 unk_2E[0x26];
    s32 velX;           // 0x54
    s32 velY;           // 0x58
    s32 velZ;           // 0x5C
    u8 unk_60[4];
    s32 speedY;         // 0x64
    u8 standMode;       // 0x68
    u8 unk_69[0xB];
    u32 hitMask;        // 0x74
    u8 unk_78[8];
    u8 busy;            // 0x80
    u8 unk_81[7];
    u8 ringLocked;      // 0x88
    u8 unk_89[3];
    u32 timer;          // 0x8C
    u8 unk_90[2];
    u8 bounce;          // 0x92
    u8 unk_93;
    u8 ringCount;       // 0x94
    u8 unk_95[3];
    struct crate *ring[5]; // 0x98
};

#define D18C_P ((struct d18c_player *)gPlayer)
/* AddCollisionCandidate's queue, at player+0x108 (+4: "position committed"). */
#define D18C_QUEUE(p) ((u8 *)(p) + 0x108)
#define D18C_COMMIT()                                                          \
    if (1)                                                                     \
    {                                                                          \
        u8 *_q = D18C_QUEUE(D18C_P);                                           \
                                                                               \
        _q[4] = 1;                                                             \
    }                                                                          \
    else                                                                       \
        (void)0

struct d18c_level
{
    u8 unk_00[0x78];
    s32 mode;           // 0x78
};

struct d18c_quad
{
    s16 xOff;
    s16 yOff;
    u8 w;
    u8 h;
};

struct d18c_pos
{
    s32 x;
    s32 y;
};

struct d18c_flag8
{
    u8 value;
} __attribute__((packed));

extern void SetAabbPos(void *buf, s32 x, s32 y);
extern void SetAabbSize(void *buf, s32 w, s32 h);
extern s32 gStaticData_0816BBF0[];
extern u8 gCrateKindUnbreakable[];
extern u8 gStaticData_0816BF00[];
extern u8 gStaticData_0816B2F8[];
extern s32 gCrateHitResponse[][7];
extern void *GetSpriteFrame(void *part);
extern u8 AabbOverlapsInclusiveX(struct aabb *a, struct aabb *b);
extern u8 sub_800CEAC(void *self, struct d18c_quad *quad, struct aabb *box, s32 x, s32 y);
extern struct crate *sub_800CF70(struct crate *self, struct aabb *box, u8 *found);
extern struct crate *GetCrateBelow(struct crate *obj);
extern struct crate *GetCrateAbove(struct crate *obj);
extern struct crate *GetBottomCrate(struct crate *obj);
extern struct crate *GetTopCrate(struct crate *obj);
extern u8 sub_800B324(void *self);
extern void SetMaskLevel(void *self, s32 arg);
extern void sub_800E494(struct crate *self);
extern void sub_800E4E4(struct crate *self, struct aabb *box);
extern void ActivateNitroSwitchCrate(struct crate *self);
extern void ActivateIronSwitchCrate(struct crate *self);
extern void LightTntCrate(struct crate *self);
extern void BreakCrateInStack(struct crate *self, u32 a, u32 b, u32 c);
extern void ExplodeCrate(struct crate *self, u8 a);
extern void OpenCheckpointCrate(struct crate *self);
extern void AddCollisionCandidate(void *queue, struct crate *obj, s32 kind, s32 code,
                        s32 edge, s32 depth, struct d18c_pos pos, s32 hit,
                        struct d18c_flag8 f20, struct d18c_flag8 f21);

#define D18C_CALL68(a, b, c) \
    PhysCall3(D18C_P, (struct method *)&D18C_P->vtable->m68, (a), (b), (c))

/* sub_8008518 inlined: the player's current hitbox quad. */
#define D18C_HITBOX(dst, part)                                                 \
    if (1)                                                                     \
    {                                                                          \
        u8 *_info = GetSpriteFrame(part);                                         \
                                                                               \
        switch (**(u8 **)(_info + 4) >> 4)                                     \
        {                                                                      \
        case 0:                                                                \
            (dst) = (struct d18c_quad *)(_info + 0x1c);                        \
            break;                                                             \
        case 1:                                                                \
        case 2:                                                                \
        case 3:                                                                \
            (dst) = (struct d18c_quad *)gStaticData_0816B2F8;                  \
            break;                                                             \
        case 4:                                                                \
            (dst) = (struct d18c_quad *)(_info + 0x1c);                        \
            break;                                                             \
        case 5:                                                                \
        case 6:                                                                \
            (dst) = (struct d18c_quad *)gStaticData_0816B2F8;                  \
            break;                                                             \
        default:                                                               \
            (dst) = (struct d18c_quad *)gStaticData_0816B2F8;                  \
            break;                                                             \
        }                                                                      \
    }                                                                          \
    else                                                                       \
        (void)0

/* Pushes `obj` onto the player's 5-slot ring of touched boxes. */
#define D18C_RING_PUSH(obj)                                                    \
    if (1)                                                                     \
    {                                                                          \
        if (D18C_RingLocked() == 0 && D18C_P->ringCount <= 4)                 \
            D18C_P->ring[D18C_P->ringCount] = (obj);                           \
        if (D18C_P->ringLocked == 0)                                           \
            D18C_P->ringCount++;                                               \
    }                                                                          \
    else                                                                       \
        (void)0

/* Returns its argument. Writing a position's y through it (instead of a
 * `pp` pointer local) lets gcse make the ROM's pointer copy: the store goes
 * through `add r0, sp, #N` and the copy (`adds r2, r0, #0`) is used after. */
static inline struct d18c_pos *D18C_PosPtr(struct d18c_pos *p)
{
    return p;
}

/* The response code for an object kind (used in `case 8`). As an inline,
 * `tgt->kind` is loaded before the table address, as in the ROM. */
static inline s32 D18C_Code(s32 row, s32 k)
{
    return gCrateHitResponse[row][k];
}

/* The response code at the first lookup. There the ROM loads the table
 * address first, then `&obj->kind`, then adds `row * 28 + k * 4` to the
 * table: the table is an argument (all inline arguments are expanded
 * before the body), and the offset sum is written in that order. */
static inline s32 D18C_CodeIn(s32 (*t)[7], u8 *row, s32 k)
{
    return *(s32 *)((u8 *)t + (*row * 28 + k * 4));
}

/* The ring lock byte as an int, for the first test in D18C_RING_PUSH.
 * Written in place, both tests are the same expression and cse's jump
 * following sends a failed first test straight past the second one; the
 * ROM re-tests (its `bne` goes to the second load). */
static inline s32 D18C_RingLocked(void)
{
    return D18C_P->ringLocked;
}

/* The ring count as an int. The first `!= 0` test then loads it with the
 * same zero-extending load as the loop test, and cse reuses the value for
 * the loop's entry test. Written in place, shorten_compare narrows the test
 * to a QImode load and the loop test reloads it. */
static inline s32 D18C_RingCount(void)
{
    return D18C_P->ringCount;
}

static inline s32 D18C_Span(s32 a, s32 b, s32 c)
{
    return a + b - c;
}

static inline void D18C_Hit(struct d18c_player *p, u32 bit)
{
    p->hitMask |= bit;
}

static inline void D18C_SetBusy(struct d18c_player *p, u8 v)
{
    p->busy = v;
}

static inline s32 D18C_TimerOver(void)
{
    return D18C_P->timer > gRoomFrameCount;
}

/* `a` through a copy that an empty asm claims to modify (emits nothing):
 * it hides the copy's value from cse, so each use of a stack box address
 * is its own pseudo instead of one held across calls. */
#define BOX_ADDR(a) ({ struct aabb *_p = (a); asm("" : "+r"(_p)); _p; })

void sub_0800D18C(struct crate *self, s32 idx)
{
    struct
    {
        struct aabb a;
        struct aabb c;
        struct aabb b;
        u8 found;
        struct d18c_pos p1;
        struct d18c_pos p2;
        struct d18c_pos p3;
        struct d18c_pos pos;
    } f;
    s32 px;
    s32 py;
    s32 kind;
    s32 edge;
    s32 dirX;
    s32 dirY;
    s32 f21;
    s32 hit;
    s32 f20;
    struct crate *obj;
    struct crate *tgt;
    s32 code;
    s32 dx;
    s32 dy;
    s32 n;
    s32 r; /* shared by both slope checks, so both get r2 as in the ROM */
    struct d18c_quad *q;
    struct d18c_pos *pp;
    struct d18c_quad *hb;
    struct aabb *bb;
    /* &self->state, kept for the `case 1`/`case 2` test. Declared last, it
     * is the last user variable on the stack, so it takes the slot right
     * after them (0x94) and the `kind * 4` gcse temp gets 0x98, as in the
     * ROM. Left to gcse, both are temps, numbered in expression-hash order
     * (`kind * 4` first), and the slots come out swapped. */
    u8 *st;

    {
        u8 *rec = (u8 *)&self->anim->records[self->tag];
        struct d18c_quad *pb = (struct d18c_quad *)(rec + 4);
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        px = self->x >> 8;
        py = self->y >> 8;
        offX = pb->xOff;
        offY = pb->yOff;
        w = pb->w;
        h = pb->h;
        SetAabbPos(&f.a, offX + px, offY + py);
        SetAabbSize(&f.a, w, h);
        if (self->flipX)
            f.a.x = px * 2 - (f.a.x + f.a.w);
        if (self->flipY)
            f.a.y = py * 2 - (f.a.y + f.a.h);
    }
    px = D18C_P->x >> 8;
    py = D18C_P->y >> 8;
    if (((struct d18c_level *)gLevelState)->mode == 3)
        kind = 6;
    else
    {
        kind = gStaticData_0816BBF0[idx];
        if (self->kind == 0xd && kind == 5 && D18C_P->dir == 4)
            kind = 2;
    }
    {
        /* The state is loaded and masked before `st` is set, so `st` is a
         * copy of the load's address register, stored before the compare
         * (`ldrb; ands; str r1, [sp, #0x94]; cmp`). */
        s32 s = self->state & 0x7f;

        st = &self->state;
        if (s == 1)
            goto tail;
    }
    if (self->unk_44 != 0)
        goto tail;
    {
        s32 empty;

        D18C_HITBOX(hb, D18C_P);
        empty = 0;
        if (hb->w == 0)
            if (hb->h == 0)
                empty = 1;
        if (empty)
            goto tail;
    }
    {
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        offX = hb->xOff;
        offY = hb->yOff;
        w = hb->w;
        h = hb->h;
        {
            s32 x = offX + px, y = offY + py;

            SetAabbPos(BOX_ADDR(&f.b), x, y);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (D18C_P->flipX)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (D18C_P->flipY)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    bb = BOX_ADDR(&f.b);
    if (!AabbOverlaps(&f.a, bb))
        goto tail;
    f.found = 0;
    if (kind <= 4)
        obj = sub_800CF70(self, bb, &f.found);
    else
        obj = self;
    code = D18C_CodeIn(gCrateHitResponse, &obj->kind, kind);
    {
        s32 bnc = D18C_P->bounce;

        if (bnc > 4)
            code = 0;
    }
    if (f.found != 0 && D18C_RingCount() != 0)
    {
        s32 i;

        for (i = 0; i < D18C_P->ringCount; i++)
        {
            struct crate *e;

            if (D18C_P->ringLocked == 0 && (i <= 4 || i < D18C_P->ringCount))
                e = D18C_P->ring[i];
            else
                e = NULL;
            if (e != NULL)
            {
                struct crate *h = GetCrateBelow(e);

                if (h != NULL)
                {
                    while (GetCrateBelow(h) != NULL)
                        h = GetCrateBelow(h);
                }
                else
                    h = e;
                for (; h != NULL; h = GetCrateAbove(h))
                {
                    if (self == h)
                    {
                        code = 0;
                        break;
                    }
                }
            }
        }
    }
    n = 0;
    switch (code)
    {
    case 0:
    case 1:
        if (obj->kind == 6)
            ActivateNitroSwitchCrate(obj);
        else if (obj->kind == 3)
            ActivateIronSwitchCrate(obj);
        break;
    case 2:
        obj->state |= 0x80;
        D18C_SetBusy(D18C_P, 1);
        break;
    case 3:
        BreakCrateInStack(obj, 0, 0, 0);
        if (D18C_P->ringCount == 0)
        {
            u8 *rec = (u8 *)&D18C_P->anim->records[D18C_P->tag];

            if (kind != 3 && sub_800CEAC(self, (struct d18c_quad *)(rec + 4), &f.a, px, py))
            {
                struct crate *e = GetCrateAbove(obj);

                if (e != NULL && (e->state & 0x7f) != 1)
                {
                    s32 c2 = gCrateHitResponse[e->kind][kind];

                    if (c2 == 3)
                        BreakCrateInStack(e, 0, 0, 0);
                    else if (c2 == 2)
                    {
                        e->state |= 0x80;
                        D18C_SetBusy(D18C_P, 1);
                    }
                    else if (c2 == 4)
                        ExplodeCrate(e, 1);
                }
            }
            else if (D18C_P->dir != 0)
                n += 2;
        }
        if (f.found == 0)
            return;
        if ((D18C_P->x >> 8) < (self->x >> 8))
        {
            if (kind != 3 || GetCrateAbove(obj) != NULL)
            {
                D18C_CALL68(0, 0xc, 1);
                D18C_Hit(D18C_P, 1);
            }
        }
        else if (kind != 3 || GetCrateAbove(obj) != NULL)
        {
            D18C_CALL68(0, 0xc, 2);
            D18C_Hit(D18C_P, 2);
        }
        D18C_RING_PUSH(obj);
        if (D18C_P->bounce == 0)
            D18C_P->bounce = 1;
        for (; n != 0; n--)
            D18C_RING_PUSH(NULL);
        return;
    case 4:
        if ((obj->state & 0x7f) == 0)
            ExplodeCrate(obj, 1);
        return;
    case 5:
        OpenCheckpointCrate(obj);
        return;
    }
tail:
    if (kind == 5)
    {
        if (D18C_P->dir == 4)
            kind = 2;
    }
    else if (kind == 3)
        kind = 1;
    if (self->touched != 0)
    {
        self->touched = 0;
        if (self->unk_44 == 0)
            return;
    }
    {
        /* Through a pointer local: the block copy then takes a copy of it
         * (`add r2, sp, #0x2c; adds r1, r2, #0`), as in the ROM. */
        struct aabb *pc = &f.c;

        *pc = f.a;
    }
    if (kind != 6)
        sub_800E4E4(self, &f.a);
    {
        u8 *rec = (u8 *)&D18C_P->anim->records[D18C_P->tag];
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        q = (struct d18c_quad *)(rec + 4);
        offX = q->xOff;
        offY = q->yOff;
        w = q->w;
        h = q->h;
        {
            s32 x = offX + px, y = offY + py;

            SetAabbPos(BOX_ADDR(&f.b), x, y);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (D18C_P->flipX)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (D18C_P->flipY)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    bb = BOX_ADDR(&f.b);
    if (!AabbOverlapsInclusiveX(&f.a, bb))
        return;
    edge = 0;
    f21 = 0;
    if (f.b.y < f.a.y)
        f21 = 1;
    if (self->unk_44 != 0)
    {
        if (!AabbOverlapsInclusiveX(&f.c, &f.b))
            return;
        if (gCrateKindUnbreakable[self->kind] != 0)
        {
            /* `ax` (and the other path's `side`) is dead here, but the ROM
             * keeps a reload of px (`ldr r1, [sp, #0x70]`) right after the
             * call: a leftover of a compare deleted after reload. The empty
             * asm emits nothing; it only uses ax and px, which gives the
             * same reload into r1. */
            s32 ax = GetSpritePrevX((struct gobj *)D18C_P);

            asm("" : : "r"(ax), "r"(px));
            if ((D18C_P->x >> 8) < (self->x >> 8))
            {
                dirX = 1;
                dx = D18C_Span(f.b.x, f.b.w, f.c.x) + 1;
            }
            else
            {
                dirX = 2;
                dx = D18C_Span(f.c.x, f.c.w, f.b.x) + 1;
            }
            if ((D18C_P->y >> 8) > (self->y >> 8))
            {
                dirY = 4;
                dy = D18C_Span(f.c.y, f.c.h, f.b.y);
            }
            else
            {
                dirY = 8;
                dy = D18C_Span(f.b.y, f.b.h, f.c.y);
            }
            if (dx > 5 && f21 == 0)
            {
                if ((((struct d18c_level *)gLevelState)->mode == 0
                     && ((D18C_P->flags >> 6) & 1)
                     && !D18C_TimerOver())
                    || self->kind != 0xd)
                {
                    D18C_P->flags |= 0x40;
                    SetMaskLevel(gLevelState, 0);
                    D18C_CALL68(0, 0xa, 0);
                }
                else
                {
                    BreakCrateInStack(self, 0, 0, 0);
                    D18C_CALL68(0, 1, 0);
                }
                return;
            }
            else if (dx > 6 && dy > 1 && f21 != 0)
            {
                s32 y;

                f.p1.x = D18C_P->x;
                y = D18C_P->y;
                pp = &f.p1;
                pp->y = y - ((dy - 1) << 8);
                D18C_P->speedY = 0;
                SetEntityPos((struct gobj *)D18C_P, f.p1.x, pp->y);
                D18C_COMMIT();
                D18C_Hit(D18C_P, dirY);
                return;
            }
            else if (dx <= 6 && dy > 2)
            {
                s32 y;

                f.p2.x = D18C_P->x;
                y = D18C_P->y;
                D18C_PosPtr(&f.p2)->y = y;
                if (dirX == 2)
                    f.p2.x = (dx << 8) + f.p2.x;
                else if (dirX == 1)
                    f.p2.x -= dx << 8;
                SetEntityPos((struct gobj *)D18C_P, f.p2.x, D18C_PosPtr(&f.p2)->y);
                D18C_COMMIT();
                D18C_CALL68(0, 0xc, dirX);
                D18C_Hit(D18C_P, dirX);
                return;
            }
            else
            {
                s32 y;

                if (D18C_P->ringLocked != 1)
                    return;
                f.p3.x = D18C_P->x;
                y = D18C_P->y;
                D18C_PosPtr(&f.p3)->y = y;
                pp = &f.p3; /* shared with the p1 arm, where it gets r2 */
                if (dirY == 4)
                    pp->y = (dy << 8) + pp->y;
                else if (dirX == 8)
                    pp->y -= dy << 8;
                SetEntityPos((struct gobj *)D18C_P, f.p3.x, pp->y);
                D18C_COMMIT();
                D18C_CALL68(0, 0xc, dirY);
                D18C_Hit(D18C_P, dirY);
                return;
            }
        }
        else
        {
            struct crate *e;

            /* A `for` with the first call on `self`: its copy is
             * cross-jumped into the loop's call, so the ROM enters with
             * `mov r0, sl`. */
            for (e = GetCrateBelow(self); e != NULL; e = GetCrateBelow(e))
            {
                if (gCrateKindUnbreakable[e->kind] != 0 && (e->state & 0x7f) == 0)
                    return;
            }
            code = gCrateHitResponse[self->kind][5];
            dx = 0;
            dy = 0;
            dirX = 0;
            dirY = 0;
        }
    }
    else
    {
        s32 ax = GetSpritePrevX((struct gobj *)D18C_P);
        s32 ay = GetSpritePrevY((struct gobj *)D18C_P);
        s32 side = 2;

        if (px > ax)
            side = 1;
        if ((D18C_P->x >> 8) < (self->x >> 8))
        {
            dirX = 1;
            dx = D18C_Span(f.b.x, f.b.w, f.a.x) + 1;
        }
        else
        {
            dirX = 2;
            dx = D18C_Span(f.a.x, f.a.w, f.b.x) + 1;
        }
        if ((D18C_P->y >> 8) > (self->y >> 8))
        {
            dirY = 4;
            dy = D18C_Span(f.a.y, f.a.h, f.b.y);
        }
        else
        {
            dirY = 8;
            dy = D18C_Span(f.b.y, f.b.h, f.a.y);
        }
        if (ay == py)
        {
            if (ax == px)
            {
                /* `else edge = dirX` after the other arm (not a `goto
                 * edge_x`): cross-jumping later merges it into edge_x, but
                 * its reload of dirX (r0) still advances reload's
                 * round-robin, so the next arm reloads dy into r1 as the
                 * ROM does. */
                if (dy <= 2)
                {
                    edge = 4;
                    if (f21 != 0)
                        edge = 8;
                }
                else
                    edge = dirX;
            }
            /* `else edge = dirX` (not a `goto edge_x`): its reload of dirX
             * gets r6, so cross-jumping sends this arm to the r6 copy of
             * `edge = dirX` (the one the first slope check ends in), as the
             * ROM's branch does. */
            else if (dy <= 2 && (dx > 3 || !sub_800B324(D18C_P)))
            {
                edge = 4;
                if (f21 != 0)
                    edge = 8;
            }
            else
                edge = dirX;
        }
        else if (ax == px)
        {
            edge = dirY;
            if (dy > 2)
            {
                edge = dirX;
                if (dy <= 7 && dx > 3)
                    edge = dirY;
            }
        }
        else if (ay <= py && f21 != 0)
        {
            ay += q->yOff + q->h;
            /* An `if`/`else edge = dirX` (not a `goto edge_x`): the `else`
             * is the last code of this arm in the RTL. Its reload of dirX
             * takes r0 and moves reload's round-robin on, so the second
             * slope check's `ay += q->yOff` loads into r1 with r0 as the
             * scratch, as in the ROM. Cross-jumping then merges the `else`
             * into edge_x, which also reloads dirX into r0. */
            if (ay <= f.a.y + f.a.h)
            {
                edge = 0;
                if (side == 1)
                {
                    if (f.b.x + f.b.w >= f.a.x && dx > 4)
                        edge = 8;
                }
                else if (f.b.x <= f.a.x + f.a.w && dx > 4)
                    edge = 8;
                if (edge == 0)
                {
                    py += q->yOff + q->h;
                    /* The call is in each arm (cross-jumping merges the
                     * tails): the stack argument is stored before the join
                     * and px is passed from the register it was just
                     * computed in, as in the ROM. */
                    if (dirX == 1)
                    {
                        ax += q->xOff + q->w;
                        px = f.b.x + f.b.w;
                        r = sub_800FDC8(ax, ay, px, py, f.a.x);
                    }
                    else
                    {
                        ax += q->xOff;
                        px = f.b.x;
                        r = sub_800FDC8(ax, ay, px, py, f.a.x + f.a.w);
                    }
                    if ((r < 0 && f21 != 0 && dx > 4) || (r > 0 && r <= f.a.y))
                        edge = 8;
                    else
                        edge = dirX;
                }
            }
            else
                edge = dirX;
        }
        else
        {
            ay += q->yOff;
            if (ay < f.a.y)
                goto edge_x;
            {
                edge = 0;
                if (side == 1)
                {
                    if (f.b.x + f.b.w >= f.a.x && dx > 5)
                        edge = 4;
                }
                else if (f.b.x <= f.a.x + f.a.w && dx > 5)
                    edge = 4;
                if (edge == 0)
                {
                    py += q->yOff;
                    if (dirX == 1)
                    {
                        ax += q->xOff + q->w;
                        px = f.b.x + f.b.w;
                        r = sub_800FDC8(ax, ay, px, py, f.a.x);
                    }
                    else
                    {
                        ax += q->xOff;
                        px = f.b.x;
                        r = sub_800FDC8(ax, ay, px, py, f.a.x + f.a.w);
                    }
                    if (D18C_P->ringLocked == 1)
                        r += 2;
                    if ((r < 0 && f21 == 0 && dx > 5) || (r > 0 && r >= f.a.y + f.a.h))
                        edge = 4;
                    else
                        edge = dirX;
                }
            }
        }
        goto edge_done;
    edge_x:
        edge = dirX;
    edge_done:
        code = 0;
    }
    {
        s32 y;

        f.pos.x = D18C_P->x;
        y = D18C_P->y;
        D18C_PosPtr(&f.pos)->y = y;
    }
    if (dx < 0)
        dx = 0;
    if (dy < 0)
        dy = 0;
    hit = 0;
    f20 = gStaticData_0816BF00[kind];
    tgt = self;
    switch (edge)
    {
    case 0:
    case 3:
    case 5:
    case 6:
    case 7:
        break;
    case 4:
        tgt = GetBottomCrate(self);
        code = gCrateHitResponse[tgt->kind][kind];
        if (tgt->kind == 4 && kind == 2)
            code = 3;
        if (kind <= 3 || kind == 6 || (kind == 4 && code <= 2))
        {
            D18C_CALL68(0, 0xc, 4);
            D18C_Hit(D18C_P, 4);
            if (D18C_P->standMode != 8)
                D18C_PosPtr(&f.pos)->y = (dy << 8) + D18C_PosPtr(&f.pos)->y;
        }
        break;
    case 8:
        tgt = GetTopCrate(self);
        code = D18C_Code(tgt->kind, kind);
        if (kind == 4 && tgt->kind != 0xa && D18C_P->ringCount != 0)
        {
            D18C_P->speedY = 0;
            D18C_P->velX = 0;
            D18C_P->velY = 0;
            D18C_P->velZ = 0;
            code = 1;
        }
        if (code == 1 || code == 2)
        {
            D18C_PosPtr(&f.pos)->y -= (dy - 1) << 8;
            D18C_PosPtr(&f.pos)->y &= ~0xff;
            SetEntityPos((struct gobj *)D18C_P, f.pos.x, D18C_PosPtr(&f.pos)->y);
            D18C_COMMIT();
        }
        else if (code == 0 || code == 2)
            D18C_PosPtr(&f.pos)->y -= dy << 8;
        D18C_PosPtr(&f.pos)->y &= ~0xff;
        break;
    case 1:
    case 2:
        hit = dirX;
        if (!AabbOverlapsInclusiveX(&f.c, &f.b))
        {
            code = 0;
            sub_800E494(self);
            hit = 0;
        }
        else
        {
            if ((*st & 0x7f) == 0)
            {
                if (hit == 2)
                    f.pos.x += dx << 8;
                else if (hit == 1)
                    f.pos.x -= dx << 8;
            }
            if (kind > 2)
            {
                code = gCrateHitResponse[self->kind][kind];
                if (kind == 4 && code == 2)
                    code = 0;
                if (kind == 5 && code == 3)
                    f.pos.x = D18C_P->x;
            }
            else if (dy <= 4 && dx > 3 && f21 != 0)
            {
                code = gCrateHitResponse[self->kind][kind];
                if (code > 1)
                    code = 0;
            }
            else if (gCrateHitResponse[self->kind][kind] == 4)
            {
                f.pos.x = D18C_P->x;
                code = gCrateHitResponse[self->kind][kind];
            }
        }
        if (code != 1 && hit != 0)
        {
            s32 ok = 1;
            struct crate *next = GetCrateAbove(self);
            struct crate *prev = GetCrateBelow(self);
            s32 vy = D18C_P->speedY >> 8;

            if (dirY == 8 && next == NULL && (vy >= dy - 1 || dy <= 2))
                ok = 0;
            else if (dirY == 4 && prev == NULL && (vy >= dy - 1 || dy <= 2))
                ok = 0;
            if (ok)
            {
                SetEntityPos((struct gobj *)D18C_P, f.pos.x, D18C_PosPtr(&f.pos)->y);
                D18C_COMMIT();
            }
        }
        break;
    }
    if (D18C_P->ringLocked == 1 && self->kind == 0xe && code <= 1
        && AabbOverlapsInclusiveX(&f.c, &f.b) == 1)
        LightTntCrate(tgt);
    AddCollisionCandidate(D18C_QUEUE(D18C_P), tgt, kind, code, edge, dy, f.pos, hit,
                (struct d18c_flag8){f20}, (struct d18c_flag8){f21});
}

/* A further jump-table dispatcher in the same physics/collision
 * subsystem (1032 B), called only from `sub_0800D18C` (the 9-case
 * dispatch's cases 1/2/4). Takes `self` plus a dispatch id (`arg1`),
 * an edge/side value (`arg2`), a third register arg (`arg3`), and 3
 * more stack-passed byte args (per docs/rom_map.md's existing read).
 * Reads/writes several `gPlayer+0x24`/`+0x88`/`+0x92`/`+0x94`
 * fields not otherwise touched outside this subsystem, and its own
 * 6-case jump table (case ids 0-5) dispatches to the exact same
 * handler family `sub_0800D18C` itself uses -
 * `ActivateNitroSwitchCrate`/`ActivateIronSwitchCrate`/`LightTntCrate`/`BounceWumpaCrate`/
 * `BreakCrateInStack`/`ExplodeCrate`/`OpenCheckpointCrate` - confirming these really
 * are the subsystem's shared per-edge collision-response leaves, not
 * distinct per-caller logic.
 *
 * Built with old_agbcc (this file is on OLD_AGBCC_OBJS; the NAKED
 * `sub_0800D18C` above is compiler-independent). Closed in the last-five
 * NAKED retry (docs/matching/last5-naked-retry.md): the first flag byte
 * is a register union of a u32 and a one-byte struct, stored whole
 * (`str`) in the prologue, and passed as that one-byte struct to
 * BreakCrateInStack in case 3. A one-byte struct argument goes in QImode, so
 * the spilled union's low byte is reloaded with `mov r5, sp; ldrb` in
 * argument order, as in the ROM. */
#include "phys_obj.h"
extern void PlaySfx(void *ctx, s32 id, s32 volume);
extern void *gAudioContext;
extern s32 gCrateHitResponse[][7];
extern void ActivateNitroSwitchCrate(struct crate *self);
extern void ActivateIronSwitchCrate(struct crate *self);
extern void LightTntCrate(struct crate *self);
extern void BounceWumpaCrate(struct crate *self);
extern void BreakCrateInStack(struct crate *self, u32 a, u32 b, u32 c);
extern void ExplodeCrate(struct crate *self, u8 a);
extern void OpenCheckpointCrate(struct crate *self);

struct e08c_pos
{
    s32 x;
    s32 y;
};

struct flag8
{
    u8 value;
} __attribute__((packed));

#define E08C_CALL68(a, b) \
    PhysCall3(PHYS_PLAYER, (struct method *)&PHYS_PLAYER->vtable->m68, 0, (a), (b))

/* BreakCrateInStack as this caller sees it: the flag argument is a one-byte
 * struct, passed in QImode. */
extern void sub_800E7A8_flag(struct crate *self, u32 a, struct flag8 b, u32 c) asm("BreakCrateInStack");

void sub_800E08C(struct crate *self, s32 kind, s32 code, s32 edge, s32 depth,
                 struct e08c_pos pos, s32 hit, struct flag8 p20, struct flag8 p21,
                 struct flag8 pforced)
{
    union { u32 w; struct flag8 s; } f20;
    u8 f21;
    u8 forcedIn;
    u8 forced;

    f20.w = p20.value;
    f21 = p21.value;
    forcedIn = pforced.value;
    if ((self->state & 0x7f) != 0)
        goto commit;
    if (PHYS_PLAYER->ringLocked == 1 && code > 2)
    {
        pos.x = PHYS_PLAYER->x;
        pos.y = PHYS_PLAYER->y;
    }
    if ((u32)(code - 2) <= 1 || code == 5)
    {
        u8 k = self->kind;
        s32 dir = PHYS_PLAYER->dir;
        s32 d4 = dir & 4;

        if (d4 == 0)
        {
            if (k != 0xd)
            {
                if (kind == 2)
                {
                    if (k == 4 || k == 8)
                    {
                        PlaySfx(gAudioContext, 2, 0x100);
                        E08C_CALL68(0xe, 8);
                    }
                    else
                        E08C_CALL68(0xd, 8);
                    PHYS_PLAYER->speedY = 0;
                    PHYS_PLAYER->velX = 0;
                    PHYS_PLAYER->velY = 0;
                    PHYS_PLAYER->velZ = 0;
                }
                else if ((u32)(kind - 5) <= 1 && k == 8)
                {
                    PlaySfx(gAudioContext, 2, 0x100);
                    E08C_CALL68(0xe, 8);
                    PHYS_PLAYER->speedY = 0;
                    PHYS_PLAYER->velX = 0;
                    PHYS_PLAYER->velY = 0;
                    PHYS_PLAYER->velZ = 0;
                }
            }
            else if (code == 2)
            {
                self->state |= 0x80;
                {
                    u8 one = 1;

                    PHYS_PLAYER->busy = one;
                }
                code = 1;
            }
        }
    }
    if (code == 3 && self->kind == 0xf && (self->u48.n & 7) == 3)
    {
        {
            u8 e = 0xe;

            self->kind = e;
            self->u48.n = 0;
        }
        code = gCrateHitResponse[self->kind][kind];
    }
    forced = forcedIn;
    if (code == 1 && kind == 4 && PHYS_PLAYER->bounce == 1 && !(PHYS_PLAYER->dir & 0xc))
    {
        code = gCrateHitResponse[self->kind][kind];
        PHYS_PLAYER->bounce = 2;
        PHYS_PLAYER->bounce++;
        PHYS_PLAYER->bounce++;
        PHYS_PLAYER->bounce++;
        forced = 1;
    }
    switch (code)
    {
    case 0:
    case 1:
        if (!(PHYS_PLAYER->dir & 4) && f21 != 0)
        {
            if ((edge == 8 && depth <= 1) || (depth <= 1 && code == 1) || (depth <= 7 && code == 1 && edge == 8))
            {
                PHYS_PLAYER->carried = self;
                {
                    u8 m = 8;

                    PHYS_PLAYER->standMode = m;
                }
                {
                    struct e08c_pos *pp = &pos;
                    s32 y = PHYS_PLAYER->y;

                    pp->y = y - ((depth - 1) << 8);
                    hit = 0;
                    pp->x = PHYS_PLAYER->x;
                }
            }
        }
        if (code != 1)
            goto commit;
        if ((u32)(edge - 1) <= 1 && kind <= 1)
        {
            hit = 0;
            pos.x = PHYS_PLAYER->x;
        }
        if (self->kind == 6)
            ActivateNitroSwitchCrate(self);
        else if (self->kind == 3)
            ActivateIronSwitchCrate(self);
        goto commit;
    case 2:
        if (self->kind == 0xe)
            LightTntCrate(self);
        else if (self->kind == 0xc)
            BounceWumpaCrate(self);
        else
        {
            self->state |= 0x80;
            {
                struct phys_player *p = PHYS_PLAYER;
                u8 one = 1;

                p->busy = one;
            }
        }
        goto commit;
    case 3:
        if ((u32)(kind - 5) <= 1)
            BreakCrateInStack(self, 0, 0, 0);
        else if (self->unk_44 != 0)
            BreakCrateInStack(self, 0, 0, 4);
        else if (kind == 2)
            sub_800E7A8_flag(self, 0, f20.s, edge);
        else
        {
            struct phys_player **pp = (struct phys_player **)&gPlayer;

            if ((*pp)->ringCount != 0 && forced == 0)
                return;
            if (edge == 8 || edge == 4)
            {
                BreakCrateInStack(self, 0, 0, edge);
                if ((*pp)->ringLocked == 0 && (*pp)->ringCount <= 4)
                    (*pp)->ring[(*pp)->ringCount] = self;
                if (PHYS_PLAYER->ringLocked == 0)
                    PHYS_PLAYER->ringCount++;
            }
        }
        return;
    case 4:
        if ((self->state & 0x7f) == 0)
            ExplodeCrate(self, 1);
        return;
    case 5:
        OpenCheckpointCrate(self);
        return;
    }
commit:
    {
        struct phys_player *p = PHYS_PLAYER;
        u8 *q = (u8 *)p + 0x108;

        if (q[4] == 0)
            SetEntityPos((struct gobj *)p, pos.x, pos.y);
    }
    if (hit != 0)
    {
        E08C_CALL68(0xc, hit);
        PHYS_PLAYER->hitMask |= hit;
    }
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
