# Last-seven NAKED retry

This pass retried the drafts of `HandleLinkSerial` (#4), `ReceiveSaveTransferChunk` (#5)
and, if time allowed, `ResetLinkSessionState` (#4). One closed and one draft got
much closer.

| Function | File | Before | Now | Status |
|---|---|---|---|---|
| `HandleLinkSerial` (#4) | `src/system/link_cable.c` | 6 | match (old_agbcc) | Real C |
| `ReceiveSaveTransferChunk` (#5) | `src/graphics/settings_menu8a2.c` | 97 (4 bytes long) | 14 (same size, both compilers) | Draft updated |
| `ResetLinkSessionState` (#4) | `src/system/link_cable.c` | 136 | 136 | Not attempted |

## `HandleLinkSerial`: closed

What was left was the first receive loop. The loop dump shows why.
loop.c scans for givs starting at `scan_start` (the loop entry) and
prepends each giv to its biv's list. New registers and their inits
are created in list order. When givs combine, the survivor is the one
with the highest total benefit, and ties go to the later insn. The ROM's
inits are load, `w`, test. So the surviving insns must be found in the
order test, `w`, load. But the load reads memory before `w[i]` is
stored and read, so no survivor of the load giv can come after
`w`'s. (A `pw = &w[i]` DEST_REG giv that only an asm uses does get the
load/test pair right. But CSE makes the store and read use `pw`'s
register, so `combine_givs_benefit_from` gives it 0 for them. It loses
11 to 12 and ends up as a fourth pointer.)

The answer is that the load isn't a giv. It goes through a pointer biv
`p` (`w[i] = *(struct link_rx_word *)p; ... p++;` at the end of the
body), which strength reduction leaves alone. The test address is taken
first (`u16 *t = &d2[i]`), so the two remaining givs are reduced in the
order `w`, test. That gives r1/r2/r3 for load/`w`/test, and the
spilled counter reloads into r0 as well.

A biv's init stays where the source puts it, while loop invariants are
hoisted to just before the loop. So `p = data` came before the hoisted
0xF0B/0xffff and `self + 0x20`. The fix sets all three by hand
before `p`, in the ROM's order:

```c
f20 = &self->field_20;
asm("" : "=r"(kid) : "0"(0xF0B));
asm("" : "=r"(kfree) : "0"(0xffff));
p = data;
```

The constants use the constant-init form, so loop has nothing to hoist.
`f20` is used only for the `f20->lo = nId` store after the loop. The
following `field_400` copy still reads `self->field_20` (the ROM's
`ldrh r0, [sl, #0x20]`).

## `ReceiveSaveTransferChunk`: 97 to 14

- **One 0xc8 register.** thumb `mulsi3`'s output is earlyclobber
  (`=&l`), so local-alloc never ties it to an input. The ROM's
  `muls r2, r1` into the 0xc8 register is two things: a single 0xc8
  value live across both products, and a second product that reuses
  its register. A `register s32 c asm("r2") = 0xc8` does both. The
  first product is `pi * c`, written `(u8 *)(pi * c + (s32)s) + 0x18c`
  for the ROM's `adds r0, r0, r3` operand order. The second is
  `c = c * pi + (s32)s`. CSE doesn't share the two because `c` changes.
  The `s` and index pins from the last pass aren't needed.
- **Channel base.** `ch = (struct sio_channel *)(c + 0x108)` with an
  `asm volatile("" : "+r"(ch))` escape keeps the +0x108 out of the field
  offsets. `dst = *wp` is loaded after `ch` through `wp = &self->writePtr`,
  which is taken first (the ROM's `add r0, ip` before the multiply).
- **`n` in r6.** Three extra references (`asm("" : : "r"(n))`) raise
  `n`'s global-alloc priority, so it is allocated before `rd` and
  the loop pointers. One or two references do nothing.

Left (14 halfwords, a register permutation): `rd` gets r7 (ROM r5). In
the wrap loop the draft uses r5/r2/r1 for the count pointer, ring
pointer and `old`, where the ROM uses r1/r7/r2. Extra references on
`rd` (1-3, alone or combined with `n`'s) were 18-30.

## Helpers

These are in the scratchpad's `last7/`: `d.py`, `var.py`, `rtl.sh`, `fnrtl.py`
(copies pointed at this worktree), the specs `s1`-`s3` (`HandleLinkSerial`)
and `e1`-`e10` (`ReceiveSaveTransferChunk`).

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
