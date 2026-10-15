"""The editors' backend: crashdata over HTTP, on 127.0.0.1 (#1167, docs/editors.md "The editor's backend").

    tools/editor_server.py [--port N] [--static DIR] [--token T] [--open] [--verbose]
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
    POST /api/projects/ID/preview          {"level", "room", "headless", "frames"}: tools/preview.py
    GET  /api/projects/ID/preview.png      a headless preview's picture

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
import hashlib
import http.server
import json
import mimetypes
import os
import re
import secrets
import shutil
import subprocess
import sys
import threading
import time
import urllib.parse
import webbrowser
from pathlib import Path

from crashdata import editing, schemas
from crashdata.common import diagnostics as D
from crashdata.common import jsonfmt
from crashdata.common.game import GAMES, ROOT, Game

API_VERSION = 1
DEFAULT_PORT = 8765
DESKTOP_ORIGINS = ("tauri://localhost", "http://tauri.localhost", "https://tauri.localhost")
CHECK_TIMEOUT = 600
PREVIEW_TIMEOUT = 3600


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
        self.baseline = None  # (fingerprint, the model's diagnostics)

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
    def __init__(self, tree=ROOT, static=None, token=None, python=sys.executable):
        self.tree = Path(tree).resolve()
        self.static = Path(static) if static else self.tree / "apps" / "editor" / "dist"
        self.token = token
        self.python = python
        self.projects = {}
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
        h = hashlib.sha1()
        for p, _ in schemas.files(Game(project.game, project.root), project.root):
            st = p.stat()
            h.update(("%s %d %d\n" % (p, st.st_mtime_ns, st.st_size)).encode())
        return h.hexdigest()

    def baseline(self, project):
        fp = self.fingerprint(project)
        if project.baseline is None or project.baseline[0] != fp:
            project.baseline = (fp, self.model_check(project)["diagnostics"])
        return project.baseline[1]

    def save(self, project, items):
        """Save files ([(path, value)]): rendered, schema-validated, then the
        model's checks; all saved or none."""
        if not isinstance(items, list) or not items:
            raise HttpError(400, "nothing to save")
        with project.lock:
            plan, diags = [], []
            for rel, value in items:
                p, schema = self.resolve(project, rel)
                old = p.read_bytes() if p.is_file() else None
                old_text = old.decode("utf-8") if old is not None else None
                text = editing.render(p, value, old_text, schema)
                if not jsonfmt.same(json.loads(text), value):
                    raise HttpError(500, "%s: the writer didn't give back the value" % rel)
                if old_text == text:
                    continue
                if not p.parent.is_dir():
                    raise HttpError(400, "%s: its directory doesn't exist" % rel)
                tmp = p.with_name(".%s.editor-%s.tmp" % (p.name, secrets.token_hex(4)))
                tmp.write_text(text, encoding="utf-8")
                try:
                    schemas.load(tmp, schema)
                except D.DataError as e:
                    tmp.unlink()
                    diags += _diags(e.diagnostics, rel)
                    continue
                plan.append((rel, p, tmp, old))
            if diags:
                for _, _, tmp, _ in plan:
                    tmp.unlink()
                raise HttpError(422, "the schema rejects it", diags)
            if not plan:
                return {"ok": True, "saved": [], "diagnostics": []}
            try:
                before = self.baseline(project)
            except BaseException:
                for _, _, tmp, _ in plan:
                    tmp.unlink()
                raise
            for _, p, tmp, _ in plan:
                os.replace(tmp, p)
            try:
                after = self.model_check(project)["diagnostics"]
            except BaseException:
                self._restore(plan)
                raise
            seen = {_key(d) for d in before}
            new = [d for d in after if d["severity"] == D.ERROR and _key(d) not in seen]
            if new:
                self._restore(plan)
                raise HttpError(422, "the model's checks reject it", new)
            project.baseline = (self.fingerprint(project), after)
            return {"ok": True, "saved": [rel for rel, _, _, _ in plan], "diagnostics": after}

    @staticmethod
    def _restore(plan):
        for _, p, _, old in plan:
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
            project.baseline = None
            try:
                report = json.loads(r.stdout)
            except json.JSONDecodeError:
                report = {"ok": r.returncode == 0, "diagnostics": [], "output": r.stdout + r.stderr}
            if r.returncode != 0:
                raise HttpError(422, "the mod didn't apply", report.get("diagnostics"))
            return {"ok": True, "project": project.as_dict(self.tree), "report": report}

    def preview(self, project, body):
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
        if origin in own or (self.app.token and origin in DESKTOP_ORIGINS):
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

    def _send(self, status, body, ctype="application/json; charset=utf-8", extra=()):
        if isinstance(body, (dict, list)):
            body = (json.dumps(body, ensure_ascii=False) + "\n").encode("utf-8")
        elif isinstance(body, str):
            body = body.encode("utf-8")
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
        if (method, what) == ("GET", "preview.png"):
            p = project.root / "build" / "editor" / "preview.png"
            if not p.is_file():
                raise HttpError(404, "no preview picture yet")
            return self._send(200, p.read_bytes(), "image/png")
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
    ap.add_argument("--open", action="store_true", help="open the editor in the browser")
    ap.add_argument("--verbose", action="store_true", help="log every request on stderr")
    args = ap.parse_args(argv)
    app = Server(ROOT, args.static, args.token)
    httpd = serve(app, args.port, args.verbose)
    url = "http://127.0.0.1:%d/" % httpd.server_address[1]
    # the desktop app reads this line for the port
    print("editor-server: listening on %s" % url, flush=True)
    if not (app.static / "index.html").is_file():
        print("editor-server: no web editor in %s yet: `make editor-build` (docs/editors.md)" % app.static,
              flush=True)
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
