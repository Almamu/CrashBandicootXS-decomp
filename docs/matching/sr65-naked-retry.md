# #65 strength-reduction retry: `sub_803686C`

`sub_803686C` (the 20-slot OAM builder, the last NAKED function in
#65) is now real C under old_agbcc. The draft was 329 halfwords off.

| Function | File | Compiler | Result |
|---|---|---|---|
| `sub_803686C` | `src/graphics/graphics_loading_3686c.c` (new) | old_agbcc, strength reduction on | matched |

## File split

The function needs strength reduction on. Its header loop is
check_dbra_loop's reversed counter placed after the hoisted `&oamA`, and
its row pointer is a reduced giv. `sub_8036600` needs it off.
`graphics_loading_35d1c.c` was split at `0x0803686C`:

- `graphics_loading_35d1c.c` keeps `sub_8035D1C`..`sub_8036668`, still on
  `NO_STRENGTH_REDUCE_OBJS`.
- `graphics_loading_3686c.c` has `sub_803686C`..`sub_8036FBC`. It is on
  `OLD_AGBCC_OBJS` only. The shared declarations are copied.

The split was first checked with the old NAKED body: `make compare`
passed. `ldscript.txt` lists the new object right after the old one.

## What the row-copy loop needed

The ROM's loop:

```
ip = 0x80000050; r3 = r4 + 0x60; sl = 0x800; r6 = 7
loop: DMA(r1 -> r3); r1 += 0xa0; DMA(r1 -> r4 + sl); r1 += 0xa0
      r7 = 0x100; r4 += r7; r3 += r7; if (--r6 >= 0) goto loop
```

**Why `buf + 0x60` is a giv and `buf + 0x800` isn't.** loop.c
(`strength_reduce`) drops a giv when `lifetime * threshold * benefit <
insn_count`, where `benefit` is the giv's `rtx_cost` minus `add_cost`
for each biv increment. A single `(plus reg const)` costs 2, the same as
`add_cost`, so any `buf + K` off a `buf` biv has benefit 0 and is never
reduced. (An explicit `d = buf + 0x60` stepped by 0x100 is turned back
into a giv of `buf` by the "initial value is giv of biv" code, and then
has benefit 0 too. That is the "folds back" seen in earlier passes.)

What reduces it is the other rule: a giv of a biv that check_dbra_loop
reverses is always reduced (`bl->reversed`). So the destination is
written from the row counter:

```c
base = buf;
for (row = 0; row < 8; row++) {
    ... dma->dst = (u32)(base + row * 0x100 + 0x60); ...
    ... dma->dst = (u32)(buf + 0x800); ...
    buf += 0x100;
}
```

`buf` steps by a 0x100 that loop pass 1 leaves in the loop. That
register isn't invariant, so `buf` is not a biv at all and `buf + 0x800`
is not a giv. The reduced pointer's `+= 0x100` gets its own constant,
which cse2 merges with `buf`'s. So both adds use one register.

**Which constants move, and when.** The ROM's order
(`ip`, reduced-pointer init, `sl = 0x800`, counter) means 0x80000050
moved in loop pass 1 and 0x800 in pass 2. `move_movables` moves a
constant when `threshold * savings * lifetime >= insn_count`, and each
move lowers `threshold` by 3. Three bare `asm("")` in the loop body put
`insn_count` where pass 1 moves only 0x80000050 and pass 2 moves 0x800.
Pass 2 also moves the 0x100 into a pseudo. That pseudo goes to r7, and
reload spills it for `buf + sl` (the insn needs two low registers, and
r7's pseudo has the fewest refs). So it is rematerialized in the loop as
`movs r7, #0x80; lsls r7, #1`, and `reload_cse` turns the post-loop
`pa != 0x100` into `adds r0, r7, #0`. With 0 to 2 or 5 or more `asm("")`
the moves come out wrong.

## Register fixes

After the loop had the ROM's shape, the rest was allocation:

- **`tile` in r8, `self` in sb:** one `asm("" : : "r"(tile))` in the
  slot loop raises `tile`'s global-alloc priority above `self`'s.
- **Tail `slot` copy in r8:** `slot = SLOT_AT(self, 0)` is assigned after
  the flag/`PlaySfx` block, and the pre-loop fields are read through it.
  The `QueueVramDmaTransfer` arguments still use `self`, as in the ROM.
- **`dma` in r2:** `register struct dma_regs *dma asm("r2")`. Without
  the pin, local-alloc gives the `self + 0x430` address r2 first, and
  `dma` and the reduced pointer end up swapped (20 halfwords).
- **`affine` in r7:** it is a `u8` (an `s32` puts the result straight in
  the flag register, and then r7 no longer holds 0x100 at the compare). One
  `asm("" : : "r"(affine))` right after it is set lifts its priority
  above `matrix`'s. Without it, `affine` got sb. Once `affine`
  outranks `matrix`, `find_reg`'s pass 0 excludes sb (a register another
  conflicting allocno prefers), so `affine` takes r7. The nudge sits right
  after the assignment so it doesn't lengthen `affine`'s life. Placing it
  at the end of the block adds 4 bytes.
- **Slot-loop `*flag = 0`:** storing it from a `u8 z = 0` local puts the
  zero in r3, which moves the next reload (`matrix`) to r0, as in the ROM.
- **Tail matrix write:** `SetAffineZ` writes the two zero terms as
  literals, so the `movs r3, #0` comes after the entry address.
- **Second x:** `s32 x = slot->posA.h.i;` is read first, then
  `((slot->velA << 5) >> 16) + x - 0x40`. This gives the ROM's load order
  and operand order.

## Techniques worth reusing

- **To get a giv reduced whose benefit is only `add_cost`, derive it
  from a counter that check_dbra_loop reverses.** Reversed-biv givs are
  always reduced. Keep the other address off a biv so it isn't.
- **A pseudo in r7 with few refs is what reload spills.** A ROM constant
  that is re-materialized inside a loop, with the same register reused
  after it, points to a moved invariant that reload spilled.
- **`regs_someone_prefers`** comes only from *lower*-priority conflicting
  allocnos. A reference nudge that changes the priority order can move a
  register choice, even with no conflict involved.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
