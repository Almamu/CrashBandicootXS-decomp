# Late NAKED retry 3

This pass retried six drafts outside the #4/#9 zones: `sub_8014B54`
(#17), `sub_800BD48` (#10), `sub_80352AC` (#64), `sub_803686C` (#65),
and the GAX functions `sub_8038538` (#67) and `sub_8039B44` (#68). Two
closed, both with one new technique. Issues #10 and #17 have no NAKED
functions left.

## Closed (2)

| Function | File | Compiler | What it took |
|---|---|---|---|
| `sub_8014B54` | `src/graphics/actor_part_14674.c` (already old_agbcc) | old_agbcc | Was 3 halfwords off: the 0x600 reload was in r2 where the ROM has r3. An `r2` register variable `hold`, set by `asm("" : "=r"(hold))` before the add and used by `asm("" : : "r"(hold))` after it, keeps r2 live across the add. No code is emitted. Reload then spills r3 for the constant, and the later reloads rotate as in the ROM. |
| `sub_800BD48` | `src/graphics/actor_part112.c` (already old_agbcc) | old_agbcc | Was 21 halfwords off, nearly all reload registers. The same `hold` in r2 across the first MarkGone's id compare (`MarkGoneHeld`) left 5 halfwords. That was the layer's `1` in states 1/21/22, loaded after the `-4` mask instead of before it. Storing the layer from an `s32 one = 1` local fixes the order. On its own, that local was CSE'd into the bitmap's `1 << n` (4 bytes over). `MarkGoneFreshBit` computes the shift count first and builds its bit with the constant-init asm, so it gets its own `movs #1`. |

## The technique: holding a hard register live

In agbcc's reload (`reload1.c`, the `insn_chain` version), each insn
that needs a reload register picks one from `potential_reload_regs`.
That list puts call-clobbered registers with no live pseudo first, in
register-number order. A register that is live as a *hard* register at
that insn goes into `bad_spill_regs` and is skipped. After spilling,
`finish_spills` builds the function's spill-register array in
register-number order. `allocate_reload_reg` then gives out reload
registers from it round-robin (`last_spill_reg`), skipping registers
that pseudos use at the insn.

So when the ROM's first reload register is r3 where the draft gets r2,
the ROM had r2 busy at that insn. From then on, every later reload
rotates through a different set: {1,3,6} against {1,2,6} in
`sub_8014B54`, and r3, r3, r3, r4, r6, r2 against r6, r4, r4, r6, r2 in
`sub_800BD48`. The fix is to make r2 hard-live at that one insn:

```c
register s32 hold asm("r2");

asm("" : "=r"(hold));   /* r2 live from here: no code */
self->part->y += 0x600; /* the reload that should take r3 */
asm("" : : "r"(hold));  /* ...to here */
```

Only the spill choice at the first reload needs to change. Later
reloads follow from the round-robin, so there's no need to cover them.
To find the insn, dump with `-da` and read the `.greg` file's
`Spilling for insn N. / Spilling reg R.` lines.

Don't use it to hold r7 or r8 (see the brief's hard rules). Also make
sure no pseudo needs the held register in that range, or the allocation
shifts.

## Not closed (4)

| Function | Before | Now | What's left |
|---|---|---|---|
| `sub_80352AC` (#64) | 114 hw, 4 bytes long | 114 (unchanged draft) | GCSE's PRE still hoists `slot << 5` to the y loop's pre-test, next to `slot + 1` and `i + 1`, and spills it. A `"+r"` escape on `slot` after the palette `LoadTaggedAsset` stops the hoist (right size, 82 hw) but also stops `slot + 1` being hoisted, and puts `slot` in r9. A `"+r"` copy of `slot` used only for the palette address gets its copy hoisted instead (424 bytes). An `r0` register variable for `slot << 5` changes nothing, because expand computes it in a pseudo first. `gcse.c` only enters sets of pseudos in its table (`hash_scan_set`), so the ROM's `slot << 5` probably isn't a plain pseudo set at that point. The source form that does this wasn't found. |
| `sub_803686C` (#65) | 329 hw | 329 (unchanged draft) | New finding: six or more bare `asm("")` statements in the row-copy loop body push `insn_count` past loop.c's `threshold * savings * lifetime` limit, so the 0x100 step stays in the loop as in the ROM (327 hw, 1152 bytes). But `buf + 0x60` is still not reduced to its own giv, and the self/`i` register roles are still swapped. A per-row `d = buf + 0x60` local with padding before and after it, and a `d + 0x7a0` second destination, did not reduce it either (about 100 variants). |
| `sub_8038538` (#67, GAX) | ~294 hw | not retried | Too far from the ROM for this pass (register choice after `field_1c`, the tap scans, the constant order before the copy loops, the tail). |
| `sub_8039B44` (#68, GAX) | ~202 hw | not retried | Same: `self`/`info`/`flag` land in r5/r7/r9 instead of r6/r4/r5, and that difference spreads through the function. The hard-register hold can't fix it, since the three values are live for the whole function. |

Helper scripts (not committed) are in the scratchpad's `late3/`: `d.py`
(one-function diff against the ROM, with a register-normalized count),
`var.py` (variant runner), and `rtl.sh`/`fnrtl.py` (per-function RTL
dumps).

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
