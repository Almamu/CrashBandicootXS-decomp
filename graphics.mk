
# All rules below take a source under graphics/ and produce the
# corresponding built file under $(GRAPHICS_BUILDDIR), mirroring the
# subdirectory layout, so graphics/ only ever holds editable sources.

# Tile-based graphics (sprites, tilesets): plain gbagfx round trip.
$(GRAPHICS_BUILDDIR)/%.4bpp: graphics/%.png | $(GFX)
	@mkdir -p $(dir $@)
	$(GFX) $< $@

$(GRAPHICS_BUILDDIR)/%.8bpp: graphics/%.png | $(GFX)
	@mkdir -p $(dir $@)
	$(GFX) $< $@

# Palettes: JASC .pal text (human editable) -> raw GBA .gbapal.
$(GRAPHICS_BUILDDIR)/%.gbapal: graphics/%.pal | $(GFX)
	@mkdir -p $(dir $@)
	$(GFX) $< $@

# Full-screen Mode 4 bitmaps (240x160, linear/non-tiled framebuffers).
# gbagfx always arranges pixel data into 8x8 tiles, which is the wrong
# layout for these, so they go through tools/linear_gfx.py instead.
# (to regenerate a PNG from a .bin, run tools/linear_gfx.py directly)
$(GRAPHICS_BUILDDIR)/%_bitmap.bin: graphics/%_bitmap.png
	@mkdir -p $(dir $@)
	python3 tools/linear_gfx.py to-bin $< $@

# Raw binary blobs that need no conversion, just repacking byte-for-byte.
$(GRAPHICS_BUILDDIR)/%.bin: graphics/%.bin
	@mkdir -p $(dir $@)
	cp $< $@

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
$(GRAPHICS_BUILDDIR)/unknown/$(1)/$(2).bin: $$(sort $$(wildcard graphics/unknown/$(1)/$(2)/*.png))
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

# Generic LZ77 compression, used for every asset type above.
$(GRAPHICS_BUILDDIR)/%.lz: $(GRAPHICS_BUILDDIR)/% | $(GFX)
	@mkdir -p $(dir $@)
	$(GFX) $< $@

# PNG -> C const array (grit), see docs/graphics.md. grit does the pixel
# layout, gbagfx the byte-exact LZ77 (grit's own -gzl compressor does not
# reproduce the ROM's streams), tools/bin2c.py the initializer bytes that a
# src/data/*.c file #includes. GRIT_C_PNGS lists the PNGs converted this
# way, so the Makefile doesn't also build them through the .s incbin path.
GRIT_C_PNGS := $(wildcard graphics/intro/*_bitmap.png) \
	$(wildcard graphics/category_bg/*.png)

# Mode 4 bitmaps: linear 8bpp, no palette (it's a separate asset).
$(GRAPHICS_BUILDDIR)/%_bitmap.img.bin: graphics/%_bitmap.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gb -gB8 -p! -ftb -fh! -o $(@:.img.bin=)

$(GRAPHICS_BUILDDIR)/%.lz.inc: $(GRAPHICS_BUILDDIR)/%.lz tools/bin2c.py
	python3 tools/bin2c.py $< $@ --lz

$(C_BUILDDIR)/data/%.o: CPPFLAGS += -iquote $(GRAPHICS_BUILDDIR)
# The 24 cutscene pictures (palette + Mode 4 bitmap each).
$(C_BUILDDIR)/data/cutscene_pictures_5a9f70.o: $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin.lz.inc,$(wildcard graphics/intro/*_bitmap.png))

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
$(CATBG_DIR)/%_picture.pal.bin: graphics/category_bg/%_picture.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -g! -p -pn256 -ftb -fh! -o $(CATBG_DIR)/$*_picture
$(CATBG_DIR)/%_picture_tiles.img.bin: graphics/category_bg/%_picture_tiles.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -ftb -fh! -o $(CATBG_DIR)/$*_picture_tiles
$(CATBG_DIR)/%_picture_map.map.bin: graphics/category_bg/%_picture.png graphics/category_bg/%_picture_tiles.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -m -mRtp -mLf -fx $(word 2,$^) -ftb -fh! -o $(CATBG_DIR)/$*_picture_map
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

# Raw (uncompressed) tile pools of gStaticData_0817E78C, see docs/data.md:
# the 56 sprite banks and the 125 fixed OBJ tiles (4bpp, graphics/sprites/)
# and the five tag-0x00 level BG tile sets (8bpp, graphics/level_tilesets/).
# grit does the layout, tools/bin2c.py the initializer bytes. The PNGs are
# padded to whole rows of tiles with blank tiles (tools/tile_pools.py, which
# also extracts them), so each one's real byte count is listed below, and
# bin2c --size trims the (all-zero) padding again.
TILE_POOL_4BPP_PNGS := $(wildcard graphics/sprites/*.png)
TILE_POOL_8BPP_PNGS := $(wildcard graphics/level_tilesets/*.png)
GRIT_C_PNGS += $(TILE_POOL_4BPP_PNGS) $(TILE_POOL_8BPP_PNGS)

TILE_BYTES_bank00_2bf120 := 0x3cfe0
TILE_BYTES_bank01_2fc100 := 0x2b580
TILE_BYTES_bank02_327680 := 0x7a60
TILE_BYTES_bank03_32f0e0 := 0x6760
TILE_BYTES_bank04_335840 := 0x7f20
TILE_BYTES_bank05_33d760 := 0x33c0
TILE_BYTES_bank06_340b20 := 0x13c0
TILE_BYTES_bank07_341ee0 := 0x70c0
TILE_BYTES_bank08_348fa0 := 0x36c0
TILE_BYTES_bank09_34c660 := 0x14a0
TILE_BYTES_bank10_34db00 := 0x60c0
TILE_BYTES_bank11_353bc0 := 0x5a60
TILE_BYTES_bank12_359620 := 0xaca0
TILE_BYTES_bank13_3642c0 := 0x20e0
TILE_BYTES_bank14_3663a0 := 0x4d40
TILE_BYTES_bank15_36b0e0 := 0x5e40
TILE_BYTES_bank16_370f20 := 0x5940
TILE_BYTES_bank17_376860 := 0x25a0
TILE_BYTES_bank18_378e00 := 0x3ae0
TILE_BYTES_bank19_37c8e0 := 0x3ec0
TILE_BYTES_bank20_3807a0 := 0x23e0
TILE_BYTES_bank21_382b80 := 0x1800
TILE_BYTES_bank22_384380 := 0x5a80
TILE_BYTES_bank23_389e00 := 0x13dc0
TILE_BYTES_bank24_39dbc0 := 0xd560
TILE_BYTES_bank25_3ab120 := 0x7880
TILE_BYTES_bank26_3b29a0 := 0x3a80
TILE_BYTES_bank27_3b6420 := 0x2260
TILE_BYTES_bank28_3b8680 := 0x300
TILE_BYTES_bank29_3b8980 := 0xbf80
TILE_BYTES_bank30_3c4900 := 0x194c0
TILE_BYTES_bank31_3dddc0 := 0x145a0
TILE_BYTES_bank32_3f2360 := 0xcc0
TILE_BYTES_bank33_3f3020 := 0x1c00
TILE_BYTES_bank34_3f4c20 := 0xfe0
TILE_BYTES_bank35_3f5c00 := 0x1640
TILE_BYTES_bank36_3f7240 := 0x8a0
TILE_BYTES_bank37_3f7ae0 := 0xa00
TILE_BYTES_bank38_3f84e0 := 0x5500
TILE_BYTES_bank39_3fd9e0 := 0x6680
TILE_BYTES_bank40_404060 := 0x3020
TILE_BYTES_bank41_407080 := 0xb00
TILE_BYTES_bank42_407b80 := 0x3e0
TILE_BYTES_bank43_407f60 := 0x3da0
TILE_BYTES_bank44_40bd00 := 0x660
TILE_BYTES_bank45_40c360 := 0x3400
TILE_BYTES_bank46_40f760 := 0x140
TILE_BYTES_bank47_40f8a0 := 0x1ec0
TILE_BYTES_bank48_411760 := 0x8000
TILE_BYTES_bank49_419760 := 0x380
TILE_BYTES_bank50_419ae0 := 0x26a0
TILE_BYTES_bank51_41c180 := 0xd40
TILE_BYTES_bank52_41cec0 := 0x200
TILE_BYTES_bank53_41d0c0 := 0x103c0
TILE_BYTES_bank54_42d480 := 0x3dd20
TILE_BYTES_bank55_46b1a0 := 0x394c0
TILE_BYTES_tile_pool_4a4660 := 0xfa0
TILE_BYTES_tileset1_17e7ac := 0x67b80
TILE_BYTES_tileset2_1e6330 := 0x1aac0
TILE_BYTES_tileset3_200df4 := 0x4a840
TILE_BYTES_tileset4_270f08 := 0x28ec0
TILE_BYTES_tileset5_299dcc := 0x1f400

$(GRAPHICS_BUILDDIR)/sprites/%.img.bin: graphics/sprites/%.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -ftb -fh! -o $(@:.img.bin=)

$(GRAPHICS_BUILDDIR)/level_tilesets/%.img.bin: graphics/level_tilesets/%.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB8 -p! -ftb -fh! -o $(@:.img.bin=)

$(GRAPHICS_BUILDDIR)/sprites/%.img.bin.inc: $(GRAPHICS_BUILDDIR)/sprites/%.img.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@ --size $(TILE_BYTES_$*)

$(GRAPHICS_BUILDDIR)/level_tilesets/%.img.bin.inc: $(GRAPHICS_BUILDDIR)/level_tilesets/%.img.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@ --size $(TILE_BYTES_$*)

tile_pool_incs = $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin.inc,$(1))
$(C_BUILDDIR)/data/level_tilesets_17e78c.o: $(call tile_pool_incs,$(filter graphics/level_tilesets/tileset1_% graphics/level_tilesets/tileset2_% graphics/level_tilesets/tileset3_%,$(TILE_POOL_8BPP_PNGS)))
$(C_BUILDDIR)/data/level_tilesets_270f08.o: $(call tile_pool_incs,$(filter graphics/level_tilesets/tileset4_% graphics/level_tilesets/tileset5_%,$(TILE_POOL_8BPP_PNGS)))
$(C_BUILDDIR)/data/sprite_tiles_2bf120.o: $(call tile_pool_incs,$(TILE_POOL_4BPP_PNGS))

# Zero-run-compressed OBJ frame sets (the old "rotation strips", see
# docs/data.md "Compressed sprite frames"): one PNG per set, the frames
# stacked top to bottom. grit does the 4bpp layout, tools/rle_sprites.py
# compresses each frame the way the ROM's encoder did and writes the bytes
# plus a header of frame offsets (for the frame pointer tables).
RLE_SPRITE_PNGS := $(wildcard graphics/rle_sprites/*.png)
GRIT_C_PNGS += $(RLE_SPRITE_PNGS)
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
