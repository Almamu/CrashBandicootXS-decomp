# Last-eleven NAKED retry

This pass retried the two drafts left by
[last-ten-naked-retry.md](last-ten-naked-retry.md). Both closed.

| Function | File | Before | Now | Compiler |
|---|---|---|---|---|
| `ResetLinkSessionState` (#4) | `src/link/link_session_reset.c` | 51 | match | old_agbcc + `-fno-rerun-loop-opt` |
| `SpawnFlamethrowerLabAssistant` (#31) | `src/graphics/graphics_loading_1feec.c` | 4 | match | old_agbcc |

## `ResetLinkSessionState`: closed

Three changes on top of the last-ten draft.

**0x1234 in a function-scope local.** The ROM loads 0x1234 separately
for each of the two magic stores:

```
ldr   r1, =0x1234
strh  r1, [r0]
adds  r0, #2
adds  r2, r1, #0
strh  r2, [r0]
```

So the constant had no hard register. Each use was an input reload of
a REG_EQUIV constant, and reload_cse turned the second `ldr` into a copy
of r1. In the draft, the constant was set inside the loop body.
loop.c rated moving it "not desirable" (savings 2, life 4, against a
threshold already cut by six earlier moves). It then stayed a
block-local pseudo in r1, used by both stores.

With `u32 magic = 0x1234;` declared at the top of the function and
`field_8 = (field_6 = magic)` in the loop, the set is outside the loop.
The pseudo is live across the whole loop, global allocation finds no
free register for it, and reload rematerializes it at each store, as
in the ROM. The extra reload also shifts reload's round-robin
register choice, so the tail's reload registers (r3/r7/r2 for the
0x3f0/0x3f4/0x3f8 offsets) came out right too. A `u16` local works when
declared at the top without an initializer (`magic = 0x1234;` before the
loop), but `u16 magic = 0x1234;` does not. Writing the two stores
separately (`field_6 = magic; field_8 = magic;`) is 78 halfwords off.

**Nibble address order.** `&self->players[i]` expands to
`(plus (mult i 200) self)`, which gives `adds r0, r2, r5`. With the
offset in a local first,

```c
s32 t = i * 0xc8;

((struct nibble_pair *)((u8 *)self + t + 0xd1))->lo--;
```

the add is built as `(plus self t)`, the ROM's `adds r0, r5, r2`.
`(s32)self + i * 0xc8` and `(u8 *)self + i * 0xc8` keep the old order.

**Tail read-back.** `v = *p400;` through the plain `u16 *` (not the
`vu16` read) gives the ROM's `ldrh r1, [r1]; ldr r0, =REG; strh r1, [r0]`.

The 13 `id` references and the one-insn padding `asm("")` from the
last-ten draft are still needed (removing any of them is 39-183
halfwords). Without `-fno-rerun-loop-opt` the final C is 147 halfwords
off and 16 bytes long.

## `SpawnFlamethrowerLabAssistant`: closed

Two changes on top of the last-ten draft.

**r3 hold after the call.** Between `AddToPartList` and the second flip,
the ROM reloads &gEntityFlags out of sb through r1
(`mov r1, sb`). The draft used r3. A hard-register hold on r3 from after
the call's `"m"` asm to the end of the `rec2` lookup removes r3 from
reload's choices there, and reload then picks r1:

```c
register s32 h3 asm("r3");
...
asm("" : "=r"(h3));
SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
rec2 = LEVEL_RECORD(arg3);
asm("" : : "r"(h3));
```

Holds on r0 or r2 over the same range are 102 halfwords off.

**Store after the argument move.** The ROM's `str r3, [sp]` comes after
`adds r1, r7, #0`, right before the `bl`, like a caller-save. With
`(q2 = ..., part)` as the argument, the store came first, because every
side effect of argument evaluation comes before the hard-register moves.
A copy of `part` that an empty asm keeps as its own pseudo fixes that:

```c
AddToPartList(gCollidableList, ({
    struct popup_part *t = part;

    asm("" : "+r"(t));
    q2 = (struct popup_bits *)((u8 *)part + 0x28);
    t;
}));
```

`t` is tied to r1, so `adds r1, r7, #0` happens at the copy, before the
`q2` store. A plain `(t = part, q2 = ..., t)` is merged back by CSE and
stays at 2 halfwords. `asm volatile` and the `"=r"`/`"0"` form also
match. Doing the same trick in the first argument does not help.

The last-nine r9 hold is not needed any more and was dropped. The
`rec2` references (9 halfwords without them) and the reloaded-pointer
reference (5 without) stay.

**Single-pointer drafts.** These still don't work. With one
part+0x28 pseudo live across the call, global allocation never takes the
caller-save path here:

- With default priorities, the second flip's byte (3 refs over 10
  insns) is allocated before the pointer and takes r3.
- Raising the pointer's priority with extra references gives it r5.
- Extra `arg3` references keep arg3 in r4, but then the pointer goes to
  r8 through the alternate class.

The best single-pointer variants are 61-62 halfwords. Holds on r0, r1,
r2, r4 and pairs around the first flip are 93-121 halfwords (`s1.py`
through `s6.py` in the helpers).

## Helpers

These are in the scratchpad's `last11/`:

- `d.py`, `var.py`, `rtl.sh`, `regs.sh` and `fnrtl.py`, as before.
- `t.sh`: runs `d.py` from the worktree. A relative source path is
  taken from `last11/`.
- `m2.py`/`m3.py`/`m4.py`: the `ResetLinkSessionState` variants: magic local,
  nibble and tail spellings, and reference-count ablations.
- `s2.py`/`s7.py`/`s8.py`: the `SpawnFlamethrowerLabAssistant` variants: holds, argument
  statement-expressions, and ablations.
- `cs_scan.py`: flags variants whose output has a `str rN, [sp]` right
  before a `bl`.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
