# The game_loop and HUD NAKED functions under old_agbcc

Issues #34, #37, #38, #39, #40, #41 and #45 left 26 functions in
`0x08022D50`-`0x08027138` as NAKED transcriptions. Most were parked on
"register-allocation permutation" gaps, or on the r7 prologue problem
(the ROM pushes r7 and the current `agbcc` never adds it).

Like the neighbouring regions (docs/matching/old-agbcc-retry.md,
issue-42-bg-scroll-layer.md), this part of the ROM was built with
`tools/agbcc/bin/old_agbcc`. Several of these functions only match under
it (for example `sub_80240E4`, `sub_8022D50`, `sub_8027138`, and four of
the terrain tile cache's lookups), and every other function in the
touched files matches under it unchanged.

## Result: 18 of 26 are now plain C

No register pins, no asm in a function body and no NAKED.

| function | file |
|---|---|
| `sub_8022D50` | `game_loop40.c` |
| `sub_80240E4` | `game_loop8.c` |
| `sub_8024590` | `game_loop37.c` |
| `sub_8024BAC`, `sub_8024C08`, `sub_8024C64` | `game_loop57.c` |
| `sub_8024F24`, `sub_80250BC`, `sub_8025130`, `sub_8025228` | `game_loop3.c` |
| `sub_8025460` | `game_loop4.c` |
| `sub_8025A64` | `game_loop29.c` |
| `sub_8025BAC` | `game_loop14.c` |
| `sub_8025E98`, `sub_8025F3C` | `game_loop16.c` |
| `sub_8026A18`, `sub_8026AE8` | `game_loop46.c` |
| `sub_8027138` | `hud_digit_array.c` |

All eleven files move to `OLD_AGBCC_OBJS` whole.

Still NAKED, with the remaining gap under old_agbcc:

| function | gap |
|---|---|
| `sub_8025CA4` | 5 halfwords: one `movs r0, #0` scheduled after the wrong address |
| `sub_8024960` | 13 halfwords: sign-extension order around the delta byte, and the copy loop's index order |
| `sub_8025334` | 30 halfwords: the accumulator and the pair loop's pointer swap r4/r5 |
| `sub_802732C` | 32 bytes: the ROM keeps three copies of one nibble insert |
| `sub_8025B0C` | 61 halfwords: `&srcBox` held in a callee-saved register across a call |
| `sub_8024820` | 77 halfwords: the ROM reloads `&gUnknown_03001300` at each OAM flush |
| `sub_80255D4` | 153 halfwords: the second pass's list searches are peeled and index-based |
| `sub_8023A1C` | not attempted |

## What mattered

- **The ROM's r7 is usually just the allocator.** `sub_8024590` gets
  the r7 prologue (and the redundant `orr` copy that needed an asm trick
  before) once the item pointer is re-read inside the compare instead of
  kept live across the call. `sub_8025334`'s r7 "shadow pointer" is
  gcc's strength-reduced `&dest[written]`.
- **Inline-helper parameters.** Stores whose value comes before the
  address (`part->tag` in `sub_8025A64` and `sub_8022D50`) go through an
  `s32` setter; `sub_8025F3C` uses `bg_scroll_layer_25fc8.c`'s `Mod32`.
- **The tile cache's `GetCell`.** A shared inline returning `u16` gives
  the ROM's extra `lsl`/`lsr 16`, with the index written as
  `(y & 7) * 16 + (x & 0xf)` (`<< 4` schedules differently).
  `sub_8025228` reads 36-byte `struct terrain_type` rows.
- **Stepwise index arithmetic.** `sub_8024BAC`/`sub_8024C08` build the
  map index in separate statements; `sub_8024C64` keeps the row stride
  and block height in `s32` locals so they stay in `sl`/`sb`.
- **Parameter widths.** `sub_8025A64` takes its arguments as words and
  reads the sixth as the stack slot's low byte, as the ROM does.
- **Existing types.** `sub_80240E4` uses `level_menu.h`'s
  `union blend`; `sub_8025E98`/`sub_8025F3C` use `bg_scroll_layer.h`.

`sub_8025B0C` takes seven arguments (`pool, arg1, kind, margin, z,
speed, src`); its call into `sub_8025BAC` is an ordinary six-argument
call, not the "stack-reuse coincidence" its old comment described.

## Pins that turned out to be unnecessary

Under old_agbcc, five already-matched functions in these files match
with all of their register pins removed, so the pins are gone:
`sub_8024640` (game_loop37.c), `sub_8024B18`, `sub_8024B48`,
`sub_8024B78` (game_loop57.c) and `sub_802400C` (game_loop8.c).
`sub_8025D28` and `sub_8024708` still need theirs (5 and 3 halfwords off
without them).

## Later pass: NAKED retry (mid45)

`sub_802732C` (`hud_digit_array.c`) is now plain C. Its three separate
nibble-insert copies survive when the two inner stores go through a
`SetPal` inline, and the slot tests are `switch`es. See
[naked-retry-mid45.md](naked-retry-mid45.md).
