# Issue #23: 0x080188D0-0x0801967C (25 functions)

All 25 functions of the former `asm/code_3_2_17_188d0.s` now live in
`src/graphics/actor_part_188d0.c` (file retired; `ldscript.txt` links
`actor_part_188d0.o` in its place). **23 are real C, 2 are NAKED**
transcriptions (C kept under `#if NON_MATCHING`), 0 left raw. Full clean
`make compare` passes; `make NON_MATCHING=1 report` builds with no warnings
for this file. Issue #23 stays open for the two NAKED functions.

## What the code is

The same C++-style object family as `actor_part_17524.c` and
`actor_part27*.c`: small classes with a method table ("vtable") at
`self+0xc`, gcc 2.x `{s16 this-adjust, pad, fn}` method entries called
through the `sub_803AD80`/`AD84`/`AD88` call-via-register trampolines.
Every class has a constructor (base constructor `sub_800B8C8`,
`sub_8017A8C` or `sub_801B7D8`, then its own table pointer, returns
`self`) and a destructor (table pointer, then the base destructor).

The objects drive a "part": an on-screen object built by `sub_8009ED0`
(`struct gfx_part` in the file): position at `+0`/`+4`, bitmap id `+8`,
flags byte `+0xc` (bit 0 gone, bit 2 hidden, bit 4 active), animation bank
`+0x20` (28-byte records, frame count at record `+0x16`), mirror bit
(`+0x28` bit 4), frame nibble (`+0x29` low 4 bits, from `sub_800815C`),
animation tag `+0x2d`, frame `+0x30`, "animation finished" `+0x38`,
controller `+0x44`.

- `sub_80188D0`/`sub_80188E8` (`gStaticData_087E4494`),
  `sub_8018948`/`sub_8018960` (`gStaticData_087E44FC`),
  `sub_80195EC`/`sub_80195D8` (`gStaticData_087E45CC`),
  `sub_801961C`/`sub_8019608` (`gStaticData_087E4634`),
  `sub_8019660`/`sub_801964C` (`gStaticData_087E469C`),
  `sub_80189EC`/`sub_80189C4` (`gStaticData_087E4564`, frees/allocates a
  257-entry `i*i >> 8` squares table at `+0x48`): constructor/destructor
  pairs. `sub_8018948` has no caller anywhere (no `bl`, no Thumb pointer in
  the ROM) - **UNUSED**, matched anyway.
- `sub_80188FC`: once the part's animation finishes, the inlined "mark
  gone" sequence (`sub_80072D8`'s flags bit 0 + `gUnknown_030012B4+0x108`
  bitmap bit).
- `sub_8018978`: mirror the part towards `self+0x30`, reset two counters
  to 26 and store the part's offset from `self+0x30/0x34`.
- `sub_8018A30` (table method) with `sub_8018BDC`/`sub_8018CB0`: a
  two-part effect. State 0 spawns two child parts (tags 3 and 15, the
  second offset by `+0x2000,-0x4000`), state 1 sets the first child's tag
  from the second's height, mirrors both towards it and derives both
  frames from the horizontal distance (`5 - min(5, |dx|*12 / width)`),
  state 2 waits three ticks, state 3 sinks everything until it passes
  the level bottom and then signals `sub_80241A4`.
- The "mover" (`sub_8018D70` spawner, `sub_8018E4C` per-frame update,
  `sub_8019094` state setter, `sub_8019214` hit-effect spawner):
  `sub_80196B8` (issue #24's range) stores a target and step count,
  `sub_8018E4C` interpolates the part towards it
  (`target - delta*remaining/steps`), and the state setter bounces it
  across the level; the level config's `+0x10` index picks the height
  pattern and the per-config timings in
  `gStaticData_0816C358`/`35C`/`35F`/`362`.
- `sub_8019324`: box-overlap hit test (`sub_8007CF8`/`sub_8007C30`/
  `sub_8007B98` boxes, `sub_8001688` overlap) of a part against the
  player (fires the player's `+0x68` method with code 9 unless it's busy)
  and against every entry of the `gUnknown_030012F0` list.
- `sub_8019464`, `sub_80194E0`: two more per-frame methods (frame reset
  and loop; a delayed re-skin followed by "mark gone").

## Matching notes

The recurring theme: this code always materializes a read-modify-write's
constant or mask *before* loading the byte it applies to, and computes a
stored value before its address - the shape an inlined C++ setter's
argument evaluation leaves. Plain C bitfield/`|=`/`&=` code loads first.
Fixes, all plain C plus pins/barriers unless noted:

- **Byte RMW helpers** (`OrFlags`, `AndFlags`, `SetFrameNibble`,
  `CopyFlipX`, the two-case flip in `sub_8018978`): the constant goes
  through an empty `asm("" : "+r"(c))` before the load. Bitfield clears
  must use the sign-extended QImode masks (`-5`, `-0x11`, `-0x10`), which
  is what a bitfield store produces. `SetFrameNibble` also needs the
  established `mov #0x10; neg` inline asm (as in
  `UPDATE_ICON_FRAME_NIBBLE`) since gcc otherwise folds `-0x10` out of
  the `0xf` it just used.
- **Spawner tag store**: the tag value is pinned to r0 and barriered
  before its address is formed. In `sub_8018CB0` the ROM also keeps the
  tag constant 15 in r5 across three calls and reuses it as the frame
  nibble mask - reproduced with an r5 pin (`SetFrameNibbleM`).
- **Param-order pinning so `self` lands in r7**: `sub_8019214` and
  `sub_8018E4C` keep `self` in r7, which can never be pinned. Pinning the
  *other* parameters (`part` to r6, `kind`/`n` to r5) leaves r7 as the
  allocator's natural choice for `self`, with the parameter copies in the
  ROM's order.
- **Mark-gone bitmap**: `MARK_GONE` is the `sub_80178EC` sequence
  (volatile id re-read, signed `/ 32`) with the per-site registers as
  macro parameters; `sub_8019324` (part in r8) additionally holds `0x108`
  in r4.
- **Frame clamp** (`SET_FRAME_R`): the ROM loads the record table before
  the tag byte and puts the tag in a callee-saved register (r4/r5/r6)
  distinct from its address register; pinned per site.
- **`sub_8019094`'s inner switch**: the ROM loads the config index into
  r0, copies it to r1, runs three compares on r0 and the `== 2` one on r1.
  No `switch` shape (local, inline-function parameter, pins) reproduced
  the copy, so the compare tree is written with `goto`s and the copy is a
  one-instruction `asm("mov %0, %1")`.
- **Zero `ldrsh` index**: gcc's reload picks r2 for the zero index of the
  method-table `ldrsh`; for one call in `sub_80194E0` (r3) and one in
  `sub_8018E4C` (r4) the ROM picked another register, so that single load
  is a narrow `asm("ldrsh %0, [%1, %2]")` with the index pinned.
- **Hoisted zero**: `sub_8019094` case 1 keeps a 0 in r4 across two calls
  (loaded between the `unk_2C` store's address and the store itself) -
  reproduced with an r4 pin.
- Smaller ones: `x += 0x2000; y -= 0x4000` as separate statements
  (`sub_8018CB0`); `c->pos = part->pos` struct copy for the ldr/ldr/str/str
  order; `self->squares[i]` indexing instead of a walking pointer
  (`sub_80189EC`); an explicit empty `case 10` to keep `sub_8018E4C`'s
  11-entry jump table; `if (stepsLeft) break; goto next;` for its
  branch-trampoline shape; the `sub_803AD88` call's function pointer
  loaded into r4 through a volatile read (the `actor_part78.c` idiom);
  `part` pinned to `ip` in `sub_8018978`.

## NAKED (C kept under NON_MATCHING)

- **`sub_8018A30`**: every operation and the 5-entry jump table are
  reproduced, but the ROM keeps `self`/`part` in r5/r6 and uses r7 as a
  short-lived scratch register three times (the flip byte, the first
  frame clamp's tag, the `0x4000` constant). Unpinned, this compiler puts
  `self`/`part` in r6/r7; pinning them to r5/r6 makes it spill to r8
  rather than ever using r7 for scratch, and r7 itself can't be pinned
  (agbcc's push/pop bug).
- **`sub_801961C`**: `sub_801B7D8`'s fifth argument is a one-byte value
  passed by value - the caller `strb`s it into the outgoing stack slot and
  the callee reads it back through the slot's address. A `u8` parameter
  is promoted to a word `str`; a packed one-byte struct/union gives the
  `strb`, but the 0 is always materialized before the `mov r1, sp` slot
  address, the reverse of the ROM (tried: local, initializer, compound
  literal, cast-to-union, byte array, inline constructor).
