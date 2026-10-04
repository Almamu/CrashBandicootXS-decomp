# Issues #15/#16/#17: second NAKED retry (0x08010F8C-0x08014BCC)

This pass retried the 12 NAKED functions left in issues #15, #16 and #17
after the first retry (docs/matching/issue-15-16-naked-retry.md) and the
issue #17 passes (docs/matching/issue-17-0x08012fbc-actor.md). 3 are now
real C.

## Closed (3)

| Function | File | Compiler | Technique |
|---|---|---|---|
| `sub_801434C` | actor_part18.c | old | Plain C on `struct act` (`include/action_obj.h`). The case 0/2 trio goes through a local `ActQueue27(self, 0, 0)` inline, so its 0 is materialized before the stores. In case 1 the trio is written out in both branches and the method calls use the do/while `ACT_VCALL*` form, so gcc cross-jumps the shared `bl` of the second call as the ROM does. With the if/else `ACT_CALL*` form there the whole block changes. The whole file matches under old_agbcc, so `actor_part18.o` moved to `OLD_AGBCC_OBJS`. |
| `sub_80145E4` | actor_part18b.c | both | Plain C. The old gap was the masked bit going into a scratch register and then being copied to the register `flag` lives in. It goes away when the assignment is inside the test: `if ((flag = gKeys & 0x100) != 0)`. No compiler change. |
| `sub_8012694` | actor_part84.c | old | The tag tests read `self->part->tag` / `self->part->frame` each time instead of going through a `part` local. old_agbcc's GCSE turns the reloads into the ROM's copy of the pointer in r2, and recomputes the +0x2D address for each test. `pressed & 1` is folded into the assignment (`pressed = INPUT_PRESSED(in) & 1; one = 1; if (pressed)`), which puts the AND's constant after `one` into a fresh register, as in the ROM. |

## Not closed (9)

| Function | State |
|---|---|
| `sub_8014084` (actor_part_13c60.c) | Draft improved to one instruction off. The facing block now reads `self->part` for every access, so GCSE produces the ROM's `adds r2, r0, #0` copy, and spells its two bit tests differently (`(s32)(x << 27) < 0` / `(s8)(x << 3) >= 0`). Otherwise gcc threads the second test into the first and drops the ROM's re-test. The body of the second branch is written `m = -0x11; m &= self->part->flags28; ...`, which gives the ROM's registers but emits the -0x11 before the in-place `adds r2, #40`. Writing the pointer first moves it to r0 instead. |
| `sub_8014674` (actor_part_14674.c) | Draft restructured: `self->part` everywhere, a differently spelled inner tag test (keeps the ROM's re-test), parameter inlines for the trios whose constant is materialized first, and `fire = 1; fire &= pressed` with an explicit `one`. That removed the r8 spill and most of the gap (about 50 differing lines in the disassembly diff, down from about 190). Still off: a copy of the tag in r1, register choice in the state-0xE `& 0x30` test and its else trio (the ROM uses r5, gcc the known-zero AND result), and the `fire` AND operand order. |
| `sub_8014B54` (actor_part_14674.c) | Unchanged: 3 halfwords. The 0x600 constant is a reload (insn `set r0 (plus r0 1536)`), and reload gives it r2 where the ROM has r3. Tried: alternative spellings of the add, inline helpers, do/while vs if/else call macros, explicit trio stores, and a preceding dummy function (reload rotation doesn't carry across functions). A register pin plus barrier gets r3 but shifts every later reload. |
| `sub_8012AF4` (actor_part83.c) | Not reworked. The ROM computes `&self->flag2F` in both state paths, keeps the loaded flag in `ip`, keeps the +0x27 slot in r8 and loads the record-table address late. The draft's layout differs in several places. |
| `sub_8012420` (actor_part84.c) | Not reworked. Besides the PMF stack slot, the draft reads `part->y` after the camera value (the ROM reads it first), treats `unk_94` as unsigned (the ROM uses a signed `bgt`), and the timer block's registers differ. |
| `sub_801173C` (game_loop53.c) | Unchanged. The ROM puts `id`/`special` in r8/sb and `mode` in r7. Its +0x4B store reuses the clamp's zero and +0x49 gets a fresh `movs`. The draft has these the other way round. |
| `sub_8010F8C` (game_loop54.c) | Unchanged. Mode 1 in the ROM recomputes `x + velX` from the two loaded registers for the bound check. That suggests the source compares an expression CSE did not merge with the store. Not pursued. |
| `sub_8011548` (game_loop53.c) | Not attempted: no draft, and it shares `sub_8010F8C`'s mode blocks. |
| `sub_8011BD4` (actor_part82.c) | Not attempted: no draft, 1420 bytes with nested jump tables. |

## Techniques worth reusing

- **Read `self->part` each time instead of caching it in a local.** When
  the ROM loads a pointer once, copies it (`adds r2, r0, #0`) and then
  uses the copy across several blocks, the source likely re-read the
  field. old_agbcc's GCSE (`-fgcse` is on at -O2; `-fno-gcse` removes the
  copy) replaces the later loads with that copy. A `part` local gives one
  pseudo and no copy. This closed `sub_8012694` and fixed most of
  `sub_8014084`/`sub_8014674`.
- **Spell a repeated test differently to keep the ROM's re-test.** When
  the ROM re-evaluates a condition that gcc could thread (`(s32)(x << 27)
  < 0` and later `(s8)(x << 3) >= 0` on the same byte), writing it the
  same way both times lets jump threading drop the second test.
- **`x = expr & 1; one = 1; if (x)`** puts the AND's constant in a fresh
  register after `one`. Testing `expr & 1` directly reuses `one`.
- **Cross-jumping depends on the call macro form.** The do/while
  `ACT_VCALL*` form let gcc merge the identical `bl` tails of two
  branches. The if/else `ACT_CALL*` form did not.

## Tools

Scratch helpers (not committed): a per-function disassembly diff
against the ROM under either compiler, with extra cc1 flags from the
environment, a multi-snippet variant runner, and a whole-file
per-function compare.
