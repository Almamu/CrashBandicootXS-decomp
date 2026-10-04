# Category driver NAKED retry (issues #48, #49)

Two large NAKED functions from the actor-category code, neither with a
kept C draft. Both closed as real C under old_agbcc.

| Function | File | Issue | Compiler | Result |
| --- | --- | --- | --- | --- |
| `InitActorCategory` | `actor_part101.c` | #48 | old_agbcc (object added to `OLD_AGBCC_OBJS`) | matched |
| `RunActorCategoryFrame` | `actor_part103.c` | #49 | old_agbcc (file already on it) | matched |

## `RunActorCategoryFrame`

The first full draft came out 150 halfwords off with an extra `r9`,
the same result as the old note. The cause was not register pressure:

- The sub-effect loop is a plain `while (cond) body;`. gcc's jump pass
  (`duplicate_loop_exit_test`) copies the exit test ahead of the loop,
  so the ROM has the test twice and the body starts at a label. CSE
  cannot carry the test's loads into the body, which is why the ROM
  re-reads `gActorSpawnIndex`/`gUnknown_03001420`/the vtable in the
  body.
- The copy is refused if the test contains block notes, so the test
  cannot call an inline function (the `NextThreshold` helper that
  `SelectActorCategory`'s draft uses). It is a macro instead.
- The "next record" address is built in `sub_802A51C`'s order (`off =
  idx * 0x14`, then `base + 0x14`, then the sum) through two locals in
  a comma expression.
- A `do { } while` with the same test lays the `&&` halves out rotated,
  and a hand-written goto loop loses the loop notes (no hoisting). Only
  the `while` form matches.

## `InitActorCategory`

A first structurally faithful draft (switch on the exit state, DMA
through a `struct dma_regs *` local) was 293 halfwords off, but with
most instructions already right. What closed it:

1. **Exit-state tests as an if/else chain.** A 4-case `switch` builds a
   balanced compare tree (`== 1` first). The ROM tests 0, 1, 2, 3 in
   order with the 0 case at the end: `if (status != 0) { chain } else {
   option screen; continue; } break;`. Ending the option-screen branch
   in `continue` keeps the inner loop from being rotated.
2. **The `RunPauseMenu()` result tests stay ifs** (a `switch` there also
   builds a tree).
3. **`-sub_802A5AC() < 0`** gives the ROM's `neg; lsr #31`; `!= 0`
   adds an `orr`.
4. **`(gKeys >> 16) & 8`** (u32 global) loads with
   `ldrh [rX, #2]` from the same literal as the later `& 4` word test.
   `((u16 *)&g)[1]` gets its own `g+2` literal.
5. **Pointer locals for gUnknown_03001384/gUnknown_03001388** (19 -> 4
   halfwords). They are spilled, so every use rematerializes the
   address through a reload register. That is where the ROM's odd
   choices come from (`r3`/`r7` in the prologue stores, `r5` in the
   `unknown_28` test, `r0`/`r1`/`r3`/`r5` in the four spilled `ret`
   stores). With plain globals the reload rotation is one step behind,
   and jump2 then cross-jumps the two identical `ret = 1; goto done`
   blocks together. The pointers must be assigned at the top of the
   outer loop; initialized at declaration they do not match.
6. **`state = &gLevelState` right before the inner loop** (4 -> 0).
   Loop.c's first pass hoists `gLevelState`, the category base and
   the palette constant; `gUnknown_03001300` only gets hoisted by the
   second loop pass, so it landed after the base copy. Assigning the
   pointer as a statement before the loop puts it ahead of the movables,
   which gives the ROM's preheader order.
7. `zero` (the DMA fill source) is `vu16`, as in the `DmaFill16` idiom,
   so its address is taken before the `strh`.
8. Trailing `asm(".align 2, 0")` for the 2-byte pad after `bx r1`.

The brute-force runners (`brute2.py` with small spec files) found items
3-6. Declaration order made no difference at all.

