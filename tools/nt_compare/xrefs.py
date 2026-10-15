#!/usr/bin/env python3
"""XS -> N-Tranced address map from matched function pairs.

For every function match.py placed (exact/near/similar), the two
instruction streams are aligned on their shape tokens; at each aligned
`ldr rX, [pc, #n]` the two pool words are paired, and at each aligned BL
the two call targets. Every pair is a vote. Calls also propagate: a called
XS function gets the NT target as its address.

Usage: python3 tools/nt_compare/xrefs.py <nt.gba> <match.json> <out.json>
"""
import collections, difflib, json, struct, subprocess, sys

NT, MATCH, OUT = sys.argv[1:4]
xs = open('crashbandicootxs.gba', 'rb').read(); nt = open(NT, 'rb').read()
R = json.load(open(MATCH))


def hw(b, o): return b[o] | (b[o + 1] << 8)


def tok(h):
    if (h & 0xF800) == 0xF000: return 'BL'
    if (h & 0xF800) == 0xF800: return 'BL2'
    if (h >> 11) == 9: return 'lp'
    return h >> 11


def insns(b, off, n):
    return [tok(hw(b, off + 2 * k)) for k in range(n)]


def bl_target(b, o):
    h, h2 = hw(b, o), hw(b, o + 2)
    v = ((h & 0x7FF) << 12) | ((h2 & 0x7FF) << 1)
    if v & 0x400000: v -= 0x800000
    return 0x08000000 + o + 4 + v


def pool(b, o):
    h = hw(b, o)
    a = ((o + 4) & ~3) + (h & 0xFF) * 4
    return struct.unpack_from('<I', b, a)[0]


# XS symbols, to name what the pointers point at
syms = {}
out = subprocess.run(['arm-none-eabi-nm', '-n', 'crashbandicootxs.elf'], capture_output=True, text=True).stdout
for line in out.splitlines():
    f = line.split()
    if len(f) == 3 and f[1] in 'TtDdBbRr':
        syms.setdefault(int(f[0], 16), f[2])
sym_addrs = sorted(syms)

data = collections.defaultdict(collections.Counter)
calls = collections.defaultdict(collections.Counter)
for r in R:
    if r['kind'] not in ('exact', 'near', 'similar') or r.get('arm') or r['nt'] <= 0: continue
    xo = r['addr'] - 0x08000000; no = r['nt'] - 0x08000000
    n = r['size'] // 2; m = (r.get('ntsize') or r['size']) // 2
    a, b = insns(xs, xo, n), insns(nt, no, m)
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    for blk in sm.get_matching_blocks():
        for k in range(blk.size):
            i, j = blk.a + k, blk.b + k
            if a[i] == 'lp':
                px, pn = pool(xs, xo + 2 * i), pool(nt, no + 2 * j)
                if px != pn or (px >> 24) in (8, 9):
                    data[px][pn] += 1
            elif a[i] == 'BL' and i + 1 < n and a[i + 1] == 'BL2':
                calls[bl_target(xs, xo + 2 * i)][bl_target(nt, no + 2 * j)] += 1


def name(a):
    import bisect
    if a in syms: return syms[a]
    if a | 1 in syms: return syms[a | 1]
    i = bisect.bisect_right(sym_addrs, a) - 1
    if i >= 0 and a - sym_addrs[i] < 0x10000: return '%s+0x%x' % (syms[sym_addrs[i]], a - sym_addrs[i])
    return None


res = {'data': [], 'calls': []}
for px, c in sorted(data.items()):
    (pn, v), = c.most_common(1)
    res['data'].append(dict(xs=px, nt=pn, votes=v, total=sum(c.values()), alts=len(c), name=name(px)))
for px, c in sorted(calls.items()):
    (pn, v), = c.most_common(1)
    res['calls'].append(dict(xs=px, nt=pn, votes=v, total=sum(c.values()), name=name(px)))
json.dump(res, open(OUT, 'w'), indent=0)
rom = [d for d in res['data'] if (d['xs'] >> 24) == 8]
print('data pointers', len(res['data']), 'ROM', len(rom), 'calls', len(res['calls']))
