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
struct collision_candidate {
    void *neighbor; // 0x00 - the other entity involved in the collision
    s32 field4;      // 0x04 - part of a position pair (see field8)
    s32 field8;       // 0x08
    s32 kind;          // 0x0c - a collision-state/dispatch id
    void *field10;       // 0x10
    void *field14;         // 0x14
    s32 field18;             // 0x18
    s32 field1c;               // 0x1c
    u8 field20;                  // 0x20 - one of two flag bytes
    u8 field21;                   // 0x21 - the other flag byte
};

COMPILE_TIME_ASSERT(sizeof(struct collision_candidate) == 0x24);

/* The player's own small append-only queue of pending collision
 * candidates, embedded inside the same per-entity collision-state
 * record at `gUnknown_030012D8 + 0x108` that `sub_8010A0C`-`sub_8010B68`
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
 * Written as NAKED asm, not plain C: an equivalent straight-line C
 * reconstruction (`self->candidates[self->count].field = value;`
 * repeated per field, in the same order as the ROM's own stores)
 * reproduces the ROM's exact *shape* - same 8-way common-subexpression
 * grouping of the repeated `self->count`-based index computation
 * (consecutive field writes to the same record share one computation,
 * exactly where the ROM's own disassembly does and nowhere else) - but
 * gcc 2.9 -O2 picks a different scratch register for the "copy of
 * `self` used to read `self->count`" step almost every time (e.g. `r4`
 * where the ROM uses `r1`, forcing the ROM to cache the `neighbor`
 * argument into `sb` before clobbering `r1`, something the plain-C
 * version never needs). This isn't one isolated register letter to pin
 * - it recurs at nearly every one of the 8 index computations - so
 * transcribed instruction-for-instruction from the ROM disassembly
 * instead, the same escape hatch already established throughout this
 * subsystem (`sub_0800D18C`/`sub_800E08C`, game_loop47.c, and others).
 * No branches and no literal pool in this function (every operand is
 * either an argument or a small immediate), so the transcription needed
 * no label renumbering or pool-placement care. */
NAKED void sub_8010D54(struct collision_queue *self, void *neighbor, s32 kind,
                        void *field10, void *field14, s32 field18, s32 field4,
                        s32 field8, s32 field1c, u8 field20, u8 field21)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "mov ip, r0\n\t"
        "mov sb, r1\n\t"
        "ldr r7, [sp, #0x1c]\n\t"
        "ldr r6, [sp, #0x2c]\n\t"
        "add r0, sp, #0x30\n\t"
        "add r4, sp, #0x34\n\t"
        "ldrb r0, [r0]\n\t"
        "mov r8, r0\n\t"
        "ldrb r5, [r4]\n\t"
        "mov r1, ip\n\t"
        "ldr r0, [r1]\n\t"
        "lsl r4, r0, #3\n\t"
        "add r4, r4, r0\n\t"
        "lsl r4, r4, #2\n\t"
        "mov r0, ip\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r4\n\t"
        "mov r1, sb\n\t"
        "str r1, [r0]\n\t"
        "mov r0, ip\n\t"
        "add r0, #0x14\n\t"
        "add r0, r0, r4\n\t"
        "str r2, [r0]\n\t"
        "mov r2, ip\n\t"
        "ldr r0, [r2]\n\t"
        "lsl r1, r0, #3\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "mov r0, ip\n\t"
        "add r0, #0x18\n\t"
        "add r0, r0, r1\n\t"
        "str r3, [r0]\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, ip\n\t"
        "add r0, #0x29\n\t"
        "strb r5, [r0]\n\t"
        "ldr r0, [r2]\n\t"
        "lsl r1, r0, #3\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "mov r0, ip\n\t"
        "add r0, #0x24\n\t"
        "add r0, r0, r1\n\t"
        "str r6, [r0]\n\t"
        "ldr r0, [r2]\n\t"
        "lsl r1, r0, #3\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "mov r0, ip\n\t"
        "add r0, #0x1c\n\t"
        "add r0, r0, r1\n\t"
        "str r7, [r0]\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, ip\n\t"
        "add r0, #0x28\n\t"
        "mov r1, r8\n\t"
        "strb r1, [r0]\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, ip\n\t"
        "ldr r1, [sp, #0x24]\n\t"
        "ldr r2, [sp, #0x28]\n\t"
        "str r1, [r0, #0xc]\n\t"
        "str r2, [r0, #0x10]\n\t"
        "mov r2, ip\n\t"
        "ldr r0, [r2]\n\t"
        "lsl r1, r0, #3\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "mov r0, ip\n\t"
        "add r0, #0x20\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [sp, #0x20]\n\t"
        "str r1, [r0]\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #1\n\t"
        "str r0, [r2]\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

/* Already matched/documented elsewhere in the codebase (graphics.c's
 * `sub_8006AF4`, `src/graphics/graphics.c`) as the exact same
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
