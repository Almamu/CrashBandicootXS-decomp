# Issue #16 remainder: 0x08011BD4-0x08012D24 (10 functions)

Continuation of
[issue-16-actor-11b0c.md](./issue-16-actor-11b0c.md), which matched the
chunk's leading 15 `struct actor` functions and left the remaining 10
completely untouched, citing `docs/rom_map.md`'s own conclusion that
this "child object" family (a per-level 42-slot action dispatch table,
`gStaticData_0816BF20`) "isn't understood with byte-exact precision
yet." This pass picked that remainder back up: 4 of the 10 turned out
tractable with the same base+offset+fn-pointer trampoline and
state/flag/table-index-trio conventions `actor_part18.c`/
`actor_part18b.c` already established for other members of the same
table; the other 6 (including the two hardest - `sub_8011BD4`'s
25-case/7-case nested jump table and `sub_8012AF4`'s stack-array/`r8`
usage) were left raw again, still out of scope.

## New files

`asm/code_3_2_17_11bd4.s` is trimmed to just `sub_8011BD4` (unchanged
start address, 0x08011BD4-0x08012160). `sub_8012160`/`sub_8012238`/
`sub_80122CC` (contiguous, 0x08012160-0x08012420) move to a new
`src/graphics/actor_part79.c`. A new `asm/code_3_2_17_12420.s` picks up
`sub_8012420`/`sub_8012694`/`sub_801283C` (0x08012420-0x08012A7C).
`sub_8012A7C` alone (0x08012A7C-0x08012AF4, not ROM-adjacent to either
matched group) moves to a new `src/graphics/actor_part80.c`. A final
new `asm/code_3_2_17_12af4.s` picks up `sub_8012AF4` onward - this file
is much wider than issue #16's own range, since `code_3_2_17_11bd4.s`
already covered everything through `sub_801426C-1` before this pass
started; only the portion up to `sub_8012FBC` (0x08012FBC) is actually
issue #16's remaining scope, the rest already belonged to later,
separately-tracked chunks. `ldscript.txt` and `tools/report_units.py`'s
`UNITS` list were updated to place all five pieces in that exact link
order.

## Matched (4/10)

- **`sub_8012160`**: plays a sound (id `0x1b`), fires the `+0x50`/
  `+0x54` trampoline pair with the function's second argument as the
  "part" object, then the `+0x20`/`+0x24` pair (id `0x1d`), resets
  both halves of the state/flag/table-index trio via a single walked
  pointer (the `sub_8011B90`-style idiom from `actor_part39.c`), runs
  `sub_8012AF4`, clears/sets a few more `self+0x10`-record bytes
  (`+0x100`/`+0x102`/`+0x103`/`+0x104`, and two bits of `+0xc` via the
  established negative-constant-mask idiom), calls `sub_8023234`, then
  looks up a byte through the 28-byte-record-array dereference chain
  `docs/rom_map.md`'s "eight more core reads" section already
  documented from three other call sites (`part+0x20 -> *ptr +
  tag*0x1C`, reading byte `+0x14`) to feed `sub_8006D08`. Needed the
  most register-pinning of the four: the `0x104` field-write's offset
  is small enough (0x104 = `0x82<<1`) that the compiler always
  constant-folds it into whichever scratch register it likes (r2)
  regardless of any `register`-variable hint aimed at that offset
  computation, the opposite of the ROM's r1 - only an inline-asm
  anchor computing the shift itself (`asm volatile("mov %0, #0x82\n\tlsl
  %0, %0, #1")`) pins that specific register, the same escape hatch
  `actor_part39.c`/`graphics.c` already use for shift results elsewhere.
  The closing dereference chain also needed a compound-assignment
  register pin (`record += (s32)base;` rather than `base + tag*0x1c`)
  to force the ROM's `Rd==Rs` `adds r2, r2, r4` encoding instead of the
  compiler's own (numerically identical, but differently-encoded)
  `adds r2, r4, r2`.
- **`sub_8012238`**: if the player (`gUnknown_030012D8`)'s `+0x100`
  flag is set, dispatches on the player's `+0x2d` type byte - `0x12`
  (only when `+0x60` is nonzero) or `0xd`/`0x18` re-tag the player
  `0x25`/`0x26` and fire the standard `sub_80087C0`/`sub_80087B4`/
  `sub_800872C(..., 0)` teardown trio; otherwise, while the flag is
  clear, a player type of `0x25`/`0x26` plays a sound and resets the
  trio via `sub_8015780`. The `type > 0x12` branch needed an explicit
  `goto check_18:` block (rather than a nested `if`) to keep the
  compiler from inverting it into the opposite branch/fallthrough pair,
  the same "goto forces exact fall-through shape" technique
  `actor_part39.c`'s `sub_8011A8C` already used, plus a `s32` (not
  `u8`) type for the compared byte to get the ROM's signed `bgt`
  instead of an unsigned `bhi`. The final `type2 == 0x25 || type2 ==
  0x26` check needed splitting into two literal `if`/`goto` comparisons
  - the natural `||` form gets optimized into a single `(u8)(type2 -
  0x25) <= 1` range check, which the ROM doesn't do.
- **`sub_80122CC`**: reads D-pad input (via `sub_8000760`, kept only
  for its side effect on register allocation - the result feeds later
  comparisons) and dispatches `self+8`'s type through a 39-entry jump
  table (values 0-0x26; anything higher returns 0 directly without
  touching the table). 16 of the 39 case values share one body: clear
  `part+0x28` bit `0x20`, then on bit `0x10` set, either re-clear it
  (D-pad remap `4`/`6`/`8`) or set it back while also restoring bit
  `0x10` (remap `3`/`5`/`7`), setting `self+0x2f`=1/`self+0x29`=0
  either way and returning 1; every other case/path returns 0. A plain
  C `switch` over the 16 "true" case values with a `default: goto end;`
  reproduces the ROM's dense 39-slot jump table byte-for-byte (gcc
  fills the unlisted values with the default target automatically).
  The two near-identical `case`-body copies needed heavy register
  pinning to stop the compiler from noticing they're structurally
  identical and cross-jumping them into one shared tail - the ROM
  keeps them duplicated with slightly different register choices in
  each copy (`ldrb r3`/self+0x29 via `r2` in one, `ldrb r5`/self+0x29
  via `r1` in the other), and reproducing those exact register choices
  (not just the logic) was what kept the compiler from merging them.
- **`sub_8012A7C`**: if `part+0x68` bit 3 is set, returns 0 (busy).
  Otherwise, on `part+0x69 > 2`, fires the `+0x20`/`+0x24` trampoline
  pair (id `0x1a`) then the `+0x50`/`+0x54` pair (id `0x1b`); on
  `part+0x69 <= 2`, fires only the `+0x20`/`+0x24` pair (id `0x1c`).
  Either way, sets the second half of the trio (`self+0x32`=0/
  `+0x30`=1/`+0x28`=4) and returns 1. Needed the same negative-constant
  bit-clear idiom for the `+0x68` flag test (`mask &= *p; if (mask ==
  0) { ... } return 0;` - an early return inverts the ROM's fall-through
  shape, so the whole "matched" body has to live inside the `if`, with
  the trailing `return 0;` as the block's fallthrough) plus a
  register-pinned constant (`four`, r2) to get the ROM's fresh-address
  computation for `self+0x28` (`adds r0, r4, #0; adds r0, #0x28`)
  instead of a cheaper chained `subs` off the two prior fields' shared
  pointer.

## Left raw (6/10)

`sub_8011BD4`, `sub_8012420`, `sub_8012694`, `sub_801283C`,
`sub_8012AF4`, `sub_8012D24` stay exactly as
[issue-16-actor-11b0c.md](./issue-16-actor-11b0c.md) already
characterized them:

- `sub_8011BD4` (1420 B) is the documented 25-case jump table with a
  further 7-case sub-dispatch, sharing the type-`0x1d` gate with
  `sub_8016288` (also still raw) - a substantial companion state
  machine, not attempted this pass.
- `sub_8012420`/`sub_8012694`/`sub_801283C` are further members of the
  42-slot action-dispatch table, real coverage in `docs/rom_map.md` but
  not read closely enough here to attempt byte-exact matching.
- `sub_8012AF4` (284 B of ROM, the widest of the six by instruction
  count) uses a `struct { s16; s16; s16 }`-shaped stack-local record
  copied via `ldm`/`stm` from `gStaticData_0816B304`, plus `r8` for a
  cross-call-preserved value - a real step up in register-allocation
  complexity from the four matched functions above.
- `sub_8012D24` is a further sibling/callee of the same family.

Given the two hardest members of this remainder (`sub_8011BD4`,
`sub_8012AF4`) are exactly the kind of function `docs/rom_map.md`
already flagged as needing a dedicated pass, and this session's four
matches already represent real, verified progress, these six stay raw
rather than forcing a low-confidence match. Issue #16 stays open for
whoever picks up this remainder next.
