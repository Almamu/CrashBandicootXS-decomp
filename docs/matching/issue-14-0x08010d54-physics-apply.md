# 0x08010D54: physics/collision apply-commit step, and Phase 2 planning for its 24-function tail

`tools/report_units.py` (around the old `0x08010D54` entry) described
this as "the start of the next still-unexamined chunk past issue #14's
range" - 25 functions, `asm/code_3_2_17_e560_10d54.s`. This is the
write-up for a Phase 1 pass over that chunk: crack the entry point
(`sub_8010D54` itself, already strongly implicated by both
`docs/rom_map.md` and `sub_8010B6C`'s own doc comment - see below),
match it, and leave a semantic map + address/size list of the other 24
functions for a future Phase 2 to parallelize, mirroring the approach
already used for the `sub_800B8DC` (issue #9/#10) and `sub_0800D18C`
(issue #12 Phase 1) clusters earlier in this session.

## Ground truth: the caller side

`docs/rom_map.md` (around its "Confirmed: a shared physics/collision
subsystem" section) already read `sub_0800D18C`'s own tail (matched as
NAKED transcription in `src/system/game_loop47.c`, see
[docs/matching/issue-12-physics-collision.md](issue-12-physics-collision.md))
and confirmed it hands off to `sub_8010D54` with "~8 packed arguments"
as "the actual apply/commit step" - the very last call
`sub_0800D18C`'s own three-jump-table dispatch makes before returning.
`sub_8010B6C` (matched earlier this session as a NAKED transcription in
`src/system/game_loop28.c`, GitHub issue #14) went further: its own doc
comment already calls `sub_8010D54` its **"mirror-image"** function -
`sub_8010B6C` *reads* the exact record shape `sub_8010D54` *writes*.
That doc comment (`game_loop28.c`) documents the record layout in
detail:

> `self+8` onward is an array of `0x24`-byte "candidate" records
> (`neighbor` pointer at `+0`, a position pair at `+4`/`+8`, a `kind`
> tag at `+0xc`, three more fields at `+0x10`/`+0x14`/`+0x18`/`+0x1c`,
> two flag bytes at `+0x20`/`+0x21`)

and `game_loop28.c`'s own extern declaration for `sub_800E08C`
(`src/system/game_loop27.c`) already names every one of those fields:

```c
extern void sub_800E08C(void *neighbor, s32 kind, void *field10, void *field14,
                         s32 field18, s32 field4, s32 field8, s32 field1c,
                         u8 field20, u8 field21, u8 extra);
```

This is decisive ground truth: `sub_8010D54`'s own raw bytes (read
directly from `asm/code_3_2_17_e560_10d54.s` before this pass) write to
exactly these ten fields, in a pattern that maps one-for-one onto this
already-established naming.

## `sub_8010D54`'s confirmed shape

A single straight-line function (192 bytes, no branches, no literal
pool - every operand is either an argument or a small immediate).
Appends one record to a small append-only queue and increments its
counter:

```c
struct collision_candidate {
    void *neighbor; // 0x00
    s32 field4;      // 0x04
    s32 field8;       // 0x08
    s32 kind;          // 0x0c
    void *field10;       // 0x10
    void *field14;         // 0x14
    s32 field18;             // 0x18
    s32 field1c;               // 0x1c
    u8 field20;                  // 0x20
    u8 field21;                   // 0x21
};

struct collision_queue {
    s32 count;                                 // 0x00
    u8 unk4[4];                                   // 0x04
    struct collision_candidate candidates[1];        // 0x08+
};

void sub_8010D54(struct collision_queue *self, void *neighbor, s32 kind,
                  void *field10, void *field14, s32 field18, s32 field4,
                  s32 field8, s32 field1c, u8 field20, u8 field21);
```

`self->candidates[self->count]`'s ten fields are written in this exact
order (matching the ROM's own store order, not simple field-declaration
order): `neighbor`, `kind`, `field10`, `field21`, `field1c`, `field14`,
`field20`, `field4`, `field8`, `field18`, then `self->count++`.
Consecutive writes that land on the *same* record-base computation
(`neighbor`+`kind`, and `field4`+`field8`) share one index computation
in the ROM; every other write recomputes `self->count` fresh - 8
distinct index computations for 10 field writes, confirmed instruction-
for-instruction.

### Caller-side argument confirmation

Every argument's meaning is confirmed against `sub_0800D18C`'s own
final call site (`game_loop47.c`, the `bl sub_8010D54` right before that
function's epilogue):

- `self` = `gUnknown_030012D8 + 0x108` (dereferencing the pointer
  variable first) - the *player's* own instance of this queue. This is
  the exact same base address `sub_8010A0C`-`sub_8010B68`
  (`game_loop27.c`) and `sub_8010B6C` (`game_loop28.c`) already operate
  on as `self` for their own "collision box" record fields
  (`self+0x44`-`self+0x58`) - i.e. the queue this function appends to is
  embedded in the *front* of that same larger per-entity record, past
  `struct actor`'s own documented 0x1c bytes.
- `neighbor` = the entity whose collision is being committed (`self`
  from `sub_0800D18C`'s own perspective, held in `r8` at the call site).
- `kind` = `sub_0800D18C`'s own adjusted dispatch id (`[sp,#0x78]` at
  the call site).
- The remaining seven fields are packed position/rect values
  `sub_0800D18C` accumulated across its own three jump tables.

### Independent confirmation: `sub_8010E14`/`sub_8010E2C`

The chunk's first two remaining functions (right after `sub_8010D54`,
`0x08010E14`/`0x08010E2C`) are **already extern-declared elsewhere** in
the codebase, and both operate on this exact same `self+0x108` queue on
*arbitrary* entities (not just the player):

- `src/graphics/actor_part15.c`: `extern void sub_8010E14(void *arg0,
  s32 arg1);`, called as `sub_8010E14(self + 0x108, 2)`.
- `src/graphics/actor_part77.c`: `extern void sub_8010E2C(void *arg0);`,
  called as `sub_8010E2C(self + 0x108)`, documented there as "clears its
  trailing `+0x108`/`+0x10c` fields" - i.e. clears exactly `count`
  (`+0x108+0x0`) and the field this doc calls `unk4`
  (`+0x108+0x4`/`+0x10c`) **together**, confirming `unk4` is a real,
  meaningful second field (not padding) paired with `count` - likely a
  second counter or a "high-water mark" the reader side also consults.
  Worth resolving in Phase 2.

This confirms the `collision_queue` struct is generic per-entity
infrastructure (every entity has its own queue at `self+0x108`), not
player-specific - the player is just the one instance `sub_0800D18C`'s
one call site happens to target. `sub_8010D54`/`sub_8010E14`/
`sub_8010E2C` read as a three-function accessor family for the same
struct: a fixed-argument append (`sub_8010D54`), a mode-parameterized
insert (`sub_8010E14`, `arg1` presumably selecting append-vs-dedup-vs-
replace), and a reset (`sub_8010E2C`).

## Matching result

**Matched via NAKED transcription.** A plain-C reconstruction (the
struct/function above, with the field writes in the ROM's own order)
reproduces the ROM's exact *shape* - identical 8-way common-
subexpression grouping, sharing index computations in exactly the same
two spots the ROM does and nowhere else - but gcc 2.9 `-O2` picks a
different scratch register for the "copy of `self` used to read
`self->count`" step almost every time (e.g. `r4` where the ROM uses
`r1`, which is why the ROM has to cache the `neighbor` argument into
`sb` before clobbering `r1` - something the plain-C version never
needs to do). This recurs at nearly every one of the 8 index
computations, not one isolated register letter to pin, so closed via
NAKED transcription instead - the same escape hatch already established
throughout this subsystem. No branches and no literal pool in this
function, so the transcription needed no label renumbering or pool-
placement care (unlike the larger NAKED functions elsewhere in this
subsystem).

Verified in two stages: an isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pass against the raw ROM bytes at `0x08010D54` matched
byte-for-byte outright (no relocations to account for - the function
calls nothing and references no globals), then the authoritative full
clean `make NON_MATCHING=1 report` (no warnings) followed by
`make compare`, which passed outright ("La suma coincide").

New file `src/system/game_loop50.c`, inserted in `ldscript.txt` between
`game_loop28.o` and the now-trimmed `asm/code_3_2_17_e560_10d54.o`
(which now begins at `sub_8010E14`). `tools/report_units.py`'s old
`0x08010D54` entry is split in two: `0x08010D54` now points at
`game_loop50.o` (category `game_loop`, matching the rest of this
subsystem), and a new `0x08010E14` entry covers the remaining 24-
function tail (category left as the pre-existing `graphics` placeholder
- see "Open question" below).

## Phase 2 planning: the remaining 24 functions

`0x08010E14`-`0x080119A8` (2964 bytes total), immediately followed by
the already-matched `actor_part39.c` (`sub_80119A8`, GitHub issue #16).
Sizes below are exact (from address deltas between `thumb_func_start`
labels in the trimmed `asm/code_3_2_17_e560_10d54.s`). "Known" notes
come from existing extern declarations/doc mentions found elsewhere in
the codebase (`grep` cross-reference only - none of these 24 were read
branch-by-branch this pass, per Phase 1's scope).

| Address | Size | Known from elsewhere |
|---|---|---|
| `sub_8010E14` | 24 B | `void sub_8010E14(void *arg0, s32 arg1)` - called as `sub_8010E14(self+0x108, 2)` from `actor_part15.c`. Sibling of `sub_8010D54`/`sub_8010E2C` (see above) - mode-parameterized queue insert. |
| `sub_8010E2C` | 8 B | `void sub_8010E2C(void *arg0)` - called as `sub_8010E2C(self+0x108)` from `actor_part77.c`, "clears its trailing `+0x108`/`+0x10c` fields" - queue reset (`count`+`unk4`). |
| `sub_8010E34` | 120 B | No existing cross-reference found. |
| `sub_8010EAC` | 224 B | `docs/rom_map.md`: part of "the randomized-behavior... famil[y]" alongside `sub_8016048`. |
| `sub_8010F8C` | 392 B | `docs/rom_map.md`: "a bounds-checked, mode-selected object state machine that self-destructs off-screen" - default mode reads `gStaticData_0816A820` (shared trig table), a rotating/orbiting projectile-or-hazard behavior. |
| `sub_8011114` | 164 B | `struct actor *sub_8011114(u16 arg0, u16 arg1, u16 arg2, s32 arg3)` - spawns a part-object; extern in `game_loop29.c`. |
| `sub_80111B8` | 144 B | `void sub_80111B8(void *part)` - extern in `game_loop29.c`; `game_loop14.c` notes a sibling call site uses `sub_8011870` "instead of `sub_80111B8`" (mutually-exclusive alternative behavior). |
| `sub_8011248` | 124 B | No existing cross-reference found. |
| `sub_80112C4` | 44 B | No existing cross-reference found. |
| `sub_80112F0` | 4 B | No existing cross-reference found - tiny, likely a trivial accessor/tail-call stub. |
| `sub_80112F4` | 20 B | No existing cross-reference found. |
| `sub_8011308` | 8 B | No existing cross-reference found. |
| `sub_8011310` | 32 B | No existing cross-reference found. |
| `sub_8011330` | 52 B | No existing cross-reference found. |
| `sub_8011364` | 20 B | No existing cross-reference found. |
| `sub_8011378` | 16 B | No existing cross-reference found. |
| `sub_8011388` | 8 B | No existing cross-reference found. |
| `sub_8011390` | 184 B | No existing cross-reference found. |
| `sub_8011448` | 256 B | `docs/rom_map.md`: "a randomized-position spawn picker, same flavor as the documented `sub_800EAFC` randomized-behavior selector but for position rather than behavior choice." Called as `sub_8011448(entry, 1)`/`(other, 1)` from `game_loop40.c`/`game_loop49.c` for despawn. |
| `sub_8011548` | 500 B | `docs/rom_map.md`: entity-vtable-dispatched (`gStaticData_087Exxx` 93-entry family); "integrates position from velocity fields, manages a wrapping counter with mode-gated increment/decrement, and on a branch plays `PlaySfx(0xe, 0x100)` plus calls a scoring/counter candidate, `sub_8023430`." |
| `sub_801173C` | 308 B | `void sub_801173C(u16 arg0)` - the achievement/unlock-icon spawn helper; extern in `graphics_loading_21d80.c`, referenced from `game_loop14.c`/`docs/rom_map.md`. |
| `sub_8011870` | 172 B | Alternative to `sub_80111B8` (see above), called from `game_loop14.c`. |
| `sub_801191C` | 16 B | `void sub_801191C(struct actor *self)` - extern in `actor_part39.c`; also called from `game_loop14.c` alongside `sub_801173C` for a "special" 4th spawn-mode case. |
| `sub_801192C` | 16 B | No existing cross-reference found - address-adjacent to `sub_801191C`, likely a closely related tiny accessor. |

**11 functions in the middle (`sub_8011248`-`sub_8011390`, addresses
`0x08011248`-`0x08011398`) have zero existing cross-references** and
are tightly clustered (all within ~0x150 bytes, several under 32 bytes)
- the same shape as the small bit-field accessor families this
subsystem already has several of (`sub_8010A0C`-`sub_8010B68`,
`game_loop27.c`). A reasonable Phase 2 split: one group for this
accessor cluster (likely fast to characterize/match together once the
struct they operate on is identified), separate groups for the larger,
already-partially-characterized functions above and below it
(`sub_8010E34`/`sub_8010EAC`/`sub_8010F8C`/`sub_8011114`/`sub_80111B8`
as one group; `sub_8011448`/`sub_8011548`/`sub_801173C`/`sub_8011870`/
`sub_801191C`/`sub_801192C` as another), plus the confirmed
`sub_8010E14`/`sub_8010E2C` queue-accessor pair as a quick standalone
win given how well-understood they already are from this pass.

### Open question for Phase 2: category

This chunk's category is left as the pre-existing `graphics` placeholder
in `tools/report_units.py` (unchanged from before this pass) rather than
recategorized to `game_loop`, unlike every other slice of this
subsystem (issues #12/#13/#14 all recategorized on their first
examining pass). Reasoning: `sub_8010E14`/`sub_8010E2C` are called
*from* `actor_part15.c`/`actor_part77.c` (both `graphics`-categorized
files) as generic per-entity accessors, not from `game_loop`-side code
- structurally similar to how `game_loop27.c`'s own bit-field accessors
turned out to also be called from graphics-side code despite being
categorized `game_loop` themselves, so this alone doesn't settle it
either way. Left for Phase 2 to decide once more of the 24 functions are
actually read, rather than guessing from two data points.

## Cross-references

- `docs/status/game_loop.md` - `sub_8010D54`'s matched entry and the
  remaining 24-function chunk's "still raw" entry added.
- `tools/report_units.py` - old `0x08010D54` entry split into a matched
  `game_loop50.o` entry and a new `0x08010E14` entry for the remaining
  24 functions.
- `docs/matching/issue-12-physics-collision.md` /
  `docs/matching/issue-14-0x08010a0c-graphics.md` - the calling
  convention and struct fields this issue's work depended on.
- `docs/rom_map.md` - the read-only reconnaissance (`sub_0800D18C`'s
  own hand-off to `sub_8010D54`, and the `sub_8010EAC`/`sub_8010F8C`/
  `sub_8011448`/`sub_8011548` notes used in the Phase 2 table above).

## Phase 2, second parallel slice: `sub_8011448`-`sub_801192C` (the chunk's tail 6)

Two agents worked Phase 2 in parallel, each in an isolated worktree, on
non-overlapping subsets of the 24-function chunk. This slice covers the
6 largest/individually-characterized functions the Phase 2 table above
already flagged with real cross-references: `sub_8011448`, `sub_8011548`,
`sub_801173C`, `sub_8011870`, `sub_801191C`, `sub_801192C` - all 6
**matched**, confirmed by a full clean `make compare` ("La suma
coincide").

### Semantics confirmed

- **`sub_8011448`** (256B) - a randomized-position spawn/despawn picker,
  called `sub_8011448(entry, 1)`/`(other, 1)` from `game_loop40.c`/
  `game_loop49.c` for despawn. `PlaySfx(gUnknown_030012BC, 8, 0x100)`,
  then either derives a randomized `(dx,dy)` offset from `rand()`
  (`arg1` nonzero - `self->0x48 = 2`, `self->0x49` tags which of three
  `rand()`-driven bands was picked) or uses a fixed `(0x1000,0x1000)`
  offset and fires `sub_80284D4(gUnknown_03001318)` (`self->0x48 = 1`).
  Either way: `self->0x3c = 0xa0`, `self->0x30` clamped from a
  `self->0x20` table lookup at `self->0x2d*0x1c+0x16`, `self->0x25 = 1`,
  `self->0xc |= 0x10`, then calls `sub_8007174` and re-derives
  `self->x`/`self->y` plus `self->0x40`/`self->0x44` (a "distance to
  travel" pair, `-sub_80008F0(newPos<<8 - offset, 0x1400)`) from the
  results - the exact same tail shape `sub_8010EAC`/`sub_80111B8`/
  `sub_8011870` all share (see "Not integrated" below).
- **`sub_8011548`** (500B) - an entity-vtable-dispatched velocity
  integrator, dispatching on `self->0x48` (modes 0-3): mode 1/2
  integrate `self->x`/`self->y` by `self->0x40`/`self->0x44` and wrap
  `self->0x3c` (a timer/animation-phase field) by different
  thresholds/directions; on arrival (`|x|<=0x10 && |y|<=0x10` in mode 1,
  or the mode-2 wrap threshold) both fire
  `PlaySfx(gUnknown_030012BC,0xe,0x100)`, call
  `sub_8023430(gUnknown_030012C0)` (a scoring/counter candidate per
  `docs/rom_map.md`), set `self->0xc` bit 0, and - unless `self->8 ==
  0xffff` - set `self->8`'s bit in the `gUnknown_030012B4+0x108`
  collision bitmap (the same inline idiom `sub_80072D8`/`sub_8025A64`
  use). Mode 3 increments `self->0x49` each frame, and every 11th frame
  resets it and calls `sub_8025CA4(gUnknown_030012E4, self->x>>8,
  self->y>>8, 0, 1, 0)` (already-matched NAKED part-object spawner,
  `game_loop14.c`) - `self->0x4b` increments every frame too, falling
  into the same collision-bitmap tail every 10th frame. Mode 0
  (default): gated by `self->0x4a`, increments `self->0x49` or
  `self->0x4b`, wrapping `self->0x4a`'s own gate off after 32
  `self->0x4b` ticks; then, still under `self->0x48 == 0`, either
  computes a step via `gStaticData_0816A820[(self->0x49 & 0x7f)*2]` and
  `sub_80008FC` added into `self->0x50` (stored to `self->y` - a
  "rotate around a fixed center by a table-driven step" idiom, same
  table/shape as `sub_8010F8C`'s own default-mode branch) when
  `self->0x4a` is clear, or calls `sub_801192C` (below) when set. If
  `self->0x48 == 3` specifically, `self->x`/`self->y` are instead reset
  to `gUnknown_030012D8`'s own position minus a fixed
  `-0x400`/`-0xe00` (Q8) offset. Every path ends with a tail call to
  `sub_8008364(self)` (already matched, `actor_part5.c`).
- **`sub_801173C`** (308B) - the achievement/unlock-icon spawn helper.
  Extern-declared as `void sub_801173C(u16 arg0)` in
  `graphics_loading_21d80.c` (that call site only ever reads `arg0`, per
  its own doc comment), but the function's **real** signature is 4
  arguments - confirmed against its other call site,
  `sub_8025CA4` (`game_loop14.c`, NAKED, already matched):
  `sub_801173C(id, x, y, special)` where `special` is `0xFFFF` or `0`
  selecting which of two `dual_array_manager` lists
  (`gUnknown_030012F4` vs `gUnknown_030012EC`) the new part joins.
  Allocates a `0x54`-byte object (`sub_8026EDC`), re-initializes it
  (`sub_80084A4`), points its vtable at `gStaticData_087E414C`,
  re-initializes via `sub_80119EC` (`actor_part39.c`, already matched),
  stores `id`/`x`/`y` (mirrored into `+0x4c`/`+0x50` as a "home
  position" pair `sub_8011548`'s mode-3 branch reads back), joins the
  `special`-selected list, points `+0x20` at `gUnknown_030012D0`'s
  shared resource table (fixed slot `0xd2*2`, the same
  `sub_8025A64`/`sub_8025CA4` convention), tags `+0x2d = 1`, builds the
  OAM/keyframe trio, derives `+0x30` from the same
  `table[tag]->+0x16` clamp idiom `sub_8011448`/`sub_8011870` use,
  clears bits 0/5 of `+0x28`, and always tags `+0x4a`/`+0x4b` both `0`
  (the ROM's own `cmp r7,#0xff` dead-code check for a
  `sub_801191C` special case is unreachable - `r7` is a hardcoded `0`
  local here, not an argument). Finishes with the same `+0x29`
  nibble-from-`sub_8006DF8` bitfield combine `sub_8025A64`/`sub_8025CA4`
  already use, returning the new part.
- **`sub_8011870`** (172B) - the alternative to `sub_80111B8`
  (`game_loop29.c`), called from `game_loop14.c` "instead of
  `sub_80111B8`" per that file's own doc comment. Same tail shape as
  `sub_8011448`/`sub_80111B8`: `PlaySfx(gUnknown_030012BC, 8, 0x100)`,
  `self->0x48 = 1`, `self->x -= self->0x4a<<8`, `self->0x3c = 0xa0`,
  `self->0x30` clamped via the same table-lookup idiom, `self->0x25 =
  1`, re-derives `self->x`/`self->y` plus `self->0x40`/`self->0x44` with
  a fixed `-0x1000` offset on both axes, then
  `sub_80284D4(gUnknown_03001318)` (not `sub_80284A4`, unlike
  `sub_80111B8`).
- **`sub_801191C`** (16B) - a trivial leaf: `self->0x48 = 3`,
  `self->0x49 = 0xa`. Already extern-declared as `void
  sub_801191C(struct actor *self)` in `actor_part39.c`.
- **`sub_801192C`** (124B, not 16B as the Phase 1 table above estimated
  - that estimate was a Phase-1 address-delta approximation that turned
  out wrong for this one entry; corrected here from this pass's own
  direct byte read) - address-adjacent to `sub_801191C`, a small
  `self->0x4b`/`self->0x4a`-driven table helper. Copies a fixed 3-word
  table (`gStaticData_0816BF14`) onto the stack, computes `self->y` from
  a `gStaticData_0816A820[self->0x4b*4]` lookup scaled by `sub_80008FC`
  against `self->0x50` (the "home Y" `sub_801173C`/`sub_8011548` both
  write), then computes `self->x` from a second
  `gStaticData_0816A820[self->0x4b*2]` lookup scaled by `sub_80008FC`
  against the stack copy indexed by `self->0x4a-1`, added to or
  subtracted from `self->0x4c` (the "home X") depending on whether
  `self->0x4a` is 1, 2, or anything else. Called from `sub_8011548`'s
  own default-mode tail when `self->0x4a` is nonzero.

### Matching result: NAKED transcription for 5 of 6, real C for 1

`sub_801191C` is a trivial two-field-store leaf with no push/pop -
matched as plain C outright (`*((u8*)self+0x48)=3; *((u8*)self+0x49)=
0xa;`), first try.

The other 5 all closed via **NAKED transcription**, each confirmed
structurally byte-exact first via an isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pass against this file's own raw ROM bytes (disassembly
diffed instruction-by-instruction - every difference found was exactly
the expected class: `bl` branch-target encodings and external-symbol
literal-pool 4-byte values, both unresolved in an isolated, unlinked
compile and both guaranteed correct once actually linked against real
symbol addresses), then by the authoritative full clean `make
NON_MATCHING=1 report` (no warnings from this file) followed by `rm -rf
build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map &&
make compare`, which passed outright ("La suma coincide"):

- `sub_8011448`/`sub_8011870` both need `self->0x2d`'s "tag" byte alive
  in `r7` across their own `self->0x20`-table lookup (ROM's own
  `push {r4,r5,r6,r7,lr}`) - a natural plain-C compile never pressures
  gcc 2.9's allocator into using `r7` at all (`push {r4,r5,r6,lr}`, one
  register short). This is the same confirmed-unfixable `r7` hazard
  already documented at length for this subsystem (`docs/matching.md`
  technique 10: an explicit `register T x asm("r7")` pin never makes it
  into this compiler's own push/pop list).
- `sub_801173C`/`sub_8011548`'s mode-3 spawn call share the identical
  shape as the already-NAKED `sub_8025A64`/`sub_8025CA4` wrappers
  (`game_loop29.c`/`game_loop14.c`): truncated arguments held live in
  `r8`/`sb` across a `sub_8026EDC`/`sub_80084A4`/re-init call sequence,
  the `mov r_lo,r_hi`/`push {r_lo,...}` high-register save dance this
  compiler only reproduces when its own *unforced* allocator picks
  those registers - `sub_801173C` is in fact the **callee** those two
  wrappers spawn through, confirming the whole family shares one root
  cause.
- `sub_8011548` as a whole (500B, the largest function in this slice)
  additionally has its own shared "`self->0xc` bit 0 + collision-bitmap"
  tail duplicated near-identically after modes 1/2/3, each recomputing
  its own address/shift chain slightly differently depending on which
  registers survive from that mode's own preceding branch - a shape a
  natural compile collapses into one shared subroutine instead of the
  ROM's own three separately-kept inlined copies.
- `sub_801192C`'s register picks (which scratch register holds the
  reloaded `self->0x4b`/`self->0x4a` byte, and whether the ROM's own
  extra register-to-register `mov` before each `sub_80008FC` call
  argument survives) resisted both named-temporary and pointer-cached-
  field-address rephrasing - the same class of "close but gcc's own
  choice differs" gap this subsystem has hit repeatedly, not resolved by
  the techniques that worked for `sub_8006A48`/similar elsewhere in this
  project (an explicit two-register pin plus a compiler barrier), tried
  and discarded here for time.

**Toolchain gotcha hit and fixed while transcribing:** `NAKED` inline
`asm()` blocks in this codebase use **divided** (legacy) Thumb assembler
syntax - `add`/`sub`/`lsl`/`lsr`/`asr`/`and`/`orr`/`mov`/`neg`, no
trailing `s` - not the **unified** syntax spelling (`adds`/`subs`/
`lsls`/.../`rsbs rN,rN,#0`) that raw ROM disassembly text (and this
file's own working notes, copied verbatim from
`asm/code_3_2_17_e560_10d54.s`) uses. Pasting disassembly-style mnemonics
straight into a `NAKED` block fails to assemble outright (`instruction
not supported in Thumb16 mode`) since agbcc's own generated `.s` output
never emits a `.syntax unified` directive; and Thumb `NEG` (encoded as
`rsbs rN,rN,#0` in unified syntax) must be spelled `neg rN,rN` in divided
syntax specifically (`rsb` alone doesn't accept an immediate flags
suffix at 16-bit width). Confirmed by every other `NAKED` function
already in this codebase (`sub_8025A64`/`sub_8025CA4`/`sub_8010D54`/etc.)
using exactly this divided spelling - worth calling out explicitly since
it's easy to copy raw disassembly text unmodified and get a confusing
assembler error instead of a silent miscompile.

**A genuine transcription bug caught by the isolated-verification step
before it reached `make compare`:** `sub_8011548`'s first
`self->0xc`-bit/collision-bitmap tail block's literal-pool
(`gUnknown_030012BC`/`gUnknown_030012C0`/`0x0000FFFF`) was initially
placed **before** the `_080115BA: b _08011664`-equivalent label instead
of **after** it (an easy off-by-one when splitting a raw disassembly's
own `.align 2, 0` + `.4byte` block away from the label it actually
follows) - this silently produced a function 4 bytes too long (`0x1f8`
vs the ROM's own `0x1f4`) and a branch target that jumped clean over 9
extra instructions instead of skipping exactly 1. Caught by comparing
`arm-none-eabi-nm --size-sort -S`'s reported function sizes against the
Phase 1 table's own known byte counts before ever running the real
build - a fast, cheap sanity check worth doing on every multi-label
`NAKED` transcription in a file with more than one function, not just
the final `make compare`.

### Not integrated this pass

`sub_8010E34` (120B), `sub_8010EAC` (224B), `sub_8010F8C` (392B), and
`sub_80111B8` (144B) were all read, semantically characterized, and
drafted as C reconstructions this same pass (`sub_8010EAC`/
`sub_8011870`/`sub_8011448`/`sub_80111B8` share one obvious near-
identical tail shape worth noting for whoever picks these up: fixed or
randomized `(dx,dy)`, `self->0x3c`/`self->0x30`/`self->0x25`/`self->0xc`
setup, `sub_8007174`, then the `-sub_80008F0(...)` distance-pair
derivation). None were integrated into `ldscript.txt`/
`tools/report_units.py` this pass: `sub_8011114` (a fifth member of the
same address-contiguous group, called from `game_loop29.c`, already
extern-declared) sits physically between `sub_8010F8C` and `sub_80111B8`
in ROM order and was left for a sibling parallel session's own pass over
the chunk's remaining/small-accessor functions - splitting this file's
own carved range around it would need a second new C file, and this
session's two-parallel-agent convention reserved a single collision-free
filename (`game_loop53.c`) for this slice specifically to avoid the kind
of `ldscript.txt`/asm-fragment merge collision earlier passes in this
same session hit repeatedly. Left as a clearly-flagged, ready-to-pick-up
follow-up rather than guessed at or force-fit around the constraint.

### Cross-references (this section)

- `docs/status/game_loop.md` - matched entry for this slice added, and
  the "still raw" 24-function entry shrunk to the remaining 18.
- `tools/report_units.py` - new `0x08011448` entry (`game_loop53.o`);
  the `0x08010E14` entry's own note updated for the reduced range.
- `docs/matching.md` - technique 10 (`r7` hazard) and the high-register
  save/restore idiom (technique 8), both reconfirmed here.
