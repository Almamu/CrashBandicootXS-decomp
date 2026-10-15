# The pc platform (#1093, docs/platforms.md): Crash Bandicoot XS built
# for the host, a 64-bit Linux with a stock g++/gcc, by
# `make PLATFORM=pc`. The Makefile includes this file instead of the GBA's
# link and compile rules (compile.mk); everything before that is shared:
# the object lists, the generated data and headers (graphics.mk,
# levels.mk, text.mk, cutscenes.mk, the sound) and their dependencies,
# all under build/pc/crashbandicootxs/.
#
# The game compiles and links into a host executable, with the platform's
# functions in plain C (platform/pc/src/): the frame loop, a software PPU,
# and an SDL2 window with the keyboard and gamepads (docs/platforms.md).

HOST_CC      ?= cc
HOST_CXX     ?= c++
HOST_NM      ?= nm
HOST_CXXFILT ?= c++filt
HOST_OBJCOPY ?= objcopy

HOST_EXE := build/$(PLATFORM)/crashbandicootxs_$(PLATFORM)

# The window (src/sdl.c) needs SDL2, found by pkg-config; without it (or
# with HOST_SDL=0) the executable runs headless only, which is all the
# tests need.
HOST_PKG_CONFIG ?= pkg-config
HOST_SDL ?= $(shell $(HOST_PKG_CONFIG) --exists sdl2 2>/dev/null && echo 1 || echo 0)
ifeq ($(HOST_SDL),1)
HOST_SDL_CFLAGS := -DHOST_SDL $(shell $(HOST_PKG_CONFIG) --cflags sdl2)
HOST_SDL_LIBS := $(shell $(HOST_PKG_CONFIG) --libs sdl2)
endif

.PHONY: all
all: $(HOST_EXE)

# The flags. The include path and defines are the GBA build's (CPPFLAGS,
# with this platform's include directory in the GBA's place), without
# agbcc's own headers: the host's C library is used. The game is g++ 2.9
# C++, built here as GNU C++98 with the GBA's no-RTTI/no-exceptions:
# - -funsigned-char: `char` is unsigned on the ARM, signed on x86.
# - -fno-strict-aliasing, -fwrapv: the code was written for (and matched
#   against) gcc 2.9, which assumed neither.
# - -fno-builtin: the game has its own strcpy, strlen, strcat, sprintf
#   (string.cpp, printf.cpp), with u8 * signatures.
# - -fno-toplevel-reorder: the data in a file's order, as agbcc emits it
#   and as the game reads some of it: a cutscene picture's bitmap right
#   after its palette (slideshow.cpp reads it at +0x200), which are two
#   definitions (cutscene_pictures.cpp). A modern g++ otherwise emits them
#   in reverse order. (Both are aligned(4), so the host's alignment of a
#   large array to 16 or 32 bytes leaves no gap either.)
# No -Werror: the host compiler's warnings aren't the build's (#577's
# warnings are checked on the GBA build).
HOST_CPPFLAGS = $(subst -I tools/agbcc/include,,$(filter-out -nostdinc -undef,$(CPPFLAGS)))
HOST_FLAGS := -g -femit-class-debug-always -O1 -funsigned-char -fno-strict-aliasing -fwrapv -fno-builtin -fno-toplevel-reorder
HOST_CXXFLAGS := $(HOST_FLAGS) -std=gnu++98 -fno-rtti -fno-exceptions
HOST_CFLAGS := $(HOST_FLAGS) -std=gnu99

# entity.o is the one key-method object where g++ 2.9 (`#pragma
# implementation`) emitted out-of-line copies of its class's inline
# methods that the ROM uses (compile.mk, NO_IMPLEMENT_INLINES_OBJS):
# other files call SetEntityPos, SetEntityPixelPos (entity.hpp) and the
# constructor out of line. A modern g++ emits only the copies a file
# uses itself, so entity.o keeps them all (g++ only: clang has no such
# option, and with it the link misses those methods).
$(C_BUILDDIR)/game/entities/entity.o: HOST_CXXFLAGS += -fkeep-inline-functions

# Units: a file platform/pc/units/<path>.cpp is built in place of
# src/<path>.cpp, into the same object (so the shared dependencies and
# flags of that object still apply). For a unit the host can't compile
# as it is: bios_util.cpp, whose DivMod is an inline ARM `svc`.
PLATFORM_UNITS := $(shell find $(PLATFORM_DIR)/units -name '*.cpp')
PLATFORM_UNIT_OBJS := $(patsubst $(PLATFORM_DIR)/units/%.cpp,$(C_BUILDDIR)/%.o,$(PLATFORM_UNITS))

# The platform library (platform/pc/src/): the host memory, DMA, the BIOS
# calls, startup and IRQ dispatch, GAX2's and AgbEeprom's APIs (which
# replace lib/gax and lib/agb_eeprom), lib/libgcc's 32-bit division
# helpers the game calls by name (the rest of libgcc is the host
# compiler's), and the sound blocks of data/data.s the game reads.
PLATFORM_SRCS := $(wildcard $(PLATFORM_DIR)/src/*.c $(PLATFORM_DIR)/src/*.S)
PLATFORM_OBJS := $(patsubst $(PLATFORM_DIR)/src/%,$(PLATFORM_BUILDDIR)/src/%.o,$(basename $(PLATFORM_SRCS)))

HOST_OBJS := $(C_OBJS) $(CXX_OBJS) $(PLATFORM_OBJS)

$(PLATFORM_OBJS): | $(GENERATED_HEADERS)

# The link. host_link.py writes a linker script of symbol assignments,
# the host's version of what ldscript.txt and the objcopy renames do on
# the GBA: the linker-placed IWRAM and EWRAM globals (sym_iwram.txt,
# sym_ewram.txt, which have no C definitions) inside the host's work
# RAMs, and the C names the code uses for C++ symbols (cxx_symbols.txt,
# the asm("CName") labels) bound to the host compiler's mangled names.
CXX_SYMBOLS := cxx_symbols.txt
SYM_IWRAM := $(dir $(LDSCRIPT))sym_iwram.txt
SYM_EWRAM := $(dir $(LDSCRIPT))sym_ewram.txt
HOST_LINK := $(OBJ_DIR)/host_link.ld

#
# The game's own strcpy, strlen, rand, sprintf, ... and its global
# operator new and delete (libc_names.txt) are renamed in its objects as
# they are built, so that they don't stand in for the C library's and
# libstdc++'s in the executable; host_link.py checks that no other name
# clashes with the host's libraries (HOST_LIBS).
HOST_RENAMES := $(PLATFORM_DIR)/libc_names.txt
HOST_LIBS := $(wildcard $(shell $(HOST_CC) -print-file-name=libc.so.6) $(shell $(HOST_CC) -print-file-name=libm.so.6) \
	$(shell $(HOST_CXX) -print-file-name=libstdc++.so.6))

$(HOST_LINK): $(HOST_OBJS) $(CXX_SYMBOLS) $(SYM_IWRAM) $(SYM_EWRAM) $(HOST_RENAMES) $(PLATFORM_DIR)/tools/host_link.py
	python3 $(PLATFORM_DIR)/tools/host_link.py --nm '$(HOST_NM)' --cxxfilt '$(HOST_CXXFILT)' \
		--cxx-symbols $(CXX_SYMBOLS) --iwram $(SYM_IWRAM) --ewram $(SYM_EWRAM) --libs $(HOST_LIBS) \
		--renames $(HOST_RENAMES) -o $@ $(HOST_OBJS)

# --level and --room (src/level_pick.c) wrap the two functions that pick
# the level and its room.
comma := ,
HOST_WRAPS := _ZN4Game2Ui11LevelSelect3RunEPi _ZN4Game5Level13LevelProgress10SelectRoomEv

$(HOST_EXE): $(HOST_OBJS) $(HOST_LINK)
	@mkdir -p $(dir $@)
	$(HOST_CXX) -o $@ $(HOST_OBJS) $(HOST_LINK) $(addprefix -Wl$(comma)--wrap=,$(HOST_WRAPS)) $(HOST_SDL_LIBS) -lm

$(PLATFORM_UNIT_OBJS): $(C_BUILDDIR)/%.o: $(PLATFORM_DIR)/units/%.cpp $(HOST_RENAMES)
	@mkdir -p $(dir $@)
	$(HOST_CXX) $(HOST_CPPFLAGS) $(HOST_CXXFLAGS) $(DEPFLAGS) -c -o $@ $<
	$(HOST_OBJCOPY) --redefine-syms=$(HOST_RENAMES) $@

$(C_BUILDDIR)/%.o: $(C_SUBDIR)/%.cpp $(HOST_RENAMES)
	@mkdir -p $(dir $@)
	$(HOST_CXX) $(HOST_CPPFLAGS) $(HOST_CXXFLAGS) $(DEPFLAGS) -c -o $@ $<
	$(HOST_OBJCOPY) --redefine-syms=$(HOST_RENAMES) $@

$(C_BUILDDIR)/%.o: $(C_SUBDIR)/%.c $(HOST_RENAMES)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CPPFLAGS) $(HOST_CFLAGS) $(DEPFLAGS) -c -o $@ $<
	$(HOST_OBJCOPY) --redefine-syms=$(HOST_RENAMES) $@

$(PLATFORM_BUILDDIR)/src/%.o: $(PLATFORM_DIR)/src/%.c
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CPPFLAGS) $(HOST_CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(PLATFORM_BUILDDIR)/src/%.o: $(PLATFORM_DIR)/src/%.S
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CPPFLAGS) $(DEPFLAGS) -c -o $@ $<

# sdl.o is rebuilt when HOST_SDL changes (the flags file).
$(PLATFORM_BUILDDIR)/src/sdl.o: HOST_CFLAGS += $(HOST_SDL_CFLAGS)
$(PLATFORM_BUILDDIR)/src/sdl.o: $(PLATFORM_BUILDDIR)/sdl_flags
$(PLATFORM_BUILDDIR)/sdl_flags: FORCE
	@mkdir -p $(dir $@)
	@echo '$(HOST_SDL_CFLAGS) $(HOST_SDL_LIBS)' | cmp -s - $@ || echo '$(HOST_SDL_CFLAGS) $(HOST_SDL_LIBS)' > $@

.PHONY: FORCE
FORCE:

$(PLATFORM_BUILDDIR)/src/sound_data.o: $(SOUND_BUILT)
$(PLATFORM_BUILDDIR)/src/sound_data.o: HOST_CPPFLAGS += -D 'SFX_TABLE_BIN="$(SOUND_BUILDDIR)/sfx_table.bin"' \
	-D 'GAX_MUSIC_BIN="$(SOUND_BUILDDIR)/gax_audio_data.bin"'

-include $(PLATFORM_OBJS:.o=.d)
