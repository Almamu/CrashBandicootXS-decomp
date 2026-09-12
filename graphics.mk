
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

# Generic LZ77 compression, used for every asset type above.
$(GRAPHICS_BUILDDIR)/%.lz: $(GRAPHICS_BUILDDIR)/% | $(GFX)
	@mkdir -p $(dir $@)
	$(GFX) $< $@
