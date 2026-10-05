# Issue #51: 0x0802AC28-0x0802B364 - the actor factory

GitHub issue #51 covers `0x0802AC28`-`0x0802BED8`. Its tail was already
done: `UpdatePolarPlayer`-`PolarPlayerStateCaught` in `actor_part127.c`
([issue-52-gap-b364.md](./issue-52-gap-b364.md), four real C, seven
NAKED) and `DispensePolarWumpa`-`PolarPlayerStateBoost` in `actor_part107.c`
([issue-50-actor-bc68.md](./issue-50-actor-bc68.md)). This pass did the
first seven functions, the whole of what was left in
`asm/code_3_2_20_8b7c_ac28.s`. They are now real C in
`src/graphics/actor_part_2ac28.c`, and the raw file is retired.

| function | what it is |
|---|---|
| `CreateActor` | the per-kind actor factory |
| `CreatePolarCheckpointText` | builds record 40 with `InitActorPart`, method table `gPolarCheckpointTextVtable` |
| `SpawnPolarCollectedWumpa` | builds record 11 with `CreatePolarCollectedWumpa` (0x60 bytes) |
| `SpawnPolarAkuAku` | builds record 27 with `CreatePolarAkuAku`, passing a 4th argument through |
| `ConstructAnimTableState` | category vtable slot 0: installs the animation table, builds the player |
| `SpawnActor` | category vtable slot 1: spawn record -> factory call |
| `ConstructActorPart` | the player constructor |

All seven match under both `agbcc` and `old_agbcc`, since nothing in them
depends on what differs between the two. They are built with the current
agbcc like the rest of this zone (`actor_part52.c`/`actor_part54.c`/
`actor_part127.c`).

## The pieces

**`gActorAnimTable`** is the category's animation table, a pointer to
`struct anim_table_record` (`include/actor_anim.h`, 0x28 bytes). This pass
named the record's tail. `InitActorPart` copies +0x14..+0x20 into the
object at +0x38 (`vector_14`), and the factory adds +0x20/+0x24 to the
spawn position (`spawnX`/`spawnY`). The data dump says these are 0 in
every record observed. The code still reads them.

**`CreateActor(u8 kind, x, y, z, spawn)`** adds the record's spawn offset
to x/y and then switches on `kind` (1-39, through a jump table). Every
case body is an inlined C++ `new Foo(...)`. It allocates with
`mem_alloc(size, MEM_HEAP_IWRAM)`, runs a base constructor
(`InitActorPart`, `InitPolarCrate`, or one of the class constructors
`CreatePolarCheckpointCrate`/`CreatePolarLauncher`/`CreatePolarBoostPad`/`CreatePolarPenguin`/`CreatePolarElectricFence`/
`CreatePolarIcicle`/`CreatePolarGoal`/`sub_802CE38`), and, where the base constructor
is a shared one, stores the class's method table at +0x50. Points of
note:

- Kinds 8 and 35 check `IsSpawnCollected(spawn)`, the fixed-slot registry. If
  the spawn is already registered they build a plain record-28 object.
  Otherwise they build a 0x58-byte object that remembers `spawn` at +0x54
  (`struct actor_tracked`).
- Kinds 13 and 25 build two objects from the same class. The first is
  built from a fixed record (14/26) with its animation restarted at
  sequence 1, and the second is the one returned.
- Kind 3 and the kind-13/25 pairs pass the record's own `spawnX` as the X
  coordinate, not the adjusted `x`. This is exactly what the ROM does.
- Kinds 36-39 only select a palette-cycle preset (`SetActorPaletteCycle(kind -
  36)`) and return NULL, as do all unlisted kinds.

The case bodies appear in the ROM in source order (9, 3, 5-7, 1, 4, 22,
12, 24, 28-31, 35, 8, 10, 11, 23, 16/18/20, 25, 13, 2, 36-39). The C
lists them in that order.

**`SpawnActor(spawn, useBonus, zOffset)`** picks the kind from the
level's spawn record (`struct actor_spawn`: kind/altKind/bonusKind bytes
and x/y/z in tiles). In the `gLevelState+0x8C` mode it uses
`altKind`, returns NULL for 11 and folds 3/8/28-31/35 to 1. Otherwise it
takes `bonusKind` when `useBonus` is set. It skips kinds 0, 32-34 and 62,
and calls the factory with the position scaled by 256 (plus `zOffset` on
z).

**`ConstructAnimTableState(table, z)`** sets `gActorAnimTable = table`,
clears the player pointer `gActorList`, and builds the player from
record 0 with `ConstructActorPart`. That function runs `InitActorPart`
(y = `z ? 0x2800 : -0x5000`), installs `gPolarPlayerVtable`, calls
`AllocPolarPlayerTiles`, and starts either state 0xD/anim 0xC (when z != 0) or anim
8. It then resets the `gPolarPauseLocked`-`gPolarPlayerVelY` player-state
globals (the ones `actor_part19.c`/`actor_part44.c`/`actor_part107.c`
drive).

## What it took

- **Allocation through a `static inline` function, not a macro.** The ROM
  loads the size (`movs r0, #0x54`) before the heap flags
  (`movs r1, #0x80; lsls r1, #24`). A direct
  `mem_alloc(0x54, MEM_HEAP_IWRAM)` evaluates the arguments last-first. An
  inline wrapper whose parameter is the size gives the ROM's order.
- **One local per case (block-scoped `self`), not one function-wide
  `self`.** With a single variable, the long-lived pseudo lost `r4` to
  `x`. With a short-lived pseudo per case, the allocator gives the ROM's
  assignment: `self` r4, `kind` r5, `z` r6, `x` r7, `y` r8, `spawn` sb.
  The `NEW_*` macros in the file each open their own block.
- **`REC_AT(kind)`**, i.e. `(struct anim_table_record *)(kind * 0x28 +
  (u32)gActorAnimTable)`, for the record passed by the inlined
  constructors. The ROM scales the index *before* loading the table
  pointer there. Plain `&gActorAnimTable[kind]` (and `kind[...]`, and
  a `u8 *` byte offset) loads the pointer first. Only the integer-first
  sum gives the index-first order. The prologue and the direct
  class-constructor calls use the pointer-first order, so those stay
  `gActorAnimTable[kind]`/`gActorAnimTable + kind`.
- **Return types from the epilogue.** `CreatePolarCheckpointText`/`SpawnPolarCollectedWumpa`/
  `ConstructAnimTableState` end in `pop {r0}; bx r0`, so they are
  `void`. The others end in `pop {r1}; bx r1` and return the object.
- **`SpawnActor`**: `u8 kind` rather than a wider type. A narrower type
  is passed to the factory without re-truncation, which keeps `kind` in
  r4 across the argument setup. The skip test is one
  `kind == 0 || kind == 32 || ... || kind == 62` chain. Written after a
  separate `kind == 0` test, gcc folds `32 || 33 || 34` into a range
  check. The ROM's jump over the 0 test after the fold to 1 is jump
  threading on the known constant.
- **`ConstructActorPart`**: `ACTOR_SET_STATE` for the z != 0 branch and
  an `idx` local for the other one. The shared tail (anim timer, done
  flag, time) then cross-jumps into one copy, as in the ROM.

Issue #51 stays open: `UpdatePolarPlayer`, `DrawPolarPlayer`, `AllocPolarPlayerTiles`,
`PolarPlayerStateMount`, `PolarPlayerStateJump`, `PolarPlayerStateDash` and `PolarPlayerStateCaught`
(`actor_part127.c`) are still NAKED.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(no warnings from the new file) and a full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`crashbandicootxs.gba: OK`).
