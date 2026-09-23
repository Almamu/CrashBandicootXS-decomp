# `sub_801E990` progress: 96.8% instruction match, still NAKED

`sub_801E990` (`src/graphics/graphics_loading_1e990.c`, sound-trigger
dispatch/position writer) is still a byte-correct NAKED asm
transcription for the default build - see
[issue-30-graphics-loading.md](./issue-30-graphics-loading.md)'s
"Fifth pass" for the original parking rationale. This doc records a
much closer C reconstruction (96.8% instruction match, kept in the
file under `#if NON_MATCHING`) so a future attempt doesn't have to
re-derive most of this.

## What's closed

The function's two independent halves (a table-resolution/bitfield-pack
section gated on `sub_80232F4`, and a budget-gated trampoline-fire
section) both compile to fully correct, mostly byte-exact C:

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
- **Two double-dereference bugs caught during development**: this
  file's other globals are simple `void *`/pointer variables holding
  the target value directly (`gUnknown_030012D8`, `gUnknown_030012BC`)
  - an early draft wrote `*(u8 **)gUnknown_030012D8` (treating the
  *value* as a second address to dereference again), which compiled
  without error but inserted an extra, wrong `ldr` the ROM doesn't
  have. Caught by comparing against the real target object's
  disassembly, not by trusting the isolated-compile "looks plausible"
  read - the general `docs/workflow.md` caution about isolated
  compiles not being proof applies here too.
- **A structural insight into the ROM's own register reuse**: the
  mask section's `+0x28` write and the later `x`/`y` position writes
  both target the *same* `gUnknown_030012D8`-rooted object, and the
  ROM issues a **second, genuinely redundant** `ldr` to re-fetch that
  same value for the second use rather than keeping the first load's
  result register alive across the intervening `strb` - confirmed via
  direct `arm-none-eabi-objdump` of `raw_0801E990_target.o`, not
  inferred. Reproduced by keeping the *address-of-global* pinned to
  `r3` across both uses, and re-dereferencing it (`register u8 *obj2
  asm("r1") = d8addr;`) rather than reusing a cached dereferenced
  value.

## What's still open (residual register-choice gap, context-sensitive)

In isolation (a minimal standalone harness with stub declarations),
the reconstruction above matches **100%** against
`raw_0801E990_target.o`. Integrated into the real file (with the
actual `struct actor *gUnknown_030012D8` declaration and the real
`core.h`/`actor.h` surrounding types), the very first
`gUnknown_030012D8` access - loading its address into one register
then its value into another for the `+0x28` write - drops to a
different register allocation than the isolated test: the real build
puts the *address* in r1 and the *value* in r3, then inserts an extra
`mov r1, r3` copy before the `+0x28` add, where the ROM (and the
isolated test) keep the address in r3 and the value directly in r1
with no copy. Every variant tried (dropping the shared `d8addr` local
entirely and re-referencing the global independently at each site,
swapping which local occupies which pinned register) either
reproduced the same gap or actively regressed further (dropping the
shared local costs an extra full re-resolution and nets *fewer*
matching instructions, not more). This is consistent with this
project's other documented cases where a function's real surrounding
context (more live registers, different call argument pressure)
changes gcc 2.9's register-allocation choices in ways an isolated
compile doesn't predict - not a new class of problem, just a fresh
instance of it. 4 of the remaining ~5% diff instructions cascade
directly from this one register swap (the literal-pool PC-relative
offsets shift once the instruction count changes); the true residual
is one extra `mov` instruction.

## Verification

The `#if NON_MATCHING` reconstruction: `rm -rf build && make
NON_MATCHING=1 report` succeeds (including the `arm-none-eabi-as`
assemble-verification step, no warnings); instruction match against
`build/expected/units/raw_0801E990_target.o` via `objdiff-cli diff`
sits at 96.8% for `sub_801E990`.

The default (NAKED) build: full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`. Unchanged from
before this investigation; `tools/report_units.py`'s entry for
`0x0801E990` stays `base_object=None` (still parked, not matched).
