# NAKED retry: issues #12, #13, #24, #25, #26

This pass retried 15 parked NAKED functions. **3 closed**, all under
old_agbcc. The rest stay NAKED. Three of them now have improved
`#if NON_MATCHING` drafts that differ from the ROM by one allocation
detail.

## Closed

| Function | File | Technique |
|---|---|---|
| `sub_801A03C` (#24) | `actor_part_1967c.c` | Virtual calls written as plain blocks (`VCALL1_B`) instead of `do { } while (0)`, whose loop notes swapped the part and controller between r4 and r5. `p->f28.facing = facing` is stored unmasked into the 1-bit field; an explicit `& 1` makes gcc reuse the held constant 1 for the tag store. |
| `InitLevelSelect` (#26) | `actor_part_1b85c.c` | The BLDCNT/BLDY/DISPCNT shadow registers are plain bitfield stores. Under old_agbcc each store becomes an unfolded `orr` in the ROM's order, so the `Opaque()`/constant-variable scaffolding was dropped. The six-entry loop has its own block-scoped counter. The sprite loop's `0x80` sits in a variable set in the `for` initializer (`for (i = 0, v = 0x80; ...)`), which gets the ROM's hoisted r8 constant. It also relies on `SetAnim(s32)` (see below). |
| `DropCratesAbove` (#12) | `game_loop48.c` | The notes blamed the "triple accumulator". The real cause was `spread = n->unk_40 - n->y`: writing it as two statements (`spread = n->unk_40; spread -= n->y;`) gives the ROM's r8/r7/r9/r10 for self/spread/carry/base. Other fixes: the record lookup is anim table first, then the byte offset added to `records`; `n->unk_40` is stored in both arms of an if/else (the ROM re-reads it); the s8 step delta is widened into its own `s32` before the add; `gCrateKindExplosive` is held in a local set before the loop. |

Shared changes in `actor_part_1b85c.c`, checked against every
function in the file under old_agbcc:

- `SetAnim` takes `s32` (the ROM loads the full word from index tables).
- `struct level_info`'s `time0`-`time2` are `u32` (the ROM compares them unsigned).
- `struct level_save` uses `u16` bitfields, which makes gcc narrow the `time != 0` test to `ldrh`+mask.
- The pinned `PressedBits` helper was removed. Plain `gKeys.half.pressed & N` gives the same code.

## Not closed

- **`sub_801A114`** (#24): allocation. The ROM keeps long-lived values in
  r7/r9/r10/r8 and leaves r4-r6 unused. The state-0 BLDCNT `orr` chain
  (which gcc normally folds) is now reproduced by setting `bld` before
  the `switch`. CSE can't see its constant from inside the case. The
  allocation didn't move.
- **`LoadLevelSelectRecord`** (#26): identical except that the ROM also spills
  `info` (sp+0) and reloads it for the `time0` test. Declaration order
  and placement didn't help.
- **`LevelSelectLoop`** (#26): identical except for one `adds r1, r2, #0`
  key-word copy before the 0x80 test. Variants that move or retype the
  copy didn't help.
- **`sub_801AB98`** (#25, 1648 bytes), **`BreakCrate`**,
  **`sub_0800D18C`**, **`sub_800D040`**, **`sub_800E08C`**,
  **`UpdateSlotCrate`** (#12), **`UpdateCrateFall`**, **`CreateCrate`**,
  **`UpdateCrate`** (#13): not attempted this pass. The time went to the
  functions above. None of the #12/#13 ones has an in-tree C draft yet.

## Reusable lessons

- In old_agbcc files, try plain bitfield stores before any
  `Opaque`/pin scaffolding. That compiler chains the `orr`s itself.
- If gcc merges a subtract into a copy (`t = a - b; x = t`), splitting it
  into `x = a; x -= b;` can change allocation priorities.
- To get "load the base after computing the index", compute the byte
  offset first and add it to the loaded base pointer.
