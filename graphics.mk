
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
