#include "core.h"
#include "crates.h"

/* GitHub issue #14: 0x08010A0C-0x08010D54, continuing the physics/
 * collision subsystem (crate_reset.c-slot_crate.c). `ResolveCollisionCandidates` is
 * this chunk's final and by far largest function - the collision-
 * candidate scan/resolve helper `ResolvePlayerCollisions` (crate.c) already
 * calls once a frame as `ResolveCollisionCandidates(gPlayer + 0x108)`. */

struct vec2
{
    s32 x;
    s32 y;
};

/* One queued collision candidate, 0x24 bytes. */
struct candidate
{
    struct vec2 *neighbor;  // 0x00
    struct e08c_pos pos;    // 0x04
    s32 kind;               // 0x0C
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    struct byte_arg unk_20; // 0x20 - passed on the stack as a byte (`strb`)
    struct byte_arg unk_21; // 0x21
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

/* Resolves the frame's queued collision candidates. `records[0]` seeds
 * the "nearest to the player" choice (by Y distance, X as tiebreak).
 * Any later candidate whose Y distance is more than 8 off the current
 * best, or whose kind is 4, is resolved on the spot with ApplyCrateCollision.
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
                ApplyCrateCollision((struct crate *)n, self->records[i].kind, self->records[i].unk_10,
                            self->records[i].unk_14, self->records[i].unk_18,
                            self->records[i].pos, self->records[i].unk_1C,
                            self->records[i].unk_20, self->records[i].unk_21,
                            (struct byte_arg){0});
                forced = 1;
            }
            else if (dy < bestDy || (dy == bestDy && dx < bestDx))
            {
                bestDx = dx;
                bestDy = dy;
                best = i;
            }
        }

        ApplyCrateCollision((struct crate *)self->records[best].neighbor, self->records[best].kind,
                    self->records[best].unk_10, self->records[best].unk_14,
                    self->records[best].unk_18, (self->records + best)->pos,
                    self->records[best].unk_1C, self->records[best].unk_20,
                    self->records[best].unk_21, (struct byte_arg){forced});
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
 * docs/matching/issue-14-0x08010d54-physics-apply.md for the full
 * semantic map and Phase 2 planning notes on the other 24 functions. */

/* One queued "commit this collision" candidate - the record
 * `AddCollisionCandidate` appends here and `ResolveCollisionCandidates` (collision_queue.c, already
 * matched) later scans/resolves via `ApplyCrateCollision` (slot_crate.c's own
 * extern declaration for it). Field names/types mirror
 * `ApplyCrateCollision`'s own already-established extern signature exactly,
 * confirmed field-for-field against this function's own stores - both
 * functions operate on the same record shape (`ResolveCollisionCandidates`'s doc
 * comment already calls `AddCollisionCandidate` its "mirror image" for exactly
 * this reason). 0x24 bytes (0x22 bytes of real fields, naturally
 * padded to a 4-byte multiple by the trailing `s32` alignment). */
/* A position pair, copied into the record as one 8-byte struct (the
 * ROM's paired `ldr; ldr; str; str` at +0x04/+0x08 is a by-value struct
 * copy, not two independent field stores). */
struct pos_pair {
    s32 x;
    s32 y;
};

struct collision_candidate {
    void *neighbor; // 0x00 - the other entity involved in the collision
    struct pos_pair pos; // 0x04 - position pair
    s32 kind;          // 0x0c - a collision-state/dispatch id
    s32 field10;         // 0x10
    s32 field14;           // 0x14
    s32 field18;             // 0x18
    s32 field1c;               // 0x1c
    u8 field20;                  // 0x20 - one of two flag bytes
    u8 field21;                   // 0x21 - the other flag byte
};

COMPILE_TIME_ASSERT(collision_queue_c, sizeof(struct collision_candidate) == 0x24);

/* The player's own small append-only queue of pending collision
 * candidates, embedded inside the same per-entity collision-state
 * record at `gPlayer + 0x108` that `DecrementSlotCrateStage`-`GetCrateTrialKind`
 * (slot_crate.c) and `ResolveCollisionCandidates` (collision_queue.c) already operate on
 * - confirmed by this function's own caller
 * (`QueueCratePlayerCollision` in crate_break.c) passing exactly that address as `self`.
 * `self+0x44`-`self+0x58` (per slot_crate.c) are further fields of the
 * *same* record past this queue - true capacity of `candidates` beyond
 * one confirmed slot isn't established here (see this file's own issue
 * doc); declared with a single element and indexed dynamically past it
 * via `self->count`, which is legal C and produces identical codegen to
 * an unbounded pointer-arithmetic cast, per this project's standing
 * preference for named struct fields over raw offset casts. */
struct collision_queue {
    s32 count;                                 // 0x00
    u8 unk4[4];                                   // 0x04 - unexamined
    struct collision_candidate candidates[1];        // 0x08+
};

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
#define STACK_ARG_U8_ADDR(ptr, arg) asm("" : "=r"(ptr) : "0"(&(arg)))

void AddCollisionCandidate(struct collision_queue *self, void *neighbor, s32 kind,
                 s32 field10, s32 field14, s32 field18, struct pos_pair pos,
                 s32 field1c, s32 field20, s32 field21)
{
    u8 *p20;
    register u8 *p21 asm("r4");
    u8 f20, f21;

    STACK_ARG_U8_ADDR(p20, field20);
    STACK_ARG_U8_ADDR(p21, field21);
    f20 = *p20;
    f21 = *p21;

    self->candidates[self->count].neighbor = neighbor;
    self->candidates[self->count].kind = kind;
    self->candidates[self->count].field10 = field10;
    self->candidates[self->count].field21 = f21;
    self->candidates[self->count].field1c = field1c;
    self->candidates[self->count].field14 = field14;
    self->candidates[self->count].field20 = f20;
    self->candidates[self->count].pos = pos;
    self->candidates[self->count].field18 = field18;
    self->count++;
}

/* Already matched/documented elsewhere in the codebase (graphics.c's
 * `DestroyOamBuffer`, `src/gfx/graphics.c`) as the exact same
 * one-line "conditional call on bit 0" shape: `OperatorDelete` (VRAM
 * upload manager, matched in graphics.c) only fires when `arg1`'s low
 * bit is set. `src/player/player_update.c` already externs this
 * function and calls it as `DestroyCollisionQueue(self + 0x108, 2)` - i.e. bit 0
 * clear, so that call site is itself a no-op (the manager call never
 * fires); nevertheless this confirms `self` is the same
 * `struct collision_queue` `AddCollisionCandidate` above operates on (Phase 2 of
 * docs/matching/issue-14-0x08010d54-physics-apply.md's own planning:
 * this was already flagged there as a "mode-parameterized insert"
 * sibling before being read branch-by-branch - turns out to be this
 * simpler shape instead, `arg1` gates a VRAM-manager refresh rather
 * than selecting an insert mode). */
extern void OperatorDelete(void *arg0);

void DestroyCollisionQueue(void *arg0, s32 arg1)
{
    if (arg1 & 1) {
        OperatorDelete(arg0);
    }
}

/* Sibling reset: clears just `count` (`+0x00`) and `unk4`'s first byte
 * (`+0x04`) - confirming (per `src/player/player_init.c`'s own doc
 * comment, already noting this exact function) that `unk4` is read
 * back elsewhere as a real field, not unexamined padding, though its
 * own full meaning/width past this one byte remains open. Called as
 * `ResetCollisionQueue(self + 0x108)` from `player_init.c`. */
void ResetCollisionQueue(void *arg0)
{
    struct collision_queue *self = arg0;

    self->count = 0;
    self->unk4[0] = 0;
}
