# The rooms' level data (docs/levels.md): data/levels/ holds the editable
# sources, one folder per room, data/levels/<level>/<slot>/ (room.json, each
# BG layer as its whole picture, bg0-3.png, and the collision layer as an
# image, collision.png), listed by levels.json's "rooms"; tools/levels.py
# builds them back:
# - each BG layer's cells, decoded from its picture against its tile set
#   (levels.py from-png, into build/.../data/levels/<room>/<layer>.map.bin);
# - each room's level asset (the per-layer chunk streams): raw for 7 rooms,
#   LZ77-packed by gbagfx for the other 34, as C initializer bytes
#   (tools/bin2c.py);
# - the C definitions of the two room-data regions (each room's palette
#   read from its layer pictures, which must all carry the same one), which
#   src/data/level_rooms_*.cpp #include, and of the 41 assets, which
#   src/data/level_assets_64f830.cpp #includes.
LEVELS_BUILDDIR := $(OBJ_DIR)/data/levels
LEVEL_ROOMS := $(shell python3 tools/levels.py rooms)
LEVEL_PACKED_ROOMS := $(shell python3 tools/levels.py rooms --packed)
LEVEL_ROOM_JSONS := $(foreach room,$(LEVEL_ROOMS),data/levels/$(room)/room.json)
# The rooms' layer pictures, which also carry each room's palette.
LEVEL_BG_PNGS := $(foreach room,$(LEVEL_ROOMS),$(wildcard data/levels/$(room)/bg?.png))
LEVEL_REGIONS := level_rooms_24b638 level_rooms_2b91d0

LEVEL_TOOLS := tools/levels.py tools/collision_bmp.py data/levels/collision_legend.json
# A room's BG layers (bg0-3), from its pictures, and their decoded maps.
level_bg_layers = $(patsubst data/levels/$(1)/%.png,%,$(wildcard data/levels/$(1)/bg?.png))
level_bg_maps = $(foreach l,$(call level_bg_layers,$(1)),$(LEVELS_BUILDDIR)/$(1)/$(l).map.bin)
LEVEL_BG_MAPS := $(foreach room,$(LEVEL_ROOMS),$(call level_bg_maps,$(room)))

# A BG layer's picture -> its cells (docs/levels.md, "BG layers"). An
# error names the layer, the cell and its pixels. levels.py also writes a
# .d file naming the layer's tile set PNG (graphics/level_tilesets/ for
# BG0, graphics/level_bg_tiles/ for BG1-3), so editing a tile set decodes
# again only the layers drawn with it.
define LEVEL_MAP_RULE
$(LEVELS_BUILDDIR)/$(1)/$(2).map.bin: data/levels/$(1)/$(2).png data/levels/$(1)/room.json tools/levels.py
	@mkdir -p $$(dir $$@)
	python3 tools/levels.py from-png data/levels/$(1) $(2) $$@
endef
$(foreach room,$(LEVEL_ROOMS),$(foreach l,$(call level_bg_layers,$(room)),$(eval $(call LEVEL_MAP_RULE,$(room),$(l)))))
-include $(LEVEL_BG_MAPS:=.d)

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
$(LEVELS_BUILDDIR)/level_rooms_%.inc: data/levels/levels.json $(LEVEL_ROOM_JSONS) $(LEVEL_BG_PNGS) \
		$(foreach room,$(LEVEL_ROOMS),$(LEVELS_BUILDDIR)/$(room)/asset.bin) data/levels/entity_types.json $(LEVEL_TOOLS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py c level_rooms_$* $@ --build $(LEVELS_BUILDDIR)

LEVEL_ASSET_INCS := $(foreach room,$(filter-out $(LEVEL_PACKED_ROOMS),$(LEVEL_ROOMS)),$(LEVELS_BUILDDIR)/$(room)/asset.bin.inc) \
	$(foreach room,$(LEVEL_PACKED_ROOMS),$(LEVELS_BUILDDIR)/$(room)/asset.bin.lz.inc)

$(LEVELS_BUILDDIR)/level_assets_64f830.inc: data/levels/levels.json $(LEVEL_ROOM_JSONS) tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py assets $@

$(foreach r,$(LEVEL_REGIONS),$(eval $(C_BUILDDIR)/data/$(r).o: $(LEVELS_BUILDDIR)/$(r).inc))
$(foreach r,$(LEVEL_REGIONS),$(eval $(C_BUILDDIR)/data/$(r).o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))
$(C_BUILDDIR)/data/level_assets_64f830.o: $(LEVELS_BUILDDIR)/level_assets_64f830.inc $(LEVEL_ASSET_INCS)
$(C_BUILDDIR)/data/level_assets_64f830.o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)

# The constants headers named in the level data (#655): the entity types
# (data/levels/entity_types.json), the crate kinds (crate_kinds.json) and
# the level ids (levels.json's "levels" list).
# They are generated into build/include, which is on every object's
# include path. The Makefile adds the sound constants (songs.h, sfx.h) to
# GENERATED_HEADERS, and makes every C object wait for the whole list.
GENERATED_INCLUDE_DIR := build/include
GENERATED_HEADERS := $(GENERATED_INCLUDE_DIR)/constants/entities.h $(GENERATED_INCLUDE_DIR)/constants/crates.h \
	$(GENERATED_INCLUDE_DIR)/constants/levels.h
CPPFLAGS += -iquote $(GENERATED_INCLUDE_DIR)

$(GENERATED_INCLUDE_DIR)/constants/entities.h: data/levels/entity_types.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants entities $@

$(GENERATED_INCLUDE_DIR)/constants/crates.h: data/levels/crate_kinds.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants crates $@

$(GENERATED_INCLUDE_DIR)/constants/levels.h: data/levels/levels.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants levels $@
