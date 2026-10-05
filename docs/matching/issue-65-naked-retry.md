# Retrying issue #65's NAKED functions (`0x080354E0`-`0x08037110`)

Issue #65's range had 18 NAKED functions: `LoadTitleScreenBg` in
`src/graphics/level_graphics.c` and 17 in
`src/graphics/graphics_loading_35780.c`. The first pass
([issue-65-0x08035780-graphics-loading.md](issue-65-0x08035780-graphics-loading.md))
made one quick C attempt per function and then transcribed them.
This pass closed 10 of the 18. The other 8 stay NAKED. Seven of those
now have a C draft under `#if NON_MATCHING`.

**Compiler:** old_agbcc, for both files. Every function that already
matched in the two files (`InitTitleScreen`, `LoadTitleScreenObjTiles`,
`UpdateTitleLogoPieces`, `HashTitleCheatInput`) compiles the same under both compilers.
`LoadTitleScreenBg`, `InitLogoActor`, `DrawLogoActor` and `RunCompanyLogos`
only match under old_agbcc. So both objects went onto the Makefile's
`OLD_AGBCC_OBJS`, and no split was needed.

## Closed (10 functions)

| Function | What it took |
|---|---|
| `LoadTitleScreenBg` | Needed nothing but old_agbcc. Two passes had documented a "dead r7 in the push list" gap that no pin could reproduce, and it was only the wrong compiler. Plain C with indexed `mapBuf[i]`/`mapBuf[i + 1]` reads matches with no pins. The loop optimizer turns the indexing into the ROM's separate `r2` walk pointer, and `mapBuf` stays in `r8` for the final free. `bg2cnt` is left uninitialized: the ROM does `& 0xFFFF0000` on whatever the register held. |
| `CommitTitleScreenFrame` | The "`REG_BG2PA`'s address derived from `REG_BG2Y`'s" gap is a chained assignment, `REG_BG2PA = scale = self[0x87]`. It makes gcc compute the address before the load. |
| `DrawTitleMenuItem` | The two `_call_via_r2` calls are virtual calls through the icon manager's `record->slots[0]`/`[2]`. It is written like `actor_part_1b85c.c`: a `slot` pointer, the X position computed from the first call, then one fresh `im` read feeding a `SetIconPos` inline and the second call. |
| `DrawTitleScreen` | Plain C. |
| `DestroyTitleScreen` | Plain C, plus a `zero` local assigned before the palette pointer, so the constant is materialized first as in the ROM. The old comment was wrong: the loop clears BG palette RAM (`0x05000000`), not OAM. |
| `RunCompanyLogos` | The subsystem driver (888 bytes). It uses an inline `New(size)` wrapper around `mem_alloc(size, 0x80000000)`, which puts the size constant after the flag, and `_call_via_r1` virtual calls through the actor part's method table. The DISPCNT shadow updates go through `struct dispcnt_bits`. In the zoom-in phase, "step the counter, then test it again (`== -1` grow / `<= 0x40` shrink)" gives the ROM's block order. The BG2 X/Y values are computed before either store. It also needs one `register ... asm("r1")` pin on the fade-out counter; without it the value lands in r2 and costs a copy. |
| `LoadVvLogoGraphics` | Direct `self[...] = Alloc(...)` stores. The last two use a destination pointer taken before the call: `dst = &self[0x10c]; *dst = (u32)(buf = sub_8026EC0(size))`, because the ROM computes the address first. |
| `InitLogoActor` | A `CurFrame`-style inline (as in `actor_part128.c`), with `frame[1] * frame[0] * 32` in that order. |
| `UpdateLogoActor` | `ACTOR_SET_STATE`, plus `switch ((u32)self->state)` to get the ROM's unsigned case tree. |
| `DrawLogoActor` | The same draw shape as `actor_part128.c`'s `DrawJetpackPlayer`. Division goes through `__divsi3` and the virtual calls through `_call_via_r2`, and the tile number is pinned to r0. The keyframe record is a declared local assigned from the `table[idx]` CSE temp. `w`/`h`/`sx`/... are declared in a nested block, so their stack slots come after `rec`'s as in the ROM. |

New shared pieces in `graphics_loading_35780.c`:
- `struct dispcnt_bits` (the `gDispcnt` DISPCNT shadow).
- `union bgcnt`.
- `struct slot_seed`.
- `struct cam_ref`.
- The `.set` aliases for `__divsi3`, `__modsi3`, `_call_via_r1` and `_call_via_r2`.

## Still NAKED (8 functions)

| Function | State | What is left |
|---|---|---|
| `TitleScreenCheatInput` | draft | Everything matches except the R-shoulder gate. That includes the cross-jumped hash branches the first pass blamed, which come from a `static inline` hash (split rotate, v/hi/lo pinned to r0/r1) called in each `else if` arm. At the gate, the ROM builds `0x100` in r4 and copies it to r1, one extra instruction. `u16` mask locals, operand order and a 32-bit struct read did not reproduce it. |
| `UpdateVvLogoPieces` | draft, 20 hw | The body matches (accessor-per-field, `recordLoaded` pinned to r0 as in `UpdateTitleLogoPieces`). Two things differ. The drain loop's preheader computes the end pointer before hoisting its two constants. And the tail's `0x444`/`0x448` offsets land in different registers. |
| `LoadUniversalLogoBg` | draft, 30 hw | BGCNT and DISPCNT are bitfield stores (`union bgcnt`, `struct dispcnt_bits`). This reproduces the ROM's unmerged `& 0xFFFF3FFF` and the `-8` mask. `dest`/`y`/`bg2cnt` get r6/r4/r5 instead of r4/r5/r6. None of the 5040 declaration orders does better. |
| `InitVvLogoPieces` | draft | The body is right, but this compiler reverses the 20-iteration loop into a count-down. The ROM keeps `i` counting up. Explicit walking pointers do not stop the reversal (`-fno-strength-reduce` does, but that is not a per-file option here). |
| `ResetTitleLogoPieces` | draft | The same seed-loop problem as `RunTitleScreen`. With `self` pinned to r3, the countdown and record accessors come out as in the ROM (`self+CONST` re-added each iteration). The ROM also does three things this draft does not. It re-materializes 0 and -1 every iteration instead of hoisting them. It leaves `i<<3` unreduced for the hold field. And it post-increments a high-register counter pointer. |
| `RunTitleScreen` | draft | The same seed loop, inlined. The phases after the loop are written out, including the `(self[0] + 1) % 3` via `__modsi3`. |
| `DrawTitleLogoPieces` | draft, ~400 hw | Structurally right. The three OAM bitfield builders match exactly, including the ROM's interleaved byte and/or sequences with the `oam_attrs` layout from `actor_part_1dfec.c`. So does the `SetAffine(buf, m, pa, pb, pc, pd)` inline, which writes `entries[m*4 + k]`. Two things were needed for the X/Y reads: the position reads must be `(posX.q >> 16)`, which makes gcc narrow the load to `ldrb`/`ldrsh`, and the 5-slot loop's slot pointer is `SLOT_AT(self, 7) - i`. What is left is loop register and stack-slot allocation. The ROM hoists the DMA pointer into r9 and spills the offset-table pointer, and this draft does the opposite. |
| `DrawVvLogoPieces` | not attempted | The 20-slot OAM builder, 1160 bytes, the same shape as `DrawTitleLogoPieces`. It is left for the pass that closes `DrawTitleLogoPieces`. |

A pattern across the three seed loops (`InitVvLogoPieces`, `ResetTitleLogoPieces`,
`RunTitleScreen`): the ROM's loop optimizer reduced some givs and not
others, kept the counter, and did not hoist cheap constants. Our
compilers at `-O2` either reduce everything or, with hard-register
pins, nothing. The next attempt should look at what makes those
particular givs non-reducible, for example C++ inline member calls
whose argument copies are set inside the loop.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from either file.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`: `crashbandicootxs.gba: OK`.

## Later pass (issue #64/#65 NAKED retry)

`TitleScreenCheatInput`, `UpdateVvLogoPieces` and `LoadUniversalLogoBg` are now real C (same
old_agbcc + `-fno-strength-reduce` object; `InitVvLogoPieces` had already
closed through that flag). The flag itself changed none of the drafts;
what closed them were source-shape details - see
[issue-64-65-naked-retry.md](issue-64-65-naked-retry.md).
`DrawTitleLogoPieces`, `RunTitleScreen`, `ResetTitleLogoPieces` and `DrawVvLogoPieces` are still
NAKED.
