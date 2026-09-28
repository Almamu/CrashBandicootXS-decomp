# Issues #64/#65: second NAKED retry (0x080352AC-0x08036C00)

This pass retried the five NAKED functions the first retry left
([issue-64-65-naked-retry.md](issue-64-65-naked-retry.md)). Three are
now real C, all under old_agbcc. Two are still NAKED.

| Function | File | Result |
|---|---|---|
| `sub_80360DC` | graphics_loading_35d1c.c | matched |
| `sub_8035E14` | graphics_loading_35d1c.c | matched |
| `sub_80358A8` | graphics_loading_35780.c | matched (needs strength reduction on) |
| `sub_80352AC` | actor_part131.c | still NAKED, draft note updated |
| `sub_803686C` | graphics_loading_35d1c.c | not attempted |

## File split

`sub_80358A8` only matches with strength reduction **on**, and
`sub_8036600` only matches with it off. The old
`graphics_loading_35780.c` was therefore split at the 4-byte-aligned
boundary `0x08035D1C`:

- `graphics_loading_35780.c` has `sub_8035780` and `sub_80358A8`. It is
  old_agbcc with the default flags. `sub_8035780` matches either way.
- `graphics_loading_35d1c.c` has everything from `sub_8035D1C` on. It is
  old_agbcc and keeps `-fno-strength-reduce` (`NO_STRENGTH_REDUCE_OBJS`).
  A whole-file compile of the old file with strength reduction on
  differed only in `sub_8036600` (16 halfwords).

The shared declarations and helpers are copied into both files.
`ldscript.txt` lists the new object right after the old one.

Trying to make `sub_8036600` match with strength reduction on did not
work. It either got reversed (check_dbra_loop runs again in the loop-rerun
pass after the first pass has reduced the givs) or, as a `goto` loop, came
out with permuted registers.

## `sub_80360DC` (18 -> 0)

The `goto` loop shape from the first retry was already right. What was
left was global-alloc priority:

- `stride = 0` makes local-alloc double `stride`'s live length (the `-dl`
  dump: 25 insns in flow, 50 in lreg). That drops its priority below
  `slot`, so the two swap r4/r5. `asm("" : "=r"(stride) : "0"(0))` emits
  the same `movs r4, #0` but is not a constant set, so the length stays
  at 25.
- One empty `asm("" : : "r"(self))` in the loop, and `seedBase`/`zero`
  references after it, give the ROM's r3/r8/ip.
- `off = i << 3` computed before an integer `holdBase` gives the ROM's
  `lsl` first, `add r0, r0, r1` order. Pointer arithmetic
  canonicalizes the operands the other way round.

## `sub_8035E14` (100 -> 0)

- The seed loop is the same `goto` loop. Here a plain `stride = 0` is
  right, and two `i` references let `i` take r3 before `slot`/`stride`.
  The `-1` store is `base + off` with `off` computed first.
- The menu loop is a real `for (;;)`, because the ROM hoists
  `&gUnknown_030012BC` into r6. Leaving it with `goto fadeLoop` instead
  of `break` stops jump.c rotating the loop around the `pressed & 9`
  exit.
- The fade loop is a `goto` loop (nothing hoisted) with its own counter.
  Reusing `i` made `i` cross calls and pushed it out of r3.
- `pressed = gUnknown_030007E0.pressed;` as its own statement gives the
  ROM's `ldrh r5` / `add r1, r5, #0`. The `0x210` zero is a local, so it
  is loaded before the `1`.
- The `register ... asm("r4")` pin on `self` turned out unnecessary.

## `sub_80358A8` (409 -> 0)

Brute-forced piece by piece with the variant runner:

- `ClearOam` is a macro that sets a function-level `zero` before
  loading the DMA base (an inline function evaluated `&oam` first).
- The x offsets use `px`/`dx` locals so fold cannot reassociate the
  `-0x20` onto the other operand.
- The counter addresses compute `(7 - i) << 2` or `k << 2` before
  `self + 0x1e4`, as integers.
- The second slot loop does its DMA through a `dma2` pointer set before
  the loop and has its own counter `k`. loop.c then hoists the DMA base
  into r9 and spills the table pointer, as in the ROM. This took the
  function from 248 to 9 halfwords.
- `oamB.matrixLo = matrix;` (the bitfield store masks), and the shake
  keeps `a - 5` as its own local after the call.
- The last 3 halfwords were `movs r6, #3` coming before the inner
  loop's hoisted invariants instead of after them. That order is
  check_dbra_loop's: the reversed loop's new start value is emitted
  after the movables. Writing the loop up-counting (`j = 0; j < 4`) and
  building with strength reduction on reproduces it exactly, hence the
  split.

## `sub_80352AC` (not closed)

The extra spilled `slot << 5` is not from loop.c. The `-dG` dump shows
GCSE's PRE inserting `slot << 5`, `slot + 1` and `i + 1` at the end of
the y loop's pre-test block ("PRE/HOIST: end of bb 7"). The ROM has
only the last two. `-fno-gcse` removes it but breaks much more. These
all leave it hoisted or add a copy: every `palSlots[slot]` spelling
(shift/multiply, integer/pointer, narrowed types), index-form copy
loops, and an asm-opaque copy or `"+m"` on `slot`. Something must stop
PRE from treating `slot << 5` as anticipated at that point without
blocking `slot + 1`. The draft note says this.

## `sub_803686C`

Not attempted for lack of time. Its slot pointer walks while `i`
counts up, like `sub_80358A8`'s strength-reduced loops. So it may also
need strength reduction on, which would mean another split, or moving
it next to `sub_80358A8`'s object if the boundaries allow.

*Later pass (big NAKED retry):* a full C draft is now under
`#if NON_MATCHING`, 329 halfwords off. It does need strength reduction
on. Still NAKED; see [big-naked-retry.md](big-naked-retry.md).

## Techniques worth reusing

- **Local-alloc doubles the live length of a pseudo set from a
  constant.** That can drop an induction pointer below its siblings. An
  `asm("" : "=r"(x) : "0"(0))` start avoids it.
- **A `-dG` dump separates GCSE PRE hoists from loop.c moves.** The
  `PRE/HOIST` lines name the inserted expressions.
- **A count-down loop whose start comes after the hoisted invariants is
  check_dbra_loop's.** Write the loop up-counting and build it with
  strength reduction on.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.

*Later pass (size2 NAKED retry):* `sub_80352AC` is matched; see
[size2-naked-retry.md](size2-naked-retry.md).
