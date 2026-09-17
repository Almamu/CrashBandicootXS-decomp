# Issue #16: 0x080119A8-0x08012FBC (25 functions)

Category label was `graphics`, but the chunk splits cleanly into two
different systems by content, not by address contiguity:

- **The leading 15 functions (`sub_80119A8`-`sub_8011B90`, matched
  here)** all operate on plain `struct actor` (`table@0x18`,
  `flags@0xc`, `field_08@8`, `x@0`, `y@4` - the exact layout
  `include/actor.h`/`actor_part*.c` already established), extended
  with a handful of not-yet-characterized fields beyond its own 0x1c
  bytes (`+0x38`, `+0x48`-`0x4b`, `+0x4c`-`0x50`). This is squarely
  `actor`, matching `docs/status/README.md`'s note that several
  `graphics`-labeled chunks turn out to be `actor` on inspection
  (issues #17/#18/#22/#52/#58/#62 already established this pattern).
- **The remaining 10 functions (`sub_8011BD4` onward, left raw)**
  belong to a different, still-unnamed "child object" struct
  (`self+0xc`/`+0x10`/`+0x18` hold *pointers* to further sub-records,
  not `struct actor` fields at all) - `docs/rom_map.md`'s own
  "Undifferentiated core" investigation already read several of these
  functions and explicitly flagged them as not understood with
  byte-exact precision yet (see below).

## New files

`asm/code_3_2_17_e560.s` is truncated to end right before `sub_80119A8`
(0x080119A8, unchanged start address). The 15 matched functions move to
a new `src/graphics/actor_part39.c` (numbered `39` - the next free
number after `actor_part38d.c`, this repo's highest existing
`actor_part*.c` at the time this chunk was picked up). A new
`asm/code_3_2_17_11bd4.s` picks up the unexamined remainder starting at
`sub_8011BD4` (0x08011BD4) through the end of this chunk's range
(0x08012FBC, where `sub_8012FBC` - outside this issue's function list -
begins); `ldscript.txt` and `tools/report_units.py`'s `UNITS` list were
updated to place all three pieces (`code_3_2_17_e560.o`,
`actor_part39.o`, `code_3_2_17_11bd4.o`) in that exact link order.

## Matched (15/25)

- **`sub_80119A8`**: tail-calls `sub_8007A84` (already matched,
  `actor_part.c`) with `gUnknown_030012CC` as `self`, then clears
  `part->flags` bit 3 if `part+0x38` is nonzero. Needed the
  negative-constant bit-clear idiom (`& -9` computed via a genuine
  runtime `movs`+`rsbs`, not folded to a positive immediate AND) with
  the mask register-pinned to `r0` and a compound assignment
  (`mask &= flags; part->flags = mask;`) so the `ands` instruction's
  result lands back in `r0` (matching the ROM) instead of the operand
  the compiler would otherwise pick as destination.
- **`sub_80119D4`**: always-2 stub, same shape as `sub_8008480`'s
  always-true stub.
- **`sub_80119D8`**: sets `self->table = gStaticData_087E414C` then
  tail-calls `sub_8008484` (already matched, `actor_part6.c`), which
  unconditionally overwrites `table` again with `gStaticData_087E3BEC`
  - this function's own store is immediately clobbered by its callee,
  but kept faithfully since the compiler can't see through the opaque
  call to know the store is dead.
- **`sub_80119EC`**: sets flag bit 6, clears `self+0x48`. Needed the
  mask register-pinned to `r1` (loaded *before* the `flags` byte read,
  matching the ROM's instruction order) and a `volatile` store to
  `self->flags` to stop the compiler hoisting the second field's zero
  constant into the same store's dead window - a plain sequential
  statement pair let it reorder the loads across the two independent
  stores.
- **`sub_80119FC`**: re-inits `self` via `sub_80084A4`, overwrites
  `table` with `gStaticData_087E414C`, runs `sub_80119EC` on it.
- **`sub_8011A1C`**: if `self+0x48` is zero and the player
  (`gUnknown_030012D8`)'s flags top bit is set, fires a
  `self->table+0x68`-driven trampoline (same idiom documented in
  `actor_part12.c`/`actor_part13.c`) on `self` via `sub_803AD7C`.
  Matched directly, no register-pinning needed - the natural ABI
  register choice already matched the ROM.
- **`sub_8011A50`**: sets `self->x`/`self->y` (Q8) from raw pixel
  `x`/`y`, then mirrors the *stored* value (not the just-computed
  local) into a second `{x, y}` pair at `self+0x4c`/`+0x50` - the ROM
  reloads both fields from memory after storing them rather than
  reusing the shifted register value, so the reconstruction reads them
  back through `volatile` pointers into fresh locals before the second
  pair of stores.
- **`sub_8011A64`**: sets `self+0x4a`/`+0x4b` via a single walked `u8 *`
  (incremented between the two stores, matching the ROM's own
  `adds r2, #1` pointer-walk rather than two independent offset
  computations), and calls `sub_801191C` (still raw, just above this
  ROM region) if `value == 0xff`. The incoming `value` parameter is
  `s32`, not `u8` - a narrower type made the compiler insert an
  entry-sequence truncation (`lsl`/`lsr` pair) the ROM never has, since
  the original C evidently never narrowed it either (the whole
  parameter is compared and stored as-is).
- **`sub_8011A84`**: trivial one-byte setter at `self+0x49`.
- **`sub_8011A8C`**: distance-gate - if the player is within 0x180
  (384 px) of `self` on both axes, calls `sub_8008364` (already
  matched, `actor_part5.c`); otherwise sets `self->flags` bit 0 and,
  unless `self->field_08 == 0xFFFF`, marks its bit in the same
  `gUnknown_030012B4+0x108` bitmap `actor_part2.c` already writes,
  reusing that file's exact register-pinned `>> 5` idiom. By far the
  most register-pinning of this batch's functions - the ROM keeps a
  raw load and its shifted result in two different registers at three
  separate points (`player->x`/`y` and the reloaded `field_08`) where
  a plain C expression lets the compiler collapse load+shift into one
  register; each needed an explicit two-register split (either a
  `register` pair plus an inline-`asm` shift, or an explicitly
  `volatile`-reloaded second read) to reproduce. The two branch
  conditions (`dx > 0x180` / `dy <= 0x180`) also needed literal `goto`
  labels matching the ROM's exact fall-through shape (out-of-range code
  falls straight through; the in-range call sits past a forward jump)
  rather than a nested `if`, which the compiler would otherwise invert
  into the opposite branch-target/condition-code pairing.
- **`sub_8011B0C`**: constructor - allocates a `struct actor`-shaped
  object (`sub_8026EDC(0x40)`, same size as `sub_8008434`'s constructor
  in `actor_part6.c`), re-inits it, sets `table = gStaticData_087E41BC`,
  runs the empty `nullsub_16` on it, then sets `field_08`/`x`/`y` from
  the raw pixel arguments.
- **`nullsub_16`**: empty stub.
- **`sub_8011B5C`**: same `table`-set/tail-call-`sub_8008484` shape as
  `sub_80119D8`, different vtable (`gStaticData_087E41BC`).
- **`sub_8011B70`**: same re-init/table-set/`nullsub_16` shape as
  `sub_8011B0C`, but re-initializing an existing `self` rather than
  allocating a new one.
- **`sub_8011B90`**: zeroes/initializes a run of fields from
  `self+0x25` through `+0x34`, plus `self+8`/`+0x10`/`+0x14`/`+0x18`/
  `+0x1c`, setting `+0x2f`/`+0x30` to 1. Its only caller in this ROM
  (`sub_801588C`, still raw at `0x0801588C`) stores its own vtable
  pointer at `self+0xc`, not `+0x18` the way `struct actor` does -
  proof `self` here is a genuinely different, still-unnamed "child
  object" struct, so this function takes `void *` rather than
  `struct actor *` to avoid implying a layout it doesn't share. Needed
  a single walked `u8 *` pointer (register-pinned to `r1`, matching the
  ROM) reused across all twelve byte writes via `+=`/`-=` rather than
  twelve independent `p[N] = 0` expressions - the latter let the
  compiler split the walk into two separate pointer chains (and,
  before that fix, cost 4 extra bytes total across the whole file,
  which shifted every symbol after this point in the ROM and made
  `make compare` fail on a completely unrelated early data pointer that
  merely happened to reference something past this shift).

## Left raw (10/25) - `sub_8011BD4` onward

`sub_8011BD4`, `sub_8012160`, `sub_8012238`, `sub_80122CC`,
`sub_8012420`, `sub_8012694`, `sub_801283C`, `sub_8012A7C`,
`sub_8012AF4`, `sub_8012D24` (ROM 0x08011BD4-0x08012FBC, now
`asm/code_3_2_17_11bd4.s`) were left completely untouched. All ten
operate on the same still-unnamed "child object" struct
`sub_8011B90`'s caller hinted at above (`self+0xc`/`+0x10`/`+0x18` as
pointers to further sub-records with their own `+0x20`/`+0x24`/
`+0x50`/`+0x54` fields, `self+0x27`-`0x35` as a state/flag/table-index
block) - none of it is `struct actor`. `docs/rom_map.md`'s own
"Undifferentiated core" investigation already read several of these
functions in detail and reached the same conclusion:

- `sub_8011BD4` (1420 B) is explicitly documented there as a 25-case
  jump table with a further 7-case sub-dispatch, sharing a type-ID gate
  (`self+0x8 == 0x1d`) with `sub_8016288` (also still raw) - "a
  companion state machine" to that function, not independently
  understood.
- `sub_8012420`, `sub_8012694`, `sub_801283C` are all documented as
  members of the 42-slot action-dispatch table family
  (`gStaticData_0816BF20`) - real, cross-referenced coverage, but not
  matched to byte-exact precision by that investigation either.
- The remaining functions (`sub_8012160`, `sub_8012238`, `sub_80122CC`,
  `sub_8012A7C`, `sub_8012AF4`, `sub_8012D24`) are direct siblings/
  callees of the above, sharing the same struct and calling
  conventions.

Given the project's own prior investigation already characterizes this
neighborhood as needing a dedicated pass (the same reasoning issue #12
used to leave the adjacent 0x0800E560-0x0800FC70 physics/collision
family raw), these ten functions were left in place rather than forcing
a low-confidence match or park. Issue #16 stays open for whoever picks
up this remainder next.
