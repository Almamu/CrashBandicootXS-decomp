# Editors: the data's machine-facing contract

The level, cutscene and music editors (#1167 stages 4-6: a TypeScript
web app and a Tauri desktop shell) edit the same files a modder edits by
hand (games/<game>/data/, graphics/, sound/, text/; [modding.md](./modding.md))
and run the same tools. This page is what they can rely on (#1167 stage 3,
#1093 part 1):

- **JSON Schemas** of every data file, generated from crashdata's model
  ("Schemas");
- **machine-readable errors**: `--json` on the data tools, and one error
  format ("Errors");
- **`make data-check`**: every check of the data, without building
  ("Checking the data").

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

## Errors

Every data tool reports a problem as a **diagnostic**
(`crashdata.common.diagnostics.Diagnostic`):

| field | what |
|---|---|
| `file` | the file, from the tree's root (or as given) |
| `pointer` | where in it: a JSON pointer (RFC 6901), `/entities/3/param`; `null` when the check doesn't know |
| `line`, `col` | the pointer's place in the file (1-based), or a JSON syntax error's; `null` when unknown |
| `severity` | `error` (the build stops) or `warning` (it goes on) |
| `code` | which check: `schema`, `json-syntax`, `entity-type`, `entity-param`, `link`, `rooms`, `themes`, `categories`, `graphics`, `text`, ... |
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
   entities' order, each entity's parameter record, the links' ids), the
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
