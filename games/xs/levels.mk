# The rooms' level data (docs/levels.md): data/levels/ holds the editable
# sources, one folder per room, data/levels/<level>/<slot>/ (room.json, each
# BG layer as its whole picture, bg0-3.png, and the collision layer as an
# image, collision.png), listed by levels.json's "rooms"; tools/levels.py
# builds them back:
# - the BG tile sets, each generated from the pictures of the layers that
#   name it, and with them every BG layer's cells (levels.py tileset, into
#   build/.../data/levels/tilesets/ and <room>/<layer>.map.bin);
# - each room's level asset (the per-layer chunk streams), raw or
#   LZ77-packed by gbagfx, as C initializer bytes (tools/bin2c.py);
# - the C definitions of the two room-data regions (each room's palette
#   read from its layer pictures, which must all carry the same one), which
#   src/game/level/data/level_rooms_*.cpp #include, and of their rooms' assets, which
#   src/game/level/data/level_assets.cpp #includes. The level data that isn't
#   the ROM's goes with them, in place (docs/levels.md, "Adding a room"):
#   the rooms in no region of levels.json and the tile sets only they use
#   after the last region's rooms, their assets after the ROM's.
LEVELS_BUILDDIR := $(OBJ_DIR)/data/levels
LEVEL_ROOMS := $(shell python3 tools/levels.py rooms)
LEVEL_PACKED_ROOMS := $(shell python3 tools/levels.py rooms --packed)
LEVEL_ROOM_JSONS := $(foreach room,$(LEVEL_ROOMS),data/levels/$(room)/room.json)
# The rooms' layer pictures, which also carry each room's palette.
LEVEL_BG_PNGS := $(foreach room,$(LEVEL_ROOMS),$(wildcard data/levels/$(room)/bg?.png))
LEVEL_REGIONS := level_rooms_main_bonus level_rooms_bosses_gem_paths

LEVEL_TOOLS := tools/levels.py tools/collision_bmp.py data/levels/collision_legend.json
# A room's BG layers (bg0-3) and their cells.
level_bg_layers = $(patsubst data/levels/$(1)/%.png,%,$(wildcard data/levels/$(1)/bg?.png))
level_bg_maps = $(foreach l,$(call level_bg_layers,$(1)),$(LEVELS_BUILDDIR)/$(1)/$(l).map.bin)

# The BG tile sets (docs/levels.md, "Tile sets"): one per symbol the rooms'
# layers name, generated from their pictures together with those layers'
# cells. tilesets.mk, generated from levels.json and the rooms, has a rule
# per set: a stamp that depends on the pictures and room.json of every room
# using the set (and tileset_order.json), whose recipe writes the set's
# tiles, its size header and its layers' cells, each only if it changed;
# the outputs depend on the stamp with an empty recipe, so an unchanged map
# doesn't rebuild its room. It also lists the new tile sets
# (LEVEL_NEW_TILESETS_8BPP, _4BPP), which no room of the ROM uses.
$(LEVELS_BUILDDIR)/tilesets.mk: data/levels/levels.json $(LEVEL_ROOM_JSONS) $(wildcard data/levels/tileset_order.json) \
		tools/levels.py
	python3 tools/levels.py tilesets-mk $@ --build $(LEVELS_BUILDDIR)
include $(LEVELS_BUILDDIR)/tilesets.mk

$(LEVELS_BUILDDIR)/tilesets/%.bin.inc: $(LEVELS_BUILDDIR)/tilesets/%.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@

# The ROM's tile sets, defined by hand in src/game/level/data/ (the BG0 ones raw 8bpp
# tagged assets, sized by tilesets/<set>.h; the BG1-3 ones LZ77 4bpp).
LEVEL_TILESET_DEFS = $(foreach t,$(1),$(LEVELS_BUILDDIR)/tilesets/$(t).bin.inc $(LEVELS_BUILDDIR)/tilesets/$(t).h)
$(C_BUILDDIR)/game/level/data/level_tilesets_jungle_snow_underwater.o: $(call LEVEL_TILESET_DEFS,gJungleBg0Tiles gSnowBg0Tiles gUnderwaterBg0Tiles)
$(C_BUILDDIR)/game/level/data/level_tilesets_sewer_space.o: $(call LEVEL_TILESET_DEFS,gSewerBg0Tiles gSpaceBg0Tiles)
$(C_BUILDDIR)/game/level/data/level_bg_tiles.o: $(foreach t,gSnowBgTiles gJungleBgTiles gTempleBgTiles gSewerBgTiles \
	gSpaceBgTiles gUnderwaterBgTiles,$(LEVELS_BUILDDIR)/tilesets/$(t).bin.lz.inc)
$(foreach o,level_tilesets_jungle_snow_underwater level_tilesets_sewer_space level_bg_tiles,\
	$(eval $(call data_obj,$(o)): CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))

define LEVEL_ASSET_RULE
$(LEVELS_BUILDDIR)/$(1)/asset.bin: data/levels/$(1)/room.json $(call level_bg_maps,$(1)) \
		$$(wildcard data/levels/$(1)/collision.png) $(LEVEL_TOOLS)
	@mkdir -p $$(dir $$@)
	python3 tools/levels.py asset data/levels/$(1) $$@ --build $(LEVELS_BUILDDIR)
endef
$(foreach room,$(LEVEL_ROOMS),$(eval $(call LEVEL_ASSET_RULE,$(room))))

$(LEVELS_BUILDDIR)/%.lz: $(LEVELS_BUILDDIR)/% | $(GFX)
	$(GFX) $< $@

# The asset bytes: a packed one trimmed to its LZ77 stream (the zero bytes
# gbagfx pads it with are the next asset's word alignment), a raw one whole.
$(LEVELS_BUILDDIR)/%.lz.inc: $(LEVELS_BUILDDIR)/%.lz tools/bin2c.py
	python3 tools/bin2c.py $< $@ --lz
$(LEVELS_BUILDDIR)/%/asset.bin.inc: $(LEVELS_BUILDDIR)/%/asset.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@

# The room data takes the layers' asset offsets and chunk grids from the
# asset.layout.json `levels.py asset` writes next to each asset.bin.
# The last region also defines the new rooms and the new tile sets (a BG0
# one sized by its built tiles).
$(LEVELS_BUILDDIR)/level_rooms_%.inc: data/levels/levels.json $(LEVEL_ROOM_JSONS) $(LEVEL_BG_PNGS) \
		$(foreach room,$(LEVEL_ROOMS),$(LEVELS_BUILDDIR)/$(room)/asset.bin) \
		$(foreach t,$(LEVEL_NEW_TILESETS_8BPP),$(LEVELS_BUILDDIR)/tilesets/$(t).bin) \
		data/levels/entity_types.json $(LEVEL_TOOLS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py c level_rooms_$* $@ --build $(LEVELS_BUILDDIR)

level_asset_incs = $(foreach room,$(filter-out $(LEVEL_PACKED_ROOMS),$(1)),$(LEVELS_BUILDDIR)/$(room)/asset.bin.inc) \
	$(foreach room,$(filter $(LEVEL_PACKED_ROOMS),$(1)),$(LEVELS_BUILDDIR)/$(room)/asset.bin.lz.inc)

$(LEVELS_BUILDDIR)/level_assets.inc: data/levels/levels.json $(LEVEL_ROOM_JSONS) tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py assets $@

$(foreach r,$(LEVEL_REGIONS),$(eval $(call data_obj,$(r)): $(LEVELS_BUILDDIR)/$(r).inc))
$(foreach r,$(LEVEL_REGIONS),$(eval $(call data_obj,$(r)): CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))
$(call data_obj,$(lastword $(LEVEL_REGIONS))): \
	$(foreach t,$(LEVEL_NEW_TILESETS_8BPP),$(LEVELS_BUILDDIR)/tilesets/$(t).bin.inc) \
	$(foreach t,$(LEVEL_NEW_TILESETS_4BPP),$(LEVELS_BUILDDIR)/tilesets/$(t).bin.lz.inc)
$(C_BUILDDIR)/game/level/data/level_assets.o: $(LEVELS_BUILDDIR)/level_assets.inc $(call level_asset_incs,$(LEVEL_ROOMS))
$(C_BUILDDIR)/game/level/data/level_assets.o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)

# The room classes (docs/levels.md, "Room classes"): rooms.hpp declares a
# class per room of levels.json, whose BG palette, chunk grids and level
# asset the definitions above name as static members. Written only if it
# changed, behind a stamp, like the constants headers below.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/rooms.hpp
$(GENERATED_INCLUDE_DIR)/rooms.hpp.stamp: data/levels/levels.json $(LEVEL_ROOM_JSONS) tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py classes $(GENERATED_INCLUDE_DIR)/rooms.hpp
	@touch $@
$(GENERATED_INCLUDE_DIR)/rooms.hpp: $(GENERATED_INCLUDE_DIR)/rooms.hpp.stamp
	@test -f $@ || python3 tools/levels.py classes $@

# The animation tables (docs/levels.md, "Animation tables"):
# data/levels/anim_tables/<name>.json, a type's records (one per actor
# kind) and the keyframes and frames they point at. `levels.py anim-table
# NAME` writes the ROM's table NAME, which its family's C file #includes
# after the palettes; `levels.py categories` the ones only a category
# names. Its frames are named by the sprite sheet's frame folders (a
# framed_sheet of the manifest, FRAMED_SHEETS) and the player frames' PNG,
# which it checks them against.
ANIM_TABLE_DEPS := $(wildcard data/levels/anim_tables/*.json) include/constants/actor_kinds.h tools/levels.py \
	tools/framed_gfx.py graphics/manifest.json tools/gfx_manifest.py $(foreach s,$(FRAMED_SHEETS),graphics/$(s)/sheet.json \
	$(wildcard graphics/$(s)/*/*.png)) graphics/vehicles/polar/player.png graphics/vehicles/jetpack/player.png

# The actor categories (docs/levels.md, "Actor categories"): levels.json's
# "categories" and their spawn lists, data/levels/spawns/*.json (whose kinds
# are named by constants/actor_kinds.h). `levels.py categories` writes
# ActorCategory::descriptors (and the spawn lists that aren't the ROM's),
# which src/engine/actor/data/actor_categories.cpp #includes; `levels.py
# spawns SYMBOL` the ROM's spawn list SYMBOL, which its C file #includes
# where the ROM has it (nothing if no category has it); `levels.py constants
# categories` the CATEGORY_* constants.
CATEGORY_DEPS := data/levels/levels.json $(wildcard data/levels/spawns/*.json) include/constants/actor_kinds.h \
	tools/levels.py
# A category may name its own graphics (docs/levels.md, "Category
# graphics"): new ones (graphics/manifest.json entries with no ROM address)
# are defined in actor_categories.inc, whose generator reads their PNGs'
# sizes and their palettes; their built .inc files
# (CATEGORY_GRAPHICS_NEW_INCS, graphics_manifest.mk) are #included there.
# So are its own animation table and the frame offsets of its sheet
# ("Category sprite sheets").
$(LEVELS_BUILDDIR)/actor_categories.inc: $(CATEGORY_DEPS) graphics/manifest.json $(CATEGORY_GRAPHICS_NEW) \
	tools/gfx_manifest.py tools/jasc_pal.py $(ANIM_TABLE_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py categories $@
$(LEVELS_BUILDDIR)/spawns/%.inc: $(CATEGORY_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py spawns $* $@
$(C_BUILDDIR)/engine/actor/data/actor_categories.o: $(LEVELS_BUILDDIR)/actor_categories.inc \
	$(CATEGORY_GRAPHICS_NEW_INCS) $(foreach s,$(FRAMED_SHEETS),$(GRAPHICS_BUILDDIR)/$(s)_frames.h)
$(C_BUILDDIR)/game/vehicles/polar/data/polar_cell_anim.o: $(LEVELS_BUILDDIR)/spawns/gCategory0SpawnTable.inc
$(C_BUILDDIR)/game/vehicles/polar/data/polar_spawn_tables.o: \
	$(foreach t,gCategory1SpawnTable gCategory2SpawnTable,$(LEVELS_BUILDDIR)/spawns/$(t).inc)
$(C_BUILDDIR)/game/vehicles/jetpack/data/jetpack_cell_anim.o: $(LEVELS_BUILDDIR)/spawns/gCategory3SpawnTable.inc
$(C_BUILDDIR)/game/vehicles/jetpack/data/jetpack_sky_pictures.o: \
	$(foreach t,gCategory4SpawnTable gCategory5SpawnTable gCategory6SpawnTable,$(LEVELS_BUILDDIR)/spawns/$(t).inc)
$(foreach o,engine/actor/data/actor_categories game/vehicles/polar/data/polar_cell_anim \
	game/vehicles/polar/data/polar_spawn_tables game/vehicles/jetpack/data/jetpack_cell_anim \
	game/vehicles/jetpack/data/jetpack_sky_pictures,$(eval $(C_BUILDDIR)/$(o).o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))

# The ROM's animation tables.
$(LEVELS_BUILDDIR)/anim_tables/%.inc: $(ANIM_TABLE_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py anim-table $* $@
$(foreach f,polar jetpack,$(eval $(C_BUILDDIR)/game/vehicles/$(f)/data/$(f)_anim_family.o: \
	$(LEVELS_BUILDDIR)/anim_tables/$(f).inc))
$(foreach f,polar jetpack,$(eval $(C_BUILDDIR)/game/vehicles/$(f)/data/$(f)_anim_family.o: \
	CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))

# The level table (docs/levels.md, "The level table"), which
# src/game/level/data/level_table.cpp #includes: gLevelTable and
# gLevelRoomLists (`levels.py table levels`), and the room records and the
# arrays of the rooms each level plays (`levels.py table records`), from
# levels.json's "levels" and "table" and the rooms' room.json ("record").
# The stages are named by levels.json's "categories" (CATEGORY_DEPS below).
# level_table.inc also has the level themes' tables (docs/levels.md,
# "Themes"): the colour cycles' lists and gThemeMusicCues, from levels.json's
# "themes" and "palette_cycles"; a theme's song is one of the songs of
# sound/gax_manifest.json, its picture one of graphics/manifest.json's.
THEME_DEPS := data/levels/levels.json sound/gax_manifest.json graphics/manifest.json tools/levels.py
LEVEL_TABLE_DEPS := data/levels/levels.json $(LEVEL_ROOM_JSONS) $(CATEGORY_DEPS) tools/levels.py
$(LEVELS_BUILDDIR)/level_table.inc: $(LEVEL_TABLE_DEPS) $(THEME_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py table levels $@
$(LEVELS_BUILDDIR)/level_table_16c814_rooms.inc: $(LEVEL_TABLE_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py table records $@
$(C_BUILDDIR)/game/level/data/level_table.o: $(LEVELS_BUILDDIR)/level_table.inc \
	$(LEVELS_BUILDDIR)/level_table_16c814_rooms.inc
$(C_BUILDDIR)/game/level/data/level_table.o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)

# The level select's pictures, one per theme (gLevelSelectPictures,
# `levels.py table pictures`), which
# src/game/ui/data/level_select_picture_table.cpp #includes.
$(LEVELS_BUILDDIR)/level_select_pictures.inc: $(THEME_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py table pictures $@
$(call data_obj,level_select_picture_table): $(LEVELS_BUILDDIR)/level_select_pictures.inc
$(call data_obj,level_select_picture_table): CPPFLAGS += -iquote $(LEVELS_BUILDDIR)

# RunRoom's colour-cycle cases per theme and the cycles' lists
# (theme_palette_cycles.h, `levels.py palette-cycles`), written only if it
# changed, behind a stamp.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/theme_palette_cycles.h
$(GENERATED_INCLUDE_DIR)/theme_palette_cycles.h.stamp: $(THEME_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py palette-cycles $(GENERATED_INCLUDE_DIR)/theme_palette_cycles.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/theme_palette_cycles.h: $(GENERATED_INCLUDE_DIR)/theme_palette_cycles.h.stamp
	@test -f $@ || python3 tools/levels.py palette-cycles $@

# The boss levels' case labels per boss kind (level_bosses.h, `levels.py
# bosses`, docs/levels.md "Boss levels"), for LevelState::GetBossIndex and
# GetBossHealth, from levels.json's "boss_kind"; it checks each against the
# bosses the level's rooms place and the types of the stages it plays.
# Written only if it changed, behind a stamp.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/level_bosses.h
$(GENERATED_INCLUDE_DIR)/level_bosses.h.stamp: data/levels/levels.json $(LEVEL_ROOM_JSONS) $(CATEGORY_DEPS) \
	data/levels/entity_types.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py bosses $(GENERATED_INCLUDE_DIR)/level_bosses.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/level_bosses.h: $(GENERATED_INCLUDE_DIR)/level_bosses.h.stamp
	@test -f $@ || python3 tools/levels.py bosses $@

# The BG palette cycle frames ActorPaletteCycle::Update DMAs
# (category_palette_cycles.h, `levels.py category-cycles`, docs/levels.md
# "Category graphics"): the ROM's, or each polar category's own "palette_cycle"
# (a palette_cycle of graphics/manifest.json). Written only if it changed,
# behind a stamp.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/category_palette_cycles.h
$(GENERATED_INCLUDE_DIR)/category_palette_cycles.h.stamp: $(CATEGORY_DEPS) graphics/manifest.json \
	$(CATEGORY_GRAPHICS_NEW) tools/gfx_manifest.py tools/jasc_pal.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py category-cycles $(GENERATED_INCLUDE_DIR)/category_palette_cycles.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/category_palette_cycles.h: $(GENERATED_INCLUDE_DIR)/category_palette_cycles.h.stamp
	@test -f $@ || python3 tools/levels.py category-cycles $@

# What clearing a level does (level_on_clear.h, `levels.py on-clear`,
# docs/levels.md "Boss levels"): LevelState::UpdateGameFrame's cases on the
# cleared level (the powers, cutscenes, the step on to mega-mix and the
# credits), from levels.json's "on_clear" (its cutscenes are
# data/cutscenes.json's, by index or constant); it warns about a numbered boss
# level that nothing marks cleared, which reads its rooms. Written only if it
# changed, behind a stamp.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/level_on_clear.h
$(GENERATED_INCLUDE_DIR)/level_on_clear.h.stamp: data/levels/levels.json $(LEVEL_ROOM_JSONS) $(CATEGORY_DEPS) \
	data/levels/entity_types.json data/cutscenes.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py on-clear $(GENERATED_INCLUDE_DIR)/level_on_clear.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/level_on_clear.h: $(GENERATED_INCLUDE_DIR)/level_on_clear.h.stamp
	@test -f $@ || python3 tools/levels.py on-clear $@

# The level select's per-world tables, IsNextWorldOpen's cases and the page
# macros (level_select_worlds.h, `levels.py worlds`, docs/levels.md
# "Level-select pages" and "Partial pages"), from levels.json's "worlds" and
# the levels; it checks that bank 51 has a number icon for every numbered
# level. Written only if it changed, behind a stamp.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/level_select_worlds.h
$(GENERATED_INCLUDE_DIR)/level_select_worlds.h.stamp: data/levels/levels.json $(LEVEL_ROOM_JSONS) \
	graphics/sprites/bank51_level_select_icons/bank.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py worlds $(GENERATED_INCLUDE_DIR)/level_select_worlds.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/level_select_worlds.h: $(GENERATED_INCLUDE_DIR)/level_select_worlds.h.stamp
	@test -f $@ || python3 tools/levels.py worlds $@

# The constants headers named in the level data (#655): the entity types
# (data/levels/entity_types.json), the crate kinds (crate_kinds.json) and
# the level ids (levels.json's "levels" list).
# They are generated into build/include (GENERATED_INCLUDE_DIR), which is
# on every object's include path. graphics.mk adds graphics.hpp and the
# Makefile the sound constants (songs.h, sfx.h) to GENERATED_HEADERS, and
# the Makefile makes every C object wait for the whole list.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/constants/entities.h $(GENERATED_INCLUDE_DIR)/constants/crates.h \
	$(GENERATED_INCLUDE_DIR)/constants/levels.h $(GENERATED_INCLUDE_DIR)/constants/themes.h \
	$(GENERATED_INCLUDE_DIR)/constants/save.h $(GENERATED_INCLUDE_DIR)/constants/categories.h

$(GENERATED_INCLUDE_DIR)/constants/entities.h: data/levels/entity_types.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants entities $@

$(GENERATED_INCLUDE_DIR)/constants/crates.h: data/levels/crate_kinds.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants crates $@

# The level ids and the counts the code takes from the levels (GEM_TOTAL
# counts the gem-path gems the rooms place), and the theme ids. Each is
# written only if it changed, behind a stamp, so an edit that keeps it (an
# entity in a room.json, a level's times) doesn't rebuild every object.
$(GENERATED_INCLUDE_DIR)/constants/levels.h.stamp: data/levels/levels.json $(LEVEL_ROOM_JSONS) tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants levels $(GENERATED_INCLUDE_DIR)/constants/levels.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/constants/levels.h: $(GENERATED_INCLUDE_DIR)/constants/levels.h.stamp
	@test -f $@ || python3 tools/levels.py constants levels $@

$(GENERATED_INCLUDE_DIR)/constants/themes.h.stamp: $(THEME_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants themes $(GENERATED_INCLUDE_DIR)/constants/themes.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/constants/themes.h: $(GENERATED_INCLUDE_DIR)/constants/themes.h.stamp
	@test -f $@ || python3 tools/levels.py constants themes $@

# The actor category ids (CATEGORY_*, CATEGORY_COUNT), from levels.json's
# "categories"; the types and exit statuses are include/constants/category_types.h.
$(GENERATED_INCLUDE_DIR)/constants/categories.h.stamp: $(CATEGORY_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants categories $(GENERATED_INCLUDE_DIR)/constants/categories.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/constants/categories.h: $(GENERATED_INCLUDE_DIR)/constants/categories.h.stamp
	@test -f $@ || python3 tools/levels.py constants categories $@

# The save data's EEPROM and record size, from the level count, and its
# version and checksum seed, from a CRC-32 of the levels and the rooms'
# room.json (docs/levels.md, "The save"). Written only if it changed, behind
# a stamp: an edit that keeps them rebuilds nothing.
$(GENERATED_INCLUDE_DIR)/constants/save.h.stamp: data/levels/levels.json $(LEVEL_ROOM_JSONS) tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants save $(GENERATED_INCLUDE_DIR)/constants/save.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/constants/save.h: $(GENERATED_INCLUDE_DIR)/constants/save.h.stamp
	@test -f $@ || python3 tools/levels.py constants save $@
