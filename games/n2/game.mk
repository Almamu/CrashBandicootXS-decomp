# The Crash Bandicoot 2: N-Tranced (Europe) build (GAME=n2, #1108/#1110,
# docs/n_tranced.md), included by the Makefile. The ROM is the
# disassembled code (games/n2/asm/), the sources listed in
# games/n2/objects.txt linked in its place (built with compile.mk's rules
# and per-object flags, as for XS), and the rest of the ROM incbin'd from
# the original, which must be in the project root as baserom_n2.gba.
#
# The sources are XS's (engine/src/), shared, with N-Tranced's own on top (#1109,
# docs/n_tranced.md, "Sharing sources"): games/n2/src/<path>, when it
# exists, replaces engine/src/<path> in this build, and games/n2/include/ comes
# before engine/include/.

CPPFLAGS += -D GAME_N2=1
ASFLAGS += --defsym GAME_N2=1

OBJ_DIR  := build/ntranced
ROM      := ntranced.gba
ELF      := $(ROM:.gba=.elf)
MAP      := $(ROM:.gba=.map)
LDSCRIPT := games/n2/ldscript.txt
BASEROM  := baserom_n2.gba
N2_DIR   := games/n2

# N-Tranced's own sources and headers, over the shared ones
N2_SRC   := $(N2_DIR)/src
CPPFLAGS := -iquote $(N2_DIR)/include $(CPPFLAGS)

# compile.mk's directories, under build/ntranced/
C_SUBDIR          := engine/src
C_BUILDDIR        := $(OBJ_DIR)/src
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

# The sources (games/n2/objects.txt's third column), each as its engine/src/ path:
# games/n2/src/<path> builds the same object as engine/src/<path> would
SHARED_SRCS := $(patsubst $(N2_SRC)/%,$(C_SUBDIR)/%,$(shell sed -e 's/\#.*//' $(N2_DIR)/objects.txt | awk 'NF { print $$3 }'))
# and N2_TRY_SRCS, sources tools/nt_compare/n2_objects.py builds as if
# they were in objects.txt (the same compiler and flags), unlinked
SHARED_SRCS += $(N2_TRY_SRCS)
CXX_OBJS    := $(patsubst $(C_SUBDIR)/%.cpp,$(C_BUILDDIR)/%.o,$(filter %.cpp,$(SHARED_SRCS)))
C_OBJS      := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.o,$(filter $(C_SUBDIR)/%.c,$(SHARED_SRCS)))
# lib/'s (compile.mk's lib rules, as for XS)
LIB_C_OBJS  := $(patsubst $(LIB_SUBDIR)/%.c,$(LIB_BUILDDIR)/%.o,$(filter $(LIB_SUBDIR)/%.c,$(SHARED_SRCS)))
# libgcc's, which objects.txt names by their object (lib/libgcc/<function>.o):
# one source each builds several (compile.mk's rules)
LIBGCC2_OBJS   := $(patsubst %,$(LIB_BUILDDIR)/libgcc/%.o,$(filter $(LIBGCC2_FUNCS),$(patsubst $(LIB_SUBDIR)/libgcc/%.o,%,$(SHARED_SRCS))))
LIB1FUNCS_OBJS := $(patsubst %,$(LIB_BUILDDIR)/libgcc/%.o,$(filter $(LIB1FUNCS),$(patsubst $(LIB_SUBDIR)/libgcc/%.o,%,$(SHARED_SRCS))))

N2_ASM_OBJS := $(patsubst %.s,$(OBJ_DIR)/%.o,$(wildcard $(N2_DIR)/asm/*.s $(N2_DIR)/data/*.s))
OBJS        := $(N2_ASM_OBJS) $(CXX_OBJS) $(C_OBJS) $(LIB_C_OBJS) $(LIBGCC2_OBJS) $(LIB1FUNCS_OBJS)
DEPS        := $(CXX_OBJS:.o=.d) $(C_OBJS:.o=.d) $(LIB_C_OBJS:.o=.d) $(LIBGCC2_OBJS:.o=.d)

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
# And with agbcp without -fprologue-bugfix (N2_NO_PROLOGUE_BUGFIX_OBJS too),
# its registers agbcp's, a leaf function's push {lr} as without the
# bugfix: most of XS's old_agbcp objects, and some of its agbcp ones
# (fixed_math.o, entity_bitmap.o), but not every object
# (tools/nt_compare/n2_objects.py --agbcp tells).
N2_AGBCP_NO_BUGFIX_OBJS := $(C_BUILDDIR)/engine/gfx/fade.o $(C_BUILDDIR)/engine/gfx/oam_buffer.o \
	$(C_BUILDDIR)/engine/gfx/palette_cycle.o $(C_BUILDDIR)/game/level/tile_slot_pool.o \
	$(C_BUILDDIR)/engine/util/fixed_math.o $(C_BUILDDIR)/game/crates/crate_list_update.o \
	$(C_BUILDDIR)/game/level/bg_layer.o $(C_BUILDDIR)/game/level/entity_bitmap.o \
	$(C_BUILDDIR)/game/level/pooled_bg_layer.o
# and N2_TRY_AGBCP, objects tools/nt_compare/n2_objects.py --agbcp tries so
N2_AGBCP_NO_BUGFIX_OBJS += $(N2_TRY_AGBCP)
NEW_COMPILER_OBJS := $(C_BUILDDIR)/engine/sys/vram_dma_queue.o $(N2_AGBCP_NO_BUGFIX_OBJS)

# N-Tranced's copy of a source, games/n2/src/<path>.cpp, over engine/src/<path>.cpp:
# the same object, so compile.mk's per-object flags apply to it too. Before
# compile.mk's rule, which make would otherwise pick (the first pattern
# rule whose source exists).
$(C_BUILDDIR)/%.o : $(N2_SRC)/%.cpp cxx_symbols.txt
	@mkdir -p $(dir $@)
	$(CPP) -x c++ $(CPPFLAGS) $(DEPFLAGS) $< | $(CXX1) -quiet $(CC1FLAGS) -o $(C_BUILDDIR)/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s
	$(OBJCOPY) --redefine-syms=$(CXX_SYMBOLS) $@

include compile.mk

# Objects N-Tranced's build compiled with agbcp but without
# -fprologue-bugfix: a leaf function still pushes lr and returns with
# pop {r0}; bx r0 (line_step.o's StepBresenhamLine, 0x08001294), and
# N2_AGBCP_NO_BUGFIX_OBJS (above).
N2_NO_PROLOGUE_BUGFIX_OBJS := $(C_BUILDDIR)/engine/util/line_step.o $(N2_AGBCP_NO_BUGFIX_OBJS)
$(N2_NO_PROLOGUE_BUGFIX_OBJS): CC1FLAGS := $(filter-out -fprologue-bugfix,$(CC1FLAGS))

include $(N2_DIR)/levels.mk
include $(N2_DIR)/graphics.mk
include $(N2_DIR)/text.mk

$(GFX):
	$(MAKE) -C tools/gbagfx

$(OBJ_DIR)/$(N2_DIR)/asm/%.o: $(N2_DIR)/asm/%.s asm/macros.inc asm/macros/function.inc
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/$(N2_DIR)/data/%.o: $(N2_DIR)/data/%.s $(BASEROM)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BASEROM):
	$(error $(BASEROM) is missing: the N-Tranced build needs the original ROM (Europe, sha1 ee9234ca15ba2caa71fad7d6af71e27b2030ae0b) in the project root)

-include $(DEPS)
