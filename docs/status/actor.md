# Status: actor

The per-instance actor "self" object family - `struct actor` and its
many satellite files (`src/graphics/actor_part*.c`,
`src/graphics/actor_aabb_setup.c`). Filed under `src/graphics/` on disk
(the ROM's actor code lives interleaved with rendering code, and
several actor functions are themselves OAM/sprite-draw routines), but
tracked as its own `actor` category here since `docs/rom_map.md` and
the `decomp-chunk` issue generator both treat it as a distinct system
from "core" graphics.

## Matched

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

- `src/graphics/actor_part19.c`/`actor_part19c.c`/`actor_part19d.c`/
  `actor_part19f.c`/`actor_part19g.c` (new files, non-adjacent since
  three parked functions and one left-raw function sit between them -
  see `docs/matching.md`, issue #52): `sub_802BED8`, `sub_802BF30`,
  `sub_802BFA0`, `sub_802BFD4`, `sub_802C018`, `sub_802C078`,
  `sub_802C0A8`, `sub_802C0BC`, `sub_802C128`, `sub_802C14C`,
  `sub_802C19C`, `sub_802C264`, `sub_802C270`, `sub_802C394`,
  `sub_802C464`, `sub_802C4A4`, `sub_802C4C8`, `sub_802C540`,
  `sub_802C614`, `sub_802C6C0`, `sub_802C904` - the same large
  per-instance "self" object's action-table/trampoline/circular-list
  conventions as `actor_part17.c`/`actor_part18.c`

- `src/graphics/actor_aabb_setup.c` (new file, GitHub issue #70, ROM
  `0x0803AFDC`-`0x0803B060` - right after the parked division/modulo
  trio in `src/util/math_div_util.c`, see that file's `docs/matching.md`
  entry): `sub_803AFDC`/`sub_803AFE4` (the shared AABB set-size/
  set-position primitive already referenced by name from
  `actor_part.c`/`actor_part2.c`/`oam_count.c`), `sub_803AFEC` (a
  trivial raw-offset getter), `sub_803AFF0`/`sub_803B024` (two more
  `gStaticData_087E3BEC`-family per-type descriptor table constructors)

- `src/graphics/actor_part20.c`/`actor_part21.c`/`actor_part22.c`/
  `actor_part23.c`/`actor_part24.c`/`actor_part25.c`/`actor_part26.c`
  (new files, issue #58, ROM `0x08030334`-`0x08031784` - the boss-
  weapon effect state machine, non-adjacent since 18 raw functions sit
  between/around them; see
  [docs/matching/issue-58-0x08030334-actor.md](../matching/issue-58-0x08030334-actor.md)):
  `sub_8030530`, `sub_8030640`, `sub_80306A4`, `sub_8030C98`,
  `sub_80312C4`, `sub_803146C`, `sub_803171C`, `sub_8031744` - a
  countdown-timer state transition, a trivial byte setter/getter pair,
  a screen-accumulator/tracker-reset step, a BG2 zoom-effect updater,
  a "charge" countdown, and a palette flash/animation-refresh pair.
- `src/graphics/actor_part27.c` (new file, GitHub issue #22, ROM
  0x08017A44-0x08017AAC - numbered `27` rather than `20` since issue
  #58's parallel PR above independently claimed `actor_part20.c`-
  `actor_part26.c` first): `sub_8017A44`-`sub_8017AAC` (9 functions) -
  the same player/action-object family as `actor_part18.c`/
  `actor_part19.c` (`self+0xc` table pointer, `self+0x10` part
  pointer); see `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/graphics/actor_part27b.c` (new file, GitHub issue #22, ROM
  0x08017ECC-0x08017FE8, non-adjacent to `actor_part27.c` since the
  raw `sub_8017AB0` sits between them): `sub_8017ECC`, `sub_8017F14`,
  `sub_8017F5C`, `sub_8017F80`, `sub_8017FA4`, `sub_8017FD4`,
  `sub_8017FE8` - a `self+4` double-pointer-chain record lookup (same
  shape as `sub_800B704`/`sub_800B838`) feeding the
  `gStaticData_0816C2D8` per-vector-component trampoline table; see
  `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/graphics/actor_part27c.c` (new file, GitHub issue #22, ROM
  0x080187FC-0x08018884, non-adjacent to `actor_part27b.c` since the
  raw `sub_8018008`-`sub_80186F0` block sits between them):
  `sub_80187FC`, `sub_8018858`, `sub_801886C`, `sub_8018884`; see
  `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/graphics/actor_part28.c` (new file, GitHub issue #18, ROM
  0x08014F8C): `sub_8014F8C` - a `gUnknown_030012F0`-list proximity-
  trigger scan for the same "self" action-table object family as
  `actor_part18.c`; see `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part28b.c` (new file, GitHub issue #18, ROM
  0x080151C8, non-adjacent to `actor_part28.c` since the parked
  `sub_8015038` sits raw between them): `sub_80151C8`; see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part28c.c` (new file, GitHub issue #18, ROM
  0x08015350-0x080156B4, non-adjacent to `actor_part28b.c` since the
  parked `sub_8015238`/`sub_80152F0` sit raw between them):
  `sub_8015350`, `sub_8015398`, `sub_80153FC`, `sub_8015460`,
  `sub_8015508`, `sub_8015558`, `sub_80155A8`, `sub_80155AC`,
  `sub_80155B8`, `sub_80155F8`, `sub_8015650`, `sub_8015690`,
  `sub_80156B4` - more of the same self+0xc/self+0x10 trampoline-pair
  family, including two near-identical self+0x29-keyed mgr-trampoline
  arms (`sub_8015460`) and several part+0x38-gated trampoline firers;
  see `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part28d.c` (new file, GitHub issue #18, ROM
  0x0801574C-0x08015780, non-adjacent to `actor_part28c.c` since the
  parked `sub_80156EC` sits raw between them): `nullsub_17`,
  `sub_8015750`, `nullsub_18`, `sub_8015774`, `sub_8015780` - two
  nullsubs, two tail-call wrappers, and the shared trampoline-pair-
  plus-sentinel-store helper called by `actor_part18.c`'s
  `sub_801426C`/`sub_80142B0`; see
  `docs/matching/issue-18-0x08014f8c-actor.md`.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

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
- **`sub_802C208`** (`asm/code_3_2_20_28568_c208.s`, C in
  `src/graphics/actor_part19e.c`) - a `gStaticData_0817A6B8` stride-8
  trampoline-record dispatcher. Every load/store, branch and call
  confirmed correct; parked on register-allocation/instruction-
  scheduling around two `record = base + state*8` re-derivations - see
  `docs/matching.md`, issue #52.
- **`sub_802C2FC`** (`asm/code_3_2_20_28568_c2fc.s`, C in
  `src/graphics/actor_part19b.c`) - OAM setup for one sprite frame.
  Matches instruction-for-instruction except a single dead `flag = 0`
  initializer this compiler's dead-store elimination always removes -
  see `docs/matching.md`, issue #52.
- **`sub_802C3E8`** (`asm/code_3_2_20_28568_c3e8.s`, C in
  `src/graphics/actor_part19c2.c`) - a homing/seek-toward-point spawn-
  effect constructor. Every field access and call confirmed correct;
  parked on this compiler's register choice for a couple of
  intermediate abs-value-computation values - see `docs/matching.md`,
  issue #52.
- **`sub_8015038`** (`asm/code_3_2_17_15038.s`, C in
  `src/graphics/actor_part28.c`) - a three-arm mgr-trampoline handler
  keyed on `self+0x24`/`self+0x22`, picking one of three table-index
  fallbacks. Every load/store, branch and call is understood and
  semantically correct; parked on this compiler's register allocation
  across the three near-identical arms (it won't keep the computed
  `self+0x21`/`self+0x22` field addresses in the ROM's own `r7`/`r5`
  once real trampoline calls intervene) - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_8015238`** (`asm/code_3_2_17_15238.s`, C in
  `src/graphics/actor_part28b.c`) - `self+0x26`/`mode`/`flags`-gated
  mgr-trampoline dispatcher. Every load/store, branch and call is
  correct, in the right order, and in the right registers - parked
  purely on the two parameter home-copies at function entry (this
  compiler always truncates `mode` before copying `self`, the ROM does
  the opposite, and neither order nor register pins nor hand-written
  `asm volatile` copies could override the compiler's own fixed
  parameter-home-copy prologue pass) - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80152F0`** (`asm/code_3_2_17_15238.s`, C in
  `src/graphics/actor_part28b.c`) - `self+0x27`/`self+0x2b`/`mode`-
  gated state/counter/table-index trio reset, tail-calling
  `sub_80122CC`. Every load/store, branch and call confirmed correct
  and in the right order; parked on a single instruction (a `+6` byte
  offset folds into a `strb`'s own addressing mode where the ROM keeps
  it as a separate `adds`) - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80156EC`** (`asm/code_3_2_17_156ec.s`, C in
  `src/graphics/actor_part28c.c`) - `part+0x38`/`sub_80231BC`-gated
  mgr-trampoline dispatcher. Every load/store, branch and call
  confirmed correct; parked on the `else` arm recomputing `self` into
  a fresh register (an extra push/pop this compiler insists on once
  its own `mgr` local is redeclared in that arm) where the ROM reuses
  the same `self` register the whole function already lives in - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80157C4`** (`asm/code_3_2_17_157c4.s`, C in
  `src/graphics/actor_part28d.c`) - player's `+0x100`-flag-gated
  `mode` remapper (a 3-way dispatch playing a fixed cue via
  `sub_80019A8`/`PlaySfx`), tail-calling `sub_800B86C`. Every load/
  store, branch and call is understood and semantically correct;
  parked on register allocation across the 3-way dispatch - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.

## Left raw (not attempted, or attempted and set aside)

- **`sub_8017AB0`** (`asm/code_3_2_17_17ab0.s`, ROM 0x08017AB0-
  0x08017ECC, GitHub issue #22) - a ~500-instruction player-vs-camera-
  viewport state dispatcher; left raw, out of scope for this pass - see
  `docs/matching/issue-22-0x08017a44-actor.md`.
- **`sub_8018008`/`sub_8018400`/`sub_801865C`/`sub_80186F0`**
  (`asm/code_3_2_17_18008.s`, ROM 0x08018008-0x080186F0, GitHub issue
  #22) - a ~480-instruction jump-table player action-state machine
  plus two high-register-pressure helpers it calls; left raw, out of
  scope for this pass - see `docs/matching/issue-22-0x08017a44-actor.md`.
