# Actor-zone NAKED near-miss retry

A retry of seven NAKED functions whose near-miss C drafts were already
in tree under `#if NON_MATCHING`
([issue-51-54-naked-retry.md](issue-51-54-naked-retry.md),
[issue-58-61-naked-retry.md](issue-58-61-naked-retry.md)). All seven
closed as plain C with no register pins. Issue #54 has nothing left.

| Function | File | Compiler | Was |
|---|---|---|---|
| `UpdateYeti` | `actor_part74.c` | old_agbcc (file already on it) | 52 off |
| `sub_802DD9C` | `actor_part75.c` | old_agbcc (file moved) | 69 off |
| `LoadYetiGraphics` | `actor_part75.c` | both | 5 off |
| `IsTouchingAirship` | `actor_part24b.c` | old_agbcc (file moved) | 53 off |
| `DrawAirshipMap` | `actor_part23b.c` | old_agbcc (file moved) | 11 off |
| `DrawHovercraftMap` | `actor_part130.c` | old_agbcc (whole file moved) | 11 off |
| `sub_8030E08` | `actor_part23c.c` | both (file stays on agbcc) | 19 off |

`actor_part130.c` was checked function by function under old_agbcc
before the move: every function in it, the previously matched ones
included, compiles to the ROM's bytes (the NAKED `ConvertHovercraftTiles`
assembles identically either way).

## The box-copy trio: one frame struct

`UpdateYeti`, `sub_802DD9C` and `IsTouchingAirship` all build a static box,
build a second box from an actor's `+0x38` vector plus its position,
copy it into a third slot, run that slot through the `MemCopy32`
self-copy, and compare. The ROM recomputes `add rX, sp, #0xc` after the
block copy and takes box A's address (`mov r1, sp`) again after the
call. As three separate locals, gcc computed `&b` once and kept it (and
`&a`) in callee-saved registers, which shifted everything else.

The fix, as in `actor_part81.c`'s `struct aabb_copy`: make the three
boxes members of one stack struct.

```c
struct {
    struct box16 a, b, t;
} f;
...
f.t = *(struct box16 *)self->box;
BoxMove(&f.t, self->x >> 8, self->y >> 8, self->z >> 8);
f.b = f.t;
b = &f.b;
MemCopy32(b, b, sizeof(*b));
return BoxOverlap(&f.a, b);
```

Member addresses are frame offsets, so each use is rematerialized from
sp. Only `&f.b`, through the pointer local, stays live across the call,
and `&f.a` is taken after it. The struct-returning `ActorBox()` inline
had to go too: returning into a struct member adds a fourth temporary.

Two function-specific details:

- `UpdateYeti`'s hit block uses its own `g` local for the gauge object.
  The function-wide `obj` is live across calls, so it gets a
  callee-saved register there.
- `IsTouchingAirship` needs `goto hit; ... return 0; hit: return 1;` so the
  failure path falls through, plus a trailing `asm(".align 2, 0")`.

Under current agbcc they are still off (25-31 halfwords for
`sub_802DD9C`/`IsTouchingAirship`), in the early box-A arithmetic.

## `LoadYetiGraphics`: `CurFrame()` on the global

The three hoisted addresses have to land in r8/sb/sl as `...14BC`,
`...14C0`, `...0898`. With a `struct actor_self *obj = gYeti`
local, `&gYeti` came last. Passing the global straight into
the usual `CurFrame()` inline (`t = animTime >> 8` first, then
`frameOffsets[anims[animIndex].frameIndex + t]`) gives the ROM's
assignment under both compilers. The call through the
`gUnpackNibbleTilesFunc` pointer needs `_call_via_r2` aliased to
`_call_via_r2`.

## `DrawAirshipMap` / `DrawHovercraftMap`: bias narrowing and declaration order

Two changes, identical in both twins:

1. Read the bias byte as `u8 bias = gUnknown_03001530;` (a narrowing of
   the `s32` global), not `*(u8 *)&gUnknown_03001530`. That turns the
   hoisted `&bias` copy into the ROM's `mov r3, sb` in the outer loop.
   This got the drafts down to 6 halfwords.
2. Declare `row` before `i`. The loop optimizer creates the `row + 0x20`
   and `i + 1` pseudos (`adds r7, r2, #1` / `mov ip, r0`). They tie on
   global-alloc priority (refs 4, live length 32), so the lower pseudo
   number gets r7. Declaring `row` first makes `i + 1` the lower one.

The `-dg` dump (`Register N, refs = R, live_length = L`) is how the tie
showed up. Priority is `floor_log2(R) * R / L`, and ties go to the lower
pseudo number.

## `sub_8030E08`: store count per branch

The ROM gives the `&gUnknown_03001558` copy r6 and the
`&gUnknown_0300155C` copy r4. Both copies are made by the same pass
right before the first branch, and with equal refs (10 each) the 1558
copy's shorter live range gave it priority and r4. Instruction-level
changes can't help there; the reference counts before cross-jumping
can:

- The X step stores its `vx -+ 3` result once, through a temporary
  (`t = vx - 3; ... else t = vx + 3; gUnknown_03001558 = t;`). That
  leaves the 1558 copy with 9 refs.
- The Y step stores in each branch (`gUnknown_0300155C = v - 2;` and
  `v = gUnknown_0300155C; gUnknown_0300155C = v + 2;`). Cross-jumping
  merges the two stores later, but global-alloc sees 11 refs.

That makes the 155C copy the higher-priority pseudo. Making `v` a
variable shared by both Y branches also gives it r0 ahead of the
`dy >> 10` temporary, as in the ROM. The result matches under both
compilers.

## Not attempted

- `ConvertHovercraftTiles` (`actor_part130.c`) and its 4-row twin `ConvertAirshipTiles`
  (`actor_part26c.c`) are still NAKED with their drafts, 95-105 halfwords
  off (unchanged from issue-58-61-naked-retry.md). They are why issues
  #58 and #61 stay open.

## Verification

`rm -rf build && make NON_MATCHING=1 report` shows no warnings from the
touched files. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` gives `crashbandicootxs.gba: OK`.
