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
| `sub_802C208` | `actor_part19e.c` | none |
| `sub_802F748` | `actor_part44b.c` | none |
| `sub_8030648` | `actor_part21b.c` | none |
| `sub_8031A08` | `actor_part125.c` | none |
| `sub_80322F4` | `actor_part129.c` | none |
| `sub_8032A94` | `actor_part130.c` | none |
| `sub_8030574` | `actor_part20b.c` | state-2 destroy (`m08`) or `sub_802A7B8` |
| `sub_8032950` | `actor_part130.c` | state-1 destroy or `sub_802A7B8` |
| `sub_8031A6C` | `actor_part129.c` | two destroy conditions or `sub_802A7B8` |
| `sub_8033B44` | `actor_part31.c` | `sub_802A7B8` unless state 2 has finished |
| `sub_8033E80` | `actor_part37.c` | same, on `gStaticData_0817C4F8` |
| `sub_8033C84` | `actor_part33.c` | returns 0 once state 2 has finished, else 1 |
| `sub_8033FE4` | `actor_part64.c` | same, on `gStaticData_0817C4F8` |
| `sub_802FA38` | `actor_part46b.c` | position update before the dispatch, then a player push-out/damage block |

These functions were not the PMF shape, but they sat in the same files
and closed:

| Function | File | What it is |
|---|---|---|
| `sub_80317E0` | `actor_part125.c` | per-frame update; its dispatch is a call to `sub_8031A08` |
| `sub_8031858` | `actor_part125.c` | damage handler: releases a linked object through its `m38` method, then `ACTOR_SET_STATE` |
| `sub_80318D0`, `sub_8031954`, `sub_80319A0` | `actor_part125.c` | the shared anim-frame-advance-and-clamp idiom |
| `sub_8032718` | `actor_part130.c` | the same idiom, gated by an out-of-bounds check |
| `sub_803283C` | `actor_part130.c` | reward-dispensing destructor |

`actor_part125.c` now has no NAKED functions left.

## Techniques

- **The PMF dispatch** is just `ACTOR_PMF_CALL(self, table)` with the
  table declared as `extern struct actor_pmf table[];` and one
  `ACTOR_CALL_VIA_ALIASES` per file. It needed no register pins, and
  nothing is pinned to `r7`. The ROM's `r7` use is the allocator's own
  choice for the table base.
- **Trailing padding.** Files that end in a lone dispatch function need
  `asm(".align 2, 0");` for the ROM's zero-padded word alignment.
- **Predicate tails.** In `sub_8033B44`/`sub_8033E80`, the ROM loads
  `state` *before* materializing the flag's `1`. That needs
  `state = self->state; step = 1; if (state == 2 && ...) step = 0;`. If
  the field is read directly inside the `if`, the constant goes first.
- **`sub_802FA38`** (issue #56). Its NAKED note said the damage block
  added "`r8`/`sb` register pressure". That turned out to be ordinary
  allocation once the two `sub_803ADB4` calls were written as plain `/`
  (it is libgcc's `__divsi3`, aliased with
  `asm(".set __divsi3, sub_803ADB4")`, the `level_select_parts.h`
  technique). Two more changes were needed. The Z gap is written
  `player->z - (self->base.z - 10)`, so `z - 10` is computed once and
  reused as the `sub_802E674` argument. The absolute values are two
  named locals, in the order `signDx, absDx, signDy, absDy`.
- **The anim-frame-advance idiom** (`sub_80318D0`/`sub_8031954`/
  `sub_80319A0`/`sub_8032718`) had been parked three times on the
  "`#4`/`#6` constants scheduled one instruction early" gap. The ROM
  re-indexes `self->anims[self->animIndex]` for each of
  `loopThreshold`/`loopBase`. Going through a
  `struct anim_frame_record *rec` pointer gives the early scheduling.
  In `sub_8032718`, the `1` that is stored to `+0x14` and later to
  `animDone` is one local, so it stays in `r5`.
- **`sub_80317E0`** shares its destroy tail between the "fell behind the
  camera" path and the state checks. A `goto destroy` into the
  `if` body reproduces the ROM's block order. An `else if (!(...))`
  form puts the `sub_8031A08` call first.
- **`sub_803283C`** had been parked on a claimed parameter-copy order
  gap (`flags` before `self`). A plain C destructor, with the list
  unlink written as `self->l4c->l48 = self->l48; self->l48->l4c =
  self->l4c;`, compiles to the ROM's order, `adds r4, r0` then
  `adds r7, r1`. Both compilers do this. The compiler put `flags` in
  `r7` itself; nothing is pinned.

## Header changes (`include/actor_self.h`)

- `struct actor_vtable` gains the `m38` slot (+0x38), called by
  `sub_8031858` with no argument.
- `struct actor_self`'s `u8 unk_13[5]` is split into `u8 unk_13` and
  `s32 visible` (+0x14), which `sub_8032718` sets. The layout does not
  change.

## Skipped (not the PMF shape)

- `actor_part129.c`: `sub_80321FC` (the documented b/c/d param-save
  order gap, skipped as instructed) and `sub_80325EC` (constructor
  register choreography). Neither was attempted. `sub_803283C` shows
  that parameter-order claims like these can be wrong, so both are
  worth a later look.
- `actor_part130.c`: `sub_80327A4` (sprite draw with an
  `r8`-flag-across-calls shape), `sub_8032C0C`/`sub_8032EA0`/
  `sub_80330FC`/`sub_80336CC` (many-high-register `ip`/`sb`/`sl`/`r8`
  loops), `sub_80331BC`/`sub_8033264` (singleton constructors),
  `sub_8033470`/`sub_8033604` (per-frame drivers with DMA/tile
  streaming). These were checked against their notes only and not
  attempted.

## Verification

`rm -rf build && make NON_MATCHING=1 report` produced no warnings from
the touched files. `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` printed
`crashbandicootxs.gba: OK`.
