# Last-nine NAKED retry

This pass retried the three drafts left by
[last-eight-naked-retry.md](last-eight-naked-retry.md). Nothing closed,
but two drafts got much closer. All three functions stay NAKED, with the
drafts under `#if NON_MATCHING`.

| Function | File | Before | Now | Status |
|---|---|---|---|---|
| `sub_802062C` (#31) | `src/graphics/graphics_loading_1feec.c` | 12 | 4 (same size, old_agbcc) | Draft updated |
| `DrawPauseFraction` (#7) | `src/graphics/settings_menu16.c` | 45 | 14 (same size, both compilers) | Draft updated |
| `sub_8001DB4` (#4) | `src/system/link_cable.c` | 136 | 136 | Note added |

## `sub_802062C`: 12 to 4

The greg dump explains the sl/r9 swap:

- `&gEntityFlags` is local to block 0, so local-alloc gives it r5.
- Reload then spills r5, and `retry_global_alloc` places the address
  again.
- By then global.c has already given -0x11 (a global pseudo, live into
  the second flip) r9, so the address gets sl.

A hard-register hold fixes it:

```c
register s32 h9 asm("r9");
...
rec2 = LEVEL_RECORD(arg3);
asm("" : "=r"(h9));      /* hold starts */
{ ... second flip ... }
asm("" : : "r"(h9));     /* hold ends */
```

-0x11 is live across that range and the address isn't. So -0x11 skips
r9 and gets sl, and the retried address takes r9.

The r4/r3 swap in the second flip comes from global.c's priority order:

- The ROM has the reloaded `q2` pointer in r3 and the flag byte in r4.
- One extra reference on a `p = q2` copy puts the pointer ahead of the
  byte.
- That extra reference also puts the pointer ahead of `rec2` (2*4/14 vs
  2*5/24), and then the pointer takes r2. Four `asm("" : : "r"(rec2))`
  keep `rec2` ahead of it.

Left (4 halfwords):

- `str r3, [sp]` comes before `adds r1, r7, #0`, where the ROM has it
  after.
- The second `&gEntityFlags` reload goes through r3, where the ROM
  uses r1.

The ROM's shape is a caller-save of one part+0x28 pseudo in r3: reload
puts the save right before the call and the restore right before the
next use. For that, global.c must find every callee-saved register
taken when that pseudo's turn comes (6 refs over ~50 insns, so it's
allocated early). A single-pointer draft with an `r4` pin on `arg3`
and a zero-length r5 hold after the call was 62-140 hw.

## `DrawPauseFraction`: 45 to 14

**r7 is never a pseudo's register here.** The function is one basic
block, so local-alloc allocates everything, and local-alloc never uses
the eliminable frame pointer (r7). In the ROM, r7 is reload's register
for the plain 0x110 constant:

1. In the first half, reload loads 0x110 into r7 because r0-r3 are all
   busy at the posX load, and it inherits r7 for the posX store.
2. In the second half, reload_cse turns `ox = 0x110` into
   `adds r6, r7, #0`.
3. Reload loads the posY-load constant into r7 again, and move2add turns
   that into `adds r7, #4`.
4. The posY store's constant becomes `adds r2, r7, #0`.

The draft follows that:

- **First half.** Plain `d->posX`/`d->posY`, and the plain constant
  `0x110` passed to `set_icon_mgr_pos`. The earlier `OFF` pseudo was
  what put 0x110 in r2.
- **`pe0` assigned where it's first used**, not at the top. This fixes
  the hoisted `&gLargeFont` load. It also puts `pe0` in r6 and
  frees r6 for the `#32` reload in the first draw, as in the ROM.
- **Second-half posY through an inline** (`get_icon_mgr_posy`). A direct
  `e->posY` read has its 0x114 forced into a pseudo at expand, and CSE
  shares that pseudo with the first half's posY load (69 hw). Through
  the inline, the address stays `(reg + 0x114)` for reload.
- **Sums passed straight to the setter**:
  `set_icon_mgr_pos(*pdc, ox, AT(e, ox) - 5, get_icon_mgr_posy(e) + 8)`
  is 14 hw, where separate `x`/`y` locals are 17.

The first half, the prologue and the r4/r5/r6 assignment of
0x130/`pdc`/`pe0` now match. Left:

- The second half's `ox` (still an `OFF` pseudo) gets r2. The ROM has
  r6, so `e`, both address temporaries, `x` and `y` have to take r0-r3
  first.
- As a result, reload uses r6 instead of r7 for 0x114.

Things that didn't help:

- extra references on `ox` (22-40);
- `ox` set before `e = *pe0` (it then overlaps `pe0`);
- `"+r"`-escaped `ox` (69);
- bare `asm("")` padding (no change);
- extra `pdc` references (fix `pdc` but flip `pe0`/0x130);
- `asm("" : : "r"(0x130))` (35+).

## `sub_8001DB4`: notes only

`-fno-rerun-loop-opt` stops the inner copy loop's reversal: the counter
counts up with pointer bivs, as in the ROM. That fits last-eight's
finding that the reversal happens in the rerun pass. But the flag also
does three things that rule it out for now:

- It un-reverses the first id loop, which the ROM does reverse.
- Combine folds `w & 0xff` into a second `ldrb` (142 hw, 12 bytes
  short).
- It changes `sub_8002114`, which already matches, in the same file.

A hand-reversed first loop (`for (i = 3; i >= 0; i--)` over a
`field_28` pointer) under the flag was 161-171 hw. If this function
ever closes with the flag, it needs its own address-keyed file.

## Helpers

These are in the scratchpad's `last9/`, pointed at this worktree:

- `d.py`, `var.py`, `rtl.sh` and `fnrtl.py`;
- `flat.py` (one RTL insn per line);
- `pri.sh` (call-crossing pseudos and their hard registers from the
  lreg dump);
- `fcheck.sh` (a whole file with and without a flag);
- the variant specs `a1`-`a5` (`sub_802062C`), `m1`-`m9`/`n1`-`n6`
  (`DrawPauseFraction`) and `l1` (`sub_8001DB4`).

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
