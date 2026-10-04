# GAX NAKED retry 5: `GAX2_init` and `GaxChannelMix` (issues #67/#68)

Fifth pass on the last two GAX2 functions, after
[gax-naked-retry-4.md](./gax-naked-retry-4.md).

| Function | File | Before | Now |
|---|---|---|---|
| `GAX2_init` | gax_playstart.c | 100 | **matched** (real C) |
| `GaxChannelMix` | gax_note_trigger.c | 43 | 43 (still NAKED) |

Scores are `brute2.py`'s sequence score, as in retries 3 and 4.
`GAX2_init` builds with plain agbcc and the normal flags. It was
verified with a clean `make compare`.

## `GAX2_init` (play start): what closed it

The work was done in this order. Each step used
`gax4/prio2.py`/`gorder.py` (global.c's priority list with final
registers) to find which pseudo took the wrong register and why.

1. **The `GaxCreateHandlers` arguments** (100 → 69). GCSE's PRE hoists the
   `p->layout`, `p->sfxTypes` and `&size` loads into both predecessors of
   the call block. Those are the two copies of the argument setup in the
   ROM. Global alloc then takes them in priority order. The ROM gives
   `p->layout` r4, which means it comes first, ahead of the
   post-call `&gGaxPlayerState` copy (8 refs over 188 insns). A local
   `l = p->layout` passed to the call, plus one no-code
   `asm("" : : "r"(l))`, raises it to the top. Then the whole group
   (layout/sfx/&size/loop copy of &g/a67c source/zero) lands on
   r4/sb/r6/r8/r5/r8, as in the ROM.
2. **A separate counter for the dspFn17c copy** (63 → 46). With one `k`
   for all four copy loops, `k` conflicts with the 380 offset constant
   that local-alloc puts in r2 in the fourth loop's preheader. So `k`
   loses r2 in the first three loops. The ROM uses r2 there and r3 in the
   fourth loop, so they are two variables.
3. **Indexed copies of the constant tables** (46 → 34). Write
   `dspCode48[k] = gGaxArmDownmix[k]`,
   `dspCode9c[k] = src[k]` with `src = gGaxArmEcho`, then
   `src = gGaxArmResample` right after that loop, and
   `field_44[k] = src[k]`. Drop the `a73c`/`a818`/`layout` locals and
   let the `field_1b` test read `p->layout` again. The ROM's setup order
   (`&g` into sl, `p->layout` into r4, 0803A73C into r6, 0803A818 into
   r8) is not source order. It is GCSE PRE inserting all four loads at
   the end of the first block, in expression-table order (first
   appearance in the function). 0803A818 is only anticipated there if
   `src = 0803A818` is unconditional, i.e. before the `words` test.
   loop.c then strength-reduces each index into the `ldmia` pointer
   (`adds r5, r6, #0` and similar in each preheader). Retries 3 and 4 tried every
   order of the setup statements, which can't produce this.
4. **The ALIGN4 after `field_1c`** (34 → 26). The aligned size (4 refs
   over 20 insns) lost r3 to the format pointer (3 refs over 7). One
   no-code `asm("" : : "r"(size))` after the ALIGN4 reverses them. Reload
   then can't inherit sl's copy from r3 and reloads it into r4, as the
   ROM does (`mov r4, sl`).
5. **The first tap scan's `layout`** (26 → 2 halfwords). The ROM loads
   `p->layout` into r0, loads `types[0]`, then copies to r4
   (`adds r4, r0, #0`) before the `data.dsp` load. That is a block-local
   `l = p->layout; t0 = l->types[0]; layout = l; tap = t0->...`.
6. **`layout` in r4** (2 → match). With the copy in place, global alloc
   gives `layout` r3, which is free between the scan and the `types[2]`
   test. The ROM leaves r3 unused there. A no-code hold on r3
   (`register u32 hold asm("r3")`, set right before `layout = l`, used
   right before the `types[2]` test) models that.

All three `asm` statements and the hold emit no code and are commented
in the source.

## `GaxChannelMix` (mixer): tried, no gain

The draft is unchanged. Findings for the next pass:

- **The tune's instrument copy.** The ROM's order is tune `ldrsh`,
  table load, `pitch + tune`, 0xEF3 load, `mov r8, r0`, `cmp`. This comes
  out instruction for instruction from a block-local
  `ip = self->instrument`, `t = ip->rows[row].tune`,
  `tab = gGaxPeriodTable`, `idx = pitch + t`, the constant through
  `asm("" : "=r"(m) : "0"(0xef3))`, an opaque copy
  `asm("" : "=r"(inst) : "0"(ip))`, then `if (idx > m) idx = m;
  period = tab[idx]` (scratch `gax5/b8.py`, variant `o|k|if|t`).
  - Why the pieces are needed: `MIN_EXPR` expands operand 1 before
    operand 0, and combine sinks the add into the target copy, so the
    `?:` form can't put the copy between the constant and the `cmp`.
    A plain `inst = ip` gets merged back into the load.
  - But `ip` then becomes a local that local-alloc puts in r2, and that
    pushes `pitch` to r3, the flag to sl and `info` to r5 (86). Pinning
    `ip` to r0 (`register ... asm("r0")`) gives the ROM's r0/r1 in the
    tune. The escaped row copy still takes r2, so `pitch` stays in r3 (78).
- **Why the ROM's row copy is in r3.** `mov r3, r9` fits a reload of the
  sb-held row with r0 (`ip`), r1 (the product) and r2 (`pitch`) busy.
  So the ROM's tune probably reads the global row pseudo directly, with
  no escape. Without the escape, CSE or GCSE merges the tune's `row * 28`
  with the ping-pong test's (`mov r8, r1` saves the product). An escape
  on the product instead of the row doesn't help either, because GCSE
  PRE still merges the shift/sub expressions. The ROM needs the two
  products to stay apart without a separate pseudo for the row.
- **Backward-end `sweepMin`** (ROM `ldr r3; lsls r0, r3, #11`). Holds on
  r0, r1, r0-r1, r0-r2, r1-r2 or r0/r2 around the load move the whole
  block's registers (49-75). Neither splitting the load and the shift
  with a label, `do { } while (0)` or `if (1)`, nor using an
  `"=r"`/`"0"` copy changed anything. On Thumb, update_equiv_regs never
  gives a low-register pseudo a MEM equivalence
  (`CLASS_LIKELY_SPILLED_P`), so rematerializing from memory isn't an
  option here.
- The copy might also come from GCSE PRE inserting a
  `self->instrument` load at the end of the tune block (before the
  `cmp`), with reload_cse turning `ldr r0, [r6, #60]` into a no-op
  because r0 already holds that word. That needs the ping-pong reads to
  be partially redundant at that point. None of the volatile-tune and
  non-volatile-ping-pong variants produced that insertion.

## Tools

Scratch area `gax5/` (not checked in): `p1`-`p9.py` are the
`GAX2_init` steps above, and `b1`-`b12.py` are the `GaxChannelMix`
attempts, all run with `gax3/brute2.py`/`sbs.py`. `prio2.py`/`gorder.py`
are copies that write to `gax5/rtl/`, and `locs.py` lists
local-allocated pseudos by block and register.
