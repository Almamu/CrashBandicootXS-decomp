#!/usr/bin/env python3
"""Enumerates every still-raw function across asm/*.s (thumb_func_start /
non_word_aligned_thumb_func_start / arm_func_start), sorts them by ROM
address, tags each with the category tools/report_units.py's UNITS list
already assigns that address range, and groups them into contiguous,
single-category chunks capped at --max-functions (default 25) for use as
self-contained "pick this up" issues.

Usage: python3 tools/chunk_remaining_work.py [--max-functions N] [--json out.json] [--md out.md]
       python3 tools/chunk_remaining_work.py --issues-dir DIR --parked-issues   # write title/body files
       python3 tools/chunk_remaining_work.py --issues-dir DIR --parked-issues --create-github-issues  # also open them on GitHub via `gh`
       python3 tools/chunk_remaining_work.py --cleanup-scan --issues-dir DIR   # retroactive cleanup issues instead, one per already-matched file

This is read-only with respect to asm/ and src/ - it never touches them.
--create-github-issues is the one mode with a real side effect (it opens
issues on GitHub via `gh issue create`); everything else just writes
local files. Re-run any time more functions get matched (and cut out of
asm/*.s) to regenerate the chunk list against current ground truth -
re-running --create-github-issues will open duplicates of anything
already-open, so only do that deliberately.
"""
import argparse
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ASM_DIR = ROOT / "asm"
SRC_DIR = ROOT / "src"
REPO_URL = "https://github.com/Almamu/CrashBandicootXS-decomp"

FUNC_START_RE = re.compile(
    r"^\s*(?:thumb_func_start|non_word_aligned_thumb_func_start|arm_func_start)\s+(\S+)\s*$"
)
ADDR_RE = re.compile(r"^(\S+):\s*@\s*0x([0-9A-Fa-f]+)")
IF_NON_MATCHING_RE = re.compile(r"^\s*\.if\s+NON_MATCHING\s*==\s*0\s*$")
ENDIF_RE = re.compile(r"^\s*\.endif\b")
IF_RE = re.compile(r"^\s*\.if\b")


def load_units():
    src = (ROOT / "tools" / "report_units.py").read_text()
    start_idx = src.index("UNITS = [")
    end_idx = src.index("\n]\n", start_idx) + 3
    ns = {}
    exec(src[start_idx:end_idx], ns)
    return ns["UNITS"]


def category_for(addr, units):
    for i in range(len(units) - 1):
        start, base_rel, category = units[i]
        end = units[i + 1][0]
        if start <= addr < end:
            return category, base_rel
    return None, None


def scan_functions():
    funcs = []
    for path in sorted(ASM_DIR.glob("*.s")):
        lines = path.read_text().splitlines()
        pending_name = None
        # Stack of booleans: True if this .if level is a "NON_MATCHING == 0"
        # guard (i.e. everything inside it is an already-parked function's
        # real bytes, kept for the NON_MATCHING=0 build - not raw/unmatched
        # work). Any nonzero depth of NON_MATCHING==0 guard marks a parked
        # function; a plain .if unrelated to NON_MATCHING nested inside one
        # (e.g. a jump-table guard) still counts as parked since it's inside
        # the outer guard.
        if_stack = []
        for line in lines:
            if IF_NON_MATCHING_RE.match(line):
                if_stack.append(True)
                continue
            if ENDIF_RE.match(line):
                if if_stack:
                    if_stack.pop()
                continue
            if IF_RE.match(line):
                if_stack.append(False)
                continue

            m = FUNC_START_RE.match(line)
            if m:
                pending_name = m.group(1)
                continue
            if pending_name is not None:
                m2 = ADDR_RE.match(line.strip())
                if m2 and m2.group(1) == pending_name:
                    addr = int(m2.group(2), 16)
                    is_parked = any(if_stack)
                    funcs.append({
                        "name": pending_name,
                        "addr": addr,
                        "file": path.name,
                        "parked": is_parked,
                    })
                    pending_name = None
                # if the immediately-following line isn't the address line
                # (shouldn't happen with this project's convention), drop
                # pending_name so we don't mis-attribute a later address
                elif m2:
                    pending_name = None
    return funcs


def annotate(funcs, units):
    """Sorts by address and attaches category/size to every function
    (parked or not) - sizes need the full, unfiltered address sequence to
    be correct, so this must run before any filtering."""
    funcs = sorted(funcs, key=lambda f: f["addr"])
    for i, f in enumerate(funcs):
        f["category"], f["base_rel"] = category_for(f["addr"], units)
        f["size"] = funcs[i + 1]["addr"] - f["addr"] if i + 1 < len(funcs) else None
    return funcs


FUNC_SIG_RE = re.compile(r"^[A-Za-z_][\w \*]*?\b(sub_[0-9A-Fa-f]{6,8}|nullsub_\d+)\s*\(")
IFDEF_NON_MATCHING_RE = re.compile(r"^#if\s+NON_MATCHING\s*$")
ENDIF_C_RE = re.compile(r"^#endif\b")


DIR_TO_CATEGORY = {
    "graphics": "graphics",
    "system": "system",
    "util": "util",
    "audio": "audio",
}


def category_from_path(rel_path):
    parts = Path(rel_path).parts
    if len(parts) >= 2 and parts[0] == "src":
        return DIR_TO_CATEGORY.get(parts[1])
    return None


def scan_parked_functions():
    """Finds every function inside a `#if NON_MATCHING` block in src/**/*.c
    and pulls its immediately-preceding /* ... */ doc comment (this
    project's convention always documents a parked function's remaining
    gap right above it)."""
    parked = []
    for path in sorted(SRC_DIR.rglob("*.c")):
        lines = path.read_text().splitlines()
        depth = 0
        comment_buf = []
        in_comment = False
        for line in lines:
            stripped = line.strip()
            if IFDEF_NON_MATCHING_RE.match(stripped):
                depth += 1
                continue
            if ENDIF_C_RE.match(stripped) and depth > 0:
                depth -= 1
                continue
            if depth == 0:
                comment_buf = []
                in_comment = False
                continue

            if in_comment:
                comment_buf.append(line)
                if "*/" in stripped:
                    in_comment = False
                continue
            if stripped.startswith("/*"):
                comment_buf = [line]
                in_comment = not ("*/" in stripped)
                continue

            m = FUNC_SIG_RE.match(stripped)
            if m and "extern" not in stripped:
                parked.append({
                    "name": m.group(1),
                    "file": str(path.relative_to(ROOT)),
                    "comment": "\n".join(comment_buf).strip(),
                })
                comment_buf = []
                continue
            if stripped and not stripped.startswith("*") and not stripped.startswith("extern"):
                # any other real code line resets the "comment directly
                # above" assumption
                comment_buf = []
    return parked


# A raw offset-cast smell: `*(TYPE *)((u8 *)base + 0xNN)` or the
# equivalent without the inner cast, `*(TYPE *)(base + 0xNN)`. Doesn't
# try to catch every possible spelling - this is a starting point for a
# human/agent to look at, not an exhaustive linter.
RAW_OFFSET_CAST_RE = re.compile(
    r"\*\s*\([A-Za-z_][\w ]*\*\)\s*\(\s*(?:\([A-Za-z_][\w ]*\*\)\s*)?[\w>.\[\]-]+\s*\+\s*0x[0-9A-Fa-f]+\s*\)"
)
RAW_HW_ADDR_RE = re.compile(r"0x0[4-7]00[0-9A-Fa-f]{4}\b")
REGISTER_ASM_RE = re.compile(r"\bregister\b.*\basm\s*\(")


def scan_cleanup_candidates():
    """Scans already-matched src/**/*.c (skipping #if NON_MATCHING
    blocks - those are parked functions, tracked separately) for raw
    pointer-arithmetic offset casts and raw hardware addresses that
    docs/workflow.md step 7 says should become named struct fields /
    REG_* macros. Also counts register-pin/inline-asm density per file
    as a rough "how risky would cleanup here be" signal, per the
    project's own carve-out for register-allocation-sensitive code."""
    results = []
    for path in sorted(SRC_DIR.rglob("*.c")):
        lines = path.read_text().splitlines()
        depth = 0
        in_comment = False
        offset_hits = []
        hw_addr_hits = []
        register_asm_count = 0
        for lineno, line in enumerate(lines, 1):
            stripped = line.strip()
            if IFDEF_NON_MATCHING_RE.match(stripped):
                depth += 1
                continue
            if ENDIF_C_RE.match(stripped) and depth > 0:
                depth -= 1
                continue
            if depth > 0:
                continue  # inside a parked function - not this scan's job
            was_in_comment = in_comment
            if "/*" in stripped:
                in_comment = "*/" not in stripped.split("/*", 1)[1]
            elif in_comment and "*/" in stripped:
                in_comment = False
            if was_in_comment or stripped.startswith("*") or stripped.startswith("//"):
                continue  # comment line/continuation, not real code
            if RAW_OFFSET_CAST_RE.search(line):
                offset_hits.append((lineno, stripped))
            if RAW_HW_ADDR_RE.search(line):
                hw_addr_hits.append((lineno, stripped))
            if REGISTER_ASM_RE.search(line):
                register_asm_count += 1
        if offset_hits or hw_addr_hits:
            results.append({
                "file": str(path.relative_to(ROOT)),
                "offset_hits": offset_hits,
                "hw_addr_hits": hw_addr_hits,
                "register_asm_count": register_asm_count,
            })
    return results


def render_cleanup_issue(entry):
    f = entry["file"]
    n_offset = len(entry["offset_hits"])
    n_hw = len(entry["hw_addr_hits"])
    risk_note = ""
    if entry["register_asm_count"] > 0:
        risk_note = (
            f"\n**Caution:** this file has {entry['register_asm_count']} "
            "register-pin/inline-asm site(s) - some of the raw offsets "
            "below may be load-bearing for an exact ROM register "
            "allocation (see docs/workflow.md step 7's carve-out). Verify "
            "each change with a full clean `make compare`, and revert "
            "(with a one-line comment explaining why) anything that "
            "changes the generated bytes.\n"
        )
    title = f"Cleanup: raw pointer arithmetic in {f} ({n_offset} offset cast(s), {n_hw} raw hw address(es))"
    offset_list = "\n".join(f"- `{f}:{ln}`: `{code}`" for ln, code in entry["offset_hits"][:40])
    if n_offset > 40:
        offset_list += f"\n- ... ({n_offset - 40} more, re-run the scan for the full list)"
    hw_list = "\n".join(f"- `{f}:{ln}`: `{code}`" for ln, code in entry["hw_addr_hits"][:40])
    body = f"""Retroactive cleanup candidate, generated by
`tools/chunk_remaining_work.py --cleanup-scan` from the current state of
`{f}`. See [docs/workflow.md]({REPO_URL}/blob/main/docs/workflow.md)
step 7 for the actual cleanup process this issue is scoping, and
[CONTRIBUTING.md]({REPO_URL}/blob/main/CONTRIBUTING.md#cleanup-tasks) for
the general cleanup-task conventions.
{risk_note}
**What to do:** for each raw offset cast below, check whether a named
struct for the same object already exists elsewhere in the codebase
(reuse/extend it rather than inventing a second one for the same
bytes); if not and the layout is confidently understood, add one. Do
the same for any raw hardware address, using the matching `REG_*`/
`OAM`/`PLTT`/`DMA_*` macro. **Rebuild and `make compare` after each
edit**, not just once at the end - a struct-typed rewrite can silently
change codegen (CSE, instruction scheduling, register choice).

**Raw offset casts ({n_offset}):**

{offset_list or "(none)"}
"""
    if hw_list:
        body += f"\n**Raw hardware addresses ({n_hw}):**\n\n{hw_list}\n"
    return title, body


def build_chunks(funcs, max_functions):
    """funcs must already be annotate()-d and pre-filtered (e.g. to
    exclude parked functions) by the caller."""
    chunks = []
    current = []
    current_cat = object()  # sentinel, never equals a real category
    for f in funcs:
        if f["category"] != current_cat or len(current) >= max_functions:
            if current:
                chunks.append(current)
            current = [f]
            current_cat = f["category"]
        else:
            current.append(f)
    if current:
        chunks.append(current)
    return chunks


def chunk_summary(chunk):
    start = chunk[0]["addr"]
    sizes = [f["size"] for f in chunk if f["size"] is not None]
    total_size = sum(sizes) if sizes else None
    end = chunk[-1]["addr"] + (chunk[-1]["size"] or 0)
    return {
        "start": start,
        "end": end,
        "category": chunk[0]["category"],
        "count": len(chunk),
        "total_size": total_size,
        "functions": [f["name"] for f in chunk],
        "files": sorted({f["file"] for f in chunk}),
    }


CATEGORY_BLURBS = {
    "game_loop": "core per-frame/state-machine logic - see docs/rom_map.md's \"Subdividing game_loop\" for the sub-bucket this range likely falls in.",
    "actor": "the category/part/vtable object-construction system - see docs/rom_map.md's actor-zone sections.",
    "overlay_ui": "the pause-menu/settings/dialog UI system - see docs/rom_map.md's \"audio_sfx was almost entirely wrong\" section and follow-ups.",
    "graphics_loading": "package/tile/level asset loading - see docs/rom_map.md's graphics_loading sections.",
    "graphics": "sprite/actor rendering and screen effects - check docs/status/graphics.md for neighboring already-matched files first.",
    "audio": "the licensed Shin'en GAX2 sound engine - see docs/audio.md before starting, this is harder/lower-priority than game code.",
    "hud": "HUD icon/text widgets and stat counters - see docs/rom_map.md's HUD sections.",
    "system": "startup/memory/interrupt/input infrastructure - check docs/status/system.md for neighboring already-matched files first.",
    "util": "math/string/RNG/line-drawing helpers - check docs/status/util.md for neighboring already-matched files first.",
    None: "not yet categorized by docs/rom_map.md - you may be the first to look at this range; consider a docs/rom_map.md note once you understand it.",
}


def render_issue(summary, index, total):
    start, end, cat, count = summary["start"], summary["end"], summary["category"], summary["count"]
    size_kb = (summary["total_size"] or 0) / 1024
    title = f"Match 0x{start:08X}-0x{end:08X} ({count} functions, ~{size_kb:.1f} KB, {cat or 'uncategorized'})"
    blurb = CATEGORY_BLURBS.get(cat, CATEGORY_BLURBS[None])
    func_list = "\n".join(f"- [ ] `{n}`" for n in summary["functions"])
    files = ", ".join(f"`asm/{f}`" for f in summary["files"])
    body = f"""One chunk ({index} of {total} in this generation pass) of the project's
remaining-work inventory, generated by `tools/chunk_remaining_work.py`
from the current state of `asm/*.s`. See [docs/workflow.md]({REPO_URL}/blob/main/docs/workflow.md)
for the required per-function matching process before starting - this
issue is scope, not instructions.

**This chunk is a scoping convenience, not a contract.** There's no
obligation to match every function below before opening a PR, or to do
it alone - multiple people can each take a few functions from this list
(say which ones in a comment, so nobody duplicates work), split it
across several PRs, or hand off partial progress. Reference this issue
from your PR with `Closes` **only if every function below ends up
byte-exact matched** - a parked or left-raw function, however well
understood or documented, keeps this issue open; otherwise just say
what's left (and why) in a comment.

**Claiming:** check off a function's box once it's matched/parked (or
say in a comment which ones you're taking, if you don't have write
access to edit the checklist) - see [CONTRIBUTING.md]({REPO_URL}/blob/main/CONTRIBUTING.md#claiming-work).
A checked box means the function is handled (matched or parked), not
that this issue is ready to close - see "Opening the PR" in
CONTRIBUTING.md for the closing rule.

**Category:** `{cat or "uncategorized"}` - {blurb}

**Range:** `0x{start:08X}`-`0x{end:08X}` (~{size_kb:.1f} KB)

**Source file(s):** {files}

**Functions in this chunk ({count}):**

{func_list}

---

*This is a point-in-time snapshot. If some of these functions have
already been matched by the time you pick this up, skip them and note it
in your PR instead of re-doing or reverting that work. If a Claude Code
session is picking this up, see `.claude/skills/match-chunk/SKILL.md`.*
"""
    return title, body


def render_parked_issue(p):
    title = f"Byte-match parked function {p['name']}"
    comment = p["comment"] or "*(no doc comment found directly above the function - check the file for context.)*"
    body = f"""One already-parked (`#if NON_MATCHING`) function that's fully
understood semantically but not yet byte-exact - generated by
`tools/chunk_remaining_work.py` from the current state of `src/**/*.c`.
See [docs/workflow.md]({REPO_URL}/blob/main/docs/workflow.md) for the
general process; this specific case is narrower than a normal "match
this function" task - **the C is already correct, it just doesn't
compile to the same bytes as the ROM**. This is usually a specific gcc
2.9 register-allocation, instruction-scheduling, or peephole-optimization
quirk. See `docs/matching.md` for a large catalog of techniques that have
worked on similar cases elsewhere in this codebase (register `asm("rN")`
pins, forcing block order with `goto`, the negative-constant bit-clear
idiom, etc.) before assuming something is truly unfixable.

**Function:** `{p['name']}`

**File:** `{p['file']}`

**Current doc comment (what's already been tried):**

```c
{comment}
```

---

*If you find a fix, update the function (removing the `#if NON_MATCHING`
guard and the now-obsolete comment), cut its raw bytes out of the
corresponding `asm/*.s` file, run the full clean `make compare` to
confirm, and update `docs/matching.md`/`docs/status/<system>.md`
accordingly - see `docs/workflow.md` step 6 onward.*
"""
    return title, body


def get_issue_node_id(number):
    result = subprocess.run(
        ["gh", "api", f"repos/{{owner}}/{{repo}}/issues/{number}", "--jq", ".node_id"],
        capture_output=True, text=True,
    )
    if result.returncode != 0:
        print(f"Failed to look up node id for issue #{number}: {result.stderr}", file=sys.stderr)
        return None
    return result.stdout.strip()


def add_blocked_by(issue_number, blocking_issue_number):
    """Sets a native GitHub 'blocked by' relationship (the same one the
    web UI shows in an issue's sidebar) via GraphQL - there's no `gh
    issue` CLI flag for this yet. Best-effort: prints a warning and
    returns False on failure rather than raising, so a whole batch isn't
    lost over one lookup hiccup."""
    issue_id = get_issue_node_id(issue_number)
    blocking_id = get_issue_node_id(blocking_issue_number)
    if not issue_id or not blocking_id:
        return False
    query = """
    mutation($issueId: ID!, $blockingId: ID!) {
      addBlockedBy(input: {issueId: $issueId, blockingIssueId: $blockingId}) {
        clientMutationId
      }
    }
    """
    result = subprocess.run(
        ["gh", "api", "graphql", "-f", f"query={query}",
         "-f", f"issueId={issue_id}", "-f", f"blockingId={blocking_id}"],
        capture_output=True, text=True,
    )
    if result.returncode != 0:
        print(f"Failed to link #{issue_number} as blocked by #{blocking_issue_number}: {result.stderr}", file=sys.stderr)
        return False
    return True


def list_open_issues_with_label(label):
    result = subprocess.run(
        ["gh", "issue", "list", "--label", label, "--state", "open",
         "--json", "number,title,body", "--limit", "300"],
        capture_output=True, text=True,
    )
    if result.returncode != 0:
        print(f"Failed to list issues with label {label}: {result.stderr}", file=sys.stderr)
        return []
    return json.loads(result.stdout)


FUNC_CHECKBOX_RE = re.compile(r"- \[[ x]\] `(\S+)`")
RANGE_RE = re.compile(r"`0x([0-9A-Fa-f]+)`-`0x([0-9A-Fa-f]+)`")
CATEGORY_LINE_RE = re.compile(r"\*\*Category:\*\* `([^`]*)`")


def render_cleanup_followup_issue(chunk_number, chunk_title, functions, category, start, end):
    size_kb = (end - start) / 1024
    title = f"Cleanup follow-up for #{chunk_number}: 0x{start:08X}-0x{end:08X}"
    func_list = "\n".join(f"- [ ] `{n}`" for n in functions)
    body = f"""**Blocked by #{chunk_number} - do not start until every function
there is matched or parked.** This issue is a placeholder generated
alongside #{chunk_number} so the cleanup pass doesn't get forgotten once
that chunk lands; there's nothing to scan yet because the C doesn't
exist until #{chunk_number} is done.

Once #{chunk_number} is complete, go through whatever `src/**/*.c`
file(s) its functions ended up in and apply
[docs/workflow.md]({REPO_URL}/blob/main/docs/workflow.md) step 7's
cleanup pass: replace raw pointer-arithmetic offset casts
(`*(u32 *)((u8 *)base + 0x10)`-style) with named struct fields (reusing
an existing struct for the same object if one exists elsewhere in the
codebase), and raw hardware addresses with the matching `REG_*`/`OAM`/
`PLTT`/`DMA_*` macro.

**The carve-out matters here more than usual:** #{chunk_number}'s
functions were just freshly matched, often with register pins/inline
asm to reproduce an exact ROM register allocation - don't touch those
just because they look unclean. Only replace a raw offset where you can
demonstrate (by rebuilding) that the cleaner version still compiles to
the identical bytes; if it doesn't, leave it with a one-line comment
explaining why, per [CONTRIBUTING.md]({REPO_URL}/blob/main/CONTRIBUTING.md#cleanup-tasks).

**Category:** `{category or "uncategorized"}`

**Functions from #{chunk_number} to check ({len(functions)}):**

{func_list}
"""
    return title, body


def create_github_issue(title, body, labels):
    with tempfile.NamedTemporaryFile("w", suffix=".md", delete=False) as f:
        f.write(body)
        body_path = f.name
    cmd = ["gh", "issue", "create", "--title", title, "--body-file", body_path]
    for label in labels:
        cmd += ["--label", label]
    result = subprocess.run(cmd, capture_output=True, text=True)
    Path(body_path).unlink(missing_ok=True)
    if result.returncode != 0:
        print(f"FAILED: {title}\n{result.stderr}", file=sys.stderr)
        return None
    return result.stdout.strip()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--max-functions", type=int, default=25)
    ap.add_argument("--json", type=str, default=None)
    ap.add_argument("--md", type=str, default=None)
    ap.add_argument("--min-count", type=int, default=1,
                     help="Skip printing chunks with fewer than this many functions (still counted in totals)")
    ap.add_argument("--issues-dir", type=str, default=None,
                     help="Write one ready-to-use GitHub issue body (title + body .md pair) per chunk into this directory")
    ap.add_argument("--min-issue-count", type=int, default=3,
                     help="Skip generating a chunk issue for groups smaller than this (they're usually a single already-tracked parked function)")
    ap.add_argument("--parked-issues", action="store_true",
                     help="Also emit one issue per already-parked (#if NON_MATCHING) function into --issues-dir")
    ap.add_argument("--create-github-issues", action="store_true",
                     help="Actually open each generated issue on GitHub via `gh issue create` (requires gh to be authenticated). Real side effect - see module docstring.")
    ap.add_argument("--cleanup-scan", action="store_true",
                     help="Instead of scanning asm/*.s for unmatched work, scan already-matched src/**/*.c for raw pointer-arithmetic/hardware-address cleanup candidates and write one issue per file into --issues-dir")
    ap.add_argument("--pair-cleanup", action="store_true",
                     help="For every open `decomp-chunk` issue on GitHub, create (or, without --create-github-issues, just render) a paired cleanup-follow-up issue blocked by it via GitHub's native issue-dependency relationship. Requires gh.")
    args = ap.parse_args()

    if args.pair_cleanup:
        chunk_issues = list_open_issues_with_label("decomp-chunk")
        print(f"Found {len(chunk_issues)} open decomp-chunk issues", file=sys.stderr)
        out_dir = Path(args.issues_dir) if args.issues_dir else None
        if out_dir:
            out_dir.mkdir(parents=True, exist_ok=True)
        created, failed, linked, link_failed = 0, 0, 0, 0
        for i, issue in enumerate(chunk_issues, 1):
            number, chunk_title, body = issue["number"], issue["title"], issue["body"]
            functions = FUNC_CHECKBOX_RE.findall(body)
            m = RANGE_RE.search(body)
            start, end = (int(m.group(1), 16), int(m.group(2), 16)) if m else (0, 0)
            cm = CATEGORY_LINE_RE.search(body)
            category = cm.group(1) if cm else None
            if not functions:
                print(f"WARNING: couldn't parse functions out of #{number}'s body, skipping", file=sys.stderr)
                continue
            title, followup_body = render_cleanup_followup_issue(number, chunk_title, functions, category, start, end)
            if out_dir:
                stem = f"{i:03d}_cleanup_followup_for_{number}"
                (out_dir / f"{stem}.title.txt").write_text(title + "\n")
                (out_dir / f"{stem}.body.md").write_text(followup_body)
            if args.create_github_issues:
                labels = ["cleanup", "blocked"]
                if category and category != "uncategorized":
                    labels.append(category)
                url = create_github_issue(title, followup_body, labels)
                if url:
                    created += 1
                    new_number = int(url.rstrip("/").rsplit("/", 1)[-1])
                    print(f"[{i}/{len(chunk_issues)}] {url} (blocked by #{number})", file=sys.stderr)
                    if add_blocked_by(new_number, number):
                        linked += 1
                    else:
                        link_failed += 1
                else:
                    failed += 1
        if out_dir:
            print(f"Wrote {len(chunk_issues)} cleanup-follow-up title/body pairs to {out_dir}", file=sys.stderr)
        if args.create_github_issues:
            print(f"Created {created} GitHub issues ({failed} failed), linked {linked} as blocked-by ({link_failed} link failures)", file=sys.stderr)
        return

    if args.cleanup_scan:
        if not args.issues_dir:
            print("--cleanup-scan requires --issues-dir", file=sys.stderr)
            sys.exit(1)
        out_dir = Path(args.issues_dir)
        out_dir.mkdir(parents=True, exist_ok=True)
        candidates = scan_cleanup_candidates()
        print(f"Found {len(candidates)} files with cleanup candidates "
              f"({sum(len(c['offset_hits']) for c in candidates)} raw offset "
              f"casts, {sum(len(c['hw_addr_hits']) for c in candidates)} raw "
              f"hw addresses total)", file=sys.stderr)
        created, failed = 0, 0
        for i, entry in enumerate(candidates, 1):
            title, body = render_cleanup_issue(entry)
            stem = f"{i:03d}_cleanup_{Path(entry['file']).stem}"
            (out_dir / f"{stem}.title.txt").write_text(title + "\n")
            (out_dir / f"{stem}.body.md").write_text(body)
            if args.create_github_issues:
                url = create_github_issue(title, body, ["cleanup"])
                if url:
                    created += 1
                    print(f"[{i}/{len(candidates)}] {url}", file=sys.stderr)
                else:
                    failed += 1
        print(f"Wrote {len(candidates)} cleanup issue title/body pairs to {out_dir}", file=sys.stderr)
        if args.create_github_issues:
            print(f"Created {created} GitHub issues ({failed} failed)", file=sys.stderr)
        return

    units = load_units()
    all_funcs = scan_functions()
    all_funcs = annotate(all_funcs, units)
    unmatched_funcs = [f for f in all_funcs if not f["parked"]]
    parked_asm_names = {f["name"] for f in all_funcs if f["parked"]}
    print(f"Scanned {len(all_funcs)} functions still physically raw in asm/*.s: "
          f"{len(unmatched_funcs)} genuinely unmatched, {len(parked_asm_names)} already parked",
          file=sys.stderr)

    chunks = build_chunks(unmatched_funcs, args.max_functions)
    summaries = [chunk_summary(c) for c in chunks]

    if args.json:
        Path(args.json).write_text(json.dumps(summaries, indent=2))
        print(f"Wrote {args.json} ({len(summaries)} chunks)", file=sys.stderr)

    if args.md:
        lines = [f"# Remaining-work chunks ({len(summaries)} total, max {args.max_functions} functions each)\n"]
        by_cat = {}
        for s in summaries:
            by_cat.setdefault(s["category"], []).append(s)
        for cat in sorted(by_cat, key=lambda c: -sum(x["count"] for x in by_cat[c])):
            group = by_cat[cat]
            total_fns = sum(x["count"] for x in group)
            total_kb = sum(x["total_size"] or 0 for x in group) / 1024
            lines.append(f"\n## {cat or '(uncategorized)'} - {len(group)} chunks, {total_fns} functions, ~{total_kb:.1f} KB\n")
            for s in group:
                if s["count"] < args.min_count:
                    continue
                size_kb = (s["total_size"] or 0) / 1024
                lines.append(
                    f"- `0x{s['start']:08X}`-`0x{s['end']:08X}` ({s['count']} fns, ~{size_kb:.2f} KB): "
                    + ", ".join(f"`{n}`" for n in s["functions"][:6])
                    + (f", ... (+{s['count']-6} more)" if s["count"] > 6 else "")
                )
        Path(args.md).write_text("\n".join(lines) + "\n")
        print(f"Wrote {args.md}", file=sys.stderr)

    if args.issues_dir:
        out_dir = Path(args.issues_dir)
        out_dir.mkdir(parents=True, exist_ok=True)
        eligible = [s for s in summaries if s["count"] >= args.min_issue_count]
        created, failed = 0, 0
        for i, s in enumerate(eligible, 1):
            title, body = render_issue(s, i, len(eligible))
            stem = f"{i:03d}_0x{s['start']:08X}"
            (out_dir / f"{stem}.title.txt").write_text(title + "\n")
            (out_dir / f"{stem}.body.md").write_text(body)
            if args.create_github_issues:
                labels = ["decomp-chunk"]
                if s["category"]:
                    labels.append(s["category"])
                url = create_github_issue(title, body, labels)
                if url:
                    created += 1
                    print(f"[{i}/{len(eligible)}] {url}", file=sys.stderr)
                else:
                    failed += 1
        print(f"Wrote {len(eligible)} issue title/body pairs to {out_dir} "
              f"({len(summaries) - len(eligible)} smaller chunks skipped, "
              f"see docs/matching.md for those individually-tracked functions)",
              file=sys.stderr)
        if args.create_github_issues:
            print(f"Created {created} GitHub issues ({failed} failed)", file=sys.stderr)

        if args.parked_issues:
            parked = scan_parked_functions()
            # cross-check against the asm-side parked set so we only emit
            # an issue for functions that are genuinely still guarded in
            # both the .c (comment/#if) and the .s (raw bytes) - if one
            # side lost track of a function that's a real inconsistency
            # worth surfacing, not silently papering over.
            mismatched = [p["name"] for p in parked if p["name"] not in parked_asm_names]
            if mismatched:
                print(f"WARNING: {len(mismatched)} functions are #if NON_MATCHING in "
                      f"src/**/*.c but their raw bytes aren't guarded in asm/*.s (or weren't "
                      f"found there) - skipping issue generation for these, check by hand: "
                      f"{', '.join(mismatched)}", file=sys.stderr)
            parked = [p for p in parked if p["name"] in parked_asm_names]
            start_idx = len(eligible) + 1
            p_created, p_failed = 0, 0
            for offset, p in enumerate(parked):
                title, body = render_parked_issue(p)
                stem = f"{start_idx + offset:03d}_parked_{p['name']}"
                (out_dir / f"{stem}.title.txt").write_text(title + "\n")
                (out_dir / f"{stem}.body.md").write_text(body)
                if args.create_github_issues:
                    labels = ["parked-function"]
                    cat = category_from_path(p["file"])
                    if cat:
                        labels.append(cat)
                    url = create_github_issue(title, body, labels)
                    if url:
                        p_created += 1
                        print(f"[parked {offset+1}/{len(parked)}] {url}", file=sys.stderr)
                    else:
                        p_failed += 1
            print(f"Wrote {len(parked)} additional per-function parked-issue "
                  f"title/body pairs to {out_dir}", file=sys.stderr)
            if args.create_github_issues:
                print(f"Created {p_created} GitHub issues ({p_failed} failed)", file=sys.stderr)

    if not args.json and not args.md and not args.issues_dir:
        for s in summaries:
            size_kb = (s["total_size"] or 0) / 1024
            print(f"0x{s['start']:08X}-0x{s['end']:08X} [{s['category']}] {s['count']} fns ~{size_kb:.2f}KB")


if __name__ == "__main__":
    main()
