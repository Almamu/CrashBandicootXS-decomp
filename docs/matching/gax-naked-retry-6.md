# GAX NAKED retry 6: `sub_8039B44` (issue #68)

Sixth pass on the per-channel mixer, after
[gax-naked-retry-5.md](./gax-naked-retry-5.md). **It closed.**
`sub_8039B44` is real C in `src/audio/gax_note_trigger.c`, built with
plain agbcc and the normal flags, and verified with a clean
`make compare`. It was the last GAX function and the last NAKED function
in issue #68's range.

| Function | File | Before | Now |
|---|---|---|---|
| `sub_8039B44` | gax_note_trigger.c | 43 | **matched** |

(Retry 3's `brute2.py` sequence score, ROM cut at `0x3EC`.)

## What closed it

Retry 5 left three gaps: the tune's instrument copy, the separate
`row * 28` products in the tune and the ping-pong test, and the
backward end's `sweepMin` register. The first two turned out to be one
problem.

1. **A two-armed tune clamp** (43 → 31). The ROM's tune reads the row
   from GCSE's sb copy with no escape (`mov r3, sb` is the reload for
   the high register). Without the escape, cse1 merges the tune's
   `self->instrument`/`self->row` reads and its `row * 28` with the
   ping-pong test's. The `.cse` dump shows why: the extended basic block
   starting at the tune follows 7 skip-blocks (clamp, envelope, 4
   volumes, `flag`) straight into the ping-pong test. Every one-armed
   `if`/`?:` is a skip-block, and jump1 turns a `?:` or an `if/else`
   with a simple arm into one before cse1 runs. The ROM's source must
   have had a join label there. Writing the clamp as

   ```c
   if (idx > m) {
       idx = m;
       asm("" : "+r"(idx));
       period = tab[idx];
   } else
       period = tab[idx];
   ```

   gives cse1 a label that ends the path. GCSE then replaces both row
   reads with copies of the sb pseudo, and the two products stay
   separate. Cross-jumping after reload merges the two identical
   `lsl/add/ldr` tails, so the arm shrinks to the ROM's
   `add r1, r2, #0`. The no-code escape keeps cse from rewriting the
   first arm's `tab[idx]` as `tab[m]`, which would stop the tails from
   matching. Retry 5's tune form (block-local `ip`, `t`, `tab`,
   `m = 0xef3`, `inst = ip` before the clamp) then gives the ROM's tune
   exactly, with `ip` in r0 and the `mov r8, r0` before the `cmp`. A
   plain `m = 0xef3` works here; the `"=r"`/`"0"` constant is no longer
   needed. The hold and pin ideas from the brief weren't needed.
2. **The backward end reuses `len`** (31 → 1 halfword, the trailing
   pad). `len = ...sweepMin; item.end = len << 11;`. As one pseudo with
   the wave length from the top of the function, `len` is global, and
   global-alloc gives it r3. The ROM does the same at both places: `ldr r3, [r4, #4]; ...
   lsl r0, r3, #11` for the initializer, `ldr r3, [r2]; lsl r0, r3, #11`
   at the backward end. A block-local value is tied to the shift's
   output by local-alloc and gets r0. None of `step`, `period`, `idx` or
   `vol` works as the shared variable.
3. **`asm(".align 2, 0")` after the function.** The ROM zero-fills the
   halfword after the final `bx r1`, where the assembler pads with a
   Thumb `nop`.

`sub_8039E50` (the `nop` the ARM call returns to) no longer has a label,
as with gax_unknownc_play.c's return points (see
[gax-toolchain-retry.md](./gax-toolchain-retry.md)'s reporting note).

## Tried, no gain

- Holds on r0/r1 around the backward `sweepMin` load, plus an extra use
  after the shift: to put the hold between the address and the load,
  the address has to go through a pointer local. That changes the
  address form (`[rX, #20]` folded) and scores worse (35-39).
- A `?:`, `if` or `if/else` with a constant or register arm for the
  envelope default: jump1 folds them before cse1, so they don't break
  the path.

## Tools

Scratch area `gax6/` (not checked in): variant specs `c1`-`c9.py`, run
with retry 3's `gax3/brute2.py` and `gax4/showall.py`. `prio2.py`
(copied, dumps to `gax6/rtl/`) was used to read the `.cse`, `.lreg` and
`.greg` dumps. `bindiff.py` compares raw bytes against the ROM with
relocations masked and branch targets kept.
