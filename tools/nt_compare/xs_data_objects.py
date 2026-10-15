#!/usr/bin/env python3
"""XS's data objects N-Tranced has whole (#1131, docs/n_tranced.md "Tables
shared with XS"): each `engine/src/*/data/` or `lib/*/data/` object of XS's build
whose `.rodata` is in N-Tranced's ROM as one run, its pointer words aside,
and whether every pointer is right there: to the N-Tranced address of the
symbol XS's relocation names (ntranced.map's), or into the object itself.

    python3 tools/nt_compare/xs_data_objects.py [baserom_n2.gba]

Needs both games built (crashbandicootxs.map, ntranced.map, XS's objects).
An object listed can be linked into N-Tranced as it is (objects.txt, with
data_names.txt naming what the code points at in it), if it builds for
N-Tranced: a header it includes may need a constant N-Tranced doesn't
generate yet. Its pointers are all checked: one to a symbol N-Tranced
doesn't name yet leaves the object out until it does (a table of pointers
only matches anywhere otherwise).
"""
import re
import struct
import subprocess
import sys

B = 0x08000000
XS_BUILD = 'build/crashbandicootxs/'


def map_symbols(path):
    """{name: address} of a linker map's symbols."""
    out = {}
    for line in open(path):
        # a defined symbol, or one the linker script assigns (`name = 0x...`)
        m = re.match(r'\s+0x([0-9a-f]{8})\s+([A-Za-z_][\w.$]*)\s*(?:$|= )', line)
        if m:
            out[m.group(2)] = int(m.group(1), 16)
    return out


def data_objects(path):
    """[(address, size, object)] of the map's data objects' .rodata."""
    out, lines = [], open(path).read().splitlines()
    pat = r'\s+0x(08[0-9a-f]{6})\s+0x([0-9a-f]+)\s+(%s(?:src|lib)/\S*/data/\S+\.o)' % XS_BUILD
    for i, line in enumerate(lines):
        m = re.match(r'\s*\.rodata' + pat, line)
        if not m and re.match(r'\s*\.rodata\s*$', line) and i + 1 < len(lines):
            m = re.match(pat, lines[i + 1])
        if m and int(m.group(2), 16):
            out.append((int(m.group(1), 16), int(m.group(2), 16), m.group(3)))
    return out


def relocations(obj):
    """[(offset, symbol, addend)] of the object's .rodata."""
    text = subprocess.run(['arm-none-eabi-objdump', '-r', '-j', '.rodata', obj],
                          capture_output=True, text=True).stdout
    out = []
    for line in text.splitlines():
        m = re.match(r'^([0-9a-f]{8}) \S+\s+(\S+)$', line)
        if m:
            sym, add = m.group(2), 0
            mm = re.match(r'(.+?)\+0x([0-9a-f]+)$', sym)
            if mm:
                sym, add = mm.group(1), int(mm.group(2), 16)
            out.append((int(m.group(1), 16), sym, add))
    return out


def own_symbols(obj):
    """{name: offset} of what the object defines in its .rodata."""
    text = subprocess.run(['arm-none-eabi-nm', obj], capture_output=True, text=True).stdout
    return {p[2]: int(p[0], 16) for p in (l.split() for l in text.splitlines()) if len(p) == 3 and p[1] in 'RrDd'}


def main():
    n = open(sys.argv[1] if len(sys.argv) > 1 else 'baserom_n2.gba', 'rb').read()
    x = open('crashbandicootxs.gba', 'rb').read()
    nsym = map_symbols('ntranced.map')
    for a, size, obj in data_objects('crashbandicootxs.map'):
        blob, rel = x[a - B:a - B + size], relocations(obj)
        pat, last = b'', 0
        for off, _, _ in sorted(rel):
            pat += re.escape(blob[last:off]) + b'.{4}'
            last = off + 4
        pat += re.escape(blob[last:])
        hits = [m.start() + B for m in re.finditer(pat, n, re.S)][:50]
        own = own_symbols(obj)
        results = []
        for at in hits:
            bad = []
            for off, sym, add in rel:
                got = struct.unpack_from('<I', n, at - B + off)[0]
                want = at + add if sym == '.rodata' else at + own[sym] + add if sym in own else \
                    nsym[sym] + add if sym in nsym else None
                if want is None:
                    bad.append('%s unnamed (%#x)' % (sym, got))
                elif got not in (want, want | 1):  # a Thumb function's has bit 0 set
                    bad.append('%s: %#x, not %#x' % (sym, got, want))
            results.append((at, bad))
        # the places where every pointer is right (a table of pointers
        # only "matches" anywhere: the pointers tell)
        for at, bad in results:
            if not bad:
                print('%-60s %6d bytes, %3d pointers: XS %#x, N-Tranced %#x'
                      % (obj[len(XS_BUILD):], size, len(rel), a, at))


if __name__ == '__main__':
    main()
