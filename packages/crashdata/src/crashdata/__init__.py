"""crashdata: the games' data as Python, for the build's tools and the editors (#1167).

By domain, each reading the editable sources (games/<game>/...), checking
them, extracting them from a ROM and converting them to what the build
compiles:

- common: the games and their directories (common.game), JASC-PAL
  palettes (common.jasc_pal);
- levels: the rooms, the level table and themes, the actor categories
  (docs/levels.md), and the collision images (levels.collision);
- graphics: the graphics manifest (graphics.manifest, docs/graphics.md)
  and the codecs of its kinds (sprite banks, framed sheets, OBJ sheets,
  BG pictures, boss pictures, zero-run frames);
- cutscenes: games/xs/data/cutscenes.json (docs/data.md, "Cutscenes");
- audio: the GAX music and sound effects (audio.gax, audio.sfx_table,
  docs/audio.md);
- text: the games' text, gettext .pot/.po (docs/text.md);
- mods: a mod's copy of the tree and its edits (docs/modding.md).

The commands that run these are packages/buildtools' (tools/<name>.py).
"""
