"""Apply one batch (#575) of tools/file_layout_plan.tsv to the repo.

usage: tools/apply_file_layout.py <repo> <dirs,comma,separated> <phase>
phases:
  rename  git mv every single-file row and every merge group's first member
          to its new path; rewrite ldscript/Makefile/report_units tokens.
  merge   append the remaining members of each merge group to the group's
          file (ROM order, .pool rule, leading #includes hoisted), git rm
          them, drop their ldscript lines and Makefile entries, point their
          report_units entries at the merged object.
  refs    rewrite path / file-name mentions in docs, comments and tools.
"""
import os, re, subprocess, sys
from collections import OrderedDict

repo, dirs, phase = sys.argv[1], sys.argv[2].split(','), sys.argv[3]
os.chdir(repo)
rows = [l.rstrip('\n').split('\t') for l in open('tools/file_layout_plan.tsv')
        if not l.startswith(('#', 'old_path'))]
batch = [(o, n) for o, n, g, w in rows if n.startswith('src/') and n.split('/')[1] in dirs and o != n]
groups = OrderedDict()
for o, n in batch:
    groups.setdefault(n, []).append(o)

def git(*a):
    subprocess.run(['git'] + list(a), check=True)

def stem(p):
    return p[:-2]  # drop .c

def rewrite(path, fn):
    s = open(path).read()
    t = fn(s)
    if t != s:
        open(path, 'w').write(t)

def obj_rename(mapping):
    """mapping: old 'src/x/y' -> new 'src/a/b' (no extension)."""
    def ld(s):
        for o, n in mapping.items():
            s = s.replace('build/crashbandicootxs/%s.o(' % o, 'build/crashbandicootxs/%s.o(' % n)
        return s
    rewrite('ldscript.txt', ld)
    def mk(s):
        for o, n in mapping.items():
            s = re.sub(r'\$\(C_BUILDDIR\)/%s\.o\b' % re.escape(o[4:]), '$(C_BUILDDIR)/%s.o' % n[4:], s)
        return s
    rewrite('Makefile', mk)
    def ru(s):
        for o, n in mapping.items():
            s = s.replace('"%s.o"' % o, '"%s.o"' % n)
        return s
    rewrite('tools/report_units.py', ru)

def needs_pool(src):
    code = re.sub(r'/\*.*?\*/', '', src, flags=re.S)
    lds = [m.end() for m in re.finditer(r'"[^"\n]*\bldr\b[^"\n]*=', code)]
    return bool(lds) and not re.search(r'\.(pool|ltorg)\b', code[lds[-1]:])

def split_includes(src):
    """Leading run of #include lines (and blank lines) -> (includes, rest)."""
    lines = src.split('\n')
    inc = []; i = 0
    while i < len(lines) and (lines[i].startswith('#include') or not lines[i].strip()):
        if lines[i].startswith('#include'):
            inc.append(lines[i])
        i += 1
    return inc, '\n'.join(lines[i:])

if phase == 'rename':
    mapping = {}
    for n, olds in groups.items():
        os.makedirs(os.path.dirname(n), exist_ok=True)
        git('mv', olds[0], n)
        mapping[stem(olds[0])] = stem(n)
    obj_rename(mapping)
    print('renamed', len(mapping))

elif phase == 'merge':
    dropped = {}
    for n, olds in groups.items():
        if len(olds) < 2:
            continue
        head = open(n).read()
        inc, body = split_includes(head)
        parts = [body.rstrip('\n')]
        prev_src = head
        for o in olds[1:]:
            src = open(o).read()
            if needs_pool(prev_src):
                parts.append('asm(".pool");')
            minc, mbody = split_includes(src)
            for l in minc:
                if l not in inc:
                    inc.append(l)
            # drop file-scope single-line externs/#defines already present verbatim
            seen = set(x.strip() for x in '\n'.join(parts).split('\n')
                       if re.match(r'(extern\s.*;|#define\s)', x))
            kept = []
            for x in mbody.split('\n'):
                if re.match(r'(extern\s.*;|#define\s)', x) and not x.rstrip().endswith('\\') and x.strip() in seen:
                    continue
                kept.append(x)
            mbody = re.sub(r'\n{3,}', '\n\n', '\n'.join(kept))
            parts.append(mbody.strip('\n'))
            prev_src = src
            git('rm', '-q', o)
            dropped[stem(o)] = stem(n)
        open(n, 'w').write('\n'.join(inc) + '\n\n' + '\n\n'.join(parts) + '\n')
    # ldscript: drop members' lines
    def ld(s):
        out = []
        for l in s.split('\n'):
            m = re.search(r'build/crashbandicootxs/(src/\S+)\.o\(', l)
            if m and m.group(1) in dropped:
                continue
            out.append(l)
        return '\n'.join(out)
    rewrite('ldscript.txt', ld)
    def mk(s):
        for o in dropped:
            tok = r'\$\(C_BUILDDIR\)/%s\.o' % re.escape(o[4:])
            s2 = re.sub(r'[ \t]*' + tok + r'[ \t]*\\\n', '', s)
            if s2 == s:
                s2 = re.sub(r'[ \t]*\\\n[ \t]*' + tok + r'[ \t]*\n', '\n', s)
            if s2 == s:
                s2 = re.sub(r'(:=\s*)' + tok + r'[ \t]*\n', r'\1\n', s)
            s = s2
        return s
    rewrite('Makefile', mk)
    def ru(s):
        for o, n in dropped.items():
            s = s.replace('"%s.o"' % o, '"%s.o"' % n)
        return s
    rewrite('tools/report_units.py', ru)
    print('merged away', len(dropped))

elif phase == 'refs':
    import fnmatch
    full = {}   # 'graphics/actor_part19' -> 'player/foo'  (no src/, no ext)
    for o, n in batch:
        full[stem(o)[4:]] = stem(n)[4:]
    base = {os.path.basename(o): os.path.basename(n) for o, n in full.items()}
    all_old = [stem(o)[4:] for o, n, g, w in rows]
    alt = lambda keys: '|'.join(sorted(map(re.escape, keys), key=len, reverse=True))
    # a C/shell/markdown line continuation: newline, indent, optional comment leader
    CONT = r'(?:[ \t]*\n[ \t]*(?:\*(?!/)|//|#)?[ \t]*)'
    # 0) shorthand lists: game_loop37.c/38.c, hud_icon_widget2/3/4.c -> full names
    pat_short = re.compile(r'(?<![\w-])(' + alt(base) + r')(\.[cos])?/(' + CONT + r'?)([0-9a-z]{1,4})(?=[./])')
    def expand(m):
        prev, ext, gap, suf = m.group(1), m.group(2) or '', m.group(3), m.group(4)
        for cand in (prev[:len(prev) - len(suf)] + suf, prev.rstrip('0123456789') + suf):
            if cand in base and cand != prev:
                return '%s%s/%s%s' % (prev, ext, gap, cand)
        return m.group(0)
    # 1) dir-qualified (optionally src/ or build prefixes), 2) bare basename,
    #    also right after another file name and a slash (a.c/b.c)
    pat_full = re.compile(r'(?<![\w.-])((?:[\w./]*/)?)(' + alt(full) + r')\.(c|o|s)\b')
    pat_base = re.compile(r'(?:(?<![\w./-])|(?<=\.[cos]/))(' + alt(base) + r')\.(c|o|s)\b')
    pat_new = re.compile(r'`?(?<![\w-])(?:[\w./]*/)?(?:%s)\.[cos]\b`?'
                         % alt({os.path.basename(v) for v in full.values()}))
    # what may sit between two mentions of one file for the second to be a duplicate
    SEP = re.compile(r'[ \t]*(?:,|/|\band\b|\bor\b)' + CONT + r'?(?:[ \t]*(?:and|or)[ \t]+' + CONT + r'?)?[ \t]*'
                     r'|' + CONT + r'(?:and|or)[ \t]+')
    pat_glob = re.compile(r'(?<![\w-])((?:[\w.]+/)*)(\w*\*[\w*]*)\.(c|o)\b')
    warnings = []
    def dedupe(s):
        out = []; last = 0; prev = None
        for m in pat_new.finditer(s):
            tok = m.group(0)
            key = (os.path.basename(tok.strip('`')), tok.startswith('`'))
            if prev and prev[1] == key and SEP.fullmatch(s[prev[0]:m.start()]):
                out.append(s[last:prev[0]]); last = m.end()
                continue
            prev = (m.end(), key)
        out.append(s[last:])
        return ''.join(out)
    def globs(s, f):
        def g(m):
            d, pat, ext = m.groups()
            sub = d[4:] if d.startswith('src/') else d
            hits = [o for o in all_old if fnmatch.fnmatch(os.path.basename(o), pat)
                    and (not sub or o.startswith(sub))]
            if not hits or not all(h in full for h in hits):
                return m.group(0)
            news = sorted({full[h] for h in hits})
            if len(news) == 1:
                nd = news[0]
                pre = d[:len(d) - len(sub)] + os.path.dirname(nd) + '/' if sub else ''
                return pre + os.path.basename(nd) + '.' + ext
            warnings.append('%s: wildcard %r -> %s' % (f, m.group(0), ', '.join(x + '.c' for x in news)))
            return m.group(0)
        return pat_glob.sub(g, s)
    def fix(s, f):
        prev = None
        while prev != s:
            prev, s = s, pat_short.sub(expand, s)
        s = globs(s, f)
        def f1(m):
            pre = m.group(1)
            if pre and not (pre.endswith('src/') or pre == '' or pre.endswith('/')):
                return m.group(0)
            return pre + full[m.group(2)] + '.' + m.group(3)
        s = pat_full.sub(f1, s)
        s = pat_base.sub(lambda m: base[m.group(1)] + '.' + m.group(2), s)
        # merged files can now be listed several times in a row: keep one
        return dedupe(s)
    files = subprocess.run(['git', 'ls-files'], capture_output=True, text=True, check=True).stdout.split()
    skip = {'tools/file_layout_plan.tsv', 'docs/file_layout_plan.md', 'tools/apply_file_layout.py'}
    exts = ('.c', '.h', '.s', '.md', '.txt', '.py', '.mk', '.inc', '.yml', '.json')
    todo = [f for f in files if f not in skip and not (f.startswith('expected/') and f.endswith('.s'))
            and (f.endswith(exts) or os.path.basename(f) == 'Makefile') and os.path.isfile(f)]
    n = 0
    for f in todo:
        try:
            s = open(f).read()
        except UnicodeDecodeError:
            continue
        t = fix(s, f)
        if t != s:
            open(f, 'w').write(t); n += 1
    print('rewrote', n, 'files')
    for w in warnings:
        print('fix by hand:', w)
    # old names still followed by an extension, slash, star or backtick
    left = re.compile(r'(?<![\w-])(' + alt(base) + r')(?=\.[cos]\b|[/*`])')
    for f in todo:
        try:
            for i, l in enumerate(open(f), 1):
                if left.search(l):
                    print('leftover: %s:%d: %s' % (f, i, l.strip()[:140]))
        except UnicodeDecodeError:
            pass
