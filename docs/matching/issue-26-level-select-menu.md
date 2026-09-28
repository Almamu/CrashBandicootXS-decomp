# Issue #26: 0x0801B85C-0x0801CEE0 (level-select screen and two small classes)

All 25 functions of the former `asm/code_3_2_17_188d0_1b85c.s` now live in
`src/graphics/actor_part_1b85c.c` (the `.s` file is retired). **22 are
plain C; 3 (`sub_801BC28`, `sub_801C608`, `sub_801C96C`) are NAKED
transcriptions** with their C reconstructions kept under
`#if NON_MATCHING`. Verified with a full clean
`rm -rf build && make NON_MATCHING=1 report` and
`rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`
(`crashbandicootxs.gba: OK`).

**Update (old_agbcc retry, docs/matching/old-agbcc-retry.md):** the
object is now built with `tools/agbcc/bin/old_agbcc` (Makefile
`OLD_AGBCC_OBJS`), the compiler this region was originally built with.
All 22 plain-C functions match under it; `sub_801B984` needed its zero/id
pins dropped to do so, and the pins/barriers of `sub_801B8BC`,
`sub_801B984`, `sub_801C104`, `sub_801C364` and `sub_801C51C` turned out
to be unnecessary and were removed. The three NAKED functions still don't
match under old_agbcc (see "Parked" below) and are unchanged.

## What the code is

Three gcc 2.x C++ classes (method tables, virtual calls through the
`sub_803AD7C`/`AD80`/`AD88` call-via-register thunks, inlined member
functions):

- **`struct follow_child`** (`sub_801B85C`-`sub_801B980`, method table
  `gStaticData_087E4ABC`, 0x80 bytes) - the child object `sub_8017600`
  (`actor_part_17524.c`) spawns. It registers itself as
  `gUnknown_030012D4`'s follow target (`+0x10`) and each frame
  (`sub_801B8BC`, table slot `+0x18`) eases a Q8 x offset from the
  player (`+0x7C`) toward a target (`+0x78`, clamped to 0xA00-0x3200 by
  `sub_801B960`) by 0x200, then copies the player's position. The
  destructor (`sub_801B91C`, slot `+0x50`) hands the follow target back
  to the player.
- **A 0x78-byte sprite subclass** (`sub_801B984`-`sub_801BAD0`, method
  table `gStaticData_087E4B34`) - a factory that inlines the constructor
  and starts animation 0 of `**gUnknown_030012D0 + 0x150`, and a slot
  `+0x70` handler (`sub_801BA60`) that fires the player's method 13 with
  `(0, 0x19, 0)` when the player (flags bit 7 set) overlaps its hit box.
- **`struct level_menu`** (`sub_801BAF0`-`sub_801CE60`, 0xAC bytes) - the
  paged level-select screen docs/rom_map.md found from the other side
  ("a paged menu/screen with a smooth horizontal page-turn animation").
  `sub_801BAF0` (called from `game_loop55.c`) is the modal entry point:
  display/icon-manager setup (the same sequence as `sub_80062A8`),
  construct (`sub_801BC28`), run (`sub_801C96C`), return the selected
  level through `*arg`. Five entries per page (`arg / 5`, `arg % 5`;
  `arg >= 20` is the last page). The loop dispatches the newly-pressed
  keys: Up/Down page turns (`sub_801D548`/`sub_801D4C4`), Left/Right
  cursor moves (`sub_801CDE0`/`sub_801CE60`, repeating while held),
  Start exits (`sub_801D300`), A on an open entry selects
  (`sub_801D110`). Each level's fixed record is `gStaticData_0816C86C`
  (36 bytes: name text, three centisecond thresholds); its saved record
  is a word in `sub_80236EC`'s save block (`cleared`/two more flags/a
  13-bit best time). The object keeps shadow copies of BLDCNT/BLDALPHA
  (`+0xA0`), BLDY (`+0xA4`) and DISPCNT (`+0xA8`), modelled as bitfield
  unions, and commits them with the scroll registers every frame - the
  commit docs/rom_map.md described for `sub_801CCF8` is an inlined
  helper (`CommitDisplay`), also inlined three times into
  `sub_801C96C`.

UNUSED (no `bl`/`.4byte` reference in `asm/`, `data/` or `src/`, and no
Thumb pointer anywhere in the ROM): `sub_801B85C`, `sub_801B960`,
`sub_801B980`, `sub_801BAD0`. Matched anyway. `sub_801B8BC`,
`sub_801B91C`, `sub_801BA60` and `sub_801BAB0` are reached only through
their method tables.

## Techniques

- **`Opaque(c)` - constant-first operand order without inline asm.** This
  ROM builds the constant operand of `&`/`|` before loading the memory
  operand (`movs r1,#0x41; negs r1,r1; ldrb r2,[r0,#0xc]; ands r1,r2`)
  almost everywhere; agbcc loads first. The cause is gcc's tree folder,
  which moves an `INTEGER_CST` operand of a commutative operator to the
  second position before expansion (the ROM was compiled by the C++
  front end, which doesn't). Wrapping the constant in a trivial
  `static inline s32 Opaque(s32 v) { return v; }` hides it from the
  folder - inlining only happens at RTL level - so `x &= Opaque(~0x40)`
  expands with the constant first. Used for `sub_801BAC4` (now one
  line) and for the key/flag tests in the NON_MATCHING reconstructions;
  it closes the ordering in isolation but doesn't by itself fix register
  choice (`sub_801B984`'s masks still need the pinned/`mov`+`neg` form,
  since `Opaque` there shifts the allocation).
- **Inline member helpers** reproduce the ROM recomputing field addresses
  after every call (the `static inline` anti-CSE technique from
  docs/matching/issue-59-60-static-inline-cse-promotion.md) and
  evaluating an argument before the rest of a call: `IconSetup`/
  `IconReserve`/`LoadMenuPalette` (`sub_801BAF0`), `SetIconPos`
  (`sub_801C104`, `sub_801C3E8` - it also puts `posY`'s constant ahead of
  the stores), `AnimTable`/`SetAnim` (the inlined `sub_80087D0`), and
  `CommitDisplay`. `sub_801BAF0` also needed the global's address taken
  first (`struct level_menu **menuAddr = &gUnknown_03000820;`, the same
  idiom as `settings_menu8b.c`'s `sub_800300C`) and was previously the
  kind of function this project would have NAKED'd (its sibling
  `sub_80062A8` is).
- **Operand order of an indexed address.** `&recs[idx]` with a 28-byte
  stride and `&items[i]` with a 4-byte stride come out with opposite
  `adds` operand orders; where the ROM wanted the other one, the index
  is written as `idx * sizeof + (u32)base` (`sub_801C2B0`) or through a
  pinned offset (`ItemAt`, `sub_801C51C`).
- **Keys.** `gUnknown_030007E0` is declared as a union of the whole word
  and a `{held, pressed}` halfword pair (as in `settings_menu8b.c`) so
  `pressed` reads as `ldrh [base, #2]` instead of a folded `sym+2`
  literal.
- **`asm volatile("" : "+r"(self))`** after a call stops gcc hoisting
  `self + 0x24` above it (`sub_801C51C`); a non-volatile barrier or one
  on the derived pointer does not.
- The rest is the usual register pinning (`sub_801B864`, `sub_801B8BC`,
  `sub_801B984`, `sub_801C104`, `sub_801C2B0`, `sub_801C364`). No pins
  on `r7`.

## Parked (NAKED + NON_MATCHING C)

- **`sub_801BC28`** (constructor, 1052 bytes). With `AnimTable`/`SetAnim`
  and explicit blend/DISPCNT sequences the reconstruction has the ROM's
  instruction stream and `self` in `r7`, but the ROM keeps `0`/`1`/`2`/
  `0x10` and `&gUnknown_030012D0` live in `sb`/`r3`/`r8`/`r5`/`sl`
  across dozens of calls and picks a different scratch register at
  almost every store; matching it would mean pinning most of the
  function (including `r8`, which this compiler mishandles).
- **`sub_801C608`** (record loader, 868 bytes). The reconstruction
  differs only in which low register reload picks for each
  `mov rX, r8`/`mov rX, sl` copy before a store, and in the stack slots
  of the cached `self+0x90`/`+0x94` addresses - at nearly every
  statement.
- **`sub_801C96C`** (menu loop, 908 bytes). Off in three places, all
  register choice: in each of the three inlined `CommitDisplay`s the ROM
  loads the BLDY byte into the dying address register
  (`ldrb r4,[r4]` / `ldrb r3,[r7]` / `ldrb r7,[r7]`) where agbcc ties it
  to the shift's output (`r0`) - every phrasing tried (u8/u32 bitfield
  views, a u8 local, an inline getter) either kept `r0` or moved the
  load ahead of the register address, and the last site would need an
  `r7` pin; the ROM also copies the key word once (`adds r1, r2, #0`)
  before the third direction test; and one `mov r2, sb` vs `mov r3, sb`
  at the end. The fade-in `evy--` only matches with a u8 bitfield view
  of BLDY, which in turn breaks `sub_801CCF8`'s commit; the
  reconstruction uses the u32 view.

### Under old_agbcc

The NON_MATCHING C (unchanged in the tree) was recompiled with old_agbcc;
none of the three matches, but two got much closer with small changes
(recorded here, not applied):

- **`sub_801C96C`**: the three `CommitDisplay` BLDY-register problems
  disappear under old_agbcc. Two differences remain: the fade-in
  (`evy--`) and the key-word copy. Reading/decrementing `evy` through a
  packed `struct { u8 evy:5; u8 rest:3; }` view of `self->bldy` fixes the
  fade (908 bytes, same as the ROM). The ROM's `adds r1, r2, #0` key copy
  (made before the 0x80 test, used only by the 0x20 test) gets eaten by
  CSE for every plain `union key_state k = keys` placement. With an
  `asm("" : "+r"(k.all))` barrier it survives, but it lands either in the
  wrong block (after the 0x80 branch, 10 bytes off) or in the right block
  with `keys`/`k` in r3/r2 instead of r2/r1 (12 bytes off).
- **`sub_801C608`**: taking `SetAnim`'s index as `s32` instead of `u8`
  fixes the rank-icon load (the ROM loads the table word, then `strb`s
  it; with a `u8` parameter old_agbcc narrows the load to `ldrb`).
  Testing the saved time with `*(u16 *)sv & 0xFFF8` gives the ROM's
  `ldrh`/mask test, and the ROM's compares are unsigned (`bhi`). What is
  left: the ROM's frame is 12 bytes (it spills the `info` pointer to
  `[sp]` besides the `unk_90`/`unk_94` addresses; this C has an 8-byte
  frame), and the ROM keeps `time << 16` live and re-shifts `>> 19` for
  each compare, reusing the loaded threshold registers as the next
  `FormatCentiseconds` argument.
- **`sub_801BC28`**: with `SetAnim(s32)` the long middle of the function
  (the sprite setup) matches instruction for instruction. It is still
  off in the opening blend/BLDY/DISPCNT blocks (the ROM keeps `0`/`1`/`2`/
  `0x10` in sb/r3/r8/r5 and addresses DISPCNT's second byte through its
  own `self + 0xA9` register), in the six-item loop's counter/pointer
  registers (r4/r5 swapped), and in hoisting the sprites' `0x80` constant
  into r8 ahead of the eight-sprite loop.

## Struct notes

- `struct level_menu` (0xAC) is fully laid out; `unk_80`-`unk_94` are the
  record panel's slide offset and per-row y offsets (0 or 0x1C),
  `rank` is the first of `sub_802336C`/`sub_8023360`/`sub_8023354`/
  `sub_8023348`/`sub_802333C` that holds for the level (5 = none), and
  byte 2 of the save block holds four more flags tested per rank.
- `struct sprite` is the 0x40-byte part `sub_8008904` constructs
  (`+0x20` animation table, `+0x29` palette nibble, `+0x2D` animation
  index, `+0x30` frame, `+0x3C`); `struct anim_record` is 28 bytes with
  the tile-cache record id at `+0x14` and the frame count at `+0x16`.
- `struct item` (0x14, `sub_801DFEC`) has its method table at `+0x10`:
  `+0x08` update, `+0x20` draw, `+0x28` destructor.

## Later pass (issue #12/#24/#26 NAKED retry)

Under old_agbcc the `Opaque`/pinned shadow-register code is unnecessary:
plain bitfield stores (`self->blend.bits.effect = 3; ...`) chain the
`orr`s exactly like the ROM. `sub_801BC28` is real C (plus a separate
counter for the six-entry loop and the sprite loop's 0x80 in a variable
set with the counter). `struct level_save` became u16 bitfields,
`struct level_info`'s thresholds u32, and `SetAnim` takes an int index;
with these `sub_801C608` and `sub_801C96C` are each one allocation
detail away. See [issue-24-26-12-naked-retry.md](issue-24-26-12-naked-retry.md).
