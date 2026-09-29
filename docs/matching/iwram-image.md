# The IWRAM image: IntrMain, the ARM routines and the IWRAM data

ROM `0x087E55E4`-`0x087E5FCC` (0x9E8 bytes) is the image crt0 copies to
IWRAM `0x03000000` at boot. It was one `.incbin` in `data/data.s`
(`gStaticData_087E55E4`, together with the 0xFF fill after it). It is now
built from source and linked to run at `0x03000000`: see
[decomp_dev.md](../decomp_dev.md), "The IWRAM image", for the linker
script and the report. Contents:

| IWRAM | Name | Source | Status |
|---|---|---|---|
| `03000000` | `IntrMain` (alias `IntrMain_Buffer`) | `asm/intr_main.s` | hand-written ARM, `HANDWRITTEN` |
| `030000D4` | `strlen_arm` | `src/iwram/string_arm.c` | matched, UNUSED |
| `030000FC` | `strcpy_arm` | same | matched, UNUSED |
| `03000120` | `strncpy_arm` | same | parked, UNUSED |
| `0300015C` | `strcat_arm` | same | matched, UNUSED |
| `03000198` | `itoa_arm` | same | parked, UNUSED |
| `0300024C` | `UnpackNibbleTiles` (`gUnknown_03000898`) | `src/iwram/sprite_arm.c` | matched |
| `0300036C` | `DrawMirroredTilemap` (`gUnknown_0300087C`) | same | matched |
| `03000474` | `HeapSortActorsByKey` (`gUnknown_03000880`) | same | parked |
| `03000634` | `UnpackRleSpriteFrame` (`gUnknown_03000874`) | same | matched |
| `030006FC` | `LookupSpriteFrameCache` (`gUnknown_03000870`) | same | parked |
| `030007CC`-`030009E8` | initialised globals | `src/iwram/iwram_data.c` | typed C, data |

The five string routines have no caller: none of their addresses occurs
as a word anywhere in the ROM, and Thumb code can only reach ARM code
through a pointer. They are ARM builds of the Thumb ones in
`src/util/string_util*.c`.

## Compiler

The ARM functions are gcc output, and `tools/agbcc/bin/agbcc_arm`
(gcc 2.9-arm-000512, installed by SAT-R/agbcc next to agbcc and
old_agbcc) reproduces six of them byte for byte with
`-mthumb-interwork -O2 -fomit-frame-pointer` (without the last flag it
sets up an APCS frame the ROM doesn't have). The Makefile builds
`string_arm.o` and `sprite_arm.o` that way (`ARM_OBJS`). Adding
`-fno-expensive-optimizations` breaks the string functions, so it isn't
used.

## Why four are parked

The four parked functions are `NAKED` transcriptions in the matching
build, with their C under `#if NON_MATCHING` (what the report scores).
All four differ in ways that point at a different build of ARM gcc, not
at the C:

- **Return sequences.** agbcc_arm's `return` pattern for an
  interworking function that saved only lr is `ldmfd sp!, {ip}; bx ip`,
  and it uses conditional returns (`bxeq lr`) in leaf functions. In the
  ROM, `LookupSpriteFrameCache` returns three times with
  `ldmfd sp!, {lr}; bx lr`, and `strncpy_arm` branches to its single
  `bx lr` instead of returning conditionally. No function in the image
  uses a conditional return. That output is fixed in agbcc_arm's arm.c
  (`output_return_instruction`), so no C reaches it.
- **Comparisons.** `itoa_arm`'s ROM tests `digit < 10` as `cmp r1, #10`
  with `addge`/`addlt`; agbcc_arm always canonicalizes to `cmp r1, #9`
  with `addgt`/`addle`.
- **Register allocation.** `itoa_arm` never uses lr (base in ip,
  buf/len/neg in r6/r5/r4); agbcc_arm hands lr to one of those first.
  Clobbering lr in the Div SWI's asm keeps it free, but then lr is saved
  in the prologue, which the ROM doesn't do.
- **Constant materialization.** `HeapSortActorsByKey` tests every key
  comparison as a 1/0 held in registers hoisted out of the loop
  (`mov r3, r9; movls r3, sl; cmp r3, #0`). agbcc_arm either folds the
  flag into branches (int-returning comparator, every phrasing tried:
  plain return, ternary, `!`, flag variable, casts) or keeps it with
  immediates (`mov r3, #1; movls r3, #0`, u8 or s8 comparator with early
  returns). `LookupSpriteFrameCache`'s ROM computes
  `vramAddr - 0x06010000` in place at each return, where agbcc_arm hoists
  the constant out of the loop (only `-fno-expensive-optimizations`
  keeps it in place, and only in the second loop).

What does match in each: the control flow, the loads and stores, and the
arithmetic, as far as the differences above allow (the report scores
them 67-81%).

## Data

`iwram_data.c` defines every global from `0x030007CC` up to
`gUnknown_030009E8`, with an initialiser each so agbcc puts them all in
`.data` in definition order (checked with `nm`). The hook pointers now
point at the ARM functions by name, the cutscene language table
`gUnknown_03000834` at `src/data/cutscenes_16d1c8.c`'s six tables, and
the pointers into still-raw blobs (`gStaticData_0816AF10`,
`gStaticData_081725C4`, `gStaticData_0817D074`) are the blob plus an
offset. `gUnknown_030008D0` is GAX2's Huffman-compressed fatal-error
font. `sym_iwram.txt` lost the 46 entries below `0x9E8`, which are now
defined by these objects.

## Verification

- `make tidy && make`: `crashbandicootxs.gba: OK`.
- `rm -rf build objdiff.json && make NON_MATCHING=1 report` and
  `objdiff-cli report generate`: code 242,482 / 243,378 (99.63%, was
  241,594 / 241,594), functions 2,055 / 2,059; data 7,954,460 /
  8,036,176 (98.98%, was 7,953,920 / 8,038,172).
