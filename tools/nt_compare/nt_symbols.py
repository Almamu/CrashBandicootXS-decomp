#!/usr/bin/env python3
"""XS names for N-Tranced addresses: functions (match.py, plus call targets
xrefs.py propagated) and data (xrefs.py), one per line:
`<nt addr> <name> <how>`. A name several XS functions would give the same
NT address (shared template code) is listed once per name, marked `shared`.

Usage: python3 tools/nt_compare/nt_symbols.py <match.json> <xrefs.json> > nt_symbols.txt
"""
import collections, json, sys

R = json.load(open(sys.argv[1])); X = json.load(open(sys.argv[2]))
out = {}
for r in R:
    if r['kind'] in ('exact', 'near', 'similar') and r['nt'] > 0 and not r.get('arm'):
        out[r['name']] = (r['nt'], r['kind'])
for c in X['calls']:
    n = c['name']
    if n and '+' not in n and n not in out and c['votes'] >= 2 and c['votes'] == c['total']:
        out[n] = (c['nt'], 'call')
for d in X['data']:
    n = d['name']
    if n and '+' not in n and n not in out and d['votes'] == d['total'] and (d['nt'] >> 24) in (2, 3, 8):
        out[n] = (d['nt'], 'data%d' % d['votes'])
by = collections.Counter(a for a, _ in out.values())
for n, (a, how) in sorted(out.items(), key=lambda x: x[1][0]):
    print('%08x %s %s%s' % (a, n, how, ' shared' if by[a] > 1 else ''))
