# strag4 retry: issue #18's last function (`sub_8015038`)

The strag2 retry (`docs/matching/strag2-naked-retry.md`) left
`sub_8015038` (`src/graphics/actor_part38.c`, 400 bytes) NAKED. Its
C draft was 2 halfwords off under both compilers.

**1 of 1 closed**, under old_agbcc (`actor_part38.o` was already on
`OLD_AGBCC_OBJS`). It was the last function in issue #18.

## Closed

| Function | File | Was | Technique |
|---|---|---|---|
| `sub_8015038` | `actor_part38.c` | NAKED (draft 2 hw off) | test `self[0x22]` directly in the `self+0x24 != 0` arm instead of through a `u8 v` local; one no-code `r1` hold spanning the test |

## What was wrong

- The ROM's `adds r5, r0, #0; ldrb r2, [r5]` at the top of the `else`
  arm is not a GCSE *copy*. It is a PRE insertion. The RTL dump
  (`-da`, `.gcse`) shows old_agbcc inserting a fresh
  `reg = self + 0x22` at the **end of the block**, just before the
  compare. cse2 later turns it into a copy of the first computation.
- With `u8 v = self[0x22]`, the load is one `zero_extend` insn, placed
  before that insertion point. So the draft loaded through `r0` and
  copied afterwards.
- With `if (self[0x22] > 0xf0)`, the load is a QImode load followed by
  an extension. combine merges them at the position of the later insn,
  which is after the insertion. The load then goes through the PRE
  register (`r5`), as in the ROM. The first arm's `== 1` test already
  had this shape, which is why its copy/load order was right.
- This explains why pointer locals, `"+r"` escapes and barriers didn't
  help: none of them moves the load relative to the end of the block.
  An inline `At22(self)` accessor and an identity `Id8(self + 0x22)`
  inline were tried too. Both folded away and left the draft 2 off.

## Holds

- One `register s32 hold1 asm("r1")` is set before the test and used
  at the top of the `> 0xf0` arm. It keeps `r1` live so the byte loads
  into `r2` and `id` goes to `ip`, as in the ROM. Without it the
  function is 113 halfwords off.
- The strag2 draft's `r0` hold (first arm) and `r2` hold (`0x17`) are
  no longer needed under old_agbcc, so they were dropped.
- Current agbcc is still 54 halfwords off with this form. The object
  stays on `OLD_AGBCC_OBJS`, and `sub_8014F8C` in the same file matches
  under both compilers.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`
  prints `crashbandicootxs.gba: OK`.
- `huge3/bindiff.py` (branch targets kept): no differences.
