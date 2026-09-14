#!/usr/bin/env python3
"""Applies expected/corrections.txt to an assembled target object in place.

See expected/corrections.txt and docs/decomp_dev.md for why this exists:
expected/code_3.s (and expected/legacy.s) are frozen, never-edited
historical disassembly, so any function boundary or name they never
anticipated (because later matching work discovered/renamed it) has to
be patched onto the assembled object instead - never onto the source.

Also used per-file (see tools/report_units.py) against small slices of
those frozen sources, one per matched src/*.c file - the full
corrections.txt is passed every time, and entries that don't apply to
a given slice (the renamed symbol isn't in it, or the split address
falls outside its range) are silently skipped rather than erroring, so
the same file doesn't need per-slice filtering maintained by hand.
"""
import re
import subprocess
import sys

OBJCOPY = "arm-none-eabi-objcopy"
NM = "arm-none-eabi-nm"
READELF = "arm-none-eabi-readelf"


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
    return lowest


def text_section_size(obj_path):
    out = subprocess.run([READELF, "-S", obj_path], capture_output=True, text=True, check=True).stdout
    for line in out.splitlines():
        m = re.search(r"\.text\s+PROGBITS\s+[0-9a-f]+\s+[0-9a-f]+\s+([0-9a-f]+)", line)
        if m:
            return int(m.group(1), 16)
    sys.exit(f"patch_expected_target: couldn't find .text section size in {obj_path}")


def existing_symbols(obj_path):
    out = subprocess.run([NM, obj_path], capture_output=True, text=True, check=True).stdout
    names = set()
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 3:
            names.add(parts[2])
        elif len(parts) == 2:
            names.add(parts[1])
    return names


def main():
    argv = sys.argv[1:]
    base_override = None
    for arg in list(argv):
        if arg.startswith("--base="):
            base_override = int(arg[len("--base="):], 16)
            argv.remove(arg)

    if len(argv) != 2:
        sys.exit(f"usage: {sys.argv[0]} [--base=0xADDR] <corrections.txt> <target.o>")
    corrections_path, obj_path = argv

    base = base_override if base_override is not None else text_base_address(obj_path)
    size = text_section_size(obj_path)
    symbols = existing_symbols(obj_path)

    objcopy_args = []
    with open(corrections_path) as f:
        for lineno, raw_line in enumerate(f, 1):
            line = raw_line.split("#", 1)[0].strip()
            if not line:
                continue
            parts = line.split()
            if parts[0] == "rename" and len(parts) == 3:
                _, old, new = parts
                if old not in symbols:
                    continue
                objcopy_args += ["--redefine-sym", f"{old}={new}"]
            elif parts[0] == "split" and len(parts) == 3:
                _, address, name = parts
                if base is None:
                    continue
                offset = int(address, 16) - base
                if not (0 <= offset < size):
                    continue
                objcopy_args += ["--add-symbol", f"{name}=.text:{offset:#x},function,global"]
            else:
                sys.exit(f"{corrections_path}:{lineno}: malformed line: {raw_line!r}")

    if objcopy_args:
        subprocess.run([OBJCOPY, *objcopy_args, obj_path], check=True)


if __name__ == "__main__":
    main()
