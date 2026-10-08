#!/usr/bin/env python3
"""Audit struct and class layouts from the compiler's own debug info (#656).

Every game translation unit (src/*/*.c and src/*/*.cpp) is compiled again
with -g, by the Makefile's own rules, into a temporary directory: the same
compiler (agbcc, old_agbcc, agbcc_arm(_patched), agbcp, old_agbcp) and the
same per-object flags as the ROM build (OLD_AGBCC_OBJS, CXX_OBJS,
NO_IMPLEMENT_INLINES_OBJS, the ARM objects, ...). The tool runs make with
an extra makefile that only adds `-g -w` to those objects, redirects
OBJ_DIR and GENERATED_INCLUDE_DIR into the temporary directory, and ends
each .s with the terminating 0 byte agbcc leaves out of .debug_abbrev (so
readelf can parse it). build/ is never read or written.

The DWARF2 of every object (arm-none-eabi-readelf --debug-dump=info) is
read into a layout database: one record per struct, union and class
definition, keyed by `name@file` (the header or source that defines it),
with its size, its members (byte offset, bitfield position and width,
type, size) and its base classes. A definition seen with two different
layouts (an `#ifdef __cplusplus` arm, say) gets one record per layout
(`name@file#2`). Anonymous structs are named after their typedef, or
`<anon>@file:line`.

Layouts are compared on their flattened leaves: every scalar, pointer,
array and bitfield member, with by-value struct members and base classes
expanded in place (`pos.x`; an anonymous union's members keep their own
names). Two leaves "match" when they cover the same bits and their types
are compatible: equal once typedefs and const/volatile are dropped
(agbcc names some base types after a typedef, so integers are compared
as s8..u32), both pointers, both integers of the same size, or arrays of
those. Fields named like padding (unk*, pad*, filler*, unused*, ...)
never conflict with anything, nor do arrays, g++ pointers to member
functions and a bitfield inside an integer of the other layout (a flags
byte and its bits). Leaves both layouts get from the same embedded
struct or base class are not compared: two classes derived from Entity
are not views of each other because both start with Entity's fields.

Subcommands:

  views [NAME...]     layouts that are a prefix or partial view of a
                      larger (or same-size) one: every field they name is
                      at an offset where the other one has a compatible
                      field or padding. One line per pair,
                        A (size) ~ B (size): kind, N matching
                      with the fields whose names (`name`) or spelled
                      types (`type~`, as in diff) differ listed under it
                      (every match is type-compatible; -v also lists
                      A's fields that are padding in B). The kind is
                      "prefix view" (A is smaller) or "same-size view",
                      plus "partial: N of its named fields are padding
                      here" when B names fields A leaves as padding. A
                      pair needs --min matching fields (default 4), no
                      conflicting field, and evidence it isn't a
                      coincidence: at least two matches, and a quarter of
                      them, agree on the name or point at (or hold)
                      structs or functions the same way. A view of a base
                      class is reported once, not again for each class
                      derived from it. Pairs of src/data/ layouts (one
                      struct per data table, such as the level rooms'
                      parameter blocks) are left out unless --data.
                      NAMEs restrict the report to pairs with one of them.
  diff A B [C...]     field-by-field side-by-side table of two or more
                      layouts, one row per bit range. Rows are flagged
                      `name` (no common name), `type` (incompatible
                      types), `type~` (compatible, spelled differently:
                      u16 vs s16, two struct pointers) and `shape` (a
                      named field overlaps fields of another extent).
                      --types shows each field's type; --flagged,
                      --names-only and --types-only filter the rows.
  names [NAME...]     the families of layouts the views relation links
                      (its connected groups), and for each the offsets
                      where two layouts of a views pair name the same
                      bytes differently: one row per offset (`/N` is the
                      width in bytes) with each name and the layouts
                      using it. NAMEs restrict it to their families.
  show NAME           one layout, flattened.

A NAME is a struct/class name (`Entity`), optionally with a file suffix to
pick one definition (`actor@include/actor.h`, `actor@actor.h`), or a full
key (`cutscene_player@include/cutscene.h#2`). An ambiguous NAME lists the
candidates. A file shown as `src/data/x.c?` is a guess: an object with no
code has no line-number file table, so a layout only such objects define
is attributed to the first of them.

Compiling every TU takes about 15 seconds on 16 cores. `--db FILE` saves
the database as JSON and reuses it on later runs (`--rebuild` recompiles;
do that after editing a header). `--keep DIR` compiles into DIR and keeps
the -g objects there (later runs with the same DIR only rebuild what
changed). `--lib` adds lib/*/{src,data}/*.c. Needs the devshell's
arm-none-eabi binutils and the compilers in tools/agbcc/bin, as `make`
does; an object that fails to compile is reported and skipped.

Usage:
  tools/layout_audit.py --db /tmp/layouts.json views
  tools/layout_audit.py --db /tmp/layouts.json views Ctrl ctrl
  tools/layout_audit.py --db /tmp/layouts.json diff --types PauseMenu pause_menu
  tools/layout_audit.py --db /tmp/layouts.json names pool_manager
  tools/layout_audit.py --db /tmp/layouts.json show Entity
"""

import argparse
import collections
import concurrent.futures
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
READELF = 'arm-none-eabi-readelf'

# Added to the Makefile by `make -f Makefile -f <this>`. -w: the warnings
# don't matter here, and agbcc_arm warns about -g with
# -fomit-frame-pointer, which -Werror would turn into an error.
EXTRA_MAKEFILE = r"""
ZERO_PAD_TEXT := \t.text\n\t.align\t2, 0\n\t.section\t.debug_abbrev\n\t.byte\t0\n
$(C_OBJS) $(CXX_OBJS) $(LIB_C_OBJS): CC1FLAGS += -g -w
.PHONY: layout-audit-objs
layout-audit-objs: $(LAYOUT_AUDIT_OBJS)
"""

AGGREGATE_TAGS = {
    'DW_TAG_structure_type': 'struct',
    'DW_TAG_union_type': 'union',
    'DW_TAG_class_type': 'class',
}

# Leaf classes whose overlap with differently-shaped fields is not a
# conflict: an array may be split into fields elsewhere, and a C view
# spells a g++ pointer to member function as a struct.
LOOSE = ('array', 'pmf')

DATA_DIR = 'src/data/'

PAD_RE = re.compile(r'^(unk|pad|filler|unused|reserved|gap|field_|_+\d|_+$)', re.I)

# ---------------------------------------------------------------------------
# Compiling


def compile_objects(workdir, include_lib, jobs):
    """Build every game object with -g into workdir; return [(source, obj)]."""
    extra = os.path.join(workdir, 'layout_audit.mk')
    with open(extra, 'w') as f:
        f.write(EXTRA_MAKEFILE)
    objdir = os.path.join(workdir, 'obj')
    objs = '$(C_OBJS) $(CXX_OBJS)' + (' $(LIB_C_OBJS)' if include_lib else '')
    cmd = ['make', '--no-print-directory', '-k', f'-j{jobs}',
           '-f', 'Makefile', '-f', extra,
           f'OBJ_DIR={objdir}',
           f'GENERATED_INCLUDE_DIR={os.path.join(workdir, "include")}',
           f'LAYOUT_AUDIT_OBJS={objs}',
           'layout-audit-objs']
    log = os.path.join(workdir, 'make.log')
    with open(log, 'w') as f:
        rc = subprocess.run(cmd, cwd=ROOT, stdout=f, stderr=subprocess.STDOUT).returncode
    sources = []
    for top in ('src',) + (('lib',) if include_lib else ()):
        for dirpath, _, files in os.walk(os.path.join(ROOT, top)):
            for fn in files:
                if fn.endswith(('.c', '.cpp')):
                    sources.append(os.path.relpath(os.path.join(dirpath, fn), ROOT))
    out, missing = [], []
    for src in sorted(sources):
        if src.startswith('src/') and src.count('/') != 2:
            continue
        if src.startswith('lib/') and not re.match(r'lib/[^/]+/(src|data)/[^/]+\.c$', src):
            continue
        obj = os.path.join(objdir, os.path.splitext(src)[0] + '.o')
        if os.path.exists(obj):
            out.append((src, obj))
        else:
            missing.append(src)
    if missing:
        print(f'warning: {len(missing)} objects failed to compile (see {log}'
              f'{"" if rc == 0 else ", make exit status " + str(rc)}): '
              + ', '.join(missing[:8]) + (' ...' if len(missing) > 8 else ''),
              file=sys.stderr)
    return out


# ---------------------------------------------------------------------------
# DWARF parsing

DIE_RE = re.compile(r'^\s*<(\d+)><([0-9a-f]+)>: Abbrev Number: (\d+)(?: \((\w+)\))?')
ATTR_RE = re.compile(r'^\s*<[0-9a-f]+>\s+(DW_AT_\w+)\s*:\s*(.*)$')
FILE_RE = re.compile(r'^\s+(\d+)\t\d+\t\d+\t\d+\t(.+)$')
REF_RE = re.compile(r'<0x([0-9a-f]+)>')


def readelf(obj):
    info = subprocess.run([READELF, '--debug-dump=info', obj],
                          capture_output=True, text=True).stdout
    line = subprocess.run([READELF, '--debug-dump=rawline', obj],
                          capture_output=True, text=True).stdout
    return info, line


def parse_dies(info):
    dies = {}
    stack = []
    cur = None
    for text in info.splitlines():
        m = DIE_RE.match(text)
        if m:
            depth, off, abbrev, tag = int(m.group(1)), int(m.group(2), 16), m.group(3), m.group(4)
            if abbrev == '0':
                cur = None
                continue
            cur = {'tag': tag, 'attrs': {}, 'children': []}
            dies[off] = cur
            while stack and stack[-1][0] >= depth:
                stack.pop()
            if stack:
                stack[-1][1]['children'].append(off)
            stack.append((depth, cur))
            continue
        m = ATTR_RE.match(text)
        if m and cur is not None:
            cur['attrs'][m.group(1)] = m.group(2).strip()
    return dies


def parse_files(line):
    files = {}
    for text in line.splitlines():
        m = FILE_RE.match(text)
        if m:
            files[int(m.group(1))] = os.path.normpath(m.group(2).strip())
    return files


def attr_ref(d, name='DW_AT_type'):
    m = REF_RE.match(d['attrs'].get(name, ''))
    return int(m.group(1), 16) if m else None


def attr_name(d):
    v = d['attrs'].get('DW_AT_name')
    if v is None:
        return None
    if v.startswith('(indirect'):
        v = v.split('): ', 1)[-1]
    return v


def attr_int(d, name, default=None):
    v = d['attrs'].get(name)
    if v is None:
        return default
    m = re.match(r'(-?(?:0x[0-9a-f]+|\d+))', v)
    return int(m.group(1), 0) if m else default


def member_offset(d):
    v = d['attrs'].get('DW_AT_data_member_location')
    if v is None:
        return 0
    m = re.search(r'DW_OP_plus_uconst: (\d+)', v)
    if m:
        return int(m.group(1))
    m = re.match(r'(\d+)', v)
    return int(m.group(1)) if m else 0


def base_canon(d):
    """s8/u8/.../u32 for an integer base type, from its size and encoding.

    agbcc sometimes names a base type after a typedef of it (a base type
    DIE called `u8`), so the DWARF name can't be trusted.
    """
    size = attr_int(d, 'DW_AT_byte_size', 0)
    enc = attr_int(d, 'DW_AT_encoding', 0)
    if enc in (5, 6):  # DW_ATE_signed, DW_ATE_signed_char
        return f's{size * 8}'
    if enc in (7, 8):  # DW_ATE_unsigned, DW_ATE_unsigned_char
        return f'u{size * 8}'
    if enc == 2:
        return 'bool'
    if enc == 4:
        return f'float{size * 8}'
    return attr_name(d)


class TU:
    """One object's DIEs, turned into layout records in a shared DB."""

    def __init__(self, src, dies, files, db):
        self.src = src
        self.dies = dies
        self.files = files
        self.db = db
        self.keys = {}
        self.typedef_of = {}
        for off, d in dies.items():
            if d['tag'] == 'DW_TAG_typedef':
                t = attr_ref(d)
                if t is not None and t not in self.typedef_of:
                    self.typedef_of[t] = attr_name(d)

    def strip(self, off, depth=0):
        """Follow typedefs and cv-qualifiers to the underlying type DIE."""
        while off is not None and depth < 16:
            d = self.dies.get(off)
            if d is None or d['tag'] not in ('DW_TAG_typedef', 'DW_TAG_const_type',
                                             'DW_TAG_volatile_type'):
                return off
            off = attr_ref(d)
            depth += 1
        return off

    def agg_name(self, off):
        d = self.dies[off]
        return attr_name(d) or self.typedef_of.get(off)

    def type_str(self, off, canon=False, depth=0):
        """C spelling of a type; canon drops typedefs, cv and struct/class."""
        if off is None:
            return 'void'
        d = self.dies.get(off)
        if d is None or depth > 12:
            return '?'
        t, nxt = d['tag'], attr_ref(d)
        if t == 'DW_TAG_base_type':
            return base_canon(d) if canon else attr_name(d)
        if t == 'DW_TAG_typedef':
            return self.type_str(nxt, canon, depth + 1) if canon else attr_name(d)
        if t in AGGREGATE_TAGS:
            nm = self.agg_name(off) or '<anon>'
            if canon:
                return nm if AGGREGATE_TAGS[t] != 'union' else 'union ' + nm
            return f'{AGGREGATE_TAGS[t]} {nm}' if attr_name(d) else nm
        if t == 'DW_TAG_enumeration_type':
            return 'enum ' + (attr_name(d) or self.typedef_of.get(off) or '<anon>')
        if t == 'DW_TAG_pointer_type':
            return self.type_str(nxt, canon, depth + 1) + ' *'
        if t == 'DW_TAG_reference_type':
            return self.type_str(nxt, canon, depth + 1) + ' &'
        if t in ('DW_TAG_const_type', 'DW_TAG_volatile_type'):
            inner = self.type_str(nxt, canon, depth + 1)
            q = 'const ' if t == 'DW_TAG_const_type' else 'volatile '
            return inner if canon or inner.startswith(q) else q + inner
        if t == 'DW_TAG_array_type':
            return self.type_str(nxt, canon, depth + 1) + ''.join(
                f'[{n if n is not None else ""}]' for n in self.array_dims(d))
        if t == 'DW_TAG_subroutine_type':
            return 'fn'
        if t == 'DW_TAG_ptr_to_member_type':
            return 'pmf'
        return t.replace('DW_TAG_', '')

    def array_dims(self, d):
        dims = []
        for c in d['children']:
            ub = attr_int(self.dies[c], 'DW_AT_upper_bound')
            dims.append(ub + 1 if ub is not None and 0 <= ub < 0x10000000 else None)
        return dims

    def size_of(self, off, depth=0):
        d = self.dies.get(off)
        if d is None or depth > 12:
            return None
        if 'DW_AT_byte_size' in d['attrs']:
            return attr_int(d, 'DW_AT_byte_size')
        t = d['tag']
        if t in ('DW_TAG_typedef', 'DW_TAG_const_type', 'DW_TAG_volatile_type'):
            return self.size_of(attr_ref(d), depth + 1)
        if t == 'DW_TAG_array_type':
            n = 1
            for dim in self.array_dims(d):
                n *= dim or 0
            es = self.size_of(attr_ref(d), depth + 1)
            return None if es is None else n * es
        if t in ('DW_TAG_pointer_type', 'DW_TAG_reference_type'):
            return 4
        return None

    def type_class(self, off):
        """'ptr', 'int<N>', 'array', 'agg' or 'other', for compatibility."""
        base = self.strip(off)
        d = self.dies.get(base)
        if d is None:
            return 'other'
        t = d['tag']
        if t in ('DW_TAG_pointer_type', 'DW_TAG_reference_type'):
            return 'ptr'
        if t in ('DW_TAG_base_type', 'DW_TAG_enumeration_type'):
            return f'int{self.size_of(base)}'
        if t == 'DW_TAG_array_type':
            return 'array'
        if t in AGGREGATE_TAGS:
            return 'agg'
        return 'other'

    def file_of(self, d):
        fi = attr_int(d, 'DW_AT_decl_file')
        return self.files.get(fi) if fi is not None else None

    def is_pmf(self, off):
        """g++ 2.9's pointer-to-member-function record {__delta, __index, ...}."""
        names = {attr_name(self.dies[c]) for c in self.dies[off]['children']}
        return {'__delta', '__index', '__pfn_or_delta2'} <= names

    def key_of(self, off):
        """The DB key of the aggregate DIE at off (recording it if new)."""
        if off in self.keys:
            return self.keys[off]
        d = self.dies[off]
        self.keys[off] = None  # guards against by-value recursion
        kind = AGGREGATE_TAGS[d['tag']]
        name = self.agg_name(off)
        members, bases = [], []
        first_line, first_file = None, None
        for c in d['children']:
            cd = self.dies[c]
            if cd['tag'] not in ('DW_TAG_member', 'DW_TAG_inheritance'):
                continue
            if cd['tag'] == 'DW_TAG_member' and kind != 'union' \
                    and 'DW_AT_data_member_location' not in cd['attrs']:
                continue  # a static data member
            tref = attr_ref(cd)
            byte = member_offset(cd)
            size = self.size_of(tref)
            m = {'name': attr_name(cd) or '', 'off': byte, 'type': self.type_str(tref),
                 'canon': self.type_str(tref, canon=True), 'size': size,
                 'class': self.type_class(tref), 'bit': byte * 8,
                 'bits': (size or 0) * 8, 'sub': None}
            sub = self.strip(tref)
            if m['class'] == 'array':
                m['elem'] = self.type_class(attr_ref(self.dies[sub]))
            if sub in self.dies and self.dies[sub]['tag'] in AGGREGATE_TAGS \
                    and 'DW_AT_declaration' not in self.dies[sub]['attrs']:
                if self.is_pmf(sub):
                    m['class'] = 'pmf'
                else:
                    m['sub'] = self.key_of(sub)
            if cd['tag'] == 'DW_TAG_inheritance':
                m['name'] = '<base>'
                m['base'] = True
                bases.append(m['type'])
            if 'DW_AT_bit_size' in cd['attrs']:
                width = attr_int(cd, 'DW_AT_bit_size')
                unit = attr_int(cd, 'DW_AT_byte_size', size) or 0
                msb = attr_int(cd, 'DW_AT_bit_offset', 0)
                m['bit'] = byte * 8 + unit * 8 - msb - width
                m['bits'] = width
                m['bitfield'] = True
            if m['name'].startswith('_vptr'):
                m['vptr'] = True
            if first_line is None and 'DW_AT_decl_line' in cd['attrs']:
                first_line = attr_int(cd, 'DW_AT_decl_line')
                first_file = self.file_of(cd)
            members.append(m)
        # The struct's own decl_file/line can be those of an earlier
        # forward declaration (`struct actor;` in gfx.h), so the first
        # member's are used.
        file = first_file or self.file_of(d)
        line = first_line or attr_int(d, 'DW_AT_decl_line', 0)
        rec = {'kind': kind, 'name': name, 'file': file, 'line': line,
               'size': attr_int(d, 'DW_AT_byte_size', 0), 'members': members,
               'bases': bases, 'anon': not name, 'tus': [self.src]}
        key = self.db.add(rec)
        self.keys[off] = key
        return key

    def collect(self):
        for off, d in self.dies.items():
            if d['tag'] in AGGREGATE_TAGS and 'DW_AT_declaration' not in d['attrs'] \
                    and 'DW_AT_byte_size' in d['attrs'] and not self.is_pmf(off):
                self.key_of(off)


class LayoutDB:
    def __init__(self, records=None):
        self.records = {}
        self._sig = {}
        self._ident = {}

    @staticmethod
    def base_key(r):
        return r['name'] if r['anon'] else f"{r['name']}@{r['file']}"

    @staticmethod
    def signature(r):
        return (r['size'], tuple((m['name'], m['bit'], m['bits'], m['canon'])
                                 for m in r['members']))

    def add(self, rec):
        """Record rec (or merge it into an equal one); return its key.

        An object without code has an empty line-number file table, so its
        records have no file. Those are matched by name, line and layout
        against the records of the objects that do have one (build_db reads
        them first), or else attributed to the object itself, marked
        `file_guessed`.
        """
        sig = self.signature(rec)
        ident = (rec['name'] or '', rec['line'], sig)
        if rec['file'] is None:
            k = self._ident.get(ident)
            if k is not None:
                if rec['tus'][0] not in self.records[k]['tus']:
                    self.records[k]['tus'].append(rec['tus'][0])
                return k
            rec['file'] = rec['tus'][0]
            rec['file_guessed'] = True
        if rec['anon']:
            rec['name'] = f"<anon>@{rec['file']}:{rec['line']}"
        base = self.base_key(rec)
        variants = self._sig.setdefault(base, [])
        for s, k in variants:
            if s == sig:
                if rec['tus'][0] not in self.records[k]['tus']:
                    self.records[k]['tus'].append(rec['tus'][0])
                return k
        key = base if not variants else f'{base}#{len(variants) + 1}'
        variants.append((sig, key))
        self.records[key] = rec
        self._ident.setdefault(ident, key)
        return key


def build_db(objects, jobs):
    db = LayoutDB()
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        dumps = list(ex.map(lambda so: (so[0],) + readelf(so[1]), objects))
    tus = [(src, parse_dies(info), parse_files(line)) for src, info, line in dumps]
    # The objects with a file table first (see LayoutDB.add).
    for src, dies, files in sorted(tus, key=lambda t: not t[2]):
        TU(src, dies, files, db).collect()
    return db.records


# ---------------------------------------------------------------------------
# Flattening and comparing


class Layouts:
    def __init__(self, records):
        self.rec = records
        self._flat = {}
        self.by_name = collections.defaultdict(list)
        for k, r in records.items():
            if not r['anon']:
                self.by_name[r['name']].append(k)

    def resolve(self, spec):
        """A NAME or NAME@file-suffix (or an exact key) -> key, or exit."""
        if spec in self.rec:
            return spec
        name, _, suffix = spec.partition('@')
        cands = self.by_name.get(name, [])
        if suffix:
            cands = [k for k in cands if self.rec[k]['file'].endswith(suffix)
                     or k.endswith(suffix)]
        if len(cands) == 1:
            return cands[0]
        if not cands:
            sys.exit(f'error: no struct/class named {spec!r}')
        sys.exit(f'error: {spec!r} is ambiguous: ' + ', '.join(sorted(cands)))

    def label(self, key):
        r = self.rec[key]
        return f"{r['kind']} {r['name']}"

    def where(self, key):
        r = self.rec[key]
        return f"{r['file']}{'?' if r.get('file_guessed') else ''}:{r['line']}"

    def flat(self, key, depth=0):
        """Leaves of key at offset 0: dicts with bit, bits, path, owners, ..."""
        if key in self._flat:
            return self._flat[key]
        out = []
        for m in self.rec[key]['members']:
            sub = m['sub']
            if sub is not None and m['class'] == 'agg' and sub in self.rec and depth < 12:
                prefix = '' if m.get('base') or not m['name'] else m['name'] + '.'
                for leaf in self.flat(sub, depth + 1):
                    out.append(dict(leaf, bit=leaf['bit'] + m['bit'],
                                    path=prefix + leaf['path'],
                                    owners=leaf['owners'] | {key}))
                continue
            name = m['name']
            out.append({'bit': m['bit'], 'bits': m['bits'], 'path': name,
                        'type': m['type'], 'canon': m['canon'], 'class': m['class'],
                        'pad': is_pad(name), 'vptr': m.get('vptr', False),
                        'bitfield': m.get('bitfield', False), 'elem': m.get('elem'),
                        'owners': frozenset((key,))})
        self._flat[key] = out
        return out

    def listed(self, key):
        """Not anonymous and not a compiler-made record (__vtbl_ptr_type)."""
        r = self.rec[key]
        return not r['anon'] and r['file'] != '<internal>'

    def named(self, key):
        return [x for x in self.flat(key) if not x['pad']]


def is_pad(name):
    return not name or bool(PAD_RE.match(name))


def leaf_name(leaf):
    n = leaf['path'].rsplit('.', 1)[-1]
    return '_vptr' if leaf['vptr'] else n


def same_name(a, b):
    """The leaf names agree, or one is a component of the other's path
    (`dispcnt` and `dispcnt.raw`)."""
    na, nb = leaf_name(a), leaf_name(b)
    return na == nb or na in b['path'].split('.') or nb in a['path'].split('.')


def scalar_compatible(ca, cb):
    return ca == cb and (ca in ('ptr', 'pmf') or ca.startswith('int'))


def compatible(a, b):
    if a['canon'] == b['canon'] or (a['vptr'] and b['vptr']):
        return True
    if scalar_compatible(a['class'], b['class']):
        return True
    if a['class'] == b['class'] == 'array' and a['elem'] and \
            scalar_compatible(a['elem'], b['elem']):
        return True
    return False


def overlaps(a, b):
    return a['bit'] < b['bit'] + b['bits'] and b['bit'] < a['bit'] + a['bits']


def contains(outer, inner):
    return outer['bit'] <= inner['bit'] and \
        inner['bit'] + inner['bits'] <= outer['bit'] + outer['bits']


def splits(a, b):
    """One of a, b is a bitfield inside the other, an integer (flags vs bits)."""
    if a['bitfield'] and not b['bitfield']:
        return b['class'].startswith('int') and contains(b, a)
    if b['bitfield'] and not a['bitfield']:
        return a['class'].startswith('int') and contains(a, b)
    return False


def fmt_off(bit, bits):
    s = f'+0x{bit // 8:02x}'
    if bit % 8 or bits % 8:
        s += f'.{bit % 8}:{bits}'
    return s


def compare(L, ka, kb):
    """A's named leaves against B's: matches [(a, b)], conflicts [(a, [b])]
    and the leaves B has nothing named for."""
    B = L.flat(kb)
    by_pos = collections.defaultdict(list)
    for x in B:
        by_pos[(x['bit'], x['bits'])].append(x)
    matches, conflicts, alone = [], [], []
    for a in L.named(ka):
        same = by_pos.get((a['bit'], a['bits']), [])
        if any(a['owners'] & b['owners'] for b in same):
            continue  # both get it from the same embedded struct or base
        named = [b for b in same if not b['pad']]
        hit = [b for b in named if compatible(a, b)]
        if hit:
            best = [b for b in hit if same_name(a, b)] or hit
            matches.append((a, best[0]))
            continue
        over = [b for b in B if overlaps(a, b) and not b['pad']
                and not (a['owners'] & b['owners']) and not splits(a, b)]
        if over and not (a['class'] in LOOSE or all(b['class'] in LOOSE for b in over)):
            conflicts.append((a, over))
        else:
            alone.append(a)
    return matches, conflicts, alone


BASE_WORDS = re.compile(r'^([su]\d+|bool|float\d+|void|fn|pmf|enum|union|const|volatile)$')


def type_shape(canon):
    """canon with every struct/class/enum name replaced by S (`S *[256]`)."""
    return re.sub(r'[A-Za-z_]\w*', lambda m: m.group(0) if BASE_WORDS.match(m.group(0))
                  else 'S', canon)


def evidence(matches):
    """Matches that agree on more than an integer's or a void *'s size: the
    names agree, or both types point at (or hold) structs or functions the
    same way (`struct pool_node *[256]` and `struct CrateGridNode *[256]`)."""
    n = 0
    for a, b in matches:
        sa = type_shape(a['canon'])
        if same_name(a, b) or (sa == type_shape(b['canon']) and re.search(r'\bS\b|fn', sa)):
            n += 1
    return n


def find_views(L, min_match, only=None, data=False):
    """[(A, B, matches, alone)] for A a view of B (A.size <= B.size).

    A pair needs min_match matching fields, no conflicting one, and some
    evidence it isn't a coincidence: at least two matches, and a quarter
    of them, are evidence() matches.
    A ~ B is dropped when every match is a field B gets from one embedded
    struct or base class C and A ~ C is reported itself (A ~ Sprite, not
    A ~ each class derived from Sprite), and the same the other way round.
    Pairs of src/data/ layouts (one struct per data table instance, like
    the level_params_roomNN) are left out unless data is set.
    """
    index = collections.defaultdict(list)
    for k in L.rec:
        if L.listed(k):
            for x in L.named(k):
                index[(x['bit'], x['bits'], x['class'])].append((k, x['owners']))
    found = {}
    for ka, ra in L.rec.items():
        if not L.listed(ka) or ra['kind'] == 'union' or len(L.named(ka)) < min_match:
            continue
        counts = collections.Counter()
        for a in L.named(ka):
            seen = set()
            for kb, owners in index.get((a['bit'], a['bits'], a['class']), ()):
                if kb != ka and kb not in seen and not (owners & a['owners']):
                    seen.add(kb)
                    counts[kb] += 1
        for kb, n in counts.items():
            rb = L.rec[kb]
            if n < min_match or rb['kind'] == 'union' or ra['size'] > rb['size']:
                continue
            if ra['size'] == rb['size'] and kb < ka:
                continue  # same-size pairs are reported once
            matches, conflicts, alone = compare(L, ka, kb)
            if len(matches) < min_match or conflicts:
                continue
            ev = evidence(matches)
            if ev < 2 or ev * 4 < len(matches):
                continue
            found[(ka, kb)] = (matches, alone)
    out = []
    for (ka, kb), (matches, alone) in found.items():
        via_b = frozenset.intersection(*(b['owners'] for a, b in matches)) - {kb}
        if any((ka, c) in found or (c, ka) in found for c in via_b):
            continue
        via_a = frozenset.intersection(*(a['owners'] for a, b in matches)) - {ka}
        if any((kb, c) in found or (c, kb) in found for c in via_a):
            continue
        if not data and L.rec[ka]['file'].startswith(DATA_DIR) \
                and L.rec[kb]['file'].startswith(DATA_DIR):
            continue
        if only and ka not in only and kb not in only:
            continue
        out.append((ka, kb, matches, alone))
    return out


# ---------------------------------------------------------------------------
# Subcommands


def cmd_views(L, args):
    only = {L.resolve(n) for n in args.names} if args.names else None
    res = find_views(L, args.min, only, args.data)
    res.sort(key=lambda x: (L.rec[x[0]]['file'], L.rec[x[0]]['line'], -len(x[2])))
    for ka, kb, matches, alone in res:
        ra, rb = L.rec[ka], L.rec[kb]
        kind = 'same-size view' if ra['size'] == rb['size'] else 'prefix view'
        renamed = [(a, b) for a, b in matches if not same_name(a, b)
                   and not (a['vptr'] or b['vptr'])]
        retyped = [(a, b) for a, b in matches if a['canon'] != b['canon']
                   and not (a['vptr'] or b['vptr'])]
        hidden = [b for b in L.named(kb) if b['bit'] < ra['size'] * 8
                  and not any(overlaps(a, b) for a in L.named(ka))]
        extra = f', partial: {len(hidden)} of its named fields are padding here' if hidden else ''
        print(f"{L.where(ka)} {L.label(ka)} (0x{ra['size']:x}) ~ {L.label(kb)} "
              f"@{L.where(kb)} (0x{rb['size']:x}): {kind}, {len(matches)} matching"
              f"{extra}")
        for a, b in matches:
            flags = []
            if (a, b) in renamed:
                flags.append('name')
            if (a, b) in retyped:
                flags.append('type~')
            if flags:
                print(f"    {fmt_off(a['bit'], a['bits']):<11} {a['path']} ({a['type']})"
                      f"  vs  {b['path']} ({b['type']})  [{', '.join(flags)}]")
        if args.verbose:
            for a in alone:
                print(f"    {fmt_off(a['bit'], a['bits']):<11} {a['path']} ({a['type']})"
                      f"  vs  (padding)")
    print(f'{len(res)} views', file=sys.stderr)


def cmd_diff(L, args):
    keys = [L.resolve(n) for n in args.names]
    rows = collections.defaultdict(lambda: [[] for _ in keys])
    for i, k in enumerate(keys):
        for x in L.flat(k):
            rows[(x['bit'], x['bits'])][i].append(x)
    w = args.width
    print(f"{'offset':<12}" + ''.join(
        f"{(L.rec[k]['name'] + ' (0x%x)' % L.rec[k]['size'])[:w - 1]:<{w}}" for k in keys) + 'flags')
    print(f"{'':<12}" + ''.join(f"{L.where(k)[-(w - 1):]:<{w}}" for k in keys))
    nflag = collections.Counter()
    allleaves = [L.flat(k) for k in keys]
    for pos in sorted(rows, key=lambda p: (p[0], -p[1])):
        cols = rows[pos]
        present = [c for c in cols if c]
        flags = []
        named = [[x for x in c if not x['pad']] for c in cols if c]
        named = [c for c in named if c]
        if len(named) >= 2 and not any(x['vptr'] for c in named for x in c):
            ref = named[0]
            if any(not any(same_name(x, y) for x in ref for y in c) for c in named[1:]):
                flags.append('name')
            if any(not any(compatible(x, y) for x in ref for y in c) for c in named[1:]):
                flags.append('type')
            elif any(not {x['canon'] for x in ref} & {y['canon'] for y in c}
                     for c in named[1:]):
                flags.append('type~')
        bit, bits = pos
        mine = [x for c in present for x in c if not x['pad']]
        for i, c in enumerate(cols):
            if not c and any(overlaps(x, y) and not y['pad'] and not splits(x, y)
                             for x in mine for y in allleaves[i]):
                flags.append('shape')
                break
        for f in flags:
            nflag[f] += 1
        if args.names_only and 'name' not in flags:
            continue
        if args.types_only and not any(f.startswith('type') for f in flags):
            continue
        if args.flagged and not flags:
            continue
        cells = []
        for c in cols:
            txt = ', '.join(x['path'] + ('' if not args.types else f" ({x['type']})")
                            for x in c)
            cells.append(f"{txt[:w - 1]:<{w}}")
        print(f"{fmt_off(bit, bits):<12}" + ''.join(cells) + ' '.join(flags))
    print(f"{sum(nflag.values())} flagged rows: " + ', '.join(
        f'{n} {f}' for f, n in sorted(nflag.items())) if nflag else '0 flagged rows',
        file=sys.stderr)


def cmd_names(L, args):
    res = find_views(L, args.min, data=args.data)
    parent = {}

    def find(x):
        while parent.get(x, x) != x:
            parent[x] = parent.get(parent[x], parent[x])
            x = parent[x]
        return x

    for ka, kb, _, _ in res:
        ra, rb = find(ka), find(kb)
        if ra != rb:
            parent[ra] = rb
    families = collections.defaultdict(set)
    for ka, kb, _, _ in res:
        families[find(ka)].update((ka, kb))
    only = {L.resolve(n) for n in args.names} if args.names else None
    shown = 0
    for fam in sorted(families.values(), key=lambda f: (-len(f), sorted(f))):
        if only and not (fam & only):
            continue
        # Only the fields a views pair matched: two siblings of one base
        # are free to name what follows the shared prefix differently.
        bad = collections.defaultdict(lambda: collections.defaultdict(set))
        for ka, kb, matches, _ in res:
            if ka not in fam:
                continue
            for a, b in matches:
                if not same_name(a, b) and not (a['vptr'] or b['vptr']):
                    pos = (a['bit'], a['bits'])
                    bad[pos][leaf_name(a)].add(L.rec[ka]['name'])
                    bad[pos][leaf_name(b)].add(L.rec[kb]['name'])
        if not bad:
            continue
        shown += 1
        print(f"family of {len(fam)}: " + ', '.join(
            f"{L.rec[k]['name']} ({L.rec[k]['file']})" for k in sorted(
                fam, key=lambda k: (-L.rec[k]['size'], k))))
        for pos in sorted(bad, key=lambda p: (p[0], -p[1])):
            names = bad[pos]
            width = '' if pos[0] % 8 or pos[1] % 8 else f'/{pos[1] // 8}'
            print(f"    {fmt_off(*pos) + width:<13} " + ' | '.join(
                f"{n}: {', '.join(sorted(s))}" for n, s in sorted(
                    names.items(), key=lambda t: (-len(t[1]), t[0]))))
    print(f'{shown} families with name disagreements', file=sys.stderr)


def cmd_show(L, args):
    k = L.resolve(args.name)
    r = L.rec[k]
    print(f"{L.label(k)} @{L.where(k)} (0x{r['size']:x})"
          + (f" : {', '.join(r['bases'])}" if r['bases'] else '')
          + f"  [{len(r['tus'])} TUs]")
    for x in L.flat(k):
        print(f"    {fmt_off(x['bit'], x['bits']):<11} {x['type']:<28} {x['path']}")


# ---------------------------------------------------------------------------


def load_db(args):
    if args.db and os.path.exists(args.db) and not args.rebuild:
        with open(args.db) as f:
            return json.load(f)
    workdir = args.keep or tempfile.mkdtemp(prefix='layout_audit_')
    os.makedirs(workdir, exist_ok=True)
    try:
        objects = compile_objects(workdir, args.lib, args.jobs)
        records = build_db(objects, args.jobs)
    finally:
        if not args.keep:
            shutil.rmtree(workdir, ignore_errors=True)
    print(f'{len(records)} layouts from {len(objects)} objects', file=sys.stderr)
    if args.db:
        with open(args.db, 'w') as f:
            json.dump(records, f, indent=0, sort_keys=True)
    return records


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--db', help='layout database JSON: read if it exists, else written')
    ap.add_argument('--rebuild', action='store_true', help='recompile even if --db exists')
    ap.add_argument('--keep', metavar='DIR', help='compile into DIR and keep the -g objects')
    ap.add_argument('--lib', action='store_true', help='also read lib/*/{src,data}/*.c')
    ap.add_argument('-j', '--jobs', type=int, default=os.cpu_count() or 4)
    sub = ap.add_subparsers(dest='cmd', required=True)

    p = sub.add_parser('views', help='prefix and partial views of another layout')
    p.add_argument('names', nargs='*', help='only pairs involving these')
    p.add_argument('--min', type=int, default=4, help='matching fields needed (default 4)')
    p.add_argument('--data', action='store_true',
                   help='also pair src/data/ layouts with each other')
    p.add_argument('-v', '--verbose', action='store_true',
                   help='also list the fields the other layout leaves as padding')
    p.set_defaults(func=cmd_views)

    p = sub.add_parser('diff', help='side-by-side field table of two or more layouts')
    p.add_argument('names', nargs='+', metavar='NAME')
    p.add_argument('--width', type=int, default=30, help='column width (default 30)')
    p.add_argument('--types', action='store_true', help='show each field\'s type')
    p.add_argument('--flagged', action='store_true', help='only the flagged rows')
    p.add_argument('--names-only', action='store_true', help='only name disagreements')
    p.add_argument('--types-only', action='store_true', help='only type disagreements')
    p.set_defaults(func=cmd_diff)

    p = sub.add_parser('names', help='same bytes, different names across a family')
    p.add_argument('names', nargs='*', help='only the families of these')
    p.add_argument('--min', type=int, default=4, help='matching fields needed (default 4)')
    p.add_argument('--data', action='store_true',
                   help='also pair src/data/ layouts with each other')
    p.set_defaults(func=cmd_names)

    p = sub.add_parser('show', help='one layout, flattened')
    p.add_argument('name')
    p.set_defaults(func=cmd_show)

    args = ap.parse_args()
    if args.cmd == 'diff' and len(args.names) < 2:
        ap.error('diff needs at least two layouts')
    L = Layouts(load_db(args))
    args.func(L, args)


if __name__ == '__main__':
    main()
