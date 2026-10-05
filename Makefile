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
AIF := tools/aif2pcm/aif2pcm
MID := $(abspath tools/mid2agb/mid2agb)
SCANINC := tools/scaninc/scaninc
PREPROC := tools/preproc/preproc
RAMSCRGEN := tools/ramscrgen/ramscrgen
FIX := tools/gbafix/gbafix

CC1FLAGS := -mthumb-interwork -Wimplicit -Wparentheses -O2 -fhex-asm  -fprologue-bugfix
# The libraries' public headers (lib/*/include) are on the -I path, so
# game code includes them as <gax.h>, <agb_eeprom.h>, <agb_syscall.h>.
CPPFLAGS := -I tools/agbcc/include -iquote include $(patsubst %,-I %,$(wildcard lib/*/include)) -nostdinc -undef
ASFLAGS  := -mcpu=arm7tdmi -mthumb-interwork -I asminclude

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

OBJS := $(C_OBJS) $(LIB_OBJS) $(ASM_OBJS) $(DATA_ASM_OBJS)
OBJS_REL := $(patsubst $(OBJ_DIR)/%,%,$(OBJS))

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

clean:
	$(RM) $(ROM) $(ELF) $(MAP) $(OBJS) $(C_ASMS) $(LIB_C_ASMS) $(LIBGCC2_ASMS)

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
report: $(C_OBJS) $(LIB_C_OBJS) $(LIBGCC2_OBJS) $(GRAPHICS_BUILT) $(LEVELS_BUILT) $(SOUND_BUILT)
	python3 tools/report_units.py

#### Recipes ####
	
$(ELF): $(OBJS) $(LDSCRIPT)
	$(LD) -T $(LDSCRIPT) -Map $(MAP) $(OBJS) tools/agbcc/lib/libgcc.a tools/agbcc/lib/libc.a -o $@

%.gba: %.elf
	$(OBJCOPY) -O binary $< $@

# Translation units the original build compiled with the older agbcc
# (tools/agbcc/bin/old_agbcc). Its scheduler loads a constant *before*
# the byte it is combined with (`movs rA, #K; ldrb rB, [..]; ands rA, rB`)
# where the current agbcc loads the byte first - see
# docs/matching/issue-24-boss-actor.md. old_agbcc has no
# -fprologue-bugfix option.
OLD_AGBCC_OBJS := $(C_BUILDDIR)/graphics/actor_part.o \
                  $(C_BUILDDIR)/gfx/sprite_pieces.o \
                  $(C_BUILDDIR)/gfx/affine_sprite_pieces.o \
                  $(C_BUILDDIR)/text/wrapped_text.o \
                  $(C_BUILDDIR)/graphics/actor_part101.o \
                  $(C_BUILDDIR)/graphics/actor_part103.o \
                  $(C_BUILDDIR)/graphics/actor_part109.o \
                  $(C_BUILDDIR)/graphics/actor_part118.o \
                  $(C_BUILDDIR)/graphics/actor_part112.o \
                  $(C_BUILDDIR)/graphics/actor_part110.o \
                  $(C_BUILDDIR)/graphics/actor_part111.o \
                  $(C_BUILDDIR)/graphics/actor_part11b.o \
                  $(C_BUILDDIR)/graphics/actor_part11c.o \
                  $(C_BUILDDIR)/graphics/actor_part11d.o \
                  $(C_BUILDDIR)/graphics/actor_part11e.o \
                  $(C_BUILDDIR)/graphics/actor_part11f.o \
                  $(C_BUILDDIR)/graphics/actor_part12.o \
                  $(C_BUILDDIR)/graphics/actor_part120.o \
                  $(C_BUILDDIR)/graphics/actor_part122.o \
                  $(C_BUILDDIR)/graphics/actor_part123.o \
                  $(C_BUILDDIR)/graphics/actor_part127.o \
                  $(C_BUILDDIR)/graphics/actor_part128.o \
                  $(C_BUILDDIR)/graphics/actor_part130.o \
                  $(C_BUILDDIR)/graphics/actor_part131.o \
                  $(C_BUILDDIR)/graphics/actor_part18.o \
                  $(C_BUILDDIR)/graphics/actor_part19h.o \
                  $(C_BUILDDIR)/graphics/actor_part23b.o \
                  $(C_BUILDDIR)/graphics/actor_part24b.o \
                  $(C_BUILDDIR)/graphics/actor_part27a.o \
                  $(C_BUILDDIR)/graphics/actor_part2.o \
                  $(C_BUILDDIR)/graphics/actor_part3.o \
                  $(C_BUILDDIR)/graphics/actor_part38.o \
                  $(C_BUILDDIR)/graphics/actor_part38b.o \
                  $(C_BUILDDIR)/graphics/actor_part38c.o \
                  $(C_BUILDDIR)/graphics/actor_part45d.o \
                  $(C_BUILDDIR)/graphics/actor_part7.o \
                  $(C_BUILDDIR)/graphics/actor_part74.o \
                  $(C_BUILDDIR)/graphics/actor_part75.o \
                  $(C_BUILDDIR)/graphics/actor_part78.o \
                  $(C_BUILDDIR)/graphics/actor_part7b.o \
                  $(C_BUILDDIR)/graphics/actor_part81.o \
                  $(C_BUILDDIR)/graphics/actor_part82.o \
                  $(C_BUILDDIR)/graphics/actor_part83.o \
                  $(C_BUILDDIR)/graphics/actor_part84.o \
                  $(C_BUILDDIR)/graphics/actor_part86.o \
                  $(C_BUILDDIR)/graphics/actor_part89.o \
                  $(C_BUILDDIR)/graphics/actor_part86b.o \
                  $(C_BUILDDIR)/graphics/actor_part88.o \
                  $(C_BUILDDIR)/graphics/actor_part_12fbc.o \
                  $(C_BUILDDIR)/graphics/actor_part_134b8.o \
                  $(C_BUILDDIR)/graphics/actor_part_138e8.o \
                  $(C_BUILDDIR)/graphics/actor_part_13c60.o \
                  $(C_BUILDDIR)/graphics/actor_part_14674.o \
                  $(C_BUILDDIR)/graphics/actor_part_16048.o \
                  $(C_BUILDDIR)/graphics/actor_part_17524.o \
                  $(C_BUILDDIR)/graphics/actor_part_18008.o \
                  $(C_BUILDDIR)/graphics/actor_part_188d0.o \
                  $(C_BUILDDIR)/graphics/actor_part_1967c.o \
                  $(C_BUILDDIR)/graphics/actor_part_1a878.o \
                  $(C_BUILDDIR)/graphics/actor_part_1ab98.o \
                  $(C_BUILDDIR)/graphics/actor_part_1b85c.o \
                  $(C_BUILDDIR)/graphics/actor_part_1cee0.o \
                  $(C_BUILDDIR)/graphics/actor_part_1da38.o \
                  $(C_BUILDDIR)/graphics/actor_part_1dfec.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_1e990.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_1ea5c.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_1ef0c.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_1fdec.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_1feec.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_21280.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_21668.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_35780.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_35d1c.o \
                  $(C_BUILDDIR)/graphics/graphics_loading_3686c.o \
                  $(C_BUILDDIR)/gfx/graphics_package.o \
                  $(C_BUILDDIR)/graphics/hud_digit_array.o \
                  $(C_BUILDDIR)/text/font_glyph.o \
                  $(C_BUILDDIR)/text/font_draw_text.o \
                  $(C_BUILDDIR)/text/font_measure.o \
                  $(C_BUILDDIR)/graphics/hud_stat_widget2.o \
                  $(C_BUILDDIR)/graphics/hud_stat_widget3.o \
                  $(C_BUILDDIR)/graphics/level_graphics.o \
                  $(C_BUILDDIR)/graphics/settings_menu.o \
                  $(C_BUILDDIR)/graphics/settings_menu10.o \
                  $(C_BUILDDIR)/graphics/settings_menu20.o \
                  $(C_BUILDDIR)/graphics/settings_menu22.o \
                  $(C_BUILDDIR)/graphics/settings_menu6.o \
                  $(C_BUILDDIR)/graphics/trigger_effect.o \
                  $(C_BUILDDIR)/system/bg_scroll_layer_25fc8.o \
                  $(C_BUILDDIR)/system/game_loop14.o \
                  $(C_BUILDDIR)/system/game_loop16.o \
                  $(C_BUILDDIR)/system/game_loop29.o \
                  $(C_BUILDDIR)/system/game_loop3.o \
                  $(C_BUILDDIR)/system/game_loop32.o \
                  $(C_BUILDDIR)/system/game_loop36.o \
                  $(C_BUILDDIR)/system/game_loop37.o \
                  $(C_BUILDDIR)/system/game_loop4.o \
                  $(C_BUILDDIR)/system/game_loop40.o \
                  $(C_BUILDDIR)/system/game_loop41.o \
                  $(C_BUILDDIR)/system/game_loop42.o \
                  $(C_BUILDDIR)/system/game_loop46.o \
                  $(C_BUILDDIR)/system/game_loop47.o \
                  $(C_BUILDDIR)/system/game_loop48.o \
                  $(C_BUILDDIR)/system/game_loop49.o \
                  $(C_BUILDDIR)/system/game_loop51.o \
                  $(C_BUILDDIR)/system/game_loop52.o \
                  $(C_BUILDDIR)/system/game_loop53.o \
                  $(C_BUILDDIR)/system/game_loop54.o \
                  $(C_BUILDDIR)/system/game_loop55.o \
                  $(C_BUILDDIR)/system/game_loop56.o \
                  $(C_BUILDDIR)/system/game_loop57.o \
                  $(C_BUILDDIR)/system/game_loop6.o \
                  $(C_BUILDDIR)/system/game_loop7.o \
                  $(C_BUILDDIR)/system/game_loop8.o \
                  $(C_BUILDDIR)/link/link_handshake.o \
                  $(C_BUILDDIR)/link/link_session_reset.o \
                  $(C_BUILDDIR)/link/link_session.o
$(OLD_AGBCC_OBJS): CC1 := $(CC1_OLD)
$(OLD_AGBCC_OBJS): CC1FLAGS := $(filter-out -fprologue-bugfix,$(CC1FLAGS))

# Objects built with -fno-strength-reduce on top of their compiler's -O2.
# graphics_loading_35d1c: InitVvLogoPieces's first loop keeps its up-counting
# `i` (with strength reduction on, gcc reverses a loop whose counter only
# feeds the exit test), and the flag leaves every other real-C function in
# the file byte-identical. It is NOT a global property: adding it to all
# old_agbcc objects breaks matched functions in 11 other files, and
# DrawTitleLogoPieces (graphics_loading_35780.o, split off for this reason) needs
# strength reduction ON: its up-counting inner loop must be reversed. See
# docs/matching/per-file-flags-investigation.md and
# docs/matching/issue-64-65-naked-retry-2.md. graphics_loading_3686c.o
# (DrawVvLogoPieces onward) was split off it for the same reason and is NOT
# listed: DrawVvLogoPieces's reversed header loop and reduced row pointer need
# strength reduction on; the rest of that file also matches with it on
# (docs/matching/sr65-naked-retry.md).
NO_STRENGTH_REDUCE_OBJS := $(C_BUILDDIR)/graphics/graphics_loading_35d1c.o
$(NO_STRENGTH_REDUCE_OBJS): CC1FLAGS += -fno-strength-reduce

# Objects built with -fno-rerun-loop-opt (one loop-optimizer pass).
# link_cable_01db4 holds only ResetLinkSessionState, which is real C: it keeps the
# ROM's up-counting inner copy loop only with this flag (the rerun pass
# reverses it; without the flag the matching C is 147 halfwords off and
# 16 bytes long). The flag changes the matching HandleLinkSerial, which is
# why ResetLinkSessionState was split out of link_cable.c. See
# docs/matching/last-ten-naked-retry.md and
# docs/matching/last-eleven-naked-retry.md.
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
# docs/matching/gax-toolchain-retry.md.
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
$(ARM_OBJS): CC1FLAGS := -mthumb-interwork -Wimplicit -Wparentheses -O2 -fomit-frame-pointer

$(C_BUILDDIR)/%.o : $(C_SUBDIR)/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s

$(LIB_BUILDDIR)/%.o : $(LIB_SUBDIR)/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $(LIB_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(LIB_BUILDDIR)/$*.s

$(LIBGCC2_OBJS): $(LIB_BUILDDIR)/libgcc/%.o: $(LIB_SUBDIR)/libgcc/libgcc2.c $(LIB_SUBDIR)/libgcc/libgcc2_udivmoddi4.h
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -DL$* $< | $(CC1) $(CC1FLAGS) -o $(LIB_BUILDDIR)/libgcc/$*.s
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
