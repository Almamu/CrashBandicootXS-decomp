# `sub_8000CBC` converted from NAKED transcription to real matched C

`sub_8000CBC` (`src/util/printf_util.c`) had been parked as a
byte-correct NAKED asm transcription since an early pass - see
[naked-transcription-parked-functions.md](./naked-transcription-parked-functions.md)
for the original parking rationale. It's now genuinely matched as real
decompiled C.

## The original gaps

The function is a case-insensitive `strstr`: scan `haystack0` for the
first occurrence of `needle`, lowercasing both sides byte-by-byte
before comparing whenever `caseInsensitive` is nonzero. It had two
independent compiler gaps.

### Gap 1: the lowercase-fold branch shape

Every "normalize a char to lowercase" site (there are four: the
needle's first char, each outer-scan haystack char, and both chars in
the inner verify loop) has the same ROM shape:

```
add r0, rX, #0
sub r0, #0x41
lsl r0, r0, #0x18
lsr r0, r0, #0x18
cmp r0, #0x19
bhi 1f
add r0, rX, #0
add r0, #0x20
b 2f
1:
add r0, rX, #0
2:
lsl r0, r0, #0x18
lsr rX, r0, #0x18
```

Both the "changed" and "unchanged" arms explicitly copy into r0 before
a shared truncate writes the result back into `rX`. A plain
`if (cond) x += 0x20;`, or an equivalent ternary, lets gcc prove the
truncate is a no-op (since `x` is already a clean byte and the range
check keeps `x + 0x20` in range too) and folds it away, leaving only
`add rX, rX, #0x20` on the taken path with nothing on the untaken one -
the same "value-propagation defeats a redundant instruction" class of
gap as [`sub_8001524`](./naked-sub_8001524-matched.md). Fixed the same
way: each fold is emitted as one opaque `asm volatile` block the
optimizer can't see into, so it can't prove the truncate redundant.

### Gap 2: the inner loop's "needle exhausted" branch sense and layout

The inner verify loop's exit test (`if (nc == 0) return match;`)
compiled with the opposite branch sense from the ROM - a `bne` to a
same-iteration fallthrough, with the match-found code computed inline
right at the branch site, instead of the ROM's `beq` clear across to a
tail block shared with the epilogue:

```
ROM:                                  This compiler (naive C):
9:                                    ldrb r3, [r2]
ldrb r3, [r2]                         add r2, r2, #1
add r2, #1                            cmp r3, #0
cmp r3, #0                            bne  <continue, same block>
beq  15f      ; far, to the tail          sub r0, r5, #1
<continue-verify code, same block>        b    <epilogue>
...                                   <continue-verify code>
15: sub r0, r5, #1                    ...
16: pop {r4,r5,r6,r7}; pop {r1}; bx r1
```

This wasn't a condition-polarity issue (every rephrasing - positive,
negative, `switch`, a structured `for(;;)`/`break`) produced the same
inverted sense with the match-found value computed inline at the
branch. The actual driver was gcc's block linearizer always inlining a
short taken-branch target right next to the branch that reaches it,
regardless of source order for the *check* itself. Deferring the
match-found *computation* to a label placed after the entire
scan/verify loop - so it's the function's last basic block textually,
exactly where the ROM's own compiler put it, immediately before the
shared epilogue - gets this compiler to lay out the same way:

```c
inner:
    nc = *matchNeedle;
    matchNeedle++;
    if (nc == 0) {
        goto matchFound;
    }
    hc2 = *matchHaystack;
    /* ...rest of the loop body... */
    if (nc == hc2) {
        goto inner;
    }
    goto scan;

matchFound:
    return haystack - 1;
}
```

With `matchFound:` as the last label in the function (nothing but the
epilogue follows), the `if (nc == 0)` check compiles to a clean
`cmp`/`beq` straight across the rest of the loop body, and
`matchFound:`'s own `sub r0, r5, #1` falls directly into the pop
sequence with no extra branch - both matching the ROM one-for-one.

## Other small gaps closed along the way

- The `needleRest = needle + 1` setup: writing it as a single
  `needle + 1` expression compiles as `add r1,#1; mov ip,r1` (two
  instructions, computed in a low register first). The ROM does the
  increment against the high register `ip` directly, which Thumb can't
  do with an immediate operand, so it loads `#1` into r0 first:
  `mov ip,r1; mov r0,#1; add ip,r0` (three instructions). Splitting the
  assignment (`needleRest = needle;` then a separate increment,
  materialized via `asm volatile("mov r0,#1\n\tadd %0,r0")`) reproduces
  the ROM's exact sequence.
- Comparing the lowercase-folded values (`hc == c0`, `nc == hc2`, etc.)
  as `u8` re-extends them via `lsl #0x18` before the compare, because
  gcc can't see into the opaque asm fold to know the result is already
  byte-clean. Declaring the fold's output variables as `u32` instead of
  `u8` (the asm still only ever writes a byte 0-255 into them) lets gcc
  trust the value across the comparison and emit a direct `cmp`,
  matching the ROM.

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report`, `objdiff-cli
report generate` (no symbol-pairing errors), then full clean `rm -rf
build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map &&
make compare` - `crashbandicootxs.gba: La suma coincide`. `sub_8000CBC`
is folded into the same `src/util/printf_util.o` unit as the
already-matched `sub_8000AA8`/`sub_8000CA8` in `tools/report_units.py`,
since it's the same object file.
