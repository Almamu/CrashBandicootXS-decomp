# DrawAffineSpritePieces retry (issue #9)

`DrawAffineSpritePieces` (`src/gfx/affine_sprite_pieces.c`, 1044 bytes) was the last
NAKED function in issue #9's range. Its old `NON_MATCHING` draft was 468
halfwords off (1032 bytes under agbcc, 1024 under old_agbcc). It is now
matched as real C, with no pins and one extra-reference nudge.

| Function | File | Compiler | Before | Now |
|---|---|---|---|---|
| `DrawAffineSpritePieces` | `src/gfx/affine_sprite_pieces.c` (added to `OLD_AGBCC_OBJS`; the only function in the file) | old_agbcc | 468 | MATCH |

## Compiler

The ROM loads the `0xf` mask before the `ldrb` it is combined with
(`movs r3, #0xf; ldrb r0, [r0]; ands r0, r3`), in the loop head and in
the first-piece centre. agbcc loads the byte first, old_agbcc matches.
The final draft is still 247 halfwords off under agbcc.

## The spilling

The earlier notes were right that this is reload spilling, not memory
locals: every long-lived value (params, `total`, `info`, `tile`,
`scale`, the six pull/base values, `i`, `id`, `tiles`, the flag-pointer
copy) gets a slot in pseudo order. The draft's problem was not register
pressure but reload's register rotation. `allocate_reload_reg` goes
round-robin through the set of all registers spilled anywhere in the
function. In the ROM that set is r0-r4 **and r7**, which is why the
six zero-inits use r4, r7, r0, r1, r2, r3 and the loop reloads land in
r7. The draft never spilled r7, so every reload register after the
prologue was off by one step.

r7 gets spilled at the first-piece centre. The ROM takes the size-table
addresses `&W[id0]`/`&H[id0]` before it reads `pos`. With `cx`, `cy`,
`id0` and both addresses live in r0-r4, the `pos` reload needs r7. So
the source takes the two addresses into pointer locals before
`cx += pos[0]` (`pw`/`ph`). That one change took the draft from 381 to
169 halfwords.

## The other source shapes (169 to MATCH)

- **Pull computation (i == 0) interleaved per axis:** `pullX = x + W/2;
  pullY = y + H/2; pullX -= cx; pullY -= cy;` scale both, negate both.
  This is the ROM's store pattern into the `pullX`/`pullY` slots
  (169 → 103). The `else` arm uses `dx`/`dy` temps in the same
  interleaved order.
- **Matrix index first:** `k = idx * 4;` before the four `param`
  stores gives the ROM's `lsls r1, r3, #2` ahead of `idx << 5`
  (103 → 92).
- **Shape/size through inline helpers:** `PieceShape7634(id)` returns
  `(id >> 2) & 3` and `PieceSize7634(id)` returns `id & 3`. Written
  inline in the bitfield assignment, tree folding drops both masks, and
  combine then folds the shape's `& 3` into an `lsrs`. Through RTL
  inlining the masks survive, CSE shares one `movs #3`, and neither
  gets folded: the ROM's `asrs; movs r4, #3; ands` and `ands r3, r4;
  lsls #30` (92 → 2; the size also went to 1044 bytes).
- **Two matrix param variables:** the ROM keeps the `FixedInverse16`
  result in r6 and a copy in r7 for the last store. `pd = pa;` gives
  the copy, but cse2 moves the computation into whichever of the two
  lives longer, which is `pd` (used last). An extra reference
  `asm("" : : "r"(pa))` after the stores (no code) makes `pa` live
  longer and closes the last 2 halfwords.
- **Smaller ones:** `oam_attr2` has `u16` fields, so the tile store
  masks with `lsl/lsr #22` (a `u32` field loads a `0x3ff` constant).
  The loop's flag tests are sign tests on the byte at `part+0x28`
  (`PART_FLAG_SET(part, 26)` → `lsls #26; cmp #0; bge`); a 1-bit field
  test compiles to `movs #0x20; ands`. The prologue reads the same byte
  through `part->gfxMode`/`mosaic`/`colorMode`, and old_agbcc's GCSE
  then gives the ROM's `part+0x28` pointer in r8 and its spilled copy
  at `[sp, #0x44]`. `w`/`h` are rescaled in place (`w = (w << 8) *
  scale >> 16`), so the byte sizes and the scaled sizes share r8/sb.
  Dropping `k` (indexing with `idx * 4` directly) costs 12 halfwords.

## Tools

Scratch helpers in the shared scratchpad `g7634/`: `d.py` (one-function
diff against the ROM), `var.py` (parallel variant runner),
`rtl.sh` (RTL dumps), `t.sh`, and variant specs `s1.py`-`s12.py`.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from
  `affine_sprite_pieces.c`.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
