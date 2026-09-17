# Status: audio

`docs/audio.md`'s scope is different from this file: it documents the
**data layout** (how the editable-source instrument/sample/song pipeline
under `sound/` rebuilds `gStaticData_0855BCB4`), which is a separate,
already-largely-solved problem from matching the **engine code** that
plays it back, which is what this page tracks.

## Matched

`src/audio/` (new this pass - see `docs/matching.md`'s
"`0x08037110`-`0x08038538`" entry for the full write-up):

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

None yet.

## Left raw (not attempted, or attempted and set aside this pass)

Everything else in `asm/code_3.s`'s `0x08037110`-`0x0803B0C4` range
(interleaved with some generic compiler-runtime helpers that aren't
actually audio-related - see [docs/audio.md](../audio.md)), including,
from this pass specifically:

- `sub_80372BC`/`sub_8037388` - fully understood (an icon-manager draw
  loop and tile-cache-init helper for the counter widget above) but hit
  the same many-register gcc-2.9 allocation difficulty already
  documented for `sub_8006600` (`src/graphics/oam_count.c`).
- `sub_8037648`/`sub_8037A7C`/`sub_8037E54`/`sub_8037ECC`/`sub_8037F3C` -
  generic 64-bit software division/multiply helpers, per `docs/audio.md`.
- `sub_8037FC0`/`sub_8038240`/`sub_80384DC` - genuine GAX2 mixer-state/
  hardware-register internals; not attempted this pass.
