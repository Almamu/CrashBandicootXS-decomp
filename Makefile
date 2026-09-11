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

$(shell mkdir -p $(C_BUILDDIR) $(ASM_BUILDDIR) $(DATA_ASM_BUILDDIR))

C_SRCS := $(wildcard $(C_SUBDIR)/*.c)
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
	$(patsubst %_bitmap.png,%_bitmap.bin.lz,$(filter %_bitmap.png,$(GRAPHICS_PNGS))) \
	$(patsubst %.png,%.4bpp.lz,$(filter-out %_bitmap.png,$(GRAPHICS_PNGS))) \
	$(patsubst %.pal,%.gbapal.lz,$(GRAPHICS_PALS)) \
	$(patsubst %.bin,%.bin.lz,$(GRAPHICS_BINS))

#### Main Targets ####

compare: $(ROM)
	sha1sum -c checksum.sha1

# Every incbin in data/*.s that pulls from graphics/ needs the corresponding
# built (converted + recompressed) file to exist first.
$(DATA_ASM_OBJS): $(GRAPHICS_BUILT)

clean:
	$(RM) $(ROM) $(ELF) $(MAP) $(OBJS) $(C_ASMS)

tidy:
	rm -f $(ROM) $(ELF) $(MAP)
	rm -r build/*

graphicsclean:
	find graphics -type f \( -name '*.4bpp' -o -name '*.8bpp' -o -name '*.gbapal' -o -name '*.lz' \) -delete

#### Recipes ####
	
$(ELF): $(OBJS) $(LDSCRIPT)
	$(LD) -T $(LDSCRIPT) -Map $(MAP) $(OBJS) tools/agbcc/lib/libgcc.a tools/agbcc/lib/libc.a -o $@

%.gba: %.elf
	$(OBJCOPY) -O binary $< $@

$(C_BUILDDIR)/%.o : $(C_SUBDIR)/%.c
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s

$(ASM_BUILDDIR)/%.o: $(ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(DATA_ASM_BUILDDIR)/%.o: $(DATA_ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(GFX):
	$(MAKE) -C tools/gbagfx