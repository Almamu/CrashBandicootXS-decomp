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
- **a preview**: the PC port started at a level's room, or playing a
  cutscene ("Preview").

The second half is the editor itself (#1167 stages 4 to 6): a web app in
`apps/` ("Running the editor", "The TypeScript workspace") on a small
local server, crashdata over HTTP ("The editor's backend"), the same
app as a desktop program, no browser needed ("The desktop app"), and in
it the level editor ("The level editor"), the cutscene editor ("The
cutscene editor") and the music editor ("The music editor").

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
describes, and stay words where nothing does (unused or shared records,
N-Tranced's types whose spawners' reads aren't known). The schema's `room.schema.json` has each view
(`$defs/param_<layout>`).

**Migration (#1167, one time).** XS's 41 rooms were rewritten with
`tools/levels.py name-params`: 587 of their 624 records are named, every
one giving back the same words (the ROM is the same; the save version's
CRC hashes the words, not the form). The loader accepts both forms, but
`make data-check` rejects a raw record that `name-params` can name, so the
named form is the one the data keeps. A mod's room copied before this
(`tree.copy_room` copies the tree's, already named) or written by hand as
words is still read: `tools/levels.py name-params games/xs/data/levels/<room>`
names it. N-Tranced's 36 rooms followed (`tools/levels.py --levels-dir
games/n2/data/levels name-params`): 1,229 of their 1,268 records, by the
views its assembly shows ([n_tranced.md, "Parameter
records"](./n_tranced.md#parameter-records)); its ROM is the same.

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
   table, the screens' music and the sound effects; the music block and
   the sound-effect set built as `tools/gax_audio.py` and
   `tools/sfx_table.py` build them, into a temporary folder (`music-build`,
   `sfx-build`: every song encoded from its XM, the new instruments, the
   samples' WAVs, the limits of "GAX2's limits" in
   [audio.md](./audio.md#gax2s-limits)); the text (`text.py check`).

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
  or one of its room folders (`snow_crash/part2`: the level and the room;
  `jungle_jam/bonus`: the level and its bonus room).
- **ROOM** is the index in the level's `"rooms"` (0 by default), or one of
  them by folder: a stage (`{"stage": ...}`) is a room too. `bonus` and
  `gem_path` (or their folders) are the level's bonus and gem-path rooms,
  which aren't in `"rooms"`: the game enters them from the level (the "?"
  platform, the gem path), so the PC port's `--room bonus` or `--room
  gem-path` starts them instead. The level's first room is picked as
  usual, then the room block's own `EnterBonusRoom` or `EnterGemPathRoom`
  makes the extra room the one played (level_pick.c). The game isn't in
  its bonus round or on its gem path then (the platform sets that): the
  player starts at the room's start marker without the warp-in, and the
  room's exit goes on to the level's first room. Ace of space's gem path,
  the one `HOVER` room (room 16), stops the PC port on its first frame
  however it is entered: the hover controller writes its camera lead's
  speed before making the lead, and the lead is a sprite with no bank
  that the sprite code reads through NULL in several places; on the GBA
  those land in the BIOS area. The port doesn't cover that yet
  ([platforms.md](./platforms.md), "Seams").
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

**A cutscene** plays the same way, without a level:

    make preview CUTSCENE=CUTSCENE_INTRO LANGUAGE=fr          # in a window
    make preview CUTSCENE=1 PREVIEW_ARGS="--headless --screens shots"
    python3 tools/preview.py --cutscene CUTSCENE [--language LANG] [--headless] [--every N] ...

`--cutscene` (an id, or a constant: `CUTSCENE_INTRO` or `INTRO`) makes
the boot play that cutscene in place of the legal notice, in `--language`
(en, fr, de, es, it or nl, or 0-5; English by default), then boot on (the
logos, the language select, ...). It is the PC port's `--cutscene N
--language L` (platform/pc/src/cutscene_pick.c: the link wraps
`LevelState::PlayBootCutscene`, as level_pick.c wraps the level select;
nothing in the game changes). In a window the keyboard plays it; headless,
A is pressed every `--every` frames (60) and the picture before each
press is kept, one a screen of text (a slide without text is one), as
many as the cutscene's text needs in that language
(`crashdata.cutscenes.edit.cutscene_steps`, which counts them as the game
does) and one more. `--json`'s `"preview"` has the cutscene, its
constant, the language, the key presses and the pictures.

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

What the editor does (its header's tabs: "Level editor", "Cutscenes"
and "Music", below, and "Data files", stage 4's forms):

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

The level editor, the cutscene editor and the music editor are stage
6's three parts.

## The level editor

The editor's first tab (#1167 stage 6) opens a room and edits it: its
entities and their parameter records, its BG layers' tiles, its collision
layer and its palette. It saves them back to the room's folder (room.json,
`bg0.png`-`bg3.png`, `collision.png`: [levels.md, "Room
folders"](./levels.md#room-folders)) through the backend, which checks a
room's save like any other and keeps all of it or none.

**Opening a room.** The left column is the project's levels.json: each
level's rooms in play order, its bonus and gem-path rooms, and the stages
played in an actor category (which have no room to open). XS's rooms,
N-Tranced's (whose entity types are partly named, and whose parameter
records are named where its spawners show their views) and a mod's tree's
open the same way.

**The view** draws the room the way the game does:

- the BG layers back to front by priority (room.json's `cnt`, BGnCNT's
  priority bits; at one priority the lower BG is in front), from their
  pictures: BG0's index 0 and BG1-3's colour 0 of every bank are
  transparent, and the backdrop is black (`LoadRoom` sets
  `BACKDROP_COLOR` to 0 after copying the room's palette, so its entry 0
  is never shown);
- each layer scrolled by its parallax factor for the camera, the 240x160
  screen at the view's centre kept inside the room's scrollable size
  (`size_tiles`): a layer is shifted by `camera - camera * scale / 256`
  (`BgLayerBase::ScaleScroll`'s, rounded toward zero), so inside the
  screen the room looks as it does in the game at that camera. Without
  "parallax" every layer is drawn at its own (0, 0);
- the collision layer over them, in its legend's colours
  (collision_legend.json; 0 is clear), at the overlay's opacity;
- the entities, each a frame of its sprite bank at its position
  (`games/<game>/graphics/sprites/`: the frame `crashdata.levels.room_edit.SPRITES`
  names, else the bank of its enemy kind, else the bank named after its
  type, frame 0), or a labelled box where none is known; their links, an
  arrow from the lower crate to the one on top;
- the entity groups (256-pixel columns), the 240x160 screens and the
  camera's screen, as asked.

The wheel zooms at the cursor; the middle or right button, space and drag,
or a drag on an empty spot with the Select tool pans. The status bar gives
the camera, and the world point and cell under the cursor.

**Tools** (their keys): **Select** (V) selects an entity and drags it
(Shift: in steps of 8); **Add** (A) places an entity of the type chosen;
**Link** (L) links two crates, the lower one first; **Tiles** (T) and
**Collision** (C) paint. Delete deletes the entity selected, Escape
deselects, Ctrl+Z undoes, Ctrl+Shift+Z or Ctrl+Y redoes, Ctrl+S saves.
Every change is a step of the undo stack (a drag, a field, a stroke, a
colour; 500 of them), which also says whether the room is as saved.

**Entities.** The right column's Entities tab lists them, or edits the one
selected: its type (any of entity_types.json's, an unnamed one by number),
x, y, its parameter record and its links. room.json keeps the entities in
id order, the last group (256-pixel column) first (docs/levels.md,
"Entities"): an entity moved to another group goes to the end of that
group's entities, an added one too, and the ids after it change; the
links follow every renumbering, and a deleted entity's links go with it.
The record is a form of its view (docs/levels.md, "Parameter records"):
the flags and a crate's options as their bits, `trial_kind` and
`solid_kind` as entity types, the numbers in their ranges, or a raw
record's words; changing the view starts a record of zeros, and a view
other than the one its entities' type reads is marked (data-check refuses
it). A record several entities share is edited for all of them, unless
"Give this one its own copy" gives the entity a copy first. A new entity
gets its own record: a copy of one its type already uses in the room, or
every field of its type's view at zero. A deleted entity's record stays
(another may use it).

**Tiles.** The Tiles tab paints a BG layer with the tiles of its tile
set, the set as the build makes it from every room that names it
(docs/levels.md, "Tile sets"; the backend makes it once, a second or so
for the largest): a tile, its X and Y flips and, on BG1-3, its palette
bank (the bank it is first drawn in, to start with). A click paints one
cell, a drag a line of them, Shift and drag a rectangle; Alt and click
takes a cell's tile, flips and bank. A painted cell is the tile's pixels
in the layer's picture, BG1-3's as `bank * 16 + pixel`, which the build
reads back as that tile: painting never adds a tile, though a tile used
earlier in the set's order than before renumbers the set (the ROM data
after it moves). On a layer that doesn't scroll at the camera's speed,
untick "parallax" to paint it where it is. New tiles are drawn in an
image editor ([levels.md, "BG layers"](./levels.md#bg-layers)).

**Collision.** The Collision tab paints the collision layer's cells with
a value of the legend (one 8x8 cell of BG0's each; click, drag, Shift and
drag, Alt and click to pick); a value no room has yet needs its own colour
in collision_legend.json first ([levels.md, "Collision
layers"](./levels.md#collision-layers)).

**Palette.** The Palette tab edits the room's 256 colours, 5 bits a
channel (bit 15 is kept, docs/graphics.md "Palettes"; a colour picked
with the system's picker follows `tools/jasc_pal.py`'s rule), and the room
is drawn with them at once. Saved, the palette goes into every layer
picture of the room, as the build requires (docs/levels.md, "The room
palette").

**Room.** The Room tab edits the room's record in the level table
(room.json's `"record"`, [levels.md, "The level
table"](./levels.md#the-level-table)): its **kind** (`ON_FOOT`,
`UNDERWATER`, `HOVER`: the player's controller; the tab warns when the
room's start marker isn't the kind's) and its **blend**, the colour effect
`SetupRoomBlend` writes: BLDCNT's effect (0 none, 1 alpha blending, 2
brighten, 3 darken) with BG3 the first target and BG0-2 and the sprites
the second, and BLDALPHA's weights EVA and EVB (0-31, the PPU stops at
16). The view draws the alpha blend as the PPU does ("blend" in the layer
bar): where BG3 is the front layer and BG0-2 the next one under it, each
channel is `(BG3 * EVA + under * EVB) / 16`, at most full; over the
backdrop alone BG3 isn't blended (the backdrop isn't a second target).
Brighten and darken go by BLDY, which the room doesn't set (the fades
do), so they are drawn unchanged. The schema and the build take an
effect of 0-3 and weights of 0-31, the bits SetupRoomBlend keeps.

**Rooms.** "New room…" (under the level table) makes a room
(`crashdata.levels.room_ops`): a copy of a room, or an *empty* room laid
out like one (its layers, tile sets, scroll scales and priorities, its
palette and record; blank pictures and collision, the size asked, and
only its start marker), in a new folder `<level>/<slot>`. It gets the
next free `gRoomNN`/`gLevelRoomNN` symbols, goes at the end of
levels.json's `"rooms"` and, if asked, into a level: after its rooms, or
as its bonus or gem-path room (N-Tranced's `"table"` `"records"` get it
too); XS's cxx_symbols.txt gets its class's members (`levels.py
symbols`). The Room tab **resizes** the room: cells added (or taken) at
each edge of BG0 and the collision layer, which share a grid. Their
pictures move with the left and top edges, and so do the entities; an
entity left outside goes, with its links, and the entities are put back
in id order (the links follow). Each BG1-3 layer grows by its parallax
share of the same edges (`cells * scale / 256`), so at every camera
position it lines up with BG0 as before. The scrollable sizes, the chunk
grids and so the camera's bounds and the entity groups follow; the
layers' `pad_overrides` (the ROM's leftover bytes) go. "Check" says what
a resize would cut (entities, links, cells that aren't blank) before
anything is written. **Delete this room** deletes a room no level plays:
its folder, its entries in levels.json (with a ROM room's in
`"regions"` and `"assets"`) and tileset_order.json, its classes'
symbols. Each is one save, checked like any (all or nothing); the sizes
are held to what the engine takes ([levels.md, "Room
sizes"](./levels.md#room-sizes)).

**Saving** sends one request with what changed since the room was opened
or last saved: room.json's value, the painted cells of each layer, the
collision cells, the palette (`POST .../room/save`, "The editor's
backend"). A room saved unchanged writes nothing; a picture is written
only when its pixels or its palette change: a new palette alone replaces
the picture's PLTE and keeps its pixels' bytes, painted cells write the
picture as `tools/levels.py` writes it (which gives back the tree's
pictures byte for byte), so a change shows only where it is.

**Play this room** runs `tools/preview.py` at the room ("Preview"): in
the desktop app the PC port's window opens, in the browser it runs
headless and shows the room after 700 frames. It builds the saved data.
A bonus or gem-path room starts as the level's (`ROOM=bonus` or
`gem_path`, "Preview"); a room no level plays can't be started.

**Not yet:** drawing new tiles, adding a
collision value to the legend. N-Tranced's records of types 73, 92 and
106 stay words (what their spawners read isn't known yet).

## The cutscene editor

The "Cutscenes" tab (#1167 stage 6) edits the cutscenes
([data.md, "Cutscenes"](./data.md#cutscenes)): their slides, each a
slide record (a picture, timing, fades, a music cue, a sound effect) and
maybe a page of text in the six languages. It saves them back to
`games/xs/data/cutscenes.json`, the text's files (`games/xs/text/crashxs.pot`
and the five `.po`, [text.md](./text.md)) and the pictures
(`games/xs/graphics/cutscenes/`, their manifest entries) through the backend,
which checks a save like any other, the text's checks too, and keeps all
of it or none.

**The cutscenes** are the left column, in id order (PlayCutscene's
argument), each with what plays it: code that names its constant, a
level's `"on_clear"` (levels.json) that names its id or constant; "nothing
plays it" for the two the ROM never plays. "New cutscene…" adds one at the
end (its constant, `CUTSCENE_<NAME>`; its first slide shows the selected
slide's record, without text): something has to play it, a level's
`"on_clear"`. Only the last cutscene can be deleted, and only when nothing
plays it: a cutscene's id is its place, which levels.json and the code
name.

**The timeline** along the top is the cutscene's slides in order, each a
thumbnail of its picture and its first screen of text. Drag a slide, or
◀ ▶, to move it; "Add" puts a new slide after the selected one (a copy of
its record, with a new page of text: "..." in every language), "Duplicate"
the same record and a copy of its page, "Delete" deletes it (a cutscene
keeps one slide or more). ⇄ marks a record other slides show too, ∅ a
slide without text, and a time its wait.

**The slide view** draws the slide the way the game does (the PC port's
pictures, pixel for pixel, below): the picture in Mode 4 (its 256 colours,
at the screen's top left), and a screen of the text over it in the small
font, as `CutscenePlayer::Run` draws it: `DrawWrappedText` a word at a
time into the text box (228x30 pixels at (7, 126): three 9-pixel lines),
a word that doesn't fit on the line on the next, and what doesn't fit in
three lines after a key press; each glyph an OBJ of the font's sheet
(`fonts/small.png`, its own 16 colours) at its record's Y offset, a
character the font lacks drawn as its first glyph ("a"), an earlier glyph
in front where two overlap (OAM's order). ◀ ▶ (or the arrow keys) step
through the screens as A does, into the next slide; the language and the
zoom are below it. The colours are the hardware's (`c << 3 | c >> 2`). The
editor and crashdata.cutscenes.edit (the backend's, which `tools/preview.py`
counts the key presses with) are the same algorithm; the fades (the
slide's brightness fading in from black or white, out after it) aren't
drawn, the panel names them.

**The text** under the view is the slide's page: its strings, a key
press between them, each in the six languages side by side (English is
the template's `msgid`, the others each `.po`'s `msgstr`; ticks hide
languages). Each is checked as it is typed, as the save and `make
text-check` check it: in the game's encoding (Latin-1, Windows-1252's œ),
every character in the small font (lower case, digits, A B L R for the
buttons, its punctuation and accents), not empty (where a translation
keeps the English, it is the English: "= en"). Under each: its screens
(key presses) and its widest line of the box's 228 pixels; a newline (the
game wraps the words itself, and a newline isn't counted as a line: the
box can overflow) and a word wider than the box are warnings. ↑ ↓ move a
string, + adds one after it ("..."), × deletes it. A page other slides
show too (cutscene 10 shows cutscene 9's) is edited for them all, unless
"Give this one its own copy". A new page is
`cutscene<NN>.page<P>` (NN the cutscene's id, P the first number free)
and its strings go into the template and every `.po` after the last
cutscene entry, with the template's comments.

**The record** (the right column) is the slide's `struct cutscene_slide`:
which record the slide shows, its name; the picture (any cutscene picture
of the manifest); the wait (WaitForKeyPress's frames, 60 a second: 0
waits for a key) and the keys (A goes on, Start skips the slides that
also take keys); the fade in (from black or white) and out (none, to black
or white); the music cue (a song of the song table, SONG_*), "fade the
music out after it", the sound effect (SFX_*, or none) and "stop the
sound after it" (`rearm_sfx`: EndSlide's StopSfx). A record several
slides show is edited for all of them (the panel lists them), unless "Give
this one its own copy". A record or page no slide shows any more is
deleted with the last slide that showed it (the build refuses an unused
one).

**Pictures.** "Import a PNG" takes a 240x160 picture (another size is
refused, with its size: crop or scale it first) and makes it a cutscene
picture through the backend (`crashdata.cutscenes.edit.import_picture`):
on *its own colours* an indexed PNG keeps its palette and pixels, any
other is reduced to the GBA's 15-bit colours, exactly when it has 256 of
them or fewer, else to 256 by median cut (no dithering); on *this
picture's colours* each pixel is the nearest of the slide's picture's 256.
The result is shown, then "Replace" puts it in the picture (every slide
that shows it), or "As a new picture" adds it to the manifest
(`cutscenes/NN_<name>.png`, NN after the last; its class
`CutscenePictureNN`, defined with the ROM's in `cutscene_pictures`, and its
two lines of cxx_symbols.txt's graphics classes block, as
[data.md, "A new cutscene"](./data.md#a-new-cutscene) asks) and the slide
shows it (through its own record if others show its record).

Every change is a step of the undo stack (Ctrl+Z, Ctrl+Shift+Z or Ctrl+Y;
typing in one field is one step), which also says whether the cutscenes
are as saved. Ctrl+S saves. The page's address remembers the cutscene,
the slide and the language (`#mode=cutscenes&cutscene=1&slide=3&lang=fr`).

**Saving** sends one request with what changed since the cutscenes were
opened or last saved (`POST .../cutscenes/save`, "The editor's backend"):
cutscenes.json's value, the pages that changed (`null`: a page that goes),
the pictures whose pixels or colours changed, the new pictures. Nothing
else is written, and a file whose bytes come out the same isn't written:
cutscenes.json keeps its layout (a new slide is written like its
siblings); in the text's files only the entries that change are written
again, as msgcat writes them (`crashdata.text.po`: gettext's wrapping at
79 columns, which gives back every entry of XS's files byte for byte), the
others keep their bytes; a picture is written only when its pixels or its
colours change (a new palette alone replaces its PLTE chunk), as
crashdata's writers write one. The checks: the slides' records, pictures,
songs and sound effects, the pages' strings (the encoding, the font), then
the model's (`data-check`) and, when the text changes, the text's
(`text-check`: msgfmt, msgcmp, the generator's), every file put back on a
new error.

**Play this cutscene** runs `tools/preview.py --cutscene` ("Preview") in
the language shown: in the desktop app the PC port's window opens and the
game plays the cutscene at boot (then boots on); in the browser it runs
headless and shows a picture before each key press. It builds the saved
data.

**Checked against the game.** The PC port played the cutscenes headless
(A every 60 frames) and the editor (headless Chromium) showed the same
slides: the intro's fourth slide in French, the new game's sixth in German
and eighth in English, the legal notice, the mega-mix ending's second in
Italian, the Neo Cortex ending's sixth in Dutch, the 100% ending's second
in Spanish and N. Gin's in English: every pixel the same colour (5 bits a
channel), the picture and every glyph. So were an edit's (in a scratch
copy of the tree: a string added to N. Gin's page, a new slide with a new
picture imported from an RGB PNG and its own page, in English and
French), which data-check, text-check and the build passed.

**Not yet / not here:** the credits (`credits.json`, a scrolling stream of
lines, logos and font switches, not slides: its text is the `credits.*`
entries, edited in the "Data files" tab and the `.po` files); the fades
drawn in the view; drawing a picture (import one); a slide's sound and
music heard in the editor ("Play this cutscene" plays them); `rom_order`
(kept as it is; a new cutscene goes after the ROM's); N-Tranced's
cutscenes, which aren't data yet (games/n2/data has no cutscenes.json:
the tab says so).

## The music editor

The "Music" tab (#1167 stage 6) plays the songs and the sound effects
the way the game's GAX2 mixes them and edits what they are made of
([audio.md](./audio.md)): the songs' XMs (`games/xs/sound/songs/`), the
song table and what plays each song (`gax_manifest.json`'s `song_table`,
`music.json`, levels.json's themes), the sound-effect table
(`sfx_table.json`) and the samples (`samples/`, `sfx_samples/`, new
instruments in `instruments/`). It saves through the backend, which
checks a save like any other, builds the music and the sound effects as
the build does, and keeps all of it or none ("Saving the music", in "The
editor's backend").

**The player.** The editor has its own GAX2
(`apps/editor/src/lib/music/gax.ts`): lib/gax's replay and mixer
function for function (the Info handler's rows and orders, each
channel's pattern rows, instrument sequence, envelope, vibrato, sweep and
portamento, the resampler with its half-rate mode, the echo, the downmix
to 8 bits) with the integer widths and truncations of the C and of the
ARM routines as `platform/pc/src/gax_dsp.c` writes them. It plays what
the backend gives it, as `tools/gax_audio.py` builds it: a song's
`GaxSongData`, order lists and packed patterns and the two pools' instruments and samples
(`music/song`, `music/pool`), starting the song as `StartSong` does (3
sound-effect voices) and mixing a frame's block at a time, which the
page queues on the browser's audio at the song's mix rate. Nothing is
taken from the ROM.

**Checked against the game.** The PC port's GAX2 is lib/gax's matched C,
block for block mGBA's ([platforms.md, "The
sound"](./platforms.md)); `crashbandicootxs_pc --gax-trace TRACE --wav
OUT` replays a trace written for it (`tools/gax_render.py`: the song
started as StartSong starts it, `GAX_set_music_volume`, the sound
effects' `GAX_fx_ex` and `GAX_set_fx_volume`, a `GAX_play` a frame) and
writes what it mixes. Against it the editor's player mixed every one of
the 19 songs for 3 minutes (10,800 frames, 2.85 to 7.6 million samples
each) with every sample the same; two songs at the game's duck volumes
(192, 128) for a minute; sound effects over a level song (half-rate
voices), a menu song and the 42 kHz title cheat, with and without their
music, and all 87 of the set's instruments one after the other: every
sample the same. The page itself, in headless Chromium (each buffer it
queued captured): jungle, a sound effect, an edited cell played before
saving against the PC port built from the saved edit, a sound-effect
sample replaced by an imported WAV and a new sound effect with its own
sample, both before saving, against the PC port of the saved tree: every
sample the same. CI keeps it so: `apps/editor/test/gax_reference.json`
has the SHA-1s of every song's first 1200 frames and three sound-effect
sequences as the PC port mixes them, and of songs the editor changed
(`tools/gax_render.py`'s `edited_body`: a ROM song restructured with
alternating speeds and GAX2's own slides, cell edits of shared ROM
patterns, a channel added), saved by the backend in a copy of the tree
whose PC port renders them; the player's test plays the same edits
unsaved, as the editor does, and host.yml renders them again with the PC
port (`tools/gax_render.py check`).

**Songs.** The left column's first tab is the song table, in song id
order (`SONG_<KEY>`), each with what plays it: a level theme, a screen,
a cutscene slide. The middle is the selected song's patterns: the order
list along the top (a ROM song's order position p plays its XM pattern
p, a new song's its order list's; the position it loops back to is
marked), the pattern as rows of cells, note, instrument, volume column
and effect, as a tracker writes them. ▶ Play plays the song from its
start (or from the position shown, ▶ From N, or a double click on a
position: GAX2 has no seek, so the frames before it are mixed and
dropped), the row playing marked and followed ("follow"); "volume" is
`GAX_set_music_volume`, the game's duck volume (255 full), and the
toolbar gives how long the song plays before it loops. "Save as WAV"
writes the song as the PC port's `--wav` does: 8-bit mono at its mix
rate.

**Cells.** A cell selected (the mouse, the arrow keys; Page Up and Down
change the order position) is edited on the right: its note (C-4, C#4,
=== key off), instrument (the XM's: `Instrument N` is the pool's N),
volume column, effect and parameter; Delete clears it. What GAX2 makes
of it is shown as the build makes it (crashdata.audio.gax's XM import,
[audio.md, "The XM's cells"](./audio.md#the-xms-cells)): the note, the
one effect of the row (1xx, 2xx, 3xx, 7xy alternating speeds, Ax0/A0y,
Cxx, Dxx, EDx, Fxx under 0x20, else the volume column's; a volume column
01 makes the effect column GAX2's own command and parameter), and what
the build drops (another effect, a second effect in the volume column),
marked in the pattern too. The XM says everything a pattern's bytes hold
(the ROM's alternating speeds, its volume slides and volumes, its drums'
notes), so every pattern of the ROM's songs packs back to the ROM's
bytes and an edited one plays what its cells say. A ROM song stores a
channel's pattern once for every order position (and channel) that plays
the same notes: the panel says which, and an edit is made in all of
them, as the save writes it (with their transposes). The edited song
plays as the build will build it (`POST music/song` with the edits): an
edit is heard before it is saved.

**Structure.** Above the order list, the song's structure, each change a
step of the undo stack: the order position shown (the pattern it plays,
a new position after it, taken out, moved earlier or later, "loop here"
for the position the song loops back to), the pattern it plays (its rows,
a row inserted or taken out at the cell selected, a new empty pattern or
a copy played after the position, cleared, taken out with the positions
that play it), a channel added or the selected cell's taken out, and the
speed the song starts at (GAX2 starts every song at 6, so the build puts
an `Fxx` with another one on the first row, unless the first row sets
one: the bar says when it does). The XM's rules are the build's: one
pattern length a song (the longest; a shorter pattern ends with the
pattern break the build adds), the BPM unused (one tick a frame), and the
work RAM StartSong gives GAX2 limits the channels at a mix rate (a save
the build refuses is refused with its reason). A ROM song is built with
its recorded scaffold, its patterns shared between order positions and
channels with transposes, which gives back the ROM's bytes; its first
structural change makes it a song built from its XM (each order
position's pattern its own, the sharing found again by content; the
save takes the scaffold out of the manifest), and the bar says so. A
changed song plays as the build will build it, before it is saved.

A new song ("Import an XM as a new song…") opens and plays before it is
saved: the backend reads its XM and builds it, with the new instruments
it brings, as the build will (`POST music/patterns`, `music/song` and
`music/pool` with the unsaved new songs), and its cells and structure
are edited like any song's.

**Where songs play.** The right column also has every screen's song
(music.json: the title, the level select, the credits, the bonus rooms,
the invincibility drums, ...) and every level theme's (levels.json), any
song of the table; a cutscene slide's song is the Cutscenes tab's.
"Import an XM as a new song…" adds one at the table's end (its key, its
comment: `SONG_<KEY>`, the next id, docs/audio.md "Adding a song"); the
save builds it, so an XM GAX2 can't play is refused with the encoder's
reason. A new song nothing plays can be taken out of the table again; the
ROM's stay.

**Sound effects.** The second tab is the sound-effect table, in id order.
The middle shows the selected effect's sample and plays it as `PlaySfx`
does: its instrument on a voice through `GAX_fx_ex` with its priority,
`GAX_set_fx_volume` with its volume, over a song chosen for its mix rate
and half-rate voices (the level songs'), its music muted unless "with its
music"; "Play the WAV" plays the file as it is. On the right its entry:
its name (locked when the code or a cutscene names `SFX_<NAME>`), the
slot of the set or the new instrument it plays, priority, volume (8.8;
`PlaySfx` gives the voice at most 255) and comment, and where it is
named. "Replace it with a WAV" gives its slot a new sample, resampled to
the rate the slot plays at (gGaxPeriodTable / 2048: 8002.6 Hz for every
slot of the ROM), so it keeps its pitch; every effect on that slot plays
it. "New sound effect…" adds an entry at the end, on a slot or with its
own sample from a WAV (`instruments/<name>.json` and
`sfx_samples/<name>.wav`, docs/audio.md "Adding a sound effect"), which
plays before it is saved as the build will make it; the last entry can
be deleted when nothing names it.

**Instruments.** The third tab lists the music pool's instruments
(`gax_manifest.json`'s records, `struct GaxChannelInstrument`; a song's
XM plays instrument N as `Instrument N`, and an instrument is shared: an
edit is heard in every song that plays it). The middle draws the
selected one's volume envelope, edited there (a point dragged, one added
with a click, one taken out with a double click; the sustain point and
the loop marked), its first wave row's sample with its loop, and plays a
note on it as a song plays it ("▶ Play the note": a one-channel song
with the note and a key off after the rows asked for, through the
editor's GAX2, with the pool the backend builds with the unsaved edits).
The right has its record: the vibrato (delay, depth, speed); each of its
four wave rows' sample (the pool's, the new ones too), tune (as a
relative note and a finetune in 32nds: GAX2's tune is 1/32 semitones),
start, and loop (none, forward or ping-pong, its start and length; a
sweep's window, length, step and rate where the ROM has one); its
sequence (a step every N ticks: each step's note, fixed pitch, wave row
and two effects); and the envelope's points (tick, volume 0-255), sustain
and loop. The backend checks a record (crashdata.audio.pool: the fields
in range, a wave row the edit changes inside its sample, the envelope's
ticks increasing from 0) and works out the envelope's slopes
(`GaxEnvelopePoint.slope`, Q8 a tick, as the ROM's; a point the edit
doesn't move keeps its recorded one). GAX2 mixes mono, so an instrument
has no panning envelope (the record's second envelope, `unknown12`, is
one the engine never reads), and the bytes it never reads aren't edited.

**Samples.** The fourth tab lists the music pool's samples (with the
instruments and songs that play each, and those no instrument plays) and
the sound-effect set's, each drawn and played; a new sound for one comes
from a WAV (any PCM or 32-bit float, mixed to mono, resampled to the
rate asked for, 8 bits: `music/import`), at least as long as its
instruments' starts and loops reach (the save refuses a shorter one).
"New sample from a WAV…" adds one to the music pool (`samples/NN.wav`,
the pool's next number, and the manifest's `samples` entry), which a wave
row of an instrument then plays; a sample no instrument plays can be
deleted (its WAV and its entry). A music sample's WAV rate is cosmetic:
an instrument plays it at the note's pitch with its own tune.

Every change is a step of the undo stack (Ctrl+Z, Ctrl+Shift+Z or
Ctrl+Y); Ctrl+S saves, space plays and stops. The page's address keeps
the song (`#mode=music&song=jungle`).

**Not yet / not here:** a new song's settings (`songs/<key>.json`) and
a ROM song's (its mix rate, volume and echo taps: the "Data files" tab
edits the manifest); a new instrument in the music pool's records (a
new song brings its own, in its XM or as `instruments/<name>.json`, and
an XM names a pool instrument as `Instrument N`, which the editor's cell
panel only offers from the XM's instruments); a new song's own XM
instruments (they come from its XM); a sound-effect instrument's record
(`gax_sfx_manifest.json`, the "Data files" tab); the game's timing around the sound (fades, ducks,
sound effects as the gameplay triggers them: the player plays a song or
one sound effect, with the music volume asked for); GAX2's jingle player
and low-pass filter, which the game doesn't use; N-Tranced's sound,
which isn't data yet (no `games/n2/sound/`: the tab says so).

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
| `GET /api/projects/ID/room?path=ROOM` | a room's folder (`games/<game>/data/levels/<level>/<slot>`) for the level editor: `{path, room, palette, layers, legend, entityTypes, sprites}` (room.json's value, its 256 BGR555 colours, each layer's size, priority, scroll scale and tile set, the collision legend, entity_types.json's types, a sprite frame per type) |
| `GET /api/projects/ID/room/layer?path=ROOM&layer=L` | a layer's data: its width and height (u32 LE), then a BG picture's palette indices (a byte a pixel) or the collision layer's cells (u16 LE); gzipped when asked |
| `GET /api/projects/ID/room/tiles?path=ROOM&layer=L` | a BG layer's tile set as the build makes it: `{tileset, bpp, count, tiles, banks, cells}` (tiles: 64 bytes each, base64; cells: the layer's, u16 LE, base64) |
| `GET /api/projects/ID/sprite?bank=N&frame=K[&palette=P]` | a sprite bank's frame as a PNG, index 0 transparent, in OBJ palette P or its own |
| `POST /api/projects/ID/room/save` `{path, room?, layers?, collision?, palette?}` | saves a room's edits, all or none (below) |
| `POST /api/projects/ID/room/new` `{from, folder, empty?, size?, attach?}` | makes a room (`room_ops.new_room`): a copy of `from`, or an empty one laid out like it, `size` cells; `attach` `{level, as: "rooms" \| "bonus" \| "gem_path", index?}`; the answer's `report`: its number, symbols, warnings; `path` |
| `POST /api/projects/ID/room/delete` `{path}` | deletes a room no level plays (`room_ops.delete_room`) |
| `POST /api/projects/ID/room/resize` `{path, edges: {left, right, top, bottom}, check?}` | resizes a room (`room_ops.resize_room`); `report`: the sizes before and after, each layer's move and cut cells, the entities and links cut, warnings; with `check` nothing is written |
| `GET /api/projects/ID/cutscenes` | the cutscene editor's view (`crashdata.cutscenes.edit.view`): `{cutscenes, pictures, pages, pageOrder, languages, songs, sfx, font, box, screen, encoding, references, numberFields}` (cutscenes.json's value, the manifest's cutscene pictures, every page's strings in the six languages, the song and sound-effect names, the small font's characters, metrics, glyph pixels and colours, the text box, what plays each cutscene); 404 for a game whose cutscenes aren't data (N-Tranced) |
| `GET /api/projects/ID/cutscenes/picture?path=cutscenes/P.png` | a cutscene picture: its width and height (u32 LE), its 256 colours (u16 LE), then a byte a pixel; gzipped when asked |
| `POST /api/projects/ID/cutscenes/import` `{png, palette?}` | a PNG (base64) made into a cutscene picture, on its own colours or mapped to `palette`: `{width, height, palette, pixels, colours, exact, how}`; 422 if it isn't 240x160 |
| `POST /api/projects/ID/cutscenes/save` `{cutscenes?, pages?, pictures?, newPictures?}` | saves the cutscene editor's edits, all or none (below) |
| `GET /api/projects/ID/music` | the music editor's view (`crashdata.audio.edit.view`): `{songs, screens, themes, sfx, sfxNoneComment, sfxSlots, samples, instruments, mixRates, limits, files}` (the song table, each song's XM in brief and what plays it: the level themes, the screens, the cutscene slides; the screens' and themes' songs; the sound-effect table with where each name is used, code and cutscenes; the sound-effect set's instruments and the rate each plays its sample at; the music pool's samples, what plays them and the shortest sample their loops take; the new instruments); 404 for a game whose sound isn't data (N-Tranced) |
| `GET /api/projects/ID/music/pool` | what the music editor's player needs besides a song (`player_pool`): GAX2's tables (`lib/gax/data/gax_tables.c`: gGaxPeriodTable, gGaxVibratoTable, the mix rates), the music pool's and the sound-effect set's instruments as `struct GaxChannelInstrument` (the new songs' and sound effects' new instruments too) and their samples, `{tables, music, sfx, sfxSlots}`; gzipped when asked |
| `GET /api/projects/ID/music/song?key=K` | a song as `tools/gax_audio.py` builds it (`player_song`), as GAX2_init sees it: its `GaxSongData` fields, the mixer's echo taps (`dsp`), the packed pattern streams (`patterns`, base64) and each channel's order list (`orders`: `[offset in patterns, transpose]`) |
| `POST /api/projects/ID/music/song` `{key, edits?, newSongs?, structures?, patterns?}` | the same, built with unsaved edits (`unsaved_songs`): cell edits of its XM (`edits`), and the songs' unsaved new songs, structures and cell edits as the save's; a new song's key plays an unsaved new song |
| `POST /api/projects/ID/music/pool` `{newSongs?, structures?, patterns?, instruments?, samples?, newSamples?, deleteSamples?}` | the player's pool with those edits: the instruments' edited records, the new samples (and those taken out), an unsaved new song's new instruments, numbered as the build will number them |
| `GET /api/projects/ID/music/patterns?key=K` | a song's XM for the pattern view: `{rows, cells, channels, order, restart, speed, playOrder, loopOrder, instruments, scaffold, groups?, transposes?}` (each pattern's cells, 5 bytes a cell, base64; for a ROM song that keeps its scaffold, which order positions share a stored pattern) |
| `POST /api/projects/ID/music/patterns` `{key, xm}` | the same of an unsaved new song's XM (base64) |
| `POST /api/projects/ID/music/import` `{wav, rate?}` | a WAV (base64: PCM 8 to 32-bit or 32-bit float, any channels) made into a GAX2 sample, mixed to mono, resampled to `rate` (linearly) and rounded to 8 bits: `{wav, length, rate, source, peak, clipped, how}` |
| `POST /api/projects/ID/music/save` `{sfxTable?, music?, themes?, songComments?, newSongs?, removeSongs?, structures?, patterns?, samples?, newInstruments?, instruments?, newSamples?, deleteSamples?}` | saves the music editor's edits, all or none (below) |

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

**Saving a room** (`room/save`, `crashdata.levels.room_edit`) is the same
transaction over the room's files: `room` is room.json's new value (the
steps above), `layers` the painted cells of each BG layer (`{"bg1": [[cx,
cy, base64 of the cell's 64 palette indices], ...]}`; a BG1-3 cell's
pixels must be of one bank), `collision` the collision cells (`[[cx, cy,
value], ...]`, a value of the legend) and `palette` the room's 256
colours, which every layer picture gets. A picture whose pixels and
palette come out as they were isn't written; one with a new palette only
gets its PLTE chunk replaced (the pixels' bytes stay); painted cells
write the picture as crashdata's writers do (`write_layer_png`'s
settings, `collision.write_image`). The new files are checked by the
model's checks (data-check reads the pictures: their palettes, the
collision image's size and colours) and all go back on an error.

**New, deleted and resized rooms** (`room/new`, `room/delete`,
`room/resize`, `crashdata.levels.room_ops`) are plans of files to write
and remove (room.json, the pictures, levels.json, tileset_order.json,
cxx_symbols.txt's room classes block), saved the same way: a new room's
folder is made first and goes again on a refusal; a deleted file is put
back.

**Saving the cutscenes** (`cutscenes/save`, `crashdata.cutscenes.edit.plan_save`)
is the same transaction over cutscenes.json, the text's files, the
pictures, and for a new picture the manifest and cxx_symbols.txt (the one
file outside `games/<game>/` a save writes: its graphics classes block's
two lines for the picture's class, which `gfx_manifest.py check` holds
equal to what `gfx_manifest.py symbols` writes). `cutscenes` is
cutscenes.json's new value (the steps above), `pages` the pages whose
strings change (`{"cutscene01.page0": [{"en", "fr", "de", "es", "it",
"nl"}, ...]}`, `null` for a page that goes), `pictures` the pictures whose
pixels or colours change (`{"cutscenes/12_coco_on_screen.png": {"palette":
[256 BGR555], "pixels": base64 of 240x160 bytes}}`), `newPictures` the new
ones (`[{"path": "cutscenes/24_lab.png", "about", "palette", "pixels"}]`).
Before anything is written, the editor's own checks: each slide's record,
picture, song (SONG_*, the song table's) and sound effect (SFX_*), each
page shown by a slide and each slide's page there, each string in every
language, in the encoding and the font; a refusal's diagnostics point into
cutscenes.json (`/slides/0/song`) or at a string (`/cutscene01.page0/0/de`,
file `text`). The text's files are edited in place (`crashdata.text.po`),
and when they change the model's checks are run with the text's
(`data_check.py --json`, text included: msgfmt, msgcmp, the generator's
encoding and font checks), so a save the build or `make text-check` would
refuse is put back.

**Saving the music** (`music/save`, `crashdata.audio.edit.plan_save`) is
the same transaction over the sound's files (`games/xs/sound/`) and
levels.json. `sfxTable` is sfx_table.json's new value, `music`
music.json's (the screens are the code's, `SCREEN_SONG_<SCREEN>`: only
their songs change), `themes` `{THEME_<NAME>: song}` (levels.json's
themes' `"song"`), `songComments` `{song: comment}` (the song table's),
`newSongs` `[{key, xm, comment}]` (an XM, base64, written as
`songs/<key>.xm` and added at the song table's end: `SONG_<KEY>` is the
next id, [audio.md, "Adding a song"](./audio.md#adding-a-song)),
`removeSongs` (a new song taken out of the song table; its XM stays, and
no table entry builds it), `structures` `{song: {channels, patterns:
[{rows, cells}], order, restart, speed}}` (the song's XM with new
patterns, rows, channels, order list, loop point or speed: a ROM song's
scaffold then leaves the manifest, below; a new song's of this save too),
`patterns` `{song: [{pattern, row, channel, cell: [note, instrument,
volume, effect, parameter]}]}` (cell edits of the song's XM), `samples` `{"samples/NN.wav" or "sfx_samples/NN.wav":
base64 WAV}` (new sounds for the tree's samples, 8-bit mono as
`music/import` gives them) and `newInstruments` `[{name, wav, rate?,
volume?, comment?}]` (`instruments/<name>.json` with its WAV,
`sfx_samples/<name>.wav`, for a new sound effect: an entry's
`"instrument"`, [audio.md, "Adding a sound
effect"](./audio.md#adding-a-sound-effect)), `instruments` `{N: record}`
(the music pool's instrument N, its `gax_manifest.json` record with its
envelope, vibrato, wave rows or sequence edited), `newSamples` `{NN:
base64 WAV}` (new music pool samples, `samples/NN.wav` and the
manifest's `samples` entry, numbered after the pool's) and
`deleteSamples` `[NN]` (music pool samples no instrument plays, their
WAV and entry taken out: put back like any file if the save is
refused). Before anything is written,
the editor's own checks: the sound effects' names (one entry a name, and
every `SFX_<NAME>` the code or a cutscene names kept), slots (the set's
1-87) and instruments, the volumes; the screens' and themes' songs (in
the song table); a new song's key and XM; that a ROM song stays in the
table and a removed song isn't played; each cell (bytes, an instrument
the module has); a sample's WAV (8-bit mono) and length, at least what
the loops and starts of the instruments that play it reach. A refusal's
diagnostics point into sfx_table.json (`/entries/12/slot_id`),
music.json, levels.json or gax_manifest.json (`/song_table`).

A cell edit of a XM is written there alone: the pattern it is in is
packed again (the way the songs' XMs pack theirs: every pattern of the
tree's songs packs back to its own bytes), the other patterns and the
instruments keep theirs. A ROM song stores a pattern once for the order
positions and channels that play the same notes (`pattern_groups`, with
their transposes): an edit of one cell is made in each of them (the note
moved by their transposes), as the ROM would share it; the build packs it from
its cells, as it packs every pattern ([audio.md, "The XM's
cells"](./audio.md#the-xms-cells)).
A structure is written in the XM's header (its order list, restart
position, channels, pattern count, speed) and patterns; a pattern whose
rows and cells are the old one's of its number keeps its bytes, the
instruments theirs. A ROM song whose structure changes loses its
scaffold: its `channel_transposes`, `pattern_groups`,
`pattern_group_order`, `channel_order`, `num_items` and the four
structure fields (`num_channels`, `num_rows_per_pattern`,
`num_patterns_per_channel`, `loop_point`) leave the manifest, and it is
built from its XM like a new song ([audio.md, "A ROM song without its
scaffold"](./audio.md#a-rom-song-without-its-scaffold)), with the
manifest's settings (mix rate, volume, echo taps, title bytes) and its
place in the music block.
After the files are in place the model's checks build the music and the
sound effects (`music-build`, `sfx-build`: a XM the encoder refuses, a
WAV, an instrument), and every file goes back on a new error. A value
saved unchanged writes nothing, a XM whose cells come out the same and a
WAV with the same bytes aren't written.

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
helpers, the level editor's coordinates and parallax, undo stack,
painting, entity order and links, palette rule and record views; the
cutscene editor's text (the encoding, the font, the screens of XS's own
pages as crashdata gives them, glyph for glyph), rendering, timeline
edits and their rules, undo stack and save body; the music editor's GAX2
against the PC port's (`gax_reference.json`: every song's first 1200
frames and three sound-effect sequences, sample for sample, on the data
crashdata gives), the XM cells and what the build makes of them
(crashdata's own mapping), a shared ROM pattern's edit as the save
writes it, a new sound effect's instrument as the build makes it, the
save body),
`pnpm build`, the backend's Python tests (every data file of both games
written back unchanged, a change in the tool's layout, the refusals, the
token; every room of both games opened and saved back unchanged through
the room API, a room edit saved and put back, the tile set drawing the
layer's picture, the room save's refusals; every cutscene, page and
cutscene picture saved back unchanged with nothing written (`git diff
--exit-code`), an edit in a scratch copy of the tree (text, timing, a
picture imported as a new one and another replaced) that data-check and
the text's checks pass, which `CRASH_EDITOR_TEST_BUILD=1` also builds, the
cutscene save's refusals; crashdata's gettext writer giving back every
entry of both games' text files; the sound-effect table, the screens'
and themes' songs, every cell of every song's XM and every sample saved
back unchanged with nothing written (`git diff --exit-code`), in XS's
tree and in the sample mod's (applied to a scratch folder), an edit in a
scratch copy (a sound effect's volume and priority, a new one with its
own sample imported from a WAV, a sound-effect sample replaced, the
title screen's and a theme's song, a note of a shared ROM pattern) that
data-check passes and `CRASH_EDITOR_TEST_BUILD=1` builds, the music
save's refusals, every pattern of the songs' XMs packed back to its own
bytes, each song's patterns as the build lays them out), and a smoke
test of
`tools/editor_server.py` as `make editor` runs it, through the typed
client.

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
