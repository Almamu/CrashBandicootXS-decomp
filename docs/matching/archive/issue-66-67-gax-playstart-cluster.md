# Issues #66/#67: `GaxCreateHandlers`/`GaxResetSoundHardware`/`GAX2_init`/`GAX2_jingle`/`GAX_irq` (audio)

Closes the two remaining raw regions `docs/matching.md`'s `0x08037110`-
`0x08038538` entry and `docs/matching/archive/issue-67-0x08038538-audio.md` left
open: `GAX2_estimate`/`GaxCreateHandlers`/`GaxResetSoundHardware` (issue #66) and
`GAX2_init`/`GAX2_jingle`/`GAX_irq` (issue #67's leftover
play-start/init cluster). `GAX2_estimate` was read but not attempted this
pass - see "Left raw" below. Of the other four:

- **`GaxResetSoundHardware`** (`lib/gax/src/gax_hw_reset.c`) and **`GAX_irq`**
  (`lib/gax/src/gax_playback_ticker.c`) - genuinely matched as real C.
- **`GaxCreateHandlers`** (`lib/gax/src/gax_channel_table_alloc.c`),
  **`GAX2_init`** (`lib/gax/src/gax_playstart.c`), and **`GAX2_jingle`**
  (`lib/gax/src/gax_channel_pool_alloc.c`) - NAKED transcriptions, byte-
  correct but not real decompiled C, tracked as parked.

## Matched: `GaxResetSoundHardware` - hardware sound-register reset

Re-arms then disarms `DMA1CNT_H` (a real hardware settle delay between
the two writes - not padding, see below), resets `DMA1CNT`'s word count/
control, disables `SOUNDCNT_X`, sets `SOUNDCNT_H` for Direct Sound A on
timer0 (`0x0B04`), flushes `FIFO_A` (8 zero halfwords), writes
`SOUNDBIAS_H`, and points `DMA1DAD` at `FIFO_A`. This is the "hardware
sound-register reset" `docs/status/audio.md` already flagged as raw.

Two things needed real iteration to land byte-exact:

1. The delay between the two `DMA1CNT_H` writes is a genuine hardware
   settle delay, already flagged in-source in the raw disassembly this
   replaces (`# this expands to adds r3, r3, #0, but for some reason the
   compiler changes it from 1b 1c to 00 33, which is not correct`).
   Confirmed this is actually an **assembler** instruction-selection
   ambiguity, not a compiler-codegen one: GAS's default encoding of the
   literal text `adds r3, r3, #0` picks the 2-operand immediate form
   (`0x3300`, "ADDS Rd, #imm8" with Rd=Rn=r3) over the 3-operand register
   form (`0x1C1B`, "ADDS Rd, Rn, #imm3") the ROM actually has - both are
   semantically identical (add zero to r3, a 1-cycle timing NOP), so this
   is forced via a small `asm(".byte 0x1b, 0x1c\n\t" "mov r8, r8\n\t" ...)`
   anchor between two plain C register-store statements, reusing the
   `.byte` workaround already present in the raw asm rather than
   inventing a new one.
2. The `FIFO_A`-flush loop (`for (i = 7; i >= 0; i--) *fifo = zero;`)
   needed a separate `zero` local variable initialized *before* the loop
   (mirroring `GAX_resume`'s existing idiom in `gax_dma_control.c`
   exactly) rather than a bare `*fifo = 0;` literal store - with the
   literal written inline, gcc materializes the loop-counter constant
   (`movs r0, #7`) before the store-value constant (`movs r1, #0`); with
   a separate initialized-first `zero` variable, it materializes them in
   source order (`movs r1, #0` then `movs r0, #7`), matching the ROM.
   This was caught by the mandatory full clean `make compare` after an
   isolated-compile pass had looked correct on a `.hex()` eyeball
   comparison but actually differed at 4 bytes - a reminder that even
   isolated-compile verification needs an automated byte-diff, not a
   visual scan of two long hex strings.

## Matched: `GAX_irq` - per-frame DMA1/Timer0 direct-sound-output follow-up

Once a song is loaded (`magic == "GAX2"`) and `state` is non-zero: a
fresh `state == 1` (just-started) primes `SOUNDCNT_X`, advances `state`
to 2, and reloads Timer0 from a per-song sample-rate-divisor field
(`+0x34`); if the song's own data flags a fatal condition (`songPtr+0x38`)
and it hasn't already been reported (`+0x43`), shows GAX2's fatal-error
screen (`GaxFatalError`); finally, when `+0x2c == 1`, re-arms DMA1 for
Direct Sound A output from `+0x18` (the same `DMA1CNT_H` settle-delay
quirk as `GaxResetSoundHardware` above) and clears the `+0x43` flag.

Two gotchas, both about matching which register the ROM keeps the
address of `gGaxPlayerState` in across the whole function (`r4`, never
reloaded) versus the *dereferenced* pointer value (reloaded fresh via
`r4` every time it's needed, never cached across a call):

1. Referencing `gGaxPlayerState->field` directly at each use site
   (rather than caching `struct GaxPlayerState *p = gGaxPlayerState;`
   once at the top and reusing `p` throughout) is what makes gcc's own
   address-of-global CSE put `&gGaxPlayerState` in `r4` for the whole
   function and re-dereference through it at each use - exactly the
   ROM's pattern. A `p` local is still useful *within* one basic block
   that reads several fields without an intervening call (e.g. inside
   the `state == 1` and `+0x2c == 1` bodies), where the ROM does hold the
   dereferenced pointer in one register across several field accesses.
2. The `state == 0` and `state == 1` checks read the same field twice in
   the ROM (`ldr r0, [r3, #0x30]` appears twice, once per comparison)
   instead of once with the result reused - gcc's own CSE collapses this
   to a single load unless forced. A scoped `*(vu32 *)&gGaxPlayerState->state`
   volatile-cast re-read for the second check only (not marking the
   field volatile project-wide, which would perturb every other already-
   matched GAX2 function reading `state`) reproduces the ROM's second
   load without disturbing anything else - the
   `matching_decomp_register_pinning` memory's "scoped-volatile casts"
   technique.

## Parked (NAKED): `GaxCreateHandlers`, `GAX2_init`, `GAX2_jingle`

All three share the same many-register (`r8`/`sb`/`sl`) gcc-2.9
allocation ceiling already established for this exact ROM region across
three prior passes (`DrawPowerDialog`/`DrawLanguageSelect`, and this cluster's own
`GaxChannelPlay`/`GaxChannelDecodeRow` - see `docs/status/audio.md`) - `GaxCreateHandlers`
and `GAX2_init` both use all three of `r8`/`sb`/`sl` as genuine scratch
throughout deeply nested loops (a running priority-maximum accumulator
spanning two nested voice-scan loops in `GAX2_init`, several bank/table
index computations reusing `sl` across loop iterations in `GaxCreateHandlers`);
`GAX2_jingle` uses `r8`/`sb` (arg0 preserved in `sb`, a constant `1`
reused across two calls in `r8`) across a multi-level pointer-chase and
two calls. Given this pattern's established, repeated resistance to every
C-level technique documented in `docs/matching.md` across this project
(register pins, `goto`-based block reordering, scoped-volatile casts),
these were transcribed directly as NAKED asm rather than spending another
multi-hour cycle re-confirming the same ceiling a fourth time.

`GaxCreateHandlers` (core GAX2 channel-table allocator/wiring over
`gGaxPlayerState`, called by both `GAX2_init` and `GAX2_jingle`) is
also notable for its `arg4` (bufSize): the ROM's own `ldr r2, [sp, #0x48]`
read - initially mistaken for the callee reaching backward into its
caller's frame at a hand-tuned fixed offset - is exactly what a normal
Thumb 5th-stack-argument read looks like once accounted for the callee's
own `push`/`sub sp` prologue size (`0x20` pushed + `0x28` locals =
`0x48`): the caller writes its 5th argument to its own `[sp, #0]` before
the `bl`, and the callee reads it back at `[sp, #own_frame_size]` after
its own prologue. Nothing hand-tuned about it - a completely ordinary
5-argument C function, just not one this compiler's register allocator
can reproduce byte-exact for the rest of the body.

Every field offset, branch condition, and loop bound in all three
functions was independently understood first via the same reading that
wrote each file's doc comment - the NAKED bodies are mechanical,
byte-verified transcriptions of the ROM's own instructions (unified
syntax converted to this project's established plain/divided NAKED
syntax - `adds`/`subs`/`movs`/`ands`/`lsls`/`lsrs`/`muls`/`orrs` stripped
of their `s` suffix, `rsbs Rd, Rn, #0` rewritten as `neg Rd, Rn`, since
the non-unified assembler used for inline `asm()` doesn't accept the
suffixed forms - confirmed by a direct test compile that fails with
"instruction not supported in Thumb16 mode" otherwise), not inferred
control-flow guesses. Local labels were renumbered per
`docs/matching/archive/issue-4-sio-settings-sync.md`'s convention. Each was
verified byte-identical in isolation first (assembled standalone and
diffed against the corresponding `baserom.gba` byte range - every
remaining difference traced to an expected, then-unresolved symbol/`bl`
relocation that the real link fixes), then again via the full clean
`make compare` this doc's functions were all landed together with.

## Object shapes

`gGaxPlayerState` (`struct GaxPlayerState *`, `lib/gax/src/gax_internal.h`) still
only names `magic`/`songPtr`/`channels`/`curChannelIdx`/`state` - all
five functions here touch many more fields (`+0x10`/`+0x14`/`+0x18`/
`+0x1c`/`+0x24`/`+0x2c`/`+0x30`/`+0x34`/`+0x40`-`+0x44`/`+0x48`+/`+0x9c`+/
`+0x184`/`+0x188`, and more) that clearly belong to the same object (cross-
referenced by `GAX2_init` writing them and `GaxCreateHandlers`/`GAX2_jingle`/
`GAX_irq` reading them back) but add up to a struct far larger than
what's currently modeled (at least ~0x18C bytes, based on the highest
offset seen). Extending the shared struct correctly would need auditing
every field across all four functions at once - out of scope for this
pass, so kept as raw offsets throughout, consistent with every other
GAX2_SoundHandler function in this cluster (`gax_channel_init.c`,
`gax_sound_handler_unknownc.c`, etc.).

## Left raw

`GAX2_estimate` (issue #66's other function) - read in full ("a large,
genuinely hard-to-follow GAX2 mixer/timing computation over
`gGaxMixRates` and several SoundHandler-shaped structures" per
`docs/status/audio.md`) but not attempted this pass; still genuinely
GAX2 mixer-state internals, not a false-positive generic helper.

## Verification

Full clean `make compare` (`rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`) printed
`La suma coincide` after cutting all five functions out of
`asm/code_3_2_20d.s`/`asm/code_3_2_20e.s` (both files now empty and
removed) and adding `lib/gax/src/gax_channel_table_alloc.o`/
`gax_hw_reset.o`/`gax_playstart.o`/`gax_channel_pool_alloc.o`/
`gax_playback_ticker.o` to `ldscript.txt` in their place. `make
NON_MATCHING=1 report` also verified clean (356 units, 9 categories).

## Later pass: GAX toolchain retry

`GAX2_jingle` now matches as plain C against the structs in `lib/gax/src/gax_internal.h`. `GaxCreateHandlers` and `GAX2_init` stay NAKED with drafts under `#if NON_MATCHING` (register-assignment gaps, not an allocation "ceiling"). See [gax-toolchain-retry.md](./gax-toolchain-retry.md).

## Later pass: GAX NAKED retry 2

`GaxCreateHandlers` is now real C, under current agbcc with the normal flags.
What closed it:
- `/` for the rate division (`__udivsi3` is lib1funcs' routine, so the call
  is a const libcall);
- a separate child-count local;
- `t->childTypes[j]` re-read in the linking loop;
- a block-scoped `next`;
- index-first addressing.

`GAX2_init`'s draft is improved: `maxRate`/`fmt` are now in the ROM's
r8/r9, and it is ~294 halfwords off by alignment-insensitive count (was
~364). It stays NAKED. See [gax-naked-retry-2.md](./gax-naked-retry-2.md).

## Later pass: GAX NAKED retry 5

`GAX2_init` is now real C, under current agbcc with the normal flags.
The closing changes were indexed copies of the constant ARM-code tables,
a separate counter for the dspFn17c copy, a block-local `layout` read, and
three documented no-code nudges. See
[gax-naked-retry-5.md](./gax-naked-retry-5.md).
