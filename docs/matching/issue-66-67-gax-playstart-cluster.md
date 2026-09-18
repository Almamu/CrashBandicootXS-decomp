# Issues #66/#67: `sub_8038240`/`sub_80384DC`/`sub_8038538`/`sub_8038A1C`/`sub_8038B68` (audio)

Closes the two remaining raw regions `docs/matching.md`'s `0x08037110`-
`0x08038538` entry and `docs/matching/issue-67-0x08038538-audio.md` left
open: `sub_8037FC0`/`sub_8038240`/`sub_80384DC` (issue #66) and
`sub_8038538`/`sub_8038A1C`/`sub_8038B68` (issue #67's leftover
play-start/init cluster). `sub_8037FC0` was read but not attempted this
pass - see "Left raw" below. Of the other four:

- **`sub_80384DC`** (`src/audio/gax_hw_reset.c`) and **`sub_8038B68`**
  (`src/audio/gax_playback_ticker.c`) - genuinely matched as real C.
- **`sub_8038240`** (`src/audio/gax_channel_table_alloc.c`),
  **`sub_8038538`** (`src/audio/gax_playstart.c`), and **`sub_8038A1C`**
  (`src/audio/gax_channel_pool_alloc.c`) - NAKED transcriptions, byte-
  correct but not real decompiled C, tracked as parked.

## Matched: `sub_80384DC` - hardware sound-register reset

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
   (mirroring `sub_8038C50`'s existing idiom in `gax_dma_control.c`
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

## Matched: `sub_8038B68` - per-frame DMA1/Timer0 direct-sound-output follow-up

Once a song is loaded (`magic == "GAX2"`) and `state` is non-zero: a
fresh `state == 1` (just-started) primes `SOUNDCNT_X`, advances `state`
to 2, and reloads Timer0 from a per-song sample-rate-divisor field
(`+0x34`); if the song's own data flags a fatal condition (`songPtr+0x38`)
and it hasn't already been reported (`+0x43`), shows GAX2's fatal-error
screen (`sub_80392E0`); finally, when `+0x2c == 1`, re-arms DMA1 for
Direct Sound A output from `+0x18` (the same `DMA1CNT_H` settle-delay
quirk as `sub_80384DC` above) and clears the `+0x43` flag.

Two gotchas, both about matching which register the ROM keeps the
address of `gUnknown_03001630` in across the whole function (`r4`, never
reloaded) versus the *dereferenced* pointer value (reloaded fresh via
`r4` every time it's needed, never cached across a call):

1. Referencing `gUnknown_03001630->field` directly at each use site
   (rather than caching `struct GaxPlayerState *p = gUnknown_03001630;`
   once at the top and reusing `p` throughout) is what makes gcc's own
   address-of-global CSE put `&gUnknown_03001630` in `r4` for the whole
   function and re-dereference through it at each use - exactly the
   ROM's pattern. A `p` local is still useful *within* one basic block
   that reads several fields without an intervening call (e.g. inside
   the `state == 1` and `+0x2c == 1` bodies), where the ROM does hold the
   dereferenced pointer in one register across several field accesses.
2. The `state == 0` and `state == 1` checks read the same field twice in
   the ROM (`ldr r0, [r3, #0x30]` appears twice, once per comparison)
   instead of once with the result reused - gcc's own CSE collapses this
   to a single load unless forced. A scoped `*(vu32 *)&gUnknown_03001630->state`
   volatile-cast re-read for the second check only (not marking the
   field volatile project-wide, which would perturb every other already-
   matched GAX2 function reading `state`) reproduces the ROM's second
   load without disturbing anything else - the
   `matching_decomp_register_pinning` memory's "scoped-volatile casts"
   technique.

## Parked (NAKED): `sub_8038240`, `sub_8038538`, `sub_8038A1C`

All three share the same many-register (`r8`/`sb`/`sl`) gcc-2.9
allocation ceiling already established for this exact ROM region across
three prior passes (`sub_8006600`/`sub_80372BC`, and this cluster's own
`sub_80395A4`/`sub_8039658` - see `docs/status/audio.md`) - `sub_8038240`
and `sub_8038538` both use all three of `r8`/`sb`/`sl` as genuine scratch
throughout deeply nested loops (a running priority-maximum accumulator
spanning two nested voice-scan loops in `sub_8038538`, several bank/table
index computations reusing `sl` across loop iterations in `sub_8038240`);
`sub_8038A1C` uses `r8`/`sb` (arg0 preserved in `sb`, a constant `1`
reused across two calls in `r8`) across a multi-level pointer-chase and
two calls. Given this pattern's established, repeated resistance to every
C-level technique documented in `docs/matching.md` across this project
(register pins, `goto`-based block reordering, scoped-volatile casts),
these were transcribed directly as NAKED asm rather than spending another
multi-hour cycle re-confirming the same ceiling a fourth time.

`sub_8038240` (core GAX2 channel-table allocator/wiring over
`gUnknown_03001630`, called by both `sub_8038538` and `sub_8038A1C`) is
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
`docs/matching/issue-4-sio-settings-sync.md`'s convention. Each was
verified byte-identical in isolation first (assembled standalone and
diffed against the corresponding `baserom.gba` byte range - every
remaining difference traced to an expected, then-unresolved symbol/`bl`
relocation that the real link fixes), then again via the full clean
`make compare` this doc's functions were all landed together with.

## Object shapes

`gUnknown_03001630` (`struct GaxPlayerState *`, `include/audio.h`) still
only names `magic`/`songPtr`/`channels`/`curChannelIdx`/`state` - all
five functions here touch many more fields (`+0x10`/`+0x14`/`+0x18`/
`+0x1c`/`+0x24`/`+0x2c`/`+0x30`/`+0x34`/`+0x40`-`+0x44`/`+0x48`+/`+0x9c`+/
`+0x184`/`+0x188`, and more) that clearly belong to the same object (cross-
referenced by `sub_8038538` writing them and `sub_8038240`/`sub_8038A1C`/
`sub_8038B68` reading them back) but add up to a struct far larger than
what's currently modeled (at least ~0x18C bytes, based on the highest
offset seen). Extending the shared struct correctly would need auditing
every field across all four functions at once - out of scope for this
pass, so kept as raw offsets throughout, consistent with every other
GAX2_SoundHandler function in this cluster (`gax_channel_init.c`,
`gax_sound_handler_unknownc.c`, etc.).

## Left raw

`sub_8037FC0` (issue #66's other function) - read in full ("a large,
genuinely hard-to-follow GAX2 mixer/timing computation over
`gStaticData_085A6150` and several SoundHandler-shaped structures" per
`docs/status/audio.md`) but not attempted this pass; still genuinely
GAX2 mixer-state internals, not a false-positive generic helper.

## Verification

Full clean `make compare` (`rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`) printed
`La suma coincide` after cutting all five functions out of
`asm/code_3_2_20d.s`/`asm/code_3_2_20e.s` (both files now empty and
removed) and adding `src/audio/gax_channel_table_alloc.o`/
`gax_hw_reset.o`/`gax_playstart.o`/`gax_channel_pool_alloc.o`/
`gax_playback_ticker.o` to `ldscript.txt` in their place. `make
NON_MATCHING=1 report` also verified clean (356 units, 9 categories).
