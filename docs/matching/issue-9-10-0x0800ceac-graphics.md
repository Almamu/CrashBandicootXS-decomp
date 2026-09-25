# Issues #9/#10 follow-up: `sub_800CEAC`/`sub_800CF70` (graphics -> game_loop)

Dedicated deep-investigation session against the two functions
`tools/report_units.py` tracked as parked (`base_object=None`) at
`0x0800CEAC`-`0x0800D040` (404 bytes total), the last still-raw gap
between the just-closed `sub_800CD00` (issue #9/#10,
[issue-9-10-0x0800aaec-graphics.md](./issue-9-10-0x0800aaec-graphics.md))
and the already-matched `sub_800D040` (issue #12,
[issue-12-physics-collision.md](./issue-12-physics-collision.md)). The
real bytes lived in `asm/code_3_2_17_ceac.s` (now deleted - fully
consumed by this session's work).

## Starting point

`docs/rom_map.md` (line ~2137) already had a partial note on one of the
two: **`sub_800CF70`**, sitting 144 bytes before the physics/collision
subsystem's *stated* `0x0800D000` start, "calls the same linked-list
walkers that subsystem uses and reaches the same 28-byte-record chain -
functionally part of it despite sitting just outside the documented
boundary." Its sibling, **`sub_800CEAC`** (the first 196 of the 404
bytes), had no note anywhere. Both are called *only* from
`sub_0800D18C` (`asm/code_3_2_17_d18c.s`), the physics/collision
subsystem's ~1960-byte collision-response commit function documented
at length in `issue-12-physics-collision.md`.

## Reading the real bytes

`asm/code_3_2_17_ceac.s` held exactly these two functions, nothing
else (`thumb_func_start sub_800CEAC` at line 6, `thumb_func_start
sub_800CF70` at line 107, 212 lines total). Both call sites in
`sub_0800D18C` were also read in context (`asm/code_3_2_17_d18c.s`
around its own AABB-build blocks) to recover the caller's argument
setup, since neither function's own body makes its argument roles
obvious in isolation.

### `sub_800CEAC(void *self, struct hitbox_quad *quad, struct aabb *box, s32 xOffset, s32 yOffset)`

Called once, from `sub_0800D18C`, with:
- `self` = `sl` (the collision-response commit's own subject) - loaded
  into a callee-saved register by the prologue but **never read again**
  after that; confirmed genuinely unused against the raw disassembly,
  not a transcription slip.
- `quad` = the player's (`gUnknown_030012D8`) own hitbox quad: `player's
  own +0x20 table[player's own +0x2d tag] + 4` - i.e. a pointer straight
  to the `{s16 xOff, s16 yOff, u8 w, u8 h}` quad, not the 28-byte
  record's own base (the caller has already added the `+4`).
- `box` = `self`'s own already-built AABB, constructed at the very top
  of `sub_0800D18C` from `self`'s own `+0x20` table - the same "AABB1"
  shape `sub_800D040`'s header documents.
- `xOffset`/`yOffset` = `self.x>>8`/`self.y>>8`, cached by the caller
  early on and reused across many of its own AABB builds.

Body:
1. Tests `gUnknown_030012D8`-the-player's own `+0x90` byte (dereferences
   the global pointer, then reads `+0x90`). This offset isn't documented
   anywhere else under this project's naming - the closest confirmed
   neighbors are `+0x92`/`+0x94`, a state-byte pair `sub_8015350`
   clears together (`docs/matching/issue-18-0x08014f8c-actor.md`), and
   `+0x94`/`+0x98` is the physics subsystem's own 5-slot "recently
   touched" ring buffer inside the player object
   (`issue-12-physics-collision.md`). `+0x90` stays an unconfirmed
   player state/mode byte here - named `field_90`/prose-only, not
   guessed at semantically beyond "toggles a wider hitbox variant"
   below.
2. Builds a local AABB from `quad`, positioned at `quad->xOff +
   xOffset`, `quad->yOff + yOffset`, sized `quad->w`, `quad->h` - **or**,
   if the player's `+0x90` byte is nonzero, positioned 2 pixels further
   left (`x - 2`) and sized `quad->w + 4` (a box widened symmetrically
   by 2 on each side, "wide mode"). `y`/`h` are unaffected either way.
3. Mirrors that local AABB horizontally/vertically around `(xOffset,
   yOffset)` according to the **player's own** `+0x28` flags (bits 4/5 -
   the same mirror-flag convention `sub_800D040`/`actor_part16.c`/
   `actor_part17.c` already establish, just keyed off the player's flags
   here since the box represents the player's shape, not `self`'s).
4. Tests the mirrored local AABB against `box` (`self`'s own real AABB)
   via `sub_8001688` (the strict/non-touching overlap variant) and
   returns the boolean result.

Read together: "would a player-shaped hitbox, standing where `self`
currently is (optionally in its wider variant), overlap `self`'s own
actual hitbox". The caller (`sub_0800D18C`) uses a `1` result to trigger
a further `sub_801070C` ("get next") list-walk step - consistent with a
"can something player-sized occupy this spot" gate feeding further
traversal, e.g. deciding whether `self` currently blocks the space a
player-sized object would need there.

### `sub_800CF70(void *self, struct aabb *box, u8 *foundFlag)`

Called from `sub_0800D18C` only while its own 5-slot ring-buffer index
counter is `<= 4` (confirmed at the call site: `cmp r3,#4; bgt` skips
the call entirely and substitutes `self` directly when the counter
exceeds 4) - `self` = the caller's own subject, `box` = the player's own
AABB the caller built just before the call, `foundFlag` = a byte buffer
the caller pre-loads with its own current edge-code value.

Body:
1. `next = sub_801070C(self)` ("get next"), `prev = sub_8010708(self)`
   ("get prev") - the established doubly-linked neighbor-list accessor
   pair (`src/system/game_loop7.c`'s own header comment; also used by
   `sub_0800D18C` itself and `sub_800E494`/`sub_800E4E4`).
2. If **both** are `NULL`: return `self` unchanged, no other side
   effect - `self` is isolated in the list.
3. Otherwise (at least one neighbor exists): `*foundFlag = 1`,
   overwriting whatever edge-code the caller pre-loaded there.
4. If `prev` is `NULL`, or `prev`'s own `+0x4d & 0x7f` state byte reads
   `1` (the exact early-out gate `sub_800D040`'s own header documents -
   objects in this state are excluded from the subsystem's AABB tests
   entirely), return `self` unchanged.
5. Otherwise build `prev`'s own AABB from the shared `+0x20`-table
   convention (indexed by `prev`'s own `+0x2d` tag, quad at record `+4`,
   offset by `prev.x>>8`/`prev.y>>8`, mirrored per `prev`'s own `+0x28`
   flags) - the exact "AABB1" shape again, just for `prev` instead of
   `self`. Test it against `box` via `sub_8001688`; on overlap, return
   `prev` instead of `self`.
6. Return whichever of `self`/`prev` was selected.

This confirms `docs/rom_map.md`'s existing note in the strongest sense:
the entire function body *is* one AABB-build-and-overlap-test cycle from
the physics/collision subsystem, using its own table convention and its
own `+0x4d` exclusion gate, operating specifically on the "prev"
neighbor - not a loose family resemblance, a direct reuse of the
subsystem's own primitives one step outside its stated address range.

## Category correction

Both functions are recategorized `graphics` -> `game_loop`, the same
correction issue #12 already made for the adjacent `sub_800D040`: their
only caller (`sub_0800D18C`) is squarely inside the confirmed
physics/collision subsystem inside the `game_loop` zone, not
entity-specific `graphics` behavior. Unlike `sub_800CD00` (which stayed
`graphics` since its caller `sub_800AAEC` is itself an entity
action-dispatch gate), these two have no caller outside the subsystem.

## Matching: both NAKED transcription, not real C

Neither was attempted as a plain-C reconstruction. Both are the same
single-inlined-AABB-build primitive `sub_8007B98`
(`src/graphics/actor_part.c`) already documents as resistant to gcc
2.9's register allocation *even in its simplest, unbranched, single-call
form* ("about 10 of ~73 instructions... which anonymous scratch
register" gaps) - a shape this project has now independently hit and
NAKED-transcribed four times (`sub_8007B98` itself, `sub_800D040`'s two
inlined copies, `sub_800CD00`'s three inlined copies). Both of this
session's functions compound that established-resistant core further
rather than simplifying it:

- `sub_800CEAC` adds an extra branch (the player `+0x90` test) selecting
  between two slightly different operand sequences before the shared
  mirror/overlap tail - a genuinely different C-level shape (an `if`)
  layered on top of a build primitive already known not to survive gcc
  2.9 register allocation in its plain form.
- `sub_800CF70` stacks the AABB-build primitive on top of a
  `sub_801070C`/`sub_8010708` neighbor-list read - and that *simpler*
  shape (list read with no AABB build at all) is itself independently
  documented as resistant for `sub_800E494`/`sub_800E4E4`
  (`issue-12-physics-collision.md`: a `0x7f`-mask-before-`ldrb`-load
  instruction-scheduling order this compiler never reproduces from any
  C-level operand-order/negation phrasing tried). Two independently
  confirmed-resistant shapes stacked in one function was not a good use
  of remaining session time to re-litigate from scratch.

Per this project's established recognition rule for this exact
neighborhood (see the `sub_800D040`/`sub_800CD00`/`sub_800E494`/
`sub_800E4E4` precedent above), both were transcribed directly as
byte-exact NAKED asm instead: the ROM disassembly translated
instruction-for-instruction, unified-syntax mnemonics converted to this
project's plain/divided-syntax NAKED convention (`adds`->`add`,
`movs`->`mov`, `ands`->`and`, `lsls`/`lsrs`->`lsl`/`lsr`,
`asrs`->`asr`), with the original `_08XXXXXX:` labels renumbered to GNU-as
local numeric labels.

Verified byte-exact via the isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pipeline against `baserom.gba`'s own bytes at
`0x0800CEAC`-`0x0800D040`: the only differing bytes fell into exactly 12
four-byte clusters, matching the expected relocation-site count exactly
(7 for `sub_800CEAC` - 5 `bl` calls + 2 `.word gUnknown_030012D8`
literals; 5 for `sub_800CF70` - 5 `bl` calls, no literal pool needed
since it never touches the player global). These resolve correctly once
linked, the same pattern every prior NAKED closure in this neighborhood
has shown.

New file `src/system/game_loop42.c` (both functions - they're
ROM-contiguous with each other, `0x0800CEAC`-`0x0800D040`, so one file
per `docs/workflow.md`'s "one `.c` file per contiguous ROM region"
rule), inserted in `ldscript.txt` exactly where `asm/code_3_2_17_ceac.o`
used to sit (between `src/graphics/actor_part109.o` and
`src/system/game_loop6.o`). `asm/code_3_2_17_ceac.s` deleted (fully
consumed, nothing else was in it).

## Full-ROM verification

`rm -rf build && make NON_MATCHING=1 report` - clean, no warnings from
the new file. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide` (checksum matches).

## Techniques used

- NAKED transcription as the deliberate, documented default given two
  independently-confirmed-resistant shapes (single-inlined-AABB-build,
  and this specific `0x7f`-mask/list-walk scheduling gap) stacked inside
  the same two functions - recognizing the established pattern rather
  than re-deriving the same negative result from scratch, per this
  project's own stated convention for this exact neighborhood.
- Cross-referencing the caller (`sub_0800D18C`)'s own argument-setup
  code to recover each function's parameter roles, since neither
  function's own body makes them obvious in isolation (`sub_800CEAC`'s
  first argument in particular is a dead parameter with no in-body
  read at all).
- Relocation-site-count cross-check (`cmp -l` byte-diff clustering) as a
  cheap, precise way to confirm an isolated-compile NAKED transcription
  is byte-exact modulo only `bl`/`.word` relocations, without needing a
  full link.

## Cross-references

- `docs/status/game_loop.md` - `sub_800CEAC`/`sub_800CF70` added to the
  "Parked - NAKED transcription" list.
- `tools/report_units.py` - the `0x0800CEAC` unit now points at
  `src/system/game_loop42.o`, category `game_loop` (was `None`/
  `graphics`).
- `docs/rom_map.md` - new "Follow-up: `sub_800CF70`'s partial note
  confirmed, `sub_800CEAC` found and both closed" section, appended
  after the original partial note (append-only convention - the
  original note is left untouched).
- `docs/matching/issue-12-physics-collision.md` - the
  `sub_800D040`/`sub_800CD00`/`sub_800E494`/`sub_800E4E4` precedent this
  session's NAKED-transcription judgment call is based on.
