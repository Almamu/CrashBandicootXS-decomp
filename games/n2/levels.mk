# N-Tranced's level data (#1113, docs/n_tranced.md "Levels"), from
# games/n2/data/levels/ with tools/levels.py --levels-dir, as levels.mk
# builds XS's from games/xs/data/levels/: the BG tile sets the rooms' pictures make,
# each room's level asset, and the assets' definitions
# (engine/src/game/level/data/level_assets.cpp, shared with XS, in objects.txt).
# Included by games/n2/game.mk.

N2_LEVELS_DIR      := $(N2_DIR)/data/levels
N2_LEVELS          := python3 tools/levels.py --levels-dir $(N2_LEVELS_DIR)
N2_LEVELS_BUILDDIR := $(OBJ_DIR)/data/levels
N2_LEVEL_ROOMS        := $(shell $(N2_LEVELS) rooms)
N2_LEVEL_PACKED_ROOMS := $(shell $(N2_LEVELS) rooms --packed)
N2_LEVEL_ROOM_JSONS   := $(foreach room,$(N2_LEVEL_ROOMS),$(N2_LEVELS_DIR)/$(room)/room.json)
N2_LEVEL_TOOLS        := $(PY_LEVELS) $(PY_COLLISION_BMP) $(N2_LEVELS_DIR)/collision_legend.json

n2_level_bg_maps = $(foreach l,$(patsubst $(N2_LEVELS_DIR)/$(1)/%.png,%,$(wildcard $(N2_LEVELS_DIR)/$(1)/bg?.png)),\
	$(N2_LEVELS_BUILDDIR)/$(1)/$(l).map.bin)

# the tile sets, and each layer's cells in them (levels.py tilesets-mk)
$(N2_LEVELS_BUILDDIR)/tilesets.mk: $(N2_LEVELS_DIR)/levels.json $(N2_LEVEL_ROOM_JSONS) \
		$(wildcard $(N2_LEVELS_DIR)/tileset_order.json) $(PY_LEVELS)
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

$(N2_LEVELS_BUILDDIR)/%.lz.inc: $(N2_LEVELS_BUILDDIR)/%.lz $(PY_BIN2C)
	python3 tools/bin2c.py $< $@ --lz

$(N2_LEVELS_BUILDDIR)/%/asset.bin.inc: $(N2_LEVELS_BUILDDIR)/%/asset.bin $(PY_BIN2C)
	python3 tools/bin2c.py $< $@

# the assets' definitions, in the ROM's order, and the rooms' classes
$(N2_LEVELS_BUILDDIR)/level_assets.inc: $(N2_LEVELS_DIR)/levels.json $(N2_LEVEL_ROOM_JSONS) $(PY_LEVELS)
	@mkdir -p $(dir $@)
	$(N2_LEVELS) assets $@

$(GENERATED_INCLUDE_DIR)/rooms.hpp: $(N2_LEVELS_DIR)/levels.json $(N2_LEVEL_ROOM_JSONS) $(PY_LEVELS)
	@mkdir -p $(dir $@)
	$(N2_LEVELS) classes $@

n2_level_asset_incs = $(foreach room,$(filter-out $(N2_LEVEL_PACKED_ROOMS),$(N2_LEVEL_ROOMS)),\
		$(N2_LEVELS_BUILDDIR)/$(room)/asset.bin.inc) \
	$(foreach room,$(filter $(N2_LEVEL_PACKED_ROOMS),$(N2_LEVEL_ROOMS)),$(N2_LEVELS_BUILDDIR)/$(room)/asset.bin.lz.inc)

$(C_BUILDDIR)/game/level/data/level_assets.o: $(N2_LEVELS_BUILDDIR)/level_assets.inc $(n2_level_asset_incs) \
	$(GENERATED_INCLUDE_DIR)/rooms.hpp
$(C_BUILDDIR)/game/level/data/level_assets.o: CPPFLAGS += -iquote $(N2_LEVELS_BUILDDIR)

# The rooms' data, per region (games/n2/src/game/level/data/level_rooms_N.cpp,
# levels.py c), and the tile sets
# (games/n2/src/game/level/data/level_bg0_tiles_N.cpp, level_bg_tiles_N.cpp)
N2_LEVEL_REGIONS := $(shell python3 -c "import json; print(' '.join(r['name'] for r in json.load(open('$(N2_LEVELS_DIR)/levels.json'))['regions']))")

$(N2_LEVELS_BUILDDIR)/level_rooms_%.inc: $(N2_LEVELS_DIR)/levels.json $(N2_LEVEL_ROOM_JSONS) \
		$(foreach room,$(N2_LEVEL_ROOMS),$(N2_LEVELS_BUILDDIR)/$(room)/asset.bin) $(N2_LEVEL_TOOLS)
	@mkdir -p $(dir $@)
	$(N2_LEVELS) c level_rooms_$* $@ --build $(N2_LEVELS_BUILDDIR)

$(N2_LEVELS_BUILDDIR)/tilesets/%.bin.inc: $(N2_LEVELS_BUILDDIR)/tilesets/%.bin $(PY_BIN2C)
	python3 tools/bin2c.py $< $@

N2_LEVEL_DATA_OBJS := $(foreach r,$(N2_LEVEL_REGIONS),$(C_BUILDDIR)/game/level/data/$(r).o) \
	$(foreach n,0 1 2,$(C_BUILDDIR)/game/level/data/level_bg0_tiles_$(n).o) \
	$(foreach n,0 1,$(C_BUILDDIR)/game/level/data/level_bg_tiles_$(n).o)
$(N2_LEVEL_DATA_OBJS): CPPFLAGS += -iquote $(N2_LEVELS_BUILDDIR)
$(foreach r,$(N2_LEVEL_REGIONS),$(eval $(C_BUILDDIR)/game/level/data/$(r).o: \
	$(N2_LEVELS_BUILDDIR)/$(r).inc $(GENERATED_INCLUDE_DIR)/rooms.hpp))
n2_bg0_tiles = $(N2_LEVELS_BUILDDIR)/tilesets/$(1).bin.inc $(N2_LEVELS_BUILDDIR)/tilesets/$(1).h
$(C_BUILDDIR)/game/level/data/level_bg0_tiles_0.o: $(call n2_bg0_tiles,gArabiaBg0Tiles)
$(C_BUILDDIR)/game/level/data/level_bg0_tiles_1.o: $(call n2_bg0_tiles,gVolcanoBg0Tiles)
$(C_BUILDDIR)/game/level/data/level_bg0_tiles_2.o: $(call n2_bg0_tiles,gEgyptBg0Tiles)
$(C_BUILDDIR)/game/level/data/level_bg_tiles_0.o: $(foreach t,gArabiaBgTiles gArabiaSkyBgTiles \
	gEgyptBgTiles,$(N2_LEVELS_BUILDDIR)/tilesets/$(t).bin.lz.inc)
$(C_BUILDDIR)/game/level/data/level_bg_tiles_1.o: $(foreach t,gVolcanoBgTiles \
	gIslandBgTiles,$(N2_LEVELS_BUILDDIR)/tilesets/$(t).bin.lz.inc)

# The level table and the room records under it
# (games/n2/src/game/level/data/level_table.cpp, levels.py table, as XS's)
N2_LEVEL_TABLE_DEPS := $(N2_LEVELS_DIR)/levels.json $(N2_LEVEL_ROOM_JSONS) $(PY_LEVELS)
$(N2_LEVELS_BUILDDIR)/level_table.inc: $(N2_LEVEL_TABLE_DEPS)
	@mkdir -p $(dir $@)
	$(N2_LEVELS) table levels $@
$(N2_LEVELS_BUILDDIR)/level_table_rooms.inc: $(N2_LEVEL_TABLE_DEPS)
	@mkdir -p $(dir $@)
	$(N2_LEVELS) table records $@
$(C_BUILDDIR)/game/level/data/level_table.o: $(N2_LEVELS_BUILDDIR)/level_table.inc \
	$(N2_LEVELS_BUILDDIR)/level_table_rooms.inc $(GENERATED_INCLUDE_DIR)/rooms.hpp
$(C_BUILDDIR)/game/level/data/level_table.o: CPPFLAGS += -iquote $(N2_LEVELS_BUILDDIR)

# The constants XS generates from its levels.json, entity_types.json and
# crate_kinds.json (constants/levels.h, themes.h, entities.h, categories.h,
# crates.h): N-Tranced's
# aren't generated yet (#1113), so each is a placeholder with only the
# counts the shared headers' declarations need, N-Tranced's own values
# (N2_CONSTANTS_<header>, "NAME value" pairs). A source that uses one of
# their other constants doesn't build for N-Tranced until they are
# generated.
# - LEVEL_COUNT: gLevelTable (0x081D0430) has 29 levels (24, then the 5
#   bosses), its 0x24-byte records up to their room lists (0x081D0844),
#   whose 29 records of 16 end at gThemeMusicCues.
# - UI_TEXT_COUNT: each of the 6 languages' UI string tables (gUiTextTables,
#   in the IWRAM image at 0x087FBE94, +0x31D0: 0x081D3984, ...) has 196
#   entries, each table followed by the next language's strings.
# - PROGRESS_EXTRA_FLAG_BYTES: 0, as XS's: the progress flags a mod adds
#   past the ROM's (levels.json's "extra_progress_flags").
# - THEME_COUNT: the levels' themes are 0-11, and gThemeMusicCues
#   (0x081D0A14) has 12 entries where XS's has 11 and a padding byte.
# - CRATE_KIND_COUNT: the gCrateKind* tables (0x081CFAC4, ...) are 26 bytes
#   apart (XS's 22), and gCrateHitResponse (0x081CFC04) has 26 rows (of
#   8, ATTACK_KIND_COUNT) up to gAttackKindBreakLimited.
# - SFX_COUNT and SONG_COUNT (constants/sfx.h, songs.h, which XS's build
#   generates from its sound data): gSfxTable (0x081CD958) has 64 entries
#   of 12 bytes up to gCrc16Table, gSongTable (0x081CD904) 21 pointers up
#   to gSfxTable.
N2_CONSTANTS_levels     := LEVEL_COUNT 29 UI_TEXT_COUNT 196 PROGRESS_EXTRA_FLAG_BYTES 0
N2_CONSTANTS_themes     := THEME_COUNT 12
N2_CONSTANTS_categories :=
N2_CONSTANTS_crates     := CRATE_KIND_COUNT 26
N2_CONSTANTS_sfx        := SFX_COUNT 64
N2_CONSTANTS_songs      := SONG_COUNT 21
N2_PLACEHOLDER_CONSTANTS := $(foreach h,levels themes categories crates sfx songs,$(GENERATED_INCLUDE_DIR)/constants/$(h).h)
$(N2_PLACEHOLDER_CONSTANTS): $(GENERATED_INCLUDE_DIR)/constants/%.h: $(N2_DIR)/levels.mk $(N2_LEVELS_DIR)/levels.json
	@mkdir -p $(dir $@)
	printf '/* N-Tranced: not generated from its levels yet (games/n2/levels.mk). */\n#ifndef GUARD_CONSTANTS_$*_H\n#define GUARD_CONSTANTS_$*_H\n' > $@
	$(if $(N2_CONSTANTS_$*),printf '#define %s %s\n' $(N2_CONSTANTS_$*) >> $@)
	printf '#endif\n' >> $@
$(CXX_OBJS) $(C_OBJS) $(LIB_C_OBJS): | $(N2_PLACEHOLDER_CONSTANTS) $(GENERATED_INCLUDE_DIR)/constants/entities.h

# constants/entities.h (ENTITY_*, ENTITY_COUNT), generated as XS's from
# N-Tranced's entity_types.json (docs/n_tranced.md, "Levels")
$(GENERATED_INCLUDE_DIR)/constants/entities.h: $(N2_LEVELS_DIR)/entity_types.json $(PY_LEVELS)
	@mkdir -p $(dir $@)
	$(N2_LEVELS) constants entities $@
