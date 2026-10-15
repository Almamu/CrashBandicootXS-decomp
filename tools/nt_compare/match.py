#!/usr/bin/env python3
"""Locate every Crash XS function in Crash 2 N-Tranced.

Per halfword we compute
  - a masked byte value (BL offsets and pointer-looking aligned words zeroed), and
  - a shape token (instruction format/opcode only: no registers, immediates).
A masked match is only "exact" once the raw bytes agree everywhere but the
function's own relocations (verified()): the pointer-looking words the mask
zeroes can be instruction pairs too. Otherwise each XS function is located
in NT by voting on shape 6-gram hits with the
offset delta, then scored at the best location:
  byte  = fraction of halfwords identical after masking (same codegen)
  shape = difflib match of shape tokens (same logic, maybe different regs,
          offsets or small insertions)

Usage (after `make compare`):
  python3 tools/nt_compare/match.py <nt.gba> <out.json>
"""
import re, struct, sys, json, difflib, collections, subprocess

NT, OUT = sys.argv[1:3]
XS, ELF, MAP = 'crashbandicootxs.gba', 'crashbandicootxs.elf', 'crashbandicootxs.map'
xs = open(XS, 'rb').read(); nt = open(NT, 'rb').read()
XS_END = 0x3C000; NT_END = 0x5C800

def masked_hw(b, end):
    n = end // 2
    v = list(struct.unpack_from('<%dH' % n, b, 0))
    m = v[:]
    for i in range(n - 1):
        if (v[i] & 0xF800) == 0xF000 and (v[i + 1] & 0xF800) == 0xF800:
            m[i] = 0xF000; m[i + 1] = 0xF800
    for i in range(0, n - 1, 2):  # aligned words
        lo, hi = v[i], v[i + 1]
        if (hi >> 8) in (0x02, 0x03, 0x04, 0x08, 0x09):
            m[i] = m[i + 1] = 0xDEAD
    return v, m

def tok(h):
    if (h & 0xF800) == 0xF000: return 'BL'
    if (h & 0xF800) == 0xF800: return 'BL2'
    top = h >> 11
    if top <= 2: return 's%d' % top
    if top == 3: return 'as%d' % ((h >> 9) & 3)
    if top <= 7: return 'i%d' % (top - 4)
    if (h & 0xFC00) == 0x4000: return 'a%d' % ((h >> 6) & 0xF)
    if (h & 0xFC00) == 0x4400:
        return 'bx' if (h & 0xFF00) == 0x4700 else 'h%d' % ((h >> 8) & 3)
    if top == 9: return 'lp'
    if (h >> 12) in (5, 6, 7, 8): return 'm%x' % (h >> 9)
    if (h >> 12) == 9: return 'sp%d' % ((h >> 11) & 1)
    if (h >> 12) == 0xA: return 'ad'
    if (h & 0xFF00) == 0xB000: return 'asp'
    if (h & 0xF600) == 0xB400: return 'pop' if h & 0x800 else 'push'
    if (h >> 12) == 0xC: return 'lm%d' % ((h >> 11) & 1)
    if (h >> 12) == 0xD: return 'b%x' % ((h >> 8) & 0xF)
    if top == 0x1C: return 'b'
    return '?'

xv, xm = masked_hw(xs, XS_END); nv, nm = masked_hw(nt, NT_END)
xt = [tok(h) for h in xv]; ntok = [tok(h) for h in nv]
# pool words: tokenize the masked pointer slots as 'P' in both
for arr_m, arr_t in ((xm, xt), (nm, ntok)):
    for i, h in enumerate(arr_m):
        if h == 0xDEAD: arr_t[i] = 'P'
TOKID = {}
xt = [TOKID.setdefault(t, len(TOKID)) for t in xt]
ntok = [TOKID.setdefault(t, len(TOKID)) for t in ntok]

G = 6
gram_pos = collections.defaultdict(list)
for i in range(len(ntok) - G):
    gram_pos[tuple(ntok[i:i + G])].append(i)
# raw masked bytes index for exact hits
nm_bytes = struct.pack('<%dH' % len(nm), *nm)

funcs = []
syms = subprocess.run(['arm-none-eabi-readelf', '-sW', ELF], capture_output=True, text=True, check=True).stdout
for line in syms.splitlines():
    f = line.split()
    if len(f) < 8 or f[3] != 'FUNC': continue
    a, sz, name = int(f[1], 16), int(f[2]), f[7]
    if a < 0x08000000 or sz == 0: continue
    funcs.append(((a & ~1) - 0x08000000, sz, name, a & 1))

objs = [(int(m.group(1), 16) - 0x08000000, int(m.group(2), 16), m.group(3))
        for m in re.finditer(r'^ \.text\s+0x(08[0-9a-f]{6})\s+0x([0-9a-f]+)\s+(\S+)', open(MAP).read(), re.M)]
def owner(a):
    for s, z, p in objs:
        if s <= a < s + z: return p.replace('build/crashbandicootxs/', '')
    return '?'

POINTER_BYTES = (0x02, 0x03, 0x04, 0x08, 0x09)

def verified(off, sz, p):
    """Whether NT's code at p is XS's function at off byte for byte, but for
    what moves with the code and data: BL offsets, and the pool words its
    `ldr rX, [pc, #n]` load that look like pointers."""
    n = (sz + 1) // 2
    skip = set()
    for k in range(n):
        h = xv[off // 2 + k]
        if (h & 0xF800) == 0xF000 and k + 1 < n and (xv[off // 2 + k + 1] & 0xF800) == 0xF800:
            skip.update((k, k + 1))
        elif (h & 0xF800) == 0x4800:
            w = (((off + 2 * k + 4) & ~3) + (h & 0xFF) * 4 - off) // 2
            if 0 <= w < n - 1 and xs[off + 2 * w + 3] in POINTER_BYTES:
                skip.update((w, w + 1))
    return all(xv[off // 2 + k] == nv[p // 2 + k] for k in range(n) if k not in skip)

res = []
for off, sz, name, thumb in funcs:
    r = dict(name=name, addr=off + 0x08000000, size=sz, obj=owner(off))
    i0, n = off // 2, (sz + 1) // 2
    if not thumb:
        j = nt.find(xs[off:off + sz], 0, NT_END)
        r.update(kind='exact' if j >= 0 else 'none', nt=j, byte=1.0 if j >= 0 else 0, shape=1.0 if j >= 0 else 0, arm=True)
        res.append(r); continue
    pat = struct.pack('<%dH' % n, *xm[i0:i0 + n])
    j = -1
    if n >= 4:
        k = nm_bytes.find(pat)
        while k >= 0 and (k % 2 or not verified(off, sz, k)): k = nm_bytes.find(pat, k + 1)
        j = k
    if j >= 0:
        r.update(kind='exact', nt=j + 0x08000000, byte=1.0, shape=1.0,
                 dup=nm_bytes.count(pat))
        res.append(r); continue
    # vote
    votes = collections.Counter()
    seq = xt[i0:i0 + n]
    for k in range(0, max(1, n - G)):
        ps = gram_pos.get(tuple(seq[k:k + G]), ())
        if len(ps) > 60: continue
        for p in ps: votes[p - k] += 1
    best = None
    for d, v in votes.most_common(5):
        if d < 0: continue
        win = ntok[d:d + int(n * 1.4) + 8]
        sm = difflib.SequenceMatcher(None, seq, win, autojunk=False)
        blocks = sm.get_matching_blocks()
        mt = sum(b.size for b in blocks)
        # end of the matched NT extent, to size the NT function
        last = max((b.b + b.size for b in blocks if b.size), default=0)
        shape = 2 * mt / (n + last) if last else 0
        same = sum(1 for k in range(n) if d + k < len(nm) and xm[i0 + k] == nm[d + k])
        cand = (shape, same / n, d, last)
        if best is None or cand > best: best = cand
    if best is None:
        r.update(kind='none', nt=-1, byte=0, shape=0)
    else:
        shape, byte, d, last = best
        r.update(nt=d * 2 + 0x08000000, byte=round(byte, 3), shape=round(shape, 3), ntsize=last * 2)
        r['kind'] = ('near' if shape >= 0.95 else 'similar' if shape >= 0.80 else
                     'weak' if shape >= 0.60 else 'none')
    res.append(r)

# ---- pass 2: ordered anchors ----
import bisect
cand = [r for r in res if not r.get('arm') and r['size'] >= 24 and r['nt'] > 0 and
        ((r['kind'] == 'exact' and r.get('dup', 1) == 1) or (r['shape'] >= 0.97 and r['byte'] >= 0.9))]
cand.sort(key=lambda r: r['addr'])
# longest increasing subsequence on nt
tails, prev, idxs = [], [None] * len(cand), []
for i, r in enumerate(cand):
    k = bisect.bisect_left([cand[t]['nt'] for t in tails], r['nt'])
    if k == len(tails): tails.append(i)
    else: tails[k] = i
    prev[i] = tails[k - 1] if k else None
chain = []; i = tails[-1] if tails else None
while i is not None: chain.append(cand[i]); i = prev[i]
chain.reverse()
for r in chain: r['anchor'] = True
anc_x = [r['addr'] for r in chain]
print('anchors', len(chain), 'of', len(cand), file=sys.stderr)

def local_search(r):
    k = bisect.bisect_left(anc_x, r['addr'])
    lo = (chain[k - 1]['nt'] + chain[k - 1]['size']) if k else 0x08000000
    hi = chain[k]['nt'] if k < len(chain) else 0x08000000 + NT_END
    lo = (lo - 0x08000000) // 2; hi = (hi - 0x08000000) // 2
    if hi <= lo or hi - lo > 40000: return None
    off = r['addr'] - 0x08000000; i0, n = off // 2, (r['size'] + 1) // 2
    seq = xt[i0:i0 + n]; mseq = xm[i0:i0 + n]
    # exact masked inside window
    for p in range(lo, hi - n + 1):
        if nm[p:p + n] == mseq and verified(off, r["size"], 2 * p): return (1.0, 1.0, p, n, True)
    g = 4 if n >= 8 else 2
    grams = collections.defaultdict(list)
    for k2 in range(n - g + 1): grams[tuple(seq[k2:k2 + g])].append(k2)
    votes = collections.Counter()
    for p in range(lo, hi - g + 1):
        for k2 in grams.get(tuple(ntok[p:p + g]), ()): votes[p - k2] += 1
    best = None
    for d, v in votes.most_common(8):
        if d < lo: continue
        win = ntok[d:min(d + int(n * 1.4) + 8, hi)]
        sm = difflib.SequenceMatcher(None, seq, win, autojunk=False)
        bl = sm.get_matching_blocks(); mt = sum(b.size for b in bl)
        last = max((b.b + b.size for b in bl if b.size), default=0)
        shape = 2 * mt / (n + last) if last else 0
        same = sum(1 for q in range(n) if d + q < len(nm) and mseq[q] == nm[d + q]) / n
        c = (shape, same, d, last, False)
        if best is None or c > best: best = c
    return best

for r in res:
    if r.get('arm') or r.get('anchor'): continue
    if r['kind'] == 'exact' and r.get('dup', 1) == 1: continue
    b = local_search(r)
    if b is None: continue
    shape, byte, d, last, ex = b
    if ex:
        r.update(kind='exact', nt=d * 2 + 0x08000000, shape=1.0, byte=1.0, local=True); continue
    if r['kind'] != 'exact' and shape > r['shape'] - 0.02:
        r.update(nt=d * 2 + 0x08000000, shape=round(shape, 3), byte=round(byte, 3), ntsize=last * 2, local=True)
        r['kind'] = ('near' if shape >= 0.95 else 'similar' if shape >= 0.80 else
                     'weak' if shape >= 0.60 else 'none')
json.dump(res, open(OUT, 'w'), indent=0)
