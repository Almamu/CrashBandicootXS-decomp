# `make` builds the ROM from whatever is in the tree; `make compare` also
# checks it against games/xs/checksum.sha1 (the original ROM's).
.DEFAULT_GOAL := all

#### Tools ####

PREFIX	 := arm-none-eabi-
CC1      := tools/agbcc/bin/agbcc
CPP      := $(PREFIX)cpp
AS       := $(PREFIX)as
LD       := $(PREFIX)ld
OBJCOPY  := $(PREFIX)objcopy

GFX := tools/gbagfx/gbagfx
GRIT := tools/grit/grit

# Warnings (#577). Every C and C++ object, whichever compiler builds it
# (agbcc, agbcp, old_agbcp, agbcp_arm_patched), gets this set, and -Werror makes any warning
# fail the build, locally and in CI. A fix must keep the ROM matching;
# see "Compiler warnings" in CONTRIBUTING.md for how to handle a warning
# byte-neutrally and the per-site escape hatches.
WARNFLAGS := -Wall -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wnested-externs -Wredundant-decls -Werror
CC1FLAGS := -mthumb-interwork $(WARNFLAGS) -O2 -fhex-asm  -fprologue-bugfix
# The platform (#1093, docs/platforms.md): the GBA's hardware headers
# (platform/gba/include/gba/*.h, included as "gba/io_reg.h" and so on),
# its startup and interrupt code and its BIOS wrappers. A port builds
# with its own platform/<name>/ in its place: `make PLATFORM=pc` builds
# the game for the host (platform/pc/platform.mk). Both games build for
# the GBA.
PLATFORM ?= gba
PLATFORM_DIR := platform/$(PLATFORM)
# The libraries' public headers (lib/*/include, and the platform's own,
# platform/gba/lib/*/include) are on the -I path, so game code includes
# them as <gax.h>, <agb_eeprom.h>, <agb_syscall.h>.
CPPFLAGS := -I tools/agbcc/include -iquote include -I $(PLATFORM_DIR)/include \
	$(patsubst %,-I %,$(wildcard lib/*/include $(PLATFORM_DIR)/lib/*/include)) -nostdinc -undef
ASFLAGS  := -mcpu=arm7tdmi -mthumb-interwork
# The GBA has no memory protection, so the ELF's RWX LOAD segment is
# expected; binutils >= 2.39 warns about it unless told not to.
LDFLAGS  := $(shell $(LD) --help 2>/dev/null | grep -q -- --no-warn-rwx-segments && echo --no-warn-rwx-segments)

# Header dependency tracking: the preprocess step of every C object also
# writes a make fragment (foo.o -> foo.d) listing the headers it read, and
# the .d files are included at the end of this Makefile, so editing a
# header rebuilds exactly the objects that include it. -MP adds an empty
# rule per header so deleting or renaming one doesn't break the build.
DEPFLAGS = -MMD -MP -MF $(@:.o=.d) -MT $@

#### Custom flags to alter compilation output ####
ifeq ($(NON_MATCHING),1)
	CPPFLAGS += -D NON_MATCHING=1
	ASFLAGS += --defsym NON_MATCHING=1
else
	ASFLAGS += --defsym NON_MATCHING=0
endif

#### Game ####
# GAME picks the ROM (#1108): xs, Crash Bandicoot XS (the default, the rest
# of this Makefile), or n2, Crash Bandicoot 2: N-Tranced (games/n2/game.mk,
# docs/n_tranced.md). Each game builds into its own build/<rom>/ and
# defines GAME_XS or GAME_N2 for the C/C++ and the assembly.
GAME ?= xs

ifeq ($(GAME),n2)
ifneq ($(PLATFORM),gba)
$(error PLATFORM=$(PLATFORM) builds Crash Bandicoot XS only (#1093))
endif
include games/n2/game.mk
else ifneq ($(GAME),xs)
$(error GAME must be xs or n2, not "$(GAME)")
else

CPPFLAGS += -D GAME_XS=1
ASFLAGS += --defsym GAME_XS=1

#### Files ####
ifeq ($(PLATFORM),gba)
OBJ_DIR:= build/crashbandicootxs
else
OBJ_DIR:= build/$(PLATFORM)/crashbandicootxs
endif
ROM      := crashbandicootxs.gba
ELF      := $(ROM:.gba=.elf)
MAP      := $(ROM:.gba=.map)
LDSCRIPT := games/xs/ldscript.txt

C_SUBDIR = src
ASM_SUBDIR = asm
DATA_ASM_SUBDIR = data

C_BUILDDIR = $(OBJ_DIR)/$(C_SUBDIR)
ASM_BUILDDIR = $(OBJ_DIR)/$(ASM_SUBDIR)
DATA_ASM_BUILDDIR = $(OBJ_DIR)/$(DATA_ASM_SUBDIR)
SOUND_BUILDDIR = $(OBJ_DIR)/sound
GRAPHICS_BUILDDIR = $(OBJ_DIR)/graphics

$(shell mkdir -p $(C_BUILDDIR) $(ASM_BUILDDIR) $(DATA_ASM_BUILDDIR) $(SOUND_BUILDDIR) $(GRAPHICS_BUILDDIR))

C_SRCS := $(shell find $(C_SUBDIR) -name '*.c' | sort)
C_ASMS := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.s,$(C_SRCS))
C_OBJS := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.o,$(C_SRCS))

# The game objects written as C++ (#664, docs/cplusplus.md): every .cpp
# under src/, at any depth (src/engine/<ns>/, src/game/<ns>/,
# src/game/vehicles/polar/, the data/ subdirectories; docs/file_layout.md),
# built by agbcp/old_agbcp (see "C++ objects" below).
CXX_SRCS := $(shell find $(C_SUBDIR) -name '*.cpp' | sort)
CXX_ASMS := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.s,$(CXX_SRCS))
CXX_OBJS := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.o,$(CXX_SRCS))

# The ROM data tables: the .cpp files of the data/ subdirectories, next to
# the code that uses them (src/<area>/<namespace>/data/, docs/file_layout.md).
# Each table's name is unique: $(call data_obj,NAME) is the object of
# NAME.cpp, wherever it is.
DATA_SRCS := $(foreach f,$(CXX_SRCS),$(if $(findstring /data/,$(f)),$(f)))
DATA_OBJS := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.o,$(DATA_SRCS))
data_obj = $(filter %/$(1).o,$(DATA_OBJS))

ASM_SRCS := $(wildcard $(ASM_SUBDIR)/*.s)
ASM_OBJS := $(patsubst $(ASM_SUBDIR)/%.s,$(ASM_BUILDDIR)/%.o,$(ASM_SRCS))

DATA_ASM_SRCS := $(wildcard $(DATA_ASM_SUBDIR)/*.s)
DATA_ASM_OBJS := $(patsubst $(DATA_ASM_SUBDIR)/%.s,$(DATA_ASM_BUILDDIR)/%.o,$(DATA_ASM_SRCS))

#### Libraries ####
# Third-party and SDK code linked into the ROM, kept apart from the game
# under lib/ (see docs/libraries.md). They are linked as plain objects:
# games/xs/ldscript.txt interleaves them with the game code in ROM order.
#  - lib/gax: Shin'en's GAX2 sound engine (src/*.c code, asm/*.s its
#    hand-written ARM DSP routines, data/*.c tables)
#  - lib/agb_eeprom: Nintendo's AgbEeprom SDK library, EEPROM_V122
#  - lib/libgcc: libgcc2.c's 64-bit helpers and lib1funcs.asm's routines
# and the platform's own library, platform/gba/lib/libagbsyscall: the BIOS
# SWI wrappers (see "Platform" below).
LIB_SUBDIR = lib
LIB_BUILDDIR = $(OBJ_DIR)/lib

LIB_C_SRCS := $(wildcard $(LIB_SUBDIR)/*/src/*.c $(LIB_SUBDIR)/*/data/*.c)
LIB_C_ASMS := $(patsubst $(LIB_SUBDIR)/%.c,$(LIB_BUILDDIR)/%.s,$(LIB_C_SRCS))
LIB_C_OBJS := $(patsubst $(LIB_SUBDIR)/%.c,$(LIB_BUILDDIR)/%.o,$(LIB_C_SRCS))

# As in gcc's own libgcc build, libgcc2.c is compiled once per function
# with -DL_<name> and lib1funcs.s assembled once per routine with
# L_<name> defined, one object per function.
LIBGCC2_FUNCS := _divdi3 _udivdi3 _muldi3
LIB1FUNCS := _udivsi3 _divsi3 _dvmd_tls _modsi3 _umodsi3 _call_via_rX
LIBGCC2_OBJS := $(patsubst %,$(LIB_BUILDDIR)/libgcc/%.o,$(LIBGCC2_FUNCS))
LIBGCC2_ASMS := $(LIBGCC2_OBJS:.o=.s)
LIB1FUNCS_OBJS := $(patsubst %,$(LIB_BUILDDIR)/libgcc/%.o,$(LIB1FUNCS))

# A library's hand-written assembly (lib/<name>/asm/*.s: GAX2's ARM DSP code).
LIB_ASM_OBJS := $(patsubst $(LIB_SUBDIR)/%.s,$(LIB_BUILDDIR)/%.o,$(wildcard $(LIB_SUBDIR)/*/asm/*.s))
$(LIB_ASM_OBJS): asm/macros.inc asm/macros/function.inc

LIB_OBJS := $(LIB_C_OBJS) $(LIBGCC2_OBJS) $(LIB1FUNCS_OBJS) $(LIB_ASM_OBJS)

#### Platform ####
# The GBA platform's assembly (docs/platforms.md): crt0 and IntrMain
# (platform/gba/asm/) and the BIOS SWI wrappers
# (platform/gba/lib/libagbsyscall/), all hand-written.
PLATFORM_BUILDDIR := $(OBJ_DIR)/$(PLATFORM_DIR)
PLATFORM_ASM_SRCS := $(wildcard $(PLATFORM_DIR)/asm/*.s $(PLATFORM_DIR)/lib/*/*.s)
PLATFORM_ASM_OBJS := $(patsubst $(PLATFORM_DIR)/%.s,$(PLATFORM_BUILDDIR)/%.o,$(PLATFORM_ASM_SRCS))
$(PLATFORM_ASM_OBJS): asm/macros.inc asm/macros/function.inc
$(PLATFORM_BUILDDIR)/asm/crt0.o: $(PLATFORM_DIR)/asm/rom_header.inc

OBJS := $(C_OBJS) $(CXX_OBJS) $(LIB_OBJS) $(PLATFORM_ASM_OBJS) $(ASM_OBJS) $(DATA_ASM_OBJS)

# The .d files the C objects' preprocess step writes (see DEPFLAGS).
DEPS := $(C_OBJS:.o=.d) $(CXX_OBJS:.o=.d) $(LIB_C_OBJS:.o=.d) $(LIBGCC2_OBJS:.o=.d)

# The headers generated from the data (graphics.mk's graphics.hpp, levels.mk's
# constants, the sound constants below), on every object's include path.
GENERATED_INCLUDE_DIR := build/include
CPPFLAGS += -iquote $(GENERATED_INCLUDE_DIR)

include graphics.mk
include levels.mk
include text.mk
include cutscenes.mk

# A new song's settings are its sound/songs/<key>.json (docs/audio.md,
# "Adding a song").
SOUND_SONGS := $(wildcard sound/songs/*.xm) $(wildcard sound/songs/*.json)
SOUND_SAMPLES := $(wildcard sound/samples/*.wav)
# New instruments (docs/audio.md, "New instruments"): sound/instruments/
# <name>.json, each with a WAV under sound/, for the songs and for the
# sound effects sfx_table.json names.
SOUND_INSTRUMENTS := $(wildcard sound/instruments/*.json) $(SOUND_SAMPLES) $(wildcard sound/sfx_samples/*.wav)
SOUND_SFX_SOURCES := sound/gax_sfx_manifest.json sound/sfx_table.json $(SOUND_INSTRUMENTS)
SOUND_BUILT := $(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_audio_data.s \
	$(SOUND_BUILDDIR)/gax_default_layout.bin $(SOUND_BUILDDIR)/gax_default_layout.s \
	$(SOUND_BUILDDIR)/gax_sfx_data.bin $(SOUND_BUILDDIR)/gax_sfx_data.s $(SOUND_BUILDDIR)/sfx_table.bin

#### Main Targets ####

.PHONY: all compare
ifeq ($(PLATFORM),gba)
all: $(ROM)

compare: $(ROM)
	sha1sum -c games/xs/checksum.sha1
endif

# The ROM shiftability check (#967, docs/shifting.md): links the ROM again
# with padding in front of several objects, and fails if anything that
# should move with its target isn't a relocation. No baserom.gba needed.
.PHONY: shiftcheck
shiftcheck: $(OBJS) $(LDSCRIPT)
	python3 tools/shiftcheck.py --ld '$(LD) $(LDFLAGS)' --ldscript $(LDSCRIPT) --out $(OBJ_DIR)/shiftcheck $(SHIFTCHECK_FLAGS) \
		-- $(OBJS) tools/agbcc/lib/libgcc.a tools/agbcc/lib/libc.a

# A mod (docs/modding.md): `make mod MOD=mods/sample` applies the mod to a
# copy of the tree, MOD_TREE (tools/apply_mod.py), and builds it there with
# a plain make: the ROM is $(MOD_TREE)/crashbandicootxs.gba. This tree's
# data, and so `make compare`, stays as it is. No baserom.gba needed.
MOD ?= mods/sample
MOD_TREE ?= build/mods/$(notdir $(MOD))
.PHONY: mod
mod:
	rm -rf $(MOD_TREE)
	python3 tools/apply_mod.py $(MOD) $(MOD_TREE)
	$(MAKE) -C $(MOD_TREE)

# Every incbin and include in data/*.s (the sound blocks) needs the
# corresponding built file to exist first. Each GAX2 block is a .bin and
# the .s that data.s includes: its bytes, with the pointers as
# relocations. The graphics and the level assets are C arrays in
# the data/ directories (graphics.mk, levels.mk).
$(DATA_ASM_OBJS): $(SOUND_BUILT)

# The music block starts with pointers into the sound-effect set (to its
# handler type, at an offset the set's layout gives), so it depends on
# the set's sources too. The same run writes the default song's layout
# struct (gGaxDefaultSong) and the song offsets the song table
# (src/engine/audio/data/song_table.cpp) is written with.
$(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_audio_data.s $(SOUND_BUILDDIR)/gax_default_layout.bin \
		$(SOUND_BUILDDIR)/gax_default_layout.s $(SOUND_BUILDDIR)/gax_songs.h &: sound/gax_manifest.json $(SOUND_SONGS) $(SOUND_INSTRUMENTS) $(SOUND_SFX_SOURCES) tools/gax_audio.py
	python3 tools/gax_audio.py $(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_default_layout.bin $(SOUND_BUILDDIR)/gax_songs.h

# gSongTable itself, in song id order (the manifest's song_table), which
# song_table.cpp #includes.
$(SOUND_BUILDDIR)/song_table.inc: sound/gax_manifest.json tools/gax_audio.py
	python3 tools/gax_audio.py --table $@

$(C_BUILDDIR)/engine/audio/data/song_table.o: CPPFLAGS += -iquote $(SOUND_BUILDDIR)
$(C_BUILDDIR)/engine/audio/data/song_table.o: $(SOUND_BUILDDIR)/gax_songs.h $(SOUND_BUILDDIR)/song_table.inc

$(SOUND_BUILDDIR)/gax_sfx_data.bin $(SOUND_BUILDDIR)/gax_sfx_data.s &: $(SOUND_SFX_SOURCES) tools/gax_audio.py
	python3 tools/gax_audio.py --sfx $(SOUND_BUILDDIR)/gax_sfx_data.bin

# An entry naming a new instrument gets its number in the set.
$(SOUND_BUILDDIR)/sfx_table.bin: $(SOUND_SFX_SOURCES) tools/sfx_table.py tools/gax_audio.py
	python3 tools/sfx_table.py $@

# The song and sound-effect IDs (#655), generated like levels.mk's
# constants headers: the manifest's song_table (gSongTable's order) and
# the names in sfx_table.json. Only the manifest is read, so these don't
# wait for the music block.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/constants/songs.h $(GENERATED_INCLUDE_DIR)/constants/sfx.h

$(GENERATED_INCLUDE_DIR)/constants/songs.h: sound/gax_manifest.json sound/music.json tools/gax_audio.py
	@mkdir -p $(dir $@)
	python3 tools/gax_audio.py --constants $@

$(GENERATED_INCLUDE_DIR)/constants/sfx.h: sound/sfx_table.json tools/sfx_table.py
	@mkdir -p $(dir $@)
	python3 tools/sfx_table.py --constants $@

# Every C object waits for all the generated constants headers (order-only,
# so a regenerated header rebuilds only the objects whose .d lists it).
$(C_OBJS) $(CXX_OBJS) $(LIB_C_OBJS) $(LIBGCC2_OBJS): | $(GENERATED_HEADERS)

clean:
	$(RM) $(ROM) $(ELF) $(MAP) $(OBJS) $(C_ASMS) $(CXX_ASMS) $(LIB_C_ASMS) $(LIBGCC2_ASMS) $(DEPS) $(GENERATED_HEADERS)

tidy:
	rm -f $(ROM) $(ELF) $(MAP)
	rm -r build/*

#### Screen-size compile check (#979) ####
# `make screen-check` compiles every game object (src/, and of the
# data tables those that spell a DISPLAY_ name or a logo fly-in's
# _FLY_IN: the layouts' position tables, the logo pieces' motions and
# their seeds) with another screen size, SCREEN_CHECK_WIDTH x
# SCREEN_CHECK_HEIGHT (426x240 by default, a widescreen size), into
# build/screen-check. Nothing is linked or compared: it only has to
# compile, with the same warnings as errors, so a screen constant stored
# in a type or a bitfield too narrow for the bigger size (agbcc's "large
# integer implicitly truncated") or a compile-time size that stops
# holding fails it. A literal written where the defines are meant still
# compiles: tools/magic_numbers.py --screen --check catches those. See
# docs/screen.md.
SCREEN_CHECK_WIDTH ?= 426
SCREEN_CHECK_HEIGHT ?= 240

.PHONY: screen-check screen-check-objs
screen-check:
	$(MAKE) OBJ_DIR=build/screen-check SCREEN_CHECK=1 screen-check-objs

ifeq ($(SCREEN_CHECK),1)
CPPFLAGS += -D DISPLAY_WIDTH=$(SCREEN_CHECK_WIDTH) -D DISPLAY_HEIGHT=$(SCREEN_CHECK_HEIGHT)
endif

SCREEN_CHECK_DATA_OBJS := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.o,$(shell grep -lE 'DISPLAY_|_FLY_IN' $(DATA_SRCS)))

screen-check-objs: $(filter-out $(DATA_OBJS),$(C_OBJS) $(CXX_OBJS)) $(SCREEN_CHECK_DATA_OBJS)

#### Progress report (objdiff) ####
# See docs/progress_report.md. `report` builds one objdiff unit per source
# file (rather than one merged blob) so the report shows
# per-category (Graphics/Util/System) progress, not just an overall
# total - tools/report_units.py has the full explanation of why it has
# to be this fine-grained, the per-file address table, and how it slices
# games/xs/expected/legacy.s/expected/code_3.s (frozen, never rebuilt from
# anything else) and applies games/xs/expected/corrections.txt per slice. Run
# under `NON_MATCHING=1` so parked functions are included as their
# (possibly imperfect) C reconstruction rather than omitted or silently
# swapped for raw asm. The data units (data/data.s) need the repo-built
# sound blocks it incbins, but not data.o itself (that needs baserom.gba).
# itoa_arm's unit is an assembly object (asm/itoa_arm.s, #662).

.PHONY: report
report: $(C_OBJS) $(CXX_OBJS) $(LIB_C_OBJS) $(LIBGCC2_OBJS) $(ASM_BUILDDIR)/itoa_arm.o $(SOUND_BUILT)
	python3 tools/report_units.py

#### Recipes ####
	
ifeq ($(PLATFORM),gba)
$(ELF): $(OBJS) $(LDSCRIPT)
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) -Map $(MAP) $(OBJS) tools/agbcc/lib/libgcc.a tools/agbcc/lib/libc.a -o $@

%.gba: %.elf
	$(OBJCOPY) -O binary $< $@

include compile.mk

$(PLATFORM_BUILDDIR)/%.o: $(PLATFORM_DIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<
else
# Another platform's build (docs/platforms.md): its `all`, its compile
# rules and its link.
include $(PLATFORM_DIR)/platform.mk
endif

$(GFX):
	$(MAKE) -C tools/gbagfx

$(GRIT):
	$(MAKE) -C tools/grit

-include $(DEPS)

endif # GAME == xs
