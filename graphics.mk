
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
GRIT_C_PNGS := graphics/intro/00_5aa170_bitmap.png \
	$(wildcard graphics/category_bg/*.png)

# Mode 4 bitmaps: linear 8bpp, no palette (it's a separate asset).
$(GRAPHICS_BUILDDIR)/%_bitmap.img.bin: graphics/%_bitmap.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gb -gB8 -p! -ftb -fh! -o $(@:.img.bin=)

$(GRAPHICS_BUILDDIR)/%.lz.inc: $(GRAPHICS_BUILDDIR)/%.lz tools/bin2c.py
	python3 tools/bin2c.py $< $@ --lz

$(C_BUILDDIR)/data/%.o: CPPFLAGS += -iquote $(GRAPHICS_BUILDDIR)
$(C_BUILDDIR)/data/intro_bitmap_5aa170.o: $(GRAPHICS_BUILDDIR)/intro/00_5aa170_bitmap.img.bin.lz.inc

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
