# Hard-register hold retry

This pass tried the hard-register hold from
[late-naked-retry-3.md](late-naked-retry-3.md) on six drafts whose gap
was register rotation or unused registers: `sub_800450C` (#6, raw asm),
`sub_80053F4` (#7), `sub_8023A1C` (#37), `sub_801A114` (#24),
`sub_800E08C` (#12) and `sub_802062C` (#31). Four closed. The raw
`asm/code_3_1_10_4.s` is gone.

## Closed (4)

| Function | File | Compiler | What it took |
|---|---|---|---|
| `sub_800450C` | `src/graphics/settings_menu.c` (already old_agbcc) | old_agbcc | Was 5 halfwords off. The pre-header fix from [early-rom-naked-retry-2.md](early-rom-naked-retry-2.md) (plain `u8 *`/`u16 *` stores, an SImode `-16` mask local) left 6: the third icon's `frameIndex = 0`. Splitting the store (`if (frame)` cast store, else a store through a pointer pinned to r0) puts the address in r0 as in the ROM. The ROM reloaded the 0, which advanced reload's round-robin. The pinned version doesn't, so the next nibble mask took r1 where the ROM has r3. An r1 hold at that mask, for frame 0 only, fixes it. The hold's asm statements swapped two stack slots. Three bare `asm("")` at the top of the function restore the order. |
| `sub_80053F4` | `src/graphics/settings_menu21.c` | both | Was 20 halfwords off. In each computed-x `set_icon_mgr_pos` call, an r2 hold over the x computation puts y in r2, and the address reloads then rotate as in the ROM (r4, r3, then r4/r6 for the `ldrsh` offset). The ROM also never ties x (r3) to the value it is computed from (r1). An extra `asm("" : : "r")` reference on `0xf0 - w` (first call, with `w` its own local) and on the 0x8c and `width` (second call) stops local-alloc tying them. |
| `sub_8023A1C` | `src/system/game_loop56.c` (object added to `OLD_AGBCC_OBJS`; only function in the file) | old_agbcc (agbcc 42 hw) | Was 15 halfwords off. 12 of them were the one-byte `direction` stack argument: as a QImode packed struct, the compound literal is built in a register before the slot address is computed. Adding a zero-length array member (`u8 pad[0]`) makes the struct BLKmode. The literal is then stored straight into the outgoing slot, address first, as in the ROM. The other 2 were the post-fade position copy. With an r0/r1 hold across the player-pointer load (and no r2 pin), both the global's address and the pointer land in r2. |
| `sub_801A114` | `src/graphics/actor_part_1967c.c` (already old_agbcc) | both | Was 159 halfwords off. As the gap4 note said, the ROM gives out registers in the draft's order but starting at r7. Holding r5 and r6 across the box builders (from `a = sub_8007C30(other)` to just before `sub_8001688`) gives 16. The rest was the state-0 BLDCNT orr chain, which the ROM builds in r5. A block-scoped `register u32 acc asm("r5")`, set with the constant-init asm (`asm("" : "=r"(acc) : "0"(BLDCNT_TGT1_OBJ))`) so it is neither folded nor moved after the first constant, matches. The outside `bld` variable is gone. |

## Not closed (2)

| Function | Before | Now | What's left |
|---|---|---|---|
| `sub_800E08C` (#12, `game_loop47.c`) | 49 (old_agbcc) | 49 (draft unchanged) | The case-3 reload is `(set r2 (mem:SI sp))`, because the spilled `f20` is a promoted SImode pseudo and nothing asks for its QImode part. A hold changes which register a reload gets, not its mode. `u32`/`s32`/`u16`/`s8` `f20` with a `(u8)` cast and calls through `u8`-parameter function pointers stay at 49-51 halfwords (or grow). `game_loop47.c` would also need `sub_0800D18C` to build under old_agbcc (it's NAKED, so that part is fine). |
| `sub_802062C` (#31, `graphics_loading_1feec.c`) | 62 | 62 (draft unchanged) | It isn't only the `arg3` register. The draft keeps `arg3 << 16` (the first half of the zero-extension) alive, spilled, to rebuild `arg3 * 2` for the second `LEVEL_RECORD`. The ROM zero-extends a copy in r4 and doubles it in place. A `u32` copy behind `asm("" : "+r")` gets arg3 into r4 but moves the extension after the first call (120). r4/r5 holds over parts of the part+0x28 range were 80 or worse. |

## Notes on the technique

- The hold works for global-alloc as well as reload. A hard register
  that is live across a region conflicts with every pseudo live there,
  so holding callee-saved r5/r6 across calls models "the ROM leaves
  these registers unused" (`sub_801A114`). The push/pop doesn't change,
  since the function already saves those registers.
- A hold inside an inline that is expanded several times (or behind a
  constant `if`) still adds RTL, which can shift temporary numbering and
  the stack-slot order. Bare `asm("")` padding at the top of the
  function restores it (`sub_800450C`).
- A register pin decides which pseudo gets a register; a hold only
  decides which registers are unavailable. Where the ROM put a *reload*
  in a register (not a pseudo), use a hold. Where it put a pseudo in a
  register the allocator would not choose, use a pin
  (`sub_800450C`'s r0 address, `sub_801A114`'s r5 accumulator).
- Not register-related, but new: a struct argument with a zero-length
  array member is BLKmode, and gcc stores a BLKmode compound literal
  directly into the argument slot, address first (`sub_8023A1C`).

Helper scripts (not committed) are in the scratchpad's `hold/`: `d.py`
(one-function diff), `vr.py` (variant runner: exact-once text
replacement, restores the file) and `rtl.sh`/`fnrtl.py`.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
