"""The editors' backend: crashdata over HTTP, on 127.0.0.1 (#1167, docs/editors.md "The editor's backend").

    tools/editor_server.py [--port N] [--static DIR] [--token T] [--allow-origin O] [--watch-stdin]
                           [--open] [--verbose]
    make editor [EDITOR_ARGS=...]

serves the web editor (apps/editor's build, apps/editor/dist) and its
API, for the browser and for the desktop app (apps/desktop), which
starts it on its own port with a token. It binds 127.0.0.1 only. The
API (all JSON; docs/editors.md has each one's fields):

    GET  /api/health                       the server, the tree it serves
    GET  /api/projects                     XS, N-Tranced and the tree's mods (mods/*/mod.py)
    POST /api/projects                     {"mod": DIR}: another mod
    POST /api/projects/ID/apply            a mod: apply it to its tree (build/mods/NAME), as `make mod`
    GET  /api/schemas                      packages/crashdata/schemas/index.json
    GET  /api/schemas/NAME                 one schema
    GET  /api/projects/ID/files            the project's data files: [{path, schema}]
    GET  /api/projects/ID/file?path=P      one: {path, schema, data}
    GET  /api/projects/ID/raw?path=P       any file of the game's directory, as it is
    PUT  /api/projects/ID/file?path=P      {"data": ...}: save one (see below)
    POST /api/projects/ID/save             {"files": [{"path", "data"}, ...]}: save several at once
    GET  /api/projects/ID/check[?text=0]   `data-check --json`'s report
    POST /api/projects/ID/preview          {"level", "room", "headless", "frames"} or {"cutscene", "language",
                                           "headless"}: tools/preview.py
    GET  /api/projects/ID/preview.png      a headless preview's picture (?shot=N: a cutscene's, frame N's)

and the level editor's rooms (room, room/layer, room/tiles, room/save,
room/new, room/delete, room/resize, room/import, sprite), the cutscene
editor's cutscenes (cutscenes, cutscenes/picture, cutscenes/import, cutscenes/save) and the music
editor's songs and sound effects (music, music/pool, music/song,
music/patterns, music/import, music/save): docs/editors.md.

A save writes each file as crashdata.editing.render gives it (the tool's
writer, or the file's own layout: a value the file has gives back its
bytes), validates it with crashdata.schemas.load, then runs the model's
checks (tools/data_check.py --no-text in the project's tree): an error
that wasn't there before the save puts the files back as they were and
is the answer (422, its diagnostics). Paths are relative to the game's
directory's tree (games/<game>/...); one outside games/<game>/, or not a
data file the schemas describe, is refused.

A request that changes something needs the header `X-Crash-Editor: 1`
(a page of another site can't send it without asking first, which this
server never allows), and every request's Host must be this server's,
so another site can't reach it through its own name. With --token (or
CRASH_EDITOR_TOKEN), every /api request needs `Authorization: Bearer
TOKEN` (a GET may give `?token=TOKEN` instead: an <img>'s picture), and
the desktop app's pages (tauri://localhost, http(s)://tauri.localhost)
may call it.
"""
import argparse
import base64
import contextlib
import gzip
import hashlib
import http.server
import json
import mimetypes
import os
import re
import secrets
import shutil
import struct
import subprocess
import sys
import threading
import time
import urllib.parse
import webbrowser
from pathlib import Path

from crashdata import editing, schemas
from crashdata import levels as L
from crashdata.levels import room_edit, room_ops
from crashdata.cutscenes import edit as cutscene_edit
from crashdata.audio import edit as music_edit
from crashdata.common import diagnostics as D
from crashdata.common import jsonfmt
from crashdata.common.game import GAMES, ROOT, Game

API_VERSION = 1
DEFAULT_PORT = 8765
DESKTOP_ORIGINS = ("tauri://localhost", "http://tauri.localhost", "https://tauri.localhost")
CHECK_TIMEOUT = 600
SCHEMA_REJECTS = "the schema rejects it"
PREVIEW_TIMEOUT = 3600
# crashdata.levels binds its game in module state (levels.use_game): one
# request at a time works on it
LEVELS_LOCK = threading.Lock()


class HttpError(Exception):
    def __init__(self, status, message, diagnostics=None):
        super().__init__(message)
        self.status, self.message, self.diagnostics = status, message, diagnostics


# ---------------------------------------------------------------------------
# Projects: a game of the tree, or a mod's tree

class Project:
    """What the editor edits: a game's data in a tree. A mod's is the tree
    tools/apply_mod.py makes for it (build/mods/NAME), applied on demand."""

    def __init__(self, pid, kind, game, root, title, mod=None):
        self.id, self.kind, self.game, self.root, self.title, self.mod = pid, kind, game, Path(root), title, mod
        self.lock = threading.Lock()
        self.baseline = {}  # with the text's checks or not: (fingerprint, the model's diagnostics)

    @property
    def applied(self):
        return (self.root / "games" / self.game).is_dir()

    @property
    def game_dir(self):
        return self.root / "games" / self.game

    def as_dict(self, tree):
        out = {"id": self.id, "kind": self.kind, "game": self.game, "gameTitle": GAMES[self.game],
               "title": self.title, "root": _shown(self.root, tree), "ready": self.applied}
        if self.mod is not None:
            out["mod"] = _shown(self.mod, tree)
        return out


def _shown(p, tree):
    p = Path(p)
    try:
        return p.resolve().relative_to(Path(tree).resolve()).as_posix() or "."
    except ValueError:
        return p.as_posix()


class Server:
    def __init__(self, tree=ROOT, static=None, token=None, python=sys.executable, origins=()):
        self.tree = Path(tree).resolve()
        self.static = Path(static) if static else self.tree / "apps" / "editor" / "dist"
        self.token = token
        # with a token, the pages that may call it: the desktop app's, and those asked for
        self.origins = DESKTOP_ORIGINS + tuple(origins)
        self.python = python
        self.projects = {}
        self.tile_cache = {}  # (project, tile set): (fingerprint, {(room, layer): the answer})
        for name, title in GAMES.items():
            self.projects[name] = Project(name, "game", name, self.tree, title)
        mods = self.tree / "mods"
        for d in sorted(mods.iterdir()) if mods.is_dir() else ():
            if (d / "mod.py").is_file():
                self.add_mod(d)

    def add_mod(self, mod_dir):
        mod_dir = Path(mod_dir)
        if not mod_dir.is_absolute():
            mod_dir = self.tree / mod_dir
        mod_dir = mod_dir.resolve()
        if not (mod_dir / "mod.py").is_file():
            raise HttpError(400, "%s: not a mod (no mod.py; docs/modding.md, \"Writing a mod\")" % mod_dir)
        pid = "mod:" + mod_dir.name
        if pid not in self.projects:
            self.projects[pid] = Project(pid, "mod", "xs", self.tree / "build" / "mods" / mod_dir.name,
                                         "mod %s" % mod_dir.name, mod=mod_dir)
        return self.projects[pid]

    def project(self, pid, ready=True):
        p = self.projects.get(pid)
        if p is None:
            raise HttpError(404, "no project %r" % pid)
        if ready and not p.applied:
            raise HttpError(409, "%s isn't applied yet: POST /api/projects/%s/apply (make mod MOD=%s)"
                            % (p.title, pid, _shown(p.mod, self.tree)))
        return p

    # -- paths -------------------------------------------------------------

    def resolve(self, project, rel, data_file=True):
        """A path from the project's tree's root (games/<game>/...), checked
        to be in the game's directory: the file, and its schema."""
        if not rel or "\\" in rel or "\0" in rel or rel.startswith("/") or re.match(r"^[A-Za-z]:", rel):
            raise HttpError(400, "%r: give a path from the tree's root (games/%s/...)" % (rel, project.game))
        parts = rel.split("/")
        if any(x in ("", ".", "..") for x in parts):
            raise HttpError(400, "%r: no empty, '.' or '..' parts" % rel)
        base = project.game_dir.resolve()
        p = (project.root / rel).resolve()
        if not p.is_relative_to(base):
            raise HttpError(403, "%s: outside games/%s/ of %s" % (rel, project.game, project.title))
        schema = schemas.schema_for(rel)
        if data_file and schema is None:
            raise HttpError(403, "%s: not a data file the schemas describe (packages/crashdata/schemas/index.json)"
                            % rel)
        return p, schema

    # -- the API -------------------------------------------------------------

    def health(self):
        return {"ok": True, "version": API_VERSION, "tree": str(self.tree), "python": sys.version.split()[0],
                "editor": (self.static / "index.html").is_file()}

    def list_projects(self):
        return {"projects": [p.as_dict(self.tree) for p in self.projects.values()]}

    def schema_index(self):
        return json.loads((self.tree / "packages" / "crashdata" / "schemas" / "index.json").read_text())

    def schema(self, name):
        if not re.fullmatch(r"[a-z0-9_]+\.schema\.json", name):
            raise HttpError(404, "no schema %r" % name)
        p = self.tree / "packages" / "crashdata" / "schemas" / name
        if not p.is_file():
            raise HttpError(404, "no schema %r" % name)
        return json.loads(p.read_text())

    def files(self, project):
        out = []
        for p, name in schemas.files(Game(project.game, project.root), project.root):
            out.append({"path": p.relative_to(project.root).as_posix(), "schema": name})
        return {"files": out}

    def read(self, project, rel):
        p, schema = self.resolve(project, rel)
        if not p.is_file():
            raise HttpError(404, "%s: no such file" % rel)
        try:
            data = D.read_json(p)
        except D.DataError as e:
            raise HttpError(422, "%s isn't JSON" % rel, _diags(e.diagnostics, rel))
        return {"path": rel, "schema": schema, "data": data}

    def raw(self, project, rel):
        p, _ = self.resolve(project, rel, data_file=False)
        if not p.is_file():
            raise HttpError(404, "%s: no such file" % rel)
        return p

    def model_check(self, project, text=False):
        """tools/data_check.py --json in the project's tree: its report."""
        cmd = [self.python, "tools/data_check.py", "--game", project.game, "--json"]
        if not text:
            cmd.append("--no-text")
        r = subprocess.run(cmd, cwd=project.root, capture_output=True, text=True, timeout=CHECK_TIMEOUT)
        try:
            report = json.loads(r.stdout)
        except json.JSONDecodeError:
            raise HttpError(500, "data_check.py gave no report (exit %d): %s" % (r.returncode, r.stderr[-2000:]))
        return report

    def fingerprint(self, project):
        """The data files', the rooms' and cutscenes' pictures', the text's and
        cxx_symbols.txt's sizes and times: the model's checks are run again
        when it changes."""
        h = hashlib.sha1()
        game = Game(project.game, project.root)
        files = ([p for p, _ in schemas.files(game, project.root)] + sorted(game.levels_dir.glob("*/*/*.png"))
                 + sorted((game.graphics_dir / "cutscenes").glob("*.png")) + sorted(game.text_dir.glob("*.po*"))
                 + sorted(game.sound_dir.glob("*/*.xm")) + sorted(game.sound_dir.glob("*/*.wav"))
                 + [p for p in [project.root / "cxx_symbols.txt"] if p.is_file()])
        for p in files:
            st = p.stat()
            h.update(("%s %d %d\n" % (p, st.st_mtime_ns, st.st_size)).encode())
        return h.hexdigest()

    def baseline(self, project, text=False):
        fp = self.fingerprint(project)
        if project.baseline.get(text, (None,))[0] != fp:
            project.baseline[text] = (fp, self.model_check(project, text)["diagnostics"])
        return project.baseline[text][1]

    def save(self, project, items):
        """Save files ([(path, value)]): rendered, schema-validated, then the
        model's checks; all saved or none."""
        if not isinstance(items, list) or not items:
            raise HttpError(400, "nothing to save")
        with project.lock:
            staged, diags = [], []
            try:
                for rel, value in items:
                    try:
                        staged.append(self._stage_json(project, rel, value))
                    except HttpError as e:
                        if e.message != SCHEMA_REJECTS:
                            raise
                        diags += e.diagnostics
                if diags:
                    raise HttpError(422, SCHEMA_REJECTS, diags)
            except BaseException:
                self._unstage(staged)
                raise
            return self._commit(project, staged)

    def _stage_json(self, project, rel, value):
        """A data file's new text in a temporary file next to it, checked
        against its schema: (rel, path, tmp, old bytes), or None when it
        would be the same text."""
        p, schema = self.resolve(project, rel)
        old = p.read_bytes() if p.is_file() else None
        old_text = old.decode("utf-8") if old is not None else None
        text = editing.render(p, value, old_text, schema)
        if not jsonfmt.same(json.loads(text), value):
            raise HttpError(500, "%s: the writer didn't give back the value" % rel)
        if old_text == text:
            return None
        if not p.parent.is_dir():
            raise HttpError(400, "%s: its directory doesn't exist" % rel)
        tmp = p.with_name(".%s.editor-%s.tmp" % (p.name, secrets.token_hex(4)))
        tmp.write_text(text, encoding="utf-8")
        try:
            schemas.load(tmp, schema)
        except D.DataError as e:
            tmp.unlink()
            raise HttpError(422, SCHEMA_REJECTS, _diags(e.diagnostics, rel))
        return (rel, p, tmp, old)

    @staticmethod
    def _stage_bytes(rel, p, data):
        """Another file's new bytes (a room's picture), staged the same way;
        None when they are its bytes."""
        old = p.read_bytes() if p.is_file() else None
        if old == data:
            return None
        tmp = p.with_name(".%s.editor-%s.tmp" % (p.name, secrets.token_hex(4)))
        tmp.write_bytes(data)
        return (rel, p, tmp, old)

    @staticmethod
    def _stage_delete(rel, p):
        """A file to remove (a deleted room's), staged: its bytes are kept to
        put it back."""
        return (rel, p, None, p.read_bytes()) if p.is_file() else None

    @staticmethod
    def _unstage(staged):
        for item in staged:
            if item is not None:
                if item[2] is not None:  # a file deleted has nothing staged
                    item[2].unlink(missing_ok=True)

    def _commit(self, project, staged, text=False, check=None):
        """Put the staged files in place and run the model's checks (and the
        text's, with `text`; then `check()`, which gives back errors or
        raises an HttpError): an error that wasn't there before puts every
        file back. Under project.lock."""
        plan = [x for x in staged if x is not None]
        if not plan:
            return {"ok": True, "saved": [], "diagnostics": []}
        try:
            before = self.baseline(project, text)
        except BaseException:
            self._unstage(plan)
            raise
        for _, p, tmp, _ in plan:
            if tmp is None:  # a file the save deletes (its old bytes are kept to put back)
                p.unlink()
            else:
                os.replace(tmp, p)
        try:
            after = self.model_check(project, text)["diagnostics"]
        except BaseException:
            self._restore(plan)
            raise
        seen = {_key(d) for d in before}
        new = [d for d in after if d["severity"] == D.ERROR and _key(d) not in seen]
        if not new and check is not None:
            try:
                new = check()
            except BaseException:
                self._restore(plan)
                raise
        if new:
            self._restore(plan)
            raise HttpError(422, "the model's checks reject it", new)
        project.baseline = {text: (self.fingerprint(project), after)}
        return {"ok": True, "saved": [rel for rel, _, _, _ in plan], "diagnostics": after}

    @staticmethod
    def _restore(plan):
        for _, p, _, old in plan:
            p.parent.mkdir(parents=True, exist_ok=True)
            if old is None:
                p.unlink(missing_ok=True)
            else:
                p.write_bytes(old)

    def apply_mod(self, project):
        if project.kind != "mod":
            raise HttpError(400, "%s isn't a mod" % project.title)
        with project.lock:
            if project.root.exists():
                shutil.rmtree(project.root)
            r = subprocess.run([self.python, "tools/apply_mod.py", "--json", str(project.mod), str(project.root)],
                               cwd=self.tree, capture_output=True, text=True, timeout=CHECK_TIMEOUT)
            project.baseline = {}
            try:
                report = json.loads(r.stdout)
            except json.JSONDecodeError:
                report = {"ok": r.returncode == 0, "diagnostics": [], "output": r.stdout + r.stderr}
            if r.returncode != 0:
                raise HttpError(422, "the mod didn't apply", report.get("diagnostics"))
            return {"ok": True, "project": project.as_dict(self.tree), "report": report}

    def preview(self, project, body):
        if body.get("cutscene") not in (None, ""):
            return self.preview_cutscene(project, body)
        level = body.get("level")
        if not isinstance(level, (str, int)) or str(level) == "":
            raise HttpError(400, "give a level (an index, a constant, a name or a folder)")
        cmd = [self.python, "tools/preview.py", str(level)]
        if body.get("room") not in (None, ""):
            cmd.append(str(body["room"]))
        cmd.append("--json")
        if body.get("noBuild"):
            cmd.append("--no-build")
        out_png = project.root / "build" / "editor" / "preview.png"
        if body.get("headless"):
            out_png.parent.mkdir(parents=True, exist_ok=True)
            out_png.unlink(missing_ok=True)
            cmd += ["--headless", "--out", str(out_png)]
            if body.get("frames"):
                cmd += ["--frames", str(int(body["frames"]))]
            r = subprocess.run(cmd, cwd=project.root, capture_output=True, text=True, timeout=PREVIEW_TIMEOUT)
            try:
                report = json.loads(r.stdout)
            except json.JSONDecodeError:
                report = {"ok": False, "diagnostics": [], "output": r.stdout + r.stderr}
            report["image"] = ("/api/projects/%s/preview.png?t=%d" % (urllib.parse.quote(project.id), time.time())
                               if out_png.is_file() else None)
            return report
        # in a window: the game runs on its own, the answer doesn't wait for it
        log = project.root / "build" / "editor" / "preview.log"
        log.parent.mkdir(parents=True, exist_ok=True)
        with open(log, "w") as f:
            proc = subprocess.Popen(cmd, cwd=project.root, stdout=f, stderr=subprocess.STDOUT,
                                    start_new_session=True)
        return {"ok": True, "started": True, "pid": proc.pid, "command": cmd[1:],
                "log": _shown(log, project.root)}

    def preview_cutscene(self, project, body):
        """tools/preview.py --cutscene: in the window, or headless with a
        picture per screen (A pressed every 60 frames)."""
        cmd = [self.python, "tools/preview.py", "--cutscene", str(body["cutscene"]), "--json"]
        if body.get("language") not in (None, ""):
            cmd += ["--language", str(body["language"])]
        if body.get("noBuild"):
            cmd.append("--no-build")
        if not body.get("headless"):
            log = project.root / "build" / "editor" / "preview.log"
            log.parent.mkdir(parents=True, exist_ok=True)
            with open(log, "w") as f:
                proc = subprocess.Popen(cmd, cwd=project.root, stdout=f, stderr=subprocess.STDOUT,
                                        start_new_session=True)
            return {"ok": True, "started": True, "pid": proc.pid, "command": cmd[1:],
                    "log": _shown(log, project.root)}
        shots = project.root / "build" / "editor" / "cutscene"
        if shots.exists():
            shutil.rmtree(shots)
        shots.mkdir(parents=True)
        cmd += ["--headless", "--screens", str(shots)]
        r = subprocess.run(cmd, cwd=project.root, capture_output=True, text=True, timeout=PREVIEW_TIMEOUT)
        try:
            report = json.loads(r.stdout)
        except json.JSONDecodeError:
            report = {"ok": False, "diagnostics": [], "output": r.stdout + r.stderr}
        t = int(time.time())
        report["images"] = ["/api/projects/%s/preview.png?shot=%s&t=%d" % (urllib.parse.quote(project.id), p.stem, t)
                            for p in sorted(shots.glob("*.png"))]
        report["image"] = report["images"][0] if report["images"] else None
        return report

    def preview_png(self, project, shot=None):
        if shot is None:
            p = project.root / "build" / "editor" / "preview.png"
        elif re.fullmatch(r"\d{1,9}", shot):
            p = project.root / "build" / "editor" / "cutscene" / ("%s.png" % shot)
        else:
            raise HttpError(400, "a shot is a frame number")
        if not p.is_file():
            raise HttpError(404, "no preview picture yet")
        return p.read_bytes()

    # -- the cutscene editor ---------------------------------------------------

    @contextlib.contextmanager
    def cutscene_errors(self):
        try:
            yield
        except cutscene_edit.EditError as e:
            raise HttpError(422, str(e), e.diagnostics)
        except D.DataError as e:
            raise HttpError(422, str(e), _diags(e.diagnostics, None))
        except SystemExit as e:
            raise HttpError(422, str(e.code if e.code is not None else e))

    def cutscene_game(self, project):
        game = Game(project.game, project.root)
        if not cutscene_edit.available(game):
            raise HttpError(404, "%s's cutscenes aren't data yet (no games/%s/data/cutscenes.json)"
                            % (GAMES[project.game], project.game))
        return game

    def cutscenes(self, project):
        game = self.cutscene_game(project)
        with self.cutscene_errors():
            return cutscene_edit.view(game)

    def cutscene_picture(self, project, path):
        """A cutscene picture: its width and height (u32 LE), its 256
        colours (u16 LE, BGR555), then a byte a pixel."""
        game = self.cutscene_game(project)
        with self.cutscene_errors():
            w, h, pal, px = cutscene_edit.picture(game, path or "")
        return struct.pack("<II256H", w, h, *pal) + px

    def cutscene_import(self, project, body):
        self.cutscene_game(project)
        png = body.get("png")
        if not isinstance(png, str):
            raise HttpError(400, "give the picture as {\"png\": base64}")
        try:
            data = base64.b64decode(png, validate=True)
        except ValueError:
            raise HttpError(400, "the picture isn't base64")
        with self.cutscene_errors():
            return cutscene_edit.import_picture(data, body.get("palette"))

    def save_cutscenes(self, project, body):
        """The cutscene editor's save, all or none (crashdata.cutscenes.edit):
        {"cutscenes": cutscenes.json's value, "pages": {page: [{en, fr, ...}] or
        null}, "pictures": {path: {palette, pixels}}, "newPictures": [{path,
        about, palette, pixels}]}, each optional. The model's checks and, when
        the text changes, the text's (msgfmt, msgcmp, the generator's)."""
        game = self.cutscene_game(project)
        if not isinstance(body, dict):
            raise HttpError(400, "give the edits as an object")
        with project.lock:
            with self.cutscene_errors():
                plan = cutscene_edit.plan_save(game, body)
            staged = []
            try:
                for rel, value in plan["json"]:
                    staged.append(self._stage_json(project, rel, value))
                for rel, data in plan["bytes"]:
                    if rel == "cxx_symbols.txt":
                        p = project.root / rel
                    else:
                        p, _ = self.resolve(project, rel, data_file=False)
                    staged.append(self._stage_bytes(rel, p, data))
            except BaseException:
                self._unstage(staged)
                raise
            text = any(x is not None and x[0].startswith("games/%s/text/" % project.game) for x in staged)
            return self._commit(project, staged, text=text)

    # -- the music editor ------------------------------------------------------

    @contextlib.contextmanager
    def music_errors(self):
        try:
            yield
        except music_edit.EditError as e:
            raise HttpError(422, str(e), e.diagnostics)
        except D.DataError as e:
            raise HttpError(422, str(e), _diags(e.diagnostics, None))
        except SystemExit as e:
            raise HttpError(422, str(e.code if e.code is not None else e))

    def music_game(self, project):
        game = Game(project.game, project.root)
        if not music_edit.available(game):
            raise HttpError(404, "%s's sound isn't data yet (no games/%s/sound/)" % (GAMES[project.game], project.game))
        return game

    def music(self, project):
        game = self.music_game(project)
        with self.music_errors():
            return music_edit.view(game)

    @staticmethod
    def music_unsaved(body):
        """The songs' unsaved edits a player request carries (newSongs,
        structures, patterns, settings, the pools' edits:
        music_edit.unsaved_songs)."""
        keys = ("newSongs", "songInstruments", "structures", "patterns", "songSettings", "instruments", "samples",
                "newSamples", "deleteSamples", "deleteInstruments", "sfxInstruments")
        return {k: body[k] for k in keys if k in body} if body else {}

    def music_pool(self, project, body=None):
        game = self.music_game(project)
        with self.music_errors():
            return music_edit.player_pool(game, self.music_unsaved(body))

    def music_song(self, project, key, edits=None, body=None):
        game = self.music_game(project)
        if not isinstance(key, str) or not key:
            raise HttpError(400, "which song? (?key=KEY, or {\"key\": KEY, \"edits\": [...]})")
        with self.music_errors():
            return music_edit.player_song(game, key, edits, self.music_unsaved(body))

    def music_patterns(self, project, key, xm=None):
        game = self.music_game(project)
        with self.music_errors():
            return music_edit.patterns(game, key or "", xm)

    def music_import(self, project, body):
        self.music_game(project)
        wav = body.get("wav")
        if not isinstance(wav, str):
            raise HttpError(400, "give the sound as {\"wav\": base64, \"rate\": Hz}")
        try:
            data = base64.b64decode(wav, validate=True)
        except ValueError:
            raise HttpError(400, "the sound isn't base64")
        with self.music_errors():
            return music_edit.import_wav(data, body.get("rate"))

    def save_music(self, project, body):
        """The music editor's save, all or none (crashdata.audio.edit.plan_save):
        the sound-effect table, the screens' and themes' songs, the song
        table, the songs' cells, the samples and new instruments; then the
        model's checks, which build the music and the sound effects as the
        build does (data-check's music-build and sfx-build)."""
        game = self.music_game(project)
        if not isinstance(body, dict):
            raise HttpError(400, "give the edits as an object")
        with project.lock:
            with self.music_errors():
                plan = music_edit.plan_save(game, body)
            # a new instrument's folder (games/xs/sound/instruments/: the ROM has none)
            made = []
            for rel, _ in plan["json"] + plan["bytes"]:
                d = (project.root / rel).parent
                if d.resolve().is_relative_to(game.sound_dir.resolve()) and not d.exists():
                    d.mkdir(parents=True)
                    made.append(d)
            staged = []
            try:
                for rel, value in plan["json"]:
                    staged.append(self._stage_json(project, rel, value))
                for rel, data in plan["bytes"]:
                    p, _ = self.resolve(project, rel, data_file=False)
                    staged.append(self._stage_bytes(rel, p, data))
                # a music pool sample taken out: nothing staged, its bytes kept to put back
                for rel in plan.get("delete", ()):
                    p, _ = self.resolve(project, rel, data_file=False)
                    staged.append((rel, p, None, p.read_bytes()))
                return self._commit(project, staged)
            except BaseException:
                self._unstage(staged)
                for d in reversed(made):
                    if d.is_dir() and not any(d.iterdir()):
                        d.rmdir()
                raise

    # -- the level editor: rooms ---------------------------------------------

    @contextlib.contextmanager
    def levels(self, project):
        """crashdata.levels bound to the project's game (module state, so one
        request at a time); its errors are 422s."""
        with LEVELS_LOCK:
            L.use_game(Game(project.game, project.root))
            try:
                yield
            except room_edit.EditError as e:
                raise HttpError(422, str(e))
            except D.DataError as e:
                raise HttpError(422, str(e), _diags(e.diagnostics, None))
            except SystemExit as e:
                raise HttpError(422, str(e.code if e.code is not None else e))

    def room_dir(self, project, rel):
        """A room's folder (games/<game>/data/levels/<level>/<slot>): its path,
        its key (<level>/<slot>)."""
        rel = (rel or "").rstrip("/")
        p, schema = self.resolve(project, rel + "/room.json")
        if schema != "room.schema.json":
            raise HttpError(400, "%s isn't a room's folder (games/%s/data/levels/<level>/<slot>)" % (rel, project.game))
        if not p.is_file():
            raise HttpError(404, "%s: no room.json" % rel)
        return p.parent, p.parent.relative_to(Game(project.game, project.root).levels_dir.resolve()).as_posix()

    def room_layer(self, project, d, lname):
        room = D.read_json(d / "room.json")
        layer = (room.get("layers") or {}).get(lname)
        if not isinstance(layer, dict):
            raise HttpError(404, "the room has no layer %r" % lname)
        p = (d / layer["map"]).resolve()
        if not p.is_relative_to(d.resolve()) or not p.is_file():
            raise HttpError(404, "%s: the layer's picture %s isn't in the room's folder" % (lname, layer["map"]))
        return room, p, room_edit.layer_info(room)[lname]

    def room(self, project, rel):
        d, _ = self.room_dir(project, rel)
        game = Game(project.game, project.root)
        with self.levels(project):
            view = room_edit.room_view(d, project.game, game.graphics_dir)
        view["path"] = rel.rstrip("/")
        return view

    def room_layer_data(self, project, rel, lname):
        """A layer's picture's indices (BG) or cells (collision): 8 bytes of
        width and height (u32 LE), then a byte per pixel or a u16 per cell."""
        d, _ = self.room_dir(project, rel)
        with self.levels(project):
            _, p, info = self.room_layer(project, d, lname)
            if lname == "collision":
                w, h, data = room_edit.collision_cells(p)
            else:
                w, h, data = room_edit.layer_pixels(p, info)
        return struct.pack("<II", w, h) + data

    def room_tiles(self, project, rel, lname):
        """The tile set a BG layer names (the build's, from its rooms'
        pictures) and the layer's cells in it; cached until a room's data
        changes."""
        d, key = self.room_dir(project, rel)
        with self.levels(project):
            _, _, info = self.room_layer(project, d, lname)
            if info["tileset"] is None:
                raise HttpError(400, "%s has no tile set" % lname)
            fp = self.fingerprint(project)
            cache = self.tile_cache.get((project.id, info["tileset"]))
            if cache is None or cache[0] != fp:
                cache = (fp, {})
                self.tile_cache[(project.id, info["tileset"])] = cache
            if (key, lname) not in cache[1]:
                bpp, tiles, banks, cells, uses, limit = room_edit.tileset(info["tileset"], key, lname)
                cache[1][(key, lname)] = {
                    "tileset": info["tileset"], "bpp": bpp, "count": len(tiles) // 64,
                    "tiles": base64.b64encode(tiles).decode("ascii"), "banks": banks,
                    "cells": base64.b64encode(cells).decode("ascii"), "uses": uses, "limit": limit}
            return cache[1][(key, lname)]

    def sprite(self, project, bank, frame, palette=None):
        """A sprite bank's frame, index 0 transparent (in OBJ palette
        `palette`, else its own): PNG bytes."""
        try:
            bank, frame = int(bank), int(frame)
            palette = int(palette) if palette not in (None, "") else None
        except (TypeError, ValueError):
            raise HttpError(400, "bank, frame and palette are numbers")
        game = Game(project.game, project.root)
        banks = room_edit.sprite_banks(game.graphics_dir)
        if bank not in banks or not (banks[bank] / ("f%03d.png" % frame)).is_file():
            raise HttpError(404, "no frame %d in sprite bank %d" % (frame, bank))
        pal = None
        if palette is not None:
            found = sorted((game.graphics_dir / "sprites" / "palettes").glob("%03d_*.pal" % palette))
            if not found:
                raise HttpError(404, "no OBJ palette %d" % palette)
            pal = found[0]
        with self.levels(project):
            return room_edit.sprite_png(banks[bank], frame, pal)

    def save_room(self, project, body):
        """A room's edits, saved together, all or none
        (crashdata.levels.room_edit): {"path": its folder, "room":
        room.json's value, "layers": {"bg1": [[cx, cy, base64 of the 64
        pixels], ...]}, "collision": [[cx, cy, value], ...], "palette": [256
        BGR555 entries]}, each but "path" optional."""
        rel = body.get("path")
        if not isinstance(rel, str):
            raise HttpError(400, "give the room's folder as \"path\"")
        rel = rel.rstrip("/")
        d, _ = self.room_dir(project, rel)
        layers = body.get("layers") or {}
        if not isinstance(layers, dict):
            raise HttpError(400, "\"layers\" is {layer: [[cx, cy, pixels], ...]}")
        palette = body.get("palette")
        collision = body.get("collision") or []
        if not isinstance(collision, list):
            raise HttpError(400, "\"collision\" is [[cx, cy, value], ...]")
        with project.lock:
            staged = []
            try:
                if "room" in body:
                    staged.append(self._stage_json(project, rel + "/room.json", body["room"]))
                with self.levels(project):
                    room = D.read_json(d / "room.json")
                    for lname in layers:
                        if lname not in room_edit.BG_LAYERS or lname not in room["layers"]:
                            raise HttpError(400, "the room has no BG layer %r" % lname)
                    for lname in room_edit.BG_LAYERS:
                        if lname not in room["layers"] or (lname not in layers and palette is None):
                            continue
                        _, p, info = self.room_layer(project, d, lname)
                        cells = [[c[0], c[1], _b64(c[2])] if isinstance(c, list) and len(c) == 3 else c
                                 for c in layers.get(lname) or []]
                        data = room_edit.patch_layer(p, info, lname, cells, palette)
                        staged.append(self._stage_bytes(_rel(p, project), p, data))
                    if collision:
                        _, p, info = self.room_layer(project, d, "collision")
                        data = room_edit.patch_collision(p, info, collision)
                        staged.append(self._stage_bytes(_rel(p, project), p, data))
            except BaseException:
                self._unstage(staged)
                raise
            sets = {room["layers"][n]["tileset"] for n in layers if n in room["layers"]}
            return self._commit(project, staged, check=(lambda: self.tileset_check(project, d, sets)) if sets else None)

    def tileset_check(self, project, room_dir, sets):
        """After a room's painted cells are in place: the tile sets they name
        as the build will make them (room_edit.tileset_errors), as
        diagnostics."""
        with self.levels(project):
            errors = room_edit.tileset_errors(sets)
        rel = _rel(room_dir / "room.json", project)
        return [{"file": rel, "pointer": "/layers", "line": None, "col": None, "severity": D.ERROR,
                 "code": "tileset", "message": e} for e in errors]

    def import_tiles(self, project, body):
        """POST room/import {"path", "layer", "png": base64, "palette": the
        room's 256 colours (as edited), "banks": [BG1-3 banks to use]}: the
        picture as the layer's cells (room_edit.import_tiles): {cols, rows,
        cells: base64 of cols * rows * 64 palette indices, stats}."""
        d, key = self.room_dir(project, body.get("path"))
        lname = body.get("layer")
        if lname not in room_edit.BG_LAYERS:
            raise HttpError(400, "import into a BG layer (bg0-bg3)")
        png = body.get("png")
        if not isinstance(png, str):
            raise HttpError(400, "give the picture as {\"png\": base64}")
        try:
            data = base64.b64decode(png, validate=True)
        except ValueError:
            raise HttpError(400, "the picture isn't base64")
        tiles = self.room_tiles(project, body.get("path"), lname)
        with self.levels(project):
            room = D.read_json(d / "room.json")
            palette = body.get("palette") or list(L.room_palette(d, room))
            cols, rows, cells, stats = room_edit.import_tiles(data, palette, tiles["bpp"],
                                                              base64.b64decode(tiles["tiles"]), body.get("banks"))
        stats.update(setCount=tiles["count"], limit=tiles["limit"])
        return {"cols": cols, "rows": rows, "cells": base64.b64encode(cells).decode("ascii"), "stats": stats}

    # -- the level editor: new, deleted and resized rooms ---------------------

    def commit_room_plan(self, project, plan):
        """A crashdata.levels.room_ops plan ({json: [(path, value)], bytes:
        [(path, data)], delete: [path], dirs: [path], report}) saved as one
        save, all or none: the new folders made, every file staged
        (written, schema-checked), then the model's checks; on a refusal the
        files go back, and the folders it made go."""
        root = project.root.resolve()
        game_dir = project.game_dir.resolve()

        def rel_of(p):
            p = Path(p)
            p = (p if p.is_absolute() else project.root / p).resolve()
            if not (p.is_relative_to(game_dir) or p == root / "cxx_symbols.txt"):
                raise HttpError(403, "%s: outside games/%s/" % (p, project.game))
            return p, p.relative_to(root).as_posix()

        made, staged = [], []
        try:
            for d in plan["dirs"]:
                d, _ = rel_of(d)
                if not d.exists():
                    for x in reversed([d] + [q for q in d.parents if q.is_relative_to(game_dir)]):
                        if not x.exists():
                            x.mkdir()
                            made.append(x)
            for p, value in plan["json"]:
                staged.append(self._stage_json(project, rel_of(p)[1], value))
            for p, data in plan["bytes"]:
                p, rel = rel_of(p)
                staged.append(self._stage_bytes(rel, p, data))
            for p in plan["delete"]:
                p, rel = rel_of(p)
                staged.append(self._stage_delete(rel, p))
            result = self._commit(project, staged)
        except BaseException:
            self._unstage(staged)
            for d in reversed(made):
                if d.is_dir() and not any(d.iterdir()):
                    d.rmdir()
            raise
        # a deleted room's folder, and its level's when it was the last
        for p in plan["delete"][:1]:
            d, _ = rel_of(Path(p).parent)
            for x in (d, d.parent):
                if x.is_dir() and x != game_dir and x.is_relative_to(game_dir) and not any(x.iterdir()):
                    x.rmdir()
        result["report"] = plan["report"]
        return result

    def new_room(self, project, body):
        """POST room/new {"from": a room's folder, "folder": its new folder
        (<level>/<slot>), "empty": bool, "size": [w, h] cells, "attach":
        {"level", "as", "index"}}: the new room, saved."""
        base = "games/%s/data/levels/" % project.game
        src, dst = (str(body.get(k) or "") for k in ("from", "folder"))
        src, dst = (x[len(base):] if x.startswith(base) else x for x in (src, dst))
        with project.lock:
            with self.levels(project):
                plan = room_ops.new_room(src, dst, bool(body.get("empty")), body.get("size"), body.get("attach"))
            out = self.commit_room_plan(project, plan)
        out["path"] = base + dst
        return out

    def delete_room(self, project, body):
        """POST room/delete {"path"}: a room no level plays, deleted."""
        _, key = self.room_dir(project, body.get("path"))
        with project.lock:
            with self.levels(project):
                plan = room_ops.delete_room(key)
            return self.commit_room_plan(project, plan)

    def resize_room(self, project, body):
        """POST room/resize {"path", "edges": {left, right, top, bottom},
        "check": bool}: the room resized (with "check", only what it would
        change and cut: nothing is written)."""
        _, key = self.room_dir(project, body.get("path"))
        with project.lock:
            with self.levels(project):
                plan = room_ops.resize_room(key, body.get("edges") or {})
            if body.get("check"):
                return {"ok": True, "saved": [], "diagnostics": [], "report": plan["report"]}
            return self.commit_room_plan(project, plan)


def _b64(v):
    if not isinstance(v, str):
        raise HttpError(400, "a cell's pixels are base64")
    try:
        return base64.b64decode(v, validate=True)
    except ValueError:
        raise HttpError(400, "a cell's pixels aren't base64")


def _rel(p, project):
    return p.relative_to(project.root.resolve()).as_posix()


def _key(d):
    return (d.get("file"), d.get("code"), d.get("message"))


def _diags(diags, rel):
    out = []
    for d in diags:
        x = d.as_dict() if isinstance(d, D.Diagnostic) else dict(d)
        if x.get("file") is None or ".editor-" in str(x.get("file")):
            x["file"] = rel
        out.append(x)
    return out


# ---------------------------------------------------------------------------
# HTTP

class Handler(http.server.BaseHTTPRequestHandler):
    server_version = "crash-editor/%d" % API_VERSION
    protocol_version = "HTTP/1.1"
    app = None  # the Server

    def log_message(self, fmt, *args):
        if self.server.verbose:
            sys.stderr.write("editor-server: %s\n" % (fmt % args))

    # -- the checks every request passes ---------------------------------------

    def _origin_ok(self):
        origin = self.headers.get("Origin")
        if origin is None:
            return None
        port = self.server.server_address[1]
        own = ("http://127.0.0.1:%d" % port, "http://localhost:%d" % port)
        if origin in own or (self.app.token and origin in self.app.origins):
            return origin
        return False

    def _host_ok(self):
        host = (self.headers.get("Host") or "").lower()
        port = self.server.server_address[1]
        return host in ("127.0.0.1:%d" % port, "localhost:%d" % port)

    def _cors(self):
        origin = self._origin_ok()
        if origin:
            self.send_header("Access-Control-Allow-Origin", origin)
            self.send_header("Vary", "Origin")

    def _send(self, status, body, ctype="application/json; charset=utf-8", extra=(), compress=False):
        if isinstance(body, (dict, list)):
            body = (json.dumps(body, ensure_ascii=False) + "\n").encode("utf-8")
        elif isinstance(body, str):
            body = body.encode("utf-8")
        # a room's layers are megabytes of palette indices: gzipped, a tenth
        if compress and "gzip" in (self.headers.get("Accept-Encoding") or ""):
            body = gzip.compress(body, compresslevel=1)
            extra = tuple(extra) + (("Content-Encoding", "gzip"),)
        self.send_response(status)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Content-Type-Options", "nosniff")
        self._cors()
        for k, v in extra:
            self.send_header(k, v)
        self.end_headers()
        if self.command != "HEAD":
            self.wfile.write(body)

    def _error(self, e):
        body = {"ok": False, "error": e.message}
        if e.diagnostics is not None:
            body["diagnostics"] = e.diagnostics
        self._send(e.status, body)

    def _body(self):
        n = int(self.headers.get("Content-Length") or 0)
        if n > 64 * 1024 * 1024:
            raise HttpError(413, "too big")
        raw = self.rfile.read(n) if n else b""
        if not raw:
            return {}
        try:
            return json.loads(raw)
        except json.JSONDecodeError as e:
            raise HttpError(400, "the request isn't JSON: %s" % e)

    def _guard(self, mutating):
        if not self._host_ok():
            raise HttpError(403, "the Host isn't this server's (127.0.0.1:%d)" % self.server.server_address[1])
        if self._origin_ok() is False:
            raise HttpError(403, "requests from %s aren't allowed" % self.headers.get("Origin"))
        if self.path.startswith("/api/") and self.app.token:
            auth = self.headers.get("Authorization") or ""
            # a GET may give it in the query instead: an <img>'s picture
            query = dict(urllib.parse.parse_qsl(urllib.parse.urlsplit(self.path).query))
            if not (secrets.compare_digest(auth, "Bearer " + self.app.token)
                    or (not mutating and secrets.compare_digest(query.get("token", ""), self.app.token))):
                raise HttpError(401, "this server needs its token (Authorization: Bearer ...)")
        if mutating and self.headers.get("X-Crash-Editor") != "1":
            raise HttpError(403, "a change needs the header X-Crash-Editor: 1")

    # -- methods -------------------------------------------------------------

    def do_OPTIONS(self):
        origin = self._origin_ok()
        if not origin or not self._host_ok():
            self._send(403, {"ok": False, "error": "not allowed"})
            return
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", origin)
        self.send_header("Access-Control-Allow-Methods", "GET, PUT, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Authorization, Content-Type, X-Crash-Editor")
        self.send_header("Access-Control-Max-Age", "600")
        self.send_header("Vary", "Origin")
        self.send_header("Content-Length", "0")
        self.end_headers()

    def do_GET(self):
        self._dispatch("GET")

    def do_HEAD(self):
        self._dispatch("GET")

    def do_PUT(self):
        self._dispatch("PUT")

    def do_POST(self):
        self._dispatch("POST")

    def _dispatch(self, method):
        try:
            self._guard(method != "GET")
            url = urllib.parse.urlsplit(self.path)
            query = dict(urllib.parse.parse_qsl(url.query))
            path = urllib.parse.unquote(url.path)
            if path.startswith("/api/"):
                self._api(method, path[len("/api/"):].rstrip("/").split("/"), query)
            elif method == "GET":
                self._static(path)
            else:
                raise HttpError(405, "no %s here" % method)
        except HttpError as e:
            self._error(e)
        except subprocess.TimeoutExpired as e:
            self._error(HttpError(504, "%s took too long" % " ".join(map(str, e.cmd[1:3]))))
        except (BrokenPipeError, ConnectionResetError):
            pass
        except Exception as e:  # an answer, not a dropped connection
            self._error(HttpError(500, "%s: %s" % (type(e).__name__, e)))

    def _api(self, method, parts, query):
        app = self.app
        route = (method,) + tuple(parts[:1])
        if route == ("GET", "health"):
            return self._send(200, app.health())
        if route == ("GET", "schemas"):
            if len(parts) == 1:
                return self._send(200, app.schema_index())
            return self._send(200, app.schema(parts[1]), "application/schema+json; charset=utf-8")
        if parts[0] != "projects":
            raise HttpError(404, "no API %s" % "/".join(parts))
        if len(parts) == 1:
            if method == "GET":
                return self._send(200, app.list_projects())
            if method == "POST":
                body = self._body()
                return self._send(200, app.add_mod(body.get("mod") or "").as_dict(app.tree))
            raise HttpError(405, "GET or POST")
        pid, what = parts[1], "/".join(parts[2:])
        if (method, what) == ("POST", "apply"):
            return self._send(200, app.apply_mod(app.project(pid, ready=False)))
        project = app.project(pid)
        if (method, what) == ("GET", "files"):
            return self._send(200, app.files(project))
        if what == "file":
            rel = query.get("path", "")
            if method == "GET":
                return self._send(200, app.read(project, rel))
            if method == "PUT":
                body = self._body()
                if "data" not in body:
                    raise HttpError(400, "give the file's value as {\"data\": ...}")
                return self._send(200, app.save(project, [(rel, body["data"])]))
        if (method, what) == ("POST", "save"):
            files = self._body().get("files")
            if not isinstance(files, list) or not all(isinstance(f, dict) and "path" in f and "data" in f
                                                      for f in files):
                raise HttpError(400, "give {\"files\": [{\"path\": ..., \"data\": ...}, ...]}")
            return self._send(200, app.save(project, [(f["path"], f["data"]) for f in files]))
        if (method, what) == ("GET", "raw"):
            p = app.raw(project, query.get("path", ""))
            return self._send(200, p.read_bytes(), mimetypes.guess_type(p.name)[0] or "application/octet-stream")
        if (method, what) == ("GET", "check"):
            return self._send(200, app.model_check(project, text=query.get("text", "1") != "0"))
        if (method, what) == ("POST", "preview"):
            return self._send(200, app.preview(project, self._body()))
        if (method, what) == ("GET", "room"):
            return self._send(200, app.room(project, query.get("path", "")))
        if (method, what) == ("GET", "room/layer"):
            return self._send(200, app.room_layer_data(project, query.get("path", ""), query.get("layer", "")),
                              "application/octet-stream", compress=True)
        if (method, what) == ("GET", "room/tiles"):
            return self._send(200, app.room_tiles(project, query.get("path", ""), query.get("layer", "")),
                              compress=True)
        if (method, what) == ("POST", "room/save"):
            return self._send(200, app.save_room(project, self._body()))
        if (method, what) == ("POST", "room/import"):
            return self._send(200, app.import_tiles(project, self._body()))
        if (method, what) == ("POST", "room/new"):
            return self._send(200, app.new_room(project, self._body()))
        if (method, what) == ("POST", "room/delete"):
            return self._send(200, app.delete_room(project, self._body()))
        if (method, what) == ("POST", "room/resize"):
            return self._send(200, app.resize_room(project, self._body()))
        if (method, what) == ("GET", "sprite"):
            return self._send(200, app.sprite(project, query.get("bank"), query.get("frame"), query.get("palette")),
                              "image/png")
        if (method, what) == ("GET", "preview.png"):
            return self._send(200, app.preview_png(project, query.get("shot")), "image/png")
        if (method, what) == ("GET", "cutscenes"):
            return self._send(200, app.cutscenes(project), compress=True)
        if (method, what) == ("GET", "cutscenes/picture"):
            return self._send(200, app.cutscene_picture(project, query.get("path", "")), "application/octet-stream",
                              compress=True)
        if (method, what) == ("POST", "cutscenes/import"):
            return self._send(200, app.cutscene_import(project, self._body()))
        if (method, what) == ("POST", "cutscenes/save"):
            return self._send(200, app.save_cutscenes(project, self._body()))
        if (method, what) == ("GET", "music"):
            return self._send(200, app.music(project), compress=True)
        if (method, what) == ("GET", "music/pool"):
            return self._send(200, app.music_pool(project), compress=True)
        if (method, what) == ("GET", "music/song"):
            return self._send(200, app.music_song(project, query.get("key", "")), compress=True)
        if (method, what) == ("POST", "music/pool"):
            return self._send(200, app.music_pool(project, self._body()), compress=True)
        if (method, what) == ("POST", "music/song"):
            body = self._body()
            return self._send(200, app.music_song(project, body.get("key"), body.get("edits"), body), compress=True)
        if (method, what) == ("GET", "music/patterns"):
            return self._send(200, app.music_patterns(project, query.get("key", "")), compress=True)
        if (method, what) == ("POST", "music/patterns"):
            body = self._body()
            return self._send(200, app.music_patterns(project, body.get("key"), body.get("xm")), compress=True)
        if (method, what) == ("POST", "music/import"):
            return self._send(200, app.music_import(project, self._body()))
        if (method, what) == ("POST", "music/save"):
            return self._send(200, app.save_music(project, self._body()))
        raise HttpError(404, "no API %s %s" % (method, "/".join(parts)))

    def _static(self, path):
        root = self.app.static.resolve()
        if not (root / "index.html").is_file():
            return self._send(200, NO_EDITOR % {"dir": root}, "text/html; charset=utf-8")
        rel = path.lstrip("/")
        p = (root / rel).resolve() if rel else root / "index.html"
        if not p.is_relative_to(root) or not p.is_file():
            p = root / "index.html"  # the app's own routes
        ctype = mimetypes.guess_type(p.name)[0] or "application/octet-stream"
        if ctype.startswith("text/") or ctype in ("application/javascript", "application/json"):
            ctype += "; charset=utf-8"
        self._send(200, p.read_bytes(), ctype)


NO_EDITOR = """<!doctype html><meta charset="utf-8"><title>Crash editor</title>
<body style="font-family:sans-serif;max-width:40em;margin:3em auto">
<h1>The editor isn't built</h1>
<p>The backend is running, but there is no web editor in <code>%(dir)s</code>. Build it
(Node.js and pnpm, docs/editors.md "Running the editor"):</p>
<pre>pnpm install --frozen-lockfile
pnpm --filter @crash/editor build</pre>
<p>or run <code>make editor-build</code>, then reload. The API is at <a href="/api/health">/api/health</a>.</p>
"""


class _HTTPServer(http.server.ThreadingHTTPServer):
    daemon_threads = True
    verbose = False


def serve(app, port=DEFAULT_PORT, verbose=False):
    """The server on 127.0.0.1:port (0: any free port), not yet running."""
    handler = type("BoundHandler", (Handler,), {"app": app})
    httpd = _HTTPServer(("127.0.0.1", port), handler)
    httpd.verbose = verbose
    return httpd


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", type=int, default=int(os.environ.get("CRASH_EDITOR_PORT", DEFAULT_PORT)),
                    help="the port on 127.0.0.1 (0: any free one; default %(default)s)")
    ap.add_argument("--static", help="the built web editor (default: the tree's apps/editor/dist)")
    ap.add_argument("--token", default=os.environ.get("CRASH_EDITOR_TOKEN"),
                    help="require it on every API request (the desktop app's; default $CRASH_EDITOR_TOKEN)")
    ap.add_argument("--allow-origin", action="append", default=[], metavar="ORIGIN",
                    help="with --token, another page that may call it (the desktop app's `tauri dev`)")
    ap.add_argument("--watch-stdin", action="store_true",
                    help="stop when stdin closes (the desktop app's: the backend goes with the app, however it ends)")
    ap.add_argument("--open", action="store_true", help="open the editor in the browser")
    ap.add_argument("--verbose", action="store_true", help="log every request on stderr")
    args = ap.parse_args(argv)
    app = Server(ROOT, args.static, args.token, origins=args.allow_origin)
    httpd = serve(app, args.port, args.verbose)
    url = "http://127.0.0.1:%d/" % httpd.server_address[1]
    # the desktop app reads this line for the port
    print("editor-server: listening on %s" % url, flush=True)
    if not (app.static / "index.html").is_file():
        print("editor-server: no web editor in %s yet: `make editor-build` (docs/editors.md)" % app.static,
              flush=True)
    if args.watch_stdin:
        def watch():
            while sys.stdin.buffer.read(4096):
                pass
            os._exit(0)
        threading.Thread(target=watch, daemon=True).start()
    if args.open:
        webbrowser.open(url)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        httpd.server_close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
