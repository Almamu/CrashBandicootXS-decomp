#include "core.h"

/* GitHub issue #14: 0x08010A0C-0x08010D54, continuing the physics/
 * collision subsystem (game_loop17.c-game_loop27.c). `ResolveCollisionCandidates` is
 * this chunk's final and by far largest function - the collision-
 * candidate scan/resolve helper `ResolvePlayerCollisions` (game_loop23.c) already
 * calls once a frame as `ResolveCollisionCandidates(gPlayer + 0x108)`. */

/* A byte passed on the stack as a genuine byte (`strb`); a plain `u8`
 * parameter is widened to a word `str`. */
struct flag8
{
    u8 value;
} __attribute__((packed));

struct vec2
{
    s32 x;
    s32 y;
};

/* One queued collision candidate, 0x24 bytes. */
struct candidate
{
    struct vec2 *neighbor;  // 0x00
    struct vec2 pos;        // 0x04
    s32 kind;               // 0x0C
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    struct flag8 unk_20;    // 0x20
    struct flag8 unk_21;    // 0x21
    u8 unk_22[2];
};

struct candidate_list
{
    s32 count;              // 0x00
    u8 unk_04;              // 0x04
    u8 unk_05[3];
    struct candidate records[1]; // 0x08
};

extern struct vec2 *gPlayer;
extern void sub_800E08C(void *neighbor, s32 kind, s32 field10, s32 field14,
                        s32 field18, struct vec2 pos, s32 field1c,
                        struct flag8 field20, struct flag8 field21,
                        struct flag8 forced);

/* Resolves the frame's queued collision candidates. `records[0]` seeds
 * the "nearest to the player" choice (by Y distance, X as tiebreak).
 * Any later candidate whose Y distance is more than 8 off the current
 * best, or whose kind is 4, is resolved on the spot with sub_800E08C.
 * The rest only compete for nearest. The nearest one is then resolved
 * too, told whether any forced resolve happened, and the list is
 * emptied. */
void ResolveCollisionCandidates(struct candidate_list *self)
{
    if (self->count != 0)
    {
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
        bestDx = self->records[0].neighbor->x;
        bestDy = self->records[0].neighbor->y;
        bestDx -= px;
        if (bestDx < 0)
            bestDx = -bestDx;
        bestDy -= py;
        if (bestDy < 0)
            bestDy = -bestDy;
        forced = 0;

        for (i = 1; i < self->count; i++)
        {
            struct vec2 *n = self->records[i].neighbor;
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
            if (d > 8 || self->records[i].kind == 4)
            {
                sub_800E08C(n, self->records[i].kind, self->records[i].unk_10,
                            self->records[i].unk_14, self->records[i].unk_18,
                            self->records[i].pos, self->records[i].unk_1C,
                            self->records[i].unk_20, self->records[i].unk_21,
                            (struct flag8){0});
                forced = 1;
            }
            else if (dy < bestDy || (dy == bestDy && dx < bestDx))
            {
                bestDx = dx;
                bestDy = dy;
                best = i;
            }
        }

        sub_800E08C(self->records[best].neighbor, self->records[best].kind,
                    self->records[best].unk_10, self->records[best].unk_14,
                    self->records[best].unk_18, (self->records + best)->pos,
                    self->records[best].unk_1C, self->records[best].unk_20,
                    self->records[best].unk_21, (struct flag8){forced});
        self->count = 0;
        self->unk_04 = 0;
    }
}

/* Zero-fill the trailing halfword before AddCollisionCandidate, as the ROM does. */
asm(".align 2, 0");
