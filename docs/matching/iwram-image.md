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
| `030000D4` | `strlen_arm` | `src/iwram/string_arm.cpp` | matched, UNUSED |
| `030000FC` | `strcpy_arm` | same | matched, UNUSED |
| `03000120` | `strncpy_arm` | same | matched (second pass; plain since the ninth step, `-mno-cond-return`), UNUSED |
| `0300015C` | `strcat_arm` | same | matched, UNUSED |
| `03000198` | `itoa_arm` | same | matched (seventh pass, agbcc_arm_patched), UNUSED |
| `0300024C` | `UnpackNibbleTiles` (`gUnpackNibbleTilesFunc`) | `src/iwram/sprite_arm.cpp` | matched |
| `0300036C` | `DrawMirroredTilemap` (`gDrawMirroredTilemapFunc`) | same | matched |
| `03000474` | `HeapSortActorsByKey` (`gHeapSortActorsByKeyFunc`) | same | matched (fourth pass; plain since the ninth step, `-mstrict-cross-jump`) |
| `03000634` | `UnpackRleSpriteFrame` (`gUnpackRleSpriteFrameFunc`) | same | matched |
| `030006FC` | `LookupSpriteFrameCache` (`gLookupSpriteFrameCacheFunc`) | same | matched (seventh pass, agbcc_arm_patched) |
| `030007CC`-`030009E8` | initialised globals | `src/iwram/iwram_data.cpp` | typed C++, data |

The five string routines have no caller: none of their addresses occurs
as a word anywhere in the ROM, and Thumb code can only reach ARM code
through a pointer. They are ARM builds of the Thumb ones in
`src/util/number_format.cpp` and `src/util/string.cpp`.

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

The last two, `itoa_arm` and `LookupSpriteFrameCache`, need prologue
and return code that agbcc_arm can't produce. Since the seventh pass both
objects are built with `agbcc_arm_patched`, agbcc_arm with two opt-in
options added (`PATCHED_ARM_OBJS`; see "Seventh pass" below). Without
the options its output is agbcc_arm's, byte for byte.

Since the C++ conversion the two files are C++ (`string_arm.cpp`,
`sprite_arm.cpp`), like the rest of the game, and the Makefile builds
them with `agbcp_arm_patched`: notyourav/agbcc's `cp` branch's ARM C++
compiler (`g++_arm`, the same gcc 2.9-arm-000512 with the C++ front end)
with the same patch, built by `tools/build_agbccpp.sh`. Both objects
are byte-identical to the agbcc_arm_patched C build; see "Eighth step:
C++" below and docs/cplusplus.md, "The IWRAM ARM code".

strncpy_arm and HeapSortActorsByKey matched on stock agbcc_arm only with
an empty-asm barrier each (two for HeapSortActorsByKey), against jump.c
rules the ROM's compiler doesn't have. Since the ninth step the patch
has an opt-in option for each of those too (`-mno-cond-return`,
`-mstrict-cross-jump`), and both functions are plain code; see "Ninth
step" below.

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

The C is in `src/iwram/sprite_arm.cpp`. The `NAKED` transcription and the
`NON_MATCHING` draft are gone. Without the barriers the function is
19 instruction lines off (the two jumps and their shifted offsets).

What was tried before the match, beyond the second pass's list:
`MATCH_HOLD_REG` pins of 1/0 in r9/sl (registers right, but `movhi; movls`
from one if_then_else, and phase two needs sl/r8), `MATCH_KEEP` and
`MATCH_CONST` on the locals (immediates, or `movhi; movls`), s32/u32/s8
variants of the comparator, and ten first-phase and six second-phase
loop shapes.

## Fifth pass: both blockers re-checked, LookupSpriteFrameCache six instructions off

This pass treated the second pass's two "impossible" verdicts as
hypotheses. It read every path in agbcc_arm's back end that can push
registers or print a return (SAT-R/agbcc `gcc_arm/config/arm/arm.c` and
`arm.md`; `arm_020422.c` has the same code at shifted lines), and it
traced the drafts through the RTL dumps (`-da`). Both verdicts hold, and
the exact lines are below. Neither function matches. But
LookupSpriteFrameCache's draft is now exact apart from the three return
sequences, and itoa_arm's is closer. Both drafts were updated. Line
numbers are `arm.c`'s.

**itoa_arm: no path pushes r4-r6 without lr.**

- **The prologue is RTL.** `arm.md` defines `prologue` (it calls
  `arm_expand_prologue`) and no `epilogue`, so the push always comes from
  `arm_expand_prologue` (5712). It collects r0-r10 from `regs_ever_live`
  (5732-5735), adds lr if lr is live (5737-5738), and then does
  `if (live_regs_mask) { live_regs_mask |= 0x4000; emit_multi_reg_push
  (...) }` (5757-5762, "If we have to push any regs, then we must push
  lr as well"). Nothing in between can clear bit 14.
- **The other paths don't help.**
  - A `volatile`/noreturn function (`TREE_THIS_VOLATILE`, 5722) skips
    both loops (5732, 5737), so it pushes nothing at all, not r4-r6.
  - The pretend-args push (`0xf0 >> n`) only covers r0-r3.
  - `frame_pointer_needed` adds fp/ip/lr/pc (`0xD800`).
  - `naked` (`arm_naked_function_p`, 5726) emits no prologue at all.
  - This version has no `interrupt`/`isr` attribute (no match in
    `arm.c`).
  - `-mapcs-frame` only adds a frame.
  - The only `stmfd sp!, {rN, rN+1, rN+2}` printed outside the prologue
    is `output_mov_long_double_fpu_from_arm` (4356), an FPA move that
    is followed by an `ldfe`.
- **lr can't be dropped from the restore either.**
  `output_func_prologue` sets `lr_save_eliminated` only when nothing else
  is saved (5322-5342). The epilogue then puts lr back into every restore
  (5565-5566, 5593-5594). A return insn adds lr's slot whenever any
  register is saved (5172-5181).
- **The same holds even if lr isn't used.** With pins (below), the draft
  never touches lr, as in the ROM. agbcc_arm still prints
  `stmfd sp!, {r4, r5, r6, lr}` / `ldmfd sp!, {r4, r5, r6, lr}`. That is
  two instructions the ROM has without lr.

**itoa_arm: the `cmp r1, #10` + `addge`/`addlt` test.**

- **The tree folder.** fold-const.c 6005-6022 rewrites `X >= C` to
  `X > C-1` and `X < C` to `X <= C-1` for any positive constant. That
  covers every spelling tried: `<`, `>=`, `!(>=)`, `(... >= 10) == 0`, a
  `(s16)`/`(s64)` cast, and `10 + 0 * num`. All give `cmp #9`. A `(u64)`
  cast gives `ls`/`hi`, and `digit - 10 < 0` gives `cmp #10` with
  `mi`/`pl`.
- **combine.** A local `s32 ten = 10` survives the tree folder and
  gives the ROM's `lt`/`ge`, but as `cmp r3, ip` with the 10 loaded
  outside the loop. Without that load (a goto loop, so loop.c doesn't
  hoist it), combine substitutes the 10 into the compare.
  `simplify_comparison` (combine.c 9775-9784) then makes it `LE 9`
  again.
- **What's left.** The only way left for the immediate to reach the
  compare after combine is reload putting in a spilled pseudo's
  `REG_EQUIV` constant (`reload_cse_simplify_operands` only replaces
  constants with registers, not the reverse). That needs register
  pressure that itoa_arm doesn't have. So the test can't match either.
- **Arm order.** `if (digit >= 10) digit += 'A' - 10; else digit +=
  '0';` does give the ROM's order, the +55 arm first.

**itoa_arm: closest C.** The new draft pins base/buf/len/neg to the
ROM's ip/r6/r5/r4 (without the pins agbcc_arm keeps len in lr). It uses
`neg` for the '-' and as the swap's left index, as the ROM's r4 does.
`MATCH_CONST(len, 0)` keeps cse from reusing len's zero for
`neg = 0`. The draft compiles to 45 instructions, the ROM's count. What
still differs:

- the two blockers (lr in the push and pop, `cmp #9`);
- the prologue's schedule (`mov r5, #0` after the `cmp`, `cmp ip, #16`
  instead of `cmp r2, #16`);
- each loop's `cmp r0, #0` ahead of the `add r5`;
- `subne r4, r4, #45` instead of `movne r4, #0`. cse finds 0 as
  "'-' minus 45". A `MATCH_CONST` there breaks the ccfsm block instead.
- the swap's temporary in r2 instead of r0.

objdiff scores it 72.6%.

**LookupSpriteFrameCache: no return insn pops into lr.**

- **Three exits mean three return insns.** The ROM's three
  `ldmfd sp!, {lr}; bx lr` exits can't come from the text epilogue,
  which final.c prints once per function (`FUNCTION_EPILOGUE`,
  final.c 1403).
- **The text epilogue alone would be right.** `output_func_epilogue`
  prints exactly `ldmfd sp!, {lr}` + `bx lr` for an interworking
  function that saved only lr (5562-5575). But when return insns were
  used, it skips itself (5372, `use_return_insn (FALSE) &&
  return_used_this_function`).
- **Every return insn pops into ip.** The return insns are the
  `return`, `*cond_return` and `*cond_return_inverted` patterns
  (arm.md 4340-4395), and all three call `output_return_instruction`
  with `really_return` = TRUE. Without a frame pointer, that function
  pops the return address into ip when interworking (5213-5215,
  `if (TARGET_THUMB_INTERWORK && really_return) strcat (instr,
  reg_names[12])`) and then prints `bx ip` (5222-5228,
  `frame_pointer_needed ? "lr" : "ip"`).
- **The other paths don't help.**
  - Popping into lr needs `frame_pointer_needed` (`ldmea fp, {fp, sp,
    lr}`).
  - The call+return peepholes print `ldmfd sp!, {lr}` with `b func`
    (`really_return` = FALSE), a tail call, not `bx lr`.
  - Without interworking the pop goes into pc.
  - Making `use_return_insn` false (varargs, a frame, `naked`) turns every
    return into a jump to the one epilogue.
  - gcc 2.9 has no basic-block duplication, and the epilogue isn't RTL,
    so cross-jumping or an `asm("")` can't copy it.

**LookupSpriteFrameCache: the hoisted constant is fixed.** The second
pass couldn't keep `vramAddr - 0x06010000` in place in the first loop.
The cause:

- **The expander.** The addsi3 expander calls `arm_split_constant` with
  `subtargets = preserve_subexpressions_p ()`. With
  `-fexpensive-optimizations` that is always 1 (stmt.c 2334). So the
  -0x06010000 is built in its own register (`mov rN, #0xF9000000; add
  rN, rN, #0xFF0000`), and loop.c hoists that register.
- **The `-fno-expensive-optimizations` result.** With that flag,
  `preserve_subexpressions_p` is 1 only within `n_non_fixed_regs * 3`
  insn UIDs of the loop's start label (stmt.c 2337-2344). That is why
  only the second loop, whose hit block is far from its start, stayed
  in place.
- **The fix.** Subtract the constant in two statements, each a valid
  immediate: `vramAddr -= 0x07000000; vramAddr += 0xFF0000;` in an
  `ObjTileIndex` inline. Expansion then needs no constant register.
  combine merges the two into one `plus` with -0x06010000, which
  `*addsi3_insn` accepts through its `?n` alternative. The split before
  sched1 prints it in place as `add r0, r0, #0xF9000000; add r0, r0,
  #0xFF0000`, as in the ROM.

**LookupSpriteFrameCache: the result.** With the object's own flags
(`-O2 -fomit-frame-pointer`, no `-fno-expensive-optimizations`), the
draft compiles to the ROM's 50 instructions. 44 of them are identical,
with the same registers, schedule, `ldm ip, {r1, r3}` peephole and
literal pool. The six that differ are the three
`ldmfd sp!, {ip}; bx ip` returns, where the ROM has `{lr}; bx lr`.
objdiff scores it 99.4%.

**Other things tried.**

- A goto-built first loop also keeps the constant in place. But
  loop.c's `find_and_verify_loops` (loop.c 2758-2900) then moves the
  second loop's hit block after the first return's barrier, so it was
  dropped.
- Up to 20 `MATCH_BARRIER()`s in each hit block (to raise loop.c's insn
  count) didn't stop the hoisting.
- MATCH_CONST of the constant wasn't needed.

**Verdict.** Both functions stay `NAKED`. Each blocker is a fixed
string in `arm.c`'s output code (5215/5227 for the returns,
5757-5762 for the push), not something C shape or flags reach.
Matching them needs the later Cygnus/Red Hat build the third pass
pointed to, or a modified compiler. A modified compiler isn't a real
toolchain, so this pass didn't build one.

## Sixth pass (decomp-permuter)

This pass ran [decomp-permuter](https://github.com/simonlindholm/decomp-permuter)
on both drafts with agbcc_arm. The setup is now in
[tools/permuter/](../../tools/permuter/README.md).

**Setup.** The compile script preprocesses with the repo's include
paths and compiles with the objects' flags (`-mthumb-interwork -O2
-fomit-frame-pointer`). The target `.o` is the `NAKED` asm, assembled
on its own. `itoa_arm`'s `base.c` is the draft with the pins and the
SWI in `PERM_IGNORE`, behind `#pragma _permuter latedefine` copies of
the macros. `LookupSpriteFrameCache`'s has `PERM_GENERAL` alternatives
for each return (a direct `return`, a `goto` to a shared return, a
result variable with one `return`) and for the first loop's kind. The
script also saves any candidate that pushes without lr, or that has two
`ldmfd sp!, {lr}; bx lr` returns, whatever its score.

**Runs.** Two sessions, `-j24`, `--better-only`:

| Function | Iterations | Base score | Best score |
|---|---|---|---|
| `itoa_arm` | about 2.56 million (1.97M, then 0.59M from the improved draft) | 1060, then 535 | 525, then 320 |
| `LookupSpriteFrameCache` | about 1.98 million, plus about 2,300 with the return alternatives (most of which don't compile) | 30 | 30 |

Neither function reached 0, and nothing showed either blocked shape.
Both floors are the fifth pass's blockers: `LookupSpriteFrameCache`'s 30
is its three return pairs, and the permuter never got under it.

**The lr-pop hit was a false positive.** An early version of the
script counted `ldmfd sp!, {lr}` lines alone. It saved one
`LookupSpriteFrameCache` variant that ends in `return (float) new_var;`
on both arms of an `if`. Compiled with the object's flags, it has
`stmfd sp!, {lr}` and no frame pointer. Its two `ldmfd sp!, {lr}` are
the call+return peephole's tail calls (`bl __floatsisf; ldmfd sp!,
{lr}; b __fixsfsi`). The two real returns are still `ldmfd sp!, {ip};
bx ip`. That is the fifth pass's `really_return` = FALSE path, so it
doesn't help. The script now needs `bx lr` right after the pop.

**itoa_arm: two fixes for the draft.**

- **The hex test matches.** `if (digit >= (ten = 10))` gives the ROM's
  `cmp r1, #10` + `addge`/`addlt`. fold-const.c's `X >= C` to `X > C-1`
  rewrite needs a constant operand, and the assignment isn't one. The
  constant survives to the compare. This overturns the fifth pass's
  verdict on the test, which only tried a separate `ten` variable.
- **`mov r5, #0` ahead of the `cmp`.** The sign test, `neg` and
  `b = buf` inside `do { ... } while (0)` move the zeroing of `len` back
  before `cmp r0, #0`, as in the ROM.

The draft's diff against the ROM dropped from 32 to 22 instruction lines
(16 to 11 instructions, out of 45). The difference left: the lr push/pop
(the blocker); `movge r4, #0` after the `movlt`/`rsblt` pair; `cmp ip,
#16` for `cmp r2, #16`; each loop's `add`/`cmp` order; the
terminator's `strb` and the swap's `add`/`sub` order; `subne r4, r4,
#45` for `movne r4, #0`; and the swap's temporary (r2 for r0). objdiff
scores it 72.8% (was 72.6%).

The permuter's best scores below these (320 for `itoa_arm`) all change
what the function does: an uninitialized local for `neg = 0`, or
`b[neg]` loaded once before the swap loop. They weren't used.
Its other `itoa_arm` hits (`b[len] = (neg = '-')` for `movne r4, #0`)
fix one line and break two (the '-' goes into r3).

**Verdict.** Unchanged: both stay `NAKED`, for the fifth pass's
reasons. `LookupSpriteFrameCache`'s draft is already at the floor, and
`itoa_arm`'s other differences are scheduling and register choices
around the blocker.

## Seventh pass: patched agbcc_arm

Both functions now match, as real C, with a locally patched agbcc_arm.
This is not the ROM's compiler. That compiler is a later build of
agbcc_arm's own Cygnus/Red Hat line (third pass), and it has never been
released. The fifth pass pinned its differences from agbcc_arm down to
two fixed strings in `arm.c`'s prologue and return code, which no C
reaches. With the owner's approval (#553), these two functions are built
with agbcc_arm plus a patch that emulates exactly those two behaviours,
each behind an opt-in option. Everything else in the compiler is stock.

**The patch.** `tools/agbcc_patches/agbcc_arm_prologue_return.patch`,
against SAT-R/agbcc's `gcc_arm/config/arm/arm.h` and `arm.c` (11 hunks;
the patch's header lists them):

- `-mleaf-no-lr-save`: when a function never uses or clobbers lr (no
  calls, lr never allocated) and has no frame pointer, the push and the
  restore leave lr out, and the function returns with `bx lr`. This is
  `arm_expand_prologue`'s "we must push lr as well" (5757-5762) and the
  epilogue's restore (5593), made conditional; the stack-argument offset
  (`INITIAL_ELIMINATION_OFFSET`) drops lr's slot to match, and
  `use_return_insn` leaves such a function to the text epilogue (a
  return insn would pop the return address from the missing slot).
- `-minterwork-return-lr`: an interworking return insn without a frame
  pointer pops into lr and returns with `bx lr`, where
  `output_return_instruction` uses ip (5213-5227).

Both are bits in `target_flags` with `-mno-` forms in
`TARGET_SWITCHES`. With neither given, every path is the stock one.
Checked: agbcc_arm and agbcc_arm_patched without the options give
identical assembly for all 333 C files in `src/` and `lib/` that
agbcc_arm compiles (as ARM, `-O2 -fomit-frame-pointer
-mthumb-interwork`, `NON_MATCHING=1`), and a full `make compare` of the
tree before this pass, with agbcc_arm_patched as the ARM compiler,
prints `crashbandicootxs.gba: OK`.

**Building it.** `tools/build_patched_agbcc_arm.sh <agbcc-dir> [<repo>]`
copies the agbcc checkout to a temporary directory, applies the patch
(unless the checkout already has it), runs SAT-R's `build.sh` steps for
`gcc_arm` (`configure --target=arm-elf --host=i386-linux-gnu`, with
`CC="gcc -std=gnu99 -w -fpermissive"` since on a current host gcc some
of the 1999 configure checks otherwise fail to compile and answer wrong,
`-fpermissive` only where gcc accepts it for C; then `make clean` - a built
checkout's objects are stock and the Makefile doesn't track `arm.h` -
and `make cc1`), and installs `cc1` as
`tools/agbcc/bin/agbcc_arm_patched`. agbcc, old_agbcc and agbcc_arm
stay as they are. CI ran it after "Install agbcc" until the eighth step,
which builds the C++ twin of this compiler instead.

**Which objects.** The Makefile's `PATCHED_ARM_OBJS` builds
`string_arm.o` and `sprite_arm.o` with agbcc_arm_patched, each with only
the option its function needs. Neither object had to be split: the other
four functions in each come out the same with and without the options.

- `sprite_arm.o`: `-minterwork-return-lr`. LookupSpriteFrameCache is the
  fifth pass's draft unchanged. Its 44 matching instructions stay, and
  its three returns become `ldmfd sp!, {lr}; bx lr`.
- `string_arm.o`: `-mleaf-no-lr-save`, plus `-fno-schedule-insns
  -fno-schedule-insns2` (next).

**itoa_arm: scheduling.** With the push fixed, the sixth pass's draft
was still off in five orderings: each loop's `add r5`/`cmp r0`, the
terminator `strb` against `sub r1, r5, #1`, and the swap's `add r4`/`sub
r1`. In the ROM all of them are in source order. sched.c moves them:
an insn anti-dependent on a store (the `add` that follows `strb r1, [r6,
r5]`) inherits the store's latency, gets a higher priority than the
compare, and is scheduled last. Either scheduling pass alone does it, so
only turning off both keeps the order. The four other string functions
come out the same either way. The sprite functions need scheduling, and
they're in another object. So the flags are per object, under the
project's rule (every function in the object stays exact). Whether the
original string file was built without scheduling or by a scheduler that
leaves this function alone can't be told from the ROM. Tried and
dropped: a third option making anti and output dependences free, as
later ARM back ends' `arm_adjust_cost` does. It fixed the orderings but
changed the four matched sprite functions, so the ROM's compiler doesn't
have it.

**itoa_arm: the C.** Without scheduling, the sixth pass's draft differs
in three places. Each has a cause in agbcc_arm's source and a C fix:

- **`movge r4, #0; movlt r4, #1; rsblt r0, r0, #0`.** The ROM has the
  `ge` arm first. As one if/else with `neg = 0` first, jump.c's "`if
  (...) { x = a; goto l; } x = b;` becomes `x = a; if (...) goto l; x =
  b;`" hoists `neg = 0` above the branch (`mov r4, #0; addlt r4, r4,
  #1`). With `num = -num` first in the other arm the transform doesn't
  apply, but then `rsblt` comes before `movlt`. Two ifs on the same test,
  `if (num >= 0) neg = 0; if (num < 0) { neg = 1; num = -num; }`, give
  the ROM: jump.c turns the first into a conditional move (`movge`), cse
  drops the second compare, and the ccfsm conditionalizes the second if.
  gcc then warns that `neg` might be used uninitialized. That is a false
  positive, left enabled on purpose: string_arm.o is built with
  `-Wno-error` (the Makefile's `UNINIT_WARNING_OBJS`, #662). The
  `MATCH_HOLD(neg)` that used to silence it emitted nothing.
- **`movne r4, #0` after `movne r4, #45`.** The draft's `subne r4, r4,
  #45` comes from `reload_cse_move2add` (reload1.c), which rewrites a
  constant load as an add from the register's last known constant. It
  only does that from a set of the same or a wider mode. The '-' is now
  a `u8` pinned to r4 (`MATCH_HOLD_REG(u8, minus, r4) = '-'`), so the
  later `neg = 0` (SImode) stays a `mov`.
- **The swap's temporary in r0 and `j` in r1.** The swap loads both bytes
  first (`lo = b[neg]; hi = b[j]; b[j] = lo; b[neg] = hi;`), with `hi`
  pinned to r0 and `j` to r1, as in the ROM. (#662 round 8: `j` and
  `hi` are `digit` and `num` reused, the SWI's r1 and r0, with no pins,
  as `neg` is reused for the left index.)

Also needed, from the earlier drafts: the pins on `digit`, `b`, `len`
and `neg`, `MATCH_CONST(len, 0)`, `(ten = 10)`, and a `MATCH_KEEP(base)`
after `divisor = base` (without it the `!= 16` test uses ip, not r2).
Not needed any more: the `do { } while (0)` around the sign test and
the pin on `divisor`. (The match doesn't need `num`'s pin either, but
the SWI takes it in r0, so it stays.)

**Result.** `itoa_arm` (45 instructions) and `LookupSpriteFrameCache`
(50 instructions) match byte for byte, as C, in the matching build. Their
`NAKED` transcriptions and `#if NON_MATCHING` drafts are gone. Every
function in the ROM is now matched.

## Eighth step: C++

The game is C++ (docs/cplusplus.md), and notyourav/agbcc's `cp` branch,
whose Thumb `g++/` tree gives agbcp, also has an ARM one, `g++_arm`:
agbcc_arm's gcc 2.9-arm-000512 (same version string, same
`config/arm/arm.c` and `arm.h`) with the C++ front end. Its `cc1plus` is
`agbcp_arm`. `tools/build_agbccpp.sh` builds it with this patch applied
(the hunks' `gcc_arm/` paths rewritten to `g++_arm/`; it applies
cleanly) and installs it as `tools/agbcc/bin/agbcp_arm_patched`, with
the same configure line as the branch's `build.sh` and agbcp's host
flags.

The two files compile as C++ unchanged but for their includes (wrapped
in `extern "C"`, so the functions keep their C names) and
HeapSortActorsByKey's list, now `ActorSelf **`, as its caller and the
`gHeapSortActorsByKeyFunc` hook declare it in C++. Checked, on both
files (assembled code compared, `-O2 -fomit-frame-pointer
-mthumb-interwork`):

- agbcc_arm (C) against stock agbcp_arm and agbcp_arm_patched (C++)
  without the options, with and without the scheduling passes: the same
  code.
- agbcc_arm_patched (C) against agbcp_arm_patched (C++) with each option
  and with both: the same code.
- The Makefile build (agbcp_arm_patched, the objects' flags plus
  `-fno-rtti -fno-exceptions`): `string_arm.o` and `sprite_arm.o` are
  byte-identical to the agbcc_arm_patched objects, and `make compare`
  prints `crashbandicootxs.gba: OK`.

So the Makefile builds them as C++ (`ARM_OBJS`), and CI no longer
builds agbcc_arm_patched: `tools/build_patched_agbcc_arm.sh` stays as
the C build of the same patch, for comparisons like the ones above.

## Ninth step: the two jump.c rules (#662)

After the eighth step two IWRAM functions still had matching
workarounds, both empty asm (`MATCH_BARRIER`), both against agbcc_arm's
jump pass rather than anything in the C:

- **strncpy_arm** (one barrier, second pass). The `n == 0` test branches
  to the final `bx lr` in the ROM (`beq`). jump.c turns any jump to a
  label that is directly followed by a return into a conditional return
  (`redirect_jump (insn, NULL_RTX)`, "turn it into a RETURN insn"), here
  `bxeq lr`. Its only gate is arm.c's `use_return_insn (TRUE)`, which
  holds for every frameless leaf that saves nothing (#662 round 4: the
  only ways out are a frame, pretend arguments or a register saved under
  interworking; a 3-argument leaf in r0-r3 has none). The barrier sat
  between the label and the return.
- **HeapSortActorsByKey** (two barriers, fourth pass). The second loop's
  entry test and its bottom test are the same two insns (`cmp r7, #1;
  ble`), and in the ROM both follow a label (0x308 is the first phase's
  `ble` target, 0x3d8 the sift loop's exits'). jump2's cross-jumping
  (`find_cross_jump`) needs two matching insns for a conditional jump,
  but one fewer when the run reaches a label ("those jumps will be
  tensioned to go directly to the new label"), so it merges the two tests
  either way round: with one barrier, the other test is merged. #662
  rounds 2-4 tried dead stores, `while (--n > 0)`, counted `for` loops,
  an `if (n > 1) do ... while` and inline sift functions; each loses a
  test or moves registers. Only a non-note insn between each label and
  its `cmp` that survives to jump2 and emits nothing stops it: a
  volatile asm, or a USE or CLOBBER, which a void function without calls
  doesn't produce.

#662 round 4 built two private agbcp_arm_patched variants: one without
the `--minimum` at a label in find_cross_jump, one whose
use_return_insn refuses conditional returns. The first compiles plain
HeapSortActorsByKey, and the whole of `sprite_arm.o`, byte-identical to
the ROM's; the second does the same for strncpy_arm and `string_arm.o`.
So the ROM's later ARM gcc has neither rule, as it lacks the two
prologue/return strings of the seventh pass. With the owner's approval
they are now two more opt-in options in
`tools/agbcc_patches/agbcc_arm_prologue_return.patch` (hunk 6 and the
new hunk 12; the header lists them):

- `-mno-cond-return`: `use_return_insn` is false for a conditional
  return (`iscond`), so jump.c leaves the branch alone.
- `-mstrict-cross-jump`: `find_cross_jump` keeps its full minimum after
  a label. The hunk is in `jump.c`, the patch's first outside
  `config/arm/`; it tests `TARGET_STRICT_CROSS_JUMP` under `#ifdef`, so
  the option is a `target_flags` bit like the others and no `toplev.c`
  change is needed. `g++_arm/jump.c` differs from `gcc_arm/jump.c`
  elsewhere (coverage and branch-probability code), so the hunk applies
  there 7 lines further down; `tools/build_agbccpp.sh` and
  `tools/build_patched_agbcc_arm.sh` apply it unchanged.

**Which objects.** `string_arm.o` adds `-mno-cond-return`, and
`sprite_arm.o` adds `-mstrict-cross-jump`. Each option changes only its
own function: with the plain sources, toggling `-mstrict-cross-jump` on
`sprite_arm.cpp` changes only HeapSortActorsByKey's code (the two
tests), and toggling `-mno-cond-return` on `string_arm.cpp` changes only
strncpy_arm's `bxeq lr`. The other option is a no-op on each object.
Both functions are now plain C++ with no barrier, and `sprite_arm.cpp`
no longer includes `match.h` (itoa_arm's pins keep it in
`string_arm.cpp`).

**Option-off identity.** Both scripts built from the same sources with
the previous patch and with this one (`agbcp_arm_patched` from
notyourav/agbcc's `cp` branch, `agbcc_arm_patched` from SAT-R/agbcc).
Same flags, old against new compiler, assembly compared:

- `agbcp_arm_patched` on both ARM objects (the sources before and after
  this step), with no option, each of `-mleaf-no-lr-save` and
  `-minterwork-return-lr`, both, `string_arm.o`'s full flag set, `-O1`
  and with a frame pointer: identical (14 compiles each).
- `agbcc_arm_patched` on every `lib/*.c` that compiles as ARM, plus the
  pre-#748 C `string_arm.c` and `sprite_arm.c`, with no option, both old
  options, and with a frame pointer: identical (126 compiles).

## Data

`iwram_data.cpp` defines every global from `0x030007CC` up to
`gIntrTable`, with an initialiser each so agbcp puts them all in
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
- Fifth pass (drafts only, both still `NAKED`): `make tidy`, full
  build, `make compare`: `crashbandicootxs.gba: OK`. `rm -rf build
  objdiff.json && make NON_MATCHING=1 report` and `objdiff-cli report
  generate`: code 242,990 / 243,378, functions 2,057 / 2,059, data
  8,036,176 / 8,036,176 (unchanged). Draft scores: itoa_arm 72.6%,
  LookupSpriteFrameCache 99.4%.
- Sixth pass (itoa_arm draft only, both still `NAKED`): `make tidy`,
  full build, `make compare`: `crashbandicootxs.gba: OK`. `rm -rf build
  objdiff.json && make NON_MATCHING=1 report` and `objdiff-cli report
  generate`: code 242,990 / 243,378, functions 2,057 / 2,059, data
  8,036,176 / 8,036,176 (unchanged). Draft scores: itoa_arm 72.8%,
  LookupSpriteFrameCache 99.4%.
- Seventh pass (both matched with agbcc_arm_patched): agbcc_arm_patched
  built from scratch with `tools/build_patched_agbcc_arm.sh` (from the
  SAT-R checkout, and from a copy stripped of its build products). `rm
  -rf build crashbandicootxs.elf crashbandicootxs.gba
  crashbandicootxs.map && make compare`: `crashbandicootxs.gba: OK`. `rm
  -rf build objdiff.json && make NON_MATCHING=1 report` and
  `objdiff-cli report generate`: code 243,378 / 243,378 (100%),
  functions 2,059 / 2,059, data 8,036,176 / 8,036,176 (100%).
  Option-off identity: as above (333 files, and the pre-pass tree's
  `make compare` with agbcc_arm_patched).
- Ninth step (strncpy_arm and HeapSortActorsByKey plain): both compilers
  rebuilt from scratch with `tools/build_agbccpp.sh` and
  `tools/build_patched_agbcc_arm.sh`. `make clean && make compare`:
  `crashbandicootxs.gba: OK`. `make NON_MATCHING=1 report` and
  `objdiff-cli report generate`: code 243,378 / 243,378 (100%),
  functions 2,059 / 2,059, data 8,036,176 / 8,036,176 (100%).
  `tools/match_idioms.py --functions`: `src/iwram/` 9 of 10 functions
  without workarounds (was 7; itoa_arm keeps its pins). Option-off
  identity: as above.
