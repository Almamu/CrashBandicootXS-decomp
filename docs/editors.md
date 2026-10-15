# Editors: the data's contract, the editor and its backend

The level, cutscene and music editors (#1167 stages 4-6: a TypeScript
web app and a Tauri desktop shell) edit the same files a modder edits by
hand (games/<game>/data/, graphics/, sound/, text/; [modding.md](./modding.md))
and run the same tools. The first half of this page is what they can rely
on (#1167 stage 3, #1093 part 1):

- **JSON Schemas** of every data file, generated from crashdata's model
  ("Schemas");
- **machine-readable errors**: `--json` on the data tools, and one error
  format ("Errors");
- **`make data-check`**: every check of the data, without building
  ("Checking the data");
- **named room parameters** in room.json ("Named room parameters");
- **a preview**: the PC port started at a level's room ("Preview").

The second half is the editor itself (#1167 stages 4 and 5): a web app in
`apps/` ("Running the editor", "The TypeScript workspace") on a small
local server, crashdata over HTTP ("The editor's backend"), and the same
app as a desktop program, no browser needed ("The desktop app").

Nothing here changes what the build makes: both ROMs and both .map files
are the same with or without it.

## Schemas

`packages/crashdata/schemas/` has one JSON Schema (draft 2020-12) per kind
of data file, and `index.json`, which files each one describes:

| schema | the files |
|---|---|
| `levels.schema.json` | `games/*/data/levels/levels.json` |
| `room.schema.json` | `games/*/data/levels/<level>/<slot>/room.json` |
| `spawn_list.schema.json` | `games/*/data/levels/spawns/*.json` |
| `anim_table.schema.json` | `games/*/data/levels/anim_tables/*.json` |
| `entity_types.schema.json`, `crate_kinds.schema.json`, `collision_legend.schema.json`, `tileset_order.schema.json` | the level data's other files |
| `cutscenes.schema.json`, `credits.schema.json` | `games/*/data/cutscenes.json`, `credits.json` |
| `graphics_manifest.schema.json` | `games/*/graphics/manifest.json` |
| `sprite_bank.schema.json`, `sprite_sheet.schema.json`, `palette_high_bits.schema.json` | a sprite bank's `bank.json`, a framed sheet's `sheet.json`, `logos/universal_high.json` |
| `gax_manifest.schema.json`, `gax_sfx_manifest.schema.json`, `music.schema.json`, `sfx_table.schema.json` | `games/*/sound/`'s manifests and tables |
| `song_settings.schema.json`, `instrument.schema.json` | a new song's `songs/<key>.json`, a new instrument's `instruments/<name>.json` |

`index.json` is `{"schemas": [{"schema": NAME, "fileMatch": [GLOB, ...]}]}`,
the globs from the tree's root, as an editor's JSON settings take them
(VS Code's `json.schemas`). The data files carry no `$schema` key: some of
the data is hashed or copied into the build (the save version is a CRC of
the level data), and the mapping is the editors' business.

**Where they come from.** Each domain of crashdata describes its own
files, from the field lists and limits its loaders use:
`crashdata/levels/schema.py` (from `crashdata.levels`' `LEVEL_KEYS`,
`THEME_KEYS`, `ON_CLEAR_KEYS`, `CATEGORY_KEYS`, `ROOM_KINDS`, `LAYERS`,
`ANIM_RECORD_KEYS`, ...), `crashdata/cutscenes/schema.py` (`SLIDE_FIELDS`,
`NUMBER_FIELDS`; the credits from `crashdata.text`),
`crashdata/graphics/schema.py` (a table of the manifest's kinds and each
kind's keys, `kind_keys()`) and `crashdata/audio/schema.py` (the GAX
manifests, `NEW_SONG_DEFAULTS`, `INSTRUMENT_FIELDS`). `crashdata.schemas`
lists them and the files each describes. They live under
`packages/crashdata/` because they are that package's output, next to the
code that writes them, where the TypeScript workspace will generate its
types from (stage 4).

**They can't drift from the loaders.** The loaders read the files through
`crashdata.schemas.load(path)`, which validates each file against its
schema before the model's own checks (a file read again unchanged isn't
validated again). So what the schema rejects, the build rejects, with the
same message; and since every object's other keys are rejected
(`additionalProperties: false`), a key a loader starts reading is refused
until the schema has it. A key the model lists (a `*_KEYS` list) that the
schema doesn't describe yet is allowed with any value (`$comment: read by
the model, not described here yet`), so a new model field never breaks the
build, only shows up in the generated schema.

    make schemas            # tools/schemas.py write: packages/crashdata/schemas/
    make schemas-check      # tools/schemas.py --check: fail if they're out of date (CI)
    python3 tools/schemas.py validate [--game xs|n2] [FILE ...]

A change to a model's fields comes with `make schemas`; CI's "Check the
data schemas" fails otherwise. The schemas are built and checked by
crashdata's own small validator (`crashdata.common.schema`: types, ranges,
patterns, required and unknown keys, `anyOf`/`oneOf`, `$ref`), so the
build needs no `jsonschema` package; any draft 2020-12 validator accepts
the same files.

What a schema can't say, the model checks: that a name exists (an entity
type, a room folder, a song, a graphic of the manifest), that the
entities are in id order, that a palette matches. Those are "Checking the
data"'s second half.

## Named room parameters

A room's entity parameter records were raw word lists; they now name
their fields where the entity types say which view their spawner reads
(entity_types.json's `"params"`), as docs/levels.md, "Parameter records"
describes, and stay words where nothing does (N-Tranced's, unused or
shared records). The schema's `room.schema.json` has each view
(`$defs/param_<layout>`).

**Migration (#1167, one time).** XS's 41 rooms were rewritten with
`tools/levels.py name-params`: 587 of their 624 records are named, every
one giving back the same words (the ROM is the same; the save version's
CRC hashes the words, not the form). The loader accepts both forms, but
`make data-check` rejects a raw record that `name-params` can name, so the
named form is the one the data keeps. A mod's room copied before this
(`tree.copy_room` copies the tree's, already named) or written by hand as
words is still read: `tools/levels.py name-params games/xs/data/levels/<room>`
names it.

## Errors

Every data tool reports a problem as a **diagnostic**
(`crashdata.common.diagnostics.Diagnostic`):

| field | what |
|---|---|
| `file` | the file, from the tree's root (or as given) |
| `pointer` | where in it: a JSON pointer (RFC 6901), `/entities/3/param`; `null` when the check doesn't know |
| `line`, `col` | the pointer's place in the file (1-based), or a JSON syntax error's; `null` when unknown |
| `severity` | `error` (the build stops) or `warning` (it goes on) |
| `code` | which check: `schema`, `json-syntax`, `entity-type`, `entity-param`, `param-raw`, `param-layout`, `param-record`, `link`, `rooms`, `themes`, `categories`, `graphics`, `text`, ... |
| `message` | what's wrong, as the text output says it |

As text a diagnostic is `FILE: POINTER: MESSAGE` (`FILE: warning: MESSAGE`
for a warning), on stderr, which is how the tools have always printed
their errors. The model's checks raise `DataError` (a `SystemExit`, so a
tool that doesn't catch it prints the text and exits 1); the older checks
still exit with a message, which `--json` reads back as a diagnostic (its
file from the message's `FILE: ` prefix, no pointer).

**`--json`.** `tools/levels.py`, `gfx_manifest.py`, `cutscenes.py`,
`text.py`, `gax_audio.py`, `sfx_table.py`, `apply_mod.py` (mods),
`schemas.py` and `data_check.py` take `--json` anywhere on their command
line: instead of the text, they print one JSON report on stdout,

```json
{
 "version": 1,
 "tool": "cutscenes",
 "ok": false,
 "diagnostics": [
  {"file": "games/xs/data/cutscenes.json", "pointer": "/slides/0/buttons", "line": 63, "col": 126,
   "severity": "error", "code": "schema", "message": "900 is out of range (0 to 255)"}
 ],
 "output": ""
}
```

`output` is what the command printed on stdout (its own and the commands
it ran), `ok` whether it succeeded; the exit status is the same as
without `--json` (1 on an error). `version` changes if a field changes
meaning. Without `--json` every command prints what it always has.

## Checking the data

    make data-check                      # both games: XS's and N-Tranced's
    make GAME=n2 data-check              # one game
    make data-check MOD=mods/sample      # a mod: applied to build/mods/sample, then checked there
    make data-check DATA_CHECK_ARGS=--json
    python3 tools/data_check.py [--game xs|n2] [--no-text] [--json]

`make data-check` builds nothing (it needs no compiler and no
`baserom.gba`; the text check needs gettext's `msgfmt` and `msgcmp`,
`--no-text` skips it). For the game, `crashdata.datacheck`:

1. validates every data file against its schema;
2. runs the model's checks, each on its own so one failure doesn't hide
   another: the rooms (`crashdata.levels.check_room`: the layer
   pictures' palettes, the collision image's size, the entity types, the
   entities' order, each entity's parameter record and its view, the
   links' ids), the
   level table, the themes, the actor categories (spawn lists, graphics,
   animation tables), the bosses, what clearing a level does, the
   level-select worlds, the save's layout, the tile set order; the
   graphics manifest (`gfx_manifest.py check`); the cutscenes; the song
   table, the screens' music and the sound effects; the text (`text.py
   check`).

It prints each problem and `data-check: xs OK`, or the error count and
exit status 1. A check that stops at its first problem (most of the
model's do) reports that one; fix it and run again. CI runs it on both
games (build.yml, "Check the data") and on the sample mod's tree
(mods.yml, "Check the mod's data").

## Preview

    make preview LEVEL=jungle_jam                    # in a window, from the level select on
    make preview LEVEL=snow_crash ROOM=1 PREVIEW_ARGS="--headless --out polar.png"
    make preview MOD=mods/sample LEVEL=sample_grove  # a mod: applied, then previewed in its tree
    python3 tools/preview.py LEVEL [ROOM] [--headless] [--frames N] [--every N]
                             [--screens DIR] [--out PNG] [--keys FILE] [--sdl 0|1]
                             [--no-build] [--json]

`tools/preview.py` (buildtools.preview) builds the PC port from the
tree's data (`make PLATFORM=pc`: with SDL2's window when pkg-config finds
SDL2, `--sdl 0|1` to say; [platforms.md](./platforms.md)) and runs
`build/pc/crashbandicootxs_pc` with `platform/pc/preview.keys`, which goes
through the logos and menus to the level select, and `--level`/`--room`
(platform/pc/src/level_pick.c), which make it start the level in the
room. Nothing in the game changes: the same two hooks are what the host's
mGBA comparisons use.

- **LEVEL** is gLevelTable's index, a level's constant (`LEVEL_JUNGLE_JAM`
  or `JUNGLE_JAM`), its name (`"jungle jam"`), its folder (`jungle_jam`)
  or one of its room folders (`snow_crash/part2`: the level and the room).
- **ROOM** is the index in the level's `"rooms"` (0 by default), or one of
  them by folder: a stage (`{"stage": ...}`) is a room too. A bonus or
  gem-path room isn't in `"rooms"` (the level enters it, it isn't
  started), so it can't be previewed on its own.
- **In a window** the game runs paced from the level select on, and the
  keyboard or a gamepad plays it (Esc quits, F12 takes a screenshot).
- **Headless** (`--headless`, `--out`, `--screens`, or an executable
  built without SDL2) it runs as fast as it can and stops at `--frames`
  (700 by default, a few frames into the level: the level select picks at
  frame 511 and the room has loaded by then); `--screens DIR` gets every
  `--every`'th frame's picture (`DIR/FRAME.png`), `--out FILE` the last.
- `--no-build` runs the executable as it is; `--keys` plays another key
  file (`FRAME KEYS` lines, platforms.md, "Running it").
- `--json` prints one report (see "Errors") with `"preview"`: the level's
  index and constant, the room's index and entry, the command and the
  pictures written.

For a mod, `make preview MOD=...` applies it to `MOD_TREE`
(`build/mods/<name>`) and runs the tool there, building the mod's own PC
port; an editor working on a mod's tree runs `tools/preview.py` in it.

## Running the editor

    make editor-build    # once, and after a change to apps/: pnpm install, then the web editor's build
    make editor          # the backend on http://127.0.0.1:8765/, serving the editor: open it in a browser

`make editor` needs only Python 3.12 (the tools' own); `make editor-build`
needs Node.js 22 or later and pnpm (package.json's `packageManager`
gives the version; the project's devshell has both). `EDITOR_ARGS` go to
the server (`--port N`, `--open` to open the browser, `--verbose`).
Working on the editor itself:

    make editor                              # the backend, in one terminal
    pnpm dev                                 # Vite on http://127.0.0.1:5173/, /api proxied to the backend
    pnpm typecheck && pnpm lint && pnpm test # what CI runs (apps.yml), with pnpm gen:check
    make editor-test                         # all of it, and the backend's Python tests

What the editor (stage 4's shell) does:

- **Projects**: XS, N-Tranced, or a mod. A mod is a script
  ([modding.md](./modding.md), "Writing a mod"), so the editor edits the
  tree it makes, `build/mods/<name>` (as `make mod`): "Apply the mod"
  runs `tools/apply_mod.py` (again: the tree is made anew, and edits made
  there are lost). The tree's own `mods/*/mod.py` are listed; another
  folder can be added by its path.
- **Files**: every file `packages/crashdata/schemas/index.json`
  describes, grouped by schema.
- **A form for any data file**, made from its schema: objects and their
  keys (an optional key added where the schema has it, or removed),
  arrays (added, moved, removed; long ones a hundred items a page), one
  of several forms (`anyOf`/`oneOf`: room.json's parameter records), enums
  as lists, numbers with their ranges. A key the model reads that the
  schema doesn't describe yet is a JSON field. The value is checked
  against the schema as it changes (Ajv, draft 2020-12), the problems
  marked where they are; a JSON view edits the whole file as text.
  Ctrl+S saves.
- **Diagnostics**: "Check the data" runs `make data-check`'s checks on the
  project (with the text's, if "text" is ticked: gettext's msgfmt and
  msgcmp), and a refused save shows why; a problem in a data file opens
  it at its JSON pointer.
- **Preview**: `tools/preview.py` in the project's tree ("Preview" above):
  a level from levels.json and one of its rooms, headless (a picture
  after N frames) or in the PC port's window.

The real level, cutscene and music editors are stage 6.

## The editor's backend

`tools/editor_server.py` (`buildtools.editor_server`, with
`crashdata.editing`) is the editors' one implementation of reading,
writing and checking the data: the browser editor and the desktop app
both talk to it. It is Python's standard library over crashdata (no new
dependency), listens on **127.0.0.1 only**, and serves:

| request | what |
|---|---|
| `GET /api/health` | `{ok, version, tree, python, editor}` |
| `GET /api/projects` | `{projects: [{id, kind, game, gameTitle, title, root, ready, mod?}]}`: `xs`, `n2`, `mod:<name>` |
| `POST /api/projects` `{mod: DIR}` | adds a mod folder |
| `POST /api/projects/ID/apply` | applies a mod to its tree (`tools/apply_mod.py`, as `make mod`) |
| `GET /api/schemas`, `/api/schemas/NAME` | `index.json`, a schema |
| `GET /api/projects/ID/files` | `{files: [{path, schema}]}`, the project's data files |
| `GET /api/projects/ID/file?path=P` | `{path, schema, data}` |
| `PUT /api/projects/ID/file?path=P` `{data}` | saves one file (below) |
| `POST /api/projects/ID/save` `{files: [{path, data}]}` | saves several, all or none |
| `GET /api/projects/ID/raw?path=P` | any file of `games/<game>/` as it is (a picture) |
| `GET /api/projects/ID/check[?text=0]` | `tools/data_check.py --json`'s report ("Errors") |
| `POST /api/projects/ID/preview` `{level, room, headless, frames, noBuild}` | `tools/preview.py --json`'s report and `image`, or `{started, pid}` for the window |

A path is from the project's tree's root (`games/xs/data/levels/levels.json`),
and must be in the project's `games/<game>/`, with no `..`; a file the
schemas don't describe can't be read as data or written. A refusal is
`{ok: false, error, diagnostics?}` with a 4xx status.

**Saving.** A save is the file's new value. The backend writes it as
`crashdata.editing.render` gives it, checks it, and only then keeps it:

1. **The text.** Where the file is in a tool's own layout, the tool's
   writer writes it (`crashdata.levels.room_json_text` for a room.json,
   `levels_json_text` for levels.json, a framed sheet's sheet.json as
   json.dumps), so it is what the tool would have written. Every other
   file keeps its own layout (`crashdata.common.jsonfmt.dumps_like`): a
   value the file already had is the file's own text, byte for byte; a
   changed object or array keeps its brackets and the whitespace between
   its items, only its changed items are written again, and a new item is
   written like its siblings (another spawn of the list on one line,
   another slide). **A value saved unchanged gives the same bytes, and
   isn't written at all**; a change shows in git as the lines it changed.
2. **The schema.** The text is validated by `crashdata.schemas.load` (the
   loaders' own reading): an error is the answer, 422, with its diagnostics.
3. **The model.** The file replaces the old one and the model's checks run
   (`tools/data_check.py --no-text --json` in the project's tree, as
   `make data-check`): an error that wasn't there before the save puts
   every file of the save back as it was, and is the answer. Errors the
   data already had don't stop a save; they come back with it, and so do
   the warnings.

**Who can call it.** Besides listening on 127.0.0.1, the server answers
only requests whose `Host` is its own (`127.0.0.1:PORT` or
`localhost:PORT`: another site can't reach it under its own name), from
its own origin, and a request that changes something needs the header
`X-Crash-Editor: 1` (a page of another site can't send one without asking
first, which the server never allows). With `--token T` (or
`CRASH_EDITOR_TOKEN`), as the desktop app runs it, every `/api` request
also needs `Authorization: Bearer T` (or, for a GET, `?token=T`), and the
desktop app's pages may call it (CORS for `tauri://localhost` and
`http(s)://tauri.localhost`).

## The TypeScript workspace

A pnpm workspace at the root (`package.json`, `pnpm-workspace.yaml`,
`pnpm-lock.yaml`, `tsconfig.base.json`, `eslint.config.js`):

| package | what |
|---|---|
| `apps/schema-types` (`@crash/schema-types`) | the data files' types, generated from the schemas (`src/generated/`: `Room`, `Levels`, ..., `SchemaFiles` from a schema's name to its type, `FILE_MATCH`, index.json's globs), the backend's typed client (`EditorClient`: `read<"room.schema.json">(...)` gives a `Room`), `schemaFor(path)` (index.json's globs, as `crashdata.schemas.schema_for` matches them), JSON pointers |
| `apps/editor` (`@crash/editor`) | the web editor: Svelte 5, Vite, Ajv |

**The types are generated and committed**, like the schemas they come
from: `pnpm gen` (`apps/schema-types/scripts/generate.mjs`,
json-schema-to-typescript) writes `src/generated/`, and `pnpm gen:check`
(CI) fails when it is out of date. A change to crashdata's model is
`make schemas`, then `pnpm gen`. A schema's other types are under its
namespace (`RoomDefs.ParamFlags`).

**Why Svelte.** The editors are forms and canvases over large JSON
documents: Svelte 5 compiles to small code with no virtual DOM, its
`$state` proxies let a deep form bind straight into the document
(`bind:value={room.entities[3].x}`), and a recursive schema-driven
component stays one file. React would bring a larger runtime and
immutable-update plumbing for the same form; Solid was the other
candidate (as small, as fast), but Svelte's two-way binding and
single-file components fit form-heavy editors best, and svelte-check
typechecks the templates.

CI (`.github/workflows/apps.yml`) runs `pnpm gen:check`, `pnpm typecheck`
(tsc, svelte-check), `pnpm lint` (ESLint), `pnpm test` (vitest: the
generation, the client, the globs and pointers, the editor's schema
helpers), `pnpm build`, the backend's Python tests (every data file of
both games written back unchanged, a change in the tool's layout, the
refusals, the token), and a smoke test of `tools/editor_server.py` as
`make editor` runs it, through the typed client.

## The desktop app

`apps/desktop` (#1167 stage 5) is the same editor as a program of its
own, no browser: Tauri v2, the system's webview (WebKitGTK on Linux,
WebView2 on Windows, WKWebView on macOS) around apps/editor's build,
bundled into the binary. It has no backend of its own: it runs the
checkout's `tools/editor_server.py`, so the browser and the desktop app
read, write and check the data through one implementation.

- **First run**: a folder dialog asks for the checkout (a clone of this
  repository: it has `tools/editor_server.py`), remembered in the app's
  config directory (`config.json`: `~/.config/io.github.almamu.crashxs-editor/`
  on Linux, `%APPDATA%\io.github.almamu.crashxs-editor\` on Windows,
  `~/Library/Application Support/io.github.almamu.crashxs-editor/` on
  macOS); File > "Choose the checkout…" changes it, `--checkout DIR`
  gives it for one run.
- **The backend**: the app runs `python3 tools/editor_server.py --port 0
  --watch-stdin` in the checkout (`py -3`, `python` or `python3` on
  Windows: the first that is 3.12 or later; `--python CMD`,
  `CRASH_EDITOR_PYTHON` or config.json's `"python"` to choose), with a
  random token (`CRASH_EDITOR_TOKEN`), reads the port it got, and gives
  the page the URL and the token before it loads
  (`window.__CRASH_EDITOR__`). The backend stops when the app does,
  however it ends (`--watch-stdin`: its stdin closes).
- **What it needs**: Python 3.12 or later and the checkout; the build's
  toolchain (agbcc, arm-none-eabi, SDL2 for the PC port) only for
  "Check the data"'s text check (gettext) and Preview (`make PLATFORM=pc`),
  as for `make editor`. Linux also needs WebKitGTK 4.1 (the .deb and
  .rpm depend on it; the AppImage carries it).
- `crash-editor --smoke [--checkout DIR]` starts the backend, asks its
  `/api/health` and stops, with no window (CI's test of every platform's
  binary).

Building it needs Rust (rustup's stable), Node.js and pnpm, and on
Linux WebKitGTK 4.1, libsoup 3, GTK 3 and librsvg's development files
(Debian/Ubuntu: `libwebkit2gtk-4.1-dev libsoup-3.0-dev libgtk-3-dev
librsvg2-dev libssl-dev`):

    pnpm install --frozen-lockfile
    pnpm --filter @crash/desktop build      # tauri build: apps/desktop/src-tauri/target/release/
                                            # crash-editor, and the bundles in .../bundle/
    pnpm --filter @crash/desktop dev        # tauri dev: Vite's page, reloaded on a change

The Tauri CLI is the workspace's (`@tauri-apps/cli`): no `cargo
install`. On NixOS a binary built in the devshell finds its libraries
only with the devshell's library path (the bundles don't have that
problem). CI (`.github/workflows/desktop.yml`) builds it on Linux
(Ubuntu 22.04: .deb, .rpm, AppImage), Windows (.msi, NSIS .exe) and
macOS (.app, .dmg), runs `--smoke` on each, opens the Linux build's
window in xvfb (its page loads and lists XS's files through its
backend), and keeps the bundles as the run's artifacts. They are
unsigned and nothing is released: Windows SmartScreen and macOS
Gatekeeper warn about them (macOS: right-click > Open, or
`xattr -d com.apple.quarantine`).
