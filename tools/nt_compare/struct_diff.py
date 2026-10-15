#!/usr/bin/env python3
"""Which struct members moved between XS and N-Tranced, and where to.

Needs the XS objects rebuilt with -gstabs (same code, plus stabs; see
README.md) under <stabs dir>/src/... For every function pair match.py
placed, the two instruction streams are aligned on shape tokens; at each
aligned immediate-offset load/store (same opcode and registers) we have
the XS offset and the NT offset. The XS source line (stabs SLINE/SOL)
and the struct layouts (stabs type definitions) tell which member it is:
a member at that offset, of that access size, whose name is on the line.

Usage: python3 tools/nt_compare/struct_diff.py <nt.gba> <match.json> <stabs dir> <out.json> <dwarf dir>
"""
import bisect, collections, difflib, json, os, re, subprocess, sys

NT, MATCH, SDIR, OUT = sys.argv[1:5]
xs = open('crashbandicootxs.gba', 'rb').read(); nt = open(NT, 'rb').read()
R = json.load(open(MATCH))

# ---------------------------------------------------------------- stabs types
class Parser:
    def __init__(self):
        self.types = {}  # number -> ('struct', name, size, [(name, typeno, bitpos, bits)]) or other
        self.structs = {}  # name -> typeno

    def num(self, s, i):
        if s[i] == '(':
            j = s.index(')', i); return s[i:j + 1], j + 1
        j = i
        while j < len(s) and (s[j].isdigit() or s[j] == '-'): j += 1
        return s[i:j], j

    def typ(self, s, i, name=None):
        """Parse a type at s[i]; returns (typeno or None, i)."""
        if s[i].isdigit() or s[i] == '(':
            n, i = self.num(s, i)
            if i < len(s) and s[i] == '=':
                d, i = self.defn(s, i + 1, name)
                self.types[n] = d
                if d and d[0] == 'struct' and name: self.structs.setdefault(name, n)
            return n, i
        d, i = self.defn(s, i, name)
        return None, i

    def defn(self, s, i, name):
        c = s[i]
        while c == '@':  # type attributes: @s32; etc.
            i = s.index(';', i) + 1; c = s[i]
        if c in 'su':
            i += 1
            size, i = self.num(s, i)
            members = []
            if i < len(s) and s[i] == '!':  # base classes: !n,virt access offset type;
                i += 1
                cnt, i = self.num(s, i); i += 1
                for _ in range(int(cnt)):
                    i += 2
                    off, i = self.num(s, i); i += 1
                    t, i = self.typ(s, i)
                    members.append(('<base>', t, int(off), 0))
                    i += 1
            while i < len(s) and s[i] != ';':
                j = s.index(':', i)
                mname = s[i:j]
                if s[j + 1] == ':':  # methods: give up on the rest of this type
                    return ('struct', name, int(size), members), len(s)
                i = j + 1
                if s[i] == '/': i += 2  # visibility
                t, i = self.typ(s, i)
                if s[i] == ':':  # static member: name:type:physname;
                    i = s.index(';', i) + 1; continue
                i += 1
                bp, i = self.num(s, i); i += 1
                bs, i = self.num(s, i); i += 1
                members.append((mname, t, int(bp), int(bs)))
            return ('struct' if c == 's' else 'union', name, int(size), members), i + 1
        if c in '*&kB':
            t, i = self.typ(s, i + 1); return ('ptr' if c in '*&' else 'q', t), i
        if c == 'a':
            i += 1
            _, i = self.typ(s, i + 1)  # index type r...
            i += 1
            lo, i = self.num(s, i); i += 1
            hi, i = self.num(s, i); i += 1
            t, i = self.typ(s, i)
            return ('array', t, int(lo), int(hi)), i
        if c == 'r':
            _, i = self.typ(s, i + 1); i += 1
            while s[i] != ';': i += 1
            i += 1
            while s[i] != ';': i += 1
            return ('int',), i + 1
        if c == 'e':
            i = s.index(';;', i) + 2; return ('enum',), i
        if c == 'x':
            j = s.index(':', i); return ('xref', s[i + 2:j]), j + 1
        if c == 'f':
            t, i = self.typ(s, i + 1); return ('func', t), i
        if c == '#':
            return ('method',), len(s)
        if c.isdigit() or c == '(':
            t, i = self.typ(s, i); return ('alias', t), i
        return None, len(s)


def stabs_strings(path):
    out, cur = [], ''
    for line in open(path, errors='replace'):
        m = re.match(r'\s*\.stabs\s+"(.*)",(\d+),', line)
        if not m: continue
        st = m.group(1).replace('\\\\', '\\')
        if st.endswith('\\'):
            cur += st[:-1]; continue
        out.append((cur + st, int(m.group(2)))); cur = ''
    return out


def parse_types(spath):
    p = Parser()
    for st, kind in stabs_strings(spath):
        if kind != 128: continue  # LSYM
        m = re.match(r'([^:]*):([Tt])', st)
        if not m: continue
        try:
            p.typ(st, m.end(), m.group(1) or None)
        except (ValueError, IndexError):
            pass
    return p


def resolve(p, t, depth=0):
    while depth < 20:
        d = p.types.get(t)
        if not d: return None
        if d[0] in ('alias', 'q'): t = d[1]; depth += 1; continue
        if d[0] == 'xref':
            n = p.structs.get(d[1])
            if n is None or n == t: return None
            t = n; depth += 1; continue
        return d
    return None


def leaves(p, sname, t):
    """(owner struct, member, offset in owner, offset in sname, size) of
    every scalar member, flattened through bases and struct members."""
    out = []
    def walk(t, base, owner, obase, prefix, depth):
        d = resolve(p, t)
        if not d or depth > 6: return
        if d[0] in ('struct', 'union'):
            own = d[1] or owner
            ob = base if d[1] else obase
            for mname, mt, bp, bs in d[3]:
                if bp % 8: continue
                md = resolve(p, mt)
                name = prefix + mname if d[1] is None else mname
                if md and md[0] in ('struct', 'union'):
                    if md[1]:
                        walk(mt, base + bp // 8, own, ob, '', depth + 1)
                    else:  # anonymous: its members belong to `own`
                        walk(mt, base + bp // 8, own, ob, name + '.' if mname else '', depth + 1)
                elif md and md[0] == 'array':
                    ed = resolve(p, md[1])
                    if ed and ed[0] in ('struct', 'union'):
                        walk(md[1], base + bp // 8, own, ob, '', depth + 1)
                    else:
                        n = md[3] - md[2] + 1
                        es = bs // 8 // n if n else 0
                        for k in range(min(n, 64)):
                            out.append((own, '%s[%d]' % (name, k), base + bp // 8 + k * es - ob,
                                        base + bp // 8 + k * es, es))
                elif bs:
                    out.append((own, name, base + bp // 8 - ob, base + bp // 8, bs // 8))
    walk(t, 0, sname, 0, '', 0)
    return out


# ------------------------------------------------------------- line tables
def line_table(obj):
    txt = subprocess.run(['arm-none-eabi-objdump', '-G', obj], capture_output=True, text=True).stdout
    rows, cur = [], None
    for l in txt.splitlines():
        f = l.split(None, 6)
        if len(f) < 6 or not f[0].isdigit(): continue
        typ = f[1]
        if typ in ('SO', 'SOL') and len(f) == 7: cur = f[6].strip()
        elif typ == 'SLINE': rows.append((int(f[4], 16), int(f[3]), cur))
    rows.sort()
    return rows


# --------------------------------------------------------------- the ROM side
def hw(b, o): return b[o] | (b[o + 1] << 8)

def tok(h):
    if (h & 0xF800) == 0xF000: return 'BL'
    if (h & 0xF800) == 0xF800: return 'BL2'
    return h >> 11 if (h >> 11) not in (0xC, 0xD, 0xE, 0xF, 0x10, 0x11) else ('ls', h >> 11, h & 0x3F)

LS = {0xC: ('str', 4), 0xD: ('ldr', 4), 0xE: ('strb', 1), 0xF: ('ldrb', 1), 0x10: ('strh', 2), 0x11: ('ldrh', 2)}

def ls(h):
    t = h >> 11
    if t not in LS: return None
    op, size = LS[t]
    return op, size, ((h >> 6) & 31) * size, (h & 7, (h >> 3) & 7)

def dest(h):
    """The low register an instruction writes, if any (None = none or unknown)."""
    top = h >> 11
    if top <= 3 or (h & 0xFC00) == 0x4000 or (h >> 12) in (5, 6, 7, 8) and (h & 0x0800 or (h >> 12) == 5 and (h & 0x0E00) in (0x0600, 0x0800, 0x0A00, 0x0C00, 0x0E00)):
        return h & 7
    if 4 <= top <= 7 or top == 9 or (h >> 12) == 9 and h & 0x800 or (h >> 12) == 0xA:
        return (h >> 8) & 7
    return None


def effective(b, o, reg, imm):
    """imm plus the `adds reg, #k` folded into the base just before o."""
    for k in range(2, 14, 2):
        h = hw(b, o - k)
        if (h & 0xF800) == 0x3000 and ((h >> 8) & 7) == reg: return imm + (h & 0xFF)
        if (h & 0xFE00) == 0x1C00 and (h & 7) == reg: return imm + ((h >> 6) & 7)
        if (h >> 12) in (0xD, 0xE, 0xF) or (h & 0xFF00) == 0x4700: return imm
        if dest(h) == reg: return imm
    return imm


objs = [(int(m.group(1), 16), int(m.group(2), 16), m.group(3))
        for m in re.finditer(r'^ \.text\s+0x(08[0-9a-f]{6})\s+0x([0-9a-f]+)\s+(\S+)', open('crashbandicootxs.map').read(), re.M)]
objs.sort()

def obj_funcs(o):
    out = []
    for l in subprocess.run(['arm-none-eabi-nm', '-n', o], capture_output=True, text=True).stdout.splitlines():
        f = l.split()
        if len(f) == 3 and f[1] in 'Tt' and not f[2].startswith('.'): out.append((int(f[0], 16), f[2]))
    return out

cache = {}
def objinfo(a):
    i = bisect.bisect_right([o[0] for o in objs], a) - 1
    start, size, path = objs[i]
    rel = path.replace('build/crashbandicootxs/', '')
    if rel not in cache:
        o = os.path.join(SDIR, rel); s = o[:-2] + '.s'
        cache[rel] = (obj_funcs(o), line_table(o)) if os.path.exists(o) else None
    return start, rel, cache[rel]

src_cache = {}
def src_line(path, n):
    if path not in src_cache:
        try: src_cache[path] = open(path, errors='replace').read().split('\n')
        except OSError: src_cache[path] = []
    L = src_cache[path]
    return L[n - 1] if 0 < n <= len(L) else ''

# One catalogue of every struct, from every object (g++ 2.9 emits a class's
# layout only in some of the objects that use it): name -> leaves.
sys.path.insert(0, os.path.dirname(__file__))
import dwarf_types
DW = sys.argv[5] if len(sys.argv) > 5 else SDIR
CATALOGUE = dwarf_types.catalogue([os.path.join(dp, f) for dp, _, fs in os.walk(os.path.join(DW, 'src'))
                                   for f in fs if f.endswith('.dw')])
BY_OFF = collections.defaultdict(list)
for name, lv in CATALOGUE.items():
    for own, mem, rel, off, size in lv:
        BY_OFF[(off, size)].append((name, own, mem, rel))


def method_class(mangled):
    """The class of a g++ 2.x mangled method name: Name__Q34Game5Crash10ActionCtrl..., Name__4Font..."""
    i = mangled.find('__', 1)
    if i < 0: return None
    s = mangled[i + 2:]
    if s.startswith('Q'):
        m = re.match(r'Q(\d)', s); n = int(m.group(1)); s = s[2:]; name = None
        for _ in range(n):
            m = re.match(r'(\d+)', s)
            if not m: return name
            k = int(m.group(1)); name = s[len(m.group(1)):len(m.group(1)) + k]; s = s[len(m.group(1)) + k:]
        return name
    m = re.match(r'(\d+)', s)
    if m:
        k = int(m.group(1)); return s[len(m.group(1)):len(m.group(1)) + k]
    return None


def candidates(cls, off, size, line):
    words = set(re.findall(r'[A-Za-z_]\w*', line))
    def named(c):
        leaf = re.sub(r'\[\d+\]$', '', c[2]).split('.')[-1]
        return leaf in words
    if cls in CATALOGUE:  # `this` first
        mine = [(cls, own, mem, rel) for own, mem, rel, o, sz in CATALOGUE[cls] if o == off and sz == size]
        hit = [c for c in mine if named(c)]
        if hit: return [c + ('name',) for c in hit[:1]]
    hit = [c for c in BY_OFF.get((off, size), ()) if named(c)]
    owners = set((c[1], c[2], c[3]) for c in hit)
    if hit and len(owners) <= 3:
        return [next(c for c in hit if (c[1], c[2], c[3]) == o) + ('name',) for o in owners]
    # nothing named on the line (an inlined accessor, a temporary): `this`'s
    # one member at that offset and size, if it has exactly one
    if cls in CATALOGUE and len(mine) == 1:
        return [mine[0] + ('this',)]
    return None


NEW = (0x08026EDC, 0x08026EC0)  # OperatorNew, OperatorNewArray

def bl_target(b, o):
    h, h2 = hw(b, o), hw(b, o + 2)
    v = ((h & 0x7FF) << 12) | ((h2 & 0x7FF) << 1)
    if v & 0x400000: v -= 0x800000
    return 0x08000000 + o + 4 + v

def r0_const(b, o):
    """The constant last moved into r0 in the few instructions before o."""
    for k in range(2, 14, 2):
        h = hw(b, o - k)
        if (h & 0xFF00) == 0x2000: return h & 0xFF
        if (h & 0xFF00) == 0x4800:
            a = ((o - k + 4) & ~3) + (h & 0xFF) * 4
            return int.from_bytes(b[a:a + 4], 'little')
    return None

sizes = collections.defaultdict(collections.Counter)
votes = collections.defaultdict(collections.Counter)   # (struct, member) -> Counter((xs_off, nt_off))
unattributed = collections.Counter()
seen = 0
for r in R:
    if r['kind'] not in ('exact', 'near', 'similar') or r.get('arm') or r['nt'] <= 0: continue
    xo = r['addr'] - 0x08000000; no = r['nt'] - 0x08000000
    n = r['size'] // 2; m = (r.get('ntsize') or r['size']) // 2
    start, rel, info = objinfo(r['addr'])
    if not info: continue
    fns, lines = info
    a = [tok(hw(xs, xo + 2 * k)) for k in range(n)]
    b = [tok(hw(nt, no + 2 * k)) for k in range(m)]
    # align on shapes with the immediates masked out
    sa = [x if not isinstance(x, tuple) else ('ls', x[1]) for x in a]
    sb = [x if not isinstance(x, tuple) else ('ls', x[1]) for x in b]
    for blk in difflib.SequenceMatcher(None, sa, sb, autojunk=False).get_matching_blocks():
        for k in range(blk.size):
            i, j = blk.a + k, blk.b + k
            hx, hn = hw(xs, xo + 2 * i), hw(nt, no + 2 * j)
            if a[i] == 'BL' and bl_target(xs, xo + 2 * i) in NEW:
                o = xo + 2 * i - (start - 0x08000000)
                li = bisect.bisect_right(lines, (o, 1 << 30, '')) - 1
                text = src_line(lines[li][2], lines[li][1]) if li >= 0 else ''
                mm = re.search(r'new\s+(?:\w+::)*(\w+)', text)
                if mm:
                    sizes[mm.group(1)][(r0_const(xs, xo + 2 * i), r0_const(nt, no + 2 * j))] += 1
            lx, ln = ls(hx), ls(hn)
            if not lx or not ln or lx[0] != ln[0]: continue
            lx = (lx[0], lx[1], effective(xs, xo + 2 * i, lx[3][1], lx[2]), lx[3])
            ln = (ln[0], ln[1], effective(nt, no + 2 * j, ln[3][1], ln[2]), ln[3])
            seen += 1
            o = xo + 2 * i - (start - 0x08000000)
            li = bisect.bisect_right(lines, (o, 1 << 30, '')) - 1
            if li < 0: continue
            _, lineno, path = lines[li]
            text = src_line(path, lineno)
            fi = bisect.bisect_right([f[0] for f in fns], o) - 1
            cls = method_class(fns[fi][1]) if fi >= 0 else None
            c = candidates(cls, lx[2], lx[1], text)
            if not c:
                unattributed[lx[2] != ln[2]] += 1
                continue
            for outer, own, mem, relo, how in c:
                votes[(own, mem, relo)][(outer, lx[2], ln[2], how)] += 1

res = []
for (own, mem, relo), c in votes.items():
    obs = collections.Counter(); how = collections.Counter()
    for (outer, x, y, h), v in c.items():
        obs[y - x + relo] += v; how[h] += v
    res.append(dict(struct=own, member=mem, xs=relo, nt=[[y, v] for y, v in obs.most_common()],
                    by=dict(how), via=sorted(set(k[0] for k in c))))
res.sort(key=lambda e: (e['struct'], e['xs']))
json.dump(dict(members=res, sizes={k: [[x, y, v] for (x, y), v in c.most_common()] for k, c in sizes.items()}),
          open(OUT, 'w'), indent=0)
print('aligned load/stores', seen, 'attributed members', len(res), 'unattributed (same, moved)',
      unattributed[False], unattributed[True], file=sys.stderr)
