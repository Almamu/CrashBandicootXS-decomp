# Issues #48/#49/#52 NAKED retry (actor category and AABB code)

A retry of the 13 NAKED functions in `0x080297C8`-`0x0802C8xx`
(issues #48, #49, #52), prompted by the frame-struct fix for the
AABB trio in [actor-zone-naked-retry.md](actor-zone-naked-retry.md).
None of them had a C draft in tree, so each was written from the ROM
disassembly and compiled under both agbcc and old_agbcc.

Nine closed as plain C, with no register pins:

| Function | File | Compiler | Technique |
|---|---|---|---|
| `sub_802A018` | `actor_part103.c` | old_agbcc (file moved) | one frame struct for the boxes |
| `sub_802A110` | `actor_part103.c` | old_agbcc | same inline as `sub_802A018` |
| `sub_802A3AC` | `actor_part103.c` | old_agbcc | same inline, inside a list walk |
| `sub_802C7A8` | `actor_part19h.c` | old_agbcc (file moved) | same inline, inside a list walk |
| `FillCellAnimTilemap` | `actor_part98.c` | both | `tile++` in each branch |
| `ResetCellAnimBg` | `actor_part95.c` | both | `FillCellAnimTilemap` inlined twice, upward clear loop |
| `UploadCellAnimFrame` | `actor_part95.c` | both | expression order and locals |
| `sub_802A674` | `actor_part94.c` | both | returns the callee's result |
| `sub_802A688` | `actor_part94.c` | both | returns the callee's result |

`sub_802A688` was not on the list, but it is `sub_802A674`'s twin in the
same file and parked for the same reason.

Still NAKED: `InitActorCategory`, `InitCellAnim`, `SelectActorCategory`,
`RunActorCategoryFrame`.

## The AABB group: one inline, one frame struct

`sub_802A018`, `sub_802A110`, `sub_802A3AC` and `sub_802C7A8` all run
the same test: copy actor A's `+0x38` box to a stack slot, translate it
by A's position, copy it to a second slot and run the `MemCopy32`
self-copy, then do the same for actor B, and compare. In the source this
is one `static inline` (`ActorsOverlap`, in `actor_part103.c` and again
in `actor_part19h.c`):

```c
struct {
    struct box16 a, t, s;
} f;
struct box16 *t;
s32 x, y, z;

f.t = *(struct box16 *)pl->unk_38;
x = pl->x >> 8;
y = pl->y >> 8;
z = pl->z >> 8;
t = &f.t;
BoxMove(t, x, y, z);
f.a = *t;
MemCopy32(&f.a, &f.a, sizeof(f.a));
f.s = *(struct box16 *)self->unk_38;
BoxMove(&f.s, self->x >> 8, self->y >> 8, self->z >> 8);
*t = f.s;
MemCopy32(t, t, sizeof(*t));
return BoxOverlap(&f.a, t);
```

- The three boxes are members of one struct, so every box address
  except `t` is a fresh `add rX, sp, #off`, as in the ROM.
- `t` (the middle box) is the only address kept in a register. It is
  the ROM's `r4`. In the two list walks, gcc also hoists `&f.s` out of
  the loop into `r7` by itself. That is the "unreachable `r7`" the old
  notes described.
- Reading A's position into `x`/`y`/`z` before taking `t` puts the ROM's
  `add r4, sp, #0xc` after the three loads. B's position is passed
  directly, which gives the ROM's order for the second box.

The type test in `sub_802C7A8` is `**(u8 **)(node + 0x30) == 4`. The
state test is `animIndex != 0x12`; `+0x0c` is `animIndex`, not `state`.
`sub_802A3AC`'s gate is method slot `0x28` of the node's method table,
called through `_call_via_r1`.

All four need old_agbcc. Current agbcc schedules the `asr`s in the box
translation differently, which puts them 24-26 halfwords off.
`actor_part103.c` also holds the NAKED `RunActorCategoryFrame`, which assembles
the same under either compiler. `actor_part19h.c` holds only
`sub_802C7A8`. Both files moved to `OLD_AGBCC_OBJS`.

## The tile-map fill: `tile++` in each branch

`FillCellAnimTilemap` fills a screen block with consecutive tile numbers.
Columns past 31 go to the next screen block:

```c
for (row = 0; row < h; row++) {
    for (col = 0; col < w; col++) {
        if (col <= 0x1f)
            base[col] = tile++;
        else
            base[col + 0x3e0] = tile++;
    }
    base += 0x20;
}
```

With one `tile++` after the `if`, everything matched except that `base`
and `tile` swapped registers. That cascaded into the `r8`/`ip` roles of
the `0x7c0` offset and `h` that the old note described. The two
increments give `tile` more references before cross-jumping merges them.
It then outranks `base` in global allocation.

`ResetCellAnimBg` inlines the same body twice, once with `arg0 == 0` and
once with `arg0 == 1`. The inlined copy needs both branches to assign
`base`. The standalone version's default-initializer form is 5
halfwords off once inlined with a constant. Its 8-word clear loop is
`u32 *vram = (u32 *)0x06000000; for (i = 0; i < 8; i++) vram[i] = 0;`.
Loop reversal and biv elimination turn that into the ROM's signed
pointer loop, with `vram` in `r1`. A downward `i`, a constant base, or a
hand-written pointer loop each give a different shape.

## `UploadCellAnimFrame`

`gDrawMirroredTilemapFunc` is a function pointer, called through
`_call_via_r4`. Three changes each fixed one piece of the instruction
order:

- `base + ((gCellAnimTime >> 8) * gCellAnimFrameSize + 0x204)`,
  with the parentheses.
- `dst = gCellAnimPage != 0 ? 0x06000000 : 0x06002000;`
- The callback's first argument goes into its own local.

## `sub_802A674` / `sub_802A688`

They return what `sub_802F4C0`/`sub_802BD18` return (`pop {r1}; bx r1`).
The callees are `u8`, but these wrappers do not re-narrow the value, so
they are declared `s32` here. The old note put the epilogue down to a
TU-wide allocator quirk.

## Not closed

- **`InitCellAnim`** (37 halfwords, both compilers). All of it is in the
  `gCellAnimFrameSize` block. The ROM stores `A4` once, after the
  `if (flag)`, and then reloads it for the division through a copy of
  its address (`adds r1, r4, #0` ... `ldr r1, [r1]`). The draft
  (`A4 = A0 = area << 5; if (B8) A4 += ...`) stores twice. Storing a
  local once makes gcc forward the value into the division, and the flag
  byte drops from `r5` to `r3`. Also tried: if/else stores, a pointer
  local, a volatile read, and `/` with the `__divsi3` alias.
- **`SelectActorCategory`** (125 halfwords, both compilers). The function
  takes six arguments. The 5th goes to vtable slot 2, the 6th is `y`.
  The draft produces the ROM's instruction sequence, including the
  `table + 0x14 + i*0x14` address shape (an inline returning a pointer,
  so the load comes after the limit) and `&gActorDrawList` loaded
  before `mem_alloc`. But the `&gActorSpawnIndex` pseudo and the cached
  `sub_8029B2C()` value swap `r7`/`r8`, which adds two `mov`s. In the
  `-dg` dump the cached value (refs 7, live 105) outranks the address
  (refs 4, live 84). Goto and `for (;;)` forms of the first loop did not
  change that.
- **`RunActorCategoryFrame`** (about 150 halfwords). A first full draft had the
  right structure but needed `r9` on top of the ROM's `r8`, as the old
  note says. Not pursued further. No draft kept.
- **`InitActorCategory`**: not attempted. About 230 instructions and
  20+ calls.

## Verification

`rm -rf build && make NON_MATCHING=1 report` shows no warnings from the
touched files. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` gives `crashbandicootxs.gba: OK`.
