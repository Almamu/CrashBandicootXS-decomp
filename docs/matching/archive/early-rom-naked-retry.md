# Early-ROM NAKED retry (`0x080019F8`-`0x08005E5C`)

This pass retried 14 functions: the NAKED transcriptions left in issues
#3-#7, the raw `InitSaveMenuIcons`, and the raw `PlaySfx`. Each was tested
under both compilers. Two closed. Five drafts moved closer, and one
function that had no draft now has one.

## Closed (2)

| Function | File | Compiler | What it took |
|---|---|---|---|
| `PlayAmbientSfx` | `src/audio/audio.c` | both | Written fresh as C, it matched on the first compile. The fifth argument is a one-byte struct passed by value, which gives the `add rX, sp, #0x14; ldrb` read. The volume is read as `gSfxTable[id].baseVolume`. The old note said the ROM "recomputes" this address where C would CSE it. In fact `base + 8 + offset` is simply how gcc computes a non-zero field offset, so there was no CSE gap. |
| `DrawYesNoPrompt` | `src/save/save_menu_draw.c` (old_agbcc object) | both | The r8/sb swap is a global-alloc priority tie. The 0x87 constant ranks 8/112 = 0.0714 and the 0x130 offset ranks 14/200 = 0.070, so the constant gets r8 first. `y` is now pinned to r9. It is set inside a block, after `x = 0xa0 - w` and after the manager pointer is loaded into a local, which keeps the ROM's `ldr r4, [r7]` ahead of `mov sb, r2`. The unpinned allocator then gives 0x130 r8. |

A pre-existing `initialization makes pointer from integer` warning in
`DisableMusicVCountIrq` (same file as `PlayAmbientSfx`) is fixed with a `(vu8 *)`
cast. The code is unchanged.

## Not closed (12)

| Function | Before | Now | What's left |
|---|---|---|---|
| `PlaySfx` (raw, draft in `audio.c`) | 4 | 4 (unchanged) | Without the `self` pin, and passing `gSfxVoiceToggle` directly, every register matches. The only difference then is that the toggle is loaded after `chanArg`. Loading it first (a `toggle` local) permutes r8/r9/sl: the address pseudo ranks 12/88 against `self` at 8/57 and `id` at 8/58. No spelling of the retry or tail changed that. |
| `MakeLinkHandshakeId` | 49 | 11 (old) | The fill loop is written over an integer address. That gives the ROM's signed `cmp; bge`, which the ROM got from strength-reducing `self[i]`; gcc here declines with "giv not worth while". A `c = 0xec` local ahead of it puts the constant load first. The hash loop is `tbl[idx] ^ (hash << 8)`. In the tail, the high nibble is read before `(hi << 8) \| self[6]` is formed. Left: after the loop, CSE folds `hash >> 8` into `(x << 16) >> 24` of the zero-extend temporary, and -16 comes out as `mov #16; neg` instead of the ROM's post-reload `sub r0, #31`. |
| `ResetLinkSessionState` | 136 | 136 | Not re-attempted beyond triage. |
| `UpdateLinkSession` | 37 | 37 | The ROM holds two separate constant-1 registers: sb for the test, `field_8` and IME, and r1 for the arm3 `eor`/`and`. One `one` local gives `bic`. Not converged. |
| `HandleLinkSerial` | - | - | Not attempted (1488 B). |
| `ValidateSaveData` | 18 | 6 | Pointing the marker pointer at r6 (pinned) and using a `u8 z = 0` local makes every store and every register match. Left: the ROM computes `&flags` before `&field_1fb` but still gives `flags` r7. Computing them in that order here makes `field_1fb` live one insn shorter (14 vs 15), so it wins r7. Pinning `field_1fb` to r8 or `version` to r9 makes it worse. |
| `ReceiveSaveTransferChunk` | no draft | about 100 | There is now a C draft. It keeps the ROM's `n - 1 != -1` loop tests (the old note said C folds them). Left: the ROM computes `playerIndex * 0xc8 + s` twice (count test, then channel pointer plus 0x108); gcc CSEs the second. `-fno-cse-follow-jumps`/`-skip-blocks`/`-rerun-cse-after-loop` don't split it. |
| `InitSaveMenuIcons` (raw) | 5 | 5 | A loop dump explains the pre-header order. loop.c's first pass moves four invariants: the `gSpriteBankSet` address, 15, and the QI and SI -16 of the nibble insert. Each move lowers the threshold by 3, so the 0x80 (84 insns of 117) is "not desirable" until the rerun, after strength reduction has emitted the pointer copies. The ROM moved it in pass 1. Nibble spellings, `field_3c` store forms, dropping `Opaque`, loop shapes, `-fmove-all-movables` and `-freduce-all-givs` don't fix it. |
| `RunPauseMenu` | 54 | 54 | The 0x12c offset (4 refs / 36 insns) outranks the icon-manager address pseudos, so it takes a low register. The ROM leaves it without a register and rematerializes it. Pinning the three global addresses to r6/r4/r5 then gives 0x12c r8 (80 halfwords). |
| `PauseMenuLoop` | 14 (old) | 5 (old) | `pressed` pinned to r1 keeps the word load plus `lsr #16`; unpinned, combine turns it into `ldrh [keys+2]`. `key` pinned to r3, with the pressed test spelled using the literal, gives the ROM's two `mov #K`. Left: the ROM computes the fade pointer (self+0xcc) before `disp`. A `fade` local changes the fade loops. |
| `DrawPauseMenu` | 20 | 20 | The two computed-x positions: the ROM puts x in r3 and y in r2. None of an x local, setter argument orders, blocks, direct stores, pins, or the save_menu_draw.c macro shape changed it. |
| `DrawPauseFraction` | 73 | 73 | Not re-attempted beyond triage. The ROM derives 0x114 as `r7 + 4` in the last reposition only. |

## Techniques worth keeping

- **Read the global-alloc dump (`-dg`) before trying variants.** In both
  `DrawYesNoPrompt` and `ValidateSaveData` the "swap" was a priority tie
  (`floor_log2(refs) * refs / live_length`), close enough to compute by
  hand. That tells you which pseudo to pin, or which live range has to
  change.
- **Read the loop dump (`-dL`).** "Not desirable" lines and the
  `threshold -= 3` per move explain pre-header orders that source
  reordering can't reach.
- **Combine turns `(u32 >> 16) & K` on a word load into a narrower
  `ldrh`.** Pinning the shifted value to a hard register stops it.

Helper scripts (not committed): a loop-dump filter, and brute/try
variant specs for each function, in the scratchpad's `early3/`.

## Verification

`rm -rf build && make NON_MATCHING=1 report` shows no warnings from the
touched files. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` prints `crashbandicootxs.gba: OK`.
