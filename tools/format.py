#!/usr/bin/env python3
"""Format the C sources with clang-format (#572).

The style lives in .clang-format; CONTRIBUTING.md ("Code style") describes
it. The paths in FORMATTED (the whole tree) are kept formatted and checked
by CI, except the UNFORMATTED data directories: their tool-emitted
ROM-order tables are laid out one entry per line on purpose, which
clang-format would bin-pack into columns.

Before running clang-format, this wraps every multi-line `asm(...)`
statement, and every one-line statement longer than the column limit, in
`// clang-format off` / `// clang-format on`, so the asm strings and the
operand layout stay as written. clang-format itself never touches macro
bodies (SkipMacroDefinitionBody), comments (ReflowComments) or string
literals (BreakStringLiterals), and never reorders #includes (SortIncludes).

Usage:
  tools/format.py                format the FORMATTED paths in place
  tools/format.py PATH...        format just these files/directories
                                 (UNFORMATTED ones are still skipped
                                 inside a directory)
  tools/format.py --check [PATH...]
                                 change nothing; fail if any file isn't
                                 formatted or has a misaligned block
                                 comment, see below (what CI runs)

--check also flags multi-line /* */ comments whose continuation lines
don't line up with the opening line: a ` * ` line must have its `*` one
column right of the `/*`, any other line must start three columns right
of it. clang-format moves only the first line of a comment it re-indents
(ReflowComments is off), so a comment whose first line moved (a trailing
comment realigned, a case block indented) leaves the rest behind; fix
those by hand.

clang-format comes from $CLANG_FORMAT, else `clang-format` on PATH. It must
be major version CLANG_FORMAT_MAJOR: other versions format some constructs
differently, and CI would disagree. `pip install clang-format==21.1.8` or
`nix shell nixpkgs#clang-tools` gives a matching one.
"""
import fnmatch
import os
import re
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# The paths kept formatted: all the C (#572).
FORMATTED = [
    "include",
    "lib",
    "src",
]

# ...except the data: tool-emitted ROM-order tables whose one-entry-per-line
# layout is their documentation. clang-format would pack them into columns
# (fnmatch patterns, skipped while walking a directory).
UNFORMATTED = [
    "src/data",
    "lib/*/data",
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
            rel = os.path.relpath(dirpath, ROOT)
            dirnames[:] = sorted(
                d for d in dirnames
                if not any(fnmatch.fnmatch(os.path.join(rel, d), pat) for pat in UNFORMATTED))
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


def block_comments(text):
    """Yield (line, column) of the opening of every /* */ comment that spans
    several lines, and the index of its last line. Skips string and
    character literals and // comments."""
    i, n = 0, len(text)
    line, col = 0, 0
    while i < n:
        ch = text[i]
        if ch == "\n":
            line, col = line + 1, 0
            i += 1
        elif ch in "\"'":
            j = i + 1
            while j < n and text[j] not in (ch, "\n"):
                j += 2 if text[j] == "\\" else 1
            col += j + 1 - i
            i = j + 1
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j == -1 else j
            col += j - i
            i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j == -1 else j + 2
            body = text[i:j]
            newlines = body.count("\n")
            if newlines:
                yield line, col, line + newlines
                line += newlines
                col = len(body) - body.rfind("\n") - 1
            else:
                col += len(body)
            i = j
        else:
            col += 1
            i += 1


def misaligned_comments(path):
    """The continuation lines of path's block comments that don't line up
    with their opening `/*`."""
    with open(os.path.join(ROOT, path)) as fp:
        text = fp.read()
    lines = text.split("\n")
    bad = []
    for start, col, end in block_comments(text):
        for k in range(start + 1, end + 1):
            body = lines[k].lstrip()
            if not body:
                continue
            want = col + 1 if body.startswith("*") else col + 3
            if len(lines[k]) - len(body) != want:
                bad.append((k + 1, start + 1, want))
    return bad


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
        misaligned = 0
        for f in files:
            for line, start, want in misaligned_comments(f):
                print(f"{f}:{line}: block comment line not lined up with the /* on line "
                      f"{start} (should start at column {want + 1})", file=sys.stderr)
                misaligned += 1
        if misaligned:
            print("format.py: re-indent these comment lines by hand", file=sys.stderr)
        return 1 if r.returncode or misaligned else 0
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
