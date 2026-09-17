# Issue #22: 0x08017A44-0x080188D0 (25 functions)

Labeled `graphics` by the chunk generator, but on inspection every
function here operates on the same large per-level "player/action"
object already documented in `actor_part18.c`/`actor_part19*.c`
(`self+0xc` per-category table pointer, `self+0x10` "part" sub-object,
the `sub_803AD80`/`sub_803AD84` base+offset+fn-pointer trampoline
convention) - filed under `docs/status/actor.md` instead, matching the
precedent already set for `actor_part18.o`/`actor_part18b.o` (also
`graphics`-labeled-but-actor-shaped, see their own `tools/report_units.py`
entries).

15 of the 25 functions matched; the remaining 6 (`sub_8017AB0`,
`sub_8018008`, `sub_8018400`, `sub_801865C`, `sub_80186F0`) were left
raw - out of scope for this pass given their size (one ~500-instruction
and one ~480-instruction jump-table state machine, plus two
high-register-pressure helpers) - see "Left raw" below. Issue stays
open; `Closes #22` not used.

## Matched

- `src/graphics/actor_part20.c` (new file, ROM 0x08017A44-0x08017AAC):
  `sub_8017A44`-`sub_8017AAC` (9 functions) - trivial `self+0x14`/
  `self+0x17`/`self+0x18` byte/word accessors, plus the
  `sub_8017A78`/`sub_8017A8C` table-pointer-reset pair (same
  `gStaticData_087E435C`/`sub_800B8A8`/`sub_800B8C8` double-set pattern
  seen throughout this object family).
- `src/graphics/actor_part20b.c` (new file, ROM 0x08017ECC-0x08017FE8,
  non-adjacent to `actor_part20.c` since the raw `sub_8017AB0` sits
  between them): `sub_8017ECC`, `sub_8017F14`, `sub_8017F5C`,
  `sub_8017F80`, `sub_8017FA4`, `sub_8017FD4`, `sub_8017FE8` - a
  `self+4` double-pointer-chain record lookup (same shape as
  `sub_800B704`/`sub_800B838` in `actor_part17.c`) feeding the
  `gStaticData_0816C2D8` per-vector-component trampoline table, with
  `part+0x28` bit 4/bit 5 mirror-flag X/Z negation exactly like
  `sub_800B734`/`sub_800B7B0`/`sub_800B6A0`/`sub_800B6D0`.
- `src/graphics/actor_part20c.c` (new file, ROM 0x080187FC-0x08018884,
  non-adjacent to `actor_part20b.c` since the raw `sub_8018008`-
  `sub_80186F0` block sits between them): `sub_80187FC` (a two-state
  "charge" handler), `sub_8018858`/`sub_801886C` (another table-pointer
  reset pair, `gStaticData_087E442C`), `sub_8018884` (a struct-actor-
  shaped "part"'s flags-OR plus `gUnknown_030012B4+0x108` bitmap-set,
  same idiom as `sub_8007DBC`'s `part->field_08` bitmap-set in
  `actor_part2.c`).

### Compiler-codegen notes

This whole cluster hit the same "systematic single-register/operand-
order choice" class of gap documented throughout `docs/matching.md`,
but every instance here was chased down and fixed rather than parked -
worth recording since the techniques generalize:

- **`sub_8017ECC`/`sub_8017F14`** (record lookup + type-indexed table
  entry): this compiler naturally puts the intermediate `rec` pointer
  and the final `tableEntry` pointer in *different* registers than the
  ROM's single reused register, and picks operand order for the
  pointer `+`s inconsistently with the ROM. Fixed by literally
  reproducing the ROM's register reuse with `register ... asm("rN")`
  locals plus two explicit `asm("add %0, %0, %1" ...)`/
  `asm("add %0, %1, %2" ...)` statements for the two pointer
  combines (`rec = recOffset + arr`, `tableEntry = typeOffset + base`)
  - a plain C `+` reliably picked the wrong operand order or CSE'd the
    two into one register regardless of source statement order.
- **`sub_8017F5C`/`sub_8017F80`** (same lookup, tail-calling
  `sub_800B6D0`/`sub_800B7B0`): same fix, but here the ROM reuses a
  *single* register (`r2`) as the accumulator across the whole
  computation (`recOffset` -> `rec` -> `type*12` -> `tableEntry`) with
  `r3` as the transient "current source" - modeled with one
  `register s32 acc asm("r2")` local reused across all four steps.
- **`sub_8017FA4`**: ROM computes `self[0x1c] = -1` via `subs r0, #1`
  reusing the `r0 = 0` already sitting in the register from the
  preceding `self[0x20] = 0` store (cheaper than a fresh `movs r0,
  #-1`/`ldr` sequence) - reproduced with a single `register s32 zero
  asm("r0")` read after the store and `zero - 1` instead of a literal
  `-1`; also needed the `self+0x20` pointer expression evaluated
  *before* `zero`'s assignment (matching ROM's `adds r1,r4,#0` /
  `adds r1,#0x20` preceding `movs r0,#0`) since gcc otherwise scheduled
  the independent statements in the opposite order.
- **`sub_80187FC`**: written as `if (state == 1) ... else if (state >
  1) {} else if (state == 0) ...` to match the ROM's literal 3-way
  compare chain (`cmp #1;beq`, `cmp #1;bgt`, `cmp #0;bne`), but this
  compiler silently inverted the branch and swapped the physical block
  order (state-1 body first, state-0 body second) even though the
  source listed them the other way. Rewritten with explicit
  `goto`s/labels forcing the ROM's actual block order (state-0 body
  inline, state-1 body at the jump target) - see `docs/workflow.md`'s
  "`goto`s to force block ordering" note. Also needed: `obj[8] = 1`
  evaluated *before* re-reading `table = obj->0xc` (ROM order, not the
  more "natural" read-then-write order); the `table + 0x50` offset
  mutated in place on `table` itself (`table += 0x50`) instead of
  computed into a fresh pointer, since the plain `table + 0x50`
  expression got a new register instead of reusing `table`'s; and the
  state-1 body's `other[4] = timer` store moved to happen immediately
  after computing `timer`, before the `gUnknown_03001308` threshold
  lookup, matching the ROM's store-then-compute order (the more
  natural "compute both, then store" order left `r2` from `other`
  alive too long, so the trailing `0x2000` constant got a spare
  register `r4` instead of the ROM's reused-now-dead `r2`).
- **`sub_8018884`**: needed `other` pinned to `r1` for the entire
  function (never copied to another register, matching the ROM's
  direct-parameter-register reuse throughout), a `volatile` re-read of
  `other+8` to stop this compiler CSE-ing away the ROM's own
  seemingly-redundant second `ldrh` reload (real in the ROM because its
  register allocator picks a different register for the value on each
  side of the branch), and a raw `asm("add %0, %1, #0\n\tasr %0, %0,
  #5")` for the index shift - without it this compiler proves the
  zero-extended `u16` value never has its top bit set and folds the
  ROM's `asrs`+`adds` shift-setup pair into a single `lsr`, a real
  (if value-preserving here) byte-count mismatch.

**General lesson reconfirmed**: every one of the above looked like a
match in an isolated per-function compile and then diverged once
linked into the real ROM at its real address - full clean `make
compare` (not the isolated compile) is what actually caught each of
these, exactly as `docs/workflow.md` step 2/3 warns.

## Left raw (out of scope this pass)

- **`sub_8017AB0`** (`asm/code_3_2_17_17ab0.s`, ROM 0x08017AB0-
  0x08017ECC) - a ~500-instruction player-vs-camera-viewport state
  dispatcher (branches on `self+8`'s 0-2 state, does AABB/screen-bound
  checks against `gUnknown_030012D8`, and fires the usual table
  trampolines). Semantics are broadly graspable but a byte-exact
  reconstruction of a function this size was out of scope for this
  pass; left completely untouched.
- **`sub_8018008`/`sub_8018400`/`sub_801865C`/`sub_80186F0`**
  (`asm/code_3_2_17_18008.s`, ROM 0x08018008-0x080186F0) - a
  ~480-instruction jump-table player action-state machine
  (`sub_8018008`, 16-case dispatch on `self+8`) together with its
  companion sub-state handler (`sub_8018400`, 15-case dispatch) and two
  high-register-pressure helpers it calls (`sub_801865C`, a
  nearest-target scan over `gUnknown_030012EC`'s array using `r8`/
  `sl`/`sb`; `sub_80186F0`, a spawn-effect constructor also using
  `r8`/`sb`). Same reasoning as `sub_8017AB0` - left raw rather than
  force a low-confidence reconstruction of this much control flow in
  one pass.

## Build

Full clean `rm -rf build && make compare` passes ("La suma coincide").
`make NON_MATCHING=1 report` also succeeds (no parked functions in this
chunk, so this is mostly a sanity check).

See [docs/status/actor.md](../status/actor.md) for the matched-function
list and this raw span's entry.
