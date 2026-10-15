"""Struct layouts from `readelf --debug-dump=info` text (agbcp -ggdb: DWARF 2).

catalogue(paths) -> {struct name: [(owner, member, offset in owner,
offset in struct, size)]}: every scalar member, flattened through base
classes, struct members and arrays, with the struct (or base) that owns it.
"""
import re

DIE = re.compile(r'^ <(\d+)><([0-9a-f]+)>: Abbrev Number: \d+ \((DW_TAG_\w+)\)')
ATTR = re.compile(r'^    <[0-9a-f]+>\s+(DW_AT_\w+)\s*: (.*)$')


def parse(path):
    dies = {}; stack = []
    cur = None
    for line in open(path, errors='replace'):
        m = DIE.match(line)
        if m:
            depth, off, tag = int(m.group(1)), int(m.group(2), 16), m.group(3)
            cur = dict(tag=tag, off=off, kids=[])
            dies[off] = cur
            del stack[depth:]
            if stack: stack[-1]['kids'].append(cur)
            stack.append(cur)
            continue
        m = ATTR.match(line)
        if m and cur is not None:
            k, v = m.group(1), m.group(2).strip()
            if k == 'DW_AT_name':
                cur['name'] = v.split(': ')[-1].strip()
            elif k == 'DW_AT_type':
                cur['type'] = int(v.strip('<>'), 16)
            elif k == 'DW_AT_byte_size':
                cur['size'] = int(v, 0)
            elif k == 'DW_AT_upper_bound':
                cur['upper'] = int(v, 0)
            elif k == 'DW_AT_data_member_location':
                mm = re.search(r'DW_OP_plus_uconst: (\d+)', v)
                cur['loc'] = int(mm.group(1)) if mm else (int(v, 0) if v.isdigit() else None)
            elif k == 'DW_AT_bit_size':
                cur['bits'] = int(v, 0)
            elif k == 'DW_AT_declaration':
                cur['decl'] = True
    return dies


def strip(dies, t, depth=0):
    while t in dies and depth < 30:
        d = dies[t]
        if d['tag'] in ('DW_TAG_typedef', 'DW_TAG_const_type', 'DW_TAG_volatile_type') and 'type' in d:
            t = d['type']; depth += 1; continue
        return d
    return None


def size_of(dies, d):
    if d is None: return 4  # void * etc.
    if d['tag'] in ('DW_TAG_pointer_type', 'DW_TAG_reference_type'): return 4
    if 'size' in d: return d['size']
    if d['tag'] == 'DW_TAG_array_type':
        e = strip(dies, d.get('type'))
        n = 1
        for k in d['kids']:
            if k['tag'] == 'DW_TAG_subrange_type': n *= k.get('upper', 0) + 1
        return n * size_of(dies, e)
    return 0


def leaves(dies, d, name):
    out = []

    def walk(d, base, owner, obase, prefix, depth):
        if depth > 8: return
        own = d.get('name') or owner
        ob = base if d.get('name') else obase
        for m in d['kids']:
            if m['tag'] not in ('DW_TAG_member', 'DW_TAG_inheritance') or m.get('loc') is None: continue
            if 'bits' in m: continue
            t = strip(dies, m.get('type'))
            off = base + m['loc']
            mname = prefix + (m.get('name') or '')
            if t and t['tag'] in ('DW_TAG_structure_type', 'DW_TAG_union_type', 'DW_TAG_class_type'):
                if t.get('name'):
                    walk(t, off, own, ob, '', depth + 1)
                else:
                    walk(t, off, own, ob, mname + '.' if m.get('name') else prefix, depth + 1)
            elif t and t['tag'] == 'DW_TAG_array_type':
                e = strip(dies, t.get('type'))
                es = size_of(dies, e); n = size_of(dies, t) // es if es else 0
                if e and e['tag'] in ('DW_TAG_structure_type', 'DW_TAG_union_type', 'DW_TAG_class_type'):
                    for k in range(min(n, 16)):
                        walk(e, off + k * es, own, ob, '', depth + 1)
                else:
                    for k in range(min(n, 64)):
                        out.append((own, '%s[%d]' % (mname, k), off + k * es - ob, off + k * es, es))
            else:
                out.append((own, mname, off - ob, off, size_of(dies, t)))

    walk(d, 0, name, 0, '', 0)
    return out


def catalogue(paths):
    cat = {}
    for p in paths:
        dies = parse(p)
        for d in dies.values():
            if d['tag'] in ('DW_TAG_structure_type', 'DW_TAG_class_type', 'DW_TAG_union_type') \
                    and d.get('name') and not d.get('decl') and d['kids']:
                lv = leaves(dies, d, d['name'])
                if len(lv) > len(cat.get(d['name'], ())):
                    cat[d['name']] = lv
    return cat
