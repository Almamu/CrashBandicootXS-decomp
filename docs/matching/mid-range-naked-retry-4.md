# Fourth mid-range NAKED retry

This pass took `ConvertAirshipTiles`, the twin of the just-closed `ConvertHovercraftTiles`,
and six NAKED functions with `#if NON_MATCHING` drafts that were
42-300 halfwords off. It used the brute-force variant runner from
the third near-miss sweep (`brute2.py`).

One now matches as real C:

| Function | File | Compiler | Was | Technique |
|---|---|---|---|---|
| `ConvertAirshipTiles` | `src/graphics/actor_part26c.c` | both | 56 | the `ConvertHovercraftTiles` fixes, ported unchanged |

`actor_part26c.c` holds only this function and isn't on
`OLD_AGBCC_OBJS`. The C matches under both compilers, so no Makefile
change. With it, issue #58's range has no NAKED or raw functions left.

## What worked

**Porting the twin's fixes (`ConvertAirshipTiles`).** This is the four-row
version of `ConvertHovercraftTiles`'s meter builder. The draft already had the
`"+m"` reload of `heights[k]` from #470. The changes from
[near-miss-polish-3.md](near-miss-polish-3.md) went in unchanged:

- the 0xf mask from `asm("" : "=r"(m) : "0"(0xf))`, ANDed as `m & b`;
- a separate local `c` for the second byte;
- its own counter for the second loop;
- the row header written one statement at a time in ROM order (row
  pointer, height address, `d = dst`, `src = row + stride`, `n = *hp`).

It matched on the first compile.

## Improved but not closed

**`ActionCtrlStateLeftGround`** (#17, `action_ctrl_hang.c`, old_agbcc): 300
halfwords off before this pass (one extra instruction shifted
everything), 1 now.
Every instruction and register matches. The draft keeps these changes:

- Re-read `self->part->tag` in the `else if (... == 0x18)` test too.
  This removes the `tag` copy into r1.
- Give `fire` a fresh 1 from `asm("" : "=r"(fire) : "0"(1))`, with
  `pressed` read into a local first. Add one `asm("" : : "r"(one))`
  reference so that `one` stays in r6 and the input pointer in r7.
- Write `alt` as `t = 2; t &= p; alt = t;`. This gives the ROM's
  `movs r0, #2; ands r0, r1`.
- Use a `u8 z` zero, set right before the `unk_34` store through a
  byte pointer, and reuse it in the state-0xE else trio and the last
  branch's `next32`. Without it, gcc's jump equivalence uses the
  `& 0x30` result (known zero there) for those stores. The ROM uses r5.

What's left is one branch displacement. The ROM's `beq` for tag 0xD
jumps straight to the 0xD body, skipping the second `cmp r0, #13`,
which it keeps for the 0x18 path. That is jump threading, which in
gcc 2.x runs before cse1. So the two compares must use the same
pseudo, and cse1 must then still not fold the 0x18 path's re-test.
Here it goes one of two ways:

- With a re-read in the inner test, nothing is threaded.
- With `tag` in the inner test, both paths are folded and the function
  is 8 bytes short.

These didn't change it:

- about 60 spellings of the outer, inner and else tests (casts,
  `(tag = ...)` in the condition, `goto` into the body);
- a different type for `tag`;
- `-fno-cse-follow-jumps` and `-fno-cse-skip-blocks`.

## What didn't close

| Function | Was | Now | What was observed |
|---|---|---|---|
| `ApplyCrateCollision` (#12) | 49 | 49 | The ROM stores the first flag byte as a word and reloads it as a byte (`mov r5, sp; ldrb`) for the case-3 call. That needs a QImode use of a spilled SImode pseudo. These all still reload with `ldr`, or break the prologue: a `u8`/`u16` callee prototype, the flag as a `u32`/`s32` with a `(u8)` cast, a plain `u8` parameter (the prologue then loads a word and zero-extends it), the three flags in a stack array, and an `asm` copy (which gives `ldr; lsl; lsr`). |
| `PlayerAnimWouldTouchCrate` (#11) | 42 | 42 | The ROM re-adds `sp, #16` for each call of the player box's first build and only takes it into r6 at the overlap test. Here one pseudo holds it from the first build on. No change from: an `asm` copy for `pb`, per-call `asm` copies of `&f.b` (their inputs are still CSE'd into one pseudo), an inline builder, an inline address helper, separate locals or an array instead of the frame struct, a `"memory"` barrier between the calls, `-fno-gcse`, or cse flags. Reading `offX` through `rec` gets it to 39 but doesn't give the ROM's r1/r0 split, so the draft is unchanged. |
| `BreakCrateTouchedByPlayer` (#12) | 151 | 151 | The same `sp+16` player-box pattern as `PlayerAnimWouldTouchCrate`. Not tried separately after nothing moved `PlayerAnimWouldTouchCrate`. |
| `SpawnFlamethrowerLabAssistant` (#31) | 62 | 62 | The ROM keeps `arg3` in r4 and computes `arg3*2` in place, keeps `&gEntityFlags` in r9, and spills `part+0x28`. Here `arg3` gets r9. No change from: extra `"r"` references on `arg3` (0-3), the constant trick for the shared 1, local declaration order, a `u32` copy of `arg3`, or six spellings of the flipX toggle before or after `rec2`. |
| `UpdateDingodileShield` (#24) | 159 | 159 | Same instructions, different registers. The ROM leaves r6 unused and r5 only for `bld`, with `self`/`other`/`&gPlayer`/`&b` in r7/r9/sl/r8. The constant trick for `bld` (outside or inside case 0) gets it to 157 but makes the function 16 bytes short. The draft is unchanged. |

## Notes for next time

- `brute2.py` replaces the first occurrence of a pattern in the file.
  In files with several near-identical functions (the text-popup
  spawners in `graphics_loading_1feec.c`), a pattern can hit an earlier
  function. Replace the whole function text instead. This pass's specs
  do that with a `FNTEXT(src, signature)` helper.
- `brute2.py` drops branch targets from its disassembly by default, so
  a draft that differs only in a branch displacement shows as "1
  halfword" with no visible diff. `RAWB=1` keeps the targets, relative
  to the function start.
- If the ROM stores a known constant from a long-lived zero register
  where gcc uses a just-tested value, gcc has recorded a jump
  equivalence (`x & K` is 0 on that path). A named zero local that
  lives longer than the tested value wins the equivalence class back.
- The runners and specs are in the scratchpad `mid4/`: `b.sh SPEC
  [label]` runs `brute2.py` from the worktree, `fnbody.py` has
  `FNTEXT`, and `firstdiff.py` prints the first differing ROM bytes
  with their symbol.
