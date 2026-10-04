# Issue #66: `0x08037648`-`0x08037FA0` (5 functions, `math_div64_util.c`)

`__divdi3`/`__udivdi3`/`__udivsi3`/`__muldi3`/`GaxZeroFill` sit
inside issue #66's nominally-`audio` chunk (`asm/code_3_2_20b.s`,
`0x08037648`-`0x08037FA0`), but - per `docs/audio.md`'s own existing
`__divdi3`/`__udivdi3` entries, which already flagged this address
range as "very likely not GAX2 code at all" - none of the five are
actually GAX2 engine code. All five are generic compiler-runtime
division/multiply helpers, the 64-bit-flavored siblings of
`src/util/math_div_util.c`'s already-matched 32-bit trio
(`__divsi3`/`__modsi3`/`__umodsi3`, issue #70). They're filed
under `src/util/math_div64_util.c` and category `util`, not `audio`,
for the same reason that file is - see `docs/workflow.md`'s category
convention ("about what a function does, not the ROM neighborhood it
happens to ship in").

## What each function does

- **`__divdi3`** - signed 64-bit division (`a / b`, truncating
  toward zero), the 64-bit counterpart of `__divsi3`. Negates both
  operands' magnitude (tracking overall sign as an all-0s/all-1s XOR
  word), then runs a three-block unsigned 64/64 long-division core
  built from repeated `__umodsi3`/`__udivsi3` calls against
  16-bit-digit chunks, normalized via `gStaticData_085A4C70` (a
  256-entry leading-zero-count table), and restores the sign on the
  64-bit quotient at the end. **UNUSED** - confirmed no caller anywhere
  in the ROM (every `asm/*.s`, `expected/code_3.s`, `expected/
  legacy.s`, and every matched `src/*.c` file checked). Its unsigned
  sibling `__udivdi3` *is* called (from `GaxChannelInit`'s NAKED body) -
  plausibly both symbols shipped in the same compiled library object
  (a common `libgcc`-style shape), so pulling in the used one dragged
  this one along at link time.
- **`__udivdi3`** - unsigned 64-bit division, the 64-bit counterpart
  of `__umodsi3`: the exact same three-block long-division core as
  `__divdi3`, minus the sign handling, with its own copy of the
  normalization table (`gStaticData_085A4D70`).
- **`__udivsi3`** - unsigned 32-bit division (quotient only, no
  remainder-correction mask) - the 64-bit division routines' own
  workhorse, called from both of them (and, project-wide, already
  declared `s32 __udivsi3(s32 value, s32 divisor)` by several
  already-matched files: `time_util.c`, `word_util.c`,
  `hud_icon_widget5.c`, `hud_stat_widget2.c`, `gax_playstart.c`,
  `gax_channel_table_alloc.c` - this file keeps that exact signature).
- **`__muldi3`** - 64x64->64 truncating multiply: this ROM's
  compiled copy of libgcc2.c's classic `__muldi3` with `__umulsidi3`'s
  16-bit-half `umul_ppmm` decomposition fully inlined.
- **`GaxZeroFill`** - a plain zero-fill memset helper via the BIOS
  `CpuFastSet` SWI, *not* part of the division/multiply family (it's
  simply the next function in ROM order) - see "What actually
  matched" below.

## The shared gap: a non-interworking return convention

`__divdi3`/`__udivdi3`/`__udivsi3`/`__muldi3` all return via
the ROM's combined `pop {r4-r7, pc}` (or, on `__udivsi3`'s
fallthrough path, a bare `mov pc, lr`) - a genuinely non-interworking
epilogue shape. Across this whole project's `expected/code_3.s`
(~120k lines, the frozen original disassembly), that exact
combined-pop-with-`pc` shape appears in only **7** places total, and
*all 7* are inside this one division/multiply cluster (these four plus
`math_div_util.c`'s own `__divsi3`/`__modsi3`/`__umodsi3`) -
strong, direct evidence this whole cluster was built without
`-mthumb-interwork`, unlike the rest of this ROM.

This isn't just precedent-following from `math_div_util.c` - it was
independently re-confirmed this pass by direct experiment:
`__muldi3` (no branches, no register-allocation ambiguity at all - a
completely straight-line multiply) reproduces every single body
instruction byte-for-byte from a plain-C `__muldi3` reconstruction,
*including* the exact `mul`/`and`/`orr` sequence - but both
`tools/agbcc/bin/agbcc` and `tools/agbcc/bin/old_agbcc` always emit the
interworking-safe split `pop {reg}; bx reg` instead of the ROM's
combined `pop {r4-r7, pc}`, with no C-level phrasing able to change
that (this project's `-mthumb-interwork` build flag is global). All
four are byte-verified NAKED transcriptions.

## What actually matched: `GaxZeroFill`

`GaxZeroFill` is a plain memset-via-`CpuFastSet` helper (byte-fills up
to 3 leading bytes to reach 4-byte alignment, zero-fills the largest
32-byte-aligned chunk via `CpuFastSet` with the
`FIXED_SRC` flag, then finishes any remainder one byte at a time) - and
it uses the *ordinary* interworking `pop {reg}; bx reg` return this
project's compiler does produce, unlike its four neighbors. It matched
as real C:

- The `CpuFastSet` control word's word-count needed the sign-correction
  and 21-bit mask written out explicitly (`if (corrected < 0) corrected
  += 3; ... ((u32)corrected >> 2) & 0x1FFFFF`) rather than as a plain
  `count / 4` - the plain form compiles to a bare `asr #2`, while the
  explicit form reproduces the ROM's `lsl #9`/`lsr #0xb` combined
  shift-and-mask.
- `cnt`/`remaining` needed explicit register pins (`r6`/`r1`,
  `matching_decomp_register_pinning`) to land in the ROM's own chosen
  registers - left unpinned, this compiler picks `r5`/`r6` instead.
- The tail loop needed a `do { ... } while (remaining != 0)` shape
  (after an `if (remaining > 0)` entry guard) rather than a plain
  `while (remaining > 0)` - the ROM's back-edge branch is `bne`
  (`!= 0`), not `bgt` (`> 0`); this was the one real functional
  mismatch caught only by the full clean `make compare` (an isolated
  compile had looked identical at a glance, illustrating exactly the
  "isolated compile is a diagnostic tool, not proof" caveat
  `docs/workflow.md` warns about).
- The function needed a trailing `asm(".align 2, 0")` to reproduce two
  bytes of zero padding before the next matched unit
  (`song_slot_lookup.o`) - `matching_decomp_alignment_fix`.

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report` followed by
`rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `La suma coincide`.

## Later pass: GAX toolchain retry

The non-interworking return was a build flag, not a C gap: built without `-mthumb-interwork` (Makefile `NO_INTERWORK_OBJS`), `__divdi3`/`__udivdi3`/`__muldi3` are gcc 2.x `libgcc2.c`'s own `__divdi3`/`__udivdi3`/`__muldi3` and now match as real C. `__udivsi3` is lib1funcs.asm hand-written asm and stays NAKED for good. `GaxZeroFill` moved to `src/audio/gax_zero_fill.c`. See [gax-toolchain-retry.md](./gax-toolchain-retry.md).
