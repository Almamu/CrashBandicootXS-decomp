# Issue #41: 0x08025894-0x08025FC8 (game_loop, 25 functions)

Source: `asm/code_3_2_17_255d4.s` at issue-generation time; by the time
this was picked up the file had already been further split by earlier
matched work in this neighborhood, so the real functions lived in
`asm/code_3_2_17_255d4.s` (the tail beyond `sub_80255D4`).

This chunk turned out to be two interleaved families:

1. A **bit-grid accessor family** (`sub_8025944`/`sub_8025968`/
   `sub_802599C`/`sub_80259D4`/`sub_8025A0C`) - three overlapping
   32-bit-word-per-row bitmap arrays living at `self+8`/`self+0x208`/
   `self+0x308`, using the exact same "floor-divide-by-32, adjust for
   negative n by `+0x1f` before the shift" idiom already proven in
   `sub_8025554`/`sub_8025588` (game_loop5.c, issue #40). Getting these
   byte-exact took real iteration on **statement order**, not register
   pins: this compiler folds a `base + fixedOffset + (index << 2)`
   pointer expression differently depending on whether the fixed
   offset and the shift are computed as two separate statements (`base
   += fixedOffset; word = base + shifted;`) versus one combined
   expression - the ROM's own instruction order (compute the shifted
   index first, *then* add the fixed offset, *then* combine) only came
   out when written as three separate C statements in that same order.

2. A **BG-scroll-layer / tile-streaming family**
   (`sub_8025D74`/`sub_8025DE8`/`sub_8025E2C`/`sub_8025E70`/
   `sub_8025E84`/`sub_8025E98`/`sub_8025F24`/`sub_8025F3C`) - a
   per-BG scroll-layer object (distinct from, but structurally similar
   to, `game_loop6.c`'s viewport/parallax-scroll-layer family) that
   caches hardware `BGnCNT`/`BGnHOFS` register addresses at
   construction (`sub_8025D74`), grows a streamed tile range one row/
   column at a time firing a per-layer trampoline on each step
   (`sub_8025DE8`/`sub_8025E2C`, with `sub_8025E70`/`sub_8025E84` as
   their plain-clamp-only counterparts), computes the four screen-edge
   tile coordinates and drives the whole streaming update each frame
   (`sub_8025E98`), and separately streams decoded tile data into a
   circular row buffer (`sub_8025F3C`) before the final hardware-
   register write (`sub_8025F24`).

3. A **part-object spawn family**
   (`sub_8025A64`/`sub_8025B0C`/`sub_8025BAC`/`sub_8025CA4`) and one
   **table-indexed function-pointer dispatcher** (`sub_8025D28`) and a
   **jump-table list counter** (`sub_8025894`) - these reuse the
   `struct actor` + `gUnknown_030012D0` triple-indirection convention
   already established in `actor_part8.c`/`trigger_effect.c`.

## Matched (16 of 25)

`sub_8025944`, `sub_8025968`, `sub_802599C` (game_loop12.c),
`sub_8025A0C`, `sub_8025A3C`, `sub_8025A44`, `sub_8025A5C`
(game_loop13.c), `sub_8025D28`, `sub_8025D4C`, `sub_8025D54`,
`sub_8025D6C` (game_loop14.c), `sub_8025DE8`, `sub_8025E2C`,
`sub_8025E70`, `sub_8025E84` (game_loop15.c), `sub_8025F24`
(game_loop16.c). All confirmed via the full clean `make compare`
cycle, not just isolated compiles.

`sub_8025D28` is worth calling out: it's a table-indexed
function-pointer dispatch (`fn(self, p1, p2, p3)`), and on real
hardware an indirect call through a stored function pointer has to go
through one of this ROM's fixed per-register interworking trampolines
(`src/system/reg_trampolines.c`, `sub_803AD78`-`sub_803AD94` - "bx
r0" through "bx sp"). *Which* trampoline gets used is not something
the C source picks - it falls out purely of which register this
compiler's allocator happens to land the function pointer in for that
particular call, matching `sub_803AD80`'s existing use in
`actor_part8.c`'s `sub_8009F1C` (there, naturally in `r2`, the 3rd
AAPCS argument register). For `sub_8025D28`'s case the ROM picked
`r5`/`sub_803AD8C`, which required:

- `register void *fn asm("r5") = ...;` to force the fn-pointer local
  into `r5`.
- An empty `asm("" :: "r"(fn));` barrier right before the call - since
  the target (`sub_803AD8C`) takes no C parameters, without this the
  compiler saw no use of `fn` and optimized the whole load away.
- Matching the ROM's own read order for the table-index/shift
  computation needed one more explicit register pin
  (`register u16 idx asm("r3")`) to land the intermediate load in the
  same register the ROM uses, rather than reusing the shift's own
  destination register.

## Parked, not byte-matching (9 of 25)

All nine are semantically traced against the ROM (every field offset,
branch and call confirmed) but not iterated to an exact register
allocation within this issue's scope - each has its own `PARKED, NOT
BYTE-MATCHING` doc comment at the definition explaining what was tried:

- **`sub_8025894`** (game_loop12.c) - the group/item list counter.
  Every operand matches; the remaining gap is that the ROM's
  `item->type == 0x1a` lookup branch does its whole four-load chain
  using only two scratch registers (`r0`/`r1`, aggressively reusing
  each as soon as it's dead), while every C shape tried here needs a
  third register that collides with the outer loop's `i` counter and
  forces an extra `r7` push/pop.
- **`sub_80259D4`** (game_loop13.c) - the dual bit-grid setter. The
  ROM is a true leaf function (`self` pinned to `ip`/`r12` for the
  whole body, no `push`/`pop` at all); pinning `self` to `ip` here
  gets every operand right but this compiler still inserts an
  unnecessary `push {r4, lr}`/`pop {r4}` pair around the two address
  computations.
- **`sub_8025A64`/`sub_8025B0C`/`sub_8025BAC`/`sub_8025CA4`**
  (game_loop14.c) - four part-object spawn helpers, sharing the
  `struct actor` + `gUnknown_030012D0`-table + OAM/keyframe-trio
  construction idiom already established in `trigger_effect.c`'s
  parked `sub_8020E84` family. Large, register-heavy functions; not
  iterated to an exact allocation within this chunk's scope.
  `sub_8025BAC`'s two stack-passed parameters (`testY`/`mirrorFlag`)
  are worth a follow-up look: `sub_8025B0C`'s own call site to it
  doesn't visibly set them up before the `bl`, so they land on
  whatever `sub_8025B0C`'s own `sub sp, #0x28` scratch buffer happens
  to hold at that point - an implicit stack-reuse coincidence not
  fully resolved here (passed as `0`/`0` in the C reconstruction,
  flagged with a `NOTE` at the call site).
- **`sub_8025D74`** (game_loop15.c) - the BG-scroll-layer hardware-
  register/bitfield initializer. Every field/offset confirmed; two
  small gaps remain in the `& -0x20`/`& -0xd` bitfield masks and in
  hoisting `bgIndex+0x1c` into a register that stays live across the
  `sub_8024DAC` call (see the function's own doc comment for detail).
- **`sub_8025E98`/`sub_8025F3C`** (game_loop16.c) - the screen-edge
  tile-coordinate computer/streaming driver, and the circular-buffer
  decoded-tile streaming loop. Both are large, register-heavy
  functions (`sub_8025E98` keeps `r8` live across most of its body);
  not iterated to an exact allocation within this chunk's scope.

## Build layout

`asm/code_3_2_17_255d4.s` (which held `sub_80255D4` through
`sub_8025FC8` and beyond) got split into six raw fragments around the
five matched runs, per `docs/workflow.md`'s "everything before,
everything after" rule:

- `asm/code_3_2_17_255d4.s` (truncated) - `sub_80255D4`, `sub_8025894`
- `asm/code_3_2_17_259d4.s` (new) - `sub_80259D4`
- `asm/code_3_2_17_25a64.s` (new) - `sub_8025A64`-`sub_8025CA4`
- `asm/code_3_2_17_25d74.s` (new) - `sub_8025D74`
- `asm/code_3_2_17_25e98.s` (new) - `sub_8025E98`
- `asm/code_3_2_17_25f3c.s` (new) - `sub_8025F3C` onward (everything
  after this issue's range, still raw, untouched)

Each parked function's `NON_MATCHING` C reconstruction lives in the
`.c` file holding the *adjacent* matched run (following the existing
`sub_8008770`/`sub_800E494` "widens this unit" convention documented
in `tools/report_units.py`) rather than in a standalone file: under
the real (`NON_MATCHING=0`) build each `.c` file only contributes its
matched functions' bytes (the guarded parked block compiles to
nothing), so `ldscript.txt` still places the untouched raw asm
fragment immediately after it at the correct address. `sub_8025944`'s
own file (`game_loop12.c`) needed one addition beyond this: an
explicit trailing `asm(".align 2, 0")` after its last matched
function, reproducing the ROM's own 2-byte zero-fill between
`sub_802599C`'s end and `sub_80259D4`'s start - without it, `ld`'s
default inter-object padding (a `mov r8, r8` NOP, not zero bytes) broke
the checksum by exactly those 2 bytes. The other four matched-run/
parked-run boundaries in this chunk didn't need this fix since the
ROM's function lengths there already land on 4-byte boundaries with no
padding at all.

Full clean `make compare` passes: `crashbandicootxs.gba: La suma
coincide`.
