# Retrying issues #4, #6 and #8's NAKED functions

Issues #4 (`0x08001C80`-`0x08002C84`), #6 (`0x08003B40`-`0x08004CB4`) and
#8 (`0x080060AC`-`0x08006600`) had 18 NAKED transcriptions and one raw
function (`InitSaveMenuIcons`) between them. None had a C draft left. This
pass wrote each one again as C and tested it under both compilers. It
closed 12 of the 19.

| Issue | Closed | Left |
|---|---|---|
| #8 | 5 of 5 | none |
| #6 | 7 of 9 | `DrawYesNoPrompt` (NAKED), `InitSaveMenuIcons` (raw) |
| #4 | 0 of 5 | `MakeLinkHandshakeId`, `ResetLinkSessionState`, `UpdateLinkSession`, `HandleLinkSerial`, `ValidateSaveData` |

**Compilers:**

- `power_dialog_loop.c` (`PowerDialogLoop`) only matches under old_agbcc and
  is now on `OLD_AGBCC_OBJS`.
- `save_menu_draw.c` is also on `OLD_AGBCC_OBJS`. Its five matched
  functions compile the same under both compilers, but the
  `InitSaveMenuIcons` draft at the end of the file is 5 halfwords off under
  old_agbcc and 23 under agbcc. The ROM loads that function's nibble
  mask before the byte it combines with, which is old_agbcc's tell.
- `src/link/link_handshake.c` is on `OLD_AGBCC_OBJS` for the same
  reason. Its one matched function, `LinkStop`, compiles the same
  under both. The `MakeLinkHandshakeId` and `ResetLinkSessionState` drafts are closer
  under old_agbcc, and the ROM's `MakeLinkHandshakeId` has the
  mask-before-`ldrb` order.
- Everything else here matches under both compilers and stays on
  agbcc.

No file needed splitting. The `.if NON_MATCHING == 0` guard in
`asm/code_3_1_10_4.s` keeps `InitSaveMenuIcons`'s raw bytes out of the
NON_MATCHING build, where the C draft replaces them.

## The techniques that closed them

- **Icon-manager draws are virtual calls.** `_call_via_r2` is
  `_call_via_r2`, so `record->slots[0]` (measure) and
  `record->slots[2]` (draw) are gcc 2.x virtual calls. The call is a
  statement-expression macro, `ICON_TEXT_CALL(mgr, n, label)`, with
  `_m`/`_s` locals, so `this` is computed before the label argument.
  The ROM does the same, even when the label is itself a
  `GetUiText(...)` call. `set_icon_mgr_pos(m, u32 x, u32 y)` is a
  plain inline setter. This closed `DrawPauseTimeTrialPage`, `DrawPauseCrystalsPage`,
  `DrawPauseMenuPageTitle`, `DrawSaveMenuCancel`, `DrawEmptySlotLabel` and `DrawSaveMenuTitle` on the
  first compile. Their old notes described a "last mile" register gap
  that doesn't exist once the calls are written this way.
- **`ShowPowerDialog`** uses the `IconSetup`/`IconReserve` inline helpers
  from `RunLevelSelect` (`src/menus/level_select.c`), which has the
  same display/icon-manager setup sequence. It needed one more fix:
  `FontResetPalette` takes one argument. The old two-argument declaration
  added a `movs r1, #0` before each call.
- **`PowerDialogLoop`** (old_agbcc): `field_24` is a
  `struct { u8 level:5; u8 rest:3; } __attribute__((packed))`. Without
  `packed`, agbcc pads it to 4 bytes and moves `field_28`. `field_28`
  is a `u16`/`{u8, u8}` union, because it is cleared as a halfword and
  then bit 6 of its low byte is set. The three loops are plain
  `while`/`do` loops.
- **`LinkExchangeSaveData`**: the cancel test is `if ((u16)(keys & 2))`, and
  the else branch writes `gLinkSessionReset = 0`. CSE then reuses the
  known-zero register, which gives the ROM's `strb r1`. With a named
  `u16 cancel` local the draft had an extra copy. The `GetSaveTransferData`
  result goes into a local so it is computed before `self->field_90`
  is loaded.
- **`DrawSaveMenuMessageLines`**: the centre X needs its own local,
  `x = (0xf0 - w) >> 1`. That puts X in r3 and lets the Y constant
  spill to ip, as the ROM has it. With the expression inline, X lands
  in r1.
- **`DrawSaveSlotStats`** / **`DrawSaveSlots`**:
  - The fifth argument is a one-byte struct passed by value,
    `struct byte_arg { u8 v; } __attribute__((packed))`. The callee
    reads it with `add r0, sp, #0x3c; ldrb`, and the caller stores it
    with `strb`. A `u8` parameter is read as a whole word and stored
    with `str`. `*(u8 *)&arg` gives the `ldrb` in the callee, but then
    the caller in the same file stores with `str`.
  - `DrawSaveSlotStats` keeps running `x`/`y` locals per block, as in
    `x = label1 + 0x2b; ...; x += 0xd;`. The third block re-derives `y`
    the same way the second does (`y = label2 + 0x1e; ...; y -= 7`),
    and CSE turns that into the ROM's spilled `y`.
  - Before the `%`-append loop, `i = 0` has to come ahead of
    `y = label2 - 2`.
  - `DrawSaveSlots`'s "selected" branch is an inlined copy of
    `DrawEmptySlotLabel` (`draw_row_mark`), which explains its fresh
    record-offset loads. A small table plus a loop does not work; the
    ROM has four `DRAW_ROW` expansions.

## Didn't close (7 functions, drafts left under `NON_MATCHING`)

- **`DrawYesNoPrompt`** (`save_menu_draw.c`, NAKED): 9 halfwords off under
  both compilers. Everything else matches, but two long-lived constants
  are swapped: the ROM keeps the record offset `0x130` in r8 and the Y
  constant `0x87` in sb. I tried setting the positions through the
  setter or with direct stores in all 32 combinations, a `y` local,
  and a ternary. None of them swapped the pair.
- **`InitSaveMenuIcons`** (raw; draft in `save_menu_draw.c`): 5 halfwords off
  under old_agbcc. The whole body matches, including the palette-copy
  loop (`u16 (*pal)[16]` indexing gives the ROM's two base pointers),
  the icon setup, and the down-counting icon-array loop with `ldm`
  post-increments. The per-icon step is an inline `new_row_icon(slot,
  tblOff, frame)`, and its `frame` goes through `Opaque()` so the
  constant is loaded before the `+0x2d` address. The difference is in
  the loop pre-header. The ROM loads `0x80` into sb before copying the
  `rowObjB`/`rowObjC` loop pointers, so it reloads `rowObjB`'s pointer
  from the stack. The draft loads it afterwards and copies the pointer
  from a register. Things that didn't fix it: a `0x80` local, passing
  it as a parameter, `field_3c` typed `s16`/`u32`, reordering the
  array-pointer locals, and per-file flags (`-fno-rerun-loop-opt`,
  `-frerun-cse-after-loop`).
- **`ValidateSaveData`** (`save_data.c`, NAKED): 18 halfwords off.
  The checksum is an inlined copy of `CheckSaveChecksum` with its result
  pinned to r1, as there, and that part and the `DmaFill16` match.
  The ROM computes the four marker-byte addresses before the row loop
  and keeps them in r6/sb/r7/r8, as if they had been hoisted out of a
  loop. `ResetSaveData`, the same sequence without the checksum, computes
  them after the loop, and so do plain stores here. Local pointers get
  them computed early but in other registers, and 0x1fb comes out as
  0x1f8 + 3 instead of from its own literal.
- **`MakeLinkHandshakeId`** (`link_handshake.c`, NAKED): 49 halfwords off under
  old_agbcc. The hash loop is a `for (i = 4; i != -1; i--)` countdown,
  which gives the ROM's `cmp r4, r5(-1); bne`. Two things are left:
  - The ROM strength-reduces the fill loop into a pointer compared
    signed (`cmp r0, r3; bge`). Neither compiler reduces
    `self[i] = 0xec`.
  - In the nibble fold, the ROM reloads byte 6. gcc folds
    `(self[7] << 8) | self[6]` back into `hash`.
- **`ResetLinkSessionState`** (`link_handshake.c`, NAKED): 136 halfwords off, same
  size as the ROM. It establishes `struct link_session`:
  - 0x0c/0x10/0x14 counters.
  - The id word at 0x20, as `lo:4`/`hi:12`.
  - The handshake id at 0x30.
  - A 0x90-byte `struct link_ring` at 0x40.
  - Four 0xc8-byte `struct link_player` records at 0xd0, each with its
    own ring at +0x38.
  - The timeout counters at 0x3f0-0x404.

  The id copies need `(src[1] << 8) | src[0]` and an explicit
  `lo = v & 0xff` local to keep the ROM's `and`. What's left is the
  per-player loop: the ROM recomputes `i * 0xc8` for each field,
  computes `i + 1` before the inner copy loop, and keeps that loop
  counting up.
- **`UpdateLinkSession`** (`link_handshake.c`, NAKED): 37 halfwords off under
  both compilers, same size as the ROM. Everything from the timeout
  counter on matches. The IRQ indices are `TIMER3` (6) and
  `SERIAL` (7). `LinkStop` is called with the session in r0, so it
  now takes an unused `struct link_session *` parameter; that doesn't
  change its code. What's left:
  - The ROM holds the constant 1 in sb for the ready-bit test,
    `field_8 = 1` and `REG_IME = 1`, and a second 1 in r1 for the arm3
    flag's `eor`/`and`. A `one` local gets the sb part, but then gcc
    turns the flag into `bic`.
  - The IME/IE save sequence is scheduled differently.
- **`HandleLinkSerial`** (1488 B) was not attempted.

`Closes #8`. Issues #4 and #6 stay open.

## Tools

Scratch copies (not committed): a per-file compiler that diffs every
function against the ROM under either compiler (`--old`/`--new`,
`--DNON_MATCHING=1`, `--flags=`), and a variant runner that swaps in
alternative bodies for one function and reports each.

## Verification

`rm -rf build && make NON_MATCHING=1 report` shows no warnings from the
touched files. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` prints `crashbandicootxs.gba: OK`.

## Later pass

The early-ROM NAKED retry
([early-rom-naked-retry.md](early-rom-naked-retry.md)) closed
`DrawYesNoPrompt`. `y` is pinned to r9 and set after the manager pointer is
loaded. Global-alloc was ranking the 0x87 constant just above the 0x130
offset (0.0714 vs 0.070), which is why they swapped. The
`ValidateSaveData`, `MakeLinkHandshakeId` and `InitSaveMenuIcons` drafts are closer (6, 11
and 5 halfwords). The same doc says what is left in each.
