# GAX2 NAKED retry 2: toolchain probe, then source (issues #66/#67/#68)

Second retry of the four GAX2 functions still parked after
[gax-toolchain-retry.md](./gax-toolchain-retry.md) and
[late-rom-naked-retry.md](./late-rom-naked-retry.md). Each had a C draft
under `#if NON_MATCHING` with the ROM's control flow, and register and
spill-slot assignment differed throughout. **2 of 4 closed as real C**,
both under current agbcc with the normal flags.

| Function | File | Before | Now |
|---|---|---|---|
| `GAX2_estimate` | gax_work_size.c | 228 hw off | **real C** |
| `GaxCreateHandlers` | gax_channel_table_alloc.c | 289 hw off | **real C** |
| `GAX2_init` | gax_playstart.c | 422 hw off (seq 364) | NAKED, better draft (seq 294) |
| `GaxChannelMix` | gax_note_trigger.c | 438 hw off | NAKED, draft unchanged |

"hw off" is `triage_naked.py`'s position-by-position count. "seq" counts
both sides after a sequence alignment of the halfwords. The seq count
isn't thrown off when an early size difference shifts the rest of the
function, so it is the better progress measure for drafts whose size
still differs.

## Step 1: toolchain probe - negative

Idea: GAX2 is a prebuilt third-party object, so it might have been
built with other flags. Method: compile every `src/audio/gax_*.c` with
`-DNON_MATCHING=1` under each configuration. Count how many of the 50
GAX functions that match today stay byte-exact, and measure how far the
four drafts are from the ROM. Selected rows (full list: the scratch
probe output, not checked in):

| configuration | matched kept | 7FC0 | 8240 | 8538 | 9B44 |
|---|---|---|---|---|---|
| **agbcc `-O2 -mthumb-interwork -fprologue-bugfix`** (project default) | **50/50** | 228 | 289 | 422 | 438 |
| agbcc, no `-fprologue-bugfix` | 42/50 | 228 | 289 | 422 | 438 |
| agbcc, no `-mthumb-interwork` | 25/50 | 227 | 287 | 420 | 436 |
| agbcc `-O1` | 24/50 | 221 | 298 | 586 | 436 |
| agbcc `-O3` | 43/50 | 228 | 289 | 422 | 438 |
| old_agbcc `-O2` (no pbf) | 38/50 | 241 | 289 | 449 | 462 |
| old_agbcc `-O1` / `-O3` | 24/50 / 32/50 | worse | | | |
| `-fno-strength-reduce` | 47/50 | 239 | 287 | 400 | 438 |
| `-fno-expensive-optimizations` | 30/50 | 227 | 289 | 391 | 437 |
| `-fno-cse-follow-jumps` | 45/50 | 228 | 289 | 422 | 428 |
| `-freduce-all-givs` | 42/50 | 206 | 299 | 471 | 438 |
| `-fmove-all-movables` | 46/50 | 220 | 280 | 422 | 440 |
| `-fno-gcse` | 43/50 | 251 | 306 | 429 | 445 |
| `-ffixed-r8`/`r9`/`r10`/`ip`, `-fcall-used-r8` | 42-49/50 | 229-250 | 298-319 | 422-577 | 433-436 |
| `-fno-strict-aliasing` | 49/50 | 228 | 304 | 415 | 451 |
| `-Os` / `-O0` | 35/50 / 8/50 | | | | |
| `-fomit-frame-pointer`, `-f[no-]caller-saves`, `-fno-defer-pop`, `-fno-function-cse`, `-fno-thread-jumps`, `-fno-force-addr`, `-fno-peephole`, `-fshort-enums`, `-fno-common`, `-fno-builtin`, `-fargument-noalias`, `-fkeep-inline-functions` | 50/50 | identical to the default | | | |

Pairs of the "improving" flags (`-fno-strength-reduce` +
`-fno-expensive-optimizations`, and so on) also do worse on the matched
set than the default. Neither compiler accepts `-fno-schedule-insns`,
`-fschedule-insns`, `-mapcs-frame`/`-mtpcs-frame`,
`-mcallee/caller-super-interworking`, `-fno-if-conversion` or
`-f[un]signed-char`.

**Result:** only the project's default configuration keeps every
matched GAX function exact. The flags that leave them exact produce the
same draft code as the default. The flags that move a draft do so by at
most ~10%, and each breaks 3-25 matched neighbours. No configuration
comes close to "clearly closes or dramatically improves". GAX2 was built
with the same agbcc and flags as the game code, so the Makefile is
unchanged. The drafts were wrong in the source, not in the toolchain.
Both closures below confirm it: both are plain `-O2` agbcc. For
`GaxCreateHandlers` the flag sweep was repeated on the 2-halfword near-miss
(`-fmove-all-movables`, `-fno-strength-reduce`, `-fno-rerun-loop-opt`,
`-freduce-all-givs`, `-fno-gcse`, `-fno-rerun-cse-after-loop`). Every
flag took it back above 200 halfwords.

## Step 2: what closed them

The techniques below came from reading gcc 2.95's RTL dumps (`-dg -dl
-dG -dL`) and from a nested-template brute-force runner in the scratch
area. In `global.c`, the order of allocnos, their conflict lists and
their priorities (`floor_log2(refs) * refs / live_length`) explain most
of the register choices.

1. **`__udivsi3` is the ROM's own lib1funcs routine, so divide with `/`.** With
   a plain `/` (the libcall resolves to it), the division is
   a libcall. gcc treats a libcall as a const call, which does not
   clobber memory. An explicit `__udivsi3(a, b)` call does clobber
   it. That difference decides what GCSE may carry across the division.
   In `GaxCreateHandlers`, the spilled `gGaxPlayerState` address then
   reaches into the DSP-rate loop, and reload rematerializes it into r1
   (`ldr r1, =gGaxPlayerState`). This was the last 2 halfwords.
   `GAX2_estimate` needs it too.
2. **Re-read fields instead of caching them in locals** (the #463 GCSE
   hint also holds for current agbcc). In `GAX2_estimate`, `p->layout`
   and `p->flags` are read at every use. GCSE's reaching registers
   become the ROM's spilled copies (`[sp, #0x10]` for the layout and a
   *halfword* `strh`/`ldrh [sp, #0x18]` for the flags, because the
   reaching register is HImode). Loop invariant motion stores them in
   the carving loop's preheader. The same GCSE copies keep the carving
   loop re-reading `layout->count` without strength reduction. In
   `GaxCreateHandlers`, the linking loop compares against
   `t->childTypes[j]` directly (the ROM loads it twice). In
   `GAX2_estimate`'s second alternative-layout scan, the condition reads
   `types[2]` itself, and the list pointer is read again inside. The ROM
   copies it (`adds r3, r0, #0`) after the null test.
3. **Index-first addressing** (`*(i + p->layout->types)`,
   `*(k + list->layouts)`, `(i + taps)->rate`, `i * 8 + base`)
   reproduces the ROM's `adds rX, rIdx, rBase` operand order. The
   `(i + taps)->rate` form also gives `ldr [rX, #4]` addressing where
   `taps[i].rate` produces `adds #4` first.
4. **Priority and conflict engineering for the allocator:**
   - `GaxCreateHandlers`: `cnt = t->childCount; if (i == 0) cnt += numSfx;
     n = cnt * 4;` (a separate local) puts `n` in r8.
     `need = n + (sizeof(struct GaxHandler) + t->instanceSize)` puts
     `need` in ip and keeps the ROM's `(n + 12) + inst` order.
   - `GAX2_estimate`: one counter `i` shared by the carving loop, the
     alternative layouts' inner loop and both tap scans. That makes `i`
     conflict with the inner loop's walking pointer, which is allocated
     first and takes r3, so `i` lands in r4 as in the ROM.
   - `next = k + 1` in a block-scoped `next` (both functions) numbers
     the pseudo so that its stack slot or register matches.
5. **GCSE's hash table size** (`GAX2_estimate`, the last 12 halfwords).
   GCSE creates its reaching registers in hash-bucket order. Their
   pseudo numbers set the order of their stack slots. The table size
   comes from the function's insn count. Everything else matched, but
   the `p->layout` / `rate << 5` / `p->flags` copies came out in slots
   0x18/0x10/0x14 instead of 0x10/0x14/0x18. Any 4-11 extra RTL insns
   give the ROM's order. The source now has four `asm("")` statements
   (no code) at the top, with a comment explaining them. The original
   source evidently had a few more RTL insns that later passes removed.
   Natural candidates tried without success: callee and local types,
   the `continue` form, re-reads in conditions, split statements and
   `sizeof` spellings. Flagged for review: this is a compiler-internals
   nudge like the `asm("" : : "r"(x))` ones, and a natural source form
   should replace it if one turns up.

## What's left

- **`GAX2_init`** (play start): the r8/r9 swap of `maxRate`/`fmt`
  that the earlier retry described is fixed in the draft. The fix is a
  no-code `asm("" : : "r"(maxRate))` inside the first tap scan, where
  refs are loop-weighted. The draft also has `/` for the echo length, an
  s32 copy counter, and the tap and alternative-layout scans in
  `GAX2_estimate`'s matched shape. Remaining differences, each local:
  - the ALIGN4 after `field_1c` (the new size lands in r3 in the ROM);
  - the first tap scan's `layout` copy (`adds r4, r0, #0` after the
    `types[0]` load);
  - the order of the constants hoisted before the ARM-code copy loops.
    The ROM materializes the `gGaxPlayerState` address into sl before
    `layout = p->layout`, and loads `types[1]` late;
  - the tail (`flags & 2` and the mixer-DSP test).
  The draft is ~294 halfwords off by the alignment-insensitive count.
- **`GaxChannelMix`** (per-channel mixer): only probed briefly this pass;
  the draft is unchanged. The ROM puts `self`/`info`/`flag` in r6/r4/r5
  and `row` in sb, spills the wave pointer, and holds `(s32)period` in a
  DImode r4:r5 pair across the `gGaxMixRateReciprocal` loads. Changing the
  row/period types and the wave/row statement order had no effect. The
  next retry should start from the tricks above: `/`-style libcalls,
  re-read fields, and loop-counter sharing.

Helpers (probe, nested-template brute-force runner with linked choices,
priority dumper `prio.py`) are in the scratch area (`gax/r2/`) and are
not checked in.
