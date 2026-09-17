# Status: graphics

`src/graphics/` - OAM/sprite rendering, screen fades, palette blending,
per-actor animation frames, text layout.

## Matched

- `src/graphics/graphics.c`: `AllocVramDmaQueue`, `QueueVramDmaTransfer`,
  `FreeVramDmaQueue`, `FlushVramDmaQueue`, `sub_8006B0C`, `sub_8006AF4`,
  `sub_8006AC8`, `sub_8006AAC`, `sub_8006A78`, `sub_8006A84`, `sub_8006A90`,
  `sub_8006A48`, `sub_8006A14`, `sub_80069E8`, `sub_800697C`,
  `sub_8006C28`, `sub_8006C30`, `sub_8006C38`, `sub_8006C44`, `sub_8006C4C`,
  `sub_8006C58`, `sub_8006C84`, `sub_8006CD0`, `sub_8006CE8`, `sub_8006D08`,
  `sub_8006D40`, `sub_8006D50`, `sub_8006D68`, `sub_8006D84`, `sub_8006DA0`,
  `sub_8006DC8`, `sub_8006DF8`, `sub_8006E64`, `sub_8006EA8`, `sub_8006EF0`,
  `sub_8006F5C`, `sub_8006F94`, `sub_8006FB4`, `sub_8006FC8`, `nullsub_1`,
  `sub_8006FE4`, `sub_8007048`, `nullsub_11`, `sub_80070D4`, `sub_80070E8`,
  `sub_80070EC`, `sub_800710C`, `sub_8007110`, `sub_8007114`,
  `sub_8007174`, `sub_800719C`, `nullsub_12`, `sub_80071E4`,
  `sub_800722C`, `sub_8007230`, `sub_800725C`, `sub_8007278`,
  `sub_8007284`, `sub_8007290`, `sub_800729C`, `sub_80072A8`,
  `sub_80072B4`, `sub_80072C0`, `sub_80072CC`, `sub_80072D8`,
  `sub_800731C`, `sub_8007328`, `sub_8007334`, `sub_8007340`,
  `sub_800734C`, `sub_8007358`, `sub_8007364`, `sub_800736C`,
  `sub_8007374`, `sub_8007378`, `sub_800737C`, `sub_8007388`,
  `sub_8007398`, `sub_80073A0`, `sub_80073B0`, `sub_80073B4`,
  `sub_80073B8`, `sub_80073BC`
- `src/graphics/oam_count.c`: `sub_8006700`, `sub_8006714`, `sub_8006770`,
  `sub_80067A4`, `sub_80067B4`, `sub_80067C4`, `sub_80067D4`, `sub_80067E4`,
  `sub_80067EC`, `sub_8006820`, `sub_8006864`, `sub_80068A8`, `sub_80068CC`,
  `sub_8006920`, `sub_800695C`
- `src/graphics/fade_util.c`: `sub_80012AC`, `sub_800132C`
- `src/graphics/palette_blend.c`: `sub_80013FC`
- `src/graphics/actor_anim.c`: `GetAnimFrameBaseOffset`
- `src/graphics/actor_part.c` (new file - `sub_8007A48`'s real ROM
  address isn't adjacent to `graphics.c`'s matched functions, since
  `sub_80073DC`/`sub_8007634` sit unclaimed between them; see
  `docs/matching.md`): `sub_8007A48`, `sub_8007A84`, `sub_8007A98`,
  `nullsub_2`, `sub_8007AB4`
- `src/graphics/actor_part2.c` (new file - `sub_8007C30`'s real ROM
  address isn't adjacent to `actor_part.c`'s matched functions either,
  since the parked `sub_8007B00`/`sub_8007B98` sit raw between them;
  see `docs/matching.md`): `sub_8007C30`, `sub_8007CF8`
- `src/graphics/actor_part3.c` (new file - `sub_8007F78`'s real ROM
  address isn't adjacent to `actor_part2.c`'s matched functions
  either, since the parked `sub_8007DBC` sits raw between them; see
  `docs/matching.md`): `sub_8007F78`, `sub_8007FD8`
- `src/graphics/actor_part4.c` (new file - `sub_80080C0`'s real ROM
  address isn't adjacent to `actor_part3.c`'s matched functions
  either, since the parked `sub_8008044` sits raw between them; see
  `docs/matching.md`): `sub_80080C0`, `sub_800815C`
- `src/graphics/actor_part5.c` (new file - `sub_8008304`'s real ROM
  address isn't adjacent to `actor_part4.c`'s matched functions
  either, since the parked `sub_8008188`/`sub_8008200`/`sub_8008278`
  sit raw between them; see `docs/matching.md`): `sub_8008304`,
  `sub_8008328`, `sub_800834C`, `sub_8008350`, `sub_8008364`,
  `sub_8008394`, `sub_80083A8`
- `src/graphics/actor_part6.c` (new file - `sub_8008408`'s real ROM
  address isn't adjacent to `actor_part5.c`'s matched functions
  either, since the parked `sub_80083B8` sits raw between them; see
  `docs/matching.md`): `sub_8008408`, `sub_8008434`, `sub_8008480`,
  `sub_8008484`, `sub_80084A4`, `sub_80084C4`, `sub_8008518`,
  `sub_8008564`, `sub_80085B8`, `sub_8008604`, `sub_8008618`,
  `sub_8008640`, `sub_8008648`, `sub_8008650`, `sub_800865C`,
  `sub_8008674`, `sub_8008680`, `sub_800868C`, `sub_8008698`,
  `sub_80086A4`, `sub_80086B0`, `sub_80086BC`, `sub_80086C4`,
  `sub_80086CC`, `sub_80086D8`, `sub_80086E4`, `sub_80086EC`,
  `sub_80086F4`, `sub_8008710`, `sub_800872C`, `sub_8008734`,
  `sub_8008748`, `sub_8008754`, `sub_8008768`, `sub_800876C`
- `src/graphics/actor_part7.c` (new file - `sub_800878C`'s real ROM
  address isn't adjacent to `actor_part6.c`'s matched functions
  either, since the parked `sub_8008770` sits raw between them; see
  `docs/matching.md`): `sub_800878C`, `sub_80087A0`, `sub_80087B4`,
  `sub_80087BC`, `sub_80087C0`, `sub_80087C8`, `sub_80087D0`,
  `sub_80087F4`, `sub_80087FC`, `sub_8008804`, `sub_800880C`,
  `sub_8008814`, `sub_8008818`, `sub_800881C`, `sub_8008824`,
  `sub_8008830`, `sub_8008844`, `sub_8008850`, `sub_800885C`,
  `sub_8008864`, `sub_8008870`, `sub_800887C`, `sub_8008888`,
  `sub_800888C`, `sub_8008890`, `sub_80088D8`, `sub_80088E8`,
  `sub_80088F0`, `sub_8008904`

- `src/graphics/actor_part10.c` (new file - `sub_8008C80`'s real ROM
  address isn't adjacent to `actor_part7.c`'s matched functions
  either, since the parked `sub_8008AD8` sits raw between them; see
  `docs/matching.md`): `sub_8008C80`, `sub_8008CEC`, `sub_8008D30`

- `src/graphics/actor_part11.c` (new file - `sub_8008DC0`'s real ROM
  address isn't adjacent to `actor_part10.c`'s matched functions
  either, since the parked `sub_8008D80` sits raw between them; see
  `docs/matching.md`): `sub_8008DC0`, `sub_8008DEC`, `sub_8008E50`,
  `sub_8008E94`, `sub_8008EB4`, `sub_8008EE4`

- `src/graphics/actor_part12.c` (new file - `sub_8009A30`'s real ROM
  address isn't adjacent to `actor_part11.c`'s matched functions
  either, since the parked `sub_8008F20` and the raw `sub_8009008`-
  `sub_8009914` span sit between them; see `docs/matching.md`):
  `sub_8009A30`, `sub_8009AA0`, `sub_8009AF0`, `sub_8009B3C`,
  `sub_8009B70`, `sub_8009B9C`

- `src/graphics/actor_part13.c` (new file - `sub_8009CA0`'s real ROM
  address isn't adjacent to `actor_part12.c`'s matched functions
  either, since the raw `sub_8009BE0` sits between them; see
  `docs/matching.md`): `sub_8009CA0`

- `src/graphics/actor_part8.c` (new file - `sub_8009EA8`'s real ROM
  address isn't adjacent to `code_3_2_15.o`'s raw content either, since
  the parked `sub_8009DF4` sits raw between them, and `sub_8009BE0`
  before that was left raw rather than guessed at; see
  `docs/matching.md`):
  `sub_8009EA8`, `sub_8009EB0`, `sub_8009EBC`, `sub_8009EC4`,
  `sub_8009ECC`, `sub_8009ED0`, `sub_8009F1C`, `sub_8009F50`,
  `sub_8009F90`, `sub_8009FB0`
- `src/graphics/actor_part9.c` (new file - `sub_8009FD4`'s real ROM
  address isn't adjacent to `actor_part8.c`'s matched functions
  either, since the parked `sub_8009DF4` sits raw between them; see
  `docs/matching.md`): `sub_8009FD4`, `sub_8009FF4`, `sub_800A050`,
  `sub_800A068`, `sub_800A06C`, `sub_800A078`, `sub_800A080`,
  `sub_800A088`, `sub_800A090`, `sub_800A098`, `sub_800A09C`,
  `sub_800A0A0`, `sub_800A0A4`, `sub_800A0A8`, `sub_800A0AC`,
  `sub_800A0CC`, `sub_800A0D8`, `sub_800A0E0`, `sub_800A0EC`,
  `sub_800A0F4`

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8006600`** (`src/graphics/oam_count.c`) - HUD-icon-plus-number
  renderer. Prologue/epilogue and most register choices now match the ROM;
  four register-letter mismatches remain in the second half, resistant to
  every technique tried so far - see `docs/matching.md`, "Parked, not
  matched: `sub_8006600`" for the full account.
- **`sub_8000EE4`** (`src/graphics/text_layout.c`) - word-wrap text
  renderer. Matches the ROM instruction-for-instruction except ~8 bytes
  from two small codegen details (incoming-argument spill ordering, and a
  couple of loop-bound comparisons compiling one instruction shorter than
  the ROM's) - see `docs/matching.md`, "Parked, not matched: `sub_8000EE4`".
- **`sub_80073DC`** (`src/graphics/graphics.c`) - builds and queues one
  OAM entry per visible sub-piece of an animated part, plus a combined
  VRAM tile upload. Logic/instruction shape confirmed correct (every
  AND/OR/shift constant and branch condition matches the ROM), but the
  ROM spills more locals to its stack frame than gcc does here, causing
  register-letter differences through most of the per-piece loop - see
  `docs/matching.md`, "Parked, not matched: `sub_80073DC`".
- **`sub_8007B00`** (`src/graphics/actor_part.c`) - builds an AABB for
  `part`'s current animation keyframe (via the shared `sub_803AFE4`/
  `sub_803AFDC` primitive) and mirrors it horizontally/vertically per
  flag bits. Matches the ROM instruction-for-instruction except one
  systematic register choice (`part` lands in r6 here vs the ROM's r7,
  cascading into a 3- vs 4-register push/pop list) - see
  `docs/matching.md`, "Parked, not matched: `sub_8007B00`".
- **`sub_8007B98`** (`src/graphics/actor_part.c`) - the same AABB-for-
  keyframe shape as `sub_8007B00`, for a second, differently-laid-out
  keyframe table. Matches the ROM's operations/order throughout except
  a recurring "which anonymous scratch register" choice (about 10 of
  73 instructions) - see `docs/matching.md`, "Parked, not matched:
  `sub_8007B98`".
- **`sub_8007DBC`** (`src/graphics/actor_part2.c`) - collision-with-
  player handler: tests two `part` flag bits, AABB-collides `part`
  against the player, plays a sound and marks a global bitmap slot on
  hit, then spawns one of several "kind"s of object at `part`'s
  position depending on a `part` sub-type field. Matches the ROM
  instruction-for-instruction except one systematic register choice
  (the cached `&gUnknown_030012D8` address lands in r6 here vs the
  ROM's r7 - an explicit r7 pin crashes the compiler outright here) -
  see `docs/matching.md`, "Parked, not matched: `sub_8007DBC`".
- **`sub_8008044`** (`src/graphics/actor_part3.c`) - advances `part`'s
  per-keyframe animation timer by one tick. The first half (the
  counter-vs-duration test) matches the ROM instruction-for-instruction
  exactly, including its `ip`-register `part` trick; sharing the
  keyframe-table pointer/index-byte address into the second half (as
  the ROM itself does, avoiding a second `part+0x20` reload) reliably
  makes gcc stop using `ip` for `part` altogether instead, trading one
  mismatch for a worse one - see `docs/matching.md`, "Parked, not
  matched: `sub_8008044`".
- **`sub_8008188`** (`src/graphics/actor_part4.c`) - adjusts a `dest`
  position per a `kind` selector and a small `rec` record. Matches the
  ROM instruction-for-instruction - including the exact non-obvious
  case-body layout order and the two duplicate case labels correctly
  sharing one code block - except a single register-register `add`'s
  operand order in that shared block; this compiler appears to always
  canonicalize such an add so the destination's prior value is the
  first source operand, with no C-level way found to override it -
  see `docs/matching.md`, "Parked, not matched: `sub_8008188`".
- **`sub_8008200`** (`src/graphics/actor_part4.c`) - the same shape as
  `sub_8008188` (mirror-image add/subtract directions), parked for the
  identical single-instruction gap - see `docs/matching.md`, "Parked,
  not matched: `sub_8008200`".
- **`sub_8008278`** (`src/graphics/actor_part4.c`) - a third variant:
  kind 1/2 update `field_0` and *unconditionally* also update
  `field_4`; kinds 4/8/12 update `field_4` and unconditionally also
  update `field_0`. Parked for the same single-instruction gap as
  `sub_8008188`/`sub_8008200` - see `docs/matching.md`, "Parked, not
  matched: `sub_8008278`".
- **`sub_80083B8`** (`src/graphics/actor_part5.c`) - looks up `part`'s
  current keyframe record, conditionally clamps its frame index/resets
  its sub-counter (mirroring `sub_8008044`'s "done" handling), then
  resolves a two-level pointer-array lookup. Matches the ROM
  instruction-for-instruction (confirmed the apparent `ands` mismatch
  is a disassembly-style artifact, not a real one) except the same
  "which operand goes first" `add`-operand-order gap as
  `sub_8008188`/`sub_8008200`/`sub_8008278` - see `docs/matching.md`,
  "Parked, not matched: `sub_80083B8`".
- **`sub_8008770`** (`src/graphics/actor_part6.c`) - looks up `part`'s
  current keyframe record (same lookup as `sub_8008734`) and tests its
  `+0x17` flags bit 1, returning it as 0/1. Matches the ROM through the
  `ands` that computes the bit; the ROM's two trailing truncation
  instructions (`lsls`/`lsrs` to a byte) get optimized away here since
  this compiler can prove the value already fits - see
  `docs/matching.md`, "Parked, not matched: `sub_8008770`".
- **`sub_800891C`** (`src/graphics/actor_part7.c`) - filters/compacts
  an array of `part`-like objects into a second output array each
  call, broad/narrow-phase-testing each one against a `gUnknown_03001308`-
  sub-object-centered region pair via `part->table`-driven
  trampolines, with array-entry removal handled via the GBA BIOS
  `CpuSet` SWI. Semantics fully understood and every call shape
  confirmed correct, but this compiler puts the loop counter into a
  high register (`r8`, needing a second high register for another
  loop-invariant pointer) instead of the ROM's low register `r7` -
  explicitly pinning it to `r7` triggers the `r7`-pin corruption
  pattern documented elsewhere in this ROM region instead of fixing
  it - see `docs/matching.md`, "Parked, not matched: `sub_800891C`".
- **`sub_8008A40`** (`src/graphics/actor_part7.c`) - iterates a
  `manager`'s array of `part`-like objects, broad-phase-testing each
  via a `table+0x48/0x4c` trampoline and a flags-bit check, then
  dispatches an incoming rectangle to `sub_8008AD8` or `sub_8008D80`
  depending on whether a caller-supplied "compare viewport" argument
  matches the current `gUnknown_030012D8` (the camera/viewport).
  Resolved `sub_800014C` as a plain `memcpy`-style BIOS `CpuSet`
  wrapper along the way. Every branch and call argument confirmed
  correct; parked purely on a register-spill gap for the
  "compareViewport" argument, which needs an extra high register here
  instead of the ROM's `r7` - explicitly pinning it to `r7` produces a
  genuine miscompile (aliases with the unrelated loop counter) rather
  than fixing the gap - see `docs/matching.md`, "Parked, not matched:
  `sub_8008A40`".
- **`sub_8008AD8`** (`src/graphics/actor_part7.c`) - resolves
  collision push-out between a `part` and the player
  (`gUnknown_030012D8`) against the box `sub_8008A40` passes in,
  branching on `gUnknown_030012C0`'s mode field and `part`'s own flag
  bits, and firing `table+0x68`-driven trampolines and (in one path) a
  sound effect. A ~150-instruction function, but every branch, field
  offset, and call argument confirmed correct; parked on two small
  structural gaps this compiler can't directly avoid: an unavoidable
  extra load for one box coordinate that the ROM leaves untouched in
  its original stack slot (the same ABI stack-layout trick used by
  `sub_8008A40`), and a knock-on register-letter difference for `part`
  - see `docs/matching.md`, "Parked, not matched: `sub_8008AD8`".
- **`sub_8008D80`** (`src/graphics/actor_part7.c`) - `sub_8008AD8`'s
  sibling, resolving the same collision-hit logic when the "compare
  viewport" doesn't match the current one. Every branch, field offset,
  and call argument confirmed correct; parked on the same box-
  coordinate stack-layout gap as `sub_8008AD8`/`sub_8008A40` - see
  `docs/matching.md`, "Parked, not matched: `sub_8008D80`".
- **`sub_8008F20`** (`src/graphics/actor_part11.c`) - initializes a
  fixed-slot object-pool manager struct: two big 256-word zeroed
  tables (likely a pair of spatial-partition/collision grids), plus a
  singly-linked free list built over an allocated node array. Every
  load, store, and field offset confirmed correct; parked purely on a
  many-register (item count, two persistent field addresses, a reused
  loop index, a running byte offset) allocation gap across
  `r3`/`sb`/`sl`/`r4`/`r8` - see `docs/matching.md`, "Parked, not
  matched: `sub_8008F20`".
- **`sub_8009150`** (`src/graphics/actor_part11.c`) - lazily creates a
  "large object" bucket-255 grid registration for an object that
  didn't get one at insert time. Every load, store, and field offset
  confirmed correct; parked purely on a loop-invariant-hoisting gap (a
  free-list-head address computation this compiler correctly hoists
  out of a 255-iteration loop, where the ROM recomputes it fresh every
  non-empty bucket) - see `docs/matching.md`, "Parked, not matched:
  `sub_8009150`".
- **`sub_8009914`** (`src/graphics/actor_part11.c`) - resets a pool
  manager to empty: tears down every active object, then rebuilds the
  grid and free list from scratch. The teardown loop is confirmed
  correct; the rebuild loop is a byte-for-byte copy of `sub_8008F20`'s
  own tail and hits the identical many-register allocation gap - see
  `docs/matching.md`, "Parked, not matched: `sub_8009914`".
- **`sub_80099F0`** (`src/graphics/actor_part12.c`) - `sub_8008D80`'s
  twin: byte-identical in shape (same collision-hit-resolve logic,
  same "dead read" trampoline call), called from elsewhere in this
  cluster. Parked on the same `boxH` stack-layout gap - see
  `docs/matching.md`, "Parked, not matched: `sub_80099F0`".
- **`sub_8009D5C`** (`src/graphics/actor_part13.c`) - fires a
  `part->table+0x68`-driven trampoline based on `gUnknown_030012C0`'s
  mode, on the player and/or `part` depending on the mode value.
  Every branch, call, and argument confirmed correct (a `switch`
  reproduces the ROM's exact 3-way mode dispatch, and explicit `goto`s
  into a shared, ABI-register-pinned tail reproduce the mode-0/mode-
  1-2 call sharing); parked on a single remaining conditional-branch
  encoding gap in the mode-3 case - see `docs/matching.md`, "Parked,
  not matched: `sub_8009D5C`".
- **`sub_8009DF4`** (`src/graphics/actor_part8.c`) - a velocity/
  position integrator: steps each axis's velocity toward its max by
  its accel amount (clamped so it never overshoots), builds a
  direction-flags byte from the clamped velocities' signs, caches the
  pre-move position, applies the velocity, and updates a global with
  the Y velocity. Every branch and memory access confirmed correct;
  parked purely on a leaf-vs-non-leaf register-budget gap (the ROM
  needs no stack frame at all, fitting entirely in r0-r3 with `self`
  in r2 reused once dead; every arrangement tried here needs one extra
  spilled register) - see `docs/matching.md`, "Parked, not matched:
  `sub_8009DF4`".
