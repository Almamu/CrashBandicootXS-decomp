#!/usr/bin/env python3
"""
Builds build/crashbandicootxs/sound/gax_audio_data.bin (the Shin'en GAX2 sound-engine data block used
by this game: shared instrument/sample pool + all 19 songs) from editable
sources:

  games/xs/sound/gax_manifest.json  - instrument definitions (envelopes, vibrato, rows),
                              per-song scaffold fields, and the raw bytes for a
                              small pre-existing shared structure this data
                              happens to sit next to (see NOTE below).
  games/xs/sound/songs/*.xm          - one FastTracker II module per song. Patterns,
                              notes and instrument assignments are read from
                              here - this is what a modder edits to change a
                              song.
  games/xs/sound/songs/<key>.json    - a new song's settings (optional): a song of
                              the manifest's song_table with no recorded
                              scaffold has it derived from its XM
                              (new_song, docs/audio.md "Adding a song").
  games/xs/sound/samples/*.wav       - one 8-bit mono PCM sample per instrument voice
                              (shared across songs) - this is what a modder
                              edits to change a sound.
  games/xs/sound/instruments/*.json  - new instruments, each with a WAV under games/xs/sound/,
                              for new songs and sound effects; a new song's
                              XM can also bring its own (docs/audio.md, "New
                              instruments"). They are appended to the pool.

It also builds the GAX2 sound-effect set that sits right before the music
block (`--sfx`, see docs/audio.md) from

  games/xs/sound/gax_sfx_manifest.json - its 88 instruments (the same record types as
                              the music's), the handler type's song fields
                              and how many voice-type pointers to emit.
  games/xs/sound/sfx_samples/*.wav     - its 87 samples (8-bit mono PCM).
  games/xs/sound/sfx_table.json        - the new instruments its new sound effects
                              name, appended to the set.

The music block starts with the 36-byte GaxSongHeader.sfxTypes array
(StartSong passes gGaxMusicData there): nine pointers to the
sound-effect set's one handler type, generated from that set's layout.

Each block is written twice: OUTPUT.bin, its bytes with every pointer
left 0, and OUTPUT.s, which games/xs/data/data.s includes: the .bin's bytes
(`.incbin`) with each pointer as a relocation (`.4byte gGaxMusicData +
0x1234`, `.4byte GaxInfoInit`), so the blocks can sit anywhere in the
ROM and change size (#967, docs/shifting.md). Each block must start
word-aligned; games/xs/data/data.s aligns them.

The block ends with a 20th, silent song: the engine's default handler
layout (one channel, one empty pattern, no instruments; manifest
`default_song`). Its song struct is written to a separate file (LAYOUT.bin),
which games/xs/data/data.s incbins as gGaxDefaultSong, the label the code uses.
SONGS.h #defines each song's offset in the block for the song table
(engine/src/engine/audio/data/song_table.cpp), so songs can change size.

The song IDs (the gSongTable index) come from the manifest's `song_table`
list, in table order, each with a comment on where the game plays it, and
`no_song_comment` documents SONG_NONE (one past the table).
`--constants SONGS.h` writes them as the SONG_* #defines of
constants/songs.h (build/include), with the song each screen plays
(games/xs/sound/music.json's screen_music) as SCREEN_SONG_<SCREEN>, and `--table SONG_TABLE.inc` gSongTable
in the same order (the song table song_table.cpp #includes); both read only
the manifest.

This tool does not need baserom.gba - the manifest captured everything
required when it was generated (a one-time step; see the project history for
how gax_manifest.json/songs/samples were produced from the original ROM).
With unedited sources, the output reproduces the original ROM's audio block
byte-for-byte (verified during development).
"""
import base64
import copy
import json
import math
import os
import struct
import sys
import wave

from crashdata.common.game import ROOT
from crashdata import schemas

SOUND_DIR = os.path.join(ROOT, 'games', 'xs', 'sound')

# The labels games/xs/data/data.s gives the two blocks: every pointer into a block
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


def read_xm(path, buf=None):
    """An XM's module: its patterns, order list and instruments. buf: its
    bytes, when they aren't the file's (path is then only its name)."""
    if buf is None:
        with open(path, 'rb') as f:
            buf = f.read()
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
    instruments = []
    for _ in range(num_instruments):
        inst_start = pos
        inst_size = struct.unpack_from('<I', buf, pos)[0]
        iname = buf[pos + 4:pos + 4 + 22].split(b'\x00')[0].decode('ascii', errors='replace')
        number = iname[len('Instrument '):] if iname.startswith('Instrument ') else ''
        gax_instrument_indices.append(int(number) if number.isdigit() else None)
        num_samples = struct.unpack_from('<H', buf, pos + 27)[0]
        xi = {'name': iname, 'samples': []}
        if num_samples and inst_size >= 241:
            sample_header_size = struct.unpack_from('<I', buf, pos + 29)[0]
            xi['keymap'] = list(buf[pos + 33:pos + 129])
            for env, at, count_at in (('vol', 129, 225), ('pan', 177, 226)):
                n = buf[pos + count_at]
                xi[env + '_points'] = [struct.unpack_from('<HH', buf, pos + at + 4 * k) for k in range(min(n, 12))]
                xi[env + '_count'] = n
            (xi['vol_sustain'], xi['vol_loop_start'], xi['vol_loop_end'], _, _, _,
             xi['vol_type'], xi['pan_type'], xi['vib_type'], xi['vib_sweep'], xi['vib_depth'],
             xi['vib_rate']) = buf[pos + 227:pos + 239]
            xi['fadeout'] = struct.unpack_from('<H', buf, pos + 239)[0]
        else:
            sample_header_size = 40
        pos = inst_start + inst_size
        for _ in range(num_samples):
            length, loop_start, loop_len, volume, finetune, stype, panning, relnote, packing = \
                struct.unpack_from('<IIIBbBBbB', buf, pos)
            xi['samples'].append({'length': length, 'loop_start': loop_start, 'loop_len': loop_len,
                                  'volume': volume, 'finetune': finetune, 'type': stype, 'panning': panning,
                                  'relnote': relnote, 'packing': packing})
            pos += sample_header_size
        for xs in xi['samples']:
            xs['data'] = buf[pos:pos + xs['length']]
            pos += xs['length']
        instruments.append(xi)

    return {'name': name, 'num_channels': num_channels, 'patterns': patterns,
            'order': order, 'restart': restart, 'tempo': tempo, 'bpm': bpm,
            'gax_instrument_indices': gax_instrument_indices, 'instruments': instruments}


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
        to asm_path the assembly that games/xs/data/data.s includes, the bytes as
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


def pack_groups(song, instruments_by_index):
    """Each pattern group's packed bytes, {group: bytes}: the group's first
    member built from its XM column, or the ROM's recorded bytes when its
    notes and instruments are those (see NOTE in link())."""
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
    return group_packed


def song_sequence(song, instruments_by_index):
    """A song's patterns as link() lays them out: (the packed pattern
    streams, each group once in `pattern_group_order`; for each channel its
    order list, [(the pattern's offset in the streams, transpose)]), which
    is GaxSongData.patterns and each channel handler's GaxOrderEntry list."""
    group_packed = pack_groups(song, instruments_by_index)
    streams, offsets = bytearray(), {}
    for gid in song.pattern_group_order:
        offsets[gid] = len(streams)
        streams += group_packed[gid]
    orders = [[(offsets[gid], ch.pattern_headers[i][0]) for i, gid in enumerate(ch.pattern_groups)]
              for ch in song.channels]
    return bytes(streams), orders


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
        group_packed = pack_groups(song, instruments_by_index)

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
# New instruments (docs/audio.md, "New instruments"): an instrument the
# ROM's pool doesn't have, from an XM's own instrument and sample or from
# games/xs/sound/instruments/<name>.json and a WAV, appended to a pool after the
# ROM's instruments and samples.
# ---------------------------------------------------------------------------

INSTRUMENTS_DIR = os.path.join(SOUND_DIR, 'instruments')

# gGaxPeriodTable (lib/gax/data/gax_tables.c): entry i is the playback
# rate, in Hz * 2048, of pitch i in 1/32 semitones: PERIOD_0_HZ * 2 ** (i /
# 384). GaxChannelMix clamps the index to the last entry (as unsigned, so
# a negative one too).
PERIOD_0_HZ = 0x105A02 / 2048
PERIOD_COUNT = 0xEF4
# A note's pitch index is (note - 2) * 32 (GaxChannelSetNote), plus the
# instrument sequence's note ((2 - 2) * 32 for a new instrument), plus the
# wave row's tune. XM's middle C, C-4, is note 49, and plays a sample with
# no relative note or finetune at 8363 Hz.
XM_C4 = 49
XM_C4_RATE = 8363
# Instrument numbers are bytes in a pattern row and in GAX_fx_ex's queue
# (GaxChannelState.pendingInstrument), sample numbers in the instrument's
# waveIdx; instrument and sample 0 are the empty placeholders.
MAX_INDEX = 255
# GaxChannelMix keeps sample positions in Q11 in an s32.
MAX_SAMPLE_LENGTH = (1 << 20) - 1
# An envelope's point count is a byte (GaxEnvelope.count).
MAX_ENVELOPE_POINTS = 255
# A note when the new samples grow the ROM a lot, and the music block's
# limit: the GBA maps 32 MiB of ROM, and the ROM's other data is about 8.
LARGE_SAMPLES = 1024 * 1024
MAX_BLOCK = 24 * 1024 * 1024

INSTRUMENT_FIELDS = ('sample', 'rate', 'loop', 'loop_start', 'loop_end', 'volume', 'envelope', 'vibrato',
                     'comment')
LOOP_KINDS = ('none', 'forward', 'pingpong')


def tune_for_rate(rate, fixed_pitch):
    """The wave row's tune that plays a sample at `rate` Hz on XM's C-4
    (49), or at every note for a fixed-pitch instrument (a sound effect's)."""
    t = round(384 * math.log2(rate / PERIOD_0_HZ))
    return t if fixed_pitch else t - (XM_C4 - 2) * 32


def pitch_index(note, tune):
    """The gGaxPeriodTable index a pattern note plays a new instrument at."""
    return (note - 2) * 32 + tune


def note_name(note):
    return ['C-', 'C#', 'D-', 'D#', 'E-', 'F-', 'F#', 'G-', 'G#', 'A-', 'A#', 'B-'][(note - 1) % 12] + \
        str((note - 1) // 12)


def to_8bit(frames, width, fail):
    """Signed 8-bit samples from little-endian PCM `width` bytes wide
    (8-bit unsigned, wider signed, as in a WAV). GAX2's samples and its
    output are 8-bit, so wider ones are rounded to their top byte."""
    if width == 1:
        return bytes((b - 128) & 0xFF for b in frames)
    if width not in (2, 3, 4):
        fail(f"{width * 8}-bit samples: a sample is 8, 16, 24 or 32-bit PCM (rounded to 8 bits)")
    out = bytearray()
    shift = 8 * width - 8
    for i in range(0, len(frames) - width + 1, width):
        v = int.from_bytes(frames[i:i + width], 'little', signed=True)
        out.append(max(-128, min(127, (v + (1 << (shift - 1))) >> shift)) & 0xFF)
    return bytes(out)


def load_wav(path, fail):
    """(signed 8-bit samples, the WAV's rate) of a mono PCM WAV."""
    rel = os.path.relpath(path, ROOT)
    try:
        with wave.open(path, 'rb') as w:
            channels, width, rate = w.getnchannels(), w.getsampwidth(), w.getframerate()
            frames = w.readframes(w.getnframes())
    except (wave.Error, EOFError) as e:
        fail(f"{rel}: not a PCM WAV ({str(e) or 'it ends early'})")
    if channels != 1:
        fail(f"{rel}: {channels} channels: GAX2 mixes mono, so a sample is mono")
    return to_8bit(frames, width, fail), rate


def check_sample(spec, fail):
    """The limits a new instrument's sample and loop must keep."""
    n = len(spec['sample'])
    if n == 0:
        fail("the sample is empty")
    if n > MAX_SAMPLE_LENGTH:
        fail(f"the sample is {n} samples long: GAX2 keeps sample positions in Q11 in 32 bits, "
             f"so at most {MAX_SAMPLE_LENGTH}")
    if spec['loop'] != 'none' and not (isinstance(spec['loop_start'], int) and isinstance(spec['loop_end'], int)
                                       and 0 <= spec['loop_start'] < spec['loop_end'] <= n):
        fail(f"the loop {spec['loop_start']}-{spec['loop_end']} isn't inside the sample (0-{n}, the end "
             "exclusive)")


def gax_envelope(env, fail):
    """A GAX2 volume envelope from XM's form: points (tick, volume 0-64),
    the first at tick 0, and an optional sustain point and loop (point
    indices). None is no envelope: full volume, cut at the note-off.

    Without a sustain point the note-off doesn't cut the note, as in XM:
    the sustain is then the last point, where the envelope stops anyway
    (GaxEnvelopeTick), and GaxChannelSetNote only cuts a note whose
    envelope has none."""
    if env is None:
        return {'sustain': None, 'loop_start': None, 'loop_end': None,
                'points': [{'x': 0, 'interpolation': 0, 'y': 255, 'byte_05': 0, 'ushort_06': 0}]}
    pts = env['points']
    if not 1 <= len(pts) <= MAX_ENVELOPE_POINTS:
        fail(f"the envelope has {len(pts)} points (1-{MAX_ENVELOPE_POINTS})")
    if any(not isinstance(v, int) for p in pts for v in p):
        fail("the envelope's ticks and volumes are integers")
    if pts[0][0] != 0:
        fail("the envelope's first point isn't at tick 0")
    for (x0, _), (x1, _) in zip(pts, pts[1:]):
        if not x0 < x1 <= 0xFFFF:
            fail(f"the envelope's points must be at increasing ticks up to 65535 ({x0}, then {x1})")
    for x, y in pts:
        if not 0 <= y <= 64:
            fail(f"the envelope's point at tick {x} has volume {y} (0-64)")
    last = len(pts) - 1
    sustain, ls, le = env.get('sustain'), env.get('loop_start'), env.get('loop_end')
    for what, v in (('sustain point', sustain), ('loop start', ls), ('loop end', le)):
        if v is not None and not (isinstance(v, int) and 0 <= v <= last):
            fail(f"the envelope's {what} {v} isn't one of its points (0-{last})")
    if (ls is None) != (le is None) or ls is not None and ls > le:
        fail("the envelope's loop needs both a start and an end point, the start first")
    points = []
    for i, (x, y) in enumerate(pts):
        v = min(255, y * 4)
        slope = 0
        if i:
            # GaxEnvelopePoint.slope: Q8 per tick from the point before,
            # rounded toward 0 as in the ROM's; past the s16 only between
            # points a tick apart, where it isn't used
            px, pv = pts[i - 1][0], points[-1]['y']
            q = abs(v - pv) * 256 // (x - px)
            slope = max(-0x7FFF, min(0x7FFF, q if v >= pv else -q))
        points.append({'x': x, 'interpolation': slope, 'y': v, 'byte_05': 0, 'ushort_06': 0})
    return {'sustain': last if sustain is None else sustain, 'loop_start': ls, 'loop_end': le, 'points': points}


def instrument_entry(spec, sample_index, fail):
    """A new instrument in the manifest's form (Instrument), as the ROM's
    one-sample instruments are: one wave row; a one-step sequence that
    starts it at the note (or at its fixed pitch) and sets the sample's
    volume; the volume envelope; and the second envelope (`unknown12`,
    one flat point) every ROM instrument has and the engine never reads."""
    loop = spec['loop']
    empty_row = {'byte_02': 0, 'is_bidirectional': False, 'start_position': 0, 'loop_start': 0, 'loop_end': 0,
                 'int_10': 0, 'ushort_14': 0, 'ushort_16': 0, 'pitch': 0}
    row = dict(empty_row, is_bidirectional=loop == 'pingpong', pitch=spec['tune'])
    if loop != 'none':
        row.update(loop_start=spec['loop_start'], loop_end=spec['loop_end'])
    volume = spec['volume']
    effect = {'type': 12, 'param': volume * 4} if volume < 64 else {'type': 0, 'param': 0}
    delay, depth, speed = spec['vibrato']
    return {
        'byte_00': 0, 'sample_indices': [sample_index, 0, 0, 0], 'byte_05': 0, 'byte_06': 0, 'byte_07': 0,
        'vibrato_delay': delay, 'vibrato_depth': depth, 'vibrato_speed': speed, 'byte_0b': 0,
        'unknown12': [1, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0], 'row_speed': 1, 'ushort_12': 0,
        'samples': [row, dict(empty_row), dict(empty_row), dict(empty_row)],
        'rows': [{'relative_note_number': 2, 'dont_use_note_pitch': spec['fixed_pitch'], 'sample_index': 1,
                  'byte_03': 0, 'effects': [effect, {'type': 0, 'param': 0}]}],
        'envelope': gax_envelope(spec['envelope'], fail),
    }


def xm_instrument_spec(xi, sample_no, fail):
    """A new instrument from an XM instrument and one of its samples: the
    sample (8-bit), its loop, volume, relative note and finetune, the
    volume envelope and the auto-vibrato. Stops on what GAX2 can't
    play."""
    if sample_no >= len(xi['samples']):
        fail(f"its key map plays sample {sample_no}, which it doesn't have")
    xs = xi['samples'][sample_no]
    if xs['packing'] == 0xAD:
        fail("its sample is ADPCM-compressed (ModPlug): save it uncompressed")
    if xs['type'] & 0x20:
        fail("its sample is stereo: GAX2 mixes mono, so a sample is mono")
    wide = xs['type'] & 0x10
    # XM stores the sample as deltas
    raw, acc = bytearray(), 0
    if wide:
        for (d,) in struct.iter_unpack('<h', xs['data'][:len(xs['data']) & ~1]):
            acc = (acc + d + 0x8000) % 0x10000 - 0x8000
            raw += struct.pack('<h', acc)
        data = to_8bit(bytes(raw), 2, fail)
    else:
        for d in xs['data']:
            acc = (acc + d) & 0xFF
            raw.append(acc)
        data = bytes(raw)
    unit = 2 if wide else 1
    kind = xs['type'] & 3
    if kind == 3:
        fail("its sample's loop type is 3, which isn't one (none, forward or ping-pong)")
    spec = {
        'sample': data, 'loop': LOOP_KINDS[kind] if xs['loop_len'] else 'none',
        'loop_start': xs['loop_start'] // unit, 'loop_end': (xs['loop_start'] + xs['loop_len']) // unit,
        'volume': min(64, xs['volume']),
        'tune': round(384 * math.log2(XM_C4_RATE / PERIOD_0_HZ) + 32 * xs['relnote'] + xs['finetune'] / 4)
        - (XM_C4 - 2) * 32,
        'fixed_pitch': False, 'envelope': None, 'vibrato': (0, 0, 0),
    }
    check_sample(spec, fail)
    if xi.get('pan_type', 0) & 1:
        fail("it has a panning envelope: GAX2 mixes mono, so nothing pans (switch the envelope off)")
    if xi.get('vol_type', 0) & 1:
        if xi['fadeout']:
            fail(f"its volume fadeout is {xi['fadeout']}: GAX2 has no fadeout (set it to 0 and shape the "
                 "release with envelope points after the sustain point)")
        if not 1 <= xi['vol_count'] <= 12:
            fail(f"its volume envelope has {xi['vol_count']} points (1-12)")
        spec['envelope'] = {
            'points': xi['vol_points'],
            'sustain': xi['vol_sustain'] if xi['vol_type'] & 2 else None,
            'loop_start': xi['vol_loop_start'] if xi['vol_type'] & 4 else None,
            'loop_end': xi['vol_loop_end'] if xi['vol_type'] & 4 else None,
        }
    if xi.get('vib_depth') and xi.get('vib_rate'):
        if xi['vib_type'] != 0:
            fail("its auto-vibrato isn't a sine: GAX2's vibrato is one (gGaxVibratoTable)")
        if xi['vib_sweep']:
            fail("its auto-vibrato has a sweep: GAX2's vibrato starts at full depth (set the sweep to 0)")
        # XM's depth d swings the pitch by d/64 semitones, GAX2's by
        # gGaxVibratoTable (+-127) * depth >> 8 in 1/32 semitones: the same
        # number. XM steps through 256 phases a cycle, GAX2 through 64.
        spec['vibrato'] = (0, xi['vib_depth'], max(1, min(63, round(xi['vib_rate'] / 4))))
    return spec


def json_instrument_spec(name, fixed_pitch, fail):
    """A new instrument from games/xs/sound/instruments/<name>.json and its WAV
    (docs/audio.md, "New instruments")."""
    path = os.path.join(INSTRUMENTS_DIR, f'{name}.json')
    rel = os.path.relpath(path, ROOT)

    def bad(msg):
        fail(f"{rel}: {msg}")

    try:
        m = schemas.load(path)
    except ValueError as e:
        bad(f"not JSON ({e})")
    unknown = sorted(set(m) - set(INSTRUMENT_FIELDS))
    if unknown:
        bad(f"unknown fields {', '.join(unknown)} (an instrument's are {', '.join(INSTRUMENT_FIELDS)})")
    if 'sample' not in m:
        bad('no "sample" (the WAV, a path under games/xs/sound/, e.g. "samples/lead.wav")')
    wav = os.path.normpath(os.path.join(SOUND_DIR, m['sample']))
    if not os.path.isfile(wav):
        bad(f"its sample {m['sample']} isn't a file under games/xs/sound/")
    data, wav_rate = load_wav(wav, bad)
    rate = m.get('rate', wav_rate)
    if isinstance(rate, bool) or not isinstance(rate, (int, float)) or rate <= 0:
        bad(f"rate {rate!r}: the rate is a number of Hz")
    loop = m.get('loop', 'none')
    if loop not in LOOP_KINDS:
        bad(f"loop {loop!r}: one of {', '.join(LOOP_KINDS)}")
    volume = m.get('volume', 64)
    if isinstance(volume, bool) or not isinstance(volume, int) or not 0 <= volume <= 64:
        bad(f"volume {volume!r}: 0-64")
    vib = m.get('vibrato', {})
    unknown = sorted(set(vib) - {'delay', 'depth', 'speed'})
    if unknown:
        bad(f"unknown vibrato fields {', '.join(unknown)} (delay, depth, speed)")
    vibrato = (vib.get('delay', 0), vib.get('depth', 0), vib.get('speed', 0))
    for what, v, top in zip(('delay', 'depth', 'speed'), vibrato, (255, 255, 63)):
        if isinstance(v, bool) or not isinstance(v, int) or not 0 <= v <= top:
            bad(f"vibrato {what} {v!r}: 0-{top}")
    env = m.get('envelope')
    if env is not None:
        unknown = sorted(set(env) - {'points', 'sustain', 'loop_start', 'loop_end'})
        if unknown:
            bad(f"unknown envelope fields {', '.join(unknown)} (points, sustain, loop_start, loop_end)")
        if not all(isinstance(p, list) and len(p) == 2 for p in env.get('points', [])):
            bad("the envelope's points are [tick, volume] pairs")
        env = dict(env, points=[tuple(p) for p in env.get('points', [])])
    spec = {
        'sample': data, 'loop': loop, 'loop_start': m.get('loop_start', 0), 'loop_end': m.get('loop_end', len(data)),
        'volume': volume, 'tune': tune_for_rate(rate, fixed_pitch), 'fixed_pitch': fixed_pitch,
        'envelope': env, 'vibrato': vibrato,
    }
    check_sample(spec, bad)
    if fixed_pitch and not 0 <= spec['tune'] < PERIOD_COUNT:
        bad(f"rate {rate} Hz: a sound effect plays at {PERIOD_0_HZ:.0f}-"
            f"{PERIOD_0_HZ * 2 ** ((PERIOD_COUNT - 1) / 384):.0f} Hz (gGaxPeriodTable)")
    gax_envelope(env, bad)
    return spec


class Pool:
    """A data set's instrument and sample pool (the music's or the sound
    effects'), which new instruments are appended to: after the ROM's
    instruments, each new sample after the ROM's samples. Both are
    deduplicated by content: an instrument or a sample the pool already
    has (the ROM's, or an earlier song's) is that one."""

    def __init__(self, manifest, instruments, samples, label):
        self.instruments = instruments
        self.samples = samples
        self.label = label
        self.rom_instruments = max(instruments) + 1
        self.sample_ids = {}
        for idx in sorted(samples):
            self.sample_ids.setdefault(samples[idx], idx)
        self.instrument_ids = {}
        for k in sorted(manifest['instruments'], key=int):
            self.instrument_ids.setdefault(json.dumps(manifest['instruments'][k], sort_keys=True), int(k))
        self.named = {}
        self.new_instruments = []
        self.new_samples = []

    def add(self, spec, fail):
        data = spec['sample']
        sidx = self.sample_ids.get(data)
        if sidx is None:
            sidx = max(self.samples, default=0) + 1
            if sidx > MAX_INDEX:
                fail(f"{self.label} would have sample {sidx}: an instrument names its samples by a byte "
                     f"(at most {MAX_INDEX})")
            self.samples[sidx] = data
            self.sample_ids[data] = sidx
            self.new_samples.append(sidx)
        entry = instrument_entry(spec, sidx, fail)
        key = json.dumps(entry, sort_keys=True)
        idx = self.instrument_ids.get(key)
        if idx is None:
            idx = max(self.instruments) + 1
            if idx > MAX_INDEX:
                fail(f"{self.label} would have instrument {idx}: a pattern row and GAX_fx_ex name an "
                     f"instrument by a byte (at most {MAX_INDEX})")
            self.instruments[idx] = Instrument(entry)
            self.instrument_ids[key] = idx
            self.new_instruments.append(idx)
        return idx

    def add_named(self, name, fixed_pitch, fail):
        """games/xs/sound/instruments/<name>.json's instrument: one pool entry
        however many songs or sound effects use it."""
        if name not in self.named:
            self.named[name] = self.add(json_instrument_spec(name, fixed_pitch, fail), fail)
        return self.named[name]

    def report(self):
        """A line on what the new instruments add, for the build log."""
        if not self.new_instruments:
            return
        size = sum(len(self.samples[i]) for i in self.new_samples)
        print(f"{self.label}: {len(self.new_instruments)} new instruments (from {self.new_instruments[0]}), "
              f"{len(self.new_samples)} new samples, {size} bytes of sample data")
        if size > LARGE_SAMPLES:
            print(f"note: the new samples grow the ROM by {size / (1024 * 1024):.1f} MiB "
                  "(a GBA ROM is at most 32 MiB, this one about 8 without them)")


def named_instrument(name):
    """Whether an XM instrument's name is games/xs/sound/instruments/<name>.json's."""
    return bool(name) and os.path.basename(name) == name and \
        os.path.isfile(os.path.join(INSTRUMENTS_DIR, f'{name}.json'))


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


def sfx_pool(manifest_path=None, samples_dir=None, table_path=None):
    """The sound-effect set's pool, with the new instruments sound effects
    name (games/xs/sound/sfx_table.json's entries with an "instrument",
    games/xs/sound/instruments/<name>.json, docs/audio.md "Adding a sound
    effect"), and {name: its instrument number}, which tools/sfx_table.py
    writes as the entry's slot_id."""
    manifest_path = manifest_path or SFX_MANIFEST
    table_path = table_path or SFX_TABLE
    manifest = schemas.load(manifest_path)
    instruments, samples = load_pool(manifest, samples_dir or SFX_SAMPLES_DIR)
    pool = Pool(manifest, instruments, samples, 'the sound-effect set')
    rel = os.path.relpath(table_path, ROOT)
    slots = {}
    for i, e in enumerate(schemas.load(table_path)['entries']):
        def fail(msg, i=i):
            sys.exit(f"{rel}: entry {i} ({e.get('name', 'unnamed')}): {msg}")

        if ('slot_id' in e) == ('instrument' in e):
            fail('an entry has either a "slot_id" (one of the set\'s instruments) or an "instrument" '
                 '(games/xs/sound/instruments/<name>.json)')
        if 'slot_id' in e:
            if not 0 < e['slot_id'] < pool.rom_instruments:
                fail(f"slot_id {e['slot_id']}: the set's instruments are 1-{pool.rom_instruments - 1}")
            continue
        name = e['instrument']
        if not named_instrument(name):
            fail(f"instrument {name!r}: there is no games/xs/sound/instruments/{name}.json")
        slots[name] = pool.add_named(name, True, fail)
    return pool, slots


def build_sfx(manifest_path, samples_dir):
    """The sound-effect set (games/xs/sound/gax_sfx_manifest.json, games/xs/sound/sfx_samples/,
    and the new instruments games/xs/sound/sfx_table.json names) and the
    handler-type array the music block starts with.

    Returns (the set's Allocator, the sfxTypes Chunk). Every pointer is a
    relocation, so the set can change size (a longer sample) and the
    blocks can move."""
    manifest = schemas.load(manifest_path)
    pool, _ = sfx_pool(manifest_path, samples_dir)
    alloc, handler_addr = link_sfx(pool.instruments, pool.samples, SongInfo(manifest['song_info']))
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
    table (engine/src/engine/audio/data/song_table.cpp)."""
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
    `songs`) and the new ones, songs with an XM in games/xs/sound/songs/ but no
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
            sys.exit(f"{rel}: song_table's {k} is neither a song of the ROM nor games/xs/sound/songs/{k}.xm")


def song_keys(manifest_path=None):
    """The songs' names in song id order (SONG_<KEY>, constants/songs.h):
    what data that names a song by name (a level theme's "song", a
    screen's, a cutscene's) checks the name against."""
    manifest = schemas.load(manifest_path or os.path.join(SOUND_DIR, 'gax_manifest.json'))
    return [t['song'] for t in manifest['song_table']]


def screen_songs(music_path, keys):
    """games/xs/sound/music.json's screen_music: (SCREEN_SONG_<SCREEN>, its song's
    SONG_<KEY>, comment) for each screen, in the file's order."""
    rel = os.path.relpath(music_path, ROOT)
    out = []
    for s in schemas.load(music_path)['screen_music']:
        name = 'SCREEN_SONG_' + s['screen'].upper()
        if any(n == name for n, _, _ in out):
            sys.exit(f"{rel}: screen_music lists {s['screen']} more than once")
        if s['song'] not in keys:
            sys.exit(f"{rel}: {s['screen']}'s \"song\" {s['song']} isn't a song of the manifest's "
                     f"song_table ({', '.join(keys)})")
        out.append((name, 'SONG_' + s['song'].upper(), s['comment']))
    return out


def write_constants(manifest_path, path):
    """constants/songs.h: SONG_<KEY> for each song, in gSongTable order,
    and SCREEN_SONG_<SCREEN>, the song each screen plays (music.json)."""
    manifest = schemas.load(manifest_path)
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
        ' * Song IDs: the index into gSongTable (engine/src/engine/audio/data/song_table.cpp)',
        ' * that PlaySong/StartSong take, in the order of the manifest\'s',
        ' * song_table. Each name is the song\'s key in games/xs/sound/gax_manifest.json',
        ' * (the title the ROM stores with the song, e.g. "jungle (c) Manfred',
        ' * Linzner"; a new song\'s is its XM\'s, games/xs/sound/songs/<key>.xm), and',
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
        '/*',
        f' * The song each screen plays, from {os.path.relpath(MUSIC_JSON, ROOT)}\'s screen_music.',
        ' */',
        '',
    ]
    screens = screen_songs(MUSIC_JSON, [t['song'] for t in table])
    width = max(len(n) + len(v) for n, v, _ in screens)
    out += [f'#define {n} {v}{" " * (width - len(n) - len(v) + 1)}// {c}' for n, v, c in screens]
    out += [
        '',
        '#endif /* GUARD_CONSTANTS_SONGS_H */',
        '',
    ]
    with open(path, 'w') as f:
        f.write('\n'.join(out))


def write_song_table(manifest_path, path):
    """song_table.inc: gSongTable, which engine/src/engine/audio/data/song_table.cpp
    #includes. One entry per song of the manifest's song_table, in song id
    order: the song's GAX_SONG_<KEY> offset in the music block (gax_songs.h).
    Generated with constants/songs.h's ids, so no song is left NULL."""
    manifest = schemas.load(manifest_path)
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


# A new song's settings (games/xs/sound/songs/<key>.json, every field optional):
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


# The mix rates GAX2 plays at (gGaxMixRates, lib/gax/data/gax_tables.c):
# GAX2_init rounds a song's up to the next one.
GAX_MIX_RATES = (5735, 10513, 11469, 13380, 15769, 18158, 21025, 26760, 31537, 36316, 40138, 42049)
# AudioContext::gaxWork, the work RAM StartSong gives GAX2_init, and the
# sound-effect voices it asks for.
GAX_WORK_SIZE = 0x2000
GAX_GAME_SFX_VOICES = 3


def work_size(num_channels, sample_rate, half_rate_fx, mixer_data_bytes):
    """GAX2_estimate's work-RAM size for a song StartSong plays: the player
    state, the handler array (the song's 3 + channels and the 3 sound-effect
    voices) and format, each handler's header, instance and children (the
    mixer 0xc, the Info handler 0x1c, a channel 0x48 and its child; the
    third item, the empty list of alternative layouts, is skipped), the
    voices, the echo line for the longest echo tap, the resampler and two
    frame-sized buffers."""
    fx = GAX_GAME_SFX_VOICES
    rate = next((r for r in GAX_MIX_RATES if r >= sample_rate), GAX_MIX_RATES[-1])  # GaxFindMixRate
    size = 0x18c + (3 + num_channels + fx) * 4 + 8
    size += 0xc + 0xc + num_channels * 4 + fx * 4  # the mixer
    size += 0xc + 0x1c  # the Info handler
    size += num_channels * (0xc + 0x48 + 4)
    size += fx * 0x58
    taps = struct.unpack_from('<6I', bytes(mixer_data_bytes))
    longest = max(taps[1], taps[3], taps[5])
    if longest:
        size += longest * rate // 1000 * 2 + 0x18
    size += 0x130 if half_rate_fx else 0xdc
    frames = rate * 1000 // 0xe94f
    return size + frames * 4 + 0x20


def new_song_instruments(xm, order_rows, pool, fail):
    """The pool instrument each XM instrument the song plays is
    (`gax_instrument_indices` of a ROM song's XM; None for the others): an
    `Instrument N` is the ROM pool's N, an instrument named like
    games/xs/sound/instruments/<name>.json is that one, any other one is a new
    instrument from its own sample (Pool.add). Also checks that the notes
    a new instrument plays are in GAX2's period table.

    order_rows: the rows of each order position, every note with its
    instrument filled in."""
    xis = xm['instruments']
    first_use, notes = [], {}
    for rows in order_rows:
        for row in rows:
            for cell in row:
                if cell.instrument:
                    if cell.instrument not in first_use:
                        first_use.append(cell.instrument)
                    if 1 <= cell.note <= 96:
                        notes.setdefault(cell.instrument, set()).add(cell.note)
    out = [None] * len(xis)
    for n in first_use:
        xi = xis[n - 1]
        name = xi['name']

        def bad(msg, n=n, name=name):
            fail(f"instrument {n} ({name!r}): {msg}")

        if name.startswith('Instrument ') and name[len('Instrument '):].isdigit():
            idx = int(name[len('Instrument '):])
            if not 0 < idx < pool.rom_instruments:
                bad(f"there is no Instrument {idx}: the pool's instruments are 1-{pool.rom_instruments - 1} "
                    f"({pool.label}); give a new instrument another name")
            out[n - 1] = idx
            continue
        if named_instrument(name):
            idx = pool.add_named(name, False, bad)
        else:
            if not xi['samples']:
                bad("it has no sample: name it \"Instrument N\" for the pool's instrument N, name it after "
                    "games/xs/sound/instruments/<name>.json, or give it a sample")
            keymap = xi.get('keymap', [0] * 96)
            used = sorted({keymap[k - 1] for k in notes.get(n, ())}) or [keymap[XM_C4 - 1]]
            if len(used) > 1:
                bad(f"its key map plays samples {', '.join(map(str, used))}: a GAX2 instrument has one sample "
                    "for every note (split it into one instrument per sample)")
            idx = pool.add(xm_instrument_spec(xi, used[0], bad), bad)
        out[n - 1] = idx
        tune = pool.instruments[idx].samples[0].pitch
        for note in sorted(notes.get(n, ())):
            if not 0 <= pitch_index(note, tune) < PERIOD_COUNT:
                bad(f"note {note_name(note)} is pitch {pitch_index(note, tune)}, past the ends of GAX2's period "
                    f"table (0-{PERIOD_COUNT - 1}): play it in another octave, or retune the sample")
    return out


def gax_effect(cell):
    return _xm_effect_to_gax(cell.effect, cell.param, cell.volume)[0]


def set_effect(rows, row, effect, param, why, fail):
    """Puts a GAX2 effect (`effect`, `param`) on `row` of an order
    position, in the first channel with no effect there: a GAX2 row has
    one."""
    for cell in rows[row]:
        if gax_effect(cell) == 0:
            cell.effect, cell.param, cell.volume = effect, param, 0
            return
    fail(f"{why}, but row {row} has an effect on every channel (a GAX2 row has one)")


def new_song(key, xm_path, xm, pool):
    """A song with no ROM history: its scaffold derived from the XM, with
    the settings of the optional games/xs/sound/songs/<key>.json next to it.

    - one GAX2 channel per XM channel, in order;
    - one order position per entry of the XM's order list (a channel's
      pattern there is its column of the XM pattern the position plays),
      looping back to the XM's restart position;
    - identical patterns stored once (a pattern the order repeats, or the
      same notes on two channels); no transposes.

    The XM's instruments are the shared pool's (`Instrument N`, as in the
    ROM's songs), games/xs/sound/instruments/<name>.json's by name, or new ones
    from their own samples (new_song_instruments). GAX2 songs have one
    pattern length and start at speed 6: a shorter pattern gets a pattern
    break (D00) on its last row, and another speed an Fxx on the first
    row. The BPM is ignored: GAX2 steps a tick per frame."""
    where = os.path.relpath(xm_path, ROOT)

    def fail(msg):
        sys.exit(f"{where}: {msg}")

    settings = dict(NEW_SONG_DEFAULTS)
    json_path = os.path.splitext(xm_path)[0] + '.json'
    if os.path.exists(json_path):
        given = schemas.load(json_path)
        unknown = sorted(set(given) - set(NEW_SONG_DEFAULTS))
        if unknown:
            sys.exit(f"{os.path.relpath(json_path, ROOT)}: unknown settings {', '.join(unknown)} "
                     f"(a song's are {', '.join(NEW_SONG_DEFAULTS)})")
        settings.update(given)
    if len(settings['mixer_data_bytes']) != 28:
        fail("mixer_data_bytes must be 28 bytes")
    if settings['sample_rate'] not in GAX_MIX_RATES:
        fail(f"sample_rate {settings['sample_rate']}: GAX2 mixes at {', '.join(map(str, GAX_MIX_RATES))} Hz "
             "(gGaxMixRates)")

    order = xm['order']
    if not order:
        fail("the order list is empty")
    if any(p >= len(xm['patterns']) for p in order):
        fail("the order list plays a pattern the module doesn't have")
    if xm['restart'] >= len(order):
        fail(f"restart position {xm['restart']} is past the order list")
    if not 1 <= xm['tempo'] < 0x20:
        fail(f"speed {xm['tempo']}: a speed is 1-31")
    num_channels = xm['num_channels']

    # each order position's rows, a copy to add effects to, every note
    # with its instrument: a note with none plays the channel's last one
    order_rows = []
    current = [0] * num_channels
    for pos, p in enumerate(order):
        rows = [[copy.copy(cell) for cell in row] for row in xm['patterns'][p]]
        for r, row in enumerate(rows):
            for c, cell in enumerate(row):
                if cell.instrument > len(xm['instruments']):
                    fail(f"pattern {p} plays instrument {cell.instrument}, which the module doesn't have")
                if cell.instrument:
                    current[c] = cell.instrument
                elif 1 <= cell.note <= 96:
                    if not current[c]:
                        fail(f"pattern {p}, row {r}, channel {c}: a note before the channel has an instrument")
                    cell.instrument = current[c]
        order_rows.append(rows)

    # one pattern length: the longest, the others end on a pattern break
    num_rows = max(len(rows) for rows in order_rows)
    for pos, rows in enumerate(order_rows):
        last = len(rows) - 1
        if len(rows) < num_rows and all(gax_effect(cell) != 13 for cell in rows[last]):
            set_effect(rows, last, 13, 0, f"pattern {order[pos]} has {len(rows)} rows, under the song's "
                       f"{num_rows}, so it needs a pattern break (D00) on its last row", fail)
    # the starting speed: GaxInfoResetPosition starts every song at 6
    if xm['tempo'] != 6 and all(gax_effect(cell) != 15 for cell in order_rows[0][0]):
        speeds = any(gax_effect(cell) == 15 for rows in order_rows for row in rows for cell in row)
        if xm['restart'] == 0 and speeds:
            fail(f"speed {xm['tempo']}: GAX2 starts a song at speed 6, so the tool sets {xm['tempo']} with an "
                 "Fxx on the first row, which this song plays again when it loops (restart position 0) "
                 "after its own Fxx: start the module at speed 6, or put the Fxx on the first row yourself")
        set_effect(order_rows[0], 0, 15, xm['tempo'], f"speed {xm['tempo']}: GAX2 starts a song at speed 6, "
                   "so the first row needs an Fxx", fail)

    used = new_song_instruments(xm, order_rows, pool, fail)

    entry = {
        'num_channels': num_channels,
        'num_rows_per_pattern': num_rows,
        'num_patterns_per_channel': len(order),
        'loop_point': xm['restart'],
        'ushort_0a': 0,
        'title': settings['title'] or xm['name'] or key,
    }
    for field in ('artist', 'volume', 'sample_rate', 'num_fx_channels', 'half_rate_fx', 'mixer_data_bytes'):
        entry[field] = settings[field]
    need = work_size(num_channels, settings['sample_rate'], settings['half_rate_fx'], settings['mixer_data_bytes'])
    if need > GAX_WORK_SIZE:
        fail(f"GAX2 needs {need:#x} bytes of work RAM for {num_channels} channels at {settings['sample_rate']} Hz "
             f"(GAX2_estimate), but StartSong gives it {GAX_WORK_SIZE:#x} (AudioContext::gaxWork): "
             "use fewer channels, a lower sample_rate or shorter echo taps")
    song = Song(entry, num_items=3 + num_channels, unk_ptr=0)

    groups = {}  # packed pattern -> its group
    for ci in range(num_channels):
        ch = Channel()
        for rows in order_rows:
            pat = build_pattern_from_xm([row[ci] for row in rows], used, pool.instruments, 0)
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


def load_songs(manifest, songs_dir, samples_dir, xms=None):
    """The music pool and the songs in the music block's order: (the pool,
    with the new songs' new instruments appended; the songs' keys; their
    Songs). xms: {key: read_xm's value} to use in place of a song's XM
    file (an editor's unsaved edits)."""
    instruments, samples = load_pool(manifest, samples_dir)
    # the new songs' new instruments are appended to it
    pool = Pool(manifest, instruments, samples, 'the music pool')

    keys = block_songs(manifest)
    songs = []
    for key in keys:
        xm_path = os.path.join(songs_dir, f"{key}.xm")
        xm = xms[key] if xms and key in xms else read_xm(xm_path)
        if key in manifest['songs']:
            songs.append(rom_song(manifest['songs'][key], xm, instruments))
        else:
            songs.append(new_song(key, xm_path, xm, pool))
    return pool, keys, songs


def build(manifest_path, songs_dir, samples_dir, out_path, layout_path=None, header_path=None):
    manifest = schemas.load(manifest_path)

    pool, keys, songs = load_songs(manifest, songs_dir, samples_dir)
    instruments, samples = pool.instruments, pool.samples
    pool.report()

    default = default_song(manifest['default_song'])
    _, header_prefix = build_sfx(SFX_MANIFEST, SFX_SAMPLES_DIR)

    alloc = link(instruments, songs + [default], samples, header_prefix)
    if len(alloc.buf) > MAX_BLOCK:
        sys.exit(f"the music block is {len(alloc.buf)} bytes: with the ROM's other 8 MiB it doesn't fit "
                 "the GBA's 32 MiB of ROM")
    # The default song's own song struct is the layout the code references
    # as gGaxDefaultSong: a separate output, so it can have that label.
    # Its pointers are relocations against gGaxMusicData too, so games/xs/data/data.s
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
MUSIC_JSON = os.path.join(SOUND_DIR, 'music.json')
SFX_SAMPLES_DIR = os.path.join(SOUND_DIR, 'sfx_samples')
SFX_TABLE = os.path.join(SOUND_DIR, 'sfx_table.json')


def use_game(game):
    """Work on the sound of `game`'s tree (a crashdata.common.game.Game:
    its root; the sound is XS's, games/xs/sound/). The tools run in the
    tree they build, whose root these paths are by default; an editor
    working on another tree (a mod's) binds them to it."""
    global ROOT, SOUND_DIR, INSTRUMENTS_DIR, SFX_MANIFEST, MUSIC_JSON, SFX_SAMPLES_DIR, SFX_TABLE
    ROOT = os.path.abspath(game.root)
    SOUND_DIR = os.path.join(ROOT, 'games', 'xs', 'sound')
    INSTRUMENTS_DIR = os.path.join(SOUND_DIR, 'instruments')
    SFX_MANIFEST = os.path.join(SOUND_DIR, 'gax_sfx_manifest.json')
    MUSIC_JSON = os.path.join(SOUND_DIR, 'music.json')
    SFX_SAMPLES_DIR = os.path.join(SOUND_DIR, 'sfx_samples')
    SFX_TABLE = os.path.join(SOUND_DIR, 'sfx_table.json')
