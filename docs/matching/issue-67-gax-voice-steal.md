# Issue #67: `0x08038C88`-`0x08038F94` (3 functions, `gax_voice_steal.c`)

`sub_8038C88`/`sub_8038DC0`/`sub_8038E74` (issue #67's remaining
`0x08038C88`-`0x08038F94` slice, `asm/code_3_2_20e_8c88.s`) are genuine
GAX2 mixer-tick/voice-stealing internals - not the false-positive
generic-helper situation issue #66's `math_div64_util.c` chunk turned
out to be. All three walk the same nested, still only partly-modeled
object chain the rest of this GAX2_SoundHandler cluster does
(`gUnknown_03001630->channels[curChannelIdx]` -> a "handler" object at
`+0` -> a third object at *its* `+0` whose `+0xc` supplies a base array
index, plus the handler's own `+8`/`+0x14` array-base/count pair; and a
separate per-voice state object at `channels[idx]->+4`, the same one
`sub_803A104`/`gax_channel_init.c` initializes). None of these nested
objects have confident names yet - kept as raw offsets throughout, per
this cluster's established convention.

All three are byte-verified NAKED transcriptions - for three genuinely
different reasons, not one blanket excuse:

## `sub_8038C88` - per-frame mixer tick

Once a song is loaded and playing, zero-fills a per-song `0x40`-byte
scratch buffer when present, clamps a byte-ish field to `0xFF`, mirrors
it into the current voice and into `gUnknown_03001630->+0x180`, flags
the current voice `+0x1a`, calls `sub_803A5A8` with an argument derived
from `+0x18`/`+0x2c` and a handler sub-field, toggles `+0x2c` bit 0,
copies the current voice's `+0x21` byte into the song's `+0x39`, and -
specifically when `curChannelIdx == 1` and that byte is non-zero -
resets `curChannelIdx` to 0 and re-links every one of the song's
channel-row `+8` voice-pointer slots to channel 0's voice.

Not attempted as real C this pass: the `p->channels[curChannelIdx]`
chase alone repeats 5 times through the function, several three hops
deep - substantially larger and more tangled than `sub_8038DC0` below,
which already resisted an extensive real-C attempt on a much simpler
shape. Deprioritized in favor of a confident semantic write-up (above)
plus a correct byte-verified transcription, rather than an open-ended
register-allocation chase with uncertain payoff.

## `sub_8038DC0` - UNUSED near-twin of `sub_8038E74`

No caller anywhere in the ROM (checked every `asm/*.s`, `expected/
code_3.s`, `expected/legacy.s`, and every matched `src/*.c` file).
Structurally it's `sub_8038E74`'s unconditional "scan every row for the
lowest `+0x4c` priority" path, standalone: walks `handler->+0x14`
array entries from `handler->+8 + (subObj->+0xc << 2)`, tracks the
lowest-priority index, then claims it (`+0x24 = 8`, `+0x25 = chanArg`,
`+0x4c = 0`) and returns the index. Reads like a simpler sibling
allocator that never got wired to a real call site.

This one got a real, extensive real-C attempt - unlike `sub_8038C88`
above, the blocker here isn't scope, it's a genuine register-allocation
gap: only `r4`-`r7` are live (no `r8`/`sb`/`sl` many-register ceiling),
and several reconstructions got very close - the loop shape, the
`bestIdx`-uninitialized-when-`count == 0` behavior (matching the ROM's
own apparent UB: `bestIdx` is never zero-initialized, only ever set
inside the "found a new best" branch), the `+8` channel-array-base
computed via an explicit intermediate expression, and most individual
register choices - but no version reproduced the ROM's exact
`r4`=best/`r5`=bestIdx/`r6`=saved-`&gUnknown_03001630`-copy/
`r7`=`chanArg` combination all at once:

- Left unpinned, this compiler homes `chanArg` (used only once, near
  the very end) to `r12` - a plausible, valid choice, just not the
  ROM's.
- Explicitly pinning `chanArg` to `r7` (`register s32 chanArg
  asm("r7")`) doesn't hold: the allocator still reuses `r7` for the
  post-loop saved-address value, which - confirmed by direct byte
  comparison against the real ROM disassembly, not assumed - means the
  final `strb` would read the *wrong* value at runtime (the saved
  address's low byte, not `chanArg`). This project's other NAKED
  write-ups already flag `register ... asm(...)` pins as reliable only
  for short-lived, tightly-scoped values; this is a concrete case where
  a pin spanning a whole function with intervening register pressure
  isn't safe to trust even when it happens to compile.

Given the function is provably unreachable, further register
archaeology wasn't worth chasing indefinitely once the semantics were
fully confirmed and a byte-verified transcription was in hand.

## `sub_8038E74` - the voice-stealing mixer allocator

Already characterized in `docs/audio.md` (one of `PlaySfx`'s two direct
callees): resolves a specific requested channel-row index (with a
priority-based fallback if it's already claimed), or - when the caller
passes `channel == -1` - runs the same unconditional lowest-priority
scan `sub_8038DC0` does, then claims the winner (`+0x24` gets `8` or,
when `pitchOffset != -1`, `(pitchOffset >> 5) + 2`; `+0x25` gets
`handle`; `+0x4c` gets `priority`).

NAKED for the same many-register (`r8`/`ip`) gcc-2.9 allocation
ceiling already documented throughout this ROM region
(`sub_8006600`/`sub_80372BC`/`sub_8038240`/`sub_8038538`/
`sub_8038A1C`) - `self` lives in `r8` and the saved
`&gUnknown_03001630` copy in `ip`, both held live across the whole
function including two separate scan loops.

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report` followed by
`rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `La suma coincide`.
