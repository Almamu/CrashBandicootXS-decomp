# Retrying the "r7 table-base" NAKED functions with `ACTOR_PMF_CALL`

Issue #57's pass (`docs/matching/issue-57-0x0802fbf0-actor.md`) found
that the shape this project had parked as NAKED again and again, the
"r7 table-base-pin hazard" or "stride-8 trampoline table
dispatcher", is gcc 2.x's pointer-to-member-function call
`(this->*table[this->state])()`. The `ACTOR_PMF_CALL` macro in
`include/actor_self.h` reproduces it byte for byte with no register
pins. This pass retried every NAKED function that had been parked for
that reason, plus a few same-file neighbors that turned out to be
closable too.

**Compiler:** the current `agbcc`. No file needed `old_agbcc`, and none
was added to `OLD_AGBCC_OBJS`.

## Closed (21 functions)

| Function | File | Tail after the dispatch |
|---|---|---|
| `RunPolarPlayerState` | `actor_part19e.c` | none |
| `RunJetpackPlayerState` | `actor_part44b.c` | none |
| `RunAirshipFireballState` | `actor_part21b.c` | none |
| `RunJetpackBalloonState` | `actor_part125.c` | none |
| `RunJetpackBalloonCrateState` | `actor_part129.c` | none |
| `RunHovercraftFireballState` | `actor_part130.c` | none |
| `UpdateAirshipFireball` | `actor_part20b.c` | state-2 destroy (`m08`) or `UpdateActor` |
| `UpdateHovercraftFireball` | `actor_part130.c` | state-1 destroy or `UpdateActor` |
| `UpdateJetpackBalloonCrate` | `actor_part129.c` | two destroy conditions or `UpdateActor` |
| `UpdateHovercraftCannon` | `actor_part31.c` | `UpdateActor` unless state 2 has finished |
| `UpdateHovercraftLauncher` | `actor_part37.c` | same, on `gHovercraftLauncherStateFuncs` |
| `RunHovercraftCannonState` | `actor_part33.c` | returns 0 once state 2 has finished, else 1 |
| `RunHovercraftLauncherState` | `actor_part64.c` | same, on `gHovercraftLauncherStateFuncs` |
| `UpdateJetpackPlane` | `actor_part46b.c` | position update before the dispatch, then a player push-out/damage block |

These functions were not the PMF shape, but they sat in the same files
and closed:

| Function | File | What it is |
|---|---|---|
| `UpdateJetpackBalloon` | `actor_part125.c` | per-frame update; its dispatch is a call to `RunJetpackBalloonState` |
| `DamageJetpackBalloon` | `actor_part125.c` | damage handler: releases a linked object through its `m38` method, then `ACTOR_SET_STATE` |
| `MoveJetpackBalloon`, `sub_8031954`, `sub_80319A0` | `actor_part125.c` | the shared anim-frame-advance-and-clamp idiom |
| `UpdateJetpackCollectedWumpa` | `actor_part130.c` | the same idiom, gated by an out-of-bounds check |
| `DestroyJetpackCollectedWumpa` | `actor_part130.c` | reward-dispensing destructor |

`actor_part125.c` now has no NAKED functions left.

## Techniques

- **The PMF dispatch** is just `ACTOR_PMF_CALL(self, table)` with the
  table declared as `extern struct actor_pmf table[];` and one
  `ACTOR_CALL_VIA_ALIASES` per file. It needed no register pins, and
  nothing is pinned to `r7`. The ROM's `r7` use is the allocator's own
  choice for the table base.
- **Trailing padding.** Files that end in a lone dispatch function need
  `asm(".align 2, 0");` for the ROM's zero-padded word alignment.
- **Predicate tails.** In `UpdateHovercraftCannon`/`UpdateHovercraftLauncher`, the ROM loads
  `state` *before* materializing the flag's `1`. That needs
  `state = self->state; step = 1; if (state == 2 && ...) step = 0;`. If
  the field is read directly inside the `if`, the constant goes first.
- **`UpdateJetpackPlane`** (issue #56). Its NAKED note said the damage block
  added "`r8`/`sb` register pressure". That turned out to be ordinary
  allocation once the two `__divsi3` calls were written as plain `/`
  (it is libgcc's `__divsi3`, then reached through a `.set` alias,
  the `level_select_parts.h` technique). Two more changes were needed. The Z gap is written
  `player->z - (self->base.z - 10)`, so `z - 10` is computed once and
  reused as the `SpawnJetpackCannonball` argument. The absolute values are two
  named locals, in the order `signDx, absDx, signDy, absDy`.
- **The anim-frame-advance idiom** (`MoveJetpackBalloon`/`sub_8031954`/
  `sub_80319A0`/`UpdateJetpackCollectedWumpa`) had been parked three times on the
  "`#4`/`#6` constants scheduled one instruction early" gap. The ROM
  re-indexes `self->anims[self->animIndex]` for each of
  `loopThreshold`/`loopBase`. Going through a
  `struct anim_frame_record *rec` pointer gives the early scheduling.
  In `UpdateJetpackCollectedWumpa`, the `1` that is stored to `+0x14` and later to
  `animDone` is one local, so it stays in `r5`.
- **`UpdateJetpackBalloon`** shares its destroy tail between the "fell behind the
  camera" path and the state checks. A `goto destroy` into the
  `if` body reproduces the ROM's block order. An `else if (!(...))`
  form puts the `RunJetpackBalloonState` call first.
- **`DestroyJetpackCollectedWumpa`** had been parked on a claimed parameter-copy order
  gap (`flags` before `self`). A plain C destructor, with the list
  unlink written as `self->l4c->l48 = self->l48; self->l48->l4c =
  self->l4c;`, compiles to the ROM's order, `adds r4, r0` then
  `adds r7, r1`. Both compilers do this. The compiler put `flags` in
  `r7` itself; nothing is pinned.

## Header changes (`include/actor_self.h`)

- `struct actor_vtable` gains the `m38` slot (+0x38), called by
  `DamageJetpackBalloon` with no argument.
- `struct actor_self`'s `u8 unk_13[5]` is split into `u8 unk_13` and
  `s32 visible` (+0x14), which `UpdateJetpackCollectedWumpa` sets. The layout does not
  change.

## Skipped (not the PMF shape)

- `actor_part129.c`: `InitJetpackBalloonCrate` (the documented b/c/d param-save
  order gap, skipped as instructed) and `CreateJetpackRocket` (constructor
  register choreography). Neither was attempted. `DestroyJetpackCollectedWumpa` shows
  that parameter-order claims like these can be wrong, so both are
  worth a later look.
- `actor_part130.c`: `DrawJetpackCollectedWumpa` (sprite draw with an
  `r8`-flag-across-calls shape), `sub_8032C0C`/`sub_8032EA0`/
  `DrawHovercraftMap`/`ConvertHovercraftTiles` (many-high-register `ip`/`sb`/`sl`/`r8`
  loops), `CreateHovercraft`/`SpawnHovercraft` (singleton constructors),
  `UpdateHovercraft`/`LoadHovercraftGraphics` (per-frame drivers with DMA/tile
  streaming). These were checked against their notes only and not
  attempted.

## Verification

`rm -rf build && make NON_MATCHING=1 report` produced no warnings from
the touched files. `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` printed
`crashbandicootxs.gba: OK`.
