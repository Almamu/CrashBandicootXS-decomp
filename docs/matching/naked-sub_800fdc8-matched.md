# `sub_800FDC8` converted from NAKED transcription to real matched C

`sub_800FDC8` (`src/system/game_loop33.c`, the full 4-octant
Bresenham-line-style line-stepper `sub_8010784`/`sub_80107C4`
(game_loop31.c) are fixed single-octant variants of) had been parked
as a byte-correct NAKED asm transcription - see
[issue-13-fc70-continuation.md](./issue-13-fc70-continuation.md) for
the original parking rationale. It's now genuinely matched as real
decompiled C.

## The original gap

The function treats `(pos, count)` and `(a, b)` as two `(position,
value)` pairs, sorts them by `count`/`b` (swapping both coordinates
together if `count > b`), then walks from the lower-`count` point
toward the higher-`count` one along whichever of `pos`/`count` is the
"major" (always-advancing) axis - the classic 4-case Bresenham octant
split, each case a fixed single-octant variant of the same shape
`sub_8010784`/`sub_80107C4` already establish. Every branch, field
access, and octant case was already individually verified matching in
isolation before this function was originally parked - the sole
blocker was that this compiler's cross-jump pass notices the
X-major-increasing case's own early-return (`adds r0,r1,#0; b <exit>`)
is byte-identical to the shared early-return the other three cases
legitimately share in the ROM too, and folds *all four* into one
shared tail instead of the ROM's two (one solo copy for the
X-major-increasing case, one shared by the other three) - 4 bytes
short.

This is the same gap *class* as `sub_8010914`/`sub_801095C`
(game_loop30.c, see
[naked-sub_8010914-matched.md](./naked-sub_8010914-matched.md)), but
notably harder: that function's two `return cur;` sites were *both*
meant to collapse into a single shared copy in the ROM too, so the fix
there was pure block *placement* (moving an already-correctly-merged
target to the ROM's own physical position). Here the ROM genuinely
keeps **two** separate physical copies where this compiler wants only
one - a real anti-merge problem, not just a placement problem.

## The fix

### Gap 1: keeping the solo return un-merged

A `goto`-based restructuring alone (matching `sub_8010914`'s
technique: place the X-major-increasing case's own return target
physically right after that case's own loop, before case 2's code,
mirroring the ROM's block order) was **not sufficient on its own** -
tried first, and confirmed by inspecting the generated asm: gcc's
cross-jump pass still recognized the `mov r0,<count>; b <exit>`
sequence as identical to the shared copy purely by instruction
*content*, irrespective of where each copy sits in the emitted code,
and merged them into one anyway (this makes sense once you consider
that cross-jumping is a late RTL-level pass operating on the final
compiled instruction sequence, not on source-level structure).

The fix that actually worked: materialize the X-major-increasing
case's own return as a literal `asm volatile` block instead of a
plain `return count;`:

```c
            goto returnNeg1;

returnSolo:
            {
                register s32 retVal asm("r0");
                asm volatile("add %0, %1, #0" : "=r"(retVal) : "r"(count));
                return retVal;
            }
```

placed physically right after that case's own loop (matching the
ROM's block order, same as the `sub_8010914` technique), while the
other three cases keep plain `return count;` statements, which this
compiler's own cross-jump pass still merges into a single shared tail
on its own (correctly, since that's what the ROM does too). An inline
`asm` statement is an opaque node to the cross-jump pass - a
fundamentally different kind of RTL entity from a compiler-generated
`SET`/`return` sequence - so it can never be unified with the
plain-C-generated shared copy even when their final emitted bytes
happen to coincide. This reproduces the ROM's exact "one solo copy,
one copy shared by three" layout.

### Gap 2: the `diff`/`err` register roles

Independent of the merge issue, every one of the four octant cases in
the ROM computes its own `diff`/`err` values into the *same* two
registers every time - `err` always in `r0`, `diff` always in `r6` -
and always computes the loop's `diff` calculation the same way:

```
lsl r0, <subtrahend-source>, #1   @ double the subtrahend into r0 (a scratch,
                                   @ since err hasn't been computed yet)
sub r6, <minuend*2>, r0           @ diff = minuend*2 - subtrahend*2
sub r0, <minuend*2>, <subtrahend> @ err = minuend*2 - subtrahend (overwrites r0)
```

Unconstrained, this compiler instead computes the doubled subtrahend
directly into `diff`'s own pinned register (`r6`) as scratch, *before*
overwriting it with the real `diff` value - a different (also valid)
register choice that doesn't match the ROM. Pinning `diff`/`err` to
`r6`/`r0` (`register s32 diff asm("r6");` / `register s32 err
asm("r0");`) alone doesn't fix the scratch-register choice for the
intermediate `lsl`; the doubling calculation needed to be materialized
as its own small `asm volatile` block matching the ROM's exact
instruction/register pair:

```c
            s32 twoDy = dy * 2;
            register s32 diff asm("r6");
            register s32 err asm("r0");
            s32 n;

            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(dx), "r"(twoDy)
                : "r0"
            );
            err = twoDy - dx;
```

(All declarations precede all statements in each case's block - this
compiler, unlike a more modern C99/C11 one, rejects a declaration
appearing after a statement within the same block, so the
`register ... err asm("r0");` declaration can't simply follow the
`asm volatile` statement the way an initializer-form declaration
could.)

This same `lsl`+`sub` idiom, with the operand roles swapped to match
which of `dx`/`dy` (or their negated/doubled forms) is the minuend vs.
subtrahend in each specific case, is repeated for all four octant
cases.

### Gap 3: keeping `r7` free for `limit`

`limit` is alive across all four octant cases (compared against `pos`
every loop iteration) and the ROM keeps it in `r7` for the whole
function, loaded once at entry (`ldr r7, [sp, #0x14]`). Left as a
completely unconstrained parameter with no pin of its own, this
compiler's unforced allocator initially chose to reload it into `r0`
and copy it into `ip` instead (`ldr r0, [sp, #0x14]; mov ip, r0`) - a
valid choice (this function makes no calls at all, so a
non-callee-saved `ip` has no clobber risk), but a 4-byte-larger one
that doesn't match the ROM.

Once `count` was pinned to `r1` (needed anyway - the ROM's own choice,
matching its dual role as an ordinary loop accumulator and the
eventual return value) and, per Gap 2, `err`/`diff` were pinned to
`r0`/`r6`, this compiler's own allocator had nowhere else to put
`limit` and picked `r7` on its own - exactly matching the ROM. This is
the same "starve the allocator of alternatives and let it find `r7`
naturally" pattern already documented extensively in `docs/matching.md`
(e.g. the `itoa`/`len` case, and the `sub_8006600` "raises the
register pressure enough that gcc's own allocator reaches for r7"
finding) and in `matching_decomp_register_pinning` memory point 10.
**`r7` itself is never pinned explicitly anywhere in this function** -
doing so is a confirmed toolchain bug that silently drops `r7` from
the prologue/epilogue's `push`/`pop` list.

## Verification

Isolated pipeline (`cpp` | `agbcc -O2 -fhex-asm -fprologue-bugfix` |
`arm-none-eabi-as` | `objcopy --only-section=.text` | `cmp` against the
ROM's own raw bytes at `0x0800FDC8`-`0x0800FEB0`, 232 bytes): exact
match. Full clean `rm -rf build && make NON_MATCHING=1 report` - no
warnings or errors for `game_loop33.c`. Full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`.
`tools/report_units.py`'s `0x0800FDC8` entry now points at
`src/system/game_loop33.o` instead of `None`.
