#!/usr/bin/env python3
"""XS's code objects built for N-Tranced, compared with N-Tranced's ROM
(#1116, docs/n_tranced.md "Linking a shared source into N-Tranced").

    python3 tools/nt_compare/n2_objects.py [--min 0.9] [object ...]

For each XS object (match.json's, or those given, `src/.../x.o`) whose
functions are at least --min exact or near (by bytes), builds it as
N-Tranced does (`make GAME=n2 build/ntranced/<object>`: GAME_N2 and
N-Tranced's headers), puts it where the comparison found its first
function, and compares its .text with the ROM there, the words its
relocations fill in aside (BL offsets and literal-pool pointers, which the
link sets). Prints per object:

    match    the whole .text is N-Tranced's: it can be linked as it is
             (objects.txt), once the names it uses are N-Tranced's; with
             the symbols.txt lines its pointers need (a symbol
             ntranced.map doesn't have, at the address N-Tranced's ROM
             has there, less the addend)
    differs  N bytes differ (the first few offsets)
    no build the build fails (the first error)
With --agbcp, as agbcp without -fprologue-bugfix builds them: N-Tranced's
build used it for some objects XS's built with old_agbcp (game.mk).
    linked   already in objects.txt (an object given by name is checked)
    overlaps where the comparison put it, another linked object is: a
             copy of short code that object has

Needs build/nt_compare/match.json (run.py) and XS built.
"""
import argparse
import collections
import json
import re
import subprocess
import sys
from pathlib import Path

B = 0x08000000


def pointers(obj):
    """[(offset, symbol, addend)] of an object's .text R_ARM_ABS32s."""
    rel = subprocess.run(['arm-none-eabi-objdump', '-r', '-j', '.text', str(obj)], capture_output=True, text=True).stdout
    out = []
    for line in rel.splitlines():
        m = re.match(r'^([0-9a-f]{8}) R_ARM_ABS32\s+(\S+)$', line)
        if m:
            sym, add = m.group(2), 0
            mm = re.match(r'(.+?)\+0x([0-9a-f]+)$', sym)
            if mm:
                sym, add = mm.group(1), int(mm.group(2), 16)
            out.append((int(m.group(1), 16), sym, add))
    return out


def map_names():
    """The names ntranced.map has (defined, or assigned by the script)."""
    names = set()
    for line in open('ntranced.map'):
        m = re.match(r'\s+0x[0-9a-f]{8}\s+([A-Za-z_][\w.$]*)\s*(?:$|= )', line)
        if m:
            names.add(m.group(1))
    return names


def text_of(obj):
    """(bytes, [relocated offsets]) of an object's .text."""
    tmp = Path('build/nt_compare/text.bin')
    subprocess.run(['arm-none-eabi-objcopy', '-O', 'binary', '-j', '.text', str(obj), str(tmp)], check=True)
    rel = subprocess.run(['arm-none-eabi-objdump', '-r', '-j', '.text', str(obj)], capture_output=True, text=True).stdout
    offs = []
    for line in rel.splitlines():
        m = re.match(r'^([0-9a-f]{8}) (\S+)', line)
        if m:
            offs.append(int(m.group(1), 16))
    return tmp.read_bytes(), offs


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--min', type=float, default=0.9, help='the share of exact/near bytes (default 0.9)')
    ap.add_argument('--agbcp', action='store_true',
                    help="build them with agbcp without -fprologue-bugfix (game.mk's N2_AGBCP_NO_BUGFIX_OBJS)")
    ap.add_argument('objects', nargs='*')
    args = ap.parse_args()
    rom = open('baserom_n2.gba', 'rb').read()
    rows = json.load(open('build/nt_compare/match.json'))
    by = collections.defaultdict(list)
    for r in rows:
        by[r['obj']].append(r)
    rows = [l.split() for l in open('games/n2/objects.txt') if l.strip() and not l.startswith('#')]
    # objects.txt names the shared sources by their path (engine/src/...),
    # match.json by their object's under the build directory (engine/src/...)
    linked = {r[2].rsplit('.', 1)[0].removeprefix('engine/') for r in rows}
    # the linked objects' ranges: a place inside one is another copy of the
    # same code (a short function the ROM has several of), not this object
    spans = [(int(r[0], 16), int(r[0], 16) + int(r[1], 16), r[2]) for r in rows]
    linked |= {s.replace('games/n2/', '') for s in linked}
    objs = args.objects or sorted(by)
    known = map_names()
    for obj in objs:
        fs = sorted(by[obj], key=lambda r: r['addr'])
        if not fs:
            continue
        good = sum(r['size'] for r in fs if r['kind'] in ('exact', 'near')) / sum(r['size'] for r in fs)
        if not args.objects and good < args.min:
            continue
        if obj[:-2] in linked and not args.objects:  # named ones are checked anyway
            print('%-50s linked' % obj)
            continue
        anchor = next((r for r in fs if r['nt'] > 0), None)
        if anchor is None:
            print('%-50s no place in N-Tranced' % obj)
            continue
        out = 'build/ntranced/' + obj
        # built as if in objects.txt (game.mk's N2_TRY_SRCS): its compiler
        # and flags are then the ones the link would have
        base = 'engine/' + obj[:-2] if obj.startswith('src/') else obj[:-2]
        src = next((s for s in (base + '.cpp', base + '.c') if Path(s).exists()), base + '.cpp')
        Path(out).unlink(missing_ok=True)
        r = subprocess.run(['make', 'GAME=n2', '-s', 'N2_TRY_SRCS=' + src] + (['N2_TRY_AGBCP=' + out] if args.agbcp else [])
                           + [out], capture_output=True, text=True)
        if r.returncode:
            err = [l for l in (r.stdout + r.stderr).splitlines() if 'error' in l.lower() or 'not declared' in l]
            print('%-50s no build: %s' % (obj, (err or ['?'])[0][:110]))
            continue
        text, relocs = text_of(out)
        if not text:
            print('%-50s empty    no .text built for N-Tranced' % obj)
            continue
        # the object's start: the anchor's N-Tranced address less its offset
        # in XS's object (its first function's address in XS is the start)
        at = anchor['nt'] - (anchor['addr'] - fs[0]['addr'])
        mask = set()
        for o in relocs:
            mask.update(range(o, o + 4))
        got = rom[at - B:at - B + len(text)]
        diff = [i for i in range(len(text)) if i not in mask and text[i] != got[i]]
        inside = [s for lo, hi, s in spans if lo < at + len(text) and at < hi and s.rsplit('.', 1)[0] != obj[:-2]]
        if inside:
            print('%-50s overlaps at %#x: %s is linked there' % (obj, at, inside[0]))
            continue
        if not diff:
            print('%-50s match    at %#x, %d bytes' % (obj, at, len(text)))
            need = {}
            for off, sym, add in pointers(out):
                if sym not in known and not sym.startswith('.'):
                    word = int.from_bytes(rom[at - B + off:at - B + off + 4], 'little')
                    need.setdefault(sym, word - add)
            for sym, a in sorted(need.items(), key=lambda x: x[1]):
                print('    %s = 0x%08X;' % (sym, a))
        else:
            print('%-50s differs  at %#x: %d of %d bytes (%s)' % (obj, at, len(diff), len(text),
                                                                 ', '.join('+%#x' % i for i in diff[:4])))


if __name__ == '__main__':
    main()
