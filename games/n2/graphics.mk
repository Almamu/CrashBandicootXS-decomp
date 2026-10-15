# N-Tranced's graphics (#1115, docs/n_tranced.md "Graphics"), from
# games/n2/graphics/manifest.json with graphics_rules.mk, the rules XS's
# graphics.mk uses: a source is games/n2/graphics/<path>, or XS's
# games/xs/graphics/<path> for a graphic both games have (the continue prompt's).
# Included by games/n2/game.mk.

GRAPHICS_DIR      := $(N2_DIR)/graphics
GRAPHICS_BUILDDIR := $(OBJ_DIR)/graphics

# the graphics class objects are data tables, as in XS's build
DATA_OBJS := $(foreach o,$(CXX_OBJS),$(if $(findstring /data/,$(o)),$(o)))
data_obj   = $(filter %/$(1).o,$(DATA_OBJS))
$(DATA_OBJS): CPPFLAGS += -iquote $(GRAPHICS_BUILDDIR)

# its framed sheets have frames of 3 tiles a side (tools/framed_gfx.py)
FRAMED_TO_BIN_FLAGS := --any-size

include graphics_rules.mk

# graphics.hpp, which the class objects' sources include
$(foreach o,$(GRAPHICS_CLASS_OBJECTS),$(call data_obj,$(o))): $(GRAPHICS_CLASSES_HEADER)

$(GRIT):
	$(MAKE) -C tools/grit

# the sprite tile pool and the OBJ palettes (graphics_rules.mk's rules)
$(C_BUILDDIR)/engine/gfx/data/sprite_tiles.o: $(SPRITE_FRAMES_OUT) $(GRAPHICS_BUILDDIR)/sprites/obj_palettes.bin.inc

# the sprite banks' tables (sprite_banks_*.cpp, tools/sprite_banks.py):
# their piece arrays and frames' tile offsets
$(foreach o,$(filter %/sprite_banks_00_06.o %/sprite_banks_07_25.o %/sprite_banks_26_46.o %/sprite_banks_47_48.o,$(DATA_OBJS)),$(eval $(o): $(SPRITE_FRAMES_OUT)))

# the actor categories' framed sprite sheets (graphics_rules.mk's rules)
$(C_BUILDDIR)/game/vehicles/ocean/data/ocean_sprite_sheet.o: $(GRAPHICS_BUILDDIR)/vehicles/ocean/sprite_sheet.bin.lz.inc
$(C_BUILDDIR)/game/vehicles/space/data/space_sprite_sheet.o: $(GRAPHICS_BUILDDIR)/vehicles/space/sprite_sheet.bin.lz.inc
