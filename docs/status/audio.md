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
- `src/audio/sfx_ambient.c`: `PlaySfx` (`sub_8001854`, one-shot sfx
  play; real C since the near-miss polish pass, its raw
  `asm/code_3_1_10.s` retired - see
  [near-miss-polish.md](../matching/near-miss-polish.md)),
  `sub_800190C` (ambient/looping-sfx-channel
  tick update), `sub_80019A8` (stop-if-playing scan), `sub_80019CC`
  (reset), `sub_80019E8` (force-expire).
- `src/audio/audio_context.c`: `sub_80019F8` (ambient-sfx play request;
  plain C since the early-ROM NAKED retry, see
  [early-rom-naked-retry.md](../matching/early-rom-naked-retry.md)), `sub_8001AB8`,
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
- `src/audio/gax_channel_bind_instrument.c` - `sub_803985C` (binds a new
  instrument entry to a per-channel voice object and resets its
  envelope/state fields) - closed by pinning `self` to `ip` for the
  whole function, the same idiom that closed `sub_80259D4`; see the
  "Update" section of
  [`docs/matching/issue-68-channel-bind-envelope-note.md`](../matching/issue-68-channel-bind-envelope-note.md)

7 of this chunk's 21 functions were matched as real C in the first
passes; the rest were parked as NAKED transcriptions (see the history
below). A later toolchain retry
([`docs/matching/gax-toolchain-retry.md`](../matching/gax-toolchain-retry.md))
matched most of them as real C - see the next section.

### Matched in the GAX toolchain retry (issues #66-#68)

GAX2 turned out to be ordinary current-agbcc output (not old_agbcc, not
ARM); what had parked these functions was mostly heavily register-pinned
drafts. Written plainly against the handler/channel structs now in
`include/audio.h` they match outright. See
[`docs/matching/gax-toolchain-retry.md`](../matching/gax-toolchain-retry.md)
for the per-function notes.

- `src/audio/counter_selector_icons.c` - `sub_80372BC` (counter widget
  digit-icon draw loop)
- `src/audio/gax_zero_fill.c` - `sub_8037F3C` (split out of
  `src/util/math_div64_util.c`, unchanged C)
- `src/audio/gax_channel_pool_alloc.c` - `sub_8038A1C` (builds the SFX
  player out of the work buffer)
- `src/audio/gax_voice_steal.c` - `sub_8038C88` (per-frame mixer tick),
  `sub_8038DC0` (UNUSED voice steal), `sub_8038E74` (SFX voice allocator)
- `src/audio/gax_swi.c` - `sub_80392C4` (HuffUnComp wrapper; the ROM's
  missing r7 save is agbcc's own r7-pin bug, reproduced deliberately)
- `src/audio/gax_sound_handler_channel_init.c` - `sub_8039518`
- `src/audio/gax_sound_handler_channel_play.c` - `sub_80395A4`,
  `sub_8039658` (pattern-row decoder)
- `src/audio/gax_channel_note_scheduler.c` - `sub_80398DC` (instrument
  sequence stepper)
- `src/audio/gax_channel_envelope_tick.c` - `sub_8039AA4`
- `src/audio/gax_note_lookup.c` - `sub_8039F30`
- `src/audio/gax_channel_pos_sweep.c` - `sub_803A03C`
- `src/audio/gax_channel_note_cut_driver.c` - `sub_803A158`
- `src/audio/gax_unknownc_play.c` - `sub_803A278`, `sub_803A2C8`,
  `sub_803A324`, `sub_803A5A8` (the Thumb-to-ARM call is GAX2's own
  inline-asm idiom, `GAX_CALL_ARM`; `sub_803A318`/`sub_803A608` were
  only its return points, not functions)
- `src/util/math_div64_util.c` - `sub_8037648`/`sub_8037A7C`/
  `sub_8037ECC` (`__divdi3`/`__udivdi3`/`__muldi3`, category `util` -
  see [docs/status/util.md](./util.md))

## Parked - NAKED asm transcription (byte-correct, not decompiled C)

- **`sub_8037388`** (`src/audio/counter_selector_icons.c`, the counter
  widget's tile-cache/icon-manager init) - a draft under
  `#if NON_MATCHING` matches through the tile-copy loop; the tail's
  field-offset constants get CSE'd into callee-saved registers across
  the calls, where the ROM rematerializes them after every call.
- **`sub_8038240`** (`src/audio/gax_channel_table_alloc.c`, instantiates
  and links a player's handlers) - draft under `#if NON_MATCHING`; the
  carving loop's register/spill assignment differs and cascades.
- **`sub_8038538`** (`src/audio/gax_playstart.c`, the play-start/init
  entry point) - close draft under `#if NON_MATCHING` (same control
  flow, buffer carving, literal pool); agbcc swaps the r8/r9 homes of
  the format pointer and the max tap rate, which cascades.
- **`sub_8039B44`** (`src/audio/gax_note_trigger.c`, the per-channel
  mixer; `sub_8039E50` is only the ARM call's return point inside it) -
  the call is no longer a blocker (`GAX_CALL_ARM_R`); a complete draft
  is kept under `#if NON_MATCHING`, register allocation differs
  throughout.

`sub_8037E54` (`__udivsi3`, `src/util/math_div64_util.c`) also stays
NAKED - it's lib1funcs.asm's hand-written routine, not compiler output
(see [docs/status/util.md](./util.md)).

History of the earlier parking notes for the functions matched above:
[issue-67-counter-selector-icons.md](../matching/issue-67-counter-selector-icons.md),
[issue-67-68-channel-init-play.md](../matching/issue-67-68-channel-init-play.md),
[issue-66-67-gax-playstart-cluster.md](../matching/issue-66-67-gax-playstart-cluster.md),
[issue-67-gax-voice-steal.md](../matching/issue-67-gax-voice-steal.md),
[issue-68-channel-bind-envelope-note.md](../matching/issue-68-channel-bind-envelope-note.md),
[naked-sub_803a03c-matched.md](../matching/naked-sub_803a03c-matched.md),
[issue-68-0x08039818-audio.md](../matching/issue-68-0x08039818-audio.md),
[issue-68-note-trigger-trampoline.md](../matching/issue-68-note-trigger-trampoline.md).

## Left raw (not attempted, or attempted and set aside)

Everything else in `asm/code_3.s`'s `0x08037110`-`0x0803B0C4` range
(interleaved with some generic compiler-runtime helpers that aren't
actually audio-related - see [docs/audio.md](../audio.md)), including,
from the `0x08037110`-`0x08038538` pass specifically:

- `sub_8037648`/`sub_8037A7C`/`sub_8037E54`/`sub_8037ECC` - GAX2's
  bundled libgcc helpers (`__divdi3`/`__udivdi3`/`__udivsi3`/
  `__muldi3`), matched/parked under category `util` in
  `src/util/math_div64_util.c` (issue #66) rather than this page - see
  [docs/status/util.md](./util.md). `sub_8037F3C` (GAX2's zero-fill
  helper) now lives in `src/audio/gax_zero_fill.c`.
- `sub_8037FC0` - computes the work-RAM size a GAX2 song header needs
  (handler instances plus mix/echo buffers); still raw
  (`asm/code_3_2_20c.s`, issue #66). A first C draft is described in
  [docs/matching/gax-toolchain-retry.md](../matching/gax-toolchain-retry.md).
  `sub_8038240`/
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
