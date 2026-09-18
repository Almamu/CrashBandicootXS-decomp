# Issue #68: `0x0803985C`-`0x08039FFC` (audio, channel bind/envelope/note)

Continues issue #68's pass past `sub_8039818`-`sub_803A22C` (see
[`issue-68-0x08039818-audio.md`](./issue-68-0x08039818-audio.md)). This
pass covers the `0x0803985C`-`0x08039FFC` cluster: 1 of 6 functions
matched as real C, 4 parked as byte-verified NAKED transcriptions, and
one entangled pair (`sub_8039B44`/`sub_8039E50`) left raw.

## Matched

None of this cluster's functions landed as plain C byte-exact against the
real full-ROM link this pass - see "Parked" below for the closest
attempts and why each still needed a NAKED fallback.

## Parked - NAKED asm transcription

- **`sub_803985C`** (`src/audio/gax_channel_bind_instrument.c`) - binds a
  new instrument entry (`table->0x10[cmd]`) to a per-channel voice object
  and resets its envelope/state fields, clearing the binding back out if
  the entry's own first byte flags it invalid, then records `cmd` into
  the current song's per-slot table. A real C reconstruction reproduced
  every field write and even the "two named zero temps" split already
  established for `sub_803A104` (`gax_channel_init.c`), but the ROM's
  `self` register choreography - reloaded fresh from `ip` into a rotating
  r0/r1/r3 cast exactly when each group of field writes needs it, with
  `r3` itself later mutated in place (`adds r3, #0x23`) - always needed
  one extra callee-saved register (`r5`) that the ROM's version doesn't
  spend. This exact function was already set aside for the same reason in
  a prior pass (see `issue-68-0x08039818-audio.md`'s `sub_803985C`
  entry) - this pass's fresh attempt, informed by the redundant-re-fetch
  technique that closed `sub_8038FD0`'s cluster
  (`issue-67-channel-mute-volume-dma-stop.md`), still didn't close it,
  confirming it as a genuine register-choreography gap rather than an
  easy miss.
- **`sub_80398DC`** (`src/audio/gax_channel_note_scheduler.c`) - the
  per-tick pattern-note/priority-steal scheduler with a 15-way command
  jump table. Not attempted as real C this pass - keeps `r8`/`sb` live as
  genuine scratch (a running "steal" candidate index/slot-array base
  pair) across the whole priority-steal block and the jump table, the
  same many-register gcc-2.9 allocation ceiling already documented
  throughout this ROM region.
- **`sub_8039AA4`** (`src/audio/gax_channel_envelope_tick.c`) - per-tick
  envelope/portamento-pitch update. This one got very close: register-
  pinning `self` to `r4` and the two envelope-clamp temporaries landed
  everything except a handful of `ldrsh`-with-non-immediate-offset reads
  in the portamento tail (the same "materialize the field offset into a
  scratch register first" gotcha already documented for `sub_803943C` in
  `gax_sound_handler_info.c` - Thumb's `ldrsh` has no immediate-offset
  encoding). Each individual read could be forced to the ROM's exact
  register via a tiny fixed-register `asm` block, but doing so for all of
  them together kept perturbing an *earlier*, already-correct clamp
  block's register choice - gcc-2.9's hard-register variable reservations
  turned out not to be scoped as tightly as their enclosing C block, so a
  later read's `asm` clobber list changed unrelated, already-matching
  codegen upstream of it. Parked as a byte-verified NAKED transcription
  rather than chase that ripple further within this pass's budget - a
  reasonable next target for a future pass with more room to iterate
  register-by-register.
- **`sub_8039F30`** (`src/audio/gax_note_lookup.c`) - resolves a
  pattern-note index into an interpolated pitch/volume byte from a sorted
  breakpoint table (called by `sub_8039AA4`). A real C reconstruction
  matched this function's full control flow (every branch and
  computation, confirmed by isolated compile), but the ROM's specific
  register choices - `self` kept in `r5`, `table` kept in `r3` for the
  entire function (never the parameter's own `r1`), and a
  `lsls/lsrs #0x10` zero-extension dance for the note-index parameter -
  didn't come out byte-identical from the C forms tried in the time
  available this pass. Parked as a byte-verified NAKED transcription;
  register-pinning every one of these to force the exact rotation (the
  same technique that worked for `sub_8038FD0`'s cluster) is a reasonable
  next step for a future pass.

All four NAKED transcriptions above were verified byte-exact two ways:
directly against `baserom.gba` via an isolated-compile-then-objcopy byte
comparison (skipping only the a few bytes genuinely dependent on final
link addresses - `bl` call-site relocations and a jump table's absolute
address entries), and then again via this pass's full clean
`make compare` pass (`La suma coincide`).

## Left raw

- **`sub_8039B44`/`sub_8039E50`** - one logical note-trigger routine
  split by a manual return-address trampoline: `sub_8039B44`'s tail
  computes a return address into `lr` and `bx`-jumps into `sub_8039E50`
  (which starts with a `nop`/`mov r8, r8` alignment pad, not a real
  instruction), the same entangled-function idiom already left raw for
  `sub_803A278`/`sub_803A318` in the first `0x08039818` pass. Beyond the
  trampoline itself, `sub_8039B44` builds a large stack-resident struct
  (a `sp+0x28`-based, 0x28-byte argument block) passed to `sub_800014C`
  and calls into `sub_8037ECC` (both callees still raw/unnamed
  elsewhere), and reads several parallel per-song-slot tables
  (`gStaticData_0803A874`/`gStaticData_0803A818`/`gStaticData_0803A884`/
  `gStaticData_0803A8B4`/`gStaticData_0803A8C4`) whose shapes aren't
  modeled. Not attempted this pass given its size (over 250 combined
  lines of disassembly) and the established precedent that this manual-
  trampoline shape doesn't factor cleanly into two independent C
  functions - a reasonable next target would be a dedicated NAKED
  transcription pass, given both halves are otherwise fully understood.

See `docs/status/audio.md` for the updated matched/parked/left-raw
summary and `tools/report_units.py`'s `UNITS` table for the exact current
file boundaries.
