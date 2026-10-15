#!/usr/bin/env python3
"""Adds XS's names for N-Tranced's RAM variables and data: RAM to
games/n2/symbols.txt (the linker script's), data in the ROM to
games/n2/data_names.txt (games/n2/disasm.py's labels).

    python3 tools/nt_compare/name_data.py [build/nt_compare]

From run.py's xrefs.json (each aligned literal-pool word of a matched
function pair is a vote for "XS's X is N-Tranced's Y"), a name is added
when at least two votes say so and none says otherwise, it is a symbol and
not `symbol+offset`, and no other XS name maps to the same N-Tranced
address (docs/naming.md: only a confident name). A name either file
already has keeps its address. games/n2/disasm.py then writes these names
in place of the raw words.
"""
import collections, json, re, sys
from pathlib import Path

DIR = Path(sys.argv[1] if len(sys.argv) > 1 else 'build/nt_compare')
SYMBOLS = Path('games/n2/symbols.txt')
DATA_NAMES = Path('games/n2/data_names.txt')
ENTRY = re.compile(r'^(\w+) = (0x[0-9A-Fa-f]+);')
DATA_ENTRY = re.compile(r'^(0x[0-9A-Fa-f]+)\s+(\w+)')
GENERATED = '/* From the XS/N-Tranced comparison (tools/nt_compare/name_data.py). */'


def main():
    data = json.load(open(DIR / 'xrefs.json'))['data']
    by_nt = collections.Counter(e['nt'] for e in data)
    sym_lines = SYMBOLS.read_text().rstrip('\n').split('\n')
    data_lines = DATA_NAMES.read_text().rstrip('\n').split('\n')
    have = {m.group(1): int(m.group(2), 16) for m in map(ENTRY.match, sym_lines) if m}
    have.update({m.group(2): int(m.group(1), 16) for m in map(DATA_ENTRY.match, data_lines) if m})
    taken = set(have.values())
    ram, rom = {}, {}
    for e in data:
        name, addr = e['name'], e['nt']
        if (not name or '+' in name or e['votes'] != e['total'] or e['votes'] < 2
                or by_nt[addr] != 1 or name in have or addr in taken):
            continue
        if addr >> 24 in (0x02, 0x03):
            ram[name] = addr
        elif addr >> 24 in (0x08, 0x09):
            rom[name] = addr
        else:
            continue
        taken.add(addr)

    if GENERATED not in sym_lines:
        sym_lines += ['', GENERATED]
    sym_lines += ['%s = 0x%08X;' % (n, a) for n, a in ram.items()]
    i = sym_lines.index(GENERATED) + 1
    sym_lines[i:] = sorted((l for l in sym_lines[i:] if ENTRY.match(l)),
                           key=lambda l: int(ENTRY.match(l).group(2), 16))
    SYMBOLS.write_text('\n'.join(sym_lines) + '\n')

    head = [l for l in data_lines if not DATA_ENTRY.match(l)]
    entries = [l for l in data_lines if DATA_ENTRY.match(l)] + ['0x%08X %s' % (a, n) for n, a in rom.items()]
    entries.sort(key=lambda l: int(DATA_ENTRY.match(l).group(1), 16))
    DATA_NAMES.write_text('\n'.join(head + entries) + '\n')
    print('added %d names (%d RAM, %d data)' % (len(ram) + len(rom), len(ram), len(rom)), file=sys.stderr)


if __name__ == '__main__':
    main()
