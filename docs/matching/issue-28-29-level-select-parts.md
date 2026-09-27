# Issues #28 and #29: 0x0801DA38-0x0801E578, the level-select screen's sub-objects

All 41 functions of the former `asm/code_3_2_17_188d0_1da38.s` (issue #28,
25 functions) and `asm/code_3_2_17_188d0_1dfec.s` (issue #29, 16
functions) are now **real C**, with no NAKED or NON_MATCHING code:

- `src/graphics/actor_part_1da38.c`: `sub_801DA38`-`sub_801DF98`
- `src/graphics/actor_part_1dfec.c`: `sub_801DFEC`-`sub_801E524`
- `include/level_select_parts.h`: the structs, macros and inlines the two
  files share

Both files are on the Makefile's `OLD_AGBCC_OBJS` list (they are built
with `tools/agbcc/bin/old_agbcc`). Compiled with the current `agbcc`, the finished C
matches only 17/25 and 12/16 functions; with `old_agbcc` all of them match. The
compiler boundary is not inside this range: the code right after it
(`graphics_package_1e578.c`, issue #30) still builds with `agbcc`.

Verified with a clean `make NON_MATCHING=1 report` (no warnings from these
files) and a clean `make compare` (`crashbandicootxs.gba: OK`).

## What the code is

These are the three sub-objects of `struct level_menu`, the level-select
screen in `actor_part_1b85c.c` (issue #26):

- **`struct zoom_bg`** (`level_menu.bg2`, 0x8C bytes, constructor
  `sub_801D828` in the issue #27 range): the selected level's picture on
  the affine BG2 layer. `sub_801DAD8` runs this state machine:
  - 1: zoom out, then go to 2.
  - 2: load the requested picture (palette through a stack buffer and
    DMA3, 8bpp tiles to the BG2 char base), then go to 0.
  - 0: zoom in, then go to 3.
  - 3: shown. The picture wobbles on the sine table `gStaticData_0816A820`.
  - 4: zoom away while rotating, then go to 5 (gone).

  `sub_801DC28` builds the BgAffineSet source (`sub_803A944`) and
  `sub_801DCBC` commits the result to BG2PA-BG2Y. `sub_801DCF8`-
  `sub_801DD38` are the state queries. `sub_801DD48`, `sub_801DD5C` and
  `sub_801DD80` start the exit, start a page turn and request a picture.
  Four "twinkle" sprites (`+0x5C`, 12 bytes each) show a random frame for
  a random time. They move with the wobble during their blink window
  (`sub_801DD90`, `sub_801DDB4`, `sub_801DE04`).
- **`struct level_item`** (0x14 bytes, `level_menu.items[]`): one entry
  on the page. Its constructor is `sub_801DFEC`, its method table is
  `gStaticData_087E4BAC`, and its methods are:
  - +0x08 `sub_801DE30`: bob
  - +0x10 `sub_801DEA4`: set world/index (indices 0-4 are levels; later
    indices are the world's extra entry)
  - +0x18 `sub_801DF70`: position
  - +0x20 `nullsub_20`
  - +0x28 `sub_801DF98`: destructor

  It also has plain accessors (`sub_801DE28`, `sub_801DE2C`,
  `sub_801DEA0`) and `sub_801DF0C`, which sets the box animation.
- **`struct cursor_panel`** (`level_menu.panel`, 0x54 bytes): the
  cursor. Its first member is a `struct bresenham_line`
  (`include/line_util.h`), and `sub_801E43C` takes two steps along it per
  frame toward the target `sub_801E480` sets. `sub_801E480` also derives
  the zoom speed from half the major-axis distance. `sub_801E190` runs an
  idle animation cycle (`gStaticData_0816C634`) at random intervals
  (`sub_801E504`), plus the grow-in and shrink-away states 4 and 5. While
  growing or shrinking, `sub_801E2BC` draws the cursor itself as an affine
  OBJ. It takes the next matrix slot from the OAM shadow buffer
  (`gUnknown_03001300->field_08`), writes the ObjAffineSet result
  (`sub_801E3A4`) into the four entries' affine words, and queues the
  panel's own OAM attributes (`+0x34`) with `sub_8006AC8`.

UNUSED: `sub_801E3D4`, `sub_801E3E4` and `sub_801E4E4` have no
`bl`/`.4byte` reference in `asm/`, `data/` or `src/`, and no Thumb pointer
anywhere in the ROM. `sub_801E408` contains an inlined copy of
`sub_801E4E4`. All three are matched anyway.

## Matching notes

- **`__divsi3` is `sub_803ADB4`.** `0x10000 / self->scale` (`sub_801DC28`,
  `sub_801E3A4`) and `0xF8 / d` (`sub_801E480`) load the divisor before
  the constant. A direct `sub_803ADB4(0x10000, scale)` call does the
  reverse. The header aliases the libcall with
  `asm(".set __divsi3, sub_803ADB4")`, which works like the
  `_call_via_rN` aliases from issue #24.
- **Sine lookup: `(tbl[i] * 4) >> 8`, not `tbl[i] >> 6`.** Both compute
  the same value. With `>> 6`, gcc's combiner rewrites the `s16` load and
  shift as `ldrh; lsl #16; asr #22`, but the ROM has `ldrsh; asr #6`. The
  multiply-then-shift form (amplitude 4, Q8) keeps the `ldrsh`.
- **`(u16)sub_8000E1C(n)` with an `s32` return.** Declaring the RNG as
  returning `u16` lets CSE merge the zero-extension into a later `>> 1`
  (`lsrs r0, r1, #17`). With `s32` plus a cast at each call, and
  `t->timer / 2`, the ROM's `lsrs r0, r0, #1` survives. gcc knows the
  value is non-negative, so the division is a plain shift.
- **Frame clamp and record access use a macro, not an inline.**
  `PART_RECORD(p)` expands to `p->anim->records[p->animIndex]`. An inline
  returning `&records[idx]` loads `animIndex` before `records`, which is
  the reverse of the ROM. `SetFrame`/`SetAnim` stay inlines. `SetAnim`
  takes an `s32`, because a `u8` parameter narrows the ROM's word table
  loads to `ldrb`.
- **Range `case`s** reproduce the ROM's two-compare range tests:
  `case 0 ... 3:`/`case 4 ... 5:` in `sub_801DC28`, `case 1 ... 2:` in
  `sub_801DD80`, and `case 4 ... 5:` with a `default` in `sub_801E2BC`.
  An `if (s >= 1 && s <= 2)` gives `subs; cmp; bhi`.
- **Packed bitfield unions.** `union bgcnt` (BG2CNT at `+0x34`) needs
  `__attribute__((packed))`. Without it, this ABI rounds the union to 4
  bytes and shifts every later field. The OAM attributes (`struct
  oam_attrs`) use `u32` bitfields, so gcc picks the ROM's byte or
  halfword access per field. The matrix number is three fields (3 + 1 + 1
  bits), because the ROM inserts it as `idx & 7`, `(idx >> 3) & 1` and
  `(idx >> 4) & 1`. Assigning `idx` directly to the 3-bit field (rather
  than `idx & 7`) gives the ROM's operand order.
- **Value before address: an inline setter.** `sub_801E2BC` loads each
  matrix word before it computes the destination address. A plain
  `buf->entries[n].affineParam = m[k]` computes the address first.
  `SetAffineParam(gUnknown_03001300, idx * 4 + k, self->matrix[k])`
  evaluates the inline arguments right to left, which gives exactly the
  ROM's order. That includes a single load of the buffer pointer and
  `idx * 4` kept in a register. The value parameter must be `u16`: with
  `s16`, the loads become `ldrsh`.
- **Statement shape for register choice.**
  - `sub_801DF70`: an inline `SetPosQ8(part, x, y - 3)` loads both
    coordinates before shifting, as the ROM does.
  - `sub_801DE30`: the frame is `if (selected) SetFrame(f, 1); else
    SetFrame(f, 0);`. The two tails cross-jump into the ROM's shared
    clamp. `selected ? 1 : 0` gives a branchless `neg/orr/lsr`.
  - `sub_801DFEC`: re-reading `self->frame`/`self->icon` after each
    store keeps the part in r0. A local copies it to r1.
  - `sub_801E190`: using `self->part` directly in every state gives the
    ROM's `self` in r4 and part in r5. A shared `p` local swaps them.
  - `sub_801E480`: `(x1 - x0) / 2` goes inside each branch of the
    major-axis `if`.
