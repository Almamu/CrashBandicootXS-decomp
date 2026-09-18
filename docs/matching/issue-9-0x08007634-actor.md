# Issue #9: 0x08007634-0x0800B3F0 - the `actor` "part" object family

GitHub issue #9 (`decomp-chunk`, labeled `game_loop` by the chunk
generator) listed 18 raw functions across `asm/code_3_2.s`,
`asm/code_3_2_11.s`, `asm/code_3_2_13.s`, `asm/code_3_2_14.s`, and
`asm/code_3_2_16.s`. This is the write-up for the work done against
that list.

## Category correction: `game_loop` -> `actor`

Every function in this chunk sits directly between already-matched
`src/graphics/actor_part*.c` files (`actor_part9.c`/`actor_part11.c`
before it, `actor_part14.c`/`actor_part15.c` interleaved and after it)
- the same "part" object family `docs/status/actor.md` has tracked
since `actor_part2.c`. The chunk generator's `game_loop` label is
stale/inherited, not re-derived from the actual function content (the
same mistake already documented for GitHub issue #12 in
`docs/matching/issue-12-physics-collision.md`). `tools/report_units.py`
already tracks this whole neighborhood as category `graphics` (the
label this project's `actor_part*.c` family has used since
`actor_part2.c` - not a new choice made here), so this chunk's new
units use that same category rather than `game_loop`.

## Prior history: this exact range was already assessed, twice

Both `docs/matching.md` (the frozen historical log) and
`docs/status/actor.md` already discuss most of these 18 functions by
name, from earlier sessions working the surrounding `actor_part11.c`-
`actor_part17.c` clusters:

- `sub_8007634`: previously read at a high level and left entirely
  untouched - real GBA hardware affine (rotation/scaling) sprite-matrix
  setup, judged too large and unexamined to reconstruct with confidence
  in a single pass (`docs/matching.md`'s `sub_8007634` entry).
- `sub_8009008`/`sub_80091D4`/`sub_8009868`/`sub_8009BE0`: each
  previously read and explicitly left raw - "complex spatial-hash-grid
  removal logic ... higher-level why isn't recoverable without more
  context" (`sub_8009008`), "complex list-management logic" 
  (`sub_80091D4`), "calls still-unexamined helpers" (`sub_8009868`), "a
  physics/collision step-probe ... left raw rather than guess"
  (`sub_8009BE0`).
- `sub_800A0FC`-`sub_800A590`: flagged as "a raw span ... left raw
  rather than guess at semantics" when `actor_part14.c` was matched.
- `sub_800A734` onward: flagged as "the start of a still-raw 94 KB
  span" in the same pass.

This session re-read all 18 with the additional context accumulated
since (the `+0x8c`/`gUnknown_0300082C` frame-counter convention, the
`+0x88`/`+0xac`/`+0xb0` field family cross-referenced across
`sub_800A734`/`sub_800A810`/`sub_800AFF4`/`sub_800A884`, and the
established register-pin/goto idioms this file family needs) and found
6 of the 18 tractable enough to fully understand and reconstruct in C.
The other 12 remain genuinely hard for the reasons the prior sessions
already gave - see "Left untouched" below.

## Matched - 1 function

- **`sub_800A810`** (`src/graphics/actor_part48.c`, right after the
  still-parked `sub_800A734` in the same file): dispatches a sub-state
  byte (`self+0x88`) to one of three teardown helpers
  (`sub_8015840`/`sub_80159A4`/`sub_8017994`), each called with the
  same `self+0x44` "record" argument `sub_800A730` already established,
  after resetting the usual velocity/state fields. Matched with `self`
  pinned to `r3` (kept live across all three `bl` calls); the two
  AND-mask field clears (`self+0x28` bit 5, `self+0xc` bit 3) built via
  the "negative-constant register-pinned mask" idiom (`register s32
  mask asm("r0") = -0x21;`/`-9;`, forcing the ROM's own runtime
  `mov`+`neg` pair instead of a folded 8-bit AND immediate); the
  `self+0x68`-to-`self+0x28` field address expressed as a single
  decremented `u8 *p` cursor (`p -= 0x40;`) instead of a fresh
  computation, reproducing the ROM's own `subs r1, #0x40` register
  reuse; and the whole dispatch rewritten with explicit `goto`s
  (`if (state == 1) goto do1; if (state > 1) goto gt1; if (state == 0)
  goto do0;`) plus a second register-pinned copy of the state byte
  (`register s32 state2 asm("r1") = state;`, read once up front) for the
  inner `state2 == 2`/`state2 == 3` checks - this is what finally
  reproduced the ROM's exact `beq`/`bgt`/`beq` chain and its `adds r1,
  r0, #0` copy; every `if`/`else if`/`switch` phrasing tried previously
  collapsed the branch polarity to `bne`-skip and let the compiler's CSE
  drop the redundant copy entirely (see "Real gotchas" below for the
  general form of this pin-scope lesson).

## Parked (`NON_MATCHING`, not yet byte-exact) - 5 functions

All five are fully understood (every field offset, branch, and call
confirmed against the ROM) but don't yet produce byte-identical output.
(`sub_800B270` was mislabeled "Matched" in this write-up's first
version even though its own source was already `#if NON_MATCHING` and
its "Real gotchas" entry below already described it as parked - fixed
here; it stays genuinely parked, see below.)

- **`sub_800A528`/`sub_800A590`** (`src/graphics/actor_part47.o`, new
  file; real bytes in `asm/code_3_2_11_a528.s`) - a moving-platform
  "ride along" hookup: looks up a position record via a
  `self->table+0x10/0x14` trampoline and, if it changed since the last
  call, nudges `self->y` by the delta between the old and new record's
  position (interpreted differently depending on `self+0x68`'s state,
  8 or 4). `sub_800A528` is the same body with an extra unconditional
  `sub_8009FB0(self)` call first. Every load/store/branch confirmed
  correct; `self` and the returned record pointer pinned to `r4`/`r3`
  matching the ROM exactly. The remaining gap: the ROM's inner
  scratch-register use for the record's `+2`/`+5` field reads needs a
  genuine *fifth* register (`r5`) purely to hold an offset immediate,
  since `r0`-`r3` are all already committed to real values at that
  point - no C phrasing tried (separate statements per load, explicit
  `register ... asm("r5")` pins on the offset constant, alternate
  operand orders) makes this compiler introduce that fifth register;
  it always finds a way to reuse `r0`-`r3` instead (a *smaller*
  register footprint than the ROM's own, ironically - no `push
  {r4,r5}` needed - but not the same bytes).
- **`sub_800A734`** - **UPDATE: matched in a later session, see
  `docs/matching/issue-14-0x08010a0c-graphics.md`'s "Follow-up"
  section** for the techniques that closed the gap described below.
  (`src/graphics/actor_part48.c`; real bytes were in
  `asm/code_3_2_16_a734.s`, now removed) - a part-object velocity/state
  reset that
  additionally hooks up a child object at `self+0xb0` (calls
  `sub_800815C` on it, packs the result's low nibble into the child's
  `+0x29` byte) and zeroes the `+0x100`-`+0x105` per-phase flag bytes
  `actor_part15.c`'s doc comment already describes. Field writes
  confirmed correct one-for-one against the ROM; the ROM builds several
  field addresses as one running pointer incremented by small relative
  offsets across a long stretch of otherwise-unrelated-looking writes,
  which no source restructuring reproduced. Its sibling `sub_800A810`,
  right after it in ROM order, *was* matched this way (a `u8 *p` cursor
  plus register-pinned negative-constant masks) - see "Matched" above -
  but that technique only closed `sub_800A810`'s gap; re-applying the
  same cursor idea to `sub_800A734`'s much longer, less uniform stretch
  of field writes (`+0x24`/`+0x44`/`+0x78`/`+0x1c`/`+0x90`/`+0xac`/
  `+0x80`/`+0x88`/`+0x8c`, several via `subs` as well as `adds`) did not
  reproduce the ROM's exact increment sequence in the same pass; left
  parked rather than force it.
- **`sub_800B270`** (`src/graphics/actor_part49.c`, new file): a
  per-frame velocity integrator. Moves `self+0x60`/`self+0x64`
  (current X/Y velocity) toward `self+0x50`/`self+0x5c` (target X/Y
  velocity) by up to `self+0x4c`/`self+0x58` (X/Y acceleration step)
  each call, clamping at the target instead of overshooting. Derives a
  `self+0x24` direction-flag byte from the resulting velocity's sign,
  snapshots the pre-move position into `self+0x6c`/`self+0x70`, applies
  velocity to `self+0`/`self+4` (position), and records the resulting
  Y velocity into an unlabeled RAM address (`0x0300129C` - no symbol
  found anywhere else in this ROM, kept as a raw address rather than
  inventing a name). Needed `self`/`v`/`target`/each clamp `result`
  pinned to `r2`/`r1`/`r3`/`r0` respectively, the flags-byte pointer
  pinned to `r1`, the OR-mask pinned to `r0`, an explicit `goto`-based
  merge for the Y-axis flag write, and `vs32`-cast forced reloads
  matching the ROM's own redundant re-reads of `self->x`/`self->y`
  right before applying velocity to them - every instruction up to and
  including the position-update store matches one-for-one. The
  remaining gap is confined to the trailing 14-instruction `0x0300129C`
  block (see "Real gotchas" below for the specific register-pin-scope
  reason); re-attempted in a later session with several more register-
  pin combinations on the same block (address pinned to `r0` alone,
  value pinned to `r2` alone, both together, a `vs32`-qualified
  pointer, a plain unpinned local) and all either reproduced the same
  address/value register swap or reintroduced the `push {r4}`
  regression - still parked on this one block.

## Left untouched (raw) - 12 functions

- **`sub_8007634`** (`asm/code_3_2.s`, ROM `0x08007634`, ~1044 B) -
  real GBA hardware-affine (rotation/scaling) sprite-matrix setup: reads
  a Q8 "scale" factor from `part+0x3c`, allocates a rotation-group index,
  selects OBJ mode 1/3 based on scale, and blends a cached "previous"
  position against the current record's position by the scale fraction.
  Already flagged in `docs/matching.md` as needing "a dedicated session"
  of its own; not attempted again here for the same reason - a guessed
  reconstruction of real-time affine-matrix math risks leaving wrong
  documentation behind, worse than leaving it unclaimed.
- **`sub_8009008`** (`asm/code_3_2_13.s`, ROM `0x08009008`) - spatial-
  hash-grid node removal built on the `sub_8008F20` pool-manager struct;
  a two-phase bucket search whose higher-level "why" (as opposed to the
  mechanical "what") isn't recoverable without more context on the
  manager struct's callers.
- **`sub_80091D4`** (`asm/code_3_2_13.s`, ROM `0x080091D4`) - list-
  management logic in the same still-unclear manager struct family.
- **`sub_8009868`** (`asm/code_3_2_13.s`, ROM `0x08009868`) - calls
  still-unexamined `sub_800D040`/`sub_80109A4`; per
  `docs/matching/issue-12-physics-collision.md`, `sub_80109A4` leads
  into the large, still-mostly-raw physics/collision subsystem
  (`sub_0800D18C` and friends) that issue explicitly left raw as "not
  understood branch-by-branch with the precision a byte-exact
  reconstruction needs".
- **`sub_8009BE0`** (`asm/code_3_2_14.s`, ROM `0x08009BE0`) - a
  physics/collision step-probe calling still-unexamined
  `sub_8008278`/`sub_8026628`.
- **`sub_800A0FC`** (`asm/code_3_2_11.s`, ROM `0x0800A0FC`) - calls the
  raw `sub_800A178` and the parked `sub_8009BE0`; left raw since its
  own correctness depends on functions whose exact behavior isn't
  pinned down.
- **`sub_800A178`** (`asm/code_3_2_11.s`, ROM `0x0800A178`, ~680 B) - a
  movement-resolution function built on `sub_8008200`/`sub_8026628`/
  `sub_8026C3C`/`sub_8026BF8`, none of which are matched or precisely
  understood yet.
- **`sub_800A420`** (`asm/code_3_2_11.s`, ROM `0x0800A420`, ~264 B) -
  the same `sub_8008200`/`sub_8026BF8` dependency as `sub_800A178`.
- **`sub_800A884`** (`asm/code_3_2_16.s`, ROM `0x0800A884`, ~616 B) - a
  reentrancy-guard-shaped wrapper around `sub_800A0FC` with a two-level
  jump-table dispatch; calls the unexamined `sub_8026BC0`.
- **`sub_800AAEC`** (`asm/code_3_2_16.s`, ROM `0x0800AAEC`) - iterates
  a global list (`gUnknown_0300130C`) calling the unexamined
  `sub_8026628`/`sub_800CD00`.
- **`sub_800AB9C`** (`asm/code_3_2_16.s`, ROM `0x0800AB9C`) - calls the
  raw `sub_8009868` and the unexamined `sub_80106DC`.
- **`sub_800AC2C`** (`asm/code_3_2_16.s`, ROM `0x0800AC2C`, ~950 B) - a
  38-case jump-table player action-state dispatcher (the same shape
  `docs/status/actor.md` already flags as "left raw, out of scope" for
  `sub_8018008` in GitHub issue #22); calls a dozen still-unexamined
  state-transition functions (`sub_8023404`, `sub_8022D50`,
  `sub_8025BAC`, `sub_80231EC`, `sub_80232E4`, `sub_8023224`, and
  others).
- **`sub_800AFF4`** (`asm/code_3_2_16.s`, ROM `0x0800AFF4`, ~636 B) -
  high register-pressure (`sb`/`sl`/`r8` all live simultaneously)
  hitbox-record lookup/commit logic referencing the `+0x20`/`+0x2d`
  convention from `docs/rom_map.md`'s physics/collision write-up, but
  with several branches gated on state values and callees
  (`gStaticData_0816A820`) not independently confirmed.

None of these were force-matched or guessed at; each is either blocked
on an unmatched/unexamined callee whose real behavior isn't pinned
down, or (for `sub_8007634`/`sub_800AC2C`) large enough that a
low-confidence single-pass reconstruction risks leaving wrong
documentation behind.

## Real gotchas found along the way (useful beyond this issue)

1. **`-N` as a bit-clear mask clears the bits of `N-1`, not `N`** (the
   `-N == ~(N-1)` identity) - confirmed again here for `sub_800A734`
   (`-0x11` clears only bit 4, since `0x11-1 = 0x10`) and `sub_800A810`
   (`-9` clears only bit 3, since `9-1 = 8`). Already documented
   elsewhere in this project but easy to mis-read at a glance if you
   assume `-N` simply clears `N`'s own bit pattern.
2. **A plain `if (cond) *p |= mask;` compiles to a full independent
   read-modify-write per branch**, not a shared one, even when both
   branches funnel into logically the same store - `sub_800B270`'s
   Y-axis flag write needed an explicit `goto`-based merge (compute the
   mask conditionally, fall through to *one* shared `*p |= mask;`) to
   match the ROM's single reload.
3. **A pinned base-pointer register variable (`register T *p asm("rN")`)
   claims that register for its entire lexical scope**, which can push
   a *later*, logically-unrelated local out of the register the ROM
   uses for it - `sub_800B270`'s trailing `0x0300129C` read/write block
   needs the global's address in `r0` and its value in `r2`, but every
   attempt to pin those two registers directly produced either the
   ROM's own address/value swapped (address in `r2`, value in `r0` -
   functionally identical, wrong letters) or an unrelated `push
   {r4}`/`pop {r4}` regression elsewhere in the function. Parked on
   this specific swap rather than chase it further.

## Cross-references

- `docs/status/actor.md` - matched/parked lists updated for this
  issue's functions.
- `tools/report_units.py` - `UNITS` list split for
  `0x0800A0FC`-`0x0800B324`, corrected from `game_loop` to `graphics`
  to match the surrounding `actor_part*.c` family's existing category.
- `docs/matching.md` - the frozen historical log already discusses
  most of these functions from earlier sessions; see "Prior history"
  above for the specific entries.
- `docs/matching/issue-12-physics-collision.md` - the neighboring
  physics/collision subsystem several of this issue's left-raw
  functions (`sub_8009868` via `sub_80109A4`, `sub_800AFF4`) eventually
  lead into.
