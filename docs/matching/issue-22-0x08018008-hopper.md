# Issue #22: 0x08018008-0x080187FC, and `sub_8017AB0` retried

This pass finishes issue #22. The last raw block, `asm/code_3_2_17_18008.s`
(`UpdateTiny`, `SetTinyState`, `PickTinyHopTarget`, `sub_80186F0`), is now
`src/graphics/actor_part_18008.c`. `sub_8017AB0`
(`src/graphics/actor_part27a.c`), which was a NAKED transcription, is
now real C too. All five match under the older compiler, old_agbcc, and
both files are on the Makefile's `OLD_AGBCC_OBJS` list.

## Compiler: old_agbcc

old_agbcc was already confirmed for `0x080188D0`-`0x0801E578`, which
starts right after this range. The same holds here. Compiled with the
current agbcc, the final C matches only `PickTinyHopTarget`: `UpdateTiny`
comes out 4 bytes short and `sub_80186F0` 4 bytes long, while
`SetTinyState` and `sub_8017AB0` have the right size but differ in 22 and
6 bytes. Under old_agbcc all five are byte-exact. The ROM also shows old_agbcc's signature: the constant
loaded before the byte it is combined with (`movs r0, #0x10; ldrb r3,
[r4, #0xc]; orrs r0, r3` in `sub_80186F0`, `movs r0, #0x7f; ldrb r1,
[r1]; ands r0, r1` in `sub_8017AB0`).

The other #22 files (`actor_part27.c`, `actor_part27b.c`,
`actor_part27c.c`) also match byte for byte under old_agbcc as they are,
pins included. They stay on the current agbcc, because switching them
gains nothing without also dropping their pins.

## What the functions do

`UpdateTiny`/`SetTinyState` are the per-frame update and "enter state"
methods of the `gTinyVtable` class (constructor `CreateTiny`,
`actor_part_188d0.c`). The class is a boss that hops its part along
parabolic arcs between the anchor objects of the `gUnknown_030012EC`
list:

- A hop runs over `steps` of `total` frames. X is linear from the start
  point, and Y follows the 257-entry `i*i>>8` table at +0x48 (rising
  arcs read it backwards). `sub_8018978` (`actor_part_188d0.c`) sets up
  the deltas.
- `SetTinyState` (15 cases) sets the next hop's start point from the
  target anchor, the player's position or the level floor. It also fires
  the part's animation method at +0x50, and always finishes by calling
  the "set state" method at +0x20.
- `UpdateTiny` (16-case jump table) first resets a stomped anchor's
  controller (a fresh `sub_801886C` object). It then runs the player
  contact test: in state 8 a `GetSpriteAttackBox`/`GetSpriteBodyBox` box overlap
  with the player in state 0x13 knocks the boss (state 9). Otherwise an
  overlap hurts the player through the player's +0x68 method. Last it
  steps the current hop or timer.
- `PickTinyHopTarget` picks the next anchor: the one nearest the player
  (Manhattan distance) selects a column of the per-round, per-anchor
  table at `gTinyHopTargets`.
- `sub_80186F0` spawns a falling hazard (a `gCollidableList` part
  with a `sub_80188D0` controller) at the `n`th third of the way from
  the boss towards the player. `UpdateTiny` calls it on a 0x46-frame
  timer while in state 13.

`sub_8017AB0` is described in `docs/matching/issue-22-0x08017ab0-actor.md`.
That description still holds, with one correction: state 2's
out-of-range exit goes to state 0's mirror-bit test, not to the
`animDone` check.

## How they matched

- **Byte stores leave a dead `& 0` behind.** old_agbcc expands a plain
  `u8` struct-member store as a read-modify-write that includes an
  `& 0`. The store itself comes out as a single `strb`, but CSE keeps
  that 0 and reuses it for a later zero store, which moves the zero's
  `movs` earlier. Where the ROM materializes the 0 late, two things fix
  it: the flag RMWs go through a raw byte pointer (`PART_FLAGS`, as in
  `actor_part_188d0.c`), and a named `one` local is used for the 1-stores
  in `UpdateTiny`'s case 0.
- **Box builders by value.** `a = GetSpriteAttackBox(player)` gives the ROM's
  stack slots. `UpdateTiny`'s else branch writes `b = GetSpriteAttackBox(..);
  a = b;`, as `sub_8019324` does. The `valid` word of `b` is read back
  through a `volatile` (`BOX_VALID`, from `actor_part_1967c.c`).
- **`CollidePartList` takes the box by value.** The earlier NAKED note on
  `sub_8017AB0` called its stack-argument order (6th, 7th, then 5th)
  unclosable from C. With the prototype
  `CollidePartList(void *, struct box, s32, void *)`, it closes. The 16-byte
  box goes three words in r1-r3 and one on the stack, gcc stores the two
  scalar stack arguments first, and that is the ROM's order. The same
  prototype probably closes `actor_part81.c`'s parked `CollidePlayerWithObjects`,
  which the NAKED note cites for the identical order. It has not been
  tried there.
- **The `gCrateList` list walk.** The "per-iteration literal
  reload" from the old NAKED note turns out to be a plain guarded
  do-while: `i = 0; if (i < list->count) do { ... } while (i <
  list->count);`. A `for` loop shares the list pointer between the entry
  test and the body instead of reloading it.
- **Switch state tests.** In `UpdateTiny` the ROM tests `state == 14`
  and `== 3` on a low-register copy of the switch value, then re-copies
  it for `== 7`. A plain if-chain reloads `sb` into a rotating register
  for each test. An `asm("" : "+r"(s))` copy reproduces the ROM.
- **Reload-register rotation.** gcc picks reload registers in rotation.
  In `sub_8017AB0` the ROM's rotation is one step away from the one this
  C gets, so every later `movs rX, #off; ldrsh` reload index was r2 where
  the ROM has r3, and the other way round. Spelling out the registers of
  two of the three `+0x104` busy-byte reads (`Busy()`, r3 offset / r0
  address) removes one reload from the count, and everything after it
  lines up.
- **Pins that remain.** `SetTinyState` pins `anchor`/`x` to r2/r1.
  `sub_80186F0` pins the new part to r4, the tag value to r0, and a
  barriered zero (the ROM keeps it in r8 across the calls, like
  `sub_8018CB0`'s 0xF). `sub_8017AB0` pins the kind byte to r1 and keeps
  a barriered copy of the list entry. Each has a comment in the source.
- `PickTinyHopTarget`'s nearest-anchor loop needed the player's x/y read into
  locals first, in that order, before the anchor's own x/y. With any
  other order, the two player coordinates swap r7/r8.

## Status

Issue #22 has no raw, NAKED or `NON_MATCHING` functions left:
`sub_8017A44`-`sub_8018884` (0x08017A44-0x080188D0) are all real C.
