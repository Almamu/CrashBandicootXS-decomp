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
| `03000474` | `HeapSortActorsByKey` (`gHeapSortActorsByKeyFunc`) | same | matched (fourth pass) |
| `03000634` | `UnpackRleSpriteFrame` (`gUnpackRleSpriteFrameFunc`) | same | matched |
| `030006FC` | `LookupSpriteFrameCache` (`gLookupSpriteFrameCacheFunc`) | same | parked |
| `030007CC`-`030009E8` | initialised globals | `src/iwram/iwram_data.c` | typed C, data |

The five string routines have no caller: none of their addresses occurs
as a word anywhere in the ROM, and Thumb code can only reach ARM code
through a pointer. They are ARM builds of the Thumb ones in
`src/util/number_format.c` and `src/util/string.c`.

## Compiler

The ARM functions are gcc output, and `tools/agbcc/bin/agbcc_arm`
(gcc 2.9-arm-000512, installed by SAT-R/agbcc next to agbcc and
old_agbcc) reproduces eight of them byte for byte (six from the first
pass, strncpy_arm from the second, HeapSortActorsByKey from the fourth)
with `-mthumb-interwork -O2 -fomit-frame-pointer` (without the last flag it
sets up an APCS frame the ROM doesn't have). The Makefile builds
`string_arm.o` and `sprite_arm.o` that way (`ARM_OBJS`). Adding
`-fno-expensive-optimizations` breaks the string functions, so it isn't
used.

## Why three are parked

(Written before the fourth pass, which matched HeapSortActorsByKey. Its
"constant materialization" point below turned out to be C shape, not
the compiler. Two functions stay parked.)

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
pool placement). The third pass below tested this lead and ruled it out:
no FSF gcc release from 2.95.3 to 3.4.6 fits.

## Third pass: FSF gcc 2.95.3-3.4.6 built and ruled out

This pass built the candidate compilers from official sources and
compared their output with the ROM. None fits. The ROM's compiler is a
later build of agbcc_arm's own lineage (Cygnus/Red Hat
`2.9-arm-YYMMDD`), not FSF gcc. That build isn't public, so nothing could
be vendored and the three functions stay parked.

**What was built.** `cc1` for `--target=arm-elf` (`make all-gcc`, no
libraries) from the `gcc-core` tarballs on sourceware.org's
`pub/gcc/releases` (the gcc.gnu.org mirror; ftp.gnu.org didn't respond):

| Version | sha256 of `gcc-core-<v>.tar.gz` | Host fixes needed |
|---|---|---|
| 2.95.3 | `56811ee6...` | `arm.c`: `arm_prog_mode =` is a cast lvalue, assign `arm_prgmode` |
| 3.0.4 | `f60b23b2...` | `arm.c`: `DECL_RTL (sym) = new` becomes `SET_DECL_RTL (sym, new)` |
| 3.1.1 | `29039c00...` | `include/obstack.h` from 3.3.6 (cast-as-lvalue `++`) |
| 3.2.3 | `712df2ef...` | same as 3.1.1 |
| 3.3.6 | `eb28f630...` | none |
| 3.4.6 | `c6030bf0...` | none |

Each was built as a 32-bit host binary: 2.95.3's `config.sub` doesn't
know `x86_64`, and gcc of that age assumes a 32-bit `long`. The host
compiler was `nixpkgs#pkgsi686Linux.gcc` (gcc 15) with
`--host=--build=i686-pc-linux-gnu`, `CC='gcc -std=gnu89 -fcommon -w
-fno-strict-aliasing'` and `MAKEINFO=true`. Every build stops at libgcc
(no cross assembler), after `gcc/cc1` is linked. The modern
`arm-none-eabi-gcc` 15 from the devshell was also tried as a trend check.

**How it was compared.** The `NON_MATCHING=1` drafts of `string_arm.c` and
`sprite_arm.c` were preprocessed and compiled by each `cc1` with
`-mcpu=arm7tdmi -mthumb-interwork -fomit-frame-pointer` at -O1, -O2,
-O2 `-fno-expensive-optimizations`, -O3 and -Os. The output was assembled
and its words diffed against `baserom.gba`. Two caveats: the drafts were
tuned for agbcc_arm, which biases the instruction-level scores towards
it, and the decisive evidence below comes from the back-end source, not
from the scores.

**The ROM's distinguishing features, compiler by compiler:**

| Feature (ROM) | agbcc_arm | 2.95.3 | 3.0.4 | 3.1.1-3.4.6 | gcc 15 |
|---|---|---|---|---|---|
| Push r4-r6 without lr when lr is unused (`itoa_arm`) | no, lr forced | no, lr forced | yes, but the allocator uses lr in itoa | yes, but lr used in itoa | yes, but lr used in itoa |
| Single-register save as `stmfd sp!, {lr}` | yes | yes | no: `str lr, [sp, #-4]!` | no: `str lr, [sp, #-4]!` | n/a (saves nothing there) |
| Mid-function return `ldmfd sp!, {lr}; bx lr` | no: `ldmfd sp!, {ip}; bx ip` | no: `{ip}` | **yes** | no: `ldr lr, [sp], #4` or a single epilogue | n/a |
| `cmp #10` with `ge`/`lt` | no, `cmp #9` | no | no | no | no |
| 1/0 in hoisted registers (`HeapSortActorsByKey`) | no | no | no | no | no |

The FSF back-end history settles the first two rows. The gitweb
history of `gcc/config/arm/arm.c` and `arm.md`, fetched at
`d5b7b3ae3302` (2000-04-08, the arm/thumb back-end merge),
`5895f7938440` (2000-10-09) and `6d3d91336c1a` (2000-12-08), shows:

- **Single-register `str`.** `*push_multi` has emitted a single-register
  push as `str rN, [sp, #-4]!` since 2000-01-09 ("use single STR/LDR
  when..."). Every FSF revision after that prints the
  `LookupSpriteFrameCache` prologue as `str lr, ...`, never as the ROM's
  `stmfd sp!, {lr}`.
- **lr forced into every push.** Until 2000-12-08,
  `arm_expand_prologue` adds lr to every register push ("If we have to
  push any regs, then we must push lr as well"), so `itoa_arm`'s
  `push {r4, r5, r6}` can't come out. That date's
  `arm_compute_save_reg_mask` stops forcing lr, but by then the `str`
  change is eleven months old.

So no FSF revision combines the ROM's `stmfd {lr}` single push with an
lr-free push of r4-r6. agbcc_arm (`2.9-arm-000512`) has the first and
not the second, so it predates the str patch. It is a Cygnus branch
build, not mainline. The ROM's compiler has both, plus the
`ldmfd sp!, {lr}; bx lr` return insn (3.0-style), so it must be a later
build of that same Cygnus/Red Hat branch. Its code generation otherwise
looks like agbcc_arm's. On `LookupSpriteFrameCache` with
`-fno-expensive-optimizations`, agbcc_arm reproduces the ROM's second
loop instruction for instruction: the same scheduling, `ldm ip, {r1, r3}`
peephole and literal pool. Only the return sequence differs, plus the
constant hoisted in the first loop. gcc 3.0.4 also gets the
`ldmfd {lr}` returns right, but it moves the hit block out of line and
schedules it differently. gcc 3.x is also clearly not the image's
compiler. It doesn't reproduce the seven matched functions:
`UnpackRleSpriteFrame` alone has ~70 differing lines under every 3.x
version, against an exact match under agbcc_arm. 2.95.3 comes closest
there (it shares agbcc_arm's middle end), but it has 2.9's prologue.

**Closeness** (instruction-sequence similarity of the best flag set
against the ROM; register names count, branch and pool offsets don't):

| Compiler | itoa_arm | HeapSortActorsByKey | LookupSpriteFrameCache |
|---|---|---|---|
| agbcc_arm | 40% | 27% | 78% (`-fno-expensive-optimizations`) |
| gcc 2.95.3 | 40% | 27% | 70% |
| gcc 3.0.4 | 38% (-O1) | 24% (-O1) | 62% |
| gcc 3.1.1-3.4.6 | 31-38% (-O1) | 8-21% | 18-42% |

No version reproduced any parked function byte for byte.

**What it would take.** The matching compiler would be a Red Hat GNUPro /
Cygnus ARM toolchain from late 2000 or 2001, the line agbcc_arm's
`2.9-arm-000512` comes from. Nintendo may have shipped one in a later
AGB SDK. Neither has public sources, and patching agbcc_arm's back end
to imitate it wouldn't be a real toolchain. ARM SDT/ADS (`armcc`) is
unlikely: everything apart from frame handling is gcc output, and
agbcc_arm's own gcc output at that. Until such a compiler turns up, the
three stay `NAKED` with their drafts. `HeapSortActorsByKey` is still the
only one that might be reachable from C under agbcc_arm, since its
prologue and epilogue fit. (It was: see the fourth pass. itoa_arm and
LookupSpriteFrameCache stay `NAKED`.)

## Fourth pass: HeapSortActorsByKey matched

HeapSortActorsByKey matches under agbcc_arm with the existing
`-mthumb-interwork -O2 -fomit-frame-pointer` (no new flags, no new
object). Its 1/0-in-registers tests weren't a compiler difference. They
come from four C details, found by reading the RTL dumps (`-da`) rather
than sweeping flags:

1. **The comparator takes its results as arguments.** The tests aren't
   if-converted conditional moves. agbcc_arm keeps them as
   `x = one; if (key > key) goto L; x = zero; L:` until final, and the
   ARM ccfsm prints that as `mov r3, one; movls r3, zero`. So the arms
   are registers when the inline function returns two *variables*:
   `static inline u8 KeyGreater(a, i, j, u8 one, u8 zero)`, whose body is
   `if (a[i]->key <= a[j]->key) return zero; return one;`, with
   `u8 one = 1, zero = 0;` declared in the caller. The u8 types matter.
   With int or u32 parameters cse propagates the constants back into the
   returns (immediates again). With s32 locals passed to u8 parameters,
   an `and rN, rM, #255` truncation is left in the loop.
2. **The locals are scoped to the sift loop's body.** loop.c then
   hoists `one = 1; zero = 0` to the sift loop's own preheader (the
   ROM's `mov r9, #1; mov sl, #0` after the `bge`). Declared at function
   or sift-function scope, they go to the outermost preheader instead.
   In the second phase cse2 replaces the hoisted constants with
   registers already holding 1 and 0 (`mov sl, ip; mov r8, r4`), as in
   the ROM.
3. **The comparator takes indices, not pointers.** Each inlined
   `child + 1` argument is a separate pseudo. gcse turns them into
   copies of one register (the ROM's `add r3, ip, #1` ... `mov r0, r3`)
   but can't then see that the two `a[child + 1]` loads are the same,
   so the second test reloads it, as in the ROM. The PRE placement of
   `a[root]` (loaded at the top and again after the first test) and
   `a[child]` (top only) falls out of the same shape.
4. **The sift loop is written out in both phases on shared
   `root`/`child` locals.** An `inline SiftDown(a, root, n)` gets
   different register priorities (`a` in ip and `child` in lr, swapped
   with the ROM). Spelled out as
   `for (root = i; (child = root * 2 + 1) < n; root = child)` in both
   loops, with `for (i = n / 2; i > 0;) { i--; ... }` for the first
   phase, every register matches.

That leaves one difference: agbcc_arm's jump2 cross-jumps the
`while (n > 1)` loop's duplicated entry test (`cmp r7, #1; ble`) with
its bottom test (`cmp r7, #1; bgt`). `find_cross_jump` (jump.c) accepts
the single matching `cmp` because a label precedes it (the first loop's
skip jumps there, or the inner loop's exits land there). The ROM keeps
both tests. The first loop escapes this only because sched1 puts
`mov lr, r1` between its `cmp` and `ble`. No spelling of the second
loop was found that avoids it: `for (; n > 1;)`, `if () do {} while`,
`for (;;) { if (n <= 1) break; ...}`, `1 < n`, `n >= 2` and `--n` in
the index all cross-jump. Shapes that don't (a `for (i = n - 1; ...)`
counter, `n--` at the end) change the rest of the loop. So two
`MATCH_BARRIER()`s block it, one before the loop and one at the end of
its body. An `asm("")` is an `ASM_INPUT`, which `find_cross_jump`
refuses to match, and it emits nothing. Each one blocks one direction:
the first stops the entry test from matching the bottom one, the
second the reverse. With only the first, the bottom test is
cross-jumped instead.

The C is in `src/iwram/sprite_arm.c`. The `NAKED` transcription and the
`NON_MATCHING` draft are gone. Without the barriers the function is
19 instruction lines off (the two jumps and their shifted offsets).

What was tried before the match, beyond the second pass's list:
`MATCH_HOLD_REG` pins of 1/0 in r9/sl (registers right, but `movhi; movls`
from one if_then_else, and phase two needs sl/r8), `MATCH_KEEP` and
`MATCH_CONST` on the locals (immediates, or `movhi; movls`), s32/u32/s8
variants of the comparator, and ten first-phase and six second-phase
loop shapes.

## Data

`iwram_data.c` defines every global from `0x030007CC` up to
`gIntrTable`, with an initialiser each so agbcc puts them all in
`.data` in definition order (checked with `nm`). The hook pointers now
point at the ARM functions by name, the cutscene language table
`gCutsceneTexts` at `src/data/cutscenes_16d1c8.c`'s six tables, and
the pointers into still-raw blobs (`gCrc16Table`,
`gTerrainHeights3`, `gTitleMenuBlinkPalette`) are the blob plus an
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
- Fourth pass (HeapSortActorsByKey matched): `make tidy`, full build,
  `make compare`: `crashbandicootxs.gba: OK`. `rm -rf build objdiff.json
  && make NON_MATCHING=1 report` and `objdiff-cli report generate`:
  code 242,990 / 243,378 (99.84%), functions 2,057 / 2,059 (the two
  left are itoa_arm and LookupSpriteFrameCache); data 8,036,176 /
  8,036,176 (100%). The other functions in `sprite_arm.o` still match.
