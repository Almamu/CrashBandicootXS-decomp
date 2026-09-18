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
- `src/system/game_loop10.c` (GitHub issue #37 - numbered `10` rather
  than `6` since issue #12's parallel PR independently claimed
  `game_loop6.c`/`game_loop7.c` first): `sub_80234E8`,
  `sub_80234F4`, `sub_8023500`, `sub_8023510`, `sub_802352C`,
  `sub_8023548`, `sub_802356C`, `sub_80235E4`, `sub_802364C`,
  `sub_8023658`, `sub_8023674`, `sub_802369C`, `nullsub_24`,
  `sub_80236AC` - camera-position setters, checkpoint/level-transition
  snapshot helpers, the `sub_8022468` mode-trampoline family, and a
  packed-bitfield unpacker
- `src/system/game_loop11.c` (GitHub issue #37): `sub_8023738` - lazily
  allocates and returns `gUnknown_03000828`
- `src/system/game_loop8.c` (GitHub issue #37): `sub_802400C` - the
  DMA3/VRAM refresh pass gated on `self+0x0 <= 0x1000`
- `src/system/game_loop9.c` (GitHub issue #37): `sub_8024198`,
  `sub_80241A4`, `sub_80241B0`, `sub_80241BC`, `sub_802423C` - a
  boolean flag clear/set/get trio, the level-end teardown, and the
  shared vram-upload-cursor/OAM-shadow flush tail
- `src/system/game_loop12.c` (GitHub issue #41): `sub_8025944`,
  `sub_8025968`, `sub_802599C` - the first two of three overlapping
  bit-grid accessors at `self+8`/`self+0x208`/`self+0x308`
- `src/system/game_loop13.c` (GitHub issue #41): `sub_8025A0C`
  (third bit-grid setter), `sub_8025A3C` (Q8-to-int store),
  `sub_8025A44` (conditional `sub_8026ED0` forward), `sub_8025A5C`
  (zero two Q8 words)
- `src/system/game_loop14.c` (GitHub issue #41): `sub_8025D28`
  (table-indexed function-pointer dispatch via the interworking
  trampoline convention), `sub_8025D4C` (store two Q8 words),
  `sub_8025D54` (conditional `sub_8026ED0` forward, dup of
  `sub_8025A44`), `sub_8025D6C` (zero two Q8 words, dup of
  `sub_8025A5C`)
- `src/system/game_loop15.c` (GitHub issue #41): `sub_8025DE8`,
  `sub_8025E2C` (streamed-tile-range growers firing a `self->0x30`-
  table trampoline per step), `sub_8025E70`, `sub_8025E84` (their
  plain clamp-only counterparts)
- `src/system/game_loop16.c` (GitHub issue #41): `sub_8025F24` -
  truncates the Q8 position to a tile-scroll halfword pair and writes
  it through the `self+0x58` hardware-register pointer
- `src/system/game_loop17.c` (GitHub issue #38): `sub_802425C`,
  `nullsub_25`, `sub_8024278` - a bit-tested `sub_8026ED0` teardown
  wrapper, an empty stub, and the medal-table per-level tally
- `src/system/game_loop18.c` (GitHub issue #38): `sub_80243E0`,
  `sub_8024404`, `sub_8024428`, `sub_8024434`, `sub_8024440`,
  `sub_802444C`, `sub_8024458`, `sub_8024464`, `sub_8024498`,
  `sub_80244F0`, `sub_8024524`, `sub_8024540`, `sub_802455C` -
  medal-table entry/item-list field accessors, the sound-cue resolver,
  and the `sub_8024344` constant wrappers (`sub_8024344` itself is left
  raw, see below)
- `src/system/game_loop19.c` (GitHub issue #38): `sub_8024784` -
  trivial `gUnknown_03001314` setter
- `src/system/game_loop20.c` (GitHub issue #38): `sub_80247EC`,
  `sub_8024804` - the `sub_802425C`-shaped teardown wrapper and a
  trivial constructor
- `src/system/game_loop22.c` (GitHub issue #13 - numbered `22` rather
  than `17` since issue #38's PR above independently claimed
  `game_loop17.c`-`20.c` first): `sub_800FEB0` - resets
  `self`'s collision-response state/timer/neighbor-list-pointer block
  on reset
- `src/system/game_loop23.c` (GitHub issue #13): `sub_80106DC`
  (viewport collision-box refresh), `sub_8010708`/`sub_801070C`
  (neighbor-list "get prev"/"get next"), `sub_8010710`/`sub_8010714`
  ("set prev"/"set next"), `sub_8010718` (UNUSED trivial constant)
- `src/system/game_loop24.c` (GitHub issue #13): `sub_8010804` (a
  state-3-countdown-expiry sweep over `gUnknown_0300130C`),
  `sub_801085C` (viewport trampoline-pair/cue-1 firing)
- `src/system/game_loop25.c` (GitHub issue #13): `sub_8010908` -
  trivial `gStaticData_0816BBAE[idx]` lookup
- `src/system/game_loop26.c` (GitHub issue #13): `sub_8010A00` -
  `self+0x48` bits 6-7 sub-state extractor

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED transcription (byte-correct, not decompiled)

These are byte-exact (confirmed by a full clean `make compare`), but
as `NAKED` functions whose body is the ROM's own disassembly
transcribed instruction-for-instruction rather than real decompiled C,
they don't count as "matched" for this project's tracking - the goal
is readable C, and an asm blob wrapped in a C function signature
doesn't advance that even when byte-correct. See
[docs/workflow.md](../workflow.md)'s NAKED-transcription escape hatch
(`sub_8001CB8`/`sub_8001DB4` in `src/system/link_cable.c`) for the
established convention, and each entry's linked write-up for why
plain C didn't converge.

- **`sub_800D040`** (`src/system/game_loop6.c`, GitHub issue #12) -
  builds `self`'s and the player's AABB from the shared
  `+0x20`-table-pointer/`+0x2d`-tag hitbox-record convention
  (`sub_8007B00`/`sub_8007B98` in `actor_part.c`), dispatches to
  `sub_800EEF0`/`sub_800E7A8` on overlap. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
- **`sub_800E494`/`sub_800E4E4`** (`src/system/game_loop7.c`, GitHub
  issue #12) - bidirectional linked-list walkers (`sub_801070C`/
  `sub_8010708`) clearing/setting each neighbor's `+0x58` flag. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).

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
- **`sub_80236EC`** (`src/system/game_loop10.c`, GitHub issue #37 -
  numbered `10` rather than `6` since issue #12's parallel PR above
  independently claimed `game_loop6.c`/`game_loop7.c` first) - the
  inverse of `sub_80236AC`'s packed-bitfield unpacker; real bytes
  in `asm/code_3_2_17_236ec.s`.
- **`sub_80240E4`** (`src/system/game_loop8.c`, GitHub issue #37) - the
  `REG_BLDCNT`/`REG_BLDALPHA` shadow-word rebuild; real bytes in
  `asm/code_3_2_17_240e4.s`. See
  [docs/matching/issue-37-game-loop-234e8.md](../matching/issue-37-game-loop-234e8.md)
  for both parked functions' exact register-allocation gaps.
- **`sub_8025894`** (`src/system/game_loop12.c`, GitHub issue #41) - a
  group/item list counter with a 19-entry jump table; real bytes stay
  in `asm/code_3_2_17_255d4.s`. See
  [docs/matching/issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md).
- **`sub_80259D4`** (`src/system/game_loop13.c`, GitHub issue #41) -
  sets a bit in both the `self+0x208` and `self+0x308` bit-grids at
  once; real bytes in `asm/code_3_2_17_259d4.s`. See
  [docs/matching/issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md).
- **`sub_8025A64`/`sub_8025B0C`/`sub_8025BAC`/`sub_8025CA4`**
  (`src/system/game_loop14.c`, GitHub issue #41) - four part-object
  spawn helpers (`gUnknown_030012D0`-table-indexed,
  `sub_8009ED0`/`sub_8011114`/`sub_801173C`-family constructors); real
  bytes in `asm/code_3_2_17_25a64.s`. See
  [docs/matching/issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md).
- **`sub_8025D74`** (`src/system/game_loop15.c`, GitHub issue #41) -
  BG-scroll-layer hardware-register/bitfield initializer; real bytes
  in `asm/code_3_2_17_25d74.s`. See
  [docs/matching/issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md).
- **`sub_8025E98`/`sub_8025F3C`** (`src/system/game_loop16.c`, GitHub
  issue #41) - the streamed-tile-range screen-edge-tile computer, and
  a circular-buffer decoded-tile streaming loop; real bytes in
  `asm/code_3_2_17_25e98.s`/`asm/code_3_2_17_25f3c.s` respectively.
  See
  [docs/matching/issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md).

## Left raw, semantics traced but not byte-matching (GitHub issue #38)

- **`sub_8024344`** (ROM `0x08024344`, real bytes in
  `asm/code_3_2_17_24344.s`) - scans a medal item list for a nonzero
  `u16` flag; every field/offset/branch confirmed, but the ROM keeps its
  `flagIdx` parameter alive in `ip`/r12 across the whole function rather
  than a normally-allocated register. See
  [docs/matching/issue-38-medal-results-tally.md](../matching/issue-38-medal-results-tally.md).
- **`sub_8024590`/`sub_8024640`/`sub_80246D8`/`sub_8024708`** (ROM
  `0x08024590`-`0x08024783`, real bytes in `asm/code_3_2_17_24590.s`) -
  a sound-channel-handle helper family (start/wait-then-play cue
  selection, its per-item driver loop, an index scanner, and a
  VRAM-bank-toggling tile-asset streamer + palette DMA + second
  `DISPCNT` writer); every field/offset/call argument confirmed, gap is
  register-allocation-level throughout. See
  [docs/matching/issue-38-medal-results-tally.md](../matching/issue-38-medal-results-tally.md).
- **`sub_8024790`** (ROM `0x08024790`, real bytes in
  `asm/code_3_2_17_24790.s`) - the tail half of `sub_8024640`'s
  per-item body, reused standalone; same open gap as the group above.
  See
  [docs/matching/issue-38-medal-results-tally.md](../matching/issue-38-medal-results-tally.md).

## Still raw, category-mapped (GitHub issue #12/#34/#40)

- **`sub_0800D18C`/`sub_800E08C`** (`asm/code_3_2_17_d18c.s`, ROM
  `0x0800D18C`-`0x0800E494`, GitHub issue #12) - the physics/collision
  subsystem's largest, most tangled functions (docs/rom_map.md:
  "Confirmed: a shared physics/collision subsystem, entered from
  multiple different entity types" - `sub_0800D18C` alone is ~1960B, a
  6-case jump-table collision-response commit that maintains a 5-slot
  ring buffer inside `gUnknown_030012D8` and calls 27 other functions
  in this same neighborhood). Not understood branch-by-branch with the
  precision a byte-exact reconstruction needs yet - see
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
- **`sub_800E560` onward through `sub_800F990`** (`asm/code_3_2_17_e560.s`,
  ROM `0x0800E560`-`0x0800FC70`, GitHub issue #12) - the rest of this
  chunk's 25-function list: the collision-response jump-table handlers
  `sub_0800D18C` itself dispatches to (`sub_800E620`, `sub_800E6B0`,
  `sub_800E7A8`, `sub_800E888`, `sub_800EAFC`, `sub_800ED08`,
  `sub_800EDBC`, `sub_800EEF0`, `sub_800F06C`, `sub_800F1B8`,
  `sub_800F258`, `sub_800F2BC`, `sub_800F368`, `sub_800F4F4`,
  `sub_800F5B8`, `sub_800F6B8`, `sub_800F798`, `sub_800F8E0`,
  `sub_800F990`), each a moderately-sized state-machine function with
  several sibling calls within this same still-raw neighborhood; left
  untouched for this pass - see
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
- **`sub_800FC70`/`sub_800FDC8`** (`asm/code_3_2_17_e560_fc70.s`, ROM
  `0x0800FC70`-`0x0800FEB0`, GitHub issue #13) - a position-wrap
  advance function and a Bresenham-line-style step algorithm; not
  attempted this pass - see
  [docs/matching/issue-13-graphics-fc70.md](../matching/issue-13-graphics-fc70.md).
- **`sub_800FF0C`/`sub_8010480`/`sub_80104E4`/`sub_8010674`**
  (`asm/code_3_2_17_e560_ff0c.s`, ROM `0x0800FF0C`-`0x080106DC`, GitHub
  issue #13) - `sub_800FF0C` is a large (~660-instruction) projectile/
  hazard-spawn dispatcher with two big jump tables, out of scope for
  this pass; `sub_8010480`/`sub_80104E4`/`sub_8010674` are its smaller
  neighbors, not attempted - see
  [docs/matching/issue-13-graphics-fc70.md](../matching/issue-13-graphics-fc70.md).
- **`sub_801071C`/`sub_801075C`/`sub_8010784`/`sub_80107C4`**
  (`asm/code_3_2_17_e560_1071c.s`, ROM `0x0801071C`-`0x08010804`,
  GitHub issue #13) - a part-object init helper, another init helper,
  and two more Bresenham-line-style step algorithms; not attempted -
  see [docs/matching/issue-13-graphics-fc70.md](../matching/issue-13-graphics-fc70.md).
- **`sub_801089C`** (`asm/code_3_2_17_e560_1089c.s`, ROM
  `0x0801089C`-`0x08010908`, GitHub issue #13) - a cue-3-plus-spawn
  helper; not attempted - see
  [docs/matching/issue-13-graphics-fc70.md](../matching/issue-13-graphics-fc70.md).
- **`sub_8010914`/`sub_801095C`/`sub_80109A4`**
  (`asm/code_3_2_17_e560_10914.s`, ROM `0x08010914`-`0x08010A00`,
  GitHub issue #13) - two neighbor-list-walk-and-filter helpers and a
  distance-gated dispatcher; not attempted - see
  [docs/matching/issue-13-graphics-fc70.md](../matching/issue-13-graphics-fc70.md).
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
- **`sub_802375C`/`sub_8023A1C`** (`asm/code_3_2_17_2375c.s`, ROM
  `0x0802375C`-`0x08024007`, GitHub issue #37) - a level-start
  dispatcher (spawns several HUD/counter widget objects) and its
  ~650-instruction jump-table-driven continuation; several callees
  (`gStaticData_0816C8xx` tables, `sub_8027018`, `sub_80266BC`,
  `sub_800B3F0`) not characterized precisely enough yet - see
  [docs/matching/issue-37-game-loop-234e8.md](../matching/issue-37-game-loop-234e8.md).
