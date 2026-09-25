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
