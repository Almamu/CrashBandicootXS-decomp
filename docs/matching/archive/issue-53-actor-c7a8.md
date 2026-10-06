# Issue #53: 0x0802C99C-0x0802D3A8 range boundary (actor) - `DetonateNearbyPolarNitros` and 12 more

`DetonateNearbyPolarNitros` (ROM `0x0802C7A8`-`0x0802C8FA`, right before issue #53's
own `0x0802C99C` chunk start) was the one function GitHub issue #53
identified as a good semantics-understood candidate outside its literal
chunk boundary. It sat between `polar_pickups.c`'s `UpdatePolarNitroCrate` and
`polar_crates.c`'s `UpdatePolarAkuAkuCrate` - directly adjacent to both once
matched, so it gets a new file, `src/vehicle/polar_nitro.c`, slotted
into `ldscript.txt` at its real ROM address in place of the removed
`asm/code_3_2_20_28568_c7a8.s`.

With that done, this pass also picked up the first 12 functions of
issue #53's own chunk (`0x0802C99C`-`0x0802CC9C`, directly adjacent to
`DetonateNearbyPolarNitros`/`UpdatePolarAkuAkuCrate`) - `UpdatePolarTimeCrate`, `DetonatePolarNitroCrate`,
`sub_802CA6C`, `UpdatePolarBasicCrate`, `InitPolarCrate`, `CreatePolarTimeCrate`,
`CreatePolarQuestionCrate`, `CreatePolarAkuAkuCrate`, `CreatePolarNitroCrate`, `CreatePolarLifeCrate`,
`sub_802CC54`, `CreatePolarBasicCrate` - all genuinely **matched as real C**, in
a new file `src/vehicle/polar_crates.c`. The raw
`asm/code_3_2_20_28568_c99c.s` fragment (which spanned far beyond this
chunk) was cut at that boundary and its remainder renamed
`asm/code_3_2_20_28568_c99c_cc9c.s`, starting at `UpdatePolarElectricFence` - the
next function, a larger AABB-overlap-driven state machine, not
attempted this pass.

## Semantics

Called from `UpdatePolarNitroCrate` (`polar_pickups.c`) once a "used" pickup
(`self`, state `self+0xc == 0x12`) has stayed used for `self+0x44 ==
0x14` frames. It walks the whole `self+0x4c`-rooted circular actor
list - the same sentinel-head list `IsActorVisible`/`DestroyPolarPlayer`/
`DestroyPolarCollectedWumpa` unlink from, rooted at `gActorList` (the player
object) - looking for every *other* actor whose type byte
(`*(u8*)(*(u8**)(node+0x30))`, the same type-byte indirection
`UpdatePolarQuestionCrate` dispatches on) is `4`, and that overlaps `self`'s own
translated `self+0x38` AABB.

Both `self`'s and the candidate node's boxes use the same 12-byte
`{s16 x, y, z, sizeX, sizeY, sizeZ}` record documented in
`docs/matching/archive/issue-54-actor-d3a8.md`'s "Pinning down the 12-byte
AABB-record layout" section: each box is copied from `+0x38` into a
stack scratch buffer, then translated into world space by adding the
owning object's own `+0x1c`/`+0x20`/`+0x24` position (each `>>8`) into
just the `x`/`y`/`z` fields (the `sizeX`/`sizeY`/`sizeZ` half stays raw
- these are extents, not absolute coordinates). Both boxes are then run
through `MemCopy32` - the same confirmed no-op `memcpy(dst, dst,
0xc)` self-copy documented in `yeti_update.c` (`MemCopy32`'s own
definition lives in `src/system/boot.c`, a real `CpuSet`-wrapper
`memcpy`) - kept byte-faithful, not simplified away. The 3-axis overlap
test itself compares Z, then Y, then X (matching the ROM's own
instruction order, not storage order), exactly like `UpdateYeti`/
`IsTouchingYeti`'s player-overlap test.

On overlap, and only while the candidate node isn't already in the used
state (`node+0xc != 0x12`), fires the same shared "used"-state
transition idiom seen throughout this ROM region
(`UpdatePolarCrate`/`UpdatePolarQuestionCrate`/`UpdatePolarLifeCrate` in `polar_pickups.c`): a
sound cue (`PlaySfx(gAudioContext, 4, 0x100)` - the same sound id 4
`UpdatePolarNitroCrate`'s own proximity-pickup branch uses), the lap-counter tie
`AddBrokenCrate(gLevelState)`, `node+0x44`/`node+0x12`/`node+8`
cleared, `node+0xc = 0x12`, and the node's own anim-frame base reloaded
from its part table's `+0xd8` halfword into `node+0x10`. In short: once
one pickup has been "used" for 0x14 frames, it triggers a proximity
chain-reaction that also marks every nearby type-4 actor used.

The list walk itself starts at `gActorList`'s own `+0x4c` (its
first list member, skipping the sentinel head itself) and continues via
each node's own `+0x4c` until it wraps back around to
`gActorList` itself - a plain circular singly-linked list, one
iteration per member, `self` skipped via an explicit `node != self`
check (since `self` is itself a member of the same list).

## Why NAKED, not plain C

This is the exact same heavy-stack-AABB-plus-register-reuse shape this
project already NAKED-parked twice for `UpdateYeti`/`IsTouchingYeti`
(`src/vehicle/yeti_update.c`/`yeti_graphics.c`, `docs/matching/issue-54-actor-
d3a8.md`) - two 12-byte scratch AABB records built via raw `ldm`/`stm`
block copies inside one 0x24-byte stack frame, with the second box's
scratch address (`add r7, sp, #0x18`) computed once and held in `r7`
for the *entire* loop body, spanning both `MemCopy32` calls and the
whole comparison chain.

A real C attempt got remarkably close before hitting this: pinning
`self` to `sb`/`r9` (matching the ROM's own `mov sb, r0`), `node` to
`r4`, and a zero constant to `r8` (all matching the ROM's own register
roles for those three) reproduced the correct 0x24-byte stack frame,
the correct `gActorList` double-reload (once at entry, once
again at the loop-end condition check - not cached across the
intervening `PlaySfx`/`AddBrokenCrate` calls, since referencing the global
directly at both C-level use sites rather than caching it in a local
lets the compiler's own conservative cross-call reload behavior do the
work), and the correct zero-register reuse for the `+0x44`/`+0x12`/`+8`
clear-on-transition (`register s32 zero asm("r8") = 0;` declared once
outside the loop, matching the ROM's own `movs r0,#0; mov r8,r0`
hoisted before the loop rather than re-materialized per iteration).

What it could not reproduce is the ROM's `r7` role: this compiler's
unforced register allocator never lands the second box's scratch
pointer in `r7` for the AABB-record-building/translate section, instead
spilling it to a stack slot (needing a fresh high register, `sl`, just
to hold `sp` across the halfword store sequence) or reusing a different
low register each time depending on how the two boxes' temp/final
locals are split. This is the same categorical "r7 cannot be pinned in
this toolchain, ever" limitation from `matching_decomp_register_pinning`
memory point 10, already on file for `CheckSpritePickup`
(`src/objects/sprite.c`) and `MovePolarAkuAku` (this issue's own
sibling, `docs/matching/archive/issue-54-actor-d3a8.md`) - not something more
C-level rephrasing was likely to fix, so parked the same way as its two
closest siblings rather than continuing to chase it.

Every instruction in the `NAKED` body below is a direct, byte-verified
transcription of the ROM's own disassembly (`expected/code_3.s`,
identical to the removed `asm/code_3_2_20_28568_c7a8.s`), translated
from the disassembler's unified syntax to this project's established
NAKED plain/divided syntax (`adds`->`add`, `asrs`->`asr`, `movs`->`mov`,
`lsls`->`lsl`, `ldm`/`stm` kept as the base non-`ia` form the raw file
already used), with the original's `_08XXXXXX:` labels renumbered to
GNU-as local numeric labels (`N:`, referenced `Nf`/`Nb`) - not an
inferred control-flow guess.

## Matched (real C): UpdatePolarTimeCrate-CreatePolarBasicCrate, 12 functions

Directly adjacent to `UpdatePolarAkuAkuCrate` (`polar_crates.c`), the first 12
functions of issue #53's own `0x0802C99C` chunk start are genuinely
matched as plain C, in `src/vehicle/polar_crates.c`:

- **`UpdatePolarTimeCrate`** - the type-byte-dispatch/proximity family
  (`UpdatePolarQuestionCrate`'s shape, `polar_pickups.c`): on `IsTouchingPlayer`
  proximity and `self+0xc != 0x12`, plays a sound, ties the lap
  counter, then `switch`es on `self+0x30`'s type byte (`5`/`6`/`7` each
  dispatch a different `FreezeLevelClock` tier) before the shared used-state
  transition; tail-calls `UpdatePolarCrate`.
- **`DetonatePolarNitroCrate`** - the same used-state transition, unconditional
  (no `IsTouchingPlayer` guard), also clearing `self+0x44`; no tail call.
- **`sub_802CA6C`**/**`UpdatePolarBasicCrate`** - the proximity-gated shape again,
  forwarding a fixed accumulator delta (`4`/`1`) to
  `QueuePolarWumpa(gActorList, ...)`; each tail-calls `UpdatePolarCrate`.
- **`InitPolarCrate`** - an `InitActorPart`-based constructor: installs
  `self+0x50 = gPolarCrateVtable`, then classifies a "kind"
  (`self+0xc`) from a `__divsi3`-scaled function of the `b`
  parameter (clamped to `[0, 5]`) plus up to two `+6` bumps keyed off
  the `c` parameter's own range (`<= 0x2b`, `<= 6`) - selecting one of
  up to 18 per-kind anim records from the part table (`self[0]`, stride
  `0xc`) to seed `self+0x10`/`self+0x12`/`self+8`, the same idiom as
  `CreatePolarBoostPad` (`src/vehicle/polar_aku_aku.c`).
- **`CreatePolarTimeCrate`**, **`CreatePolarQuestionCrate`**, **`CreatePolarAkuAkuCrate`**,
  **`CreatePolarNitroCrate`**, **`sub_802CC54`**, **`CreatePolarBasicCrate`** - thin
  `InitPolarCrate`-forwarding constructors, each installing a different
  `self+0x50` event table
  (`gPolarTimeCrateVtable`/`ED4`/`EF4`/`F14`/`F54`/`F74`).
- **`CreatePolarLifeCrate`** - same shape, `self+0x50 = gPolarLifeCrateVtable`,
  plus a 6th argument stashed into `self+0x54`.

Every one of these matched cleanly on the first or second isolated
compile except two register-order gotchas worth recording:

1. **`DetonatePolarNitroCrate`'s doubled zero.** A first draft wrote
   `*(s32 *)(self + 0x44) = 0;` as a plain literal, separate from the
   later `register s32 zero2 asm("r2") = 0;` used for the shared
   anim-reset block's `self+8` store. The ROM reuses a *single* `r2`
   zero for both stores (materialized once, after the `PlaySfx`/
   `AddBrokenCrate` calls, then carried through the intervening `self+0xc`/
   anim/`+0x12` writes to the final `self+8` store) - two independent
   zero materializations cost 2 extra bytes, which cascaded into a
   4-byte function-boundary shift once the trailing `.align 2, 0`
   tipped over a 4-byte boundary differently than the ROM's own,
   corrupting every subsequent function's address. Caught by the
   step-6 full-link `make compare` failing (`La suma no coincide`)
   after the isolated per-file compile looked fine - `UpdatePolarTimeCrate`'s
   own isolated compile was genuinely byte-identical, so this was
   invisible until the whole batch was cut into its real files and
   linked, exactly docs/workflow.md's warning about isolated compiles.
   Fix: wrap `self+0x44 = 0;` and the trailing `self+8 = zero2;` in the
   *same* `register s32 zero2 asm("r2") = 0;` scope as the anim block,
   matching the ROM's single materialization.
2. **`InitPolarCrate`'s table-lookup register swap.** `u8 *table =
   *(u8 **)self; u16 anim = *(u16 *)(table + idx * 0xc);` compiled
   `table` into `r0` (evaluated first, source order) and the
   `idx * 0xc` offset into `r1` - the ROM has it backwards (`ldr r1,
   [r5]` for `table`, then the offset math in `r0`). Reordering the
   C statements or the addition's operands didn't change gcc's choice
   (it canonicalizes the commutative add either way); only explicitly
   pinning `table` to `r1` (`register u8 *table asm("r1") = *(u8
   **)self;`) forced the offset computation into the remaining `r0`,
   matching the ROM exactly - without disturbing the earlier
   clamp/range-check section's own `r2` choice for `idx` (an earlier
   attempt that introduced a *plain* `s32 offset` local before `table`
   fixed the register order here but rippled backward and knocked
   `idx` from `r2` to `r1` in the clamp section above, a good
   reminder that even a non-pinned extra local can shift unrelated
   register choices elsewhere in the same function).

## Verification

Full clean rebuild confirmed twice (`rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`), once for
`DetonateNearbyPolarNitros` alone and again after adding the 12-function batch:
`crashbandicootxs.gba: La suma coincide`.

## What's left

`UpdatePolarElectricFence` onward (the remainder of issue #53's own
`0x0802C99C`-`0x0802D3A8` chunk, now `asm/code_3_2_20_28568_c99c_cc9c.s`)
is a larger, `IsTouchingYeti`/`IsTouchingPlayer`/`ShockPolarPlayer`-calling state
machine with three branches and heavy `r5`/`r6`/`r7` register reuse -
not attempted this pass, issue #53 stays open. The wider raw actor
spans this pass also looked at (`0x0802CC9C` onward before issue #54's
chunk, and `0x0802E0A4` onward after it) are large, still-unattempted
raw runs beyond what's matched/parked above; no other functions from
either were matched or parked this pass. `DetonateNearbyPolarNitros` does **not**
count toward closing issue #53 (it's NAKED-parked, not a real C match),
and the 12 matched functions above are only part of issue #53's own
chunk, not all of it - issue #53 stays open.

See [docs/status/actor.md](../../status/actor.md) for the running
matched/parked/left-raw list this entry feeds into.

## Follow-up: the rest of the 0x0802CC9C-0x0802D3A8 gap

A later pass closed out `UpdatePolarElectricFence` (NAKED-parked - the heavy
`r5`/`r6`/`r7`-reuse state machine flagged above) and the 12 functions
after it, finishing this whole gap up to issue #54's own
`MovePolarAkuAku`. See
[docs/matching/archive/issue-53-issue-54-gap-cc9c.md](./issue-53-issue-54-gap-cc9c.md).

## Later pass: `DetonateNearbyPolarNitros` promoted

`DetonateNearbyPolarNitros` (in GitHub issue #52's range) is now plain C under
old_agbcc (`polar_nitro.c` moved). It uses the same `ActorsOverlap`
inline as `actor_category_frame.c`, with the three boxes in one frame struct.
gcc then hoists the third box's address into `r7` by itself, with no
pin. See
[issue-48-49-52-aabb-naked-retry.md](issue-48-49-52-aabb-naked-retry.md).
