# Issue #13: 0x0800FC70-0x08010A0C (graphics -> game_loop)

25-function `decomp-chunk` covering the ROM span right after issue #12's
`0x0800D040`-`0x0800FC70` physics/collision-subsystem chunk left off
(see `docs/matching/issue-12-physics-collision.md`) - same
`asm/code_3_2_17_e560.s` source file, same neighborhood. The chunk
generator's `graphics` label for this span was always a placeholder
(the pre-existing category `report_units.py` inherited from the
already-matched `graphics.c` code far before it, per issue #12's own
recategorization note); this issue's own address range sits squarely
inside `docs/rom_map.md`'s confirmed shared physics/collision
subsystem (`~0x0800D000`-`0x08010D54`, entered from multiple different
entity-type vtables as shared infrastructure), so `tools/report_units.py`
now recategorizes `0x0800FC70`-`0x08010A0C` `graphics` -> `game_loop`
too, continuing issue #12's precedent.

## Matched (11 of 25 functions)

- **`sub_800FEB0`** (`src/system/game_loop17.c`) - resets `self`'s
  collision-response state: sets flags `+0xc` bits 2/6, clears the low
  7 bits of `+0x4d` while also clearing the global
  `gUnknown_030012D8+0x80` "hit" latch, zeroes the timer/list-link
  block `+0x44`-`+0x51`/`+0x58` and the two neighbor-list pointers
  `+0x5c`/`+0x60`, and sets the `+0x54` countdown to -1 (disabled).
  Needed an inline-asm anchor for both `mask & self[0x4d]`-and-store
  sequences: the ROM computes the `0x7f`/`0x80` mask immediate
  *before* the `ldrb` byte load in each one (`movs r0,#mask; ldrb
  r4,[r3]; ands r0,r4; strb r0,[r3]`, loaded byte in r4, mask/result in
  r0), but every plain-C phrasing tried (compound assignment either
  direction, a named `mask`/`loaded` pair with and without register
  pins) instead had this compiler either load the byte first or land
  the AND result in the wrong register - anchoring the literal
  instruction sequence via `asm volatile` with `self`/the field address
  separately pinned to `r2`/`r3` was more reliable than continuing to
  chase the scheduler. Also needed the first block split so the
  following `zero = 0` assignment lands *between* the `ands` and the
  `strb` (the ROM interleaves `movs r1,#0` there, reusing that same
  zero for every later zeroing store) rather than after the whole
  inline-asm block.
- **`sub_80106DC`**/**`sub_8010708`**/**`sub_801070C`**/
  **`sub_8010710`**/**`sub_8010714`**/**`sub_8010718`**
  (`src/system/game_loop18.c`) - the viewport collision-box refresh
  (`sub_8010B6C` on `gUnknown_030012D8+0x108`, then a saturating-at-
  zero `+0x92` hit counter), and the neighbor-list "get prev"/"get
  next"/"set prev"/"set next" accessor quartet (`self+0x60`/`+0x5c`)
  `docs/rom_map.md` already ties to `sub_0800D18C`'s linked-list walk.
  `sub_8010718` is a trivial `return 3;` constant accessor with no
  caller anywhere in the ROM (checked every `asm/*.s`, `expected/*.s`
  and `src/*.c` file) - tagged `UNUSED`. All six matched with no
  gotchas beyond `sub_80106DC` needing its second `gUnknown_030012D8`
  dereference kept as a separate local (not reusing the first) to get
  the post-call reload the ROM does.
- **`sub_8010804`**/**`sub_801085C`** (`src/system/game_loop19.c`) -
  a state-3-countdown-expiry sweep over the `gUnknown_0300130C` object
  list (same list/table layout `sub_800F1B8`/`sub_800F258` elsewhere in
  this still-raw region read), and a viewport `+0x18`-table trampoline-
  pair/cue-1 firer gated on `+0xc` bit 7. `sub_8010804` needed the
  `&gUnknown_0300130C` address cached into its own local declared
  *inside* the `if` guard (not before it) to match the ROM's own
  "check with one register, cache into a second only once past the
  check" shape. `sub_801085C` needed the same inline-asm-anchor
  treatment as `sub_800FEB0` for `flags >> 7` (ROM keeps the loaded
  byte in `r1` and the shifted result in a separate `r0`; this compiler
  always shifts in place) - and a `u32` (not `u8`) result type on the
  asm output operand to avoid a spurious truncation instruction the
  ROM doesn't have.
- **`sub_8010908`** (`src/system/game_loop20.c`) - trivial
  `gStaticData_0816BBAE[idx]` byte-table lookup; its first parameter is
  unused in the ROM.
- **`sub_8010A00`** (`src/system/game_loop21.c`) - extracts `self+0x48`
  bits 6-7. Needed a trailing `asm(".align 2, 0")` - the function body
  is 10 bytes (not 4-aligned), and the ROM pads the 2-byte gap before
  the next function with a zero halfword rather than the assembler's
  default `nop` (`mov r8, r8`) - the standard alignment-padding gotcha
  (`matching_decomp_alignment_fix`).

## Left untouched (14 of 25 functions)

- **`sub_800FC70`**/**`sub_800FDC8`** (`asm/code_3_2_17_e560_fc70.s`) -
  a position-wrap advance function with heavy `r8`/`sb` register
  pressure, and a Bresenham-line-style step algorithm. Readable at a
  high level but not attempted this pass.
- **`sub_800FF0C`** (`asm/code_3_2_17_e560_ff0c.s`) - a large
  (~660-instruction) projectile/hazard-spawn dispatcher with two big
  jump tables (19 and 23 cases) and packed bitfield arguments - out of
  scope for a single pass, would need its own dedicated chunk.
- **`sub_8010480`**/**`sub_80104E4`**/**`sub_8010674`**
  (`asm/code_3_2_17_e560_ff0c.s`) - a moderate flag-dispatch function, a
  large (~195-instruction) state dispatcher calling several still-raw
  siblings, and an AABB-overlap check. Not attempted this pass.
- **`sub_801071C`**/**`sub_801075C`** (`asm/code_3_2_17_e560_1071c.s`)
  - two part-object init helpers (`sub_80087C0`/`sub_80087B4`/
  `sub_800872C` shape, same as several already-matched siblings in this
  file) - readable but not attempted this pass.
- **`sub_8010784`**/**`sub_80107C4`** (`asm/code_3_2_17_e560_1071c.s`)
  - two more Bresenham-line-style step algorithms (variants of
  `sub_800FDC8`'s shape) - not attempted this pass.
- **`sub_801089C`** (`asm/code_3_2_17_e560_1089c.s`) - a cue-3 SFX plus
  spawn-helper call; not attempted this pass.
- **`sub_8010914`**/**`sub_801095C`** (`asm/code_3_2_17_e560_10914.s`)
  - two neighbor-list-walk-and-filter helpers (built on
  `sub_8010708`/`sub_801070C`) - not attempted this pass.
- **`sub_80109A4`** (`asm/code_3_2_17_e560_10914.s`) - a distance-gated
  dispatcher calling `sub_0800D18C` (still-raw); not attempted this
  pass.

No functions were parked (`NON_MATCHING`) this pass - every function
attempted reached a byte-exact match; the rest were left fully raw
rather than force a low-confidence attempt.

## Splitting `asm/code_3_2_17_e560.s`

Following this project's "cut at the boundary" convention (see
issue #12's/issue #56's own write-ups for the same pattern), the
original single `asm/code_3_2_17_e560.s` was split into a head
fragment (unchanged name, everything before `sub_800FC70`) and six new
untouched-fragment files named by the lower 5 hex digits of their first
function's address (`..._fc70.s`, `..._ff0c.s`, `..._1071c.s`,
`..._1089c.s`, `..._10914.s`, `..._10a0c.s` - the last one is the tail,
everything from `sub_8010A0C` onward, outside this issue's range),
interleaved with the five new matched `.c` files in `ldscript.txt`'s
link order.

## Cross-references

- `docs/status/game_loop.md` - matched/parked/raw lists updated,
  including the `graphics` -> `game_loop` recategorization for this
  span.
- `tools/report_units.py` - `UNITS` list split/recategorized for
  `0x0800FC70`-`0x08010A0C`.
- `docs/rom_map.md` - "Confirmed: a shared physics/collision
  subsystem, entered from multiple different entity types" is the
  read-only reconnaissance this issue's category correction is based
  on.
