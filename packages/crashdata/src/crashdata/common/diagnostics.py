"""The data tools' errors and warnings, for people and for editors (#1167, docs/editors.md).

A Diagnostic is one problem with the data: the file, the place in it (a
JSON pointer, RFC 6901, such as `/params/3/range_x`, and the line and
column where they are known), its severity ("error" or "warning"), a code
naming the check, and the message. As text it reads as the tools have
always printed it, `FILE: MESSAGE`; with `--json` a command prints them
as one JSON document instead (report()).

The model's checks raise DataError, a SystemExit, so a command that
doesn't catch it stops with the text on stderr and exit status 1, as
with `sys.exit(message)`. The older checks still exit with a message;
`from_exit` turns such a message into a Diagnostic, taking the file from
its `FILE: ` prefix.

    collect()            the warnings the checks give (warn) go into a
                         list instead of to stderr, for --json
    json_cli(tool, main) runs a command's main() with `--json`: its
                         diagnostics, and what it printed, as one report
"""
import contextlib
import json
import os
import re
import sys

ERROR, WARNING = "error", "warning"
# The --json report's format: bumped when a field changes meaning.
REPORT_VERSION = 1


class Diagnostic:
    """One problem with the data."""

    def __init__(self, message, file=None, pointer=None, severity=ERROR, code="error", line=None, col=None):
        self.message = message
        self.file = str(file) if file is not None else None
        self.pointer = pointer
        self.severity = severity
        self.code = code
        self.line = line
        self.col = col

    def text(self):
        """As the tools print it: `FILE: POINTER: MESSAGE`, `FILE: warning: ...`."""
        parts = [p for p in (self.file, self.pointer or None) if p]
        if self.severity == WARNING:
            parts.append("warning")
        return ": ".join(parts + [self.message])

    def as_dict(self):
        return {"file": self.file, "pointer": self.pointer, "line": self.line, "col": self.col,
                "severity": self.severity, "code": self.code, "message": self.message}

    def __repr__(self):
        return "Diagnostic(%r)" % self.text()


class DataError(SystemExit):
    """The data is wrong: one or more Diagnostics (errors). As a
    SystemExit its code is their text, so an uncaught one prints it and
    exits 1."""

    def __init__(self, diagnostics):
        if isinstance(diagnostics, Diagnostic):
            diagnostics = [diagnostics]
        self.diagnostics = list(diagnostics)
        super().__init__("\n".join(d.text() for d in self.diagnostics))


def error(message, file=None, pointer=None, code="error", line=None, col=None):
    """Raise a DataError of one error."""
    raise DataError(Diagnostic(message, file, pointer, ERROR, code, line, col))


# ---------------------------------------------------------------------------
# Warnings: to stderr, or into the collecting list (--json)

_collecting = []


@contextlib.contextmanager
def collect():
    """Within it, warn() appends to the list it gives instead of printing."""
    out = []
    _collecting.append(out)
    try:
        yield out
    finally:
        _collecting.pop()


def warn(message, file=None, pointer=None, code="warning"):
    """A warning: the build goes on. Printed to stderr as `FILE: warning:
    MESSAGE` (as the tools always have), or collected."""
    d = Diagnostic(message, file, pointer, WARNING, code)
    if _collecting:
        _collecting[-1].append(d)
    else:
        print(d.text(), file=sys.stderr)
    return d


# ---------------------------------------------------------------------------
# An exit message as a Diagnostic

# `path/to/file.ext: rest`: a message that names its file first.
_FILE_PREFIX = re.compile(r"^((?:[\w.+-]+/)*[\w.+-]+\.(?:json|po|pot|png|bmp|pal|xm|wav|txt|py|h|inc|bin|mk)"
                          r"|(?:games|mods|build|engine|lib|platform)/[\w./+-]*):\s*(.*)$", re.S)
_TOOL_PREFIX = re.compile(r"^[a-z_]+(?:\.py)?:\s+(.*)$", re.S)
_WARNING = re.compile(r"^warning:\s*(.*)$", re.S)


def from_text(text, severity=ERROR, code="error"):
    """A message `FILE: MESSAGE` (or just MESSAGE) as a Diagnostic."""
    text = text.rstrip("\n")
    # a tool's own prefix before the file (`text.py: games/xs/text/fr.po: ...`)
    t = _TOOL_PREFIX.match(text)
    if t and _FILE_PREFIX.match(t.group(1)):
        text = t.group(1)
    m = _FILE_PREFIX.match(text)
    file, message = (m.group(1), m.group(2)) if m else (None, text)
    w = _WARNING.match(message)
    if w:
        severity, code, message = WARNING, "warning", w.group(1)
    # an optional JSON pointer after the file, as Diagnostic.text() writes it
    pointer = None
    p = re.match(r"^(/[^\s:]*):\s*(.*)$", message, re.S)
    if file and p:
        pointer, message = p.group(1), p.group(2)
    return Diagnostic(message, file, pointer, severity, code)


def from_exit(e):
    """The Diagnostics of a SystemExit: a DataError's own, an exit
    message's lines (one per line that names a file, the lines after it
    its message's), or none for a plain exit status."""
    if isinstance(e, DataError):
        return list(e.diagnostics)
    if e.code is None or isinstance(e.code, int):
        return [] if not e.code else [Diagnostic("exit status %d" % e.code, code="exit")]
    out = []
    for line in str(e.code).split("\n"):
        d = from_text(line)
        if out and d.file is None:
            out[-1].message += "\n" + line
        else:
            out.append(d)
    return out


# ---------------------------------------------------------------------------
# JSON pointers and where they are in the text

def pointer(path):
    """A path (keys and indices) as a JSON pointer: ("params", 3) -> "/params/3"."""
    return "".join("/" + str(p).replace("~", "~0").replace("/", "~1") for p in path)


def json_positions(text):
    """{JSON pointer: (line, col)} of every value in the JSON text (1-based,
    the value's first character). Only for well-formed JSON: json.loads
    gives a malformed text's position itself."""
    out = {}
    i, n = 0, len(text)
    line_starts = [0] + [m.end() for m in re.finditer("\n", text)]

    def where(k):
        lo, hi = 0, len(line_starts) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if line_starts[mid] <= k:
                lo = mid
            else:
                hi = mid - 1
        return lo + 1, k - line_starts[lo] + 1

    ws = re.compile(r"[ \t\r\n]*")
    string = re.compile(r'"(?:[^"\\]|\\.)*"', re.S)
    scalar = re.compile(r"-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][-+]?\d+)?|true|false|null")

    def value(i, path):
        i = ws.match(text, i).end()
        out[pointer(path)] = where(i)
        c = text[i]
        if c == "{":
            i = ws.match(text, i + 1).end()
            if text[i] == "}":
                return i + 1
            while True:
                m = string.match(text, i)
                key = json.loads(m.group(0))
                i = ws.match(text, m.end()).end() + 1  # the ':'
                i = ws.match(text, value(i, path + (key,))).end()
                if text[i] == "}":
                    return i + 1
                i = ws.match(text, i + 1).end()  # the ','
        if c == "[":
            i = ws.match(text, i + 1).end()
            if text[i] == "]":
                return i + 1
            k = 0
            while True:
                i = ws.match(text, value(i, path + (k,))).end()
                k += 1
                if text[i] == "]":
                    return i + 1
                i += 1
        m = string.match(text, i) if c == '"' else scalar.match(text, i)
        return m.end()

    if ws.match(text, 0).end() < n:
        value(0, ())
    return out


def locate(diagnostics, texts):
    """Fill in each diagnostic's line and column from its pointer, the
    files' texts given by {file: text}."""
    cache = {}
    for d in diagnostics:
        if d.line is not None or d.pointer is None or d.file not in texts:
            continue
        if d.file not in cache:
            try:
                cache[d.file] = json_positions(texts[d.file])
            except (IndexError, AttributeError, ValueError):
                cache[d.file] = {}
        p = d.pointer
        pos = cache[d.file]
        while p and p not in pos:  # a missing key: its parent's position
            p = p.rsplit("/", 1)[0]
        if p in pos:
            d.line, d.col = pos[p]
    return diagnostics


def read_json(path):
    """A JSON file, or a DataError at its syntax error's line and column."""
    with open(path, encoding="utf-8") as f:
        text = f.read()
    try:
        return json.loads(text)
    except json.JSONDecodeError as e:
        error("not JSON: %s (line %d, column %d)" % (e.msg, e.lineno, e.colno), path, code="json-syntax",
              line=e.lineno, col=e.colno)


# ---------------------------------------------------------------------------
# The --json report

def report(tool, diagnostics, output="", ok=None):
    """The --json report: the tool, whether it succeeded, its diagnostics
    and what it printed."""
    if ok is None:
        ok = not any(d.severity == ERROR for d in diagnostics)
    return {"version": REPORT_VERSION, "tool": tool, "ok": ok,
            "diagnostics": [d.as_dict() for d in diagnostics], "output": output}


def relativize(diagnostics, base="."):
    """Each diagnostic's file relative to base (the tree's root) when it is
    under it, so one file has one name."""
    base = os.path.realpath(base)
    for d in diagnostics:
        if d.file and os.path.isabs(d.file):
            real = os.path.realpath(d.file)
            if real.startswith(base + os.sep):
                d.file = os.path.relpath(real, base)
    return diagnostics


def file_texts(diagnostics):
    texts = {}
    for d in diagnostics:
        if d.file and d.file not in texts and d.file.endswith(".json"):
            try:
                with open(d.file, encoding="utf-8") as f:
                    texts[d.file] = f.read()
            except OSError:
                pass
    return texts


class _Captured:
    text = ""


@contextlib.contextmanager
def capture_fd_stdout():
    """Within it, what is printed on stdout (by Python, and by the commands
    it runs, at the file descriptor, in the order it was printed) goes into
    the `.text` of what it gives instead."""
    import tempfile
    got = _Captured()
    sys.stdout.flush()
    saved = os.dup(1)
    with tempfile.TemporaryFile(mode="w+b") as tmp:
        os.dup2(tmp.fileno(), 1)
        try:
            yield got
        finally:
            sys.stdout.flush()
            os.dup2(saved, 1)
            os.close(saved)
            tmp.seek(0)
            got.text = tmp.read().decode("utf-8", "replace")


def json_cli(tool, main, argv=None):
    """Run a command's main(). With `--json` among the arguments (argv,
    sys.argv by default; it is taken out), print one report on stdout
    instead of the text: the command's diagnostics (its errors and
    warnings) and what it printed. Exit status 1 when there is an error,
    as without --json. Without --json, main() runs as it always has."""
    args = sys.argv if argv is None else argv
    if "--json" not in args[1:]:
        return main()
    args.remove("--json")
    status = 0
    with collect() as diags, capture_fd_stdout() as printed:
        try:
            r = main()
            status = r if isinstance(r, int) else 0
        except SystemExit as e:
            diags.extend(from_exit(e))
            status = e.code if isinstance(e.code, int) else 1
    relativize(diags)
    locate(diags, file_texts(diags))
    if status and not any(d.severity == ERROR for d in diags):
        diags.append(Diagnostic("exit status %d" % status, code="exit"))
    failed = bool(status) or any(d.severity == ERROR for d in diags)
    print(json.dumps(report(tool, diags, printed.text, ok=not failed), indent=1))
    sys.exit(1 if failed else 0)
