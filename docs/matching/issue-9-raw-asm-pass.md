# Issue #9/#10 raw-asm pass

This pass covered the last three functions in the issue #9 range that were
still linked from `asm/*.s`, the two NAKED oscillators from issue #10, and
two NAKED holdouts from #9. It removes `asm/code_3_2.s`,
`asm/code_3_2_16_a884.s` and `asm/code_3_2_16_ac2c.s`.

## Closed (3)

| Function | File | Compiler | Technique |
|---|---|---|---|
| `sub_800AC2C` | `src/graphics/actor_part111.c` | old_agbcc | New plain-`switch` C, see below |
| `sub_800C940` | `src/graphics/actor_part116.c` | either | Empty asm clobber of r5, plus pins |
| `sub_800C97C` | `src/graphics/actor_part116.c` | either | Empty asm clobber of r8, one pin |

### sub_800AC2C (formerly raw)

This is the 38-case event dispatcher before `sub_800AFF4`. It had been
left raw under the old "big dispatcher" policy. Written as a plain
`switch` with the cases in the ROM's block order, it was 122 halfwords off
under old_agbcc on the first compile, and three changes closed it:

- For the hit cases, the mode-0 branch goes in the `else`. The ROM lays it
  out after the mode-1/2 body and reaches it with `beq`.
- The ROM reloads `gUnknown_030012C0->mode` after the listener call and
  never uses it. Only a volatile read reproduces that load (commented in
  the source). With it in place, `&gUnknown_030012C0` lands in `sb`, as
  in the ROM.
- The star-burst position (`child->x >> 8`, `child->y >> 8`, mirror bit)
  goes through locals, so the `gUnknown_030012E4` pool load comes after
  them.

It matches only under old_agbcc (agbcc is 218 halfwords off), so
`actor_part111.o` is now on `OLD_AGBCC_OBJS`. The file's other function,
`sub_800AFF4`, is NAKED, so the switch does not affect it. Before the
switch, `movs #1; ldrb; orrs` in the ROM (constant before the byte) had
already pointed to old_agbcc.

### sub_800C940 / sub_800C97C

The note on these functions said the ROM pushes a callee-saved register
that it never uses (r5 in C940, r8 in C97C). The brief suggested
`-fprologue-bugfix`, so all four combinations were compiled: agbcc and
old_agbcc, each with and without the flag. All four produce identical
code, so the flag plays no part.

What reproduces the push is an empty `asm("" : : : "r5")` (or `"r8"`). It
marks the register live, so the prologue saves it, and it emits no code.
The remaining register choices:

- C940: `target` pinned to r3, and the `-0x100` bias created in r6 with
  the constant-init asm form (brief item 10), which also keeps it from
  being loaded early.
- C97C: `table` pinned to r6, which puts `target` in r5.

Each empty asm has a comment in the source.

## Moved to C as NAKED + draft (not matched)

- **`sub_8007634`** is in the new `src/graphics/graphics_7634.c`, which
  replaces `asm/code_3_2.o` in `ldscript.txt`. It is the affine sibling of
  `sub_80073DC`. The draft follows the ROM block for block, with bitfield
  OAM words and the affine-matrix slot allocation. It is 468 halfwords off
  (1032 bytes under agbcc and 1024 under old_agbcc, against the ROM's
  1044). The ROM spills nearly every local into a 0x48-byte frame, the
  same obstacle that parks `sub_80073DC`.
- **`sub_800A884`**'s NAKED body moved into `src/graphics/actor_part78.c`,
  and `asm/code_3_2_16_a884.s` is gone. The old draft used register pins
  and two asm islands and was 137 halfwords off under both compilers,
  not the near match its comment described. It was replaced with plain C:
  real virtual calls through `self+0x18`, `sub_80084C4` inlined, and empty
  cases so the switch is built as a jump table. The new draft is 127
  halfwords off under old_agbcc, which this function needs (`movs #0x40`
  and `movs #8` come before their `ldrb`). What's left: the ROM builds the
  `+0x100`/`+0x102`/`+0x103` offsets by walking one register
  (`adds r1, #3`, `subs r2, #3`), while the draft gives each constant its
  own register. As a result gcc cross-jumps the kind-5 tail into the
  kind-7/10 tail, which the ROM keeps separate. A greedy search over
  statement orders, pointer locals and zero-constant forms (`raw9/g884.py`)
  gained only 3 halfwords.

## Tried, not converged (left as they were)

- **`sub_800AFF4`**: the draft is now built under old_agbcc along with its
  file. It is 256 halfwords off (624 bytes against 636); `self` is in r6
  and the frame is 8 bytes where the ROM uses 4. It was not pursued past
  the triage.
- **`sub_80091D4`**: 215 halfwords off under both compilers. Adding
  extra-reference nudges (`asm("" : : "r"(manager/node))`) at the loop
  head, the loop tail and after the outer loop brings the best case to
  210. `manager` and `node` still land in r8/sb instead of r7/r8. Brief
  item 9 (loop shape) would need the grid walk restructured, which this
  pass didn't have time for.
