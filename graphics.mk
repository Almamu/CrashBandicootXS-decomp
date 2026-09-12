
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

# Framed OBJ sprite sheets (per-record PNG + a ".frames" sidecar listing
# each frame's tile dimensions) - see tools/framed_gfx.py's header comment
# and docs/graphics.md, "Found the real per-actor animation-frame system".
# gbagfx can't be used directly: each frame has a 4-byte non-pixel header
# and frames within one record can differ in size, neither of which fits
# a plain PNG<->4bpp round trip.
$(GRAPHICS_BUILDDIR)/%.bin: graphics/%.png graphics/%.frames
	@mkdir -p $(dir $@)
	python3 tools/framed_gfx.py to-bin $< $(word 2,$^) $@

# Split multi-record sprite sheets: each ROM asset below was originally one
# big LZ77 stream, so it must be recompressed as a single unit - the
# individual records under graphics/unknown/<name>/ only exist to make the
# source human-navigable (per-object byte ranges), and get concatenated
# back in filename order (numeric prefixes preserve original ROM order)
# before compression.
$(GRAPHICS_BUILDDIR)/unknown/00_0b2120.bin: $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.bin,$(sort $(wildcard graphics/unknown/00_0b2120/*.png)))
	@mkdir -p $(dir $@)
	cat $^ > $@

$(GRAPHICS_BUILDDIR)/unknown/01_14174c.bin: $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.bin,$(sort $(wildcard graphics/unknown/01_14174c/*.png)))
	@mkdir -p $(dir $@)
	cat $^ > $@

# Generic LZ77 compression, used for every asset type above.
$(GRAPHICS_BUILDDIR)/%.lz: $(GRAPHICS_BUILDDIR)/% | $(GFX)
	@mkdir -p $(dir $@)
	$(GFX) $< $@
