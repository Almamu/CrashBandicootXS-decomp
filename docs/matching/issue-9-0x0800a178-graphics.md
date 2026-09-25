# Issue #9/#10: `sub_800A178`/`sub_800A420` (graphics)

Dedicated deep-investigation session against the last two functions in
the `tools/report_units.py`-tracked `sub_800A0FC`-through-`sub_800A420`
still-raw span (`base_object=None`): `sub_800A178` (ROM `0x0800A178`,
680 bytes) and `sub_800A420` (ROM `0x0800A420`, 264 bytes).
`sub_800A0FC` itself, the only caller of `sub_800A178` (both live in
`asm/code_3_2_11.s`), stays raw - its own gate logic depends on
`sub_8009BE0` (parked NAKED, `actor_part12b.c`,
[naked-spatial-grid-tail.md](./naked-spatial-grid-tail.md)), so closing
`sub_800A178`/`sub_800A420` alone doesn't unblock it.

## Starting point

`docs/rom_map.md` (line ~2624) had already partially flagged
`sub_800A178`: "mid-function, unconditionally zeroes `self+0x74` - the
same field `UpdateGameFrame`'s level-load branch sets once from
`sub_8035E14`'s return value ... consistent with 'total for this level'
being cleared and presumably recomputed under some condition, not fully
traced here." `docs/matching/issue-9-0x08007634-actor.md` (line ~206)
had also already flagged both functions as built on `sub_8008200`/
`sub_8026628`/`sub_8026C3C`/`sub_8026BF8` - two of those four
(`sub_8008200`, `sub_8026628`) are already matched this session
(`src/graphics/actor_part4.c`, `src/system/game_loop43.c`), leaving
only `sub_8026C3C`/`sub_8026BF8` genuinely unexamined.

## Reading the real bytes

Both functions sit in `asm/code_3_2_11.s`, immediately after the
still-raw `sub_800A0FC` and immediately before the already-matched
`sub_800A528`/`sub_800A590` (`src/graphics/actor_part47.c`, which
itself already notes "still-raw `sub_800A0FC`/`sub_800A178`/
`sub_800A420`" as its own neighbors). `sub_8026C3C`/`sub_8026BF8`
themselves live in `asm/code_3_2_17_266bc.s` (the file `sub_8026628`
was split out of, right after it).

### `sub_8026C3C`/`sub_8026BF8`: single-point terrain-height probes

`docs/rom_map.md`'s "15 more reads" pass (line ~2581) had already
placed these as "single-point collision-test siblings of
`sub_8026A18`/`sub_8026AE8` [the axis resolvers `sub_8026628` already
dispatches to], one via the raw terrain streamer and one via the
`CheckTerrainFlag` API." Reading their bytes directly confirms and
sharpens that: both are `s32 fn(void *player, struct probe_pos *pos,
s32 *outValue)`, computing `pos->x >> 3`/`pos->y >> 3` tile coords from
`player->0x20`'s terrain-data pointer:

- **`sub_8026BF8`** looks the tile row up via `sub_80250BC` ("the raw
  terrain streamer" - returns a row pointer, or `NULL` on a miss).
  On a hit, reads a **signed byte** height sample at
  `row[pos->x & 7]`, computes `((pos->y >> 3) << 3) + heightByte -
  pos->y`, shifts to Q8, and accumulates it into `*outValue`. Returns
  `1` on a row hit, `0` if `sub_80250BC` returned `NULL`.
- **`sub_8026C3C`** is the exact same shape, but the height byte comes
  from `sub_8025228(terrainPtr, tileX, tileY, 0, &scratch)` instead of
  a direct row-pointer byte read - the "CheckTerrainFlag"-style API
  `sub_8026A18`/`sub_8026AE8` already use via their own `sub_8025130`
  calls (same argument shape: base pointer, tile coords, a submode, an
  out-parameter - see [issue-9-10-41-0x08026628-game-loop.md](./issue-9-10-41-0x08026628-game-loop.md)).
  Returns `0` if the returned signed byte is negative, `1` otherwise,
  with the same `(tileY<<3)+byte-pos->y` delta accumulation.

Both are Y-axis (floor-height) probes - matching how `sub_800A178`
only ever uses them against `self.y`/`self->y`, never `self.x`. Neither
was attempted as a byte-exact C match this session (see "Matching"
below); this account is only deep enough to place both callers'
argument roles precisely.

### `sub_800A420(void *self, struct hitbox_quad *quad, u8 *outFlag)`

A single Y-axis "floor" probe. Builds an int `{x, y}` position at the
*bottom* of `quad` (`self.x`/`self.y + (quad->yOff + quad->h) << 8`,
via the already-matched `sub_8008200(dest, 8, quad)`, nudged left/right
by half the quad's width depending on `self+0x28` bit 4's mirror flag -
the same established convention `game_loop43.c`/`actor_part109.c`
document), then probes it via `sub_8026BF8(*gUnknown_03001308, &pos,
&origY)` where `origY` is `self.y`'s own original (unmodified) Q8
value, kept aside as the probe's out-parameter target.

- On a hit: snaps `self->y` to the probed value (`origY & ~0xFF`),
  additionally moving it one pixel higher (`-= 0x100`) if `self+0xd`
  bit 1 was clear on entry, then sets `self+0xd` bit 1.
- On a miss: if `self+0xd` bit 1 was already set, skips straight to the
  tail (no retry). Otherwise retries one integer pixel lower
  (`pos.y += 1`) via a second `sub_8026BF8` call:
  - if *that* hits, snaps `self->y` to its own `origY` and sets
    `self+0xd` bit 1 (same as the first-hit path);
  - if it also misses, `*outFlag` is set to `1` only on the path where
    bit 1 was set from the very start (i.e. never on this specific
    retry-then-miss path) - `self+0xd` bit 1 always ends up cleared on
    any all-miss outcome, regardless of which sub-path was taken.

Returns whichever `sub_8026BF8` call's hit boolean was computed last.
(The `outFlag`/bit-1 gating looks like a redundant double-test of the
same bit at the disassembly level - `sub_800A4C4`'s own bit-1 check,
then a second bit-1 check at `sub_800A500` that's always false whenever
control actually reaches it - but it reconstructs cleanly as two
sequential `if (!(flags & 2))` checks in the natural C source shape,
the second one simply re-testing a flag that didn't change in between,
not a compiler artifact.)

### `sub_800A178(void *self)`

The larger orchestrator, gated on two checks - a
`sub_803AD7C(self->table[0x38]/[0x3c])` trampoline truthiness test,
then `self+0xc` bit 7 - either failing returns `0` immediately with no
other effect. Once past both:

1. **Zeroes `self+0x74` unconditionally** - confirming
   `docs/rom_map.md`'s partial note in full: this is a genuine,
   unconditional write (once the two gates pass), and reading the rest
   of the function completes the "recomputed under some condition"
   half that note left open - see "Confirming `self+0x74`" below.
2. Computes a starting result mask: `self+0xd` bit 0 expanded to `8`,
   but only when `self+0x24 & 0xc` is clear (otherwise the mask starts
   at `0`).
3. If `self+0xd` bit 0 is set, calls `sub_800A420` once (with a
   stack-local `u8` initialized `0` as `outFlag`) and keeps its return
   value as a "found ground already" flag (`sl`). If that flag is set
   and the result mask is still `0`, forces the mask to `8`.
4. If the `sub_800A420` call's `outFlag` came back `1`: builds an int
   position at the bottom of the quad (same mirrored-half-width
   adjustment as `sub_800A420`'s own), probes it via
   `sub_8026C3C(player, pos, &origY)`. On a hit: snaps `self->y` to the
   probed value and nudges `self->x` by ±1 pixel depending on
   `self+0x24 & 3` (`2` -> left, any other nonzero -> right, `0` -> no
   nudge). On a miss: nudges `self->y` down one pixel instead. Either
   way, calls `sub_800A420` again afterward to refresh the "found
   ground" flag (`sl`).
5. Three more blocks, each gated on `self+0x24`'s own 2-bit sub-fields
   (`& 3` for the X-axis modes `1`/`2`, `& 0xc` for the Y-axis modes
   `4`/`8`) and on `sl` still being `0`: build a plain int `{x, y}`
   position via the already-matched `sub_8008278(dest, mode, quad)`,
   probe it through the already-matched `sub_8026628(player, mode,
   pos, span, outValue)` tile-scan API, with `span` taken from the
   quad's own `h`/`w` byte - **three deliberately different probe
   geometries**, not a shared constant: `quad->h - 16` for the first
   X-axis block, `quad->w` for the Y-axis block, `quad->h` (full) for
   the second X-axis block. On a hit, OR's `mode` into both
   `self+0x74` and the running result mask, and restores whichever
   coordinate `sub_8026628` didn't touch (`sub_8026628`'s own
   `outValue` only ever carries the probed axis; the other axis is
   restored from a stack-cached original).
6. Returns the final result mask.

### Confirming `self+0x74`

`self+0x74` starts this call zeroed (step 1), then only ever gets
OR'd with a `mode` value (`1`/`2`/`4`/`8`, `self+0x24`'s own per-axis
bits) at each of the three `sub_8026628` probe blocks in step 5, each
time gated on that probe actually reporting a hit. So `self+0x74` is a
**per-call bitmask of which movement axes/directions actually resolved
a collision this call** - zeroed at the top of every `sub_800A178`
invocation and rebuilt bit by bit as each axis probe fires, exactly
matching `docs/rom_map.md`'s "'total for this level' being cleared and
presumably recomputed" framing in spirit (the "presumably recomputed"
half is now fully traced: it's a per-axis hit mask, not a level-wide
total in the way the first flagged write site - `UpdateGameFrame`'s
level-load branch - might have suggested; the two write sites share a
field offset but serve different purposes, one per-level-load, one
per-physics-call).

## Matching

Neither function was attempted as a byte-exact C reconstruction.
Both keep `sb`/`sl`/`r8` (and, for `sub_800A178`, `r7` too) live
simultaneously across many `bl` calls, reused for genuinely different
values block to block:

- `sub_800A420`'s `r8` holds `&gUnknown_03001308` across two separate
  `sub_8026BF8` calls (a conservative re-derive-via-cached-address
  idiom, not a straight cached value) rather than re-fetching the
  literal pool address each time; `sb` holds `outFlag` for the whole
  function despite only being dereferenced once, right at the end.
- `sub_800A178`'s `sb` accumulates a result bitmask across five
  different probe blocks while `sl` independently tracks the "found
  ground" boolean and `r8` holds the quad pointer for the entire
  function; `r7` gets reused three separate times for three different
  `self+0x24`-derived mode values in unrelated blocks.

This is the exact `r7`/`r8`/`sb` cross-block register-reuse shape this
immediate ROM neighborhood has already independently established as
resistant to gcc 2.9 C reconstruction, four times over: `sub_8009BE0`
([naked-spatial-grid-tail.md](./naked-spatial-grid-tail.md)),
`sub_800CD00`, `sub_800CEAC`, `sub_800CF70`
([issue-9-10-0x0800aaec-graphics.md](./issue-9-10-0x0800aaec-graphics.md),
[issue-9-10-0x0800ceac-graphics.md](./issue-9-10-0x0800ceac-graphics.md)).
Rather than re-litigating that from scratch across two more, larger
functions, this session ran a single honest isolated-compile attempt
against `sub_800A420` (the smaller of the two, with the cleanest
worked-out C-level logic of the pair - see the `outFlag`/bit-1 aside
above) as a direct confirmation: the straightforward C reconstruction
compiled cleanly, but this compiler's natural register allocation used
**no high registers at all** (`r4`-`r7` sufficed for every value,
including the `gUnknown_03001308` address and `outFlag`) - a
structurally different register-allocation solution from the ROM's own
deliberate `sb`/`r8` choice, not a near-miss fixable with one or two
register pins. Recognizing the established pattern, both functions
were transcribed directly as byte-exact NAKED asm instead: the ROM
disassembly translated instruction-for-instruction, unified-syntax
mnemonics converted to this project's plain/divided-syntax NAKED
convention (`adds`->`add`, `movs`->`mov`, `ands`->`and`,
`lsls`/`lsrs`->`lsl`/`lsr`, `asrs`->`asr`, `rsbs`->`neg`), with the
original `_08XXXXXX:` labels renumbered to GNU-as local numeric labels.

Verified byte-exact via the isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pipeline against `baserom.gba`'s own bytes at
`0x0800A178`-`0x0800A528` (944 bytes, both functions together): the
only differing bytes fell into exactly the expected relocation-site
set - 16 `bl` calls (`sub_803AD7C` x2, `sub_800A420` x3, `sub_8008200`
x2, `sub_8026C3C` x1, `sub_8026628` x3, `sub_8008278` x3, `sub_8026BF8`
x2) plus 3 `.4byte gUnknown_03001308` literal-pool words, all of which
resolve correctly once linked.

## Build layout

New object `src/graphics/actor_part110.c` holds both functions,
inserted in `ldscript.txt` exactly where `sub_800A178`/`sub_800A420`'s
real bytes used to sit: between the trimmed `asm/code_3_2_11.o` (now
ending right after `sub_800A0FC`'s own trailing `bx r1`) and the
already-matched `src/graphics/actor_part47.o` (`sub_800A528` onward).
`asm/code_3_2_11.s` is trimmed accordingly - `sub_800A0FC` unchanged,
everything after it removed.

## Full-ROM verification

`rm -rf build && make NON_MATCHING=1 report` - clean, no warnings from
the new file. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide` (checksum matches).

## Techniques used

- NAKED transcription as the deliberate, documented default given this
  ROM neighborhood's own established `r7`/`r8`/`sb` cross-block
  register-reuse resistance (four prior confirmed instances) -
  confirmed directly rather than assumed, via one isolated-compile
  attempt against the smaller of the two functions before committing
  to the NAKED path for both.
- Reading `sub_8026C3C`/`sub_8026BF8`'s own raw bytes (not attempting
  to match them) just far enough to pin down both callers' argument
  roles precisely, the same "read the callee enough to place the
  caller" approach `issue-9-10-41-0x08026628-game-loop.md` used for
  `sub_8026AE8`/`sub_8026A18`.
- Cross-referencing `docs/rom_map.md`'s own partial `self+0x74` note
  against the fully-read function body to confirm and complete it,
  rather than leaving the "presumably recomputed" half unresolved.

## Cross-references

- `docs/status/actor.md` - `sub_800A178`/`sub_800A420` moved from
  "Left raw" into "Parked - NAKED transcription"; the `sub_800A0FC`
  "Left raw" entry narrowed to cover only itself.
- `tools/report_units.py` - new unit at `0x0800A178` (`base_object:
  None`, category `graphics`); the `0x0800A0FC` unit's comment
  narrowed to describe only `sub_800A0FC` itself.
- `docs/matching/issue-9-0x08007634-actor.md` - the original "Left
  untouched (raw)" entries for both functions (left as-is, per this
  project's append-only convention for per-investigation docs already
  published - this doc supersedes them going forward).
