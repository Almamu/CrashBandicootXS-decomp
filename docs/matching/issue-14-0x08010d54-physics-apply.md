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

## Phase 2 update: "quick win" (sub_8010E14/sub_8010E2C) and "accessor cluster" (sub_8011248-sub_8011390) groups closed

Two of Phase 2's own proposed groups (see the planning table above),
worked as a single combined pass since both were expected to be
small/fast. Both fully matched; confirmed by a full clean `make
NON_MATCHING=1 report` (no warnings) followed by `make compare`
("La suma coincide").

### `sub_8010E14`/`sub_8010E2C` - folded into `game_loop50.c`

Both trivially small (24B/8B) and already semantically understood from
Phase 1's own pass (see above). Reading the raw bytes confirmed:

- `sub_8010E14(void *arg0, s32 arg1)`:
  ```c
  void sub_8010E14(void *arg0, s32 arg1)
  {
      if (arg1 & 1) {
          sub_8026ED0(arg0);
      }
  }
  ```
  This is **byte-identical in shape** to an already-matched function
  elsewhere in the codebase - `src/graphics/graphics.c`'s own
  `sub_8006AF4(void *arg0, u32 arg1) { if (arg1 & 1) { sub_8026ED0(arg0); } }`.
  Not a "mode-parameterized insert" after all (Phase 1's own guess,
  written before this function was read branch-by-branch) - `arg1`
  gates a VRAM-upload-manager refresh (`sub_8026ED0`, matched
  elsewhere), unrelated to the insert-mode idea. `actor_part15.c`'s own
  call site (`sub_8010E14(self + 0x108, 2)`) passes `arg1 = 2`, whose
  bit 0 is clear - so that specific call is itself a no-op at runtime,
  though it still confirms `self` is the same `struct collision_queue`
  `sub_8010D54` operates on.
- `sub_8010E2C(void *arg0)`: `self->count = 0; self->unk4[0] = 0;` -
  matches Phase 1's own prediction exactly (a two-field reset, only the
  first byte of `unk4` cleared despite the field being reserved as
  `u8 unk4[4]`).

Both matched as plain, unremarkable real C on the first isolated
compile pass (no register pinning needed). New object `src/system/game_loop50.o`'s
own `.text` grows to include these two (contiguous, no gap, since they
immediately follow `sub_8010D54` in the ROM); `asm/code_3_2_17_e560_10d54.s`
trimmed further to begin at `sub_8010E34`.

### `sub_8011248`-`sub_8011390` - new `src/system/game_loop52.c` (11 functions)

Zero prior cross-references (per Phase 1's own table). Reading all 11
functions' raw bytes at once (as planned) revealed a shared struct
immediately: `sub_8011364` (seed anchor+position) and `sub_8011248`
(per-frame position update) both touch `self+0x0`/`self+4` (current
Q8 x/y) and `self+0x4c`/`self+0x50` (Q8 anchor x/y) with the exact same
offsets, and `sub_8011378`/`sub_8011388`/`sub_8011330`/`sub_8011390`
all touch the adjacent `self+0x48`-`self+0x4b` byte run - a small
**"orbiting hazard" behavior** family on a further still-unnamed "part"
object (distinct from `struct actor`'s own 0x1c bytes and from
`sub_8010D54`'s own `struct collision_queue`):

| Function | Size | Role |
|---|---|---|
| `sub_8011248` | 124B | Per-frame orbit-position update: two lookups into the shared sine table `gStaticData_0816A820` (`self+0x4b`'s phase, at strides `*4` and `*2`), combined via the overflow-avoiding fixed-point multiply `sub_80008FC` (already matched, `math_util.c`) - `self+4` (`y`) is always anchor-y minus the y-offset; `self` (`x`) is anchor-x minus/plus the x-offset depending on `self+0x4a` (mode 1/2), or just the anchor x unchanged for any other mode value. |
| `sub_80112C4` | 44B | Re-derives visibility via `sub_8007A84(gUnknown_030012CC, self)` (already matched), clears flags bit 3 when `self+0x38` is nonzero. |
| `sub_80112F0` | 4B | Trivial - always returns 2. |
| `sub_80112F4` | 20B | Repoints `self->table` at `gStaticData_087E40DC`, tail-calls `sub_8008484` (already matched) with `self`+its own 2nd argument passed through. |
| `sub_8011308` | 8B | Clears the "spawned/active" gate byte `self+0x48`. |
| `sub_8011310` | 32B | `sub_80084A4(self)` (already matched, return discarded) + table repoint (`gStaticData_087E40DC`) + `sub_8011308(self)`; returns `self`. Same init/reset/table-repoint trio shape as `actor_part8.c`. |
| `sub_8011330` | 52B | If `self+0x48 == 0` and the player's `+0xc` bit 7 is set, fires `self->table+0x68/0x6c`'s trampoline (`sub_803AD7C`, already matched) - the usual "offset + fn pointer" pair convention. Always returns 0. |
| `sub_8011364` | 20B | Seeds `self`/`self+4` (Q8 x/y) from raw `x`/`y` arguments (`<<8`), mirrors both into `self+0x4c`/`self+0x50` (the orbit anchor). |
| `sub_8011378` | 16B | Sets orbit mode (`self+0x4a`), resets orbit phase (`self+0x4b`) to 0. |
| `sub_8011388` | 8B | Unexamined byte setter, `self+0x49` - address-adjacent to the mode/phase pair but not read by anything else in this group. |
| `sub_8011390` | 184B | Per-frame player-proximity/hit-resolve step: gated by the same orbit-mode/phase fields plus flags bits 2/3 (`self+0xc`), AABB-tests `self` against the player (`gUnknown_030012D8`) - primary AABB (`sub_8007C30`) when the player's own `+0xa == 0x13`, secondary AABB (`sub_8007B98`) otherwise - and on overlap sets flags bit 3 and tail-calls the despawn picker `sub_8011448` (Phase 2's neighboring group, not read this pass - only extern'd) with a mode that differs per path, playing a hit SFX only on the primary-AABB path. |

All matched as real C except `sub_8011248`, closed as a NAKED
transcription: a plain-C reconstruction reproduces the ROM's exact
*shape* (same struct-copy local via `ldm`/`stm`, same two
`gStaticData_0816A820` lookups at the right strides and program-order
position, same mode-1/mode-2/else branch structure) but gcc 2.9 -O2
persistently picks the opposite register/operand order for the two
`table + phase*stride` pointer adds (`adds r0, r4, r0` instead of the
ROM's own `adds r0, r0, r4`) no matter how the C source phrases the
addition - pointer-arithmetic normalizes the pointer operand first
internally in this compiler, so this isn't one isolated register letter
to pin (same category of gap as `sub_8010D54` itself, Phase 1 above).

Two smaller gcc-2.9 quirks recurred across the *real-C* functions in
this group and needed the project's established register-pinning/
opaque-materialization toolbox:

- `self[0xc] &= ~8` (a byte-sized bit-clear) compiles as a single
  folded `mov r0, #0xf7` immediate by default, but the ROM computes the
  full 32-bit `~8` mask at runtime instead (`movs r0, #9; rsbs r0, r0,
  #0`) - not something a byte-typed mask needs, but apparently how this
  particular call site's own source was phrased. Fixed via a
  `register s32 mask asm("r0") = 9; mask = -mask;` opaque
  materialization (`sub_80112C4`, and the two `self[0xc] |= 8` sites in
  `sub_8011390`, same idiom with `+=`/`|=` swapped appropriately) -
  same technique as the project's established `matching_decomp_register_pinning`
  memory point.
- `self[0x4a] = mode; self[0x4b] = 0;`-shaped byte-pair setters
  (`sub_8011378`) compile the `0` constant lazily, right before its own
  use, but the ROM materializes it *earlier*, between the address
  computation and the first store, reusing the same scratch register
  for both stores' worth of bookkeeping. Fixed by writing the local
  `zero` variable's assignment as an explicit statement positioned
  between the address computation and the first store (source order
  controls scratch-register lifetime for this compiler beyond just the
  final `-O2` schedule).
- `sub_8011364`'s trailing `self->anchor = self->pos;` mirror reads
  compile away entirely (the optimizer sees `self->pos` was *just*
  written with a known value and reuses the register instead of
  reloading) but the ROM does reload from memory - fixed via
  `*(volatile s32 *)self` casts on the two mirror reads, forcing real
  `ldr` instructions.
- `sub_8011390`'s own trailing byte-padding: its body isn't a multiple
  of 4 bytes and gcc's own default function-end alignment padding is a
  `nop`/`mov r8, r8` instruction, not the ROM's own zero-byte padding -
  fixed with an explicit `asm(".align 2, 0");` after the function body,
  the same `matching_decomp_alignment_fix` technique already used
  elsewhere in this codebase (e.g. `game_loop27.c`'s `sub_8010AEC`).

`asm/code_3_2_17_e560_10d54.s` further trimmed to end at `sub_80111B8`
(its own tail, `sub_8011448`-`sub_801192C`, split off into the new
`asm/code_3_2_17_e560_11448.s` since it's no longer address-adjacent to
the file's own remaining front half once the middle was carved out).
New object `src/system/game_loop52.o` inserted between the two in
`ldscript.txt`. Categorized `actor` in `tools/report_units.py` (not
`game_loop`): this is per-entity behavior state on the "part" object
family, not the shared physics/collision-queue infrastructure
`sub_8010D54`/`sub_8010E14`/`sub_8010E2C` operate on - unlike those
three, nothing here touches the `self+0x108` queue or is called from
the `sub_0800D18C`/`sub_800E08C` dispatch chain.

### Cross-references (this update)

- `docs/status/game_loop.md` - both groups' matched entries added, the
  "still raw" entry split into its two remaining pieces.
- `tools/report_units.py` - the `0x08010D54` entry's comment extended;
  the old `0x08010E14` raw entry split into `0x08010E34` (still raw),
  `0x08011248` (`game_loop52.o`, matched), and `0x08011448` (still raw).
- `src/graphics/actor_part15.c` / `src/graphics/actor_part77.c` - the
  external call-site confirmation for `sub_8010E14`/`sub_8010E2C`
  already used in Phase 1, re-verified against the actual instruction
  bytes this pass.
- `src/graphics/graphics.c` - `sub_8006AF4`, the already-matched twin
  shape that confirmed `sub_8010E14`'s own semantics.

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

## Final mop-up: `sub_8010E34`/`sub_8010EAC`/`sub_8010F8C`/`sub_8011114`/
`sub_80111B8` matched - **entire chunk closed**

The 5 functions the "Not integrated this pass" section above left as
drafted-but-unwired (`sub_8010E34`, `sub_8010EAC`, `sub_8010F8C`,
`sub_80111B8`, plus `sub_8011114` which two sibling parallel sessions
had deliberately left untouched since it sits physically between
`sub_8010F8C` and `sub_80111B8` and its own filename-collision
avoidance convention hadn't reserved a slot for it) are now all matched,
in a new file `src/system/game_loop54.c` (the next available
`game_loopNN.c` slot after this session's `50`-`53`). **This closes the
entire `0x08010D54` physics/collision-apply chunk (GitHub issue
#12/#14): every function between `sub_8010D54` and the already-matched
`src/graphics/actor_part39.c` (`sub_80119A8`) is now matched.**
`asm/code_3_2_17_e560_10d54.s` is fully consumed and removed from the
tree; its `ldscript.txt` line is replaced by `game_loop54.o` (inserted
between `game_loop50.o` and `game_loop52.o`, its own correct ROM-order
position).

### Semantics confirmed

- **`sub_8010E34`** (120B) - a bounds-checked AABB gate, previously
  without any cross-reference. Reading it directly revealed it shares
  its *entire* opening gate verbatim with `sub_8011390`
  (`game_loop52.c`, already matched): `if (self->0x4a != 0 &&
  self->0x4b <= 0x16 && player->0x88 != 3) return;` followed by the same
  flags-bit-3-clear/bit-2-set test (compiled via the
  `(u32)flags << 0x18` shift-extract idiom, not a direct `& 8`/`& 4` -
  confirmed this is genuinely how gcc 2.9 -O2 compiles this exact
  bit-test phrasing in this codebase, not a hand-picked quirk). Past the
  gate: builds `self`'s own AABB and the player's AABB via two
  `sub_8007B98` calls (in that order - `self` first), tests overlap via
  `sub_8001688`, and on overlap sets flags bit 3 and calls
  `sub_8010EAC(self, 0)` - i.e. this is `sub_8010EAC`'s own player-
  proximity trigger, the "randomized-behavior family"'s entry point.
- **`sub_8010EAC`** (224B) - `docs/rom_map.md`'s "randomized-behavior"
  family sibling of `sub_8016048`, confirmed as one more member of the
  "(dx,dy) offset then distance-pair" tail shape shared with
  `sub_8011448`/`sub_8011870`/`sub_80111B8`: plays a hit SFX, sets
  `self->0x3c = 0xa0`, then either derives a randomized `(dx,dy)` from
  `rand()` (`randomize` nonzero - three `rand()`-driven bands select the
  x-offset, `self->0x49` tags which one, `self->0x48 = 2`) or uses a
  fixed `(0xb400, 0xc00)` offset and fires
  `sub_80284A4(gUnknown_03001318)` (`self->0x48 = 1`); either way,
  `self->0xc |= 0x10`, `self->0x25 = 1`, `sub_8007174(...)`, then
  `self->0x40`/`self->0x44` become `-sub_80008F0(newPos<<8 - offset,
  0x1400)`.
- **`sub_8010F8C`** (392B) - the bounds-checked, mode-selected
  rotating/orbiting hazard state machine `docs/rom_map.md` already
  flagged. Mode 1: integrates position by velocity, and once inside
  screen bounds (`|x|<=0xb4`, `|y|<=0xc`) plays a hit SFX, calls
  `sub_8023464(gUnknown_030012C0)`, sets flags bit 0, and (unless
  `self->8 == 0xffff`) sets `self->8`'s bit in the
  `gUnknown_030012B4+0x108` collision bitmap. Mode 2: integrates
  position, wraps a `self->0x3c` timer up or down depending on
  `self->0x49`, and on wrap fires the identical
  PlaySfx-less-but-otherwise-same trigger tail. Any other mode (0, or
  3+): gated by `self->0x4a`, advances `self->0x49`/`self->0x4b`
  (wrapping the gate off after 32 ticks). Shared tail: unless
  `self->0x48 != 0`, computes an orbit step via
  `gStaticData_0816A820[self->0x49 & 0x7f]` and `sub_80008FC` added into
  `self->0x50`, stored to `self->y`, when `self->0x4a` is clear, or
  calls `sub_8011248` (`game_loop52.c`'s orbit-position updater) when
  set - then always tail-calls `sub_8008364`.
- **`sub_8011114`** (164B) - `struct actor *sub_8011114(u16 arg0, u16
  arg1, u16 arg2, s32 arg3)`, the part-object spawn helper
  extern-declared in `game_loop29.c`. Confirmed `arg3` is genuinely
  dead - the ROM hardcodes the three fields it would otherwise feed
  (`self+0x29`/`+0x2a`/`+0x2b`) to a compile-time `0` regardless,
  matching the extern's own always-`0` call sites. Allocates a
  `0x54`-byte object, re-initializes it, repoints `self->table` at
  `gStaticData_087E40DC`, clears the "spawned/active" gate
  (`sub_8011308`), stores `arg0` at `self+8` and `arg1`/`arg2` (Q8) at
  `self+0`/`self+4` mirrored into the orbit anchor
  `self+0x4c`/`self+0x50`, joins the `gUnknown_030012EC`
  `dual_array_manager` list, derives `self+0x30` from the
  `table[self->0x2d]->+0x16` clamp idiom, clears bits 0/5 of
  `self+0x28`, and returns the new part.
- **`sub_80111B8`** (144B) - `void sub_80111B8(void *part)`, confirmed
  as the documented "mutually exclusive alternative" to `sub_8011870`
  (`game_loop53.c`) - reading both side by side, `sub_80111B8` is
  notably *simpler*: it has no `self->0x3c`/`self->0x30` table-lookup-
  clamp setup at all, just `self->0x48 = 1`, `self->x -=
  self->0x4a<<8`, `self->0x25 = 1`, `sub_8007174(...)`, the same
  distance-pair derivation with fixed `(0xb400, 0xc00)` offsets, then
  `sub_80284A4(gUnknown_03001318)` (not `sub_80284D4`, unlike
  `sub_8011870`).

### Matching result: 3 of 5 real C, 2 NAKED

Unlike the previous Phase 2 slice (5 of 6 NAKED), most of this final
slice closed as **real C** - `sub_8010E34`, `sub_8010EAC`, and
`sub_80111B8`. Neither uses `r7`/`r8`/`sb` at all (all three push only
`r4`-`r6`), so the confirmed-unfixable `r7` hazard documented at length
for their siblings (`sub_8011448`/`sub_8011870`) simply doesn't apply
here - the extra table-lookup-clamp section that drags `r7` into the
picture for those two is entirely absent from `sub_8010EAC`/
`sub_80111B8`.

Three genuine gcc-2.9 -O2 codegen-order quirks were hit and fixed with
this project's established register-pinning/opaque-materialization
toolbox (not new techniques - direct re-applications of
`matching_decomp_register_pinning`):

- **`sub_8010EAC`'s prologue**: a plain `u8 *self = selfArg;` as the
  first statement still let gcc schedule the `randomize` parameter's
  8-bit truncation *before* the `self` register copy, opposite the
  ROM's own order. Fixed with `asm volatile("" : "+r"(self));`
  immediately after the assignment - an empty compiler barrier forcing
  `self`'s materialization to actually happen at that program point
  rather than being freely reordered.
- **`sub_8010EAC`'s `self->0xc |= 0x10;`/`self->0x25 = 1;` pair**: gcc
  naturally loads the existing field value before materializing the
  small integer constant; the ROM does the opposite (constant into `r0`
  first, then the field load/address computation second). Fixed with
  `register s32 mask asm("r0") = 0x10;` / `register u8 one asm("r0") =
  1;` pins, forcing constant-first evaluation order - the same
  established idiom as `game_loop52.c`'s `sub_80112C4`, just for a
  plain positive immediate instead of a negated one.
- **`sub_80111B8`'s `self->x -= self->0x4a<<8;`**: the ROM loads
  `self->0x4a` directly into the same register that held its own
  address (dead after the load), then shifts the result into a
  *different* register, freeing the first for reuse holding `self->x`'s
  current value. A natural plain-C compile picks the opposite pairing.
  Fixed by explicitly splitting the load and shift into two named
  locals, each pinned to a specific register: `register s32 off
  asm("r0") = self[0x4a]; register s32 shifted asm("r1") = off << 8;`
  then `*(s32 *)self -= shifted;` - a two-register pin, one step beyond
  the single-register pins used elsewhere in this codebase, needed
  because both the source and destination registers of the shift
  mattered to the byte match, not just the shift's own presence.
- A **sign bug caught by the isolated-verification step**: an early
  draft wrote the fixed offsets in `sub_80111B8` as the raw 32-bit
  pool-word patterns seen in the ROM's own literal pool
  (`newX - 0xFFFF4C00`), not realizing those patterns are already the
  *negative* two's-complement encoding (`0xFFFF4C00 == -0xb400`) that
  the ROM's own `adds` (not `subs`) instruction then adds back - i.e.
  the actual arithmetic is `newX - 0xb400`, and writing the raw pool
  word literal computed `newX + 0xb400` instead (double-negated, wrong
  by `2*0xb400`). Caught immediately by the isolated `cmp`/disassembly
  diff (gcc synthesized a `movs`/`lsls` positive-immediate sequence
  instead of a literal-pool load, a dead giveaway of a wrong-sign
  constant) before it ever reached `make compare` - worth flagging
  since the raw ROM `.4byte` pool value and the "correct C literal to
  write" are not the same number whenever the ROM's own compile chose
  `adds`-with-negative-literal over `subs`-with-positive-immediate for
  an oversized immediate.

`sub_8010F8C` and `sub_8011114` both closed via **NAKED transcription**,
verified structurally byte-exact via the isolated `cpp`/`agbcc`/`as` +
`objcopy`/disassembly-diff pass (every difference found was exactly the
expected class: `bl` targets and external-symbol literal-pool values,
both unresolved in an isolated unlinked compile) before the
authoritative full clean `make NON_MATCHING=1 report` (no warnings)
followed by `make compare` ("La suma coincide"):

- `sub_8010F8C` duplicates its own "PlaySfx+`sub_8023464`+collision-
  bitmap" trigger tail twice (once per arrival mode), each with a
  different register allocation surviving from that mode's own
  preceding branch - notably mode 1's copy opportunistically reuses
  `r5` (still holding the just-tested `self->0x48 == 1` mode value) as
  the literal `1` for its `1 << bit` shift, which a natural compile of
  the same C logic can't be coaxed into reproducing since it's a
  coincidence of *which* mode is being tested, not something expressible
  as source-level intent. Exactly the same "shared tail duplicated with
  different register survivors" shape already documented for
  `sub_8011548` (`game_loop53.c`).
- `sub_8011114` needs a `0` sentinel alive in `r8` across the
  `sub_8026EDC`/`sub_80084A4`/`sub_8011308` call sequence purely so it
  can later be spilled back out for three trailing byte stores - the
  same confirmed `mov r_lo,r_hi`/`push {r_lo,...}` high-register
  save/restore dance this compiler only reproduces when its own
  *unforced* allocator picks those registers itself, already documented
  for `sub_801173C`/`sub_8011548` (`game_loop53.c`) - `sub_8011114` is
  in fact one of the functions those two ultimately spawn through,
  reconfirming the same root cause a third time in this one chunk.

### Cross-references (this section)

- `docs/status/game_loop.md` - the chunk's former "still raw" entry
  replaced with `game_loop54.c`'s matched entry, explicitly flagged as
  closing the entire chunk.
- `tools/report_units.py` - the `0x08010E34` entry now points at
  `game_loop54.o`/category `game_loop`; the `0x08011448` entry's own
  "NOT integrated" note updated to point at the new file.
- `ldscript.txt` - `asm/code_3_2_17_e560_10d54.o` removed entirely,
  replaced by `game_loop54.o` in the correct ROM-order position (between
  `game_loop50.o` and `game_loop52.o`).
- `src/system/game_loop29.c` - the `sub_8011114`/`sub_80111B8` extern
  declarations and call-site context used to confirm both signatures.
- `src/system/game_loop52.c`/`game_loop53.c` - the sibling functions
  (`sub_8011390`, `sub_8011448`/`sub_8011548`/`sub_8011870`) whose
  already-documented gate/tail/register-hazard shapes this pass reused
  or explicitly confirmed did *not* apply.
