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
