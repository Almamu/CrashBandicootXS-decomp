#!/usr/bin/env python3
"""Gives games/n2/gbadisasm.cfg's unnamed functions their XS names.

    python3 tools/nt_compare/name_cfg.py [build/nt_compare]

From run.py's match.json and nt_symbols.txt, and the current disassembly
(games/n2/asm/: the function starts gbadisasm found), a `sub_<address>`
function gets an XS name when (docs/naming.md: only a confident name):

- an XS function that only one N-Tranced address maps to (not `shared`)
  starts there: an exact, near or similar match, or a call target every
  aligned call agrees on (nt_symbols.txt); or
- a near or similar match with a shape score of 0.9 or more lands at most
  8 bytes into it, and no other name does (a match's place is where the
  shapes line up, which can be after a changed prologue).

An exact match where gbadisasm found no function start becomes a new
entry. A function that already has a name keeps it, and a name is only
used once. Run games/n2/disasm.py and `make GAME=n2 compare` after it.
"""
import bisect, collections, json, re, sys
from pathlib import Path

DIR = Path(sys.argv[1] if len(sys.argv) > 1 else 'build/nt_compare')
CFG = Path('games/n2/gbadisasm.cfg')
ASM = Path('games/n2/asm')
LABEL = re.compile(r'^(\w+): @ 0x([0-9A-F]{8})$')
SNAP = 8
MIN_SHAPE = 0.9


def function_starts():
    starts = {}
    for f in sorted(ASM.glob('*.s')):
        lines = f.read_text().split('\n')
        for i, line in enumerate(lines):
            m = LABEL.match(line)
            if m and i and '_func_start' in lines[i - 1]:
                starts[int(m.group(2), 16)] = m.group(1)
    return starts


def main():
    match = {r['name']: r for r in json.load(open(DIR / 'match.json'))}
    starts = function_starts()
    order = sorted(starts)
    cfg = CFG.read_text().split('\n')
    entries = {}  # address -> line index of its thumb_func entry
    for i, line in enumerate(cfg):
        p = line.split()
        if len(p) >= 2 and p[0] == 'thumb_func':
            entries[int(p[1], 16)] = i
    used = set(starts.values()) | {line.split()[2] for line in cfg if len(line.split()) >= 3}

    named = {}  # address -> (name, why)
    snapped = collections.defaultdict(list)
    seeds = {}
    for line in open(DIR / 'nt_symbols.txt'):
        p = line.split()
        addr, name, how = int(p[0], 16), p[1], p[2]
        if how.startswith('data') or 'shared' in p[3:] or name in used:
            continue
        if addr in starts:
            if starts[addr].startswith('sub_'):
                named.setdefault(addr, (name, how))
            continue
        r = match.get(name, {})
        if how == 'exact':
            seeds[addr] = name
            continue
        i = bisect.bisect_right(order, addr) - 1
        if i >= 0 and addr - order[i] <= SNAP and how in ('near', 'similar') \
                and r.get('shape', 0) >= MIN_SHAPE and starts[order[i]].startswith('sub_'):
            snapped[order[i]].append(name)
    for addr, names in snapped.items():
        if len(names) == 1 and addr not in named:
            named[addr] = (names[0], 'snapped')

    taken = set()
    counts = collections.Counter()
    for addr, (name, how) in sorted(named.items()):
        if name in taken:
            continue
        taken.add(name)
        counts[how] += 1
        line = 'thumb_func 0x%x %s' % (addr, name)
        if addr in entries:
            cfg[entries[addr]] = line
        else:
            cfg.append(line)
    for addr, name in sorted(seeds.items()):
        if name not in taken:
            taken.add(name)
            counts['new entry (exact)'] += 1
            cfg.append('thumb_func 0x%x %s' % (addr, name))

    head = [l for l in cfg if l and not l.startswith('thumb_func')]
    funcs = sorted((l for l in cfg if l.startswith('thumb_func')), key=lambda l: int(l.split()[1], 16))
    CFG.write_text('\n'.join(head + funcs) + '\n')
    print('named:', dict(counts), file=sys.stderr)


if __name__ == '__main__':
    main()
