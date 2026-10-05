# Huge NAKED retry 3: `QueueCratePlayerCollision`

| Function | File | Issue | Before | After |
|---|---|---|---|---|
| `QueueCratePlayerCollision` | `src/system/game_loop47.c` | #12 | 33 hw, size-exact, NAKED | real C, matches |

The function now builds under old_agbcc (`game_loop47.o` was already on
`OLD_AGBCC_OBJS`). The NAKED copy and the `#if NON_MATCHING` split are
gone. It was the last NAKED function in issue #12's range.

The halfword counts below are from the triage compare (old_agbcc,
relocations masked). That compare also counts branch targets, which the
second pass's `asmdiff.py` normalizes away. So four of the 33 halfwords
were branch targets that asmdiff never showed.

## What fixed it

1. **Spill slots 0x94/0x98 (33 -> 29).** reload gives stack slots to
   pseudos without a hard register in pseudo-number order (`alter_reg`
   in reload1.c, called for every pseudo from `reload()`). The user
   variables that end up on the stack come first (0x70-0x90), then the
   gcse temps, which gcse numbers in hash-bucket order. `kind * 4`
   (hash 452) got a lower number than `self + 77` (hash 511).

   The fix is a user variable for `&self->state`, declared last among
   the function's locals (`u8 *st;`). It is numbered below every gcse
   temp and above the other stack variables, so it gets slot 0x94 and
   `kind * 4` gets 0x98. It is set right after the state is loaded and
   masked:

   ```c
   {
       s32 s = self->state & 0x7f;

       st = &self->state;
       if (s == 1)
           goto tail;
   }
   ```

   Then `st` is a copy of the load's address register and is stored
   before the compare, as in the ROM. The `case 1`/`case 2` test reads
   `*st`. Setting `st` before the load, or putting the assignment inside
   the condition, reloads the address from the stack for the load
   (96 halfwords). Setting it after the `if` stores it only on the
   fall-through path, after the branch.

2. **Table address before `&obj->kind` (29 -> 26).** The first lookup
   goes through `D18C_CodeIn(gCrateHitResponse, &obj->kind, kind)`,
   which returns `*(s32 *)((u8 *)t + (*row * 28 + k * 4))`. All of an
   inline call's arguments are expanded first, so the table is loaded
   first. The sum order gives the ROM's `(k * 4 + row * 28) + table`.
   `(u8 *)t + (k * 4 + *row * 28)` evaluates the row first (55).
   Array indexing (`t[*row][k]`) adds the table in the middle (28).

3. **Second slope check's reload registers (26 -> 4).** The ROM's
   `ldrsh` there has its result and base in r1 and the scratch in r0.
   The draft had them the other way round. The result pseudo is spilled
   by reload for that insn and comes back through inheritance, so its
   register is just the next spill register in reload's round-robin
   (`allocate_reload_reg` starts after `last_spill_reg`). Only r0 and r1
   are free there. The last reload before it was dirX into r6 (the first
   slope check's `edge = dirX`), so the round-robin wrapped to r0.

   In the ROM, one more reload happens in between. That reload is
   the first check's `ay > limit` exit, written as an `if`/`else
   edge = dirX` instead of `goto edge_x`:

   ```c
   ay += q->yOff + q->h;
   if (ay <= f.a.y + f.a.h)
   {
       ...
   }
   else
       edge = dirX;
   ```

   The `else` is the last code of that arm in the RTL. Its reload of
   dirX takes r0, and the second check starts at r1. After reload,
   cross-jumping merges the `else` into `edge_x` (which also reloads
   dirX into r0), so it leaves no code. This is the same effect as step
   10 of the second pass. The r0 holds suggested in the brief move the
   result to r1, but reload then spills the value, because the hold
   clobbers r0 between the output reload and the inherited use.

4. **Ring-push re-test (4 -> 2).** In `D18C_RING_PUSH`, a failed first
   `ctrlMode == 0` test jumped straight past the second test. cse's
   jump following knew its outcome. The ROM's `bne` goes to the second
   test's load. The first test now reads the byte through an `s32`
   inline (`D18C_CtrlMode()`), so the two tests are no longer the
   same expression. Two branches changed, one in case 3 and one in its
   `n` loop.

5. **The `dy > 2 || (dx <= 3 && sub_800B324())` arm (2 -> 0).** The
   ROM's two branches for this arm go to the r6 copy of `edge = dirX`
   (the first slope check's), not to `edge_x`'s r0 copy. The arm is now
   written like the `ax == px` arm above it:

   ```c
   else if (dy <= 2 && (dx > 3 || !sub_800B324(D18C_P)))
   {
       edge = 4;
       if (f21 != 0)
           edge = 8;
   }
   else
       edge = dirX;
   ```

   The `else`'s reload of dirX gets r6. Cross-jumping then sends both
   branches to the r6 block. A plain `edge = dirX` in place of the `goto`
   (without inverting the test) makes jump optimization rearrange the
   arms and is 12 bytes short.

## What didn't work

- Holding r0 around `ay += q->yOff`, or between the `ldrsh` and the add
  (`s32 t = q->yOff;` then the hold): the result gets r1, but it is
  spilled (`str r0, [sp, #0xa0]` / `ldr r1`), 4 bytes long.
  Holds on r2/r3 are worse.
- `volatile u8 *st`: 53 halfwords or more.

## Tools

The scratch helpers are in `huge3/`, copied from `huge2/`, plus:

- `bindiff.py`: compiles the file, disassembles the function and the ROM
  bytes, and diffs them. Unlike `asmdiff.py`, it shows branch-target
  differences. Relocated `bl`/pool words show up as noise.
- `rtlseq.py`: prints the post-reload RTL (from a `-dg` dump) between two
  insn uids, one line per insn.
- `a1.py`-`a9.py`: `bf.py` variant specs for the steps above.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from
  `game_loop47.c`.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
