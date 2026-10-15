# N-Tranced's level data (#1113, docs/n_tranced.md "Levels"), from
# games/n2/data/levels/ with tools/levels.py --levels-dir, as levels.mk
# builds XS's from data/levels/: the BG tile sets the rooms' pictures make,
# each room's level asset, and the assets' definitions
# (src/game/level/data/level_assets.cpp, shared with XS, in objects.txt).
# Included by games/n2/game.mk.

N2_LEVELS_DIR      := $(N2_DIR)/data/levels
N2_LEVELS          := python3 tools/levels.py --levels-dir $(N2_LEVELS_DIR)
N2_LEVELS_BUILDDIR := $(OBJ_DIR)/data/levels
N2_LEVEL_ROOMS        := $(shell $(N2_LEVELS) rooms)
N2_LEVEL_PACKED_ROOMS := $(shell $(N2_LEVELS) rooms --packed)
N2_LEVEL_ROOM_JSONS   := $(foreach room,$(N2_LEVEL_ROOMS),$(N2_LEVELS_DIR)/$(room)/room.json)
N2_LEVEL_TOOLS        := tools/levels.py tools/collision_bmp.py $(N2_LEVELS_DIR)/collision_legend.json

n2_level_bg_maps = $(foreach l,$(patsubst $(N2_LEVELS_DIR)/$(1)/%.png,%,$(wildcard $(N2_LEVELS_DIR)/$(1)/bg?.png)),\
	$(N2_LEVELS_BUILDDIR)/$(1)/$(l).map.bin)

# the tile sets, and each layer's cells in them (levels.py tilesets-mk)
$(N2_LEVELS_BUILDDIR)/tilesets.mk: $(N2_LEVELS_DIR)/levels.json $(N2_LEVEL_ROOM_JSONS) \
		$(wildcard $(N2_LEVELS_DIR)/tileset_order.json) tools/levels.py
	$(N2_LEVELS) tilesets-mk $@ --build $(N2_LEVELS_BUILDDIR)
include $(N2_LEVELS_BUILDDIR)/tilesets.mk

# each room's level asset, LZ77-packed if the ROM has it packed
define N2_LEVEL_ASSET_RULE
$(N2_LEVELS_BUILDDIR)/$(1)/asset.bin: $(N2_LEVELS_DIR)/$(1)/room.json $(call n2_level_bg_maps,$(1)) \
		$$(wildcard $(N2_LEVELS_DIR)/$(1)/collision.png) $(N2_LEVEL_TOOLS)
	@mkdir -p $$(dir $$@)
	$(N2_LEVELS) asset $(N2_LEVELS_DIR)/$(1) $$@ --build $(N2_LEVELS_BUILDDIR)
endef
$(foreach room,$(N2_LEVEL_ROOMS),$(eval $(call N2_LEVEL_ASSET_RULE,$(room))))

$(N2_LEVELS_BUILDDIR)/%.lz: $(N2_LEVELS_BUILDDIR)/% | $(GFX)
	$(GFX) $< $@

$(N2_LEVELS_BUILDDIR)/%.lz.inc: $(N2_LEVELS_BUILDDIR)/%.lz tools/bin2c.py
	python3 tools/bin2c.py $< $@ --lz

$(N2_LEVELS_BUILDDIR)/%/asset.bin.inc: $(N2_LEVELS_BUILDDIR)/%/asset.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@

# the assets' definitions, in the ROM's order, and the rooms' classes
$(N2_LEVELS_BUILDDIR)/level_assets.inc: $(N2_LEVELS_DIR)/levels.json $(N2_LEVEL_ROOM_JSONS) tools/levels.py
	@mkdir -p $(dir $@)
	$(N2_LEVELS) assets $@

$(GENERATED_INCLUDE_DIR)/rooms.hpp: $(N2_LEVELS_DIR)/levels.json $(N2_LEVEL_ROOM_JSONS) tools/levels.py
	@mkdir -p $(dir $@)
	$(N2_LEVELS) classes $@

n2_level_asset_incs = $(foreach room,$(filter-out $(N2_LEVEL_PACKED_ROOMS),$(N2_LEVEL_ROOMS)),\
		$(N2_LEVELS_BUILDDIR)/$(room)/asset.bin.inc) \
	$(foreach room,$(filter $(N2_LEVEL_PACKED_ROOMS),$(N2_LEVEL_ROOMS)),$(N2_LEVELS_BUILDDIR)/$(room)/asset.bin.lz.inc)

$(C_BUILDDIR)/game/level/data/level_assets.o: $(N2_LEVELS_BUILDDIR)/level_assets.inc $(n2_level_asset_incs) \
	$(GENERATED_INCLUDE_DIR)/rooms.hpp
$(C_BUILDDIR)/game/level/data/level_assets.o: CPPFLAGS += -iquote $(N2_LEVELS_BUILDDIR)
