# `sub_801E990`: matched

`sub_801E990` (`src/graphics/graphics_loading_1e990.c`, sound-trigger
dispatch/position writer) was a byte-correct NAKED asm transcription
for a long time - see [issue-30-graphics-loading.md](./issue-30-graphics-loading.md)'s
"Fifth pass" for the original parking rationale. This doc originally
recorded a 96.8%-matching C reconstruction kept under `#if NON_MATCHING`
for documentation purposes; it now records how the remaining gap
closed and the function is fully matched as real C (NAKED asm and the
`#if NON_MATCHING` toggle both removed).

## What was closed (this session, previous rounds)

The function's two independent halves (a table-resolution/bitfield-pack
section gated on `sub_80232F4`, and a budget-gated trampoline-fire
section) both compile to fully correct, byte-exact C:

- **The `movs r0,#1`/`subs r0,#0x12` negative-mask idiom** - the exact
  same technique as `sub_8021D04`'s (`graphics_loading_21bfc.c`) and
  `UPDATE_ICON_FRAME_NIBBLE`'s (`src/graphics/settings_menu6.c`): an
  `asm volatile("sub %0, %0, #0x12" : "+r"(one))` materializes the
  mask instead of a plain C `-0x11`/`~...` expression this compiler
  folds differently.
- **The ROM's `ldrsh` register-offset form** - Thumb's `LDRSH` has no
  immediate-offset encoding (only register+register), so the ROM
  loads a zero into a scratch register first (`movs r3,#0`) and uses
  it as the offset; reproduced via
  `asm volatile("mov r3, #0\n\tldrsh %0, [%1, r3]" : "=r"(fnOffset) :
  "r"(entry) : "r3")` with `fnOffset` typed `s32` (not `s16` - typing
  it `s16` makes gcc re-sign-extend the already-sign-extended asm
  result with a redundant `lsl`/`asr` pair on use).
- **A genuine "dead read"** - `*(u32 volatile *)(entry + 4)` loaded
  into r4 but never used, the same documented idiom as `sub_8009FD4`
  (`actor_part9.c`) and `sub_8007DBC`; needs the `volatile` qualifier
  or this compiler dead-store-eliminates it.
- **Two double-dereference bugs caught during development**: an early
  draft wrote `*(u8 **)gUnknown_030012D8` (treating the *value* as a
  second address to dereference again), which compiled without error
  but inserted an extra, wrong `ldr` the ROM doesn't have. Caught by
  comparing against the real target object's disassembly, not by
  trusting the isolated-compile "looks plausible" read - the general
  `docs/workflow.md` caution about isolated compiles not being proof
  applies here too.
- **A structural insight into the ROM's own register reuse**: the
  mask section's `+0x28` write and the later `x`/`y` position writes
  both target the *same* `gUnknown_030012D8`-rooted object, and the
  ROM issues a **second, genuinely redundant** `ldr` to re-fetch that
  same value for the second use rather than keeping the first load's
  result register alive across the intervening `strb` - confirmed via
  direct `arm-none-eabi-objdump` of `raw_0801E990_target.o`, not
  inferred.

## What closed the final gap

Across two earlier sessions, the real-file build (with the actual
`struct actor *gUnknown_030012D8` declaration and real `core.h`/
`actor.h` surrounding types - an isolated minimal harness gave a
*misleading* 100% result that didn't reproduce once integrated) kept
putting the object's *address* in r1 and its *value* in r3 for the
`+0x28` write, then inserting an extra `mov r1, r3` copy before the
add - where the ROM keeps the address in r3 and the value directly in
r1 with no copy. Every earlier variant (dropping the shared local,
swapping pinned registers) reproduced the same gap or regressed
further.

The fix, found this session, was a *type* change, not a register-pin
change: the r3-pinned local was modeled as `u8 *d8addr` holding the
*dereferenced value* of `gUnknown_030012D8` (`d8addr = (u8
*)gUnknown_030012D8;`), with `addr28 = d8addr + 0x28` computed
afterward as a separate step. That's semantically correct but requires
gcc to first materialize the dereferenced value in *some* register (it
chose r1) before copying it into d8addr's forced r3 - hence the extra
`mov`. Re-modeling the r3-pinned local as `struct actor **d8ptr`
holding the *address of the global itself* (`d8ptr =
&gUnknown_030012D8;`), with the `+0x28` write computed as a single
dereference-and-add expression (`addr28 = (u8 *)*d8ptr + 0x28;`),
matches what the ROM actually does: load `&gUnknown_030012D8` into r3
once, dereference-and-add directly into r1 with no intermediate
register at all. The later re-fetch for the x/y write
(`obj2 = (u8 *)*d8ptr`) becomes a real second dereference of `d8ptr`
(not a cached-value copy), which reproduces the ROM's own redundant
second `ldr r1, [r3]` for free - the same "genuine redundant reload"
structural insight from earlier sessions, now falling out naturally
from the type change instead of needing a separate workaround.

This is consistent with the pattern seen closing `sub_800A884`'s
`kindZero` gap the same session (`src/graphics/actor_part78.c`,
docs comment point 7): a register-choice gap caused by *what value a
pinned register is asked to hold* (an address vs. a dereferenced
value) can resist every register-pinning/matching-constraint trick and
still close once the C-level modeling of the value itself changes to
match what the compiler naturally does with that type - no inline asm,
no matching-constraint operand, and no new hard-register binding were
needed here, just changing `u8 *d8addr = gUnknown_030012D8` to
`struct actor **d8ptr = &gUnknown_030012D8` and moving the `+0x28` add
into the same expression as the dereference.

Verified via direct `.text`-section byte comparison (`objcopy -O
binary --only-section=.text` on both the isolated-compile output and
the full-clean-build object) against `raw_0801E990_target.o`: fully
byte-identical, including relocation records (same call/global
targets at the same offsets).

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report` succeeds with
no warnings for this file. Full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`.
`tools/report_units.py`'s entry for `0x0801E990` now points at
`src/graphics/graphics_loading_1e990.o` (matched), no longer `None`
(parked).
