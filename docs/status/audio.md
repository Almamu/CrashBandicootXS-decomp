# Status: audio

`docs/audio.md`'s scope is different from this file: it documents the
**data layout** (how the editable-source instrument/sample/song pipeline
under `sound/` rebuilds `gStaticData_0855BCB4`), which is a separate,
already-largely-solved problem from matching the **engine code** that
plays it back, which is what this page tracks.

The Shin'en GAX2 sound engine itself is still mostly raw assembly,
roughly `asm/code_3.s`'s `0x08037110`-`0x0803B0C4` range (interleaved
with some generic compiler-runtime helpers that aren't actually
audio-related - see [docs/audio.md](../audio.md)). The wrapper layer
everything else in the ROM calls (start/stop music, fade/duck it,
trigger one-shot and ambient sound effects) is matched, operating on
one shared `AudioContext` object - see
[include/audio.h](../../include/audio.h) for the struct and
[docs/matching.md](../matching.md)'s "`0x080016EC`-`0x08001C80`" entry
for the full write-up.

## Matched

- `src/audio/music_player.c`: `sub_80016EC` (per-tick fade-envelope
  update), `sub_80017BC` (start playing a song).
- `src/audio/sfx_ambient.c`: `sub_800190C` (ambient/looping-sfx-channel
  tick update), `sub_80019A8` (stop-if-playing scan), `sub_80019CC`
  (reset), `sub_80019E8` (force-expire).
- `src/audio/audio_context.c`: `sub_8001AB8`,
  `sub_8001ABC`, `sub_8001AC0`, `sub_8001AC4`, `sub_8001AD8`,
  `sub_8001AEC`, `sub_8001B00`, `sub_8001B14`, `sub_8001B30`,
  `sub_8001B50`, `sub_8001B54`, `sub_8001B88`, `sub_8001BAC`,
  `sub_8001BD4`, `sub_8001C04`, `sub_8001C2C` (constructor),
  `sub_8001C64`.
- `src/audio/music_irq.c` (new file - `sub_8001C80`/`sub_8001CA4`,
  0x08001C80): installs the VCount-IRQ handler that forwards into
  `sub_80016EC`'s per-tick fade update above - `music_player.c`'s
  header comment already anticipated this pair. Matched - GitHub
  issue #4, see `docs/matching/issue-4-sio-settings-sync.md`.

`src/audio/` (further in, at `0x08037110`-`0x08038538` - see
`docs/matching.md`'s "`0x08037110`-`0x08038538`" entry for the full
write-up):

- `src/audio/counter_selector.c` - `sub_8037110`, `nullsub_7`,
  `sub_8037154`, `sub_803716C` (UNUSED - no caller anywhere in the
  ROM), `sub_80371B4`, `sub_8037224`
- `src/audio/counter_selector_setup.c` - `sub_80374D0`, `sub_8037534`,
  `sub_8037548`, `sub_8037578`, `sub_80375A0`, `sub_80375EC`,
  `sub_8037620`
- `src/audio/song_slot_lookup.c` - `sub_8037FA0`
- `src/audio/sound_object_init.c` - `sub_80381FC`

These six-through-one-function groups read like game/HUD-side code that
merely *calls into* audio (`PlaySfx`) or is a SoundHandler-shaped object
constructor, not confirmed GAX2 mixer internals - see the matching.md
entry for what's still just an educated guess (a jukebox/sound-test
track selector) versus confirmed.

`src/audio/` (further in, at `0x08038538`-`0x08039658` - see
`docs/matching.md`'s "`0x08038538`-`0x08039658`" entry (PR #198, first
pass) and
[`docs/matching/issue-67-0x08038538-audio.md`](../matching/issue-67-0x08038538-audio.md)
(second pass) for the full write-up, GitHub issue #67):

- `src/audio/gax_dma_control.c` - `sub_8038C28`/`sub_8038C50` (Direct
  Sound A output stop/start pair)
- `src/audio/gax_note_param.c` - `sub_8038F94` (conditional per-voice
  note-period update)
- `src/audio/gax_channel_mute_volume.c` - `sub_8038FD0`/`sub_8039064`/
  `sub_80390F8` (per-channel mute/volume-set family - previously
  documented as a "confirmed many-register loop-allocation ceiling";
  matched by never caching the `gUnknown_03001630->channels[curChannelIdx]`
  chase into a local, reproducing the ROM's own r0-r3-only allocation -
  see [`docs/matching/issue-67-channel-mute-volume-dma-stop.md`](../matching/issue-67-channel-mute-volume-dma-stop.md))
- `src/audio/gax_dma_stop.c` - `sub_8039198`/`sub_80391E8` (Direct Sound A/
  Timer0 stop, the counterpart to `sub_8038B68`'s start, plus a generic
  single-DMA-channel "off" helper; same writeup as above)
- `src/audio/gax_swi.c` - `sub_80392C4` (HuffUnComp SWI 0x13 wrapper,
  transcribed as NAKED asm)
- `src/audio/gax_fatal_error.c` - `sub_80392E0` (the fatal-error
  display screen)
- `src/audio/gax_sound_handler_info.c` - `sub_80393D0`/`sub_80393FC`/
  `sub_803941C`/`nullsub_39`/`sub_803943C` (the GAX2_SoundHandler
  "Info" type's init_fn/unknown_fn/play_fn, per `docs/audio.md`'s
  per-type function-pointer table)
- `src/audio/gax_sound_handler_channel.c` - `nullsub_40` (the "Channel"
  type's unknown_fn)
- `src/audio/gax_text_render.c` - `sub_8039214` (word-wrap text/
  console-tile renderer, called by `sub_80392E0`) - see
  [docs/matching/issue-67-word-wrap-text-renderer.md](../matching/issue-67-word-wrap-text-renderer.md)

These read as genuine GAX2 mixer/SoundHandler internals (not
game/HUD-side callers), the first real dive past `sub_80381FC`'s single
constructor.

`src/audio/` (issues #66/#67's leftover `0x08038240`-`0x08038B68`
cluster - see
[`docs/matching/issue-66-67-gax-playstart-cluster.md`](../matching/issue-66-67-gax-playstart-cluster.md)):

- `src/audio/gax_hw_reset.c` - `sub_80384DC` (hardware sound-register
  reset: DMA1/SOUNDCNT_H/SOUNDBIAS)
- `src/audio/gax_playback_ticker.c` - `sub_8038B68` (per-frame DMA1/
  Timer0 direct-sound-output follow-up to play-start)

`src/audio/` (further in, at `0x08039818`-`0x0803A944` - see
[`docs/matching/issue-68-0x08039818-audio.md`](../matching/issue-68-0x08039818-audio.md)
for the full write-up, GitHub issue #68):

- `src/audio/gax_channel_note_cut.c` - `sub_8039818` (per-channel note-
  cut/note-on command dispatch)
- `src/audio/gax_channel_effect_table.c` - `sub_8039FFC` (per-tick
  vibrato/tremolo-style effect-table lookup)
- `src/audio/gax_channel_init.c` - `sub_803A104` (per-channel voice
  object constructor)
- `src/audio/gax_sound_handler_unknownc.c` - `nullsub_41` (UNUSED - no
  caller anywhere in the ROM), `sub_803A22C` (the "UnknownC" type's
  init_fn), `nullsub_42` (its unknown_fn)

6 of this chunk's 21 functions are matched as real C; two further
passes (see [`docs/matching/issue-68-0x08039818-audio.md`](../matching/issue-68-0x08039818-audio.md)'s
updated write-up and
[`docs/matching/issue-68-note-trigger-trampoline.md`](../matching/issue-68-note-trigger-trampoline.md))
parked the remaining 15 as byte-verified NAKED transcriptions - see
"Parked - NAKED asm transcription(s)" below. Most hit the same
many-register (`r8`/`sb`/`sl`) gcc-2.9 allocation ceiling already
documented for `sub_8038538`'s cluster; the rest (`sub_803A278` family,
`sub_8039B44`/`sub_8039E50`) are entangled with a neighboring function
via a manual return-address-trampoline idiom. None left raw.

## Parked

- `PlaySfx` (`sub_8001854`, real bytes in `asm/code_3_1_10.s`,
  reconstruction in `src/audio/sfx_ambient.c`): a one-instruction
  prologue register-save-scheduling difference.

See docs/matching.md for `PlaySfx`'s remaining gap.

## Parked - NAKED asm transcription (byte-correct, not decompiled C)

- **`sub_80372BC`/`sub_8037388`** (`src/audio/counter_selector_icons.c`,
  the counter widget's per-frame icon draw loop and its one-time
  tile-cache init helper) - fully understood, but hit the same
  many-register gcc-2.9 allocation ceiling already documented for
  `sub_8006600` (`src/graphics/oam_count.c`) and `sub_80062A8`
  (`src/graphics/settings_menu14.c`, whose tail is this same
  `sub_803AD7C`/`sub_8006C58`/constant-reuse idiom `sub_8037388`'s tail
  uses). Byte-verified NAKED transcriptions - see
  [docs/matching/issue-67-counter-selector-icons.md](../matching/issue-67-counter-selector-icons.md).
- **`sub_80019F8`** (`src/audio/audio_context.c`, ambient-sfx-channel
  play-request driver) - a full C reconstruction closed the `u8`
  stack-parameter byte-load gap but left a base-volume field-address
  CSE difference the compiler always makes. Converted to a
  byte-verified NAKED asm transcription instead - byte-exact but not
  real decompiled C, so tracked here as parked, not matched. See
  [docs/matching/issue-3-overlay-ui-audio-wrapper.md](../matching/issue-3-overlay-ui-audio-wrapper.md)
  for the original gap analysis this closed.
- **`sub_8039518`** (`src/audio/gax_sound_handler_channel_init.c`, the
  "Channel" SoundHandler type's init_fn) - a real C reconstruction
  landed the division call's previously-unreproducible `ldr rX,=0`/
  `ldr rX,=1` literal-pool loads (passing them as one real `(s64)1 <<
  32` constant), but two further gaps (an unavoidable extra
  defensive-copy instruction from pinning the division's second
  argument to `r2`, and the ROM's single shared 4-word literal pool
  splitting into two once any of that call site becomes hand-placed
  asm) resisted every C-level fix. Byte-verified NAKED transcription.
- **`sub_80395A4`/`sub_8039658`** (`src/audio/gax_sound_handler_channel_play.c`,
  the "Channel" SoundHandler type's play_fn and its direct callee) -
  `sub_80395A4`'s entire body matched in isolation except its
  3-instruction parameter-homing prologue order, which this compiler
  never reproduces (with or without register pins, and `chanArg` can't
  be pinned to `r7` to force the issue without hitting this toolchain's
  confirmed r7-pin bug); `sub_8039658` hits the same many-register
  (`r8`/`sb`) allocation ceiling as `sub_8038538`'s cluster below. Both
  byte-verified NAKED transcriptions - see
  [docs/matching/issue-67-68-channel-init-play.md](../matching/issue-67-68-channel-init-play.md)
  for both functions' full write-up.
- **`sub_8038240`** (`src/audio/gax_channel_table_alloc.c`, core GAX2
  channel-table allocator/wiring over `gUnknown_03001630`) - fully
  understood, but its prologue keeps all three of `r8`/`sb`/`sl` live as
  genuine scratch across several nested loops, the same many-register
  gcc-2.9 allocation ceiling as `sub_8038538` below. Byte-verified NAKED
  transcription.
- **`sub_8038538`** (`src/audio/gax_playstart.c`, the play-start/init
  entry point, docs/audio.md) - fully understood, but its prologue keeps
  `r8`/`sb`/`sl` live across the whole function (a running priority-
  maximum accumulator in `r8` spanning two nested voice-scan loops, plus
  `sb`/`sl` holding intermediate bank/table pointers across several
  calls) - the same many-register ceiling already documented for
  `sub_8006600`/`sub_80372BC`. Byte-verified NAKED transcription.
- **`sub_8038A1C`** (`src/audio/gax_channel_pool_alloc.c`, per-channel
  voice-pool allocator) - fully understood, but needs `r8`/`sb` as
  genuine scratch across a multi-level pointer-chase and two calls, the
  same allocation ceiling as its neighbors above. Byte-verified NAKED
  transcription.

  See [docs/matching/issue-66-67-gax-playstart-cluster.md](../matching/issue-66-67-gax-playstart-cluster.md)
  for all three functions' full write-up, plus this pass's two real
  matches (`sub_80384DC`/`sub_8038B68`, listed under "Matched" above).
- **`sub_8038C88`** (`src/audio/gax_voice_steal.c`, per-frame mixer
  tick) - fully understood (zero-fills a per-song scratch buffer,
  mirrors a clamped byte into the current voice, and - when
  `curChannelIdx == 1` and a song flag is set - resets it to 0 and
  re-links every channel row's voice pointer), not attempted as real C
  this pass - a deeply nested `p->channels[curChannelIdx]` chase
  repeated 5 times, deprioritized versus `sub_8038DC0`'s real-C attempt
  below. Byte-verified NAKED transcription.
- **`sub_8038DC0`** (`src/audio/gax_voice_steal.c`, UNUSED - no caller
  anywhere in the ROM) - a standalone near-twin of `sub_8038E74`'s
  unconditional lowest-priority scan. Extensively attempted as real C
  (several reconstructions came very close, including matching the
  ROM's own apparent uninitialized-`bestIdx`-when-empty behavior), but
  no version landed the ROM's exact `r4`/`r5`/`r6`/`r7` register
  combination at once - including an explicit `register ... asm("r7")`
  pin on the parameter, which the allocator overrode with an unrelated
  value instead of honoring (confirmed by direct byte comparison
  against the ROM, not assumed). Byte-verified NAKED transcription.
- **`sub_8038E74`** (`src/audio/gax_voice_steal.c`, the voice-stealing
  mixer allocator, docs/audio.md - one of `PlaySfx`'s two direct
  callees) - fully understood, but keeps `self` in `r8` and a saved
  `&gUnknown_03001630` copy in `ip` live across the whole function
  including two scan loops, the same many-register ceiling as
  `sub_8038240`/`sub_8038538`/`sub_8038A1C` above. Byte-verified NAKED
  transcription.

  See [docs/matching/issue-67-gax-voice-steal.md](../matching/issue-67-gax-voice-steal.md)
  for all three functions' full write-up.
- **`sub_803985C`** (`src/audio/gax_channel_bind_instrument.c`, binds a
  new instrument entry to a per-channel voice object and resets its
  envelope/state fields) - a real C reconstruction reproduced every field
  write, but the ROM's `self` register choreography - reloaded fresh from
  `ip` into a rotating r0/r1/r3 cast, with `r3` itself later mutated in
  place - always needed one extra callee-saved register the ROM doesn't
  spend; already set aside for the same reason in a prior pass. Byte-
  verified NAKED transcription.
- **`sub_80398DC`** (`src/audio/gax_channel_note_scheduler.c`, per-tick
  pattern-note/priority-steal scheduler with a 15-way command jump table)
  - keeps `r8`/`sb` live as genuine scratch across the whole priority-
  steal block and the jump table, the same many-register ceiling
  documented throughout this region. Byte-verified NAKED transcription.
- **`sub_8039AA4`** (`src/audio/gax_channel_envelope_tick.c`, per-tick
  envelope/portamento-pitch update) - a real C reconstruction (register-
  pinning `self` and the clamp temporaries) landed everything except a
  handful of `ldrsh`-with-non-immediate-offset reads in the portamento
  tail (same gotcha as `sub_803943C`), whose individual register-pins
  kept perturbing an earlier, already-correct block's codegen. Byte-
  verified NAKED transcription.
- **`sub_8039F30`** (`src/audio/gax_note_lookup.c`, resolves a pattern-
  note index into an interpolated pitch/volume byte from a sorted
  breakpoint table) - a real C reconstruction matched this function's
  full control flow, but the ROM's specific register choices (`self` in
  `r5`, `table` kept in `r3` throughout, a zero-extension dance for the
  note-index parameter) didn't come out byte-identical from the C forms
  tried. Byte-verified NAKED transcription.

  See [docs/matching/issue-68-channel-bind-envelope-note.md](../matching/issue-68-channel-bind-envelope-note.md)
  for all four functions' full write-up (none of this particular
  `0x0803985C`-`0x08039FFC` cluster landed as real C this pass - the
  `sub_8038FD0` cluster right before it did, in a separate pass, see
  [docs/matching/issue-67-channel-mute-volume-dma-stop.md](../matching/issue-67-channel-mute-volume-dma-stop.md)).
- **`sub_803A03C`** (`src/audio/gax_channel_pos_sweep.c`, per-tick
  ping-pong position sweep) - an 81.3%-matching C reconstruction (the
  `r8` pin and the `self+0x13` OR-with-0xff idiom both closed; the
  residual is register-choice/instruction-selection diffs in the
  repeated columnar-table addressing) is kept in-tree under
  `#if NON_MATCHING` - see
  [docs/matching/naked-sub_803a03c-matched.md](../matching/naked-sub_803a03c-matched.md)
  for the full derivation and what's still open.
  Byte-verified NAKED transcription (confirmed via direct binary
  comparison against the ROM's own assembled bytes, not just an
  instruction listing).
- **`sub_803A158`** (`src/audio/gax_channel_note_cut_driver.c`,
  per-tick note-cut/retrigger driver) - a wider near-twin of the
  matched-but-NAKED `sub_80395A4`'s calling pattern: needs `r8`
  (caching a field address across several branches) and `sb`/`sl`
  (preserving two incoming parameters across five separate `bl` calls)
  all three simultaneously live for most of the function. Byte-verified
  NAKED transcription.
- **`sub_803A278`/`sub_803A2C8`/`sub_803A324`/`sub_803A5A8`**
  (`src/audio/gax_unknownc_play.c`, the "UnknownC" type's `play_fn`
  (`sub_803A324`) and three of its callees) - all four use a manual
  return-address-trampoline idiom (`mov r2, pc; adds r2, #5;
  mov lr, r2; bx r1`) to call an interworked function pointer from
  Thumb on ARMv4T, which has no `blx reg` - not expressible in portable
  C at all. `sub_803A2C8`'s trampoline return address lands inside what
  the ROM's own disassembly labels as a separate function,
  `sub_803A318` (itself nothing but `sub_803A2C8`'s own epilogue, never
  called from anywhere in the ROM); `sub_803A5A8`/`sub_803A608` are the
  same fusion. Both fused pairs are transcribed as one physical NAKED
  body each (`sub_803A318`/`sub_803A608` still get their own
  `.thumb_func`/label pair purely so the address carries its ROM name,
  not as separate callable C functions). `sub_803A324` additionally
  hits the many-register gcc-2.9 allocation ceiling (confirmed via
  isolated compile of an earlier real-C attempt). This same
  translation unit also carries the raw ARM-mode DSP/mixer code block
  right after it (`gStaticData_0803A630` onward, per docs/audio.md) as
  an untouched trailing byte transcription, folded in purely because
  its own start address has no label in `expected/code_3.s` for
  `tools/report_units.py`'s slicing to target - it is not disassembled
  as real code at all yet. All four functions byte-verified (confirmed
  via direct binary comparison against the ROM's own assembled bytes
  across the full 0x0803A278-0x0803A944 span).

  See [docs/matching/issue-68-0x08039818-audio.md](../matching/issue-68-0x08039818-audio.md)
  for all eight functions' full write-up.
- **`sub_8039B44`/`sub_8039E50`** (`src/audio/gax_note_trigger.c`, the
  per-channel note-trigger routine - issue #68's last raw pair) - the
  ROM's own compiler split this single logical function into two
  disassembly labels, glued together by the same manual
  return-address-trampoline idiom as the `sub_803A278` family above:
  `sub_8039B44`'s tail computes a return address into `lr` and
  `bx`-jumps into `sub_8039E50`, whose first instruction is a `nop`
  (`mov r8, r8`) alignment pad, the same trampoline-landing tell
  already seen at `sub_803A2C8`/`sub_803A318`. Unlike the rest of this
  cluster's NAKED entries, this one is a structural gap, not a
  register-allocation one: the trampoline sits in the middle of the
  function, so no real-C reconstruction of "the part before"/"the part
  after" is possible without breaking the hand-computed-PC-offset
  relationship between the two halves. Byte-verified (confirmed both
  via direct binary comparison of the isolated assembled object against
  the ROM's own reassembled original, and via a full clean `make
  compare`, "La suma coincide") - see
  [docs/matching/issue-68-note-trigger-trampoline.md](../matching/issue-68-note-trigger-trampoline.md).

## Left raw (not attempted, or attempted and set aside)

Everything else in `asm/code_3.s`'s `0x08037110`-`0x0803B0C4` range
(interleaved with some generic compiler-runtime helpers that aren't
actually audio-related - see [docs/audio.md](../audio.md)), including,
from the `0x08037110`-`0x08038538` pass specifically:

- `sub_8037648`/`sub_8037A7C`/`sub_8037E54`/`sub_8037ECC`/`sub_8037F3C` -
  confirmed *not* GAX2 code at all (per `docs/audio.md`'s own
  `sub_8037648`/`sub_8037A7C` entries) - a generic 64-bit software
  division/multiply helper family, now matched/parked under category
  `util` in `src/util/math_div64_util.c` (issue #66) rather than this
  page - see [docs/status/util.md](./util.md) and
  [docs/matching/issue-66-67-math-div64-util.md](../matching/issue-66-67-math-div64-util.md).
- `sub_8037FC0` - genuine GAX2 mixer/timing internals over
  `gStaticData_085A6150`; not attempted, issue #66. `sub_8038240`/
  `sub_80384DC` (the rest of issue #66) are now matched/parked - see
  [docs/matching/issue-66-67-gax-playstart-cluster.md](../matching/issue-66-67-gax-playstart-cluster.md).

From the `0x08038538`-`0x08039658` pass (issue #67, PR #198 - still
raw after the second pass, see
[`docs/matching/issue-67-0x08038538-audio.md`](../matching/issue-67-0x08038538-audio.md)):

`sub_8038538`/`sub_8038A1C`/`sub_8038B68` (the play-start/init entry
point and its DMA1/Timer0 direct-sound-output follow-ups, listed raw
above as of the second pass) are now matched/parked - see
[docs/matching/issue-66-67-gax-playstart-cluster.md](../matching/issue-66-67-gax-playstart-cluster.md)
and the "Matched"/"Parked - NAKED asm transcription" sections above.

`sub_8038C88`/`sub_8038DC0`/`sub_8038E74` (more mixer-tick/voice-
stealing internals, `sub_8038E74` is the voice-stealing allocator,
listed raw above) are now matched/parked - see
[docs/matching/issue-67-gax-voice-steal.md](../matching/issue-67-gax-voice-steal.md)
and the "Parked - NAKED asm transcription" section above.

`sub_8038FD0`/`sub_8039064`/`sub_80390F8` (per-channel mute/volume-set
family) and `sub_8039198`/`sub_80391E8` (Direct Sound A/Timer0 stop pair,
using the same hardware-register NOP-delay compiler quirk already flagged
in-source at `sub_80384DC` above) - listed raw above - are now matched,
see [`docs/matching/issue-67-channel-mute-volume-dma-stop.md`](../matching/issue-67-channel-mute-volume-dma-stop.md)
and the "Matched" section above. The earlier "confirmed many-register
loop-allocation ceiling" verdict for the mute/volume family turned out to
be an artifact of caching the channel chase into a local rather than a
genuine gcc-2.9 gap - see that writeup for the technique that closed it.

`sub_8039214` (word-wrap text/console-tile renderer, called by the
matched `sub_80392E0`) is now matched, real C - see
[docs/matching/issue-67-word-wrap-text-renderer.md](../matching/issue-67-word-wrap-text-renderer.md)
and the "Matched" section above.

`sub_80372BC`/`sub_8037388` (icon-manager draw loop / tile-cache init for
the counter widget, from the `0x08037110`-`0x08038538` pass) are now
parked as byte-verified NAKED transcriptions - see
[docs/matching/issue-67-counter-selector-icons.md](../matching/issue-67-counter-selector-icons.md)
and the "Parked - NAKED asm transcription(s)" section below.

`sub_8039518`/`sub_80395A4`/`sub_8039658` (the "Channel" SoundHandler
type's init_fn/play_fn and the latter's direct callee) - listed raw
above as of the second `0x08038538`-`0x08039658` pass - are now Parked
NAKED transcriptions, see the "Parked" section above and
[docs/matching/issue-67-68-channel-init-play.md](../matching/issue-67-68-channel-init-play.md).

From the `0x08039818`-`0x0803A944` pass specifically (issue #68): three
independent passes together parked all 15 of the chunk's still-raw
functions as byte-verified NAKED transcriptions - `sub_80398DC`,
`sub_803985C`, `sub_8039AA4`, `sub_8039F30` (see
[docs/matching/issue-68-channel-bind-envelope-note.md](../matching/issue-68-channel-bind-envelope-note.md))
plus `sub_803A03C`, `sub_803A158`, `sub_803A278`, `sub_803A2C8`/
`sub_803A318` (fused), `sub_803A324`, `sub_803A5A8`/`sub_803A608`
(fused) (see "Parked - NAKED asm transcription(s)" above), plus
`sub_8039B44`/`sub_8039E50` - one logical note-trigger routine split by
a manual return-address trampoline, `src/audio/gax_note_trigger.c` -
see
[docs/matching/issue-68-note-trigger-trampoline.md](../matching/issue-68-note-trigger-trampoline.md).
Nothing in this chunk is left raw any more; see
[`docs/matching/issue-68-0x08039818-audio.md`](../matching/issue-68-0x08039818-audio.md)'s
"Left raw" section for the historical per-function reasoning (many-
register allocation ceiling, or entangled with a neighbor via a manual
return-address-trampoline idiom). The raw ARM-mode DSP/mixer code block past
`0x0803A628` (docs/audio.md's `gStaticData_0803A630` onward) is no
longer a separate raw span - it is now an untouched trailing byte
transcription inside `gax_unknownc_play.c` (see above), still not
disassembled as real code.
