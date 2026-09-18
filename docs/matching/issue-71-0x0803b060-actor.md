# Issue #71: 0x0803B060-0x0803B4EC (actor)

25 functions, ~1.1 KB, `asm/code_3_3.s`. This chunk sits immediately
after `src/graphics/actor_aabb_setup.c`/`actor_anim.c`'s existing
content (issue #70) - `GetAnimFrameBaseOffset` (already matched in
`actor_anim.c`) ends exactly at `0x0803B060`, so this chunk's C landed
in the same file (`actor_anim.c`) rather than a new `actor_partN.c`,
keeping ROM contiguity without touching the `actor_partN.c` numbering
several other parallel chunks (issues #50/#56) are also extending right
now.

## Matched (24/25)

All in `src/graphics/actor_anim.c`, in ROM order:

- **`sub_803B060`** - reads the current keyframe record's `attr`
  halfword (`frameTable[frameIndex].attr`) and returns it pre-shifted
  into the high 16 bits; `sub_803B46C` ORs this straight into an OAM
  attribute word.
- **`GetAnimFrameData`** - resolves the current keyframe's tile-
  graphics pointer: `frameTable[frameIndex].frameIndex +
  GetAnimFrameBaseOffset(self)` indexes `frameOffsets` (an array of
  byte offsets), added to the `gUnknown_0300137C` tile-graphics base
  pointer.
- **`sub_803B0A8`** - selects a new keyframe: sets `frameIndex`, copies
  that record's `duration` field into `self+0x10`, and resets the
  `+0x12` flag byte and the `field_08` playback accumulator to 0.
- **20 byte-identical "kind" teardown handlers** (`sub_803B0C4`,
  `sub_803B128`, `sub_803B154`, `sub_803B180`, `sub_803B1AC`,
  `sub_803B1D8`, `sub_803B204`, `sub_803B230`, `sub_803B25C`,
  `sub_803B288`, `sub_803B2B4`, `sub_803B2E0`, `sub_803B30C`,
  `sub_803B338`, `sub_803B364`, `sub_803B390`, `sub_803B3BC`,
  `sub_803B3E8`, `sub_803B414`, `sub_803B440`) - every one of these
  compiles to byte-identical bytes in the ROM (confirmed: each
  function's embedded literal pointer resolves to the same
  `gStaticData_087E4DF4` symbol, and the surrounding unlink/free
  sequence is otherwise identical). Same doubly-linked-list unlink
  convention already named in `src/audio/counter_selector.c`'s
  `sub_803716C` (a local `struct linked_node` with `+0x48`=prev,
  `+0x4c`=next, `+0x50`=state/vtable pointer): set `self+0x50` to the
  shared "dead" table `gStaticData_087E4DF4`, unlink `self` from its
  circular list, and free `self` when `flags & 1`. Almost certainly one
  shared per-"kind" destructor template the original build never
  deduplicated - the same per-"kind"/per-slot pattern seen elsewhere in
  this ROM (`gStaticData_0816BF20`'s 42-slot table, etc.).
- **`sub_803B0F0`** - advances `self+0x20` (a Q8 fixed-point
  accumulator, likely a fall/scroll speed) by a fixed `-0x180`/256 per
  call, then either fires the `self+0x50` trampoline record (arg `3`)
  if `self+0x12` is set, or tail-calls `sub_802A7B8(self)` otherwise -
  the same `+0x50`-rooted `{s16 offset; void *fn}` trampoline
  convention already documented in `actor_part19.c`.

### Compiler-codegen notes

- **`ptr + int` canonicalization forces an add-operand-order fix.**
  `GetAnimFrameData` computes `rec = table + idx*12` where `table` is a
  pointer and `idx*12` an int; this compiler's frontend always
  canonicalizes pointer-arithmetic so the pointer term ends up second
  in the final `ADD`, encoding `add r1,r3,r1` where the ROM has
  `add r1,r1,r3` (same math, different register-order encoding). No
  ordering of the C-level `+` operands changed this. Casting the
  pointer to an integer first (`rec = (u8 *)(idx * 12 + (s32)table)`)
  sidesteps the pointer-arithmetic path entirely and produces the exact
  ROM encoding - same technique as the "which operand goes first"
  `add`-order gap catalogued elsewhere in this file's status page, but
  resolved here rather than parked.
- **A same-valued constant materialized into two different registers.**
  `sub_803B0A8` zeroes both `self+0x12` (a byte) and `field_08` (a
  word) in the same statement group; naive C reuses one register for
  both stores, but the ROM materializes `0` twice into two different
  registers (`movs r2,#0` / `movs r3,#0`) even though one would do.
  Pinning each zero to its own register (`register u8 zero1 asm("r2")`;
  `register s32 zero2 asm("r3")`) reproduces the ROM's redundant
  double-materialize exactly - the same established register-pinning
  technique as `matching_decomp_register_pinning`.
- **Extended `struct anim_part_instance`/added `struct
  anim_frame_record`** (both in `actor_anim.c`) rather than keeping raw
  offsets for `self+0`/`self+4`/`self+0xc` - unlike the "big self"
  object's later fields (state at `+0x28`, `+0x48`/`+0x4c` list,
  `+0x50` trampoline, all still raw per `actor_part19.c`'s own
  precedent), these first three fields are exactly what
  `GetAnimFrameBaseOffset` already exposed a struct for (`field_08`),
  so extending that existing minimal struct - rather than reverting to
  raw casts or inventing a second differently-named struct for the same
  bytes - follows both the "prefer structs" and "reuse an existing
  struct for the same object" conventions. Confirmed via isolated
  recompile that the struct-typed field accesses produce byte-identical
  code to the raw-offset-cast version tried first.

## Parked (1/25)

- **`sub_803B46C`** - fixed-position (120, 106) OAM setup for one
  sprite frame: screen-space visibility cull against the frame's
  width/height, then builds the OAM attribute words (masked position,
  `sub_803B060`'s attr flag, and a priority/palette nibble from
  `self+0x18`/`self+0x14`) and calls `SetupSpriteFrameOam`. This is the
  near-identical twin of the already-parked `sub_802C2FC`
  (`src/graphics/actor_part19b.c`, self-relative position instead of a
  fixed one) - and hits the exact same two gaps documented there:
  - A `| 0`-with-a-zero-valued term (`packed = ... | attr | flag` where
    `flag` is always 0 at this call site) that the ROM keeps as a real
    `movs r0,#0` / `orrs r3,r0` pair but this compiler's dead-store
    elimination always removes, regardless of phrasing (plain literal,
    separate local variable, or `register`-pinned local with its own
    OR statement - all three tried, all three eliminated the same way).
  - A register-budget gap: this compiler needs an extra spilled/high
    register (`r8`) to keep `frame` alive across both the
    `GetAnimFrameData` and `SetupSpriteFrameOam` calls, where the ROM
    fits `self`/`x`/`y`/`frame` entirely in `r4`-`r7` with no spill.
    Explicitly pinning `frame` to `r7` (matching the ROM) instead
    causes the *other* local (`packed`, naturally allocated to `r3` in
    the working case) to collide onto `r7` once `frame`'s pin is
    treated as dead past its last "real" use, corrupting the value
    passed to `SetupSpriteFrameOam` - confirmed via isolated compile
    inspection, not just assumed.

  Semantics are fully understood and every load/store, branch and call
  is confirmed correct; the checked-in raw assembly lives in
  `asm/code_3_3_b46c.s`, gated the same way as `sub_802C2FC`'s
  `asm/code_3_2_20_28568_c2fc.s` (`.if NON_MATCHING == 0` in the `.s`
  file, `#if NON_MATCHING` around the C reconstruction).

## Build layout

- `asm/code_3_3.s` now starts at `sub_803B4EC` (0x0803B4EC) - everything
  from `0x0803B060` through `sub_803B46C`'s end was cut out.
- `asm/code_3_3_b46c.s` (new) holds `sub_803B46C`'s raw bytes, gated
  `.if NON_MATCHING == 0`, linked between `actor_anim.o` and
  `code_3_3.o` in `ldscript.txt`.
- No renames were needed - every function in this chunk keeps its
  original name (either an existing `GetAnimFrameData`-style name
  already referenced by other files, or `sub_XXXXXXXX`), so no other
  `asm/*.s`/`src/**/*.c` call site needed updating.

See [docs/status/actor.md](../status/actor.md) for the running matched/
parked list, and [docs/workflow.md](../workflow.md) for the per-function
loop.

## Update: `sub_803B46C` matched via NAKED transcription

The one function left parked above is now matched: converted to
`NAKED` and the ROM's own disassembly transcribed instruction-for-
instruction (see `docs/matching/issue-69-eeprom-timer.md`'s "NAKED
transcription pass" section for the full account, including a trailing-
padding gotcha found along the way - the disassembly's final
`movs r0, r0` before the function's alignment padding turned out to be
the padding itself, not a real instruction). `sub_802C2FC`, this
function's twin, is unaffected and stays parked - it hits a different
pair of gaps (a genuinely-eliminated `| 0` dead store and a register-
budget spill) that this pass didn't attempt. 25/25 functions in this
issue's original range are now matched; the issue itself was already
closed before this update.
