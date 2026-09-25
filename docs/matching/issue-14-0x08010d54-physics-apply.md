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
