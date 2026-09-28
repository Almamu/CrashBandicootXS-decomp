# Issue #17, leading portion: 0x08012FBC-0x08013C60 (5 functions)

Continuation of the `gStaticData_0816BF20` 42-slot action-dispatch-table
family issue #16's remainder (`docs/matching/issue-16-actor-12160.md`,
`docs/matching/issue-16-actor-remainder.md`) already closed out through
0x08012D24. Issue #17 itself scopes the whole 0x08012FBC-0x08014F8C
range (~8.0 KB, 25 functions per `tools/chunk_remaining_work.py`'s
generation pass); this pass covers only its first five functions,
0x08012FBC-0x08013C60 - the rest (`sub_8013C60` onward, through
0x08014F8C) stays raw in the trimmed `asm/code_3_2_17_12af4.s` for a
future pass.

`docs/rom_map.md`'s whole-ROM reconnaissance pass had already read two
of these five end-to-end before any matching work started:
`sub_80134B8` (the table's shared "bonus/score popup" handler, reused
across 6 of the table's 42 slots) and `sub_8013994` ("player input/
action handling" - flag-bit checks against action codes via
`sub_800AAEC`, then sound + state change). Both readings are confirmed
accurate by this pass's own full transcription.

## New files

`src/graphics/actor_part_12fbc.c` (`sub_8012FBC`/`sub_8013228`,
0x08012FBC-0x080134B8), `src/graphics/actor_part_134b8.c`
(`sub_80134B8`, 0x080134B8-0x080138E8), and
`src/graphics/actor_part_138e8.c` (`sub_80138E8`/`sub_8013994`,
0x080138E8-0x08013C60). All three are named `actor_part_<addr>.c`
(address-suffixed) rather than the next sequential `actor_partNN`
(`actor_part85.c`/`86.c`/...) - `actor_part85.c` turned out to already
be claimed by unrelated, non-ROM-adjacent issue #63 work
(`sub_8034374`/`sub_8034480`, the particle-trail BG0 object at
0x08034374). **This was discovered the hard way**: an early draft of
this pass's own `actor_part85.c` silently overwrote that file via the
Write tool before its pre-existing content had been read, destroying
377 lines of already-matched real C. Caught by an unrelated-looking
symptom - a full clean `make compare` failing with undefined references
to `sub_8034374`/`sub_8034480` from three completely different,
untouched files (`level_graphics.c`, `counter_selector_setup.c`,
`actor_part73.c`) far away in ROM address space - which made no sense
as a "pre-existing repo bug" once `git diff --stat HEAD -- ...` was
checked and showed 716 insertions/369 deletions against a file this
pass had supposedly only ever *created* fresh. Restored via
`git checkout HEAD -- src/graphics/actor_part85.c` and the real new
content moved to address-suffixed names instead, which fixed the link
on its own with no other changes needed. Filed here as a standing
warning: **check for an existing file before naming a new
`actor_partNN.c`/similar sequential-numbered file** - a `git status`
`??` (untracked) after `Write` is not proof the name was free, only
that the working tree's current content isn't tracked by git *anymore*;
diffing against `HEAD` (or just `ls`-ing the target path before the
first `Write` call) is the only way to be sure. Sequential numbering
across many independent, possibly-parallel sessions targeting
non-adjacent ROM regions is inherently collision-prone; the address
suffix used here sidesteps the problem entirely for any future file
that also can't slot into a name already reserved by a ROM-adjacent
neighbor.

`ldscript.txt` and `tools/report_units.py`'s `UNITS` list were updated:
the three new files are inserted in ROM order right after
`actor_part83.o` (0x08012AF4-0x08012FBC) and before the trimmed
`asm/code_3_2_17_12af4.o` (now starting at `sub_8013C60`,
0x08013C60). Category changed from the placeholder `graphics` to
`actor`, matching every ROM-adjacent neighbor in this same "self"
child-object/action-table family (`actor_part79.o`/`actor_part83.o`/
`actor_part84.o`/`actor_part18.o`).

## Semantics (all five)

- **`sub_8012FBC`** (620 B, `actor_part_12fbc.c`) - bails immediately
  (no `sub_80122CC` call at all) if `sub_8012A7C(self)` reports busy.
  Otherwise reads `gUnknown_030007E0`'s high 16 bits: bit 0 plays a
  fixed sound (id `0xd`), fires the `+0x20`/`+0x24` and `+0x50`/`+0x54`
  trampoline pairs (ids `5`/`0x13`), sets the trio `self+0x18=0`/
  `+0x32=0`/`+0x30=1`/`+0x28=7`, then returns directly - skipping the
  shared tail entirely, unlike every other case. Bit 1 tail-calls
  `sub_8015398(self)`. Bit `0x100` plays a different sound (id `0x1a`),
  fires another trampoline pair (ids `0xc`/`0xf`), resets `self+0x18`/
  `+0x1c`, clears the trio `+0x31`/`+0x2f`/`+0x27` to `0`/`1`/`0x1e`,
  zeroes the player's `+0x94` byte (written twice - the same "no
  redundant-store folding across statements" quirk `docs/matching.md`
  already documents for this compiler), then spawns an object via
  `sub_8025B0C(gUnknown_030012E4, 0x29, 1, 0, 0xa, 0,
  gUnknown_030012D8)` and clears two bits (`~5` on `+0xc`, then a
  `(~4)|1` pack on `+0x28`) on the returned object. All three of these
  cases (and the "none of the above" fallthrough) then converge on a
  shared tail: dispatching `sub_8000760(gUnknown_03001304)`'s result -
  `2`, or `7..8` - resets `self+0x1c=0`, fires one more trampoline pair
  (ids `0x10`/`3`), and sets the trio to `0x1d`/`1`/`0` directly; `0`
  instead calls `sub_8015780(self, 0, 0x12, 0, 0)` then re-sets the same
  trio fields to the same `0x1d`/`1`/`0`/`0x1d` shape by hand. Finally,
  if `gUnknown_030007E0`'s low-half bit `0x200` is set: for `self+8==3`,
  gates `sub_80231C4(gUnknown_030012C0)` to set `self+0x29=1`, fire a
  trampoline pair (ids `4`/`0x18`), and set the trio to `0`/`1`/`0x1b`;
  for `self+8==4`, sets `self+0x29` from the bit test and tail-calls
  `sub_8015460(self)`; otherwise, while `self+0x18` is already nonzero,
  overwrites it with the same bit-test value. Every path but the very
  first (bit-0) case ends with `sub_80122CC(self)`.
- **`sub_8013228`** (656 B, `actor_part_12fbc.c`) - first clears two
  `part+0xd` bits (`&= ~2`, then `&= ~3`, each via the runtime-negated-
  mask idiom, not a folded AND-immediate). If `part+0x68` bit 2 is set:
  fires the `+0x20`/`+0x24` and `+0x50`/`+0x54` trampoline pairs (ids
  `0x1a`/`0x15`), clamps `part+0x30`'s index against the `part+0x20`-
  pointer-to-manager/`part+0x2d`-tag/28-byte-stride record's own `+0x16`
  count (the same table-lookup convention `sub_8010480`, game_loop35.c,
  establishes), calls `sub_800B334(part)`, then clears `part+0x68`.
  Otherwise, while `self+0x26==0` and `gUnknown_030007E0`'s high-half
  bit 1 is set: plays a fixed sound (id `0xa`), fires another trampoline
  pair (ids `0xe`/`0x10`), resets `self+0x18`/`+0x1c` and the
  `+0x21..+0x24` byte run, and clears the player's `+0x92` byte.
  Otherwise, while `part+0x38` is set: depending on
  `gUnknown_030007E0`'s low bits (`1` and `0x30`) and whether
  `part+0x2d==6`, fires one or two more trampoline-pair combinations
  (ids `9`/`6`, or `7`/`0xc`) and, while `self+0x28==7`, resets the
  trio to one of `0xa`/`9`/`8` depending on those same bits. Finally,
  every path but the first converges on dispatching
  `sub_8000760(gUnknown_03001304)`'s D-pad-remap result: `<=2` resets
  the trio to `0`/`1`/`0` while the player's `+0x100` flag is clear;
  otherwise, a `self+0x27` tag of `0x1b`/`0x1c` resets it to `1`/`1`/
  `0x1c`; a nonzero `self+0x18` (tag `!=0xd`) resets it to `0`/`1`/`0xd`;
  and a clear player `+0x100` flag resets it to `0`/`1`/`7` - before
  tail-calling `sub_80122CC(self)`.
- **`sub_80134B8`** (1072 B, `actor_part_134b8.c`) - confirmed by this
  pass as `docs/rom_map.md`'s "bonus/score popup" handler, the table's
  shared default reused across 6 of its 42 slots. Caches `part+0x68`
  (busy flag) and `sub_8000760`'s D-pad-remap result up front. Unless
  `self+8==0xe`, `self+0x26!=0`, or `gUnknown_030007E0`'s bits/`self+8`
  range checks fail, plays a fixed sound (id `0xa`) and fires the usual
  `+0x20`/`+0x24` and `+0x50`/`+0x54` trampoline pairs (ids `0xe`/
  `0x10`), resetting `self+0x18`/`+0x1c` and the `+0x21..+0x24` run plus
  the player's `+0x92` byte - the same shape `sub_8013228` establishes.
  If `part+0x68` was clear: calls `sub_80122CC(self)`, then ticks
  `self+0x25`'s countdown (resetting it once the D-pad result is also
  clear) and returns. Otherwise (`part+0x68` set): calls
  `sub_801283C(self)`; for `self+8==0x1a` with `self+0x28==0`, sets the
  trio to `4`/`1`/`7` and returns. Otherwise, keyed on `part+0x68` bit
  2: when set, arms `part+0xd` bit 0 and `self+0x34=0`, fires a
  trampoline pair (ids `0x1a`/`0x15`, skipped for `self+8==0xe`),
  clamps `part+0x30` via the same table-lookup idiom `sub_8013228`
  uses, and calls `sub_800B334(part)` (or, for `self+8==0x1a`, instead
  re-runs `sub_80122CC`/`sub_801283C` and clears `part+0x68`); when
  clear, a D-pad result of `1`/`2` similarly re-runs `sub_80122CC`/
  `sub_801283C` and resets `part+0x68`, while bit 3 (and `part->0x64
  >= 0`) arms `part+0xd` bit 0, clears `self+0x34`, and - for `self+8`
  in `0x18..0x19` - spawns two objects via
  `sub_8025BAC(gUnknown_030012E4, 0x29, 1, x, y, tag)` at the player's
  de-Q8'd `+0x14`-anchored position (`+0x14` and `-0x14` X offsets),
  packing bitmasked tag/flag bytes (`+0x28`, `+0xc`) into each spawned
  object via `sl`/`sb`/`r8`-cached negated-mask idioms, and clamping
  each one's own `part+0x30` the same way; calls `sub_8014F8C(self)`
  for `self+8==0x19`; then, unless `self+8==0x1d`, plays a further
  sound (id `0x19`) and fires one more trampoline pair (ids `0x16`/
  `0x11`) before resetting the trio to `0`/`1`/`0`. Outside the
  `0x18..0x19` range: for `self+8==0xe`, dispatches
  `gUnknown_030007E0`'s low-half bit `0x30` to set the trio to either
  `1`/`1`/`1c` or `0`/`1`/tag(0/1); otherwise, gated on
  `gUnknown_030012D8+0x100` and `sub_8000760`'s result, fires one more
  trampoline pair (ids `0x17`/`0x16`) and sets the trio to `0`/`1`.
- **`sub_80138E8`** (172 B, `actor_part_138e8.c`) - a thin
  `sub_803AD84` dispatcher on `part`'s state. While `part+0x2d==6`: for
  `part->0x30==3`, fires the `+0x50`/`+0x54` trampoline with id `9`;
  for `part->0x30>3` or `part+0x38!=0`, fires it with id `8` instead
  (otherwise does nothing). Otherwise, while `part+0x38!=0`:
  `sub_80231BC(gUnknown_030012C0)` true fires the `+0x20`/`+0x24`
  trampoline (id `0x19`) then the `+0x50`/`+0x54` trampoline (id `7`);
  false fires only the `+0x20`/`+0x24` trampoline (id `0x18`).
- **`sub_8013994`** (716 B, `actor_part_138e8.c`) - confirmed by this
  pass as `docs/rom_map.md`'s "player input/action handling" reader. If
  `part+0x68==0`: sets the trio to `5`/`1`/`0` directly and returns (no
  trampoline calls). Otherwise, on the "confirm" input edge
  (`gUnknown_030007E0` low bit 0 plus `sub_800AAEC(part, 0xb)`): plays a
  sound (id `0xc`), clears two `part+0xd` bits (the same runtime
  `-2`/`-3` negated-mask idiom `sub_80142B0`, actor_part18.c, uses),
  and tail-calls `sub_8015508(self)`. On bit 1 plus
  `sub_800AAEC(part, 0x10)`: tail-calls `sub_8015398(self)` and sets
  the trio to `1`/`1`/`1`. Otherwise falls into a shared tail:
  increments `self+0x18`, and while it's still below `self+0x1c`,
  clears `part+0x34` and clamps `part+0x30` via the usual table-lookup
  idiom (initial `3`). Once `self+0x18` catches up: bails if
  `part+0x38==0`; else, if `part+0x68==0`, fires the `+0x20`/`+0x24`
  and `+0x50`/`+0x54` trampoline pair (ids `0x1a`/`0x1b`) and sets the
  trio to `4`/`1`/`0`; else, on `gUnknown_030007E0`'s low-half bit
  `0x100`: fires a further pair (ids `0x14`/`0`), resets `self+0x1c`,
  sets the trio to `0`/`1`/`3`, and tail-calls `sub_801434C(self)`
  (actor_part18.c); otherwise dispatches `sub_8000760`
  (`gUnknown_03001304`) and `sub_800AAEC(part, 2)`: when both fire and
  the D-pad result is `3`/`4`, gates `sub_80231C4(gUnknown_030012C0)`
  behind a further bit test to either fire a trampoline pair (ids
  `4`/`0x18`) and set the trio to `0x1b`, or tail-call
  `sub_8015460(self)`; any other combination fires one more trampoline
  pair (ids `0x12`/`2` or `0x11`/`4`) and sets the trio's flag/state
  halves to `1`/`0`.

## Why NAKED, not real C

Two of the five (`sub_8012FBC`, `sub_80134B8`) show the same
"unexplained extended-register-budget" wall this table family already
hit repeatedly (`docs/matching/issue-16-actor-remainder.md` and
`docs/status/actor.md`'s "Parked - NAKED transcription" section):
`sub_8012FBC` pushes `r8` (holding `&gUnknown_03001304` across a
`sub_8012A7C` call) on top of the usual `r4-r7`; `sub_80134B8` goes
further still, keeping `r8`/`sb`/`sl` all three live simultaneously
through the object-spawn/tag-mask block around `sub_8025BAC` - by far
the most extreme register-budget shape seen in this table family so
far. Both were recognized immediately from their prologues
(`push {r4,r5,r6,r7,lr}` + `mov r7,r8`/`push {r7}`, and `push
{r4,r5,r6,r7,lr}` + `mov r7,sl`/`mov r6,sb`/`mov r5,r8`/`push
{r5,r6,r7}` respectively) and NAKED-transcribed without a real-C
attempt, per this project's established policy of not re-litigating an
already-diagnosed wall.

The other three (`sub_8013228`, `sub_80138E8`, `sub_8013994`) have
ordinary-looking `r4-r6`/`lr` (or `r4`/`lr`) prologues, so each got a
genuine attempt:

- **`sub_8013228`**: a first plain-C draft, structured as straightforward
  nested `if`/`else` mirroring the ROM's own branch shape (reusing the
  exact `mgr = *(u8 **)(self + 0xc); sub_803AD80(...)` idiom already
  matched in `actor_part18.c`'s `sub_80142B0`, plus the register-pinned
  "clamp against a `part+0x20`-manager/`part+0x2d`-tag table" block
  already matched for `sub_8010480`, game_loop35.c), compiled and
  isolated-assembled cleanly but diverged at the very first instructions:
  gcc 2.9 pushed an extra `r7` (5 callee-saved registers instead of the
  ROM's 3) for the live `mgr`/`part` pointers, and `part[0xd] &= ~2`
  compiled to a folded `movs r0,#0xfd; ands r0,r1` instead of the ROM's
  runtime `movs r0,#2; rsbs r0,r0,#0` negation (the same idiom
  `sub_80142B0` already needed register-pinning to reproduce for its own
  `part[0xd]` bit clears) - not a one-line fix, given how many more
  `self+0xc`/`self+0x10` trampoline-pair call sites the rest of the
  function repeats verbatim. Given the immediate divergence and the
  size of the remaining function, this was not pursued further.
- **`sub_80138E8`**: its `part[0x38]`-gated single-vs-double
  `sub_803AD80`/`sub_803AD84` trampoline call (keyed on
  `sub_80231BC(gUnknown_030012C0)`) reproduces the *exact* shape
  already confirmed unmatchable in `sub_80156EC`
  (`actor_part38c.c`, `docs/matching/issue-18-0x08014f8c-actor.md`'s
  "Parked, not matched: sub_80156EC" - gcc 2.9 insists on an extra
  push/pop to recompute `self` into a fresh register in the `else` arm
  once `mgr` is locally redeclared there, where the ROM reuses the same
  register throughout). Recognized from the shape alone; not attempted
  again given the already-confirmed, unrelated-to-register-pinning
  nature of that specific gap.
- **`sub_8013994`**: shares the same action-table frame-shape wall
  its ROM-adjacent neighbors already hit, and additionally repeats the
  identical `sub_80231C4`-gated single-vs-double trampoline idiom
  `sub_8013228` and `sub_80138E8` both show. Given the two more
  isolated, smaller occurrences of this idiom in this very batch both
  resisted or were recognized as the same wall, this larger function
  (716 B, the most call-site-dense of the five) was NAKED-transcribed
  directly rather than spending further iteration on it.

All five were transcribed instruction-for-instruction from the ROM
disassembly (mechanical unified-to-divided-syntax mnemonic conversion -
`adds`/`movs`/`subs`/`ands`/`orrs`/`lsls`/`lsrs`/`asrs` drop the
trailing `s`, `rsbs Rd, Rs, #0` becomes `neg Rd, Rs`), keeping the
ROM's own `_0XXXXXXX` hex-address labels verbatim as plain, file-local
asm symbols rather than hand-renumbering them - safe here since each
label's hex address is inherently unique across the whole ROM, so no
collision risk exists even across the three files, the same approach
`actor_part82.c`'s `sub_8011BD4` uses for the same reason (transcription-
error avoidance for functions this size). Every function was verified
structurally byte-exact via an isolated `cpp`/`agbcc`/`arm-none-eabi-as`
+ `objcopy`/`objdump` comparison against the ROM's own raw bytes before
being wired into the real build (all differences before linking were
relocation placeholders - `bl` targets and literal-pool addresses
showing as zero/section-relative offsets or garbage disassembly,
resolving correctly once linked - exactly the expected shape for an
unlinked single-object test).

## Status

All five functions are now byte-exact, but as NAKED transcriptions -
parked, not matched, per project policy. A full clean `make compare`
(`rm -rf build && make NON_MATCHING=1 report`, then `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare`) confirms `La suma coincide` against the real ROM. Issue #17
itself stays open - only 5 of its 25 scoped functions are covered here
(and all 5 as NAKED, not real C, so none count as "matched" either);
`sub_8013C60` through 0x08014F8C remains for a future pass.

## Second pass: 0x08013C60-0x08014F8C (14 functions)

This pass covers the rest of the chunk, the two raw files
`asm/code_3_2_17_12af4.s` (`sub_8013C60`-`sub_8014084`, now
`src/graphics/actor_part_13c60.c`) and `asm/code_3_2_17_14674.s`
(`sub_8014674`-`sub_8014EE0`, now `src/graphics/actor_part_14674.c`).
Both raw files are retired. Of the 14 functions, 9 are real C and 5 are
NAKED transcriptions, each with its C kept under `#if NON_MATCHING`.

| function | result |
|---|---|
| `sub_8013C60`, `sub_8013D94`, `sub_8013EAC`, `sub_8013FD4` | matched |
| `sub_8014084` | NAKED |
| `sub_8014674` | NAKED |
| `sub_8014940`, `sub_80149BC`, `sub_8014A3C`, `sub_8014AEC` | matched |
| `sub_8014B54`, `sub_8014BCC`, `sub_8014D18` | NAKED |
| `sub_8014EE0` | matched |

### Compiler: old_agbcc

This range was built with old_agbcc too. The ROM shows its tell
throughout (`movs r0, #8; ldrb r1, [r1]; ands r0, r1` for `contact &
8`). `sub_8013FD4` is the cleanest proof: the same C is byte-exact under
old_agbcc and differs in 8 bytes under the current agbcc. Both new objects are
on `OLD_AGBCC_OBJS`. The chunk's first five functions
(`actor_part_12fbc.c`/`_134b8.c`/`_138e8.c`, all NAKED without C) were
parked against the current agbcc. They are candidates for an old_agbcc
retry, which this pass did not attempt.

### Shared header

The player/action object now has a struct view, `include/action_obj.h`
(`struct act`, `struct act_part`). The header also holds the method-call
macros, the input-snapshot accessors and the trio helpers both files
use. The older files in this family still use raw offsets.

### What made them match

- **The input snapshot.** Every handler copies `gUnknown_030007E0` to a
  stack word and reads its halves back with `ldrh [sp, #0]`/`[sp, #2]`.
  A union or struct member read gets folded into a halfword load of the
  global itself, so the halves are read through the local's address
  (`INPUT_PRESSED`/`INPUT_HELD`). Where the ROM loads `sub_8000760`'s
  argument before taking the snapshot, a `pad` local fixes the order.
- **Byte stores and the dead `& 0`.** As in `actor_part_18008.c`
  (`docs/matching/issue-22-0x08018008-hopper.md`), a struct-member byte
  store leaves a 0 that CSE reuses for a later zero store. So the
  `part+0x0D`/`+0x0C` flag RMWs go through a byte pointer, and the
  constant arrives as an inline `s32` parameter
  (`ActAndFlags0D`/`ActOrFlags0D`).
- **The "next action" trios.** `ActSetNext(self, next)` makes the ROM
  load `next` before the three stores. In `sub_8013C60`/`sub_8013EAC`
  the ROM loads a fresh 1 for +0x30 on the fire path, where C reuses the
  `& 1` test's constant from a callee-saved register. A barriered,
  pinned variant (`ActSetNextB`) handles that.
- **Ranges.** `dir` range tests the ROM writes as `cmp #8; bgt; cmp #3;
  blt` come from a GNU range `case 3 ... 8:`. An `if` gets folded to an
  unsigned `dir - 3 <= 5`.
- **`sub_8014940`'s bitmap set** is `actor_part_188d0.c`'s
  `MARK_GONE_BITMAP` with the same pins. Its word index needs a barriered
  signed shift: gcc knows a zero-extended `u16` can't be negative and
  would use `lsr`.

### Why five are NAKED

In all five, the C has the ROM's blocks and instruction sequence. They
differ only in register roles:

- `sub_8014084`: in the facing block the ROM loads `self->part` into
  r0, tests its +0x28 bit through r1, and keeps a copy in r2 for the
  rest of the block. Every phrasing tried either merges the two
  pseudo-registers or loads straight into r2. That covers a separate
  local, the assignment inside the test, a pinned or barriered copy, and
  a `mirror` bitfield.
- `sub_8014B54`: the `+= 0x600` constant is a reload that the ROM puts
  in r3 where gcc picks r2. Pinning it shifts the rotation for every
  later reload instead.
- `sub_8014BCC`, `sub_8014D18`, `sub_8014674`: the ROM keeps the
  constant 1 (and in `sub_8014BCC` the `&self->next27` pointer, in r8)
  in different callee-saved registers from gcc's choice. `sub_8014D18`
  also shares one `sub_803AD84` call between its 0x22/0x23 animation
  paths.

### Status

Issue #17 stays open. This pass leaves no raw functions, but the chunk
still has 10 NAKED functions: the first pass's five and this pass's
five.

## Third pass: old_agbcc retry of the 10 NAKED functions

This pass retried the chunk's ten NAKED functions under old_agbcc. Seven
are now real C. Three stay NAKED, each with its best C under
`#if NON_MATCHING`.

| function | file | result |
|---|---|---|
| `sub_8012FBC`, `sub_8013228` | `actor_part_12fbc.c` | matched |
| `sub_80134B8` | `actor_part_134b8.c` | matched |
| `sub_80138E8`, `sub_8013994` | `actor_part_138e8.c` | matched |
| `sub_8014084` | `actor_part_13c60.c` | NAKED |
| `sub_8014674`, `sub_8014B54` | `actor_part_14674.c` | NAKED |
| `sub_8014BCC`, `sub_8014D18` | `actor_part_14674.c` | matched |

`actor_part_12fbc.o`, `actor_part_134b8.o` and `actor_part_138e8.o` now
build with old_agbcc (Makefile `OLD_AGBCC_OBJS`). The first pass left
all three with NAKED functions only, so moving the whole object was
safe and no split was needed. All five first-pass functions match under
old_agbcc. Their "unexplained extended-register-budget" walls (`r8`,
`r8`/`sb`/`sl`) and `sub_80138E8`'s "unmatchable `sub_80156EC` shape"
were just the wrong compiler. `sub_80138E8` matches under both
compilers. `sub_8013994` differs in 35 bytes under the current agbcc.

### What made them match

- **Trio helpers with parameters.** `include/action_obj.h`'s
  `ActSetNext` idea applies to the +0x31/+0x2F/+0x27 trio too. Each file
  has local inline helpers. `ActQueue27(self, cur, next)` stores a
  literal 1 in +0x2F. `ActTrio27(self, cur, flag, next)` stores a caller
  value there, for the paths where the ROM reuses a 1 already in a
  register. The `...P` forms store the action through a pointer the
  caller already holds, when the ROM tests and stores through the same
  `&self->next27`/`&self->next28`. old_agbcc materializes inline
  parameters before the stores, which is the ROM's order. Which form a
  store needs is visible in the ROM: a constant loaded before the first
  store, a fresh `movs rX, #1` at the +0x2F store, or a register.
- **Duplicated tails.** Where the ROM shares one method call plus trio
  between two paths (`sub_8013994`'s 0x12/0x11 tails, `sub_8014D18`'s
  0x22/0x23 idle animations, `sub_8014BCC`'s alt/idle trio), the C
  writes the tail out on each path. gcc's cross-jumping merges them back
  and each copy keeps its own CSE state. A shared `goto` label instead
  starts a new basic block, and the 1 stored after it gets reloaded.
- **`ACT_CALL1`/`ACT_CALL2`** (new in `include/action_obj.h`). These
  are the method-call macros in `if (1) { ... } else (void)0` form, the
  same finding as `include/actor_self.h`. `ACT_VCALL`'s
  `do { } while (0)` puts loop notes around every call. CSE then won't
  carry the fire test's constant 1 (kept in `r6`/`r7`) into the +0x2F
  stores after the calls, but the ROM does. `sub_8012FBC`,
  `sub_8014BCC` and `sub_8014D18` need `ACT_CALL`. `sub_8013D94`
  (already matched) needs `ACT_VCALL` and breaks with `ACT_CALL`, so
  the header keeps both forms.
- **Zero/one kept across calls.** Where the ROM loads a 0 into a
  callee-saved register before a pair of method calls and stores it
  afterwards (`self->frames = 0` in `sub_8013994`/`sub_8012FBC`,
  `self->frame = 0` in `sub_8014BCC`), a `s32 zero = 0;` local declared
  before the calls reproduces it. `sub_8013228` keeps its 9/8 +0x30 1
  apart from the `cur & 1` test's constant behind one `asm("" : "+r")`
  barrier. That is the only barrier in these files, and the plain C
  differs in 187 bytes.
- **Bitfields for the spawned objects' masks.** `sub_8012FBC`'s and
  `sub_80134B8`'s spawned objects clear and set bits in +0x0C/+0x28. As
  bitfields, gcc emits the ROM's `-5`/`-4` masks, and the `| 1` in
  QImode picks up the ROM's `r4` 1. `sub_80134B8`'s -0x11 mirror mask
  goes through an `s32`-parameter helper so it stays in SImode. The
  ROM derives it from the 1 already in `r7` (`subs r7, #0x12`). The
  first spark's bit-2 clear is written twice. The second store folds
  away, but the extra use of the -5 mask gives it `sb` (and the -0x11
  mask `r8`), as in the ROM.
- **Smaller shapes.** `sub_8012FBC` takes `&gUnknown_03001304` into a
  local up front, which is what keeps the address in `r8` across the
  calls. `sub_80134B8` computes the spark coordinates as
  `x = pl->x; x >>= 8; x += 0x14;` and passes them through an inline
  `SpawnSpark(x, y, mirror)`, which fixes their evaluation order.
  `sub_8013228`/`sub_80134B8` clear `part+0x68` through
  `ActSetContact(part, 0)`, an inline with an `s32` parameter, so the 0
  is loaded before the part pointer. `sub_8014D18` switches on the
  record kind with separate case bodies for 1..5, which is what makes
  gcc emit the ROM's 7-entry jump table.

### Still NAKED

- **`sub_8014084`.** The facing block needs the second
  `flags28 << 27` test to use different hard registers from the first.
  That is what stops jump2's thread_jumps from folding it away, as it
  does for every C tried. The ROM loads `self->part` into `r0` and keeps
  a copy in `r2`. In every form tried, gcc either merges the two
  pseudos or gives the re-test the same registers.
- **`sub_8014674`.** The contact path's `tag == 0xD`/`tag == 0x18`
  re-tests have the same problem. The ROM keeps `cmp #0xD` / `cmp #0x18`
  after the `||` test. The C gets them threaded, and the constant 1 is
  then carried through into the 0x18 block. Tried: an inline `Land()`
  helper, a `switch`, a barrier-copied tag, and `do`/`while` around the
  path. `decomp-permuter` didn't find a form either.
- **`sub_8014B54`.** One reload register is off: the 0x600 constant goes
  to `r2`, the ROM's to `r3`. The rest is byte-exact. Reload picks it
  from its spill-register rotation (`order_regs_for_reload` /
  `allocate_reload_reg`). Nothing tried in the C (a pinned or named
  constant, `-=`, a pointer to `y`, an inline) moves it. `ACT_CALL`
  doesn't change it either.

## Later pass (issue #15/#16/#17 second NAKED retry)

See docs/matching/issue-15-16-17-naked-retry-2.md. `sub_801434C` and
`sub_80145E4` (actor_part18.c/actor_part18b.c) are now real C.
`sub_8014084`'s draft is down to one misplaced instruction: its facing
block now reads `self->part` for every access (GCSE produces the ROM's
r2 copy) and spells the two bit tests differently so the second is not
threaded away. `sub_8014674` and `sub_8014B54` are unchanged.
