# Audio: Shin'en GAX2 sound engine

The game's music and sound effects are driven by [Shin'en Multimedia's GAX Sound
Engine](https://www.shinen.com/) (GAX2 specifically), a third-party GBA audio
driver — not Nintendo's M4A/Sappy engine. It was identified via the `"GAX2"`
magic constant (`0x47415832`) embedded in the engine's own init code
(`asm/code_3.s`, around `sub_8038538`, ROM `0x08038538`).

The whole engine (mixer, timer IRQ handler, replay logic) lives in
`asm/code_3.s` roughly between `0x08037110` and `0x0803A950` (narrowed
from an earlier `0x0803B0C4` estimate — see
[`docs/rom_map.md`](./rom_map.md#narrowing-the-gax2-boundary) for the
evidence: `LZ77UnCompWrapper`/`RLUnCompWrapper`, confirmed non-audio, sit
right at the old upper bound). **Be aware this range also contains generic
compiler-runtime helpers** (a software 64-bit division routine, ARM/Thumb
interworking trampolines, BIOS `svc` wrapper stubs) interleaved with
genuine GAX2 code — don't assume every function in that address range is
audio-related just because of where it sits.

## Data layout

All engine data (shared instrument/sample pool + all 19 songs) is one
contiguous block at ROM `0x0855BCB4`, exposed as `gStaticData_0855BCB4` in
`data/data.s`. It is **built from editable sources**, not extracted verbatim
from `baserom.gba` — see "Build pipeline" below.

High level structure (base = `0x0855BCB4`):

```
[header_prefix, 36 bytes, unmodeled]
[instrument data: envelope + 12-byte "unknown" block + rows + header, per instrument, index order]
[instrument pointer table]
[sample data, index order, content-deduplicated]
[2-byte zero pad]
[sample table: {offset, length} x N]
[4-byte zero pad]
per song (in ROM address order, NOT alphabetical - see gax_manifest.json "song_order"):
    [patterns, allocated in true physical ROM order per song ("pattern_group_order")]
    [title/artist string, verbatim padded bytes]
    [pattern-header tables, one per channel, in true physical channel order ("channel_order"),
     NOT logical channel index order]
    [GAX_SongInfo]
    [Info SoundHandler]
    [shared 1-entry "children" array -> points at the Info handler]
    [Channel SoundHandlers, one per channel, same physical order as the pattern-header tables;
     each has num_children=1 pointing at the shared children array above]
    [UnknownC children array: channel handler addresses, in LOGICAL channel index order]
    [unknownc_data_bytes, 28 bytes]
    [UnknownC SoundHandler]
    [GAX2_Song struct]
[footer, 160 bytes, unmodeled - self-referential, purpose not understood]
```

### GAX2_Song / SoundHandler / SongInfo

- `GAX2_Song`: `num_items` (u32), `unknownc_handler_ptr`, `info_handler_ptr`,
  `unk_ptr` (see below), then `num_items - 3` channel handler pointers.
- `GAX2_SoundHandler` (28 bytes, one of three "types": Info / UnknownC /
  Channel): each type has **three fixed function-pointer constants**
  (`init_fn`/`unknown_fn`/`play_fn`) that are always the same for that type,
  regardless of song:
  - Info: `0x080393FD`, `0x08039439`, `0x0803943D`
  - UnknownC: `0x0803A22D`, `0x0803A275`, `0x0803A325`
  - Channel: `0x08039519`, `0x080395A1`, `0x080395A5`
- `song.unk_ptr` is a fixed constant equal to the address right after the end
  of the shared sample table, for every song.
- Each channel's `SoundHandler` has `num_children = 1` and `children_ptr`
  pointing at **one shared** 4-byte array (per song) containing the Info
  handler's address — not a per-channel array.
- A per-song title/artist string (`"Title" (c) Manfred Linzner`-style, with
  original padding) sits between a song's patterns and its pattern-header
  tables. Captured verbatim as `title_bytes` in the manifest rather than
  recomputed, to avoid cumulative address-drift.

### Instruments (`GAX_Instrument`)

52 shared instruments, referenced by index from every song. Each has:
byte fields, up to 4 `sample_indices` (0 = unused slot), vibrato
delay/depth/speed, an optional envelope, a 12-byte "unknown" block, and a
sequence of `rows` (the instrument's own internal note/effect mini-sequence,
distinct from a channel's pattern rows).

- `Instrument.rows[0].dont_use_note_pitch`: when true, the *pattern* note
  that triggers this instrument is irrelevant — the instrument always plays
  at a fixed pitch derived from its own `Pitch` field (see "Sample pitch /
  playback rate" below).
- Instrument ROWS are **not deduplicated** across different instruments even
  when byte-identical — the original data doesn't share them, despite it
  looking like an obvious space-saving opportunity.

### Samples

29 shared raw signed 8-bit PCM samples (`sound/samples/*.wav`, indices
01-29), referenced only via instruments — deduplicated by content when
linking (unlike instrument rows).

### Sample pitch / playback rate

GAX stores no explicit "Hz" for a sample. Each `InstrumentSample` has a
signed 16-bit `Pitch` field, which decodes into a standard XM-style
transpose + finetune pair (formula cross-checked against the [`gaxm`
GAX->XM converter](https://github.com/byvar/gaxm)'s own exporter,
`GAX_XMWriter.cs`):

```
instrPitch   = Pitch / 32
relativeNote = instrPitch - 1                      (fixed-pitch instruments ignore the row's own note)
fineTune     = (Pitch - instrPitch * 32) * 4
```

Combined with the standard XM/FastTracker reference (semitones relative to
C-4 = 8363 Hz) and the actual pattern note a sample is triggered at, this
gives a real playback rate:

```
semis = (pattern_note - 49) + relativeNote + fineTune/128
rate  = 8363 * 2 ** (semis / 12)
```

This matters for `sound/samples/*.wav` preview accuracy (the WAV's declared
sample rate is cosmetic only — the build never reads it, see below). Most
samples are melodic and get replayed at dozens of different notes across
songs, so there's no single "correct" preview rate for them — they're left
at a neutral 15769 Hz default. A handful of samples are only ever triggered
at one fixed effective pitch (one-shot/percussive use, or a single jingle
like the Japanese commercial cue that reuses sample 25); those got a real
derived rate instead (see the "Fix preview sample rates" commit for the
full list).

## A few engine internals read directly (not part of the build pipeline)

Everything above the "Build pipeline" section was reverse-engineered
from the *data* layout, without needing to read the engine's own code
closely. A few of its functions have since been read directly (see
[`docs/rom_map.md`](./rom_map.md) for how this fits into the whole-ROM
picture):

- **`sub_8038538`** (ROM `0x08038538`, the function already cited above
  for the `"GAX2"` magic constant) is the engine's **play-start/init
  entry point**: initializes a runtime player-state object at
  `gUnknown_03001630` (writes the magic, stores the song/sound struct
  pointer, resets counters), validates an item count against a `0x18B`
  (395) sanity maximum, and fills in default fields - an instrument-bank
  pointer (`gStaticData_085A4C5C`) and a default volume (`0xFF`) - when
  the caller left them zero.
- **`gStaticData_085A4C5C`** (20 bytes) turns out to sit **immediately
  after** the documented `gStaticData_0855BCB4` audio block, not
  embedded within it - confirmed exactly: `gax_audio_data.bin` (the
  built block) is `0x48FA8` bytes, and `0x0855BCB4 + 0x48FA8 =
  0x085A4C5C` precisely. Decodes as `{count=4, ptr, ptr, ptr, ptr}` -
  four pointers, all pointing back into the tail of the audio block -
  plausibly a small default-instrument-set selector table
  `sub_8038538`'s play-start logic falls back to.
- **`sub_8038E74`** (one of `PlaySfx`'s two direct callees) is the
  **voice-stealing mixer allocator**: loops the current song's active
  channel handlers via `gUnknown_03001630`'s child-pointer chain, and
  either resolves a specific requested channel index, or - when the
  caller passes `-1` - scans for the channel with the lowest priority
  value at `+0x4C` to reuse. Textbook voice stealing.
- **`sub_8037648`** (1076 B, the second-largest function in the whole
  GAX2 address range) is very likely **not GAX2 code at all** - its
  opening is the standard prologue shape for a software 64-bit
  division/multiply routine (sign-and-negate both operand halves before
  the real work), matching this document's own caveat above about
  generic compiler-runtime helpers sharing this address range. Not
  confirmed which operation, but the shape is unambiguous enough to
  flag as a likely false positive for anyone scanning this range by
  address alone.
- **`sub_8037A7C`** (984 B) is a **second confirmed non-GAX2 false
  positive** in this range: a generic 64-bit software division routine,
  read in full - normalizes the dividend via a 256-entry bit-
  normalization/leading-zero-count lookup table
  (`gStaticData_085A4D70`, sitting right next to the `gStaticData_
  085A4C5C` instrument-selector data above) before doing long division
  via `sub_803AF1C`/`sub_8037E54`. Worth noting explicitly: the small
  data cluster right after the audio block is itself mixed -
  `gStaticData_085A4C5C` plausibly audio-related,
  `gStaticData_085A4D70` confirmed unrelated - so proximity to
  known-audio data doesn't settle the question either, only reading
  the consuming function does.
- **`sub_803A608` isn't really a function to characterize - it's a
  2-instruction stub (`nop; b _0803A61E`, plus a small fallback
  zero-fill loop) sitting in front of roughly 450 bytes of genuine
  **ARM-mode (32-bit) machine code that the disassembler never actually
  disassembled as code**. The labels right after it
  (`gStaticData_0803A630`, `gStaticData_0803A67C`, `gStaticData_
  0803A73C`, `gStaticData_0803A818`) mark raw bytes that decode cleanly
  as ARM instruction encodings (e.g. `60 00 2D E9` = ARM `STMFD
  sp!,{...}`, a classic ARM function prologue) - this codebase is
  otherwise entirely Thumb, so whatever raw-asm-extraction pass
  produced `asm/code_3.s` correctly recognized this stretch wasn't
  Thumb and gave up, dumping it as opaque data instead. GAX2 shipping a
  hand-written ARM-mode routine (for mixer speed - ARM decodes faster
  per-instruction than Thumb on the ARM7TDMI, a known trick for hot
  loops) inside an otherwise all-Thumb ROM is entirely plausible and
  would explain the mismatch. Genuinely different from the two false
  positives above - not mislabeled *ownership* (GAX2 vs. generic
  helper), but a mislabeled *instruction set*, and a real gap in this
  project's disassembly coverage worth flagging for whoever eventually
  wants a byte-exact match through this stretch.
- **`sub_8039B44`** (780 B): reads a pattern/sequence pointer
  (`self+0x3C`), a note value checked against sentinel `0xFFFF8AD0`
  ("empty/no note", `self+0x2A`), and a small 0-3 index (`self+0x10`,
  plausibly a channel number) to step through pattern data and index a
  row table. Reads as the sequencer's actual **pattern/channel
  row-stepping logic** - genuinely core engine code, not a false
  positive.

## Sound effects

Distinct from music: `sub_8001854` (called ~264 times across gameplay code)
is the sound-effect trigger. It's `PlaySfx(context, sfx_id, volume_param)`
in spirit. It looks up `sfx_id` in a 99-entry table at ROM `0x0816AA6C`
(`sound/sfx_table.json`), each entry `{slot_id, pitch_offset, volume}`
(volume as 8.8 fixed point), and uses it to steal a mixing voice
(`sub_8038E74`) and play a note from the *same* shared instrument/sample
pool music uses — **there is no separate sound-effect sample bank**.

## Build pipeline

Everything is generated from editable sources, never read from
`baserom.gba` at build time:

- `sound/gax_manifest.json` — one-time-extracted instrument definitions and
  per-song scaffold data (channel transposes, pattern grouping/order, title
  strings, etc).
- `sound/songs/*.xm` — one FastTracker II module per song (patterns/notes),
  editable in any XM tracker.
- `sound/samples/*.wav` — one sample per instrument sample slot.
- `sound/gax_header_prefix.bin` / `sound/gax_footer.bin` — two small
  verbatim blobs of shared structure that aren't modeled (36 and 160 bytes).
- `tools/gax_audio.py` — a from-scratch GAX2 encoder (no external tool or
  `.NET` dependency) that links all of the above back into the original
  binary layout. With unedited sources, **it reproduces the original ROM's
  audio block byte-for-byte** (verified both in isolation and via a full
  clean `make compare`). Editing a `.xm` or `.wav` changes only the bytes
  that actually need to differ.
- `tools/sfx_table.py` — rebuilds the sound-effect trigger table from
  `sound/sfx_table.json`.

Both tools write into `build/crashbandicootxs/sound/` — `sound/` itself only
ever holds editable sources, never build products.

## Tooling used during reverse-engineering (not part of the build)

- [`gaxm`](https://github.com/byvar/gaxm) (and its `BinarySerializer.GBA.Audio`
  submodule) — used as a research/ground-truth reference only, never shipped
  in this repo. Its GAX->XM export was used to derive the initial `.xm` files
  (verified as valid sources) and its `GAX_XMWriter.cs` supplied the
  pitch/finetune formula above.
- Byte-perfect validation methodology: diff the rebuilt block against the
  original ROM, isolate the smallest reproducible discrepancy (an address
  range or byte count that doesn't match), fix one structural bug at a time,
  repeat. This is how the ~11 structural bugs in the encoder (wrong section
  order, missing boundary padding, wrong pattern/channel physical ordering,
  incorrectly deduplicated instrument rows, etc.) were found and fixed.
