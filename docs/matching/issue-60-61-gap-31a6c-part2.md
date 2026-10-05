# Issues #59 (rest)/#60/#61, Phase 2 second half: 0x080326E4-0x08033804

Second half of issue #59's Phase 2 gap - the 60 raw functions left after
[issue-59-0x08031784-actor.md](issue-59-0x08031784-actor.md)'s Phase 1
pass closed `0x08031784`-`0x08031A6C`. That Phase 2 gap
(`0x08031A6C`-`0x08033804`, `asm/code_3_2_20_28568_c99c_31784_31a6c.s`)
was split in half for parallel work: a sibling pass covers the first 30
functions (`UpdateJetpackBalloonCrate`-`UpdateJetpackRing`, `src/graphics/actor_part129.c`);
this pass covers the last 30 (`CreateJetpackRing`-`nullsub_35`, new file
`src/graphics/actor_part130.c`).

## What this pass closed: 16 matched, 14 NAKED, 0 left raw

All 30 functions in this pass's range are now either real decompiled C
or byte-verified NAKED transcriptions - none were left untouched. See
[docs/status/actor.md](../status/actor.md) for the full per-function
list (both the "Matched" and "Parked - NAKED transcription" sections
have entries for this pass, cross-referencing this document).

### Two RAM-family findings

1. **The singleton system's own camera-follow/scroll-velocity RAM
   family, `gUnknown_030015A0`-`030015FF`.** `sub_8032C0C`/`sub_8032EA0`
   are the first functions in ROM order to touch most of this family's
   fields (`gHovercraftX`-`030015EC`, ~9 live fields across a
   ~0x38-byte span with a few gaps, plus a separate small cluster at
   `030015FC`-`030015FF`). A **later** ROM region (issue #62,
   `0x08033804`+, `src/graphics/actor_part28.c`-`actor_part37.c`,
   already matched in an earlier pass) also touches this exact family
   and had *already* established the naming and, more importantly, the
   **representation convention**: flat, independently-linked `extern`
   globals (e.g. `extern s32 gHovercraftX;`), not fields of a
   struct reached through a common base pointer. This pass reuses that
   established convention verbatim (including the exact names already
   assigned by `actor_part28.c`'s own extern block) for consistency,
   rather than introducing a struct wrapper as originally guessed in
   `docs/rom_map.md`'s reconnaissance-era note ("a second RAM-struct
   family... define this as a proper named struct") - a struct wrapper
   doesn't fit how these bytes are actually laid out in the linked
   BSS (each one its own symbol, not one contiguous allocation reached
   via base+offset), and diverging from the already-merged
   `actor_part28.c` convention for the exact same bytes would be a
   worse outcome than following the a-priori guess. New fields this
   pass adds to the family (not touched by `actor_part28.c`):
   `gUnknown_030015A0`/`030015A4`/`030015A8` (P2-meter-shaped row/column
   counts and fill level, seeded by `CreateHovercraft` from
   `gHovercraftPicture`), `gUnknown_03001600` (the row-pointer array
   `DrawHovercraftMap`/`ConvertHovercraftTiles` fill, structurally identical to the boss
   cluster's `gUnknown_03001580`), `gUnknown_030015C0`/`030015C4`
   (BG2-space dy/dx offsets), `gUnknown_030015C8` (projection-scale
   source, structurally identical to the boss cluster's
   `gUnknown_03001554`), `gUnknown_030015E0`/`030015E4`/`030015E8`/
   `030015EC`/`030015F0`/`030015F4` (per-"kind" record cache fields and
   accumulators), `gUnknown_0300159C` (an "apply now" BG2 latch,
   `u8`, same role as the boss cluster's `gUnknown_03001524`), and
   `gUnknown_03001598` (a BG2 preset toggle, `s32`, same role as
   `gUnknown_03001520`).

2. **No new "singleton object" struct was introduced either**, for the
   same reason: `CreateHovercraft` (the constructor), `SpawnHovercraft`,
   `UpdateHovercraft`, `UpdateHovercraftBg2`, `LoadHovercraftGraphics` all operate on the
   singleton object through raw `self+offset` casts, matching every
   neighboring `actor_part*.c` file's own convention for this same
   large per-instance object family (state at `+0x28`, table-index at
   `+0xc`, anim-frame halfword/byte pair at `+0x10`/`+0x12`, accumulator
   at `+8`, part table at `+0`, "frame offsets" at `+4`, event table at
   `+0x50`) - "none of these objects' full shapes are pinned down yet"
   remains true here too.

### A confirmed structural tie

`HovercraftStateFall`/`DrawHovercraftMap`/`UpdateHovercraft`/`UpdateHovercraftBg2`/`LoadHovercraftGraphics`
confirm the singleton is a **second, independent "unique object"**
running machinery structurally parallel to the boss cluster (issue #58)
end to end: a patrol/oscillation driver (mirrors `AirshipStateFireballs`), a
BG-tilemap-blit tile consumer (mirrors `DrawAirshipMap`), a per-frame
animate+project+tile-stream driver (mirrors `LoadAirshipGraphics`), a BG2
affine-matrix committer (mirrors `UpdateAirshipBg2`, pure scale - no
rotation, matching `docs/rom_map.md`'s prior finding), and a top-level
per-frame driver that DMA-clears/fills a blank BG3 tile exactly like
`LoadAirshipGraphics` does before its own meter call. `ConvertHovercraftTiles` is the
P2-side twin of the already-matched VRAM fill-level meter `ConvertAirshipTiles`
(issue #58, `actor_part26c.c`), on this singleton's own per-level table
(`gHovercraftPalette`) and row array (`gUnknown_03001600`) rather than
the boss's (`gAirshipPalette`/`gUnknown_03001580`).

### A corrected semantic reading

`sub_8032A24`'s patrol-speed clamp was initially misread as a *ceiling*
(`if (delta > 0x13) delta = 0x14`); the full-link byte diff caught the
actual ROM branch condition (`bgt` skips the clamp, i.e. the clamp only
fires when the decaying value has dropped to `0x13` or below) - it's a
**floor**, consistent with the "decays... floored at 0x14" reading
already used to describe `HovercraftStateFall`'s own oscillator. Fixed to
`if (delta <= 0x13) delta = 0x14;` before this was recorded as matched.

## Compiler-quirk catalog for this pass

Several genuine full-link-only gaps surfaced here, all following the
established pattern documented in `docs/workflow.md` step 3 (an
isolated compile is a diagnostic tool, never proof of a match) - none of
these were visible from the isolated per-function compile, only from
the map-file address-shift check after cutting the whole batch into its
real `.c` file and linking:

- **`CreateHovercraft`** (the singleton constructor) - a first plain-C
  attempt kept the freshly `mem_alloc`'d pointer and
  `&gHovercraft` in the same register (collapsing the ROM's own
  `r4`(address)/`r5`(allocation) split) and used one shared "zero"
  register where the ROM keeps two independent ones (`r4` reused after
  the address store, plus a separate `r6` zero for the `self+0x12`
  byte store) - 4 bytes short of the ROM once linked. Transcribed
  NAKED instead of chasing the exact register split further.
- **`UpdateHovercraft`** (the per-frame animate+project+tile-stream driver)
  - the mirror-image gap: a plain-C attempt produced 4 bytes *longer*
  than the ROM. Transcribed NAKED.
- **`LoadHovercraftGraphics`** (the top-level per-frame driver) - a plain-C attempt
  using this codebase's `DmaSet()` macro (matching the exact
  store-then-readback pattern the ROM's own two DMA setups use) was 24
  bytes *short* of the ROM once linked - this compiler folds the two
  DMA setups' shared literal-pool addressing more aggressively than the
  ROM's own build did. Transcribed NAKED.
- **`DestroyJetpackCollectedWumpa`** (a reward-dispensing teardown, same shared shape as
  the already-matched `sub_803B0C4`) - every instruction reproduced
  correctly except the very first two: this compiler's parameter-
  register prologue shuffle always copies the incoming `flags` (r1)
  parameter before `self` (r0) regardless of C declaration order,
  register pins, or an explicit `asm volatile` sequencing barrier
  between the two copies - forcing the barrier to flip the order
  instead triggered this project's confirmed categorical `r7` hazard
  (an explicit `register T x asm("r7")` compiles correct instructions
  but silently drops `r7` from the generated push/pop list once its
  copy is scheduled second), empirically confirmed here by test-
  compiling both variants. Transcribed NAKED rather than ship code that
  corrupts the caller's `r7`.
- **`DamageHovercraftFireball`/`sub_8032A24`'s shared state/anim-frame-reset tail** -
  needed the same "nested register-pin block" idiom already documented
  for `sub_80318B4` (issue #59): a `one` (r0) and `zero` (r2) constant
  each declared in their own nested block, matching the ROM's exact
  interleaving of stores between the two literals, plus a third,
  independently-scoped `zero2` (r1) declared immediately before its one
  use (the `self+0x12` byte store) even though it's logically "the same
  zero" as `zero` - the ROM's own build materializes it as a genuinely
  separate literal load rather than reusing the already-live register.
- **`sub_8032A24`'s head** - computing `s32 speed = *(self+0x60) - 5;`
  as an eager local (evaluated before `self+0x24`'s update) made this
  compiler keep `self` in a temporary register before its final home,
  costing an extra `adds` instruction the ROM's build doesn't have;
  restructured to read `self+0x24` and `self+0x60` in the ROM's own
  order (sum first, delta second) and explicitly pin `self` itself to
  `r4` for the whole function, matching the ROM's single `adds r4, r0,
  #0` exactly.
- **`sub_8032A24`'s `_call_via_r2` call** - pre-computing the third
  argument (`table+0x24`) into a named local before the call made this
  compiler evaluate it *before* the first argument's own dependent read
  (`table+0x20`); inlining both reads directly as call-argument
  expressions restored the ROM's left-to-right evaluation order
  (`table+0x20` read, then the `player+offset` add, then `table+0x24`
  read last, immediately before the call).
- **`UpdateHovercraftHitFlash`'s counter-reload and XOR-toggle** - this compiler
  keeps the just-incremented counter value live in its own register and
  reuses it for the following `& 3` check, where the ROM's own build
  re-reads the counter from memory instead; forced via a genuine
  volatile re-read (`*(vu16 *)&gUnknown_030015FC`, not `vs16` - the
  signed variant emits extra sign-extension instructions the ROM
  doesn't have) plus explicit register pins (`r0`=the literal `3`,
  `r1`=the reloaded value, with the AND's result forced back into `r0`
  via a separate pinned output variable) to reproduce the ROM's exact
  operand/destination choice for the `ands` instruction.
- **`UpdateHovercraftHitFlash`'s loop end-address computation** - a literal
  `(vu16 *)0x0500003e` constant compiles to a single `ldr` from the
  literal pool, but the ROM's own build computes it at runtime instead
  (`dst_base + 0x1e`), a different instruction shape entirely; switched
  to explicit pointer arithmetic (`(vu16 *)((u8 *)dst + 0x1e)`) to
  match, and the terminating `while (dst <= end)` needed a signed
  comparison (`(s32)dst <= (s32)end`, producing `ble`) rather than the
  natural unsigned pointer comparison (`bls`) this compiler otherwise
  picks for `vu16 *` operands.

## NAKED transcriptions carried over from established gaps

The remaining NAKED functions hit gaps already fully documented
elsewhere in this project, re-confirmed rather than re-derived here:

- **`UpdateJetpackCollectedWumpa`** - the shared anim-frame-advance-and-clamp idiom
  (this compiler schedules the `#4`/`#6` `ldrsh` constant loads one
  instruction earlier than the ROM's own build), same as
  `MoveJetpackBalloon`/`sub_8031954`/`sub_80319A0` (issue #59).
- **`UpdateHovercraftFireball`/`sub_8032A94`** - a `gHovercraftFireballStateFuncs` stride-8
  table lookup keeping the table's base address alive in `r7` across
  straight-line code with no call to piggyback a high-register relay
  on, the same categorical `r7`-never-self-allocated hazard as
  `sub_8031A08` (issue #59)/`UpdateHovercraftCannon`/`RunHovercraftCannonState`/`UpdateHovercraftLauncher`
  (issue #62).
- **`DrawJetpackCollectedWumpa`** - a bounding-box-culled sprite draw with an
  `r8`-flag-across-calls shape, the same class of gap `DrawPowerDialog`/
  `DrawLanguageSelect`/`GAX2_init` and the hard-won `DrawActor`
  (issue #50, `actor_part55.c`) already needed elaborate register-pin/
  stack-spill workarounds for.
- **`RunHovercraftState`** - fully inlines `sub_8033828`'s own P1/P2
  speed-toggle shape (issue #62) *twice*, once per frame-counter
  schedule case, plus an outer dispatch - the same cross-jump-merging
  register-pin hazard that function's own writeup documents, doubled.
  **Update:** promoted to real C in a later pass via the static-inline
  anti-CSE technique (register-pinned `p`/`val` locals matching
  `sub_8033828`'s own idiom exactly, plus an `asm("" : "+r"(p))` barrier
  to keep each branch's pointer reload from being merged into the shared
  tail) - see
  [issue-59-60-static-inline-cse-promotion.md](issue-59-60-static-inline-cse-promotion.md).
- **`sub_8032C0C`/`sub_8032EA0`/`DrawHovercraftMap`/`SpawnHovercraft`/
  `ConvertHovercraftTiles`** - many-high-register (`ip`/`sb`/`sl`/`r8`)
  allocation, the same gcc-2.9 difficulty already documented
  project-wide for `ConvertAirshipTiles`/`DrawLanguageSelect`/`GAX2_init` and
  others. `ConvertHovercraftTiles` in particular is a near-identical twin of the
  already-NAKED `ConvertAirshipTiles` (issue #58) - same shape, different
  per-level table/row array.

## Verification

Verified via the mandatory full pipeline: `rm -rf build && make
NON_MATCHING=1 report` (no warnings for `actor_part130.c`), then
`rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` confirming `crashbandicootxs.gba:
OK` (the project's `sha1sum -c checksum.sha1` success message).

## What remains

This pass's own range (`0x080326E4`-`0x08033804`) is fully closed - 16
matched, 14 NAKED, 0 left raw. The sibling pass covering
`0x08031A6C`-`0x080326E4` (`src/graphics/actor_part129.c`, issues #59
rest/#60) is tracked separately; once both land, the whole gap between
issue #58 and issue #62 (`0x08031784`-`0x08033804`) is closed.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.

## Later pass: member-pointer dispatch

A later pass promoted `UpdateJetpackCollectedWumpa`, `DestroyJetpackCollectedWumpa`, `UpdateHovercraftFireball` and `sub_8032A94` (`actor_part130.c`). `DestroyJetpackCollectedWumpa`'s parameter-copy order came out right from a plain C destructor with no barrier; `UpdateJetpackCollectedWumpa`'s anim idiom needed per-field `anims[animIndex]` indexing. The other NAKED functions here (many-high-register, DMA/tile, constructor cases) were not retried from NAKED to real C. The "r7 table-base"
shape was gcc 2.x's pointer-to-member-function call
`(this->*table[this->state])()`, which `ACTOR_PMF_CALL` in
`include/actor_self.h` reproduces with no register pins. See
[pmf-dispatch-retry.md](pmf-dispatch-retry.md).

## Later pass: NAKED retry

A later pass promoted `sub_8032C0C`, `sub_8032EA0`, `CreateHovercraft`,
`SpawnHovercraft`, `UpdateHovercraft` and `LoadHovercraftGraphics` to plain C, mostly by
porting fixes from their issue #58 twins. The link-time byte-count gaps
noted above came from a reused `self` pseudo (`UpdateHovercraft`) and the
tile-clear loop's form (`LoadHovercraftGraphics`). `DrawHovercraftMap` and `ConvertHovercraftTiles`
stay NAKED with `#if NON_MATCHING` drafts. See
[issue-58-61-naked-retry.md](issue-58-61-naked-retry.md).

## Later pass: third near-miss sweep

`ConvertHovercraftTiles` is real C (matches under both compilers). The 0xf mask
is an opaque value (`asm("" : "=r"(m) : "0"(0xf))`) ANDed as `m & b`,
the second byte of each pair has its own local, the second loop has its
own counter, and the row header is written step by step in ROM order.
See [near-miss-polish-3.md](near-miss-polish-3.md).
