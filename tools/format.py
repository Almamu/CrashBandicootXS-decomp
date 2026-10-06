#!/usr/bin/env python3
"""Format the C sources with clang-format (#572).

The style lives in .clang-format; CONTRIBUTING.md ("Code style") describes
it. The tree is being formatted a few directories at a time, so only the
paths in FORMATTED are kept formatted (and checked by CI). A later part of
#572 formats more directories and adds them to the list.

Before running clang-format, this wraps every multi-line `asm(...)`
statement, and every one-line statement longer than the column limit, in
`// clang-format off` / `// clang-format on`, so the asm strings and the
operand layout stay as written. clang-format itself never touches macro
bodies (SkipMacroDefinitionBody), comments (ReflowComments) or string
literals (BreakStringLiterals), and never reorders #includes (SortIncludes).

Usage:
  tools/format.py                format the FORMATTED paths in place
  tools/format.py PATH...        format these files/directories instead
                                 (to format a new directory before adding
                                 it to FORMATTED)
  tools/format.py --check [PATH...]
                                 change nothing; fail if any file isn't
                                 formatted (what CI runs)

clang-format comes from $CLANG_FORMAT, else `clang-format` on PATH. It must
be major version CLANG_FORMAT_MAJOR: other versions format some constructs
differently, and CI would disagree. `pip install clang-format==21.1.8` or
`nix shell nixpkgs#clang-tools` gives a matching one.
"""
import os
import re
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# The paths kept formatted. Widened by each part of #572.
FORMATTED = [
    "src/actor",
    "src/audio",
    "src/crates",
    "src/cutscene",
    "src/enemies",
    "src/frontend",
    "src/gfx",
    "src/hud",
    "src/iwram",
    "src/link",
    "src/menus",
    "src/objects",
    "src/pickups",
    "src/player",
    "src/save",
    "src/system",
    "src/text",
    "src/util",
    "src/vehicle",
]

CLANG_FORMAT_MAJOR = 21
COLUMN_LIMIT = 100

OFF = "// clang-format off"
ON = "// clang-format on"
ASM_START = re.compile(r"^(\s*)(?:__asm__|asm)\b\s*(?:volatile\b|__volatile__\b)?\s*\(")


def clang_format():
    exe = os.environ.get("CLANG_FORMAT") or shutil.which("clang-format")
    if not exe:
        sys.exit("format.py: clang-format not found (set CLANG_FORMAT, or see CONTRIBUTING.md)")
    out = subprocess.run([exe, "--version"], capture_output=True, text=True, check=True).stdout
    m = re.search(r"clang-format version (\d+)\.", out)
    if not m or int(m.group(1)) != CLANG_FORMAT_MAJOR:
        sys.exit(f"format.py: {exe} is {out.strip()!r}; clang-format {CLANG_FORMAT_MAJOR} is needed")
    return exe


def c_files(paths):
    files = []
    for p in paths:
        full = os.path.join(ROOT, p)
        if os.path.isfile(full):
            files.append(p)
            continue
        if not os.path.isdir(full):
            sys.exit(f"format.py: no such file or directory: {p}")
        for dirpath, dirnames, names in os.walk(full):
            dirnames.sort()
            for n in sorted(names):
                if n.endswith((".c", ".h")):
                    files.append(os.path.relpath(os.path.join(dirpath, n), ROOT))
    return files


def statement_end(lines, i, col):
    """Line index of the `;` ending the asm statement whose `(` is at
    lines[i][col], skipping string/char literals and comments."""
    depth = 0
    in_block = False
    j, k = i, col
    while j < len(lines):
        line = lines[j]
        while k < len(line):
            ch = line[k]
            if in_block:
                if line.startswith("*/", k):
                    in_block = False
                    k += 1
            elif line.startswith("/*", k):
                in_block = True
                k += 1
            elif line.startswith("//", k):
                break
            elif ch in "\"'":
                k += 1
                while k < len(line) and line[k] != ch:
                    k += 2 if line[k] == "\\" else 1
            elif ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
            elif ch == ";" and depth == 0:
                return j
            k += 1
        j, k = j + 1, 0
    return None


def protect_asm(text):
    """Wrap the asm statements clang-format would reflow in off/on markers."""
    lines = text.split("\n")
    out = []
    off = False
    i = 0
    while i < len(lines):
        line = lines[i]
        stripped = line.strip()
        if stripped == OFF:
            off = True
        elif stripped == ON:
            off = False
        m = ASM_START.match(line)
        # Skip asm inside macro definitions (clang-format leaves macro
        # bodies alone) and inside existing off/on regions.
        if m and not off and not (i > 0 and lines[i - 1].endswith("\\")):
            end = statement_end(lines, i, m.end() - 1)
            if end is not None and (end > i or len(line) > COLUMN_LIMIT):
                indent = m.group(1)
                out.append(indent + OFF)
                out.extend(lines[i:end + 1])
                out.append(indent + ON)
                i = end + 1
                continue
        out.append(line)
        i += 1
    return "\n".join(out)


def main():
    args = sys.argv[1:]
    check = "--check" in args
    paths = [a for a in args if a != "--check"] or FORMATTED
    files = c_files(paths)
    exe = clang_format()
    if check:
        r = subprocess.run([exe, "--dry-run", "--Werror", "--style=file"] + files, cwd=ROOT)
        if r.returncode:
            print("format.py: run tools/format.py to format these files", file=sys.stderr)
        return r.returncode
    for f in files:
        full = os.path.join(ROOT, f)
        with open(full) as fp:
            text = fp.read()
        new = protect_asm(text)
        if new != text:
            with open(full, "w") as fp:
                fp.write(new)
    return subprocess.run([exe, "-i", "--style=file"] + files, cwd=ROOT).returncode


if __name__ == "__main__":
    sys.exit(main())
