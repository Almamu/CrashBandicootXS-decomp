# Status: audio

`src/audio/` doesn't exist yet - the Shin'en GAX2 sound engine is still
entirely raw assembly, roughly `asm/code_3.s`'s `0x08037110`-`0x0803B0C4`
range (interleaved with some generic compiler-runtime helpers that aren't
actually audio-related - see [docs/audio.md](../audio.md)). No functions
matched or parked here yet.

`docs/audio.md`'s scope is different from this file: it documents the
**data layout** (how the editable-source instrument/sample/song pipeline
under `sound/` rebuilds `gStaticData_0855BCB4`), which is a separate,
already-largely-solved problem from matching the **engine code** that
plays it back, which is what this page will track once that starts.
