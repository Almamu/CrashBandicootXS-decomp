# Issue #38: 0x0802425C-0x08024810 - medal-results tally and sound-cue helpers

GitHub issue #38 (`decomp-chunk`, category `game_loop`) listed 25 raw
functions in `asm/code_3_2_17_2425c.s`. This is the write-up for the
work done against that list.

## What this cluster turned out to be

Two loosely related families sharing the same ROM neighborhood:

1. **The medal-results tally chain** (`CountLevelCrates`-`SelectRoom`,
   `CountRoomCrates`), extending the `gLevelTable`/`CountCrateEntities`
   chain docs/rom_map.md already documents ("A per-level completion-time
   cascade, and a medal-table tally chain"). `gLevelTable`'s
   confirmed 36-slot medal table gets two more of its `unused` bytes
   resolved here: `+0x04` (a byte offset into the per-level sound-cue-ID
   table `gThemeMusicCues`, `PlayRoomMusic`) and `+0x20` (a pointer
   to a small `struct MedalItemList { count; items[]; extra1; extra2; }`
   header, `CountLevelCrates`). Each `items[]`/`extra1`/`extra2` entry is
   itself a `struct MedalListItem` with a `type` selector (0-2 dispatch
   to `CountCrateEntities`, 3 to `CountCategoryCrates`, matching CountLevelCrates's own
   earlier-documented dispatch) and a nested `linkedObj->0x1c` pointer
   feeding both of those.
2. **A sound-channel-handle helper family** (`BeginSlide`-
   `EndSlide`, plus the `sub_802425C`/`DestroySlideshow` teardown wrapper
   pair and the `ResetSlideshow` constructor) managing `gAudioContext`
   playback state for a small per-screen item list, alongside a
   VRAM-bank-toggling asset streamer + palette DMA + a second `DISPCNT`
   writer (`ShowSlidePicture`, alongside the already-documented
   `CommitDispcnt`/`gDispcnt` one).

## Matched (19 functions, full clean `make compare` passing)

`src/system/game_loop17.c` (`sub_802425C`-`CountLevelCrates`, 3 fns):
`sub_802425C` (bit-tested `OperatorDelete` teardown wrapper), `nullsub_25`
(empty stub), `CountLevelCrates` (the medal-table per-level tally).

`src/system/game_loop18.c` (`IsInGemPathRoom`-`SelectRoom`, 13 fns):
`IsInGemPathRoom`/`IsInBonusRoom` (medal item-list `extra2`/`extra1`-matches-
cached-value checks), `LevelHasYellowGemEntity`/`LevelHasBlueGemEntity`/`LevelHasGreenGemEntity`/
`LevelHasRedGemEntity`/`LevelHasGemPathGemEntity` (thin wrappers over `LevelHasEntityType` with a
baked-in flag-index constant - `LevelHasEntityType` itself is left raw, see
below), `CountRoomCrates` (standalone instance of `CountLevelCrates`'s per-item
dispatch body), `PlayRoomMusic` (medal-results sound-cue resolver),
`NextRoom` (item-list cursor advance), `EnterGemPathRoom`/`EnterBonusRoom`
(item-list `extra2`/`extra1` field-copy accessors), `SelectRoom`
(item-list nonempty check + cursor-indexed cache).

`src/system/game_loop19.c`: `SetSlideshowDispcnt` (trivial `gSlideshowDispcnt`
setter).

`src/system/game_loop20.c`: `DestroySlideshow` (the `sub_802425C`-shaped
teardown wrapper), `ResetSlideshow` (trivial constructor).

### Gotchas worth recording

- **Dispatch-on-range compiles as a genuine `switch`, not if/else-if.**
  `CountLevelCrates`'s per-item `type` dispatch (`type<0`: skip; `type<=2`:
  call `CountCrateEntities`; `type==3`: call `CountCategoryCrates`) looked like a
  natural if/else-if chain, but that shape compiles with the *wrong*
  branch polarity (`bgt`/skip-forward instead of the ROM's `ble`/jump-
  into-handler). Writing it as an actual C `switch (type) { case 0:
  case 1: case 2: ...; case 3: ...; }` reproduces the ROM's exact
  "test all conditions inline first, jump out to out-of-line handler
  blocks, fall through to the default" layout - this compiler's switch
  lowering for a tiny, mostly-contiguous case set apparently doesn't
  use a jump table, but the linear-compare form it does use has a
  different shape than a hand-written if-chain. Confirmed on both
  `CountLevelCrates` (3 copies, since the ROM inlines the same dispatch body
  three times rather than calling a shared helper) and the standalone
  `CountRoomCrates`.
- **Struct-field access order matters when a value first has to be
  loaded from memory.** Every function keying off `gLevelTable
  [self->0]` byte-matched only once the array index was written inline
  (`gLevelTable[*(s32 *)s].itemList`) instead of through a
  named `s32 idx = *(s32 *)s;` local declared first - the latter makes
  the compiler load `idx` before the table's own base address, the
  former (matching the ROM) loads the table's pool address first, the
  index second. The *reverse* ordering issue showed up in
  `CountLevelCrates`/`LevelHasEntityType`: their own accumulator (`total`/`result`)
  had to be initialized to `0` *before* the list-header computation, not
  after, to match the ROM's instruction order (both are legal, only one
  matches).
- **`x != 0` compiles differently as a condition vs. as a stored/
  returned value.** `SelectRoom` returns `list->count != 0` - which is
  also the `if` condition governing its cache-write body. The `if`
  compiles as a plain `cmp`/`beq`; the `return`, when written as
  `return list->count != 0;`, initially compiled the *same* way, not
  matching the ROM's `negs`/`orrs`/`lsrs` ("x != 0" bit-trick) idiom
  there. Spelling the return explicitly as `s32 v = list->count; return
  (u32)(-v | v) >> 31;` reproduces the ROM's bit-trick exactly. Also
  needed a *second*, independent reload of `list->count` (not reusing
  the `if` condition's already-loaded value) to match a register the
  ROM frees up in between.
- **A truncated callee-return idiom.** `PlayRoomMusic` calls
  `IsInBonusRoom` and tests its result; the ROM applies `lsls r0,r0,#24`
  before the `cmp #0`, discarding whatever garbage might be in the
  upper 24 bits rather than trusting a clean 0/1 return. Matches this
  codebase's established `(u8)funcCall(...) != 0` idiom (see e.g.
  `src/graphics/actor_part38c.c`) once applied here too.
- **`ShowSlidePicture`'s `& ~0x10`/negated-constant idiom.** The ROM computes
  `gSlideshowDispcnt`'s low byte as `(byte & -0x11) | ((toggle&1)<<4)`
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

- **`LevelHasEntityType`** (`asm/code_3_2_17_24344.s`) - scans a medal item
  list (same `MedalItemList`/`MedalListItem` shape `CountLevelCrates` uses)
  for any non-type-3 item whose `linkedObj->0x1c->0x10` table has a
  nonzero `u16` at halfword index `flagIdx` (the parameter
  `LevelHasYellowGemEntity`/`34`/`40`/`4C`/`58` bake a constant into). Every field,
  offset and branch confirmed correct (including the early-exit-on-
  match loop shape) via an isolated reconstruction, but the ROM keeps
  `flagIdx` alive across the whole function in `ip`/r12 (via an explicit
  `mov ip, r1` at entry and `mov rN, ip` reloads at each of the three
  use sites) rather than a normally-allocated register - a register-
  pressure-driven allocation choice this reconstruction didn't
  reproduce even with `register ... asm("r1")`/`asm("r3")` pins on the
  intermediate `table`/`v` values (which *did* fix a separate, smaller
  register mismatch in the same function).
- **`BeginSlide`/`RunSlideshow`/`SkipSlides`/`ShowSlidePicture`**
  (`asm/code_3_2_17_24590.s`) - a linked group managing a small
  per-screen item list's sound-channel handles (`BeginSlide`: start/
  re-select a cue via `PlaySong`, then either play a secondary sfx
  immediately or busy-poll `GetCurrentSong` until the channel reports the
  requested id before playing it), its driver loop (`RunSlideshow`:
  calls `ShowSlidePicture`/`BeginSlide` per index, polls input via
  `WaitForKeyPress`, ducks music, nudges a delay value, plays a completion
  sfx, then advances via `SkipSlides`), the "find next `+0x10==1`
  item" index scanner (`SkipSlides`), and the VRAM-bank-toggling tile-
  asset streamer + palette DMA + second `DISPCNT` writer (`ShowSlidePicture`,
  see the gotcha above - that part *did* get byte-matched in isolation
  before this whole group was reverted back to raw, since `ShowSlidePicture`
  alone wasn't enough to keep the group's `.c` file boundaries simple
  without also matching its three siblings). The remaining gap in all
  four is register-allocation-level (which register holds the item-list
  base pointer vs. the loop index vs. the per-item pointer at any given
  point), not a semantic one - every field offset and call argument is
  confirmed against the ROM.
- **`EndSlide`** (`asm/code_3_2_17_24790.s`) - the tail half of
  `RunSlideshow`'s per-item body (music duck / delay nudge / completion
  sfx) reused standalone against a caller-supplied index, same shape and
  same open gap as the group above.

## Note on `CountCategoryCrates`

Several matched functions here call `CountCategoryCrates` (category-index sfx/
effect resolver, already partially characterized in docs/rom_map.md's
"Resolved category_descriptor.sub_effect_table's record layout"
section) - it's declared `extern` with a `u16` parameter matching every
call site's `ldrh` argument load, but its own body is still raw
elsewhere in the ROM and out of scope for this chunk.

See [docs/status/game_loop.md](../status/game_loop.md) for the running
matched/parked/raw lists this updates.

**Update:** a follow-up pass matched `LevelHasEntityType` and two of the six
functions this doc's "Left raw" section lists as-is
(`RunSlideshow`/`SkipSlides`/`EndSlide`, all now real C), and produced
NON_MATCHING C reconstructions for the remaining two
(`BeginSlide`/`ShowSlidePicture`) - see
[docs/matching/issue-38-sound-channel-family.md](./issue-38-sound-channel-family.md)
for the full write-up. This doc's own "Left raw" entries above are kept
as historical record of the earlier pass and are no longer accurate.
