# Issue #9/#10 raw-asm pass

This pass covered the last three functions in the issue #9 range that were
still linked from `asm/*.s`, the two NAKED oscillators from issue #10, and
two NAKED holdouts from #9. It removes `asm/code_3_2.s`,
`asm/code_3_2_16_a884.s` and `asm/code_3_2_16_ac2c.s`.

## Closed (3)

| Function | File | Compiler | Technique |
|---|---|---|---|
| `PlayerHandleEvent` | `src/graphics/actor_part111.c` | old_agbcc | New plain-`switch` C, see below |
| `UpdateEnemyBob` | `src/graphics/actor_part116.c` | either | Empty asm clobber of r5, plus pins |
| `sub_800C97C` | `src/graphics/actor_part116.c` | either | Empty asm clobber of r8, one pin |

### PlayerHandleEvent (formerly raw)

This is the 38-case event dispatcher before `DrawPlayer`. It had been
left raw under the old "big dispatcher" policy. Written as a plain
`switch` with the cases in the ROM's block order, it was 122 halfwords off
under old_agbcc on the first compile, and three changes closed it:

- For the hit cases, the mode-0 branch goes in the `else`. The ROM lays it
  out after the mode-1/2 body and reaches it with `beq`.
- The ROM reloads `gLevelState->mode` after the listener call and
  never uses it. Only a volatile read reproduces that load (commented in
  the source). With it in place, `&gLevelState` lands in `sb`, as
  in the ROM.
- The star-burst position (`child->x >> 8`, `child->y >> 8`, mirror bit)
  goes through locals, so the `gEntitySpawner` pool load comes after
  them.

It matches only under old_agbcc (agbcc is 218 halfwords off), so
`actor_part111.o` is now on `OLD_AGBCC_OBJS`. The file's other function,
`DrawPlayer`, is NAKED, so the switch does not affect it. Before the
switch, `movs #1; ldrb; orrs` in the ROM (constant before the byte) had
already pointed to old_agbcc.

### UpdateEnemyBob / sub_800C97C

The note on these functions said the ROM pushes a callee-saved register
that it never uses (r5 in C940, r8 in C97C). The brief suggested
`-fprologue-bugfix`, so all four combinations were compiled: agbcc and
old_agbcc, each with and without the flag. All four produce identical
code, so the flag plays no part.

What reproduces the push is an empty `asm("" : : : "r5")` (or `"r8"`). It
marks the register live, so the prologue saves it, and it emits no code.
The remaining register choices:

- C940: `target` pinned to r3, and the `-0x100` bias created in r6 with
  the constant-init asm form (brief item 10), which also keeps it from
  being loaded early.
- C97C: `table` pinned to r6, which puts `target` in r5.

Each empty asm has a comment in the source.

## Moved to C as NAKED + draft (not matched)

- **`DrawAffineSpritePieces`** is in the new `src/graphics/graphics_7634.c`, which
  replaces `asm/code_3_2.o` in `ldscript.txt`. It is the affine sibling of
  `DrawSpritePieces`. The draft follows the ROM block for block, with bitfield
  OAM words and the affine-matrix slot allocation. It is 468 halfwords off
  (1032 bytes under agbcc and 1024 under old_agbcc, against the ROM's
  1044). The ROM spills nearly every local into a 0x48-byte frame, the
  same obstacle that parks `DrawSpritePieces`.
- **`CollidePlayer`**'s NAKED body moved into `src/graphics/actor_part78.c`,
  and `asm/code_3_2_16_a884.s` is gone. The old draft used register pins
  and two asm islands and was 137 halfwords off under both compilers,
  not the near match its comment described. It was replaced with plain C:
  real virtual calls through `self+0x18`, `sub_80084C4` inlined, and empty
  cases so the switch is built as a jump table. The new draft is 127
  halfwords off under old_agbcc, which this function needs (`movs #0x40`
  and `movs #8` come before their `ldrb`). What's left: the ROM builds the
  `+0x100`/`+0x102`/`+0x103` offsets by walking one register
  (`adds r1, #3`, `subs r2, #3`), while the draft gives each constant its
  own register. As a result gcc cross-jumps the kind-5 tail into the
  kind-7/10 tail, which the ROM keeps separate. A greedy search over
  statement orders, pointer locals and zero-constant forms (`raw9/g884.py`)
  gained only 3 halfwords.

## Tried, not converged (left as they were)

- **`DrawPlayer`**: the draft is now built under old_agbcc along with its
  file. It is 256 halfwords off (624 bytes against 636); `self` is in r6
  and the frame is 8 bytes where the ROM uses 4. It was not pursued past
  the triage.
- **`UpdateCrateList`**: 215 halfwords off under both compilers. Adding
  extra-reference nudges (`asm("" : : "r"(manager/node))`) at the loop
  head, the loop tail and after the outer loop brings the best case to
  210. `manager` and `node` still land in r8/sb instead of r7/r8. Brief
  item 9 (loop shape) would need the grid walk restructured, which this
  pass didn't have time for.

## Later pass: the four holdouts again (nothing closed)

A second pass retried `UpdateCrateList`, `DrawPlayer`, `CollidePlayer` and
`DrawAffineSpritePieces`. None closed. Two drafts got closer and were updated in
place; the NAKED bodies are unchanged.

- **`UpdateCrateList`: 215 → 7 halfwords** (old_agbcc; agbcc 35). The r8/sb
  problem was not about loop shape. It came from how the removal is
  inlined:
  - `pool_destroy(manager, obj)` wraps the search/compaction and the
    destroy call, and each of those gets its own copy of the pointer
    through a statement expression (`PART_COPY`). In testing, the
    inliner used a plain variable argument directly and gave a
    statement-expression argument its own copy. That gives the ROM's
    `mov sb, r5; mov ip, r5` pair, and with it `manager` goes to r7 and
    `node` to r8.
  - The first loop reads `node->data` twice (the flag test, then
    `part`), which gives the ROM's `ldr r2` and `adds r5, r2, #0`.
  - `next` declared at function scope puts it at `[sp, #0x14]` ahead of
    `gridHeadBase`. `base = pos[0]; base >>= 8;` reuses the `pos`
    register.
  - In the search, gcse copies `capacity` for the post-test, and cse2
    swaps the load and the copy when they are adjacent. That put the
    loaded value in the pre/post tests where the ROM uses the copy. An
    empty `asm("")` between the load and the pre-test keeps them apart
    (commented in the source).

  What's left: in the first loop's search, the `capacity` load and the
  `slotArray` copy get r3/r2 where the ROM has r2/r3. The second loop
  gets them right. Global-alloc priorities come out the same for both
  inlined copies, so no change inside the shared inline moves only the
  first. Nudges, padding asm and extra `do {} while (0)` depth all
  broke the second loop instead. The last diff is `gridHeadBase[i]`
  being built as `adds r0, r0, r2` rather than `adds r0, r2, r0`. A
  `(s32)gridHeadBase + (i << 2)` cast fixes that one, but it isn't in
  the draft.
- **`DrawPlayer`: 256 → 246.** The first ~40 instructions now match:
  - The mirror jitter is one assignment whose right-hand side is a
    statement expression. The store address then loads before the call,
    and `+ 2` isn't folded into the mirror term.
  - `SetPos` computes both sums before storing.
  - The `0x0300081C` update uses `+=` (the ROM stores the sum before
    clamping).

  What's left: `self` is in r6, not r7, and the history section spills
  twice. As a result the reload registers rotate differently, and gcc
  cross-jumps the shared `-0x1300` add of the two mirror branches.
- **`CollidePlayer`: unchanged (127).** The ROM's "walking" offset
  register is not a source-level pointer. gcc folds `self + 0x10x`
  into one add with a large constant, reload puts the constant in a
  reload register, and `reload_cse_move2add` turns the next constant
  load into `adds/subs #k` when the same reload register comes round
  again. Which register reload picks rotates from one reload to the
  next, so every reload before this point has to match. The first
  mismatch is already the `movs #0` index for the `+0x70` method's
  `ldrsh` (r2 in the ROM, r3 here). A pointer walk, bitfield
  `f100..f103`, and a value-first `one` local were tried. The
  value-first local does reproduce the ROM's `movs rX, #1` before the
  offset, but the registers still differ.
- **`DrawAffineSpritePieces`: not attempted past reading.** It is 1044 bytes, and
  the ROM spills nearly every local, including both parameters at
  `[sp, #8]`/`[sp, #0xc]`. Each value is stored as soon as it is
  produced and reloaded at every use, which looks like reload spilling
  rather than a declared frame struct. A frame-struct draft was not
  tried. Matching by spilling means reproducing global-alloc's choices
  across the whole function, which didn't fit the budget.

## Hold pass: `UpdateCrateList` closed, `DrawPlayer` 246 → 40

This pass applied the hard-register hold from #489
(`register s32 hold asm("rN"); asm("" : "=r"(hold)); ...
asm("" : : "r"(hold));`) to the two drafts above.

- **`UpdateCrateList`: matched** (old_agbcc; `actor_part11c.o` joined
  `OLD_AGBCC_OBJS`. It is the only function in that file, and it is
  still 28 halfwords off under agbcc).
  - The `(s32)gridHeadBase + (i << 2)` byte-offset form fixes the
    `adds r0, r2, r0` operand order (7 → 6).
  - The last 6 were the r2/r3 swap in the first loop's inlined search.
    Holding r2 from just before `if (i < manager->capacity)` until
    `base[i]` has been read makes `base` (the `slotArray` copy, still
    live there) conflict with r2. It goes to r3, and `capacity`, which
    is dead by then, takes r2 as in the ROM.
  - The hold is only in the first loop's copy. `pool_remove` takes a
    constant `holdR2` argument, which is 1 from the first loop and 0
    from the second, and wraps both asm statements in `if (holdR2)`.
    After inlining, the dead branch folds away, and the second loop's
    code is unchanged.
  - Holding r2 or r3 across the `capacity` load, or anywhere from the
    load to the found-test, either did nothing or also changed the
    second loop, as the earlier nudges did.
- **`DrawPlayer`: 246 → 40 halfwords, same size** (636 bytes). Not
  closed; the draft under `#if NON_MATCHING` was updated.
  - An r6 hold across the `DrawSprite` blink call puts `self` in r7.
    With `self` in r6, r7 had been the reload register.
  - A value-first history store (`s32 x = self->x;` before
    `self->hist[self->histIdx].x = x;`) matches the ROM's load order.
    `gUnknown_0300081C = gUnknown_0300081C + (u16)r - 1` gives its add
    order. `self+0xc` bit 3 is a bitfield, which gives the ROM's
    `movs #9; negs` mask instead of `#0xf7`.
  - With those changes, the single spill came from a second r6 hold
    around the tail's `frame` store and `RefreshChild` call. It keeps
    `&self->child` out of r6, so it goes to ip as in the ROM. That
    leaves r6 to `&histIdx` in global-alloc. Reload then evicts
    `&histIdx` to `[sp]`, and the hist bases go to sb/sl and `mode` to
    r8, which is the ROM's layout. The r6 reloads in the ROM are
    consistent with this.
  - What's left:
    - `&gRoomFrameCount` and `&gUnknown_0300081C` are in r5/r4 where
      the ROM has r4/r5. Every r4/r5 hold window in the mode block adds
      spills.
    - The orbit tail builds `idx * 8` and the x/y history addresses
      after the sine-table reads. The ROM builds them first, adding the
      offset into sb in place. None of 16 tail spellings tried
      (pointer locals, byte offsets, a `struct orbit_pos *`, `SetPos`
      argument orders) reproduced that.

## Later pass: `DrawPlayer` closed

`DrawPlayer` is real C now (old_agbcc, same 636 bytes). The orbit tail
passes its two sums straight in as arguments to a small inline setter:

```c
SetChildPos(self->child,
            self->hist[idx].x + gSineTable[gRoomFrameCount & 0xff] * 16,
            self->hist[idx].y + gSineTable[(gRoomFrameCount >> 1) & 0xff] * 8 - 0x1800);
```

gcc 2.x expands all of an inline call's arguments before it copies them
into the parameters. A sum comes back unforced, as
`(plus (mult ...) (mem ...))`, and is only forced at that copy. That
gives the ROM's order: `idx * 8` added into sb and the table reads
first, then the `child` load, the shifts, the history loads and the
adds. It also fixed the `&82C`/`&81C` r4/r5 swap. The two r6 holds from
the hold pass stay. See
[inline-arg-order-retry.md](inline-arg-order-retry.md).
