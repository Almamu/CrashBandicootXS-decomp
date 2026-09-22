# Issue #49: 0x08029E4C-0x0802A69C (actor)

25-function `decomp-chunk` continuing the BG2-affine scroll/zoom effect
subsystem from issue #48, then `SelectActorCategory` (the category
runtime-state setup function `InitActorCategory` hands off to) plus its
`gUnknown_03000884`-rooted AABB-overlap "self" object cluster, and
finally the `gUnknown_03001400` `sub_effect_table` record accessor
family plus a handful of trailing thin wrappers. See
[issue-48-0x080291a4-actor.md](./issue-48-0x080291a4-actor.md) for the
shared debugging notes (recurring gcc-2.9 codegen patterns) both halves
of this chunk hit.

## Matched (real C)

- `nullsub_6`, `sub_8029E50`, `sub_8029E98`, `sub_8029EB4`
  (`actor_part92.c`/`actor_part99.c`/`actor_part93.c`) - the tail of
  the BG2-affine scroll/zoom subsystem: a no-op stub, committing the
  scroll accumulators to `REG_BG0*`/`REG_BG1*`, and two small target-
  field getters.
- `sub_802A4D4`-`sub_802A650`/`sub_802A668` (`actor_part94.c`) - the
  `gUnknown_03001400` `sub_effect_table` record accessor family
  (`struct sub_effect_record`, see `include/actor_anim.h`), a
  circular-list marker-drawing pass (`sub_802A5E4`), and several
  trivial wrappers/setters.

## Parked - NAKED transcription (byte-correct, not decompiled)

- **`SelectActorCategory`** (`actor_part102.c`) - sets up the selected
  category's runtime state (`gUnknown_03001418` vtable pointer,
  `gUnknown_03001400` `sub_effect_table` pointer, `gUnknown_03001414`
  variant byte), draws the vtable's slot-0 icon, then runs a two-pass
  scan over `gUnknown_03001400[]` comparing each entry's threshold
  against the vtable's own `+0x20` slot. Fully understood; a real-C
  attempt reproduced every instruction but needed one more live
  register (`r9`) than the ROM's own `sb`/`r8` pair to keep the vtable
  pointer, the table pointer, and the loop index simultaneously live.
  Verified byte-for-byte against `baserom.gba`, relocation-aware.
- **`sub_802A018`/`sub_802A110`/`sub_802A3AC`** (`actor_part103.c`) -
  translate `gUnknown_03000884` (the player/list-sentinel) and `self`'s
  own 12-byte `{s16 x,y,z,sizeX,sizeY,sizeZ}` AABB record into stack
  scratch boxes via `sub_800014C`, then run the same 3-axis overlap
  test already established throughout this project
  (`sub_802D7B0`/`sub_802DD9C`/`sub_802C7A8`/`sub_8031378`, see
  `docs/matching.md` and `issue-53-actor-c7a8.md`/
  `issue-54-actor-d3a8.md`/`issue-58-0x08030574-actor.md`).
  `sub_802A018`/`sub_802A110` are near-identical (differ only in guard
  byte: `gUnknown_030014A0` vs `gUnknown_03001506`); `sub_802A3AC` wraps
  the same test in an outer walk of the whole circular actor list. All
  three hit this project's **confirmed categorical gcc-2.9 `r7`
  register-allocation bug** - the unforced allocator never reaches `r7`
  for the second scratch AABB box's address, no matter how the C is
  phrased.
- **`sub_802A208`** (`actor_part103.c`) - fires a scroll enter/exit
  trampoline pair off the selected category's vtable, drives a
  `sub_effect_table` draw loop, then walks the whole circular actor
  list twice (once unconditionally drawing each node's own marker, once
  collecting flagged nodes into `gUnknown_03001408` for a second draw
  pass). Not the AABB-overlap shape above - a different, related
  register-pressure gap: a careful plain-C reconstruction reproduced
  the exact control flow and literal-pool contents but consistently
  needed one extra high register (`r9`) on top of the ROM's own single
  `r8` to hold the vtable/table-pointer/cached-scroll-value trio
  simultaneously live.
- **`sub_802A674`/`sub_802A688`** (`actor_part94.c`) - plain one-call
  trampolines, identical in shape to already-matched siblings elsewhere
  in this project (e.g. `actor_part50.c`'s `sub_802A69C`). This
  compiler's epilogue register allocator picks `r1` for these two
  specific functions' `pop`/`bx` pair instead of the usual `r0` - every
  plain-C phrasing tried (including register-pinning the call argument)
  still compiles `pop {r0}`/`bx r0`; the divergence looks tied to this
  translation unit's cumulative pseudo-register count rather than
  anything controllable per-function. Anchored as NAKED rather than
  chasing the exact trigger further.

All four AABB-cluster/`sub_802A208` functions and both landmark
functions (`SelectActorCategory` here, `InitActorCategory` in issue
#48) were verified byte-for-byte against `baserom.gba` (relocation-
aware diff: every byte the isolated compile disagrees with the ROM on
falls inside a `bl`/`ABS32` relocation range that resolves identically
once linked).
