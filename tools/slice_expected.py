#!/usr/bin/env python3
"""Extracts a contiguous, address-bounded text slice out of one of the
frozen expected/*.s sources (never edits the source itself - see
docs/decomp_dev.md and tools/report_units.py, which drives this to build
one objdiff unit per matched src/*.c file so decomp.dev can show
per-category progress).

Slicing works at the text level (find the label line for the start
address, find the label line for the end address, take everything in
between) rather than on assembled bytes, so relocations/literal pools
stay intact - the same technique expected/legacy.s itself was carved
out with.
"""
import re
import sys

HEADER = '.include "asm/macros.inc"\n\n.syntax unified\n.arm\n\n'


def find_label_line(lines, address):
    """Line index of the *start* of the function at `address`: the label
    line itself (`<name>: @ 0x<address>`), backed up one line further if
    that's a `thumb_func_start`/`arm_func_start <name>` directive - those
    set .global/.thumb_func/.type, and cutting a slice right at the bare
    label (leaving the directive out) silently makes the first function
    in every slice local instead of global, which objdiff then drops
    from its function list entirely."""
    pattern = re.compile(rf"^(\S+):\s*@\s*0x0*{address:X}\b", re.IGNORECASE)
    for i, line in enumerate(lines):
        m = pattern.match(line)
        if not m:
            continue
        name = m.group(1)
        if i > 0 and re.search(rf"\b(?:thumb|arm)_func_start\s+{re.escape(name)}\b", lines[i - 1]):
            return i - 1
        return i
    return None


def main():
    if len(sys.argv) not in (3, 4):
        sys.exit(f"usage: {sys.argv[0]} <source.s> <start_addr_hex> [<end_addr_hex>]")
    source_path = sys.argv[1]
    start_addr = int(sys.argv[2], 16)
    end_addr = int(sys.argv[3], 16) if len(sys.argv) == 4 else None

    with open(source_path) as f:
        lines = f.readlines()

    start_line = find_label_line(lines, start_addr)
    if start_line is None:
        sys.exit(f"slice_expected: no label for address {start_addr:#x} in {source_path}")

    if end_addr is not None:
        end_line = find_label_line(lines, end_addr)
        if end_line is None:
            # Not covered by this source (crosses into the next frozen
            # file, or into still-raw asm) - take the rest of the file.
            end_line = len(lines)
    else:
        end_line = len(lines)

    sys.stdout.write(HEADER)
    sys.stdout.writelines(lines[start_line:end_line])


if __name__ == "__main__":
    main()
