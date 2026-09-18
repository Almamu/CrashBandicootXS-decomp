# Issue #38 follow-up: 0x08024344, 0x08024590-0x080247EB - the medal-
results scan and the sound-channel-handle family

This is a follow-up pass over three functions
[docs/matching/issue-38-medal-results-tally.md](./issue-38-medal-results-tally.md)
left raw: `sub_8024344` (the medal item-list per-flag nonzero scan), and
the `sub_8024590`-`sub_8024790` sound-channel-handle helper family.

## Matched (real C, full clean `make compare` passing)

- **`sub_8024344`** (`src/system/game_loop18.c`, prepended ahead of
  `sub_80243E0`) - the medal item-list per-flag nonzero scan the earlier
  pass parked over an `ip`/r12 register-pinning gap for its `flagIdx`
  parameter. That pin (`register s32 fi asm("ip") = flagIdx;`) turned out
  to be only the first of several needed: the loop also needed the guard
  test written as `if (result < list->count)` (not `if (list->count > 0)`)
  to reproduce the ROM's `cmp r3, r0` register-register compare instead of
  an immediate compare, `result`/`i` both declared *before* `list` so
  they're materialized to zero ahead of the address computation (matching
  the ROM's instruction order), and - the trickiest part - the
  `table + shift` address computation in each of the three (loop/extra1/
  extra2) flag-table reads needed `table` typed as a plain `u32` instead
  of a pointer. C's usual pointer-arithmetic canonicalization always
  reorders `int + pointer` to `pointer + int`, silently overriding
  whichever order the source actually used; with `table` as a genuine
  integer, `shift + table` stays in that literal order through to the
  RTL and reproduces the ROM's operand order in the `adds`/`orr`
  instruction exactly. The loaded `u16` flag value itself also needed a
  `register u16 v asm("r3")` pin (matching the `result` variable's own
  register) to avoid an extra register hop between the `ldrh` and the
  `-x|x` nonzero-test bit-trick.
- **`sub_8024640`** (`src/system/game_loop37.c`) - the per-item driver
  loop: streams each item's VRAM bank and sound-channel handle
  (`sub_8024708`/`sub_8024590`), polls input, applies duck-out/fade-start
  side effects, re-arms the cue if needed, then advances via
  `sub_80246D8`. Matched on the first real attempt once `self` was pinned
  to `r4` (`register struct SoundChannelList *self asm("r4")`) - every
  other register (the loop counter, the cached `i*4` byte offset, the
  polled button result) fell into place on its own.
- **`sub_80246D8`** (`src/system/game_loop37.c`) - the "find the next
  `field_10 != 1` item" index scanner sub_8024640 calls to advance.
  Matched with no special techniques at all - a straight transcription of
  the traced control flow (including writing the array access as
  `items[cur + 1]` rather than introducing a separate `next` index
  variable, to keep the "+1" folded into the load's own immediate offset
  the way the ROM does) compiled byte-identical immediately.
- **`sub_8024790`** (`src/system/game_loop38.c`) - the tail half of
  `sub_8024640`'s per-item body (duck-out/fade-start/re-arm), reused
  standalone against a caller-supplied index. Matched with only a `self`
  register pin (`r5`) - same technique as `sub_8024590`/`sub_8024708`
  below, but without either of their residual gaps.

### The `table`/pointer-canonicalization gotcha, worked example

`sub_8024344`'s loop body reads a `u16` flag at `table + shift` (`table`
a pointer loaded from `linkedObj->0x1c->0x10`, `shift` the pre-computed
`flagIdx << 1`). The ROM's instruction order is `adds r1, r7, r1` (shift
first, table second) in the loop, but `adds r1, r1, r0` (table first,
shift second) in the two `extra1`/`extra2` blocks - two different orders
for what's structurally the same expression. Writing `table` as
`u8 *`/`u16 *` and trying every permutation of `table + shift` vs.
`shift + table` in the C source made no difference at all to the
generated order, because GCC's front end normalizes `int + pointer`
expressions to `pointer + int` during parsing, before the order the
programmer wrote is ever visible to code generation - the source-level
swap was a no-op. The fix was declaring `table` as a plain `u32` (not a
pointer) so the addition stays genuine integer arithmetic with no
canonicalization step, then a `register ... asm("r1")`/`asm("r0")` pin on
`table` to pick which of the two ROM-observed register choices applied to
each call site. See `docs/matching_decomp_register_pinning`-style notes
in `docs/matching.md` for other instances of this class of fix - this one
specifically extends that catalog with the "pointer-arithmetic
canonicalization can hide a source-order fix from ever taking effect"
case.

## Parked - NON_MATCHING C reconstruction, real bytes stay in `asm/*.s`

Real bytes for both stay in their own single-function `asm/*.s` files
(split out of the original `asm/code_3_2_17_24590.s` so the matched
functions on both sides could be extracted into `game_loop37.c` - see
`tools/report_units.py`'s updated `UNITS` table for the exact address
ranges). Both were attempted extensively; every field, struct offset,
branch condition and call argument is confirmed correct against the ROM.

- **`sub_8024590`** (`src/system/game_loop37.c`; real bytes in
  `asm/code_3_2_17_24590.s`) - starts/re-selects a sound cue via
  `sub_8001B54`, then either plays a secondary sfx immediately (if the
  channel already reports the requested id) or busy-polls `sub_8001AB8`
  until it does, and either way ORs bit 7 into `field_08`'s low byte for
  a `sub_800132C` fade-start call. The residual gap: the ROM materializes
  the `-0x80` OR-mask into one register then copies it to a second
  register before the OR (`movs r2,#0x80; rsbs r2,r2,#0; adds r1,r2,#0;
  orrs r0,r1` - four instructions), while every C phrasing tried here
  (a plain expression, a named local, register-pinned locals for the
  mask/copy/result with an explicit copy step, `~0x7F` in place of
  `-0x80` per the `matching_decomp_alignment_fix`-adjacent negated-
  constant idiom) collapses the redundant copy away via constant
  propagation - `mask`'s value is trivially known at compile time, so
  GCC materializes it straight into the destination register instead of
  going through the extra hop, regardless of how insistently the source
  asks for a separate step. `volatile` on the register-pinned local
  doesn't help either (GCC explicitly refuses `volatile` on register
  variables). Two instructions short (4 bytes) in each of two call sites
  (the `playing == field_14` and `playing != field_14` branches both
  need it).
- **`sub_8024708`** (`src/system/game_loop37.c`; real bytes in
  `asm/code_3_2_17_24708.s`) - toggles `self`'s VRAM-bank flip-flop and
  streams `self->items[idx]`'s tile asset to whichever bank the new state
  selects, rebuilds `gUnknown_03001314`'s bit 4 (the same `& ~0x10 | bit`
  shadow-byte idiom `sub_8024708`'s cousin in game_loop18.c uses for a
  *different* global - see that file's own write-up), DMA3-copies the
  asset's first half into `BG_PLTT`, and commits `gUnknown_03001314`'s
  low halfword to `REG_DISPCNT`. Confirmed byte-identical everywhere
  except one branch's address computation: `asset + 0x200` inside the
  `toggle != 0` case computes its `0x80 << 2` scratch offset into `r2` in
  the ROM (`movs r2,#0x80; lsls r2,r2,#2; adds r0,r6,r2`), but every C
  phrasing tried (plain expression, a named local, `u32*`-typed pointer
  arithmetic to make the `<<2` implicit, register pins on the offset or
  on the destination constant, evaluation-order swaps) puts it in `r1`
  instead - the *other* (`toggle == 0`) branch's identically-shaped
  computation already matches exactly using `r1`, which is what makes
  this look like a genuine per-branch scheduling artifact from the ROM's
  original build (likely tied to whichever pool-load-vs-immediate shape
  the *second* argument took at that specific point) rather than
  anything under this reconstruction's control. Three instructions'
  worth of register-field bits differ (same instruction count and total
  size, just `r1` where the ROM has `r2`) in that one branch.

Both keep the `register struct SoundChannelList *self asm("r5")` pin -
without it, GCC allocates `self` to a plain low register and only needs
two working registers total instead of the ROM's three, which cascades
into further mismatches throughout the rest of each function; the pin
alone was enough to fix the bulk of both functions before the two
residual gaps above.

See [docs/status/game_loop.md](../status/game_loop.md) for the running
matched/parked/raw lists this updates.
