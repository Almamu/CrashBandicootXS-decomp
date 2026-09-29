# Last-eight NAKED retry

This pass retried four NAKED functions with C drafts under
`#if NON_MATCHING`. One closed and two drafts got much closer.

| Function | File | Before | Now | Status |
|---|---|---|---|---|
| `sub_8002E20` (#5) | `src/graphics/settings_menu8a2.c` | 14 | match (both compilers) | Real C |
| `sub_802062C` (#31) | `src/graphics/graphics_loading_1feec.c` | 62 | 12 (same size, old_agbcc) | Draft updated |
| `sub_8005E5C` (#7) | `src/graphics/settings_menu16.c` | 73 (16 bytes long) | 45 (same size, both compilers) | Draft updated |
| `sub_8001DB4` (#4) | `src/system/link_cable.c` | 136 | 136 (122 found, not adopted) | Note added |

## `sub_8002E20`: closed

What was left was a register permutation in the wrap loop. The greg dump
explains it:

- `old` has the highest priority of every global pseudo (8 refs over
  10 insns, since refs are weighted by loop depth). In pass 0 of
  `find_reg` it skips registers some conflicting pseudo prefers.
- The ring pointer (`ch + 4`) prefers r2. It copies that preference
  from `ch`, and `ch` has it because `ch = c + 0x108` with `c`
  pinned to r2 (`set_preference` treats reg+const as a copy).
- So `old` takes r1 and the ring pointer takes r2 after `ch` dies.
  With r1 gone, the count pointer ends up in r5 and `rd` in r7. The ROM
  has count/`old`/ring/`rd` in r1/r2/r7/r5.

Two changes fix it:

```c
c = c * pi + (s32)s;
asm volatile("" : "=r"(ch) : "r"(c + 0x108));
...
i = n - 1;
if (i != -1)
{
    register s32 *cnt asm("r1") = &ch->count;

    do { ... (*cnt)--; ... } while (--i != -1);
}
```

- **No preference on `ch`.** An asm with a plain `"r"` input, instead
  of the old `"+r"` escape on a copy, gives `ch` no copy preference for
  r2. The ring pointer then has no preference either, so `old` can take
  r2.
- **Count pointer pinned to r1**, the ROM's register. The loop becomes
  an explicit `if` + `do`/`while` so the pin is set after the
  zero-trip test, like the ROM's `adds r1, r2, #0; adds r1, #0x84`.
  Then `rd` gets r5 and `n` gets r6 without the three extra references
  the last pass needed.

Neither change alone was enough: 20 halfwords (pin only) and 37 (asm
only).

## `sub_802062C`: 62 to 12

The ROM's `str r3, [sp]` / `ldr r3, [sp]` around `sub_8008E94` isn't
one long-lived part+0x28 pseudo. It can be two:

- the first flip's part+0x28, block-local (r3);
- a stack-resident copy for the second flip.

```c
sub_8008E94(gUnknown_030012F0, (q2 = (struct popup_bits *)((u8 *)part + 0x28), part));
asm("" : : "m"(q2));
```

The `"m"` operand makes `q2` addressable, so it lives in a stack slot.
Then arg3 and hdr+0x84 get r4/r5 as in the ROM. The alternatives were
much worse:

- `q2` as a register pseudo (plain copy, or `"+r"`/`"r"` escapes):
  62-105 halfwords;
- `volatile q2`: 75.

`struct popup_bits` must be larger than a word (it's padded to 0x20).
A 4-byte struct is accessed as SImode, so the flip read became a word
`ldr`.

Left (12 halfwords):

- the `str` comes one instruction early, before `adds r1, r7, #0`
  (the ROM's placement looks like a caller-save);
- `q2`'s reload registers are r4/r3 swapped;
- `&gUnknown_030012B4` and -0x11 are in sl/r9 where the ROM has
  r9/sl. A `tb = &gUnknown_030012B4` local with extra references was
  84+.

## `sub_8005E5C`: 73 to 45

The ROM's second half builds its offsets from r7 during reload
(`adds r6, r7, #0` from reload_cse, `adds r7, #4` from move2add), so its
0x110/0x114 aren't CSE'd with the first half's. The draft now wraps
each posX offset, and the second half's posY load offset, in an opaque
constant:

```c
#define OFF(K) ({ s32 _o; asm("" : "=r"(_o) : "0"(K)); _o; })
#define AT(m, o) (((struct { u32 v; } *)((u8 *)(m) + (o)))->v)
```

The posY stores keep plain constants, which reload rebuilds. `AT` goes
through a one-field struct. With a plain `*(u32 *)` store, it counts as
a non-struct access that may alias the `gUnknown_030012DC` pointer, so
the pointer gets re-read. `x`/`y` are block-local in each half and the
manager pointers are locals (`pdc`/`pe0`). This gives the ROM's size
and prologue, and 10 instruction lines differ after register
normalisation.

Left:

- the `&gUnknown_030012E0` load is hoisted to the top;
- the first half's 0x110 goes to r2 instead of r7;
- the second half rematerialises 0x110 instead of copying it from r7.

Things that were worse:

- a first-half 0x110 that stays live into the second half (copied, or
  `+ 4` for posY): 70-72;
- extra references on the pointer locals: 70.

## `sub_8001DB4`: notes only

- **Inner copy destination.**
  `((struct link_player *)((u8 *)self + 8))[i + 1].id` reproduces the
  ROM's `i + 1` precompute. It also builds `self + 0xd0` in the inner
  preheader instead of a stack slot.
- **`field_400` re-read.** A `vu16` read reproduces it.
- Together they are 122 halfwords, but the instruction diff grows (52 to
  77 lines), so the draft wasn't changed.
- **Inner loop reversal.** The inner loop is still reversed. In the
  rerun loop pass, `j` has no givs left (strength reduction turned them
  into pointer bivs), so `check_dbra_loop`'s `no_use_except_counting`
  holds. With 2 stores, a non-reversed ROM loop needs `j` to still have
  a giv in that pass.
- **First loop.** A pointer-biv first loop gets its pointer/0xff order
  right but loses the `self + 0x30` register.
- **Also tried:** a goto inner loop with pointer bivs and an opaque 0xff
  (151-171), `continue`/`break` exits, and
  `(u8 *)self + i * 200 + 0xd0` spellings.

## Helpers

These are in the scratchpad's `last8/`:

- `d.py`, `var.py`, `rtl.sh` and `fnrtl.py`, pointed at this worktree;
- the variant specs `e1`-`e4` (`sub_8002E20`), `l1`-`l5`
  (`sub_8001DB4`), `m1`-`m9` (`sub_8005E5C`) and `g1`-`g6`
  (`sub_802062C`).

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
