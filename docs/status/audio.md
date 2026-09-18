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

These read as genuine GAX2 mixer/SoundHandler internals (not
game/HUD-side callers), the first real dive past `sub_80381FC`'s single
constructor.

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

Only 6 of this chunk's 21 functions matched - the rest hit the same
many-register (`r8`/`sb`/`sl`) gcc-2.9 allocation ceiling already
documented for `sub_8038538`'s cluster, or are entangled with a
neighboring function via a manual return-address-trampoline idiom (see
the write-up's "Left raw" section for the full per-function breakdown).

## Parked

- `PlaySfx` (`sub_8001854`, real bytes in `asm/code_3_1_10.s`,
  reconstruction in `src/audio/sfx_ambient.c`): a one-instruction
  prologue register-save-scheduling difference.

See docs/matching.md for `PlaySfx`'s remaining gap.

## Parked - NAKED asm transcription (byte-correct, not decompiled C)

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

## Left raw (not attempted, or attempted and set aside)

Everything else in `asm/code_3.s`'s `0x08037110`-`0x0803B0C4` range
(interleaved with some generic compiler-runtime helpers that aren't
actually audio-related - see [docs/audio.md](../audio.md)), including,
from the `0x08037110`-`0x08038538` pass specifically:

- `sub_80372BC`/`sub_8037388` - fully understood (an icon-manager draw
  loop and tile-cache-init helper for the counter widget above) but hit
  the same many-register gcc-2.9 allocation difficulty already
  documented for `sub_8006600` (`src/graphics/oam_count.c`).
- `sub_8037648`/`sub_8037A7C`/`sub_8037E54`/`sub_8037ECC`/`sub_8037F3C` -
  generic 64-bit software division/multiply helpers, per `docs/audio.md`.
- `sub_8037FC0`/`sub_8038240`/`sub_80384DC` - genuine GAX2 mixer-state/
  hardware-register internals; not attempted this pass.

From the `0x08038538`-`0x08039658` pass (issue #67, PR #198 - still
raw after the second pass, see
[`docs/matching/issue-67-0x08038538-audio.md`](../matching/issue-67-0x08038538-audio.md)):

- `sub_8038538`/`sub_8038A1C`/`sub_8038B68` - the play-start/init entry
  point (docs/audio.md) and its DMA1/Timer0 direct-sound-output
  follow-ups; hit the same many-register (`r8`/`sb`/`sl`) gcc-2.9
  allocation difficulty already documented for `sub_8006600`/
  `sub_80372BC` - not attempted further this pass.
- `sub_8038C88`/`sub_8038DC0` - more mixer-tick internals, read in full
  but not attempted as a C reconstruction; `sub_8038E74` (the
  voice-stealing allocator, docs/audio.md) hits the same many-register
  (`r8`) shape as its neighbors above - left raw.
- `sub_8038FD0`/`sub_8039064`/`sub_80390F8` - a per-channel mute/
  volume-set family; each hits the same many-register loop-allocation
  difficulty as the `sub_8038538` cluster above (confirmed via isolated
  compile - the ROM's own register allocation for this loop shape uses
  only r0-r3, no callee-saved registers, and no C rephrasing tried
  reproduced that).
- `sub_8039198`/`sub_80391E8` - contain the same hardware-register
  NOP-delay compiler quirk already flagged in-source at `sub_80384DC`
  above (`.byte 0x1b, 0x1c` / `mov r8, r8` x3); not attempted.
- `sub_8039214` - a word-wrap text/console-tile renderer (called by the
  now-matched `sub_80392E0`); fully understood, not attempted as a C
  reconstruction this pass - complex nested-loop control flow
  deprioritized in favor of the two matches this pass did land.

`sub_8039518`/`sub_80395A4`/`sub_8039658` (the "Channel" SoundHandler
type's init_fn/play_fn and the latter's direct callee) - listed raw
above as of the second `0x08038538`-`0x08039658` pass - are now Parked
NAKED transcriptions, see the "Parked" section above and
[docs/matching/issue-67-68-channel-init-play.md](../matching/issue-67-68-channel-init-play.md).

From the `0x08039818`-`0x0803A944` pass specifically (issue #68): 14 of
the chunk's 21 functions (`sub_80398DC`, `sub_803985C`,
`sub_8039AA4`, `sub_8039B44`, `sub_8039E50`, `sub_803A03C`, `sub_803A158`,
`sub_803A278`, `sub_803A2C8`, `sub_803A318`, `sub_803A324`, `sub_803A5A8`,
`sub_803A608`) - see
[`docs/matching/issue-68-0x08039818-audio.md`](../matching/issue-68-0x08039818-audio.md)'s
"Left raw" section for the per-function reason (many-register allocation
ceiling, or entangled with a neighbor via a manual return-address-
trampoline idiom).
