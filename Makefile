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
AIF := tools/aif2pcm/aif2pcm
MID := $(abspath tools/mid2agb/mid2agb)
SCANINC := tools/scaninc/scaninc
PREPROC := tools/preproc/preproc
RAMSCRGEN := tools/ramscrgen/ramscrgen
FIX := tools/gbafix/gbafix

CC1FLAGS := -mthumb-interwork -Wimplicit -Wparentheses -O2 -fhex-asm  -fprologue-bugfix
CPPFLAGS := -I tools/agbcc/include -iquote include -nostdinc -undef
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

OBJS := $(C_OBJS) $(ASM_OBJS) $(DATA_ASM_OBJS)
OBJS_REL := $(patsubst $(OBJ_DIR)/%,%,$(OBJS))

include graphics.mk

GRAPHICS_PNGS := $(wildcard graphics/*/*.png)
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

#### Main Targets ####

compare: $(ROM)
	sha1sum -c checksum.sha1

# Every incbin in data/*.s that pulls from graphics/ or sound/ needs the
# corresponding built file to exist first.
$(DATA_ASM_OBJS): $(GRAPHICS_BUILT) $(SOUND_BUILDDIR)/gax_audio_data.bin $(SOUND_BUILDDIR)/sfx_table.bin

$(SOUND_BUILDDIR)/gax_audio_data.bin: sound/gax_manifest.json sound/gax_header_prefix.bin sound/gax_footer.bin $(SOUND_SONGS) $(SOUND_SAMPLES) tools/gax_audio.py
	python3 tools/gax_audio.py $@

$(SOUND_BUILDDIR)/sfx_table.bin: sound/sfx_table.json tools/sfx_table.py
	python3 tools/sfx_table.py $@

clean:
	$(RM) $(ROM) $(ELF) $(MAP) $(OBJS) $(C_ASMS)

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
# swapped for raw asm.

.PHONY: report
report: $(C_OBJS)
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
                  $(C_BUILDDIR)/graphics/actor_part101.o \
                  $(C_BUILDDIR)/graphics/actor_part103.o \
                  $(C_BUILDDIR)/graphics/actor_part118.o \
                  $(C_BUILDDIR)/graphics/actor_part110.o \
                  $(C_BUILDDIR)/graphics/actor_part11b.o \
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
                  $(C_BUILDDIR)/graphics/actor_part45d.o \
                  $(C_BUILDDIR)/graphics/actor_part7.o \
                  $(C_BUILDDIR)/graphics/actor_part74.o \
                  $(C_BUILDDIR)/graphics/actor_part75.o \
                  $(C_BUILDDIR)/graphics/actor_part7b.o \
                  $(C_BUILDDIR)/graphics/actor_part81.o \
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
                  $(C_BUILDDIR)/graphics/graphics_package_1e578.o \
                  $(C_BUILDDIR)/graphics/graphics_package_1e640.o \
                  $(C_BUILDDIR)/graphics/graphics_package_1e688.o \
                  $(C_BUILDDIR)/graphics/graphics_package_1e8f8.o \
                  $(C_BUILDDIR)/graphics/graphics_package_1e964.o \
                  $(C_BUILDDIR)/graphics/hud_digit_array.o \
                  $(C_BUILDDIR)/graphics/hud_icon_widget_85c4.o \
                  $(C_BUILDDIR)/graphics/hud_icon_widget_8890.o \
                  $(C_BUILDDIR)/graphics/hud_icon_widget_8994.o \
                  $(C_BUILDDIR)/graphics/hud_stat_widget2.o \
                  $(C_BUILDDIR)/graphics/hud_stat_widget3.o \
                  $(C_BUILDDIR)/graphics/level_graphics.o \
                  $(C_BUILDDIR)/graphics/settings_menu.o \
                  $(C_BUILDDIR)/graphics/settings_menu10.o \
                  $(C_BUILDDIR)/graphics/settings_menu22.o \
                  $(C_BUILDDIR)/graphics/settings_menu6.o \
                  $(C_BUILDDIR)/graphics/trigger_effect.o \
                  $(C_BUILDDIR)/system/bg_scroll_layer_25fc8.o \
                  $(C_BUILDDIR)/system/game_loop14.o \
                  $(C_BUILDDIR)/system/game_loop16.o \
                  $(C_BUILDDIR)/system/game_loop29.o \
                  $(C_BUILDDIR)/system/game_loop3.o \
                  $(C_BUILDDIR)/system/game_loop32.o \
                  $(C_BUILDDIR)/system/game_loop37.o \
                  $(C_BUILDDIR)/system/game_loop4.o \
                  $(C_BUILDDIR)/system/game_loop40.o \
                  $(C_BUILDDIR)/system/game_loop42.o \
                  $(C_BUILDDIR)/system/game_loop46.o \
                  $(C_BUILDDIR)/system/game_loop48.o \
                  $(C_BUILDDIR)/system/game_loop49.o \
                  $(C_BUILDDIR)/system/game_loop51.o \
                  $(C_BUILDDIR)/system/game_loop52.o \
                  $(C_BUILDDIR)/system/game_loop53.o \
                  $(C_BUILDDIR)/system/game_loop54.o \
                  $(C_BUILDDIR)/system/game_loop57.o \
                  $(C_BUILDDIR)/system/game_loop7.o \
                  $(C_BUILDDIR)/system/game_loop8.o \
                  $(C_BUILDDIR)/system/link_cable.o
$(OLD_AGBCC_OBJS): CC1 := $(CC1_OLD)
$(OLD_AGBCC_OBJS): CC1FLAGS := $(filter-out -fprologue-bugfix,$(CC1FLAGS))

# Objects built with -fno-strength-reduce on top of their compiler's -O2.
# graphics_loading_35d1c: sub_8036600's first loop keeps its up-counting
# `i` (with strength reduction on, gcc reverses a loop whose counter only
# feeds the exit test), and the flag leaves every other real-C function in
# the file byte-identical. It is NOT a global property: adding it to all
# old_agbcc objects breaks matched functions in 11 other files, and
# sub_80358A8 (graphics_loading_35780.o, split off for this reason) needs
# strength reduction ON: its up-counting inner loop must be reversed. See
# docs/matching/per-file-flags-investigation.md and
# docs/matching/issue-64-65-naked-retry-2.md.
NO_STRENGTH_REDUCE_OBJS := $(C_BUILDDIR)/graphics/graphics_loading_35d1c.o
$(NO_STRENGTH_REDUCE_OBJS): CC1FLAGS += -fno-strength-reduce

# GAX2's bundled libgcc2.c code (__divdi3/__udivdi3/__muldi3) was built
# without -mthumb-interwork: its functions are the only ones in the ROM
# that return via a combined `pop {r4-r7, pc}`, and with the flag
# dropped they compile from libgcc2.c's own source byte-for-byte - see
# src/util/math_div64_util.c and docs/matching/gax-toolchain-retry.md.
NO_INTERWORK_OBJS := $(C_BUILDDIR)/util/math_div64_util.o
$(NO_INTERWORK_OBJS): CC1FLAGS := $(filter-out -mthumb-interwork,$(CC1FLAGS))

$(C_BUILDDIR)/%.o : $(C_SUBDIR)/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s

$(ASM_BUILDDIR)/%.o: $(ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(DATA_ASM_BUILDDIR)/%.o: $(DATA_ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(GFX):
	$(MAKE) -C tools/gbagfx