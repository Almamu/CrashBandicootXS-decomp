# `sub_800B8DC`/`sub_800BD48`: foundational semantic map for the 0x0800B8DC-0x0800D040 cluster (issue #9/#10)

Dedicated deep-investigation session against the two functions opening
the large, fully-raw `0x0800B8DC`-`0x0800D040` cluster (43 functions,
~5988 bytes, sitting right after `actor_part17.c`'s span and ending at
the already-documented physics/collision subsystem's own boundary,
`sub_800D040`/`game_loop6.c`). No existing doc described what any of
this cluster's functions do; `docs/rom_map.md`'s only prior note on
`sub_800B8DC` was a one-paragraph "18-entry jump table, `self+0x74` as
the state selector, cases 0-17" sketch from an earlier connectivity
pass. This session cracks both functions open in full and produces the
case-by-case map the cluster's remaining ~41 functions can be scoped
against.

## Bounds and vtable status

- **`sub_800B8DC`**: ROM `0x0800B8DC`-`0x0800BD48`, 1132 bytes.
- **`sub_800BD48`**: ROM `0x0800BD48`-`0x0800BFA8`, 608 bytes (starts
  the instant `sub_800B8DC` ends - no gap, no alignment padding between
  them).

Both are found (by searching `baserom.gba` for their own thumb-bit-set
addresses as raw `.4byte` pointer values) inside the already-documented
93-entry `gStaticData_087Exxx` "entity descriptor" family
(`docs/rom_map.md`'s "Found it: the 93-entry... family is 93 vtables"
section) - each record is a `{0, ptr}`-pair vtable, one per
placeable-object type, with up to 15 behavior slots. Specifically:

- `sub_800B8DC`'s address appears **once**, at `gStaticData_087E3EE4+0xC`.
- `sub_800BD48`'s address appears **twice**: at
  `gStaticData_087E3EE4+0x14` (the *same* vtable record as
  `sub_800B8DC`, two slots later - `+0x10` is a null/unused slot in
  between) and again at `0x087E4850` (a different vtable record,
  reusing the function the same way the whole 93-vtable family reuses
  its ~319 distinct implementations across records).

**This sharpens `docs/rom_map.md`'s existing "both vtable-dispatched"
note**: `sub_800B8DC` and `sub_800BD48` aren't just two unrelated
slots that happen to sit next to each other in ROM address order -
they're two *different behavior-slot indices of the same object
type's vtable* (`gStaticData_087E3EE4`), i.e. two callback hooks
(`update`-shaped vs. an event/message hook, see below) for the *same*
placeable-object type. They are **not** caller/callee of one another
though - confirmed by reading `sub_800B8DC`'s full body: there is no
`bl sub_800BD48` anywhere in it, nor does `sub_800BD48` call
`sub_800B8DC`. `sub_800B8DC`'s own vtable-owning type is very likely
the same "player (or generic dynamic-actor) state machine" candidate
`docs/rom_map.md`'s original one-paragraph note already guessed from
the shape alone (jump velocity impulse in state 17, chase/homing
routines called from several states) - not confirmed to literally be
the player, but a physics-driven dynamic actor of some kind.

## `self` vs. `self+0x70` ("owner"): two instances of one shared struct

Both functions take `self` as their first argument and, for nearly
every field access, immediately load a second pointer from
**`self+0x70`** (called `owner` throughout this doc and in the source
comments) and operate on fields of *that* object instead. `self` and
`owner` are almost certainly **two instances of the same larger
struct**, not unrelated types - they share several field offsets with
identical apparent roles:

| Offset | Role (seen on both `self` and `owner`) |
|---|---|
| `0x00`/`0x04` | Q8.8 position (owner only - `self` itself is never read at 0/4 in either function) |
| `0x0C` | flags byte - bit 0 set by the "flag active + bitmap-set" idiom (see below); bit 4 read/written in several `self+0x68`-dispatch siblings |
| `0x28` | flags byte - bit 4 is the established mirror-flag convention (`actor_part16.c`/`actor_part17.c`/`game_loop6.c`); bits 5-6 (`0x1a`/`0x1b` shift-tests) gate several branches |
| `0x2D` | table row index, paired with a `+0x20` table pointer at 28-byte stride (`sub_800D040`'s own "keyframe/hitbox record" convention) |
| `0x30`/`0x34` | a "blocking condition" pair - compared against small magic constants (`8`, `9`, `0xA`) and `0`; non-zero gates several states off entirely |
| `0x38` | an "enabled"/"active" byte gating most `self+0x68`-dispatch siblings' real work |
| `0x48`/`0x4C`/`0x50` and `0x54`/`0x58`/`0x5C` | X-axis and Y-axis "velocity-target" triples respectively - written by `sub_800C18C` (X) / `sub_800C1E8` (Y), the same `self+0x60`/`+0x48`/`+0x4C`/`+0x50` "directional-target field" layout `docs/rom_map.md` already ties to `sub_801B304`/`sub_80159F8`/`sub_800BD48` |
| `0x60`/`0x64` | current velocity/position-delta pair |
| `0x68` | a small-integer sub-state byte - the *exact* field `sub_800C40C`'s own independent 6-case dispatcher already uses (`docs/rom_map.md`: "the *exact* field offset `sub_800B8DC`'s 18-state... machine also uses") |
| `0x6C` | a second, larger-range state/anim-id byte (values seen: `0xF`, `0x12`, `0x17`, `0x1A`, `0x1E`, `0x21`...) |

`self` additionally has its own `0x74` (this function's own 18-state
selector), `0x10`/`0x14`/`0x18`/`0x1C` (X/Y homing bounds consumed by
`sub_800C18C`/`sub_800C1E8`, not read directly by `sub_800B8DC` itself),
`0xC` (a pointer to a *third* struct - see below), `0x80` (a frame/lock
flag, bit 0 tested by `sub_800C314`), `0x84` (a pointer to a small
record with `+0xC`/`+0x14` bytes compared to `8`, read by
`sub_800C40C` - looks like a surface/material-type lookup feeding
footstep/impact SFX selection) and `0x88` (a pointer to a spawned
floating-text/popup child object, set/cleared by state 18 - same
`+0xC` flags-byte / `+8` bitmap-id idiom as `owner`).

**`self+0xC`** is a pointer to a *third* record - a "directional-target
anchor" struct with **multiple** `{s16 offset, void *table}` pairs at
different byte offsets, each apparently feeding a different call to
`sub_803AD80`/`sub_803AD88` depending on which direction/event is being
triggered: `sub_800B8DC`'s own state 11 reads the pair at `+0x10`/`+0x14`
(`sub_803AD88`); `sub_800BD48`'s "spawn/launch" handler (states 19-20)
reads the pair at `+0x48`/`+0x4C` (`sub_803AD80`). This is the same
per-record convention as `owner`'s own `0x48`-`0x5C` velocity-target
triple, just on a separate/shared object rather than `owner` itself -
not resolved further here (a good next-phase target: nail down how many
of these pairs the anchor record actually has and what selects between
them).

None of this is resolved into a named struct in the matched C below -
deliberately: this pass treats the case *bodies* as opaque per the
brief's own scope, and committing full field names this early (with
several fields' exact semantics still genuinely uncertain, e.g. `0x6C`,
`self+0xC`'s exact layout) risks the next phase inheriting wrong names.
Both functions instead use raw NAKED asm (no C field-access casts at
all - see "Matching" below), and this table is what the next phase
should build any struct definitions from.

### A widely-reused "flag active + bitmap-set" idiom

Both functions (and `sub_800B8DC`'s own state 5, and several already-
matched siblings elsewhere in the ROM) repeat the exact same four-step
idiom on some object `X`: `X->0xC |= 1`; if `X->8` (a `u16`, `0xFFFF` =
"none" sentinel) `!= 0xFFFF`, then treat `X->8` as a bit index into the
`gUnknown_030012B4` bitmap array (`word = X->8 >> 5`, `bit = X->8 & 0x1F`,
`gUnknown_030012B4[0x108/4 + word] |= 1 << bit`). Reads as "mark this
object's collision/proximity bucket active" - `gUnknown_030012B4` is
the same bitmap several other matched functions in this ROM region
already tie to the hardware window-register system
(`sub_8022BF0`/`sub_8022CA0`, `docs/rom_map.md`).

## `sub_800B8DC`: the 18-case dispatch map

`self+0x74` is the state selector - **1-18 are real states, `0` or
`> 18` is a silent no-op** (the ROM computes `state - 1`, range-checks
it `<= 17` unsigned, and jumps through an 18-entry table; out-of-range
falls straight to the function epilogue). This is the *same*
comparison/field shape `sub_800C6A8` (a `menu_ui` dialog-widget update,
called by all 31 of that system's own dispatch-table entries) and
`sub_800CD00` (`actor_part109.c`, `docs/rom_map.md`) already use - a
general-purpose "18-state stateful widget" convention reused across
several *different* object types, not proof this is the same struct as
those.

| State | Target (ROM addr) | Behavior |
|---|---|---|
| 1 | `0x0800BD3A` (epilogue) | **No-op.** Falls straight through to the shared return - identical to the out-of-range default. |
| 2 | `0x0800BC48` | `sub_800C074(self)` |
| 3 | `0x0800BC40` | `sub_800C5D4(self)` |
| 4 | `0x0800BC56` | `sub_800C40C(self)` |
| 5 | `0x0800BA10` | **Inline.** Distance-band gate: compares `owner->4` (Y) against two thresholds derived from `gUnknown_03001308`'s own nested `+0x10`/`+0x14` value (a "lazy singleton" object `docs/rom_map.md` ties to a text-box/dialog system elsewhere - plausibly reused here just for its numeric value, not its text-box role). Near band: zeroes or sets `owner`'s `0x48`-`0x5C`/`0x64` velocity-target fields depending on `owner+0x68 == 8`. Far band: flips `owner->0xC` bits 0/1 and, if past the second threshold, runs the "flag active + bitmap-set" idiom on `owner`. |
| 6 | `0x0800BC6C` | `sub_800C940(self)` |
| 7 | `0x0800BC74` | `sub_800C314(self)` |
| 8 | `0x0800BC7C` | `sub_800C244(self)` |
| 9 | `0x0800BC84` | **Inline.** First-time-only caches `owner->0`/`owner->4` into globals `gUnknown_030012A0`/`gUnknown_030012A8` (guarded by one-shot flags `gUnknown_030012A4`/`gUnknown_030012AC`), calls `sub_800C18C`+`sub_800C8F8`, then **unconditionally** re-syncs `gUnknown_030012A0`/`A8` from `owner`'s *current* position regardless of the guard - the guard only affects the *first* write, every call after still updates the globals at the end. Reads as caching an "original anchor position" once, then continuously publishing the live position too - possibly a camera-anchor/save-restore pair (same "guard only matters once, real work happens every call" shape `docs/rom_map.md` flagged as a possible dead-store oddity in `sub_800CA60`). |
| 10 | `0x0800BD26` | `sub_800C1E8(self)` |
| 11 | `0x0800BCD8` | **Inline.** `sub_800C18C(self)` + `sub_800C1E8(self)`, then only if `owner->0xC` bit 3 is set **and** `self->0x6C == 6`: reads `self+0xC`'s anchor record's `+0x10`/`+0x14` pair, calls `sub_803AD88(self + offset, 0, 1, 0)`, then `PlaySfx(ctx, 4, 0x100)`. |
| 12 | `0x0800BD3A` (epilogue) | **No-op**, same as state 1. |
| 13 | `0x0800BC50` -> falls into state 4's own code | `sub_800C074(self)`; `sub_800C40C(self)` |
| 14 | `0x0800BC5E` | `sub_800C40C(self)`; `sub_800C97C(self)` |
| 15 | `0x0800BD20` -> falls into state 10's own code | `sub_800C074(self)`; `sub_800C1E8(self)` |
| 16 | `0x0800BD2E` | `sub_800C40C(self)`; `sub_800BFA8(self)` |
| 17 | `0x0800B948` | **Inline, largest block (~200 B).** A "landing/hit" handler: if `owner->4 >= self->0x64`, either nudges via `sub_800C8AC(self,0)`, falls back to `sub_800C5D4(self)`, or - when `owner->0x60 < 0` and a collision probe via `sub_803AD7C(owner + hitboxOffsetY, hitbox->0x2C)` reports no hit - applies a **fixed upward Q8.8 impulse**: `owner->0 = self->0x60`, `owner->4 = self->0x64 - 25600` (i.e. `self->0x64 - 100.0` in Q8.8 - a jump-impulse shape), then sets `owner`'s `0x64`/`0x54`/`0x5C` to `0x80` and `0x58` to the probe result, plus `owner->0xC` bit 4. Every path then falls into a **shared tail** (`0x0800B9D2`-`0x0800BA0C`, exclusive to this state): if `owner->0x30` or `owner->0x34` is non-zero, return; otherwise run a second `sub_803AD7C` probe on the same hitbox and, if it reports a hit, `PlaySfx(ctx, 0x13, 0x100)`. |
| 18 | `0x0800BAB2` | **Inline, 2nd-largest block (~400 B).** Computes `max(|ownerX - cameraX|, |ownerY - cameraY|)` against `gUnknown_030012D8` (the player/camera pointer), clamps to `[0x20, 0xA0]`, and derives a volume (`0x100 - (clamped-0x20)*2`) for a **distance-scaled ambient sound**: `sub_80019F8(ctx, 0x2B, 8, volume)`. If `owner->0x38` and `self->0x68 == 3`: spawns/updates a floating popup object via `sub_800C9C8(0x1D, 0, 0, 0x2B, 0, owner)` into `self->0x88`, tags it, and `PlaySfx(ctx, 0x12, 0x100)`. Else if `owner->0x38` and `self->0x68 == 5`: runs the "flag active + bitmap-set" idiom on `self->0x88`'s object (if set) then clears `self->0x88`. Always calls `sub_800C074(self)`+`sub_800C40C(self)`. Then, on `self->0x68 == 1` or `== 6`, sets `owner->0x28`'s mirror-flag bit from `owner`'s own X-sign-flag test and calls a `sub_800C8CC`/`sub_800C8BC` pair (state 6's variant additionally re-derives `owner->0x30` from a keyframe-record byte). Tail: if `self->0x88` is non-null, copies `owner->0` into it. Reads overall as a **"proximity growl/warning + optional floating hint text"** state. |

**No-op states worth flagging for the next phase**: states 1 and 12
both compile to literally nothing (they share the exact same jump-table
target as the out-of-range default) - if the next phase finds callers
that explicitly *set* `self+0x74` to 1 or 12, that's evidence those
values are meaningful *elsewhere* (e.g. "idle, waiting to be
retriggered") even though this function itself does nothing for them.

**Grouping for the next phase**: states {2,3,4,6,7,8,10} are pure
single-callee delegations (`sub_800C074`/`sub_800C5D4`/`sub_800C40C`/
`sub_800C940`/`sub_800C314`/`sub_800C244`/`sub_800C1E8`) - understanding
any one of those callees fully explains that state. States {13,14,15,16}
are two-callee combos of the same seven functions. States {5,9,11,17,18}
are the only ones with real inline logic in `sub_800B8DC` itself (already
captured above) - `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC` (all
`(self, mode)` shaped, modes seen: 0,1,2,3,4,6,7) are the most-called
opaque callees across those five states and are the single best next
target to unlock the rest of this state machine's meaning.

## `sub_800BD48`: a second, independent 22-case dispatch

Signature: `sub_800BD48(void *self, s32 unused, s32 state)` - `arg1`
(`r1`) is never touched by the function at all (dead parameter, or an
argument some *other* vtable slot with the same call-site shape uses
and this one ignores). `state` (`r2`/`arg2`, **not** the same field as
`sub_800B8DC`'s `self+0x74`) selects 1-22 through the function's own
independent jump table.

**Unconditional prelude** (runs before the switch, every call): if
`gUnknown_030012D8`'s own object's `+0x88` byte `== 1`, runs the "flag
active + bitmap-set" idiom on `owner` (`self+0x70`). Otherwise, if
`self+0x88`'s own pointer is non-null, runs the *same* idiom on
*that* object instead. This is a **generic per-call housekeeping step**,
independent of `state` - the next phase should *not* read it as part of
any specific case's behavior.

| `state` | Target | Behavior |
|---|---|---|
| 1 | `0x0800BF2C` | Ambient-sound-and-flag tail (see below) |
| 2-18 | `0x0800BF94` (epilogue) | **No-op** - 17 of the 22 declared states do nothing. |
| 19-20 | `0x0800BE80` | Spawn-and-launch handler (see below) |
| 21-22 | `0x0800BF2C` | Same as state 1 |

**`0x0800BF2C` ("ambient sound + re-flag" tail, states 1/21/22):**
converts `owner`'s Q8.8 position to int and calls
`sub_8025BAC(gUnknown_030012E4, 0x29, 2, ownerX, ownerY, 0)` - the same
`sub_8025BAC(pool, id, kind, x, y, ...)` shape already matched in
`actor_part2.c`, here spawning something of kind `2`/id `0x29` at
`owner`'s position (a particle or ambient-audio-emitter object, per the
global's name and the "queues something via `sub_8025BAC`" precedent
`docs/rom_map.md` already notes for a *different* id/kind pair
elsewhere). Clears bits `{0,2}` of the returned object's `+0xC`, sets
bit 0 (clearing bit 2) of its `+0x28`, then re-runs the "flag active +
bitmap-set" idiom on `owner` again.

**`0x0800BE80` ("spawn and launch a child object", states 19-20):**
allocates a `0x10`-byte object (`sub_8026EDC`), passes it straight
through to `sub_800CBD4()` (no other args set - the allocation's own
pointer is still live in `r0`), stores the result into `owner->0x44`,
then reads that new child's own `+0xC`-pointed record's `+0x18`/`+0x1C`
pair and calls `sub_803AD80` with it (the directional-target-table
trigger convention again). Clears `owner->0xC` bit 7. Compares
`owner->0` against `gUnknown_030012D8`'s own X position and picks one
of two **opposite-signed** velocity-target constant sets for
`owner->0x60`/`0x48`/`0x4C`/`0x50` (`+0x1000`/`0`/`+0x1800` vs.
`-0x1000`/`0`/`-0x800`) - reads as **launching the child away from the
player**, direction chosen by which side the player is on. Adds a small
`sub_8000E1C(3)`-derived randomized offset into `owner`'s Y
velocity-target triple, clears `owner->0xC` bit 2, `PlaySfx(ctx, 5,
0x80)`, then (unconditionally, since `self` is never actually null in
practice) fires a *second* `sub_803AD80` using `self+0xC`'s anchor
record's `+0x48`/`+0x4C` pair with submode `3`. Reads overall as a
**"detach and launch a fragment/projectile away from the player"**
event - a strong candidate for "object breaks/explodes, spawn debris"
given the crate-and-barrel-heavy object roster this ROM region's
93-vtable family covers (`docs/rom_map.md`'s dump of
`gStaticData_087E3EE4` and neighbors shows crate/barrel/creature
variants).

**Grouping for the next phase**: `sub_800BD48` is much shallower than
`sub_800B8DC` - only two real code paths (`0x0800BF2C`,
`0x0800BE80`) plus the shared prelude and a 17-state no-op majority.
`sub_800CBD4`, `sub_803AD80`'s exact record layout, and
`gUnknown_030012E4`'s own object shape are the best next targets to
fully resolve this function's remaining ambiguity (`sub_800CBD4`'s
own argument count in particular is unconfirmed - the call site sets no
registers explicitly, relying on `sub_8026EDC`'s leftover return value
in `r0`, so it may take 0 or 1 arguments; not resolved here).

## Opaque callees referenced (for the next phase to pick up)

All `(self, ...)`-shaped, still fully raw in `asm/code_3_2_17_bfa8.s`:

- **`sub_800C074`**, **`sub_800C40C`**, **`sub_800C5D4`**,
  **`sub_800C244`** - each independently dispatches on `self+0x68`
  (3-7 cases apiece) with its own further calls to
  `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC` and occasional
  `PlaySfx`/`sub_803AE4C` (a "close enough" scalar-distance check, used
  repeatedly). `sub_800C40C` is the specific function
  `docs/rom_map.md` already flagged as sharing `sub_800B8DC`'s own
  `self+0x68` field.
- **`sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC`** - `(self, mode)`
  shaped, modes 0-7 seen; called from nearly every state's inline logic
  and from all four `self+0x68` dispatchers above. The single highest-
  value next target - understanding these three unlocks most of the
  rest of this cluster's semantics at once.
- **`sub_800C18C`/`sub_800C1E8`** - X-axis/Y-axis "homing velocity-
  target setter" pair, already characterized in full above (reads
  `self+0x10`-`0x1C` bounds, `gUnknown_030012D8`'s position, writes
  `owner`'s `0x48`-`0x5C` triples).
- **`sub_800C314`**, **`sub_800C940`**, **`sub_800C97C`**,
  **`sub_800C8F8`**, **`sub_800BFA8`** - not traced beyond their own
  call signature (`self`-only) in this pass; `sub_800BFA8` is
  independently noted in `docs/rom_map.md` as "a further instance of
  the `self+0x68`/`0x74` generic state-machine selector pattern".
- **`sub_800C9C8`** - a 6-argument spawner (`s32 a, b, c, d, e, void
  *f`), called with `(0x1D, 0, 0, 0x2B, 0, owner)` from state 18 and
  with similar shapes elsewhere in this file's raw remainder - likely
  the floating-text/popup constructor `self->0x88` points at.
- **`sub_800CBD4`** - argument count unconfirmed (see above).

## Matching

Both functions closed as **hand-transcribed NAKED asm**, not real C -
an explicit, deliberate choice given this project's extensive, repeated
precedent that this exact "`self`/`owner`-style multi-field object with
many `bl` calls interspersed across branches" shape defeats gcc 2.9's
register allocator: `sub_8024F24`, `sub_8025130`/`sub_8025228`/
`sub_8025460` (`game_loop3.c`), `sub_800D040` (`game_loop6.c`),
`sub_800CD00` (`actor_part109.c`), `sub_800CEAC`/`sub_800CF70`
(`game_loop42.c`) are all NAKED in this same ROM neighborhood for the
same underlying reason. Given `sub_800B8DC`'s size (1132 B, 18 branches,
several inline blocks juggling `self`+`owner`+3-5 more live
locals/temporaries across `bl` calls - worse than any of the functions
above) and this investigation's own explicit brief not to over-invest
fighting a resistant shape, a full iterative real-C attempt was not
pursued; both were transcribed directly instead.

Translation followed this project's established NAKED-transcription
conventions: unified-syntax mnemonics to the divided/suffix-less form
(`adds`->`add`, `movs`->`mov`, `subs`->`sub`, `asrs`->`asr`,
`ands`->`and`, `lsls`->`lsl`, `lsrs`->`lsr`, `eors`->`eor`,
`orrs`->`orr`, `rsbs rX, rX, #0`->`neg rX, rX`), GNU-as local numeric
labels in place of the ROM disassembly's own `_080xxxxx` global labels
(uniquely numbered, one definition per number, to keep the large jump
table's many far-forward references unambiguous), and the ROM's own
mid-function `.pool` splits reproduced exactly - `sub_800B8DC` has 6
internal `.pool` points, `sub_800BD48` has 5 (plus a genuine trailing
one after `sub_800BD48`'s own final `bx r0`, since its last case still
has three pending literals at that point) - each placed at exactly the
ROM's own `.align 2, 0` + literal-run position, confirmed by checking
every literal's *first-use order* within its pool group matches the
ROM's own literal-label order (GNU-as pools flush in first-referenced
order, so this was verifiable statically before ever compiling).
`sub_800B8DC`'s own trailing byte count (1132) and `sub_800BD48`'s
(608) are both already 4-byte aligned with no genuine ROM padding gap
between them or after `sub_800BD48`, so neither needed the
`matching_decomp_alignment_fix` trailing `asm(".align 2, 0")` idiom.

Confirmed byte-identical to `baserom.gba`'s own raw bytes at
`0x0800B8DC`-`0x0800BFA8` (1740 bytes total, both functions) via the
isolated `cpp`/`agbcc`/`as` + `objcopy` pipeline (compiled and
assembled clean, no warnings; the isolated object's only byte
differences from a direct ROM slice were exactly its `R_ARM_THM_CALL`/
`R_ARM_ABS32` relocation sites - external `bl` targets, global
addresses, and the two internal jump tables' own `.text`-relative
entries - which resolve correctly once linked), plus a full clean
`rm -rf build && make NON_MATCHING=1 report` (no warnings from either
function) and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: La suma
coincide`).

### Techniques used

- Hand-transcribed NAKED asm, the established escape hatch for this
  ROM region's dominant `self`/`owner`-multi-field register-allocation
  gap (see `game_loop3.c`/`game_loop6.c`/`actor_part109.c`/
  `game_loop42.c` for the same bug class documented independently).
- Unique, never-reused GNU-as local numeric labels (rather than the
  more typical small reused set) to keep two separate large jump
  tables' many-target, far-forward `.4byte Nf` references unambiguous
  in one flat per-function label space.
- Literal-pool-group verification by first-use order, done statically
  against the ROM's own literal-label ordering before compiling, to
  place every `.pool` split at exactly the right point on the first
  attempt (both functions compiled clean with no iteration needed).

## Build layout

`asm/code_3_2_17.s` (which held this whole cluster, `sub_800B8DC`
through `sub_800CCE0`) is removed entirely. `sub_800B8DC`/`sub_800BD48`
now live in the new `src/graphics/actor_part112.c`; the unchanged
remainder (`sub_800BFA8` onward - the cluster's other ~41 functions,
still fully raw and unexamined) moved to the new
`asm/code_3_2_17_bfa8.s`. `ldscript.txt` now reads:

```
build/crashbandicootxs/src/graphics/actor_part112.o(.text);
build/crashbandicootxs/asm/code_3_2_17_bfa8.o(.text);
build/crashbandicootxs/src/graphics/actor_part109.o(.text);
```

`tools/report_units.py`'s single `(0x0800B8DC, None, "graphics")`
placeholder entry is replaced with a matched entry for
`actor_part112.o` plus a new `(0x0800BFA8, None, "graphics")`
placeholder for the still-raw remainder.

## What's still open

The other ~41 functions in `asm/code_3_2_17_bfa8.s` (`sub_800BFA8`
through the end of the old `code_3_2_17.s`, i.e. up to but not
including `sub_800D040`) remain completely raw. The "Opaque callees
referenced" section above names the highest-value subset (everything
`sub_800B8DC`/`sub_800BD48` themselves call) with a concrete priority
order: `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC` first (shared by
nearly everything), then the four `self+0x68` dispatchers
(`sub_800C074`/`sub_800C40C`/`sub_800C5D4`/`sub_800C244`), then
`sub_800C18C`/`sub_800C1E8`'s own already-fully-understood shape (a
quick, likely-real-C match - notably *simpler* register pressure than
either function this pass closed), then the remaining single-purpose
leaves (`sub_800C314`, `sub_800C940`, `sub_800C97C`, `sub_800C8F8`,
`sub_800BFA8`, `sub_800C9C8`, `sub_800CBD4`).

## Phase 2 (this pass): `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC` solved

Follow-up session, tackling exactly the three functions the Phase 1
pass above flagged as the single best next target. All three matched
as **real C**, first attempt clean - much smaller and, unlike
`sub_800B8DC`/`sub_800BD48`, entirely free of the `self`/`owner`
register-allocation resistance documented above (no branches, only a
handful of straight-line loads/stores per function). Sizes confirmed
from the raw ROM bytes: `sub_800C8AC` 16 B, `sub_800C8BC` 16 B,
`sub_800C8CC` 44 B (76 B total, `0x0800C8AC`-`0x0800C8F8`).

### What they actually do

All three share the same two-step shape: **cache `mode` (`arg1`) into a
`self`-local field, then delegate to a shared "fire an anchor-record
trigger" primitive.** The delegate differs per function:

- **`sub_800C8AC(self, mode)`**: `self->0x7c = mode;` then calls the
  *already-matched* `sub_800B704(self, self->0x70 /* owner */, mode)`
  (`src/graphics/actor_part17.c`). `sub_800B704` itself: looks up an
  8-byte record at `(*(self+4))[mode]`, reads that record's **second**
  word (`+4`) as a type index into the shared, 12-byte-stride
  `gStaticData_0816B304` table, then reads `self->0xc`'s "anchor"
  pointer (called `part` in `sub_800B704`'s own existing comment) and
  its **`+0x30`/`+0x34`** `{s16 offset, void *fn}` pair, and fires
  `sub_803AD84(self + offset, owner, tableEntry, fn)`.
- **`sub_800C8BC(self, mode)`**: identical shape, `self->0x78 = mode;`
  then `sub_800B838(self, owner, mode)` - the sibling accessor that
  reads the record's **first** word (`+0`) as the type index and the
  anchor's **`+0x28`/`+0x2c`** pair instead.
- **`sub_800C8CC(self, mode)`**: `self->0x68 = mode;` then triggers
  *directly*, with no `gStaticData_0816B304` lookup at all: reads the
  anchor's **`+0x50`/`+0x54`** pair for the offset/fn, and gets its
  table-entry argument by indexing **`self->0x84`'s own pointer array
  directly by `mode`** (`((void **)self->0x84)[mode]`), then fires the
  same `sub_803AD84(self + offset, owner, entry, fn)`.

### What this resolves from the Phase 1 doc

- **`self+0xc`'s anchor record** now has *four* of its `{s16 offset,
  void *fn}` pairs identified by offset: `+0x10`/`+0x14` (read directly
  by `sub_800B8DC` state 11, per the table above), `+0x28`/`+0x2c`
  (`sub_800C8BC`/`sub_800B838`), `+0x30`/`+0x34`
  (`sub_800C8AC`/`sub_800B704`), `+0x48`/`+0x4c` (read directly by
  `sub_800BD48`'s states 19-20), and now also `+0x50`/`+0x54`
  (`sub_800C8CC`). Strongly suggests a small, densely-packed array of
  these pairs (stride looks like 8 bytes: `0x10`, `0x28`... no - `0x28`
  to `0x30` to `0x48` to `0x50` isn't a fixed stride, so it's more
  likely a handful of individually-named slots for different
  event/direction kinds rather than an indexed array - still not fully
  resolved, but four concrete offsets is a solid base for whoever names
  this struct next).
- **`self+0x84`** - the Phase 1 doc's guess ("pointer to a small record
  with `+0xC`/`+0x14` bytes ... looks like a surface/material-type
  lookup") was based on a *different*, unrelated caller elsewhere in
  the cluster and doesn't describe what `sub_800C8CC` does with it:
  here it's read as `*(void ***)(self+0x84)` and direct-indexed by
  `mode` (`table[mode]`, 4-byte stride) - i.e. a **per-instance array
  of pointers**, playing the exact same "table entry" role
  `gStaticData_0816B304[type]` plays for `sub_800C8AC`/`sub_800C8BC`.
  Reads as a per-object override table parallel to the shared global
  one (`self->0x84[mode]` instead of `gStaticData_0816B304[recordType]`).
  The Phase 1 doc's original `+0xC`/`+0x14`-record guess isn't
  necessarily wrong for that *other* caller - `self->0x84` may simply
  be reused with two different shapes by two different call sites, not
  yet reconciled.
- **`self+4`** (newly identified, not previously in the Phase 1 doc's
  field table): a pointer to a "manager" object whose own first word is
  itself a pointer to an array of 8-byte records, indexed by `mode`
  (`(*(void ***)(self+4))[mode]`, 8-byte stride) - the record
  `sub_800B704`/`sub_800B838` read their type index from. Each 8-byte
  record apparently encodes *two* different type indices (word 0 vs.
  word 1) for the two different anchor-pair "axes"
  `sub_800C8BC`/`sub_800C8AC` respectively trigger.

### Matching

All three matched as real C on the first structured attempt (raw
pointer-arithmetic casts, matching the established convention of the
immediately-neighboring already-matched `sub_800B704`/`sub_800B838` in
`src/graphics/actor_part17.c` - no named structs committed yet, same
reasoning as the Phase 1 pass: several of the object's fields are still
not fully reconciled across all its callers). `sub_800C8AC`/
`sub_800C8BC` compiled byte-exact immediately. `sub_800C8CC` needed one
iteration: writing the anchor-pointer arithmetic as `self->0xc` read
once into a temporary, then `rec += 0x50`, then dereferencing that
*same* incremented pointer at `+0` and `+4` (rather than computing
`part+0x50` and `part+0x54` as two independent expressions) reproduced
gcc 2.9's own register reuse for `part+0x54`'s `ldr r3, [r3, #4]` - the
independent-expressions version instead computed a fresh base in `r2`
for the `+0x50` read and cost one extra 2-byte instruction. Also needed
a trailing `asm(".align 2, 0")` after `sub_800C8CC` (following the
`matching_decomp_alignment_fix` convention, same idiom already used
after `nullsub_13` in `actor_part17.c`): the ROM zero-pads
`sub_800C8CC`'s trailing 2 bytes to the next 4-byte boundary, but
without an explicit trailing align directive the linker instead filled
that gap with its default NOP-fill (`0xc046`) when placing the next
object file's own leading alignment.

Confirmed byte-identical to `baserom.gba` at `0x0800C8AC`-`0x0800C8F8`
(76 bytes) via the isolated `cpp`/`agbcc`/`as`+`objcopy` pipeline (the
only differences from a direct ROM slice were exactly the three `bl`
relocation sites - `sub_800B704`, `sub_800B838`, `sub_803AD84`), plus a
full clean `rm -rf build && make NON_MATCHING=1 report` (no warnings)
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: La suma
coincide`).

### Build layout

The three functions now live in the new `src/graphics/actor_part113.c`.
`asm/code_3_2_17_bfa8.s` is trimmed to end right before `sub_800C8AC`
(unchanged otherwise - still holds `sub_800BFA8` through `sub_800C898`,
raw); the remainder from `sub_800C8F8` onward (`sub_800C940` through
`sub_800CCE0`, still fully raw, unchanged bytes) moved verbatim to the
new `asm/code_3_2_17_c8f8.s`. `ldscript.txt` now reads, in this
stretch:

```
build/crashbandicootxs/src/graphics/actor_part112.o(.text);
build/crashbandicootxs/asm/code_3_2_17_bfa8.o(.text);
build/crashbandicootxs/src/graphics/actor_part113.o(.text);
build/crashbandicootxs/asm/code_3_2_17_c8f8.o(.text);
build/crashbandicootxs/src/graphics/actor_part109.o(.text);
```

`tools/report_units.py`'s single `(0x0800BFA8, None, "graphics")`
placeholder is replaced with: the same placeholder (now only covering
`sub_800BFA8`-`sub_800C898`), a matched entry for `actor_part113.o`,
and a new `(0x0800C8F8, None, "graphics")` placeholder for the
still-raw remainder in `asm/code_3_2_17_c8f8.s`.

### Still open (superseded by Phase 3 below for the four `self+0x68`
dispatchers)

`sub_800C074`/`sub_800C40C`/`sub_800C5D4`/`sub_800C244` (the four
`self+0x68` dispatchers) remain the next highest-value target per the
Phase 1 doc's own priority order - all four now have one *fewer*
opaque callee each, since `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC`'s
own semantics are fully known. The anchor record's own full field
layout (four pairs identified, exact total size/count still unknown)
and `self+4`'s "manager" object (pointer chain + 8-byte record array,
also still unnamed) are both good candidates for whoever next needs to
commit a named struct for this object shape.

## Phase 3 (this pass): the four `self+0x68` dispatchers closed

Follow-up session, tackling exactly the four functions Phase 1/2 flagged
as the next highest-value target: `sub_800C074` (280 B), `sub_800C244`
(208 B), `sub_800C40C` (456 B), `sub_800C5D4` (212 B). All four closed
as hand-transcribed **NAKED** asm, not real C - each one independently
hit the same `self`/`owner` multi-field-liveness register-pressure gap
this ROM neighborhood's other dispatchers (`sub_800B8DC`/`sub_800BD48`)
already document, plus a specific extra wrinkle worth recording for
whoever next attempts a real-C reconstruction in this cluster: **a
"shared-retest" CFG diamond** appears in `sub_800C074`'s mode 0/4 gate
blocks (and the structurally identical gate in `sub_800C314`, still
raw) - the ROM computes a mirror-flag sign test once, branches on it,
and then *re-tests the exact same cached value* on the fallthrough edge
before a second bound comparison, rather than letting either edge skip
straight to the bound check. A plain `if (signTest >= 0) goto hi; ...
hi: if (signTest < 0) break; ...` C reconstruction of this exact shape
gets silently optimized by gcc 2.9 at `-O2` into a smaller, branch-free
equivalent (jump-threading removes the provably-redundant retest) -
*shorter* than the ROM's own output, meaning the ROM's real source
almost certainly used a different idiom that defeats this optimization
(not identified in this pass), or simply accepted the same
resistant-shape NAKED fallback as everything else in this immediate ROM
neighborhood.

### What each one does

- **`sub_800C074`** (called from `sub_800B8DC` states 2/13/15/18): a
  4-case dispatcher on `self+0x68` (modes 0, 1, 4, 6; anything else is a
  silent no-op). Modes 0/4 share the mirror-aware position gate described
  above (`owner`'s X position against `self+0x10`/`self+0x14`, direction
  picked by `owner+0x28` bit 4) before triggering `sub_800C8CC` with mode
  1 or 6 respectively and always `sub_800C8BC(self, 0)`. Modes 1/6 both
  toggle `owner+0x28` bit 4 (via the `-0x11`-materialized mask-and-or
  idiom already established in `actor_part6.c`'s `sub_80086F4`/
  `sub_8008710` - `mask = -0x11; result = mask & flags; result |=
  bit << 4;`, **not** a plain XOR, which gcc would compile to a
  shorter/different instruction sequence) when `owner+0x38` is set, then
  trigger `sub_800C8CC`/`sub_800C8BC` with different constants (0/1 vs
  4/1). Mode 1 additionally clamps `owner+0x30` to
  `min(8, keyframeRecord->0x16 - 1)` when `self+0x6c == 0xf`, reusing the
  `owner+0x20`-table[`owner+0x2d`]-at-28-byte-stride keyframe-record
  convention `sub_800D040` documents at length.
- **`sub_800C244`** (called from state 8): unconditional prelude - once
  `owner->4` (Y) catches up to `self->0x64`, latches `owner->4 =
  self->0x64` and fires `sub_800C8BC(self,0)` + `sub_800C8AC(self,0)`
  every single call after that point (same "guard only matters once,
  real work happens every call" shape the Phase 1 doc already flagged
  for `sub_800B8DC` state 9). Two further `self+0x68`-keyed sub-cases
  split on `owner->0x38`: clear -> modes 0/1 trigger `sub_800C8CC(self,1)`
  or toggle `owner->0x28` bit 4 then trigger mode 0; set (further gated
  by `owner->0x30==8 && owner->0x34==0`, the established "blocking
  condition" pair) -> modes 0/1 both trigger `sub_800C8BC`/
  `sub_800C8AC(self,3)` and play SFX `0x14`, mode 1 additionally forcing
  `owner->0x28`'s mirror bit clear first (`sub_800C8BC(self,0)` instead
  of `sub_800C8BC(self,3)`).
- **`sub_800C40C`** (called from states 4/13/14/16 - the specific
  function `docs/rom_map.md` already flagged as sharing `sub_800B8DC`'s
  own `self+0x68` field): the largest and most complex of the four, a
  6-case dispatcher (modes 0, 3, 4, 5; 1/2/anything-else a no-op). Modes
  0/4 share an "impact distance" gate via `sub_803AE4C` (the
  divide/modulo-style "close enough" scalar primitive) against a
  `gUnknown_0300082C`-relative table indexed by `self->0x30`/`0x34`/
  `0x38`, then consult `self->0x84`'s pointed record (`+0xc` for mode 0,
  `+0x14` for mode 4) against the constant `8` to pick between two
  `sub_800C8CC` trigger constants; mode 0 also clears `owner->0xd` bit 3
  and re-triggers `sub_800C8BC(self,0)` when `self->0x6c==0xf`, or when
  `self->0x6c` is `0x12`/`0x1a`. Mode 3 is the largest single case: when
  `owner->0x38` is set, triggers `sub_800C8CC(self,4)`, then on
  `self->0x6c` `0x12`/`0x1a` sets `owner->0xd` bit 3 and plays SFX
  `0x26`, or on `self->0x6c==0xf` plays SFX `9`; then *unconditionally*
  (regardless of the `owner->0x38` gate), if `self->0x6c==0x17` and
  `owner->0x30==9`/`owner->0x34==0`, spawns a part via
  `sub_8025B0C(gUnknown_030012E4, 0x17, 4, -0x2d, 2, owner)` (matching
  `game_loop14.c`'s own `sub_8025B0C(arg0, arg1, arg2, margin, z, src)`
  signature - `gUnknown_030012E4` as the pool, `owner` as `src`, `2` as
  `z`, `-0x2d` as `margin`), tags the new part's `+0xc` flags/`+0xa`
  bitmap-id fields (same idiom family as the "flag active + bitmap-set"
  idiom elsewhere in this cluster), and plays SFX `0x1e`. Mode 5 mirrors
  mode 3's `owner->0x38` gate into `sub_800C8CC(self,0)` (plus, only for
  `self->0x6c==0xf`, `sub_800C8BC(self,1)`); a shared tail (also reached
  directly when `owner->0x38` was already clear) plays SFX `0x23` when
  `self->0x6c==0xf` and `owner->0x30==8`/`owner->0x34==0`.
- **`sub_800C5D4`** (called from state 3, and state 13's fallthrough): a
  3-case dispatcher (modes 0, 2; anything else falls to a shared tail).
  Unconditional prelude: if `self->0x6c==0xb` and `owner->4 <
  self->0x64`, latches `owner->4 = self->0x64` and fires
  `sub_800C8AC(self,0)`. Mode 0 builds a `struct aabb` (the same shape
  `actor_part4.c`/`actor_part15.c` already use, via `sub_803AFE4`/
  `sub_803AFDC`) at `owner`'s position offset by `self->0x20`/`0x24`,
  sized by `self->0x28-0x20`/`self->0x2c-0x24` - a per-instance trigger
  box distinct from `owner`'s own smaller flags-byte field layout at the
  same nominal offsets (confirming `self` and `owner`, while sharing
  *some* field roles per the Phase 1 doc's table, are not literally
  identical-layout instances at every offset) - mirrors it per
  `owner->0x28` bit 4, and tests it against the player
  (`gUnknown_030012D8`) via `sub_800B37C`. On overlap: triggers
  `sub_800C8CC(self,2)` and, only when `self->0x6c==0xb`, seeds
  `owner`'s `0x48`-`0x64` velocity-target fields with a fixed knockback
  impulse (`0x300`/`0x20`/`0`/`-0x200` pattern - same family as
  `sub_800B8DC` state 17's own fixed jump impulse). Mode 2 triggers
  `sub_800C8CC(self,0)` only when `owner->0x38` is set.

### Matching

All four transcribed as hand-written NAKED asm, following this
project's established conventions (unified-syntax mnemonics, numeric
local labels, `.pool` directives placed at the ROM's own literal-flush
points). `sub_800C5D4` additionally needed the
`matching_decomp_alignment_fix` trailing `asm(".align 2, 0")` idiom (its
own trailing 2 bytes zero-pad to the next 4-byte boundary, same as
`nullsub_13`/`sub_800C8CC`). Two multi-word `.pool` placement bugs were
caught and fixed during this pass (both in `sub_800C40C`): the ROM
places each `.pool` *after* the full conditional block that follows the
literal's use (not immediately after the `ldr =`/`bl` pair that
references it) - getting this wrong changes where the assembler inserts
2-byte alignment padding and desyncs everything downstream by a
half-instruction, an easy trap confirmed twice in this same function
before switching to always verifying literal-pool placement via an `-al`
assembler listing against the ROM's own label offsets rather than
guessing from the source text alone.

Confirmed byte-identical to `baserom.gba` at `0x0800C074`-`0x0800C246`,
`0x0800C244`-`0x0800C316` (sic, verified individually per function's own
address range), `0x0800C40C`-`0x0800C6A8`, and `0x0800C5D4`-`0x0800C6A8`
(456 B and 212 B respectively for the last two, contiguous with no gap)
via the isolated `cpp`/`agbcc`/`as` + `objcopy`/`cmp` pipeline (the only
differences from a direct ROM slice were `bl`/`.word` relocation sites),
plus a full clean `rm -rf build && make NON_MATCHING=1 report` (no
warnings from any of the four) and `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`
(`crashbandicootxs.gba: La suma coincide`).

A real-C reconstruction was attempted first for all four (this pass's
initial approach, not skipped) - `sub_800C074`'s dispatch-chain shape,
case-body ordering, and the mask-and-or bit-toggle idiom were all
successfully coaxed to match byte-for-byte using register-pinned locals
and an `asm volatile` block for the boolean materialization (see
`actor_part6.c`'s own established precedent for this exact idiom), but
the mode 0/4 "shared-retest" gate (described above) could not be
reproduced without gcc 2.9 collapsing it to fewer instructions than the
ROM actually emits - at that point, given the other three functions
share the identical `self`/`owner` register-pressure shape already
established as resistant throughout this ROM region, all four were
switched to NAKED transcription instead of individually re-litigating
the same wall four times.

### Techniques used

- Hand-transcribed NAKED asm (established escape hatch for this ROM
  region), reusing the unique-per-function numeric local label
  convention from `actor_part112.c`.
- `-al` assembler listing cross-checked against the ROM's own literal
  label offsets to place every `.pool` directive correctly on the
  (eventual) first try, after two placement bugs in `sub_800C40C`
  surfaced the failure mode.
- `matching_decomp_alignment_fix` trailing `asm(".align 2, 0")` for
  `sub_800C5D4`'s own non-4-aligned trailing byte count.
- Verified each function byte-exact in isolation *and* as physically
  combined into a shared object file (`sub_800C40C`+`sub_800C5D4` in one
  `actor_part116.c`) - the combined-file build caught a real bug
  (duplicate/misplaced `.pool` directives reintroduced by hand-copying
  from the isolated scratch files into the production file) that the
  per-function isolated tests alone did not, underscoring that the
  mandatory full `make compare` step is not just a formality even after
  isolated verification passes.

### Build layout

The four functions now live in three new files: `src/graphics/
actor_part114.c` (`sub_800C074`), `src/graphics/actor_part115.c`
(`sub_800C244`), and `src/graphics/actor_part116.c` (`sub_800C40C` +
`sub_800C5D4`, contiguous in ROM with no gap). The single raw
`asm/code_3_2_17_bfa8.s` (which held `sub_800BFA8` through
`sub_800C898`) is split into four pieces around the newly-matched
functions: `asm/code_3_2_17_bfa8.s` (trimmed to just `sub_800BFA8`),
`asm/code_3_2_17_c18c.s` (`sub_800C18C`/`sub_800C1E8`, still raw),
`asm/code_3_2_17_c314.s` (`sub_800C314`, still raw), and
`asm/code_3_2_17_c6a8.s` (`sub_800C6A8`/`sub_800C860`/`sub_800C87C`/
`sub_800C898`, still raw). `ldscript.txt` now reads, in this stretch:
## Phase 3 (parallel pass): the remaining single-purpose leaves

A second parallel session, tackling exactly the "remaining leaves" the
Phase 1 doc's own priority list named
(`sub_800C18C`/`sub_800C1E8`/`sub_800C314`/`sub_800C940`/`sub_800C97C`/
`sub_800C8F8`/`sub_800BFA8`/`sub_800C9C8`/`sub_800CBD4`/the function
formerly guessed as `sub_800CBD4` but actually `sub_800CBF4`), working
non-overlapping byte ranges from the four `self+0x68` dispatchers
another parallel session was assigned (`sub_800C074`/`sub_800C40C`/
`sub_800C5D4`/`sub_800C244`, all left completely untouched, still raw).

**Closed, real C**: `sub_800C9C8` (the floating-popup spawner, state
18's `sub_800C9C8(0x1D, 0, 0, 0x2B, 0, owner)` callee - a thin
`sub_8025B0C` wrapper, `src/graphics/actor_part116.c`) and `sub_800CBD4`
(`src/graphics/actor_part117.c`, see below).

**Closed, NAKED**: `sub_800C18C`/`sub_800C1E8` (`actor_part114.c`),
`sub_800C314` (`actor_part115.c`), `sub_800C8F8`/`sub_800C940`/
`sub_800C97C` (`actor_part116.c`) - each is small (60-248B),
straight-line or shallow-branching, and semantically fully understood
(documented in each file's own doc comment), but every one hit a
*different* flavor of gcc-2.9/this-agbcc-build code-selection gap that
resisted real-C reconstruction even after substantial iteration -
these are recorded here as a durable reference for the next session
tempted to re-attempt them:

- **`sub_800C18C`/`sub_800C1E8`** (the X/Y-axis "homing velocity-target
  setter" pair the Phase 1 doc flagged as the cluster's single most
  promising quick win): an isolated real-C attempt reproduced every
  branch's *polarity* correctly (after several iterations - the naive
  first draft had every single comparison inverted from the ROM, a
  useful lesson that gcc-2.9's -O2 branch-layout choice for an
  `if(cond){A}else{B-with-return}` is *not* simply "negate cond, skip
  A" - it can just as easily place the returning branch as the
  fallthrough and the continuing branch as the jump target, and which
  one it picks depends on details not fully characterized here) and
  even matched three of the function's four distinct `{vx,vy}`-store
  sites exactly - but this agbcc build's own cross-jump/tail-merging
  pass kept unifying the far-band "bound hit" case's three stores with
  the shared near-band tail's own three stores into one block
  (mismatching which register holds which value at the merge point),
  while the ROM's own compiled output keeps that exact 3-instruction
  "`vx=0, vy=0x10`" sequence *duplicated* verbatim at both of its call
  sites, never merged. This held regardless of `goto`-based
  restructuring, self-contained duplicate-store blocks, or
  `register`/`volatile` pins on the store operands (register pins
  *did* successfully suppress the merge in one direction, but only by
  also flipping the branch polarity back to wrong).
- **`sub_800C314`**: an isolated attempt reproduced the entire
  dispatch/switch structure exactly, and even reproduced the
  established `(s32)(x << 27) < 0` mirror-flag-bit-test idiom
  (`src/graphics/actor_part17.c`) correctly for both of this
  function's own bit-toggle sites - but could not reproduce the ROM's
  own instruction *sequencing* for folding the toggled bit back into
  the byte (compute 0/1 flag via the shift-test, shift it into bit
  position, materialize the "clear that bit" mask via a
  `movs #imm; rsbs r,r,#0` negate-trick, AND, OR - all as one shared
  tail, matching the exact shape `src/graphics/actor_part27c.c`'s
  `sub_8018884` doc comment already documents needing heavy register
  pinning for on a related idiom). Every C rephrasing tried (nested
  ternary, a separate `bit = cond ? 0 : 1;` statement, full if/else
  with separate per-branch stores) got a *different* but still-
  mismatched instruction order/selection - one variant even had the
  compiler algebraically reuse an already-live register left over from
  an earlier unrelated computation (`self->0x80 & 1`'s leftover
  literal `1`) instead of the ROM's fresh literal load for the mask.
- **`sub_800C8F8`/`sub_800C940`/`sub_800C97C`** (the sine-wave
  oscillator family, `gStaticData_0816A820` + `gUnknown_0300082C`):
  fully traced semantically (see `actor_part116.c`'s own doc comment
  for the per-function field/phase-derivation breakdown), and an
  isolated attempt got every field access and the table lookup itself
  correct, but two ROM-specific micro-choices resisted: (1) the ROM's
  `self->0x40 - 0x100` is always materialized as a full 32-bit literal
  `0xFFFFFF00` added in (since 256 doesn't fit Thumb's 8-bit `subs`
  immediate range), while a natural `... - 0x100` C expression lets
  the compiler re-associate into a same-value different-encoding form
  that skips the extra literal entirely; writing the constant as an
  explicit named `s32` local assigned `+ (s32)0xFFFFFF00` fixed this
  one. (2) The final `tableVal * self->0x44` product needs a spare
  register freed up for `owner` via a `mov`-style register copy of one
  of the two operands - but *which* operand gets copied, and into
  which register, depends on downstream allocator choices that proved
  impossible to pin down exactly even after matching fix (1) and
  trying direct `register asm("rN")` pins on every intermediate.
  `sub_800C940` additionally pushes 4 registers (`r4`-`r6`, `lr`)
  though only 3 hold live values anywhere in its own body - presumably
  8-byte stack-alignment padding this agbcc build doesn't reproduce for
  a leaf function making no `bl` calls.

**A genuine bug caught mid-investigation**: the first hand-decode pass
over `sub_800C314`'s raw bytes misread both `rsbs r0, r0, #0` negate-
mask idioms as clearing *two* bits each (e.g. `-0x11` clearing bits 0
*and* 4) by conflating two's-complement negation with bitwise
complement - `NOT(x) = -x - 1`, so `-0x11` (0xFFFFFFEF) clears *only*
bit 4, not bits 0 and 4. Caught by cross-checking against
`sub_800C9C8`'s own `-0x41` mask (clears only bit 6, confirmed against
its real-C match) before committing any wrong semantics to a doc or a
struct.

**`sub_800CBD4` resolves two Phase 1 open questions**: it takes
exactly **one** argument (`self`) - `sub_800BD48`'s own states 19-20
call it immediately after `sub_8026EDC(0x10)` with no registers set
explicitly, relying entirely on that allocator's leftover return value
in `r0`, confirming the Phase 1 doc's "may take 0 or 1 arguments"
question in favor of 1. And `self+0xc` (the "anchor" record every
`sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC` call reads) is set here to a
**fixed global table**, `gStaticData_087E3FA4` - every object
constructed through this path shares the same anchor record, not a
per-instance one. Matches the exact "reset via `sub_800B8C8`, re-point
`self+0xc`, return `self`" shape already established for sibling
constructors `sub_801886C`/`sub_8018858`.

**Still not attempted**: `sub_800BFA8` (~204B, a further
`self+0x68`/`0x74`-style dispatcher per `docs/rom_map.md`'s existing
note) and `sub_800CBF4` (~208B, three repetitions of the "flag active +
bitmap-set" idiom gated behind a hit-probe and two more flag tests -
the same idiom `sub_8018884`'s doc comment already documents as
needing heavy register pinning, here appearing three times over) -
both left raw for time-budget reasons, not because they were found
resistant. Good next targets given `sub_800C8AC`/`sub_800C8BC`/
`sub_800C8CC` and the mirror-flag idiom are now both fully understood.

### Build layout

`asm/code_3_2_17_bfa8.s` is trimmed further to end right after
`sub_800C074` (still raw, `sub_800C074` is one of the other parallel
session's four `self+0x68` targets). The removed
`sub_800C18C`/`sub_800C1E8` now live in the new
`src/graphics/actor_part114.c`. `sub_800C244` (untouched, another
`self+0x68` target) was carved into its own new `asm/code_3_2_17_c244.s`
so it wouldn't need to move again once its own session closes it. The
removed `sub_800C314` now lives in the new
`src/graphics/actor_part115.c`. `sub_800C40C` onward (through
`sub_800C898` - `sub_800C40C`/`sub_800C5D4` untouched `self+0x68`
targets, plus `sub_800C6A8`/`sub_800C860`/`sub_800C87C`/`sub_800C898`,
none of this pass's or the parallel session's concern) moved to the new
`asm/code_3_2_17_c40c.s`.

Likewise, `asm/code_3_2_17_c8f8.s` is fully consumed: the removed
`sub_800C8F8`/`sub_800C940`/`sub_800C97C`/`sub_800C9C8` now live in the
new `src/graphics/actor_part116.c`; the untouched, still-raw
`sub_800CA04`-`sub_800CBC0` remainder moved to the new
`asm/code_3_2_17_ca04.s`; the removed `sub_800CBD4` now lives in the
new `src/graphics/actor_part117.c`; the untouched, still-raw
`sub_800CBF4` onward (through `sub_800CCE0`) moved to the new
`asm/code_3_2_17_cbf4.s`.

`ldscript.txt` now reads, in this stretch:

```
build/crashbandicootxs/src/graphics/actor_part112.o(.text);
build/crashbandicootxs/asm/code_3_2_17_bfa8.o(.text);
build/crashbandicootxs/src/graphics/actor_part114.o(.text);
build/crashbandicootxs/asm/code_3_2_17_c18c.o(.text);
build/crashbandicootxs/src/graphics/actor_part115.o(.text);
build/crashbandicootxs/asm/code_3_2_17_c314.o(.text);
build/crashbandicootxs/src/graphics/actor_part116.o(.text);
build/crashbandicootxs/asm/code_3_2_17_c6a8.o(.text);
build/crashbandicootxs/src/graphics/actor_part113.o(.text);
build/crashbandicootxs/asm/code_3_2_17_c8f8.o(.text);
```

`tools/report_units.py`'s single `(0x0800BFA8, None, "graphics")`
placeholder is replaced with seven entries: the same placeholder (now
only covering `sub_800BFA8`), matched entries for `actor_part114.o`/
`actor_part115.o`/`actor_part116.o`, and new placeholders
`(0x0800C18C, None, "graphics")`, `(0x0800C314, None, "graphics")`, and
`(0x0800C6A8, None, "graphics")` for the still-raw remainders.

### Still open

`sub_800C18C`/`sub_800C1E8` (the X/Y "homing velocity-target setter"
pair, already fully traced in the Phase 1 doc's own field notes - "self
+0x10-0x1c bounds, gUnknown_030012D8's position, writes owner's
0x48-0x5c triples", flagged there as "notably simpler register pressure"
than the dispatchers) remain the single best next target - both are
called directly from several of `sub_800B8DC`'s own states (10, 11, 15)
and from `sub_800C5D4` itself. `sub_800C314` (self+0x80 bit-0/parity
idiom, two different owner+0x28 mirror-bit-toggle bit positions,
sub_800C8AC/sub_800C8BC/sub_800C8CC triggers) and the `sub_800C6A8`/
`sub_800C860`/`sub_800C87C`/`sub_800C898` group (the `menu_ui` 31-slot
dialog dispatcher plus three small `self+0x70`-relative accessor
triples) round out the rest of this now much-smaller raw remainder
between `sub_800BFA8` and `sub_800C8AC`.
build/crashbandicootxs/asm/code_3_2_17_c244.o(.text);
build/crashbandicootxs/src/graphics/actor_part115.o(.text);
build/crashbandicootxs/asm/code_3_2_17_c40c.o(.text);
build/crashbandicootxs/src/graphics/actor_part113.o(.text);
build/crashbandicootxs/src/graphics/actor_part116.o(.text);
build/crashbandicootxs/asm/code_3_2_17_ca04.o(.text);
build/crashbandicootxs/src/graphics/actor_part117.o(.text);
build/crashbandicootxs/asm/code_3_2_17_cbf4.o(.text);
build/crashbandicootxs/src/graphics/actor_part109.o(.text);
```

`tools/report_units.py`'s two placeholder entries
(`0x0800BFA8`/`0x0800C8F8`) are each split into the matched/still-raw
sub-ranges described above.

### A trailing-alignment gotcha caught by `make compare`

`sub_800C1E8`'s own trailing byte count (92B) is *not* a multiple of 4
past its own `.pool` literal, leaving a genuine 2-byte ROM zero-padding
gap before `sub_800C244` begins - the exact
`matching_decomp_alignment_fix`-documented scenario. Missing the
explicit trailing `asm(".align 2, 0")` after `sub_800C1E8` passed
`make NON_MATCHING=1 report` cleanly (no warnings) but failed
`make compare` with exactly a 2-byte mismatch at that boundary (the
linker's default `0xc046` NOP-fill instead of the ROM's own `0x0000`) -
caught immediately by the mandatory full-clean `make compare` step,
confirming why that step is non-negotiable even when the isolated
per-function byte-compare already looked clean.

### Verification

All seven closed functions (`sub_800C18C`, `sub_800C1E8`,
`sub_800C314`, `sub_800C8F8`, `sub_800C940`, `sub_800C97C`,
`sub_800C9C8`, `sub_800CBD4` - eight total) confirmed byte-identical to
`baserom.gba` via the isolated cpp/agbcc/as + objcopy/cmp pipeline
(only `bl`/literal-pool relocation sites differ in every case) plus a
full clean `rm -rf build && make NON_MATCHING=1 report` (no warnings)
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: La suma
coincide`).

## `sub_800BFA8`: the last raw function in `asm/code_3_2_17_bfa8.s`

Closing pass on `sub_800BFA8` (ROM `0x0800BFA8`-`0x0800C074`, 204
bytes), the sole remaining function in `asm/code_3_2_17_bfa8.s` -
`tools/report_units.py`'s own placeholder comment for it called it "a
further `self+0x68`/`0x74`-selector dispatcher per docs/rom_map.md,
not yet examined." It's called exactly once, from `sub_800B8DC` state
15 (case 42 in that function's own jump table, right after
`sub_800C40C`).

### Shape

A small `self+0x68`-keyed dispatcher, gated by a `sub_803AE4C` "close
enough" scalar check - the same primitive `sub_800C40C`'s own case 3/4
use, but here against `gUnknown_0300082C` read as a **plain word**
(`sub_803AE4C(gUnknown_0300082C + self->0x48 - self->0x4c,
self->0x48)`), not the table-base-pointer role `sub_800C40C` uses that
same still-unexplained global in. This is a fourth confirmed
"multi-shaped" site for `gUnknown_0300082C` (joining the "plain word"
sites `sub_8016C94`/`sub_801B624` and the "table base pointer" site
`sub_800C40C` already flagged in `docs/rom_map.md`).

- **Check passes (result `0`)**: `self->0x68 == 0` triggers
  `sub_800C8CC(self, 2)`; `self->0x68 == 4` triggers
  `sub_800C8CC(self, 7)`; anything else is a no-op. Function returns.
- **Check fails**: re-dispatch moves to `owner` (`self->0x70`).
  - `owner->0x38` (the cluster's established "enabled" byte) set:
    `self->0x68 == 2` triggers `sub_800C8CC(self, 0)`; `== 7` triggers
    `sub_800C8CC(self, 4)`; anything else a no-op. Returns.
  - `owner->0x38` clear: `self->0x68 == 2`/`7` each gate a
    `sub_800C9C8(0xc, 6, 0, d, 0x400, owner)` call behind an
    `owner->0x30`/`owner->0x34` magic-constant check (`d = -0xa` when
    `owner->0x30==0xa && owner->0x34==0` for mode 2; `d = 8` when
    `owner->0x30==8 && owner->0x34==0` for mode 7 - the same
    blocking-condition-pair shape this doc's field table already
    documents at offsets `0x30`/`0x34`). On success (non-null return),
    the returned record's `+0xa` byte is set to `8`.

### Matching as real C

Small enough (no nested loops, only 2-deep dispatch) to avoid this
cluster's usual `self`/`owner` multi-field-liveness register-pressure
trap that forced every `self+0x68`-dispatching sibling
(`sub_800C074`/`sub_800C244`/`sub_800C40C`/`sub_800C5D4`) to NAKED.
Straightforward pointer-offset-cast C (this neighborhood's established
convention - `u8 *self = selfArg;` plus `*(s32 *)(self + off)`, per
`actor_part113.c`/`actor_part117.c`) got every branch, constant, and
call argument right on the first pass and diffed to within two
register-allocation quirks of the ROM, both resolved via
[[matching_decomp_register_pinning]] (see that memory doc / the
project's `docs/matching.md`):

1. **`self` pinned to `asm("r4")`** (`register u8 *self asm("r4") =
   selfArg;`). Unpinned, gcc's own allocator duplicated `self` into a
   spare `r5` register (`push {r4, r5, lr}` / `add r5, r4, #0`) purely
   to re-read `self->0x68` a second time in the `owner->0x38==0`
   branch, even though `r4` was still live and unused at that point -
   the ROM never touches a second register for `self` at all
   (`push {r4, lr}` only). Pinning eliminated the duplicate register
   and its push/pop entirely.
2. **Statement-order hoist for two independent loads.** The prelude's
   `gUnknown_0300082C` global read and `self->0x48` field read have no
   data dependency on each other (only their *sum* does), so gcc 2.9's
   scheduler was free to reorder them - and did, emitting
   `self->0x48`'s load first even though the C source computed it
   second. Simply computing the global read into its own named local
   (`s32 base = (s32)gUnknown_0300082C;`) as the *first* statement,
   ahead of `s32 field48 = *(s32 *)(self + 0x48);`, was enough to make
   the scheduler honor that order - no inline-asm anchor needed here
   (contrast with [[matching_decomp_register_pinning]] technique 6,
   which was the fallback plan if this hadn't worked).

Confirmed byte-identical to `baserom.gba`'s own raw bytes at
`0x0800BFA8`-`0x0800C074` via the isolated cpp/agbcc/as +
objcopy/cmp pipeline (26 differing bytes total, all at the 5 `bl`
relocation sites and the `gUnknown_0300082C` literal-pool word - the
same expected relocation-only gap documented throughout this cluster)
plus a full clean `rm -rf build && make NON_MATCHING=1 report` (no
warnings) and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: La suma
coincide`).

This was the last function in `asm/code_3_2_17_bfa8.s` - the file is
now empty and has been deleted, with its `ldscript.txt` entry replaced
by `build/crashbandicootxs/src/graphics/actor_part121.o(.text);`
(new file, `src/graphics/actor_part121.c`).
