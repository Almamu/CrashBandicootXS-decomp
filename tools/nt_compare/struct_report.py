#!/usr/bin/env python3
"""Per-struct summary of struct_diff.py's output.

For each struct: its members in XS order with the N-Tranced offset the
code uses (most votes), runs of members with the same shift, and where
the shift changes (bytes inserted, or a reorder). An N-Tranced offset of
0 for a member that isn't at 0 in XS is the compiler addressing a member
past the Thumb immediate range (reg + #0 after an add), not a real
offset, and is ignored.

Usage: python3 tools/nt_compare/struct_report.py <structs.json> [--all]
"""
import collections, json, sys

d = json.load(open(sys.argv[1]))
by = collections.defaultdict(list)
for e in d['members']:
    by[e['struct']].append(e)
sizes = {k: v for k, v in d['sizes'].items()}


def principal(e):
    obs = [(y, v) for y, v in e['nt'] if not (y == 0 and e['xs'] != 0)]
    if not obs: return None, 0, sum(v for _, v in e['nt'])
    tot = sum(v for _, v in obs)
    y, v = max(obs, key=lambda x: x[1])
    return y, v, tot


summary = collections.Counter()
lines = []
for s in sorted(by):
    ms = sorted(by[s], key=lambda e: e['xs'])
    rows = []
    for e in ms:
        y, v, tot = principal(e)
        rows.append((e, y, v, tot))
    # confident: 2+ votes and a 60% majority; the rest are shown but don't
    # decide the struct's kind or the insertion points
    conf = lambda y, v, tot: y is not None and v >= 2 and v >= 0.6 * tot
    known = [(e, y) for e, y, v, tot in rows if conf(y, v, tot)]
    deltas = [y - e['xs'] for e, y in known]
    sz = [x for x in sizes.get(s, []) if x[0] is not None and x[1] is not None]
    size_txt = ' size %d -> %d' % (sz[0][0], sz[0][1]) if sz else ''
    if not known:
        kind = 'unknown'
    elif len(known) < 2 and not (sz and sz[0][0] != sz[0][1]):
        kind = 'too little evidence' if not deltas or deltas[0] == 0 else 'shifted (1 member)'
    elif all(x == 0 for x in deltas):
        kind = 'unchanged'
    elif len(set(deltas)) == 1:
        kind = 'shifted'
    elif deltas == sorted(deltas):
        kind = 'fields inserted'
    else:
        kind = 'reordered'
    if sz and sz[0][0] != sz[0][1] and kind == 'unchanged':
        kind = 'grown at the end'
    summary[kind] += 1
    lines.append('== %s: %s (%d members seen)%s' % (s, kind, len(ms), size_txt))
    prev = None
    for e, y, v, tot in rows:
        note = ''
        if y is None:
            note = 'moved past 0x7c (only reg+#0 seen)'
        elif not conf(y, v, tot):
            note = '(low confidence: %d/%d votes)' % (v, tot)
        else:
            dl = y - e['xs']
            if prev is not None and dl != prev:
                note = ('<- +%#x inserted before' % (dl - prev)) if dl > prev else ('<- %#x (reordered/removed before)' % (dl - prev))
            prev = dl
            note += ' [%d votes]' % v if v == tot else ' [%d/%d votes]' % (v, tot)
        ys = '%#06x' % y if y is not None else '  ?   '
        lines.append('   %-30s %#06x -> %s %s' % (e['member'], e['xs'], ys, note))

print('structs by kind:', dict(summary))
print()
print('\n'.join(lines))
