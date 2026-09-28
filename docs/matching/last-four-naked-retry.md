# Last-four NAKED retry

This pass retried four NAKED functions that had C drafts under
`#if NON_MATCHING`: `sub_800A884` (#9), `sub_8007634` (#9),
`sub_8001DB4` (#4) and `sub_8002114` (#4). One closed. `sub_8002114`
went from 422 halfwords off to 16.

## Closed (1)

| Function | File | Compiler | What it took |
|---|---|---|---|
| `sub_800A884` | `src/graphics/actor_part78.c` (object added to `OLD_AGBCC_OBJS`; only function in the file) | old_agbcc | Was 127 halfwords off. The ROM's "walking" flag offsets (`adds r1, #3`, `subs r2, #3`) are reload's move2add: a constant is reloaded into a register that already holds a nearby constant. That only happens when reload's round-robin keeps landing on the same registers. An r3 hold (no code) over the `self+0x70` method lookup puts the `ldrsh` index reload in r2. A second r3 hold, from the busy-flag clear to after the kind switch, keeps the flag resets and all switch cases rotating through r0-r2. The case-1 method call needs its own r3 hold (the call in between ends the outer one). Three smaller fixes: the kind-5/7/10 `= 1` stores set the 1 with the constant-init asm (`asm("" : "=r"(v) : "0"(1))`) so it comes before the address, which also stops the kind-5 tail being cross-jumped; the case-1 `unk_8c = 0` stores an r0 register variable through a pointer taken first; and the offset-table switch result is an r3 register variable. |

## Not closed (3)

| Function | Before | Now | What's left |
|---|---|---|---|
| `sub_8002114` (#4, `link_cable.c`) | 422 | 16 (draft updated) | See below. |
| `sub_8001DB4` (#4, `link_cable.c`) | 136 | 136 (draft unchanged) | Five separate gaps, none closed on its own. (1) The first id loop: the ROM sets its pointer before the hoisted 0xff and the count. Here SR's giv init lands after the 0xff. (2) The ROM builds the inner copy's dst giv as `i * 200 + (self + 0xd0)` in the inner pre-header with a fresh `mov #200`. Here `self + 0xd0` is hoisted to a stack slot, which also makes the frame 0x14 instead of 0x10. (3) The inner copy counts up in the ROM. `asm("" : "+r"(j))` does that, but the outer loop then hoists `0x1234` and `self + 0x104` as well. (4) `self + i * 200` operand order. (5) The ROM re-reads `field_400` after storing it. A `vu16` read does that but loads the SIOMLT address first and pads the pool by 4. Tried: escaped `self`/`players` pointers for the dst, pointer-form first loop, pointer tails. None beat 136. |
| `sub_8007634` (#9, `graphics_7634.c`) | 468 | 468 (draft unchanged) | The frame-struct idea was tried: one `struct frame7634` laid out at the ROM's offsets (oam at 0, part 8, pos 0xc, total 0x10, info 0x14, tile 0x18, scale 0x1c, base/half/pull 0x20-0x34, i 0x38, id 0x3c, tiles 0x40, flags pointer 0x44). The frame size and the prologue's parameter stores match, but 481 halfwords off under old_agbcc and 472 under agbcc. In memory, the OAM bitfields become byte read-modify-writes where the ROM does word ones. CSE also reuses stored values that the ROM reloads (`f.id` right after its store). The ROM's pattern (every use reloads its stack slot, and each zero-init store uses a different register) is reload spilling pseudos, not memory locals. Moving the six zero-inits to just before the loop gives 426 with the existing draft. That was not adopted, since the draft doesn't otherwise follow the ROM's order. |

### `sub_8002114` changes (422 to 16)

- One bare `asm("")` at the top (instead of the four noted before)
  fixes the swapped stack-slot pair.
- Receive loop: the 0xffff test is `d2[i]` (a second base pointer,
  indexed) instead of an incremented `d2`. Both pointers are then SR
  givs initialised after the hoisted 0xF0B/0xffff, as in the ROM.
- Player ring push: an extra reference `asm("" : : "r"(n))` gives `n`
  its own register (r7). The bounds test goes through an escaped copy
  of `p` (`({ struct link_player *_q = p; asm("" : "+r"(_q)); _q; })`),
  so CSE no longer shares the address with the loop pre-headers.
- Session pop: pointer locals `id`, `ring`, `cnt`, `dst`, `rd` are set
  right after `field_3f0 = 0`, before the id copy, in the ROM's order.
  The id copy is a pointer loop (`d`/`s`), which puts its pointers
  before the hoisted 0xff. The loops are an inline `LinkRingPop(r, rf, dst, n, rd)`.
  Its fast loop uses `rf`, an escaped copy of `ring` taken after the id
  copy (the ROM's `adds r4, r7, #0`). The wrap loop uses
  `nw = 0; if (old != 0x7f) nw = old + 1;`, so the 0 is set inside the
  loop.
- The `lo = nib` nibble store is an `s8` read-modify-write
  (`*b = (*b & ~0xf) | nib`): -16 mask, `nib` not masked again.
- The 0 for `field_3c = 0` is an r0 register variable set before the
  hash store.
- The `hi++` after `field_3f4` is written `(hi + 1) & 0xf`, which the
  ROM masks explicitly. The final `lo = hi + id[6..7]` is the
  `sub_8001CB8` form: an SImode `w = (n + v) & 0xf` through an `id`
  pointer. The `lo`/`hi` byte pointers are `(u8 *)self + field_38 * 2`
  plus the field offset.

What is left (16 halfwords):

- The first receive loop: the ROM's `data[i]` load giv is r1 and the
  0xffff test giv r3. Here they are swapped, and the spilled loop
  counter reloads into r7 where the ROM uses r0. In gcc's loop.c the
  giv list is built by prepending, and new registers and inits follow
  list order. So the ROM's list has the load giv first, which means it
  was discovered last. Tried: swapping which pointer each access uses,
  a pointer biv for the load (right registers, but its init moves
  before the hoisted constants), pins, extra references, a BLKmode
  copy.
- The `q[0]` low-nibble test: the ROM does `lsls #28` before the
  right-hand sum and `lsrs #28` just before the compare. Here both
  shifts come first. Operand order, a `u32` lo local, `& 0xf` forms
  and a shifted local were no better.

## Notes on the techniques

- **move2add "walking" constants** are a reload-register effect. When
  the ROM builds `self + 0x100/0x102/0x103` with `adds rN, #k` from the
  previous constant, reload has reused the same spill register. A
  hard-register hold on the register the ROM leaves out of the
  rotation (r3 here) fixes the whole chain. A call ends a hold (the
  register is call-clobbered), so a region with calls needs a hold on
  each side.
- **Empty asm statements count toward branch-length estimates.** A hold
  inside the case-1 method call pushed a `beq` over the short-branch
  limit (`bne; b` instead of `beq`). Dropping a redundant hold fixed
  it.
- **An escaped copy of a struct pointer** (`asm("" : "+r"(p))`) used
  only for one access stops CSE from sharing that field address with
  later blocks, which then recompute it from the original pointer.
- **Inline parameter copies** give the ROM's "same pointer in two
  registers" shape. A second pointer parameter, fed by an escaped copy
  taken where the ROM makes it, places the copy where the ROM has it.

Helper scripts (not committed) are in the scratchpad's `last4/`: `d.py`
(one-function diff), `var.py` (parallel variant runner), `rtl.sh`,
`fnrtl.py`, and variant specs `s*.py` (sub_800A884: s1-s14,
sub_8002114: s21-s52, sub_8001DB4: sdb*.py, sub_8007634: s31-s32,
`f7634.c`).

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
