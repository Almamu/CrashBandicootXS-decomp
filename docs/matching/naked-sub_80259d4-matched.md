# `sub_80259D4` converted from NAKED transcription to real matched C

`sub_80259D4` (`src/system/game_loop13.c`, sets a bit in both the
`self+0x208` and `self+0x308` bit-grids at once) had been parked as a
byte-correct NAKED asm transcription - see
[issue-41-game-loop-25894.md](./issue-41-game-loop-25894.md) for the
original parking rationale. It's now genuinely matched as real
decompiled C.

## The original gap

This is a true leaf function in the ROM - no `push`/`pop` at all,
`self` lives in `ip`/`r12` for the whole body. The ROM's first two
instructions:

```
mov ip, r0        ; self -> ip
add r2, r1, #0       ; n -> r2 ("t", n's own working copy)
```

A plain C reconstruction (`register u8 *base asm("ip") = self; s32 t =
n;`, or any rephrasing/statement reordering/explicit ordering barrier
between them) always emitted the `n`-copy *first*, regardless of C
source order - a fixed early-reload order for hard-register parameter
moves this compiler doesn't expose a way to influence from plain C.
Pinning both values to their exact ROM registers up front (tried in an
earlier pass, see `issue-41-game-loop-25894.md`'s "narrowed but not
closed" update) reintroduced an unwanted `push {r4, lr}`/`pop {r4}`
pair instead.

A second, more subtle gap surfaced once the first was closed: the ROM
never touches `r2` (`n`'s copy, `t`) again after the sign-check - the
"clamp negative indices" adjustment (`+= 0x1f`) happens on a
*separate* register (`r0`), and the later `bitIndex` computation reads
back the *untouched* `t`/`r2` value, not a re-derived one:

```
add r0, r2, #0      ; adjusted = t (a *copy*, not t itself)
cmp r2, #0             ; tests t directly, not the copy
bge 1f
add r0, r0, #0x1f          ; only the copy gets adjusted
1:
asr r0, r0, #5
...
lsl r0, r0, #5
sub r0, r2, r0                ; bitIndex = t (still pristine!) - wordIndex<<5
```

A single `t` variable adjusted in place (`if (t < 0) t += 0x1f;`)
doesn't reproduce this - once adjusted, the original value is gone.

## The fix

```c
void sub_80259D4(void *self, s32 n)
{
    register u8 *base asm("ip");
    register s32 t asm("r2");
    s32 adjusted, wordIndex;
    register s32 bitIndex asm("r0");
    register s32 mask asm("r2");
    register s32 shifted asm("r3");
    register s32 addr asm("r1");

    asm volatile("mov %0, %2\n\tadd %1, %3, #0" : "=r"(base), "=r"(t) : "r"(self), "r"(n));

    adjusted = t;
    if (t < 0) {
        adjusted += 0x1f;
    }
    wordIndex = adjusted >> 5;
    shifted = wordIndex << 2;

    addr = 0x208;
    addr += (s32)base;
    addr += shifted;
    bitIndex = t - (wordIndex << 5);
    mask = 1 << bitIndex;
    *(s32 *)addr |= mask;

    addr = 0x308;
    addr += (s32)base;
    addr += shifted;
    *(s32 *)addr |= mask;
}
```

Two techniques closing the two gaps:

1. **The self-stash/n-copy pair is one opaque `asm volatile` block**,
   forcing the ROM's exact order (`mov ip, self` then `add t, n, #0`)
   regardless of what this compiler's own scheduling would otherwise
   pick - the same "opaque to the optimizer" pattern used throughout
   this project's other NAKED-to-matched conversions, just applied to
   instruction *order* here rather than a folded/hoisted value.
2. **A second local, `adjusted`, instead of modifying `t` in place**
   reproduces the ROM's register split: `t` (pinned to r2) stays
   untouched by the sign-check adjustment, matching the ROM's own
   `cmp r2, #0` (testing the pristine value) and later `sub r0, r2,
   r0` (reading it back unmodified for `bitIndex`), while `adjusted`
   (an ordinary unpinned local) naturally lands in r0 for the
   `wordIndex` computation, exactly like the ROM's own separate
   register for that copy.

`bitIndex` and `mask` are both pinned to registers the ROM reuses
(`bitIndex` to r0, matching the ROM's `sub r0, r2, r0`; `mask` to r2,
matching the ROM's `mov r2, #1` reusing `t`'s now-dead register) -
without these pins, this compiler picks its own registers for both
(landing `mask` in r4, requiring an unwanted `push`/`pop` again).

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report`, `objdiff-cli
diff` against `build/expected/units/game_loop13_target.o` for
`sub_80259D4`: 100% match. `objdiff-cli report generate` succeeds (no
symbol-pairing errors). Full clean `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` -
`crashbandicootxs.gba: La suma coincide`. `sub_80259D4` is folded into
the same `src/system/game_loop13.o` unit as the already-matched
`sub_8025A0C`/`sub_8025A3C`/`sub_8025A44`/`sub_8025A5C` in
`tools/report_units.py`, since it's the same object file.
