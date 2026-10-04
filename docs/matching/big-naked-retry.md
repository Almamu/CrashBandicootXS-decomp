# Big NAKED retry: `UpdateGameFrame` and `sub_803686C`

This pass took on two of the largest parked NAKED functions. One closed.

| Function | File | Size | Result |
|---|---|---|---|
| `UpdateGameFrame` (#34) | `src/system/game_loop55.c` | ~730 insns | matched, old_agbcc |
| `sub_803686C` (#65) | `src/graphics/graphics_loading_35d1c.c` | 1160 bytes | still NAKED, draft 329 halfwords off |

## `UpdateGameFrame` (never attempted before, now real C)

The first draft was written straight from the ROM disassembly, with the
jump table, the nested loops and the stack slots read off it. After that,
most of the work was loop shape:

- **The level loop and the attempt loop are real `for (;;)` loops.** In
  the ROM, both are entered with a `b` into the middle of the loop. That
  is gcc's `expand_end_loop` rotation: it moves everything from the loop
  top down to the last exit jump (within 30 insns of the first one) to
  the bottom and jumps to it. For the level loop, the moved part ends
  with `if (sub_8034CB0()) ResetLives(self); else break;`. The part
  after it (the special-level `switch`, the best-time record and the
  `snap14C` save) becomes the loop head at `0x0802268C`. The attempt
  loop's `if (A || B) {...} else if (!sub_802455C(...)) break;` is
  rotated the same way. With `goto` loops instead, nothing was hoisted
  (no loop notes) and the seven `&self->field` addresses were not held
  in `r7`/`sl`/stack slots.
- **The restore step is a `goto` loop.** As a `do`/`while` it got its
  own loop notes, and loop.c copied the hoisted `&self->level` into a
  second register. As a `goto` loop it uses `r7` directly, as the ROM
  does.
- **A `void **bitmap = &gEntityFlags;` local set right before the
  attempt loop** gives the ROM's `mov sb, r4`. Only the uses the ROM
  reaches through `sb` go through `bitmap` (the save/allocate in the
  check step and the clear in the "done" path). The two `sub_8025A44`
  blocks still name the global and load its address fresh, as the ROM
  does. This change also moved the status flag to `r8` and the zero to
  `r5`.
- **The best-time "unset" test reads the record as a halfword.** The ROM
  compares `t` against the 13-bit field (word load, `lsl 16; lsr 19`) but
  tests "unset" with `ldrh; and 0xfff8`. `union level_best_time` has
  the bitfield and a `u16 raw`. The test is `(raw & 0xfff8) == 0`.
  Writing the field test (`time == 0`) or `!(raw >> 3)` was 300+
  halfwords off, because the size changed and everything after it
  moved. The right test also fixed the stack-slot order (see below).
- Smaller order fixes:
  - `self->unk_1b8 = self->unk_1bc = 0` computes the 0x1b8 address
    first.
  - The `SetMaskLevel` argument is `arg = 2; if (tier <= 1) arg = tier;`.
    A `?:` gave a `min` shape.
  - `sub_8027138` returns a typed pointer. The store into the `void *`
    global then has a conversion, so `expand_assignment` computes the
    global's address before the call instead of after it.

**Stack-slot order comes from GCSE's hash table.** The five spilled
field addresses (`&self->0xac`..`0xe4`) are new pseudos from GCSE PRE.
They are numbered in hash-bucket order, and reload hands out stack slots
in pseudo order. The bucket count is `n_insns / 2`, so a draft with a
different insn count gets a different order (one draft had `&self->0xac`
in the last slot instead of the first). It fixed itself once the rest of
the function matched. If the slot order is the only thing left, look for
code differences elsewhere before blaming the slots.

`game_loop55.o` joined `OLD_AGBCC_OBJS`. Under agbcc the same C is 168
halfwords off. The file holds only this function.

## `sub_803686C` (still NAKED)

The draft under `#if NON_MATCHING` is structurally complete.
`triage_naked.py` reports 329 halfwords off under old_agbcc and 345
under agbcc (4 bytes short). Findings:

- **It needs strength reduction on.** Its header loop (4 OAM entries) is
  `adds r4, r3, #0; movs r5, #3` in the ROM: the hoisted `&oamA` comes
  before the reversed counter. Only check_dbra_loop emits it in that
  order. So the loop is written up-counting and built with strength
  reduction on, like `sub_80358A8`'s inner loop. Closing it therefore
  also means splitting `graphics_loading_35d1c.c` at `0x0803686C` into
  an address-keyed file without `-fno-strength-reduce`. A whole-file
  compile with strength reduction on differs only in `sub_8036600` (16
  halfwords), so everything from `sub_803686C` on would match in the new
  file. `sub_8036668` matches either way.
- The slot loop keeps `i` counting up when the flag address is written
  `flags = self + 0x410; flag = flags + i`. `self + 0x410 + i` gets
  strength-reduced into its own pointer, and then the loop is reversed.
- The two zero words need `vu16`/`vu32` locals. Otherwise they get stack
  slots after the OAM structs.
- `oamA.y = hdr->posB.q >> 16` keeps the ROM's `ldrsh`. With the `s16`
  half it narrows to `ldrh`/`ldrb`.
- **What is left is the row-copy loop.** The ROM's loop is:

  ```
  ip = 0x80000050; r3 = r4 + 0x60; sl = 0x800; r6 = 7
  loop: DMA(r1 -> r3); r1 += 0xa0; DMA(r1 -> r4 + sl); r1 += 0xa0
        r7 = 0x100; r4 += r7; r3 += r7; if (--r6 >= 0) goto loop
  ```

  After the loop it reuses `r7` for the `pa != 0x100` compare, which
  needs cse2 and the 0x100 still being inside the loop. So `buf + 0x60`
  is a reduced giv, `buf + 0x800` is not, and the 0x100 step is never
  hoisted. No source form reproduced this. I tried walkers, index forms,
  separate `dst` walkers, a re-read `dst`, `do`/`while`/`goto` loops,
  counter types, a shared `unit` variable, and pointer-bounded loops,
  under both strength-reduction settings (about 150 variants). Each
  either hoists the 0x100 or computes `buf + 0x60` inside the loop. In
  the loop-pass dumps (`-da`), loop.c moves the 0x100 load in pass 1 (life 1, 19
  insns, under the threshold). With index forms it reduces `+0x60` and
  `+0x800` separately. Giv combination does not apply, because both are
  DEST_REG givs.
- The register gap comes with it. The ROM has self in `sb` and `i` in
  `sl`, and the tail's slot copy in `r8` with the affine flag in `r7`.
  The draft has these swapped. An `asm("" : : "r"(self))` at the end
  gives 249 halfwords (right size) but moves self to `r8` instead. The
  `asm("" : "=r"(v) : "0"(K))` constant-init form on `i`, `matrix` and
  `affine` did not help.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.

*Later pass (#65 strength-reduction retry):* `sub_803686C` is matched; see
[sr65-naked-retry.md](sr65-naked-retry.md).
