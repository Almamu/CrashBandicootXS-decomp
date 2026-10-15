#!/usr/bin/env python3
"""The whole XS/N-Tranced comparison, into build/nt_compare/.

    python3 tools/nt_compare/run.py [baserom_n2.gba]

Builds XS if it isn't (`make crashbandicootxs.gba`), then runs every step
(README.md) and writes:

  match.json       each XS function's place in N-Tranced (match.py)
  report.txt       the totals, per directory and per file (report.py --files)
  xrefs.json       the XS -> N-Tranced address map (xrefs.py)
  nt_symbols.txt   XS names for N-Tranced addresses (nt_symbols.py)
  new_code.txt     N-Tranced's code with no XS counterpart (nt_new_code.py --list)
  rooms.txt        N-Tranced's rooms through levels.py's codec (rooms_roundtrip.py)
  stabs/, dwarf/   the XS objects with debug info (debug_build.py)
  structs.json     the struct members' offsets in both games (struct_diff.py)
  structs.txt      per struct (struct_report.py)

Only build/nt_compare/ is written; build/crashbandicootxs/ is only read
(and built, if it isn't).
"""
import json, subprocess, sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
OUT = Path('build/nt_compare')


def step(what, args, out=None):
    print('nt_compare: %s' % what, file=sys.stderr)
    cmd = [sys.executable, str(HERE / args[0])] + [str(a) for a in args[1:]]
    if out:
        with open(OUT / out, 'w') as f:
            subprocess.run(cmd, stdout=f, check=True)
    else:
        subprocess.run(cmd, check=True)


def main():
    nt = Path(sys.argv[1] if len(sys.argv) > 1 else 'baserom_n2.gba')
    if not nt.exists():
        sys.exit('nt_compare: %s is missing (the N-Tranced ROM, Europe)' % nt)
    OUT.mkdir(parents=True, exist_ok=True)
    # the XS objects, generated headers and debug builds below need it
    step('the XS objects with debug info (and the XS build)', ['debug_build.py', OUT])
    step('XS functions in N-Tranced', ['match.py', nt, OUT / 'match.json'])
    step('report', ['report.py', OUT / 'match.json', '--files'], 'report.txt')
    step('address map', ['xrefs.py', nt, OUT / 'match.json', OUT / 'xrefs.json'])
    step('names', ['nt_symbols.py', OUT / 'match.json', OUT / 'xrefs.json'], 'nt_symbols.txt')
    step('new code', ['nt_new_code.py', nt, OUT / 'match.json', '--list'], 'new_code.txt')
    table = [d['nt'] for d in json.load(open(OUT / 'xrefs.json'))['data'] if d['name'] == 'gLevelTable']
    if table:
        step('rooms', ['rooms_roundtrip.py', nt, '%x' % table[0]], 'rooms.txt')
    else:
        print('nt_compare: gLevelTable not found in N-Tranced, rooms skipped', file=sys.stderr)
    step('structs', ['struct_diff.py', nt, OUT / 'match.json', OUT / 'stabs', OUT / 'structs.json', OUT / 'dwarf'])
    step('structs report', ['struct_report.py', OUT / 'structs.json'], 'structs.txt')
    print('nt_compare: done, see %s/' % OUT, file=sys.stderr)


if __name__ == '__main__':
    main()
