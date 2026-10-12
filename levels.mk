# The rooms' level data (docs/levels.md): data/levels/ holds the editable
# sources (room.json, one decoded tilemap per BG layer and the collision
# layer as an image, collision.png), tools/levels.py
# builds them back:
# - each room's level asset (the per-layer chunk streams): raw for 7 rooms,
#   LZ77-packed by gbagfx for the other 34, as C initializer bytes
#   (tools/bin2c.py);
# - the C definitions of the two room-data regions, which
#   src/data/level_rooms_*.cpp #include, and of the 41 assets, which
#   src/data/level_assets_64f830.cpp #includes.
LEVELS_BUILDDIR := $(OBJ_DIR)/data/levels
LEVEL_ROOMS := $(notdir $(wildcard data/levels/room*))
LEVEL_PACKED_ROOMS := $(notdir $(patsubst %/room.json,%,$(shell grep -l '"packed": true' data/levels/room*/room.json)))
LEVEL_REGIONS := level_rooms_24b638 level_rooms_2b91d0

LEVEL_TOOLS := tools/levels.py tools/collision_bmp.py data/levels/collision_legend.json
LEVEL_SOURCES := data/levels/levels.json $(wildcard data/levels/room*/room.json) $(wildcard data/levels/room*/*.map.bin) \
	$(wildcard data/levels/room*/collision.png)

define LEVEL_ASSET_RULE
$(LEVELS_BUILDDIR)/$(1)/asset.bin: data/levels/$(1)/room.json $$(wildcard data/levels/$(1)/*.map.bin) \
		$$(wildcard data/levels/$(1)/collision.png) $(LEVEL_TOOLS)
	@mkdir -p $$(dir $$@)
	python3 tools/levels.py asset data/levels/$(1) $$@
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

$(LEVELS_BUILDDIR)/level_rooms_%.inc: $(LEVEL_SOURCES) data/levels/entity_types.json $(LEVEL_TOOLS)
	@mkdir -p $(dir $@)
	python3 tools/levels.py c level_rooms_$* $@

LEVEL_ASSET_INCS := $(foreach room,$(filter-out $(LEVEL_PACKED_ROOMS),$(LEVEL_ROOMS)),$(LEVELS_BUILDDIR)/$(room)/asset.bin.inc) \
	$(foreach room,$(LEVEL_PACKED_ROOMS),$(LEVELS_BUILDDIR)/$(room)/asset.bin.lz.inc)

$(LEVELS_BUILDDIR)/level_assets_64f830.inc: data/levels/levels.json $(wildcard data/levels/room*/room.json) tools/levels.py
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
