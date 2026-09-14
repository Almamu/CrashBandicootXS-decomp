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
# See docs/decomp_dev.md. `report` builds the two objects objdiff.json
# points at: a "target" object assembled from expected/legacy.s +
# expected/code_3.s (frozen, original ROM disassembly covering the
# entire game's code - never rebuilt from anything else) and a "base"
# object that's every currently-matched/parked src/*.c object merged
# into one. Run under `NON_MATCHING=1` so parked functions are included
# as their (possibly imperfect) C reconstruction rather than omitted or
# silently swapped for raw asm.

EXPECTED_DIR      := expected
EXPECTED_BUILDDIR := build/expected

$(shell mkdir -p $(EXPECTED_BUILDDIR))

.PHONY: report
report: $(EXPECTED_BUILDDIR)/target.o $(EXPECTED_BUILDDIR)/base_combined.o

# legacy.s covers the lower addresses (main.c/memory.c/irq.c's region)
# and must come first in this merge - patch_expected_target.py derives
# every correction's address from the merged object's own lowest
# sub_XXXXXXXX symbol, which only lines up with real ROM addresses if
# the two frozen sources are concatenated in the same order they
# actually sit in the ROM.
$(EXPECTED_BUILDDIR)/legacy.o: $(EXPECTED_DIR)/legacy.s
	$(AS) $(ASFLAGS) -o $@ $<

$(EXPECTED_BUILDDIR)/code_3.o: $(EXPECTED_DIR)/code_3.s
	$(AS) $(ASFLAGS) -o $@ $<

$(EXPECTED_BUILDDIR)/target.o: $(EXPECTED_BUILDDIR)/legacy.o $(EXPECTED_BUILDDIR)/code_3.o $(EXPECTED_DIR)/corrections.txt tools/patch_expected_target.py
	$(LD) -r -o $@ $(EXPECTED_BUILDDIR)/legacy.o $(EXPECTED_BUILDDIR)/code_3.o
	python3 tools/patch_expected_target.py $(EXPECTED_DIR)/corrections.txt $@

$(EXPECTED_BUILDDIR)/base_combined.o: $(C_OBJS)
	$(LD) -r -o $@ $(C_OBJS)

#### Recipes ####
	
$(ELF): $(OBJS) $(LDSCRIPT)
	$(LD) -T $(LDSCRIPT) -Map $(MAP) $(OBJS) tools/agbcc/lib/libgcc.a tools/agbcc/lib/libc.a -o $@

%.gba: %.elf
	$(OBJCOPY) -O binary $< $@

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