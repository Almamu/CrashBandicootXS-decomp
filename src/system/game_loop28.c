#include "core.h"
#include "actor.h"

/* GitHub issue #14: 0x08010A0C-0x08010D54, continuing the physics/
 * collision subsystem (game_loop17.c-game_loop27.c). `sub_8010B6C` is
 * this chunk's final and by far largest function - the collision-
 * candidate scan/resolve helper `sub_80106DC` (game_loop23.c) already
 * calls once a frame as `sub_8010B6C(gUnknown_030012D8 + 0x108)`. */

extern void *gUnknown_030012D8;
extern void sub_800E08C(void *neighbor, s32 kind, void *field10, void *field14,
                         s32 field18, s32 field4, s32 field8, s32 field1c,
                         u8 field20, u8 field21, u8 extra);

#if NON_MATCHING
/* One candidate slot in `self`'s list, `self+8` onward, stride 0x24. */
struct collisionCandidate {
    void *neighbor; /* +0x00: the candidate object (x=+0, y=+4 there) */
    s32 field4;      /* +0x04 */
    s32 field8;       /* +0x08 */
    s32 kind;          /* +0x0c: forced-resolve trigger when == 4 */
    void *field10;      /* +0x10 */
    void *field14;        /* +0x14 */
    s32 field18;            /* +0x18 */
    s32 field1c;              /* +0x1c */
    u8 field20;                /* +0x20 */
    u8 field21;                 /* +0x21 */
};

struct collisionList {
    s32 count;                                  /* +0x00 */
    u8 field4;                                    /* +0x04 */
    u8 pad[3];
    struct collisionCandidate records[1];             /* +0x08, `count` entries */
};

/* Scans `self`'s neighbor-candidate list - `records[0]` is the
 * previous/seed candidate (already resolved on some earlier call, kept
 * around as the initial "current best"), `records[1..count-1]` the new
 * candidates queued this frame - looking for whichever is closest to
 * the player (`gUnknown_030012D8`) by Y-distance (X-distance as
 * tiebreak, both measured from the player's own position). Any
 * candidate whose Y-distance jumps more than 8 past the running-best
 * Y-distance, or whose own `kind` field is 4, is treated as a forced/
 * priority hit and resolved immediately via `sub_800E08C()` without
 * affecting the running "nearest" comparison; the rest are only
 * compared against each other. After the scan, the overall nearest
 * candidate (`records[0]` itself if nothing else ever qualified) is
 * *also* resolved via `sub_800E08C()` - with its 11th byte argument
 * set to whether any forced/priority hit happened during the scan,
 * unlike every in-loop call, which always passes 0 there - and the
 * list is reset (`count = 0`, `field4 = 0`) for the next frame.
 * `sub_800E08C`'s own 9th-11th (byte) arguments land in this
 * function's own stack frame at `self` (well, `sp`) `+0x10`/`+0x14`/
 * `+0x18` - those addresses are precomputed once, outside the loop,
 * confirming they're genuinely stack-passed call arguments (AAPCS-
 * style slots 9-11), not separate mystery locals.
 *
 * PARKED (NON_MATCHING): every field offset, branch, and call argument
 * here is understood and cross-referenced against the mirror-image
 * writer `sub_8010D54` (right after this issue's own range) and the
 * caller `sub_80106DC` (game_loop23.c) - but the ROM builds nearly
 * every record-field address in both the loop body and the two
 * `sub_800E08C` call sites as a *running pointer*, incremented by
 * 0x24 once per iteration, with up to twelve of them (`r8`/`sb`/`sl`
 * among them) live across a single 0x68-byte stack frame - the same
 * "long, non-uniform stretch of field accesses via running-pointer
 * increments" gap already parked for `sub_800A734`/`sub_800A528` in
 * docs/matching/issue-9-0x08007634-actor.md, at a much larger scale.
 * Not chased to a byte-exact register allocation in this pass; left
 * parked with full field/branch/call fidelity instead of guessing. */
void sub_8010B6C(void *selfArg)
{
    struct collisionList *self = selfArg;
    struct actor *player = (struct actor *)gUnknown_030012D8;
    s32 count = self->count;

    if (count != 0) {
        s32 playerX = player->x;
        s32 playerY = player->y;
        s32 bestIndex = 0;
        struct collisionCandidate *seed = &self->records[0];
        s32 bestXDist = seed->field4 - playerX;
        s32 bestYDist = seed->field8 - playerY;
        u32 anyForcedHit = 0;
        struct collisionCandidate *rec;
        s32 j;

        if (bestXDist < 0) {
            bestXDist = -bestXDist;
        }
        if (bestYDist < 0) {
            bestYDist = -bestYDist;
        }

        for (j = 1; j < count; j++) {
            void *neighbor;
            s32 nx, ny, dx, dy, diff;

            rec = &self->records[j];
            neighbor = rec->neighbor;
            nx = *(s32 *)neighbor;
            ny = *(s32 *)((u8 *)neighbor + 4);
            dx = nx - playerX;
            dy = ny - playerY;

            if (dx < 0) {
                dx = -dx;
            }
            if (dy < 0) {
                dy = -dy;
            }

            diff = dy - bestYDist;
            if (diff < 0) {
                diff = -diff;
            }

            if (diff > 8 || rec->kind == 4) {
                anyForcedHit = 1;
                sub_800E08C(neighbor, rec->kind, rec->field10, rec->field14,
                            rec->field18, rec->field4, rec->field8, rec->field1c,
                            rec->field20, rec->field21, 0);
            } else if (dy < bestYDist || (dy == bestYDist && dx < bestXDist)) {
                bestXDist = dx;
                bestYDist = dy;
                bestIndex = j;
            }
        }

        rec = &self->records[bestIndex];
        sub_800E08C(rec->neighbor, rec->kind, rec->field10, rec->field14,
                    rec->field18, rec->field4, rec->field8, rec->field1c,
                    rec->field20, rec->field21, (u8)anyForcedHit);

        self->count = 0;
        self->field4 = 0;
    }
}
#endif /* NON_MATCHING */
