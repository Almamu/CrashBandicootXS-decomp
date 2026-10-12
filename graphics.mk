
# All rules below take a source under graphics/ and produce the
# corresponding built file under $(GRAPHICS_BUILDDIR), mirroring the
# subdirectory layout, so graphics/ only ever holds editable sources.
# Every asset ends up as C initializer bytes (tools/bin2c.py) that a
# src/data/*.cpp definition #includes (docs/data.md, "Resources");
# data/data.s incbins no graphics.

# Palettes -> raw GBA .gbapal. Read as gbagfx reads them, plus the ROM's
# stray bit-15 entries, which tools/jasc_pal.py keeps in the red value's
# lowest bit (docs/graphics.md, "Palettes"). A standalone palette is JASC
# .pal text (the OBJ palettes, graphics/sprites/palettes/); a palette that
# belongs to one picture is that picture's PNG palette, which must have
# exactly the colours the ROM's palette has: 256, or 16 for the pictures
# listed in PALETTE_COLORS_16 (png-to-gba --colors fails otherwise, so an
# editor that pads or trims the palette is caught).
$(GRAPHICS_BUILDDIR)/%.gbapal: graphics/%.pal tools/jasc_pal.py
	@mkdir -p $(dir $@)
	python3 tools/jasc_pal.py to-gba $< $@
PALETTE_COLORS_16 := bg_package/61c30c_menu_sky bg_package/628c50_continue_prompt_smoke \
	bg_package/62a958_continue_prompt_glow bg_package/62c2c8_title_bandicoot_obj \
	bg_package/62c4b0_title_crash_obj bg_package/62cc3c_title_arrow1_obj bg_package/62ce70_title_arrow2_obj \
	bg_package/62d0cc_universal_logo intro/68_631acc_tiles intro/69_6324b4_tiles intro/70_632820_tiles \
	intro/71_632bc4_tiles intro/72_6334c4_tiles intro/73_634270_tiles intro/74_636ef4_tiles intro/75_637604_tiles
$(GRAPHICS_BUILDDIR)/%.gbapal: graphics/%.png tools/jasc_pal.py
	@mkdir -p $(dir $@)
	python3 tools/jasc_pal.py png-to-gba $< $@ --colors $(if $(filter $*,$(PALETTE_COLORS_16)),16,256)

# Framed OBJ sprite sheet entities: each subdirectory under
# graphics/unknown/<sheet>/ is one distinct entity (one animation's worth
# of frame data - see docs/graphics.md, "Re-splitting into one file per
# entity"), containing one indexed PNG per frame, named so filename sort
# order matches original ROM frame order, with no sidecar metadata - a
# frame's own tile dimensions are just its PNG size / 8. gbagfx can't be
# used directly: each frame has a 4-byte non-pixel header not present in
# a plain PNG<->4bpp round trip. Explicit (non-pattern) rules, one per
# entity folder found under each sheet, so editing/adding/removing a
# frame PNG is tracked like any other prerequisite.
define FRAMED_ENTITY_RULE
$(GRAPHICS_BUILDDIR)/unknown/$(1)/$(2).bin: $$(sort $$(wildcard graphics/unknown/$(1)/$(2)/*.png)) tools/framed_gfx.py
	@mkdir -p $$(dir $$@)
	python3 tools/framed_gfx.py to-bin-folder graphics/unknown/$(1)/$(2) $$@
endef

# Split multi-record sprite sheets: each ROM asset below was originally one
# big LZ77 stream, so it must be recompressed as a single unit - every
# entity's converted bytes get concatenated back in folder-name order
# (numeric prefixes preserve original ROM order) before compression.
define FRAMED_SHEET_RULE
$(GRAPHICS_BUILDDIR)/unknown/$(1).bin: $(foreach e,$(sort $(notdir $(patsubst %/,%,$(wildcard graphics/unknown/$(1)/*/)))),$(GRAPHICS_BUILDDIR)/unknown/$(1)/$(e).bin)
	@mkdir -p $$(dir $$@)
	cat $$^ > $$@
endef

$(foreach sheet,00_0b2120 01_14174c,\
  $(foreach entity,$(sort $(notdir $(patsubst %/,%,$(wildcard graphics/unknown/$(sheet)/*/)))),\
    $(eval $(call FRAMED_ENTITY_RULE,$(sheet),$(entity)))))

$(eval $(call FRAMED_SHEET_RULE,00_0b2120))
$(eval $(call FRAMED_SHEET_RULE,01_14174c))

# The byte offset of every frame in a sheet (FRAMED_<SHEET>_<ENTITY>_<NN>),
# for the category families' pool_offset table_B arrays in src/data/.
define FRAMED_HEADER_RULE
$(GRAPHICS_BUILDDIR)/unknown/$(1)_frames.h: $(sort $(wildcard graphics/unknown/$(1)/*/*.png)) tools/framed_gfx.py
	@mkdir -p $$(dir $$@)
	python3 tools/framed_gfx.py header graphics/unknown/$(1) $$@
endef
$(eval $(call FRAMED_HEADER_RULE,00_0b2120))
$(eval $(call FRAMED_HEADER_RULE,01_14174c))

# Generic LZ77 compression, used for every asset type above.
$(GRAPHICS_BUILDDIR)/%.lz: $(GRAPHICS_BUILDDIR)/% | $(GFX)
	@mkdir -p $(dir $@)
	$(GFX) $< $@

# PNG -> C const array (grit), see docs/graphics.md. grit does the pixel
# layout, gbagfx the byte-exact LZ77 (grit's own -gzl compressor does not
# reproduce the ROM's streams), tools/bin2c.py the initializer bytes that a
# src/data/*.cpp file #includes.

# Mode 4 bitmaps: linear 8bpp. Their palette is the PNG's own (256
# colours, the .gbapal rule above), an uncompressed u16 array (bin2c
# --u16).
$(GRAPHICS_BUILDDIR)/%_bitmap.img.bin: graphics/%_bitmap.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gb -gB8 -p! -ftb -fh! -o $(@:.img.bin=)
$(GRAPHICS_BUILDDIR)/%_bitmap.pal.inc: $(GRAPHICS_BUILDDIR)/%_bitmap.gbapal tools/bin2c.py
	python3 tools/bin2c.py $< $@ --u16

# Tile sheets (graphics/intro/*_tiles.png, graphics/tileset1/*_tiles.png:
# the HUD fonts, the credits and VV logos, the level-select cursor zoom and
# pictures): grit -gt, 4bpp, or 8bpp for the *_8bpp_tiles.png ones.
TILE_SHEET_BPP = $(if $(filter %_8bpp_tiles.png,$<),8,4)
$(GRAPHICS_BUILDDIR)/intro/%_tiles.img.bin: graphics/intro/%_tiles.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB$(TILE_SHEET_BPP) -p! -ftb -fh! -o $(@:.img.bin=)
$(GRAPHICS_BUILDDIR)/tileset1/%_tiles.img.bin: graphics/tileset1/%_tiles.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB$(TILE_SHEET_BPP) -p! -ftb -fh! -o $(@:.img.bin=)

# OBJ sheets (docs/graphics.md, "OBJ sheets"): tile sets made of whole
# OBJs (1D mapping, each OBJ's tiles back to back), drawn with each OBJ
# whole. tools/obj_sheet.py to-tiles puts their tiles back in the order the
# game uploads them, then grit converts that as it would the plain sheet.
# OBJ_SHEET_<sheet>: the layout (OBJ sizes in tile order, then a grid or
# each OBJ's position), and for the title's OBJ packages the width of
# their map (--width), which grit reduces like the other bg_package maps.
OBJ_SHEET_TILE_SETS := intro/00_5a4e70_tiles intro/00_5a551c_tiles intro/74_636ef4_tiles intro/75_637604_tiles
OBJ_SHEET_PACKAGES := bg_package/62c2c8_title_bandicoot_obj bg_package/62c4b0_title_crash_obj \
	bg_package/62ce70_title_arrow2_obj bg_package/62cc3c_title_arrow1_obj
OBJ_SHEET_intro/00_5a4e70_tiles := --obj 8x16*77 --obj 16x8*2 --columns 16 --cell 16x16
OBJ_SHEET_intro/00_5a551c_tiles := --obj 16x16*75 --columns 16
OBJ_SHEET_intro/74_636ef4_tiles := --obj 16x32*18 --columns 18
OBJ_SHEET_intro/75_637604_tiles := --obj 32x16*4 --columns 4
OBJ_SHEET_bg_package/62c2c8_title_bandicoot_obj := --obj 32x16*3 --columns 3 --width 4
OBJ_SHEET_bg_package/62c4b0_title_crash_obj := --obj 64x64*5 --columns 5 --width 8
# The arrows: their four 32x32 OBJs at gTitleArrowPieceOffsets (title
# screen slot 0 draws arrow 2, slot 1 arrow 1).
OBJ_SHEET_bg_package/62ce70_title_arrow2_obj := --obj 32x32*4 --at 0,0 --at 15,32 --at 32,64 --at 49,96 --width 4
OBJ_SHEET_bg_package/62cc3c_title_arrow1_obj := --obj 32x32*4 --at 61,0 --at 34,32 --at 5,61 --at 0,70 --width 4

define OBJ_SHEET_TILES_RULE
$(GRAPHICS_BUILDDIR)/$(1).img.bin: graphics/$(1).png tools/obj_sheet.py | $(GRIT)
	@mkdir -p $$(dir $$@)
	python3 tools/obj_sheet.py to-tiles $$< $(GRAPHICS_BUILDDIR)/$(1)_objs.png $(OBJ_SHEET_$(1))
	$(GRIT) $(GRAPHICS_BUILDDIR)/$(1)_objs.png -gt -gB4 -p! -ftb -fh! -o $(GRAPHICS_BUILDDIR)/$(1)
endef
define OBJ_SHEET_PACKAGE_RULE
$(GRAPHICS_BUILDDIR)/$(1)_grit.img.bin $(GRAPHICS_BUILDDIR)/$(1)_grit.map.bin &: graphics/$(1).png tools/obj_sheet.py | $(GRIT)
	@mkdir -p $$(dir $$@)
	python3 tools/obj_sheet.py to-tiles $$< $(GRAPHICS_BUILDDIR)/$(1)_objs.png $(OBJ_SHEET_$(1))
	$(GRIT) $(GRAPHICS_BUILDDIR)/$(1)_objs.png -gt -gB4 -p! -m -mLf -mRtf -ftb -fh! -o $(GRAPHICS_BUILDDIR)/$(1)_grit
endef
$(foreach s,$(OBJ_SHEET_TILE_SETS),$(eval $(call OBJ_SHEET_TILES_RULE,$(s))))
$(foreach s,$(OBJ_SHEET_PACKAGES),$(eval $(call OBJ_SHEET_PACKAGE_RULE,$(s))))

# The C bytes of an LZ77 stream, trimmed to the stream: gbagfx pads its
# output to 4 bytes, which the definitions' aligned(4) gives back where the
# ROM has it.
$(GRAPHICS_BUILDDIR)/%.lz.inc: $(GRAPHICS_BUILDDIR)/%.lz tools/bin2c.py
	python3 tools/bin2c.py $< $@ --lz

$(C_BUILDDIR)/data/%.o: CPPFLAGS += -iquote $(GRAPHICS_BUILDDIR)
# The 24 cutscene pictures (palette + Mode 4 bitmap each).
CUTSCENE_PICTURE_PNGS := $(wildcard graphics/intro/*_bitmap.png)
$(C_BUILDDIR)/data/cutscene_pictures_5a9f70.o: $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin.lz.inc,$(CUTSCENE_PICTURE_PNGS)) \
	$(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.pal.inc,$(CUTSCENE_PICTURE_PNGS))

# The two framed OBJ sprite sheets.
$(C_BUILDDIR)/data/sprite_sheet_0b2120.o: $(GRAPHICS_BUILDDIR)/unknown/00_0b2120.bin.lz.inc
$(C_BUILDDIR)/data/sprite_sheet_14174c.o: $(GRAPHICS_BUILDDIR)/unknown/01_14174c.bin.lz.inc

# The two HUD fonts.
FONT_TILE_PNGS := $(wildcard graphics/intro/00_*_tiles.png)
$(C_BUILDDIR)/data/font_tiles_5a4e70.o: $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin.lz.inc,$(FONT_TILE_PNGS))

# The menu, title and credits graphics (0x0861BADC-0x086377C0): the
# bg_package pictures (below) and the credits and VV logo tile sheets, each
# with the palette of its PNG.
LOGO_TILE_PNGS := $(filter-out $(FONT_TILE_PNGS),$(wildcard graphics/intro/*_tiles.png))
MENU_GRAPHICS_INCS := $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin.lz.inc,$(LOGO_TILE_PNGS)) \
	$(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.gbapal.lz.inc,$(LOGO_TILE_PNGS))
$(C_BUILDDIR)/data/menu_graphics_61badc.o: $(MENU_GRAPHICS_INCS)

# The level-select cursor zoom, the ten level-select pictures and their
# palettes, each the palette of its picture's PNG (graphics/tileset1/,
# 0x086377C0-0x0863D4C0).
$(C_BUILDDIR)/data/level_select_graphics_6377c0.o: \
	$(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin.lz.inc,$(wildcard graphics/tileset1/*_tiles.png)) \
	$(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.gbapal.lz.inc,$(wildcard graphics/tileset1/*_8bpp_tiles.png))

# Actor-category backgrounds (docs/data.md, "Category backgrounds"): the
# BG0 cell animations and BG1 pictures of graphics/category_bg/. They are
# uncompressed, so grit's -ftb output goes straight into C initializers;
# tools/grit_bg.py only splits/interleaves it into the ROM's record layout.
# Every PNG is 8bpp indexed with the asset's 256-colour palette, each 8x8
# cell drawn in one 16-colour bank (index = bank * 16 + 4bpp pixel), which
# grit's -mRtp map reduction turns back into the bank bits.
CATBG_DIR := $(GRAPHICS_BUILDDIR)/category_bg

# Cell animation: all frames stacked in one tall PNG (row-major tiles, so
# the tile stream is the frames back to back), plus the 256-colour palette.
$(CATBG_DIR)/%_cell_anim.img.bin $(CATBG_DIR)/%_cell_anim.pal.bin: graphics/category_bg/%_cell_anim.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p -pn256 -ftb -fh! -o $(CATBG_DIR)/$*_cell_anim
# ... and its per-cell palette banks: only the bank bits of this map are
# used (its tile indices are a reduced set and are ignored).
$(CATBG_DIR)/%_cell_anim_banks.map.bin: graphics/category_bg/%_cell_anim.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -m -mRtp -mLf -ftb -fh! -o $(CATBG_DIR)/$*_cell_anim_banks
$(CATBG_DIR)/03b8b0_cell_anim_frames.inc: $(CATBG_DIR)/03b8b0_cell_anim.img.bin $(CATBG_DIR)/03b8b0_cell_anim_banks.map.bin tools/grit_bg.py
	python3 tools/grit_bg.py frames $< $@ --cells 247 --banks $(CATBG_DIR)/03b8b0_cell_anim_banks.map.bin
$(CATBG_DIR)/0ff1b0_cell_anim_frames.inc: $(CATBG_DIR)/0ff1b0_cell_anim.img.bin tools/grit_bg.py
	python3 tools/grit_bg.py frames $< $@ --cells 380

# BG1 picture: the tile set is its own 8px-wide strip PNG (grit's external
# tileset, -fx, so the map indices are the ROM's), the picture PNG gives
# the palette and, reduced against that tile set, the map.
# grit doesn't only read the -fx tile set, it also writes it back when it
# exits. Pointing -fx at graphics/ rewrote a source PNG while the
# %_picture_tiles.img.bin rule could be reading it, so a parallel clean
# build sometimes failed with "grit: can't read '..._picture_tiles.png'".
# The map rule therefore works on a private copy under build/.
$(CATBG_DIR)/%_picture.pal.bin: graphics/category_bg/%_picture.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -g! -p -pn256 -ftb -fh! -o $(CATBG_DIR)/$*_picture
$(CATBG_DIR)/%_picture_tiles.img.bin: graphics/category_bg/%_picture_tiles.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -ftb -fh! -o $(CATBG_DIR)/$*_picture_tiles
$(CATBG_DIR)/%_picture_map.map.bin: graphics/category_bg/%_picture.png graphics/category_bg/%_picture_tiles.png | $(GRIT)
	@mkdir -p $(dir $@)
	cp $(word 2,$^) $(CATBG_DIR)/$*_picture_map_fx.png
	$(GRIT) $< -gt -gB4 -p! -m -mRtp -mLf -fx $(CATBG_DIR)/$*_picture_map_fx.png -ftb -fh! -o $(CATBG_DIR)/$*_picture_map
$(CATBG_DIR)/%_picture_map.inc: $(CATBG_DIR)/%_picture_map.map.bin tools/grit_bg.py
	python3 tools/grit_bg.py map $< $@
$(CATBG_DIR)/%_picture_banks.inc: $(CATBG_DIR)/%_picture_map.map.bin tools/grit_bg.py
	python3 tools/grit_bg.py banks $< $@

$(CATBG_DIR)/%.pal.inc: $(CATBG_DIR)/%.pal.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@ --u16
$(CATBG_DIR)/%.img.inc: $(CATBG_DIR)/%.img.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@

catbg_picture_incs = $(foreach p,$(1),$(CATBG_DIR)/$(p)_picture.pal.inc $(CATBG_DIR)/$(p)_picture_tiles.img.inc $(CATBG_DIR)/$(p)_picture_map.inc $(CATBG_DIR)/$(p)_picture_banks.inc)
$(C_BUILDDIR)/data/cell_anim_03b8b0.o: $(CATBG_DIR)/03b8b0_cell_anim.pal.inc $(CATBG_DIR)/03b8b0_cell_anim_frames.inc
$(C_BUILDDIR)/data/cell_anim_0ff1b0.o: $(CATBG_DIR)/0ff1b0_cell_anim.pal.inc $(CATBG_DIR)/0ff1b0_cell_anim_frames.inc $(call catbg_picture_incs,13d934)
$(C_BUILDDIR)/data/bg_picture_151ac4.o: $(call catbg_picture_incs,151ac4 155260)

# The struct bg_package pictures (graphics/bg_package/, docs/graphics.md
# "Menu and title pictures"): the menu and title backgrounds and the four
# title OBJ sprites. The tile set and map of each are rebuilt from one PNG
# of the whole picture. grit reduces the picture to tiles + map, and
# tools/bg_map.py first-use puts the tiles in the ROM's order (order of
# first use; grit starts with a forced blank tile). gbagfx compresses, and
# src/data/menu_graphics_61badc.cpp holds the results. Each package's
# palette is its PNG's palette.
BG_PACKAGE_DIR := $(GRAPHICS_BUILDDIR)/bg_package
BG_PACKAGE_NAMES := $(basename $(notdir $(wildcard graphics/bg_package/*.png)))
$(C_BUILDDIR)/data/menu_graphics_61badc.o: \
	$(foreach n,$(BG_PACKAGE_NAMES),$(BG_PACKAGE_DIR)/$(n).img.bin.lz.inc $(BG_PACKAGE_DIR)/$(n).map.bin.lz.inc \
	$(BG_PACKAGE_DIR)/$(n).gbapal.lz.inc)

BG_PACKAGE_BPP_61c30c_menu_sky := 4
BG_PACKAGE_BPP_61e5f8_pause_menu := 8
BG_PACKAGE_BPP_62556c_level_select_page := 8
BG_PACKAGE_BPP_628c50_continue_prompt_smoke := 4
BG_PACKAGE_BPP_62a958_continue_prompt_glow := 4
BG_PACKAGE_BPP_62b34c_continue_prompt_uka_uka := 8
BG_PACKAGE_BPP_62d0cc_universal_logo := 8
BG_PACKAGE_BPP_62e3b0_title_screen := 8
BG_PACKAGE_BPP_62c2c8_title_bandicoot_obj := 4
BG_PACKAGE_BPP_62c4b0_title_crash_obj := 4
BG_PACKAGE_BPP_62cc3c_title_arrow1_obj := 4
BG_PACKAGE_BPP_62ce70_title_arrow2_obj := 4
# Tile reduction: tiles and flips, except for the affine maps (no flips):
# the title and the universal logo.
BG_PACKAGE_REDUCE = $(if $(filter 62e3b0_title_screen 62d0cc_universal_logo,$(1)),-mRt,-mRtf)

$(BG_PACKAGE_DIR)/%_grit.img.bin $(BG_PACKAGE_DIR)/%_grit.map.bin: graphics/bg_package/%.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB$(BG_PACKAGE_BPP_$*) -p! -m -mLf $(call BG_PACKAGE_REDUCE,$*) -ftb -fh! -o $(BG_PACKAGE_DIR)/$*_grit
$(BG_PACKAGE_DIR)/%.img.bin $(BG_PACKAGE_DIR)/%.map.bin: $(BG_PACKAGE_DIR)/%_grit.img.bin $(BG_PACKAGE_DIR)/%_grit.map.bin tools/bg_map.py
	python3 tools/bg_map.py first-use $(word 1,$^) $(word 2,$^) $(BG_PACKAGE_DIR)/$*.img.bin $(BG_PACKAGE_DIR)/$*.map.bin --bpp $(BG_PACKAGE_BPP_$*) $(addprefix --high ,$(filter %_high.json,$^))

# The universal logo's affine map has 5 entries with a stray high byte,
# which nothing reads (bg_map.py first-use --high puts them back).
$(BG_PACKAGE_DIR)/62d0cc_universal_logo.img.bin $(BG_PACKAGE_DIR)/62d0cc_universal_logo.map.bin: graphics/bg_package/62d0cc_universal_logo_high.json

# The sprite banks (graphics/sprites/bankNN_<name>/, docs/graphics.md
# "Sprite banks"): a PNG per frame and bank.json per bank.
# tools/sprite_frames.py pack cuts the frames into the tile pool, one
# tiles .inc per bank, generates the frames' piece arrays (a pieces .inc
# per bank) and sprite_frames.h with each frame's tile offset. One run for
# all banks, since a bank's offset depends on every earlier bank's size.
SPRITE_BANK_DIRS := $(sort $(wildcard graphics/sprites/bank[0-9][0-9]_*))
SPRITE_FRAME_SOURCES := $(foreach d,$(SPRITE_BANK_DIRS),$(d)/bank.json $(wildcard $(d)/f*.png))
SPRITE_FRAMES_DIR := $(GRAPHICS_BUILDDIR)/sprites
SPRITE_FRAMES_OUT := $(SPRITE_FRAMES_DIR)/sprite_frames.h \
	$(foreach d,$(SPRITE_BANK_DIRS),$(SPRITE_FRAMES_DIR)/$(word 1,$(subst _, ,$(notdir $(d))))_tiles.inc \
	$(SPRITE_FRAMES_DIR)/$(word 1,$(subst _, ,$(notdir $(d))))_pieces.inc)
$(SPRITE_FRAMES_OUT) &: $(SPRITE_FRAME_SOURCES) tools/sprite_frames.py
	python3 tools/sprite_frames.py pack $(SPRITE_FRAMES_DIR)

$(C_BUILDDIR)/data/sprite_tiles_2bf120.o $(foreach f,4a5600 4b0ae0 4b414c 4b9d7c,$(C_BUILDDIR)/data/sprite_banks_$(f).o): $(SPRITE_FRAMES_OUT)

# The 125 OBJ palettes (gObjPalettes): one JASC file per paletteId,
# graphics/sprites/palettes/NNN.pal, back to back in index order.
OBJ_PALETTE_PALS := $(sort $(wildcard graphics/sprites/palettes/*.pal))
$(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin: $(patsubst graphics/%.pal,$(GRAPHICS_BUILDDIR)/%.gbapal,$(OBJ_PALETTE_PALS))
	@mkdir -p $(dir $@)
	cat $^ > $@
$(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin.inc: $(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@
$(C_BUILDDIR)/data/sprite_tiles_2bf120.o: $(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin.inc

# Raw (uncompressed) level tile sets of gLanguageSelectPalette3, see
# docs/data.md: the five tag-0x00 level BG tile sets (8bpp,
# graphics/level_tilesets/). grit does the layout, tools/bin2c.py the
# initializer bytes. The PNGs are padded to whole rows of tiles with blank
# tiles (tools/tile_pools.py, which also extracts them), so each one's real
# byte count is listed below, and bin2c --size trims the (all-zero) padding
# again.
TILE_POOL_8BPP_PNGS := $(wildcard graphics/level_tilesets/*.png)

TILE_BYTES_tileset1_17e7ac := 0x67b80
TILE_BYTES_tileset2_1e6330 := 0x1aac0
TILE_BYTES_tileset3_200df4 := 0x4a840
TILE_BYTES_tileset4_270f08 := 0x28ec0
TILE_BYTES_tileset5_299dcc := 0x1f400

$(GRAPHICS_BUILDDIR)/level_tilesets/%.img.bin: graphics/level_tilesets/%.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB8 -p! -ftb -fh! -o $(@:.img.bin=)

$(GRAPHICS_BUILDDIR)/level_tilesets/%.img.bin.inc: $(GRAPHICS_BUILDDIR)/level_tilesets/%.img.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@ --size $(TILE_BYTES_$*)

tile_pool_incs = $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin.inc,$(1))

# The two boss pictures (graphics/boss_pictures/, see docs/data.md "Boss
# pictures"): grit turns each frame into 4bpp tiles in cell order, and
# tools/boss_pictures.py deduplicates them into the ROM's shared tile
# pool and per-frame maps.
BOSS_PICTURE_PNGS := $(wildcard graphics/boss_pictures/*.png)
BOSS_PICTURE_DIR := $(GRAPHICS_BUILDDIR)/boss_pictures

$(BOSS_PICTURE_DIR)/%.img.bin: graphics/boss_pictures/%.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -ftb -fh! -o $(@:.img.bin=)

boss_picture_tiles = $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin,$(sort $(wildcard graphics/boss_pictures/$(1)_frame*.png)))
# $(1): the picture's address, $(2): the same in upper case, $(3): its width in cells.
define BOSS_PICTURE_RULE
$(BOSS_PICTURE_DIR)/$(1).inc $(BOSS_PICTURE_DIR)/$(1).h &: tools/boss_pictures.py $(call boss_picture_tiles,$(1))
	python3 tools/boss_pictures.py pack $(BOSS_PICTURE_DIR)/$(1).inc $(BOSS_PICTURE_DIR)/$(1).h --name BOSS_PICTURE_$(2) --cols $(3) $(call boss_picture_tiles,$(1))
endef
$(eval $(call BOSS_PICTURE_RULE,167cd4,167CD4,18))
$(eval $(call BOSS_PICTURE_RULE,169ce8,169CE8,16))

$(C_BUILDDIR)/data/boss_pictures_167ad4.o: $(foreach p,167cd4 169ce8,$(BOSS_PICTURE_DIR)/$(p).inc $(BOSS_PICTURE_DIR)/$(p).h)
$(C_BUILDDIR)/data/level_tilesets_17e78c.o: $(call tile_pool_incs,$(filter graphics/level_tilesets/tileset1_% graphics/level_tilesets/tileset2_% graphics/level_tilesets/tileset3_%,$(TILE_POOL_8BPP_PNGS)))
$(C_BUILDDIR)/data/level_tilesets_270f08.o: $(call tile_pool_incs,$(filter graphics/level_tilesets/tileset4_% graphics/level_tilesets/tileset5_%,$(TILE_POOL_8BPP_PNGS)))

# Zero-run-compressed OBJ frame sets (the old "rotation strips", see
# docs/data.md "Compressed sprite frames"): one PNG per set, the frames
# stacked top to bottom. grit does the 4bpp layout, tools/rle_sprites.py
# compresses each frame the way the ROM's encoder did and writes the bytes
# plus a header of frame offsets (for the frame pointer tables).
RLE_SPRITE_PNGS := $(wildcard graphics/rle_sprites/*.png)
RLE_SPRITE_DIR := $(GRAPHICS_BUILDDIR)/rle_sprites
RLE_FRAME_0c2758 := 8x8
RLE_FRAME_0da1d8 := 10x10
RLE_FRAME_15a050 := 8x8

$(RLE_SPRITE_DIR)/%_frames.img.bin: graphics/rle_sprites/%_frames.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -ftb -fh! -o $(@:.img.bin=)
$(RLE_SPRITE_DIR)/%_frames.inc $(RLE_SPRITE_DIR)/%_frames.h: $(RLE_SPRITE_DIR)/%_frames.img.bin tools/rle_sprites.py
	python3 tools/rle_sprites.py pack $< $(RLE_SPRITE_DIR)/$*_frames.inc $(RLE_SPRITE_DIR)/$*_frames.h --frame $(RLE_FRAME_$*) --name RLE_SPRITES_$(shell echo $* | tr a-f A-F)

rle_sprite_files = $(foreach s,$(1),$(RLE_SPRITE_DIR)/$(s)_frames.inc $(RLE_SPRITE_DIR)/$(s)_frames.h)
$(C_BUILDDIR)/data/rle_sprites_0c2758.o: $(call rle_sprite_files,0c2758 0da1d8)
$(C_BUILDDIR)/data/rle_sprites_15a050.o: $(call rle_sprite_files,15a050)
$(C_BUILDDIR)/data/frame_table_17a880.o: $(call rle_sprite_files,0da1d8)
$(C_BUILDDIR)/data/anim_family_178f80.o: $(call rle_sprite_files,0c2758) $(GRAPHICS_BUILDDIR)/unknown/00_0b2120_frames.h
$(C_BUILDDIR)/data/anim_family_17aa6c.o: $(call rle_sprite_files,15a050) $(GRAPHICS_BUILDDIR)/unknown/01_14174c_frames.h

# The six 4bpp tile sets of the level BG1-3 layers (graphics/level_bg_tiles/,
# docs/graphics.md "Level BG tile sets"). Each PNG is 8bpp indexed with a
# room's 256-colour BG palette, every tile drawn in the palette bank the
# rooms' maps use it with most (index = bank * 16 + 4bpp pixel), so it shows
# its real colours; grit -gB4 keeps the low nibble. The PNGs are 16 tiles
# wide, padded with blank tiles, which bin2c.py --size trims again.
LEVEL_BG_TILES_DIR := $(GRAPHICS_BUILDDIR)/level_bg_tiles
LEVEL_BG_TILES_PNGS := $(wildcard graphics/level_bg_tiles/*.png)
$(C_BUILDDIR)/data/level_bg_tiles_63d4c0.o: $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.tiles.bin.lz.inc,$(LEVEL_BG_TILES_PNGS))

LEVEL_BG_TILE_BYTES_63d4c0_snow := 0x5be0
LEVEL_BG_TILE_BYTES_640ef8_jungle := 0x5b60
LEVEL_BG_TILE_BYTES_644a88_temple := 0x5f20
LEVEL_BG_TILE_BYTES_649418_sewer := 0x3e60
LEVEL_BG_TILE_BYTES_64bb5c_space := 0x3120
LEVEL_BG_TILE_BYTES_64d7e8_underwater := 0x37a0

$(LEVEL_BG_TILES_DIR)/%.img.bin: graphics/level_bg_tiles/%.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -ftb -fh! -o $(@:.img.bin=)
$(LEVEL_BG_TILES_DIR)/%.tiles.bin: $(LEVEL_BG_TILES_DIR)/%.img.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@ --size $(LEVEL_BG_TILE_BYTES_$*) --bin
