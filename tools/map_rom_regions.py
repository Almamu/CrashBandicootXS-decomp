#!/usr/bin/env python3
"""Rough, re-runnable categorization of the whole ROM's code by region - a
much coarser sibling to tools/report_units.py's exact per-file objdiff
units. Where report_units.py only ever looks at code that's already been
matched, this script's job is the opposite: take the still-fully-raw
asm/code_3_*.s files, split them into contiguous address ranges bounded by
whichever functions already have a real name (a strong "we understand
this" signal - see docs/naming.md), and use call-graph connectivity to
guess what fills the gaps in between.

Two things make guessing this way harder than it sounds, both worth
knowing before trusting the output:

- **Hub contamination**: a handful of functions (PlaySfx is the worst
  offender) are called from all over gameplay code for incidental reasons
  (every action plays a sound), not because the caller has anything to do
  with audio. Treating every call edge as equally meaningful floods a
  naive graph-distance categorization with false positives - see HUBS
  below, which are deliberately excluded from the "linked to landmark X"
  counts for exactly this reason.
- **This only sees calls made from *other raw asm functions***. A call
  from an already-matched src/*.c file into raw asm (like
  src/system/asset_util.c's LZ77UnCompWrapper/RLUnCompWrapper calls) is
  invisible here - re-check callers by hand (grep src/) before trusting
  an "isolated, no callers found" result.

Re-run any time more functions get real names or more of src/ is matched
- the region boundaries and cluster sizes will shift as ground truth
improves. Purely a reconnaissance tool: nothing here should be treated as
confirmed until backed by actually reading the functions in question.
"""
import re
from pathlib import Path
from collections import defaultdict, deque

REPO = Path(__file__).resolve().parent.parent
RAW_ASM_FILES = [
    "asm/code_3_1.s", "asm/code_3_1_2.s", "asm/code_3_1_3.s", "asm/code_3_1_5.s",
    "asm/code_3_1_7.s", "asm/code_3_2.s", "asm/code_3_3.s",
]

# Hand-maintained: named functions (i.e. not sub_XXXXXXXX/nullsub_N) group
# into these rough domains by what's already understood about them (see
# docs/graphics.md, docs/audio.md). Extend this as more functions get
# named - it's the only part of this script that needs real domain
# knowledge, everything else is mechanical.
LANDMARK_DOMAINS = {
    'PlaySfx': 'audio_sfx',
    'LoadGraphicsPackage': 'graphics_loading', 'DecompressCategorySpriteSheet': 'graphics_loading',
    'LoadSpriteFrameTiles': 'graphics_loading', 'SetupSpriteFrameOam': 'graphics_loading',
    'InitObjTileFreeList': 'graphics_loading', 'SetupActorVramPool': 'graphics_loading',
    'LoadLevelGraphics': 'graphics_loading', 'LoadBg2Background': 'graphics_loading',
    'LoadObjSpriteTiles': 'graphics_loading',
    'LZ77UnCompWrapper': 'graphics_loading', 'RLUnCompWrapper': 'graphics_loading',
    'UpdateGameFrame': 'game_loop', 'MainLoop': 'game_loop',
    'InitHudIconWidgetA': 'hud', 'InitHudIconWidgetB': 'hud', 'MeasureText': 'hud',
    'UploadHudTile': 'hud', 'InitHudTextWidget': 'hud',
    'InitActorCategory': 'actor_system', 'SelectActorCategory': 'actor_system',
    'InitActorPart': 'actor_system', 'UpdateAnimatedActorPart': 'actor_system',
    'ConstructAnimTableState': 'actor_system', 'ConstructActorPart': 'actor_system',
    'GetAnimFrameData': 'actor_system',
}
# Called from everywhere for incidental reasons - excluded from per-region
# "linked to landmark" evidence so genuine domain-specific links aren't
# drowned out. See the module docstring.
HUBS = {'PlaySfx'}

func_start_re = re.compile(r"^\s*(?:thumb_func_start|arm_func_start)\s+(\S+)")
label_re = re.compile(r"^(\S+):\s*(?:@\s*0x0*([0-9A-Fa-f]+))?")
bl_re = re.compile(r"\bbl\s+(\S+)")


def parse_functions_and_calls():
    functions = []
    calls = {}
    for relpath in RAW_ASM_FILES:
        lines = (REPO / relpath).read_text().splitlines()
        pending_name = None
        cur_name = None
        cur_addr = None
        for line in lines:
            m = func_start_re.match(line)
            if m:
                pending_name = m.group(1)
                continue
            lm = label_re.match(line)
            if lm and pending_name == lm.group(1):
                name, addr = lm.group(1), int(lm.group(2), 16) if lm.group(2) else None
                cur_name, cur_addr = name, addr
                calls.setdefault(cur_name, set())
                functions.append({"name": name, "addr": addr, "file": relpath})
                pending_name = None
                continue
            if cur_name is not None:
                for bm in bl_re.finditer(line):
                    calls[cur_name].add(bm.group(1))
    functions.sort(key=lambda f: f["addr"])
    return functions, calls


def size_of(functions, idx):
    nxt = functions[idx + 1]["addr"] if idx + 1 < len(functions) else functions[idx]["addr"]
    sz = nxt - functions[idx]["addr"]
    return sz if 0 <= sz <= 0x2000 else 0  # cross-file boundary artifact guard


def connected_components(names, global_adj):
    """Components using only edges *internal* to `names` - global_adj is
    the whole-program call graph, but a naive BFS over it from a handful
    of zone functions immediately walks out into the rest of the program
    and returns one giant component every time, which is useless. Zone
    boundaries only mean something once traversal is confined to the
    zone itself."""
    local_adj = {n: {nb for nb in global_adj.get(n, ()) if nb in names} for n in names}
    seen, comps = set(), []
    for n in names:
        if n in seen:
            continue
        comp, q = set(), deque([n])
        seen.add(n)
        while q:
            x = q.popleft()
            comp.add(x)
            for nb in local_adj.get(x, ()):
                if nb not in seen:
                    seen.add(nb)
                    q.append(nb)
        comps.append(comp)
    return sorted(comps, key=len, reverse=True)


def main():
    functions, calls = parse_functions_and_calls()
    by_name = {f["name"]: f for f in functions}
    adj = defaultdict(set)
    for caller, targets in calls.items():
        for t in targets:
            if t in by_name:
                adj[caller].add(t)
                adj[t].add(caller)

    landmarks = [f for f in functions if f["name"] in LANDMARK_DOMAINS]
    print(f"{len(functions)} functions parsed, {len(landmarks)} named landmarks\n")

    for i, lm in enumerate(landmarks):
        idx = functions.index(lm)
        sz = size_of(functions, idx)
        print(f"{lm['addr']:#010x} {lm['name']:32s} [{LANDMARK_DOMAINS[lm['name']]:16s}] {sz:6d}B  {lm['file']}")
        if i + 1 < len(landmarks):
            gap_start = lm["addr"] + sz
            gap_end = landmarks[i + 1]["addr"]
            gap = gap_end - gap_start
            if gap > 512:
                zone_names = {f["name"] for f in functions if gap_start <= f["addr"] < gap_end}
                comps = connected_components(zone_names, adj)
                dominant = comps[0] if comps else set()
                dom_size = sum(size_of(functions, functions.index(by_name[n])) for n in dominant)
                link_counts = defaultdict(int)
                for n in zone_names:
                    for nb in adj.get(n, ()):
                        if nb in LANDMARK_DOMAINS and nb not in HUBS:
                            link_counts[nb] += 1
                print(f"    ... gap {gap:6d}B ({gap/1024:.1f} KB), {len(zone_names)} functions, "
                      f"{len(comps)} components, dominant={len(dominant)} fns/{dom_size}B, "
                      f"non-hub landmark links: {dict(link_counts)}")


if __name__ == "__main__":
    main()
