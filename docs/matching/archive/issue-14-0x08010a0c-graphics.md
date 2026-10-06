# Issue #14: 0x08010A0C-0x08010D54 (25 functions, graphics-labeled chunk)

GitHub issue #14 (`decomp-chunk`, labeled `graphics` by the chunk
generator) listed 25 raw functions in `asm/code_3_2_17.s`'s
`asm/code_3_2_17_e560_10a0c.s` fragment, right past issue #13's own
range. This is the write-up for the work done against that list, plus
a follow-up pass on the previously-parked `ResetPlayer` from GitHub
issue #9.

## Category correction: `graphics` -> `game_loop`

Every function in this chunk operates on the same `self` type
`GetSlotCrateStage` (slot_crate.c, matched under GitHub issue #13) and
`ResetCrate` (crate_reset.c, issue #13) already do - a "collision box"
record embedded inside the player at `gPlayer+0x108`,
confirmed directly: `ResolvePlayerCollisions` (crate.c) already calls
`ResolveCollisionCandidates(gPlayer + 0x108)`. `tools/report_units.py`'s
`0x08010A0C` entry only ever carried `graphics` as a pre-existing
placeholder pending examination (its own comment said so). Now that
it's examined, this chunk is recategorized to `game_loop`, matching
the surrounding `crate_reset.c`-`slot_crate.c` file family issues
#12/#13 already recategorized for the same reason.

## Matched - 24 functions (+1 unlabeled)

`src/crates/slot_crate.c` (new file): `DecrementSlotCrateStage`-`GetCrateTrialKind`,
covering:

- A 3-way bit-field family packed into `self+0x48`: bits 6-7 (get
  already matched as `GetSlotCrateStage`; `DecrementSlotCrateStage` decrements it if
  nonzero; `SetSlotCrateStage` sets it; `ClearSlotCrateStage` clears it), bits 3-5
  (`GetSlotCrateSpins` gets, `DecrementSlotCrateSpins` decrements-if-nonzero,
  `SetSlotCrateSpins` sets), and bits 0-2 (`SetSlotCratePhase` sets, `GetSlotCratePhase`
  gets).
- Plain field accessors: `SetCrateKind`/`GetCrateKind` (`self+0x4e`
  byte), `SetCrateFallDistance`/`GetCrateFallDistance` (`self+0x44` word),
  `SetCrateState`/`GetCrateState` (`self+0x4d`'s low 7 bits, preserving bit
  7), `SetCrateFallSpeed` (`self+0x4c` byte setter), `GetCrateFallSpeed`
  (`self+0x4c` sign-extending getter), `SetCrateTouched` (`self+0x58`
  byte setter), `GetCrateParamB`/`GetCrateParamA` (`self+0x51`/`self+0x50`
  byte getters), `SetCrateSolidKind` (`self+0x48` whole-field byte setter,
  unlike `SetSlotCratePhase` which masks), `SetCrateTrialKind`/`GetCrateTrialKind`
  (`self+0x54` word accessors).
- `IsCrateBusy` - a boolean getter for `self+0x4d` bit 7. **The
  original disassembly never gave this one its own label/symbol** - it
  sits directly after `GetCrateFallSpeed`'s trailing alignment padding, at
  the address the byte-count arithmetic works out to (`GetCrateFallSpeed` is
  10 bytes, padded to 12; `IsCrateBusy` starts right after). Named
  `IsCrateBusy` per the usual `sub_XXXXXXXX` convention (its own ROM
  address) since it's still a completely ordinary function, just one
  the original tooling's symbol table missed. Not entered into
  `expected/corrections.txt`, per `docs/decomp_dev.md`'s note there:
  that mechanism renames an *existing* label, and this function never
  had a wrong one to correct - it simply had none.
- `SetCrateBusy`/`ClearCrateBusy` - set/clear `self+0x4d` bit 7 together
  with the global "hit" latch `gPlayer+0x80` `crate_reset.c`
  already established.

## Follow-up: `ResolveCollisionCandidates` - now matched (NAKED transcription)

`ResolveCollisionCandidates` (`src/objects/collision_queue.c`) - the chunk's last and
largest function (488 bytes), and the one `ResolvePlayerCollisions`
(crate.c) already calls by name - was originally parked here
under `NON_MATCHING`. A later pass in the same session that closed
`ResetPlayer` below (see the next "Follow-up" section) closed this one
too, as a byte-exact `NAKED` transcription rather than real decompiled
C. It scans `self`'s neighbor-candidate list - `self`'s own `+8`
onward is an array of 0x24-byte "candidate" records (`neighbor`
pointer at `+0`, a position pair at `+4`/`+8`, a `kind` tag at `+0xc`
compared against `4`, three more fields at `+0x10`/`+0x14`/`+0x18`/
`+0x1c`, and two flag bytes at `+0x20`/`+0x21`) - where `records[0]`
is a previous/seed candidate and `records[1..count-1]` are new
candidates queued this frame. For each candidate, computes its
Y-distance to the player (`gPlayer`); any whose Y-distance
jumps more than 8 past the running-best Y-distance, or whose own
`kind` is `4`, gets resolved immediately via `ApplyCrateCollision()` (an
11-argument call - the 9th-11th land in this function's own stack
frame at a fixed offset, confirming they're genuine AAPCS-style
stack-passed arguments, not separate mystery locals, once
cross-referenced against where their addresses are computed); the
rest are only compared against each other (Y-distance primary,
X-distance tiebreak) to find the single nearest. After the scan, that
overall-nearest candidate is *also* resolved via `ApplyCrateCollision()` -
its 11th argument set to whether any forced/priority hit happened
during the scan (`1`), unlike every in-loop call, which always passes
`0` there - and the list is reset (`count = 0`, `field4 = 0`) for the
next frame.

Every field offset, branch, and call argument here was already
understood and cross-referenced against the mirror-image writer
`AddCollisionCandidate` (right after this issue's own range, not itself in
scope) and the `ResolvePlayerCollisions` caller when this was first parked - but
the ROM builds nearly every record-field address in both the loop
body and the two `ApplyCrateCollision` call sites as a *running pointer*,
incremented by `0x24` once per loop iteration, with up to twelve of
them (`r8`/`sb`/`sl` among them) live across a single `0x68`-byte
stack frame. This is the same "long, non-uniform stretch of field
accesses via running-pointer increments" gap already parked for
`ResetPlayer`/`UpdateGroundSprite` in the issue #9 write-up
(`docs/matching/archive/issue-9-0x08007634-actor.md`), just at a larger scale
(three times the live-cursor count, on a stack frame twice the size)
- well beyond what C-level register pins can realistically express,
so this pass closed it as a `NAKED` transcription instead of chasing
a plain-C register allocation: the ROM's own Thumb instructions,
transcribed one-to-one (suffix-less mnemonics - `add`/`mov`/`lsl`/
`ldr`/`str`, not `adds`/`movs`/`lsls`/suffixed forms - which this
project's assembler invocation accepts identically), with the single
`gPlayer` literal pool kept at the ROM's own mid-function
split point (right after the loop's first `ApplyCrateCollision` call site's
`b` past it) and a trailing `asm(".align 2, 0")` for the 2-byte
zero-fill gap before `AddCollisionCandidate` (the assembler's default `nop`
fill pattern otherwise mismatches the ROM's zero halfword there - see
`matching_decomp_alignment_fix`). Verified via isolated
`arm-none-eabi-as` assembly against the ROM's raw bytes first, then
folded into `src/objects/collision_queue.c`/`.o` and confirmed with a full
clean `make compare` (`crashbandicootxs.gba: La suma coincide`). The
old `asm/code_3_2_17_e560_10b6c.s` fragment (which held only this one
function) is removed entirely, `ldscript.txt`'s now-redundant
`code_3_2_17_e560_10b6c.o` entry is dropped, and
`tools/report_units.py`'s `0x08010B6C` unit now points at
`src/objects/collision_queue.o` instead of `None`.

## Follow-up: GitHub issue #9's `ResetPlayer` - now matched

`ResetPlayer` (`src/player/player_reset.c`) was previously parked
(see this same repo's `issue-9-0x08007634-actor.md` for its original
write-up) on exactly the same shape of gap `ResolveCollisionCandidates` above hit -
the ROM building several field addresses as a running pointer
incremented by small relative offsets across a long, non-uniform
stretch of writes. A second pass this session closed that gap. The
techniques that got it over the line, in the order they were needed:

1. **Two interleaved running-pointer cursors**, not one - the ROM's
   own r1/r0 registers each walk a separate chain of fields
   (`self+0x28` -> `+0x68` -> `+0x8c` via r1; `self+0x24` -> `+0x90` ->
   `+0xac` -> `+0x80` -> `+0x88` via r0), reproduced here as two plain
   `u8 *` cursors (`p1`/`p0`) incremented/decremented by the literal
   relative offset between each field - the same idiom `ResetPlayerForRoom`
   (right after this function, matched in an earlier session) already
   needed a single cursor for, just doubled up and interleaved here.
2. **Separate statements, not one folded expression**, for
   `self+0xc`'s three-flag OR (`flags |= 0x80; flags |= 0x40; flags |=
   2;`) - a single `flags | 0x80 | 0x40 | 2` expression lets the
   compiler constant-fold the three literals into one `0xc2` immediate
   at compile time, while the ROM performs each OR as its own runtime
   instruction against the freshly-loaded byte.
3. **An inline-asm-anchored first OR**, forcing the mask constant to
   load into r0 *before* the byte load (`mov r0,#0x80; ldrb r1,[self,
   #0xc]; orr r0,r0,r1`) - a plain `flags |= 0x80` (with `flags`
   already sitting in a register from a prior load) puts the byte load
   first instead, and no re-ordering of the surrounding C statements
   changed that.
4. **The negative-constant register-pinned mask idiom** (already
   documented project-wide, see `matching_decomp_register_pinning`
   memory) for every `& ~N` clear in this function (`~4`, `~0x10`,
   `~1`) - `register s32 mask asm("rN") = -(N+1);` forces the ROM's own
   runtime `mov`+`neg` pair instead of a folded 8-bit AND immediate.
5. **Forcing a pointer to actually materialize in a register** for the
   `self+0x8c` write - without an explicit `asm volatile("add %0, %0,
   #0x24" : "+l"(p))`, this compiler folds a subsequent `p += 0x24;
   *(s32*)p = value;` straight into the next store's addressing mode
   (`str r0, [r1, #0x24]`) instead of reproducing the ROM's separate
   `adds r1, #0x24` pointer update followed by an offset-0 store.
6. **Keeping `self+0xb0`'s *address* (not the loaded child pointer)
   live across the `bl GetSpriteAnimPaletteSlot`**, reloading the child pointer
   fresh afterward - `r0` gets clobbered by the call's return value, so
   caching the loaded `child` pointer across the call (this compiler's
   natural choice, since it's cheaper by one instruction) needs an
   extra register the ROM doesn't spend; the ROM instead keeps the
   *address* in `r5` (callee-saved, survives the call for free) and
   re-derives the pointer with a second load afterward.
7. **Register-pinning the `ret & 0xf` / `~0xf` nibble-merge** in the
   exact order and registers the ROM uses (`ret & 0xf` into r0/r1
   first, *then* the `~0xf` mask into r1, reused) - and forcing that
   `~0xf` mask via inline asm specifically, since a plain `-0x10`
   constant here lets the compiler derive it as `r1 - 0x1f` from the
   still-live `0xf` mask value already in r1 (algebraically valid,
   since `0xf - 0x1f == -0x10`) instead of the ROM's fresh `movs
   r1,#0x10; negs r1,r1` pair.
8. **Three independent, deliberately narrow-scoped offset registers**
   for the trailing `self+0x100`-`self+0x105` writes, not one shared
   cursor - the ROM uses r1 (one-shot `0x100`, then reused `+3` for
   `0x103`, then `+2` more for `0x105`), r2 (a literal-pool load for
   `0x101` - it doesn't fit an 8-bit `movs` immediate - reused `+3` for
   `0x104`), and r3 (one-shot `0x102` only). Getting the exact register
   choice right needed: initializing each offset right where the ROM
   first computes it (not all three up front - otherwise the scheduler
   hoists the data-independent r2 literal load ahead of r1's
   `movs`/`lsls` pair); giving r3's one-shot use its own narrow block
   scope (a pinned register variable claims its register for its whole
   *lexical* scope, not just past its last real use - see the "Real
   gotchas" note in `issue-9-0x08007634-actor.md`); and, even after
   that scoping fix, an inline-asm-anchored final AND-clear (`self+0xc
   &= ~1`) anyway, since with r1/r2/r3 all free by that point this
   compiler's allocator always prefers the lowest-numbered free
   register (r2) over the ROM's own choice (r3), regardless of scope
   narrowing.

`tools/report_units.py`'s `0x0800A734` unit is now folded into the
existing `player_reset.o` entry (base object, not `None`) since both
functions in that file are matched and the old `asm/code_3_2_16_a734.s`
fragment has been removed entirely - `docs/status/actor.md`'s "Parked"
section entry for `ResetPlayer` is removed accordingly.

## Cross-references

- `docs/status/game_loop.md` - matched/parked lists updated for this
  issue's functions; `ResolveCollisionCandidates` moved from "Parked (NON_MATCHING)"
  to the "Parked - NAKED transcription" section.
- `docs/status/actor.md` - `ResetPlayer` moved from "Parked" to
  "Matched".
- `tools/report_units.py` - `UNITS` list split for
  `0x08010A0C`-`0x08010D54`, recategorized `graphics` -> `game_loop`;
  `0x0800A734` merged into the existing `player_reset.o` unit;
  `0x08010B6C` now points at `src/objects/collision_queue.o` instead of
  `None`.
- `ldscript.txt` - the now-redundant
  `build/crashbandicootxs/asm/code_3_2_17_e560_10b6c.o(.text);` line
  removed (the function moved into the already-present
  `collision_queue.o(.text);` line just above it).
- `docs/matching/archive/issue-9-0x08007634-actor.md` - the original write-up
  for `ResetPlayer`'s first (parked) pass; this file's "Follow-up"
  section above is the second pass that closed the gap.
- `docs/matching/archive/issue-13-graphics-fc70.md` - the immediately-preceding
  chunk in the same physics/collision subsystem file family, and the
  precedent for this file's `graphics` -> `game_loop` recategorization.
