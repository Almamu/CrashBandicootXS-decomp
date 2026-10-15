"""The games of the tree and where their data is (#1167).

A Game is one game of the tree, by its name ("xs": Crash Bandicoot XS,
"n2": Crash Bandicoot 2: N-Tranced, docs/n_tranced.md), and the root of
the tree it is in ("." by default: the tools run in the tree's root).
Its directories follow from them, games/<name>/{data,graphics,sound,text}/
(docs/file_layout.md). crashdata's modules take one where they work on a
game's data (levels.use_game, graphics.manifest.use_game,
text.use_game), so an editor can open either game, or another tree.
"""
import os
from pathlib import Path

# The tree's root, from where this file is in it
# (packages/crashdata/src/crashdata/common/; the workspace installs the
# packages editable, so they stay in the tree).
ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), *[os.pardir] * 5))

# The games: their name, title and ROM.
GAMES = {"xs": "Crash Bandicoot XS", "n2": "Crash Bandicoot 2: N-Tranced"}
BASEROMS = {"xs": "baserom.gba", "n2": "baserom_n2.gba"}


class Game:
    """One game's data in a tree: `name` (a key of GAMES) under `root`."""

    def __init__(self, name="xs", root="."):
        if name not in GAMES:
            raise ValueError("unknown game %r (one of %s)" % (name, ", ".join(GAMES)))
        self.name = name
        self.root = Path(root)

    def __repr__(self):
        return "Game(%r, %r)" % (self.name, self.root.as_posix())

    def __eq__(self, other):
        return isinstance(other, Game) and (self.name, self.root) == (other.name, other.root)

    def __hash__(self):
        return hash((self.name, self.root))

    @property
    def title(self):
        return GAMES[self.name]

    @property
    def is_xs(self):
        return self.name == "xs"

    @property
    def dir(self):
        """games/<name>/: the game's data, its linker script and symbols."""
        return self.root / "games" / self.name

    @property
    def data_dir(self):
        return self.dir / "data"

    @property
    def levels_dir(self):
        return self.data_dir / "levels"

    @property
    def graphics_dir(self):
        return self.dir / "graphics"

    @property
    def sound_dir(self):
        return self.dir / "sound"

    @property
    def text_dir(self):
        return self.dir / "text"

    @property
    def baserom(self):
        return self.root / BASEROMS[self.name]


XS = Game("xs")
N2 = Game("n2")
