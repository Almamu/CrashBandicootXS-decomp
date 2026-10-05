# Last-five NAKED retry

This pass retried four parked NAKED functions that each had a C draft
under `#if NON_MATCHING`. Two closed and two did not.

| Function | Issue | Before (old_agbcc) | Result |
|---|---|---|---|
| `ApplyCrateCollision` (`game_loop47.c`) | #12 | 49 hw | **matched**, old_agbcc |
| `sub_801AB98` (`actor_part_1ab98.c`) | #25 | 565 hw, 8 bytes short | **matched**, old_agbcc |
| `QueueCratePlayerCollision` (`game_loop47.c`) | #12 | 938 hw | still NAKED |
| `DrawVvLogoPieces` (`graphics_loading_35d1c.c`) | #65 | 329 hw | still NAKED |

`actor_part_1ab98.o` and `game_loop47.o` joined `OLD_AGBCC_OBJS`. Both
address ranges are inside the span already confirmed to be old_agbcc
code. `QueueCratePlayerCollision` is NAKED, so the compiler change doesn't affect it.

## ApplyCrateCollision: a one-byte struct argument

The ROM stores the first stack flag byte as a word (`ldrb r0, [r0]; str
r0, [sp]`). In case 3 it reloads the byte with `mov r5, sp; ldrb r2,
[r5]`, in argument order. The draft had `u8 f20` and reloaded it as a
word.

Thumb `ldrb` needs a MEM operand. `zero_extendqisi2` expands a register
operand into shifts, so a `(u8)` cast on a register never gives `ldrb`.
The address reload (`mov r5, sp`) shows the MEM only appears at reload
time: a spilled SImode pseudo read through a QImode subreg in a `movqi`.
That happens when the argument itself is QImode. A one-byte struct
argument is not promoted, so it is passed in QImode:

```c
extern void sub_800E7A8_flag(struct crate *self, u32 a, struct flag8 b, u32 c) asm("BreakCrateInStack");

union { u32 w; struct flag8 s; } f20;   /* in a register: SImode */
f20.w = p20.value;                       /* ldrb + str (word) */
...
sub_800E7A8_flag(self, 0, f20.s, edge);  /* (subreg:QI f20) -> mov r5, sp; ldrb r2, [r5] */
```

The union has to be assigned before the other two flags (all three are
declared first, then assigned in order) so the prologue's stores match.
Other attempts, each worse:
- An addressable union (`asm("" : : "m"(f20))`) gives `ldrb` from `[sp]`,
  but the address is computed early into r0.
- A plain struct local is QImode and spills with `strb`.

## sub_801AB98: several independent fixes

Each step was checked with the variant runner (`last5/var.py` specs in
the scratchpad). In order:

1. **r8 hold** (brief item on hard-register holds). The draft had `self`
   in r8 and `result` in sb, the ROM the reverse. `register s32 hold8
   asm("r8")`, defined at entry and used just before the first
   `AabbOverlaps`, keeps r8 busy while `self` is live. So `self` takes
   sb, and `result`, born after the hold, gets r8. (565 -> 550 hw.)
2. **`px` as the FindLineCrossing argument.** The ROM computes the third
   argument into r5 in both arms (`adds r5, r2, r0` / `adds r5, r2,
   #0`) and moves it to r2 before the call. That is the draft's `px`
   (pinned to r5), reassigned in each arm: `px = b.x + b.w; r =
   FindLineCrossing(tx, ty, px, py, a.x)`. The pin had to go. (-> 348.)
3. **One `r` for both classify blocks.** A function-level `r` makes the
   first block's call result take r2, as in the ROM. (-> 340.)
4. **`pb = &b` hidden from cse** with `asm("" : "+r"(pb))`. The ROM
   keeps `&b` in r4 from the first box build into the no-overlap
   switch.
5. **Inline sign-mask abs** (`sign = d >> 31; d ^= sign; d -= sign;`)
   instead of the `ABS32` do/while macro in the no-overlap switch. The
   do/while changed the local allocation (brief item 3).
6. **`flags = hdir` in case 1/2.** The ROM reloads `hdir` into r5 (the
   flag register) in that case, so the draft's always-zero `flags` was
   wrong. `flags` is the commit's hit flag, as `hit = dirX` is in
   `QueueCratePlayerCollision`.
7. **No `pp` pointer.** The ROM's `&pos` register is a gcse copy inserted
   after the `&gPlayer` copy at the end of the block. A `pp =
   &pos` statement always puts its copy first. The fix is to write `pos`
   directly and do every later read or write of `pos.y` through
   `PosPtr(&pos)->y` (an identity inline). (-> 5 hw in the last step.)
8. **Vtable call as an inline through the method pointer**
   (`Call68`), not the `OBJ_CALL68` macro with its r4-pinned `_fn`.
   Every call site had to change so they still cross-jump into one
   shared call. (243 -> 68.)
9. **`u8 m = 8; q->hitAxes = m;`** (as `QueueCratePlayerCollision` writes
   `hitAxes`). It gives the ROM's `movs r1, #8` before the address and the
   `subs r0, #68` reuse of the `+0xAC` address. (-> 12.)
10. **`y = gPlayer->y; ... = y - ((oy - 1) << 8)`** and
    **`(oy << 8) + pos.y`** / **`(ox << 8) + pos.x`** operand order, as
    in `QueueCratePlayerCollision`.
11. **r5 hold over the `result == 0` test** of the second classify block,
    so the reload of `result` there takes r0.

Holds tried and not needed: the draft's `register ... asm("r0")` and
`asm("r3")` pins (removed), the constant-init form for `flags`.

## QueueCratePlayerCollision: not closed

Findings, both kept as a note on the draft:
- Writing the `dy > 2 || (dx <= 3 && sub_800B324(...))` arm as `goto
  edge_x` gives the ROM's block order in the edge classifier.
- BOX_ADDR on the rebuilt player box matches that block.

Together they bring the register-blind instruction diff from 98 to 57,
but the function comes out 4 bytes long, so the draft was left as it
was. What's left is spread across the function:
- the `kind * 4` spill slot;
- the `f.c = f.a` copy registers;
- the p1/p2/p3/pos pointer copies (the `PosPtr` fix from `sub_801AB98`
  alone didn't help);
- the placement of the hit-flag `orr`/`str`.

## DrawVvLogoPieces: not closed

The row-copy loop still doesn't reduce `buf + 0x60` to its own giv.

- An explicit second pointer `d = buf + 0x60` stepped by 0x100 compiles
  exactly like the draft: loop.c folds it back into `buf`.
- Counting down (`row = 7; row >= 0`) changes nothing structural.
- Holds on sl/r9/r8 over the header block move a few registers (329 ->
  325 hw).

Closing it still needs strength reduction on, and so a file split (see
the draft's note and docs/matching/late-naked-retry-3.md).
