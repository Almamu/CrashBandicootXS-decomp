#!/usr/bin/env python3
"""XS's game objects again, with debug info, for struct_diff.py.

    python3 tools/nt_compare/debug_build.py [out dir]   (default build/nt_compare)

Takes every C++ compile command of the XS build (`make -n -B
crashbandicootxs.elf`) and runs it again twice, into <out>/stabs/ and
<out>/dwarf/ (never build/crashbandicootxs/):

- with -gstabs, for the line table (objdump -G);
- with -ggdb, DWARF 2 with every class's layout (g++ 2.9 leaves most
  classes out of -gstabs). agbcp doesn't end .debug_abbrev with the 0 that
  binutils' readelf needs, so one is appended before assembling, and the
  DWARF is dumped to <obj>.dw with `readelf --debug-dump=info`.

-Werror is dropped (-g adds warnings of its own) and the two ARM objects
are left out (agbcp_arm_patched doesn't take -g with -fomit-frame-pointer).
The data tables (src/*/data/) have no code and are skipped. XS is built
first (`make crashbandicootxs.gba`): the commands use its generated
headers (build/include/) and its objects are what the code is compared
with. An
object that doesn't compile with debug info is reported and skipped.
Debug info doesn't change the code: each object's .text is compared with
the XS build's, and a difference is an error.
"""
import os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

OUT = Path(sys.argv[1] if len(sys.argv) > 1 else 'build/nt_compare')
XS_BUILD = 'build/crashbandicootxs/'
ZERO_PAD = '\t.text\n\t.align\t2, 0\n'
ABBREV_END = '\t.section\t.debug_abbrev\n\t.byte\t0\n'


def commands():
    out = subprocess.run(['make', '-n', '-B', 'crashbandicootxs.elf'], capture_output=True, text=True).stdout
    for line in out.splitlines():
        if '| tools/agbcc/bin/' not in line or ' src/' not in line or '/data/' in line:
            continue
        if 'agbcp_arm_patched' in line:
            continue
        m = re.search(r' -o (%s(\S+)\.s)\s*$' % re.escape(XS_BUILD), line)
        if m:
            yield m.group(2), re.sub(r'-MMD -MP -MF \S+ -MT \S+ ', '', line.replace(' -Werror', ''))


def text(obj):
    tmp = str(obj) + '.text'
    subprocess.run(['arm-none-eabi-objcopy', '-O', 'binary', '-j', '.text', str(obj), tmp], check=True)
    data = Path(tmp).read_bytes()
    os.unlink(tmp)
    return data


def build(rel, cmd):
    errors, skipped = [], []
    for kind, flag in (('stabs', '-gstabs'), ('dwarf', '-ggdb')):
        base = OUT / kind / rel
        base.parent.mkdir(parents=True, exist_ok=True)
        s, o = str(base) + '.s', str(base) + '.o'
        c = cmd.replace(' -o %s%s.s' % (XS_BUILD, rel), ' %s -o %s' % (flag, s))
        r = subprocess.run(c, shell=True, capture_output=True, text=True)
        if r.returncode:
            skipped.append('skipped %s (%s): %s' % (rel, kind, r.stderr.strip().splitlines()[-1]))
            continue
        with open(s, 'a') as f:
            f.write(ZERO_PAD + (ABBREV_END if kind == 'dwarf' else ''))
        subprocess.run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-mthumb-interwork', '-o', o, s], check=True)
        if text(o) != text(XS_BUILD + rel + '.o'):
            errors.append('%s %s: the code differs from the XS build' % (rel, kind))
        if kind == 'dwarf':
            with open(str(base) + '.dw', 'w') as f:
                subprocess.run(['arm-none-eabi-readelf', '--debug-dump=info', o], stdout=f,
                               stderr=subprocess.DEVNULL)
    return errors, skipped


def main():
    # the commands need the XS build's generated headers, and the objects are
    # compared with its (a no-op when it is up to date)
    if subprocess.run(['make', '-j%d' % os.cpu_count(), 'crashbandicootxs.gba'],
                      stdout=subprocess.DEVNULL).returncode:
        sys.exit('the XS build failed')
    cmds = list(commands())
    with ThreadPoolExecutor(os.cpu_count()) as ex:
        results = list(ex.map(lambda c: build(*c), cmds))
    errors = [e for es, _ in results for e in es]
    for e in [s for _, ss in results for s in ss] + errors:
        print(e, file=sys.stderr)
    print('%d objects with debug info in %s/{stabs,dwarf}' % (len(cmds), OUT), file=sys.stderr)
    sys.exit(1 if errors else 0)


if __name__ == '__main__':
    main()
