# Status: audio

The Shin'en GAX2 sound engine itself is still entirely raw assembly,
roughly `asm/code_3.s`'s `0x08037110`-`0x0803B0C4` range (interleaved
with some generic compiler-runtime helpers that aren't actually
audio-related - see [docs/audio.md](../audio.md)).

The wrapper layer everything else in the ROM calls (start/stop music,
fade/duck it, trigger one-shot and ambient sound effects) is matched,
operating on one shared `AudioContext` object - see
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

## Parked

- `PlaySfx` (`sub_8001854`, real bytes in `asm/code_3_1_10.s`,
  reconstruction in `src/audio/sfx_ambient.c`): a one-instruction
  prologue register-save-scheduling difference.
- `sub_80019F8` (real bytes in `asm/code_3_1_10_2.s`, reconstruction in
  `src/audio/audio_context.c`): a `u8` stack-parameter byte-load
  codegen shape and a base-volume field-address CSE difference.

See docs/matching.md for exactly what was tried on both.
