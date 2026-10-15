#!/usr/bin/env python3
"""How much of N-Tranced's code has no XS counterpart, and where it sits.

N-Tranced's functions are the BL targets and code pointers that start with
a push (a function's end is the next one's start). Each is "from XS" when
an exact/near/similar XS match covers most of it, "loosely" for a weak
one, else "new". New code is attributed to the XS object of the nearest
XS-derived function before it in the ROM (link order is preserved), i.e.
the source file it was linked next to.

Usage: python3 tools/nt_compare/nt_new_code.py <nt.gba> <match.json> [--list]
"""
import bisect, collections, json, struct, sys

NT, MATCH = sys.argv[1:3]
nt = open(NT, 'rb').read(); R = json.load(open(MATCH))
END = 0x5C800

def hw(o): return nt[o] | (nt[o + 1] << 8)

starts = set()
for o in range(0, END - 3, 2):
    h, h2 = hw(o), hw(o + 2)
    if (h & 0xF800) == 0xF000 and (h2 & 0xF800) == 0xF800:
        v = ((h & 0x7FF) << 12) | ((h2 & 0x7FF) << 1)
        if v & 0x400000: v -= 0x800000
        t = o + 4 + v
        if 0x200 <= t < END and (hw(t) & 0xFE00) == 0xB400: starts.add(t)
for o in range(0, len(nt) - 3, 4):
    w = struct.unpack_from('<I', nt, o)[0]
    if w & 1 and 0x08000200 <= w < 0x08000000 + END and (hw((w & ~1) - 0x08000000) & 0xFE00) == 0xB400:
        starts.add((w & ~1) - 0x08000000)
# matched NT starts are function starts too
cover = {}
for r in R:
    if r['nt'] > 0 and not r.get('arm') and r['kind'] in ('exact', 'near', 'similar', 'weak'):
        s = r['nt'] - 0x08000000
        starts.add(s)
        n = r.get('ntsize') or r['size']
        rank = {'exact': 3, 'near': 3, 'similar': 3, 'weak': 1}[r['kind']]
        for k in range(s, s + n, 2):
            if cover.get(k, (0,))[0] < rank: cover[k] = (rank, r['obj'])
starts = sorted(starts)

funcs = []
for i, s in enumerate(starts):
    e = starts[i + 1] if i + 1 < len(starts) else END
    e = min(e, s + 0x4000)
    halves = range(s, e, 2)
    strong = sum(1 for k in halves if cover.get(k, (0,))[0] == 3)
    weak = sum(1 for k in halves if cover.get(k, (0,))[0] >= 1)
    n = len(halves)
    kind = 'from XS' if strong >= 0.5 * n else 'loosely' if weak >= 0.5 * n else 'new'
    objs = collections.Counter(cover[k][1] for k in halves if k in cover)
    funcs.append(dict(start=s, size=e - s, kind=kind, obj=objs.most_common(1)[0][0] if objs else None))

last = None
for f in funcs:
    if f['kind'] == 'from XS': last = f['obj']
    f['near'] = f['obj'] or last

tot = collections.Counter(); totb = collections.Counter()
for f in funcs:
    tot[f['kind']] += 1; totb[f['kind']] += f['size']
B = sum(totb.values())
print('NT functions:', dict(tot))
print('NT bytes:    ', {k: '%d (%.0f%%)' % (v, 100 * v / B) for k, v in totb.items()})
print()
area = collections.defaultdict(collections.Counter)
for f in funcs:
    if f['kind'] == 'new':
        o = (f['near'] or '?').split('/')
        key = '/'.join(o[:3]) if o[0] == 'src' else '/'.join(o[:2])
        area[key]['n'] += 1; area[key]['bytes'] += f['size']
print('new NT code, by the XS directory it is linked next to:')
for k, c in sorted(area.items(), key=lambda x: -x[1]['bytes']):
    print('  %-28s %4d functions %7d bytes' % (k, c['n'], c['bytes']))
if '--list' in sys.argv:
    obj = collections.defaultdict(collections.Counter)
    for f in funcs:
        if f['kind'] == 'new':
            obj[f['near']]['n'] += 1; obj[f['near']]['bytes'] += f['size']
    print()
    for k, c in sorted(obj.items(), key=lambda x: -x[1]['bytes']):
        print('  %-58s %4d functions %7d bytes' % (k, c['n'], c['bytes']))
