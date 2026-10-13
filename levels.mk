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
#   src/data/level_rooms_*.cpp #include, and of their rooms' assets, which
#   src/data/level_assets_64f830.cpp #includes. The level data that isn't
#   the ROM's goes with them, in place (docs/levels.md, "Adding a room"):
#   the rooms in no region of levels.json and the tile sets only they use
#   after the last region's rooms, their assets after the ROM's.
LEVELS_BUILDDIR := $(OBJ_DIR)/data/levels
LEVEL_ROOMS := $(shell python3 tools/levels.py rooms)
LEVEL_PACKED_ROOMS := $(shell python3 tools/levels.py rooms --packed)
LEVEL_ROOM_JSONS := $(foreach room,$(LEVEL_ROOMS),data/levels/$(room)/room.json)
# The rooms' layer pictures, which also carry each room's palette.
LEVEL_BG_PNGS := $(foreach room,$(LEVEL_ROOMS),$(wildcard data/levels/$(room)/bg?.png))
LEVEL_REGIONS := level_rooms_24b638 level_rooms_2b91d0

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

# The ROM's tile sets, defined by hand in src/data/ (the BG0 ones raw 8bpp
# tagged assets, sized by tilesets/<set>.h; the BG1-3 ones LZ77 4bpp).
LEVEL_TILESET_DEFS = $(foreach t,$(1),$(LEVELS_BUILDDIR)/tilesets/$(t).bin.inc $(LEVELS_BUILDDIR)/tilesets/$(t).h)
$(C_BUILDDIR)/data/level_tilesets_17e78c.o: $(call LEVEL_TILESET_DEFS,gJungleBg0Tiles gSnowBg0Tiles gUnderwaterBg0Tiles)
$(C_BUILDDIR)/data/level_tilesets_270f08.o: $(call LEVEL_TILESET_DEFS,gSewerBg0Tiles gSpaceBg0Tiles)
$(C_BUILDDIR)/data/level_bg_tiles_63d4c0.o: $(foreach t,gSnowBgTiles gJungleBgTiles gTempleBgTiles gSewerBgTiles \
	gSpaceBgTiles gUnderwaterBgTiles,$(LEVELS_BUILDDIR)/tilesets/$(t).bin.lz.inc)
$(foreach o,level_tilesets_17e78c level_tilesets_270f08 level_bg_tiles_63d4c0,\
	$(eval $(C_BUILDDIR)/data/$(o).o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))

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

$(LEVELS_BUILDDIR)/level_assets_64f830.inc: data/levels/levels.json $(LEVEL_ROOM_JSONS) tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py assets $@

$(foreach r,$(LEVEL_REGIONS),$(eval $(C_BUILDDIR)/data/$(r).o: $(LEVELS_BUILDDIR)/$(r).inc))
$(foreach r,$(LEVEL_REGIONS),$(eval $(C_BUILDDIR)/data/$(r).o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))
$(C_BUILDDIR)/data/$(lastword $(LEVEL_REGIONS)).o: \
	$(foreach t,$(LEVEL_NEW_TILESETS_8BPP),$(LEVELS_BUILDDIR)/tilesets/$(t).bin.inc) \
	$(foreach t,$(LEVEL_NEW_TILESETS_4BPP),$(LEVELS_BUILDDIR)/tilesets/$(t).bin.lz.inc)
$(C_BUILDDIR)/data/level_assets_64f830.o: $(LEVELS_BUILDDIR)/level_assets_64f830.inc $(call level_asset_incs,$(LEVEL_ROOMS))
$(C_BUILDDIR)/data/level_assets_64f830.o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)

# The level table (docs/levels.md, "The level table"), which
# src/data/level_table_16c814.cpp #includes: gLevelTable and
# gLevelRoomLists (`levels.py table levels`), and the room records and the
# arrays of the rooms each level plays (`levels.py table records`), from
# levels.json's "levels" and "table" and the rooms' room.json ("record").
# The stages are named by include/constants/categories.h.
LEVEL_TABLE_DEPS := data/levels/levels.json $(LEVEL_ROOM_JSONS) include/constants/categories.h tools/levels.py
$(LEVELS_BUILDDIR)/level_table_16c814.inc: $(LEVEL_TABLE_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py table levels $@
$(LEVELS_BUILDDIR)/level_table_16c814_rooms.inc: $(LEVEL_TABLE_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py table records $@
$(C_BUILDDIR)/data/level_table_16c814.o: $(LEVELS_BUILDDIR)/level_table_16c814.inc \
	$(LEVELS_BUILDDIR)/level_table_16c814_rooms.inc
$(C_BUILDDIR)/data/level_table_16c814.o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)

# The constants headers named in the level data (#655): the entity types
# (data/levels/entity_types.json), the crate kinds (crate_kinds.json) and
# the level ids (levels.json's "levels" list).
# They are generated into build/include, which is on every object's
# include path. The Makefile adds the sound constants (songs.h, sfx.h) to
# GENERATED_HEADERS, and makes every C object wait for the whole list.
GENERATED_INCLUDE_DIR := build/include
GENERATED_HEADERS := $(GENERATED_INCLUDE_DIR)/constants/entities.h $(GENERATED_INCLUDE_DIR)/constants/crates.h \
	$(GENERATED_INCLUDE_DIR)/constants/levels.h $(GENERATED_INCLUDE_DIR)/constants/themes.h
CPPFLAGS += -iquote $(GENERATED_INCLUDE_DIR)

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

$(GENERATED_INCLUDE_DIR)/constants/themes.h.stamp: data/levels/levels.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants themes $(GENERATED_INCLUDE_DIR)/constants/themes.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/constants/themes.h: $(GENERATED_INCLUDE_DIR)/constants/themes.h.stamp
	@test -f $@ || python3 tools/levels.py constants themes $@
