#!/usr/bin/env python3
"""Applies expected/corrections.txt to an assembled target object in place.

See expected/corrections.txt and docs/decomp_dev.md for why this exists:
expected/code_3.s is a frozen, never-edited historical disassembly, so
any function boundary or name it never anticipated (because later
matching work discovered/renamed it) has to be patched onto the
assembled object instead - never onto the source.
"""
import re
import subprocess
import sys

OBJCOPY = "arm-none-eabi-objcopy"
NM = "arm-none-eabi-nm"


def text_base_address(obj_path):
    """Lowest-address global function symbol's ROM address, i.e. what
    file offset 0 in .text corresponds to. Derived from the symbol's own
    name (this project names every not-yet-renamed function sub_<hex
    address>) rather than hardcoded, so this keeps working if
    expected/code_3.s is ever regenerated from a different commit."""
    out = subprocess.run([NM, obj_path], capture_output=True, text=True, check=True).stdout
    lowest = None
    for line in out.splitlines():
        parts = line.split()
        if len(parts) != 3 or parts[1] not in ("T", "t"):
            continue
        offset = int(parts[0], 16)
        m = re.fullmatch(r"sub_([0-9A-Fa-f]{7,8})", parts[2])
        if not m:
            continue
        address = int(m.group(1), 16)
        base = address - offset
        if lowest is None or base < lowest:
            lowest = base
    if lowest is None:
        sys.exit("patch_expected_target: couldn't find any sub_XXXXXXXX symbol to anchor addresses to")
    return lowest


def main():
    if len(sys.argv) != 3:
        sys.exit(f"usage: {sys.argv[0]} <corrections.txt> <target.o>")
    corrections_path, obj_path = sys.argv[1], sys.argv[2]

    base = text_base_address(obj_path)
    objcopy_args = []
    with open(corrections_path) as f:
        for lineno, raw_line in enumerate(f, 1):
            line = raw_line.split("#", 1)[0].strip()
            if not line:
                continue
            parts = line.split()
            if parts[0] == "rename" and len(parts) == 3:
                _, old, new = parts
                objcopy_args += ["--redefine-sym", f"{old}={new}"]
            elif parts[0] == "split" and len(parts) == 3:
                _, address, name = parts
                offset = int(address, 16) - base
                objcopy_args += ["--add-symbol", f"{name}=.text:{offset:#x},function,global"]
            else:
                sys.exit(f"{corrections_path}:{lineno}: malformed line: {raw_line!r}")

    if objcopy_args:
        subprocess.run([OBJCOPY, *objcopy_args, obj_path], check=True)


if __name__ == "__main__":
    main()
