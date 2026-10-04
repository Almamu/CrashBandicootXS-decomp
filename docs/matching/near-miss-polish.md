# Near-miss polish pass

This pass went back to eleven parked functions whose C drafts were already
within a few instructions of the ROM. Three of them now match as real C:

| Function | File | Compiler | Was | Technique |
|---|---|---|---|---|
| `PlaySfx` (`sub_8001854`) | `src/audio/sfx_ambient.c` | agbcc | raw asm, 4 halfwords | unpin `self`, then one empty `asm("" : : "r"(&gSfxVoiceToggle))` |
| `ValidateSaveData` | `src/graphics/settings_menu8e.c` | agbcc | NAKED, 6 halfwords | one empty `asm("" : : "r"(flags))` |
| `LevelSelectLoop` | `src/graphics/actor_part_1b85c.c` | old_agbcc | NAKED, 1 instruction | statement expression plus an empty `asm("" : "+r"(k.all))` |

`asm/code_3_1_10.s`, which held only `PlaySfx`, has been deleted along
with its `ldscript.txt` line.

## New technique: an extra reference to settle global-alloc ties

Both `PlaySfx` and `ValidateSaveData` had the ROM's exact instruction stream,
but some registers were swapped or rotated. gcc 2.x's global allocator
handles pseudos in priority order,
`floor_log2(n_refs) * n_refs / live_length`, and gives each one the first
free register. In `ValidateSaveData`, the ROM computes `&flags` before
`&field_1fb`. That makes `flags` live one insn longer, so it ranks lower
and gets r8 where the ROM has r7. `asm("" : : "r"(x))` produces no
code, but it adds a reference to `x` and raises its priority:

- `ValidateSaveData`: one reference to `flags` lifts it above `field_1fb`. It
  works anywhere between its computation and its store, under both
  compilers.
- `PlaySfx`: the old draft pinned `self` to r9. The pin made the save of
  `self` an ordinary body statement, placed after the parameter copies
  of `id`/`volumeParam` (the 4-halfword prologue gap). Without the pin
  the stream matches except that `self`/`id`/`&gSfxVoiceToggle` rotate
  through r8/r9/sl. One reference to `&gSfxVoiceToggle`, anywhere
  after the first `GAX_fx_ex` call, puts it first as in the ROM.
  Two references overshoot.

Try this whenever a draft is instruction-identical to the ROM but has a
register permutation among callee-saved or high registers. Brute-force
the variable and the count; the right answer is usually one reference.

## `LevelSelectLoop`: a copy the ROM makes between `ands` and `cmp`

Before the 0x80 test's `cmp`, the ROM copies the key word
(`adds r1, r2, #0`) and later tests 0x20 on the copy. A statement
expression can put code between computing a condition and testing it:

```c
else if (({
             u32 hit = keys.half.pressed & 0x80;

             k = keys;
             asm("" : "+r"(k.all));
             hit;
         }))
```

The empty `asm` stops gcc from merging `k` back into `keys`. Without it,
the same statement expression still loses the copy. A plain comma
expression is not enough either.

## Not closed

| Function | Left | What was tried |
|---|---|---|
| `sub_8014084` | `adds r2,#40` after the -0x11 load instead of before it | pointer-first forms, bitfield stores, `m`/`r` barriers and statement expressions. Every form that computes the address first puts it in a fresh register (`adds r0,r2,#0; adds r0,#40`), one instruction longer. The ROM's order fits reload's older input-address-first emission order, with both the byte and the -17 reloaded in one `and` insn, but no source form reproduced it. |
| `sub_801C608` | ROM spills `info` to sp+0 and reloads it once | computing `info` via `+`, a table pointer or an index local; re-reading `gLevelTable[...]` per use (all 16 subsets); nested if; extra `asm` references on rank/sv/self/info. The size stays 860 vs 868. |
| `sub_8014B54` | 0x600 reloads into r2, not r3 | 11 spellings of the add and store, inline wrappers, a declaration swap. An r3 pin plus `asm("" : "+r"(d))` gets r3, but it then shifts the next two reloads (r6/r1 become r2/r6). |
| `sub_800450C` | loop pre-header order (0x80 hoisted before the pointer copies) | passing 0x80 through a variable in any position fixes the order but changes which invariants the loop pass hoists (11 halfwords) |
| `PauseMenuLoop` | fade pointer computed after `disp` | fade local at top, before or after `disp`, in either or both loops; for/break/goto loop forms; late `disp`. `asm("" : : "r"(FADE(self)))` before `disp` gets the order right but computes the pointer into r0 and copies it to r4 afterwards. A fade local used in the second loop only gets the order but is 8 bytes short. |
| `sub_8001CB8` | `hash >> 8` folded into `(x << 16) >> 24`; -16 as `mov; neg` instead of `sub r0,#31` | u32/s32 hash with casts or masks, temporaries, `+r`/`r` barriers, manual nibble insert (7 halfwords, but 0xF0 replaces the -16) |
| `RunRoom` | `add rN, sp, #4` should come before the direction-byte constant | struct spellings: packed, plain, array, s8, bitfield, nested; locals; compound literals. Only the packed one-byte struct keeps `strb`. |
| `sub_80360DC` | seedBase/counter/zero and stride/slot register rotation | 0-2 extra `asm` references on each of the five variables, before the loop or inside it (3^5 combinations): the gap never dropped below 18 halfwords |

Helpers: the brute-force variant runner (`brute2.py`, adapted from the
scratchpad's `mix12b/`), specs `s*.py`.
