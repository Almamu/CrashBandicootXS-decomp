"""crashdata.text.po (gettext files edited in place) and the cutscene text
as the game draws it (crashdata.cutscenes.edit: DrawWrappedText's screens).

    python3 -m unittest discover -s packages/crashdata/tests
"""
import re
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "packages" / "crashdata" / "src"))

from crashdata import text as T  # noqa: E402
from crashdata.common.game import Game  # noqa: E402
from crashdata.cutscenes import edit  # noqa: E402
from crashdata.text import po  # noqa: E402

FIELD = re.compile(r'^(msgctxt|msgid|msgstr) (".*")$')


def fields(text):
    """Every msgctxt/msgid/msgstr of a gettext file: (keyword, its lines, its value)."""
    out = []
    for block in text.split("\n\n"):
        cur = None
        for line in block.split("\n"):
            m = FIELD.match(line)
            if m:
                cur = [m.group(1), [line]]
                out.append(cur)
            elif line.startswith('"') and cur:
                cur[1].append(line)
            else:
                cur = None
    return [(k, ls, "".join(T.po_unquote(re.match(r'^(?:\w+ )?"(.*)"$', x).group(1)) for x in ls)) for k, ls in out]


class Po(unittest.TestCase):
    def test_every_entry_is_written_as_msgcat_writes_it(self):
        """field_lines gives back every field of both games' files: XS's
        wrapped at 79 columns, N-Tranced's unwrapped (msgcat --no-wrap)."""
        for game, wrap in (("xs", True), ("n2", False)):
            for p in sorted((ROOT / "games" / game / "text").glob("*.po*")):
                for kw, lines, value in fields(p.read_text(encoding="utf-8")):
                    self.assertEqual(po.field_lines(kw, value, wrap), lines, (p.name, value))

    def test_a_file_edited(self):
        p = ROOT / "games/xs/text/fr.po"
        text = p.read_text(encoding="utf-8")
        f = po.PoFile(text)
        self.assertEqual(f.text(), text)
        self.assertFalse(f.set("cutscene01.page0.0"))  # nothing changes
        self.assertEqual(f.text(), text)
        f.set("cutscene01.page0.0", msgstr="bonjour")
        self.assertEqual(f.get("cutscene01.page0.0")["str"], "bonjour")
        changed = [a for a, b in zip(text.split("\n"), f.text().split("\n")) if a != b]
        self.assertEqual(len(f.text().split("\n")), len(text.split("\n")) - 1)  # two lines became one
        self.assertTrue(changed)
        f.insert_after("cutscene*", "cutscene11.page0.0", "hello", "bonjour", ["a comment"])
        keys = f.keys()
        self.assertEqual(keys[keys.index("cutscene11.page0.0") - 1][:9], "cutscene1")
        self.assertTrue(keys[keys.index("cutscene11.page0.0") + 1].startswith("credits."))
        f.remove("cutscene11.page0.0")
        f.set("cutscene01.page0.0", msgstr=po.PoFile(text).get("cutscene01.page0.0")["str"])
        self.assertEqual(f.text(), text)


class Screens(unittest.TestCase):
    def test_the_intro(self):
        """The intro's screens, as the PC port shows them (each key press:
        docs/editors.md, "The cutscene editor", has the comparison)."""
        steps = edit.cutscene_steps(Game("xs", ROOT), 1, "fr")
        self.assertEqual([(s, j, k) for s, j, k, _, _ in steps],
                         [(0, 0, 0), (1, 0, 0), (2, 0, 0), (2, 0, 1), (3, 0, 0), (3, 0, 1), (3, 1, 0)])

    def test_wrapping(self):
        files = edit.Files(Game("xs", ROOT))
        font = edit.TextFont.of(files)
        # most letters are 7 pixels (i 4, w 8), a space 4; 3 lines of 228 pixels
        one = edit.screens(b"abc def", font)
        self.assertEqual(len(one), 1)
        self.assertEqual([(g, x, y) for g, x, y in one[0]][:4], [(0, 7, 118), (1, 14, 118), (2, 21, 118), (3, 32, 118)])
        long = b" ".join([b"abcdefg"] * 40)  # 40 words of 49 + 4 pixels: 4 a line
        sc = edit.screens(long, font)
        self.assertEqual([len(s) for s in sc], [84, 84, 84, 28])
        self.assertEqual(sorted({y for _, _, y in sc[0]}), [118, 127, 136])
        # a word wider than the box goes on the next line, and overflows it
        wide = edit.screens(b"a" * 40, font)
        self.assertEqual({y for _, _, y in wide[0]}, {127})


if __name__ == "__main__":
    unittest.main()
