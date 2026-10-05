# NAKED retry: issues #12, #13, #25

This pass retried the nine functions the previous retry
([issue-24-26-12-naked-retry.md](issue-24-26-12-naked-retry.md)) did
not reach. **3 closed**, all under old_agbcc. Four more now have C
drafts under `#if NON_MATCHING`. Two of those are one or two details
away from matching.

## Closed

| Function | File | Technique |
|---|---|---|
| `UpdateCrateFall` (#13) | `game_loop32.c` (now in `OLD_AGBCC_OBJS`) | Every use of the speed byte goes through `self->fallSpeed` (the ROM's `sb` is the GCSE copy of that address). `speed--` is written in both step arms. The neighbour walk skips the first neighbour: `n = next(self); if (n) { n = next(n); while (n) {...} }`. One temporary `t` carries `fallTargetY` into `y` and re-reads `x` at the bottom of the loop, which is the ROM's r1. |
| `UpdateCrate` (#13) | `game_loop51.c` (now in `OLD_AGBCC_OBJS`) | The 0x13-0x15 range test is two nested `if`s on an `s32` copy of `kind`. A single `&&` gets folded into an unsigned subtract-and-compare, and testing the u8 field gives unsigned branches. The `kind == 0xf` test and its `state` test are nested for the same reason: one `&&` makes gcc merge the two adjacent byte compares into one word compare. The frame clamp is the new `PhysSetFrame(self, 0)` inline, whose parameter keeps the constant 0 in r3 for the later `animDone`/`busy` stores. The tile-cache key's record is indexed from a local copy of `anim->records`, which loads the table before the tag. Needs `_call_via_r1` aliased to `_call_via_r1` for the `m60` method call. |
| `BreakCrate` (#12) | `game_loop48.c` | The tag goes through the `PhysSetTag` inline (the constant is loaded before the tag address). The frame clamp is `PhysSetFrame(self, 3)`. Bit 4 of `flags` is set as a bitfield (`PHYS_FLAG4`). A plain `|= 0x10` leaves a zero pseudo that CSE shares with the later `busy = 0` store, which moves `self` from r4 to r5. The state store's constant 1 is a local `one` that the bitmap shift reuses (the ROM's r8). The switch cases are written in the ROM's block order, with an explicit empty `case 22`. |

All three were found with the brute-force variant runner (scratchpad
`mix12b/brute2.py`, a copy of `box9/brute.py` that also scores each
variant by disassembly-diff lines and can print a register-agnostic
diff). Scoring by diff lines instead of differing halfwords helps
because one inserted instruction shifts every later halfword.

Shared header changes (`include/crate.h`), all layout-preserving:
`crate_vtable.m60`, `crate.animDone`, `crate.trialKind`,
`phys_flag_bits.bit4` plus `PHYS_FLAG4`, the `PhysSetFrame` inline, and
`struct phys_player` extended with the player fields `ApplyCrateCollision`
touches (`x`/`y`, `vtable`, `dir`, the velocity words, `hitAxes`,
`hitMask`, `bounce`, `carried`, `unk_10C`).

## Not closed

- **`ApplyCrateCollision`** (#12, `game_loop47.c`): near miss under old_agbcc
  (49 halfwords, same size). The draft differs in exactly one place.
  In case 3, the ROM reads the first flag byte back as a byte from its
  spill slot (`mov r5, sp; ldrb r2, [r5]`). The draft reads it as a word
  (`ldr r2, [sp]`), which shifts the rest of that case by a halfword.
  The three trailing args are packed one-byte structs (the entry copies
  then match: `ldrb` from the incoming slot, `str` to the frame). The
  landing block writes `pos` through a pointer; with direct writes, gcc
  doesn't reload the player pointer after the stack stores. Tried
  without success: u8/s8/s16/u16/s32/u32 locals, casts, u8 prototypes
  for `BreakCrateInStack`, K&R parameters, struct copies, and an
  address-taken local (which gives the `ldrb` but loads it too early).
  The file is still built with agbcc. If the draft closes, the whole
  file (whose other function is the NAKED `QueueCratePlayerCollision`) can move to
  `OLD_AGBCC_OBJS`.
- **`BreakCrateTouchedByPlayer`** (#12, `game_loop6.c`): near miss under old_agbcc.
  The self box and the tail match. For the player box, gcc keeps
  `&b` (`sp+0x10`) in a callee-saved register across the two builder
  calls. The ROM re-adds it for each call, and that pushes px/py and
  `&gPlayer` into the wrong registers. No `-f` flag changes
  it. Frame structs, arrays, macros, inline builders (by pointer or by
  value) and px/py scoping didn't help either.
- **`UpdateSlotCrate`** (#12, `game_loop49.c`): 58 halfwords off under
  old_agbcc, same size. `u48` is handled as a word with explicit byte
  masks (`& 0x3f`, `& 0xc7`, `& 0xf8`), the direction switch as
  `(u32)(w & 0xc0) >> 6`, and the |dx| test as `d = a; d -= b`. What is
  left is register choice and order in the phase/count updates.
- **`CreateCrate`** (#13, `game_loop36.c`): first full C draft, 1416
  bytes vs 1396 under old_agbcc. The control flow, both jump tables
  and the per-type field stores are all reconstructed. The allocation
  differs from the prologue on: the ROM has `type` in r7 and slot*2 in
  r8, the draft the other way round.
- **`ResolvePlatformCollision`** (#25, `actor_part_1ab98.c`): the existing draft is
  still 40 bytes short under both compilers (644/656 halfwords). The
  ROM cross-jumps the overlap arithmetic of both `hdir` arms into one
  shared tail, which the draft's allocation prevents. Not iterated
  further this pass.
- **`QueueCratePlayerCollision`** (#12, `game_loop47.c`, 3840 B): not attempted. Its
  size is roughly `ApplyCrateCollision` and `CreateCrate` combined.

## Reusable lessons

- **Nested `if`s are not neutral.** `a && b` on adjacent byte fields
  can become one word compare, and `x > 0x12 && x <= 0x15` can become an
  unsigned subtract. Writing them as nested `if`s keeps the ROM's two
  compares. A range test on a u8 field also needs an `s32` copy to get
  signed branches.
- **An inline with a constant parameter** (`PhysSetFrame(self, 0)`,
  `PhysSetTag(self, K)`) gives the constant its own register before
  anything else happens. That moves register ties and instruction order
  in the ROM's direction.
- **A local `one = 1` / `m = 8` / `e = 0xe`** loads the constant before
  the store address. It also lets a later use share the register (the
  r8 "accumulator" in `BreakCrate`).
- **Stores through a pointer to a stack struct** (`pp = &pos; pp->y =`)
  make gcc reload a global pointer afterwards. The ROM had that reload
  in `ApplyCrateCollision`.
- **A local copy of `anim->records`** (`recs = self->anim->records;
  rec = &recs[self->tag]`) loads the table before the tag, which is the
  order the ROM uses in this whole cluster.
