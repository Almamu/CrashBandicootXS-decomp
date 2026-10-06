#include "core.h"
#include "match.h"
#include "crate.h"
#include "crates.h"
#include "objects.h"
#include "memory.h"
#include "globals.h"
#include "player.h"

/* GitHub issue #14: 0x08010A0C-0x08010D54, continuing the physics/
 * collision subsystem (crate_reset.c-slot_crate.c). `ResolveCollisionCandidates` is
 * this chunk's final and by far largest function - the collision-
 * candidate scan/resolve helper `ResolvePlayerCollisions` (crate.c) already
 * calls once a frame as `ResolveCollisionCandidates(gPlayer + 0x108)`. */

/* The queue and its candidates are objects.h's `struct collision_queue`
 * and `struct collision_candidate`. */

/* Resolves the frame's queued collision candidates. `candidates[0]` seeds
 * the "nearest to the player" choice (by Y distance, X as tiebreak).
 * Any later candidate whose Y distance is more than 8 off the current
 * best, or whose kind is 4, is resolved on the spot with ApplyCrateCollision.
 * The rest only compete for nearest. The nearest one is then resolved
 * too, told whether any forced resolve happened, and the list is
 * emptied. */
void ResolveCollisionCandidates(struct collision_queue *self)
{
    if (self->count != 0) {
        s32 best;
        s32 bestDx;
        s32 px;
        s32 py;
        s32 i;
        s32 bestDy;
        u8 forced;

        px = gPlayer->x;
        py = gPlayer->y;
        best = 0;
        bestDx = self->candidates[0].neighbor->x;
        bestDy = self->candidates[0].neighbor->y;
        bestDx -= px;
        if (bestDx < 0)
            bestDx = -bestDx;
        bestDy -= py;
        if (bestDy < 0)
            bestDy = -bestDy;
        forced = 0;

        for (i = 1; i < self->count; i++) {
            struct crate *n = self->candidates[i].neighbor;
            s32 dx = n->x;
            s32 dy = n->y;
            s32 d;

            dx -= px;
            if (dx < 0)
                dx = -dx;
            dy -= py;
            if (dy < 0)
                dy = -dy;
            d = dy - bestDy;
            if (d < 0)
                d = -d;
            if (d > 8 || self->candidates[i].kind == 4) {
                ApplyCrateCollision(
                    n, self->candidates[i].kind, self->candidates[i].code, self->candidates[i].edge,
                    self->candidates[i].depth, self->candidates[i].pos, self->candidates[i].hit,
                    self->candidates[i].p20, self->candidates[i].p21, (struct byte_arg){ 0 });
                forced = 1;
            } else if (dy < bestDy || (dy == bestDy && dx < bestDx)) {
                bestDx = dx;
                bestDy = dy;
                best = i;
            }
        }

        ApplyCrateCollision(self->candidates[best].neighbor, self->candidates[best].kind,
                            self->candidates[best].code, self->candidates[best].edge,
                            self->candidates[best].depth, (self->candidates + best)->pos,
                            self->candidates[best].hit, self->candidates[best].p20,
                            self->candidates[best].p21, (struct byte_arg){ forced });
        self->count = 0;
        self->unk_04 = 0;
    }
}

/* Zero-fill the trailing halfword before AddCollisionCandidate, as the ROM does. */
asm(".align 2, 0");

/* GitHub issue #12/#14's physics/collision subsystem continues past
 * 0x08010D54 into a large, still-unexamined 25-function/~27KB chunk
 * (asm/code_3_2_17_e560_10d54.s). This file is Phase 1 of that chunk's
 * examination: just the entry point, `AddCollisionCandidate` itself - see
 * docs/matching/archive/issue-14-0x08010d54-physics-apply.md for the full
 * semantic map and Phase 2 planning notes on the other 24 functions. */

/* Physics/collision subsystem's **apply/commit step** - the final call
 * `QueueCratePlayerCollision` (crate_break.c) makes at the end of its own per-edge
 * dispatch, per docs/rom_map.md's already-confirmed read: "hands off to
 * `AddCollisionCandidate` with ~8 packed arguments... the actual apply/commit
 * step". Appends one `collision_candidate` record to the player's queue
 * (`self`) at `self->candidates[self->count]`, then increments
 * `self->count`. Every field's caller-side value is confirmed against
 * `QueueCratePlayerCollision`'s own NAKED call site (the final `bl AddCollisionCandidate` in
 * crate_break.c): `neighbor` is the entity whose collision is being
 * committed (`self` from `QueueCratePlayerCollision`'s own perspective), `kind` is
 * its adjusted dispatch id, and the rest are packed position/rect
 * fields already accumulated across `QueueCratePlayerCollision`'s three jump tables.
 *
 * The two trailing byte arguments are read straight out of their stack
 * words with `ldrb` (both addresses formed first, then both loads), which
 * a plain `u8` parameter never produces under agbcc (it loads the whole
 * promoted word). `STACK_ARG_U8_ADDR` below hides each slot's address
 * behind an empty asm so the two `add rX, sp, #N` stay ahead of the loads;
 * the `r4` pin puts the second address where the ROM keeps it. Everything
 * else is plain C (matches under both agbcc and old_agbcc). */
#define STACK_ARG_U8_ADDR(ptr, arg) MATCH_CONST(ptr, &(arg))

void AddCollisionCandidate(struct collision_queue *self, struct crate *neighbor, s32 kind, s32 code,
                           s32 edge, s32 depth, struct e08c_pos pos, s32 hit, s32 p20, s32 p21)
{
    u8 *a20;
    MATCH_HOLD_REG(u8 *, a21, r4);
    u8 f20, f21;

    STACK_ARG_U8_ADDR(a20, p20);
    STACK_ARG_U8_ADDR(a21, p21);
    f20 = *a20;
    f21 = *a21;

    self->candidates[self->count].neighbor = neighbor;
    self->candidates[self->count].kind = kind;
    self->candidates[self->count].code = code;
    self->candidates[self->count].p21.v = f21;
    self->candidates[self->count].hit = hit;
    self->candidates[self->count].edge = edge;
    self->candidates[self->count].p20.v = f20;
    self->candidates[self->count].pos = pos;
    self->candidates[self->count].depth = depth;
    self->count++;
}

/* Already matched/documented elsewhere in the codebase (graphics.c's
 * `DestroyOamBuffer`, `src/gfx/graphics.c`) as the exact same
 * one-line "conditional call on bit 0" shape: `OperatorDelete` (VRAM
 * upload manager, matched in graphics.c) only fires when `flags`'s low
 * bit is set. `src/player/player_update.c` already externs this
 * function and calls it as `DestroyCollisionQueue(self + 0x108, 2)` - i.e. bit 0
 * clear, so that call site is itself a no-op (the manager call never
 * fires); nevertheless this confirms `self` is the same
 * `struct collision_queue` `AddCollisionCandidate` above operates on (Phase 2 of
 * docs/matching/archive/issue-14-0x08010d54-physics-apply.md's own planning:
 * this was already flagged there as a "mode-parameterized insert"
 * sibling before being read branch-by-branch - turns out to be this
 * simpler shape instead, `flags` gates a VRAM-manager refresh rather
 * than selecting an insert mode). */

void DestroyCollisionQueue(struct collision_queue *self, s32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Sibling reset: clears just `count` (`+0x00`) and `unk_04`
 * (`+0x04`) - confirming (per `src/player/player_init.c`'s own doc
 * comment, already noting this exact function) that `unk_04` is read
 * back elsewhere as a real field, not unexamined padding, though its
 * own full meaning/width past this one byte remains open. Called as
 * `ResetCollisionQueue(self + 0x108)` from `player_init.c`. */
void ResetCollisionQueue(struct collision_queue *self)
{
    self->count = 0;
    self->unk_04 = 0;
}
