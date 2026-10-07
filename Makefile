.DEFAULT_GOAL := compare

#### Tools ####

PREFIX	 := arm-none-eabi-
CC1      := tools/agbcc/bin/agbcc
CC1_OLD  := tools/agbcc/bin/old_agbcc
CPP      := $(PREFIX)cpp
AS       := $(PREFIX)as
LD       := $(PREFIX)ld
OBJCOPY  := $(PREFIX)objcopy

GFX := tools/gbagfx/gbagfx
GRIT := tools/grit/grit

# Warnings (#577). Every C object, whichever compiler builds it (agbcc,
# old_agbcc, agbcc_arm), gets this set, and -Werror makes any warning
# fail the build, locally and in CI. A fix must keep the ROM matching;
# see "Compiler warnings" in CONTRIBUTING.md for how to handle a warning
# byte-neutrally and the per-site escape hatches.
WARNFLAGS := -Wall -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wnested-externs -Wredundant-decls -Werror
CC1FLAGS := -mthumb-interwork $(WARNFLAGS) -O2 -fhex-asm  -fprologue-bugfix
# The libraries' public headers (lib/*/include) are on the -I path, so
# game code includes them as <gax.h>, <agb_eeprom.h>, <agb_syscall.h>.
CPPFLAGS := -I tools/agbcc/include -iquote include $(patsubst %,-I %,$(wildcard lib/*/include)) -nostdinc -undef
ASFLAGS  := -mcpu=arm7tdmi -mthumb-interwork -I asminclude
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

#### Files ####
OBJ_DIR:= build/crashbandicootxs
ROM      := crashbandicootxs.gba
ELF      := $(ROM:.gba=.elf)
MAP      := $(ROM:.gba=.map)
LDSCRIPT := ldscript.txt

C_SUBDIR = src
ASM_SUBDIR = asm
DATA_ASM_SUBDIR = data

C_BUILDDIR = $(OBJ_DIR)/$(C_SUBDIR)
ASM_BUILDDIR = $(OBJ_DIR)/$(ASM_SUBDIR)
DATA_ASM_BUILDDIR = $(OBJ_DIR)/$(DATA_ASM_SUBDIR)
SOUND_BUILDDIR = $(OBJ_DIR)/sound
GRAPHICS_BUILDDIR = $(OBJ_DIR)/graphics

$(shell mkdir -p $(C_BUILDDIR) $(ASM_BUILDDIR) $(DATA_ASM_BUILDDIR) $(SOUND_BUILDDIR) $(GRAPHICS_BUILDDIR))

C_SRCS := $(wildcard $(C_SUBDIR)/*/*.c)
C_ASMS := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.s,$(C_SRCS))
C_OBJS := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.o,$(C_SRCS))

# The game objects written as C++ (#664, docs/cplusplus.md): src/*/*.cpp,
# built by agbcp/old_agbcp (see "C++ objects" below).
CXX_SRCS := $(wildcard $(C_SUBDIR)/*/*.cpp)
CXX_ASMS := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.s,$(CXX_SRCS))
CXX_OBJS := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.o,$(CXX_SRCS))

ASM_SRCS := $(wildcard $(ASM_SUBDIR)/*.s)
ASM_OBJS := $(patsubst $(ASM_SUBDIR)/%.s,$(ASM_BUILDDIR)/%.o,$(ASM_SRCS))

DATA_ASM_SRCS := $(wildcard $(DATA_ASM_SUBDIR)/*.s)
DATA_ASM_OBJS := $(patsubst $(DATA_ASM_SUBDIR)/%.s,$(DATA_ASM_BUILDDIR)/%.o,$(DATA_ASM_SRCS))

#### Libraries ####
# Third-party and SDK code linked into the ROM, kept apart from the game
# under lib/ (see docs/libraries.md). They are linked as plain objects:
# ldscript.txt interleaves them with the game code in ROM order.
#  - lib/gax: Shin'en's GAX2 sound engine (src/*.c code, data/*.c tables)
#  - lib/agb_eeprom: Nintendo's AgbEeprom SDK library, EEPROM_V122
#  - lib/libgcc: libgcc2.c's 64-bit helpers and lib1funcs.asm's routines
#  - lib/libagbsyscall: the BIOS SWI wrappers
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

LIBAGBSYSCALL_OBJS := $(LIB_BUILDDIR)/libagbsyscall/libagbsyscall.o

LIB_OBJS := $(LIB_C_OBJS) $(LIBGCC2_OBJS) $(LIB1FUNCS_OBJS) $(LIBAGBSYSCALL_OBJS)

OBJS := $(C_OBJS) $(CXX_OBJS) $(LIB_OBJS) $(ASM_OBJS) $(DATA_ASM_OBJS)

# The .d files the C objects' preprocess step writes (see DEPFLAGS).
DEPS := $(C_OBJS:.o=.d) $(CXX_OBJS:.o=.d) $(LIB_C_OBJS:.o=.d) $(LIBGCC2_OBJS:.o=.d)

include graphics.mk
include levels.mk

# PNGs in GRIT_C_PNGS (graphics.mk) become C arrays, not data.s incbins.
GRAPHICS_PNGS := $(filter-out $(GRIT_C_PNGS),$(wildcard graphics/*/*.png))
GRAPHICS_PALS := $(wildcard graphics/*/*.pal)
GRAPHICS_BINS := $(wildcard graphics/*/*.bin)

GRAPHICS_BUILT := \
	$(patsubst graphics/%_bitmap.png,$(GRAPHICS_BUILDDIR)/%_bitmap.bin.lz,$(filter %_bitmap.png,$(GRAPHICS_PNGS))) \
	$(patsubst graphics/%_8bpp_tiles.png,$(GRAPHICS_BUILDDIR)/%_8bpp_tiles.8bpp.lz,$(filter %_8bpp_tiles.png,$(GRAPHICS_PNGS))) \
	$(patsubst graphics/%.png,$(GRAPHICS_BUILDDIR)/%.4bpp.lz,$(filter-out %_bitmap.png %_8bpp_tiles.png,$(GRAPHICS_PNGS))) \
	$(patsubst graphics/%.pal,$(GRAPHICS_BUILDDIR)/%.gbapal.lz,$(GRAPHICS_PALS)) \
	$(patsubst graphics/%.bin,$(GRAPHICS_BUILDDIR)/%.bin.lz,$(GRAPHICS_BINS)) \
	$(GRAPHICS_BUILDDIR)/unknown/00_0b2120.bin.lz \
	$(GRAPHICS_BUILDDIR)/unknown/01_14174c.bin.lz

SOUND_SONGS := $(wildcard sound/songs/*.xm)
SOUND_SAMPLES := $(wildcard sound/samples/*.wav)
SOUND_SFX_SOURCES := sound/gax_sfx_manifest.json $(wildcard sound/sfx_samples/*.wav)
SOUND_BUILT := $(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_default_layout.bin \
	$(SOUND_BUILDDIR)/gax_sfx_data.bin $(SOUND_BUILDDIR)/sfx_table.bin

#### Main Targets ####

compare: $(ROM)
	sha1sum -c checksum.sha1

# Every incbin in data/*.s that pulls from graphics/ or sound/ needs the
# corresponding built file to exist first.
$(DATA_ASM_OBJS): $(GRAPHICS_BUILT) $(LEVELS_BUILT) $(SOUND_BUILT)

# The music block starts with pointers into the sound-effect set, so it
# depends on the set's sources too. The same run writes the default song's
# layout struct (gGaxDefaultSong) and the song offsets the song table
# (src/data/song_table_16aa20.c) is written with.
$(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_default_layout.bin $(SOUND_BUILDDIR)/gax_songs.h &: sound/gax_manifest.json $(SOUND_SONGS) $(SOUND_SAMPLES) $(SOUND_SFX_SOURCES) tools/gax_audio.py
	python3 tools/gax_audio.py $(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/gax_default_layout.bin $(SOUND_BUILDDIR)/gax_songs.h

$(C_BUILDDIR)/data/song_table_16aa20.o: CPPFLAGS += -iquote $(SOUND_BUILDDIR)
$(C_BUILDDIR)/data/song_table_16aa20.o: $(SOUND_BUILDDIR)/gax_songs.h

$(SOUND_BUILDDIR)/gax_sfx_data.bin: $(SOUND_SFX_SOURCES) tools/gax_audio.py
	python3 tools/gax_audio.py --sfx $@

$(SOUND_BUILDDIR)/sfx_table.bin: sound/sfx_table.json tools/sfx_table.py
	python3 tools/sfx_table.py $@

# The song and sound-effect IDs (#655), generated like levels.mk's
# constants headers: the manifest's song_table (gSongTable's order) and
# the names in sfx_table.json. Only the manifest is read, so these don't
# wait for the music block.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/constants/songs.h $(GENERATED_INCLUDE_DIR)/constants/sfx.h

$(GENERATED_INCLUDE_DIR)/constants/songs.h: sound/gax_manifest.json tools/gax_audio.py
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

#### decomp.dev progress report ####
# See docs/decomp_dev.md. `report` builds one objdiff unit per matched
# src/*.c file (rather than one merged blob) so decomp.dev can show
# per-category (Graphics/Util/System) progress, not just an overall
# total - tools/report_units.py has the full explanation of why it has
# to be this fine-grained, the per-file address table, and how it slices
# expected/legacy.s/expected/code_3.s (frozen, never rebuilt from
# anything else) and applies expected/corrections.txt per slice. Run
# under `NON_MATCHING=1` so parked functions are included as their
# (possibly imperfect) C reconstruction rather than omitted or silently
# swapped for raw asm. The data units (data/data.s) need the repo-built
# assets it incbins, but not data.o itself (that needs baserom.gba).

.PHONY: report
report: $(C_OBJS) $(CXX_OBJS) $(LIB_C_OBJS) $(LIBGCC2_OBJS) $(GRAPHICS_BUILT) $(LEVELS_BUILT) $(SOUND_BUILT)
	python3 tools/report_units.py

#### Recipes ####
	
$(ELF): $(OBJS) $(LDSCRIPT)
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) -Map $(MAP) $(OBJS) tools/agbcc/lib/libgcc.a tools/agbcc/lib/libc.a -o $@

%.gba: %.elf
	$(OBJCOPY) -O binary $< $@

# Translation units the original build compiled with the older agbcc
# (tools/agbcc/bin/old_agbcc). Its scheduler loads a constant *before*
# the byte it is combined with (`movs rA, #K; ldrb rB, [..]; ands rA, rB`)
# where the current agbcc loads the byte first - see
# docs/matching/archive/issue-24-boss-actor.md. old_agbcc has no
# -fprologue-bugfix option.
OLD_AGBCC_OBJS := $(C_BUILDDIR)/objects/sprite.o \
                  $(C_BUILDDIR)/gfx/sprite_pieces.o \
                  $(C_BUILDDIR)/gfx/affine_sprite_pieces.o \
                  $(C_BUILDDIR)/text/wrapped_text.o \
                  $(C_BUILDDIR)/actor/actor_category_init.o \
                  $(C_BUILDDIR)/actor/actor_category_frame.o \
                  $(C_BUILDDIR)/crates/crate_touch.o \
                  $(C_BUILDDIR)/enemies/enemy_patrol.o \
                  $(C_BUILDDIR)/enemies/enemy_ctrl_update.o \
                  $(C_BUILDDIR)/objects/ground_sprite_collide.o \
                  $(C_BUILDDIR)/crates/crate_grid_unlink.o \
                  $(C_BUILDDIR)/crates/crate_list_update.o \
                  $(C_BUILDDIR)/crates/crate_player_collide.o \
                  $(C_BUILDDIR)/crates/crate_grid_collide.o \
                  $(C_BUILDDIR)/crates/crate_list.o \
                  $(C_BUILDDIR)/enemies/enemy_attack.o \
                  $(C_BUILDDIR)/objects/ctrl.o \
                  $(C_BUILDDIR)/bosses/tiny_hop_pad.o \
                  $(C_BUILDDIR)/objects/effect_ctrl.o \
                  $(C_BUILDDIR)/vehicle/polar_player.o \
                  $(C_BUILDDIR)/vehicle/jetpack_spawn.o \
                  $(C_BUILDDIR)/bosses/hovercraft.o \
                  $(C_BUILDDIR)/frontend/credits.o \
                  $(C_BUILDDIR)/vehicle/polar_nitro.o \
                  $(C_BUILDDIR)/bosses/airship_map.o \
                  $(C_BUILDDIR)/bosses/airship_touch.o \
                  $(C_BUILDDIR)/bosses/mega_mix_update.o \
                  $(C_BUILDDIR)/player/action_ctrl_moves.o \
                  $(C_BUILDDIR)/actor/bg_picture.o \
                  $(C_BUILDDIR)/objects/sprite_anim.o \
                  $(C_BUILDDIR)/vehicle/yeti_update.o \
                  $(C_BUILDDIR)/vehicle/yeti_graphics.o \
                  $(C_BUILDDIR)/player/player_collide.o \
                  $(C_BUILDDIR)/objects/part_collide.o \
                  $(C_BUILDDIR)/player/player_event.o \
                  $(C_BUILDDIR)/player/action_ctrl_event.o \
                  $(C_BUILDDIR)/player/action_ctrl_idle.o \
                  $(C_BUILDDIR)/player/action_ctrl_update.o \
                  $(C_BUILDDIR)/player/swim_ctrl_stroke.o \
                  $(C_BUILDDIR)/menus/continue_prompt.o \
                  $(C_BUILDDIR)/player/action_ctrl_run_jump.o \
                  $(C_BUILDDIR)/player/action_ctrl_states.o \
                  $(C_BUILDDIR)/player/action_ctrl_hang.o \
                  $(C_BUILDDIR)/player/swim_ctrl.o \
                  $(C_BUILDDIR)/player/input_ctrl.o \
                  $(C_BUILDDIR)/bosses/tiny_update.o \
                  $(C_BUILDDIR)/bosses/cortex.o \
                  $(C_BUILDDIR)/bosses/dingodile.o \
                  $(C_BUILDDIR)/objects/platform_create.o \
                  $(C_BUILDDIR)/objects/platform_collide.o \
                  $(C_BUILDDIR)/menus/level_select.o \
                  $(C_BUILDDIR)/menus/level_select_pages.o \
                  $(C_BUILDDIR)/menus/level_select_widgets.o \
                  $(C_BUILDDIR)/level/spawn_start_marker.o \
                  $(C_BUILDDIR)/level/spawn_gems.o \
                  $(C_BUILDDIR)/level/spawn_enemies.o \
                  $(C_BUILDDIR)/level/spawn_bosses.o \
                  $(C_BUILDDIR)/level/spawn_objects.o \
                  $(C_BUILDDIR)/frontend/title_screen.o \
                  $(C_BUILDDIR)/frontend/company_logos.o \
                  $(C_BUILDDIR)/gfx/graphics_package.o \
                  $(C_BUILDDIR)/hud/hud_init.o \
                  $(C_BUILDDIR)/text/font_glyph.o \
                  $(C_BUILDDIR)/text/font_draw_text.o \
                  $(C_BUILDDIR)/text/font_measure.o \
                  $(C_BUILDDIR)/hud/hud_boss_clock.o \
                  $(C_BUILDDIR)/hud/hud_counters.o \
                  $(C_BUILDDIR)/frontend/title_screen_init.o \
                  $(C_BUILDDIR)/save/save_menu_draw.o \
                  $(C_BUILDDIR)/menus/power_dialog_loop.o \
                  $(C_BUILDDIR)/menus/pause_menu_loop.o \
                  $(C_BUILDDIR)/menus/pause_menu_gems.o \
                  $(C_BUILDDIR)/menus/pause_menu_pages_init.o \
                  $(C_BUILDDIR)/level/spawn_gem_platforms.o \
                  $(C_BUILDDIR)/level/entity_spawner.o \
                  $(C_BUILDDIR)/level/bg_layer.o \
                  $(C_BUILDDIR)/level/drop_extra_life.o \
                  $(C_BUILDDIR)/level/bg_layer_base.o \
                  $(C_BUILDDIR)/crates/crate_create.o \
                  $(C_BUILDDIR)/cutscene/slideshow.o \
                  $(C_BUILDDIR)/level/tile_cache.o \
                  $(C_BUILDDIR)/level/time_trial.o \
                  $(C_BUILDDIR)/level/room_entities.o \
                  $(C_BUILDDIR)/crates/crate_hit.o \
                  $(C_BUILDDIR)/level/terrain_probe_axes.o \
                  $(C_BUILDDIR)/crates/crate_break.o \
                  $(C_BUILDDIR)/crates/crate_update.o \
                  $(C_BUILDDIR)/pickups/wumpa_update.o \
                  $(C_BUILDDIR)/pickups/extra_life.o \
                  $(C_BUILDDIR)/level/game_frame.o \
                  $(C_BUILDDIR)/level/run_room.o \
                  $(C_BUILDDIR)/cutscene/cutscene_player.o \
                  $(C_BUILDDIR)/level/room_frame.o \
                  $(C_BUILDDIR)/link/link_handshake.o \
                  $(C_BUILDDIR)/link/link_session_reset.o \
                  $(C_BUILDDIR)/link/link_session.o
$(OLD_AGBCC_OBJS): CC1 := $(CC1_OLD)
$(OLD_AGBCC_OBJS): CC1FLAGS := $(filter-out -fprologue-bugfix,$(CC1FLAGS))

# C++ objects (#664, docs/cplusplus.md). The game is g++ 2.x C++; these
# objects are built from C++ source with the C++ front end of the same
# compiler: agbcp, and old_agbcp for the ones in OLD_AGBCC_OBJS. Both are
# built from notyourav/agbcc's `cp` branch by tools/build_agbccpp.sh,
# which ports agbcc's -fprologue-bugfix and OLD_COMPILER switches to it,
# so the C flags above apply unchanged. The game had no RTTI (every
# vtable's slot 0 is empty) and no exceptions. After assembling,
# cxx_symbols.txt renames the mangled names to the C names the rest of
# the build links against.
CXX1     := tools/agbcc/bin/agbcp
CXX1_OLD := tools/agbcc/bin/old_agbcp
CXX_SYMBOLS := cxx_symbols.txt
$(CXX_OBJS): CC1FLAGS += -fno-rtti -fno-exceptions
$(filter $(OLD_AGBCC_OBJS),$(CXX_OBJS)): CXX1 := $(CXX1_OLD)

# Objects built with -fno-strength-reduce on top of their compiler's -O2.
# title_screen.o: InitVvLogoPieces's first loop keeps its up-counting
# `i` (with strength reduction on, gcc reverses a loop whose counter only
# feeds the exit test), and the flag leaves every other real-C function in
# the file byte-identical. It is NOT a global property: adding it to all
# old_agbcc objects breaks matched functions in 11 other files, and
# DrawTitleLogoPieces (title_screen_init.o, split off for this reason) needs
# strength reduction ON: its up-counting inner loop must be reversed. See
# docs/matching/per-file-flags-investigation.md and
# docs/matching/archive/issue-64-65-naked-retry-2.md. company_logos.o
# (DrawVvLogoPieces onward) was split off it for the same reason and is NOT
# listed: DrawVvLogoPieces's reversed header loop and reduced row pointer need
# strength reduction on; the rest of that file also matches with it on
# (docs/matching/archive/sr65-naked-retry.md).
NO_STRENGTH_REDUCE_OBJS := $(C_BUILDDIR)/frontend/title_screen.o
$(NO_STRENGTH_REDUCE_OBJS): CC1FLAGS += -fno-strength-reduce

# Objects built with -fno-rerun-loop-opt (one loop-optimizer pass).
# link_session_reset.c holds only ResetLinkSessionState, which is real C: it keeps the
# ROM's up-counting inner copy loop only with this flag (the rerun pass
# reverses it; without the flag the matching C is 147 halfwords off and
# 16 bytes long). The flag changes the matching HandleLinkSerial, which is
# why ResetLinkSessionState was split out of link_handshake.c. See
# docs/matching/archive/last-ten-naked-retry.md and
# docs/matching/archive/last-eleven-naked-retry.md.
NO_RERUN_LOOP_OPT_OBJS := $(C_BUILDDIR)/link/link_session_reset.o
$(NO_RERUN_LOOP_OPT_OBJS): CC1FLAGS += -fno-rerun-loop-opt

# Objects built with -O1 instead of -O2: the whole of lib/agb_eeprom's
# code. Nintendo's AgbEeprom SDK library (the ROM's "EEPROM_V122",
# 0x0803A968-0x0803AD7C) was compiled at -O1, and all nine of its
# functions are the SDK's plain C (TMC/pokeemerald source shape, no pins
# or volatile tricks), byte-identical at -O1:
# - eeprom_timer.o: EEPROMConfigure, EepromTimerIntr (timer
#   IRQ handler), SetEepromTimerIntr and StartEepromTimer. At -O2 the same C is 47 halfwords off in
#   StartEepromTimer; the old -O2 version needed six register pins and a
#   `vu16 * volatile` global.
# - eeprom_timer_stop.o: StopEepromTimer, DMA3Transfer - 2/43 halfwords off at -O2 (-O2 cross-jumps the
#   duplicated DMA-wait test into the loop).
# - eeprom_read_write.o: EEPROMRead/EEPROMWrite -
#   79/107 halfwords off at -O2.
# - eeprom_verify.o: EEPROMCompare/EEPROMWrite1_check.
# It is current agbcc: old_agbcc -O1 is 2/44/35 off for DMA3Transfer/
# EEPROMRead/EEPROMWrite. Every object matches as a whole with the flag.
# EEPROM_V122 is the ROM's only SDK version tag, and no other compiled
# code was found to be SDK C. The library's data object
# (lib/agb_eeprom/data/eeprom_5a9eec.o) keeps the default flags. See
# docs/matching/eeprom-sdk-o1.md.
O1_OBJS := $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_timer.o \
           $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_timer_stop.o \
           $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_read_write.o \
           $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_verify.o
$(O1_OBJS): CC1FLAGS := $(filter-out -O2,$(CC1FLAGS)) -O1

# libgcc2.c's objects (__divdi3/__udivdi3/__muldi3, linked in with the
# GAX2 library) were built without -mthumb-interwork: their functions are
# the only ones in the ROM that return via a combined `pop {r4-r7, pc}`,
# and with the flag dropped they compile from libgcc2.c's own source
# byte-for-byte - see lib/libgcc/libgcc2.c and
# docs/matching/archive/gax-toolchain-retry.md.
NO_INTERWORK_OBJS := $(LIBGCC2_OBJS)
$(NO_INTERWORK_OBJS): CC1FLAGS := $(filter-out -mthumb-interwork,$(CC1FLAGS))

# ARM-state code of the IWRAM image (ldscript.txt's `iwram` section),
# built with agbcc_arm, the ARM-targeting build of the same gcc 2.9.
# -fomit-frame-pointer: the ROM's ARM functions have no APCS frame (the
# Thumb agbcc never sets one up, the ARM one does by default). The
# prologue bugfix and -fhex-asm are Thumb agbcc-only options. See
# docs/matching/iwram-image.md.
CC1_ARM  := tools/agbcc/bin/agbcc_arm
ARM_OBJS := $(C_BUILDDIR)/iwram/string_arm.o \
            $(C_BUILDDIR)/iwram/sprite_arm.o
$(ARM_OBJS): CC1 := $(CC1_ARM)
$(ARM_OBJS): CC1FLAGS := -mthumb-interwork $(WARNFLAGS) -O2 -fomit-frame-pointer

# The two ARM objects whose last function needs agbcc_arm_patched:
# agbcc_arm plus tools/agbcc_patches/agbcc_arm_prologue_return.patch,
# built by tools/build_patched_agbcc_arm.sh. The ROM's ARM compiler is a
# later, unreleased build of agbcc_arm's line whose prologue and return
# code differ in two fixed strings; the patch adds an opt-in option for
# each. Without the options the patched compiler's output is identical
# to agbcc_arm's, so the objects' other functions are unaffected (checked
# with and without the options). See docs/matching/iwram-image.md,
# "Seventh pass".
# - string_arm.o: itoa_arm pushes r4-r6 without lr (-mleaf-no-lr-save).
#   It also needs both scheduling passes off: its loop increments,
#   terminator store and swap stay in source order in the ROM, where
#   either pass moves them. The four other string functions come out
#   the same with or without them.
# - sprite_arm.o: LookupSpriteFrameCache's three returns pop into lr
#   (-minterwork-return-lr). Its other four functions need scheduling,
#   so it keeps it.
CC1_ARM_PATCHED  := tools/agbcc/bin/agbcc_arm_patched
PATCHED_ARM_OBJS := $(C_BUILDDIR)/iwram/string_arm.o \
                    $(C_BUILDDIR)/iwram/sprite_arm.o
$(PATCHED_ARM_OBJS): CC1 := $(CC1_ARM_PATCHED)
$(C_BUILDDIR)/iwram/string_arm.o: CC1FLAGS += -mleaf-no-lr-save -fno-schedule-insns -fno-schedule-insns2
$(C_BUILDDIR)/iwram/sprite_arm.o: CC1FLAGS += -minterwork-return-lr

# Appended to every compiled .s before it is assembled (#663). agbcc
# starts each function with `.align 2, 0`, but nothing aligns the end of
# the last one, so when a .text section ends on a halfword boundary `as`
# pads it to the section's 4-byte alignment with a Thumb NOP (`mov r8, r8`,
# 0x46C0). The original build left zeros there. Ending .text with an
# explicit zero-fill `.align 2, 0` reproduces that; it adds nothing to a
# section that already ends aligned.
ZERO_PAD_TEXT := \t.text\n\t.align\t2, 0\n

$(C_BUILDDIR)/%.o : $(C_SUBDIR)/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(DEPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $(C_BUILDDIR)/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s

$(C_BUILDDIR)/%.o : $(C_SUBDIR)/%.cpp $(CXX_SYMBOLS)
	@mkdir -p $(dir $@)
	$(CPP) -x c++ $(CPPFLAGS) $(DEPFLAGS) $< | $(CXX1) -quiet $(CC1FLAGS) -o $(C_BUILDDIR)/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s
	$(OBJCOPY) --redefine-syms=$(CXX_SYMBOLS) $@

$(LIB_BUILDDIR)/%.o : $(LIB_SUBDIR)/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(DEPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $(LIB_BUILDDIR)/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(LIB_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(LIB_BUILDDIR)/$*.s

$(LIBGCC2_OBJS): $(LIB_BUILDDIR)/libgcc/%.o: $(LIB_SUBDIR)/libgcc/libgcc2.c $(LIB_SUBDIR)/libgcc/libgcc2_udivmoddi4.h
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(DEPFLAGS) -DL$* $< | $(CC1) $(CC1FLAGS) -o $(LIB_BUILDDIR)/libgcc/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(LIB_BUILDDIR)/libgcc/$*.s
	$(AS) $(ASFLAGS) -o $@ $(LIB_BUILDDIR)/libgcc/$*.s

$(LIB1FUNCS_OBJS): $(LIB_BUILDDIR)/libgcc/%.o: $(LIB_SUBDIR)/libgcc/lib1funcs.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) --defsym L$*=1 -o $@ $<

$(LIB_BUILDDIR)/%.o: $(LIB_SUBDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(ASM_BUILDDIR)/%.o: $(ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(DATA_ASM_BUILDDIR)/%.o: $(DATA_ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(GFX):
	$(MAKE) -C tools/gbagfx

$(GRIT):
	$(MAKE) -C tools/grit

-include $(DEPS)
