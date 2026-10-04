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
| `03000120` | `strncpy_arm` | same | matched (second pass), UNUSED |
| `0300015C` | `strcat_arm` | same | matched, UNUSED |
| `03000198` | `itoa_arm` | same | parked, UNUSED |
| `0300024C` | `UnpackNibbleTiles` (`gUnpackNibbleTilesFunc`) | `src/iwram/sprite_arm.c` | matched |
| `0300036C` | `DrawMirroredTilemap` (`gDrawMirroredTilemapFunc`) | same | matched |
| `03000474` | `HeapSortActorsByKey` (`gHeapSortActorsByKeyFunc`) | same | parked |
| `03000634` | `UnpackRleSpriteFrame` (`gUnpackRleSpriteFrameFunc`) | same | matched |
| `030006FC` | `LookupSpriteFrameCache` (`gLookupSpriteFrameCacheFunc`) | same | parked |
| `030007CC`-`030009E8` | initialised globals | `src/iwram/iwram_data.c` | typed C, data |

The five string routines have no caller: none of their addresses occurs
as a word anywhere in the ROM, and Thumb code can only reach ARM code
through a pointer. They are ARM builds of the Thumb ones in
`src/util/string_util*.c`.

## Compiler

The ARM functions are gcc output, and `tools/agbcc/bin/agbcc_arm`
(gcc 2.9-arm-000512, installed by SAT-R/agbcc next to agbcc and
old_agbcc) reproduces seven of them byte for byte (six from the first
pass, strncpy_arm from the second) with
`-mthumb-interwork -O2 -fomit-frame-pointer` (without the last flag it
sets up an APCS frame the ROM doesn't have). The Makefile builds
`string_arm.o` and `sprite_arm.o` that way (`ARM_OBJS`). Adding
`-fno-expensive-optimizations` breaks the string functions, so it isn't
used.

## Why three are parked

The three parked functions are `NAKED` transcriptions in the matching
build, with their C under `#if NON_MATCHING` (what the report scores).
They differ in ways that point at a different build of ARM gcc, not at
the C. For two of them, agbcc_arm's source shows that no C can produce
the ROM's code (see "Second pass" below):

- **Return sequences.** agbcc_arm's `return` pattern for an
  interworking function that saved only lr is `ldmfd sp!, {ip}; bx ip`,
  and it uses conditional returns (`bxeq lr`) in leaf functions. In the
  ROM, `LookupSpriteFrameCache` returns three times with
  `ldmfd sp!, {lr}; bx lr`. No function in the image uses a conditional
  return.
- **Prologue.** `itoa_arm` saves r4-r6 without lr
  (`push {r4, r5, r6}` ... `pop {r4, r5, r6}; bx lr`). agbcc_arm always
  pushes lr along with any other register.
- **Comparisons.** `itoa_arm`'s ROM tests `digit < 10` as `cmp r1, #10`
  with `addge`/`addlt`; agbcc_arm always canonicalizes to `cmp r1, #9`
  with `addgt`/`addle`.
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

## Second pass: strncpy_arm matched, the other three pinned down

A retry tested whether the "different compiler" diagnosis holds. It read
agbcc_arm's source (SAT-R/agbcc `gcc_arm/`, `config/arm/arm.c`, `arm.md`,
`jump.c`, `fold-const.c`) and swept flags. Result: strncpy_arm matches
as C. For itoa_arm and LookupSpriteFrameCache the source proves no C
can match under agbcc_arm. HeapSortActorsByKey is still open.

**Other compilers.** agbcc_arm is the only ARM-state compiler on hand.
`agbcc` and `old_agbcc` are Thumb-only (thumb.c back end); both reject
`-marm`. SAT-R's `gcc_arm/config/arm/` also holds `arm_990720.*` and
`arm_020422.*` variants, but they have the same `use_return_insn`,
`output_return_instruction` and prologue code, so rebuilding with them
changes nothing here.

**Flags.** The ten functions were compiled under -O1/-O2/-O3, and
itoa_arm and HeapSortActorsByKey were also swept with about 50 single
extra flags each: -fno-strength-reduce, -fno-thread-jumps,
-fno-cse-follow-jumps, -fno-gcse, -fno-schedule-insns(2),
-fno-expensive-optimizations, -fno-regmove, -fcaller-saves,
-ffixed-lr, -fcall-saved-lr, -mapcs-frame, -mno-sched-prolog,
-mcpu=arm7tdmi/arm7/arm9tdmi/strongarm, -mtune=strongarm,
-mshort-load-bytes and more. None fixed a parked function. -O1 (the
SDK's level) breaks strlen_arm, and every -mcpu other than the default
arm7tdmi breaks the three matched sprite routines. So -O2 stays for
both objects, and per-object flags buy nothing.

**strncpy_arm: matched.** The conditional return comes from jump.c.
Its jump pass rewrites any jump whose target label is directly followed
by a `return` into a return insn, conditional if the jump is (it does
so when `USE_RETURN_INSN`, which holds for a leaf with nothing saved).
An empty `asm("")` after the final store sits between that label and the
return. It emits no code, and the `beq` to the single `bx lr` stays. The
rest was C shape: a rotated `do { } while` with `c = *++src` gives
`ldrb r3, [r1, #1]!`, and `c = 0; if (n != c) *dst = c;` gives the ROM's
`mov r3, #0; cmp r2, r3; strbne r3, [r0]`. Whole-file check: the other
three string functions still match.

**itoa_arm: impossible under agbcc_arm.** `arm_expand_prologue` does
`if (live_regs_mask) { live_regs_mask |= 0x4000; ... }` ("If we have to
push any regs, then we must push lr as well"). The text epilogue in
`output_func_epilogue` does the same. So every function that saves r4-r6
also saves and restores lr. The ROM's `push {r4, r5, r6}` /
`pop {r4, r5, r6}; bx lr` can't come out. (The fold-const.c rewrite of
`X < C` to `X <= C-1` also explains `cmp r1, #9`. A `digit - 10 < 0`
spelling does give `cmp r1, #10`, but with `mi`/`pl` rather than
`lt`/`ge`.)

**LookupSpriteFrameCache: impossible under agbcc_arm.** The ROM has
three copies of `ldmfd sp!, {lr}; bx lr`. gcc 2.9 prints the text
epilogue once, so the copies must be `return` insns.
`output_return_instruction` builds an interworking return with no frame
pointer as `ldm..fd sp!, {..., ip}` + `bx ip`. It pops into lr only in
the frame-pointer case (`ldmea fp`). With the draft compiled under
-fno-expensive-optimizations (which saves only lr), agbcc_arm prints
`ldmeqfd sp!, {ip}; bxeq ip` and `ldmfd sp!, {ip}; bx ip`.

**HeapSortActorsByKey: not found.** The ROM's `mov r3, one; movls r3,
zero` with both constants in hoisted registers needs a conditional move
whose arms are both registers when loop.c runs. jump.c builds these moves
from `x = a; if (c) x = b;` using the values last assigned to x. Plain
`one`/`zero` locals get one arm hoisted (`mov r8, #0` or `mov r8, #1`),
but the other becomes an immediate. With `asm("" : "=r"(v) : "0"(K))`
constants all three tests materialize with register arms, but the arms
are not hoisted and come out `movls; movhi` instead of `mov; movls`.
It is not provably impossible. Its prologue and epilogue fit agbcc_arm.
But it comes from the same compiler as the two above, so the constant
handling is likely that compiler's too.

**Which compiler, then?** Three things match gcc 3.x's ARM back end: no
lr push when only r4+ are saved and the return is `bx lr`, interworking
returns that pop into lr, and no conditional returns. Nothing in the
repo can confirm that, so it's only a lead. The three parked functions
aren't HANDWRITTEN: they carry compiler idioms (the shared-zero
register in `itoa_arm`'s tail, ccfsm-conditionalized blocks, literal
pool placement).

## Data

`iwram_data.c` defines every global from `0x030007CC` up to
`gIntrTable`, with an initialiser each so agbcc puts them all in
`.data` in definition order (checked with `nm`). The hook pointers now
point at the ARM functions by name, the cutscene language table
`gCutsceneTexts` at `src/data/cutscenes_16d1c8.c`'s six tables, and
the pointers into still-raw blobs (`gStaticData_0816AF10`,
`gTerrainHeights3`, `gStaticData_0817D074`) are the blob plus an
offset. `gGaxHaltFont` is GAX2's Huffman-compressed fatal-error
font. `sym_iwram.txt` lost the 46 entries below `0x9E8`, which are now
defined by these objects.

## Verification

- `make tidy && make`: `crashbandicootxs.gba: OK`.
- `rm -rf build objdiff.json && make NON_MATCHING=1 report` and
  `objdiff-cli report generate`: code 242,482 / 243,378 (99.63%, was
  241,594 / 241,594), functions 2,055 / 2,059; data 7,954,460 /
  8,036,176 (98.98%, was 7,953,920 / 8,038,172).
- Second pass (strncpy_arm matched): `make tidy`, full build,
  `make compare`: `crashbandicootxs.gba: OK`. Report: code 242,542 /
  243,378 (99.66%), functions 2,056 / 2,059; data unchanged.
