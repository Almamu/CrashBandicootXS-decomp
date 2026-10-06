# Issue #49: 0x08029E4C-0x0802A69C (actor)

25-function `decomp-chunk` continuing the BG2-affine scroll/zoom effect
subsystem from issue #48, then `SelectActorCategory` (the category
runtime-state setup function `InitActorCategory` hands off to) plus its
`gActorList`-rooted AABB-overlap "self" object cluster, and
finally the `gActorSpawnTable` `sub_effect_table` record accessor
family plus a handful of trailing thin wrappers. See
[issue-48-0x080291a4-actor.md](./issue-48-0x080291a4-actor.md) for the
shared debugging notes (recurring gcc-2.9 codegen patterns) both halves
of this chunk hit.

## Matched (real C)

- `nullsub_6`, `CommitActorBgScroll`, `GetActorBgCenterY`, `GetActorBgCenterX`
  (`actor_bg.c`) - the tail of
  the BG2-affine scroll/zoom subsystem: a no-op stub, committing the
  scroll accumulators to `REG_BG0*`/`REG_BG1*`, and two small target-
  field getters.
- `GetActorCategoryFrameCount`-`UpdateActorCategoryBg2`/`SetActorCategoryExitStatus` (`actor_spawn.c`) - the
  `gActorSpawnTable` `sub_effect_table` record accessor family
  (`struct sub_effect_record`, see `include/actor_anim.h`), a
  circular-list marker-drawing pass (`DestroyAllActors`), and several
  trivial wrappers/setters.

## Parked - NAKED transcription (byte-correct, not decompiled)

- **`SelectActorCategory`** (`actor_category_select.c`) - sets up the selected
  category's runtime state (`gActorCategoryVtable` vtable pointer,
  `gActorSpawnTable` `sub_effect_table` pointer, `gUnknown_03001414`
  variant byte), draws the vtable's slot-0 icon, then runs a two-pass
  scan over `gActorSpawnTable[]` comparing each entry's threshold
  against the vtable's own `+0x20` slot. Fully understood; a real-C
  attempt reproduced every instruction but needed one more live
  register (`r9`) than the ROM's own `sb`/`r8` pair to keep the vtable
  pointer, the table pointer, and the loop index simultaneously live.
  Verified byte-for-byte against `baserom.gba`, relocation-aware.
- **`PolarIsTouchingPlayer`/`JetpackIsTouchingPlayer`/`FindShotTarget`** (`actor_category_frame.c`) -
  translate `gActorList` (the player/list-sentinel) and `self`'s
  own 12-byte `{s16 x,y,z,sizeX,sizeY,sizeZ}` AABB record into stack
  scratch boxes via `MemCopy32`, then run the same 3-axis overlap
  test already established throughout this project
  (`UpdateYeti`/`IsTouchingYeti`/`DetonateNearbyPolarNitros`/`IsTouchingAirship`, see
  `docs/matching.md` and `issue-53-actor-c7a8.md`/
  `issue-54-actor-d3a8.md`/`issue-58-0x08030574-actor.md`).
  `PolarIsTouchingPlayer`/`JetpackIsTouchingPlayer` are near-identical (differ only in guard
  byte: `gPolarPlayerInactive` vs `gJetpackPlayerInactive`); `FindShotTarget` wraps
  the same test in an outer walk of the whole circular actor list. All
  three hit this project's **confirmed categorical gcc-2.9 `r7`
  register-allocation bug** - the unforced allocator never reaches `r7`
  for the second scratch AABB box's address, no matter how the C is
  phrased.
- **`RunActorCategoryFrame`** (`actor_category_frame.c`) - fires a scroll enter/exit
  trampoline pair off the selected category's vtable, drives a
  `sub_effect_table` draw loop, then walks the whole circular actor
  list twice (once unconditionally drawing each node's own marker, once
  collecting flagged nodes into `gActorDrawList` for a second draw
  pass). Not the AABB-overlap shape above - a different, related
  register-pressure gap: a careful plain-C reconstruction reproduced
  the exact control flow and literal-pool contents but consistently
  needed one extra high register (`r9`) on top of the ROM's own single
  `r8` to hold the vtable/table-pointer/cached-scroll-value trio
  simultaneously live.
- **`JetpackIsPauseLocked`/`PolarIsPauseLocked`** (`actor_spawn.c`) - plain one-call
  trampolines, identical in shape to already-matched siblings elsewhere
  in this project (e.g. `actor.c`'s `JetpackReloadPlayerTiles`). This
  compiler's epilogue register allocator picks `r1` for these two
  specific functions' `pop`/`bx` pair instead of the usual `r0` - every
  plain-C phrasing tried (including register-pinning the call argument)
  still compiles `pop {r0}`/`bx r0`; the divergence looks tied to this
  translation unit's cumulative pseudo-register count rather than
  anything controllable per-function. Anchored as NAKED rather than
  chasing the exact trigger further.

All four AABB-cluster/`RunActorCategoryFrame` functions and both landmark
functions (`SelectActorCategory` here, `InitActorCategory` in issue
#48) were verified byte-for-byte against `baserom.gba` (relocation-
aware diff: every byte the isolated compile disagrees with the ROM on
falls inside a `bl`/`ABS32` relocation range that resolves identically
once linked).

## Later pass: NAKED retry

`PolarIsTouchingPlayer`, `JetpackIsTouchingPlayer` and `FindShotTarget` are now plain C under
old_agbcc (`actor_category_frame.c` moved). They share one inline that keeps
the three boxes in one frame struct. `JetpackIsPauseLocked`/`PolarIsPauseLocked` are
plain C: they return the callee's result. `SelectActorCategory` has a
`NON_MATCHING` draft with the ROM's instruction sequence but two
registers swapped. `RunActorCategoryFrame` is unchanged. See
[issue-48-49-52-aabb-naked-retry.md](issue-48-49-52-aabb-naked-retry.md).

## Later pass: second near-miss sweep

`SelectActorCategory` is real C (both compilers). The zeroing store of
`gActorSpawnIndex` goes through a local pointer. An empty
`asm("" : : "r"(idx))` after the `GetCellAnimDistance` call gives that pointer
one more reference, so it outranks `base` and takes r7. Both scan loops
still name the global directly, which gives the ROM's loop-local copies
of the address. See [near-miss-polish-2.md](near-miss-polish-2.md).

## Later pass: category driver retry

`RunActorCategoryFrame` is now plain C under old_agbcc. The extra `r9` came from
the draft caching values across the sub-effect loop; the ROM is a plain
`while` whose exit test gcc copies ahead of the loop, so the body
starts at a label and re-reads every global. See
[category-driver-naked-retry.md](category-driver-naked-retry.md).
