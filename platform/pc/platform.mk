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
# an SDL2 window with the keyboard and gamepads, and GAX2's sound
# (docs/platforms.md).

HOST_CC      ?= cc
HOST_CXX     ?= c++
HOST_NM      ?= nm
HOST_CXXFILT ?= c++filt
HOST_OBJCOPY ?= objcopy

HOST_EXE := build/$(PLATFORM)/crashbandicootxs_$(PLATFORM)

# The window (src/sdl.c) needs SDL2, found by pkg-config; without it (or
# with HOST_SDL=0) the executable runs headless only, which is all the
# tests need. Not finding it is said (a warning), since the executable
# then has no window; HOST_SDL=0 on the command line is taken as meant.
HOST_PKG_CONFIG ?= pkg-config
HOST_SDL ?= $(shell $(HOST_PKG_CONFIG) --exists sdl2 2>/dev/null && echo 1 || echo 0)
ifeq ($(HOST_SDL),1)
HOST_SDL_CFLAGS := -DHOST_SDL $(shell $(HOST_PKG_CONFIG) --cflags sdl2)
HOST_SDL_LIBS := $(shell $(HOST_PKG_CONFIG) --libs sdl2)
else ifneq ($(origin HOST_SDL),command line)
# Said once, not again when make restarts for the generated makefiles.
ifeq ($(MAKE_RESTARTS),)
$(warning $(HOST_PKG_CONFIG) finds no sdl2: building $(HOST_EXE) without a window (headless only))
endif
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

# The sanitizers: `make PLATFORM=pc SANITIZE=undefined` (or address, or
# both: SANITIZE=address,undefined; docs/platforms.md, "Sanitizers").
# -fsanitize goes on every compile and on the link, which brings in the
# sanitizers' runtimes (libubsan, libasan): without it the objects'
# __ubsan_handle_* and __asan_* calls are undefined. SANITIZE_FLAGS adds
# to them (-fno-sanitize=..., -fsanitize-recover=...). The objects are
# built again when either changes (the flags file, below).
SANITIZE ?=
SANITIZE_FLAGS ?=
ifneq ($(SANITIZE),)
HOST_SANITIZE_FLAGS := -fsanitize=$(SANITIZE) -fno-omit-frame-pointer $(SANITIZE_FLAGS)
HOST_CXXFLAGS += $(HOST_SANITIZE_FLAGS)
HOST_CFLAGS += $(HOST_SANITIZE_FLAGS)
endif

# ASan puts a redzone after each global, but two data files are read
# across their definitions, back to back as in the ROM (and as
# -fno-toplevel-reorder keeps them): a cutscene picture's bitmap right
# after its palette (+0x200, slideshow.cpp), and the sprite tile pool, 56
# arrays read from bank 0's (tileBase + a frame's offset). With redzones
# those reads are overflows, and the data isn't where it is read. Their
# globals aren't instrumented (--param asan-globals=0: no redzones, so
# nothing between them); every other global is.
ifneq ($(findstring address,$(SANITIZE)),)
$(C_BUILDDIR)/game/ui/data/cutscene_pictures.o $(C_BUILDDIR)/engine/gfx/data/sprite_tiles.o: HOST_CXXFLAGS += --param asan-globals=0
endif

# entity.o is the one key-method object where g++ 2.9 (`#pragma
# implementation`) emitted out-of-line copies of its class's inline
# methods that the ROM uses (compile.mk, NO_IMPLEMENT_INLINES_OBJS):
# other files call SetEntityPos, SetEntityPixelPos (entity.hpp) and the
# constructor out of line. A modern g++ emits only the copies a file
# uses itself, so entity.o keeps them all (g++ only: clang has no such
# option, and with it the link misses those methods).
$(C_BUILDDIR)/game/entities/entity.o: HOST_CXXFLAGS += -fkeep-inline-functions

# Units: a file platform/pc/units/<path>.cpp is built in place of
# engine/src/<path>.cpp, into the same object (so the shared dependencies and
# flags of that object still apply). For a unit the host can't compile
# as it is: bios_util.cpp, whose DivMod is an inline ARM `svc`.
PLATFORM_UNITS := $(shell find $(PLATFORM_DIR)/units -name '*.cpp')
PLATFORM_UNIT_OBJS := $(patsubst $(PLATFORM_DIR)/units/%.cpp,$(C_BUILDDIR)/%.o,$(PLATFORM_UNITS))

# The platform library (platform/pc/src/): the host memory, DMA, the BIOS
# calls, startup and IRQ dispatch, the audio output and GAX2's ARM
# routines in C, AgbEeprom's API (which replaces lib/agb_eeprom),
# lib/libgcc's division helpers the game and GAX2 call by name (the rest
# of libgcc is the host compiler's), and the sound blocks of games/xs/data/data.s.
PLATFORM_SRCS := $(wildcard $(PLATFORM_DIR)/src/*.c $(PLATFORM_DIR)/src/*.S)
PLATFORM_OBJS := $(patsubst $(PLATFORM_DIR)/src/%,$(PLATFORM_BUILDDIR)/src/%.o,$(basename $(PLATFORM_SRCS)))

# GAX2 (lib/gax, docs/audio.md): its matched C, built for the host, with
# a unit of platform/pc/units/lib/gax/ in place of each file that can't
# run there as it is (the ARM routines' calls and the resampler's patches,
# the work RAM carved by the GBA's sizes, the DMA and SWI asm; gax_host.h).
# The song data's structures with pointers are generated again as C
# (tools/gax_host_data.py, from tools/gax_audio.py's .s files), laid out
# by the host compiler; the rest is read in place from the built blocks.
HOST_GAX_SRCS := $(wildcard lib/gax/src/*.c lib/gax/data/*.c)
HOST_GAX_OBJS := $(patsubst lib/%.c,$(LIB_BUILDDIR)/%.o,$(HOST_GAX_SRCS))
HOST_GAX_UNITS := $(shell find $(PLATFORM_DIR)/units/lib -name '*.c')
HOST_GAX_UNIT_OBJS := $(patsubst $(PLATFORM_DIR)/units/lib/%.c,$(LIB_BUILDDIR)/%.o,$(HOST_GAX_UNITS))
HOST_GAX_DATA := $(PLATFORM_BUILDDIR)/gax_host_data.c
HOST_GAX_FLAGS = -iquote lib/gax/src -iquote $(PLATFORM_DIR)/src

# The inline methods of the headers with `#pragma interface` (many of
# engine/include's classes, docs/cplusplus.md "Emitting the vtables"):
# g++ emits no out-of-line copy of them in the files that include such a
# header. On the GBA no call to one is left out of line, and a modern g++
# inlines them all at -O1 too, but not always with -fsanitize, whose
# checks make the methods bigger (with UBSan: LinkRing::Push and Pop,
# CrateList::Append, Detach and ResetGrid), and then nothing defines
# them. So a generated file, inlines.cpp, includes each such header after
# a `#pragma implementation` of its name, which has g++ emit the copies
# there (weak, as any inline function's), called or not.
HOST_INTERFACE_HEADERS := $(shell grep -rl '^.pragma interface' engine/include | sort)
HOST_INLINES := $(PLATFORM_BUILDDIR)/inlines.cpp

$(HOST_INLINES): $(HOST_INTERFACE_HEADERS) $(PLATFORM_DIR)/platform.mk
	@mkdir -p $(dir $@)
	@{ echo '// Generated by platform/pc/platform.mk: the headers with #pragma interface.'; \
	  $(foreach h,$(HOST_INTERFACE_HEADERS),echo '#pragma implementation "$(notdir $(h))"';) \
	  $(foreach h,$(HOST_INTERFACE_HEADERS),echo '#include "$(patsubst engine/include/%,%,$(h))"';) } > $@

$(HOST_INLINES:.cpp=.o): $(HOST_INLINES) $(HOST_RENAMES) | $(GENERATED_HEADERS)
	$(HOST_CXX) $(HOST_CPPFLAGS) $(HOST_CXXFLAGS) $(DEPFLAGS) -c -o $@ $<
	$(HOST_OBJCOPY) --redefine-syms=$(HOST_RENAMES) $@

HOST_OBJS := $(C_OBJS) $(CXX_OBJS) $(PLATFORM_OBJS) $(HOST_GAX_OBJS) $(HOST_GAX_DATA:.c=.o) $(HOST_INLINES:.cpp=.o)

$(PLATFORM_OBJS) $(HOST_GAX_OBJS): | $(GENERATED_HEADERS)

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
# --cutscene (src/cutscene_pick.c) wraps the boot's cutscene.
HOST_WRAPS += _ZN4Game5Level10LevelState16PlayBootCutsceneEv
# --gax-log (src/gax_trace.c) wraps GAX2's API.
HOST_WRAPS += GAX2_init GAX_fx_ex GAX_fx GAX_fx_note GAX_stop_fx GAX_set_music_volume GAX_set_fx_volume \
	GAX_stop GAX_pause GAX_resume GAX_play GAX_irq GAX2_jingle

$(HOST_EXE): $(HOST_OBJS) $(HOST_LINK)
	@mkdir -p $(dir $@)
	$(HOST_CXX) $(HOST_SANITIZE_FLAGS) -o $@ $(HOST_OBJS) $(HOST_LINK) $(addprefix -Wl$(comma)--wrap=,$(HOST_WRAPS)) $(HOST_SDL_LIBS) -lm

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

$(HOST_GAX_UNIT_OBJS): $(LIB_BUILDDIR)/%.o: $(PLATFORM_DIR)/units/lib/%.c
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CPPFLAGS) $(HOST_GAX_FLAGS) $(HOST_CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(LIB_BUILDDIR)/gax/%.o: lib/gax/%.c
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CPPFLAGS) $(HOST_GAX_FLAGS) $(HOST_CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(HOST_GAX_DATA): $(SOUND_BUILT) $(SOUND_BUILDDIR)/gax_songs.h $(PLATFORM_DIR)/tools/gax_host_data.py
	@mkdir -p $(dir $@)
	python3 $(PLATFORM_DIR)/tools/gax_host_data.py --music $(SOUND_BUILDDIR)/gax_audio_data.s \
		--sfx $(SOUND_BUILDDIR)/gax_sfx_data.s --default $(SOUND_BUILDDIR)/gax_default_layout.s \
		--songs $(SOUND_BUILDDIR)/gax_songs.h -o $@

$(HOST_GAX_DATA:.c=.o): $(HOST_GAX_DATA) | $(GENERATED_HEADERS)
	$(HOST_CC) $(HOST_CPPFLAGS) $(HOST_GAX_FLAGS) $(HOST_CFLAGS) $(DEPFLAGS) -c -o $@ $<

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

# Every object and the executable are rebuilt when SANITIZE or
# SANITIZE_FLAGS changes.
$(HOST_OBJS) $(HOST_EXE): $(PLATFORM_BUILDDIR)/sanitize_flags
$(PLATFORM_BUILDDIR)/sanitize_flags: FORCE
	@mkdir -p $(dir $@)
	@echo '$(HOST_SANITIZE_FLAGS)' | cmp -s - $@ || echo '$(HOST_SANITIZE_FLAGS)' > $@

.PHONY: FORCE
FORCE:

$(PLATFORM_BUILDDIR)/src/sound_data.o: $(SOUND_BUILT)
$(PLATFORM_BUILDDIR)/src/sound_data.o: HOST_CPPFLAGS += -D 'SFX_TABLE_BIN="$(SOUND_BUILDDIR)/sfx_table.bin"' \
	-D 'GAX_MUSIC_BIN="$(SOUND_BUILDDIR)/gax_audio_data.bin"' -D 'GAX_SFX_BIN="$(SOUND_BUILDDIR)/gax_sfx_data.bin"'

# The platform's own GAX2 objects find lib/gax's internal header and
# gax_host.h.
$(PLATFORM_BUILDDIR)/src/gax_dsp.o $(PLATFORM_BUILDDIR)/src/audio.o $(PLATFORM_BUILDDIR)/src/gax_trace.o: \
	HOST_CFLAGS += $(HOST_GAX_FLAGS)

-include $(PLATFORM_OBJS:.o=.d) $(HOST_GAX_OBJS:.o=.d) $(HOST_GAX_DATA:.c=.d) $(HOST_INLINES:.cpp=.d)
