# Mix NAKED retry 5: `ActionCtrlStateLeftGround`, `ActionCtrlReleaseHang`, `UpdateWumpa`, GAX

A retry of five parked NAKED functions with C drafts. **2 of 5 closed
as real C**, both under old_agbcc. Their files were already on
`OLD_AGBCC_OBJS`.

| Function | File | Issue | Before | Now |
|---|---|---|---|---|
| `ActionCtrlStateLeftGround` | `actor_part_14674.c` | #17 | 1 hw (branch target) | **real C** |
| `UpdateWumpa` | `game_loop53.c` | #15 | 21 hw | **real C** |
| `ActionCtrlReleaseHang` | `actor_part_14674.c` | #17 | 3 hw | NAKED, unchanged |
| `GaxChannelMix` | `gax_note_trigger.c` | #68 | ~237 seq | NAKED, draft ~202 seq |
| `GAX2_init` | `gax_playstart.c` | #66 | ~294 seq | NAKED, not retried |

"seq" is the alignment-insensitive count from
[gax-naked-retry-2.md](gax-naked-retry-2.md).

## `ActionCtrlStateLeftGround`: the tag test is a `switch`

The ROM tests the part's tag like this:

```
cmp r0, #13 ; beq A      @ jumps past the re-test below
cmp r0, #24 ; bne other
cmp r0, #13 ; bne tag18  @ re-test, kept on the 0x18 path
A: ...
```

The earlier draft was `if (tag == 0xD || tag == 0x18) { if (... == 0xD)`.
It either kept the re-test on both paths (inner test re-reads the
field, so the compares aren't provably the same when jumps are
threaded), or dropped it on both (inner test on `tag`; cse follows the
0x18 fall-through and folds the re-test).

A `switch` fixes it:

```c
switch (tag = self->part->tag)
{
case 0xD:
case 0x18:
    if (tag == 0xD) { ... }
    else if (tag == 0x18) { ... }
    break;
default:
    ...
}
```

The two cases share one label. The 0xD `beq` jumps to that label and
is threaded past the `tag == 0xD` re-test. The 0x18 path falls into the
label from the case-tree's own `bne`, so cse doesn't carry `tag == 0x18`
into it, and the re-test stays. The assignment has to be inside the
`switch` expression. `tag = ...; switch (tag)` and
`switch (self->part->tag)` with re-reads are still 1 halfword off.

## `UpdateWumpa`: three local fixes

The draft from [big-naked-retry-3.md](big-naked-retry-3.md) had three
differences left. Each one had its own fix:

- **Modes 1/2 integrate step.** The ROM loads the position into r1 and
  the velocity into r0. `ORBIT_STEP(pos, vel)` reads both into locals
  and adds two no-code `asm("" : : "r"(_v))` references to the
  velocity (brief item 8). One reference isn't enough. With two, the
  velocity outranks the position/sum quantity for r0. The same fix
  closed `UpdateExtraLife`.
- **Spawn byte argument.** The ROM computes `add r3, sp, #4` before
  `movs r5, #1`. With a plain `*(volatile u8 *)&argP5 = 1`, the address
  is a reload of the store, so it is emitted after the constant. Taking
  the address into a pointer with `asm("" : "=r"(q) : "0"(&argP5))` (a
  statement expression inside the argument's comma expression) makes it
  its own earlier insn. The store stays a QImode store of the constant
  1, so cse still shares that 1 with mode 3's `flags |= 1`. With the
  `asm` as a separate statement before the call, the draft was worse
  (6 halfwords).
- **State-3 tail.** `nx = px - 0x400, ny = py - 0xe00` go into locals
  before the two stores, as in the ROM.

## What didn't close

- **`ActionCtrlReleaseHang`** (3 halfwords). The 0x600 in `part->y += 0x600` is
  a reload, and it lands in r2 where the ROM uses r3. The greg dump
  shows reload spilling r2 for that insn. These made no change:
  instruction-count padding (0-7 `asm("")`), extra references on
  `self`, `part`, `count` or the records pointer, `ACT_CALL*` instead
  of `ACT_VCALL*`, the constant-init `asm` for 0x600 and for the
  `ActSetNext` 4, inline `MoveY`/`Lift` helpers, a `u8`/`s32` zero for
  `unk_101`, and pointer spellings of the `y` update. Adding references
  to the records pointer moves its load instead (6 halfwords).
- **`GaxChannelMix`** (GAX mixer). The ROM has its own `__muldi3`. With
  a plain 64-bit `*` (the libcall
  lesson from #481), and with the ping-pong test re-reading
  `self->instrument->rows[self->row]`, the draft went from ~237 to ~202
  seq. The main gap is unchanged: `self`/`info`/`flag` get r5/r7/r9
  where the ROM has r6/r4/r5, and that cascades. Also tried: a local for
  the envelope volume, and re-reading `row`/`instrument` elsewhere
  (neutral or worse). Note that `triage_naked.py` takes this function's
  ROM size from the map, where the `sub_8039E50` label cuts it at 780
  bytes. The real size is 0x3EC.
- **`GAX2_init`** (GAX play start). Not retried beyond re-measuring
  it (398 positional halfwords, sizes 1248 vs 1252). The differences are
  spread over the whole function: register choice, and the order of
  the constants hoisted before the ARM-code copy loops. None of it
  looked like a quick local fix.

## Tools

`brute2.py` from the `mid4/` scratch runner (with `RAWB=1` for branch
targets), plus a `ROMSIZE` override for the GAX mixer. The variant
specs are in the scratch area (`mix5/`) and are not checked in.
