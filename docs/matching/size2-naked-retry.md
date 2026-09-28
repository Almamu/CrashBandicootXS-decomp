# NAKED retry (size2): 2 of 3 closed

This pass took three drafts whose size was nearly right:
`sub_801AB98` (#25), `sub_800FF0C` (#13) and `sub_80352AC` (#64).

| Function | File | Start | Result |
|---|---|---|---|
| `sub_80352AC` (#64) | `src/graphics/actor_part131.c` (already old_agbcc) | 114 hw, 420 vs 416 B | **Closed**, real C, old_agbcc |
| `sub_800FF0C` (#13) | `src/system/game_loop36.c` | 471 hw, 1388 vs 1396 B | **Closed**, real C, old_agbcc (`game_loop36.o` joined `OLD_AGBCC_OBJS`; it is the only function in the file) |
| `sub_801AB98` (#25) | `src/graphics/actor_part_1ab98.c` | 565 hw, 1640 vs 1648 B | Not closed, draft unchanged, note updated |

## sub_80352AC: GCSE hashes non-volatile asm

GCSE's PRE moved `slot << 5` up to the y loop's pre-test and spilled
it. The ROM computes it at the palette copy. The `-dG` dump of the
earlier `"+r"` copy attempt showed why that attempt failed. The hoisted
expression was the asm itself:

```
Index 56: (asm_operands ("") ("=r") 0 [(reg/v:SI 26)] ...)
PRE/HOIST: end of bb 7, insn 407, copying expression 56 to reg 119
```

A non-volatile asm with outputs is an ordinary expression to GCSE, so
PRE hoists it like any other. `hash_scan_set` skips volatile asms. The
palette index is now a copy `ps` of `slot` passed through
`asm volatile("" : "+r"(ps))`:

- `slot` itself is untouched, so PRE still hoists `slot + 1` and
  `i + 1` as in the ROM.
- The size and frame are right, and the draft was left 2 halfwords
  off.
- `ps` is an r1 register variable, because the ROM reloads `slot`
  into r1.
- `sh = ps << 5` is followed by `asm("" : : "r"(ps))`. This keeps `ps`
  live, so the shift result goes to r0 instead of reusing r1. The
  result is spelled `sh + (u32)palSlots`, which gives the ROM's
  `add r1, r0, r3` operand order.

**Technique worth reusing:** when PRE/GCSE hoists or merges an
`asm("" : "+r")` escape, use `asm volatile`.

## sub_800FF0C: the pointer copies come from an inline's return value

The file was not on `OLD_AGBCC_OBJS`, but the draft only converges
under old_agbcc. Under agbcc the final C is 1404 bytes and about 400
halfwords off. `game_loop36.c` holds only this function, so the whole
object moved to old_agbcc.

In order:

1. **0xb pre-check copy.** The record is read as
   `u8 *rec = Placement(slot);`, where `Placement` is a `static inline`
   that returns `PLACEMENT(i)`. The inline's return value is copied into
   `rec`, which gives the ROM's `add r2,r0,#0`. Re-reading the macro
   instead makes no difference, because cse merges the chains.
   273 hw, 1392 B.
2. **`gUnknown_0300130C`.** The draft passed `*(void **)gUnknown_0300130C`,
   one dereference too many. The ROM passes the variable's value.
3. **Case 15.**
   - `self->u48.n &= 0x3f; self->u48.n &= 0xf8;` keeps both ands.
   - The anim record goes through a `struct anim_rec *ar` local, so its
     address is computed before `gUnknown_030012B8` is loaded.
   - The table index is a `u32 idx` local, so the index is computed
     before the table address is loaded.
4. **Case 11.** Tag 0 comes from the constant-init asm
   (`asm("" : "=r"(zero) : "0"(0))`), so the `movs r0,#0` comes
   before the tag address.
5. **`flagged` block and case 15 copies.** Both use `Placement(slot)`
   as well, and no escapes are needed. The draft got here by way of
   `"+r"` copies. Those fixed the size but always put the addition in
   the longer-lived register. A brute force over case 15's forms
   (macro/inline, escape target, which pointer each use reads,
   extra-reference position; 282 variants) found that the plain
   inline matches.

The two extra `id` references and the brace variants tried on the way
were not needed and were removed. The three `type` references from the
previous pass remain.

## sub_801AB98: not closed

The draft is 565 halfwords off. Most of that is one swap: the ROM
keeps `self` in sb and `result` in r8, and the draft does the reverse.

- Pinning `self` to r9 gives exactly 1648 bytes. It also adds two
  `self->type` reloads (464 hw, and structurally further away).
- A single `sub_800FDC8` call with `c`/`d` argument locals, for the
  `add r2,r5` / `add r5,r2` copies, is worse (702 hw).
- Two calls with a `c` local give 500 hw and 1628 B.
- Inline accessors for `box->padY` and `b.x` (the pattern that closed
  `sub_800FF0C`) change nothing (565/568 hw).

This one needs the global register roles fixed first. The draft is
unchanged, and its note records these results.

## Tools

The helpers are in the session scratchpad's `size2/` and are not
committed:

- `mv.py` runs multi-edit variants: lists of `(old, new)` pairs, or a
  callable.
- `dump.py` prints one function's compiled `.s` for a variant.
- `tab.py` lists specific instructions across variants.
- `d.py` and `calls.py` are copies of the `hold/` and `gap4/` ones.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
