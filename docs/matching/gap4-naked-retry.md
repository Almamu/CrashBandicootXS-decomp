# NAKED retry (gap4): 1 of 4 closed

This pass took the four drafts that the mix-6 pass
([mix6-naked-retry.md](mix6-naked-retry.md)) ran out of time for.

| Function | File | Start | Result |
|---|---|---|---|
| CreateWumpa (#15) | `src/system/game_loop53.c` | 95 hw, 312 vs 308 B | **Closed**, real C, old_agbcc (the file already builds with it). |
| ResolvePlatformCollision (#25) | `src/graphics/actor_part_1ab98.c` | 644 hw, 1608 vs 1648 B | Not closed. The draft is now 1640 B. What's left is register copies and reload phase. |
| CreateCrate (#13) | `src/system/game_loop36.c` | 508 hw, 1416 vs 1396 B | Not closed. The type/slot*2 swap is fixed and the draft is 1388 B. Placement-record pointer copies are still missing. |
| UpdateDingodileShield (#24) | `src/graphics/actor_part_1967c.c` | 159 hw, 392 vs 404 B | Not closed. Only the note changed. |

## CreateWumpa

The last draft had `x`/`y`/`id`/`special` in r4-r6/r8 (local-alloc) and
`self` in r7. The ROM's layout is `self` r4, x r5, y r6, `id` r8,
`special` sb and `mode` r7. Local-alloc never uses r7, so the only
problem was r4. Pinning `self` with `register ... asm("r4")` makes r4
unavailable to local-alloc in the first block. It then picks
r5/r6/r8/sb for the parameters in the ROM's order, and global-alloc
gives `mode` r7. The parameter copy/truncation sequence comes out as in
the ROM, with no extra references needed.

Three smaller pieces:

- After the list `if`/`else`, the code uses an unpinned copy `p`. With
  `self` a hard register all the way through, CSE stops holding
  `&self->tag` in r5 across the anim-setup calls.
- `phase` is `0` followed by `asm("" : "+r"(phase))`. The frame clamp
  compares it with the frame count (`cmp r6,r0`), the stored frame is a
  fresh `0`, and +0x4B stores `phase`. The `"=r"`/`"0"` form routes the
  constant through r0 and adds a `mov`.
- The tag is stored through a pointer with a `u8 one` local, so that
  `mov r6,#0` lands between the tag address and its `strb`.

`mode` needs nothing special. A plain `u8 mode = 0` before the first
call is lost by CSE at the list join, so the ROM's dead
`cmp r7,#0xff` stays.

## ResolvePlatformCollision (1608 to 1640 bytes)

- **Missing jump table.** The 40 missing bytes were the 9-entry jump
  table for `switch (result)`. Cases 1 and 2 share a label and merge
  into one range, so gcc saw only three ranges and built a compare
  tree. An empty `case 0: break;` gives four ranges, and gcc then emits
  the ROM's table.
- **Load order in the ox/oy computations.** They go through a
  `Span(x, w, o)` inline. The arguments are evaluated first, which
  gives the ROM's three stack loads before the add/sub. Both arms then
  cross-jump like the ROM's.
- **`result = hdir` fallback.** The fallback for the ty range test is
  the `else` arm, with a shared `set_hdir:` block at the end. The
  `above` side's `ble; b` far branch now appears.

Four instructions are still missing. All of them are register copies:
padY into r3, and `add r2,r5` / `add r5,r2` around the FindLineCrossing
arguments. The ROM also keeps `&b` in r4 into the no-collision switch.
A `pb = &b` local keeps it live through the whole function and is much
worse.

## CreateCrate (1416 to 1388 bytes)

- **type/slot*2 swap.** Three `asm("" : : "r"(type))` references fix
  it: type goes to r7 and slot*2 to r8. One or two references are not
  enough. This removed the hi-register moves around every `type`
  assignment.
- **Case 1.** It reads the record bit as `(u32)(rec[0] << 25) >> 31`,
  which gives the ROM's `lsl #25; lsr #31`.
- **Case 15.** The table index is `(u32)(... & 0x38) >> 3`, which gives
  the ROM's `lsr`.

Still open:

- The ROM copies the placement-record pointer in three places: the 0xb
  pre-check, the `flagged` block and case 15. This looks like GCSE's
  reaching-register copy, but re-reading `PLACEMENT(slot)` there either
  changes nothing or recomputes the whole chain.
- `id`, `self` and `&gLevelState` are in different registers from
  the ROM.
- Case 15's two ands are folded into one. Two `&=` statements keep both
  ands but change the stack frame.

## UpdateDingodileShield

The size gap is only the hi-register saves. The ROM's four long-lived
values are allocated in the same order as the draft's, but from r7
instead of r5. r5 and r6 must therefore hold something live across the
first half that emits no code. Pinning `bld` to r5 and setting it before
the first call shifts everything one step toward the ROM. Nothing
natural was found for r6.

## Tools

The variant specs for `var.py`, `d.py` and `calls.py` are in the session
scratchpad under `gap4/`. `calls.py` aligns the compiled `.s` against
the NAKED text on opcodes with the registers stripped. That is how the
missing jump table showed up. Nothing from the scratchpad is committed.
