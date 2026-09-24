# `sub_8000EE4` progress: 99.89% instruction match, still NAKED

`sub_8000EE4` (`src/graphics/text_layout.c`, a text-layout/word-wrap
renderer) is still a byte-correct NAKED asm transcription for the
default build - see
[naked-transcription-parked-functions.md](./naked-transcription-parked-functions.md)
for the original parking rationale. This doc records a much closer C
reconstruction (99.89% instruction match against the ROM per
`objdiff-cli diff`, up from 99.86% - see "Session update" below - kept
in the file under `#if NON_MATCHING`) and exactly which two small
residuals are still open, so a future attempt doesn't have to re-derive
any of this.

## Session update: a third, previously-undocumented gap found and closed

A follow-up session (prompted by this project's newly-discovered
matching-constraint-asm technique, see `sub_800A884`'s doc) re-derived
the whole-function instruction diff from scratch (extracting the exact
ROM bytes straight from this file's own NAKED transcription - which is
*defined* to be byte-identical to the ROM - assembling both sides
standalone, and diffing the normalized instruction streams line by
line) rather than trusting the previously-recorded "2 residuals"
count. This surfaced a **third, real divergence** the original
instruction-by-instruction pass had missed: the `/` escape dispatch's
*default* case (a byte that's neither `b` nor `n`) `goto`'d straight to
`bottom1`, skipping the `posAccum += tokenLen` step entirely - but the
ROM's default case actually jumps into the *middle* of the shared
`handleB`/`handleN` fallthrough tail, landing exactly on the
`posAccum += tokenLen` block (skipping only the earlier
widthAccum-reset/lineCount++/newline-draw part, which is genuinely
`handleB`/`handleN`-only). This was a real control-flow bug in the
reconstruction (an unrecognized escape byte silently dropped its
`tokenLen` contribution to `posAccum`), not just a register-choice
cosmetic - fixed by giving the `posAccum += tokenLen` statement its own
label (`incrementTail`) and having the default case jump there instead
of to `bottom1`:

```c
} else {
    goto incrementTail;
}
...
  lineCount = lcN; }
incrementTail:
{ register s32 posAccumR asm("r2") = posAccum;
  ...
```

This closed one whole mismatch category (2 instructions) with **zero**
regressions elsewhere (re-verified via the full normalized-instruction
diff, not just the touched spot), raising the match from 99.86% to
99.89011% per `objdiff-cli diff`. The two previously-documented
register-choice residuals (below) are unchanged and were re-attempted
this session with the new matching-constraint-asm technique described
in `sub_800A884`'s doc; neither could be closed (see "New technique
attempts this session" at the end of this doc for exactly what was
tried and why it didn't work, including one attempt that silently
*miscompiled* - worth recording so it isn't retried blindly).

## What the original NAKED write-up got wrong

The original parking rationale believed exactly two small, structural
gaps stood between a plain C reconstruction and a byte-exact match:

1. agbcc always spills the incoming `box`/`limit` register arguments to
   their stack homes as part of a fixed prologue pass, before any body
   code runs, regardless of C source order - and the ROM moves
   `self`/`cursor` into their pinned registers *first*.
2. Two loop-bound comparisons compile to a single inverted branch in
   plain C, where the ROM has a redundant "correct-sense compare,
   branch on true, fall to an unconditional far branch" pair at both
   spots - believed to be a Thumb conditional-branch-range artifact a
   small reconstruction never grew large enough to trigger.

Both turned out to be real, and both are closed below - but a third,
larger gap sat in between them that the original write-up's
instruction-by-instruction comparison apparently missed: a pair of
pointer locals (`xAddr`/`yAddr`, the render position within
`self->posX`/`self->posY`) that the ROM keeps live in a register across
several statements and spills to the stack only at the very last
possible moment (right before the specific register is about to be
reused for something else), where this compiler always spilled them to
the stack immediately on assignment, in a completely different
instruction order. Closing this gap is what makes gaps 1 and 2 above
actually reachable - it changes the function's size and stack-frame
shape enough that the frame-slot ordering and the branch-range
trampoline both change too.

## Gap 1: argument-spill ordering (closed)

Fixed by giving `self`/`cursor` their pinned-register values via
literal inline asm reading the incoming argument registers directly,
before any other C statement runs:

```c
asm volatile("mov %0, %1" : "=r"(cursor) : "r"(textParam));
asm volatile("mov %0, %1" : "=r"(self) : "r"(selfParam));
```

and, critically, **renaming the `box`/`limit` parameters** (to
`boxParam`/`limitParam`) and copying them into fresh, ordinary local
variables (`box = boxParam; limit = limitParam;`) instead of using the
parameters directly everywhere. Plain reordering of the C statements
never moved gcc's fixed argument-homing pass (confirmed by testing);
but once `box`/`limit` are no longer *the* incoming parameters gcc's
own argument-homing logic tracks, their stack spill becomes an
ordinary body-driven store that lands exactly where the C source puts
it - after the `self`/`cursor` asm moves.

## Gap 2: the redundant loop-bound-check trampoline (closed as a side effect)

Never fixed directly - it disappeared once gap 3 (below) was fixed and
the function's overall size/branch distances shifted enough to trip
gcc's own conditional-branch-range fallback, exactly as the original
write-up predicted, just from a different trigger than expected.

## Gap 3: `xAddr`/`yAddr`'s spill timing (closed - the actual hard part)

The ROM's shape for `self->posX = box->field_0; self->posY =
box->field_4;` followed much later (inside the `/b` escape handler) by
a read/write through the same two addresses:

```
ldr r2, [sp]          ; box
ldr r0, [r2]           ; box->field_0
ldr r2, [r2, #4]         ; box->field_4
mov r3, #0x88
lsl r3, r3, #1
add r3, r8                 ; xAddr = self + 0x110
str r0, [r3]                 ; *xAddr = box->field_0   (r3 stays live)
mov r1, #0x8a
lsl r1, r1, #1
add r1, r8                     ; yAddr = self + 0x114
str r2, [r1]                     ; *yAddr = box->field_4  (r1 stays live)
... widthAccum=0, lineCount=0, *cursor load ...
str r1, [sp, #0x18]                 ; yAddr FINALLY spilled here
cmp r0, #0
...
str r3, [sp, #0x14]                    ; xAddr FINALLY spilled here, right before the loop
```

`xAddr` (r3) and `yAddr` (r1) both stay live in their registers for
several more statements after being computed, spilling only right
before the register is needed for something else (r1 gets reused for
`limit` in the very next check; r3 survives until the loop's first
call clobbers it). A plain `u32 *xAddr; u32 *yAddr;` (or any
rephrasing of the surrounding statements) always got both spilled to
the stack immediately on assignment instead - a real, different
instruction sequence, not just a register-choice cosmetic.

The fix, combining several techniques already used on this project's
other NAKED-to-matched conversions:

1. **A short-lived register-pinned alias** (`xAddrEarly`/`yAddrEarly`,
   pinned to r3/r1) computes the address and does the initial store,
   matching the ROM's own register choice for that first use.
2. **The real variable is `volatile`** (`u32 * volatile xAddr;`) -
   this is what forces gcc to give it a genuine, fixed stack slot
   instead of trying to keep it in a register (or, worse, proving a
   later self-store is a no-op and eliding it - see the `/b` handler
   below), *and*, as a side effect neither anticipated nor obviously
   documented anywhere: it changes which allocation class the variable
   falls into, which changes *frame-slot ordering* for every other
   `volatile`-qualified local too. Making `box`, `limit`, `widthAccum`,
   `lineCount`, and `posAccum` all `volatile` as well (once one local
   needs it, matching declaration order for the rest is what gets the
   whole stack frame's byte layout - `box@0`, `limit@4`, `widthAccum@8`,
   `lineCount@0xc`, `posAccum@0x10`, `xAddr@0x14`, `yAddr@0x18` -
   lined up with the ROM's own frame exactly.
3. **The assignment from the alias into the real variable happens at
   the exact program point matching the ROM's spill** - `yAddr =
   yAddrEarly;` right after the `*cursor` load (before the `if`), and
   `xAddr = xAddrEarly;` right after the `posAccum >= limit` check
   (right before the loop) - not bundled together, not left at the
   point of computation.

### The `/b` handler's redundant self-store

Once `xAddr`/`yAddr` had genuine stack homes, a second, smaller gap
surfaced: the ROM's `/b` handler reads `*xAddr`, unconditionally
writes the *same* value straight back (a real, if redundant,
instruction the ROM's own compiler emitted), and reloads each address
from the stack fresh for **every** access (not just once) - a plain
`x = *xAddr; y = *yAddr; y += 4; *xAddr = x; *yAddr = y;` let gcc prove
the `*xAddr = x;` store was a no-op and eliminate it, and cached each
address's value in a register instead of reloading it from `xAddr`'s
own memory slot each time. Forcing a genuine reload of `xAddr` and
`yAddr`'s *own* stored pointer value (not just the u32 they point at)
at each of the four accesses, via an `"m"` (memory) inline-asm operand
naming the pointer variable itself, reproduces the ROM's redundant
reload-every-time shape:

```c
asm volatile("ldr %0, %1" : "=r"(xAddr2) : "m"(xAddr));
x = *xAddr2;
```

## Other gaps closed along the way

- **`needleRest`-style register-move ordering elsewhere in the
  prologue**: same "inline-asm-materialized register move" technique
  as gap 1, applied wherever plain C reordering didn't move an
  instruction (e.g. `cursor = token + tokenLen;` needed
  `asm volatile("add %0, %1, %2" ...)` to land in r2 instead of
  gcc's own default choice).
- **Tail-merging defeats duplicated ROM code**: the ROM's `/n` escape
  handler and its `normalChar` overflow-path both do `posAccum +=
  tokenLen;`, but as two *separate*, non-shared inline copies (with
  different register choices, `r2` vs `r1`) - not one shared
  jump target. This compiler's cross-jump/tail-merging pass always
  recognized the byte-identical generated code and merged them into
  one shared block regardless of how separately they were written in
  C. Materializing one of the two copies as its own opaque inline-asm
  block (immune to tail-merging, since raw asm is never merged with
  anything) keeps them separate.
- **A stale variable for the "is there more text" check**: the
  original NON_MATCHING reconstruction (kept in this project's git
  history from before the NAKED conversion) tested `*cursor == 0` at
  the bottom-of-loop "more text?" check; the ROM actually tests
  `*token == 0` there (the just-processed token's own start, not the
  advanced-past-it cursor) - a genuine logic-level difference from the
  inherited reconstruction, not just a register/codegen quirk. Fixing
  this alone jumped the match from ~95.7% to ~99.3%.
- **`mode`'s per-site register choice**: `mode` is a stack-passed 5th
  argument, reloaded fresh at several points; the ROM picks a
  *different* scratch register at different reload sites (`r1`, `r0`,
  `r0`, `r1`) with no consistent pattern derivable from C source order
  alone. A plain register-pinned local (`register s32 modeCheck
  asm("r1") = mode;`) was silently ignored by the compiler for the
  very first reload; only forcing the load itself through inline asm
  with an `"m"` operand (`asm volatile("ldr %0, %1" : "=r"(modeCheck) :
  "m"(mode));`) actually pinned that specific instruction's register.

## What's still open (2 residuals, ~0.14% of the function)

1. The `/b` handler's *fourth* (final) address reload - the ROM uses
   `r1` for `yAddr`'s last reload+store; every attempt to pin it
   (plain register pin, or the same `"m"`-operand inline-asm technique
   that fixed the three other reloads and the `mode` loads) instead
   caused the *other three* reloads in the same handler to reallocate
   their own registers, net-regressing the match. This looks like a
   single register-allocation pass across the whole `/b` block that
   doesn't decompose into four independently-pinnable operations.
2. The final `if (mode != 0) { ...flush...; }` epilogue check (mirror
   of the very first one) - the ROM uses `r1`; this compiler picks
   `r0`. Every attempt tried (plain pin, the `"m"`-operand inline-asm
   trick that worked for the *first* `mode` check) regressed the match
   by 1-2 percentage points elsewhere, for reasons not fully
   understood - possibly an interaction with the shared `end:` merge
   point's epilogue-register selection (see the register-pinning
   memory notes' point about a function's return value shaping its own
   epilogue register choice).

Both residuals are pure register-choice differences with **zero**
semantic impact - the C reconstruction is fully correct, just not yet
byte-identical. A future attempt could try: isolating the `/b` handler
into its own tiny helper function (to give the register allocator a
fresh, smaller scope to work with independently of the rest of
`sub_8000EE4`), or a full permuter-style search over the last two
sites now that everything else is nailed down.

## New technique attempts this session (matching-constraint asm operand)

`sub_800A884`'s doc (`docs/matching/issue-9-10-0x0800a884-graphics.md`)
this session documented a new technique: an inline-asm operand with a
**matching constraint** (`"0"(existing_var)`) that reuses a register
another value is *already*, naturally (unforced), allocated to,
instead of asking the allocator to conjure a brand-new hard-register
binding - avoiding the "ripple" a fresh `register T v asm("rN")` pin
causes elsewhere. Both residuals above were re-attempted with this
technique and neither closed cleanly:

1. **`/b` handler's final reload (residual 1).** In the ROM, `x`
   (loaded from `*xAddr`) happens to live in `r1` for its whole short
   life, dies at the `str r1, [r2]` that writes it back to `*xAddr`,
   and the compiler then reuses the now-free `r1` for the final
   `yAddr` reload. This compiler's *unforced* natural choice for `x`
   is *also* `r1` (confirmed identical to the ROM in the unmodified
   reconstruction) - so the ingredients for the matching-constraint
   technique are present. Combining the `*xAddr2 = x;` store and the
   final `yAddr2` reload into one asm island tied by a `"0"(x)`
   matching constraint (`asm volatile("str %1, [%2]\n\tldr %0, %3" :
   "=r"(yAddr2) : "0"(x), "r"(xAddr2), "m"(yAddr));`) *does* reproduce
   the ROM's exact final reload+store (`ldr r1,.../str r0,[r1]`) - but
   it reliably pushes the *earlier* `yAddr2` reload (the one used for
   `y = *yAddr2`) off its own already-matching `r2` onto a *different*
   register (`r0`), a pure lateral trade (2 instructions fixed, 2
   different instructions newly broken, same total mismatch count).
   Giving the two live ranges of `yAddr2` distinct C variable names
   (`yAddr2First`/`yAddr2Last`, in case name-sharing was itself
   inducing an unwanted coalescing hint) made it *worse* (all 4
   reload/store instructions in the block reshuffled, 8 mismatched
   lines instead of 2). Reverted; the plain, unforced four-statement
   `xAddr2`/`yAddr2` form from before this session remains the closest
   working shape.
2. **Final `mode != 0` epilogue check (residual 2).** `end:` is a
   genuine 4-way control-flow merge point (reached from the
   "first-char-is-NUL" check, the "posAccum >= limit" check, the
   bottom-of-loop "`*token == 0`" check, and the "lineCount >= limit"
   check) - unlike `sub_800A884`'s `kindZero` case, there is **no
   variable that is already, naturally, unforced in `r1` on all four
   incoming edges** to hook a matching constraint onto; nothing
   upstream of `end:` uses `r1` for anything still live by the time
   control reaches it. Re-tried the plain register-pinned `"m"`-operand
   technique anyway (`register s32 modeCheck2 asm("r1"); asm
   volatile("ldr %0, %1" : "=r"(modeCheck2) : "m"(mode)); if
   (modeCheck2 != 0) { ... }`), tightly block-scoped exactly like the
   *first* `mode` check that already works - and this time the failure
   mode wasn't just a register-choice ripple, it was worse: **the
   compiler eliminated the `cmp`/`beq` entirely and made the flush
   calls unconditional**, silently miscompiling the function (verified
   by objdump - the two comparison instructions simply vanish from the
   output, and every subsequent pc-relative literal-pool offset shifts
   to match the now-shorter function). This is a genuine correctness
   hazard, not just a match-percentage regression - **do not retry a
   fresh `register ... asm("r1")` pin at this exact site** without
   first understanding why the optimizer treats it as proving
   `modeCheck2 != 0` unconditionally true. Reverted; residual 2 remains
   open with no safe technique found across two sessions.

Net result this session: the newly-found default-case control-flow bug
(above) is a genuine, safe improvement (99.86% -> 99.89011%); both
previously-known register-choice residuals remain open, and the new
matching-constraint technique - while it *can* reproduce either
residual's target instructions in isolation - does not decompose
cleanly for either one the way it did for `sub_800A884`'s
`kindZero`/`r7` case (which had a stable, single, already-forced
register to lean on across the whole function; both of this function's
residuals sit at points with no comparably stable natural register
binding available).

## Verification

The `#if NON_MATCHING` reconstruction: `rm -rf build && make
NON_MATCHING=1 report` succeeds cleanly (no warnings for
`text_layout.c`). Since this function isn't wired into `report.json` as
its own matched unit (`tools/report_units.py`'s entry for `0x08000EE4`
stays `base_object=None`, parked), the match percentage is obtained
directly: `objdiff-cli diff -1
build/expected/units/raw_08000EE4_08000EE4_target.o -2 <isolated
NON_MATCHING build of sub_8000EE4> sub_8000EE4` reports
`match_percent: 99.89011` (up from 99.86% before this session's
default-case control-flow fix).

The default (NAKED) build: full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`. Unchanged by this
session's `#if NON_MATCHING`-only change.
