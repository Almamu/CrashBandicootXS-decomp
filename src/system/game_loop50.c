#include "core.h"

/* GitHub issue #12/#14's physics/collision subsystem continues past
 * 0x08010D54 into a large, still-unexamined 25-function/~27KB chunk
 * (asm/code_3_2_17_e560_10d54.s). This file is Phase 1 of that chunk's
 * examination: just the entry point, `sub_8010D54` itself - see
 * docs/matching/issue-14-0x08010d54-physics-apply.md for the full
 * semantic map and Phase 2 planning notes on the other 24 functions. */

/* One queued "commit this collision" candidate - the record
 * `sub_8010D54` appends here and `sub_8010B6C` (game_loop28.c, already
 * matched) later scans/resolves via `sub_800E08C` (game_loop27.c's own
 * extern declaration for it). Field names/types mirror
 * `sub_800E08C`'s own already-established extern signature exactly,
 * confirmed field-for-field against this function's own stores - both
 * functions operate on the same record shape (`sub_8010B6C`'s doc
 * comment already calls `sub_8010D54` its "mirror image" for exactly
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

COMPILE_TIME_ASSERT(sizeof(struct collision_candidate) == 0x24);

/* The player's own small append-only queue of pending collision
 * candidates, embedded inside the same per-entity collision-state
 * record at `gPlayer + 0x108` that `sub_8010A0C`-`sub_8010B68`
 * (game_loop27.c) and `sub_8010B6C` (game_loop28.c) already operate on
 * - confirmed by this function's own caller
 * (`sub_0800D18C`/game_loop47.c) passing exactly that address as `self`.
 * `self+0x44`-`self+0x58` (per game_loop27.c) are further fields of the
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
 * `sub_0800D18C` (game_loop47.c) makes at the end of its own per-edge
 * dispatch, per docs/rom_map.md's already-confirmed read: "hands off to
 * `sub_8010D54` with ~8 packed arguments... the actual apply/commit
 * step". Appends one `collision_candidate` record to the player's queue
 * (`self`) at `self->candidates[self->count]`, then increments
 * `self->count`. Every field's caller-side value is confirmed against
 * `sub_0800D18C`'s own NAKED call site (the final `bl sub_8010D54` in
 * game_loop47.c): `neighbor` is the entity whose collision is being
 * committed (`self` from `sub_0800D18C`'s own perspective), `kind` is
 * its adjusted dispatch id, and the rest are packed position/rect
 * fields already accumulated across `sub_0800D18C`'s three jump tables.
 *
 * The two trailing byte arguments are read straight out of their stack
 * words with `ldrb` (both addresses formed first, then both loads), which
 * a plain `u8` parameter never produces under agbcc (it loads the whole
 * promoted word). `STACK_ARG_U8_ADDR` below hides each slot's address
 * behind an empty asm so the two `add rX, sp, #N` stay ahead of the loads;
 * the `r4` pin puts the second address where the ROM keeps it. Everything
 * else is plain C (matches under both agbcc and old_agbcc). */
#define STACK_ARG_U8_ADDR(ptr, arg) asm("" : "=r"(ptr) : "0"(&(arg)))

void sub_8010D54(struct collision_queue *self, void *neighbor, s32 kind,
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
 * `DestroyOamBuffer`, `src/graphics/graphics.c`) as the exact same
 * one-line "conditional call on bit 0" shape: `sub_8026ED0` (VRAM
 * upload manager, matched in graphics.c) only fires when `arg1`'s low
 * bit is set. `src/graphics/actor_part15.c` already externs this
 * function and calls it as `sub_8010E14(self + 0x108, 2)` - i.e. bit 0
 * clear, so that call site is itself a no-op (the manager call never
 * fires); nevertheless this confirms `self` is the same
 * `struct collision_queue` `sub_8010D54` above operates on (Phase 2 of
 * docs/matching/issue-14-0x08010d54-physics-apply.md's own planning:
 * this was already flagged there as a "mode-parameterized insert"
 * sibling before being read branch-by-branch - turns out to be this
 * simpler shape instead, `arg1` gates a VRAM-manager refresh rather
 * than selecting an insert mode). */
extern void sub_8026ED0(void *arg0);

void sub_8010E14(void *arg0, s32 arg1)
{
    if (arg1 & 1) {
        sub_8026ED0(arg0);
    }
}

/* Sibling reset: clears just `count` (`+0x00`) and `unk4`'s first byte
 * (`+0x04`) - confirming (per `src/graphics/actor_part77.c`'s own doc
 * comment, already noting this exact function) that `unk4` is read
 * back elsewhere as a real field, not unexamined padding, though its
 * own full meaning/width past this one byte remains open. Called as
 * `sub_8010E2C(self + 0x108)` from `actor_part77.c`. */
void sub_8010E2C(void *arg0)
{
    struct collision_queue *self = arg0;

    self->count = 0;
    self->unk4[0] = 0;
}
