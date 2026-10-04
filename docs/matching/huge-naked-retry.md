# Huge NAKED retry: `sub_0800D18C`

`sub_0800D18C` (issue #12, `src/system/game_loop47.c`) is the largest
NAKED function in the project, at 3840 bytes. Before this pass it had no
C draft. It still does not match, but it now has a draft that is the
ROM's exact size.

| Function | File | Issue | Size | Result |
|---|---|---|---|---|
| `sub_0800D18C` | `src/system/game_loop47.c` | #12 | 3840 bytes | still NAKED; first draft, size-exact under old_agbcc, 968 halfwords off |

Under the current agbcc the draft is 1629 halfwords off and 8 bytes
long. The ROM loads masks before `ldrb` (`movs r0, #0x7f; ldrb r2, [r1];
ands r0, r2`), which points to old_agbcc. `game_loop47.o` is not on
`OLD_AGBCC_OBJS` yet; it would have to be, along with `sub_800E08C`
(49 halfwords off under old_agbcc).

## What the function does

The draft names the fields through a local `struct d18c_player` view of
`gPlayer` and `struct crate`. In outline:

1. It builds `self`'s box and the player's box. The player's hitbox quad
   comes from the inlined `sub_8008518` switch. If the boxes overlap, it
   picks the object that was hit: `self`, or the neighbour
   `sub_800CF70` finds. It then reads a response code from
   `gCrateHitResponse[obj->kind][kind]`.
2. The code is cleared if `obj` is already in the player's 5-slot ring,
   or is chained to an object in it. It is then dispatched through a
   6-entry table to `ActivateNitroSwitchCrate`/`ActivateIronSwitchCrate`, the "busy" flag,
   `BreakCrateInStack` plus the ring push, `ExplodeCrate` or `OpenCheckpointCrate`.
3. The rest runs after that dispatch and after every early out. It
   rebuilds the player's box and works out which side the player touches:
   `edge` is 1, 2, 4 or 8. This uses two paths, one for `self->unk_44`
   set and one for it clear, and `sub_800FDC8` for slopes. It then
   corrects the position through a 9-entry table (edges 1/2, 4 and 8,
   with `GetBottomCrate`/`GetTopCrate` picking the neighbour for 4/8). It
   ends by queueing the result with `sub_8010D54`.

## What fixed the structure

The first draft, written straight from the disassembly, was 1530
halfwords off and 28 bytes short, with most blocks already right. From
there:

- **The edge `switch` needs explicit empty cases** (`case 0: case 3:
  case 5: case 6: case 7: break;`) to get the ROM's 9-entry jump table.
  With only 1/2/4/8 gcc builds a compare tree. The case bodies are in
  ROM order: 4, 8, then 1/2.
- **One function-scope `pp`** for the four position structs. With a
  separate `pp` in each block, alias analysis knew the store could not
  touch `gPlayer` and dropped the reload the ROM has after
  `pp->y = ...`.
- **`obj` and the final target (`tgt`) are separate variables**, and the
  hitbox quad of the first part (`hb`) is separate from the rebuilt
  player quad (`q`). In the ROM they sit in different registers.
- **`D18C_Span(a, b, c)` inline for the overlap depths** (`dx = Span(b.x,
  b.w, c.x) + 1`). The ROM loads all three fields before the add and the
  subtract. Only argument evaluation of an inline gives that order; the
  plain expression interleaves the loads. It also lets jump2
  cross-jump the two branches' tails, as in the ROM.
- **`D18C_Hit(p, bit)` and `D18C_SetBusy(p, v)` inlines.** The ROM loads
  the player pointer, then the constant, then the field (`ldr r2, [r5];
  movs r1, #1; ldr r0, [r2, #0x74]; orrs r0, r1`). That is argument
  evaluation order again.
- **The timer test is an inline returning the comparison.** This gives
  the ROM's `movs r3, #0; ...; movs r3, #1; cmp r3, #0` flag.
- **The dead `ldr r1, [sp, #0x70]`** after `sub_8009EC4` in the
  `unk_44` path is a copy of the other path's `side = 2; if (px > ax)
  side = 1;`, whose result is never used. Flow deletes the sets, and
  jump2 deletes the compare only after reload has loaded px.
- **`goto edge_x`** for the three "too far" rejects (`ay == py && ax == px
  && dy > 2`, `ay > a.y + a.h`, `ay < a.y`). The label and its
  `edge = dirX` come after the if-chain and before `code = 0`, so the
  shared block is the ROM's `161`. As plain `else edge = dirX` arms,
  jump.c hoisted the assignment above the compare.
- Smaller fixes: an `s32` local for the signed `bounce > 4` test, nested
  `if`s for the empty-hitbox test (`w == 0 && h == 0` became one `ldrh`),
  `y = P->y` read before `pp = &f.p1`, `(d << 8) + v` operand order, and
  a `u8 *` local for the `player+0x108` queue's committed byte, so that
  `+4` stays in the `strb`.

## Register allocation

The draft still gave `self` r9 and `px` sl. In the ROM `self` is in sl
and `px` is on the stack. The greg dump (`-dg`, "Registers to be
allocated in sorted order") showed the cause. `ay` (the
`sub_8009EBC` result) is just below the player hitbox pointer `q` in
priority (0.2115 against 0.225). `q` therefore took r7, `ay` fell to
ip, and everything after it shifted up by one register.

One `asm("" : : "r"(ay))` nudge after `side` is computed puts `ay` ahead
of `q` (brief item 8). With it the ROM's cascade appears: `ay` r7, `q`
r8, `dy` sb, `self` sl, `px` on the stack, and the `&obj->kind` and
case-3 `&gPlayer` GCSE temps in sb. The `goto` change then
made the draft exactly 3840 bytes.

## What is left (968 halfwords)

- **`&f.b` is held in a register.** cse merges the box pointer of the
  two builder calls (`SetAabbPos`/`SetAabbSize`) into one pseudo. That
  pseudo lives across the first call and gets r6 or r4. The ROM
  recomputes `add r0, sp, #0x3c` before each call and only holds the
  pointer (in r4) from `sub_8001688` to `sub_800CF70`. In the rebuild
  this pushes w/h into r5/r6 instead of r4/r5. The
  `&gPlayer` GCSE temp then lands in sb instead of r6, which
  adds four `mov rX, sb` instructions. Things that did not stop the
  merge: a frame struct, separate locals, a pointer local, an inline
  builder, `do { } while (0)` around the calls, and a cast spelling.
- The `kind * 4` and `&self->state` GCSE temps take stack slots in the
  opposite order to the ROM (0x94/0x98).
- The position structs `p2`, `p3` and `pos` are set up in the ROM
  through a temporary and then copied into `pp` (`add r0, sp, #N; str
  r1, [r0, #4]; adds r2, r0, #0`). A struct copy, `(pp = &p)->y` and an
  inline initializer did not reproduce it.
- A few `edge = dirX` copies are not cross-jumped into the ROM's `160`,
  because their reload registers differ.

## Tools

The helpers are in the scratch area `huge/`. They were not committed.

- `bf.py`: a parallel variant runner that uses a fork pool. It reports a
  disassembly diff score with and without registers.
- `asmdiff.py`: an instruction-level diff of the draft's `.s` against the
  NAKED text. It drops pools, normalizes branch targets, and can mask
  registers and spill slots. It prints a size delta for each hunk.
- `alloc.py`: the global-alloc order from the `-dl`/`-dg` dumps, with
  priority, refs, live length, calls crossed and the final register.
  This is what found the `ay`/`q` tie.
- `seg.py`: a per-call-segment size diff.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from
  `game_loop47.c`.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.

## Later pass (stack-box NAKED retry)

The `&f.b` blocker is fixed for the first player box. Its builder calls
take the address through an empty `asm("" : "+r")` copy (`BOX_ADDR`),
and a `bb` local holds it (also from `BOX_ADDR`) from `sub_8001688` to
`sub_800CF70`. That block now matches the ROM, including `r4`, and the
draft is 938 halfwords off at the exact size. The same fix on the
rebuilt box matches that block too, and puts the `&gPlayer`
temp in r6 as in the ROM. But other low registers then shift and the
draft comes out 8 bytes short, so the draft doesn't use it there yet.
See [sp-box-retry.md](sp-box-retry.md).

## Later pass (second huge-NAKED retry)

938 to 33 halfwords, still NAKED. See
[huge-naked-retry-2.md](huge-naked-retry-2.md).
