# NAKED retry: issues #4, #10, #15, #16 (third pass)

Seven NAKED functions the previous pass (PR #473) didn't reach. Two
closed; two got new or better drafts; three were not attempted.

| Function | File | Result |
| --- | --- | --- |
| `sub_8012AF4` | `actor_part83.c` | **Real C**, old_agbcc |
| `UpdateActionCtrl` | `actor_part84.c` | **Real C**, old_agbcc |
| `CreateWumpa` | `game_loop53.c` | Still NAKED, analysis added to the draft's note |
| `UpdateWumpa` | `game_loop53.c` | Still NAKED, first C draft under `NON_MATCHING` |
| `sub_8001DB4` | `link_cable.c` | Still NAKED, note on the draft extended |
| `sub_8002114` | `link_cable.c` | Not attempted (1488 B, no draft) |
| `UpdateEnemyCtrl` | `actor_part112.c` | Not attempted (1132 B, no draft) |

## Closed

### `sub_8012AF4` (issue #16)

The old draft kept the `+0x2F` flag and the `+0x27` tag in pointer
locals. The ROM computes each address once and then uses a copy, which
is what old_agbcc's GCSE does when the source re-reads the field. With
no pointer locals (`self->flag2F`, `self->next27` written out each
time), the control flow for the "state 0 or 0x11" test comes out right.
In the ROM, both paths set the flag address before the branch. Two more
changes were needed:

- the record lookup is `*(gStaticData_0816B304 + i)`, which loads the
  table address after the index is computed, as the ROM does;
- three empty `asm("" : : "r"(self))` extra references (next to the
  `next31 = f` store) settle the last register ties (the flag byte's
  copy in `ip`, the tag address in r3 and then r8).

A per-variant brute-force search went from 169 halfwords off to 18, 7, 4
and then a match.

### `UpdateActionCtrl` (issue #16)

The previous pass noted three problems: the PMF method record's stack
slot, the order of the `part->y` reads, and the signed `unk_94`
compare. Everything else was register choice. What fixed it:

- `s32 py = part->y` before each camera test (the ROM loads `y` first),
  with the second test reading `self->part` into its own local; the
  flag-clear block in between also gets its own local;
- `unk_94` read into an `s32` before the `<= 1` test (a direct `u8`
  compare is shortened to unsigned, giving `bhi` instead of `bgt`);
- the queued-state update is `ActQueue27(self, left, self->unk_2A[2])`:
  as an inline parameter, the queued byte is read before the three
  stores;
- the input-mask 0x100 goes through an empty `asm("=r" : "0"(0x100))`
  (`K100()`), so it isn't shared with the later 0x100 field offsets;
- the switch case bodies are in the ROM's order (0x13 first).

With those, the 8-byte stack slot for the PMF method record and the
`r7` push the old note blamed on a "spilled DImode pair" come out by
themselves. So did the first draft's "temporaries in r6".

## Not closed

- **`CreateWumpa`.** In the draft, `x`/`y`/`id`/`special` are live
  only in the first basic block, so local-alloc gives them r4-r6/r8
  before global-alloc places `self` (r7) and `mode` (sb). The ROM's
  layout (`self` r4, `mode` r7, `id` r8, `special` sb) is what global
  allocation of all of them gives. Extra references to them after the
  list `if`/`else` produce that (with `self` r4, the tag address r5 and
  `phase` r6), but they change how the parameters are copied in. Two
  other findings are recorded in the draft's note. Passing `phase` into
  the frame clamp as its start value reproduces the ROM's zero at
  +0x49 and the shared zero at +0x4B. `mode` is only referenced at all
  after `asm("" : "=r"(mode) : "0"(0))`. Best found: about 110
  halfwords off.
  *Later pass (gap4): closed.* Pinning `self` to r4 until the list
  `if`/`else` keeps local-alloc off r4, which gives the ROM's parameter
  registers and leaves r7 for `mode`. An unpinned copy is used after the
  join. See [gap4-naked-retry.md](gap4-naked-retry.md).
- **`UpdateWumpa`.** New draft, modelled on `UpdateExtraLife`'s
  (`game_loop54.c`). The mode-3 spawn uses `game_loop48.c`'s
  argP4/argP5 stack-argument trick. Control flow and calls are right,
  but the draft is 84 bytes long. In the ROM, cross-jumping merged the
  three "flags |= 1, set the id bit" tails: mode 1 jumps into the
  bit-set part, and modes 2 and 3 share the code from the `orr` on.
  Here the three copies get different registers, so none of them merge.
- **`sub_8001DB4`.** 136 halfwords off, same size. old_agbcc reverses
  the inner id-copy loop, which the ROM keeps counting up; do/while,
  goto, `continue` and explicit pointer forms don't prevent that. The
  ROM also rebuilds `self + 0xd0` inside the outer loop, and reads
  `field_400` back after storing it.
  `-fno-strength-reduce` is worse: the ROM's first loop *is* reversed
  and pointer-reduced.
- **`sub_8002114`, `UpdateEnemyCtrl`.** Not attempted: 1488 B and 1132 B
  with no C draft to start from.

## Techniques worth reusing

- **Drop pointer locals the ROM copies.** A pointer computed once and
  then copied into another register (`adds r3, r5, #0x27` ...
  `mov r8, r3`) comes from old_agbcc's GCSE over repeated `self->field`
  reads, not from a local. This closed `sub_8012AF4`.
- **Read a field into a local to fix load order.** If the ROM loads a
  field before a longer expression it is compared against, read the
  field into a local first.
- **`u8` compared against a constant.** gcc shortens the compare to
  unsigned. Going through an `s32` local keeps it signed.
- **A struct-copy stack slot can be a side effect.** The PMF method
  record's stack slot in `UpdateActionCtrl` needed no special handling once
  the rest of the function matched.

## Tools

Scratch copies of the brute-force variant runner (`polish2/brute2.py`)
and an apply helper. Nothing is committed.

`rm -rf build && make NON_MATCHING=1 report` (no warnings from these
files) and a clean `make compare` both pass.
