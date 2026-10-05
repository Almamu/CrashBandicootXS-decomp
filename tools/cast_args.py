#!/usr/bin/env python3
"""Cast the arguments a new header's prototypes now warn about (#574).

    tools/cast_args.py BUILD_LOG HEADER [HEADER...] [--only REGEX] [--dry-run]

After a header is applied with `tools/apply_headers.py --adopt-all`, a
call that passes a file-local view of an object (a `struct gobj *` where
the definition takes a `struct actor *`, ...) gets agbcc's "passing arg N
of `F' from incompatible pointer type" (or "discards qualifiers")
warning. For each such warning in BUILD_LOG whose function the headers
declare, this wraps argument N of the first call to F on that line in a
cast to the prototype's parameter type. A pointer cast doesn't change the
code, but check the objects anyway (docs/headers_plan.md, "Verification").

It only edits calls that open and close on the warning's line, and lists
the ones it skips. `--only` limits it to the functions matching REGEX.
"""

import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import extern_audit as ea  # noqa: E402
import apply_headers as ah  # noqa: E402

WARN = re.compile(r"^(\S+):(\d+): warning: passing arg (\d+) of `(\w+)' "
                  r"(?:from incompatible pointer type|discards qualifiers from pointer target type)")


def param_types(decl):
    m = re.match(r'^.*?\((.*)\)\s*;?\s*$', decl['text'].strip(), re.S)
    return [ea.strip_param_name(p.strip()).strip() for p in ea.split_commas(m.group(1))]


def find_call(line, fn):
    """(start of the arguments, index of the closing paren) of the first
    call to `fn` on `line`, or None."""
    m = re.search(r'\b%s\s*\(' % re.escape(fn), line)
    if not m:
        return None
    depth = 1
    for j in range(m.end(), len(line)):
        if line[j] == '(':
            depth += 1
        elif line[j] == ')':
            depth -= 1
            if depth == 0:
                return m.end(), j
    return None


def split_args(s):
    out, depth, cur = [], 0, ''
    for c in s:
        if c in '([':
            depth += 1
        elif c in ')]':
            depth -= 1
        if c == ',' and depth == 0:
            out.append(cur)
            cur = ''
        else:
            cur += c
    out.append(cur)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('log')
    ap.add_argument('headers', nargs='+')
    ap.add_argument('--only', metavar='REGEX')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()
    os.chdir(ea.ROOT)
    only = re.compile(args.only) if args.only else None
    decls = {}
    for h in args.headers:
        decls.update(ah.header_decls(h))

    edits = {}
    with open(args.log, encoding='utf-8', errors='replace') as f:
        for l in f:
            m = WARN.match(l)
            if not m:
                continue
            path, ln, n, fn = m.group(1), int(m.group(2)), int(m.group(3)), m.group(4)
            if fn in decls and not (only and not only.search(fn)):
                edits.setdefault(path, set()).add((ln, n, fn))

    for path, es in sorted(edits.items()):
        with open(path, encoding='utf-8') as f:
            lines = f.read().split('\n')
        for ln, n, fn in sorted(es, key=lambda e: (e[0], -e[1])):
            line = lines[ln - 1]
            r = find_call(line, fn)
            if not r:
                print('SKIP %s:%d: no single-line call to %s' % (path, ln, fn))
                continue
            a, b = r
            argv = split_args(line[a:b])
            types = param_types(decls[fn])
            if n > len(argv) or n > len(types):
                print('SKIP %s:%d: %s has no argument %d' % (path, ln, fn, n))
                continue
            t = types[n - 1]
            lead = argv[n - 1][:len(argv[n - 1]) - len(argv[n - 1].lstrip())]
            core = argv[n - 1].strip()
            if core.startswith('(%s)' % t):
                continue
            if re.search(r'[?:]|\s[-+*/&|<>=]\s', core) and not re.match(r'^\(.*\)$', core):
                core = '(%s)' % core
            argv[n - 1] = '%s(%s)%s' % (lead, t, core)
            lines[ln - 1] = line[:a] + ','.join(argv) + line[b:]
            print('%s:%d: %s argument %d -> (%s)' % (path, ln, fn, n, t))
        if not args.dry_run:
            with open(path, 'w', encoding='utf-8') as f:
                f.write('\n'.join(lines))


if __name__ == '__main__':
    main()
