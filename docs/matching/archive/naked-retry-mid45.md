# NAKED retry: issues #45, #31, #34, #37, #40

This pass retried 12 parked NAKED functions. Seven now compile from
plain C. Four others now have a closer C draft under `#if NON_MATCHING`,
and one was not attempted. Each candidate was compiled under both
agbcc and old_agbcc. The compiler every closed function needed is
listed in the table below.

## Closed

| Function | File | Compiler | What it took |
|---|---|---|---|
| `ConfigureHudParts` | `hud_init.c` | old_agbcc (the file already used it) | Two `switch`es (22/23/29 and `case 0xE ... 0x15`) give the ROM's compare trees. A `SetPal` inline keeps three separate nibble-insert copies, where the old "32 bytes short" draft merged them. A local `k = 13` and a `tbl` local for the position table settle two register choices. |
| `UpdateHudBoss`, `UpdateHudClock` | `hud_boss_clock.c` | old_agbcc (file switched) | The note blamed an "r7 wrong-value miscompile", but this was a compiler mismatch. Under old_agbcc the plain frame clamp reproduces `ldrb r7; lsls; adds rN, r7, #0` exactly. `CLAMP_FRAME` binds the part pointer before the frame value. `SetPartPos(x, y, part)` takes the part last, so the table symbol loads before `self->parts`. |
| `UpdateHudCrates`, `UpdateHudWumpa`, `UpdateHudPercentCounters` | `hud_counters.c` | old_agbcc (file switched) | Same clamp as above. A literal `-1` frame lets the compiler fold a clamp into a bare store, which the ROM does in some branches. Where the ROM keeps the compare, a `-1` held in a local does too. The "100%" branches read `self->parts` into a block-local, so the ROM's lack of cross-jumping comes out. `UpdateHudCrates`'s second counter reads value, cached value, then `self->parts`, in that order, and uses separate `digits`/`off` locals. `UpdateHudWumpa` matched on the first compile. |
| `SpawnRoomExit` | `spawn_bosses.c` | old_agbcc (the file already used it) | The 9-halfword gap was `{x - 2, y - 0x1e}` landing in fresh registers. Four register pins (`r1`/`r2`/`r0`/`r3`) and one documented empty `asm("" : "+r" (x))` close it. The `asm` stops combine from folding `x` back into its input before `y` is loaded. |

`include/hud.h`'s `struct hud_digit_part` now names `x`/`y` at
`+0`/`+4`; they were `unknown_00[0x18]` and nothing else used them.

## Not closed

- **`RunRoom`** (`run_room.c`, #37): the draft is 15 halfwords
  off under old_agbcc (agbcc is worse) and the same size. The "shared
  tail across two jump-table targets" in the NAKED note is ordinary
  cross-jumping, and it appears without any help. Three things got it
  close:
  - The one-byte `direction` stack argument is a packed one-byte struct
    passed by value, which gives the ROM's `strb` into the outgoing slot.
  - The level-state pointer and one point coordinate share variables
    with later code: `x` doubles as the level local, and `y` reuses the
    loop index `i`.
  - The entity-count loop is a guarded do-while over a hoisted list
    pointer.

  Still left: the ROM computes the byte slot's address before the
  constant. Every C spelling tried reverses that order: compound
  literal, local, union cast, inline wrapper, a struct holding
  count+direction, and a plain `u8` parameter. Also left is one register
  in the player-position struct copy.
- **`DecodeCollisionChunk`** (`bg_layer_base.c`, #40): the draft is 33 halfwords off
  under old_agbcc and the same size. `acc`/`pair` as `s16` reproduce the
  ROM's per-use `lsl/asr` sign extensions. A do-while pair loop
  reproduces the missing entry test. Still left: the old r4/r5
  accumulator/pointer swap, and the ROM's `lsl` of `(s8)pair` coming
  before `acc`'s sign extension. Six spellings of the add all compile
  alike.
- **`SpawnRoomEntities`** (`room_entities.c`, #40): the draft is now the ROM's
  exact size (704 bytes, where the old note had 732) but about 219
  halfwords off. The first half and the link pass's inner scans line up.
  The ROM places the link-chasing do-while's actor search before its
  link scan, while this compile places the scan first. `while`,
  `for (;;)` and hand-written `goto` loops made no difference.
- **`SpawnFlamethrowerLabAssistant`** (`spawn_enemies.c`, #31): the draft is 62
  halfwords off, unchanged. The ROM keeps `arg3` in r4 and
  `&gEntityFlags` in sb. Pinning either one, or holding the table
  address in a local, made it worse (120+).
- **`UpdateGameFrame`** (`game_frame.c`, #34): not attempted. At about
  730 instructions it was out of this pass's time budget.

## Helpers

The brute-force variant runner (`brute.py`, adapted from PR #462's
`box9/` so it accepts several `(old, new)` replacements per variant) and
the `fd.py` diff tool live in the session scratchpad (`mid45/`). They
are not committed.
