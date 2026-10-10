# Status: audio

`docs/audio.md`'s scope is different from this file: it documents the
**data layout** (how the editable-source instrument/sample/song pipeline
under `sound/` rebuilds `gGaxMusicData`), which is a separate,
already-largely-solved problem from matching the **engine code** that
plays it back, which is what this page tracks.

The engine's code is a library now, `lib/gax/` (decomp.dev category
`gax`; the game's audio manager in `src/audio/` keeps `audio`) - see
[docs/libraries.md](../libraries.md). The per-file history below keeps
its original wording.

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

- `src/audio/audio.c`: `UpdateAudio` (per-tick fade-envelope
  update), `StartSong` (start playing a song).
- `src/audio/audio.c`: `PlaySfx` (`sub_8001854`, one-shot sfx
  play; real C since the near-miss polish pass, its raw
  `asm/code_3_1_10.s` retired - see
  [near-miss-polish.md](../matching/archive/near-miss-polish.md)),
  `TickAmbientSfx` (ambient/looping-sfx-channel
  tick update), `StopSfx` (stop-if-playing scan), `ResetAmbientSfx`
  (reset), `StopAmbientSfx` (force-expire).
- `src/audio/audio.c`: `PlayAmbientSfx` (ambient-sfx play request;
  plain C since the early-ROM NAKED retry, see
  [early-rom-naked-retry.md](../matching/archive/early-rom-naked-retry.md)), `GetCurrentSong`,
  `GetSfxVolume`, `GetMusicVolume`, `FadeOutMusic`, `FadeInMusic`,
  `FadeOutMasterVolume`, `FadeInMasterVolume`, `SetMusicFilter`, `SetMusicVolume`,
  `SetSfxVolume`, `PlaySong`, `ResumeSong`, `PauseSong`,
  `StopSong`, `DestroyAudioContext`, `InitAudioContext` (constructor),
  `DisableMusicVCountIrq`.
- `src/audio/audio.c` (new file - `EnableMusicVCountIrq`/`MusicVCountIrqHandler`,
  0x08001C80): installs the VCount-IRQ handler that forwards into
  `UpdateAudio`'s per-tick fade update above - `audio.c`'s
  header comment already anticipated this pair. Matched - GitHub
  issue #4, see `docs/matching/archive/issue-4-sio-settings-sync.md`.

`src/audio/` (further in, at `0x08037110`-`0x08038538` - see
`docs/matching.md`'s "`0x08037110`-`0x08038538`" entry for the full
write-up):

- `src/frontend/language_select.c` - `LoadTaggedAssetBuffered`, `InitCompanyLogos`,
  `DestroyCompanyLogos`, `DestroyLogoActor` (reached only through
  gLogoActorVtable's slot 1; these four in `company_logos_ctor.cpp`
  since #770), `RunLanguageSelect`, `LanguageSelectInput`
- `src/frontend/language_select_setup.c` - `LoadLanguageSelectBg`, `LanguageSelectBlink`,
  `CommitLanguageSelectFrame`, `DestroyLanguageSelect`, `InitLanguageSelect`, `CloseLanguageSelect`,
  `OpenLanguageSelect`
- `lib/gax/src/gax_find_mix_rate.c` - `GaxFindMixRate`
- `lib/gax/src/gax_new.c` - `GAX2_new`

These six-through-one-function groups read like game/HUD-side code that
merely *calls into* audio (`PlaySfx`) or is a SoundHandler-shaped object
constructor, not confirmed GAX2 mixer internals - the language_select*.c group is the boot
language menu (`RunLanguageSelect`).

`src/audio/` (further in, at `0x08038538`-`0x08039658` - see
`docs/matching.md`'s "`0x08038538`-`0x08039658`" entry (PR #198, first
pass) and
[`docs/matching/archive/issue-67-0x08038538-audio.md`](../matching/archive/issue-67-0x08038538-audio.md)
(second pass) for the full write-up, GitHub issue #67):

- `lib/gax/src/gax_dma_control.c` - `GAX_pause`/`GAX_resume` (Direct
  Sound A output stop/start pair)
- `lib/gax/src/gax_note_param.c` - `GAX_fx_note` (conditional per-voice
  note-period update)
- `lib/gax/src/gax_channel_mute_volume.c` - `GAX_stop_fx`/`GAX_set_music_volume`/
  `GAX_set_fx_volume` (per-channel mute/volume-set family - previously
  documented as a "confirmed many-register loop-allocation ceiling";
  matched by never caching the `gGaxPlayerState->channels[curChannelIdx]`
  chase into a local, reproducing the ROM's own r0-r3-only allocation -
  see [`docs/matching/archive/issue-67-channel-mute-volume-dma-stop.md`](../matching/archive/issue-67-channel-mute-volume-dma-stop.md))
- `lib/gax/src/gax_dma_stop.c` - `GAX_stop`/`GaxStopDma` (Direct Sound A/
  Timer0 stop, the counterpart to `GAX_irq`'s start, plus a generic
  single-DMA-channel "off" helper; same writeup as above)
- `lib/gax/src/gax_swi.c` - `GaxHuffUnComp` (HuffUnComp SWI 0x13 wrapper,
  real C in Shin'en's form: r7/r8 register variables and an inline `swi`
  whose `"m"` inputs give the ROM's dead stack stores)
- `lib/gax/src/gax_fatal_error.c` - `GaxFatalError` (the fatal-error
  display screen)
- `lib/gax/src/gax_sound_handler_info.c` - `GaxInfoResetPosition`/`GaxInfoInit`/
  `GaxInfoRestart`/`GaxInfoUnknown`/`GaxInfoPlay` (the GAX2_SoundHandler
  "Info" type's init_fn/unknown_fn/play_fn, per `docs/audio.md`'s
  per-type function-pointer table)
- `lib/gax/src/gax_sound_handler_channel.c` - `GaxChannelUnknown` (the "Channel"
  type's unknown_fn)
- `lib/gax/src/gax_text_render.c` - `GaxDrawText` (word-wrap text/
  console-tile renderer, called by `GaxFatalError`) - see
  [docs/matching/archive/issue-67-word-wrap-text-renderer.md](../matching/archive/issue-67-word-wrap-text-renderer.md)

These read as genuine GAX2 mixer/SoundHandler internals (not
game/HUD-side callers), the first real dive past `GAX2_new`'s single
constructor.

`src/audio/` (issues #66/#67's leftover `0x08038240`-`0x08038B68`
cluster - see
[`docs/matching/archive/issue-66-67-gax-playstart-cluster.md`](../matching/archive/issue-66-67-gax-playstart-cluster.md)):

- `lib/gax/src/gax_work_size.c` - `GAX2_estimate` (the GAX2 work-RAM size
  estimator; matched in the GAX NAKED retry 2, see
  [docs/matching/archive/gax-naked-retry-2.md](../matching/archive/gax-naked-retry-2.md))
- `lib/gax/src/gax_channel_table_alloc.c` - `GaxCreateHandlers` (instantiates
  and links a player's handlers; matched in the GAX NAKED retry 2)
- `lib/gax/src/gax_hw_reset.c` - `GaxResetSoundHardware` (hardware sound-register
  reset: DMA1/SOUNDCNT_H/SOUNDBIAS)
- `lib/gax/src/gax_playback_ticker.c` - `GAX_irq` (per-frame DMA1/
  Timer0 direct-sound-output follow-up to play-start)

`src/audio/` (further in, at `0x08039818`-`0x0803A944` - see
[`docs/matching/archive/issue-68-0x08039818-audio.md`](../matching/archive/issue-68-0x08039818-audio.md)
for the full write-up, GitHub issue #68):

- `lib/gax/src/gax_channel_note_cut.c` - `GaxChannelSetNote` (per-channel note-
  cut/note-on command dispatch)
- `lib/gax/src/gax_channel_effect_table.c` - `GaxChannelTickVibrato` (per-tick
  vibrato/tremolo-style effect-table lookup)
- `lib/gax/src/gax_channel_init.c` - `GaxFxChannelInit` (per-channel voice
  object constructor)
- `lib/gax/src/gax_sound_handler_mixer.c` - `GaxFxChannelUnknown` (the sound-effect
  voice type's unknown_fn, referenced only from that type in the
  sound-effect data set), `GaxMixerInit` (the mixer type's
  init_fn), `GaxMixerUnknown` (its unknown_fn)
- `lib/gax/src/gax_channel_bind_instrument.c` - `GaxChannelSetInstrument` (binds a new
  instrument entry to a per-channel voice object and resets its
  envelope/state fields) - closed by pinning `self` to `ip` for the
  whole function, the same idiom that closed `SetEntityIdActivated`; see the
  "Update" section of
  [`docs/matching/archive/issue-68-channel-bind-envelope-note.md`](../matching/archive/issue-68-channel-bind-envelope-note.md)

7 of this chunk's 21 functions were matched as real C in the first
passes; the rest were parked as NAKED transcriptions (see the history
below). A later toolchain retry
([`docs/matching/archive/gax-toolchain-retry.md`](../matching/archive/gax-toolchain-retry.md))
matched most of them as real C - see the next section.

### Matched in the GAX toolchain retry (issues #66-#68)

GAX2 turned out to be ordinary current-agbcc output (not old_agbcc, not
ARM); what had parked these functions was mostly heavily register-pinned
drafts. Written plainly against the handler/channel structs now in
`lib/gax/src/gax_internal.h` they match outright. See
[`docs/matching/archive/gax-toolchain-retry.md`](../matching/archive/gax-toolchain-retry.md)
for the per-function notes.

- `src/frontend/language_select.c` - `DrawLanguageSelect` (counter widget
  digit-icon draw loop); `InitLanguageSelectGraphics` (the widget's tile-cache/
  icon-manager init) followed in the late-ROM NAKED retry - the two
  icon-manager steps as `static inline` helpers plus a shared `zero`
  local, see
  [`docs/matching/archive/late-rom-naked-retry.md`](../matching/archive/late-rom-naked-retry.md)
- `lib/gax/src/gax_zero_fill.c` - `GaxZeroFill` (split out of
  `lib/libgcc/libgcc2.c`, unchanged C)
- `lib/gax/src/gax_channel_pool_alloc.c` - `GAX2_jingle` (builds the SFX
  player out of the work buffer)
- `lib/gax/src/gax_voice_steal.c` - `GAX_play` (per-frame mixer tick),
  `GAX_fx` (UNUSED voice steal), `GAX_fx_ex` (SFX voice allocator)
- `lib/gax/src/gax_swi.c` - `GaxHuffUnComp` (HuffUnComp wrapper; the ROM's
  missing r7 save is agbcc's own r7-pin bug, reproduced deliberately)
- `lib/gax/src/gax_sound_handler_channel_init.c` - `GaxChannelInit`
- `lib/gax/src/gax_sound_handler_channel_play.c` - `GaxChannelPlay`,
  `GaxChannelDecodeRow` (pattern-row decoder)
- `lib/gax/src/gax_channel_note_scheduler.c` - `GaxChannelStepInstrumentSeq` (instrument
  sequence stepper)
- `lib/gax/src/gax_channel_envelope_tick.c` - `GaxChannelTick`
- `lib/gax/src/gax_note_lookup.c` - `GaxEnvelopeTick`
- `lib/gax/src/gax_channel_pos_sweep.c` - `GaxChannelTickSweep`
- `lib/gax/src/gax_channel_note_cut_driver.c` - `GaxFxChannelPlay`
- `lib/gax/src/gax_sound_handler_mixer_play.c` - `GaxMixerApplyEcho`, `GaxMixerApplyFilter`,
  `GaxMixerPlay`, `GaxMixFrame` (the Thumb-to-ARM call is GAX2's own
  inline asm, `GAX_CALL_ARM`, original source; `sub_803A318`/`sub_803A608` were
  only its return points, not functions)
- `lib/libgcc/libgcc2.c` - `__divdi3`/`__udivdi3`/
  `__muldi3` (libgcc2, category `util` -
  see [docs/status/util.md](./util.md))
- `lib/gax/src/gax_playstart.c` - `GAX2_init` (the play-start/init
  entry point; was parked NAKED, matched in GAX retry 5 - see
  [docs/matching/archive/gax-naked-retry-5.md](../matching/archive/gax-naked-retry-5.md))
- `lib/gax/src/gax_note_trigger.c` - `GaxChannelMix` (the per-channel mixer;
  `sub_8039E50` is only the ARM call's return point inside it). Was
  parked NAKED through five passes, matched in GAX retry 6 - see
  [docs/matching/archive/gax-naked-retry-6.md](../matching/archive/gax-naked-retry-6.md)

## Parked - NAKED asm transcription (byte-correct, not decompiled C)

No GAX function is parked any more.

`__udivsi3` (`lib/libgcc/libgcc2.c`) also stays
NAKED - it's lib1funcs.asm's hand-written routine, not compiler output
(see [docs/status/util.md](./util.md)).

History of the earlier parking notes for the functions matched above:
[issue-67-counter-selector-icons.md](../matching/archive/issue-67-counter-selector-icons.md),
[issue-67-68-channel-init-play.md](../matching/archive/issue-67-68-channel-init-play.md),
[issue-66-67-gax-playstart-cluster.md](../matching/archive/issue-66-67-gax-playstart-cluster.md),
[issue-67-gax-voice-steal.md](../matching/archive/issue-67-gax-voice-steal.md),
[issue-68-channel-bind-envelope-note.md](../matching/archive/issue-68-channel-bind-envelope-note.md),
[naked-sub_803a03c-matched.md](../matching/archive/naked-sub_803a03c-matched.md),
[issue-68-0x08039818-audio.md](../matching/archive/issue-68-0x08039818-audio.md),
[issue-68-note-trigger-trampoline.md](../matching/archive/issue-68-note-trigger-trampoline.md).

## Left raw (not attempted, or attempted and set aside)

Everything else in `asm/code_3.s`'s `0x08037110`-`0x0803B0C4` range
(interleaved with some generic compiler-runtime helpers that aren't
actually audio-related - see [docs/audio.md](../audio.md)), including,
from the `0x08037110`-`0x08038538` pass specifically:

- `__divdi3`/`__udivdi3`/`__udivsi3`/`__muldi3` - GAX2's
  bundled libgcc helpers (`__divdi3`/`__udivdi3`/`__udivsi3`/
  `__muldi3`), matched/parked under category `util` in
  `lib/libgcc/libgcc2.c` (issue #66) rather than this page - see
  [docs/status/util.md](./util.md). `GaxZeroFill` (GAX2's zero-fill
  helper) now lives in `lib/gax/src/gax_zero_fill.c`.
- `GAX2_estimate` - computes the work-RAM size a GAX2 song header needs
  (handler instances plus mix/echo buffers); was raw
  (`asm/code_3_2_20c.s`, issue #66), now matched real C in
  `lib/gax/src/gax_work_size.c`.
  `GaxCreateHandlers`/
  `GaxResetSoundHardware` (the rest of issue #66) are now matched/parked - see
  [docs/matching/archive/issue-66-67-gax-playstart-cluster.md](../matching/archive/issue-66-67-gax-playstart-cluster.md).

From the `0x08038538`-`0x08039658` pass (issue #67, PR #198 - still
raw after the second pass, see
[`docs/matching/archive/issue-67-0x08038538-audio.md`](../matching/archive/issue-67-0x08038538-audio.md)):

`GAX2_init`/`GAX2_jingle`/`GAX_irq` (the play-start/init entry
point and its DMA1/Timer0 direct-sound-output follow-ups, listed raw
above as of the second pass) are now matched/parked - see
[docs/matching/archive/issue-66-67-gax-playstart-cluster.md](../matching/archive/issue-66-67-gax-playstart-cluster.md)
and the "Matched"/"Parked - NAKED asm transcription" sections above.

`GAX_play`/`GAX_fx`/`GAX_fx_ex` (more mixer-tick/voice-
stealing internals, `GAX_fx_ex` is the voice-stealing allocator,
listed raw above) are now matched/parked - see
[docs/matching/archive/issue-67-gax-voice-steal.md](../matching/archive/issue-67-gax-voice-steal.md)
and the "Parked - NAKED asm transcription" section above.

`GAX_stop_fx`/`GAX_set_music_volume`/`GAX_set_fx_volume` (per-channel mute/volume-set
family) and `GAX_stop`/`GaxStopDma` (Direct Sound A/Timer0 stop pair,
using the same hardware-register NOP-delay compiler quirk already flagged
in-source at `GaxResetSoundHardware` above) - listed raw above - are now matched,
see [`docs/matching/archive/issue-67-channel-mute-volume-dma-stop.md`](../matching/archive/issue-67-channel-mute-volume-dma-stop.md)
and the "Matched" section above. The earlier "confirmed many-register
loop-allocation ceiling" verdict for the mute/volume family turned out to
be an artifact of caching the channel chase into a local rather than a
genuine gcc-2.9 gap - see that writeup for the technique that closed it.

`GaxDrawText` (word-wrap text/console-tile renderer, called by the
matched `GaxFatalError`) is now matched, real C - see
[docs/matching/archive/issue-67-word-wrap-text-renderer.md](../matching/archive/issue-67-word-wrap-text-renderer.md)
and the "Matched" section above.

`DrawLanguageSelect`/`InitLanguageSelectGraphics` (icon-manager draw loop / tile-cache init for
the counter widget, from the `0x08037110`-`0x08038538` pass) were
parked as byte-verified NAKED transcriptions (both since matched - see
"Matched in the GAX toolchain retry") - see
[docs/matching/archive/issue-67-counter-selector-icons.md](../matching/archive/issue-67-counter-selector-icons.md)
and the "Parked - NAKED asm transcription(s)" section below.

`GaxChannelInit`/`GaxChannelPlay`/`GaxChannelDecodeRow` (the "Channel" SoundHandler
type's init_fn/play_fn and the latter's direct callee) - listed raw
above as of the second `0x08038538`-`0x08039658` pass - are now Parked
NAKED transcriptions, see the "Parked" section above and
[docs/matching/archive/issue-67-68-channel-init-play.md](../matching/archive/issue-67-68-channel-init-play.md).

From the `0x08039818`-`0x0803A944` pass specifically (issue #68): three
independent passes together parked all 15 of the chunk's still-raw
functions as byte-verified NAKED transcriptions - `GaxChannelStepInstrumentSeq`,
`GaxChannelSetInstrument`, `GaxChannelTick`, `GaxEnvelopeTick` (see
[docs/matching/archive/issue-68-channel-bind-envelope-note.md](../matching/archive/issue-68-channel-bind-envelope-note.md))
plus `GaxChannelTickSweep`, `GaxFxChannelPlay`, `GaxMixerApplyEcho`, `GaxMixerApplyFilter`/
`sub_803A318` (fused), `GaxMixerPlay`, `GaxMixFrame`/`sub_803A608`
(fused) (see "Parked - NAKED asm transcription(s)" above), plus
`GaxChannelMix`/`sub_8039E50` - one logical note-trigger routine split by
a manual return-address trampoline, `lib/gax/src/gax_note_trigger.c` -
see
[docs/matching/archive/issue-68-note-trigger-trampoline.md](../matching/archive/issue-68-note-trigger-trampoline.md).
Nothing in this chunk is left raw any more; see
[`docs/matching/archive/issue-68-0x08039818-audio.md`](../matching/archive/issue-68-0x08039818-audio.md)'s
"Left raw" section for the historical per-function reasoning (many-
register allocation ceiling, or entangled with a neighbor via a manual
return-address-trampoline idiom). The raw ARM-mode DSP/mixer code block past
`0x0803A628` (docs/audio.md's `gGaxArmDownmix` onward) is no
longer a raw span: it is disassembled, hand-written ARM in
`lib/gax/asm/gax_arm_dsp.s` (docs/libraries.md, "GAX implementation
notes").
