"""crashdata.common.jsonfmt and crashdata.editing: a data file written back
in its own layout (docs/editors.md, "The editor's backend").

    python3 -m unittest discover -s packages/crashdata/tests
"""
import copy
import json
import os
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "packages" / "crashdata" / "src"))

from crashdata import editing, schemas  # noqa: E402
from crashdata.common import jsonfmt  # noqa: E402
from crashdata.common.game import GAMES, Game  # noqa: E402


def data_files():
    for name in GAMES:
        for p, schema in schemas.files(Game(name, ROOT), ROOT):
            yield p, schema


class Unchanged(unittest.TestCase):
    def test_every_data_file_gives_back_its_bytes(self):
        n = 0
        for p, schema in data_files():
            text = p.read_text(encoding="utf-8")
            value = json.loads(text)
            self.assertEqual(jsonfmt.dumps_like(text, value), text, p)
            self.assertEqual(editing.render(p, value, text, schema), text, p)
            n += 1
        self.assertGreater(n, 200)


class Changed(unittest.TestCase):
    def test_one_value_changes_one_line(self):
        p = ROOT / "games/xs/data/cutscenes.json"
        text = p.read_text()
        v = json.loads(text)
        v["cutscenes"][1]["slides"][2]["slide"] = "cortex_hologram"
        out = jsonfmt.dumps_like(text, v)
        self.assertEqual(json.loads(out), v)
        diff = [(a, b) for a, b in zip(text.splitlines(), out.splitlines()) if a != b]
        self.assertEqual(len(diff), 1)
        self.assertEqual(len(text.splitlines()), len(out.splitlines()))

    def test_a_new_item_looks_like_its_siblings(self):
        p = ROOT / "games/xs/data/levels/spawns/blimp_bonanza.json"
        text = p.read_text()
        v = json.loads(text)
        v["spawns"].insert(1, {"kind": "STOPWATCH", "x": 1, "y": 2, "z": 3})
        out = jsonfmt.dumps_like(text, v)
        self.assertEqual(json.loads(out), v)
        self.assertIn('\n    {"kind": "STOPWATCH", "x": 1, "y": 2, "z": 3},\n', out)
        self.assertEqual(len(out.splitlines()), len(text.splitlines()) + 1)

    def test_removed_and_moved_items(self):
        text = '{\n  "a": [\n    {"x": 1},\n    {"x": 2},\n    {"x": 3}\n  ],\n  "b": [1, 2, 3]\n}\n'
        v = json.loads(text)
        v["a"] = [v["a"][2], v["a"][0]]
        v["b"].append(4)
        out = jsonfmt.dumps_like(text, v)
        self.assertEqual(out, '{\n  "a": [\n    {"x": 3},\n    {"x": 1}\n  ],\n  "b": [1, 2, 3, 4]\n}\n')

    def test_new_keys_and_types(self):
        text = '{\n "a": 1,\n "b": {"c": [1, 2]}\n}'
        v = {"a": 1.5, "b": {"c": [1, 2], "d": {"e": "é"}}, "f": [{"g": None}]}
        out = jsonfmt.dumps_like(text, v)
        self.assertEqual(json.loads(out), v)
        self.assertTrue(out.startswith('{\n "a": 1.5,\n "b": {"c": [1, 2], "d": {"e": "\\u00e9"}},\n "f": [\n'))
        self.assertFalse(out.endswith("\n"))

    def test_same_is_strict(self):
        self.assertFalse(jsonfmt.same(1, 1.0))
        self.assertFalse(jsonfmt.same(1, True))
        self.assertFalse(jsonfmt.same({"a": 1, "b": 2}, {"b": 2, "a": 1}))
        self.assertTrue(jsonfmt.same({"a": [1, None]}, {"a": [1, None]}))


class ToolWriters(unittest.TestCase):
    def test_room_json_is_written_by_crashdata_levels(self):
        from crashdata import levels
        p = ROOT / "games/xs/data/levels/jungle_jam/main/room.json"
        text = p.read_text()
        v = json.loads(text)
        self.assertEqual(levels.room_json_text(v), text)
        v2 = copy.deepcopy(v)
        v2["entities"].append(dict(v2["entities"][0]))
        self.assertEqual(editing.render(p, v2, text), levels.room_json_text(v2))

    def test_levels_json_is_written_by_crashdata_levels(self):
        from crashdata import levels
        for game in GAMES:
            p = ROOT / "games" / game / "data/levels/levels.json"
            text = p.read_text()
            self.assertEqual(levels.levels_json_text(json.loads(text)), text, p)


if __name__ == "__main__":
    os.chdir(ROOT)
    unittest.main()
