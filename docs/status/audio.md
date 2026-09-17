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
- `src/audio/audio_context.c`: `sub_8001AB8`, `sub_8001ABC`,
  `sub_8001AC0`, `sub_8001AC4`, `sub_8001AD8`, `sub_8001AEC`,
  `sub_8001B00`, `sub_8001B14`, `sub_8001B30`, `sub_8001B50`,
  `sub_8001B54`, `sub_8001B88`, `sub_8001BAC`, `sub_8001BD4`,
  `sub_8001C04`, `sub_8001C2C` (constructor), `sub_8001C64`.

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

## Parked

- `PlaySfx` (`sub_8001854`, real bytes in `asm/code_3_1_10.s`,
  reconstruction in `src/audio/sfx_ambient.c`): a one-instruction
  prologue register-save-scheduling difference.
- `sub_80019F8` (real bytes in `asm/code_3_1_10_2.s`, reconstruction in
  `src/audio/audio_context.c`): a `u8` stack-parameter byte-load
  codegen shape and a base-volume field-address CSE difference.

See docs/matching.md for exactly what was tried on both.

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
