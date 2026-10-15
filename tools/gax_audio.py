#!/usr/bin/env python3
"""
Builds sound/gax_audio_data.bin (the Shin'en GAX2 sound-engine data block used
by this game: shared instrument/sample pool + all 19 songs) from editable
sources:

  sound/gax_manifest.json  - instrument definitions (envelopes, vibrato, rows),
                              per-song scaffold fields, and the raw bytes for a
                              small pre-existing shared structure this data
                              happens to sit next to (see NOTE below).
  sound/songs/*.xm          - one FastTracker II module per song. Patterns,
                              notes and instrument assignments are read from
                              here - this is what a modder edits to change a
                              song.
  sound/songs/<key>.json    - a new song's settings (optional): a song of
                              the manifest's song_table with no recorded
                              scaffold has it derived from its XM
                              (new_song, docs/audio.md "Adding a song").
  sound/samples/*.wav       - one 8-bit mono PCM sample per instrument voice
                              (shared across songs) - this is what a modder
                              edits to change a sound.

It also builds the GAX2 sound-effect set that sits right before the music
block (`--sfx`, see docs/audio.md) from

  sound/gax_sfx_manifest.json - its 88 instruments (the same record types as
                              the music's), the handler type's song fields
                              and how many voice-type pointers to emit.
  sound/sfx_samples/*.wav     - its 87 samples (8-bit mono PCM).

The music block starts with the 36-byte GaxSongHeader.sfxTypes array
(StartSong passes gGaxMusicData there): nine pointers to the
sound-effect set's one handler type, generated from that set's layout.

Each block is written twice: OUTPUT.bin, its bytes with every pointer
left 0, and OUTPUT.s, which data/data.s includes: the .bin's bytes
(`.incbin`) with each pointer as a relocation (`.4byte gGaxMusicData +
0x1234`, `.4byte GaxInfoInit`), so the blocks can sit anywhere in the
ROM and change size (#967, docs/shifting.md). Each block must start
word-aligned; data/data.s aligns them.

The block ends with a 20th, silent song: the engine's default handler
layout (one channel, one empty pattern, no instruments; manifest
`default_song`). Its song struct is written to a separate file (LAYOUT.bin),
which data/data.s incbins as gGaxDefaultSong, the label the code uses.
SONGS.h #defines each song's offset in the block for the song table
(src/engine/audio/data/song_table.cpp), so songs can change size.

The song IDs (the gSongTable index) come from the manifest's `song_table`
list, in table order, each with a comment on where the game plays it, and
`no_song_comment` documents SONG_NONE (one past the table).
`--constants SONGS.h` writes them as the SONG_* #defines of
constants/songs.h (build/include), and `--table SONG_TABLE.inc` gSongTable
in the same order (the song table song_table.cpp #includes); both read only
the manifest.

This tool does not need baserom.gba - the manifest captured everything
required when it was generated (a one-time step; see the project history for
how gax_manifest.json/songs/samples were produced from the original ROM).
With unedited sources, the output reproduces the original ROM's audio block
byte-for-byte (verified during development).
"""
import base64
import json
import os
import struct
import sys
import wave

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SOUND_DIR = os.path.join(ROOT, 'sound')

# The labels data/data.s gives the two blocks: every pointer into a block
# is a relocation against its label.
MUSIC_SYMBOL = 'gGaxMusicData'
SFX_SYMBOL = 'gGaxSfxData'


class Ptr:
    """An address the linker fills in: `symbol + off`. The blocks' own
    addresses are Ptrs into them; arithmetic and ordering work within one
    block."""

    def __init__(self, symbol, off=0):
        self.symbol = symbol
        self.off = off

    def _same(self, other):
        if not isinstance(other, Ptr) or other.symbol != self.symbol:
            raise TypeError(f"{self!r} and {other!r} are not in the same block")
        return other.off

    def __add__(self, n):
        return Ptr(self.symbol, self.off + n)

    def __sub__(self, other):
        if isinstance(other, Ptr):
            return self.off - self._same(other)
        return Ptr(self.symbol, self.off - other)

    def __lt__(self, other):
        return self.off < self._same(other)

    def __eq__(self, other):
        return isinstance(other, Ptr) and (self.symbol, self.off) == (other.symbol, other.off)

    def __hash__(self):
        return hash((self.symbol, self.off))

    def __repr__(self):
        return f"{self.symbol} + {self.off:#x}" if self.off else self.symbol


class Chunk:
    """Bytes with relocations: a packed structure whose pointer fields are
    Ptrs. A pointer's bytes are 0; `relocs` gives (offset, Ptr) for each."""

    def __init__(self, data=b''):
        self.buf = bytearray(data)
        self.relocs = []

    def u32(self, v):
        if isinstance(v, Ptr):
            self.relocs.append((len(self.buf), v))
            v = 0
        self.buf += struct.pack('<I', v)
        return self

    def raw(self, data):
        self.buf += data
        return self

    def key(self):
        return bytes(self.buf), tuple(self.relocs)


# The 3 function pointers (Init/Unknown/Play) in a GAX2_SoundHandler are the
# engine's own dispatch code, not per-song data - fixed per handler type,
# identical across every song.
HANDLER_FUNCS = {
    'info': (Ptr('GaxInfoInit'), Ptr('GaxInfoUnknown'), Ptr('GaxInfoPlay')),
    'mixer': (Ptr('GaxMixerInit'), Ptr('GaxMixerUnknown'), Ptr('GaxMixerPlay')),
    'channel': (Ptr('GaxChannelInit'), Ptr('GaxChannelUnknown'), Ptr('GaxChannelPlay')),
    # the sound-effect voices
    'sfx': (Ptr('GaxFxChannelInit'), Ptr('GaxFxChannelUnknown'), Ptr('GaxFxChannelPlay')),
}


# ---------------------------------------------------------------------------
# GAX2 binary structures (pack side only - see project history for the
# reader/verification tooling this was derived from)
# ---------------------------------------------------------------------------

class SoundHandler:
    SIZE = 28

    def __init__(self, init_fn=0, unknown_fn=0, play_fn=0, num_children=0,
                 children_ptr=0, type_flags=0, data_ptr=0):
        self.init_fn = init_fn
        self.unknown_fn = unknown_fn
        self.play_fn = play_fn
        self.num_children = num_children
        self.children_ptr = children_ptr
        self.type_flags = type_flags
        self.data_ptr = data_ptr

    def pack(self):
        c = Chunk()
        for v in (self.init_fn, self.unknown_fn, self.play_fn,
                  self.num_children, self.children_ptr, self.type_flags, self.data_ptr):
            c.u32(v)
        return c


class SongInfo:
    def __init__(self, m):
        self.num_channels = m['num_channels']
        self.num_rows_per_pattern = m['num_rows_per_pattern']
        self.num_patterns_per_channel = m['num_patterns_per_channel']
        self.loop_point = m['loop_point']
        self.volume = m['volume']
        self.ushort_0a = m['ushort_0a']
        self.sample_rate = m['sample_rate']
        self.num_fx_channels = m['num_fx_channels']
        self.half_rate_fx = m['half_rate_fx']
        self.sequence_data_ptr = 0
        self.instrument_set_ptr = 0
        self.sample_set_ptr = 0

    def pack(self):
        c = Chunk(struct.pack('<6H', self.num_channels, self.num_rows_per_pattern,
                              self.num_patterns_per_channel, self.loop_point, self.volume,
                              self.ushort_0a))
        c.u32(self.sequence_data_ptr).u32(self.instrument_set_ptr).u32(self.sample_set_ptr)
        return c.raw(struct.pack('<H2B', self.sample_rate, self.num_fx_channels, self.half_rate_fx))


class PatternRow:
    CMD_NOTE = 'note'
    CMD_NOTE_ONLY = 'note_only'
    CMD_REST = 'rest'
    CMD_EFFECT_ONLY = 'effect_only'
    CMD_REST_MULTI = 'rest_multi'

    def __init__(self):
        self.note = 0
        self.instrument = 0
        self.effect = 0
        self.effect_param = 0
        self.rest_duration = 0
        self.cmd = self.CMD_REST

    def pack(self):
        if self.cmd == PatternRow.CMD_NOTE:
            return bytes([self.note & 0x7F, self.instrument, self.effect, self.effect_param])
        elif self.cmd == PatternRow.CMD_NOTE_ONLY:
            return bytes([0x80 | (self.note & 0x7F), self.instrument])
        elif self.cmd == PatternRow.CMD_EFFECT_ONLY:
            return bytes([0xFA, self.effect, self.effect_param])
        elif self.cmd == PatternRow.CMD_REST_MULTI:
            return bytes([0xFF, self.rest_duration])
        elif self.cmd == PatternRow.CMD_REST:
            return bytes([0x80])
        raise ValueError(f"unknown cmd {self.cmd}")


class Pattern:
    def __init__(self):
        self.is_empty = False
        self.rows = []

    def pack(self):
        if self.is_empty:
            return bytes([1])
        return bytes([0]) + b"".join(r.pack() for r in self.rows)


def unpack_pattern(buf):
    """Inverse of Pattern.pack()/PatternRow.pack() - only used to compare a
    freshly-imported XM pattern against the original recorded bytes (see
    NOTE in link())."""
    pat = Pattern()
    if buf[0]:
        pat.is_empty = True
        return pat
    pos = 1
    rows = []
    while pos < len(buf):
        flags = buf[pos]
        row = PatternRow()
        if flags == 0x80:
            row.cmd = PatternRow.CMD_REST
            pos += 1
        elif flags == 0xFA:
            row.cmd = PatternRow.CMD_EFFECT_ONLY
            row.effect = buf[pos + 1]
            row.effect_param = buf[pos + 2]
            pos += 3
        elif flags == 0xFF:
            row.cmd = PatternRow.CMD_REST_MULTI
            row.rest_duration = buf[pos + 1]
            pos += 2
        elif flags & 0x80:
            row.cmd = PatternRow.CMD_NOTE_ONLY
            row.note = flags & 0x7F
            row.instrument = buf[pos + 1]
            pos += 2
        else:
            row.cmd = PatternRow.CMD_NOTE
            row.note = flags & 0x7F
            row.instrument = buf[pos + 1]
            row.effect = buf[pos + 2]
            row.effect_param = buf[pos + 3]
            pos += 4
        rows.append(row)
    pat.rows = rows
    return pat


def _note_instrument_timeline(pat, instruments_by_index):
    """(cmd_kind, note_or_None, instrument_or_None) per tick, ignoring effects
    and the exact 2-vs-4-byte encoding choice - used only to detect whether a
    modder actually changed the musical content of a pattern. For fixed-pitch
    instruments (e.g. drums) the note value is cosmetic (the game ignores it),
    and gaxm's own GAX->XM export already replaces it with a placeholder, so
    it's normalized away here too rather than treated as a real edit."""
    if pat.is_empty:
        return ('empty',)
    timeline = []
    for r in pat.rows:
        if r.cmd == PatternRow.CMD_REST:
            timeline.append(('rest',))
        elif r.cmd == PatternRow.CMD_REST_MULTI:
            timeline.extend([('rest',)] * r.rest_duration)
        elif r.cmd == PatternRow.CMD_EFFECT_ONLY:
            timeline.append(('rest',))
        else:
            ins = instruments_by_index.get(r.instrument)
            fixed_pitch = ins is not None and ins.rows and ins.rows[0].dont_use_note_pitch
            note = None if (fixed_pitch or r.note == 1) else r.note
            timeline.append(('note', note, r.instrument))
    return tuple(timeline)


class InstrumentEffect:
    def __init__(self, type_=0, param=0):
        self.type = type_
        self.param = param

    def pack(self):
        val = (self.param & 0xFF) | ((self.type & 0xFF) << 8)
        return struct.pack('<H', val)


class InstrumentRow:
    def __init__(self, m):
        self.relative_note_number = m['relative_note_number']
        self.dont_use_note_pitch = m['dont_use_note_pitch']
        self.sample_index = m['sample_index']
        self.byte_03 = m['byte_03']
        self.effects = [InstrumentEffect(e['type'], e['param']) for e in m['effects']]

    def pack(self):
        out = bytes([self.relative_note_number, 1 if self.dont_use_note_pitch else 0,
                      self.sample_index, self.byte_03])
        return out + self.effects[0].pack() + self.effects[1].pack()


class InstrumentSample:
    def __init__(self, m):
        self.byte_02 = m['byte_02']
        self.is_bidirectional = m['is_bidirectional']
        self.start_position = m['start_position']
        self.loop_start = m['loop_start']
        self.loop_end = m['loop_end']
        self.int_10 = m['int_10']
        self.ushort_14 = m['ushort_14']
        self.ushort_16 = m['ushort_16']
        self.pitch = m['pitch']

    def pack(self):
        out = bytearray(0x1C)
        out[0] = self.byte_02
        out[1] = 1 if self.is_bidirectional else 0
        struct.pack_into('<i', out, 4, self.start_position)
        struct.pack_into('<I', out, 8, self.loop_start & 0xFFFFFFFF)
        struct.pack_into('<I', out, 0xC, self.loop_end & 0xFFFFFFFF)
        struct.pack_into('<i', out, 0x10, self.int_10)
        struct.pack_into('<I', out, 0x14, self.ushort_14 & 0xFFFFFFFF)
        struct.pack_into('<H', out, 0x18, self.ushort_16)
        struct.pack_into('<h', out, 0x1A, self.pitch)
        return bytes(out)


class EnvelopePoint:
    def __init__(self, m):
        self.x = m['x']
        self.interpolation = m['interpolation']
        self.y = m['y']
        self.byte_05 = m['byte_05']
        self.ushort_06 = m['ushort_06']

    def pack(self):
        return struct.pack('<Hh2BH', self.x, self.interpolation, self.y, self.byte_05, self.ushort_06)


class Envelope:
    def __init__(self, m):
        self.sustain = m['sustain']
        self.loop_start = m['loop_start']
        self.loop_end = m['loop_end']
        self.points = [EnvelopePoint(p) for p in m['points']]

    def pack(self):
        out = bytearray([len(self.points),
                          self.sustain if self.sustain is not None else 0xFF,
                          self.loop_start if self.loop_start is not None else 0xFF,
                          self.loop_end if self.loop_end is not None else 0xFF])
        for p in self.points:
            out += p.pack()
        return bytes(out)


class Instrument:
    HEADER_SIZE = 0xC + 4 * 0x1C + 4 + 4 + 1 + 1 + 2 + 4

    def __init__(self, m):
        self.byte_00 = m['byte_00']
        self.sample_indices = m['sample_indices']
        self.byte_05 = m['byte_05']
        self.byte_06 = m['byte_06']
        self.byte_07 = m['byte_07']
        self.vibrato_delay = m['vibrato_delay']
        self.vibrato_depth = m['vibrato_depth']
        self.vibrato_speed = m['vibrato_speed']
        self.byte_0b = m['byte_0b']
        self.unknown12 = bytes(m['unknown12'])
        self.row_speed = m['row_speed']
        self.ushort_12 = m['ushort_12']
        self.samples = [InstrumentSample(s) for s in m['samples']]
        self.rows = [InstrumentRow(r) for r in m['rows']]
        self.envelope = Envelope(m['envelope']) if m['envelope'] else None
        # patched by the linker:
        self.envelope_ptr = 0
        self.gax2_unknown_ptr = 0
        self.rows_ptr = 0

    def pack_header(self):
        out = Chunk()
        out.raw(bytes([self.byte_00]))
        out.raw(bytes(self.sample_indices))
        out.raw(bytes([self.byte_05, self.byte_06, self.byte_07,
                       self.vibrato_delay, self.vibrato_depth, self.vibrato_speed, self.byte_0b]))
        for s in self.samples:
            out.raw(s.pack())
        out.u32(self.envelope_ptr).u32(self.gax2_unknown_ptr)
        out.raw(bytes([self.row_speed, len(self.rows)]))
        out.raw(struct.pack('<H', self.ushort_12))
        return out.u32(self.rows_ptr)


# ---------------------------------------------------------------------------
# XM reader (patterns only - see NOTE in module docstring)
# ---------------------------------------------------------------------------

class XMRow:
    __slots__ = ('note', 'instrument', 'volume', 'effect', 'param')

    def __init__(self):
        self.note = 0
        self.instrument = 0
        self.volume = 0
        self.effect = 0
        self.param = 0


def _read_pattern_rows(buf, pos, num_rows, num_channels):
    rows = [[XMRow() for _ in range(num_channels)] for _ in range(num_rows)]
    for r in range(num_rows):
        for c in range(num_channels):
            cell = rows[r][c]
            first = buf[pos]
            if first & 0x80:
                pos += 1
                flags = first
                if flags & 0x01:
                    cell.note = buf[pos]; pos += 1
                if flags & 0x02:
                    cell.instrument = buf[pos]; pos += 1
                if flags & 0x04:
                    cell.volume = buf[pos]; pos += 1
                if flags & 0x08:
                    cell.effect = buf[pos]; pos += 1
                if flags & 0x10:
                    cell.param = buf[pos]; pos += 1
            else:
                cell.note, cell.instrument, cell.volume, cell.effect, cell.param = buf[pos:pos + 5]
                pos += 5
    return rows, pos


def read_xm(path):
    buf = open(path, 'rb').read()
    if buf[:17] != b'Extended Module: ':
        raise ValueError(f"{path}: not an XM file")
    name = buf[17:37].split(b'\x00')[0].decode('latin-1').strip()
    header_size = struct.unpack_from('<I', buf, 60)[0]
    song_length, restart = struct.unpack_from('<HH', buf, 64)
    num_channels = struct.unpack_from('<H', buf, 68)[0]
    num_patterns = struct.unpack_from('<H', buf, 70)[0]
    tempo, bpm = struct.unpack_from('<HH', buf, 76)
    order = list(buf[80:80 + song_length])

    pos = 60 + header_size
    patterns = []
    for _ in range(num_patterns):
        phdr_len = struct.unpack_from('<I', buf, pos)[0]
        num_rows = struct.unpack_from('<H', buf, pos + 5)[0]
        packed_size = struct.unpack_from('<H', buf, pos + 7)[0]
        data_start = pos + phdr_len
        if packed_size == 0:
            rows = [[XMRow() for _ in range(num_channels)] for _ in range(num_rows)]
        else:
            rows, _ = _read_pattern_rows(buf, data_start, num_rows, num_channels)
        patterns.append(rows)
        pos = data_start + packed_size

    num_instruments = struct.unpack_from('<H', buf, 72)[0]
    gax_instrument_indices = []
    for _ in range(num_instruments):
        inst_start = pos
        inst_size = struct.unpack_from('<I', buf, pos)[0]
        name = buf[pos + 4:pos + 4 + 22].split(b'\x00')[0].decode('ascii', errors='replace')
        gax_instrument_indices.append(int(name[len('Instrument '):]) if name.startswith('Instrument ') else None)
        num_samples = struct.unpack_from('<H', buf, pos + 27)[0]
        pos = inst_start + inst_size
        sample_lengths = []
        for _ in range(num_samples):
            sample_lengths.append(struct.unpack_from('<I', buf, pos)[0])
            pos += 40
        pos += sum(sample_lengths)

    return {'name': name, 'num_channels': num_channels, 'patterns': patterns,
            'order': order, 'restart': restart, 'tempo': tempo, 'bpm': bpm,
            'gax_instrument_indices': gax_instrument_indices}


# ---------------------------------------------------------------------------
# XM -> GAX pattern import (see tools/gax_audio.py history for how this
# mapping was derived and verified against every song in the original ROM)
# ---------------------------------------------------------------------------

def _xm_effect_to_gax(effect, param, volume):
    if effect in (1, 2, 3):
        return effect, param
    if effect == 12:
        return 12, min(255, param * 4)
    if effect == 13:
        return 13, param
    if effect == 14:
        return 14, param
    if effect == 15 and param < 0x20:
        return 15, param
    if volume:
        if 0x90 <= volume <= 0x9F:
            return 10, (volume - 0x90) << 4
        if 0x80 <= volume <= 0x8F:
            return 11, volume - 0x80
        if 0x10 <= volume <= 0x50:
            return 12, min(255, (volume - 0x10) * 4)
    return 0, 0


def _compress_rests(rows):
    out = []
    i = 0
    while i < len(rows):
        if rows[i].cmd == PatternRow.CMD_REST:
            j = i
            while j < len(rows) and rows[j].cmd == PatternRow.CMD_REST and (j - i) < 255:
                j += 1
            run_len = j - i
            if run_len >= 2:
                merged = PatternRow()
                merged.cmd = PatternRow.CMD_REST_MULTI
                merged.rest_duration = run_len
                out.append(merged)
            else:
                out.append(rows[i])
            i = j
        else:
            out.append(rows[i])
            i += 1
    return out


def build_pattern_from_xm(xm_rows, used_indices, instruments_by_index, transpose):
    pat = Pattern()
    all_empty = all(r.note == 0 and r.instrument == 0 and r.volume == 0 and r.effect == 0 for r in xm_rows)
    if all_empty:
        pat.is_empty = True
        return pat

    rows = []
    for xr in xm_rows:
        gax_effect, gax_param = _xm_effect_to_gax(xr.effect, xr.param, xr.volume)

        if xr.note == 97:
            row = PatternRow()
            row.note = 1
            row.instrument = 0
            if gax_effect == 0:
                row.cmd = PatternRow.CMD_NOTE_ONLY
            else:
                row.cmd = PatternRow.CMD_NOTE
                row.effect = gax_effect
                row.effect_param = gax_param
            rows.append(row)
            continue

        if xr.note == 0 and xr.instrument == 0:
            row = PatternRow()
            if gax_effect != 0:
                row.cmd = PatternRow.CMD_EFFECT_ONLY
                row.effect = gax_effect
                row.effect_param = gax_param
            else:
                row.cmd = PatternRow.CMD_REST
            rows.append(row)
            continue

        gax_idx = used_indices[xr.instrument - 1] if xr.instrument else (rows[-1].instrument if rows else used_indices[0])
        ins = instruments_by_index.get(gax_idx)
        if ins is not None and ins.rows and ins.rows[0].dont_use_note_pitch:
            note_val = ins.rows[0].relative_note_number
        elif xr.note:
            note_val = (xr.note - transpose) & 0x7F
        else:
            note_val = 0

        row = PatternRow()
        row.note = note_val
        row.instrument = gax_idx
        if gax_effect == 0:
            # 2-byte encoding when there's no effect to carry (matches how
            # the original data prefers NoteOnly whenever possible)
            row.cmd = PatternRow.CMD_NOTE_ONLY
        else:
            row.cmd = PatternRow.CMD_NOTE
            row.effect = gax_effect
            row.effect_param = gax_param
        rows.append(row)

    pat.is_empty = False
    pat.rows = _compress_rests(rows)
    return pat


# ---------------------------------------------------------------------------
# Linker
# ---------------------------------------------------------------------------

class Allocator:
    """One block, laid out from its label `symbol`: its bytes and the
    relocations of its pointers, {offset: Ptr}."""

    def __init__(self, symbol, prefix=None):
        self.symbol = symbol
        self.buf = bytearray()
        self.relocs = {}
        self._dedup = {}
        if prefix is not None:
            self.alloc(prefix)

    def addr_here(self):
        return Ptr(self.symbol, len(self.buf))

    def u32(self, v):
        """Appends one word, a pointer or a number."""
        self.alloc(Chunk().u32(v))

    def align(self, n):
        """Zero padding up to an n-byte boundary (the block starts aligned)."""
        self.alloc(bytes(-len(self.buf) % n))

    def alloc(self, data, dedup=None):
        """dedup: None for no sharing, True for the allocator-wide (cross-song)
        dedup table, or a caller-supplied dict to scope sharing narrowly
        (e.g. per-song, matching what the original data actually does for
        patterns - see NOTE in link())."""
        if not isinstance(data, Chunk):
            data = Chunk(data)
        table = self._dedup if dedup is True else dedup
        key = data.key()
        if table is not None and key in table:
            return table[key]
        addr = self.addr_here()
        for off, ptr in data.relocs:
            self.relocs[len(self.buf) + off] = ptr
        self.buf += data.buf
        if table is not None:
            table[key] = addr
        return addr

    def write(self, bin_path, asm_path, start=0, end=None):
        """Writes buf[start:end]: the bytes to bin_path (pointers 0), and
        to asm_path the assembly that data/data.s includes, the bytes as
        `.incbin` ranges of bin_path with a `.4byte` relocation per
        pointer."""
        end = len(self.buf) if end is None else end
        with open(bin_path, 'wb') as f:
            f.write(self.buf[start:end])
        lines = [f'@ Generated by tools/gax_audio.py: {os.path.basename(bin_path)} with its',
                 '@ pointers as relocations. Do not edit.']
        pos = start
        for off in sorted(o for o in self.relocs if start <= o < end):
            if off > pos:
                lines.append(f'\t.incbin "{bin_path}", {pos - start:#x}, {off - pos:#x}')
            lines.append(f'\t.4byte {self.relocs[off]!r}')
            pos = off + 4
        if end > pos:
            lines.append(f'\t.incbin "{bin_path}", {pos - start:#x}, {end - pos:#x}')
        with open(asm_path, 'w') as f:
            f.write('\n'.join(lines) + '\n')
        return end - start


class Channel:
    def __init__(self):
        self.pattern_headers = []  # list of (transpose, reserved)
        self.patterns = []
        self.pattern_groups = []  # group id per pattern (see NOTE in link())


class Song:
    def __init__(self, manifest_entry, num_items=7, unk_ptr=0):
        self.info = SongInfo(manifest_entry)
        self.mixer_data_bytes = bytes(manifest_entry['mixer_data_bytes'])
        self.title = manifest_entry['title']
        self.artist = manifest_entry['artist']
        # the ROM's songs keep their title bytes as recorded (padding
        # included); a new song's are written from title and artist
        self.title_bytes = base64.b64decode(manifest_entry['title_bytes']) if 'title_bytes' in manifest_entry else None
        self.num_items = num_items
        self.unk_ptr = unk_ptr
        self.channels = []
        self.info_handler = SoundHandler(*HANDLER_FUNCS['info'], type_flags=0x1C)
        self.mixer_handler = SoundHandler(*HANDLER_FUNCS['mixer'], type_flags=0xC)
        self.channel_handlers = []
        self.pattern_group_bytes = {}  # gid -> original raw packed bytes
        # the default song's info has no instrument set and points its
        # sample set at its own sequence data
        self.no_pool = manifest_entry.get('no_pool', False)


def link_instruments(alloc, instruments_by_index, samples_by_index, dedup_samples=True):
    """The instrument and sample pool both data sets start with: per
    instrument its envelope, 12-byte unknown block, rows and header; the
    instrument pointer table; the samples; zero padding up to a word; the
    sample table. The music set stores identical samples once, the
    sound-effect set doesn't (its samples 84 and 87 are the same bytes,
    stored twice). Returns (instrument table address, sample table
    address)."""
    n_instr = max(instruments_by_index.keys()) + 1
    instr_addrs = {}
    for idx in range(n_instr):
        ins = instruments_by_index[idx]
        ins.envelope_ptr = alloc.alloc(ins.envelope.pack()) if ins.envelope is not None else 0
        ins.gax2_unknown_ptr = alloc.alloc(ins.unknown12)
        ins.rows_ptr = alloc.alloc(b"".join(r.pack() for r in ins.rows)) if ins.rows else 0
        instr_addrs[idx] = alloc.alloc(ins.pack_header())

    instrument_set_addr = alloc.addr_here()
    for idx in range(n_instr):
        alloc.u32(instr_addrs[idx])

    # Sample data and its pointer table come next (before the songs), matching
    # the original layout - not just instrument data/table, then songs, then
    # samples, as might be guessed.
    n_samples = max(samples_by_index.keys()) + 1 if samples_by_index else 0
    sample_addrs = {}
    for idx in range(n_samples):
        s = samples_by_index.get(idx)
        sample_addrs[idx] = alloc.alloc(s, dedup=dedup_samples or None) if s else 0

    # zero padding before the sample table, up to a word boundary (2 bytes
    # in the music set, 3 in the sound-effect set)
    alloc.align(4)

    sample_table_addr = alloc.addr_here()
    for idx in range(n_samples):
        s = samples_by_index.get(idx)
        alloc.u32(sample_addrs[idx])
        alloc.u32(len(s) if s else 0)
    return instrument_set_addr, sample_table_addr


def link(instruments_by_index, songs, samples_by_index, header_prefix):
    alloc = Allocator(MUSIC_SYMBOL, header_prefix)

    instrument_set_addr, sample_table_addr = link_instruments(alloc, instruments_by_index, samples_by_index)

    sample_table_end = alloc.addr_here()
    alloc.alloc(b"\x00\x00\x00\x00")  # 4 bytes of fixed padding before the first song

    for song in songs:
        song.unk_ptr = sample_table_end  # every song's 3rd pointer is this same value
        info = song.info
        info.instrument_set_ptr = instrument_set_addr
        info.sample_set_ptr = sample_table_addr

        # NOTE: the original data pointer-shares some patterns (e.g. a repeated
        # silence/fill reused across several positions), but this is a
        # deliberate authoring choice, not something recoverable from pattern
        # bytes alone - two independently-written patterns can be byte-identical
        # by coincidence without being "the same" pattern. So sharing is driven
        # by pattern_groups (captured once from the original ROM's actual
        # pointer graph, not re-derived by content matching): each group's
        # first member is built from its own XM data and allocated; every
        # other member of the group reuses that address unconditionally,
        # exactly like the original hardware does (they physically share
        # memory, so editing one edits all of them - not a bug).
        #
        # Within a group, if the XM-derived notes/instruments are unchanged
        # from what the original recorded bytes decode to, use those original
        # bytes verbatim (byte-for-byte, including exotic effect encodings
        # the XM round-trip can't represent) instead of the freshly-packed
        # version - this is what makes an unedited song rebuild identical to
        # the original ROM. Any actual edit is detected by content and falls
        # back to the freshly-encoded bytes.
        group_packed = {}
        for ch in song.channels:
            for i, p in enumerate(ch.patterns):
                gid = ch.pattern_groups[i]
                if gid in group_packed:
                    continue
                original = song.pattern_group_bytes.get(gid)
                if original is not None and (
                    _note_instrument_timeline(unpack_pattern(original), instruments_by_index)
                    == _note_instrument_timeline(p, instruments_by_index)
                ):
                    group_packed[gid] = original
                else:
                    group_packed[gid] = p.pack()

        # Groups must be allocated in the order they physically appear in the
        # original data (captured once from the ROM), not in channel/pattern
        # traversal order - the two don't match (a channel's "first" pattern
        # isn't necessarily the first one physically in memory).
        group_addr = {}
        for gid in song.pattern_group_order:
            group_addr[gid] = alloc.alloc(group_packed[gid])

        pattern_addrs_by_channel = [
            [group_addr[gid] for gid in ch.pattern_groups] for ch in song.channels
        ]

        anchor = min(a for addrs in pattern_addrs_by_channel for a in addrs)
        info.sequence_data_ptr = anchor
        if song.no_pool:
            info.instrument_set_ptr = 0
            info.sample_set_ptr = anchor

        # A `"Title" (c) Artist` metadata string (with its original padding)
        # sits right after the song's patterns and before its pattern-header
        # tables - captured verbatim in the manifest since it never changes.
        # A new song's is padded the way the ROM's are: zeros up to a word,
        # no terminator if it ends on one (n gin's).
        if song.title_bytes is not None:
            alloc.alloc(song.title_bytes)
        else:
            alloc.alloc(title_text(song.title, song.artist))
            alloc.align(4)

        # Layout order from here on matches what's observed in the original
        # data: all pattern-header tables grouped together, then SongInfo +
        # its handler, then a single shared 1-entry "children" array (every
        # channel handler's Children[0] points back to the info handler, and
        # they all share this same one allocation), then the channel
        # handlers themselves, then the mixer bits, then the song struct.
        # Channels aren't necessarily laid out in logical index order in
        # memory (e.g. channel 3's data physically precedes channel 0's) -
        # channel_order (captured from the ROM) gives the real allocation
        # sequence. The channel-pointer arrays elsewhere still list them by
        # logical index; only the physical allocation order changes here.
        headers_addrs = [None] * len(song.channels)
        for ci in song.channel_order:
            ch = song.channels[ci]
            hdr_bytes = bytearray()
            for i, p in enumerate(ch.patterns):
                seq_off = pattern_addrs_by_channel[ci][i] - anchor
                if seq_off > 0xFFFF:
                    sys.exit(f"{song.title}: over 64 KiB of patterns (GaxOrderEntry.patternOffset is 16 bits)")
                transpose, reserved = ch.pattern_headers[i]
                hdr_bytes += struct.pack('<Hbb', seq_off, transpose, reserved)
            headers_addrs[ci] = alloc.alloc(bytes(hdr_bytes))

        info_data_addr = alloc.alloc(info.pack())
        song.info_handler.data_ptr = info_data_addr
        info_handler_addr = alloc.alloc(song.info_handler.pack())

        channel_children_addr = alloc.alloc(Chunk().u32(info_handler_addr))

        channel_handler_addrs = [None] * len(song.channels)
        for ci in song.channel_order:
            ch = song.channels[ci]
            chh = song.channel_handlers[ci]
            chh.data_ptr = headers_addrs[ci]
            chh.num_children = 1
            chh.children_ptr = channel_children_addr
            channel_handler_addrs[ci] = alloc.alloc(chh.pack())

        children_ptr = alloc.addr_here()
        for a in channel_handler_addrs:
            alloc.u32(a)
        mixer_data_addr = alloc.alloc(song.mixer_data_bytes)
        song.mixer_handler.data_ptr = mixer_data_addr
        song.mixer_handler.children_ptr = children_ptr
        song.mixer_handler.num_children = len(channel_handler_addrs)
        mixer_handler_addr = alloc.alloc(song.mixer_handler.pack())

        song_bytes = Chunk().u32(song.num_items)
        song_bytes.u32(mixer_handler_addr).u32(info_handler_addr).u32(song.unk_ptr)
        for a in channel_handler_addrs:
            song_bytes.u32(a)
        song.new_offset = alloc.alloc(song_bytes)

    return alloc


def link_sfx(instruments_by_index, samples_by_index, song_info):
    """The sound-effect set: the same instrument/sample pool as the music,
    then the one handler type every sound-effect voice uses. Its type data
    is a song header without patterns (only the volume and the two tables
    matter), and its one child type is NULL. Returns (the set's Allocator,
    address of the handler type)."""
    alloc = Allocator(SFX_SYMBOL)
    song_info.instrument_set_ptr, song_info.sample_set_ptr = link_instruments(
        alloc, instruments_by_index, samples_by_index, dedup_samples=False)
    info_addr = alloc.alloc(song_info.pack())
    children_addr = alloc.alloc(Chunk().u32(0))
    handler = SoundHandler(*HANDLER_FUNCS['sfx'], num_children=1, children_ptr=children_addr,
                           type_flags=0x48, data_ptr=info_addr)
    handler_addr = alloc.alloc(handler.pack())
    return alloc, handler_addr


# ---------------------------------------------------------------------------
# Top-level build
# ---------------------------------------------------------------------------

def load_sample_wav(path):
    with wave.open(path, 'rb') as w:
        frames = w.readframes(w.getnframes())
    # stored as unsigned 8-bit in the WAV; GAX v2 wants signed 8-bit
    return bytes(((b - 128) & 0xFF) for b in frames)


def load_pool(manifest, samples_dir):
    instruments = {int(k): Instrument(v) for k, v in manifest['instruments'].items()}
    samples = {}
    for k in manifest['samples']:
        idx = int(k)
        wav_path = os.path.join(samples_dir, f"{idx:02d}.wav")
        if os.path.exists(wav_path):
            samples[idx] = load_sample_wav(wav_path)
    return instruments, samples


def build_sfx(manifest_path, samples_dir):
    """The sound-effect set (sound/gax_sfx_manifest.json, sound/sfx_samples/)
    and the handler-type array the music block starts with.

    Returns (the set's Allocator, the sfxTypes Chunk). Every pointer is a
    relocation, so the set can change size (a longer sample) and the
    blocks can move."""
    manifest = json.load(open(manifest_path))
    instruments, samples = load_pool(manifest, samples_dir)
    alloc, handler_addr = link_sfx(instruments, samples, SongInfo(manifest['song_info']))
    # GaxSongHeader.sfxTypes (StartSong): the handler type of each
    # sound-effect voice, all the same one
    sfx_types = Chunk()
    for _ in range(manifest['voice_types']):
        sfx_types.u32(handler_addr)
    return alloc, sfx_types


def default_song(m):
    """The engine's default song (GaxSongHeader-less playback uses it as the
    handler layout, gGaxDefaultSong - see GAX2_init): one channel,
    one empty pattern, no instruments. Linked after the 19 songs, the same
    way."""
    song = Song(m, num_items=m['num_items'], unk_ptr=0)
    song.pattern_group_order = [0]
    song.channel_order = [0]
    ch = Channel()
    pat = Pattern()
    pat.is_empty = True
    ch.patterns.append(pat)
    ch.pattern_groups.append(0)
    ch.pattern_headers.append((0, 0))
    song.channels.append(ch)
    song.channel_handlers.append(SoundHandler(*HANDLER_FUNCS['channel'], type_flags=0x48))
    return song


def c_name(key):
    return 'GAX_SONG_' + key.upper()


def write_header(path, songs_by_key):
    """#defines of each song's offset in the music block, for the song
    table (src/engine/audio/data/song_table.cpp)."""
    out = ['/* Generated by tools/gax_audio.py: the offset of each song in the',
           ' * music block gGaxMusicData (gax_audio_data.bin). */',
           '#ifndef GUARD_GAX_SONGS_H', '#define GUARD_GAX_SONGS_H', '']
    for key, song in songs_by_key:
        out.append(f'#define {c_name(key)} {song.new_offset.off:#x}')
    out += ['', '#endif', '']
    with open(path, 'w') as f:
        f.write('\n'.join(out))


def check_song_table(manifest, manifest_path):
    """song_table lists every song once: each of the ROM's (the manifest's
    `songs`) and the new ones, songs with an XM in sound/songs/ but no
    recorded scaffold."""
    rel = os.path.relpath(manifest_path, ROOT)
    keys = [t['song'] for t in manifest['song_table']]
    twice = sorted({k for k in keys if keys.count(k) > 1})
    if twice:
        sys.exit(f"{rel}: song_table lists {', '.join(twice)} more than once")
    missing = [k for k in manifest['songs'] if k not in keys]
    if missing:
        sys.exit(f"{rel}: song_table doesn't list {', '.join(missing)}")
    for k in keys:
        if k not in manifest['songs'] and not os.path.exists(os.path.join(SOUND_DIR, 'songs', f'{k}.xm')):
            sys.exit(f"{rel}: song_table's {k} is neither a song of the ROM nor sound/songs/{k}.xm")


def write_constants(manifest_path, path):
    """constants/songs.h: SONG_<KEY> for each song, in gSongTable order."""
    manifest = json.load(open(manifest_path))
    check_song_table(manifest, manifest_path)
    table = manifest['song_table']
    rows = [('SONG_' + t['song'].upper(), f'{i:#x}', t['comment']) for i, t in enumerate(table)]
    width = max(len(n) + len(v) for n, v, _ in rows)
    rel = os.path.relpath(manifest_path, ROOT)
    out = [
        f'/* Generated by tools/gax_audio.py from {rel}. Do not edit. */',
        '#ifndef GUARD_CONSTANTS_SONGS_H',
        '#define GUARD_CONSTANTS_SONGS_H',
        '',
        '/*',
        ' * Song IDs: the index into gSongTable (src/engine/audio/data/song_table.cpp)',
        ' * that PlaySong/StartSong take, in the order of the manifest\'s',
        ' * song_table. Each name is the song\'s key in sound/gax_manifest.json',
        ' * (the title the ROM stores with the song, e.g. "jungle (c) Manfred',
        ' * Linzner"; a new song\'s is its XM\'s, sound/songs/<key>.xm), and',
        ' * gSongTable pairs every SONG_<NAME> with the generated',
        " * GAX_SONG_<NAME> offset, so the two can't drift apart. The comments",
        ' * say where the game plays each one.',
        ' */',
        '',
    ]
    out += [f'#define {n} {v}{" " * (width - len(n) - len(v) + 1)}// {c}' for n, v, c in rows]
    out += [
        '',
        f'#define SONG_COUNT {len(table)}',
        f'#define SONG_NONE {len(table):#x} // {manifest["no_song_comment"]}',
        '',
        '#endif /* GUARD_CONSTANTS_SONGS_H */',
        '',
    ]
    with open(path, 'w') as f:
        f.write('\n'.join(out))


def write_song_table(manifest_path, path):
    """song_table.inc: gSongTable, which src/engine/audio/data/song_table.cpp
    #includes. One entry per song of the manifest's song_table, in song id
    order: the song's GAX_SONG_<KEY> offset in the music block (gax_songs.h).
    Generated with constants/songs.h's ids, so no song is left NULL."""
    manifest = json.load(open(manifest_path))
    check_song_table(manifest, manifest_path)
    rel = os.path.relpath(manifest_path, ROOT)
    out = [
        f'/* Generated by tools/gax_audio.py from {rel}\'s song_table. Do not edit. */',
        '',
        'const void *const gSongTable[SONG_COUNT] = {',
    ]
    out += [f'    /* SONG_{t["song"].upper()} */ {MUSIC_SYMBOL} + {c_name(t["song"])},'
            for t in manifest['song_table']]
    out += ['};', '']
    with open(path, 'w') as f:
        f.write('\n'.join(out))


def rom_song(sm, xm, instruments):
    """A song of the ROM: its patterns from the XM, laid out with the
    scaffold the manifest recorded (pattern sharing and order, channel
    order, transposes, title bytes), so it rebuilds byte for byte."""
    song = Song(sm, num_items=sm['num_items'], unk_ptr=0)
    used_indices = xm['gax_instrument_indices']

    song.pattern_group_bytes = {
        int(gid): base64.b64decode(b) for gid, b in sm.get('pattern_group_bytes', {}).items()
    }
    song.pattern_group_order = sm['pattern_group_order']
    song.channel_order = sm['channel_order']

    transposes = sm['channel_transposes']
    pattern_groups = sm['pattern_groups']
    for ci in range(sm['num_channels']):
        ch = Channel()
        ch_transposes = transposes[ci]
        ch.pattern_groups = pattern_groups[ci]
        for pi in range(sm['num_patterns_per_channel']):
            transpose = ch_transposes[pi]
            xm_rows_for_channel = [row[ci] for row in xm['patterns'][pi]]
            pat = build_pattern_from_xm(xm_rows_for_channel, used_indices, instruments, transpose)
            ch.patterns.append(pat)
            ch.pattern_headers.append((transpose, 0))
        song.channels.append(ch)
        song.channel_handlers.append(SoundHandler(*HANDLER_FUNCS['channel'], type_flags=0x48))
    return song


# A new song's settings (sound/songs/<key>.json, every field optional):
# its title and artist (the `"title" (c) artist` string stored with it) and
# its GaxSongData fields. The defaults are the ROM's level songs': volume
# 1.0, the 15769 Hz mix rate, 3 sound-effect voices mixed at half that
# rate, and no echo (mixer_data_bytes, the mixer's 28 bytes of echo taps,
# all 0 as in jungle's).
NEW_SONG_DEFAULTS = {
    'title': None,  # the XM's module name, or the key
    'artist': None,  # none: the string is just `"title"`
    'volume': 256,
    'sample_rate': 15769,
    'num_fx_channels': 3,
    'half_rate_fx': 1,
    'mixer_data_bytes': [0] * 28,
}


def title_text(title, artist):
    """The string stored after a song's patterns: `"title" (c) artist`."""
    text = f'"{title}" \xa9 {artist}' if artist else f'"{title}"'
    return text.encode('latin-1')


def new_song(key, xm_path, xm, instruments):
    """A song with no ROM history: its scaffold derived from the XM, with
    the settings of the optional sound/songs/<key>.json next to it.

    - one GAX2 channel per XM channel, in order;
    - one order position per entry of the XM's order list (a channel's
      pattern there is its column of the XM pattern the position plays),
      looping back to the XM's restart position;
    - identical patterns stored once (a pattern the order repeats, or the
      same notes on two channels); no transposes.

    The XM's instruments name the shared pool's (`Instrument N`, as in the
    ROM's songs). Every pattern the order plays must have the same number
    of rows, and the module's speed must be 6, GAX2's starting speed
    (change it with an Fxx effect). The BPM is ignored: GAX2 steps a tick
    per frame."""
    where = os.path.relpath(xm_path, ROOT)

    def fail(msg):
        sys.exit(f"{where}: {msg}")

    settings = dict(NEW_SONG_DEFAULTS)
    json_path = os.path.splitext(xm_path)[0] + '.json'
    if os.path.exists(json_path):
        given = json.load(open(json_path))
        unknown = sorted(set(given) - set(NEW_SONG_DEFAULTS))
        if unknown:
            sys.exit(f"{os.path.relpath(json_path, ROOT)}: unknown settings {', '.join(unknown)} "
                     f"(a song's are {', '.join(NEW_SONG_DEFAULTS)})")
        settings.update(given)
    if len(settings['mixer_data_bytes']) != 28:
        fail("mixer_data_bytes must be 28 bytes")

    order = xm['order']
    if not order:
        fail("the order list is empty")
    if any(p >= len(xm['patterns']) for p in order):
        fail("the order list plays a pattern the module doesn't have")
    rows = {len(xm['patterns'][p]) for p in order}
    if len(rows) != 1:
        fail(f"the patterns the order plays have {sorted(rows)} rows: a GAX2 song has one pattern length")
    if xm['tempo'] != 6:
        fail(f"speed {xm['tempo']}: a GAX2 song starts at speed 6 (set another with an Fxx effect on the first row)")
    if xm['restart'] >= len(order):
        fail(f"restart position {xm['restart']} is past the order list")
    used = xm['gax_instrument_indices']
    for i, idx in enumerate(used):
        if idx not in instruments:
            fail(f"instrument {i + 1} must be named \"Instrument N\", N one of the {len(instruments)} "
                 "instruments of the pool (sound/gax_manifest.json)")
    for p in order:
        for row in xm['patterns'][p]:
            for cell in row:
                if cell.instrument > len(used):
                    fail(f"pattern {p} plays instrument {cell.instrument}, which the module doesn't have")

    num_channels = xm['num_channels']
    entry = {
        'num_channels': num_channels,
        'num_rows_per_pattern': rows.pop(),
        'num_patterns_per_channel': len(order),
        'loop_point': xm['restart'],
        'ushort_0a': 0,
        'title': settings['title'] or xm['name'] or key,
    }
    for field in ('artist', 'volume', 'sample_rate', 'num_fx_channels', 'half_rate_fx', 'mixer_data_bytes'):
        entry[field] = settings[field]
    song = Song(entry, num_items=3 + num_channels, unk_ptr=0)

    groups = {}  # packed pattern -> its group
    for ci in range(num_channels):
        ch = Channel()
        for p in order:
            pat = build_pattern_from_xm([row[ci] for row in xm['patterns'][p]], used, instruments, 0)
            ch.patterns.append(pat)
            ch.pattern_headers.append((0, 0))
            ch.pattern_groups.append(groups.setdefault(pat.pack(), len(groups)))
        song.channels.append(ch)
        song.channel_handlers.append(SoundHandler(*HANDLER_FUNCS['channel'], type_flags=0x48))
    song.pattern_group_order = list(range(len(groups)))
    song.channel_order = list(range(num_channels))
    return song


def block_songs(manifest):
    """The songs' keys in the music block's order: the ROM's
    (`song_order`), then the songs of `song_table` that aren't in it (new
    ones), in song id order."""
    keys = list(manifest['song_order'])
    return keys + [t['song'] for t in manifest['song_table'] if t['song'] not in keys]


def build(manifest_path, songs_dir, samples_dir, out_path, layout_path=None, header_path=None):
    manifest = json.load(open(manifest_path))

    instruments, samples = load_pool(manifest, samples_dir)

    keys = block_songs(manifest)
    songs = []
    for key in keys:
        xm_path = os.path.join(songs_dir, f"{key}.xm")
        xm = read_xm(xm_path)
        if key in manifest['songs']:
            songs.append(rom_song(manifest['songs'][key], xm, instruments))
        else:
            songs.append(new_song(key, xm_path, xm, instruments))

    default = default_song(manifest['default_song'])
    _, header_prefix = build_sfx(SFX_MANIFEST, SFX_SAMPLES_DIR)

    alloc = link(instruments, songs + [default], samples, header_prefix)
    # The default song's own song struct is the layout the code references
    # as gGaxDefaultSong: a separate output, so it can have that label.
    # Its pointers are relocations against gGaxMusicData too, so data/data.s
    # must put it right after the block.
    split = default.new_offset.off
    n = alloc.write(out_path, asm_path(out_path), 0, split)
    print(f"wrote {out_path}: {n} bytes")
    if layout_path:
        n = alloc.write(layout_path, asm_path(layout_path), split)
        print(f"wrote {layout_path}: {n} bytes")
    if header_path:
        write_header(header_path, list(zip(keys, songs)))
        print(f"wrote {header_path}")


def asm_path(bin_path):
    """OUTPUT.s, the assembly written next to OUTPUT.bin."""
    return os.path.splitext(bin_path)[0] + '.s'


SFX_MANIFEST = os.path.join(SOUND_DIR, 'gax_sfx_manifest.json')
SFX_SAMPLES_DIR = os.path.join(SOUND_DIR, 'sfx_samples')

if __name__ == '__main__':
    if len(sys.argv) == 3 and sys.argv[1] == '--sfx':
        alloc, _ = build_sfx(SFX_MANIFEST, SFX_SAMPLES_DIR)
        n = alloc.write(sys.argv[2], asm_path(sys.argv[2]))
        print(f"wrote {sys.argv[2]}: {n} bytes")
        sys.exit(0)
    if len(sys.argv) == 3 and sys.argv[1] == '--constants':
        write_constants(os.path.join(SOUND_DIR, 'gax_manifest.json'), sys.argv[2])
        sys.exit(0)
    if len(sys.argv) == 3 and sys.argv[1] == '--table':
        write_song_table(os.path.join(SOUND_DIR, 'gax_manifest.json'), sys.argv[2])
        sys.exit(0)
    if len(sys.argv) not in (2, 4):
        print(f"Usage: {sys.argv[0]} OUTPUT.bin [LAYOUT.bin SONGS.h]  (music block, default layout, song offsets)")
        print(f"       {sys.argv[0]} --sfx OUTPUT.bin                (sound-effect set)")
        print("       (each OUTPUT.bin comes with OUTPUT.s: its bytes, the pointers as relocations)")
        print(f"       {sys.argv[0]} --constants SONGS.h             (constants/songs.h: the song IDs)")
        print(f"       {sys.argv[0]} --table SONG_TABLE.inc          (gSongTable, in song id order)")
        sys.exit(1)
    build(
        os.path.join(SOUND_DIR, 'gax_manifest.json'),
        os.path.join(SOUND_DIR, 'songs'),
        os.path.join(SOUND_DIR, 'samples'),
        sys.argv[1],
        *sys.argv[2:],
    )
