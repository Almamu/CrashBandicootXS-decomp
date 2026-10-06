#include "core.h"
#include "match.h"
#include "vram_pool.h"
#include "crate.h"
#include "hud.h"
#include "pickups.h"
#include "util.h"
#include "audio.h"
#include "crates.h"
#include "player.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "box_part.h"
#include "globals.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem (see crate_hit.c's header comment and
 * docs/matching/archive/issue-12-physics-collision.md). These are the two
 * functions left untouched between crate_hit.c's `BreakCrateTouchedByPlayer` and
 * crate_break.c's `ClearCrateStackTouched` - the subsystem's largest, most tangled
 * dispatchers. Real bytes formerly in asm/code_3_2_17_d18c.s (now
 * deleted, fully consumed). */

/* The physics/collision subsystem's **collision-response commit**
 * function (~3840 B, not the ~1960 B this issue's write-up originally
 * estimated - see docs/matching/archive/issue-12-physics-collision.md's
 * "Phase 1" appendix for the correction). Builds `self`'s and the
 * player's (`gPlayer`) AABBs via the shared
 * `self+0x20`-table/`self+0x2d`-tag/28-byte-stride hitbox-record
 * convention (`BreakCrateTouchedByPlayer`'s own "AABB1" shape), then:
 *
 * - Looks up a per-state jump-table id from `self`'s hitbox tag (a
 *   7-case table selecting either the just-built self AABB or a
 *   fallback `gEmptySpriteBox` box), tests it for overlap with the
 *   player's own hitbox-record box (`AabbOverlaps`), and if it overlaps,
 *   dispatches to one of `ActivateNitroSwitchCrate`/`ActivateIronSwitchCrate`/`BreakCrateInStack`/
 *   `ExplodeCrate`/`OpenCheckpointCrate`/a flag-only case, keyed by an edge-code
 *   value looked up from `gCrateHitResponse` (`self+0x4e` row,
 *   dispatch-id column) - the 6-case jump table
 *   docs/matching/archive/issue-12-physics-collision.md already documented from
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
 *   helpers, matched in crate_stack.c) and `gCrateHitResponse`,
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
 * time; it closed over three passes, see docs/matching/archive/huge-naked-retry.md,
 * docs/matching/archive/huge-naked-retry-2.md and docs/matching/archive/huge-naked-retry-3.md
 * for what each step fixed. */

/* codegen: AddCollisionCandidate's two trailing byte arguments are s32
 * in its definition (objects.h), which reads them back with `ldrb`; the
 * ROM stores them here with `strb`, as one-byte structs. docs/headers_plan.md */
extern void AddCollisionCandidate_b(struct collision_queue *queue, struct crate *obj, s32 kind,
                                    s32 code, s32 edge, s32 depth, struct e08c_pos pos, s32 hit,
                                    struct byte_arg f20,
                                    struct byte_arg f21) asm("AddCollisionCandidate");

/* AddCollisionCandidate's queue (+4: "position committed"). */
#define D18C_QUEUE(p) (&(p)->collisionQueue)
#define D18C_COMMIT()                                                          \
    if (1)                                                                     \
    {                                                                          \
        struct collision_queue *_q = D18C_QUEUE(gPlayer);                      \
                                                                               \
        _q->unk_04 = 1;                                                        \
    }                                                                          \
    else                                                                       \
        (void)0

#define D18C_CALL68(a, b, c) \
    PhysCall3(gPlayer, (struct actor_method *)&gPlayer->vtable->handleEvent, (a), (b), (c))

/* GetSpriteFrameThirdBox inlined: the player's current hitbox quad. */
#define D18C_HITBOX(dst, part)                                                 \
    if (1)                                                                     \
    {                                                                          \
        u8 *_info = GetSpriteFrame((struct gfx_part *)(part));                    \
                                                                               \
        switch (**(u8 **)(_info + 4) >> 4)                                     \
        {                                                                      \
        case 0:                                                                \
            (dst) = (struct hitbox_quad *)(_info + 0x1c);                        \
            break;                                                             \
        case 1:                                                                \
        case 2:                                                                \
        case 3:                                                                \
            (dst) = (struct hitbox_quad *)&gEmptySpriteBox;                  \
            break;                                                             \
        case 4:                                                                \
            (dst) = (struct hitbox_quad *)(_info + 0x1c);                        \
            break;                                                             \
        case 5:                                                                \
        case 6:                                                                \
            (dst) = (struct hitbox_quad *)&gEmptySpriteBox;                  \
            break;                                                             \
        default:                                                               \
            (dst) = (struct hitbox_quad *)&gEmptySpriteBox;                  \
            break;                                                             \
        }                                                                      \
    }                                                                          \
    else                                                                       \
        (void)0

/* Pushes `obj` onto the player's 5-slot ring of touched boxes. */
#define D18C_RING_PUSH(obj)                                                    \
    if (1)                                                                     \
    {                                                                          \
        if (D18C_CtrlMode() == 0 && gPlayer->listCount <= 4)                 \
            gPlayer->list[gPlayer->listCount] = (obj);                         \
        if (gPlayer->ctrlMode == 0)                                            \
            gPlayer->listCount++;                                              \
    }                                                                          \
    else                                                                       \
        (void)0

/* Returns its argument. Writing a position's y through it (instead of a
 * `pp` pointer local) lets gcse make the ROM's pointer copy: the store goes
 * through `add r0, sp, #N` and the copy (`adds r2, r0, #0`) is used after. */
static inline struct e08c_pos *D18C_PosPtr(struct e08c_pos *p)
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
static inline s32 D18C_CodeIn(const s32 (*t)[7], u8 *row, s32 k)
{
    return *(const s32 *)((const u8 *)t + (*row * 28 + k * 4));
}

/* The ring lock byte as an int, for the first test in D18C_RING_PUSH.
 * Written in place, both tests are the same expression and cse's jump
 * following sends a failed first test straight past the second one; the
 * ROM re-tests (its `bne` goes to the second load). */
static inline s32 D18C_CtrlMode(void)
{
    return gPlayer->ctrlMode;
}

/* The ring count as an int. The first `!= 0` test then loads it with the
 * same zero-extending load as the loop test, and cse reuses the value for
 * the loop's entry test. Written in place, shorten_compare narrows the test
 * to a QImode load and the loop test reloads it. */
static inline s32 D18C_RingCount(void)
{
    return gPlayer->listCount;
}

static inline s32 D18C_Span(s32 a, s32 b, s32 c)
{
    return a + b - c;
}

static inline void D18C_Hit(struct player *p, u32 bit)
{
    p->hitMask |= bit;
}

static inline void D18C_SetBusy(struct player *p, u8 v)
{
    p->busy = v;
}

static inline s32 D18C_TimerOver(void)
{
    return gPlayer->deadline > gRoomFrameCount;
}

void QueueCratePlayerCollision(struct crate *self, s32 idx)
{
    struct {
        struct aabb a;
        struct aabb c;
        struct aabb b;
        u8 found;
        struct e08c_pos p1;
        struct e08c_pos p2;
        struct e08c_pos p3;
        struct e08c_pos pos;
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
    struct hitbox_quad *q;
    struct e08c_pos *pp;
    struct hitbox_quad *hb;
    struct aabb *bb;
    /* &self->state, kept for the `case 1`/`case 2` test. Declared last, it
     * is the last user variable on the stack, so it takes the slot right
     * after them (0x94) and the `kind * 4` gcse temp gets 0x98, as in the
     * ROM. Left to gcse, both are temps, numbered in expression-hash order
     * (`kind * 4` first), and the slots come out swapped. */
    u8 *st;

    {
        u8 *rec = (u8 *)&self->anim->records[self->tag];
        struct hitbox_quad *pb = (struct hitbox_quad *)(rec + 4);
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        px = self->x >> 8;
        py = self->y >> 8;
        offX = pb->offX;
        offY = pb->offY;
        w = pb->w;
        h = pb->h;
        SetAabbPos(&f.a, offX + px, offY + py);
        SetAabbSize(&f.a, w, h);
        if (self->flipX)
            f.a.x = px * 2 - (f.a.x + f.a.w);
        if (self->flipY)
            f.a.y = py * 2 - (f.a.y + f.a.h);
    }
    px = gPlayer->x >> 8;
    py = gPlayer->y >> 8;
    if (gLevelState->maskLevel == 3)
        kind = 6;
    else {
        kind = gActionCtrlStateAttackKinds[idx];
        if (self->kind == 0xd && kind == 5 && gPlayer->dir == 4)
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
    if (self->fallDistance != 0)
        goto tail;
    {
        s32 empty;

        D18C_HITBOX(hb, gPlayer);
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

        offX = hb->offX;
        offY = hb->offY;
        w = hb->w;
        h = hb->h;
        {
            s32 x = offX + px, y = offY + py;

            SetAabbPos(BOX_ADDR(&f.b), x, y);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (gPlayer->mirror.sbits.flipX)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gPlayer->mirror.sbits.flipY)
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
        s32 bnc = gPlayer->bounce;

        if (bnc > 4)
            code = 0;
    }
    if (f.found != 0 && D18C_RingCount() != 0) {
        s32 i;

        for (i = 0; i < gPlayer->listCount; i++) {
            struct crate *e;

            if (gPlayer->ctrlMode == 0 && (i <= 4 || i < gPlayer->listCount))
                e = gPlayer->list[i];
            else
                e = NULL;
            if (e != NULL) {
                struct crate *h = GetCrateBelow(e);

                if (h != NULL) {
                    while (GetCrateBelow(h) != NULL)
                        h = GetCrateBelow(h);
                } else
                    h = e;
                for (; h != NULL; h = GetCrateAbove(h)) {
                    if (self == h) {
                        code = 0;
                        break;
                    }
                }
            }
        }
    }
    n = 0;
    switch (code) {
    case 0:
    case 1:
        if (obj->kind == 6)
            ActivateNitroSwitchCrate(obj);
        else if (obj->kind == 3)
            ActivateIronSwitchCrate(obj);
        break;
    case 2:
        obj->state |= 0x80;
        D18C_SetBusy(gPlayer, 1);
        break;
    case 3:
        BreakCrateInStack(obj, 0, 0, 0);
        if (gPlayer->listCount == 0) {
            u8 *rec = (u8 *)&gPlayer->anim->records[gPlayer->tag];

            if (kind != 3 && sub_800CEAC(self, (struct hitbox_quad *)(rec + 4), &f.a, px, py)) {
                struct crate *e = GetCrateAbove(obj);

                if (e != NULL && (e->state & 0x7f) != 1) {
                    s32 c2 = gCrateHitResponse[e->kind][kind];

                    if (c2 == 3)
                        BreakCrateInStack(e, 0, 0, 0);
                    else if (c2 == 2) {
                        e->state |= 0x80;
                        D18C_SetBusy(gPlayer, 1);
                    } else if (c2 == 4)
                        ExplodeCrate(e, 1);
                }
            } else if (gPlayer->dir != 0)
                n += 2;
        }
        if (f.found == 0)
            return;
        if ((gPlayer->x >> 8) < (self->x >> 8)) {
            if (kind != 3 || GetCrateAbove(obj) != NULL) {
                D18C_CALL68(0, 0xc, 1);
                D18C_Hit(gPlayer, 1);
            }
        } else if (kind != 3 || GetCrateAbove(obj) != NULL) {
            D18C_CALL68(0, 0xc, 2);
            D18C_Hit(gPlayer, 2);
        }
        D18C_RING_PUSH(obj);
        if (gPlayer->bounce == 0)
            gPlayer->bounce = 1;
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
    if (kind == 5) {
        if (gPlayer->dir == 4)
            kind = 2;
    } else if (kind == 3)
        kind = 1;
    if (self->touched != 0) {
        self->touched = 0;
        if (self->fallDistance == 0)
            return;
    }
    {
        /* Through a pointer local: the block copy then takes a copy of it
         * (`add r2, sp, #0x2c; adds r1, r2, #0`), as in the ROM. */
        struct aabb *pc = &f.c;

        *pc = f.a;
    }
    if (kind != 6)
        MarkCrateStackTouched(self, &f.a);
    {
        u8 *rec = (u8 *)&gPlayer->anim->records[gPlayer->tag];
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        q = (struct hitbox_quad *)(rec + 4);
        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        {
            s32 x = offX + px, y = offY + py;

            SetAabbPos(BOX_ADDR(&f.b), x, y);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (gPlayer->mirror.sbits.flipX)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gPlayer->mirror.sbits.flipY)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    bb = BOX_ADDR(&f.b);
    if (!AabbOverlapsInclusiveX(&f.a, bb))
        return;
    edge = 0;
    f21 = 0;
    if (f.b.y < f.a.y)
        f21 = 1;
    if (self->fallDistance != 0) {
        if (!AabbOverlapsInclusiveX(&f.c, &f.b))
            return;
        if (gCrateKindUnbreakable[self->kind] != 0) {
            /* `ax` (and the other path's `side`) is dead here, but the ROM
             * keeps a reload of px (`ldr r1, [sp, #0x70]`) right after the
             * call: a leftover of a compare deleted after reload. The empty
             * asm emits nothing; it only uses ax and px, which gives the
             * same reload into r1. */
            s32 ax = GetSpritePrevX((struct gfx_part *)gPlayer);

            MATCH_USE2(ax, px);
            if ((gPlayer->x >> 8) < (self->x >> 8)) {
                dirX = 1;
                dx = D18C_Span(f.b.x, f.b.w, f.c.x) + 1;
            } else {
                dirX = 2;
                dx = D18C_Span(f.c.x, f.c.w, f.b.x) + 1;
            }
            if ((gPlayer->y >> 8) > (self->y >> 8)) {
                dirY = 4;
                dy = D18C_Span(f.c.y, f.c.h, f.b.y);
            } else {
                dirY = 8;
                dy = D18C_Span(f.b.y, f.b.h, f.c.y);
            }
            if (dx > 5 && f21 == 0) {
                if ((gLevelState->maskLevel == 0 && ((gPlayer->flags.all >> 6) & 1) &&
                     !D18C_TimerOver()) ||
                    self->kind != 0xd) {
                    gPlayer->flags.all |= 0x40;
                    SetMaskLevel(gLevelState, 0);
                    D18C_CALL68(0, 0xa, 0);
                } else {
                    BreakCrateInStack(self, 0, 0, 0);
                    D18C_CALL68(0, 1, 0);
                }
                return;
            } else if (dx > 6 && dy > 1 && f21 != 0) {
                s32 y;

                f.p1.x = gPlayer->x;
                y = gPlayer->y;
                pp = &f.p1;
                pp->y = y - ((dy - 1) << 8);
                gPlayer->speedY = 0;
                SetEntityPos((struct actor *)gPlayer, f.p1.x, pp->y);
                D18C_COMMIT();
                D18C_Hit(gPlayer, dirY);
                return;
            } else if (dx <= 6 && dy > 2) {
                s32 y;

                f.p2.x = gPlayer->x;
                y = gPlayer->y;
                D18C_PosPtr(&f.p2)->y = y;
                if (dirX == 2)
                    f.p2.x = (dx << 8) + f.p2.x;
                else if (dirX == 1)
                    f.p2.x -= dx << 8;
                SetEntityPos((struct actor *)gPlayer, f.p2.x, D18C_PosPtr(&f.p2)->y);
                D18C_COMMIT();
                D18C_CALL68(0, 0xc, dirX);
                D18C_Hit(gPlayer, dirX);
                return;
            } else {
                s32 y;

                if (gPlayer->ctrlMode != 1)
                    return;
                f.p3.x = gPlayer->x;
                y = gPlayer->y;
                D18C_PosPtr(&f.p3)->y = y;
                pp = &f.p3; /* shared with the p1 arm, where it gets r2 */
                if (dirY == 4)
                    pp->y = (dy << 8) + pp->y;
                else if (dirX == 8)
                    pp->y -= dy << 8;
                SetEntityPos((struct actor *)gPlayer, f.p3.x, pp->y);
                D18C_COMMIT();
                D18C_CALL68(0, 0xc, dirY);
                D18C_Hit(gPlayer, dirY);
                return;
            }
        } else {
            struct crate *e;

            /* A `for` with the first call on `self`: its copy is
             * cross-jumped into the loop's call, so the ROM enters with
             * `mov r0, sl`. */
            for (e = GetCrateBelow(self); e != NULL; e = GetCrateBelow(e)) {
                if (gCrateKindUnbreakable[e->kind] != 0 && (e->state & 0x7f) == 0)
                    return;
            }
            code = gCrateHitResponse[self->kind][5];
            dx = 0;
            dy = 0;
            dirX = 0;
            dirY = 0;
        }
    } else {
        s32 ax = GetSpritePrevX((struct gfx_part *)gPlayer);
        s32 ay = GetSpritePrevY((struct gfx_part *)gPlayer);
        s32 side = 2;

        if (px > ax)
            side = 1;
        if ((gPlayer->x >> 8) < (self->x >> 8)) {
            dirX = 1;
            dx = D18C_Span(f.b.x, f.b.w, f.a.x) + 1;
        } else {
            dirX = 2;
            dx = D18C_Span(f.a.x, f.a.w, f.b.x) + 1;
        }
        if ((gPlayer->y >> 8) > (self->y >> 8)) {
            dirY = 4;
            dy = D18C_Span(f.a.y, f.a.h, f.b.y);
        } else {
            dirY = 8;
            dy = D18C_Span(f.b.y, f.b.h, f.a.y);
        }
        if (ay == py) {
            if (ax == px) {
                /* `else edge = dirX` after the other arm (not a `goto
                 * edge_x`): cross-jumping later merges it into edge_x, but
                 * its reload of dirX (r0) still advances reload's
                 * round-robin, so the next arm reloads dy into r1 as the
                 * ROM does. */
                if (dy <= 2) {
                    edge = 4;
                    if (f21 != 0)
                        edge = 8;
                } else
                    edge = dirX;
            }
            /* `else edge = dirX` (not a `goto edge_x`): its reload of dirX
             * gets r6, so cross-jumping sends this arm to the r6 copy of
             * `edge = dirX` (the one the first slope check ends in), as the
             * ROM's branch does. */
            else if (dy <= 2 && (dx > 3 || !HasPlayerRampYTarget(gPlayer))) {
                edge = 4;
                if (f21 != 0)
                    edge = 8;
            } else
                edge = dirX;
        } else if (ax == px) {
            edge = dirY;
            if (dy > 2) {
                edge = dirX;
                if (dy <= 7 && dx > 3)
                    edge = dirY;
            }
        } else if (ay <= py && f21 != 0) {
            ay += q->offY + q->h;
            /* An `if`/`else edge = dirX` (not a `goto edge_x`): the `else`
             * is the last code of this arm in the RTL. Its reload of dirX
             * takes r0 and moves reload's round-robin on, so the second
             * slope check's `ay += q->offY` loads into r1 with r0 as the
             * scratch, as in the ROM. Cross-jumping then merges the `else`
             * into edge_x, which also reloads dirX into r0. */
            if (ay <= f.a.y + f.a.h) {
                edge = 0;
                if (side == 1) {
                    if (f.b.x + f.b.w >= f.a.x && dx > 4)
                        edge = 8;
                } else if (f.b.x <= f.a.x + f.a.w && dx > 4)
                    edge = 8;
                if (edge == 0) {
                    py += q->offY + q->h;
                    /* The call is in each arm (cross-jumping merges the
                     * tails): the stack argument is stored before the join
                     * and px is passed from the register it was just
                     * computed in, as in the ROM. */
                    if (dirX == 1) {
                        ax += q->offX + q->w;
                        px = f.b.x + f.b.w;
                        r = FindLineCrossing(ax, ay, px, py, f.a.x);
                    } else {
                        ax += q->offX;
                        px = f.b.x;
                        r = FindLineCrossing(ax, ay, px, py, f.a.x + f.a.w);
                    }
                    if ((r < 0 && f21 != 0 && dx > 4) || (r > 0 && r <= f.a.y))
                        edge = 8;
                    else
                        edge = dirX;
                }
            } else
                edge = dirX;
        } else {
            ay += q->offY;
            if (ay < f.a.y)
                goto edge_x;
            {
                edge = 0;
                if (side == 1) {
                    if (f.b.x + f.b.w >= f.a.x && dx > 5)
                        edge = 4;
                } else if (f.b.x <= f.a.x + f.a.w && dx > 5)
                    edge = 4;
                if (edge == 0) {
                    py += q->offY;
                    if (dirX == 1) {
                        ax += q->offX + q->w;
                        px = f.b.x + f.b.w;
                        r = FindLineCrossing(ax, ay, px, py, f.a.x);
                    } else {
                        ax += q->offX;
                        px = f.b.x;
                        r = FindLineCrossing(ax, ay, px, py, f.a.x + f.a.w);
                    }
                    if (gPlayer->ctrlMode == 1)
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

        f.pos.x = gPlayer->x;
        y = gPlayer->y;
        D18C_PosPtr(&f.pos)->y = y;
    }
    if (dx < 0)
        dx = 0;
    if (dy < 0)
        dy = 0;
    hit = 0;
    f20 = gStaticData_0816BF00[kind];
    tgt = self;
    switch (edge) {
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
        if (kind <= 3 || kind == 6 || (kind == 4 && code <= 2)) {
            D18C_CALL68(0, 0xc, 4);
            D18C_Hit(gPlayer, 4);
            if (gPlayer->hitAxes != 8)
                D18C_PosPtr(&f.pos)->y = (dy << 8) + D18C_PosPtr(&f.pos)->y;
        }
        break;
    case 8:
        tgt = GetTopCrate(self);
        code = D18C_Code(tgt->kind, kind);
        if (kind == 4 && tgt->kind != 0xa && gPlayer->listCount != 0) {
            gPlayer->speedY = 0;
            gPlayer->rampY.start = 0;
            gPlayer->rampY.step = 0;
            gPlayer->rampY.target = 0;
            code = 1;
        }
        if (code == 1 || code == 2) {
            D18C_PosPtr(&f.pos)->y -= (dy - 1) << 8;
            D18C_PosPtr(&f.pos)->y &= ~0xff;
            SetEntityPos((struct actor *)gPlayer, f.pos.x, D18C_PosPtr(&f.pos)->y);
            D18C_COMMIT();
        } else if (code == 0 || code == 2)
            D18C_PosPtr(&f.pos)->y -= dy << 8;
        D18C_PosPtr(&f.pos)->y &= ~0xff;
        break;
    case 1:
    case 2:
        hit = dirX;
        if (!AabbOverlapsInclusiveX(&f.c, &f.b)) {
            code = 0;
            ClearCrateStackTouched(self);
            hit = 0;
        } else {
            if ((*st & 0x7f) == 0) {
                if (hit == 2)
                    f.pos.x += dx << 8;
                else if (hit == 1)
                    f.pos.x -= dx << 8;
            }
            if (kind > 2) {
                code = gCrateHitResponse[self->kind][kind];
                if (kind == 4 && code == 2)
                    code = 0;
                if (kind == 5 && code == 3)
                    f.pos.x = gPlayer->x;
            } else if (dy <= 4 && dx > 3 && f21 != 0) {
                code = gCrateHitResponse[self->kind][kind];
                if (code > 1)
                    code = 0;
            } else if (gCrateHitResponse[self->kind][kind] == 4) {
                f.pos.x = gPlayer->x;
                code = gCrateHitResponse[self->kind][kind];
            }
        }
        if (code != 1 && hit != 0) {
            s32 ok = 1;
            struct crate *next = GetCrateAbove(self);
            struct crate *prev = GetCrateBelow(self);
            s32 vy = gPlayer->speedY >> 8;

            if (dirY == 8 && next == NULL && (vy >= dy - 1 || dy <= 2))
                ok = 0;
            else if (dirY == 4 && prev == NULL && (vy >= dy - 1 || dy <= 2))
                ok = 0;
            if (ok) {
                SetEntityPos((struct actor *)gPlayer, f.pos.x, D18C_PosPtr(&f.pos)->y);
                D18C_COMMIT();
            }
        }
        break;
    }
    if (gPlayer->ctrlMode == 1 && self->kind == 0xe && code <= 1 &&
        AabbOverlapsInclusiveX(&f.c, &f.b) == 1)
        LightTntCrate(tgt);
    AddCollisionCandidate_b(D18C_QUEUE(gPlayer), tgt, kind, code, edge, dy, f.pos, hit,
                            (struct byte_arg){ f20 }, (struct byte_arg){ f21 });
}

/* A further jump-table dispatcher in the same physics/collision
 * subsystem (1032 B), called only from `QueueCratePlayerCollision` (the 9-case
 * dispatch's cases 1/2/4). Takes `self` plus a dispatch id (`arg1`),
 * an edge/side value (`arg2`), a third register arg (`arg3`), and 3
 * more stack-passed byte args (per docs/rom_map.md's existing read).
 * Reads/writes several `gPlayer+0x24`/`+0x88`/`+0x92`/`+0x94`
 * fields not otherwise touched outside this subsystem, and its own
 * 6-case jump table (case ids 0-5) dispatches to the exact same
 * handler family `QueueCratePlayerCollision` itself uses -
 * `ActivateNitroSwitchCrate`/`ActivateIronSwitchCrate`/`LightTntCrate`/`BounceWumpaCrate`/
 * `BreakCrateInStack`/`ExplodeCrate`/`OpenCheckpointCrate` - confirming these really
 * are the subsystem's shared per-edge collision-response leaves, not
 * distinct per-caller logic.
 *
 * Built with old_agbcc (this file is on OLD_AGBCC_OBJS; the NAKED
 * `QueueCratePlayerCollision` above is compiler-independent). Closed in the last-five
 * NAKED retry (docs/matching/archive/last5-naked-retry.md): the first flag byte
 * is a register union of a u32 and a one-byte struct, stored whole
 * (`str`) in the prologue, and passed as that one-byte struct to
 * BreakCrateInStack in case 3. A one-byte struct argument goes in QImode, so
 * the spilled union's low byte is reloaded with `mov r5, sp; ldrb` in
 * argument order, as in the ROM. */

#define E08C_CALL68(a, b) \
    PhysCall3(gPlayer, (struct actor_method *)&gPlayer->vtable->handleEvent, 0, (a), (b))

/* BreakCrateInStack as this caller sees it: the flag argument is a one-byte
 * struct, passed in QImode. */
extern void sub_800E7A8_flag(struct crate *self, u32 a, struct byte_arg b,
                             u32 c) asm("BreakCrateInStack");

void ApplyCrateCollision(struct crate *self, s32 kind, s32 code, s32 edge, s32 depth,
                         struct e08c_pos pos, s32 hit, struct byte_arg p20, struct byte_arg p21,
                         struct byte_arg pforced)
{
    union {
        u32 w;
        struct byte_arg s;
    } f20;
    u8 f21;
    u8 forcedIn;
    u8 forced;

    f20.w = p20.v;
    f21 = p21.v;
    forcedIn = pforced.v;
    if ((self->state & 0x7f) != 0)
        goto commit;
    if (gPlayer->ctrlMode == 1 && code > 2) {
        pos.x = gPlayer->x;
        pos.y = gPlayer->y;
    }
    if ((u32)(code - 2) <= 1 || code == 5) {
        u8 k = self->kind;
        s32 dir = gPlayer->dir;
        s32 d4 = dir & 4;

        if (d4 == 0) {
            if (k != 0xd) {
                if (kind == 2) {
                    if (k == 4 || k == 8) {
                        PlaySfx(gAudioContext, 2, 0x100);
                        E08C_CALL68(0xe, 8);
                    } else
                        E08C_CALL68(0xd, 8);
                    gPlayer->speedY = 0;
                    gPlayer->rampY.start = 0;
                    gPlayer->rampY.step = 0;
                    gPlayer->rampY.target = 0;
                } else if ((u32)(kind - 5) <= 1 && k == 8) {
                    PlaySfx(gAudioContext, 2, 0x100);
                    E08C_CALL68(0xe, 8);
                    gPlayer->speedY = 0;
                    gPlayer->rampY.start = 0;
                    gPlayer->rampY.step = 0;
                    gPlayer->rampY.target = 0;
                }
            } else if (code == 2) {
                self->state |= 0x80;
                {
                    u8 one = 1;

                    gPlayer->busy = one;
                }
                code = 1;
            }
        }
    }
    if (code == 3 && self->kind == 0xf && (self->u48.slotState & 7) == 3) {
        {
            u8 e = 0xe;

            self->kind = e;
            self->u48.blastState = 0;
        }
        code = gCrateHitResponse[self->kind][kind];
    }
    forced = forcedIn;
    if (code == 1 && kind == 4 && gPlayer->bounce == 1 && !(gPlayer->dir & 0xc)) {
        code = gCrateHitResponse[self->kind][kind];
        gPlayer->bounce = 2;
        gPlayer->bounce++;
        gPlayer->bounce++;
        gPlayer->bounce++;
        forced = 1;
    }
    switch (code) {
    case 0:
    case 1:
        if (!(gPlayer->dir & 4) && f21 != 0) {
            if ((edge == 8 && depth <= 1) || (depth <= 1 && code == 1) ||
                (depth <= 7 && code == 1 && edge == 8)) {
                gPlayer->carried = (struct gobj *)self;
                {
                    u8 m = 8;

                    gPlayer->hitAxes = m;
                }
                {
                    struct e08c_pos *pp = &pos;
                    s32 y = gPlayer->y;

                    pp->y = y - ((depth - 1) << 8);
                    hit = 0;
                    pp->x = gPlayer->x;
                }
            }
        }
        if (code != 1)
            goto commit;
        if ((u32)(edge - 1) <= 1 && kind <= 1) {
            hit = 0;
            pos.x = gPlayer->x;
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
        else {
            self->state |= 0x80;
            {
                struct player *p = gPlayer;
                u8 one = 1;

                p->busy = one;
            }
        }
        goto commit;
    case 3:
        if ((u32)(kind - 5) <= 1)
            BreakCrateInStack(self, 0, 0, 0);
        else if (self->fallDistance != 0)
            BreakCrateInStack(self, 0, 0, 4);
        else if (kind == 2)
            sub_800E7A8_flag(self, 0, f20.s, edge);
        else {
            struct player **pp = &gPlayer;

            if ((*pp)->listCount != 0 && forced == 0)
                return;
            if (edge == 8 || edge == 4) {
                BreakCrateInStack(self, 0, 0, edge);
                if ((*pp)->ctrlMode == 0 && (*pp)->listCount <= 4)
                    (*pp)->list[(*pp)->listCount] = self;
                if (gPlayer->ctrlMode == 0)
                    gPlayer->listCount++;
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
        struct player *p = gPlayer;
        struct collision_queue *q = &p->collisionQueue;

        if (q->unk_04 == 0)
            SetEntityPos((struct actor *)p, pos.x, pos.y);
    }
    if (hit != 0) {
        E08C_CALL68(0xc, hit);
        gPlayer->hitMask |= hit;
    }
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem (see crate_hit.c's header comment and
 * docs/matching/archive/issue-12-physics-collision.md). Compiled with
 * old_agbcc (see the Makefile's OLD_AGBCC_OBJS). */


/* Walks `obj`'s doubly-linked neighbor list both ways (`GetCrateAbove`
 * = next, `GetCrateBelow` = prev), clearing each visited neighbor's
 * `+0x58` byte whenever its own `+0x4d & 0x7f` state byte is 0.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): its scheduler is what
 * puts the `0x7f` mask immediate before the `ldrb`, the gap that kept
 * this NAKED before. The goto-into-loop shape reproduces the ROM's
 * "jump straight to the call with r0 = arg" loop entry. */
void ClearCrateStackTouched(struct crate *obj)
{
    u8 *cur;
    void *arg;

    arg = obj;
    goto next;
    do {
        if ((cur[0x4d] & 0x7f) == 0)
            cur[0x58] = 0;
        arg = cur;
    next:
        cur = (u8 *)GetCrateAbove(arg);
    } while (cur != NULL);
    arg = obj;
    goto prev;
    do {
        if ((cur[0x4d] & 0x7f) == 0)
            cur[0x58] = 0;
        arg = cur;
    prev:
        cur = (u8 *)GetCrateBelow(arg);
    } while (cur != NULL);
}

/* Same bidirectional-neighbor walk as `ClearCrateStackTouched` above, but sets
 * `+0x58` to 1 and, for the forward (`GetCrateAbove`) direction only,
 * also debits `ctx+4` and credits `ctx+0xc` by `ctx+0xc`'s *original*
 * value (`step`, cached once before the loops); the reverse direction
 * only credits `ctx+0xc`. The per-loop `one` local is what makes gcc
 * hoist the constant into a callee-saved register (r7, then r6) like
 * the ROM; a literal `1` isn't hoisted and the loop-entry call gets
 * cross-jumped away. Built with old_agbcc. */
void MarkCrateStackTouched(struct crate *obj, struct aabb *ctxArg)
{
    s32 *ctx = (s32 *)ctxArg;
    s32 step = ctx[3];
    u8 *cur;

    cur = (u8 *)GetCrateAbove(obj);
    if (cur != NULL) {
        u8 one = 1;
        do {
            if ((cur[0x4d] & 0x7f) == 0) {
                cur[0x58] = one;
                ctx[1] -= step;
                ctx[3] += step;
            }
            cur = (u8 *)GetCrateAbove((struct crate *)cur);
        } while (cur != NULL);
    }
    cur = (u8 *)GetCrateBelow(obj);
    if (cur != NULL) {
        u8 one = 1;
        do {
            if ((cur[0x4d] & 0x7f) == 0) {
                cur[0x58] = one;
                ctx[3] += step;
            }
            cur = (u8 *)GetCrateBelow((struct crate *)cur);
        } while (cur != NULL);
    }
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");

/* GitHub issue #12 Phase 2: 0x0800E560-0x0800EEF0, the lower-address
 * half of the remaining tail of the physics/collision subsystem's
 * per-edge handler family (see docs/matching/archive/issue-12-physics-collision.md's
 * "Phase 1" appendix for the confirmed dispatch map both
 * QueueCratePlayerCollision/ApplyCrateCollision, src/crates/crate_break.c, dispatch into).
 * `self` throughout is the same "collision box" object every other
 * function in this subsystem operates on (`struct crate`,
 * include/crate.h). Compiled with old_agbcc (the Makefile's
 * OLD_AGBCC_OBJS) - see docs/matching/archive/issue-12-physics-collision.md's
 * NAKED-retry section. */

/* The effect object SpawnEffectPart spawns (only the fields set here). */
struct phys_puff {
    u8 unk_00[0xC];
    u8 unk_0C_0:2; // 0x0C
    u8 hidden:1;
    u8 unk_0C_3:5;
    u8 unk_0D[0x1B];
    u8 unk_28_0:4; // 0x28
    u8 flipX:1;
    u8 unk_28_5:3;
    u8 unk_29[0x2B];
    s32 velX;   // 0x54
    s32 accelX; // 0x58
    s32 accelY; // 0x5C
    u8 unk_60[4];
    s32 velY; // 0x64
};

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

/* Dispatch-id-5 handler. Both `QueueCratePlayerCollision`'s and `ApplyCrateCollision`'s
 * per-edge jump tables' case 3 eventually reach this handler
 * transitively (via `BreakCrateInStack`), see
 * docs/matching/archive/issue-12-physics-collision.md's dispatch map.
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

    if (self->u48.bounceTimer == -0x2a) {
        self->u48.bounceTimer = 0x168;
        self->paramB = 0;
    }
    if (self->u48.bounceTimer > 0) {
        if (self->paramA == 0) {
            if (++self->paramB > 4) {
                BreakCrateInStack(self, 0, 0, 0);
            } else {
                self->state |= 0x80;
                {
                    struct player *p = gPlayer;
                    u8 one = 1;

                    p->busy = one;
                }
                self->timer = 6;
                self->paramA = 1;
            }
            {
                MATCH_HOLD_REG(u8 *, p5, r5);
                MATCH_HOLD_REG(u8, one, r4);

                {
                    s32 x = self->x >> 8;
                    s32 y = (self->y >> 8) - 6;

                    // clang-format off
                    SPAWN_CALL(gEntitySpawner, x, y, (*(volatile s32 *)&argP4 = 0xe, ({
                        p5 = (u8 *)&argP5;
                        one = 1;
                        *p5 = one;
                        0;
                    }), 0));
                    // clang-format on
                }
                {
                    s32 x = (self->x >> 8) + 3;
                    s32 y = self->y >> 8;

                    // clang-format off
                    SPAWN_CALL(gEntitySpawner, x, y, (*(volatile s32 *)&argP4 = 0, ({
                        *p5 = one;
                        0;
                    }), 0));
                    // clang-format on
                }
            }
        }
    } else {
        BreakCrateInStack(self, 0, 0, 0);
    }
}

/* Case-2 handler ("dispatch id 0xe") both `QueueCratePlayerCollision`'s and
 * `ApplyCrateCollision`'s per-edge jump tables select - see
 * docs/matching/archive/issue-12-physics-collision.md's dispatch map. Switches
 * `self` into a fresh sub-state (`+0x4e = 0x15`, hitbox tag `+0x2d =
 * 0x14`), rebuilds its hitbox record (`ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/
 * `SetSpriteAnimDone`, the same trio every hitbox-rebuild call in this
 * subsystem uses), registers it with the object-pool grid
 * (`LinkCrateToActiveBucket`), re-derives a low-nibble sub-animation value from
 * the freshly selected hitbox record's `+0x14` byte via
 * `GetPaletteSlot`'s tile-asset-cache lookup, plays SFX `0x11`, and
 * arms a `+0x4f` countdown of `0x3c` (60) frames. */
void LightTntCrate(struct crate *selfArg)
{
    u8 *self = (u8 *)selfArg;
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
        MATCH_HOLD_REG(u8 *, addr2d, r5);

        // clang-format off
        asm volatile(
            "mov r0, #0x14\n\t"
            "add r5, %1, #0\n\t"
            "add r5, r5, #0x2d\n\t"
            "strb r0, [r5]\n\t"
            : "=r"(addr2d)
            : "r"(self)
            : "r0", "cc", "memory"
        );
        // clang-format on

        ResetSpriteFrameTimer(self);
        ResetSpriteFrameIndex(self);
        SetSpriteAnimDone(self, 0);
        /* Anchored: the ROM materializes the `0x10` immediate before
         * loading `self[0xc]`, not after - a plain C `self[0xc] |=
         * 0x10;` (in either operand order) always loads the field
         * first here. */
        {
            MATCH_HOLD_REG(u32, flagsResult, r0);

            // clang-format off
            asm volatile(
                "mov r0, #0x10\n\t"
                "ldrb r1, [%1, #0xc]\n\t"
                "orr r0, r1\n\t"
                "strb r0, [%1, #0xc]\n\t"
                : "=r"(flagsResult)
                : "r"(self)
                : "r1", "cc", "memory"
            );
            // clang-format on
        }
        LinkCrateToActiveBucket(gCrateList, (struct box_part *)self);

        {
            MATCH_HOLD_REG(u8 **, p2, r0) = *(u8 ***)(self + 0x20);
            MATCH_HOLD_REG(u8 *, table2, r1) = *p2;
            MATCH_HOLD_REG(u8, tag2, r2) = *addr2d;
            MATCH_HOLD_REG(u8 *, entry2, r1);

            // clang-format off
            asm volatile(
                "lsl r0, %2, #3\n\t"
                "sub r0, r0, %2\n\t"
                "lsl r0, r0, #2\n\t"
                "add %0, %1, r0\n\t"
                : "=r"(entry2)
                : "r"(table2), "r"(tag2)
                : "r0", "cc"
            );
            // clang-format on
            entry = entry2;
        }
    }
    lo = GetPaletteSlot(gPaletteCache, entry[0x14]);
    /* Empty compiler barrier: forces the u8->u32 zero-extend implied by
     * `lo`'s use below to happen as its own step (matching the ROM's
     * `lsls r0,r0,0x18; lsrs r0,r0,0x18`), rather than letting the
     * optimizer fuse it into the `& 0xf` mask below into a single
     * shift-mask-shift sequence. */
    MATCH_KEEP_VOLATILE(lo);
    /* Anchored: the ROM computes `&self[0x29]` *before* masking `lo`
     * down to its low nibble (a plain C `self[0x29] = (self[0x29] &
     * ~0xf) | (lo & 0xf);` here always computes the mask first
     * regardless of source statement order), and materializes the
     * `~0xf` clear-mask at runtime (`movs r1,#0x10; rsbs r1,r1,#0`,
     * the negative-constant register-pinned mask idiom - see
     * matching_decomp_register_pinning and DrawCrate's own use of
     * it, crate_draw.c) rather than folding it into an 8-bit AND
     * immediate, ORing into the mask register (not the freshly-
     * extracted low-nibble register) before storing - transcribed as
     * one block to pin the whole sequence's order and registers at
     * once. */
    {
        MATCH_HOLD_REG(u8, rawLo, r0) = lo;

        // clang-format off
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
        // clang-format on
    }

    PlaySfx(gAudioContext, 0x11, 0x100);
    self[0x4f] = 0x3c;
}

/* Case-5 handler both `QueueCratePlayerCollision`'s and `ApplyCrateCollision`'s per-edge
 * jump tables select unconditionally - see
 * docs/matching/archive/issue-12-physics-collision.md's dispatch map. Spawns
 * a particle-effect object (`SpawnEffectPart`, kind `0x2a`) at `self`'s
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

        puff = SpawnEffectPart(gEntitySpawner, 0x2a, 0, x, y, 0);
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
    SetCheckpointAtPlayer(gLevelState, self->paramA != 0);
    self->state &= 0x7f;
    gPlayer->busy = 0;
    one = 1;
    self->state = (self->state & 0x80) | one;
}

/* Case-3 handler both `QueueCratePlayerCollision`'s and `ApplyCrateCollision`'s per-edge
 * jump tables select (see docs/matching/archive/issue-12-physics-collision.md's
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
        if (gPlayer->countdown != 0)
            return;
        gPlayer->countdown = 1;
        gPlayer->countdown++;
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
 * docs/matching/archive/issue-12-physics-collision.md's dispatch map. Early-
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
 * `OpenMysteryCrate`/`OpenSlotCrate`/`ExplodeCrate`/`FreezeLevelClock`, or a
 * SFX-3-plus-particle-spawn fallback) before converging on a shared
 * epilogue.
 *
 * Real C under old_agbcc (the old "r8/sb accumulators" note was wrong;
 * see docs/matching/archive/issue-12-13-25-naked-retry.md): the tag switch goes
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
    LinkCrateToActiveBucket(gCrateList, (struct box_part *)self);
    self->state &= 0x7f;
    gPlayer->busy = 0;
    one = 1;
    self->state = (self->state & 0x80) | one;
    PhysSetTag(self, 0x1d);
    {
        struct anim_rec *recs = self->anim->records;
        struct anim_rec *rec = &recs[self->tag];

        self->slot = GetPaletteSlot(gPaletteCache, rec->paletteId);
    }
    PhysSetFrame(self, 3);
    if (gCrateKindCounted[self->kind])
        AddBrokenCrate(gLevelState);
    {
        s32 id = self->id;
        u8 *base = (u8 *)gEntityFlags;
        s32 word = id / 32;
        s32 off = word * 4;
        u32 *slot = (u32 *)(base + 0x108);

        slot = (u32 *)((u8 *)slot + off);
        *slot |= one << (id - word * 32);
    }
    DropCratesAbove(self);
    switch (self->kind) {
    case 2:
        if (flag == 0)
            OpenAkuAkuCrate(self);
        break;
    case 9:
        if (flag == 0)
            OpenLifeCrate((struct actor *)self, chained);
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
            OpenSlotCrate(self, chained);
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
 * id `0xb`) - see docs/matching/archive/issue-12-physics-collision.md's
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
    if (self->paramB == 9) {
        u8 r = (u16)rand() >> 8;

        if (r <= 0x56)
            self->paramB = 1;
        else if (r <= 0xd3)
            self->paramB = 4;
        else if (r <= 0xec)
            self->paramB = 7;
        else
            self->paramB = 8;
    }
    switch (self->paramB) {
    case 10:
        {
            s32 x = self->x >> 8;
            s32 y = self->y >> 8;

            // clang-format off
            SPAWN_CALL(gEntitySpawner, x, y, (*(volatile s32 *)&argP4 = 0xff, ({
                MATCH_HOLD_REG(u8 *, p, r4) = (u8 *)&argP5;
                MATCH_HOLD_REG(u8, v, r3) = 0;
                *p = v;
                0;
            }), 0));
            // clang-format on
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
            struct player *p = gPlayer;

            if (p->flags.all >> 7) {
                PhysCall3(p, (struct actor_method *)&p->vtable->handleEvent, 0, 0x1a, 0);
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
 * id `0xf`) - see docs/matching/archive/issue-12-physics-collision.md's
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
    BONUS_CALL(gEntitySpawner, x, y, (*(volatile s32 *)p4 = 3, *(volatile u8 *)p5 = flag, 0));
}

void OpenSlotCrate(struct crate *self, u32 arg1)
{
    s32 argP4;
    u32 argP5;
    u8 flag = arg1;

    PlaySfx(gAudioContext, 3, 0x100);
    switch (self->u48.slotState & 7) {
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
 * docs/matching/archive/issue-12-physics-collision.md's dispatch map. Derives
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
 * `n->fallTargetY` is written in both arms of an if/else. */
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
    const u8 *tbl = gCrateKindExplosive;

    if (self->fallDistance != 0)
        delta = -4;
    if (n == NULL || self == NULL)
        return;
    if (self->fallDistance != 0)
        n->fallTargetY = self->fallTargetY;
    else
        n->fallTargetY = self->y;
    spread = n->fallTargetY;
    spread -= n->y;
    if (spread < 0)
        spread = 0;
    carry = 0;
    while (n != NULL) {
        s32 t;

        if (n->fallDistance != 0) {
            n->fallDistance = spread + carry;
            n->fallTargetY = n->fallTargetY + carry;
        } else {
            n->fallDistance = spread;
            n->fallTargetY = n->y + base;
        }
        t = n->fallSpeed;
        if (t > 0)
            t = 0;
        {
            s32 d = delta;

            n->fallSpeed = t + d;
        }
        n->flags |= 0x10;
        LinkCrateToActiveBucket(gCrateList, (struct box_part *)n);
        if (tbl[n->kind] && self->u48.blastState == 0 && n->fallDistance > 0x1600) {
            struct crate *next = GetCrateAbove(n);
            struct crate *prev = GetCrateBelow(n);

            if (next == NULL && prev != NULL) {
                if (n->kind != 10)
                    goto advance;
                if (n->state & 0x7f)
                    goto advance;
            }
            n->timer = 0;
            n->u48.blastState = 1;
        }
    advance:
        n = GetCrateAbove(n);
        if (carry == 0)
            carry = base;
    }
}

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem (see crate_hit.c's header comment and
 * docs/matching/archive/issue-12-physics-collision.md). Phase 2, higher-address
 * half: the twelve functions from `ExplodeCrate` through `UpdateSlotCrate`
 * (0x0800EEF0-0x0800FC70, the end of this whole cluster), all direct or
 * transitive callees of `QueueCratePlayerCollision`'s and `ApplyCrateCollision`'s per-edge
 * jump table (crate_break.c) - see that issue doc's "Phase 2 grouping
 * hint" for the confirmed dispatch map this group is built from. Real
 * bytes formerly the tail of asm/code_3_2_17_e560.s (from
 * `ExplodeCrate` onward - the head, `BounceWumpaCrate` through `DropCratesAbove`,
 * is a sibling pass's territory and untouched here).
 *
 * Compiled with old_agbcc (the Makefile's OLD_AGBCC_OBJS): all twelve
 * are real C (see docs/matching/archive/issue-12-physics-collision.md's
 * NAKED-retry sections). */

/* Per-edge jump table's **case 4 handler**
 * (`QueueCratePlayerCollision(self+0x4d & 0x7f == 0) -> ExplodeCrate(self, 1)`, and
 * `ApplyCrateCollision`'s own case 4, per crate_break.c's confirmed dispatch
 * map). Also called by several of this file's own sibling functions
 * (`BlastNearbyCrates`, `DetonateNitroCrates`, `BreakCratesInArea`, `UpdateTntCountdown`) whenever
 * their own overlap/state checks land on the same "commit an edge
 * collision" outcome, always with `arg1` (a `u8`) as either 0 or 1.
 *
 * Early-outs when `self+0x4d & 0x7f == 1` (already committed). Clears
 * `self+0x4f`, clears `self+0x4d`'s low 7 bits, resets
 * `gPlayer+0x80`, sets `self+0xc` bit `0x10` (a "collision
 * response active" render/update flag matched elsewhere in this
 * subsystem), and calls `LinkCrateToActiveBucket(gCrateList, self)` (adds
 * `self` back onto the shared active-object list). Sets `self+0x4d`'s
 * `0x80` bit unconditionally, then ORs in `arg1` on top of that -
 * `arg1` ends up as the low bit of `self+0x4d`.
 *
 * If `self+0x4e == 0xa` (a specific collision state id), rewinds
 * `self+0x4e` by `0x21` (`0xa -> ... ` - stores through
 * `self+0x4e - 0x21`, i.e. a computed offset elsewhere in `self`'s
 * struct) instead of the usual `self+0x2d = 0x21` tag write, then in
 * either case calls the `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone`
 * triplet (the "set tag, refresh sprite/animation" idiom shared by
 * every state-transition function in this cluster - see
 * `ActivateNitroSwitchCrate`/`ActivateIronSwitchCrate`/`SolidifyOutlineCrate`/`UpdateTntCountdown` below for the
 * same three-call pattern).
 *
 * Looks up `gCrateKindCounted[self+0x4e]` and, if nonzero, calls
 * `AddBrokenCrate(gLevelState)` (an external subsystem, unread -
 * likely a screen-shake/particle trigger). Sets a bit in
 * `gEntityFlags`'s 32x32 collision-cell bitmap from `self+8`'s
 * position (`>>5` row, `&0x1f` column - the same cell-grid convention
 * `QueueCratePlayerCollision` itself uses for `gPlayer`'s own state), then
 * plays a fixed sound (`gAudioContext`, id 4). Calls
 * `DropCratesAbove(self)` (already matched elsewhere in this cluster - a
 * sibling's territory).
 *
 * Tail: reads `gRoomFrameCount`'s `+0xc` byte bit `0x40` (a "combo
 * scoring active" flag elsewhere in the ROM); if set, and
 * `gPlayer+0x8c` (a running counter) hasn't exceeded
 * `gRoomFrameCount`'s threshold, and `self`'s position is within 0x1d
 * px of `gPlayer`'s (the player) on both axes (or `arg1`
 * itself is 0), calls `_call_via_r4` (the `bx r4` sound/particle
 * trampoline from `self+0x18+0x68`) with args `(0, 4, 0)`. Finally, if
 * `self+0x4e != 0xa`, forces `self+0x4e = 0x13` (a shared "settle"
 * state most of this cluster's state machines converge on - see
 * `UpdateTntCountdown` below). */

static inline s32 PhysComboMaxed(struct player *player)
{
    s32 maxed = FALSE;

    if (player->deadline > gRoomFrameCount)
        maxed = TRUE;
    return maxed;
}

void ExplodeCrate(struct crate *self, u8 near)
{
    u8 one;

    if ((self->state & 0x7f) == 1)
        return;

    self->timer = 0;
    self->state &= 0x7f;
    gPlayer->busy = 0;
    self->flags |= 0x10;
    LinkCrateToActiveBucket(gCrateList, (struct box_part *)self);
    one = 1;
    self->state = (self->state & 0x80) | one;
    if (self->kind == 0xa) {
        self->tag = one;
        ResetSpriteFrameTimer(self);
        ResetSpriteFrameIndex(self);
        SetSpriteAnimDone(self, 0);
    } else
        PhysSetTag(self, 0x21);
    if (gCrateKindCounted[self->kind])
        AddBrokenCrate(gLevelState);
    PHYS_SET_ID_BIT(self->id);
    PlaySfx(gAudioContext, 4, 0x100);
    DropCratesAbove(self);

    if ((gPlayer->flags.all >> 6) & 1 && !PhysComboMaxed(gPlayer)) {
        struct player *p;
        s32 t1 = (gPlayer->x >> 8) - (self->x >> 8);
        s32 dx = (t1 ^ (t1 >> 31)) - (t1 >> 31);

        if (dx <= 0x1d) {
            s32 t2 = (gPlayer->y >> 8) - (self->y >> 8);
            s32 dy = (t2 ^ (t2 >> 31)) - (t2 >> 31);

            if (dy <= 0x1d)
                goto call;
        }
        if (near) {
        call:
            p = gPlayer;
            PhysCall3(p, (struct actor_method *)&p->vtable->handleEvent, 0, 4, 0);
        }
    }
    if (self->kind != 0xa)
        self->kind = 0x13;
}

/* Called from `FinishBrokenCrate` (`BlastNearbyCrates(self, 0x14)` /
 * `BlastNearbyCrates(self, 0x28)`, gated on `self+0x30 == 3` / `== 6`) with
 * `arg1` a small proximity-radius constant (0x14 or 0x28 px). Walks
 * `gCrateList`'s whole object list twice:
 *
 * - First pass: for every other object whose `_call_via_r1`
 *   overlap-classification against `self` returns `3` (a "close enough
 *   to interact" code shared with several siblings below) and whose
 *   Chebyshev-ish `|dx|+|dy|` distance to `self` is within `arg1`,
 *   looks up `gCrateKindExplosive[other+0x4e]`: if nonzero, calls
 *   `BreakCrateInStack(other, 1, 0, 0)`; else if the *other* object's own
 *   `gCrateKindBreakable` byte (keyed by that lookup's result) is set,
 *   calls `BreakCrateInStack(other, 1, 0, 0)`; else dispatches on that byte's
 *   value (`3` -> `ActivateIronSwitchCrate(other)`, `6` -> `ActivateNitroSwitchCrate(other)`) -
 *   but only when `other+0x4d & 0x7f == 0` and it's not already flagged
 *   via `gUnknown_030012EC`. (Every object that passes the overlap
 *   check but isn't otherwise routed still gets `ExplodeCrate(other, 0)`
 *   when `gUnknown_030012EC[other+0x4e]` is nonzero, before falling
 *   into that dispatch.)
 * - Second pass over `gUnknown_030012EC`'s smaller secondary list:
 *   objects with `_call_via_r1 == 2` and the same distance gate get
 *   `PickUpWumpa(other, 1)` (already matched elsewhere) and an
 *   `other+0xc` bit-`0x10` set (same render/update flag `ExplodeCrate`
 *   sets above).
 *
 * Ends by resetting `self+0x48` to `-1` (0xFFFFFFFF), a sentinel this
 * whole cluster uses for "no pending sub-state timer". */

void BlastNearbyCrates(struct crate *self, s32 dist)
{
    s32 i = 0;

    if (i < gCrateList->activeCount) {
        u32 commit = (u32)gCrateKindExplosive;

        do {
            struct crate *o = (struct crate *)gCrateList->slotArray[i];

            if (PHYS_CALL(o, m48) == 3) {
                s32 t1 = (o->x >> 8) - (self->x >> 8);
                s32 dx = (t1 ^ (t1 >> 31)) - (t1 >> 31);
                s32 t2 = (o->y >> 8) - (self->y >> 8);
                s32 dy = (t2 ^ (t2 >> 31)) - (t2 >> 31);

                if (dx + dy <= dist && (o->state & 0x7f) == 0) {
                    u32 kind = o->kind;

                    if (*(u8 *)(kind + commit))
                        ExplodeCrate(o, 0);
                    else if (gCrateKindBreakable[kind])
                        BreakCrateInStack(o, 1, 0, 0);
                    else if (kind == 3)
                        ActivateIronSwitchCrate(o);
                    else if (kind == 6)
                        ActivateNitroSwitchCrate(o);
                }
            }
            i++;
        } while (i < gCrateList->activeCount);
    }

    i = 0;
    if (i < gUnknown_030012EC->count) {
        struct part_list **list = &gUnknown_030012EC;

        do {
            struct crate *o = (struct crate *)(*list)->items[i];

            if (PHYS_CALL(o, m48) == 2) {
                s32 t1 = (o->x >> 8) - (self->x >> 8);
                s32 dx = (t1 ^ (t1 >> 31)) - (t1 >> 31);
                s32 t2 = (o->y >> 8) - (self->y >> 8);
                s32 dy = (t2 ^ (t2 >> 31)) - (t2 >> 31);

                if (dx + dy <= dist) {
                    PickUpWumpa((struct orbit_part *)o, 1);
                    o->flags |= 0x10;
                }
            }
            i++;
        } while (i < (*list)->count);
    }
    self->u48.blastState = 0xff;
}

/* Takes no arguments - a pure `gCrateList` list-scan helper,
 * called from the still-raw `IsSwitchPressed`/`0x08023A1C` caller elsewhere
 * (outside this issue's scope). First calls `DetonateNitroCrates` (below) to
 * settle any pending case-`0xa` collisions, then loops
 * `gCrateList` up to twice (an outer `do { ... } while
 * (gCrateListChanged)` driven by a one-shot re-scan flag stored at
 * `gCrateListChanged`): for every object whose `_call_via_r1`
 * classification against `self` is `3` and whose `+0xc` bit `1` is set,
 * calls `RemoveCrateListAt(list, index)` (an already-elsewhere-matched
 * list-removal helper) and, if that object is still non-NULL
 * afterward, `_call_via_r2(other, 3, ...)` (a variant of the
 * `_call_via_r1` overlap-classifier that also *mutates* state, per the
 * `3` id) - decrementing the loop index to re-visit the same slot next
 * iteration since the list just shrank. Objects that overlap but don't
 * have that `+0xc` flag instead get a plain `_call_via_r1` call against
 * a *different* box (`other+0x18+0x18`/`+0x1c`, not `+0x48`/`+4`) with
 * no further action - just a classification side effect. */

void UpdateCrates(void)
{
    s32 i;

    DetonateNitroCrates();
    do {
        gCrateListChanged = 0;
        for (i = 0; i < gCrateList->activeCount; i++) {
            struct crate *o = (struct crate *)gCrateList->slotArray[i];

            if (PHYS_CALL(o, m48) == 3) {
                if (o->flags & 1) {
                    RemoveCrateListAt(gCrateList, i);
                    if (o != NULL)
                        PHYS_CALL1(o, m50, 3);
                    i--;
                } else {
                    PHYS_CALL(o, m18);
                }
            }
        }
    } while (gCrateListChanged);
}

/* Takes no arguments. A short `gCrateList` list-scan: for every
 * object whose `_call_via_r1` overlap-classification against `self` is
 * `3`, whose `+0x4e` state is `0xa`, and whose `+0x4d & 0x7f` is clear,
 * calls `ExplodeCrate(other, 0)` - i.e. settles any object still parked
 * in the "pending edge-4 commit, state 0xa" condition `ExplodeCrate`
 * itself creates (see that function's own doc comment above). Called
 * as the first step of both `UpdateCrates` (above) and `ActivateNitroSwitchCrate`
 * (below), always as a "flush anything left over from a previous
 * frame" pass before running this frame's own dispatch. */

void DetonateNitroCrates(void)
{
    s32 i = 0;

    if (i < gCrateList->activeCount) {
        struct pool_manager **list = &gCrateList;

        do {
            struct crate *o = (struct crate *)(*list)->slotArray[i];

            if (PHYS_CALL(o, m48) == 3 && o->kind == 0xa) {
                if ((o->state & 0x7f) == 0)
                    ExplodeCrate(o, 0);
            }
            i++;
        } while (i < (*list)->activeCount);
    }
}

/* Per-edge jump table's **case 0/1 handler when the dispatch-id row is
 * 6** (`QueueCratePlayerCollision`'s/`ApplyCrateCollision`'s shared case 0/1 target - see
 * crate_break.c's confirmed dispatch map). Early-outs when `self+0x48`
 * is already nonzero (a pending sub-state timer, same field
 * `BlastNearbyCrates` resets to `-1`).
 *
 * Sets `self+0x4d` bit `0x80`, `gPlayer+0x80 = 1`, tags
 * `self+0x2d = 0x23` (a "bounced/deflected" state constant, matching
 * this cluster's numbering - `ActivateIronSwitchCrate` below uses `0x22` for a
 * closely related case), then runs the
 * `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` "set tag, refresh
 * sprite/animation" triplet every state-transition function in this
 * cluster shares. Looks up `self`'s hitbox-record row
 * (`self+0x20`-table/`self+0x2d`-tag/28-byte-stride, this subsystem's
 * standard AABB convention) and calls `GetPaletteSlot` with its `+0x14`
 * byte to compute a direction/animation nibble, folded into `self+0x29`
 * (low nibble replaced, high nibble kept - `(x & 0xf) | (old & ~0xf)`).
 * Calls `DetonateNitroCrates` (flush any pending case-0xa commits), then
 * `ShowHudCrates(gHud)` (external, unread - likely a score
 * or combo-counter bump) and plays a fixed sound
 * (`gAudioContext`, id 4). Sets `self+0x48 = 1` (arms the sub-state
 * timer `BlastNearbyCrates` later drains back to `-1`) and calls
 * `PressSwitchCrate(gLevelState)` (external, unread). */

void ActivateNitroSwitchCrate(struct crate *self)
{
    if (self->u48.pressed == 0) {
        s32 one;
        struct anim_rec *recs;
        struct anim_rec *rec;

        self->state |= 0x80;
        {
            struct player *player = gPlayer;
            one = 1;
            player->busy = one;
        }
        PhysSetTag(self, 0x23);
        recs = self->anim->records;
        rec = &recs[self->tag];
        self->slot = GetPaletteSlot(gPaletteCache, rec->paletteId);
        DetonateNitroCrates();
        ShowHudCrates(gHud);
        PlaySfx(gAudioContext, 4, 0x100);
        self->u48.pressed = one;
        PressSwitchCrate(gLevelState);
    }
}

/* Per-edge jump table's **case 0/1 handler when the dispatch-id row is
 * 3** (the sibling of `ActivateNitroSwitchCrate` above, same dispatch-map entry, and
 * also called directly by `BlastNearbyCrates`'s/`BreakCratesInArea`'s/
 * `BreakCrateTouchedByPlayer`'s own dispatch). Early-outs when `self+0x48` is already
 * `-1` or `0` cleared to the "already handled" sentinels (i.e. only
 * proceeds while it's some other in-progress value) - the inverse
 * early-out shape from `ActivateNitroSwitchCrate`'s simple "nonzero" check.
 *
 * Sets `self+0xc` bit `0x10`, calls `LinkCrateToActiveBucket(gCrateList,
 * self)` (re-adds `self` to the active list, same call `ExplodeCrate`
 * makes), sets `self+0x4d` bit `0x80` and `gPlayer+0x80 = 1`,
 * tags `self+0x2d = 0x22` (this case's own state constant), and runs
 * the same `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` triplet plus the
 * `GetPaletteSlot`-driven `self+0x29` nibble update `ActivateNitroSwitchCrate` uses.
 * Calls `sub_8025A0C(gEntityFlags, self+8)` (marks `self`'s
 * position in the same 32x32 collision-cell bitmap `ExplodeCrate`
 * touches).
 *
 * Then walks `gCrateList`'s whole list a *second* time (distinct
 * from the `_call_via_r1`-classification passes above): collects up to
 * 0x20 other objects whose `_call_via_r1` result is `3`, `+0x4d & 0x7f
 * == 0`, `+0x4e == 5`, and `+0x50` matches `self+0x50`, into a local
 * stack array, calling `sub_8025A0C` on each of *their* positions too.
 * If any were collected, allocates a heap block sized for the count
 * (`OperatorNewArray`), sets `self+0x59 = 1`, and copies the collected
 * pointer array into it before storing the block at `self+0x48` -
 * building a "linked group of simultaneously-triggered neighbors" list.
 * If none were collected, `self+0x48` gets the `-1` sentinel instead.
 * Tail: clears `self+0x4f` and copies `self+0x4c`'s byte into
 * `self+0x4f` (per-object throttle fields also touched by
 * `SolidifyOutlineCrates`/`SolidifyOutlineCrate` below). */

void ActivateIronSwitchCrate(struct crate *self)
{
    struct crate *found[32];
    s32 n = 0;
    s32 i;

    if (self->u48.group == PHYS_NO_GROUP)
        return;
    if (self->u48.group != NULL)
        return;

    self->flags |= 0x10;
    LinkCrateToActiveBucket(gCrateList, (struct box_part *)self);
    self->state |= 0x80;
    {
        struct player *player = gPlayer;
        u8 one = 1;
        player->busy = one;
    }
    PhysSetTag(self, 0x22);
    {
        struct anim_rec *recs = self->anim->records;
        struct anim_rec *rec = &recs[self->tag];

        self->slot = GetPaletteSlot(gPaletteCache, rec->paletteId);
    }
    sub_8025A0C(gEntityFlags, self->id);

    i = 0;
    if (i < gCrateList->activeCount) {
        do {
            struct crate *o = (struct crate *)gCrateList->slotArray[i];

            if (PHYS_CALL(o, m48) == 3 && (o->state & 0x7f) == 0) {
                if (o->kind == 5 && o->paramA == self->paramA) {
                    found[n] = o;
                    n++;
                    n &= 0x1f;
                    sub_8025A0C(gEntityFlags, o->id);
                }
            }
            i++;
        } while (i < gCrateList->activeCount);
    }

    if (n != 0) {
        struct crate_group *g = OperatorNewArray((n + 1) * 4);

        self->groupAllocated = 1;
        g->count = n;
        for (i = 0; i < n; i++)
            ((struct crate **)g)[i + 1] = found[i];
        self->u48.group = g;
    } else {
        self->u48.group = PHYS_NO_GROUP;
    }
    self->paramA = 0;
    self->timer = self->fallSpeed;
}

/* Called from `SolidifyOutlineCrate`'s own jump-table-driven state machine
 * indirectly via re-entry (see below) and from the still-raw
 * `0x080104E4` continuation (outside this issue's scope) whenever
 * `self+0x4e` is in `0x13`-`0x15`. Bumps `self+0x50` (a per-object
 * "successive triggers" counter) and compares it against `self+0x51`
 * (a per-object cap). Once the cap is reached: if `self+0x48` (the
 * linked-group pointer `ActivateIronSwitchCrate` builds) holds more than one
 * element, calls `OperatorDeleteArray` (frees it, already-elsewhere-matched);
 * resets `self+0x48` to `-1` and `self+0x4e = 7` (a distinct "group
 * exhausted" state).
 *
 * While still under the cap: if `self+0x48`'s group has more than one
 * member, walks every member whose own `+0x4e == 5` and `+0x51 <=
 * self`'s own cached `+0x4c` throttle byte, calling `SolidifyOutlineCrate`
 * (below) on each - recursively settling every other object in the
 * same triggered group - and plays a single shared sound
 * (`gAudioContext`, id 0xf) the first time any member is actually
 * settled this call (a `once`-flag local keeps it from repeating per
 * member). */

void SolidifyOutlineCrates(struct crate *self)
{
    if (self->timer != 0)
        return;

    if (++self->paramA >= self->paramB) {
        struct crate_group *g = self->u48.group;

        if (PHYS_HAS_GROUP(g)) {
            if (g != NULL)
                OperatorDeleteArray(g);
            self->groupAllocated = 0;
        }
        self->u48.group = PHYS_NO_GROUP;
        {
            u8 kind = 7;
            self->kind = kind;
        }
    } else {
        struct crate_group *g = self->u48.group;

        if (PHYS_HAS_GROUP(g)) {
            s32 i;
            s32 n = g->count;
            struct crate **items = g->items;
            s32 played = FALSE;

            for (i = 0; i < n; i++) {
                struct crate *o = items[i];

                if (o->kind == 5 && self->paramA >= o->paramB) {
                    SolidifyOutlineCrate(o);
                    if (!played) {
                        PlaySfx(gAudioContext, 0xf, 0x100);
                        played = TRUE;
                    }
                }
            }
        }
        self->timer = self->fallSpeed;
    }
}

/* Called by `SolidifyOutlineCrates` above (settling every member of a triggered
 * group) and directly from the still-raw `0x080104E4` continuation for
 * `self+0x4e` in `0x13`-`0x15` (outside this issue's scope). Decrements
 * `self+0x48` by `0x15` into `self+0x4e` (reusing the incoming state id
 * as a byte offset into itself - a compact re-dispatch trick this
 * function uses instead of a separate id field), then runs a
 * 0x13-entry jump table (`self+0x4e` post-subtraction, `bhi` gated at
 * `0x12`) that maps each of 15 distinct sub-cases to one of a small set
 * of `self+0x2d` tag constants (`0x1f, 0x1a, 0x17, 0x18, 4, 0x20, 2, 5,
 * 0x19 (with a nested self+0x48 = -0x2a store), 6, 0x11, 0xe, 0xf, 0x10
 * (a distinct "no group" tag)`), each falling into the same shared
 * `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` triplet, plus 4 cases
 * (`3, 5, 9, 11`, per the raw table's own indices) that skip straight
 * to the tail instead. Tail (`self+0x29` nibble update via
 * `GetSpriteAnimPaletteSlot(self)`) matches the same `(x & 0xf) | (old & ~0xf)` fold
 * `ActivateNitroSwitchCrate`/`ActivateIronSwitchCrate` use, just via a different lookup helper
 * (`GetSpriteAnimPaletteSlot` instead of `GetPaletteSlot` directly - presumably an
 * already-classified variant). */

void SolidifyOutlineCrate(struct crate *self)
{
    self->kind = self->u48.solidKind - 0x15;
    switch (self->kind) {
    case 0:
        PhysSetTag(self, 0x1f);
        break;
    case 1:
        PhysSetTag(self, 0x1a);
        break;
    case 2:
        PhysSetTag(self, 0x17);
        break;
    case 4:
        PhysSetTag(self, 0x18);
        break;
    case 6:
        PhysSetTag(self, 4);
        break;
    case 7:
        PhysSetTag(self, 0x20);
        break;
    case 8:
        PhysSetTag(self, 2);
        break;
    case 10:
        PhysSetTag(self, 5);
        break;
    case 12:
        self->u48.bounceTimer = -0x2a;
        PhysSetTag(self, 0x19);
        break;
    case 13:
        PhysSetTag(self, 6);
        break;
    case 14:
        PhysSetTag(self, 0x11);
        break;
    case 16:
        PhysSetTag(self, 0xe);
        break;
    case 17:
        PhysSetTag(self, 0xf);
        break;
    case 18:
        PhysSetTag(self, 0x10);
        break;
    }
    self->slot = GetSpriteAnimPaletteSlot((struct actor *)self);
}

/* `BreakCratesInArea(s32 x, s32 y, s32 arg2, s32 arg3)` - the one function in
 * this group taking a raw position/box instead of a `self` pointer (see
 * `src/player/action_ctrl_hang.c`'s existing extern: called as
 * `BreakCratesInArea(part->x >> 8, part->y >> 8, 0x40, 0x12)`, a fixed
 * 0x40x0x12 probe box around an actor-part's own position). Walks
 * `gCrateList`'s whole list: for every object whose
 * `_call_via_r1` classification against the probe box is `3`, whose
 * Chebyshev distance is within `(arg2, arg3)` on X/Y respectively, and
 * whose `+0x4d & 0x7f == 0`, looks up
 * `gCrateKindExplosive[other+0x4e]`: if that row's
 * `gCrateKindBreakable` byte is set, dispatches `1` ->
 * `OpenCheckpointCrate(other)`, else `BreakCrateInStack(other, 0, 0, 0)`; if the row
 * itself is `0`, calls `ExplodeCrate(other, 0)` instead. The same
 * "settle nearby objects against a probe box" shape as
 * `BlastNearbyCrates`/`FinishBrokenCrate`, just driven by an explicit box rather
 * than `self`'s own hitbox record. */

void BreakCratesInArea(s32 x, s32 y, s32 dist, s32 height)
{
    s32 i = 0;

    if (i < gCrateList->activeCount) {
        const u8 *commit = gCrateKindExplosive;

        do {
            struct crate *o = (struct crate *)gCrateList->slotArray[i];

            if (PHYS_CALL(o, m48) == 3) {
                s32 t1 = (o->x >> 8) - x;
                s32 dx = (t1 ^ (t1 >> 31)) - (t1 >> 31);
                s32 t2 = (o->y >> 8) - y;
                s32 dy = (t2 ^ (t2 >> 31)) - (t2 >> 31);

                if (dx + dy <= dist && dy < height && (o->state & 0x7f) == 0) {
                    if (*(u8 *)(o->kind + (u32)commit))
                        ExplodeCrate(o, 0);
                    else if (gCrateKindBreakable[o->kind]) {
                        if (o->kind == 1)
                            OpenCheckpointCrate(o);
                        else
                            BreakCrateInStack(o, 0, 0, 0);
                    }
                }
            }
            i++;
        } while (i < gCrateList->activeCount);
    }
}

/* Called from the still-raw `0x080104E4` continuation
 * (`self+0x4d & 0x7f == 1` case, outside this issue's scope) - the
 * per-edge jump table's shared entry point once `self`'s own commit is
 * already underway. Looks up `gCrateKindExplosive[self+0x4e]`: if
 * nonzero and `self+0x34 == 0` (no pending sub-effect), dispatches on
 * `self+0x30` (`3` -> `BlastNearbyCrates(self, 0x14)`, `6` ->
 * `BlastNearbyCrates(self, 0x28)` - see that function's own doc comment).
 *
 * If `self+0x38` is set (a "linked to neighbors" flag), re-links
 * `self`'s `GetCrateBelow`/`GetCrateAbove` neighbor-list pointers
 * (`SetCrateBelow`/`SetCrateAbove`, already-elsewhere-matched splice
 * helpers) to remove `self` from the list. Unless `self+0x4e == 1`,
 * marks `self` "visited this frame" in `gCrateListChanged`'s per-cell
 * bitmap (the same 32x32-grid convention `ExplodeCrate`/`ActivateIronSwitchCrate`
 * use, here against `gEntityFlags`) and walks
 * `gPlayer+0x94`'s "recently touched" ring buffer
 * (`QueueCratePlayerCollision`'s own 5-slot buffer, per crate_break.c's doc comment)
 * clearing each slot's `+0x94` re-visit flag once it matches `self`.
 *
 * If `self+0x38` was clear instead, and `self+0x4e != 1`, sets
 * `gCrateListChanged = 1` (a one-shot "re-scan next pass" flag -
 * `UpdateCrates`'s own outer loop condition above) unconditionally. */

static inline struct crate *PhysRingAt(struct player *p, s32 i)
{
    if (p->ctrlMode == 0 && (i <= 4 || i < p->listCount))
        return p->list[i];
    return NULL;
}

void FinishBrokenCrate(struct crate *self)
{
    if (gCrateKindExplosive[self->kind] && self->stepTimer == 0) {
        if (self->frame == 3)
            BlastNearbyCrates(self, 0x14);
        else if (self->frame == 6)
            BlastNearbyCrates(self, 0x28);
    }

    if (self->animDone) {
        struct crate *prev = GetCrateBelow(self);
        struct crate *next = GetCrateAbove(self);
        s32 i;

        if (prev != NULL && next != NULL) {
            SetCrateBelow(next, prev);
            SetCrateAbove(prev, next);
        } else if (next != NULL) {
            SetCrateBelow(next, NULL);
        } else if (prev != NULL) {
            SetCrateAbove(prev, NULL);
        }

        if (self->kind == 1)
            return;
        gCrateListChanged = 1;
        PHYS_GONE(self) = 1;
        if (self->id != 0xffff)
            PHYS_SET_ID_BIT(self->id);
        i = 0;
        if (i < gPlayer->listCount) {
            struct player **pp = &gPlayer;

            do {
                if (PhysRingAt(*pp, i) == self)
                    (*pp)->listCount = 0;
                i++;
            } while (i < (*pp)->listCount);
        }
    } else if (self->kind != 1) {
        gCrateListChanged = 1;
    }
}

/* Called from `ExplodeCrate` indirectly (both converge on
 * `self+0x4e` settling to `0x13`) and reachable from the per-edge
 * dispatch whenever a settled object's state lands in `0x13`-`0x15`.
 * Early-outs when `self+0x4f` (the per-object throttle byte
 * `ActivateIronSwitchCrate` seeds from `self+0x4c`) is already nonzero. Otherwise
 * dispatches on `self+0x4e`:
 *
 * - `0x14`: tags `self+0x2d = 0x12`, runs the
 *   `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` triplet, plays a sound
 *   (`gAudioContext`, id 0x11), then falls into the shared tail
 *   with `self+0x4e = 0x13`, `self+0x4f = 0x3c` (a ~1-second cooldown
 *   at 60 fps).
 * - `> 0x14` (only `0x15` reaches here, `bgt` from the `0x14` compare):
 *   same triplet + sound + tail, but tags `0x13` first and re-enters
 *   with `self+0x4e = 0x14` instead - a one-step state regression
 *   rather than the terminal settle the `0x14` case takes.
 * - `0x13`: if `self+0x4d & 0x7f == 0`, calls `ExplodeCrate(self, 0)` -
 *   the same "commit the edge collision" call the per-edge dispatch
 *   itself makes, closing the loop back into `ExplodeCrate` above.
 * - anything else: no-op. */

void UpdateTntCountdown(struct crate *self)
{
    u8 kind;

    if (self->timer != 0)
        return;

    kind = self->kind;
    switch (kind) {
    case 0x15:
        PhysSetTag(self, 0x13);
        PlaySfx(gAudioContext, 0x11, 0x100);
        self->kind = 0x14;
        self->timer = 0x3c;
        break;
    case 0x14:
        PhysSetTag(self, 0x12);
        PlaySfx(gAudioContext, 0x11, 0x100);
        self->kind = 0x13;
        self->timer = 0x3c;
        break;
    case 0x13:
        if ((self->state & 0x7f) == 0)
            ExplodeCrate(self, 0);
        break;
    }
}

/* The largest and last function in this cluster
 * (0x0800F990-0x0800FC70, ~736 B). Called from the still-raw
 * `0x080104E4` continuation (`self+0x4e == 0xf`, outside this issue's
 * scope) - a **per-frame position-wrap/edge-scan advance**, structurally
 * similar to the already-parked `UpdateCrateFall`
 * (docs/matching/archive/issue-13-fc70-continuation.md) that immediately
 * follows this whole cluster.
 *
 * First, unless `self+0x48` already has its `0xc0` high bits set,
 * clamps `self`'s position to within 0x4f/0x3f px of
 * `gPlayer` (the player) on X/Y respectively, folding the
 * result into `self+0x48`'s packed byte (`(x & 0x3f) | 0x40`, masked
 * against `0xc7`, then `| 0x10`) - a "snap into range" step. Early-outs
 * entirely (jumps to the tail) once `self+0x4f` is nonzero.
 *
 * The bulk of the function is a small state cycle keyed by
 * `self+0x48 & 7`, advanced via `(x+1) & 3` each call and masked back
 * into `self+0x48`'s low 3 bits - a 4-phase rotation (only entered when
 * `self+0x2d == 8`, a specific tag this cluster's other functions write
 * via the `ResetSpriteFrameTimer` triplet) that dispatches each phase (`0`, `1`,
 * `2`, `3`, sub-split further by the *previous* phase value in a nested
 * compare) into per-phase blocks. These re-tag `self+0x2d`, re-run the
 * `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` triplet, and (per the
 * `0xc0`-bit branch taken near the top) call `GetSlotCrateSpins(self)` -
 * already matched elsewhere (`crate_stack.c` family) - to decide
 * whether the phase cycle continues or the object's position gets
 * finally committed. Given the size and self-contained nature of this
 * state cycle (no calls out to any other function in this cluster), a
 * full branch-by-branch semantic write-up was not attempted for this
 * pass. */

/* Matched under old_agbcc (third near-miss sweep). The phase test
 * (`w1`), the loop (`lw`) and the `0x38` switch (`w2`) each have their
 * own local, and the empty asm below gives `lw` an extra reference so it
 * wins r1 over `nx`. The count update is written as separate in-place
 * steps on a fresh local (`t = (r - 1) << 24; cw &= 0xc7; t >>= 21;
 * cw |= t`), which ties the `& 0xc7` to the reloaded word's register and
 * the shift to `t`'s, as the ROM does; a single `(w & 0xc7) | (t << 3)`
 * expression left 7 halfwords off. */
void UpdateSlotCrate(struct crate *self)
{
    s32 w;
    s32 ph0;
    s32 w1;

    w = self->u48.slotState;
    if (!(w & 0xc0)) {
        struct player *pl = gPlayer;
        s32 d;

        d = pl->x >> 8;
        d -= self->x >> 8;
        if (d < 0)
            d = -d;
        if (d <= 0x4f) {
            d = pl->y >> 8;
            d -= self->y >> 8;
            if (d < 0)
                d = -d;
            if (d <= 0x3f) {
                w &= 0x3f;
                w |= 0x40;
                w &= 0xc7;
                w |= 0x10;
                self->u48.slotState = w;
            }
        }
    }
    if (self->timer != 0)
        return;
    ph0 = self->u48.slotState & 7;
    ph0 &= 4;
    w1 = self->u48.slotState;
    if (ph0 && self->tag == 8) {
        s32 done = 0;

        do {
            s32 ph;
            s32 nx;
            s32 lw;

            lw = self->u48.slotState;
            nx = ((lw & 7) + 1) & 3;
            ph = nx;
            lw = (lw & 0xf8) | nx;
            MATCH_USE(lw); /* extra reference: lw wins r1 over nx */
            self->u48.slotState = lw;
            switch (ph) {
            case 0:
                PhysSetTag(self, 7);
                if (self->u48.slotState & 0xc0) {
                    u8 r = GetSlotCrateSpins(self);

                    if (r != 0) {
                        u32 t = (r - 1) << 24;
                        s32 cw = self->u48.slotState;

                        cw &= 0xc7;
                        t >>= 21;
                        cw |= t;
                        self->u48.slotState = cw;
                    }
                    w = self->u48.slotState;
                    if (!(w & 0x38)) {
                        s32 w2 = (w & 0xc7) | 0x10;

                        self->u48.slotState = w2;
                        switch ((s32)((u32)(w2 & 0xc0) >> 6)) {
                        case 1:
                            self->u48.slotState = (w2 & 0x3f) | 0x80;
                            break;
                        case 2:
                            self->u48.slotState = (w2 & 0x3f) | 0xc0;
                            break;
                        case 3:
                            PhysSetTag(self, 0x20);
                            self->kind = 7;
                            break;
                        }
                    }
                }
                goto out;
            case 1:
                if (self->paramA & 2) {
                    PhysSetTag(self, 9);
                    goto out;
                }
                break;
            case 2:
                if (self->paramA & 1) {
                    PhysSetTag(self, 0xb);
                    goto out;
                }
                break;
            case 3:
                if (self->paramA & 4) {
                    PhysSetTag(self, 0xd);
                    done = 1;
                }
                break;
            }
        } while (!done);
    out:
        {
            struct anim_rec *recs = self->anim->records;
            struct anim_rec *rec = &recs[self->tag];

            self->slot = GetPaletteSlot(gPaletteCache, rec->paletteId);
        }
        {
            s32 d = (s32)((u32)(self->u48.slotState & 0xc0) >> 6);

            self->timer = gSlotCrateTimers[d];
        }
    } else {
        {
            s32 p = (w1 & 7) | 4;

            w1 = p | (w1 & 0xf8);
        }
        self->u48.slotState = w1;
        self->timer = 1;
        if (self->tag == 0xc)
            PhysSetTag(self, 0xa);
        else if (self->tag == 0xa)
            PhysSetTag(self, 8);
        else {
            switch ((s32)((u32)(self->u48.slotState & 0xc0) >> 6)) {
            case 0:
            case 1:
                PhysSetTag(self, 0xc);
                break;
            case 2:
                PhysSetTag(self, 0xa);
                break;
            case 3:
                PhysSetTag(self, 8);
                break;
            }
        }
        {
            struct anim_rec *recs = self->anim->records;
            struct anim_rec *rec = &recs[self->tag];

            self->slot = GetPaletteSlot(gPaletteCache, rec->paletteId);
        }
        if (self->u48.slotState & 0xc0)
            PlaySfx(gAudioContext, 0x10, 0x100);
    }
}

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see crate_reset.c's header comment and
 * docs/matching/archive/issue-13-graphics-fc70.md). */

/* A per-frame position-wrap advance: `self+0x4c` is a signed "speed"
 * (defaulting to 1 when 0), `self+0x44` a signed Q8-ish countdown
 * ("remaining"). When `remaining == 0` the whole function is a no-op.
 * Otherwise it sets the global one-shot flag byte `gCrateListChanged`,
 * then loops, once per unit of `speed`, folding `remaining` toward
 * zero by +-0x100 (or +-0x40, when the viewport's own
 * `gPlayer`-relative `+0x88` byte reads 1 - a "half speed"
 * mode) while accumulating the matching step into `self+0x4`; each
 * time `remaining` crosses to <=0 it re-derives the wrap via the
 * `gCrateKindExplosive[self+0x4e]` per-state table and, depending on
 * that table's value and `self+0x48`/`self+0x4d`'s state, dispatches
 * `ExplodeCrate`/`LightTntCrate` on `self` and its whole `GetCrateBelow`
 * "get next" neighbor-list chain (the same list `ResetCrate`/
 * `ResolvePlayerCollisions`, crate_reset.c/crate.c, already establish). At
 * the end, `self+0x4`'s accumulated step is folded into `self`'s own
 * position (`self+0`/`self+4`), and `self+0x4c`'s "speed" byte is
 * either cleared (when `remaining` ended up exactly 0) or incremented
 * toward a clamped max of 5 (saturating, never decremented back down
 * by this function).
 *
 * Matches under old_agbcc (the NAKED note blamed the "two extra
 * high-register accumulators"; see
 * docs/matching/archive/issue-12-13-25-naked-retry.md). What mattered: the
 * speed byte is re-read through `self->unk_4C` each time (GCSE keeps
 * its address in sb), `speed--` is written in both step arms, the
 * neighbour walk skips the first neighbour, and one temporary `t` both
 * carries `fallTargetY` into `y` and re-reads `x` at the bottom of the loop
 * (the ROM's r1). */
void UpdateCrateFall(struct crate *self)
{
    s32 remaining = self->fallDistance;
    s32 speed;
    s32 acc;
    s32 t;

    if (remaining == 0)
        return;
    gCrateListChanged = 1;
    speed = self->fallSpeed;
    if (speed == 0)
        speed = 1;
    acc = 0;
    t = self->x;
    if (speed < 0) {
        do {
            acc -= 0x100;
            remaining += 0x100;
            speed++;
        } while (speed != 0);
    } else {
        do {
            if (remaining > 0) {
                if (gPlayer->ctrlMode == 1) {
                    acc += 0x40;
                    remaining -= 0x40;
                    speed--;
                } else {
                    acc += 0x100;
                    remaining -= 0x100;
                    speed--;
                }
            } else {
                struct crate *n;
                u8 kind;

                speed = 1;
                t = self->fallTargetY;
                self->y = t;
                acc = 0;
                self->fallDistance = remaining;
                if (gCrateKindExplosive[kind = self->kind]) {
                    if (self->u48.blastState != 0 || kind == 10) {
                        if ((self->state & 0x7f) == 0)
                            ExplodeCrate(self, 0);
                    } else if (kind == 0xe) {
                        struct crate *next = GetCrateAbove(self);
                        struct crate *prev = GetCrateBelow(self);

                        if (next != NULL || prev == NULL)
                            LightTntCrate(self);
                    }
                }
                n = GetCrateBelow(self);
                speed--;
                if (n != NULL) {
                    n = GetCrateBelow(n);
                    while (n != NULL) {
                        if (n->kind == 0xe)
                            LightTntCrate(n);
                        n = GetCrateBelow(n);
                    }
                }
            }
            t = self->x;
        } while (speed != 0);
    }
    {
        s32 y = self->y + acc;

        self->x = t;
        self->y = y;
    }
    self->fallDistance = remaining;
    if (remaining == 0)
        self->fallSpeed = 0;
    else {
        if (++self->fallSpeed == 0)
            ++self->fallSpeed;
        if (self->fallSpeed > 5)
            self->fallSpeed = 5;
    }
}
/* Trailing byte-padding mismatch fix: the function body is 342 bytes
 * (not 4-aligned), and the ROM pads the 2-byte gap before the next
 * function (FindLineCrossing) with a zero halfword rather than the
 * assembler's default `nop` (`mov r8, r8`) - see
 * matching_decomp_alignment_fix memory. */
asm(".align 2, 0");
