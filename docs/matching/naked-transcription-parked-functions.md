# Six parked functions converted to NAKED transcription (byte-correct, not decompiled C)

Six previously-`NON_MATCHING` functions, spread across `src/util/`,
`src/graphics/`, and `src/system/`, all shared the same shape: fully
understood semantics (a real, working C reconstruction already existed
for each, checked into git history under `#if NON_MATCHING`), blocked
purely on this project's well-documented gcc-2.9 "last mile"
codegen-choice gaps - loop-invariant hoisting, value-propagation
folding, peephole store+increment fusion, branch-sense
canonicalization, and argument-spill ordering - that no amount of
respelling, reordering, or register-pinning the C source could close.
Each was converted to a byte-verified NAKED asm transcription instead,
the established pattern for this class of gap (see
`src/system/link_cable.c`'s `sub_8001CB8`/`sub_8001DB4` and
`src/util/math_div_util.c`'s `nullsub_8` for the earliest examples).

**Tracking note**: byte-exact NAKED asm is not treated as "matched" in
this project's `tools/report_units.py`/`docs/status/*.md` tracking -
only real decompiled C counts, even when the NAKED transcription is
provably byte-correct against the ROM. All six functions below (and
their siblings converted the same pass - `sub_80019F8` in
`src/audio/audio_context.c`, and the eight
`src/graphics/settings_menu.c`/`settings_menu23.c` functions) are
tracked as **parked**, not matched, in those files. The functions
themselves are still real, working, byte-verified code - only the
progress-tracking classification differs from an ordinary match.

## Parked - byte-correct NAKED transcriptions

- **`sub_8000CBC`** (`src/util/printf_util.c`) - a case-insensitive
  `strstr`. The C reconstruction's only gap was one branch shape inside
  the "normalize a char to lowercase" logic: the ROM routes the
  *untaken* branch of the range check through a redundant copy-into-r0
  before a shared truncate, where a natural `if (cond) x += 0x20;`
  branches straight past it - reproducing that exact shape pushed
  register pressure just far enough to spill `caseInsensitive` into r8,
  a worse mismatch than the one it fixed. See the function's own doc
  comment for the full derivation. **Since matched as real C** - see
  [naked-sub_8000cbc-matched.md](./naked-sub_8000cbc-matched.md); this
  entry is left as-is since it's a frozen historical record of why the
  function was originally parked (see `docs/matching.md`).
- **`sub_8000EE4`** (`src/graphics/text_layout.c`) - a word-wrap text
  renderer. The C reconstruction matched instruction-for-instruction
  except ~8 bytes from two non-semantic codegen details: agbcc always
  spills stack-homed incoming arguments before any register-pinned move
  runs (the ROM does it in the other order), and two loop-bound
  comparisons compile to a single inverted branch here where the ROM
  has a redundant two-instruction "correct-sense compare, branch on
  true, fall to an unconditional far branch" pair (likely a Thumb
  conditional-branch-range artifact from the ROM's original build).
- **`sub_80010E0`** (`src/system/input_util.c`) - polls input until a
  button match or a poll-count timeout. The C reconstruction matched
  everywhere except one 4-byte residual: the count-limited loop's
  `if (keys & 1)` bit-test compiled with the opposite branch sense from
  the ROM (same two instructions, same size) - the identical check in
  the function's *other* loop variant already matched the ROM's sense
  with no special handling, pointing at a fixed gcc-2.9
  canonicalization for this exact shape. **Since matched as real C** -
  see [naked-sub_80010e0-matched.md](./naked-sub_80010e0-matched.md);
  this entry is left as-is since it's a frozen historical record of why
  the function was originally parked (see `docs/matching.md`).
- **`sub_80014A4`** (`src/graphics/fade_screen_mode.c`) - the
  fade-to-black palette DMA loop. The ROM caches the blended-buffer
  address in a register across the loop while recomputing the other two
  DMA fields fresh every iteration; this compiler's loop-invariant
  hoisting never reproduces that specific split - giving the buffer
  address its own local gets it cached but also hoists at least one of
  the other two fields.
- **`sub_8001524`** (`src/graphics/fade_screen_mode2.c`) - sets a
  packed shadow byte's low 3 bits. This compiler always recognizes `-8`
  as reachable from the already-loaded `7` mask via a single `SUB` and
  folds the ROM's fresh `movs r1,#8; rsbs r1,r1,#0` pair into that
  shorter subtract, regardless of how the constant is spelled. **Since
  matched as real C** - see
  [naked-sub_8001524-matched.md](./naked-sub_8001524-matched.md); this
  entry is left as-is since it's a frozen historical record of why the
  function was originally parked (see `docs/matching.md`).
- **`sub_8001624`** (`src/graphics/aabb_util.c`) - commits a blend-
  register shadow. The ROM writes a word then does a separate `adds
  r2,#4` on the same register before the second store; this compiler
  always fuses that store-then-increment-same-register pair into a
  single `stmia r2!,{r0}`, an unavoidable peephole optimization for
  this exact instruction pair. **Since matched as real C** - see
  [naked-sub_8001624-matched.md](./naked-sub_8001624-matched.md); this
  entry is left as-is since it's a frozen historical record of why the
  function was originally parked (see `docs/matching.md`).

## Mechanics

Each raw `.s` fragment (`asm/code_3_1_2.s`, `asm/code_3_1_3.s`,
`asm/code_3_1_5.s`, `asm/code_3_1_7.s`, `asm/code_3_1_8.s`,
`asm/code_3_1_9.s`) held only its one now-NAKED function, so each was
deleted outright rather than trimmed, and its `ldscript.txt` line
dropped - the neighboring already-matched objects on either side link
back-to-back with no gap. The raw disassembly was transcribed
instruction-for-instruction into a `NAKED` C function (unified-syntax
mnemonics translated to the plain/divided syntax this project's other
`NAKED` functions and `arm-none-eabi-as`'s default mode use:
`adds`->`add`, `movs`->`mov`, `ands`->`and`, `lsls`/`lsrs`->`lsl`/`lsr`,
`asrs`->`asr`, `rsbs rX,rX,#0`->`neg rX,rX`), with the original
`_08XXXXXX:` labels renumbered to GNU-as local numeric labels (`N:`,
referenced `Nf`/`Nb`) since a `NAKED` function's asm block can't use the
real ROM address as a label.

## Verification

Full clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` passes (`La suma coincide`) with
all six converted at once, alongside `sub_80019F8`
(`src/audio/audio_context.c` - see
`docs/matching/issue-3-overlay-ui-audio-wrapper.md`) and the eight
`src/graphics/settings_menu.c`/`settings_menu23.c` functions (see
`docs/matching/issue-6-0x08003f30-overlay-ui.md`) converted the same
way in the same pass. `make NON_MATCHING=1 report` also still succeeds
- none of these six had a `NON_MATCHING`-only reconstruction left
behind to fall back to, since the byte-verified NAKED version is now
the only definition. `tools/report_units.py`'s `UNITS` entries for all
15 functions across both passes carry `base_object=None` (parked, not
matched) per this project's tracking policy - see the "Tracking note"
above.
