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

$(LEVELS_BUILDDIR)/level_rooms_%.inc: $(LEVEL_SOURCES) data/levels/entity_types.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py c level_rooms_$* $@

LEVELS_BUILT := $(foreach room,$(LEVEL_ROOMS),$(LEVELS_BUILDDIR)/$(room)/asset.bin) \
	$(foreach room,$(LEVEL_PACKED_ROOMS),$(LEVELS_BUILDDIR)/$(room)/asset.bin.lz)

$(foreach r,$(LEVEL_REGIONS),$(eval $(C_BUILDDIR)/data/$(r).o: $(LEVELS_BUILDDIR)/$(r).inc))
$(foreach r,$(LEVEL_REGIONS),$(eval $(C_BUILDDIR)/data/$(r).o: CPPFLAGS += -iquote $(LEVELS_BUILDDIR)))

# The constants headers named in the level data (#655): the entity types
# (data/levels/entity_types.json) and the crate kinds (crate_kinds.json).
# They are generated into build/include, which is on every object's
# include path; the C objects wait for them (order-only, so a regenerated
# header rebuilds only the objects whose .d lists it).
GENERATED_INCLUDE_DIR := build/include
GENERATED_HEADERS := $(GENERATED_INCLUDE_DIR)/constants/entities.h $(GENERATED_INCLUDE_DIR)/constants/crates.h
CPPFLAGS += -iquote $(GENERATED_INCLUDE_DIR)

$(GENERATED_INCLUDE_DIR)/constants/entities.h: data/levels/entity_types.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants entities $@

$(GENERATED_INCLUDE_DIR)/constants/crates.h: data/levels/crate_kinds.json tools/levels.py
	@mkdir -p $(dir $@)
	python3 tools/levels.py constants crates $@

$(C_OBJS) $(LIB_C_OBJS) $(LIBGCC2_OBJS): | $(GENERATED_HEADERS)
