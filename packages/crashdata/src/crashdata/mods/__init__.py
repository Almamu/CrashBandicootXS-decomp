#!/usr/bin/env python3
"""Applies a mod to a copy of the tree (docs/modding.md).

    tools/apply_mod.py MOD DEST

copies the tree to DEST (the sources, data and built tools; not .git, the
build outputs or baserom.gba, which a mod's build doesn't need), copies
MOD/files/ over it, then runs MOD/mod.py's apply(tree) in DEST. Then build
DEST with a plain `make`. The tree itself is never changed: with the
original data it still builds the ROM `make compare` checks.

A mod is a folder:

- files/: files copied into the tree at the same paths, new or replacing;
- mod.py: `def apply(tree)`, the edits to the tree's data, with the
  helpers of the Tree class below (JSON edits, a level's name in the six
  languages, a copied room). Its own generated assets (a song, a picture)
  are written there too, so the mod stays small.
"""
import json
import re
import shutil
import subprocess
import sys
from contextlib import contextmanager
from pathlib import Path

from crashdata.common import game

ROOT = Path(game.ROOT).resolve()
LANGUAGES = ("fr", "de", "es", "it", "nl")

# What the copy leaves out: the git data, the build outputs, the ROM,
# CI's compiler checkouts (the built compilers are in tools/agbcc) and the
# Python workspace's environment (`uv sync`).
SKIP_TOP = {".git", "build", "agbcc", "agbcc-cp", ".claude", ".vscode", "objdiff-cli", "report.json", ".venv"}
SKIP_ANY = ["*.gba", "*.sav", "*.elf", "*.map", "*.o", "__pycache__"]


def copy_tree(dest):
    if dest.exists():
        sys.exit("apply_mod: %s exists" % dest)
    ignore_any = shutil.ignore_patterns(*SKIP_ANY)

    def ignore(d, names):
        out = set(ignore_any(d, names))
        if Path(d).resolve() == ROOT:
            out |= SKIP_TOP & set(names)
        return out
    shutil.copytree(ROOT, dest, ignore=ignore, symlinks=True)


def po_quote(s):
    return '"%s"' % s.replace("\\", "\\\\").replace('"', '\\"')


def po_entry(comments, key, english, text):
    return "".join("#. %s\n" % c for c in comments) + "msgctxt %s\nmsgid %s\nmsgstr %s\n" % (
        po_quote(key), po_quote(english), po_quote(text))


class Tree:
    """The copy a mod edits; paths are relative to its root."""

    def __init__(self, root, mod):
        self.root = Path(root)
        self.mod = Path(mod)

    def path(self, rel):
        return self.root / rel

    @contextmanager
    def json(self, rel):
        """Edits a JSON file: yields its data and writes it back."""
        p = self.path(rel)
        data = json.loads(p.read_text(encoding="utf-8"))
        yield data
        p.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    @contextmanager
    def levels(self):
        """Edits games/xs/data/levels/levels.json, written back in levels.py's layout."""
        from crashdata import levels as L
        data = L.read_json(L.LEVELS_JSON)
        yield data
        L.write_levels_json(data)

    def add_graphics(self, *entries):
        """Appends entries to games/xs/graphics/manifest.json's "assets", one per
        line as the manifest has them."""
        p = self.path("games/xs/graphics/manifest.json")
        lines = p.read_text(encoding="utf-8").split("\n")
        end = lines.index('  "objects": [') if '  "objects": [' in lines else len(lines)
        k = max(i for i, s in enumerate(lines[:end]) if s.startswith("    {"))
        for e in entries:
            if not lines[k].endswith(","):
                lines[k] += ","
            k += 1
            lines.insert(k, "    " + json.dumps(e, ensure_ascii=False))
        p.write_text("\n".join(lines), encoding="utf-8")

    def copy_room(self, src, dst, rename):
        """Copies the room folder games/xs/data/levels/<src> to <dst>, renaming the
        symbols of its room.json (docs/levels.md, "Adding a room")."""
        s, d = self.path("games/xs/data/levels") / src, self.path("games/xs/data/levels") / dst
        shutil.copytree(s, d)
        p = d / "room.json"
        text = p.read_text(encoding="utf-8")
        for old, new in rename.items():
            text = re.sub(r"\b%s(?=[A-Z0-9\"])" % re.escape(old), new, text)
        p.write_text(text, encoding="utf-8")

    def _text_files(self):
        return [("", self.path("games/xs/text/crashxs.pot"))] + [(c, self.path("games/xs/text/%s.po" % c)) for c in LANGUAGES]

    def _insert_after_last(self, prefix, entries):
        """Inserts each file's entry after the last entry whose key starts
        with `prefix`."""
        for code, p in self._text_files():
            s = p.read_text(encoding="utf-8")
            starts = [m.start() for m in re.finditer(r'(?m)^(#\..*\n)*msgctxt "%s' % re.escape(prefix), s)]
            if not starts:
                sys.exit("apply_mod: %s has no %s* entry" % (p, prefix))
            end = s.find("\n\n", starts[-1])
            end = len(s) if end < 0 else end + 2
            add = entries[code]
            p.write_text(s[:end] + add + ("\n" if end < len(s) else "") + s[end:], encoding="utf-8")

    def ui_count(self):
        s = self.path("games/xs/text/crashxs.pot").read_text(encoding="utf-8")
        return len(re.findall(r'(?m)^msgctxt "ui\.', s))

    def add_level_name(self, constant, names):
        """A level's name (docs/text.md, "A new level's name"): a
        ui.level.<level> entry after the last ui.* one, in the template
        and the five languages. names: {"en": ..., "fr": ..., ...}.
        Returns its text id, the level's "name_text"."""
        tid = self.ui_count()
        key = "ui.level." + constant[len("LEVEL_"):].lower()
        comments = ["Text id %d (0x%02X). A level's name (%s): the level select's title" % (tid, tid, constant),
                    "(large font) and the pause menu's info page."]
        self._insert_after_last("ui.", {code: po_entry(comments, key, names["en"], names[code] if code else "")
                                        for code, _ in self._text_files()})
        return tid

    def add_cutscene_text(self, key, texts, comment):
        """A cutscene page's string (cutsceneNN.pageP.I), after the last
        cutscene entry, in the template and the five languages."""
        self._insert_after_last("cutscene", {code: po_entry([comment], key, texts["en"], texts[code] if code else "")
                                             for code, _ in self._text_files()})

    def run(self, *cmd):
        subprocess.check_call(list(cmd), cwd=self.root)
