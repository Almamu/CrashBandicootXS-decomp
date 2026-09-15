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
  `sub_8008394`

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
