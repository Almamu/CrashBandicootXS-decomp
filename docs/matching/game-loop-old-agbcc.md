# The game_loop and HUD NAKED functions under old_agbcc

Issues #34, #37, #38, #39, #40, #41 and #45 left 26 functions in
`0x08022D50`-`0x08027138` as NAKED transcriptions. Most were parked on
"register-allocation permutation" gaps, or on the r7 prologue problem
(the ROM pushes r7 and the current `agbcc` never adds it).

Like the neighbouring regions (docs/matching/old-agbcc-retry.md,
issue-42-bg-scroll-layer.md), this part of the ROM was built with
`tools/agbcc/bin/old_agbcc`. Several of these functions only match under
it (for example `SetupRoomBlend`, `StartTimeTrial`, `InitHud`, and four of
the terrain tile cache's lookups), and every other function in the
touched files matches under it unchanged.

## Result: 18 of 26 are now plain C

No register pins, no asm in a function body and no NAKED.

| function | file |
|---|---|
| `StartTimeTrial` | `game_loop40.c` |
| `SetupRoomBlend` | `game_loop8.c` |
| `BeginSlide` | `game_loop37.c` |
| `StreamBgRow`, `StreamBgColumn`, `FillBgStreamer` | `game_loop57.c` |
| `GetCollisionChunk`, `GetTerrainHeights`, `GetSolidTerrainHeights`, `sub_8025228` | `game_loop3.c` |
| `GetTerrainType` | `game_loop4.c` |
| `DropExtraLife` | `game_loop29.c` |
| `SpawnEffectPart` | `game_loop14.c` |
| `ScrollBgLayer`, `DrawBgLayerColumn` | `game_loop16.c` |
| `sub_8026A18`, `sub_8026AE8` | `game_loop46.c` |
| `InitHud` | `hud_digit_array.c` |

All eleven files move to `OLD_AGBCC_OBJS` whole.

Still NAKED, with the remaining gap under old_agbcc:

| function | gap |
|---|---|
| `DropWumpa` | 5 halfwords: one `movs r0, #0` scheduled after the wrong address |
| `DecodeLayerChunk` | 13 halfwords: sign-extension order around the delta byte, and the copy loop's index order |
| `DecodeCollisionChunk` | 30 halfwords: the accumulator and the pair loop's pointer swap r4/r5 |
| `ConfigureHudParts` | 32 bytes: the ROM keeps three copies of one nibble insert |
| `LaunchEffectPart` | 61 halfwords: `&srcBox` held in a callee-saved register across a call |
| `RunCutscenePlayer` | 77 halfwords: the ROM reloads `&gOamBuffer` at each OAM flush |
| `SpawnRoomEntities` | 153 halfwords: the second pass's list searches are peeled and index-based |
| `RunRoom` | not attempted |

## What mattered

- **The ROM's r7 is usually just the allocator.** `BeginSlide` gets
  the r7 prologue (and the redundant `orr` copy that needed an asm trick
  before) once the item pointer is re-read inside the compare instead of
  kept live across the call. `DecodeCollisionChunk`'s r7 "shadow pointer" is
  gcc's strength-reduced `&dest[written]`.
- **Inline-helper parameters.** Stores whose value comes before the
  address (`part->tag` in `DropExtraLife` and `StartTimeTrial`) go through an
  `s32` setter; `DrawBgLayerColumn` uses `bg_scroll_layer_25fc8.c`'s `Mod32`.
- **The tile cache's `GetCell`.** A shared inline returning `u16` gives
  the ROM's extra `lsl`/`lsr 16`, with the index written as
  `(y & 7) * 16 + (x & 0xf)` (`<< 4` schedules differently).
  `sub_8025228` reads 36-byte `struct terrain_type` rows.
- **Stepwise index arithmetic.** `StreamBgRow`/`StreamBgColumn` build the
  map index in separate statements; `FillBgStreamer` keeps the row stride
  and block height in `s32` locals so they stay in `sl`/`sb`.
- **Parameter widths.** `DropExtraLife` takes its arguments as words and
  reads the sixth as the stack slot's low byte, as the ROM does.
- **Existing types.** `SetupRoomBlend` uses `level_menu.h`'s
  `union blend`; `ScrollBgLayer`/`DrawBgLayerColumn` use `bg_scroll_layer.h`.

`LaunchEffectPart` takes seven arguments (`pool, arg1, kind, margin, z,
speed, src`); its call into `SpawnEffectPart` is an ordinary six-argument
call, not the "stack-reuse coincidence" its old comment described.

## Pins that turned out to be unnecessary

Under old_agbcc, five already-matched functions in these files match
with all of their register pins removed, so the pins are gone:
`RunSlideshow` (game_loop37.c), `GetBgStreamerColumn`, `GetBgStreamerRow`,
`GetBgStreamerCell` (game_loop57.c) and `UpdateRoomFrame` (game_loop8.c).
`SpawnEntity` and `ShowSlidePicture` still need theirs (5 and 3 halfwords off
without them).

## Later pass: NAKED retry (mid45)

`ConfigureHudParts` (`hud_digit_array.c`) is now plain C. Its three separate
nibble-insert copies survive when the two inner stores go through a
`SetPal` inline, and the slot tests are `switch`es. See
[naked-retry-mid45.md](naked-retry-mid45.md).

## Later pass (strag1)

`DropWumpa`, `LaunchEffectPart`, `DecodeLayerChunk` and `RunCutscenePlayer` from the
"still NAKED" table above are now real C, all under old_agbcc with no
register pins. See [strag1-naked-retry.md](strag1-naked-retry.md).
