#!/usr/bin/env python3
"""The pc platform's GAX2 song data (#1093, docs/platforms.md, "Audio").

    gax_host_data.py --music gax_audio_data.s --sfx gax_sfx_data.s
                     --default gax_default_layout.s --songs gax_songs.h -o OUT.c

tools/gax_audio.py builds the music block (gGaxMusicData), the
sound-effect set (gGaxSfxData) and the default layout (gGaxDefaultSong)
as the GBA's bytes, with their pointers as relocations in the .s next to
each .bin (`.4byte gGaxMusicData + 0x24`, `.4byte GaxChannelInit`). The
GAX2 engine the pc platform compiles (lib/gax, with platform/pc/units/lib/
gax/ in place of the parts that can't run on a host) reads those
structures through its own C declarations (gax_internal.h), whose
pointers are 8 bytes on a 64-bit host, so the GBA's 4-byte layout can't
be read as it is.

This writes the structures that hold pointers again as C, typed with
gax_internal.h's declarations, so the host compiler lays them out: the
handler layouts and the alternative-layout lists, the handler types (their
callbacks as the host's functions, their instance size the host's
sizeof), the child-type arrays, the song data (GaxSongData), the
instrument tables and instruments, the sample tables. Everything without a
pointer (patterns, order lists, envelopes, instrument sequences, the DSP
parameters, the samples) is read in place, from the GBA's bytes, which the
platform links in as they are (platform/pc/src/sound_data.S). The walk
starts at the songs (gax_songs.h's offsets), the sound-effect voice types
at the start of the music block (AudioContext's `sfxTypes`) and the
default layout, and follows the relocations, so it converts what the
engine can reach and nothing else.

The game hands GAX2_init the GBA layout's address (gSongTable's entry,
gGaxMusicData + the song's offset) and gGaxMusicData itself as `sfxTypes`;
gHostGaxLayouts maps those offsets of the music block to the converted
objects, which the platform's GAX2_init uses instead
(platform/pc/units/lib/gax/src/gax_playstart.c).
"""

import argparse
import os
import re
import struct
import sys

BLOCKS = {"gGaxMusicData": "music", "gGaxSfxData": "sfx", "gGaxDefaultSong": "default"}

# A handler type's kind, by its init callback, and its instance: the
# struct of gax_internal.h that the engine's code reads the handler as.
KINDS = {
    "GaxInfoInit": ("info", "GaxInfoHandler"),
    "GaxChannelInit": ("channel", "GaxChannelState"),
    "GaxFxChannelInit": ("fx", "GaxChannelState"),
    "GaxMixerInit": ("mixer", "GaxMixerHandler"),
}
# The GBA instance size of each (GBA sizeof - the 12-byte header).
GBA_INSTANCE = {"info": 0x1c, "channel": 0x48, "fx": 0x48, "mixer": 0x0c}

INCBIN = re.compile(r'\s*\.incbin\s+"([^"]+)",\s*(0x[0-9a-fA-F]+|\d+),\s*(0x[0-9a-fA-F]+|\d+)')
WORD = re.compile(r"\s*\.4byte\s+(\w+)(?:\s*\+\s*(0x[0-9a-fA-F]+|\d+))?\s*$")


class Block:
    """One of the built blocks: its bytes and its relocations (offset ->
    (symbol, addend))."""

    def __init__(self, name, s_path):
        self.name = name
        self.relocs = {}
        self.data = None
        pos = 0
        for line in open(s_path):
            line = line.split("@", 1)[0].rstrip()
            if not line.strip():
                continue
            m = INCBIN.match(line)
            if m:
                path, off, n = m.group(1), int(m.group(2), 0), int(m.group(3), 0)
                if self.data is None:
                    self.data = open(path, "rb").read()
                if off != pos:
                    sys.exit("%s: .incbin at %#x, expected %#x" % (s_path, off, pos))
                pos += n
                continue
            m = WORD.match(line)
            if m:
                self.relocs[pos] = (m.group(1), int(m.group(2) or "0", 0))
                pos += 4
                continue
            sys.exit("%s: can't read %r" % (s_path, line))
        if self.data is None:
            # all relocations (the default layout has one incbin, but be safe)
            self.data = bytes(pos)
        if len(self.data) != pos:
            sys.exit("%s: %#x bytes, the .bin has %#x" % (s_path, pos, len(self.data)))

    def u8(self, off):
        return self.data[off]

    def u16(self, off):
        return struct.unpack_from("<H", self.data, off)[0]

    def s16(self, off):
        return struct.unpack_from("<h", self.data, off)[0]

    def u32(self, off):
        return struct.unpack_from("<I", self.data, off)[0]

    def s32(self, off):
        return struct.unpack_from("<i", self.data, off)[0]

    def ptr(self, off):
        """The pointer at `off`: (block name, offset), ("func", name), or
        None for NULL. A word without a relocation must be 0."""
        r = self.relocs.get(off)
        if r is None:
            if self.u32(off) != 0:
                sys.exit("%s+%#x: a pointer without a relocation (%#x)" % (self.name, off, self.u32(off)))
            return None
        sym, add = r
        if sym in BLOCKS:
            return (BLOCKS[sym], add)
        if add != 0:
            sys.exit("%s+%#x: %s + %#x" % (self.name, off, sym, add))
        return ("func", sym)


class Writer:
    def __init__(self, blocks):
        self.blocks = blocks
        self.done = {}
        self.decls = []
        self.defs = []
        self.views = {}

    def raw(self, p, ctype):
        """A pointer to pointer-free data: the GBA's bytes, in place."""
        if p is None:
            return "NULL"
        block, off = p
        sym = {"music": "gGaxMusicData", "sfx": "gGaxSfxData"}[block]
        return "(%s)(%s + 0x%x)" % (ctype, sym, off)

    def name(self, kind, p):
        return "%s_%s_%x" % (kind, p[0], p[1])

    def view(self, struct_name, ctype, field, count):
        """A layout `count` long: a union of the declared type (whose array
        is 1 long) and a struct with the header and an array of the right
        length."""
        n = "Host%s%d" % (struct_name, count)
        if n not in self.views:
            self.views[n] = "union %s { struct %s l; struct { u32 count; %s%s[%d]; } s; };" % (
                n, struct_name, ctype, field, count)
        return n

    def emit(self, key, decl, body):
        self.decls.append(decl + ";")
        self.defs.append(decl + " = " + body + ";")

    def layout(self, p, is_list=False):
        """A GaxHandlerLayout (or, in a layout's types[2], a GaxLayoutList:
        the same shape, its entries layouts)."""
        if p is None:
            return "NULL"
        kind = "list" if is_list else "layout"
        n = self.name(kind, p)
        if (kind, p) in self.done:
            return "&%s.l" % n
        self.done[(kind, p)] = n
        b = self.blocks[p[0]]
        count = b.u32(p[1])
        entries = []
        for i in range(count):
            q = b.ptr(p[1] + 4 + 4 * i)
            if is_list:
                entries.append(self.layout(q))
            elif i == 2:
                entries.append("(struct GaxHandlerType *)%s" % self.layout(q, True))
            else:
                entries.append(self.handler_type(q))
        field = "layouts" if is_list else "types"
        ctype = "struct GaxHandlerLayout *" if is_list else "struct GaxHandlerType *"
        struct_name = "GaxLayoutList" if is_list else "GaxHandlerLayout"
        # A layout is `count` long: a struct with the header's fields and
        # an array of the right length, viewed as the declared type.
        decl = "static union %s %s" % (self.view(struct_name, ctype, field, max(count, 1)), n)
        body = "{ .s = { %d, { %s } } }" % (count, ", ".join(entries) if entries else "NULL")
        self.emit((kind, p), decl, body)
        return "&%s.l" % n

    def handler_type(self, p):
        if p is None:
            return "NULL"
        n = self.name("type", p)
        if ("type", p) in self.done:
            return "&" + n
        self.done[("type", p)] = n
        b = self.blocks[p[0]]
        o = p[1]
        fns = [b.ptr(o + 4 * i) for i in range(3)]
        if any(f is None or f[0] != "func" for f in fns):
            sys.exit("%s+%#x: a handler type without its three callbacks" % (p[0], o))
        init, unknown, play = (f[1] for f in fns)
        if init not in KINDS:
            sys.exit("%s+%#x: an unknown handler type (%s)" % (p[0], o, init))
        kind, inst = KINDS[init]
        child_count = b.u32(o + 0x0c)
        children = b.ptr(o + 0x10)
        size = b.u32(o + 0x14)
        if size != GBA_INSTANCE[kind]:
            sys.exit("%s+%#x: a %s type of %#x bytes, not %#x" % (p[0], o, kind, size, GBA_INSTANCE[kind]))
        data = b.ptr(o + 0x18)
        if kind in ("info", "fx"):
            data_init = ".song = %s" % self.song(data)
        elif kind == "channel":
            data_init = ".orders = %s" % self.raw(data, "struct GaxOrderEntry *")
        else:
            data_init = ".dsp = %s" % self.raw(data, "struct GaxDspParams *")
        child_ref = self.child_types(children, child_count) if children else "NULL"
        decl = "static struct GaxHandlerType %s" % n
        body = ("{ (void (*)(void *))%s, (void (*)(void *))%s, (u8 (*)(void *, void *, u32))%s, %d, %s, "
                "sizeof(struct %s) - sizeof(struct GaxHandler), { %s } }") % (
            init, unknown, play, child_count, child_ref, inst, data_init)
        self.emit(("type", p), decl, body)
        return "&" + n

    def child_types(self, p, count):
        n = self.name("children", p)
        if ("children", p) in self.done:
            return n
        self.done[("children", p)] = n
        b = self.blocks[p[0]]
        entries = [self.handler_type(b.ptr(p[1] + 4 * i)) for i in range(count)]
        self.emit(("children", p), "static struct GaxHandlerType *%s[%d]" % (n, count),
                  "{ %s }" % ", ".join(entries))
        return n

    def song(self, p):
        if p is None:
            return "NULL"
        n = self.name("song", p)
        if ("song", p) in self.done:
            return "&" + n
        self.done[("song", p)] = n
        b = self.blocks[p[0]]
        o = p[1]
        instr_ref, waves_count = self.instruments(b.ptr(o + 0x10))
        waves_ref = self.waves(b.ptr(o + 0x14), waves_count)
        body = ("{ .channelCount = %d, .patternRows = %d, .orderCount = %d, .loopOrder = %d, .volume = %d, "
                ".patterns = %s, .instruments = %s, .waves = %s, .mixRate = %d, .numSfx = %d, .halfRateFx = %d }") % (
            b.u16(o), b.u16(o + 2), b.u16(o + 4), b.u16(o + 6), b.u16(o + 8),
            self.raw(b.ptr(o + 0x0c), "u8 *"), instr_ref, waves_ref, b.u16(o + 0x18), b.u8(o + 0x1a), b.u8(o + 0x1b))
        if b.u16(o + 0x0a) != 0:
            sys.exit("%s+%#x: song data's 0x0a isn't 0" % (p[0], o))
        self.emit(("song", p), "static struct GaxSongData %s" % n, body)
        return "&" + n

    def instruments(self, p):
        """An instrument table: the run of pointers at `p` (the table has no
        length; it ends where the relocations do). Returns its name and the
        number of sample-table entries its instruments index."""
        if p is None:
            return "NULL", 0
        n = self.name("instruments", p)
        if ("instruments", p) in self.done:
            return n, self.done[("instruments", p)][1]
        b = self.blocks[p[0]]
        entries = []
        waves = 0
        o = p[1]
        while o in b.relocs:
            q = b.ptr(o)
            ref, w = self.instrument(q)
            entries.append(ref)
            waves = max(waves, w)
            o += 4
        self.done[("instruments", p)] = (n, waves)
        self.emit(("instruments", p), "static struct GaxChannelInstrument *%s[%d]" % (n, len(entries)),
                  "{ %s }" % ", ".join(entries))
        return n, waves

    def instrument(self, p):
        n = self.name("instrument", p)
        b = self.blocks[p[0]]
        o = p[1]
        wave_idx = [b.u8(o + 1 + i) for i in range(4)]
        if ("instrument", p) in self.done:
            return "&" + n, max(wave_idx) + 1
        self.done[("instrument", p)] = n
        rows = []
        for r in range(4):
            ro = o + 0x0c + 28 * r
            rows.append("{ %d, %d, %d, %d, %d, %d, %d, %d, %d }" % (
                b.u8(ro), b.u8(ro + 1), b.s32(ro + 4), b.s32(ro + 8), b.s32(ro + 0x0c), b.s32(ro + 0x10),
                b.s32(ro + 0x14), b.u16(ro + 0x18), b.s16(ro + 0x1a)))
        body = ("{ .empty = %d, .waveIdx = { %s }, .pad_05 = { %s }, .vibratoDelay = %d, .vibratoDepth = %d, "
                ".vibratoSpeed = %d, .rows = { %s }, .envelope = %s, .pad_80 = { %s }, .seqSpeed = %d, "
                ".seqLen = %d, .seq = %s }") % (
            b.u8(o), ", ".join(map(str, wave_idx)), ", ".join(str(b.u8(o + 5 + i)) for i in range(3)),
            b.u8(o + 8), b.u8(o + 9), b.u8(o + 0x0a), ", ".join(rows),
            self.envelope(b.ptr(o + 0x7c)), ", ".join(str(b.u8(o + 0x80 + i)) for i in range(4)),
            b.u8(o + 0x84), b.u8(o + 0x85), self.raw(b.ptr(o + 0x88), "struct GaxInstrumentSeqEntry *"))
        self.emit(("instrument", p), "static struct GaxChannelInstrument %s" % n, body)
        return "&" + n, max(wave_idx) + 1

    def envelope(self, p):
        """A volume envelope. Its points are 6 bytes of fields, which agbcc
        pads to 8 (an ARM struct's size is a multiple of 4), the host
        compiler to 6: so it is converted too."""
        if p is None:
            return "NULL"
        n = self.name("envelope", p)
        if ("envelope", p) not in self.done:
            self.done[("envelope", p)] = n
            b = self.blocks[p[0]]
            o = p[1]
            count = b.u8(o)
            view = "HostGaxEnvelope%d" % max(count, 1)
            if view not in self.views:
                self.views[view] = ("union %s { struct GaxEnvelope e; struct { u8 count, sustain, loopStart, loopEnd; "
                                    "struct GaxEnvelopePoint points[%d]; } s; };" % (view, max(count, 1)))
            points = ["{ %d, %d, %d }" % (b.u16(o + 4 + 8 * i), b.s16(o + 6 + 8 * i), b.u8(o + 8 + 8 * i))
                      for i in range(count)]
            self.emit(("envelope", p), "static union %s %s" % (view, n),
                      "{ .s = { %d, %d, %d, %d, { %s } } }" % (count, b.u8(o + 1), b.u8(o + 2), b.u8(o + 3),
                                                             ", ".join(points) if points else "{ 0, 0, 0 }"))
        return "&%s.e" % n

    def waves(self, p, count):
        if p is None:
            return "NULL"
        n = self.name("waves", p)
        if ("waves", p) in self.done:
            return n
        self.done[("waves", p)] = n
        b = self.blocks[p[0]]
        entries = []
        for i in range(count):
            o = p[1] + 8 * i
            entries.append("{ %s, %d }" % (self.raw(b.ptr(o), "u8 *"), b.u32(o + 4)))
        self.emit(("waves", p), "static struct GaxWave %s[%d]" % (n, max(count, 1)),
                  "{ %s }" % ", ".join(entries) if entries else "{ { NULL, 0 } }")
        return n


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--music", required=True)
    ap.add_argument("--sfx", required=True)
    ap.add_argument("--default", required=True)
    ap.add_argument("--songs", required=True, help="gax_songs.h, the songs' offsets")
    ap.add_argument("-o", "--out", required=True)
    args = ap.parse_args()

    blocks = {"music": Block("gGaxMusicData", args.music), "sfx": Block("gGaxSfxData", args.sfx),
              "default": Block("gGaxDefaultSong", args.default)}
    w = Writer(blocks)
    songs = []
    for line in open(args.songs):
        m = re.match(r"#define (GAX_SONG_\w+) (0x[0-9a-fA-F]+)", line)
        if m:
            songs.append((m.group(1), int(m.group(2), 16)))
    maps = []
    for name, off in songs:
        maps.append((off, w.layout(("music", off)), name))
    # The sound-effect voice types: the music block's first words, up to
    # the first one that isn't a relocation.
    sfx = []
    o = 0
    music = blocks["music"]
    while o in music.relocs and music.relocs[o][0] == "gGaxSfxData":
        sfx.append(w.handler_type(music.ptr(o)))
        o += 4
    # The default layout is a block of its own; its own name is the
    # engine's gGaxDefaultSong.
    default = blocks["default"]
    count = default.u32(0)
    entries = []
    for i in range(count):
        q = default.ptr(4 + 4 * i)
        entries.append("(struct GaxHandlerType *)%s" % w.layout(q, True) if i == 2 else w.handler_type(q))
    view = w.view("GaxHandlerLayout", "struct GaxHandlerType *", "types", max(count, 1))

    out = []
    out.append("/* Generated by platform/pc/tools/gax_host_data.py from tools/gax_audio.py's")
    out.append(" * music block, sound-effect set and default layout: their structures with")
    out.append(" * pointers, laid out by the host compiler (see the tool). Do not edit. */")
    out.append("")
    out.append('#include "gax_internal.h"')
    out.append('#include "gax_host.h"')
    out.append("")
    out.append("extern const u8 gGaxMusicData[];")
    out.append("extern const u8 gGaxSfxData[];")
    out.append("")
    out.extend(w.views.values())
    out.append("")
    out.extend(w.decls)
    out.append("")
    out.extend(w.defs)
    out.append("")
    out.append("/* gGaxDefaultSong (games/xs/data/data.s on the GBA). */")
    out.append("static union %s sDefaultSong = { .s = { %d, { %s } } };" % (view, count, ", ".join(entries)))
    out.append("extern struct GaxHandlerLayout gGaxDefaultSong __attribute__((alias(\"sDefaultSong\")));")
    out.append("")
    out.append("/* AudioContext's sfxTypes (gGaxMusicData's first %d words). */" % len(sfx))
    out.append("static struct GaxHandlerType *sSfxTypes[%d] = { %s };" % (max(len(sfx), 1), ", ".join(sfx)))
    out.append("")
    out.append("const struct HostGaxLayout gHostGaxLayouts[] = {")
    for off, ref, name in maps:
        out.append("    { 0x%x, %s, NULL }, /* %s */" % (off, ref, name))
    out.append("    { 0x0, NULL, sSfxTypes },")
    out.append("};")
    out.append("const u32 gHostGaxLayoutCount = sizeof(gHostGaxLayouts) / sizeof(gHostGaxLayouts[0]);")
    out.append("")
    tmp = args.out + ".tmp"
    with open(tmp, "w") as f:
        f.write("\n".join(out))
    os.replace(tmp, args.out)


if __name__ == "__main__":
    main()
