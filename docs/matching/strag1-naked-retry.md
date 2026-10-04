# NAKED retry (strag1): 5 of 5 closed

These five NAKED functions were not tracked by any open issue and had
not had a recent pass. All five are now real C built with old_agbcc.
None of them needs a register pin.

| Function | File | Size | Before | Result |
|---|---|---|---|---|
| `sub_8025CA4` | `src/system/game_loop14.c` | 132 B | 5 hw (note) | **Closed** |
| `sub_8025B0C` | `src/system/game_loop14.c` | 160 B | 61 hw (note), no draft | **Closed** |
| `DecodeLayerChunk` | `src/system/game_loop57.c` | 320 B | 13 hw (note), no draft | **Closed** |
| `RunCutscenePlayer` | `src/system/game_loop57.c` | 284 B | 77 hw (note), no draft | **Closed** |
| `DrawSpritePieces` | `src/graphics/graphics_73dc.c` (new, split from `graphics.c`) | 600 B | draft removed long ago | **Closed** |

`game_loop14.c` and `game_loop57.c` were already on `OLD_AGBCC_OBJS`.
`DrawSpritePieces` was the last function in `graphics.c`, which is built with
agbcc. Its loop loads the `0xf` mask before the `ldrb` it is combined
with, which is the old_agbcc tell. It now lives in its own
`graphics_73dc.c` on `OLD_AGBCC_OBJS`. `0x080073DC` is 4-byte aligned,
and `ldscript.txt` places the file between `graphics.o` and
`graphics_7634.o`.

## sub_8025CA4

A plain C draft was 11 halfwords off. There were two parts to fix:

- **Stack parameters.** `p5` is read as the low byte of a word
  parameter (`*(u8 *)&flag5`), which gives the ROM's `add r0,sp,#N;
  ldrb`. `p4` is a full word, since the ROM compares it with 0xff and
  never truncates it. This is the same approach as `sub_8025A64`.
- **The +0x4B zero.** Two changes together put the `movs r0,#0` after
  the +0x49 address: the three tag bytes are written through `u8 *t`,
  and the zero goes through a local with `asm("" : "+r"(zero))`. This
  is `sub_801173C`'s `phase` trick. Either change alone leaves 3
  halfwords off.

## sub_8025B0C

The first draft was 67 halfwords off. Three source-shape changes closed
it:

- **One frame struct for both AABBs**
  (`struct { struct fx_box a, b; } f`). With separate locals, gcc held
  `&b` in a callee-saved register across the second `sub_8007B98` call.
- **Spawn arguments through locals `x0`/`y0`/`m`.** This computes all
  three before either stack-argument store, as the ROM does. It also
  fixed the register permutation: with `y0` live in r5 across the
  mirror computation, `speed` can no longer take r5, so the ROM's
  part=r5, src=r6, speed=r7 falls out. Extra-reference nudges had no
  effect on that permutation.
- **The velocity seed goes through `SetVel(part, v, 0x40)`.** It is an
  inline whose arguments are expanded before the four stores (#493). The
  X offset is a `?:`, so the flip byte is tested before `ox + dist`.

## DecodeLayerChunk

This is the terrain cache's RLE/delta decoder `DecodeCollisionChunk`
(`game_loop3.c`, already matched), writing into a 64-halfword-stride
ring buffer. Porting `DecodeCollisionChunk`'s matched source with the 2D cell
index `out[(i >> 4) * 64 + (i & 0xf)]` gave 40 halfwords off, all of
them an r4/r5 swap plus two local spots. Two changes closed it:

- **The odd trailing delta** is stored back into `acc` before the cell
  store, which gives the ROM's `add` before the address. This also
  resolved the r4/r5 swap. `DecodeCollisionChunk`'s `asm("" : : "r"(n))` nudge
  is not needed here.
- **The raw-copy loop** builds its index as `k = written >> 4;
  k = k * 64 + (written & 0xf);`. That gives the ROM's `asr; lsl` pair
  into separate registers. Every single-expression form puts `& 0xf`
  first or merges the two shifts into one register.

## RunCutscenePlayer

This is the text pager driver. The first draft was 110 halfwords off.
The old note said "the ROM reloads `&gOamBuffer` at each OAM
flush", and that was the key. The three reloads rotate r1/r2/r3, which
is reload rematerializing a spilled constant-equivalent pseudo. The
source has a function-scope `void **oamp = &gOamBuffer;` that is
set before the loop (#505). Its live range spans the loop, so
global-alloc gives it no register. That frees r4 for `self` and puts
every other value in the ROM's register. After that:

- **Prologue.** It reads `self->box[0]` and `self->target` into locals
  before the store. That gives the `movs r3,#0x8c; lsls; ... adds r3,#4`
  constant reuse.
- **Page loop.** It is a plain `for (j = 0; j < count && res == 1; j++)`.
  A manual `j++` at the top of the body splits `j` differently.

## DrawSpritePieces

The non-affine sibling of `DrawAffineSpritePieces` (#509). The draft was written
directly in `graphics_7634.c`'s matched shape and matched on the first
compile under old_agbcc. Under agbcc it is 37 halfwords off. It uses the
same structures and techniques as `DrawAffineSpritePieces`:

- `oam_attr2` has `u16` fields;
- `PART_FLAG_SET` sign tests on the `+0x28` byte;
- the `PieceShape`/`PieceSize` inline helpers;
- the part's bitfields are read for gfx mode, mosaic, colour mode and
  palette.

The OAM attr1 bits 28/29 are the H/V flip bits, set from the part's
mirror flags before the loop. The spill pattern that parked it before
(the `part+0x28` pointer at `[sp,#0x18]`, `pos` at `[sp,#0xc]`) comes
out naturally. The `graphics-7634-retry.md` diagnosis about taking early
pointer locals to force an r7 spill was not needed here.

## Tools

The scratch helpers are in the session scratchpad under `strag1/`:

- `d.py`: a one-function diff against the ROM;
- `var.py`: the parallel variant runner;
- variant specs `ca4*.py`, `b0c*.py`, `960*.py` and `820*.py`.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
