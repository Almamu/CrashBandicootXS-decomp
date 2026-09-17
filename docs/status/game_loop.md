# Status: game_loop

The top-level per-frame game loop - `MainLoop`, `UpdateGameFrame`, and
the state/counter machinery they drive. Filed under `src/system/` on
disk, tracked as its own `game_loop` category since `docs/rom_map.md`
and the `decomp-chunk` issue generator both treat it as a distinct
system from "core" system startup/init code.

## Matched

- `src/system/main_loop.c` (new file, GitHub issue #45 - categorized
  `hud` by the chunk generator, but `MainLoop` itself is squarely
  `game_loop`): `MainLoop` - the game's actual top-level loop (called
  once from `AgbMain`, sets up the central per-level state object and
  the on-screen counter widget, then runs `UpdateGameFrame` forever) -
  and `sub_8026F38`, a two-level per-counter-widget-mode table lookup
- `src/system/game_loop2.c`: `sub_8022FEC`, `sub_802306C`, `sub_8023104`,
  `sub_8023110`, `sub_8023118`, `sub_8023120`, `sub_8023128`,
  `sub_8023130`, `sub_8023138`, `sub_8023140`, `sub_802314C`,
  `sub_8023158`, `sub_8023168`, `sub_8023184`, `sub_8023190`,
  `sub_802319C`, `sub_80231A8`, `sub_80231B4`, `sub_80231BC`,
  `sub_80231C4` (GitHub issue #34, `UpdateGameFrame`-`MainLoop` cluster -
  a `self+0x80`/`0x84`/`0x88`/`0xac`/`0xc0`/`+2`-flags accessor family
  plus the two frame-counter/limit tick functions)
- `src/system/game_loop3.c` (GitHub issue #40): `sub_8024E68`,
  `sub_8024E90`, `sub_8024EB4` (a viewport/parallax-scroll-layer object)
  and `sub_8024F04`/`sub_8024F0C`/`sub_8024F10`/`sub_8024F14`/
  `sub_8024F18`/`sub_8024F1C`/`sub_8024F20` (its field accessors)
- `src/system/game_loop4.c` (GitHub issue #40): `sub_8025444`,
  `nullsub_4`
- `src/system/game_loop5.c` (GitHub issue #40): `sub_80254C0`,
  `sub_80254F8`, `sub_8025554`, `sub_8025588`, `sub_80255A8`,
  `sub_80255C4` - the terrain tile-record decode cache's constructor,
  a raw-cell-lookup variant, a floor-div-by-32 bitmap set/clear pair,
  and a `CpuSet`-based palette-bank zero-fill wrapper pair

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8022BF0`/`sub_8022CA0`** (`src/system/game_loop.c`, GitHub
  issue #34) - level-start progress-total updater and its cached-state/
  snapshot helper; real bytes in `asm/code_3_2_17_22bf0.s`. See
  `docs/matching.md`'s issue #34 entry for the exact register-allocation
  gaps.
- **`sub_8022EA8`/`sub_8022F2C`** (`src/system/game_loop2.c`, GitHub
  issue #34) - record 47's periodic-trigger setter/decrementer; real
  bytes in `asm/code_3_2_17_22ea8.s`. See `docs/matching.md`'s issue
  #34 entry for the exact register-allocation gaps.
- **`sub_8024F24`/`sub_80250BC`/`sub_8025130`/`sub_8025228`/
  `sub_8025334`** (`src/system/game_loop3.c`, GitHub issue #40) - the
  16-slot terrain tile-record decode/LRU cache's lookup dispatcher, its
  four `(x, y)`-lookup consumer variants, and the RLE/delta
  token-stream decoder; real bytes in `asm/code_3_2_17_24f24.s`. See
  [docs/matching/issue-40-terrain-tile-cache.md](../matching/issue-40-terrain-tile-cache.md)
  for the exact register-allocation gaps.

## Still raw, category-mapped (GitHub issue #34/#40)

- **`UpdateGameFrame`** (`asm/code_3_2_17_225a0.s`, ROM `0x080225A0`) -
  the main per-frame game-loop driver, a ~730-instruction jump-table
  state machine. Not understood branch-by-branch with the precision a
  byte-exact reconstruction needs yet - see `docs/matching.md`.
- **`sub_8022D50`** (`asm/code_3_2_17_22d50.s`, ROM `0x08022D50`) - a
  level-start/reset routine with several still-uncharacterized callees
  - see `docs/matching.md`.
- **`sub_80255D4`** (`asm/code_3_2_17_255d4.s`, ROM `0x080255D4`,
  GitHub issue #40) - a per-frame visible-object/window list processor
  (DMA-writes to OBJ palette RAM and a BG window register, then walks a
  small count-prefixed record list); several callees not characterized
  precisely enough yet - see
  [docs/matching/issue-40-terrain-tile-cache.md](../matching/issue-40-terrain-tile-cache.md).
