
# Tile-based graphics (sprites, tilesets): plain gbagfx round trip.
%.4bpp: %.png | $(GFX)
	$(GFX) $< $@

%.8bpp: %.png | $(GFX)
	$(GFX) $< $@

# Palettes: JASC .pal text (human editable) -> raw GBA .gbapal.
%.gbapal: %.pal | $(GFX)
	$(GFX) $< $@

# Full-screen Mode 4 bitmaps (240x160, linear/non-tiled framebuffers).
# gbagfx always arranges pixel data into 8x8 tiles, which is the wrong
# layout for these, so they go through tools/linear_gfx.py instead.
# (to regenerate a PNG from a .bin, run tools/linear_gfx.py directly)
%_bitmap.bin: %_bitmap.png
	python3 tools/linear_gfx.py to-bin $< $@

# Generic LZ77 compression, used for every asset type above plus any
# unidentified binary blob that just needs to be repacked byte-for-byte.
%.lz: % | $(GFX)
	$(GFX) $< $@
