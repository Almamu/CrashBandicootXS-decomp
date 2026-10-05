# `UnlinkCrateFromGrid`/`UpdateCrateList`/`CollidePlayerWithCrates`/`sub_8009BE0`: NAKED asm transcription (byte-exact, tracked as parked)

**Tracking note**: same convention as `naked-oam-actor-part-batch.md` -
these four are byte-exact (confirmed by a full clean `make compare`)
but, being `NAKED` transcriptions of the ROM's own disassembly rather
than real decompiled C, they count as **parked**, not matched, in this
project's tracking. `tools/report_units.py`'s `UNITS` list keeps each
one's own address range at `base_object: None`, and
`docs/status/actor.md` files them under "Parked - NAKED transcription".

These four were the last raw stretches inside the spatial-hash-grid/
object-pool AI-collision cluster that `docs/rom_map.md`'s "Two big
unnamed systems" section flagged, sitting interleaved with the
already-parked `InitCrateList`/`LinkCrateToActiveBucket`/`DrawCrateList`/`CollideCrateGrid`/
`CollideCrateGridPartWithPlayer`/`ResetCrateList`/`CollideCrateGridPartWithObject` (all `NON_MATCHING` C
reconstructions in `actor_part11.c`/`actor_part12.c`) and the already-
matched functions around them. GitHub issue #9 tracked
`UnlinkCrateFromGrid`/`UpdateCrateList`/`CollidePlayerWithCrates` (part of the `0x08009008`-
`0x08009914` range) and `sub_8009BE0` separately; none of it closes
that issue, since none of these four is a real decompiled-C match -
see `docs/matching/issue-9-0x08007634-actor.md` for the rest of that
issue's still-open scope.

## What each function does

- **`UnlinkCrateFromGrid`** - the grid-removal primitive `RemoveCrateFromList`/
  `RemoveCrateListAt` (`actor_part12.c`) call before compacting the active-
  object array. Two-phase search: phase 1 walks the object's own
  primary bucket (`gridHead[*(s16 *)(item+2)]`, the same bucket index
  `LinkCrateInGrid` computes on insert) for a node whose data pointer
  equals `item`, unlinking it and pushing its wrapper entry back onto
  the free list on the first match. Phase 2 (always run unless phase 1
  found a match and neither `*(u16 *)(item+8) == 0xFFFF` nor the
  object's "large object" flag bit holds) walks every bucket from 255
  down to 0, removing any further match the same way, but stops
  scanning deeper into a bucket's list once the combined removal count
  exceeds 1 (an object can only ever be registered in at most two
  buckets - its primary one and, for large objects, bucket 255).
- **`UpdateCrateList`** - a per-frame grid-maintenance pass, scoped to the
  3-bucket window `[baseIdx, baseIdx+2]` around `baseIdx`
  (`max(gLevelLayers`'s sub-object's own `x >> 8`, `0)`, the same
  index `DrawCrateList`/`CollideCrateGrid` compute), plus bucket 255 in a
  second pass - not a full 0-255 sweep like those two siblings. For
  each windowed object: if it's a "large object" without a bucket-255
  link yet, lazily creates one (an inline copy of `LinkCrateToActiveBucket`'s own
  body); else if its "pending removal" flag is set, removes it from
  the pool (search, `UnlinkCrateFromGrid` to unlink the grid node(s),
  `CpuSet`-compact the array) and fires the same `table+0x50/0x54`
  "destroy" trampoline `ResetCrateList`'s teardown loop fires; else tests
  it against a computed box (the tracked sub-object's position offset
  by fixed Q8 constants, wider than the plain 240x160 screen box the
  other grid-filter functions use) and, on a hit, fires a
  `table+0x18/0x1c` trampoline and marks the node so the bucket-255
  pass doesn't double-process it.
- **`CollidePlayerWithCrates`** - another 3-bucket-window pass, reading the player
  (`gPlayer`) instead of writing to the grid: if the
  player's `+0x88` byte is `3`, calls `BreakCrateTouchedByPlayer(part)` for every
  windowed object; otherwise computes a dispatch value from the
  player's state and calls `CollideCrateWithPlayer(part, dispatchValue,
  player->x, player->y)` for each.
- **`sub_8009BE0`** - a physics/collision step-probe: copies `self`'s
  position, runs it through `sub_8008278` (still unexamined), converts
  to plain integers, then probes it via `sub_8026628` (also still
  unexamined). If the first probe succeeds, restores `self->y` to its
  original value and returns `1`; otherwise clears
  `gLevelLayers`'s `+0x2a` flag and retries up to 3 more times,
  nudging the working Y down by 8 (Q8) each attempt, always returning
  `0` once it falls into the retry path (restoring the `+0x2a` flag
  either way).

## Why plain C didn't converge

All four were fully semantically understood (every load, store, field
offset, and branch traced against the ROM disassembly - see each
function's own doc comment in its `.c` file for the details) before
falling back to a transcription, per `docs/workflow.md`'s step 3. Each
hit a distinct, genuine gcc-2.9 codegen gap in the same family already
catalogued for this exact cluster's siblings:

- `UnlinkCrateFromGrid` - after unlinking a second match in phase 2, the ROM
  discards the bucket-index scratch register entirely (reloading it
  with an unrelated literal, `0x100`, right before the `bucket--` that
  terminates the *outer* loop) instead of preserving it across the
  `removedCount > 1` test the way any C-level loop naturally would -
  the same "gcc keeps a value the ROM discards, or vice versa" pattern
  as `InitCrateList`/`LinkCrateToActiveBucket`/`DrawCrateList`.
- `UpdateCrateList` - juggles more live cross-branch state across three
  high registers (`r8`/`sb`/`sl`, each reused for a different purpose
  in each of the three inner-loop branches) than any attempted C
  reconstruction reproduced exactly - the same many-register
  allocation gap as `InitCrateList`/`ResetCrateList`.
- `CollidePlayerWithCrates` - reuses `r8` for two genuinely different base
  addresses (`gridHead[0]` in one branch, a different player-derived
  address in the other) across the same loop shape; no C-level
  reconstruction attempted kept both uses pinned to the ROM's register.
- `sub_8009BE0` - keeps `self+0x69`'s address in `r6` for the whole
  retry loop while also reusing that same value as the loop's own
  termination-test operand; gcc consistently reloaded the address a
  second time instead of reusing the pinned one.

Rather than keep chasing single-instruction register-choice quirks
across four already-fully-understood functions, all four were
converted to `NAKED` functions whose body is a single `asm()` block
transcribing the real ROM disassembly instruction-for-instruction,
following the exact same conventions as `naked-oam-actor-part-batch.md`
(GNU-as local numeric labels in ROM order, `adds`→`add`/`ands`→`and`/
`orrs`→`orr`/`lsls`→`lsl`/`lsrs`→`lsr`/`asrs`→`asr`/`subs`→`sub`/
`movs`→`mov` mnemonic conversion to match this project's other `asm()`
blocks' divided syntax, and every `.align 2, 0` reproduced at the ROM's
own positions, including trailing padding).

## File/link-order changes

`UnlinkCrateFromGrid` sat inside `asm/code_3_2_13.s`, interleaved between the
already-`.if NON_MATCHING == 0`-guarded raw bodies of
`InitCrateList`/`LinkCrateToActiveBucket`/`DrawCrateList`/`CollideCrateGrid`/`CollideCrateGridPartWithPlayer`/
`ResetCrateList`/`CollideCrateGridPartWithObject` (each of those seven stays exactly as
parked as before - untouched by this batch). Since a `NAKED` function
needs to be unconditionally compiled (not itself guarded), extracting
these three raw bodies required splitting that one file into four:

- `asm/code_3_2_13.s` (trimmed to just `InitCrateList`'s guard)
- new `src/graphics/actor_part11b.c` (`UnlinkCrateFromGrid`)
- `asm/code_3_2_13_9150.s` (`LinkCrateToActiveBucket`'s guard)
- new `src/graphics/actor_part11c.c` (`UpdateCrateList`)
- `asm/code_3_2_13_944c.s` (`DrawCrateList`/`CollideCrateGrid`/`CollideCrateGridPartWithPlayer`'s
  guards)
- new `src/graphics/actor_part11d.c` (`CollidePlayerWithCrates`)
- `asm/code_3_2_13_9914.s` (`ResetCrateList`/`CollideCrateGridPartWithObject`'s guards)

`ldscript.txt` places each new object exactly where its raw block used
to sit in link order. `asm/code_3_2_14.s` held only `sub_8009BE0` and
nothing else, so it's retired entirely (same "retire an emptied split"
convention as earlier batches) in favor of a new
`src/graphics/actor_part12b.c`, inserted between `actor_part12.o` and
`actor_part13.o` in `ldscript.txt`.

Full clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` confirms `La suma coincide`
after this batch, alongside `make NON_MATCHING=1 report`.

## Later pass (issue #9 NAKED retry)

`CollidePlayerWithCrates` is now real C under old_agbcc. `sub_8009BE0`,
`UnlinkCrateFromGrid` and `UpdateCrateList` are still NAKED. See [issue-9-naked-retry.md](./issue-9-naked-retry.md) for details.

## Later pass (issue #9-#11 NAKED retry)

`sub_8009BE0` is real C now (matches under both compilers). See [issue-9-11-box-naked-retry.md](issue-9-11-box-naked-retry.md).
