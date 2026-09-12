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
  sound/samples/*.wav       - one 8-bit mono PCM sample per instrument voice
                              (shared across songs) - this is what a modder
                              edits to change a sound.

NOTE on sound/gax_header_prefix.bin (36 bytes) and sound/gax_footer.bin (160
bytes): these bracket the block and belong to a small pre-existing
GAX2_SoundHandler chain that isn't part of any of these 19 songs (it appears
to be a shared "empty channel" placeholder used elsewhere in the sound
engine's data, referencing addresses outside this block entirely). Since
nothing in these 19 songs depends on it, it's preserved verbatim rather than
modeled.

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

# GAX2 song data occupies this fixed address range in the ROM; because nothing
# before it changes size, the block always lands at the same address, so we
# can bake absolute pointers for it directly instead of doing a relocatable
# link.
BASE_ADDR = 0x0855BCB4

# The 3 function pointers (Init/Unknown/Play) in a GAX2_SoundHandler are the
# engine's own dispatch code, not per-song data - fixed per handler type,
# identical across every song.
HANDLER_FUNCS = {
    'info': (0x080393FD, 0x08039439, 0x0803943D),
    'unknownc': (0x0803A22D, 0x0803A275, 0x0803A325),
    'channel': (0x08039519, 0x080395A1, 0x080395A5),
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
        return struct.pack('<7I', self.init_fn, self.unknown_fn, self.play_fn,
                            self.num_children, self.children_ptr, self.type_flags, self.data_ptr)


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
        self.byte_1d = m['byte_1d']
        self.sequence_data_ptr = 0
        self.instrument_set_ptr = 0
        self.sample_set_ptr = 0

    def pack(self):
        return struct.pack('<6H3IH2B',
                            self.num_channels, self.num_rows_per_pattern, self.num_patterns_per_channel,
                            self.loop_point, self.volume, self.ushort_0a,
                            self.sequence_data_ptr, self.instrument_set_ptr, self.sample_set_ptr,
                            self.sample_rate, self.num_fx_channels, self.byte_1d)


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
        out = bytearray()
        out.append(self.byte_00)
        out += bytes(self.sample_indices)
        out += bytes([self.byte_05, self.byte_06, self.byte_07,
                      self.vibrato_delay, self.vibrato_depth, self.vibrato_speed, self.byte_0b])
        for s in self.samples:
            out += s.pack()
        out += struct.pack('<II', self.envelope_ptr, self.gax2_unknown_ptr)
        out += bytes([self.row_speed, len(self.rows)])
        out += struct.pack('<H', self.ushort_12)
        out += struct.pack('<I', self.rows_ptr)
        return bytes(out)


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
    header_size = struct.unpack_from('<I', buf, 60)[0]
    song_length = struct.unpack_from('<H', buf, 64)[0]
    num_channels = struct.unpack_from('<H', buf, 68)[0]
    num_patterns = struct.unpack_from('<H', buf, 70)[0]

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

    return {'num_channels': num_channels, 'patterns': patterns,
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
    def __init__(self, base_addr, prefix=b''):
        self.base_addr = base_addr
        self.buf = bytearray(prefix)
        self._dedup = {}

    def addr_here(self):
        return self.base_addr + len(self.buf)

    def alloc(self, data, dedup=None):
        """dedup: None for no sharing, True for the allocator-wide (cross-song)
        dedup table, or a caller-supplied dict to scope sharing narrowly
        (e.g. per-song, matching what the original data actually does for
        patterns - see NOTE in link())."""
        table = self._dedup if dedup is True else dedup
        if table is not None:
            key = bytes(data)
            if key in table:
                return table[key]
        addr = self.addr_here()
        self.buf += data
        if table is not None:
            table[bytes(data)] = addr
        return addr


class Channel:
    def __init__(self):
        self.pattern_headers = []  # list of (transpose, reserved)
        self.patterns = []
        self.pattern_groups = []  # group id per pattern (see NOTE in link())


class Song:
    def __init__(self, manifest_entry, num_items=7, unk_ptr=0):
        self.info = SongInfo(manifest_entry)
        self.unknownc_data_bytes = bytes(manifest_entry['unknownc_data_bytes'])
        self.title = manifest_entry['title']
        self.artist = manifest_entry['artist']
        self.title_bytes = base64.b64decode(manifest_entry['title_bytes'])
        self.num_items = num_items
        self.unk_ptr = unk_ptr
        self.channels = []
        self.info_handler = SoundHandler(*HANDLER_FUNCS['info'], type_flags=0x1C)
        self.unknownc_handler = SoundHandler(*HANDLER_FUNCS['unknownc'], type_flags=0xC)
        self.channel_handlers = []
        self.pattern_group_bytes = {}  # gid -> original raw packed bytes


def link(instruments_by_index, songs, samples_by_index, base_addr, header_prefix, footer):
    alloc = Allocator(base_addr, header_prefix)

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
        alloc.buf += struct.pack('<I', instr_addrs[idx])

    # Sample data and its pointer table come next (before the songs), matching
    # the original layout - not just instrument data/table, then songs, then
    # samples, as might be guessed.
    n_samples = max(samples_by_index.keys()) + 1 if samples_by_index else 0
    sample_addrs = {}
    for idx in range(n_samples):
        s = samples_by_index.get(idx)
        sample_addrs[idx] = alloc.alloc(s, dedup=True) if s else 0

    alloc.alloc(b"\x00\x00")  # 2 bytes of fixed padding before the sample table

    sample_table_addr = alloc.addr_here()
    for idx in range(n_samples):
        s = samples_by_index.get(idx)
        alloc.buf += struct.pack('<II', sample_addrs[idx], len(s) if s else 0)

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

        # A `"Title" (c) Artist` metadata string (with its original padding)
        # sits right after the song's patterns and before its pattern-header
        # tables - captured verbatim in the manifest since it never changes.
        alloc.alloc(song.title_bytes)

        # Layout order from here on matches what's observed in the original
        # data: all pattern-header tables grouped together, then SongInfo +
        # its handler, then a single shared 1-entry "children" array (every
        # channel handler's Children[0] points back to the info handler, and
        # they all share this same one allocation), then the channel
        # handlers themselves, then the UnknownC bits, then the song struct.
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
                transpose, reserved = ch.pattern_headers[i]
                hdr_bytes += struct.pack('<Hbb', seq_off, transpose, reserved)
            headers_addrs[ci] = alloc.alloc(bytes(hdr_bytes))

        info_data_addr = alloc.alloc(info.pack())
        song.info_handler.data_ptr = info_data_addr
        info_handler_addr = alloc.alloc(song.info_handler.pack())

        channel_children_addr = alloc.alloc(struct.pack('<I', info_handler_addr))

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
            alloc.buf += struct.pack('<I', a)
        unknownc_data_addr = alloc.alloc(song.unknownc_data_bytes)
        song.unknownc_handler.data_ptr = unknownc_data_addr
        song.unknownc_handler.children_ptr = children_ptr
        song.unknownc_handler.num_children = len(channel_handler_addrs)
        unknownc_handler_addr = alloc.alloc(song.unknownc_handler.pack())

        song_bytes = struct.pack('<I', song.num_items)
        song_bytes += struct.pack('<III', unknownc_handler_addr, info_handler_addr, song.unk_ptr)
        for a in channel_handler_addrs:
            song_bytes += struct.pack('<I', a)
        song.new_offset = alloc.alloc(song_bytes)

    # A trailing 160-byte structure follows the last song - it looks like a
    # small extra sound-handler-ish entity distinct from any of the 19 named
    # songs (references addresses ending exactly where the last song ends).
    # Nothing in these 19 songs depends on it, so - like the leading header
    # prefix - it's preserved verbatim rather than modeled.
    alloc.alloc(footer)

    return bytes(alloc.buf)


# ---------------------------------------------------------------------------
# Top-level build
# ---------------------------------------------------------------------------

def load_sample_wav(path):
    with wave.open(path, 'rb') as w:
        frames = w.readframes(w.getnframes())
    # stored as unsigned 8-bit in the WAV; GAX v2 wants signed 8-bit
    return bytes(((b - 128) & 0xFF) for b in frames)


def build(manifest_path, songs_dir, samples_dir, out_path):
    manifest = json.load(open(manifest_path))

    instruments = {int(k): Instrument(v) for k, v in manifest['instruments'].items()}

    samples = {}
    for k in manifest['samples']:
        idx = int(k)
        wav_path = os.path.join(samples_dir, f"{idx:02d}.wav")
        if os.path.exists(wav_path):
            samples[idx] = load_sample_wav(wav_path)

    songs = []
    for key in manifest['song_order']:
        sm = manifest['songs'][key]
        song = Song(sm, num_items=sm['num_items'], unk_ptr=0)

        xm_path = os.path.join(songs_dir, f"{key}.xm")
        xm = read_xm(xm_path)
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

        songs.append(song)

    header_prefix = open(os.path.join(SOUND_DIR, 'gax_header_prefix.bin'), 'rb').read()
    footer = open(os.path.join(SOUND_DIR, 'gax_footer.bin'), 'rb').read()

    blob = link(instruments, songs, samples, BASE_ADDR, header_prefix, footer)
    with open(out_path, 'wb') as f:
        f.write(blob)
    print(f"wrote {out_path}: {len(blob)} bytes")


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} OUTPUT.bin")
        sys.exit(1)
    build(
        os.path.join(SOUND_DIR, 'gax_manifest.json'),
        os.path.join(SOUND_DIR, 'songs'),
        os.path.join(SOUND_DIR, 'samples'),
        sys.argv[1],
    )
