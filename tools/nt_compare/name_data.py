#!/usr/bin/env python3
"""Adds XS's names for N-Tranced's RAM variables and data to
games/n2/symbols.txt.

    python3 tools/nt_compare/name_data.py [build/nt_compare]

From run.py's xrefs.json (each aligned literal-pool word of a matched
function pair is a vote for "XS's X is N-Tranced's Y"), a name is added
when at least two votes say so and none says otherwise, it is a symbol and
not `symbol+offset`, and no other XS name maps to the same N-Tranced
address (docs/naming.md: only a confident name). A name symbols.txt
already has keeps its address. games/n2/disasm.py then writes these names
in place of the raw words.
"""
import collections, json, re, sys
from pathlib import Path

DIR = Path(sys.argv[1] if len(sys.argv) > 1 else 'build/nt_compare')
SYMBOLS = Path('games/n2/symbols.txt')
ENTRY = re.compile(r'^(\w+) = (0x[0-9A-Fa-f]+);')
GENERATED = '/* From the XS/N-Tranced comparison (tools/nt_compare/name_data.py). */'


def main():
    data = json.load(open(DIR / 'xrefs.json'))['data']
    by_nt = collections.Counter(e['nt'] for e in data)
    lines = SYMBOLS.read_text().split('\n')
    have = {m.group(1): int(m.group(2), 16) for m in map(ENTRY.match, lines) if m}
    taken = set(have.values())
    new = {}
    for e in data:
        name = e['name']
        if (not name or '+' in name or e['votes'] != e['total'] or e['votes'] < 2 or by_nt[e['nt']] != 1
                or (e['nt'] >> 24) not in (0x02, 0x03, 0x08, 0x09)):
            continue
        if name in have or e['nt'] in taken:
            continue
        new[name] = e['nt']
        taken.add(e['nt'])
    if GENERATED not in lines:
        lines += ['', GENERATED]
    lines = [l for l in lines if l != '']
    head = lines[:lines.index(GENERATED) + 1]
    gen = [l for l in lines[lines.index(GENERATED) + 1:] if ENTRY.match(l)]
    gen += ['%s = 0x%08X;' % (n, a) for n, a in new.items()]
    gen.sort(key=lambda l: int(ENTRY.match(l).group(2), 16))
    # the hand-written header and entries first, then the generated block
    out = []
    for l in head:
        out.append(l)
        if l.endswith('*/') and l is not head[-1]:
            out.append('')
    SYMBOLS.write_text('\n'.join(out[:-1] + ['', GENERATED] + gen) + '\n')
    print('added %d names (%d RAM, %d ROM)' % (
        len(new), sum(1 for a in new.values() if a >> 24 in (2, 3)), sum(1 for a in new.values() if a >> 24 in (8, 9))),
        file=sys.stderr)


if __name__ == '__main__':
    main()
