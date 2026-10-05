# Third near-miss sweep

This pass went back to eleven NAKED functions whose `#if NON_MATCHING`
drafts were 1-29 halfwords off. It tried two tricks found after those
drafts were last worked on:

- `asm("" : "=r"(v) : "0"(K))` in place of `v = K`. It emits the same
  `movs`, but gcc no longer sees a constant set, so the value is not a
  constant for CSE/combine and local-alloc doesn't double its live
  length (#474, #475).
- Re-reading a field through `self` instead of caching a pointer in a
  local, so old_agbcc's GCSE makes the ROM's copies (#475).

Five now match as real C:

| Function | File | Compiler | Was | Technique |
|---|---|---|---|---|
| `UpdateSlotCrate` | `src/system/game_loop49.c` | old_agbcc | 7 | count update split into in-place steps on a fresh local; one earlier `"+r"` barrier dropped |
| `ActionCtrlStateCrouch` | `src/graphics/actor_part_13c60.c` | old_agbcc | 1 insn | scoped `volatile u8 *` for the facing block's second read-modify-write |
| `LoadLevelSelectRecord` | `src/graphics/actor_part_1b85c.c` | old_agbcc | spill | a second local for the record pointer (the ROM's spilled copy) |
| `UpdateExtraLife` | `src/system/game_loop54.c` | old_agbcc | 22 | plain re-reads instead of `volatile` ones; two extra references per velocity |
| `ConvertHovercraftTiles` | `src/graphics/actor_part130.c` | both | 29 | opaque 0xf mask (`asm("" : "=r"(m) : "0"(0xf))`) ANDed as `m & b`; own counter for the second loop; row header in ROM order |

All four files were already on their compiler (`actor_part130.c` matches
under both), so no Makefile change. Every empty `asm` has a comment at
its use.

## What worked

**Writing an update as separate in-place statements (`UpdateSlotCrate`).**
The ROM's count update is `t = (r - 1) << 24; w &= 0xc7; t >>= 21;
w |= t` with the `&` writing the reloaded word's register and the shift
writing `t`'s. One expression, `(w & 0xc7) | ((u8)(r - 1) << 3)`, gets
the same instructions with the AND writing the constant's register
instead. Spelling each step as its own statement on a fresh local ties
each result to the right input. The constant trick didn't help here.
The earlier `asm("" : "+r"(ph0))` barrier turned out to be unnecessary
once this was fixed and is gone.

**A scoped `volatile` pointer to fix one address computation
(`ActionCtrlStateCrouch`).** The facing block's second branch computes `part +
0x28` into the part copy's own register (`adds r2, #40`) before
loading -0x11. A plain `u8 *p = &self->part->flags28` gets the order
but puts the address in a fresh register; `volatile u8 *p` gets both.

**A second local for a spilled copy (`LoadLevelSelectRecord`).** The ROM keeps the
level record pointer in r5 and also stores it to `sp+0`, reloading it
from there only for the `time0` test. That is two variables:
`entry = &gLevelTable[levelId]; info = entry;`, with the
`time0` test reading `entry`. `entry` is live across all the calls with
few uses, so it loses its register and is spilled. An `asm` copy in
either direction gives `entry` a register (or computes into the wrong
one); the plain copy doesn't.

**Plain re-reads instead of `volatile` ones (`UpdateExtraLife`).** The draft
read the timer and the id through `vu16` to get the ROM's loads after
the stores. The ROM's loads come after the constant (`movs r0, #216;
lsls; ldrh r2, [r4, #60]; cmp r2, r0`): a pseudo set once from memory
and used once, which local-alloc moves to its use and reload loads. So
the source is a plain re-read (`self->timer = self->timer + 0xc; t =
self->timer;` and `if (self->base.field_08 != 0xffff)`). That took it
from 22 to 8 halfwords. What was left was an r0/r1 swap in the position
updates. Two `asm("" : : "r"(v))` on each velocity local give the
velocity r0 and the position/sum r1. One reference isn't enough.

**The constant trick for an AND's operand order (`ConvertHovercraftTiles`).** In
the nibble loop the ROM copies the hoisted 0xf and ANDs the byte into
the copy (`adds r4, r6, #0; ands r4, r0`); gcc copies the byte. For
`b & 0xf`, gcc keeps the constant second, and after loop hoisting the
two-address `and` copies operand 1, the byte. With `m` from
`asm("" : "=r"(m) : "0"(0xf))` inside the loop and `m & b` in the
source, operand 1 is the mask and gcc copies that. The asm is still
hoisted out of the loop as an invariant (the ROM's `movs r6, #15` ahead
of it). A separate local `c` for the second byte then fixed its
registers. Two more gaps in the loop headers:

- The first loop's reversed counter started from `sum`'s zero register
  in the ROM (`adds r2, r5, #0`), from a constant here. Giving the
  second loop its own counter variable fixes it.
- The second loop's header (`d = dst` copied into `sb` before the
  stride load reuses its register) came out right once each step was
  written as its own statement in the ROM's order: row pointer, height
  address, `d = dst`, `src = row + stride`, `n = *hp`.

The twin `ConvertAirshipTiles` (`actor_part26c.c`, 56 halfwords off) wasn't in
this pass's scope. Its draft has the same nibble loop and would probably
take the same changes.

## What didn't close

| Function | Was | Now | What was observed |
|---|---|---|---|
| `ActionCtrlReleaseHang` | 3 | 3 | The 0x600 is a reload: the ROM puts it in r3, the draft in r2. With the constant trick it becomes a pseudo, which changes the later `ldrsh` offset reloads too (9-14 halfwords). 72 combinations of spellings of the `unk_101` store and the `y` add (both orders; locals, `6 << 8`, `-= -0x600`, a part local) all stay at 3. |
| `InitSaveMenuIcons` (raw) | 5 | 5 | With the constant trick for 0x80, gcc hoists the `asm` too far (into the palette-loop area, 116 halfwords); an `asm volatile` stays put but is no better (20+). |
| `PauseMenuLoop` | 5 | 5 | The ROM computes the fade-in loop's `self+0xcc` (r4) before `disp`; here after. Constant-trick/`"+r"` forms of `disp`, first-loop shapes, a label before `disp`, and a fade pointer local for the fade-in loop don't move it. |
| `MakeLinkHandshakeId` | 11 | 11 | Mask spellings of the tail (`& 0xf0`, `& ~0xf`, `& -16`) and `"+r"` barriers on `hash` make it worse (27-51). The ROM's `sub r0, #0x1f` (-16 from the 15 in r0) looks like reload's move2add, which the draft doesn't trigger. |
| `RunRoom` | 15 | 15 | The one-byte stack argument: an `asm` copy of the direction makes things worse (109); a plain `u8`/`s8`/`u16` parameter or no prototype stores a word. |
| `HitEnemy` | 21 | 21 | Mostly reload scratch registers (the ROM's rotation is r3, r3, r3, r4, r6, r2). The constant trick for the layer's `1` (shared with the `gone` OR) costs 8 bytes. |

## Notes for next time

- When a draft computes the right instructions but ties a result to the
  wrong input, try writing each operation as its own statement on a
  fresh local before reaching for `asm`. It worked for `UpdateSlotCrate`,
  and the header order in `ConvertHovercraftTiles`.
- A load after a constant (`movs; ldrh; cmp`) with no local in the ROM
  is a pseudo set once and used once. Reproduce it with a plain re-read,
  not a `volatile` one.
- A value the ROM keeps both in a register and in a stack slot is two
  variables, one a copy of the other.
- The runners and specs for this pass are in the scratchpad `polish3/`:
  `brute2.py` (now takes `CTX=n` for diff context), `apply.py` (writes
  one variant into the source), `unnaked.py` (drops the `#else` NAKED
  half of a function's `#if NON_MATCHING` split), and `s*.py` specs.
