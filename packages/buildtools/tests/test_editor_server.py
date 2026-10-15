"""buildtools.editor_server (tools/editor_server.py): the editors' backend
(docs/editors.md, "The editor's backend"), started on a free port of
127.0.0.1 against this tree. A test that changes a file puts it back.

    python3 -m unittest discover -s packages/buildtools/tests
"""
import copy
import json
import sys
import threading
import unittest
import urllib.error
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
for _p in ("crashdata", "buildtools"):
    sys.path.insert(0, str(ROOT / "packages" / _p / "src"))

from buildtools import editor_server as E  # noqa: E402
from crashdata import levels as L  # noqa: E402


class Backend(unittest.TestCase):
    token = None

    @classmethod
    def setUpClass(cls):
        cls.app = E.Server(ROOT, token=cls.token)
        cls.httpd = E.serve(cls.app, 0)
        cls.port = cls.httpd.server_address[1]
        cls.thread = threading.Thread(target=cls.httpd.serve_forever, daemon=True)
        cls.thread.start()

    @classmethod
    def tearDownClass(cls):
        cls.httpd.shutdown()
        cls.httpd.server_close()

    def call(self, method, path, body=None, headers=None, write=None):
        h = {"Content-Type": "application/json"}
        if write if write is not None else method != "GET":
            h["X-Crash-Editor"] = "1"
        if self.token:
            h["Authorization"] = "Bearer " + self.token
        h.update(headers or {})
        data = json.dumps(body).encode() if body is not None else None
        req = urllib.request.Request("http://127.0.0.1:%d%s" % (self.port, path), data=data, method=method, headers=h)
        try:
            with urllib.request.urlopen(req, timeout=600) as r:
                return r.status, json.loads(r.read() or b"null")
        except urllib.error.HTTPError as e:
            with e:
                return e.code, json.loads(e.read() or b"null")


class Reading(Backend):
    def test_health_projects_schemas(self):
        st, h = self.call("GET", "/api/health")
        self.assertEqual((st, h["ok"], h["version"]), (200, True, 1))
        st, p = self.call("GET", "/api/projects")
        ids = [x["id"] for x in p["projects"]]
        self.assertEqual(ids[:2], ["xs", "n2"])
        self.assertIn("mod:sample", ids)
        st, idx = self.call("GET", "/api/schemas")
        self.assertEqual(st, 200)
        self.assertIn("room.schema.json", [s["schema"] for s in idx["schemas"]])
        st, s = self.call("GET", "/api/schemas/room.schema.json")
        self.assertEqual(s["$schema"], "https://json-schema.org/draft/2020-12/schema")
        self.assertEqual(self.call("GET", "/api/schemas/..%2Fx.json")[0], 404)

    def test_files_and_read(self):
        st, f = self.call("GET", "/api/projects/n2/files")
        self.assertTrue(all(x["path"].startswith("games/n2/") for x in f["files"]))
        st, r = self.call("GET", "/api/projects/xs/file?path=games/xs/data/levels/levels.json")
        self.assertEqual((st, r["schema"]), (200, "levels.schema.json"))
        self.assertEqual(r["data"], json.loads((ROOT / "games/xs/data/levels/levels.json").read_text()))

    def test_paths_outside_are_refused(self):
        for path, status in [("games/xs/../../README.md", 400), ("/etc/passwd", 400),
                             ("games/n2/data/levels/levels.json", 403), ("README.md", 403),
                             ("games/xs/data/data.s", 403), ("games/xs/data/nothing.json", 403)]:
            self.assertEqual(self.call("GET", "/api/projects/xs/file?path=" + path)[0], status, path)
        self.assertEqual(self.call("GET", "/api/projects/mod:sample/files")[0] in (200, 409), True)

    def test_a_change_needs_the_header_and_the_host(self):
        body = {"data": {}}
        self.assertEqual(self.call("PUT", "/api/projects/xs/file?path=games/xs/sound/music.json", body, write=False)[0],
                         403)
        self.assertEqual(self.call("GET", "/api/health", headers={"Host": "evil.example:80"})[0], 403)
        self.assertEqual(self.call("GET", "/api/health", headers={"Origin": "https://evil.example"})[0], 403)


class Saving(Backend):
    def test_every_file_saved_unchanged_is_not_written(self):
        for game in ("xs", "n2"):
            _, f = self.call("GET", "/api/projects/%s/files" % game)
            files = []
            for e in f["files"]:
                _, r = self.call("GET", "/api/projects/%s/file?path=%s" % (game, e["path"]))
                files.append({"path": e["path"], "data": r["data"]})
            before = {e["path"]: (ROOT / e["path"]).read_bytes() for e in files}
            st, r = self.call("POST", "/api/projects/%s/save" % game, {"files": files})
            self.assertEqual((st, r["saved"]), (200, []), r)
            for p, b in before.items():
                self.assertEqual((ROOT / p).read_bytes(), b, p)

    def test_a_change_is_saved_in_the_files_layout_then_put_back(self):
        rel = "games/xs/data/levels/jungle_jam/main/room.json"
        path = ROOT / rel
        old = path.read_bytes()
        try:
            v = json.loads(old)
            v2 = copy.deepcopy(v)
            v2["entities"][1]["y"] += 1
            st, r = self.call("PUT", "/api/projects/xs/file?path=" + rel, {"data": v2})
            self.assertEqual((st, r["saved"]), (200, [rel]), r)
            self.assertEqual(path.read_text(), L.room_json_text(v2))
            st, r = self.call("PUT", "/api/projects/xs/file?path=" + rel, {"data": v})
            self.assertEqual((st, r["saved"]), (200, [rel]))
            self.assertEqual(path.read_bytes(), old)
        finally:
            path.write_bytes(old)

    def test_the_schema_and_the_model_refuse(self):
        rel = "games/xs/sound/music.json"
        path = ROOT / rel
        old = path.read_bytes()
        try:
            v = json.loads(old)
            bad = copy.deepcopy(v)
            bad["screen_music"][0]["bogus"] = 1
            st, r = self.call("PUT", "/api/projects/xs/file?path=" + rel, {"data": bad})
            self.assertEqual(st, 422)
            self.assertEqual(r["diagnostics"][0]["pointer"], "/screen_music/0/bogus")
            self.assertEqual(r["diagnostics"][0]["file"], rel)
            bad = copy.deepcopy(v)
            bad["screen_music"][0]["song"] = "no_such_song"
            st, r = self.call("PUT", "/api/projects/xs/file?path=" + rel, {"data": bad})
            self.assertEqual(st, 422, r)
            self.assertIn("no_such_song", r["diagnostics"][0]["message"])
            self.assertEqual(path.read_bytes(), old)
            self.assertEqual(sorted(p.name for p in path.parent.glob(".*.tmp")), [])
        finally:
            path.write_bytes(old)

    def test_data_check(self):
        st, r = self.call("GET", "/api/projects/n2/check?text=0")
        self.assertEqual(st, 200)
        self.assertEqual((r["version"], r["tool"]), (1, "data-check"))
        self.assertTrue(r["ok"], r["diagnostics"])


class Token(Backend):
    token = "s3cret"

    def test_the_token_is_needed(self):
        self.assertEqual(self.call("GET", "/api/health")[0], 200)
        saved, Token.token = Token.token, None
        try:
            self.assertEqual(self.call("GET", "/api/health")[0], 401)
            self.assertEqual(self.call("GET", "/api/health?token=s3cret")[0], 200)
        finally:
            Token.token = saved

    def test_the_desktop_app_may_call(self):
        st, _ = self.call("GET", "/api/health", headers={"Origin": "tauri://localhost"})
        self.assertEqual(st, 200)
        req = urllib.request.Request("http://127.0.0.1:%d/api/health" % self.port, method="OPTIONS",
                                     headers={"Origin": "http://tauri.localhost",
                                              "Access-Control-Request-Method": "PUT"})
        with urllib.request.urlopen(req) as r:
            self.assertEqual(r.status, 204)
            self.assertEqual(r.headers["Access-Control-Allow-Origin"], "http://tauri.localhost")


if __name__ == "__main__":
    unittest.main()
