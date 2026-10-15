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

# The Python tools (#1167): `python3 tools/<name>.py` runs packages/buildtools'
# <name> command, whose data logic is packages/crashdata's (no install
# needed: tools/_packages.py). A rule that runs one depends on PY_<NAME>, the
# files that were tools/<name>.py: the shim, the command and its crashdata
# module.
CRASHDATA_SRC  := packages/crashdata/src/crashdata
BUILDTOOLS_SRC := packages/buildtools/src/buildtools
PY_BG_MAP        := $(PY_BG_MAP) $(BUILDTOOLS_SRC)/bg_map.py $(CRASHDATA_SRC)/graphics/bg_map.py
PY_BIN2C         := $(PY_BIN2C) $(BUILDTOOLS_SRC)/bin2c.py
PY_BOSS_PICTURES := $(PY_BOSS_PICTURES) $(BUILDTOOLS_SRC)/boss_pictures.py $(CRASHDATA_SRC)/graphics/boss_pictures.py
PY_COLLISION_BMP := $(PY_COLLISION_BMP) $(BUILDTOOLS_SRC)/collision_bmp.py $(CRASHDATA_SRC)/levels/collision.py
PY_CUTSCENES     := $(PY_CUTSCENES) $(BUILDTOOLS_SRC)/cutscenes.py $(CRASHDATA_SRC)/cutscenes/__init__.py
PY_FRAMED_GFX    := $(PY_FRAMED_GFX) $(BUILDTOOLS_SRC)/framed_gfx.py $(CRASHDATA_SRC)/graphics/framed_gfx.py
PY_GAX_AUDIO     := $(PY_GAX_AUDIO) $(BUILDTOOLS_SRC)/gax_audio.py $(CRASHDATA_SRC)/audio/gax.py
PY_GFX_MANIFEST  := $(PY_GFX_MANIFEST) $(BUILDTOOLS_SRC)/gfx_manifest.py $(CRASHDATA_SRC)/graphics/manifest.py
PY_GRIT_BG       := $(PY_GRIT_BG) $(BUILDTOOLS_SRC)/grit_bg.py $(CRASHDATA_SRC)/graphics/grit_bg.py
PY_JASC_PAL      := $(PY_JASC_PAL) $(BUILDTOOLS_SRC)/jasc_pal.py $(CRASHDATA_SRC)/common/jasc_pal.py
PY_LEVELS        := $(PY_LEVELS) $(BUILDTOOLS_SRC)/levels.py $(CRASHDATA_SRC)/levels/__init__.py
PY_OBJ_SHEET     := $(PY_OBJ_SHEET) $(BUILDTOOLS_SRC)/obj_sheet.py $(CRASHDATA_SRC)/graphics/obj_sheet.py
PY_RLE_SPRITES   := $(PY_RLE_SPRITES) $(BUILDTOOLS_SRC)/rle_sprites.py $(CRASHDATA_SRC)/graphics/rle_sprites.py
PY_SFX_TABLE     := $(PY_SFX_TABLE) $(BUILDTOOLS_SRC)/sfx_table.py $(CRASHDATA_SRC)/audio/sfx_table.py
PY_SPRITE_FRAMES := $(PY_SPRITE_FRAMES) $(BUILDTOOLS_SRC)/sprite_frames.py $(CRASHDATA_SRC)/graphics/sprite_frames.py
PY_TEXT          := $(PY_TEXT) $(BUILDTOOLS_SRC)/text.py $(CRASHDATA_SRC)/text/__init__.py

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
CPPFLAGS := -I tools/agbcc/include -iquote engine/include -I $(PLATFORM_DIR)/include \
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

# libgcc's functions, an object each (see "Files"); N-Tranced's build
# links the ones its objects.txt names
LIBGCC2_FUNCS := _divdi3 _udivdi3 _muldi3
LIB1FUNCS := _udivsi3 _divsi3 _dvmd_tls _modsi3 _umodsi3 _call_via_rX

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
XS_DIR   := games/xs
LDSCRIPT := $(XS_DIR)/ldscript.txt

C_SUBDIR = engine/src
ASM_SUBDIR = asm
DATA_ASM_SUBDIR = $(XS_DIR)/data

C_BUILDDIR = $(OBJ_DIR)/src
ASM_BUILDDIR = $(OBJ_DIR)/$(ASM_SUBDIR)
DATA_ASM_BUILDDIR = $(OBJ_DIR)/data
SOUND_BUILDDIR = $(OBJ_DIR)/sound
GRAPHICS_BUILDDIR = $(OBJ_DIR)/graphics

$(shell mkdir -p $(C_BUILDDIR) $(ASM_BUILDDIR) $(DATA_ASM_BUILDDIR) $(SOUND_BUILDDIR) $(GRAPHICS_BUILDDIR))

C_SRCS := $(shell find $(C_SUBDIR) -name '*.c' | sort)
C_ASMS := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.s,$(C_SRCS))
C_OBJS := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.o,$(C_SRCS))

# The game objects written as C++ (#664, docs/cplusplus.md): every .cpp
# under engine/src/, at any depth (engine/src/engine/<ns>/, engine/src/game/<ns>/,
# engine/src/game/vehicles/polar/, the data/ subdirectories; docs/file_layout.md),
# built by agbcp/old_agbcp (see "C++ objects" below).
CXX_SRCS := $(shell find $(C_SUBDIR) -name '*.cpp' | sort)
CXX_ASMS := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.s,$(CXX_SRCS))
CXX_OBJS := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.o,$(CXX_SRCS))

# The ROM data tables: the .cpp files of the data/ subdirectories, next to
# the code that uses them (engine/src/<area>/<namespace>/data/, docs/file_layout.md).
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
# L_<name> defined, one object per function (LIBGCC2_FUNCS, LIB1FUNCS,
# above the game switch).
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

include $(XS_DIR)/graphics.mk
include $(XS_DIR)/levels.mk
include $(XS_DIR)/text.mk
include $(XS_DIR)/cutscenes.mk

# A new song's settings are its games/xs/sound/songs/<key>.json (docs/audio.md,
# "Adding a song").
SOUND_SONGS := $(wildcard $(XS_DIR)/sound/songs/*.xm) $(wildcard $(XS_DIR)/sound/songs/*.json)
SOUND_SAMPLES := $(wildcard $(XS_DIR)/sound/samples/*.wav)
# New instruments (docs/audio.md, "New instruments"): games/xs/sound/instruments/
# <name>.json, each with a WAV under games/xs/sound/, for the songs and for the
# sound effects sfx_table.json names.
SOUND_INSTRUMENTS := $(wildcard $(XS_DIR)/sound/instruments/*.json) $(SOUND_SAMPLES) $(wildcard $(XS_DIR)/sound/sfx_samples/*.wav)
SOUND_SFX_SOURCES := $(XS_DIR)/sound/gax_sfx_manifest.json $(XS_DIR)/sound/sfx_table.json $(SOUND_INSTRUMENTS)
SOUND_BUILT := $(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_audio_data.s \
	$(SOUND_BUILDDIR)/gax_default_layout.bin $(SOUND_BUILDDIR)/gax_default_layout.s \
	$(SOUND_BUILDDIR)/gax_sfx_data.bin $(SOUND_BUILDDIR)/gax_sfx_data.s $(SOUND_BUILDDIR)/sfx_table.bin

#### Main Targets ####

.PHONY: all compare
ifeq ($(PLATFORM),gba)
all: $(ROM)

compare: $(ROM)
	sha1sum -c $(XS_DIR)/checksum.sha1
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

# The editors' contract (#1167, docs/editors.md). `make data-check` runs
# every check of the data without building (tools/data_check.py: each
# file against its JSON Schema, then the model's checks): both games'
# data, or GAME's when given (`make GAME=n2 data-check`), or with MOD a
# mod's, applied to MOD_TREE first (`make data-check MOD=mods/sample`).
# DATA_CHECK_ARGS go to the tool (`--json`, `--no-text`). `make schemas`
# writes packages/crashdata/schemas/ from the model, `make schemas-check`
# fails if they are out of date.
DATA_CHECK_ARGS ?=
DATA_CHECK_GAMES := $(if $(filter command line environment,$(origin GAME)),$(GAME),xs n2)
.PHONY: data-check schemas schemas-check
data-check:
ifneq ($(filter command line environment,$(origin MOD)),)
	rm -rf $(MOD_TREE)
	python3 tools/apply_mod.py $(MOD) $(MOD_TREE)
	cd $(MOD_TREE) && python3 tools/data_check.py --game xs $(DATA_CHECK_ARGS)
else
	$(foreach g,$(DATA_CHECK_GAMES),python3 tools/data_check.py --game $(g) $(DATA_CHECK_ARGS) &&) true
endif

schemas:
	python3 tools/schemas.py write

schemas-check:
	python3 tools/schemas.py --check

# Every incbin and include in games/xs/data/*.s (the sound blocks) needs the
# corresponding built file to exist first. Each GAX2 block is a .bin and
# the .s that data.s includes: its bytes, with the pointers as
# relocations. The graphics and the level assets are C arrays in
# the data/ directories (graphics.mk, levels.mk).
$(DATA_ASM_OBJS): $(SOUND_BUILT)

# The music block starts with pointers into the sound-effect set (to its
# handler type, at an offset the set's layout gives), so it depends on
# the set's sources too. The same run writes the default song's layout
# struct (gGaxDefaultSong) and the song offsets the song table
# (engine/src/engine/audio/data/song_table.cpp) is written with.
$(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_audio_data.s $(SOUND_BUILDDIR)/gax_default_layout.bin \
		$(SOUND_BUILDDIR)/gax_default_layout.s $(SOUND_BUILDDIR)/gax_songs.h &: $(XS_DIR)/sound/gax_manifest.json $(SOUND_SONGS) $(SOUND_INSTRUMENTS) $(SOUND_SFX_SOURCES) $(PY_GAX_AUDIO)
	python3 tools/gax_audio.py $(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_default_layout.bin $(SOUND_BUILDDIR)/gax_songs.h

# gSongTable itself, in song id order (the manifest's song_table), which
# song_table.cpp #includes.
$(SOUND_BUILDDIR)/song_table.inc: $(XS_DIR)/sound/gax_manifest.json $(PY_GAX_AUDIO)
	python3 tools/gax_audio.py --table $@

$(C_BUILDDIR)/engine/audio/data/song_table.o: CPPFLAGS += -iquote $(SOUND_BUILDDIR)
$(C_BUILDDIR)/engine/audio/data/song_table.o: $(SOUND_BUILDDIR)/gax_songs.h $(SOUND_BUILDDIR)/song_table.inc

$(SOUND_BUILDDIR)/gax_sfx_data.bin $(SOUND_BUILDDIR)/gax_sfx_data.s &: $(SOUND_SFX_SOURCES) $(PY_GAX_AUDIO)
	python3 tools/gax_audio.py --sfx $(SOUND_BUILDDIR)/gax_sfx_data.bin

# An entry naming a new instrument gets its number in the set.
$(SOUND_BUILDDIR)/sfx_table.bin: $(SOUND_SFX_SOURCES) $(PY_SFX_TABLE) $(PY_GAX_AUDIO)
	python3 tools/sfx_table.py $@

# The song and sound-effect IDs (#655), generated like levels.mk's
# constants headers: the manifest's song_table (gSongTable's order) and
# the names in sfx_table.json. Only the manifest is read, so these don't
# wait for the music block.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/constants/songs.h $(GENERATED_INCLUDE_DIR)/constants/sfx.h

$(GENERATED_INCLUDE_DIR)/constants/songs.h: $(XS_DIR)/sound/gax_manifest.json $(XS_DIR)/sound/music.json $(PY_GAX_AUDIO)
	@mkdir -p $(dir $@)
	python3 tools/gax_audio.py --constants $@

$(GENERATED_INCLUDE_DIR)/constants/sfx.h: $(XS_DIR)/sound/sfx_table.json $(PY_SFX_TABLE)
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
# `make screen-check` compiles every game object (engine/src/, and of the
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
# swapped for raw asm. The data units (games/xs/data/data.s) need the repo-built
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
