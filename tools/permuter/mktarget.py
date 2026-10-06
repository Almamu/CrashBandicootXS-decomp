#!/usr/bin/env python3
"""Write a decomp-permuter target.s from a function's NAKED asm.

    mktarget.py src/iwram/string_arm.c itoa_arm out/target.s

Takes the string literals of the `asm(...)` block between the function's
`NAKED ... <name>(` line and the next `// clang-format on`, and wraps them
in an ARM function. Assemble it with
`arm-none-eabi-as -mcpu=arm7tdmi -mthumb-interwork` (setup.sh does).
"""

import re
import sys


def main():
    src, fn, out = sys.argv[1:4]
    text = open(src).read()
    m = re.search(r"NAKED [^\n(]*\b%s\(" % re.escape(fn), text)
    if not m:
        sys.exit("%s: no NAKED %s" % (src, fn))
    j = text.index("asm(", m.end())
    k = text.index("// clang-format on", j)
    body = "".join(re.findall(r'"((?:[^"\\]|\\.)*)"', text[j:k]))
    body = body.replace("\\n", "\n").replace("\\t", "\t")
    with open(out, "w") as f:
        f.write(
            ".text\n.arm\n.align 2\n.global %s\n.type %s, %%function\n%s:\n%s\n"
            ".size %s, .-%s\n" % (fn, fn, fn, body, fn, fn)
        )


if __name__ == "__main__":
    main()
