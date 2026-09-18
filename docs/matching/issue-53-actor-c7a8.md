# Issue #53: 0x0802C99C-0x0802D3A8 range boundary (actor) - `sub_802C7A8` and 12 more

`sub_802C7A8` (ROM `0x0802C7A8`-`0x0802C8FA`, right before issue #53's
own `0x0802C99C` chunk start) was the one function GitHub issue #53
identified as a good semantics-understood candidate outside its literal
chunk boundary. It sat between `actor_part19g.c`'s `sub_802C6C0` and
`actor_part19d.c`'s `sub_802C904` - directly adjacent to both once
matched, so it gets a new file, `src/graphics/actor_part19h.c`, slotted
into `ldscript.txt` at its real ROM address in place of the removed
`asm/code_3_2_20_28568_c7a8.s`.

With that done, this pass also picked up the first 12 functions of
issue #53's own chunk (`0x0802C99C`-`0x0802CC9C`, directly adjacent to
`sub_802C7A8`/`sub_802C904`) - `sub_802C99C`, `sub_802CA28`,
`sub_802CA6C`, `sub_802CAD0`, `sub_802CB34`, `sub_802CB9C`,
`sub_802CBC0`, `sub_802CBE4`, `sub_802CC08`, `sub_802CC2C`,
`sub_802CC54`, `sub_802CC78` - all genuinely **matched as real C**, in
a new file `src/graphics/actor_part19i.c`. The raw
`asm/code_3_2_20_28568_c99c.s` fragment (which spanned far beyond this
chunk) was cut at that boundary and its remainder renamed
`asm/code_3_2_20_28568_c99c_cc9c.s`, starting at `sub_802CC9C` - the
next function, a larger AABB-overlap-driven state machine, not
attempted this pass.

## Semantics

Called from `sub_802C6C0` (`actor_part19g.c`) once a "used" pickup
(`self`, state `self+0xc == 0x12`) has stayed used for `self+0x44 ==
0x14` frames. It walks the whole `self+0x4c`-rooted circular actor
list - the same sentinel-head list `sub_802AA4C`/`sub_802C19C`/
`sub_802C394` unlink from, rooted at `gUnknown_03000884` (the player
object) - looking for every *other* actor whose type byte
(`*(u8*)(*(u8**)(node+0x30))`, the same type-byte indirection
`sub_802C540` dispatches on) is `4`, and that overlaps `self`'s own
translated `self+0x38` AABB.

Both `self`'s and the candidate node's boxes use the same 12-byte
`{s16 x, y, z, sizeX, sizeY, sizeZ}` record documented in
`docs/matching/issue-54-actor-d3a8.md`'s "Pinning down the 12-byte
AABB-record layout" section: each box is copied from `+0x38` into a
stack scratch buffer, then translated into world space by adding the
owning object's own `+0x1c`/`+0x20`/`+0x24` position (each `>>8`) into
just the `x`/`y`/`z` fields (the `sizeX`/`sizeY`/`sizeZ` half stays raw
- these are extents, not absolute coordinates). Both boxes are then run
through `sub_800014C` - the same confirmed no-op `memcpy(dst, dst,
0xc)` self-copy documented in `actor_part74.c` (`sub_800014C`'s own
definition lives in `src/system/boot_util.c`, a real `CpuSet`-wrapper
`memcpy`) - kept byte-faithful, not simplified away. The 3-axis overlap
test itself compares Z, then Y, then X (matching the ROM's own
instruction order, not storage order), exactly like `sub_802D7B0`/
`sub_802DD9C`'s player-overlap test.

On overlap, and only while the candidate node isn't already in the used
state (`node+0xc != 0x12`), fires the same shared "used"-state
transition idiom seen throughout this ROM region
(`sub_802C4C8`/`sub_802C540`/`sub_802C614` in `actor_part19g.c`): a
sound cue (`PlaySfx(gUnknown_030012BC, 4, 0x100)` - the same sound id 4
`sub_802C6C0`'s own proximity-pickup branch uses), the lap-counter tie
`sub_8022FEC(gUnknown_030012C0)`, `node+0x44`/`node+0x12`/`node+8`
cleared, `node+0xc = 0x12`, and the node's own anim-frame base reloaded
from its part table's `+0xd8` halfword into `node+0x10`. In short: once
one pickup has been "used" for 0x14 frames, it triggers a proximity
chain-reaction that also marks every nearby type-4 actor used.

The list walk itself starts at `gUnknown_03000884`'s own `+0x4c` (its
first list member, skipping the sentinel head itself) and continues via
each node's own `+0x4c` until it wraps back around to
`gUnknown_03000884` itself - a plain circular singly-linked list, one
iteration per member, `self` skipped via an explicit `node != self`
check (since `self` is itself a member of the same list).

## Why NAKED, not plain C

This is the exact same heavy-stack-AABB-plus-register-reuse shape this
project already NAKED-parked twice for `sub_802D7B0`/`sub_802DD9C`
(`src/graphics/actor_part74.c`/`75.c`, `docs/matching/issue-54-actor-
d3a8.md`) - two 12-byte scratch AABB records built via raw `ldm`/`stm`
block copies inside one 0x24-byte stack frame, with the second box's
scratch address (`add r7, sp, #0x18`) computed once and held in `r7`
for the *entire* loop body, spanning both `sub_800014C` calls and the
whole comparison chain.

A real C attempt got remarkably close before hitting this: pinning
`self` to `sb`/`r9` (matching the ROM's own `mov sb, r0`), `node` to
`r4`, and a zero constant to `r8` (all matching the ROM's own register
roles for those three) reproduced the correct 0x24-byte stack frame,
the correct `gUnknown_03000884` double-reload (once at entry, once
again at the loop-end condition check - not cached across the
intervening `PlaySfx`/`sub_8022FEC` calls, since referencing the global
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
memory point 10, already on file for `sub_8007DBC`
(`src/graphics/actor_part2.c`) and `sub_802D3A8` (this issue's own
sibling, `docs/matching/issue-54-actor-d3a8.md`) - not something more
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

## Matched (real C): sub_802C99C-sub_802CC78, 12 functions

Directly adjacent to `sub_802C904` (`actor_part19d.c`), the first 12
functions of issue #53's own `0x0802C99C` chunk start are genuinely
matched as plain C, in `src/graphics/actor_part19i.c`:

- **`sub_802C99C`** - the type-byte-dispatch/proximity family
  (`sub_802C540`'s shape, `actor_part19g.c`): on `sub_802A6EC`
  proximity and `self+0xc != 0x12`, plays a sound, ties the lap
  counter, then `switch`es on `self+0x30`'s type byte (`5`/`6`/`7` each
  dispatch a different `sub_8022EA8` tier) before the shared used-state
  transition; tail-calls `sub_802C4C8`.
- **`sub_802CA28`** - the same used-state transition, unconditional
  (no `sub_802A6EC` guard), also clearing `self+0x44`; no tail call.
- **`sub_802CA6C`**/**`sub_802CAD0`** - the proximity-gated shape again,
  forwarding a fixed accumulator delta (`4`/`1`) to
  `sub_802C078(gUnknown_03000884, ...)`; each tail-calls `sub_802C4C8`.
- **`sub_802CB34`** - an `InitActorPart`-based constructor: installs
  `self+0x50 = gStaticData_087E4F94`, then classifies a "kind"
  (`self+0xc`) from a `sub_803ADB4`-scaled function of the `b`
  parameter (clamped to `[0, 5]`) plus up to two `+6` bumps keyed off
  the `c` parameter's own range (`<= 0x2b`, `<= 6`) - selecting one of
  up to 18 per-kind anim records from the part table (`self[0]`, stride
  `0xc`) to seed `self+0x10`/`self+0x12`/`self+8`, the same idiom as
  `sub_802D648` (`src/graphics/actor_part58.c`).
- **`sub_802CB9C`**, **`sub_802CBC0`**, **`sub_802CBE4`**,
  **`sub_802CC08`**, **`sub_802CC54`**, **`sub_802CC78`** - thin
  `sub_802CB34`-forwarding constructors, each installing a different
  `self+0x50` event table
  (`gStaticData_087E4EB4`/`ED4`/`EF4`/`F14`/`F54`/`F74`).
- **`sub_802CC2C`** - same shape, `self+0x50 = gStaticData_087E4F34`,
  plus a 6th argument stashed into `self+0x54`.

Every one of these matched cleanly on the first or second isolated
compile except two register-order gotchas worth recording:

1. **`sub_802CA28`'s doubled zero.** A first draft wrote
   `*(s32 *)(self + 0x44) = 0;` as a plain literal, separate from the
   later `register s32 zero2 asm("r2") = 0;` used for the shared
   anim-reset block's `self+8` store. The ROM reuses a *single* `r2`
   zero for both stores (materialized once, after the `PlaySfx`/
   `sub_8022FEC` calls, then carried through the intervening `self+0xc`/
   anim/`+0x12` writes to the final `self+8` store) - two independent
   zero materializations cost 2 extra bytes, which cascaded into a
   4-byte function-boundary shift once the trailing `.align 2, 0`
   tipped over a 4-byte boundary differently than the ROM's own,
   corrupting every subsequent function's address. Caught by the
   step-6 full-link `make compare` failing (`La suma no coincide`)
   after the isolated per-file compile looked fine - `sub_802C99C`'s
   own isolated compile was genuinely byte-identical, so this was
   invisible until the whole batch was cut into its real files and
   linked, exactly docs/workflow.md's warning about isolated compiles.
   Fix: wrap `self+0x44 = 0;` and the trailing `self+8 = zero2;` in the
   *same* `register s32 zero2 asm("r2") = 0;` scope as the anim block,
   matching the ROM's single materialization.
2. **`sub_802CB34`'s table-lookup register swap.** `u8 *table =
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
`sub_802C7A8` alone and again after adding the 12-function batch:
`crashbandicootxs.gba: La suma coincide`.

## What's left

`sub_802CC9C` onward (the remainder of issue #53's own
`0x0802C99C`-`0x0802D3A8` chunk, now `asm/code_3_2_20_28568_c99c_cc9c.s`)
is a larger, `sub_802DD9C`/`sub_802A6EC`/`sub_802B7E0`-calling state
machine with three branches and heavy `r5`/`r6`/`r7` register reuse -
not attempted this pass, issue #53 stays open. The wider raw actor
spans this pass also looked at (`0x0802CC9C` onward before issue #54's
chunk, and `0x0802E0A4` onward after it) are large, still-unattempted
raw runs beyond what's matched/parked above; no other functions from
either were matched or parked this pass. `sub_802C7A8` does **not**
count toward closing issue #53 (it's NAKED-parked, not a real C match),
and the 12 matched functions above are only part of issue #53's own
chunk, not all of it - issue #53 stays open.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked/left-raw list this entry feeds into.
