# Early-ROM NAKED retry 2

This pass retried the five early-ROM functions still left: the raw
`InitSaveMenuIcons` (issue #6), `PauseMenuLoop` (#7), and `MakeLinkHandshakeId`,
`ResetLinkSessionState` and `HandleLinkSerial` (#4). Two closed. Two drafts moved
closer, and one did not.

## Closed (2)

| Function | File | Compiler | What it took |
|---|---|---|---|
| `PauseMenuLoop` | `src/menus/pause_menu_loop.c` (object added to `OLD_AGBCC_OBJS`) | old_agbcc only (agbcc is 4 bytes longer) | The input loop is a plain `for (;;)`. It confirms or plays SFX 0x48 on A, and breaks on B at the bottom. old_agbcc's loop rotation moves the B test to the loop top and enters at the body, which is the ROM's `b body; top: B test; body: ...` layout. The old goto form got that layout but not the pre-header. GCSE carries the fade pointer (`self + 0xcc`) from the fade-out loop into the fade-in loop. It inserts it at the end of the block after the fade-out loop. With `disp = &self->field_d0` in the same block, `disp` came first. Taking `disp` only after the fade-in loop puts the fade pointer first, as in the ROM. The two register pins from the earlier draft (`pressed` in r1, `key` in r3) are still needed. |
| `MakeLinkHandshakeId` | `src/link/link_handshake.c` (already old_agbcc) | old_agbcc | No pins or `asm`. `hash` is a u32, truncated with a `(u16)` cast each step and read back as `(u16)hash >> 8` into a u32 `hi`. With a u16 `hash`, CSE folded `hash >> 8` into `(x << 16) >> 24` of the loop's zero-extend temporary. The ROM's `sub r0, #0x1f` is reload's move2add building -16 from the 15 already in r0. It only fires when the 15 is in a mode at least as wide as the -16. The bitfield store of `n + v` masks it with a QImode 15, so the fix is to mask in SImode first (`w = (n + v) & 0xf`) and store `w`. |

## Not closed (3)

| Function | Before | Now | What's left |
|---|---|---|---|
| `InitSaveMenuIcons` (raw) | 5 | 5 (a 6-halfword variant has the right pre-header) | old_agbcc expands every narrow struct-field store as a read-modify-write. CSE leaves the RMW's zero mask behind as a dead constant set, and the loop pass counts it as a movable (`field_3c`'s HImode 0, or `frameIndex`'s QImode 0 when `field_3c` is stored through a cast). The draft's first pass moves the address, 15, and the QImode/SImode pair of -16s before it reaches 0x80. Each move lowers the threshold by 3, so 0x80 is "not desirable". Storing `frameIndex` and `field_3c` through plain `u8 *`/`u16 *` casts, and inserting the nibble with an SImode `-16` local, leaves three moves. The pre-header then matches exactly. But the third icon's `frameIndex = 0` becomes a zero pseudo (r0) stored after its address (r1). The ROM reloads a constant into r1 after the address in r0, which is what the RMW-folded store gives. Splitting the store (`if (frame)` cast store, else struct store) gets the order but not the registers. Also tried: an extra argument or local for 0x80, `asm volatile` on the address load, macro instead of inline, several nibble spellings. |
| `ResetLinkSessionState` | 136 | 136 | An inner `for` over explicit src/dst pointers with `asm("" : "+r"(j))` in the body keeps the counter counting up, with the ROM's in-loop `mov #0xff`. A goto loop also counts up, but then combine folds `w & 0xff` into a second `ldrb`. The outer loop then hoists 0x1234, 0 and `self + 0x104`. The ROM hoists only 200, `self + 0x100`, `self + 0xfc`, `self + 0x20` and the id pointer, in that order, which is not body order. That variant is 163 halfwords off, so the old draft stays. |
| `HandleLinkSerial` | 514 | 422 | A u32 `one`, assigned just before the store, holds the `field_4 = 1` constant, and both SIOCNT bit tests use it. With a literal 1 the store's QImode 1 is not shared. An s32 or u8 `one` initialised at its declaration was worse (544). The first receive loop tests 0xffff through a second pointer `d2`. `field_400` is stored through a pointer taken before the `field_20` load. Left: the ring push/pop loops. The ROM recomputes the ring field addresses in each loop pre-header, where CSE here reuses the bounds-test address. The ROM also keeps `n` in r7, where here it reuses the r8 byte. Four bare `asm("")` statements at the top fix a swapped pair of stack slots (419), but the draft doesn't use them. |

## Techniques worth keeping

- **Dead RMW masks count as loop movables.** Under old_agbcc a `u8`/`u16`
  struct-field store expands to `(old & 0) | v`. CSE folds that, but
  it can leave the `set (reg) (const_int 0)` behind until flow. That
  dead set takes up one of the loop pass's threshold moves. Casting the
  field address to a plain pointer avoids the RMW.
- **move2add needs matching modes.** A constant the ROM builds as
  `sub rX, #K` from a neighbouring constant is reload's move2add. It
  only fires when the earlier constant's mode is at least as wide as
  the new one's. Moving a mask out of a bitfield store into an SImode
  local is enough.
- **GCSE inserts at the end of a block.** An expression GCSE carries
  into a later loop lands after everything else in the block before
  it. If the ROM has it earlier, move the other statements out of that
  block.

Helper scripts (not committed) are in the scratchpad's `early2/`:
variant specs `s450*.py`, `s51*.py`, `scb8*.py`, `sdb4*.py` and
`s2114*.py`, a function-level RTL extractor, and the loop-dump filter.

## Verification

`rm -rf build && make NON_MATCHING=1 report` shows no warnings from the
touched files. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` prints `crashbandicootxs.gba: OK`.
