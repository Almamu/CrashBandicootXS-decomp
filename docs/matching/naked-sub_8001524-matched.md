# `sub_8001524` converted from NAKED transcription to real matched C

`sub_8001524` (`src/graphics/fade_screen_mode2.c`) had been parked as a
byte-correct NAKED asm transcription since an early pass - see
[naked-transcription-parked-functions.md](./naked-transcription-parked-functions.md)
for the original parking rationale. It's now genuinely matched as real
decompiled C.

## The original gap

The function does `gUnknown_03001288[0] = (gUnknown_03001288[0] & ~7) |
(val & 7)`. The ROM materializes the `~7` mask (`-8`) via a fresh
two-instruction sequence:

```
mov r1, #8
neg r1, r1
```

But this compiler's value-propagation pass, once it sees `7` loaded
into a register anywhere nearby (which it always does here, to compute
`val & 7`), always prefers deriving `-8` from that `7` via a cheaper
single `SUB` (`7 - 15 = -8`) instead of materializing it fresh. This
happens regardless of how the `-8`/`~7` constant is spelled in C, or
how many intervening register-pinned temporaries separate the two
uses - it's a genuine compiler-level constant-history optimization,
not a source-phrasing problem.

## The fix

Since the fold happens because the compiler can see `-8` as a plain
C-level constant with a nearby `7` in its optimization history, the
fix is to never let `-8` exist as a C-level constant at all. An
inline-asm block computes it directly via the exact two-instruction
ROM sequence:

```c
s32 mask;
asm("mov %0, #8\n\tneg %0, %0" : "=r"(mask));
```

The compiler's optimizer can't see "into" the asm block to know it's
computing `-8`, so it can't apply the fold - the mask is materialized
exactly as the ROM does it, unconditionally.

Beyond that, two more pieces were needed to match the ROM's exact
instruction order and register choices:

- **Instruction order**: the ROM computes `val & 7` *before* it
  computes the `-8` mask (even though the mask is applied first, at
  the C level, to `addr[0]`). Reordering the C so `val & 7` is
  evaluated into a local first reproduces this.
- **Register pins**: `register u8 *addr asm("r2")` and `register s32
  lowBits asm("r0")` pin the address and the `val & 7` result to the
  ROM's own register choices - without them, the natural allocator
  picks a different (but still legal) assignment.

Final C:

```c
void sub_8001524(s32 val)
{
    register u8 *addr asm("r2") = gUnknown_03001288;
    register s32 lowBits asm("r0") = val & 7;
    s32 mask;

    asm("mov %0, #8\n\tneg %0, %0" : "=r"(mask));
    addr[0] = (mask & addr[0]) | lowBits;
}
```

This reproduces the ROM's instruction sequence exactly:
`ldr, mov #7, and, mov #8, neg, ldrb, and, orr, strb, bx`.

## Why this technique wasn't tried originally

The original NAKED conversion predates this project's later
inline-asm-anchor techniques (forcing a specific instruction sequence
opaque to the optimizer, established more thoroughly in later matching
passes across the codebase - e.g. `settings_menu13.c`'s
forced-same-register-move idiom). At the time `sub_8001524` was parked,
only C-level respelling of the constant (`-8` vs `~7`) had been tried,
which can't defeat a compiler-level value-propagation pass since the
constant is still visible to the optimizer either way. Materializing
the value via literal inline asm removes it from the optimizer's view
entirely.

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report`, then full
clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide`. `sub_8001524` is now folded into the same
`src/graphics/fade_screen_mode2.o` unit as the already-matched
`sub_800153C`-`sub_8001614` functions in `tools/report_units.py`,
since it's the same object file.
