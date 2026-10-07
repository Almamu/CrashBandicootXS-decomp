# The rooms' level data (docs/levels.md): data/levels/ holds the editable
# sources (room.json and one decoded tilemap per layer), tools/levels.py
# builds them back:
# - each room's level asset (the per-layer chunk streams), which data/data.s
#   incbins: raw for 7 rooms, LZ77-packed by gbagfx for the other 34;
# - the C definitions of the two room-data regions, which
#   src/data/level_rooms_*.c #include.
LEVELS_BUILDDIR := $(OBJ_DIR)/data/levels
LEVEL_ROOMS := $(notdir $(wildcard data/levels/room*))
LEVEL_PACKED_ROOMS := $(notdir $(patsubst %/room.json,%,$(shell grep -l '"packed": true' data/levels/room*/room.json)))
LEVEL_REGIONS := level_rooms_24b638 level_rooms_2b91d0

LEVEL_SOURCES := data/levels/levels.json $(wildcard data/levels/room*/room.json) $(wildcard data/levels/room*/*.map.bin)

define LEVEL_ASSET_RULE
$(LEVELS_BUILDDIR)/$(1)/asset.bin: data/levels/$(1)/room.json $$(wildcard data/levels/$(1)/*.map.bin) tools/levels.py
	@mkdir -p $$(dir $$@)
	python3 tools/levels.py asset data/levels/$(1) $$@
endef
$(foreach room,$(LEVEL_ROOMS),$(eval $(call LEVEL_ASSET_RULE,$(room))))

$(LEVELS_BUILDDIR)/%.lz: $(LEVELS_BUILDDIR)/% | $(GFX)
	$(GFX) $< $@

$(LEVELS_BUILDDIR)/level_rooms_%.inc: $(LEVEL_SOURCES) tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py c level_rooms_$* $@

LEVELS_BUILT := $(foreach room,$(LEVEL_ROOMS),$(LEVELS_BUILDDIR)/$(room)/asset.bin) \
	$(foreach room,$(LEVEL_PACKED_ROOMS),$(LEVELS_BUILDDIR)/$(room)/asset.bin.lz)

$(foreach r,$(LEVEL_REGIONS),$(eval $(C_BUILDDIR)/data/$(r).o: $(LEVELS_BUILDDIR)/$(r).inc))
$(foreach r,$(LEVEL_REGIONS),$(eval $(C_BUILDDIR)/data/$(r).o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))

# constants/levels.h (the LEVEL_* ids) is generated from levels.json's
# "levels" list into build/.../include, which is on the quote include path
# after include/. Any C file may include it, so it's built before every C
# object (order-only; the .d files track who includes it).
LEVELS_CONSTANTS := $(OBJ_DIR)/include/constants/levels.h
CPPFLAGS += -iquote $(OBJ_DIR)/include

$(LEVELS_CONSTANTS): data/levels/levels.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants $@

$(C_OBJS): | $(LEVELS_CONSTANTS)
