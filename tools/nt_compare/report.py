#!/usr/bin/env python3
"""Summarize match.py's output: totals, per directory, per object file.

Usage: python3 tools/nt_compare/report.py <out.json> [--files]
"""
import collections, json, sys

KINDS = ['exact', 'near', 'similar', 'weak', 'none']
R = json.load(open(sys.argv[1]))


def group(obj):
    parts = obj.split('/')
    if parts[0] == 'lib': return 'lib/' + parts[1]
    if parts[0] == 'src': return '/'.join(parts[:3])
    return parts[0]


n = collections.Counter(); b = collections.Counter()
gb = collections.defaultdict(collections.Counter)
fb = collections.defaultdict(collections.Counter)
for r in R:
    n[r['kind']] += 1; b[r['kind']] += r['size']
    gb[group(r['obj'])][r['kind']] += r['size']
    fb[r['obj']][r['kind']] += r['size']
T = sum(b.values())
print('functions:', {k: n[k] for k in KINDS})
print('bytes:    ', {k: f'{b[k]} ({100 * b[k] / T:.1f}%)' for k in KINDS})
print()
print(f"{'directory':24} {'bytes':>7} " + ' '.join(f'{k:>8}' for k in KINDS))
for g in sorted(gb, key=lambda g: -sum(gb[g].values())):
    t = sum(gb[g].values())
    print(f'{g:24} {t:7} ' + ' '.join(f'{100 * gb[g][k] / t:7.0f}%' for k in KINDS))

BUCKETS = ['A shared as-is (>=90% exact/near)', 'B shared with edits (>=70% exact/near/similar)',
           'C partly shared (30-70%)', 'D XS-only or rewritten (<30%)']
rows = []
for f, c in fb.items():
    t = sum(c.values())
    en = (c['exact'] + c['near']) / t
    ens = en + c['similar'] / t
    k = 0 if en >= 0.9 else 1 if ens >= 0.7 else 2 if ens >= 0.3 else 3
    rows.append((k, f, t, en, ens))
print()
for k, name in enumerate(BUCKETS):
    sel = [r for r in rows if r[0] == k]
    print(f'{name:48} files={len(sel):4} bytes={sum(r[2] for r in sel):6} '
          f'({100 * sum(r[2] for r in sel) / T:.0f}%)')
if '--files' in sys.argv:
    print()
    for k, f, t, en, ens in sorted(rows):
        print(f'{"ABCD"[k]} {f:58} {t:6} exact+near={en:.2f} +similar={ens:.2f}')
