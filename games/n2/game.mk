# The Crash Bandicoot 2: N-Tranced (Europe) build (GAME=n2, #1108/#1110,
# docs/n_tranced.md), included by the Makefile. The ROM is the
# disassembled code (games/n2/asm/), the shared sources listed in
# games/n2/objects.txt linked in its place (built with compile.mk's rules
# and per-object flags, as for XS), and the rest of the ROM incbin'd from
# the original, which must be in the project root as baserom_n2.gba.

CPPFLAGS += -D GAME_N2=1
ASFLAGS += --defsym GAME_N2=1

OBJ_DIR  := build/ntranced
ROM      := ntranced.gba
ELF      := $(ROM:.gba=.elf)
MAP      := $(ROM:.gba=.map)
LDSCRIPT := games/n2/ldscript.txt
BASEROM  := baserom_n2.gba
N2_DIR   := games/n2

# compile.mk's directories, under build/ntranced/
C_SUBDIR          := src
C_BUILDDIR        := $(OBJ_DIR)/$(C_SUBDIR)
LIB_SUBDIR        := lib
LIB_BUILDDIR      := $(OBJ_DIR)/lib
ASM_SUBDIR        := asm
ASM_BUILDDIR      := $(OBJ_DIR)/asm
DATA_ASM_SUBDIR   := data
DATA_ASM_BUILDDIR := $(OBJ_DIR)/data

# N-Tranced's own generated headers (its data's constants) will go here;
# XS's build/include is not on the path, so a source that needs one fails
# to build until N-Tranced has it.
GENERATED_INCLUDE_DIR := $(OBJ_DIR)/include
CPPFLAGS += -iquote $(GENERATED_INCLUDE_DIR)

# The shared sources (games/n2/objects.txt's third column)
SHARED_SRCS := $(shell sed -e 's/\#.*//' $(N2_DIR)/objects.txt | awk 'NF { print $$3 }')
CXX_OBJS    := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.o,$(filter %.cpp,$(SHARED_SRCS)))
C_OBJS      := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.o,$(filter %.c,$(SHARED_SRCS)))
LIBGCC2_OBJS   :=
LIB1FUNCS_OBJS :=

N2_ASM_OBJS := $(patsubst %.s,$(OBJ_DIR)/%.o,$(wildcard $(N2_DIR)/asm/*.s $(N2_DIR)/data/*.s))
OBJS        := $(N2_ASM_OBJS) $(CXX_OBJS) $(C_OBJS)
DEPS        := $(CXX_OBJS:.o=.d) $(C_OBJS:.o=.d)

.PHONY: all compare clean tidy

all: $(ROM)

compare: $(ROM)
	sha1sum -c $(N2_DIR)/checksum.sha1

clean: tidy

tidy:
	rm -f $(ROM) $(ELF) $(MAP)
	rm -rf $(OBJ_DIR)

$(ELF): $(OBJS) $(LDSCRIPT) $(N2_DIR)/symbols.txt
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) -Map $(MAP) -o $@ $(OBJS)

$(ROM): $(ELF)
	$(OBJCOPY) -O binary $< $@

# compile.mk's compiler choices are XS's; N-Tranced's original build didn't
# always pick the same. Objects it built with the current compiler
# (agbcp, with -fprologue-bugfix) where XS's used old_agbcp:
# - vram_dma_queue.o: FlushVramDmaQueue's `ldrh r0, [r2, #8]; lsrs r0, r0, #2`
#   is agbcp's (old_agbcp loads into r1 first, as in XS).
NEW_COMPILER_OBJS := $(C_BUILDDIR)/engine/sys/vram_dma_queue.o

include compile.mk

$(OBJ_DIR)/$(N2_DIR)/asm/%.o: $(N2_DIR)/asm/%.s asm/macros.inc asm/macros/function.inc
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/$(N2_DIR)/data/%.o: $(N2_DIR)/data/%.s $(BASEROM)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BASEROM):
	$(error $(BASEROM) is missing: the N-Tranced build needs the original ROM (Europe, sha1 ee9234ca15ba2caa71fad7d6af71e27b2030ae0b) in the project root)

-include $(DEPS)
