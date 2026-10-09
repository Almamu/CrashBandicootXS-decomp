#!/usr/bin/env python3
"""Draw the README's progress badges as SVG files.

Four badges, written to DIR (CI publishes them to the `badges` branch,
see .github/workflows/build.yml):

  matching.svg      the code that builds byte for byte from C: objdiff's
                    matched_code over total_code, by bytes
  nonmatching.svg   the code that has C at all: the bytes of the report
                    units with a base object (a source file), matching
                    or not; the rest is still raw assembly
  clean.svg         the functions with no matching workarounds, as
                    tools/match_idioms.py --functions counts them
  workarounds.svg   the functions that still have at least one (#662)

The first two read the progress report (`make NON_MATCHING=1 report`,
then `objdiff-cli report generate -o report.json`, docs/decomp_dev.md)
and the objdiff.json `make report` wrote; the last two need the same
build's objects. With NON_MATCHING=1 a parked function's C builds but
doesn't match, so it counts as non-matching only, as it should.

Usage:
  tools/badges.py [--report report.json] [--objdiff objdiff.json] DIR
"""

import argparse
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import match_idioms  # noqa: E402

# The badge drawing is Almamu/MotoRacerWorldTour-decomp's (tools/chunks.py
# badge()): shields.io's flat style, drawn here so the badges need no
# external service. Widths are Verdana 11px, roughly.
BADGE_COLORS = ((90, "#4c1"), (60, "#97ca00"), (40, "#a4a61d"), (20, "#dfb317"),
                (10, "#fe7d37"), (0, "#e05d44"))
# For a count of what's left: green at none, red past a few hundred.
LEFT_COLORS = ((0, "#4c1"), (10, "#97ca00"), (50, "#a4a61d"), (100, "#dfb317"),
               (250, "#fe7d37"))
LEFT_WORST = "#e05d44"


def text_width(text):
    narrow, wide = "fijlrt.:,;!|' ()/", "mwMW%@"
    return sum(4 if ch in narrow else 10 if ch in wide else 7 for ch in text) + 10


def pct_color(pct):
    return next(c for limit, c in BADGE_COLORS if pct >= limit)


def left_color(n):
    return next((c for limit, c in LEFT_COLORS if n <= limit), LEFT_WORST)


def badge(label, value, color):
    lw, vw = text_width(label), text_width(value)
    w = lw + vw
    return f"""<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="20" role="img" aria-label="{label}: {value}">
<title>{label}: {value}</title>
<linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/><stop offset="1" stop-opacity=".1"/></linearGradient>
<clipPath id="r"><rect width="{w}" height="20" rx="3" fill="#fff"/></clipPath>
<g clip-path="url(#r)"><rect width="{lw}" height="20" fill="#555"/><rect x="{lw}" width="{vw}" height="20" fill="{color}"/><rect width="{w}" height="20" fill="url(#s)"/></g>
<g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" font-size="11">
<text x="{lw / 2}" y="15" fill="#010101" fill-opacity=".3">{label}</text><text x="{lw / 2}" y="14">{label}</text>
<text x="{lw + vw / 2}" y="15" fill="#010101" fill-opacity=".3">{value}</text><text x="{lw + vw / 2}" y="14">{value}</text>
</g>
</svg>
"""


def code_progress(report_path, objdiff_path):
    """(total, matching, with_c) code bytes from the progress report."""
    with open(report_path) as f:
        report = json.load(f)
    with open(objdiff_path) as f:
        has_base = {u["name"] for u in json.load(f)["units"] if u.get("base_path")}
    total = int(report["measures"]["total_code"])
    matching = int(report["measures"].get("matched_code", 0))
    with_c = sum(int(u["measures"].get("total_code", 0))
                 for u in report["units"] if u["name"] in has_base)
    return total, matching, with_c


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--report", default=os.path.join(ROOT, "report.json"),
                    help="objdiff-cli's progress report (default: report.json)")
    ap.add_argument("--objdiff", default=os.path.join(ROOT, "objdiff.json"),
                    help="the objdiff config `make report` wrote (default: objdiff.json)")
    ap.add_argument("dir", help="where to write the SVG files")
    args = ap.parse_args()

    total, matching, with_c = code_progress(args.report, args.objdiff)
    counts = match_idioms.function_counts()
    if counts is None:
        return 1
    funcs, with_wa, _ = counts
    n, w = sum(funcs.values()), sum(with_wa.values())

    os.makedirs(args.dir, exist_ok=True)
    badges = []
    for name, label, done in (("matching", "matching", matching),
                              ("nonmatching", "non-matching", with_c)):
        pct = 100 * done / total
        badges.append((name, label, "%.2f%%" % pct, pct_color(pct),
                       "%d/%d bytes of code" % (done, total)))
    pct = 100 * (n - w) / n
    badges.append(("clean", "clean functions", "%.2f%%" % pct, pct_color(pct),
                   "%d/%d functions with no matching workarounds" % (n - w, n)))
    badges.append(("workarounds", "workarounds left",
                   "%d function%s" % (w, "" if w == 1 else "s"), left_color(w),
                   "%d functions with at least one matching workaround" % w))
    for name, label, value, color, detail in badges:
        with open(os.path.join(args.dir, name + ".svg"), "w") as f:
            f.write(badge(label, value, color))
        print("%s: %s (%s)" % (label, value, detail))
    return 0


if __name__ == "__main__":
    sys.exit(main())
