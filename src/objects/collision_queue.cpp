#include "part_list.hpp"
#include "crate.hpp"
#include "player.hpp"

extern "C" {
#include "math_util.h"
#include "crates.h"
#include "memory.h"
#include "globals.h"
#include "player.h"
}

/* The player's collision queue, CollisionQueue (#664, part 7c;
 * include/part_list.hpp): the crate collisions QueueCratePlayerCollision
 * (crate_break.cpp) finds during the frame, resolved once a frame by
 * ResolvePlayerCollisions (crate.c). */

/* Resolves the frame's queued collision candidates. `candidates[0]` seeds
 * the "nearest to the player" choice (by Y distance, X as tiebreak).
 * Any later candidate whose Y distance is more than 8 off the current
 * best, or whose attack kind is ATTACK_KIND_SPIN, is resolved on the spot with
 * ApplyCrateCollision.
 * The rest only compete for nearest. The nearest one is then resolved
 * too, told whether any forced resolve happened, and the list is
 * emptied. */
void CollisionQueue::Resolve()
{
    if (count != 0) {
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
        bestDx = candidates[0].neighbor->x;
        bestDy = candidates[0].neighbor->y;
        bestDx -= px;
        MAKE_ABS(bestDx);
        bestDy -= py;
        MAKE_ABS(bestDy);
        forced = 0;

        for (i = 1; i < count; i++) {
            Crate *n = candidates[i].neighbor;
            s32 dx = n->x;
            s32 dy = n->y;
            s32 d;

            dx -= px;
            MAKE_ABS(dx);
            dy -= py;
            MAKE_ABS(dy);
            d = dy - bestDy;
            MAKE_ABS(d);
            if (d > 8 || candidates[i].kind == ATTACK_KIND_SPIN) {
                ApplyCrateCollision(n, candidates[i].kind, candidates[i].code, candidates[i].edge,
                                    candidates[i].depth, candidates[i].pos, candidates[i].hit,
                                    candidates[i].p20, candidates[i].p21, (struct byte_arg){ 0 });
                forced = 1;
            } else if (dy < bestDy || (dy == bestDy && dx < bestDx)) {
                bestDx = dx;
                bestDy = dy;
                best = i;
            }
        }

        ApplyCrateCollision(candidates[best].neighbor, candidates[best].kind, candidates[best].code,
                            candidates[best].edge, candidates[best].depth, candidates[best].pos,
                            candidates[best].hit, candidates[best].p20, candidates[best].p21,
                            (struct byte_arg){ forced });
        count = 0;
        posCommitted = 0;
    }
}

/* Appends a candidate: the last call of QueueCratePlayerCollision
 * (crate_break.cpp). The two byte arguments are one-byte structs (the
 * ROM reads them with `ldrb` from their stack words, both addresses
 * first); a `u8` parameter loads the whole word and narrows it. */
void CollisionQueue::Add(Crate *neighbor, s32 kind, s32 code, s32 edge, s32 depth, struct vec2 pos,
                         s32 hit, struct byte_arg p20, struct byte_arg p21)
{
    u8 f20 = p20.v;
    u8 f21 = p21.v;

    candidates[count].neighbor = neighbor;
    candidates[count].kind = kind;
    candidates[count].code = code;
    candidates[count].p21.v = f21;
    candidates[count].hit = hit;
    candidates[count].edge = edge;
    candidates[count].p20.v = f20;
    candidates[count].pos = pos;
    candidates[count].depth = depth;
    count++;
}

/* Player's destructor calls it with flags 2 (a member): no delete. */
CollisionQueue::~CollisionQueue()
{
}

/* Empties the queue: Player's constructor constructs its member. */
CollisionQueue::CollisionQueue()
{
    count = 0;
    posCommitted = 0;
}
