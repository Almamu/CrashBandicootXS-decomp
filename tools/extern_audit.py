#!/usr/bin/env python3
"""Audit the extern declarations, prototypes and local structs in .c files (#574).

Parses every .c file under src/ and lib/ (and, for reference, every header
under include/ and lib/*/include/) with a light, regex-based parser that
only looks at file scope. It does not run the preprocessor: both arms of an
`#if NON_MATCHING` are read, and a symbol declared twice in one file counts
once for that file.

For every symbol that a .c file declares (an `extern` variable or function,
or a plain function prototype) it reports:

  * where the symbol is defined: a C file (function or global definition,
    or a label/.set inside a top-level asm() block), an assembly file
    (`asm/`, `data/`, `lib/**/*.s`), the linker script (`ldscript.txt`,
    `sym_*.txt`), or `?` when nothing defines it;
  * how many .c files declare it, and which headers already do;
  * whether the declarations agree:
      identical    same text after whitespace/comment normalisation
      equivalent   same types once parameter names are dropped
      conflicting  different types; the variants are listed, and the
                   conflict is tagged with what differs:
                     pointer   only pointer target types differ
                               (void * vs struct foo * vs u8 *)
                     array     array vs pointer extern (`x[]` vs `*x`)
                     proto     prototyped vs unprototyped `f()`
                     scalar    anything else (u8 vs s32, void vs value
                               return, extra parameters, struct by value)

It also collects every struct/union definition (tagged or typedef'd) in a
.c file and computes a best-effort layout signature (size, and the offset
and size of each field) so duplicate copies of one struct can be spotted,
by name and by layout.

Output: a summary on stdout and machine-readable files in the output
directory (default build/extern_audit/):

  symbols.tsv   one row per symbol
  symbols.json  the same, with every declaration variant and file
  structs.tsv   one row per local struct definition
  structs.json  struct groups by name and by layout signature

The parser is deliberately simple. It is good enough to plan the header
work and to find conflicts; it is not a C compiler.
"""

import argparse
import collections
import glob
import json
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

KEYWORDS = {
    'extern', 'static', 'const', 'volatile', 'struct', 'union', 'enum',
    'unsigned', 'signed', 'int', 'char', 'short', 'long', 'void', 'float',
    'double', 'inline', '__inline__', 'register', 'typedef', 'asm',
    '__asm__', '__attribute__', 'NAKED', 'sizeof', 'return', 'if',
}

# ---------------------------------------------------------------------------
# Lexing helpers


def strip_comments(text):
    """Remove /* */ and // comments, keeping string literals and newlines."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '"' or c == "'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == '\\' else 1
            out.append(text[i:j + 1])
            i = j + 1
        elif text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append('\n' * text.count('\n', i, j) or ' ')
            i = j
        elif text.startswith('//', i):
            j = text.find('\n', i)
            i = n if j < 0 else j
        else:
            out.append(c)
            i += 1
    return ''.join(out)


def strip_preprocessor(text):
    """Blank out preprocessor lines (with continuations). Both #if arms stay."""
    lines = text.split('\n')
    out = []
    cont = False
    for line in lines:
        if cont or line.lstrip().startswith('#'):
            cont = line.rstrip().endswith('\\')
            out.append('')
        else:
            out.append(line)
    return '\n'.join(out)


def norm_ws(s):
    s = re.sub(r'\s+', ' ', s).strip()
    s = re.sub(r'\s*([*(),\[\];])\s*', r'\1', s)
    s = re.sub(r'(\w)\*', r'\1 *', s)
    s = s.replace(',', ', ')
    return s


def split_toplevel(text):
    """Yield (kind, stmt_text, line) for each file-scope statement.

    kind is 'stmt' (ends in ';') or 'func' (function definition: the text
    up to the body's opening brace).
    """
    i, n = 0, len(text)
    start = 0
    paren = 0
    line = 1
    start_line = 1
    while i < n:
        c = text[i]
        if c == '\n':
            line += 1
        if c == '"' or c == "'":
            j = i + 1
            while j < n and text[j] != c:
                if text[j] == '\n':
                    line += 1
                j += 2 if text[j] == '\\' else 1
            i = j + 1
            continue
        if c == '(':
            paren += 1
        elif c == ')':
            paren -= 1
        elif c == ';' and paren == 0:
            stmt = text[start:i]
            if stmt.strip():
                yield 'stmt', stmt, start_line
            start = i + 1
            start_line = line
        elif c == '{' and paren == 0:
            head = text[start:i]
            h = re.sub(r'__attribute__\s*\(\(.*?\)\)', '', head, flags=re.S).strip()
            is_func = h.endswith(')') and '=' not in h and not re.match(
                r'^(typedef\s+)?(extern\s+)?(static\s+)?(const\s+)?(struct|union|enum)\b[^()]*$', h)
            # find the matching close brace
            depth = 0
            j = i
            while j < n:
                d = text[j]
                if d == '"' or d == "'":
                    k = j + 1
                    while k < n and text[k] != d:
                        k += 2 if text[k] == '\\' else 1
                    line += text.count('\n', j, k)
                    j = k + 1
                    continue
                if d == '\n':
                    line += 1
                if d == '{':
                    depth += 1
                elif d == '}':
                    depth -= 1
                    if depth == 0:
                        break
                j += 1
            if is_func:
                yield 'func', head, start_line
                start = j + 1
                start_line = line
                i = j + 1
                continue
            # struct body / initializer: keep accumulating until ';'
            i = j + 1
            continue
        i += 1
    rest = text[start:]
    if rest.strip():
        yield 'stmt', rest, start_line


def split_commas(s):
    out, depth, cur = [], 0, []
    for c in s:
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        if c == ',' and depth == 0:
            out.append(''.join(cur))
            cur = []
        else:
            cur.append(c)
    out.append(''.join(cur))
    return out


def matching_paren(s, i):
    depth = 0
    for j in range(i, len(s)):
        if s[j] == '(':
            depth += 1
        elif s[j] == ')':
            depth -= 1
            if depth == 0:
                return j
    return -1


ATTR_RE = re.compile(r'__attribute__\s*\(\(')


def remove_attributes(s):
    while True:
        m = ATTR_RE.search(s)
        if not m:
            return s
        j = matching_paren(s, m.end() - 2)
        s = s[:m.start()] + ' ' + s[j + 1:]


def take_asm_label(s):
    """Strip a trailing asm("sym") label; return (text, label or None)."""
    m = re.search(r'\b(?:asm|__asm__)\s*\(\s*"([^"]*)"\s*\)\s*$', s)
    if m:
        return s[:m.start()], m.group(1)
    return s, None


# ---------------------------------------------------------------------------
# Declarations


def strip_param_name(p):
    p = norm_ws(p)
    if p in ('void', '...', ''):
        return p
    # function pointer parameter: void (*cb)(void)
    p = re.sub(r'\(\s*\*\s*\w+\s*\)', '(*)', p)
    m = re.match(r'^(.*?)\s*\b([A-Za-z_]\w*)\s*((?:\[[^\]]*\])*)$', p)
    if m:
        before, ident, arr = m.groups()
        toks = re.findall(r'[A-Za-z_]\w*', before)
        if before.strip() and ident not in KEYWORDS and not (
                toks and toks[-1] in ('struct', 'union', 'enum') and not before.rstrip().endswith('*')):
            # `u8 *x`, `s32 x`, `u8 x[4]` -> drop the name
            if before.strip() not in ('unsigned', 'signed', 'const', 'volatile'):
                p = before.strip() + ' *' * len(re.findall(r'\[', arr))
    return norm_ws(p)


def parse_declaration(text):
    """Parse one declarator statement. Returns dict or None.

    dict: name, asm, is_func, ret/type, params (list or None), storage.
    """
    s = remove_attributes(text)
    s = re.sub(r'\s+', ' ', s).strip()
    storage = set()
    while True:
        m = re.match(r'^(extern|static|inline|__inline__|NAKED)\s+', s)
        if not m:
            break
        storage.add(m.group(1))
        s = s[m.end():]
    init = None
    # split off an initializer (top-level '=')
    depth = 0
    for i, c in enumerate(s):
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif c == '=' and depth == 0:
            init = s[i + 1:].strip()
            s = s[:i].strip()
            break
    s, label = take_asm_label(s)
    s = s.strip()
    # struct body inside the declaration: replace by its tag
    s = re.sub(r'\b(struct|union|enum)\s*(\w*)\s*\{.*\}', lambda m: '%s %s' % (
        m.group(1), m.group(2) or '<anon>'), s, flags=re.S)
    # function pointer variable: T (*name[..])(params)
    m = re.match(r'^(.*?)\(\s*(\*+)\s*(?:const\s+)?([A-Za-z_]\w*)\s*((?:\[[^\]]*\])*)\s*\)\s*\((.*)\)\s*$', s)
    if m:
        ret, stars, name, arr, params = m.groups()
        typ = norm_ws('%s (%s%s)(%s)' % (ret, stars, arr, ','.join(
            strip_param_name(p) for p in split_commas(params))))
        return dict(name=name, asm=label, is_func=False, type=typ, storage=storage,
                    init=init, text=norm_ws(text))
    # function: T name(params)
    m = re.match(r'^(.*?)\b([A-Za-z_]\w*)\s*\((.*)\)\s*$', s, flags=re.S)
    if m and m.group(2) not in KEYWORDS:
        ret, name, params = m.groups()
        if '(' in ret or not ret.strip():
            return None  # function returning a function pointer, or a macro call
        plist = [p for p in split_commas(params)]
        types = [strip_param_name(p) for p in plist]
        if types == ['']:
            types = []
        return dict(name=name, asm=label, is_func=True, ret=norm_ws(ret),
                    params=types, unprototyped=(params.strip() == ''),
                    storage=storage, init=None, text=norm_ws(text))
    # variable: T name[..]
    m = re.match(r'^(.*?)\b([A-Za-z_]\w*)\s*((?:\[[^\]]*\])*)$', s)
    if m and m.group(2) not in KEYWORDS and m.group(1).strip():
        base, name, arr = m.groups()
        typ = norm_ws(base + ' ' + arr) if arr else norm_ws(base)
        return dict(name=name, asm=label, is_func=False, type=typ, storage=storage,
                    init=init, text=norm_ws(text))
    return None


def split_declarators(stmt):
    """`extern u8 a, *b;` -> ['extern u8 a', 'extern u8 *b']"""
    s = stmt.strip()
    if '{' in s:
        return [s]
    parts = split_commas(s)
    if len(parts) == 1:
        return parts
    first = parts[0]
    # base type = everything before the first declarator of the first part
    d = parse_declaration(first)
    if not d or d.get('is_func'):
        return [s]
    idx = first.rfind(d['name'])
    base = first[:idx].rstrip('* ')
    out = [first]
    for p in parts[1:]:
        out.append(base + ' ' + p.strip())
    return out


def type_key(d):
    if d['is_func']:
        params = d['params']
        if d['unprototyped']:
            ps = '<unprototyped>'
        else:
            ps = ', '.join(params) if params else 'void'
        return '%s (%s)' % (d['ret'], ps)
    return d['type']


def canon_scalar(t):
    t = re.sub(r'\bsigned int\b|\bint\b', 's32', t)
    t = re.sub(r'\bunsigned int\b|\bunsigned\b', 'u32', t)
    t = re.sub(r'\bunsigned char\b', 'u8', t)
    t = re.sub(r'\bsigned char\b', 's8', t)
    t = re.sub(r'\bunsigned short\b', 'u16', t)
    t = re.sub(r'\b(signed )?short\b', 's16', t)
    return t


PTR_RE = re.compile(r'(?:(?:const|volatile)\s+)*(?:(?:struct|union|enum)\s+)?[A-Za-z_]\w*(?:\s+(?:const|volatile))*\s*\*')


def conflict_kind(keys):
    keys = [canon_scalar(k) for k in keys]
    if len(set(keys)) == 1:
        return 'equivalent'
    if any('<unprototyped>' in k for k in keys):
        return 'proto'
    ptr = {PTR_RE.sub('PTR', k).replace('PTR*', 'PTR').replace('PTR *', 'PTR') for k in keys}
    while any('PTR*' in k or 'PTR *' in k for k in ptr):
        ptr = {k.replace('PTR *', 'PTR').replace('PTR*', 'PTR') for k in ptr}
    if len(ptr) == 1:
        return 'pointer'
    arr = {re.sub(r'\[[^\]]*\]', '*', k) for k in keys}
    if any('[' in k for k in keys) and any('[' not in k for k in keys):
        arrp = {PTR_RE.sub('PTR', re.sub(r'\s*\[[^\]]*\]', ' *', k)) for k in keys}
        if len(arrp) == 1 or len(arr) == 1:
            return 'array'
    return 'scalar'


# ---------------------------------------------------------------------------
# Structs

BASE_SIZES = {
    'u8': 1, 's8': 1, 'char': 1, 'bool8': 1, 'vu8': 1, 'vs8': 1,
    'u16': 2, 's16': 2, 'short': 2, 'bool16': 2, 'vu16': 2, 'vs16': 2,
    'u32': 4, 's32': 4, 'int': 4, 'long': 4, 'float': 4, 'bool32': 4,
    'vu32': 4, 'vs32': 4, 'unsigned': 4, 'signed': 4, 'size_t': 4,
    'u64': 8, 's64': 8, 'double': 8,
}

STRUCT_DEF_RE = re.compile(r'\b(struct|union)\s*([A-Za-z_]\w*)?\s*\{')


def find_struct_defs(stmt):
    """Yield (kind, tag, body, rest) for each struct/union body in stmt (outermost)."""
    for m in STRUCT_DEF_RE.finditer(stmt):
        # outermost only: skip if inside braces
        if stmt[:m.start()].count('{') != stmt[:m.start()].count('}'):
            continue
        i = m.end() - 1
        depth = 0
        for j in range(i, len(stmt)):
            if stmt[j] == '{':
                depth += 1
            elif stmt[j] == '}':
                depth -= 1
                if depth == 0:
                    break
        yield m.group(1), m.group(2), stmt[i + 1:j], stmt[j + 1:]


def eval_int(expr):
    expr = expr.strip()
    if not expr:
        return None
    if not re.fullmatch(r'[0-9a-fA-FxX+\-*/() ]+', expr):
        return None
    try:
        return int(eval(re.sub(r'\b0([0-7]+)\b', r'\1', expr), {}, {}))
    except Exception:
        return None


class Layout:
    def __init__(self, known):
        self.known = known  # name -> (size, align) for structs/typedefs

    def type_size(self, base, ptr):
        if ptr:
            return 4, 4
        base = re.sub(r'\b(const|volatile|unsigned|signed)\b', lambda m: '' if m.group(1) in (
            'const', 'volatile') else m.group(1), base).strip()
        base = re.sub(r'\s+', ' ', base)
        if base in ('unsigned', 'signed'):
            return 4, 4
        base = re.sub(r'^(unsigned|signed) ', '', base)
        if base in BASE_SIZES:
            s = BASE_SIZES[base]
            return s, min(s, 4)
        if base in self.known:
            return self.known[base]
        if base.startswith('enum '):
            return 4, 4
        return None

    def layout(self, kind, body, packed):
        """Return (size, align, fields) or (None, None, fields)."""
        fields = []
        off = 0
        maxsize = 0
        align = 1
        bit_unit = None  # (start_off, unit_size, bits_used)
        ok = True
        body = re.sub(r'\s+', ' ', body)
        # flatten nested struct bodies into their own layouts
        for raw in self.member_stmts(body):
            raw = raw.strip()
            if not raw:
                continue
            nested = list(find_struct_defs(raw))
            if nested:
                nk, ntag, nbody, nrest = nested[0]
                npacked = 'packed' in nrest or 'packed' in raw[:raw.find('{')]
                sz, al, _ = self.layout(nk, nbody, npacked)
                if ntag and sz is not None:
                    self.known['%s %s' % (nk, ntag)] = (sz, al)
                names = [x.strip() for x in split_commas(remove_attributes(nrest)) if x.strip()]
                if not names:
                    names = ['<anon>']
                decls = [(sz, al, n) for n in names]
            else:
                raw = remove_attributes(raw)
                m = re.match(r'^(.*?)\s*:\s*(\S+)$', raw)
                bits = None
                if m:
                    raw, bits = m.group(1), eval_int(m.group(2))
                parts = split_commas(raw)
                d0 = re.match(r'^((?:(?:const|volatile|unsigned|signed|struct|union|enum)\s+)*[A-Za-z_]\w*)(.*)$', parts[0].strip())
                if not d0:
                    ok = False
                    continue
                base = d0.group(1)
                decls = []
                for k, p in enumerate([d0.group(2)] + parts[1:]):
                    p = p.strip()
                    ptr = p.count('*') > 0 or '(' in p
                    nm = re.search(r'([A-Za-z_]\w*)', p)
                    name = nm.group(1) if nm else '<anon>'
                    arr = 1
                    for a in re.findall(r'\[([^\]]*)\]', p):
                        v = eval_int(a)
                        if v is None:
                            arr = None
                            break
                        arr *= v
                    ts = self.type_size(base, ptr)
                    if ts is None or arr is None:
                        decls.append((None, None, name))
                    else:
                        decls.append((ts[0] * arr, ts[1], name, ts[0], bits))
            for dd in decls:
                if dd[0] is None:
                    ok = False
                    fields.append((None, None, dd[2]))
                    continue
                sz, al = dd[0], (1 if packed else dd[1])
                bits = dd[4] if len(dd) > 4 else None
                if kind == 'union':
                    fields.append((0, sz, dd[2]))
                    maxsize = max(maxsize, sz)
                    align = max(align, al)
                    continue
                if bits is not None:
                    unit = dd[3]
                    if bit_unit and bit_unit[1] == unit and bit_unit[2] + bits <= unit * 8:
                        fields.append((bit_unit[0], unit, dd[2]))
                        bit_unit = (bit_unit[0], unit, bit_unit[2] + bits)
                        continue
                    if bit_unit and packed:
                        off = bit_unit[0] + (bit_unit[2] + 7) // 8
                    elif bit_unit:
                        off = bit_unit[0] + bit_unit[1]
                    if not packed:
                        off = (off + al - 1) // al * al
                    bit_unit = (off, unit, bits)
                    fields.append((off, unit, dd[2]))
                    align = max(align, al)
                    continue
                if bit_unit:
                    off = bit_unit[0] + ((bit_unit[2] + 7) // 8 if packed else bit_unit[1])
                    bit_unit = None
                off = (off + al - 1) // al * al
                fields.append((off, sz, dd[2]))
                off += sz
                align = max(align, al)
        if bit_unit:
            off = bit_unit[0] + ((bit_unit[2] + 7) // 8 if packed else bit_unit[1])
        size = maxsize if kind == 'union' else off
        size = (size + align - 1) // align * align
        if not ok:
            return None, None, fields
        return size, align, fields

    @staticmethod
    def member_stmts(body):
        out, depth, cur = [], 0, []
        for c in body:
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
            if c == ';' and depth == 0:
                out.append(''.join(cur))
                cur = []
            else:
                cur.append(c)
        if ''.join(cur).strip():
            out.append(''.join(cur))
        return out


# ---------------------------------------------------------------------------
# Scanning


def subsystem_of(path):
    parts = path.split('/')
    if parts[0] == 'src' and len(parts) > 2:
        return parts[1]
    if parts[0] == 'lib' and len(parts) > 2:
        return 'lib/' + parts[1]
    if parts[0] == 'include':
        return 'include'
    return parts[0]


def read(path):
    with open(os.path.join(ROOT, path), encoding='utf-8', errors='replace') as f:
        return f.read()


class FileScan:
    def __init__(self, path):
        self.path = path
        self.decls = []      # extern/prototype declarations
        self.defs = []       # (name, kind) non-static definitions
        self.static_defs = set()
        self.structs = []    # (kind, tag, typedef_name, body, packed, line, text)
        self.typedefs = {}   # alias -> 'struct tag' | base type
        self.asm_defs = set()


def scan_c(path, is_header=False):
    fs = FileScan(path)
    text = strip_preprocessor(strip_comments(read(path)))
    for kind, stmt, line in split_toplevel(text):
        st = stmt.strip()
        if kind == 'func':
            d = parse_declaration(st)
            if d and d['is_func']:
                if 'static' in d['storage']:
                    fs.static_defs.add(d['name'])
                else:
                    fs.defs.append((d['asm'] or d['name'], 'function'))
            continue
        # top-level asm block
        m = re.match(r'^(?:asm|__asm__)\s*(?:volatile\s*)?\(', st)
        if m:
            strs = ''.join(re.findall(r'"((?:[^"\\]|\\.)*)"', st))
            strs = strs.replace('\\n', '\n').replace('\\t', '\t')
            for g in re.findall(r'(?:^|\n|;)\s*([A-Za-z_]\w*)\s*:', strs):
                fs.asm_defs.add(g)
            for g in re.findall(r'\.set\s+([A-Za-z_]\w*)\s*,', strs):
                fs.asm_defs.add(g)
            for g in re.findall(r'\.(?:global|globl)\s+([A-Za-z_]\w*)', strs):
                fs.asm_defs.add(g)
            continue
        # struct/union definitions
        for skind, tag, body, rest in find_struct_defs(st):
            head = st[:st.find('{')]
            packed = 'packed' in rest.split(';')[0] or 'packed' in head
            tdname = None
            if st.startswith('typedef'):
                names = [x.strip(' *') for x in split_commas(remove_attributes(rest)) if x.strip()]
                tdname = names[0] if names else None
            fs.structs.append((skind, tag, tdname, body, packed, line, st))
        if st.startswith('typedef'):
            d = parse_declaration(re.sub(r'^typedef\s+', '', st))
            if d and not d['is_func']:
                fs.typedefs[d['name']] = d.get('type')
            continue
        if re.match(r'^(struct|union|enum)\s*\w*\s*\{', st):
            if st.startswith('enum'):
                rest = st[st.rfind('}') + 1:]
            else:
                rest = next(find_struct_defs(st))[3]
            if not remove_attributes(rest).strip():
                continue  # pure struct/enum definition
        if re.match(r'^(struct|union|enum)\s+\w+$', st):
            continue  # forward declaration
        if st.startswith('extern') and '{' in st and 'struct' not in st.split('{')[0] and 'union' not in st.split('{')[0]:
            continue
        first = line + stmt[:len(stmt) - len(stmt.lstrip())].count('\n')
        parts = split_declarators(st)
        for one in parts:
            d = parse_declaration(one)
            if not d:
                continue
            d['line'] = first
            d['end_line'] = first + st.count('\n')
            d['ndecl'] = len(parts)
            sym = d['asm'] or d['name']
            if 'extern' in d['storage'] and d['init'] is None:
                fs.decls.append(d)
            elif d['is_func']:
                if 'static' in d['storage']:
                    fs.static_defs.add(d['name'])  # static forward declaration
                else:
                    fs.decls.append(d)
            elif 'static' in d['storage']:
                fs.static_defs.add(d['name'])
            elif not is_header:
                fs.defs.append((sym, 'data'))
    return fs


def scan_asm_defs():
    """Symbols defined outside C: .s files, the linker script and sym_*.txt."""
    out = {}
    s_files = sorted(set(glob.glob('asm/**/*.s', root_dir=ROOT, recursive=True)
                         + glob.glob('data/**/*.s', root_dir=ROOT, recursive=True)
                         + glob.glob('lib/**/*.s', root_dir=ROOT, recursive=True)))
    for p in s_files:
        t = read(p)
        for g in re.findall(r'^\s*([A-Za-z_]\w*)\s*:', t, flags=re.M):
            out.setdefault(g, p)
        for g in re.findall(r'^\s*\.(?:set|equ)\s+([A-Za-z_]\w*)\s*,', t, flags=re.M):
            out.setdefault(g, p)
        # lib1funcs-style `FUNC_START name` / `LSYM(name)` macros
        for g in re.findall(r'^\s*(?:FUNC_START|THUMB_FUNC_START|ARM_FUNC_START)\s+([A-Za-z_]\w*)', t, flags=re.M):
            out.setdefault(g, p)
    for p in ('ldscript.txt', 'sym_ewram.txt', 'sym_iwram.txt'):
        if os.path.exists(os.path.join(ROOT, p)):
            for g in re.findall(r'\b([A-Za-z_]\w*)\s*=\s*[^=;]+;', read(p)):
                out.setdefault(g, p)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('-o', '--out-dir', default=os.path.join(ROOT, 'build', 'extern_audit'))
    ap.add_argument('--subsystem', help='only list conflicts for symbols owned by this subsystem')
    ap.add_argument('--conflicts', action='store_true', help='print every conflicting symbol and its variants')
    ap.add_argument('--symbol', action='append', default=[], help='print the full record for SYMBOL')
    args = ap.parse_args()

    os.chdir(ROOT)
    c_files = sorted(glob.glob('src/**/*.c', recursive=True) + glob.glob('lib/**/*.c', recursive=True))
    h_files = sorted(glob.glob('include/**/*.h', recursive=True) + glob.glob('lib/**/*.h', recursive=True))

    scans = {p: scan_c(p) for p in c_files}
    hscans = {p: scan_c(p, is_header=True) for p in h_files}
    asm_defs = scan_asm_defs()

    # --- symbol table -----------------------------------------------------
    defined = collections.defaultdict(list)
    for p, fs in scans.items():
        for name, kind in fs.defs:
            defined[name].append(p)
        for name in fs.asm_defs:
            defined[name].append(p + ' (asm)')

    # generated C fragments (levels, graphics, sound) included by src/data/*.c
    gen_root = os.path.join('build', 'crashbandicootxs')
    for sub in ('data', 'graphics', 'sound'):
        for p in sorted(glob.glob(os.path.join(gen_root, sub, '**', '*'), recursive=True)):
            if p.endswith(('.inc', '.h', '.c')) and os.path.isfile(p):
                for name, kind in scan_c(p).defs:
                    defined[name].append(p)

    decls = collections.defaultdict(list)    # sym -> [(file, decl)]
    total_decls = 0
    self_forward = 0
    for p, fs in scans.items():
        seen = set()
        own = {n for n, _ in fs.defs} | fs.asm_defs
        for d in fs.decls:
            sym = d['asm'] or d['name']
            # a declaration of something the same file defines is a forward
            # declaration, not an extern: count it separately
            if sym in own:
                self_forward += 1
                continue
            total_decls += 1
            if (sym, type_key(d)) in seen:
                continue
            seen.add((sym, type_key(d)))
            decls[sym].append((p, d))
    hdecls = collections.defaultdict(list)
    for p, fs in hscans.items():
        for d in fs.decls:
            hdecls[d['asm'] or d['name']].append((p, d))

    symbols = []
    for sym in sorted(decls):
        entries = decls[sym]
        files = sorted({p for p, _ in entries})
        texts = sorted({re.sub(r'^extern ', '', d['text']) for _, d in entries})
        keys = sorted({type_key(d) for _, d in entries})
        if len(texts) == 1:
            status = 'identical'
            ckind = ''
        elif len(keys) == 1:
            status = 'equivalent'
            ckind = ''
        else:
            ck = conflict_kind(keys)
            if ck == 'equivalent':
                status, ckind = 'equivalent', ''
            else:
                status, ckind = 'conflicting', ck
        if sym in defined:
            dfiles = sorted(set(defined[sym]))
            dsrc = dfiles[0]
            owner = 'data' if dsrc.startswith('build/') else subsystem_of(dsrc.split(' ')[0])
        elif sym in asm_defs:
            dfiles = [asm_defs[sym]]
            dsrc = asm_defs[sym]
            owner = 'ldscript' if dsrc.endswith('.txt') else ('asm' if dsrc.startswith(('asm/', 'data/')) else subsystem_of(dsrc))
        else:
            dfiles, dsrc, owner = [], '?', '?'
        users = collections.Counter(subsystem_of(p) for p in files)
        variants = collections.defaultdict(list)
        for p, d in entries:
            variants[type_key(d)].append(p)
        symbols.append(dict(
            symbol=sym,
            kind='function' if any(d['is_func'] for _, d in entries) else 'data',
            defined_in=dfiles,
            owner=owner,
            declared_in=len(files),
            files=files,
            headers=sorted({p for p, _ in hdecls.get(sym, [])}),
            status=status,
            conflict=ckind,
            top_user=users.most_common(1)[0][0],
            users=dict(users),
            variants=[dict(type=k, files=sorted(set(v))) for k, v in sorted(variants.items(), key=lambda kv: -len(kv[1]))],
            aliases=sorted({d['name'] for _, d in entries if d['asm']}),
        ))

    # --- structs ----------------------------------------------------------
    known_global = {}
    struct_rows = []
    # header structs first (seed sizes), then each .c with its own definitions
    hlay = Layout(known_global)
    for _ in range(2):
        for p, fs in hscans.items():
            for skind, tag, td, body, packed, line, st in fs.structs:
                sz, al, fields = hlay.layout(skind, body, packed)
                if sz is not None:
                    if tag:
                        known_global['%s %s' % (skind, tag)] = (sz, al)
                    if td:
                        known_global[td] = (sz, al)
    header_structs = collections.defaultdict(list)
    for p, fs in hscans.items():
        for skind, tag, td, body, packed, line, st in fs.structs:
            for nm in filter(None, ['%s %s' % (skind, tag) if tag else None, td]):
                header_structs[nm].append(p)
    for p, fs in scans.items():
        known = dict(known_global)
        lay = Layout(known)
        for _ in range(2):
            rows = []
            for skind, tag, td, body, packed, line, st in fs.structs:
                sz, al, fields = lay.layout(skind, body, packed)
                if sz is not None:
                    if tag:
                        known['%s %s' % (skind, tag)] = (sz, al)
                    if td:
                        known[td] = (sz, al)
                name = ('%s %s' % (skind, tag)) if tag else (td or '<anon>')
                sig = None
                if sz is not None:
                    sig = 'size=0x%x;' % sz + ','.join('%x:%d' % (o, s) for o, s, _ in fields)
                rows.append(dict(file=p, line=line, name=name, typedef=td, kind=skind,
                                 size=sz, nfields=len(fields), signature=sig,
                                 fields=[f[2] for f in fields],
                                 in_header=sorted(set(header_structs.get(name, []) + header_structs.get(td or '', [])))))
        struct_rows.extend(rows)

    by_name = collections.defaultdict(list)
    by_sig = collections.defaultdict(list)
    for r in struct_rows:
        by_name[r['name']].append(r)
        if r['signature'] and r['nfields'] >= 3:
            by_sig[r['signature']].append(r)

    # --- output -----------------------------------------------------------
    os.makedirs(args.out_dir, exist_ok=True)
    with open(os.path.join(args.out_dir, 'symbols.json'), 'w') as f:
        json.dump(symbols, f, indent=1)
    with open(os.path.join(args.out_dir, 'symbols.tsv'), 'w') as f:
        f.write('symbol\tkind\towner\tdefined_in\tdeclared_in\tstatus\tconflict\ttop_user\theaders\tvariants\n')
        for s in symbols:
            f.write('\t'.join([s['symbol'], s['kind'], s['owner'], ','.join(s['defined_in']) or '?',
                               str(s['declared_in']), s['status'], s['conflict'], s['top_user'],
                               ','.join(s['headers']),
                               ' | '.join('%s [%d]' % (v['type'], len(v['files'])) for v in s['variants'])]) + '\n')
    with open(os.path.join(args.out_dir, 'structs.tsv'), 'w') as f:
        f.write('file\tline\tname\tkind\tsize\tnfields\tsame_name_copies\tsame_layout_copies\tin_header\tsignature\n')
        for r in struct_rows:
            f.write('\t'.join(str(x) for x in [
                r['file'], r['line'], r['name'], r['kind'],
                '?' if r['size'] is None else hex(r['size']), r['nfields'],
                len(by_name[r['name']]), len(by_sig.get(r['signature'], [])) if r['signature'] else 0,
                ','.join(r['in_header']), r['signature'] or '?']) + '\n')
    with open(os.path.join(args.out_dir, 'structs.json'), 'w') as f:
        json.dump(dict(
            by_name={k: v for k, v in sorted(by_name.items()) if len(v) > 1},
            by_layout={k: v for k, v in sorted(by_sig.items()) if len({r['name'] for r in v}) > 1 or len(v) > 1},
        ), f, indent=1)

    # --- summary ----------------------------------------------------------
    print('Files: %d .c (src/ + lib/), %d headers' % (len(c_files), len(h_files)))
    print('Declarations in .c files: %d (externs + prototypes of functions defined elsewhere)' % total_decls)
    print('Forward declarations of a symbol the same file defines (not counted): %d' % self_forward)
    print('Unique symbols: %d' % len(symbols))
    st = collections.Counter(s['status'] for s in symbols)
    print('  identical %d, equivalent %d, conflicting %d' % (st['identical'], st['equivalent'], st['conflicting']))
    ck = collections.Counter(s['conflict'] for s in symbols if s['conflict'])
    print('  conflict kinds: ' + ', '.join('%s %d' % kv for kv in ck.most_common()))
    multi = [s for s in symbols if s['declared_in'] > 1]
    print('  declared in >1 file: %d (of which conflicting %d)' % (
        len(multi), sum(1 for s in multi if s['status'] == 'conflicting')))
    print('  already declared in a header too: %d' % sum(1 for s in symbols if s['headers']))
    owners = collections.Counter(s['owner'] for s in symbols)
    print('  undefined (no definition found): %d' % owners['?'])
    print()
    print('%-16s %8s %10s %11s %8s %10s' % ('owner', 'symbols', 'consistent', 'conflicting', 'decls', 'avg files'))
    rows = collections.defaultdict(lambda: [0, 0, 0, 0])
    for s in symbols:
        r = rows[s['owner']]
        r[0] += 1
        r[1 if s['status'] != 'conflicting' else 2] += 1
        r[3] += s['declared_in']
    for o, r in sorted(rows.items(), key=lambda kv: -kv[1][0]):
        print('%-16s %8d %10d %11d %8d %10.1f' % (o, r[0], r[1], r[2], r[3], r[3] / r[0]))
    print()
    nstruct = len(struct_rows)
    dup_names = {k: v for k, v in by_name.items() if len(v) > 1}
    print('Local struct/union definitions in .c files: %d (%d unique names)' % (nstruct, len(by_name)))
    print('  names defined in more than one .c file: %d (%d copies)' % (
        len(dup_names), sum(len(v) for v in dup_names.values())))
    print('  names that also exist in a header: %d' % sum(1 for k, v in by_name.items() if v[0]['in_header']))
    print('  layout computed: %d, unknown: %d' % (
        sum(1 for r in struct_rows if r['signature']), sum(1 for r in struct_rows if not r['signature'])))
    sig_groups = {k: v for k, v in by_sig.items() if len(v) > 1}
    print('  layout signatures (>=3 fields) shared by >1 definition: %d groups, %d definitions, %d distinct names' % (
        len(sig_groups), sum(len(v) for v in sig_groups.values()),
        len({r['name'] for v in sig_groups.values() for r in v})))
    sub = collections.Counter(subsystem_of(r['file']) for r in struct_rows)
    print('  per subsystem: ' + ', '.join('%s %d' % kv for kv in sub.most_common()))
    print()
    print('Wrote %s/{symbols,structs}.{tsv,json}' % os.path.relpath(args.out_dir, ROOT))

    if args.conflicts or args.subsystem:
        print()
        for s in symbols:
            if s['status'] != 'conflicting':
                continue
            if args.subsystem and s['owner'] != args.subsystem:
                continue
            print('%s (%s, owner %s, %d files):' % (s['symbol'], s['conflict'], s['owner'], s['declared_in']))
            for v in s['variants']:
                print('    %-60s %s' % (v['type'], ' '.join(v['files'])))
    for name in args.symbol:
        for s in symbols:
            if s['symbol'] == name:
                print(json.dumps(s, indent=1))


if __name__ == '__main__':
    main()
