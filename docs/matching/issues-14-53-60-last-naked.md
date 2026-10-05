# The last NAKED functions of issues #14, #53 and #60

Each of these three chunks had exactly one NAKED function left. All three
are plain C now, with no register pins, no asm in the function body and
no NAKED, and all three match under both `agbcc` and `old_agbcc`. None of
the files change compiler.

## `ResolveCollisionCandidates` (game_loop28.c, issue #14)

The collision-candidate scan/resolve helper `ResolvePlayerCollisions` calls once a
frame. The old park note blamed "up to twelve running pointers" live
across the loop. Those are gcc's own strength reduction of
`self->records[i].field`, and plain C reproduces them. What mattered:

- `sub_800E08C`'s last three arguments are one-byte packed structs
  (`struct flag8`). The ROM stores them with `strb`; a plain `u8`
  parameter is widened to a word store.
- The record's position is a `struct vec2` passed by value, which gives
  the ROM's load-both-then-store-both copy.
- Distances are assigned and then subtracted (`dx = n->x; dx -= px;`).
- The count reset sits inside `if (count != 0)`, and the final call
  spells its position `(self->records + best)->pos`.

## `UpdatePolarElectricFence` (actor_part126.c, issue #53)

A hazard/proximity state machine that tests the part's own box and three
`gStaticData_0817A7xx` boxes. The "heavy r5/r6/r7 reuse" in the old note
is gcc propagating the zero the hit blocks store. The hit block is a
`HAZARD_HIT` macro wrapped in `if (1)`: `do { } while (0)` and an inline
function both change the block layout. `ShockPolarPlayer` returns `u8`.

## `DrawJetpackCollectedWumpa` (actor_part130.c, issue #60)

A bounding-box-culled sprite draw with `DrawActor`'s shape.
The doubling code stays in with the scale flag fixed at 0, followed by
`scaled |= 0x100` after culling. Both position loads go into block
locals before shifting, and `highBit`/`oamPriority` are early constant
locals.
