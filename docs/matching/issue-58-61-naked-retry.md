# Retrying issues #58 and #61's NAKED functions

Issue #58 (`0x08030334`-`0x08031784`, the boss-weapon cluster, one
function per `src/graphics/actor_part2*.c` file) and issue #61
(`0x08032890`-`0x08033804`, the `gHovercraft` singleton system in
`src/bosses/hovercraft.c`) had 19 functions parked as NAKED. The
first passes ([issue-58-0x08030574-actor.md](issue-58-0x08030574-actor.md),
[issue-60-61-gap-31a6c-part2.md](issue-60-61-gap-31a6c-part2.md))
blamed most of them on "many live high registers" or on a size gap
found only at link time. This pass closed 13 of the 19. None needed a
register pin.

The two clusters are twins, and most fixes carried straight across:
`UpdateAirship` <-> `UpdateHovercraft`, `LoadAirshipGraphics` <-> `LoadHovercraftGraphics`,
`CreateAirship` <-> `CreateHovercraft`, `SpawnAirship` <-> `SpawnHovercraft`,
`SteerAirship` <-> `HovercraftStateCloseIn`, `DrawAirshipMap` <-> `DrawHovercraftMap`,
`ConvertAirshipTiles` <-> `ConvertHovercraftTiles`.

**Compiler:** every closed function builds the same under agbcc and
old_agbcc. The files stay on current agbcc, and no file was split.

## Closed (13 functions)

| Function | What it took |
|---|---|
| `AirshipStateFireballs` (`airship_states.c`) | The tracker's state transition (`gAirshipState = st; gAirshipStateTimer = 0; ...anim reset`) is a `static inline BossSetState(st, idx)`. With the state as a parameter, the constant is materialized right before its store. The first pass's "RHS address first" gap came from writing the store inline. |
| `AirshipStateCannon` (`airship_states.c`) | The distances are written `a - (b - K)`. gcc's `fold` turns that into `(a + K) - b`, which is the ROM's order. Written as `(a + K) - b` directly, the compiler shares `b - K` with the spawn call's arguments instead. Also a branchless `Abs()`, and `/` going to the libcall `__divsi3`. |
| `AirshipStateExplode` (`airship_explode.c`) | The box table is `const s16[]`, so its jitter ranges stay in registers across the calls. The palette base is assigned right where the ROM loads it; declared with an initializer, it is hoisted into a callee-saved register. The RNG `RandRange` is read back as `u16`. `&gActorList` is taken before the last lock check. |
| `CreateAirship` (`airship.c`) | An inlined C++ `gAirship = new Tracker(...)`. The destination's address is taken before the allocation. `mem_alloc` goes through an `AllocActor(size)` inline, so the size is loaded before the heap flags. The part-table setup is an inline constructor that takes its values as arguments, so all three are loaded before the stores. Then `BossSetState(0, 0)`. |
| `SpawnAirship` (`airship.c`) | The zoom divide is an explicit `__divsi3(...)` call, not `/`. The libcall is treated as not clobbering memory, but the ROM reloads `gAirshipDistance` after it. The record lookup is `level * 28 - -(s32)&table[kind]`. Written as `a + b`, `fold` pulls the constant table base out of `&table[kind]`. The ROM adds the level offset to the finished record address. |
| `UpdateAirship` (`airship.c`) | The same explicit divide. The re-blit tail reads the tracker through a fresh local, a separate pseudo from the head's `self`. Otherwise it is the usual anim advance written with `anims[animIndex]` indexing. |
| `LoadAirshipGraphics` (`airship_load_graphics.c`) | No pins; the first pass abandoned an `r7` pin. The filler-tile clear walks an `s32` address downward, which gives the signed `bge`, with its zero hoisted into a local. The fill and copy are the stock `DmaFill16`/`DmaCopy16` macros. The palette is `vu16`, and the state-5 blackout is a chained assignment `pal[15] = pal[1] = pal[4] = pal[8] = 0`, whose volatile read-backs are the ROM's `ldrh`/`strh` ladder. |
| `HovercraftStateCloseIn` (`hovercraft.c`) | Twin of `SteerAirship`. The player and camera reads are separate locals (`px = pl->x; cx = cam - 0x1200; px - cx - ...`) so `fold` can't reassociate them. The X velocity updates with `-=` and the Y velocity as `v = g - ...`. The orbit's destination and table pointers are taken before the angle is computed. |
| `HovercraftStateFallBack` (`hovercraft.c`) | Straight C on the first try, with a shared `SingletonSetKind(kind, idx)` inline. |
| `CreateHovercraft` (`hovercraft.c`) | Twin of `CreateAirship`, same recipe. The "two zero registers" come from the `SingletonSetKind` inline. `gHovercraftLevel` is a level index (`s32`), not an owner pointer. |
| `SpawnHovercraft` (`hovercraft.c`) | Twin of `SpawnAirship`, same recipe (explicit divide, `- -` record lookup). |
| `UpdateHovercraft` (`hovercraft.c`) | Twin of `UpdateAirship`. The earlier "4 extra bytes" was the tail reusing `self`. |
| `LoadHovercraftGraphics` (`hovercraft.c`) | Twin of `LoadAirshipGraphics`. The earlier "24 bytes short" was the clear loop, not the DMA macros. |

## Still NAKED (6), each with a `#if NON_MATCHING` draft

| Function | What's left |
|---|---|
| `DrawAirshipMap` (`airship_map.c`), `DrawHovercraftMap` (`hovercraft.c`) | 11 halfwords off under old_agbcc (35 under agbcc). The row setup and loops are right. The next-row pointer and the hoisted `&bias` copy get `r3`/`ip` swapped. No loop form, index-vs-pointer store, `register` hint or bias-access form moved it. |
| `SteerAirship` (`airship.c`) | 19 halfwords off under both compilers, all of it the `&gAirshipVelX`/`&gAirshipVelY` copies landing in `r4`/`r6` swapped. Every instruction is otherwise right. The fixes that make the rest match are the `px`/`cx` split, a `goto` form for the Y nudge, and a `ClampHi`-style pointer for the first clamp store. |
| `IsTouchingAirship` (`airship_touch.c`) | About 50 halfwords off. The shape is right (inline `BoxOffset`, a struct-returning `SelfBox`, the self-`MemCopy32` copy), but `&c` gets hoisted into `r4` before the box copy and costs an extra `r6` push. |
| `ConvertAirshipTiles` (`airship_graphics.c`), `ConvertHovercraftTiles` (`hovercraft.c`) | About 95-105 halfwords off. These are the 4-row and 1-row versions of the same meter builder, so a shared inline is likely. The ROM re-reads each height from the stack after the row-pointer store (`ldm r1!`), which suggests the store can alias the height array. It also allocates the nibble-expansion temporaries differently. |

### Later pass: four more closed

[actor-zone-naked-retry.md](actor-zone-naked-retry.md) closed
`DrawAirshipMap`, `DrawHovercraftMap`, `SteerAirship` and `IsTouchingAirship`:

- The blit twins read the bias as `u8 bias = <s32 global>;` and declare
  `row` before `i`, which breaks the global-alloc tie between `row + 0x20`
  and `i + 1` the ROM's way. `airship_map.c` and `hovercraft.c`
  moved to old_agbcc.
- `SteerAirship` stores the X step once and the Y step once per branch.
  The reference counts that global-alloc sees then give the Y velocity
  copy r4. It matches under both compilers.
- `IsTouchingAirship` uses the frame-struct box layout from `IsTouchingYeti`
  (old_agbcc; `airship_touch.c` moved).

`ConvertAirshipTiles` and `ConvertHovercraftTiles` are still NAKED, so issues #58 and #61
stay open.

Later pass ([late-rom-naked-retry.md](late-rom-naked-retry.md)): the
height re-read is reproduced (a `"+m"` asm on `heights[k]` after the
row-pointer store), and so is the `dst` r3/sb split (a copy `d` for the
inner loop). The drafts are now 56 and 29 halfwords off. What's left is
mostly the nibble AND: the ROM copies the hoisted 0xf and ANDs the byte
into it, gcc copies the byte.

## Techniques worth reusing

- `fold` reassociation cuts both ways. `a - (b - K)` becomes
  `(a + K) - b`, and `a + &table[i]` has its constant base pulled out.
  Use a split into separate statements, a `- -` form, or a reorder to
  get the ROM's grouping.
- An explicit call to the ROM's divide routine, rather than `/`, is
  needed when the ROM reloads a global after the divide.
- Wrap state transitions and constructors in inlines that take their
  constants as parameters. This fixes the order in which constants are
  materialized.
- Where a function has a twin, crack whichever is easier and port it.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list.
