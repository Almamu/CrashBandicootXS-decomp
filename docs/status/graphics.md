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

- `src/graphics/actor_part14.c` (new file - `sub_800A5F4`'s real ROM
  address isn't adjacent to `actor_part9.c`'s matched functions
  either, since a large raw span (`sub_800A0FC`-`sub_800A590`) sits
  between them; see `docs/matching.md`): `sub_800A5F4`, `sub_800A600`,
  `sub_800A604`, `sub_800A650`, `sub_800A664`, `sub_800A6A4`,
  `sub_800A6C4`, `sub_800A6D0`, `sub_800A6DC`, `sub_800A6E8`,
  `sub_800A6F4`, `sub_800A700`, `sub_800A70C`, `sub_800A718`,
  `sub_800A724`, `sub_800A730`

- `src/graphics/actor_part15.c`/`src/graphics/actor_part16.c` (new
  files, split around the raw untouched `sub_800B3F0` - see
  `docs/matching.md`): a new not-yet-named big object's accessors -
  `sub_800B324`, `sub_800B334`, `sub_800B33C`, `sub_800B360`,
  `sub_800B37C`, `sub_800B3AC`, `sub_800B4A4`, `sub_800B4AC`,
  `sub_800B4B8`, `sub_800B4C4`, `sub_800B4D0`, `sub_800B4F0`,
  `sub_800B4F8`, `sub_800B508`, `sub_800B510`, `sub_800B51C`,
  `sub_800B524`, `sub_800B53C`, `sub_800B544`, `sub_800B554`,
  `sub_800B55C`, `sub_800B564`, `sub_800B56C`, `sub_800B574`,
  `sub_800B57C`, `sub_800B584`, `sub_800B58C`, `sub_800B5A0`,
  `sub_800B5A8`, `sub_800B5B0`, `sub_800B5BC`, `sub_800B5C4`,
  `sub_800B5CC`, `sub_800B5D8`, `sub_800B5E0`, `sub_800B5E8`,
  `sub_800B5F0`, `sub_800B5FC`, `sub_800B608`, `sub_800B614`,
  `sub_800B620`, `sub_800B62C`, `sub_800B638`, `sub_800B644`,
  `sub_800B650`, `sub_800B678`, `sub_800B698`, `sub_800B69C`

- `src/graphics/actor_part17.c` (new file - see `docs/matching.md`):
  `sub_800B704`, `sub_800B734`, `sub_800B7B0`, `sub_800B838`,
  `nullsub_13`, `sub_800B86C`, `sub_800B8A4`, `sub_800B8A8`,
  `sub_800B8C8`, `sub_800B8D8`

- `src/graphics/actor_part18.c`/`actor_part18b.c` (new files, non-
  adjacent since the parked `sub_801434C` sits raw between them - see
  `docs/matching.md`, issue #17): `sub_801426C`, `sub_80142B0`,
  `sub_80144E0`, `sub_8014524` - four entries of the `gStaticData_0816BF20`
  42-slot action dispatch table

- `src/graphics/fade_screen_mode.c` (new file - `sub_8001510`) and
  `src/graphics/fade_screen_mode2.c` (new file - `sub_800153C`,
  `sub_8001550`, `sub_8001564`, `sub_8001578`, `sub_800158C`,
  `sub_80015A0`, `sub_80015B0`, `sub_80015C0`, `sub_80015D0`,
  `sub_80015E0`, `sub_80015F0`, `sub_8001604`, `sub_8001614`): the
  fade/screen-mode utility cluster documented in `docs/rom_map.md`,
  split around two parked functions - see `docs/matching.md`.

- `src/graphics/aabb_util.c` (new file): `sub_8001640`, `sub_8001688`,
  `sub_80016D0`, `sub_80016DC` - two AABB overlap tests (one already
  referenced by name from `actor_part15.c`) plus `mem_free`/`mem_alloc`
  wrappers.

- `src/graphics/hud_counter.c` (new file, contributed via PR #1 by
  @MiryamSanchez26 - `sub_8027838` is an isolated HUD counter update
  inside the raw HUD stat-widget region, so preserving its ROM address
  required a split of what's now `asm/code_3_2_17.s`/`asm/code_3_2_20.s`
  right around it): `sub_8027838`

- `src/graphics/hud_icon_slot.c` (new file, GitHub issue #45, non-
  adjacent to `hud_counter.c` since `sub_8027138`-`sub_802763C` sit raw
  between them): `sub_8027088`, `sub_80270A8`, `sub_80270C0`,
  `sub_80270E0`, `sub_802710C` (UNUSED - no caller anywhere in the
  ROM), `sub_8027120` - a fixed 3-entry particle/effect queue's reset/
  constructor/teardown trio, a HUD digit-slot draw helper, and two
  `struct actor`-table-swap slot constructors; see `docs/matching.md`.
  Also added `include/hud.h`, moving `hud_counter.c`'s
  `hud_anim_record`/`hud_anim_data`/`hud_digit_part`/`hud_counter`
  structs there so this file could reuse them.

- `src/graphics/hud_blink.c` (new file, GitHub issue #45):
  `sub_8028400`, `sub_8028474`, `sub_80284A4`, `sub_80284D4`,
  `sub_8028504`, `sub_8028520` - a 3-slot icon-blink animation timer
  (per-frame tick, three per-slot triggers, and the generic single-slot
  advance they share); see `docs/matching.md` for two codegen gotchas
  hit along the way.

- `src/graphics/intro_screen.c` (new file, replacing `asm/code_3_1.s` -
  boot-adjacent but not part of `src/system/boot_util.c` since
  `main.c`/`memory.c`/`irq.c` sit between them in ROM order):
  `sub_80007EC` - BG2 affine setup for a full-screen intro image; see
  `docs/matching.md` for the statement-ordering gotchas.

- `src/graphics/settings_menu2.c` (new file - the composite pause/
  options screen's BG-load helper and per-row stats gatherer/
  aggregator; see `docs/rom_map.md`'s `overlay_ui` section):
  `sub_80047F8`, `sub_8004860`, `sub_80048BC`, `sub_80048E0`
- `src/graphics/settings_menu3.c` (new file - the same screen's flag
  test, link-cancel-flag pair, six near-identical per-item wrappers,
  state jump-table dispatcher, and a final list-refresh trio):
  `sub_8004A50`, `sub_8004A64`, `sub_8004A80`, `sub_8004AA4`,
  `sub_8004ACC`, `sub_8004AFC`, `sub_8004B24`, `sub_8004B54`,
  `sub_8004B70`, `sub_8004BA0`, `sub_8004BD0`, `sub_8004C7C`

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_80014A4`** (`asm/code_3_1_7.s`, C in
  `src/graphics/fade_screen_mode.c`) - the fade-to-black palette DMA
  loop. This compiler's loop-invariant hoisting either caches nothing
  extra or, once any local variable represents the blended-buffer
  address, hoists it *and* at least one of the other two DMA fields -
  never the ROM's exact "cache only the buffer" split - see
  `docs/matching.md`, "The `0x080014A4`-`0x08001624` fade/screen-mode
  cluster, finally matched".
- **`sub_8001524`** (`asm/code_3_1_8.s`, C in
  `src/graphics/fade_screen_mode.c`) - sets a packed shadow byte's low
  3 bits. This compiler always folds the ROM's fresh `movs r1,#8;
  rsbs r1,r1,#0` mask computation into a `sub` derived from the
  already-loaded `7` mask - a value-propagation optimization no
  respelling or barrier defeated. Same doc section as above.
- **`sub_8001624`** (`asm/code_3_1_9.s`, C in
  `src/graphics/fade_screen_mode.c`) - commits a blend-register
  shadow. This compiler always fuses the ROM's separate
  store-then-pointer-increment into one `stmia` writeback instruction.
  Same doc section as above.
- **`sub_800B6A0`/`sub_800B6D0`** (`src/graphics/actor_part16.c`) -
  mirror-flag-gated 3-vector copies. This compiler unconditionally
  spills the `vec` pointer to a callee-saved register (`push
  {r4,lr}`/`pop {r4}`) whenever it's referenced in both branches of an
  if/else, even with nothing to clobber it - the ROM is a true leaf
  function using only r0-r3. Three independent fixes (pinning `self`
  alone; also pinning/reassigning `vec`; restructuring into a
  `goto`-based flow) all produced an identical 8-byte-larger result -
  see `docs/matching.md`, "A new unnamed object:
  `actor_part15.c`/`actor_part16.c`".
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
- **`sub_800944C`** (`src/graphics/actor_part11.c`) - the same
  "extended screen box" filter shape as `sub_8008C80`, but iterating
  the spatial hash grid directly and firing per-object trampolines
  instead of building a second array. Every load, store, and field
  offset confirmed correct; parked purely on a single register-reuse
  choice (`bucket = baseIdx + 2` computed in-place instead of into a
  fresh register) - see `docs/matching.md`, "Parked, not matched:
  `sub_800944C`".
- **`sub_8009528`** (`src/graphics/actor_part11.c`) - the spatial-
  hash-grid-cluster analog of `sub_8008A40`: the same grid-iteration
  shape as `sub_800944C`, dispatching each hit to `sub_80096C0`/
  `sub_80099F0` exactly like `sub_8008A40` dispatches to
  `sub_8008AD8`/`sub_8008D80`. Every branch, field offset, and call
  argument is semantically confirmed; parked on a stack-frame/register
  gap larger than the established `boxH` issue alone, not chased
  further given the size of the remaining cluster - see
  `docs/matching.md`, "Parked, not matched: `sub_8009528`".
- **`sub_80096C0`** (`src/graphics/actor_part11.c`) - `sub_8008AD8`'s
  twin: byte-identical collision-hit resolution logic, operating in
  this spatial-hash-grid cluster instead of the plain array manager.
  Parked on the same `boxH` stack-layout gap as `sub_8008AD8`/
  `sub_8008D80`/`sub_80099F0` - see `docs/matching.md`, "Parked, not
  matched: `sub_80096C0`".
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
- **`sub_801434C`** (`asm/code_3_2_17_1434c.s`, C in
  `src/graphics/actor_part18.c`) - the shared handler
  `sub_80142B0` tail-calls; one of the `gStaticData_0816BF20` action-
  table entries. Every load/store, branch and call confirmed correct,
  including the ROM's case-`0`/`2`-before-case-`1` switch layout and
  its shared `sub_803AD84` tail call; parked purely on instruction-
  *scheduling* for a handful of mutually-independent instructions in
  the closing `masked = *(u16 *)&snap & 0x180` block (right
  address/constant/load ordering, wrong relative order) - see
  `docs/matching.md`, issue #17, for everything tried.
- **`sub_80145E4`** (`asm/code_3_2_17_145e4.s`, C in
  `src/graphics/actor_part18b.c`) - same shape as the matched
  `sub_8014524` (boolean/raw-value bit test, `sub_8015780` reset
  block) but keeps the raw masked bit value rather than a `!= 0`-
  normalized boolean. Every load/store and branch confirmed correct;
  parked on a single materialize-then-copy gap in the opening bit-test
  triggered by a required nested `if` sharing the value's live range
  across both branches - see `docs/matching.md`, issue #17.
- **`sub_8003B40`**, **`sub_8003BDC`**, **`sub_8003C90`**,
  **`sub_8003D3C`**, **`sub_80041BC`**, **`sub_8004914`**,
  **`sub_80049CC`** (`asm/code_3_1_10_3.s`/`asm/code_3_1_10_4.s`/
  `asm/code_3_1_10_5.s`, C in `src/graphics/settings_menu.c`) - the
  composite pause/options screen's icon-manager centered-label draws
  (`sub_80049CC`/`sub_8003C90`/`sub_8003BDC`/`sub_8003D3C`/
  `sub_80041BC`/`sub_8004914`, all built on the same primitive
  `sub_8006600` above uses) plus a SIO-handshake spinner dialog
  (`sub_8003B40`). Every load/store, branch, and call is semantically
  confirmed for all seven; each hits the same class of gcc-2.9
  scratch-register nondeterminism `sub_8006600` documents at length
  (`sub_80049CC`/`sub_8003C90` come within one or two register-letter
  choices; `sub_8003BDC` additionally spills a constant through `ip`,
  which plain C can't request) - see `docs/matching.md`, "Match
  0x08003B40-0x08004CB4", for what was tried on each.
