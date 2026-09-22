# `0x08039214` - word-wrap text/console-tile renderer (issue #67)

`sub_8039214` (`src/audio/gax_text_render.c`) is the fatal-error screen's
(`sub_80392E0`) text renderer: it writes a NUL-terminated ASCII string as
tile indices into BG screen block 0's tilemap, starting at a given
`(col, row)` in 8x8-tile units, word-wrapping to the next tile row
whenever the current word would run past column 29 without hitting a
space/newline/NUL first. Previously left raw (`docs/status/audio.md`
called it "not attempted this pass - complex nested-loop control flow
deprioritized"); this pass matched it as real C.

## Address math and character mapping

`dst = BG_SCREEN_ADDR(0) + col*2 + row*64` (64 bytes/row = 32 tiles * 2
bytes each). Before placing each character, a lookahead scan checks up
to the rest of the current line (columns up to 29) for the next
space/newline/NUL; if none is found, the cursor wraps to the start of
the next tile row first.

ASCII is remapped onto this font's tile order: `'.'`/`':'`/`'_'` are
special-cased onto tile indices normally reserved for higher ASCII
values (`0x5b`/`0x5c`/`0x5d`) so they fall through the same
general-purpose bucket as everything else; `'\n'` re-aligns the cursor
to the current row's start plus one odd byte (kept exactly as the ROM
computes it - not "fixed" - since this renderer's two real callers,
both from `sub_80392E0`, never actually feed it a literal newline).
The general bucket then maps `' '` -> tile 0, `'0'`-`'9'` -> tiles
1-10, `'A'`-`'Z'` -> tiles 11-36 (anything `-0x20` of that, i.e.
lowercase, folds onto the same range - this font has no separate
lowercase glyphs), and the three remapped punctuation marks land at
tiles 37-39 right after `'Z'`.

## Why every local is register-pinned

This function has no calls at all (nothing to save/restore across a
`bl`), and the ROM keeps `dst`/`colIdx`/`s`/`c`/`peekSeed`/`len`/`mask`/
`peek` alive simultaneously across the word-wrap scan - genuinely
filling every low register (`r0`-`r7`) plus `ip`. An unconstrained
build reliably picks different registers for these locals (confirmed
by removing each pin in turn and rebuilding) even though the generated
*shape* is otherwise identical - so every one is pinned to match.

`colIdx` (`r1`) does double duty exactly as the ROM's own `r1` does:
the word-wrap column index while scanning ahead, then (once that value
is dead) reused for the character being remapped into a tile index -
this is the ROM's own register reuse, not a naming accident in the C.

## Gaps that needed more than a plain rewrite

A plain, idiomatically-written version of this function matches the
ROM's overall control-flow shape almost immediately, but four separate
spots needed extra work to land byte-exact:

1. **A redundant loop-back bounds check.** The ROM re-tests
   `colIdx > 0x1f` at the top of *every* lookahead iteration, not just
   once on entry - dead in practice (the `colIdx > 0x1d` wrap check
   below always fires first), but present in the ROM's own bytes.
   Reproduced with a `goto`-driven first-iteration skip into the middle
   of the lookahead `for` loop, matching gcc's own loop-rotated
   codegen instruction-for-instruction.

2. **An "r0-first" routing the ROM uses for every register-to-register
   AND/subtract that lands in a long-lived pinned register.** Plain C
   (`dst &= mask; dst += 0x40;`) lets gcc's copy propagation compute
   the AND directly into `dst`'s own register (`and r3,r3,r0`) - the
   ROM instead always routes it through `r0` first
   (`mov r0,ip; and r0,r3; add r3,r0,#0`), and the same pattern recurs
   for the column-index computation (`(dst & 0x3f) >> 1`) and for each
   of the three tile-arithmetic subtractions. A plain temporary gets
   optimized away every time (even `volatile`, which instead forced a
   pointless stack round-trip); what actually reproduces the ROM's
   bytes is a `register T tmp asm("r0")` genuinely distinct from the
   destination register, so gcc has no choice but to compute into one
   register and then copy into the other - for the two AND/wrap sites
   this needed a real `asm volatile` block (a bare register-pinned
   local still let gcc discard the "dead" copy), but for the three
   subtraction sites a plain `register s32 tmp asm("r0") = colIdx - N;`
   was enough once `tmp` and `colIdx` were pinned to genuinely
   different registers.

3. **A provably-dead-but-present re-check in the tile-selection
   chain.** After the `c <= 0x40` and `c > 0x60` branches, the ROM
   still re-tests `c > 0x40` (via its own dead `bls`-to-store branch)
   before the final subtraction, even though the two branches above it
   already establish that. Kept as a real `else if ((u32)colIdx >
   0x40)` with no trailing `else` (rather than simplified to a plain
   `else`), since collapsing it changes gcc's cross-jump merging of
   the three arithmetic branches' shared truncation tail.

4. **Statement order inside the per-character remap step.** The ROM
   copies the freshly-read character into its working register
   (`adds r1, r6, #0`) *before* advancing `s` to the next character
   (`adds r5, r7, #0`); the natural C order (advance-then-copy) is
   semantically identical but swaps which instruction comes first in
   the assembled output.

5. **Trailing alignment padding.** `sub_8039214`'s body ends on a
   2-byte boundary just short of 4-byte alignment; gcc's default
   `.align` fill (an actual `mov r8, r8` "nop" instruction) doesn't
   match the ROM's zero-byte padding there - fixed with the
   established `asm(".align 2, 0");` technique (see
   `matching_decomp_alignment_fix` memory), placed at file scope right
   after the function (not inside it, which would insert the padding
   mid-function ahead of the shared epilogue instead of after it).

Confirmed via a full clean `rm -rf build && make NON_MATCHING=1 report`
followed by `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`sha1sum -c checksum.sha1`
prints "La suma coincide") - not just the isolated per-function
compile used while iterating.
