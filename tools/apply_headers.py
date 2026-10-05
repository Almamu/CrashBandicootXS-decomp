#!/usr/bin/env python3
"""Replace local extern declarations with an #include of a header (#574).

    tools/apply_headers.py include/text.h               # every file that redeclares a symbol
    tools/apply_headers.py include/text.h src/a.c ...   # only these files
    tools/apply_headers.py include/text.h --dry-run     # report only

For each .c file it finds the file-scope declarations (`extern` variables
and functions, and plain prototypes) of symbols the header declares, and
compares each one with the header's declaration, ignoring parameter names,
`extern` and whitespace (tools/extern_audit.py's parser):

  * every declaration matches: they are deleted and `#include "<header>"`
    is added after the file's last top-of-file #include;
  * any declaration differs: the file is left alone and the differences
    are listed. Including the header would make them a "conflicting types"
    compile error. Either fix them by hand (and prove the bytes don't
    change), or keep the local declarations with a `codegen:` comment and
    don't include the header in that file (docs/headers_plan.md).
    `--adopt SYMBOL` (repeatable) treats SYMBOL's differing declarations
    as matching, so they are deleted and the file takes the header's type:
    use it once you have checked that the change is byte-neutral.

Files that define a symbol the header declares are skipped unless named on
the command line (the defining file should include its own header, but its
definition may need adjusting first).

The script never touches a declaration that shares its statement with
another declarator (`extern u8 a, b;`) or a line with other code. It
prints the trailing comments it drops and the comment blocks a deletion
leaves orphaned, so they can be moved to the header or deleted by hand.

It doesn't build anything: run the clean `make compare` and the objdiff
report afterwards, as for any change.
"""

import argparse
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import extern_audit as ea  # noqa: E402

ROOT = ea.ROOT


def header_decls(header):
    fs = ea.scan_c(header, is_header=True)
    out = {}
    for d in fs.decls:
        out[d['asm'] or d['name']] = d
    return out


def same_type(a, b):
    return ea.type_key(a) == ea.type_key(b)


def comment_free(line):
    return re.sub(r'/\*.*?\*/|//.*$', '', line).strip()


def find_include_slot(lines):
    """Index after the last #include of the leading include block."""
    last = None
    for i, l in enumerate(lines):
        s = l.strip()
        if s.startswith('#include'):
            last = i
        elif s and not s.startswith(('//', '/*', '*', '#')) and last is not None:
            break
        elif last is None and s and not s.startswith(('//', '/*', '*', '#')):
            break
    return 0 if last is None else last + 1


def apply(path, hdr_path, hdecls, adopt, dry_run, allow_definer):
    rel_inc = os.path.relpath(hdr_path, 'include') if hdr_path.startswith('include/') else os.path.basename(hdr_path)
    fs = ea.scan_c(path)
    defined = {n for n, _ in fs.defs} | fs.asm_defs
    hits = [d for d in fs.decls if (d['asm'] or d['name']) in hdecls]
    own = [n for n in defined if n in hdecls]
    if not hits and not allow_definer:
        return None
    if own and not allow_definer:
        return 'skip', ['defines %s (name the file explicitly to convert it)' % ', '.join(sorted(own))]

    msgs = []
    conflicts = []
    removable = []
    for d in hits:
        sym = d['asm'] or d['name']
        h = hdecls[sym]
        if d['asm'] != h['asm']:
            conflicts.append('%s: asm label %r vs header %r' % (sym, d['asm'], h['asm']))
        elif not same_type(d, h) and sym not in adopt:
            conflicts.append('%s:%d: %s\n        header: %s' % (path, d['line'], d['text'], h['text']))
        elif d.get('ndecl', 1) != 1:
            conflicts.append('%s:%d: %s is one of several declarators; edit by hand' % (path, d['line'], sym))
        else:
            removable.append(d)
    if conflicts:
        return 'conflict', conflicts

    with open(os.path.join(ROOT, path), encoding='utf-8') as f:
        lines = f.read().split('\n')
    drop = set()
    for d in removable:
        a, b = d['line'] - 1, d['end_line'] - 1
        span = ' '.join(comment_free(l) for l in lines[a:b + 1])
        body = re.sub(r'\s+', ' ', span).strip()
        # the statement must be the only code on its lines
        if body.count(';') != 1 or not body.endswith(';'):
            msgs.append('kept %s at line %d: shares a line with other code' % (d['name'], d['line']))
            continue
        for i in range(a, b + 1):
            m = re.search(r'(/\*.*?\*/|//.*)$', lines[i])
            if m and comment_free(lines[i]):
                msgs.append('dropped trailing comment (%s): %s' % (d['name'], m.group(1).strip()))
            drop.add(i)
        # a comment block that ends right above the declaration
        j = a - 1
        while j - 1 in drop:
            j -= 1
        if j >= 0 and lines[j].rstrip().endswith('*/') and (j + 1 not in drop or j + 1 == a):
            if not any(k in drop for k in range(j + 1, a)) or j + 1 == a:
                msgs.append('comment above %s (line %d) may be orphaned' % (d['name'], j + 1))
    # a deletion that leaves two blank lines touching drops the second one
    # (blank runs elsewhere in the file are left alone)
    for i in sorted(drop):
        if i + 1 in drop or i + 1 >= len(lines):
            continue
        j = i
        while j - 1 in drop:
            j -= 1
        if j > 0 and lines[j - 1].strip() == '' and lines[i + 1].strip() == '':
            drop.add(i + 1)
    out = [l for i, l in enumerate(lines) if i not in drop]
    inc_line = '#include "%s"' % rel_inc
    if not any(l.strip() in (inc_line, '#include <%s>' % rel_inc) for l in out):
        k = find_include_slot(out)
        out.insert(k, inc_line)
    if not dry_run:
        with open(os.path.join(ROOT, path), 'w', encoding='utf-8') as f:
            f.write('\n'.join(out))
    msgs.insert(0, 'removed %d declaration(s): %s' % (len(removable), ', '.join(sorted({d['name'] for d in removable}))))
    return 'ok', msgs


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('header')
    ap.add_argument('files', nargs='*')
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--adopt', action='append', default=[], metavar='SYMBOL')
    args = ap.parse_args()
    os.chdir(ROOT)
    hdecls = header_decls(args.header)
    if not hdecls:
        sys.exit('%s declares nothing' % args.header)
    explicit = bool(args.files)
    files = args.files or sorted(glob.glob('src/**/*.c', recursive=True) + glob.glob('lib/**/*.c', recursive=True))
    status = {}
    for p in files:
        r = apply(p, args.header, hdecls, set(args.adopt), args.dry_run, explicit)
        if r is None:
            continue
        st, msgs = r
        status.setdefault(st, []).append(p)
        print('%-8s %s' % (st.upper(), p))
        for m in msgs:
            print('    ' + m)
    print()
    print(', '.join('%s %d' % (k, len(v)) for k, v in sorted(status.items())) or 'nothing to do')
    if args.dry_run:
        print('(dry run: no files written)')


if __name__ == '__main__':
    main()
