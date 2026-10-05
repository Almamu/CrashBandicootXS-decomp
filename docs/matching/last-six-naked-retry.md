# Last-six NAKED retry

This pass retried four NAKED functions that have C drafts under
`#if NON_MATCHING`. None closed. Two drafts got closer and are updated
in place.

| Function | File | Before | Now | Status |
|---|---|---|---|---|
| `HandleLinkSerial` (#4) | `src/link/link_handshake.c` | 16 | 6 (old_agbcc) | Draft updated |
| `ReceiveSaveTransferChunk` (#5) | `src/save/save_transfer.c` | 104 | 97 (4 bytes long) | Draft updated, both loops now match |
| `SpawnFlamethrowerLabAssistant` (#31) | `src/level/spawn_enemies.c` | 62 | 62 | Draft unchanged, note added |
| `DrawPauseFraction` (#7) | `src/menus/pause_menu_widgets.c` | 73 | 73 | Draft unchanged, note added |

## `HandleLinkSerial`: nibble test closed (16 to 6)

The ROM's `lsls #28` for `q[0]`'s low nibble comes before the
right-hand sum, and its `lsrs #28` comes right before the compare. The
pair comes from an HImode compare. `lo` is extracted into a QImode
value (`lsls`/`lsrs #28`). Its zero-extension to SImode is only emitted
by `emit_cmp_insn` when it widens the narrow compare, and combine
merges that extension with the `lsrs`, placing it at the compare. The
compare is narrow because `shorten_compare` sees a `u8` against a
`u16`:

```c
u8 lo = LINK_NIB(&q[0]).lo;
u16 sum = (u16)(LINK_NIB(&q[0]).hi + ((q[7] << 8) | q[6])) % 16;

if (lo == sum)
    ok = 1;
```

The `u16` side must be a local set before the compare. Written inline,
its own HImode re-extension is emitted after `lo`'s, so the `ands`
lands after the `lsrs`. A `u8` sum is also wrong: fold narrows the
addition and drops `q[7]`.

What is left is the first receive loop (6 halfwords):

- In loop.c each biv's giv list is built by prepending, and
  strength reduction creates the new registers and their inits in list
  order. When givs combine, the survivor is the one with the highest
  combine benefit. Ties go to the lower list index, which is the later
  insn. The ROM's inits are load (r1), `w` (r2), test (r3). So the
  surviving insns must be in the order test < `w` < load. Taking the
  test address first (`u16 *t = &d2[i]` at the top of the body) gets
  load/test right, but the `w` giv's survivor is the `w[i].hi` read,
  after the load. A `pw = &w[i]` DEST_REG giv doesn't absorb the
  DEST_ADDR uses (they win on benefit), and reading through `pw` lets
  CSE reuse the stored word, which drops the ROM's `ldrh r7`. All 48
  orderings of `t`/`pw`/`ps`, read, store and load forms were 6 halfwords
  or worse (`scratchpad/last6/s10.py`).
- The spilled counter reloads into r7 where the ROM uses r0. The ROM
  also uses r7 for the `ldrh` reload, and `find_reload_regs` spills r0
  for the counter insn, so the r7 must come from later in reload. This
  was not traced further.

## `ReceiveSaveTransferChunk`: loops matched (104 to 97)

Both copy loops now match instruction for instruction. That uses the
same shapes as `HandleLinkSerial`'s session pop: a `rd = &ch->readPos`
pointer for the bounds test and the wrap loop, and
`nw = 0; if (old != 0x7f) nw = old + 1;` so the 0 is set inside the loop
(the ROM's `movs r0, #0` before each `cmp #0x7f`). The count test is
written as a byte-offset sum `(u8 *)s + i * 0xc8 + 0x18c`, which gives
the ROM's `(i * 0xc8 + s) + 0x18c` operand order. An `asm volatile`
copy of the index feeding `&s->rx[j]` gives the ROM's second `muls`,
because CSE can't see through the asm.

Left: the ROM's second multiply is `muls r2, r1`, into the 0xc8
register, which dies there. The draft copies the index into r0 first,
because thumb `mulsi3`'s tied alternative needs the output in operand
1's register, and local-alloc can't tie to global pseudos. The
channel's +0x108 is also folded into the field offsets. Pinning
`s`/index/0xc8 to r3/r1/r2 (plus a `u8 **` for `writePtr`) gets 47
halfwords with the same instruction structure, but the rest is a
register permutation. It was not adopted. Escaping the index for the
count test instead (E1) or escaping `ch` after its computation were
worse.

## `SpawnFlamethrowerLabAssistant`: caller-save analysis (62)

global.c's `find_reg` gives a call-crossing pseudo a call-clobbered
register (with caller-save code) only if no callee-saved register is
free when it is allocated and `CALLER_SAVE_PROFITABLE` holds
(`4 * calls < refs`). local-alloc.c has the same fallback, but only for
block-local pseudos. part+0x28 spans the flip's branch, so it isn't
block-local. In the draft, the global allocation order (priority is
`floor_log2(refs) * refs / live_length`) is: part+0x28 (6 refs over 49
insns, 0.245), then hdr+0x84, the constant 1, `arg3 * 2`, the
zero-extended arg3 (3/63), -0x11 (3/86) and `&gEntityFlags`
(3/94). So part+0x28 takes r5 before arg3 and the pool address get
callee-saved registers. For the ROM's result, part+0x28 must be
allocated last among the seven. With the same ref counts that would
need a live range over 300 insns, so the ROM's pseudos must be shaped
differently (fewer part+0x28 refs, or arg3/`arg3 * 2` as one
longer-lived pseudo). An escaped `&gEntityFlags` local gets that
address into sl but is 88 halfwords.

## `DrawPauseFraction` (73)

`asm volatile` escapes on the second half's source manager,
destination manager or both, an escaped 0x110 offset (and 0x110/0x114
pair), and an escaped `&posX` were 67-75 halfwords under both
compilers. The ROM's second half looks like reload output. 0x110 and
0x114 are rematerialized into r6/r7 (`adds r6, r7, #0` from reload_cse,
`adds r7, #4` from move2add), so there they are unallocated constant
pseudos, while the first half's 0x110 got r7. The draft CSEs all 0x110
uses into one pseudo.

## Helpers

These are in the scratchpad's `last6/`: `d.py`, `var.py`, `rtl.sh` and
`fnrtl.py` (copies of `last4/` pointed at this worktree), and the
variant specs `s1`-`s11` (`HandleLinkSerial`), `e1`-`e9` (`ReceiveSaveTransferChunk`),
`g1` (`SpawnFlamethrowerLabAssistant`) and `m1` (`DrawPauseFraction`).
