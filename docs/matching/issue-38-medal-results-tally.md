# Issue #38: 0x0802425C-0x08024810 - medal-results tally and sound-cue helpers

GitHub issue #38 (`decomp-chunk`, category `game_loop`) listed 25 raw
functions in `asm/code_3_2_17_2425c.s`. This is the write-up for the
work done against that list.

## What this cluster turned out to be

Two loosely related families sharing the same ROM neighborhood:

1. **The medal-results tally chain** (`sub_8024278`-`sub_802455C`,
   `sub_8024464`), extending the `gStaticData_0816C86C`/`sub_8025894`
   chain docs/rom_map.md already documents ("A per-level completion-time
   cascade, and a medal-table tally chain"). `gStaticData_0816C86C`'s
   confirmed 36-slot medal table gets two more of its `unused` bytes
   resolved here: `+0x04` (a byte offset into the per-level sound-cue-ID
   table `gStaticData_0816CD80`, `sub_8024498`) and `+0x20` (a pointer
   to a small `struct MedalItemList { count; items[]; extra1; extra2; }`
   header, `sub_8024278`). Each `items[]`/`extra1`/`extra2` entry is
   itself a `struct MedalListItem` with a `type` selector (0-2 dispatch
   to `sub_8025894`, 3 to `sub_802968C`, matching sub_8024278's own
   earlier-documented dispatch) and a nested `linkedObj->0x1c` pointer
   feeding both of those.
2. **A sound-channel-handle helper family** (`sub_8024590`-
   `sub_8024790`, plus the `sub_802425C`/`sub_80247EC` teardown wrapper
   pair and the `sub_8024804` constructor) managing `gUnknown_030012BC`
   playback state for a small per-screen item list, alongside a
   VRAM-bank-toggling asset streamer + palette DMA + a second `DISPCNT`
   writer (`sub_8024708`, alongside the already-documented
   `sub_8001614`/`gUnknown_03001288` one).

## Matched (19 functions, full clean `make compare` passing)

`src/system/game_loop17.c` (`sub_802425C`-`sub_8024278`, 3 fns):
`sub_802425C` (bit-tested `sub_8026ED0` teardown wrapper), `nullsub_25`
(empty stub), `sub_8024278` (the medal-table per-level tally).

`src/system/game_loop18.c` (`sub_80243E0`-`sub_802455C`, 13 fns):
`sub_80243E0`/`sub_8024404` (medal item-list `extra2`/`extra1`-matches-
cached-value checks), `sub_8024428`/`sub_8024434`/`sub_8024440`/
`sub_802444C`/`sub_8024458` (thin wrappers over `sub_8024344` with a
baked-in flag-index constant - `sub_8024344` itself is left raw, see
below), `sub_8024464` (standalone instance of `sub_8024278`'s per-item
dispatch body), `sub_8024498` (medal-results sound-cue resolver),
`sub_80244F0` (item-list cursor advance), `sub_8024524`/`sub_8024540`
(item-list `extra2`/`extra1` field-copy accessors), `sub_802455C`
(item-list nonempty check + cursor-indexed cache).

`src/system/game_loop19.c`: `sub_8024784` (trivial `gUnknown_03001314`
setter).

`src/system/game_loop20.c`: `sub_80247EC` (the `sub_802425C`-shaped
teardown wrapper), `sub_8024804` (trivial constructor).

### Gotchas worth recording

- **Dispatch-on-range compiles as a genuine `switch`, not if/else-if.**
  `sub_8024278`'s per-item `type` dispatch (`type<0`: skip; `type<=2`:
  call `sub_8025894`; `type==3`: call `sub_802968C`) looked like a
  natural if/else-if chain, but that shape compiles with the *wrong*
  branch polarity (`bgt`/skip-forward instead of the ROM's `ble`/jump-
  into-handler). Writing it as an actual C `switch (type) { case 0:
  case 1: case 2: ...; case 3: ...; }` reproduces the ROM's exact
  "test all conditions inline first, jump out to out-of-line handler
  blocks, fall through to the default" layout - this compiler's switch
  lowering for a tiny, mostly-contiguous case set apparently doesn't
  use a jump table, but the linear-compare form it does use has a
  different shape than a hand-written if-chain. Confirmed on both
  `sub_8024278` (3 copies, since the ROM inlines the same dispatch body
  three times rather than calling a shared helper) and the standalone
  `sub_8024464`.
- **Struct-field access order matters when a value first has to be
  loaded from memory.** Every function keying off `gStaticData_0816C86C
  [self->0]` byte-matched only once the array index was written inline
  (`gStaticData_0816C86C[*(s32 *)s].itemList`) instead of through a
  named `s32 idx = *(s32 *)s;` local declared first - the latter makes
  the compiler load `idx` before the table's own base address, the
  former (matching the ROM) loads the table's pool address first, the
  index second. The *reverse* ordering issue showed up in
  `sub_8024278`/`sub_8024344`: their own accumulator (`total`/`result`)
  had to be initialized to `0` *before* the list-header computation, not
  after, to match the ROM's instruction order (both are legal, only one
  matches).
- **`x != 0` compiles differently as a condition vs. as a stored/
  returned value.** `sub_802455C` returns `list->count != 0` - which is
  also the `if` condition governing its cache-write body. The `if`
  compiles as a plain `cmp`/`beq`; the `return`, when written as
  `return list->count != 0;`, initially compiled the *same* way, not
  matching the ROM's `negs`/`orrs`/`lsrs` ("x != 0" bit-trick) idiom
  there. Spelling the return explicitly as `s32 v = list->count; return
  (u32)(-v | v) >> 31;` reproduces the ROM's bit-trick exactly. Also
  needed a *second*, independent reload of `list->count` (not reusing
  the `if` condition's already-loaded value) to match a register the
  ROM frees up in between.
- **A truncated callee-return idiom.** `sub_8024498` calls
  `sub_8024404` and tests its result; the ROM applies `lsls r0,r0,#24`
  before the `cmp #0`, discarding whatever garbage might be in the
  upper 24 bits rather than trusting a clean 0/1 return. Matches this
  codebase's established `(u8)funcCall(...) != 0` idiom (see e.g.
  `src/graphics/actor_part38c.c`) once applied here too.
- **`sub_8024708`'s `& ~0x10`/negated-constant idiom.** The ROM computes
  `gUnknown_03001314`'s low byte as `(byte & -0x11) | ((toggle&1)<<4)`
  - `-0x11` (`0xFFFFFFEF`) is numerically identical to `~0x10`, and is
  how this compiler materializes a plain `& ~0x10` bit-clear via
  `mov`+`rsb` rather than a `mvn`/`bic`. Writing the C as the natural
  `(*flagsByte & ~0x10) | bit4` reproduced it directly - no special
  casing needed once the actual bit being touched (bit 4, not bit 0 or
  a combined 0x11 mask) was correctly identified from the disassembly
  rather than assumed from the `0x11` literal alone.

## Left raw (semantics traced, not byte-matching) - 6 functions

Real bytes for all six stay in their original `asm/code_3_2_17_*.s`
fragments (split out of the original `code_3_2_17_2425c.s` so the
matched functions on both sides of each gap could still be extracted
into their own contiguous `.c` files/`ldscript.txt` entries - see
`tools/report_units.py`'s updated `UNITS` table for the exact address
ranges). None of these got a `NON_MATCHING` C reconstruction committed;
each was attempted and understood well enough to describe precisely,
but not confidently enough to commit an admittedly-imperfect version -
left as a clear target for a future pass instead.

- **`sub_8024344`** (`asm/code_3_2_17_24344.s`) - scans a medal item
  list (same `MedalItemList`/`MedalListItem` shape `sub_8024278` uses)
  for any non-type-3 item whose `linkedObj->0x1c->0x10` table has a
  nonzero `u16` at halfword index `flagIdx` (the parameter
  `sub_8024428`/`34`/`40`/`4C`/`58` bake a constant into). Every field,
  offset and branch confirmed correct (including the early-exit-on-
  match loop shape) via an isolated reconstruction, but the ROM keeps
  `flagIdx` alive across the whole function in `ip`/r12 (via an explicit
  `mov ip, r1` at entry and `mov rN, ip` reloads at each of the three
  use sites) rather than a normally-allocated register - a register-
  pressure-driven allocation choice this reconstruction didn't
  reproduce even with `register ... asm("r1")`/`asm("r3")` pins on the
  intermediate `table`/`v` values (which *did* fix a separate, smaller
  register mismatch in the same function).
- **`sub_8024590`/`sub_8024640`/`sub_80246D8`/`sub_8024708`**
  (`asm/code_3_2_17_24590.s`) - a linked group managing a small
  per-screen item list's sound-channel handles (`sub_8024590`: start/
  re-select a cue via `sub_8001B54`, then either play a secondary sfx
  immediately or busy-poll `sub_8001AB8` until the channel reports the
  requested id before playing it), its driver loop (`sub_8024640`:
  calls `sub_8024708`/`sub_8024590` per index, polls input via
  `sub_80010E0`, ducks music, nudges a delay value, plays a completion
  sfx, then advances via `sub_80246D8`), the "find next `+0x10==1`
  item" index scanner (`sub_80246D8`), and the VRAM-bank-toggling tile-
  asset streamer + palette DMA + second `DISPCNT` writer (`sub_8024708`,
  see the gotcha above - that part *did* get byte-matched in isolation
  before this whole group was reverted back to raw, since `sub_8024708`
  alone wasn't enough to keep the group's `.c` file boundaries simple
  without also matching its three siblings). The remaining gap in all
  four is register-allocation-level (which register holds the item-list
  base pointer vs. the loop index vs. the per-item pointer at any given
  point), not a semantic one - every field offset and call argument is
  confirmed against the ROM.
- **`sub_8024790`** (`asm/code_3_2_17_24790.s`) - the tail half of
  `sub_8024640`'s per-item body (music duck / delay nudge / completion
  sfx) reused standalone against a caller-supplied index, same shape and
  same open gap as the group above.

## Note on `sub_802968C`

Several matched functions here call `sub_802968C` (category-index sfx/
effect resolver, already partially characterized in docs/rom_map.md's
"Resolved category_descriptor.sub_effect_table's record layout"
section) - it's declared `extern` with a `u16` parameter matching every
call site's `ldrh` argument load, but its own body is still raw
elsewhere in the ROM and out of scope for this chunk.

See [docs/status/game_loop.md](../status/game_loop.md) for the running
matched/parked/raw lists this updates.

**Update:** a follow-up pass matched `sub_8024344` and two of the six
functions this doc's "Left raw" section lists as-is
(`sub_8024640`/`sub_80246D8`/`sub_8024790`, all now real C), and produced
NON_MATCHING C reconstructions for the remaining two
(`sub_8024590`/`sub_8024708`) - see
[docs/matching/issue-38-sound-channel-family.md](./issue-38-sound-channel-family.md)
for the full write-up. This doc's own "Left raw" entries above are kept
as historical record of the earlier pass and are no longer accurate.
