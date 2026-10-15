"""`make data-check`: every check of a game's data, without building (#1167, docs/editors.md).

For a game (XS or N-Tranced) in a tree (this one, or a mod's copy):

1. every data file against its JSON Schema (crashdata.schemas);
2. the model's own checks, the ones the build makes on its way: the
   rooms (their pictures' palettes, entity types, entity order, parameter
   indices, links), the level table, themes, actor categories, spawn
   lists, animation tables, bosses, what clearing a level does, the
   level-select worlds; the graphics manifest (`gfx_manifest.py check`);
   the cutscenes; the music and sound-effect tables; the text
   (`text.py check`, which needs gettext's msgfmt and msgcmp).

Each check runs on its own, so one failing doesn't hide the others; a
check that stops at its first problem gives that one. The result is a
list of Diagnostics (crashdata.common.diagnostics): errors fail it,
warnings don't.

    tools/data_check.py [--game xs|n2] [--json] [--no-text]
"""
import contextlib
import io
import os
import tempfile
from pathlib import Path

from crashdata import schemas
from crashdata.common import diagnostics as D
from crashdata.common.game import Game


def run_check(name, fn, out, quiet=True):
    """Run one check: its SystemExit (a DataError or an exit message), an
    assertion or another exception, and the warnings it gives, as
    Diagnostics appended to out, each coded `name` unless it has its own."""
    with D.collect() as warnings:
        buf = io.StringIO()
        try:
            with contextlib.redirect_stdout(buf) if quiet else contextlib.nullcontext():
                fn()
            got = []
        except SystemExit as e:
            got = D.from_exit(e)
        except AssertionError as e:
            got = [D.Diagnostic("internal check failed: %s" % (e or "assertion"), code=name)]
        except (KeyError, IndexError, TypeError, ValueError, OSError) as e:
            got = [D.Diagnostic("%s: %s" % (type(e).__name__, e), code=name)]
    for d in got + warnings:
        if d.code in ("error", "warning", "exit"):
            d.code = name
        out.append(d)
    return got


def level_checks(game, root):
    """The level data's checks: (name, function) pairs."""
    from crashdata import levels as L
    L.use_game(Game(game.name, root))
    data = L.read_json(L.LEVELS_JSON)
    checks = [("rooms", lambda: [L.check_room(L.LEVELS_DIR / r) for r in data["rooms"]])]
    if "levels" in data and "table" in data:
        checks.append(("level-table", lambda: L.LevelTable(data)))
    if game.is_xs:
        checks += [
            ("themes", lambda: L.level_themes(data)),
            ("categories", lambda: L.level_categories(data)),
            ("bosses", lambda: L.level_boss_kinds(data)),
            ("on-clear", lambda: L.level_on_clear(data)),
            ("worlds", lambda: L.level_select_worlds(data)),
            ("category-palettes", lambda: L.category_palette_warnings(data)),
            ("save", lambda: L.save_layout(data)),
        ]
    if L.TILESET_ORDER_JSON.exists():
        checks.append(("tilesets", L.load_tileset_order))
    return checks


def graphics_checks(game, root):
    from crashdata.graphics import manifest as M

    def check():
        M.use_game(Game(game.name, root))
        errors = M.check(Path(root))
        if errors:
            raise D.DataError([D.from_text(e) for e in errors])
    return [("graphics", check)]


def cutscene_checks(game, root):
    if not game.is_xs:
        return []
    from crashdata import cutscenes as C
    return [("cutscenes", C.load)]


def audio_checks(game, root):
    if not game.is_xs:
        return []
    from crashdata.audio import gax as G
    from crashdata.audio import sfx_table

    def music():
        path = os.path.join(G.SOUND_DIR, "gax_manifest.json")
        G.check_song_table(schemas.load(path), path)
        G.screen_songs(G.MUSIC_JSON, G.song_keys())

    def sfx():
        G.sfx_pool()
        with tempfile.TemporaryDirectory() as tmp:  # the names' checks
            sfx_table.write_constants(G.SFX_TABLE, os.path.join(tmp, "sfx.h"))
    return [("music", music), ("sfx", sfx)]


def text_checks(game, root):
    from crashdata import text as T

    def check():
        T.use_game(game.name)
        T.check()
    return [("text", check)]


def check_game(name="xs", root=".", text=True):
    """Every check of the game's data in the tree at root (the tree the
    tools run in, whose crashdata this is: the sound and text modules know
    no other): its Diagnostics."""
    root = Path(root)
    game = Game(name, root)
    out = schemas.validate_game(game, root)
    groups = [level_checks, graphics_checks, cutscene_checks, audio_checks]
    if text:
        groups.append(text_checks)
    for group in groups:
        try:
            checks = group(game, root)
        except SystemExit as e:
            out += D.from_exit(e)
            continue
        for check_name, fn in checks:
            run_check(check_name, fn, out)
    # a check that reads a file the schemas rejected stops with the same errors
    D.relativize(out, root)
    seen, unique = set(), []
    for d in out:
        key = (d.file, d.pointer, d.message, d.severity)
        if key not in seen:
            seen.add(key)
            unique.append(d)
    D.locate(unique, D.file_texts(unique))
    return unique
