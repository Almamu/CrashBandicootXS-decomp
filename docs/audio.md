# Audio: Shin'en GAX2 sound engine

The game's music and sound effects are driven by [Shin'en Multimedia's GAX Sound
Engine](https://www.shinen.com/) (GAX2 specifically), a third-party GBA audio
driver — not Nintendo's M4A/Sappy engine. It was identified via the `"GAX2"`
magic constant (`0x47415832`) embedded in the engine's own init code
(`GAX2_init`, ROM `0x08038538`, `lib/gax/src/gax_playstart.c`).

The whole engine (mixer, timer IRQ handler, replay logic) is matched C in
[`lib/gax/`](../lib/gax), kept apart from the game as a library (see
[`docs/libraries.md`](./libraries.md)): `lib/gax/src/*.c` (ROM
`0x08037F3C`-`0x0803A944`, including the ARM mixer/DSP routines, which are
inline in `gax_unknownc_play.c`), its strings and tables in
`lib/gax/data/gax_tables_5a6100.c`, the public API in `<gax.h>`
(`lib/gax/include/gax.h`) and the internal structures in
`lib/gax/src/gax_internal.h`. The libgcc 64-bit helpers linked in with it
(`__divdi3`/`__udivdi3`/`__udivsi3`/`__muldi3`, right before the engine)
are in `lib/libgcc/`. The game's side of the audio (the `AudioContext`
manager, the song and sound-effect tables) is in `src/audio/` and
`include/audio.h` and calls the engine through `<gax.h>`. The songs and
the sound-effect set below are the game's content in GAX2's format, so
they stay with the game's assets (`sound/`).

## Data layout

The engine data is two contiguous blocks, both **built from editable
sources**, not extracted verbatim from `baserom.gba` (see "Build pipeline"
below):

- the **sound-effect set** at `0x084C0006` (`gGaxSfxData`, 638,126
  bytes): its own 88 instruments and 87 samples, and the handler type the
  sound-effect voices run (see "Sound effects" below);
- the **music block** at `0x0855BCB4` (`gGaxMusicData`): the shared
  instrument/sample pool of the music and all 19 songs.

High level structure of the music block (base = `0x0855BCB4`):

```
[sfxTypes: 9 pointers to the sound-effect voice type, 36 bytes]
[instrument data: envelope + 12-byte "unknown" block + rows + header, per instrument, index order]
[instrument pointer table]
[sample data, index order, content-deduplicated]
[zero pad to a word boundary, 2 bytes]
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
the default song (manifest "default_song"), laid out the same way: one empty
    pattern, 7 title bytes, one pattern header, SongInfo (no instrument set,
    its sample-set pointer = its own sequence data), the handlers, and its
    GAX2_Song struct, which is gGaxDefaultSong (built as a separate file)
```

The default song is the engine's default handler layout: `GAX2_init`
and `GAX2_estimate` use `gGaxDefaultSong` when no layout is given. It
was the verbatim 160-byte `gax_footer.bin` before; its 20-byte song
struct sat right after it as a raw label.

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

## Engine API names

The engine's entry points carry Shin'en's own GAX2 API names (an exception
to the PascalCase rule, see [`docs/naming.md`](./naming.md)). Five are named
by the ROM itself: the engine's error reports hand the function name to
the fatal-error screen (`GaxFatalError`, "FUNCTION NAME:"), so
`gGaxErrNameNew`/`Init`/`Jingle`/`Irq` ("GAX2_NEW", "GAX2_INIT",
"GAX2_JINGLE", "GAX_IRQ") identify their callers, and "GAX_PLAY HAS NOT
FINISHED BEFORE GAX_IRQ" names `GAX_play` (it sets the `playDone` flag
`GAX_irq` checks). The rest match the published GAX API by signature and
behaviour:

| Function | Address | What it does |
|---|---|---|
| `GAX2_new(params)` | `0x080381FC` | fills a params block (`struct GaxSongHeader`) with defaults |
| `GAX2_estimate(params)` | `0x08037FC0` | stores the work-RAM size `GAX2_init` will need in `params->workSize` |
| `GAX2_init(params)` | `0x08038538` | builds the player in the work RAM and starts the song |
| `GAX2_jingle(song)` | `0x08038A1C` | plays a song as a jingle (player 1), handing back to the music when it ends |
| `GAX_irq()` | `0x08038B68` | the Timer IRQ half: re-arms the output DMA for the next buffer half |
| `GAX_play()` | `0x08038C88` | the per-frame half: mixes the next buffer half |
| `GAX_pause()` / `GAX_resume()` | `0x08038C28` / `0x08038C50` | stop / restart the Direct Sound output |
| `GAX_stop()` | `0x08039198` | stops the engine (output, DMA1, Timer0) |
| `GAX_fx(fxid)` | `0x08038DC0` | plays a sound effect on the lowest-priority voice (UNUSED) |
| `GAX_fx_ex(fxid, fxch, prio, note)` | `0x08038E74` | plays a sound effect on a given voice (or `-1` = any), with priority and note |
| `GAX_fx_note(fxch, note)` | `0x08038F94` | changes a playing sound effect's pitch (no caller found) |
| `GAX_stop_fx(fxch)` | `0x08038FD0` | key-off on one SFX voice (`-1` = all) |
| `GAX_set_music_volume(ch, vol)` | `0x08039064` | per-channel music volume (`-1` = all) |
| `GAX_set_fx_volume(fxch, vol)` | `0x080390F8` | per-voice SFX volume (`-1` = all) |

The handler types' callbacks are `GaxInfoInit`/`GaxInfoPlay` (the "Info"
type, the song position), `GaxChannelInit`/`GaxChannelPlay` (a tracker
channel), `GaxFxChannelInit`/`GaxFxChannelPlay` (a sound-effect voice) and
`GaxMixerInit`/`GaxMixerPlay` (the "UnknownC" type: the mixer). The three
named ARM routines are `gGaxArmDownmix` (16-bit mix to 8-bit output),
`gGaxArmEcho` (the echo/delay pass) and `gGaxArmResample` (a channel's
sample resampler, patched in place by `GaxChannelMix`).

## A few engine internals read directly (not part of the build pipeline)

Everything above the "Build pipeline" section was reverse-engineered
from the *data* layout, without needing to read the engine's own code
closely. A few of its functions have since been read directly (see
[`docs/rom_map.md`](./rom_map.md) for how this fits into the whole-ROM
picture):

- **`GAX2_init`** (ROM `0x08038538`, the function already cited above
  for the `"GAX2"` magic constant) is the engine's **play-start/init
  entry point**: initializes a runtime player-state object at
  `gGaxPlayerState` (writes the magic, stores the song/sound struct
  pointer, resets counters), validates an item count against a `0x18B`
  (395) sanity maximum, and fills in default fields - an instrument-bank
  pointer (`gGaxDefaultSong`) and a default volume (`0xFF`) - when
  the caller left them zero.
- **`gGaxDefaultSong`** (20 bytes) turns out to sit **immediately
  after** the documented `gGaxMusicData` audio block, not
  embedded within it - confirmed exactly: `gax_audio_data.bin` (the
  built block) is `0x48FA8` bytes, and `0x0855BCB4 + 0x48FA8 =
  0x085A4C5C` precisely. Decodes as `{count=4, ptr, ptr, ptr, ptr}` -
  four pointers, all pointing back into the tail of the audio block.
  It turned out to be the song struct of a silent default song whose
  other objects end the block (see "Data layout" above); it is now built
  with the block.
- **`GAX_fx_ex`** (one of `PlaySfx`'s two direct callees) is the
  **voice-stealing mixer allocator**: loops the current song's active
  channel handlers via `gGaxPlayerState`'s child-pointer chain, and
  either resolves a specific requested channel index, or - when the
  caller passes `-1` - scans for the channel with the lowest priority
  value at `+0x4C` to reuse. Textbook voice stealing.
- **`__divdi3`** (1076 B, the second-largest function in the whole
  GAX2 address range) is very likely **not GAX2 code at all** - its
  opening is the standard prologue shape for a software 64-bit
  division/multiply routine (sign-and-negate both operand halves before
  the real work), matching this document's own caveat above about
  generic compiler-runtime helpers sharing this address range. Not
  confirmed which operation, but the shape is unambiguous enough to
  flag as a likely false positive for anyone scanning this range by
  address alone.
- **`__udivdi3`** (984 B) is a **second confirmed non-GAX2 false
  positive** in this range: a generic 64-bit software division routine,
  read in full - normalizes the dividend via a 256-entry bit-
  normalization/leading-zero-count lookup table
  (`__clz_tab_udivdi3`, sitting right next to the `gStaticData_
  085A4C5C` instrument-selector data above) before doing long division
  via `__umodsi3`/`__udivsi3`. Worth noting explicitly: the small
  data cluster right after the audio block is itself mixed -
  `gGaxDefaultSong` plausibly audio-related,
  `__clz_tab_udivdi3` confirmed unrelated - so proximity to
  known-audio data doesn't settle the question either, only reading
  the consuming function does.
- **`sub_803A608` isn't really a function to characterize - it's a
  2-instruction stub (`nop; b _0803A61E`, plus a small fallback
  zero-fill loop) sitting in front of **~788 bytes** (revised up from
  an initial ~450-byte estimate; confirmed by tracing the raw ARM
  bytes continuously from `0x0803A630` to `0x0803A944`, where the
  BIOS `svc` wrapper stubs below begin) of genuine **ARM-mode (32-bit)
  machine code that the disassembler never actually disassembled as
  code**. The labels right after it
  (`gGaxArmDownmix`, `gStaticData_0803A67C`, `gStaticData_
  0803A73C`, `gGaxArmResample`) mark raw bytes that decode cleanly
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
  wants a byte-exact match through this stretch. **Not one routine**:
  at least 4 separate ARM function prologues appear inside the blob
  (`STMFD sp!,{...}` shapes at `0x0803A630`, `0x0803A67C`,
  `0x0803A73C`, `0x0803A818`), and two of the chunk boundaries carry
  embedded ASCII tags - `"FILT"` right before the `0x0803A73C` chunk,
  `"BART"` right before `0x0803A818` - reading like named-routine
  markers inside a hand-written ARM-mode DSP/mixer block (plausibly
  "filter" and some `"BART"`-tagged routine). Right after the blob,
  `BgAffineSet`/`CpuFastSet`/`CpuSet` are raw BIOS `svc` wrapper stubs (`svc
  #0xe`/`#0xc`/`#0xb` - the last is `CpuSet`), 4 bytes of real code
  each - confirmed genuine BIOS wrappers, not further GAX2 internals,
  and the true end of the GAX2 engine: only 12 bytes separate them
  from `LZ77UnCompVram` at `0x0803A950` (see `docs/rom_map.md`'s
  "Narrowing the GAX2 boundary" for the full boundary resolution).
- **`GaxChannelMix`** (780 B): reads a pattern/sequence pointer
  (`self+0x3C`), a note value checked against sentinel `0xFFFF8AD0`
  ("empty/no note", `self+0x2A`), and a small 0-3 index (`self+0x10`,
  plausibly a channel number) to step through pattern data and index a
  row table. Reads as the sequencer's actual **pattern/channel
  row-stepping logic** - genuinely core engine code, not a false
  positive.

## Sound effects

Distinct from music: `sub_8001854`, now matched as `PlaySfx` (see
[docs/status/audio.md](./status/audio.md) - called ~264 times across
gameplay code), is the sound-effect trigger:
`PlaySfx(context, sfx_id, volume_param)`. It looks up `sfx_id` in a
99-entry table at ROM `0x0816AA6C` (`sound/sfx_table.json`,
`struct SfxTableEntry` in `include/audio.h`), each entry `{slot_id,
chan_arg, volume}` (volume as 8.8 fixed point; the JSON still calls the
middle field `pitch_offset`) - the matching pass corrected the middle
field's guessed name from "pitch_offset" to `chan_arg`: `PlaySfx` passes
it straight through to `GAX_fx_ex` (as its priority argument, see
below), and the ambient-sfx sibling `PlayAmbientSfx` never reads it at all
(always passes a hardcoded `0` there instead) - and uses it to steal a
mixing voice (`GAX_fx_ex`) and play a note.

That note does **not** come from the music's instrument pool: an earlier
version of this section said there was no separate sound-effect sample
bank, which is wrong. The sound effects have their own GAX2 data set, the
block at `0x084C0006` right before the music. The engine path, all in
matched C:

- `StartSong` (`music_player.c`) fills the `GaxSongHeader` it hands to
  `GAX2_init`: `numSfx` (`+0x0E`) = 3 sound-effect voices, and
  `sfxTypes` (`+0x2C`) = `gGaxMusicData`, the 9-pointer array at
  the start of the music block. Every pointer is the same handler type,
  `0x0855BC98`, the last thing in the sound-effect set.
- `GAX2_init` instantiates `numSfx` voices from `sfxTypes` after the
  song's own handlers, and the mixer (`GaxMixerHandler.extraChildren`)
  mixes them after the song's channels.
- `PlaySfx`/`PlayAmbientSfx` call `GAX_fx_ex(instrument, voice, priority,
  -1)`, which queues `instrument` (the table's `slot_id`) on a voice at
  note 8. `PlaySfx` alternates voices 0 and 1; the ambient channel of
  `PlayAmbientSfx`/`TickAmbientSfx` uses voice 2. The table's middle field is
  `GAX_fx_ex`'s *priority* argument (the voice-steal threshold), not a
  channel: the channel is the round-robin toggle.
- The voice type's play function `GaxFxChannelPlay` starts the queued
  instrument with `GaxChannelSetInstrument(self, info, instrument,
  self->type->data.song)`, and that type data is the sound-effect set's
  song header (`0x0855BC78`), whose instrument and sample tables are the
  set's own. So `slot_id` N (1-87) is sound-effect instrument N, and
  instrument N plays sample N.

The set's layout (base `0x084C0006`) is the music block's instrument and
sample pool with a one-type tail instead of songs:

```
[2-byte zero pad, to a word boundary]
[instrument data: envelope + 12-byte "unknown" block + rows + header, per instrument, index order]
[instrument pointer table, 88 entries]
[sample data, index order, NOT deduplicated (samples 84 and 87 are identical, stored twice)]
[zero pad to a word boundary, 3 bytes]
[sample table: {data, length} x 88, entry 0 empty]
[song header (GAX_SongInfo, 0x1C): no channels or patterns, volume 0x100, the two tables]
[the handler type's 1-entry child-type array: NULL]
[the sound-effect voice handler type: GaxFxChannelInit/sub_803A228/GaxFxChannelPlay, 1 child, 0x48-byte instances, data = the song header]
```

All 87 instruments (0 is an empty placeholder) have one row with a
fixed pitch (`dont_use_note_pitch`, note 2) and the same `Pitch` (1511),
so every sound effect plays its sample at one rate, about 7,994 Hz by
the formula in "Sample pitch / playback rate" below. That is the rate the
`.wav` files declare.

## Build pipeline

Everything is generated from editable sources, never read from
`baserom.gba` at build time:

- `sound/gax_manifest.json` — one-time-extracted instrument definitions and
  per-song scaffold data (channel transposes, pattern grouping/order, title
  strings, etc).
- `sound/songs/*.xm` — one FastTracker II module per song (patterns/notes),
  editable in any XM tracker.
- `sound/samples/*.wav` — one sample per instrument sample slot.
- `sound/gax_sfx_manifest.json` — the sound-effect set: its 88 instruments
  (the same fields as the music's), the voice type's song-header fields,
  and `voice_types`, the length of the `sfxTypes` array (9).
- `sound/sfx_samples/*.wav` — the sound-effect set's 87 samples, 01-87
  (8-bit mono, declared at 7,994 Hz, see "Sound effects").
- `tools/gax_audio.py` — a from-scratch GAX2 encoder (no external tool or
  `.NET` dependency) that links all of the above back into the original
  binary layout: `tools/gax_audio.py OUT LAYOUT SONGS.h` the music block,
  the default song's layout struct (`gGaxDefaultSong`) and a header
  of each song's offset in the block (`GAX_SONG_<NAME>`),
  `tools/gax_audio.py --sfx OUT` the sound-effect set. The music block's
  leading `sfxTypes` array is generated from the sound-effect set's
  layout (it was the verbatim 36-byte `gax_header_prefix.bin` before).
  With unedited sources, **it reproduces the original ROM's audio blocks
  byte-for-byte** (verified both in isolation and via a full clean
  `make compare`). Editing a `.xm` or `.wav` changes only the bytes that
  actually need to differ.
- Nothing outside the audio build points into the music block by a fixed
  address any more: the song table `gSongTable`
  (`src/data/song_table_16aa20.c`) is written as `gGaxMusicData +
  GAX_SONG_<NAME>` from the generated `gax_songs.h`, and the default layout
  is built with the block. So a song can change size. The block itself is
  linked at the fixed `BASE_ADDR` (its pointers are absolute), which is
  where the sound-effect set ends, so the sound-effect set must keep its
  total size (the tool stops with an error otherwise): a sample can be
  changed but not lengthened, unless another shrinks to match.
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
