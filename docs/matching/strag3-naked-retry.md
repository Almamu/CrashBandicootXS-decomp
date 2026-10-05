# NAKED retry (strag3): 2 of 2 closed

These were the last two NAKED game functions outside the per-issue
retries, each with an open parked-function issue. Both are now real C.

| Function | File | Size | Before | Compiler | Result |
|---|---|---|---|---|---|
| `DrawPowerDialog` | `src/graphics/oam_count.c` | 240 B | no draft in tree | agbcc (either) | **Closed** (#101) |
| `DrawWrappedText` | `src/graphics/text_layout.c` | 384 B | heavily pinned draft, "2 residuals" | old_agbcc | **Closed** (#102) |

## DrawPowerDialog

This is a HUD icon + number renderer. It measures a label with the icon
manager's `record->slots[0]` method, centers it at `(240 - w) >> 1`,
sets the Y, draws it with `slots[2]`, and does the same for a second
manager after the number is formatted. The method calls are gcc 2.x
virtual calls through `_call_via_r2`, written the same
way as in `settings_menu11.c` and `counter_selector_icons.c`.

- The first plain draft, with direct `mgr->posX = ...; mgr->posY = ...;`
  stores, was 6 instructions long and had the stores in the wrong order.
  Its old note blamed an "r7 scratch" that no pin could reach. In the
  ROM, r7 is just a reload register that nothing else uses.
- Passing the position through a `static inline set_icon_mgr_pos(m, x, y)`
  gives the ROM's order (x computed, manager reloaded, y, then both
  stores), because gcc expands all of an inline call's arguments first
  (#493). That left a permutation: 0x130 and 240 had their registers
  swapped, and so did x and y.
- **The fix:** compute `x = (u32)(240 - w) >> 1;` into a local and pass
  `x` to the setter, instead of passing the expression. It matches under
  both compilers, with no pins or asm. It stays in `oam_count.c`, which
  is built with agbcc.

## DrawWrappedText

This is the word-wrap text renderer. The in-tree `#if NON_MATCHING` draft
was about 95 lines of register pins, `volatile` locals and asm moves.
It was replaced by a plain rewrite:

- `while (*token != 0 && lineCount < limit)` loop (`token = text` before
  it). The `/b`/`/n` escape dispatch is a `switch` with `'b'` falling
  through into `'n'`, which gives the ROM's `beq b; cmp; beq n; b other`.
- `set_pos(self, x, y)` inline setter, used both for the initial position
  from `box` and for `/b`.
- **`/b` reads through `*pos_x(self)`/`*pos_y(self)` inline address
  accessors.** With plain `self->posX` reads, only the stores used the
  loop-hoisted `&posX`/`&posY` (the ROM's `[sp,#0x14]`/`[sp,#0x18]`
  slots), and the reads recomputed `0x110 + self`. Going through the
  returned pointer makes the reads use the hoisted addresses as well.
- `asm("" : : "r"(len))` after `GetWordLength` (the extra-reference nudge
  from #468). Without it, `self` gets r7 and `len` gets r9. With it,
  `len`/`self`/`charWidth` get r7/r8/r9 as in the ROM, and the copied
  entry test is no longer cross-jumped into the bottom test.
- In the fits-on-line arm, `if (mode != 1) goto skip; goto flush;` gives
  the ROM's `bne skip; b flush`. `if (mode == 1) goto flush;` inverts it.
  A duplicated flush block does not get cross-jumped.
- In the wrap arm, a hard-register hold of r1 around `lineCount++` (the
  #489 technique; it emits no code) moves the spilled `lineCount`'s
  reload to r2 and `limit`'s to r0. `++lineCount`, `lineCount + 1` into
  a temp, reversing the compare and an `asm("")` pad all left them in
  r1/r2.
- **Compiler:** with all of the above, agbcc is left with one difference:
  the `/b` handler's first `ldr rX,[sp,#0x14]` uses r0 where the ROM
  has r2. old_agbcc matches exactly. `text_layout.c` contains only this
  function, so the whole object moved to `OLD_AGBCC_OBJS`.

The old progress doc's "argument-spill ordering" and "xAddr/yAddr spill
timing" gaps turned out to follow from the loop shape and the address
accessors. They do not need their own workarounds.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from
  `oam_count.c` or `text_layout.c`.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
  crashbandicootxs.map && make compare`: `crashbandicootxs.gba: OK`.
