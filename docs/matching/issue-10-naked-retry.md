# Issue #10: NAKED retry (0x0800B3F0-0x0800CA60)

This pass retried the 13 NAKED functions in issue #10's range, the
`sub_800B8DC` controller cluster in `src/graphics/actor_part112.c`-
`actor_part122.c`. 6 are now real C.

The controller (`self`) and the part it steers (`self+0x70`, "owner"
in the older docs) now have a shared layout in the new
`include/part_ctrl.h`: `struct part_ctrl` and `struct ctrl_target`.

## Compiler

As next door (issue #9, `0x08007Bxx`-`0x0800AFxx`), this code was built
with old_agbcc. Three of the closed functions only match under it
(`sub_800C074`, `sub_800C40C`, `sub_800C6A8`), so `actor_part118.o`,
`actor_part120.o` and `actor_part122.o` moved to `OLD_AGBCC_OBJS`. The
other three match under both compilers. Every other function in the
three moved files still matches: `sub_800C5D4` is NAKED, and
`actor_part122.c`'s `sub_800C860`/`sub_800C87C`/`sub_800C898` are
plain C now. Under the current compiler they had needed a register pin
and an `asm volatile` barrier each. `sub_800C9C8` (`actor_part116.c`)
also matches under old_agbcc, but that file didn't need to move.

## Closed (6)

| Function | File | Compiler | Technique |
|---|---|---|---|
| `sub_800C18C` | actor_part114.c | both | Each of the five cases does its own `{a, b, a}` store triple through block-scoped `a`/`b` (`SET_VEL`). Cross-jumping then merges four of them into the shared tail and leaves the `0, 0x10` moves duplicated, as in the ROM. The old "this agbcc cross-jumps where the ROM didn't" note had it backwards: it was a source-shape problem. |
| `sub_800C1E8` | actor_part114.c | both | Same as `sub_800C18C`, Y axis. |
| `sub_800C314` | actor_part115.c | both | The bit toggle reads the bit into a local first (`u32 m = bit; bit = !m;`). That gives the ROM's order: load, `lsl #27` test, then the 0/1, shift and `-0x11` merge. |
| `sub_800C074` | actor_part118.c | old | The position gate `(m && x < lo) \|\| (!m && x > hi)` re-tests the mirror bit in the ROM (`cmp r3, #0; blt`) instead of being jump-threaded. The two tests must differ in RTL until after threading: the first reads the bit through an unsigned 1-bit field, the second through a signed one. The `mirror` union in `struct ctrl_target` provides both views. Combine turns both into the same sign test of one shared `lsl #27` afterwards. The toggle is `sub_800C314`'s. |
| `sub_800C40C` | actor_part120.c | old | The spawn call is an inline copy of `sub_800C9C8`. Passing the arguments through inline parameters materializes them in the ROM's order (2 first, -0x2d last). The `+0xC`/`+0xD` flag writes are u8 bitfield stores, which gives the QImode `-0x41`/`-9` masks. The ROM passes `owner->timer` (known 0) as the extra stack argument, so the call has 7 arguments. |
| `sub_800C6A8` | actor_part122.c | old | The inlined `sub_800C8AC`/`8BC`/`8CC` are `static inline` helpers. The case groups the ROM keeps apart ({1,3,17} and {6,9,10,11}) are separate cases. Reload picks their `ldrsh` scratch register round-robin in insn order, so they come out different and don't merge. For the same reason the {4,14,16} keyframe tail has to be written in both branches (cross-jumping merges them again): with one shared tail the else branch's scratch is r4 instead of r1. State 5's stores go through inline setters, which puts the shared 0 in r2 first. A trailing `asm(".align 2, 0")` gives the ROM's zero padding before `sub_800C860`. |

## Not closed (7)

| Function | State |
|---|---|
| `sub_800BD48` (actor_part112.c) | old_agbcc draft under `NON_MATCHING`, 21 hw off. Almost all of it is reload scratch registers: the ROM cycles r3, r3, r3, r4, r6, r2, ... where the draft gets r6, r4, r4, r6, r2, .... Reload picks them round-robin from an order that depends on the whole function's register use. The only real code gap is in states 1/21/22: the ROM loads the layer's `1` before the `-4` mask. A variable for it gets the order right but leaves a dead `movs`. What got it this close: `MarkGone` as an inline, the spawn as `SpawnAt(kind, x, y)` so x/y are evaluated before the pool, the velocity triples as inline setters, and `(a = t->x) > P->x` so `t->x` loads first. |
| `sub_800B8DC` (actor_part112.c) | Not converged, no draft kept (about 285 of 566 hw off). The switch needs explicit `case 1: case 12:` to get the ROM's `state - 1` table. The 5th argument of `sub_80019F8` is a packed one-byte struct (the `strb` to the stack slot, as in actor_part128.c). The ROM keeps `self` in r5 and uses r4/r6 as scratch in several states, which again looks like reload round-robin. |
| `sub_800C244` (actor_part119.c) | Draft under `NON_MATCHING`, 7 hw off under both compilers. After the two calls the ROM keeps the target in r2 and `baseY` in r1, where the draft uses r1/r0. That shifts the mode-1 toggle and the tick/timer test by one register. Locals, statement order and if/else in place of the switches made no difference. |
| `sub_800C5D4` (actor_part120.c) | Draft under `NON_MATCHING`, 5 hw off. Only the `kind == 0xB` prelude is wrong: the ROM has target r1 / `baseY` r2 and the draft has them swapped. The global-alloc dump shows the two pseudos' priorities (3 refs over 5 vs 6 insns) decide it. Everything else matches: the box built from `x = target->x >> 8` locals, and the knockback stores with the `-0x200` constant assigned mid-sequence. |
| `sub_800C8F8`/`sub_800C940`/`sub_800C97C` (actor_part116.c) | Drafts under `NON_MATCHING`, 13/10/27 hw off, the same under both compilers. The phase bias has to go through an inline parameter (`Wave`) to keep the ROM's `phase + 0xFFFFFF00` literal rather than a folded `+ 0x100`. `sub_800C8F8`: the ROM multiplies into a fresh register (`mov r2, r1; mul r2, r0`). `sub_800C940`/`sub_800C97C`: the ROM saves a callee-saved register it never uses (r5 via a 4-register push, r8) and swaps target/table. No extra pseudo tried (constant bias variable, field pointers, call inside the inline's arguments) reproduced that. |

## Tools

The per-function compare (`cmp.py`, both compilers), side-by-side
disassembly diff (`dis.py`) and template-variant runner (`var.py`) used
for this pass were scratch copies of `triage_naked.py`. Nothing is
committed. old_agbcc's `-dl`/`-dg` dumps were what showed the
round-robin reload registers (reload insns have high insn numbers and
set hard registers directly) and the global-alloc priorities.

`rm -rf build && make NON_MATCHING=1 report` and a clean `make compare`
both pass.

## Later pass (issue #9-#11 NAKED retry)

`sub_800C244`, `sub_800C5D4` and `sub_800C8F8` are real C now. `sub_800B8DC`, `sub_800BD48`, `sub_800C940` and `sub_800C97C` are still NAKED. See [issue-9-11-box-naked-retry.md](issue-9-11-box-naked-retry.md).
