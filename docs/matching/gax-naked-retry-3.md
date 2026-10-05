# GAX NAKED retry 3: `GaxChannelMix` and `GAX2_init` (issues #67/#68)

Third pass on the last two GAX2 functions, after
[gax-naked-retry-2.md](./gax-naked-retry-2.md) and
[mix-naked-retry-5.md](./mix-naked-retry-5.md). **Neither closed.** Both
stay NAKED, and both `#if NON_MATCHING` drafts are much closer now.

| Function | File | Before | Now |
|---|---|---|---|
| `GaxChannelMix` | gax_note_trigger.c | 202 | **46** |
| `GAX2_init` | gax_playstart.c | 123 | 100 |

Both columns use `brute2.py`'s sequence score (difflib over normalized
disassembly, so branch targets and literal pools count). For
`GaxChannelMix` the ROM side is cut at the real size `0x3EC`, not at the
`sub_8039E50` map label. Retry 2 gave "~294" for `GAX2_init`, but
that was a different count. Both numbers in this table come from the
same tool.

Everything below is plain agbcc with the normal flags. No draft needed
old_agbcc or per-file flags.

## `GaxChannelMix` (per-channel mixer): what moved it

1. **The 64-bit product in a DImode user variable** (202 → 135):

   ```c
   s64 prod = (s32)period;
   prod = prod * gGaxMixRateReciprocal >> 32;
   step = prod;
   ```

   The ROM keeps `(s32)period` in r4:r5 and passes it to `__muldi3`
   from there. `prod` is one pseudo, set both before and after the
   libcall. While it holds the operand, the libcall setup loads the
   global's address and value into r0-r3, so global.c can't give it
   r0-r3. It is also first in global's priority order (a DImode
   pseudo's priority is doubled: `size` is 2 in `allocno_compare`), so it
   takes r4:r5 before anything else. That removes r4/r5 from `self`'s
   choices. `self` then gets r6, `info` r4, `flag` r5 and `vol` r7,
   which is the parameter swap the earlier retries described.
   `((s64)(s32)period * (s64)g) >> 32` and every form that keeps the
   operand in a libcall temporary put the operand straight into r0:r1,
   and `self` ends up in r5.
2. **The row copy after the empty-wave test** (135 → 118). With
   `row = self->row;` placed after `if (wave->data == NULL) return 0;`,
   the waveIdx lookup reads `self->row` itself, and the ping-pong test
   reads `self->row` again, GCSE's copy of the row lands in sb exactly
   where the ROM's `mov sb, r2` is.
3. **Work-item fields read before the initializer** (118 → 105). `u32
   frames = self->format->frames; u8 *data = wave->data; s32 pos =
   self->samplePos;`, in that order, reproduce the ROM's three loads
   ahead of the stores into the initializer temporary.
4. **A "memory" clobber on `GAX_CALL_ARM_R`** (105 → 65). The ARM routine
   writes the work item. Without the clobber, GCSE keeps
   `self->format` and `item.done` in registers across the loop
   (PRE copies at the loop tail), where the ROM reloads them.
5. **`?:` for the tune clamp and the envelope default** (65 → 59).
   `gGaxPeriodTable[idx > 0xef3 ? 0xef3 : idx]` loads the table
   address before the add, as the ROM does. `vol = envOut != 0xff ?
   envOut : 0x100` loads `envOut` before the 0x100.
6. **The call's argument in a register before the routine** (59 → 55),
   and **one variable walking state → routine** (48 → 46). Both are in
   the `GAX_CALL_ARM_R` macro (lib/gax/src/gax_internal.h), which now takes the
   player state. The ROM's `mov r4, sp` comes first. It then does
   `ldr r3, [r3]; ldr r3, [r3, #0x44]`, which needs one pseudo for both
   loads. The asm inputs conflict with the clobbered r0-r2, so that
   pseudo gets r3.
7. **`volatile` work item** (55 → 48). The ROM re-reads `item.done` at
   every use: the loop test, the post-call test, and twice in the
   end-of-sample clear.

### What's left in `GaxChannelMix` (all register or reload choice)

- **The tune's row.** The ROM copies sb into r3 right at the tune
  (`mov r3, sb`). The draft copies it at the top of the pitch block
  because `row = self->row` sits there. Moving the read to the tune
  (`self->row` in the index, or the assignment next to it) makes
  the tune's and the ping-pong test's `row * 28` CSE'd instead. The
  ROM computes it twice, so the tune and the ping-pong test must be in
  different cse1 paths. With the ROM's branch structure they are not.
  From the tune's join label to the ping-pong test there are only 7
  skippable branches (PATHLENGTH is 10). None of these changed it: if/else
  instead of `?:`, `do { } while (0)` wrappers, and inline helpers for
  the pitch, volume or product. An inline product helper does split the
  path, but it breaks other things.
- **Reloads into r3.** The sweep-length initializer re-reads
  `self->instrument`/`self->row` (the draft reuses GCSE's r8 copy). The
  backward end loads `sweepMin` into r3 before shifting into r0 (the
  draft uses r0 for both). Both look like the ROM leaving a pseudo
  unallocated and rematerializing it from its REG_EQUIV memory. That
  suggests more register pressure in the ROM's loop than in the draft.
  Casts, `* 2048`, locals, and `inst`/`row` spellings didn't change them.
- The rest of the diff is literal-pool placement that follows from the
  above.

## `GAX2_init` (play start)

- **Tail fixed** (123 → 100). `if ((u16)(flags & 2)) { if (dsp[1]) ... =
  1; else ... = 0; } else ... = 0;` keeps the ROM's `lsl/lsr #16` test,
  and both zero stores use the register that was just tested. The `&&`
  form folds the u16 cast away.
- **Left:**
  - The ARM-code copy setup. The ROM materializes the
    `gGaxPlayerState` address (into sl) before `layout`, `a73c` and
    `a818`, and `a818` goes to r8. All 5! orders of the setup statements
    were tried, crossed with indexed or pointer copies: 96-98 at best.
    The ROM's order looks like loop-invariant hoisting out of a
    differently shaped first loop.
  - The ALIGN4 after `field_1c`: reload puts the new size in r4 where
    the ROM uses r3 (reload rotation).
  - The `GaxCreateHandlers` argument registers (`layout`/`sfx`/`&size` in
    r4/sb/r6 in the ROM, r9/r8/r4 in the draft), and the zero kept in r8.

## Tools

Scratch area `gax3/` (not checked in): `brute2.py` with a `DUMP=` option
that writes the variant source, `prio2.py` (RTL dumps for one function,
including cse2), `gorder.py` (global.c's sorted allocno list with final
registers), `sbs.py` (side-by-side ROM/draft), and the variant specs
`b44v*.py`/`p538v*.py`.
