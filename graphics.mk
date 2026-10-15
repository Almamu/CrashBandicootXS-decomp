
# XS's graphics. The rules the manifest drives are graphics_rules.mk's
# (shared with N-Tranced's games/n2/graphics.mk, #1115), included below;
# this file has XS's own: the lists those rules read, which go first, and
# the rules and prerequisites for XS's assets the manifest doesn't drive.

# OBJ sheets (docs/graphics.md, "OBJ sheets"): tile sets made of whole
# OBJs (1D mapping, each OBJ's tiles back to back), drawn with each OBJ
# whole. tools/obj_sheet.py to-tiles puts their tiles back in the order the
# game uploads them, then grit converts that as it would the plain sheet.
# OBJ_SHEET_<sheet>: the layout (OBJ sizes in tile order, then a grid or
# each OBJ's position), and for the title's OBJ packages the width of
# their map (--width), which grit reduces like the other BG packages' maps.
OBJ_SHEET_TILE_SETS := fonts/small fonts/large logos/vicarious_visions/letters logos/vicarious_visions/url
OBJ_SHEET_PACKAGES := title/bandicoot title/crash \
	title/arrow2 title/arrow1
OBJ_SHEET_fonts/small := --obj 8x16*77 --obj 16x8*2 --columns 16 --cell 16x16
OBJ_SHEET_fonts/large := --obj 16x16*75 --columns 16
OBJ_SHEET_logos/vicarious_visions/letters := --obj 16x32*18 --columns 18
OBJ_SHEET_logos/vicarious_visions/url := --obj 32x16*4 --columns 4
OBJ_SHEET_title/bandicoot := --obj 32x16*3 --columns 3 --width 4
OBJ_SHEET_title/crash := --obj 64x64*5 --columns 5 --width 8
# The arrows: their four 32x32 OBJs at gTitleArrowPieceOffsets (title
# screen slot 0 draws arrow 2, slot 1 arrow 1).
OBJ_SHEET_title/arrow2 := --obj 32x32*4 --at 0,0 --at 15,32 --at 32,64 --at 49,96 --width 4
OBJ_SHEET_title/arrow1 := --obj 32x32*4 --at 61,0 --at 34,32 --at 5,61 --at 0,70 --width 4

# The universal logo's affine map has 5 entries with a stray high byte,
# which nothing reads (bg_map.py first-use --high puts them back).
BG_PACKAGE_HIGH_logos/universal := graphics/logos/universal_high.json

include graphics_rules.mk

# The cutscene pictures (graphics/cutscenes/NN_<what>.png, gCutscenePictureNN):
# Mode 4 bitmaps, linear 8bpp. Their palette is the PNG's own (256
# colours, the .gbapal rule), an uncompressed u16 array (bin2c --u16).
$(GRAPHICS_BUILDDIR)/cutscenes/%.img.bin: graphics/cutscenes/%.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gb -gB8 -p! -ftb -fh! -o $(@:.img.bin=)
$(GRAPHICS_BUILDDIR)/cutscenes/%.pal.inc: $(GRAPHICS_BUILDDIR)/cutscenes/%.gbapal tools/bin2c.py
	python3 tools/bin2c.py $< $@ --u16

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

$(DATA_OBJS): CPPFLAGS += -iquote $(GRAPHICS_BUILDDIR)
# The 24 cutscene pictures (palette + Mode 4 bitmap each).
CUTSCENE_PICTURE_PNGS := $(wildcard graphics/cutscenes/*.png)
$(C_BUILDDIR)/game/ui/data/cutscene_pictures.o: $(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.img.bin.lz.inc,$(CUTSCENE_PICTURE_PNGS)) \
	$(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.pal.inc,$(CUTSCENE_PICTURE_PNGS))

# The two framed OBJ sprite sheets.
$(C_BUILDDIR)/game/vehicles/polar/data/polar_sprite_sheet.o: $(GRAPHICS_BUILDDIR)/vehicles/polar/sprite_sheet.bin.lz.inc
$(C_BUILDDIR)/game/vehicles/jetpack/data/jetpack_sprite_sheet.o: $(GRAPHICS_BUILDDIR)/vehicles/jetpack/sprite_sheet.bin.lz.inc

# The two HUD fonts (graphics/fonts/).
$(C_BUILDDIR)/engine/text/data/font_tiles.o: $(GRAPHICS_BUILDDIR)/fonts/small.img.bin.lz.inc $(GRAPHICS_BUILDDIR)/fonts/large.img.bin.lz.inc

# The menu, title and credits graphics (0x0861BADC-0x086377C0: the BG
# packages, and the credits and VV logo tile sheets) and the
# level-select cursor zoom, pictures and palettes (0x086377C0-0x0863D4C0)
# are graphics classes: graphics_manifest.mk gives their objects'
# prerequisites (see "The graphics classes" in graphics_rules.mk).

$(C_BUILDDIR)/engine/actor/data/actor_palette_cycle_frames.o: \
	$(GRAPHICS_BUILDDIR)/vehicles/polar/palette_cycle.bin.inc

$(C_BUILDDIR)/game/vehicles/polar/data/polar_cell_anim.o: $(GRAPHICS_BUILDDIR)/vehicles/polar/ice_wall.pal.inc \
	$(GRAPHICS_BUILDDIR)/vehicles/polar/ice_wall_frames.inc
$(C_BUILDDIR)/game/vehicles/jetpack/data/jetpack_cell_anim.o: $(GRAPHICS_BUILDDIR)/vehicles/jetpack/clouds.pal.inc \
	$(GRAPHICS_BUILDDIR)/vehicles/jetpack/clouds_frames.inc $(call bg_picture_incs,vehicles/jetpack/skies/rocket_racket)
$(C_BUILDDIR)/game/vehicles/jetpack/data/jetpack_sky_pictures.o: $(call bg_picture_incs,vehicles/jetpack/skies/blimp_bonanza vehicles/jetpack/skies/no_fly_zone)

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

$(C_BUILDDIR)/engine/gfx/data/sprite_tiles.o $(foreach f,00_09 10_21 22_38 39_55,$(call data_obj,sprite_banks_$(f))): $(SPRITE_FRAMES_OUT)

# The 125 OBJ palettes (gObjPalettes): one JASC file per paletteId,
# graphics/sprites/palettes/NNN_<owner>.pal (NNN the paletteId, <owner>
# the first sprite bank whose animations use it, or `unused`), back to
# back in index order.
OBJ_PALETTE_PALS := $(sort $(wildcard graphics/sprites/palettes/*.pal))
$(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin: $(patsubst graphics/%.pal,$(GRAPHICS_BUILDDIR)/%.gbapal,$(OBJ_PALETTE_PALS))
	@mkdir -p $(dir $@)
	cat $^ > $@
$(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin.inc: $(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin tools/bin2c.py
	python3 tools/bin2c.py $< $@
$(C_BUILDDIR)/engine/gfx/data/sprite_tiles.o: $(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin.inc

# The two boss pictures (docs/data.md "Boss pictures"): the airship
# (graphics/vehicles/jetpack/airship/frameN.png, 4 frames) and N. Gin's
# hovercraft (graphics/bosses/hovercraft.png, 1 frame). grit turns each
# frame into 4bpp tiles in cell order, and tools/boss_pictures.py
# deduplicates them into the ROM's shared tile pool and per-frame maps.
AIRSHIP_FRAMES := $(addprefix vehicles/jetpack/airship/frame,0 1 2 3)
BOSS_PICTURE_FRAMES := $(AIRSHIP_FRAMES) bosses/hovercraft
$(patsubst %,$(GRAPHICS_BUILDDIR)/%.img.bin,$(BOSS_PICTURE_FRAMES)): $(GRAPHICS_BUILDDIR)/%.img.bin: graphics/%.png | $(GRIT)
	@mkdir -p $(dir $@)
	$(GRIT) $< -gt -gB4 -p! -ftb -fh! -o $(@:.img.bin=)

# $(1): the picture's built name, $(2): its frames, $(3): its macro prefix,
# $(4): its width in cells.
define BOSS_PICTURE_RULE
$(GRAPHICS_BUILDDIR)/$(1).inc $(GRAPHICS_BUILDDIR)/$(1).h &: tools/boss_pictures.py $(patsubst %,$(GRAPHICS_BUILDDIR)/%.img.bin,$(2))
	@mkdir -p $$(dir $$@)
	python3 tools/boss_pictures.py pack $(GRAPHICS_BUILDDIR)/$(1).inc $(GRAPHICS_BUILDDIR)/$(1).h --name $(3) --cols $(4) $(patsubst %,$(GRAPHICS_BUILDDIR)/%.img.bin,$(2))
endef
$(eval $(call BOSS_PICTURE_RULE,vehicles/jetpack/airship,$(AIRSHIP_FRAMES),AIRSHIP_PICTURE,18))
$(eval $(call BOSS_PICTURE_RULE,bosses/hovercraft,bosses/hovercraft,HOVERCRAFT_PICTURE,16))

$(C_BUILDDIR)/game/bosses/data/boss_pictures.o: $(foreach p,vehicles/jetpack/airship bosses/hovercraft,$(GRAPHICS_BUILDDIR)/$(p).inc $(GRAPHICS_BUILDDIR)/$(p).h)
# Zero-run-compressed OBJ frame sets (the old "rotation strips", see
# docs/data.md "Compressed sprite frames"): one PNG per set, the frames
# stacked top to bottom: the polar player and the Yeti
# (graphics/vehicles/polar/player.png, yeti.png) and the jetpack player
# (graphics/vehicles/jetpack/player.png). grit does the 4bpp layout,
# tools/rle_sprites.py compresses each frame the way the ROM's encoder did
# and writes the bytes plus a header of frame offsets (for the frame
# pointer tables), RLE_SPRITES_<name>_FRAME_nnn.
# $(1): the set, $(2): its frame size in tiles, $(3): its macro name.
define RLE_SPRITE_RULE
$(GRAPHICS_BUILDDIR)/$(1).img.bin: graphics/$(1).png | $(GRIT)
	@mkdir -p $$(dir $$@)
	$(GRIT) $$< -gt -gB4 -p! -ftb -fh! -o $(GRAPHICS_BUILDDIR)/$(1)
$(GRAPHICS_BUILDDIR)/$(1).inc $(GRAPHICS_BUILDDIR)/$(1).h &: $(GRAPHICS_BUILDDIR)/$(1).img.bin tools/rle_sprites.py
	python3 tools/rle_sprites.py pack $$< $(GRAPHICS_BUILDDIR)/$(1).inc $(GRAPHICS_BUILDDIR)/$(1).h --frame $(2) --name RLE_SPRITES_$(3)
endef
$(eval $(call RLE_SPRITE_RULE,vehicles/polar/player,8x8,POLAR_PLAYER))
$(eval $(call RLE_SPRITE_RULE,vehicles/polar/yeti,10x10,YETI))
$(eval $(call RLE_SPRITE_RULE,vehicles/jetpack/player,8x8,JETPACK_PLAYER))

rle_sprite_files = $(foreach s,$(1),$(GRAPHICS_BUILDDIR)/$(s).inc $(GRAPHICS_BUILDDIR)/$(s).h)
$(C_BUILDDIR)/game/vehicles/polar/data/polar_rle_sprites.o: $(call rle_sprite_files,vehicles/polar/player vehicles/polar/yeti)
$(C_BUILDDIR)/game/vehicles/jetpack/data/jetpack_rle_sprites.o: $(call rle_sprite_files,vehicles/jetpack/player)
$(C_BUILDDIR)/game/vehicles/polar/data/yeti_frames.o: $(call rle_sprite_files,vehicles/polar/yeti)
$(C_BUILDDIR)/game/vehicles/polar/data/polar_anim_family.o: $(call rle_sprite_files,vehicles/polar/player) $(GRAPHICS_BUILDDIR)/vehicles/polar/sprite_sheet_frames.h
$(C_BUILDDIR)/game/vehicles/jetpack/data/jetpack_anim_family.o: $(call rle_sprite_files,vehicles/jetpack/player) $(GRAPHICS_BUILDDIR)/vehicles/jetpack/sprite_sheet_frames.h
