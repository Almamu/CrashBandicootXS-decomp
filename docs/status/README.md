# Status

Per-system matching status, mirroring `src/`'s `graphics/`/`util/`/`system/`
layout (plus `audio`, which has no C source yet). Each page lists that
system's matched functions by file, its parked (`NON_MATCHING`) ones with a
one-line summary of what's left, and any other file-specific note. For *how*
a function gets from one list to the other, see
[docs/workflow.md](../workflow.md); for the detailed per-function log behind
every entry here, see [docs/matching.md](../matching.md).

- [graphics.md](./graphics.md) - OAM/sprite rendering, screen fades, palette
  blending, per-actor animation frames, text layout
- [util.md](./util.md) - math, string/printf, RNG, line-drawing, time
  formatting helpers
- [system.md](./system.md) - startup, memory allocator, interrupts, input
  polling, tagged-asset loading
- [audio.md](./audio.md) - the Shin'en GAX2 sound engine (not started)
