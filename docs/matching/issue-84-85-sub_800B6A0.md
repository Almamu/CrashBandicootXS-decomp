# Issues #84/#85: `sub_800B6A0`/`sub_800B6D0`

Two `parked-function` issues, both against the same pair of sibling
functions in `src/graphics/actor_part16.c` (real bytes previously in
the now-removed `asm/code_3_2_18.s`). Both matched in the same pass,
since they share the exact same gap and fix.

## What they do

`sub_800B6A0`/`sub_800B6D0` copy a 3-vector (`vec`) into
`self+0x54`/`+0x58`/`+0x5c`, negating the X and Z components when
`self+0x28` bit 5 (a mirror-flag bit, the same encoding convention used
throughout this ROM for X/Z axis flips) is set. `sub_800B6D0`
additionally duplicates the (possibly negated) X component into
`self+0x64`.

## The gap, and what actually closed it

`docs/matching.md`'s frozen entry for this pair ("A new unnamed
object: `actor_part15.c`/`actor_part16.c`") documented this compiler
unconditionally spilling the `vec` pointer to a callee-saved register
(`push {r4,lr}`/`pop {r4}`) whenever it's referenced from both branches
of the `if`/`else`, even though nothing in either branch actually
clobbers it - the real ROM is a true `r0`-`r3`-only leaf function with
no stack frame at all. Three earlier fixes (pinning `self` alone to
`r3`; additionally pinning/reassigning `vec` to `r2`; restructuring the
`if`/`else` into an equivalent `goto`-based flow) all produced an
identical 8-byte-larger result and it was accepted as an unavoidable
limitation at the time.

Re-reading the real ROM disassembly directly (`self`/`r3`, `vec`/`r2`,
no `push`/`pop` at all) showed the actual per-branch register roles:
in the *negated* branch, the ROM's load order is X (`v[0]`, into
`r0`), then **Z** (`v[2]`, into `r1`), then **Y last** (`v[1]`, into
`r2`) - `Y`'s load is what finally overwrites `v`'s own register
(`r2`), so it has to be the last of the three loads, not the second as
the natural X/Y/Z source order would produce. The *un-negated* branch
loads in plain X/Y/Z order instead (`r0`/`r1`/`r2`), since there `Y`'s
register (`r1`) isn't the one holding `vec`.

Pinning `self`/`vec` to `r3`/`r2` up front, and then declaring each
branch's `x`/`y`/`z` locals as `register s32 ... asm("r0"|"r1"|"r2")`
**in the ROM's actual load order** (X, Z, Y for the negated branch; X,
Y, Z for the plain branch) reproduces the ROM's leaf-function code
exactly, no stack frame needed. Getting the load order wrong in the
negated branch (declaring `y` before `z`, matching the source's X/Y/Z
field-write order instead of the ROM's actual load order) doesn't just
mismatch - it's a genuine miscompile, since `y`'s load into `r2`
overwrites `v` before `z`'s load reads through it, corrupting the
final Z value. Confirmed correct only after fixing the load order,
verified via a full clean `make compare`.

Also needed the trailing `asm(".align 2, 0");` restored after removing
the `#if NON_MATCHING`/`#endif` guard - dropping it left the compiler's
default `nop` (`0x46c0`) as trailing padding instead of the ROM's
zero-padding, a one-word mismatch a full clean build caught immediately
after an otherwise-perfect instruction match (see
`matching_decomp_alignment_fix` project convention).

## Cross-references

- `docs/status/actor.md` - `actor_part16.c`'s matched-function list
  updated, the stale parked entry for this pair removed.
- `docs/matching.md` - the frozen historical entry ("A new unnamed
  object: `actor_part15.c`/`actor_part16.c`") still describes the three
  earlier failed attempts; this file supersedes only the final
  disposition (matched, not parked).
- `ldscript.txt`/`tools/report_units.py` - `asm/code_3_2_18.s` (now
  empty) removed from both; `actor_part16.o` now links directly against
  `actor_part17.o`.
