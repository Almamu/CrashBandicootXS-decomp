# Issue #97: `sub_8009DF4` (closed in a later session)

**UPDATE: matched in a later session.** The same address/value
register-role gap this doc describes below was independently closed
for `sub_800B270` (its sibling) via an opaque `asm volatile` block
that emits the final `gUnknown_0300129C`-style read-compare-write
sequence verbatim, sidestepping both failure modes this doc documents
rather than fighting them via C-level register pins. Porting that same
technique here - `vx` pinned to `r3`, `vy` left unpinned (both
required; pinning them *together* is a separate gcc-2.9 miscompile
`sub_800B270`'s own write-up covers), and the trailing
`gUnknown_03001298` block emitted as one opaque `asm volatile` instead
of a plain C `if`/store pair - closes `sub_8009DF4` byte-for-byte. The
old raw `asm/code_3_2_9.s` is retired. See `docs/status/actor.md`'s
"Matched" entry for `sub_8009DF4` and
`docs/matching/issue-9-0x08007634-actor.md`'s "Real gotchas" point 3
for the twin fix. The rest of this document is preserved as the
original parking write-up.

`parked-function` issue against `sub_8009DF4` in
`src/graphics/actor_part8.c` (real bytes in `asm/code_3_2_9.s`). Not
closed this session, but the reconstruction was rewritten and the
remaining gap is now much narrower and more precisely understood than
`docs/matching.md`'s frozen entry describes.

## What it does

A velocity/position integrator: for each axis (X at `self+0x60`/
`self+0x50` max/`self+0x4c` accel; Y at `self+0x64`/`self+0x5c`/
`self+0x58`), steps the velocity toward its max by the accel amount,
clamping so it never overshoots. Builds a `self+0x24` direction-flags
byte from the clamped velocities' signs, caches the pre-move position
at `self+0x6c`/`self+0x70`, applies the velocity to `self+0`/`self+4`,
and finally records the resulting Y velocity into
`gUnknown_03001298` (with a genuinely redundant conditional early
write the ROM performs before unconditionally overwriting it with the
same value right after). Returns whether either axis is still moving.

## The old parking rationale, and why it was incomplete

The previous session's parking note claimed this compiler "always
spills one extra value to `r4`... for every register-pin arrangement
tried", including pinning `self` to `r2` directly, and accepted an
8-byte-larger leaf-with-frame version as unavoidable.

This turned out not to be quite right. `sub_8009DF4` is structurally
almost identical to `sub_800B270` (issue #9, `src/graphics/
actor_part49.c`) - same per-axis clamp shape, same field offsets
(`+0x60`/`+0x50`/`+0x4c`, `+0x64`/`+0x5c`/`+0x58`, `+0x24`, `+0`/`+4`),
same true-leaf-function ROM shape, differing only in which global gets
the final Y-velocity write. `sub_800B270` had already been worked out
in enough detail (per-axis `goto`-based clamp with explicit register
pins, `vs32`-forced reloads for the redundant `self->x`/`self->y`
re-reads) to reproduce the ROM one-for-one through the position-update
store, with **no stack frame at all**. Porting that exact structure to
`sub_8009DF4` (register `self` pinned to `r2`, `vx` pinned to `r3`,
`vy` and the clamp temporaries left as plain locals so the compiler's
own allocation - not a source-level pin - lands them correctly)
reproduces the same true-leaf shape here too, closing the "always
needs one extra spilled register" gap entirely for everything up to
the global write.

## What's still gapped

The one thing that doesn't fully close is the same block `sub_800B270`
itself remains parked on (see `docs/matching/issue-9-0x08007634-actor.md`,
"Real gotchas found along the way", point 3): the final
`gUnknown_03001298` read-compare-write block's address/value register
roles. The ROM loads the global's *address* into `r0` and its *value*
into `r2`; this compiler's natural allocation keeps the opposite
(address in `r2`, value in `r0`) - functionally identical, same
instruction count, wrong register letters.

Every register-pin variant tried on this block reproduces one of two
consistent failure modes, both confirmed on `sub_8009DF4` directly
(not just inferred from `sub_800B270`):

1. **Pinning the loaded value** to a specific register (either
   `register s32 gval asm("r2") = *g;` or pinning the whole pointer
   dereference chain so the loaded value lands in a fixed register)
   makes this compiler's dead-store-elimination pass prove the
   `if (*g != 0 && vy == 0) *g = vy;` conditional store is provably
   redundant - since the very next statement unconditionally writes
   the same value anyway - and it deletes the entire conditional,
   collapsing the block down to a plain unconditional store (and, once
   in this narrower testing, folding the whole function's return value
   down to a wrong compile-time constant `1`). The pass isn't
   *incorrect* here, it's simply an optimization the ROM's own build
   didn't apply to this specific code shape.
2. **Pinning the address** to `r0` (`register s32 *g asm("r0") = ...`)
   avoids that elimination but reintroduces a `push {r4, lr}`/
   `pop {r4}` pair elsewhere in the function - this time to preserve
   `vy` across the register reallocation the pin forces, the same
   knock-on-regression shape `sub_800B270`'s own doc entry describes.

Using a plain, unpinned local pointer (`s32 *g = &gUnknown_03001298;`)
avoids both failure modes and keeps the true-leaf shape, but lands on
the ROM's opposite register assignment for that one block. Parked with
that version - true leaf, every instruction correct, only the
address/value register letters swapped in the trailing block.

## Cross-references

- `docs/status/actor.md` - `sub_8009DF4`'s entry rewritten to describe
  the narrowed gap.
- `docs/matching/issue-9-0x08007634-actor.md` - `sub_800B270`'s own
  parking write-up, the sibling function this reconstruction was
  modeled on and shares its exact remaining gap with.
- `docs/matching.md` - the frozen "Parked, not matched: `sub_8009DF4`"
  entry is now stale (its "always spills r4" claim doesn't hold once
  the reconstruction is modeled on `sub_800B270`'s shape) but is left
  unedited per that file's own header.
