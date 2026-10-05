# Issue #20: 0x08016128-0x08017524 (graphics), plus issue #19's `sub_8016048`

All 26 functions of the former `asm/code_3_2_17_16048.s` are byte-exact
real C in the new `src/graphics/actor_part_16048.c`: `sub_8016048` (the
last raw function of issue #19, see
[issue-19-0x08015840-actor.md](./issue-19-0x08015840-actor.md)) and the 25
functions of issue #20. No NAKED, no `NON_MATCHING`. The only register
pins are in `sub_8016AB0`. The file is built with
`tools/agbcc/bin/old_agbcc` (it is on the Makefile's `OLD_AGBCC_OBJS`).

## Compiler: old_agbcc, for the whole range

Every function was compiled with both compilers from the same C source.
Under old_agbcc all 26 match. Under the current agbcc 9 of them do not.
The clearest case is `PlayerCtrlKillPlayer`. It is the same method as issue #21's
`InputCtrlKillPlayer`, which needed four register-pinned blocks under agbcc. As
plain C with bitfield clears, old_agbcc gives the ROM's
`movs r0, #0x7f; ldrb r2, [r1, #0xc]; ands r0, r2` (constant before the
byte). agbcc loads the byte first.

**Where the old_agbcc region starts.** It covers at least
`0x08016048`-`0x0801E578`: this file, issue #21's `0x08017524`-`0x08017A44`
(see below) and the already-known `0x080188D0`-`0x0801E578`. Nothing below
`0x08016048` was decided here. `actor_part57b.c`'s `sub_8015FDC`, the
nearest function below, compiles to the same bytes under both compilers,
both with its current pins and as plain C, so it does not tell them
apart. `actor_part57.c` and `actor_part38d.c` also build identically under
both.

**Issue #21 (`actor_part_17524.c`) looks like old_agbcc code.** Its
`InputCtrlKillPlayer`, rewritten without any pins (bitfield clears for the two
flag bits, a plain `records[tag * 28 + 0x14]` read), matches under
old_agbcc and is 30 bytes off under agbcc. The file itself was not
changed. Moving it to `OLD_AGBCC_OBJS` would probably let most of its pins
and the `volatile` id reload go (see the `do`/`while (0)` finding below).
A later pass did that: `actor_part_17524.o` is on `OLD_AGBCC_OBJS` with
no pins, `volatile` or barriers left (see
[issue-21-input-ctrl.md](./issue-21-input-ctrl.md)). The same pass
matched issue #19's `sub_80159F8`/`sub_8015C6C`/`sub_8015DF8`
(`0x080159F8`-`0x08015FDC`, `actor_part86.o`/`actor_part86b.o`) under
old_agbcc too (the issue #17 pass had already found old_agbcc code at
`0x08013C60`-`0x08014F8C`, `actor_part_13c60.c`/`actor_part_14674.c`).
`sub_8015C6C` has the tell; `actor_part57b.c` in between still does not
tell the compilers apart.

## What the code is

`include/player_ctrl.h` has the layouts. `struct player_ctrl` is a C++
class with method table `gPlayerCtrlVtable`:

| slot | function |
|---|---|
| +0x0C | `UpdatePlayerCtrl` per-frame update |
| +0x14 | `PlayerCtrlHandleEvent` message handler |
| +0x1C | `AttachPlayerCtrl` set target |
| +0x4C | `DestroyPlayerCtrl` destructor |

The constructor is `InitPlayerCtrl`, called from `game_loop39.c`. Its field
reset is `actor_part57.c`'s `ResetPlayerCtrl`. The #19 dispatchers
`sub_80159F8`/`sub_8015C6C`/`sub_8015DF8` and `sub_8015FDC` are methods of
the same class. The target (`+0x10`) is the player object.

- `UpdatePlayerCtrl` counts down `cooldown`. With D-pad up/down (or in state 2)
  it steps `level` (0..12) with a 3-frame auto-repeat (`repeat`).
  Otherwise `level` drifts back towards 6. On each step it re-applies the
  target's animation (`ApplyLevel`; its out-of-line copy is
  `sub_8017348`). Then it runs the current state's handler through
  `gStaticData_0816C250`. That table holds gcc 2.x pointer-to-member
  functions for states 0-7: `sub_8016B1C`, `sub_8016C08`, `sub_8016C94`,
  `sub_8016D5C`, `sub_8016DDC`, `sub_80170EC`, `sub_8017044`,
  `sub_8017184`. It then clears the player's speeds on contact
  (`+0x74`), calls `sub_8015DF8`, and sets the target's `+0x0A`.
- `sub_8017264(self, a, mode, timer, timerMax)` calls method `+0x20(a)`,
  stores `mode`, then calls method `+0x50(target, anim)`. The animation
  comes from `gStaticData_0816C070[mode][level]`, 8 rows of 13 words read
  as bytes. `0x7FFFFFFF` means "leave `timer`/`timerMax` unchanged".
- `sub_80172D0` writes the player's `+0x48`/`+0x4C`/`+0x50` record from its
  speed. It is the `+0x54` twin of `sub_8015FDC`. `sub_8017330` is its
  `v * v / 0x4000 + 4` term on its own.
- `sub_8016AB0`/`sub_801721C`/`sub_8017240` apply animation pairs through
  `gStaticData_0816B61C` (12-byte records). They are the same shape as
  #21's `sub_8017808`.

UNUSED: there is no reference in `asm/`, `src/` or `data/`, and no Thumb
pointer anywhere in the ROM, for `sub_8016AB0`, `sub_801721C`,
`sub_8017240`, `sub_8017330`, `sub_8017348`, `sub_80174BC`, `sub_801750C`,
`sub_8017514` and `sub_801751C`.

## Matching notes

- **`do { } while (0)` is not neutral under this compiler.** gcc 2.x
  emits loop notes for it, and those notes change what CSE does. With the
  virtual-call macros written as `do`/`while (0)`, CSE reused a constant
  across two calls in `sub_8016B1C` and the address arithmetic in
  `sub_8017264` changed. Turning them into `({ })` statement expressions
  fixed that. But `({ })` leaves a `(use (const_int 0))` insn after the
  call, and that insn stopped the post-reload cross-jumping that
  `sub_8016DDC`'s ROM shows. So the call macros are plain `{ }` blocks.
  In the other direction, the "mark gone" bitmap update (`SET_ID_BIT`) is
  kept as `do`/`while (0)` on purpose. Its loop notes are what makes the
  id reload after the `0xFFFF` test come out naturally, with no
  `volatile` and no pins. Other files that pin that sequence (#21, #23)
  may be able to drop their pins the same way.
- **C++ inline methods.** Several helpers are inlined at some call sites
  and also emitted out of line. The C keeps a `static inline` helper, and
  the out-of-line function is a one-line call to it:
  `SetState`/`sub_8017264` (inlined in `sub_8016DDC` and `sub_80170EC`),
  `SetPlayerRecord`/`sub_80172D0` (inlined in `sub_8017184`) and
  `ApplyLevel`/`sub_8017348`. `ResetMode` (`sub_80174BC`'s body, a
  call to `sub_8017264`) has to be an inline helper at its call sites.
  Written as a direct call in place, the `0x7FFFFFFF` literal load is
  scheduled first.
- **Values that must arrive as inline parameters.** Some stores need
  their value to reach the store through an inline function's parameter:
  - `SetA`/`SetB` (`hasX = 1; valueX = v`): with `s32 v`, the value is
    materialized before the flag's address, as in the ROM.
  - `SetFlipX(t, 1)`: the bitfield store becomes the ROM's
    `and ~0x10; orr 0x10` pair instead of a bare `orr`.
  - `SetUnk68(t, 0)`: the zero is materialized before the `+0x68`
    address add.
- **Bitfields.** The target's `+0x28` is a packed byte of bitfields. It
  needs `u32 flipX:1`, because a `u8` field makes gcc test the bit with
  `and #0x10` and the ROM uses `lsl #27`. In `sub_8016048`, the
  `else if` re-tests the bit as `!(flipX & 1)`. That gives the second
  test a different RTL shape from the first, which keeps jump threading
  from merging the two tests (the ROM re-loads and re-tests). The
  `+0x0C` flags byte is also bitfields (`gone`, `flag6`, `flag7`). The
  ROM clears bits 6/7 with QImode masks (`0x7F`, `-0x41`).
- **Keys.** `struct keys { u16 held; u16 pressed; u8 pad[0]; }`. The
  zero-length array makes the struct BLKmode, so the local copy stays on
  the stack, where the ROM reads it with `ldrh [sp, #2]`. Two handlers
  (`sub_8016C94`, `sub_8017044`) read it through a `struct keys *` local,
  which holds the stack address in r5 as in the ROM.
- **Code order.** `ApplyLevel` tests `tag != 0x20 && != 0x1D && != 0x1F`
  so that the retag branch comes first. It restores the frame through a
  fresh `self->target` (`RestoreFrame`), which keeps the two tails in
  different registers. `PlayerCtrlHandleEvent`'s cases follow the ROM's body order,
  with an empty `case 13:` for the 13-entry jump table. `sub_8016DDC`'s
  timer switch lists `case 7` before `case 5`, and each case has its own
  `ClampFrame`, which cross-jumping merges. A shared clamp after the
  switch reached by `goto` left a fixup `USE` that blocked the merge.
- **`sub_8016AB0`** keeps two pins per half: the record index in r1 and
  the scaled offset in r0. Every unpinned form tried scales straight into
  r2.

Verified with a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: OK`).
