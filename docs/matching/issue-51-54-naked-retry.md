# Issues #51 and #54: NAKED retry

A retry of the 14 NAKED functions left in issue #51's range
(`0x0802AC28`-`0x0802BED8`, all in `src/graphics/actor_part127.c`) and
issue #54's range (`0x0802D3A8`-`0x0802E0A4`: `actor_part62.c`,
`actor_part74.c`, `actor_part75.c`, `actor_part76.c`). 11 closed, 3 are
still NAKED with near-miss C drafts under `#if NON_MATCHING`.

## Closed (11)

### `actor_part127.c`: all 7, file moved to old_agbcc

Every function in the file matches under old_agbcc, including the four
that were already C. Three of them match only under old_agbcc, so the
file is now on the Makefile's `OLD_AGBCC_OBJS` list.

| Function | What it took |
|---|---|
| `sub_802B364` | The per-frame update. It is the same shape as `actor_part128.c`'s `sub_802E84C`: branchless `Abs()` for the `visible` word, the inline anim-advance block, and `ACTOR_PMF_CALL(self, gStaticData_0817A6B8)`, which covers the "stride-8 keyframe lookup / r7 hazard" in the old note. Two details mattered. The keys are read as a `struct held_pressed_pair` copy with `keys.held & 0x20` and then `u16 right = keys.held & 0x10`, which keeps the key word in r2. The tier argument of `sub_802B1A8` is read into a local first so it loads before x/y/z. Matches under both compilers. |
| `sub_802B5B4` | The sprite draw. It is `sub_802E9FC` (actor_part128.c) with a `0x2f00000` projection constant, `CurFrame`/`CurAttr` inlines and `GET_TILE_NUM`. Needs old_agbcc; under the current agbcc 2 halfwords are off, in the `attr` load. |
| `sub_802B864` | Plain C: `h * w * 32` with `f[1] * f[0]` operand order, and `animTime >> 8` evaluated first. The "multiply into one register, copy to a second" sequence is just what old_agbcc emits for this. Needs old_agbcc. |
| `sub_802B8E8` | Plain C. `ACTOR_SET_STATE` plus `ACTOR_VCALL(*spawnAddr, m08, 3)`, with the spawn slot read through one `struct actor_self **` local. Matches under both. |
| `sub_802BA5C` | Plain C. The old comment's `0xA000` threshold is really `0x2800`. Matches under both. |
| `sub_802BAD0` | Plain C with `u16 bit = *(u32 *)input & 2` reused as the zero. Needs old_agbcc because the `1` mask is loaded before the `ldrh`. |
| `sub_802BBE4` | Plain C with `ACTOR_SET_STATE`. Matches under both. |

### `actor_part62.c`: `sub_802D3A8` (current agbcc, no pins)

The old note blamed an r7 pin. Without any pins the prologue order
comes out right on its own. What was left needed three changes:

- Each table offset goes through its own local (`ox`/`oy`) and the
  position is added afterwards (`tx = posX + ox`). This stops gcc from
  folding `posX - 0x1000` and hoisting it into r8.
- State 1 gets its own `oy`, with `self->x = posX` before it, so that
  it does not share a pseudo with state 0's `ty`.
- An explicit `goto ease_y` from state 0 into the default case's Y/Z
  easing, carrying `cur`/`d`, reproduces the ROM's shared tail.

A file-level `asm(".align 2, 0")` supplies the ROM's zero padding.

### `actor_part74.c`: `sub_802D9A8`, `sub_802DA68` (file moved to old_agbcc)

- `sub_802D9A8` is the palette ramp. It needs `DmaCopy16`/`DmaFill16`,
  and the two `0x1f` masks must be separate locals (`mask` before the
  pointers, `mask2` after them), which puts one in ip and one in r7 as
  in the ROM. The ROM packs `R | G<<5 | G<<10`, reusing the scaled
  green for blue. Matches under both compilers.
- `sub_802DA68` is the affine BG2 setup; the old comment described
  `0x0400000C`/`0x04000020`-`0x2C` as sound registers, which they are
  not. The flag address is copied into a local after the load
  (`u8 *p = &flag; v = *p; changed = p;`, with `v` pinned to r1). The
  `REG_BG2CNT` value is written as an if/else of two stores. Needs
  old_agbcc, where the `1` of the XOR loads before the `ldrb`.

`sub_802D7B0` stays NAKED in the same file. The NAKED body assembles
identically under either compiler.

### `actor_part76.c`: `sub_802E058` (current agbcc)

A plain nested loop. The condition has to be written as the "store
0xff" test so that branch comes first. The function is UNUSED (it has
no caller).

## Not closed (3), drafts in tree

- **`sub_802D7B0`** (actor_part74.c): right size, about 52 halfwords
  off under old_agbcc (55 under agbcc). Box A is built with
  `BoxMove(&a, x, 0, z)`: the zero Y is what gets the ROM's x-then-z
  evaluation order. Box B comes from a struct-returning `ActorBox()`.
  The overlap test needs `goto hit; return 0; hit: return 1;` so the
  `0` path falls through. What remains: `&b` (sp+0xc) is computed once
  into a callee-saved register before the player box is built, where
  the ROM recomputes `add rX, sp, #0xc` after the copy. That costs a
  register, and r5/r6 roles shift for the rest of the function. Passing
  the box by value, wrapping the `sub_800014C` call in an inline and
  using pointer locals all failed to fix it.
- **`sub_802DD9C`** (actor_part75.c): the same box code as a standalone
  function, with the same `&b` hoist (about 51 halfwords off, plus an
  extra r6 push). This is also the leftover recorded for
  `actor_part24b.c`'s `sub_8031378` in issue-58-61-naked-retry.md.
  Whoever fixes it for one of the three should get the other two.
- **`sub_802DE70`** (actor_part75.c): 5 halfwords off under either
  compiler. The two fill loops are a `static inline` copy of
  `sub_802E058`'s body. The clear loop is written as an `s32` address
  walk (`p >= base`, signed, with a separate `zero` local). Only the
  high-register assignment is wrong: the three hoisted addresses
  (`&gUnknown_030014C0`, `&gUnknown_03000898`, `&gUnknown_030014BC`)
  land in r8/sb/sl in that order, where the ROM gives `...14BC` r8.
  Pinning sb/sl moves the loads to the declaration point, and pointer
  locals let gcc hoist the dereferences as well, so neither helps.

### Later pass: all three closed

[actor-zone-naked-retry.md](actor-zone-naked-retry.md) closed the
three, so issue #54 has nothing left. `sub_802D7B0` and `sub_802DD9C`
keep their boxes as members of one stack-frame struct, which makes gcc
rematerialize `&b` from sp the way the ROM does (old_agbcc;
`actor_part75.c` moved to it). `sub_802DE70` passes
`gUnknown_030014BC` straight into a `CurFrame()` inline instead of
going through an `obj` local, which fixes the r8/sb/sl order.

## Verification

`rm -rf build && make NON_MATCHING=1 report` gives no warnings from the
touched files. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` gives `crashbandicootxs.gba: OK`.
