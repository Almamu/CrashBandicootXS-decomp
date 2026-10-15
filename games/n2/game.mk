# The Crash Bandicoot 2: N-Tranced (Europe) build (GAME=n2, #1108/#1110,
# docs/n_tranced.md), included by the Makefile. For now it is the
# disassembled code (games/n2/asm/) and the rest of the ROM incbin'd from
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
ASM_SRCS := $(wildcard $(N2_DIR)/asm/*.s)
DATA_SRCS := $(wildcard $(N2_DIR)/data/*.s)
OBJS     := $(patsubst %.s,$(OBJ_DIR)/%.o,$(ASM_SRCS) $(DATA_SRCS))

.PHONY: all compare clean tidy

all: $(ROM)

compare: $(ROM)
	sha1sum -c $(N2_DIR)/checksum.sha1

clean: tidy

tidy:
	rm -f $(ROM) $(ELF) $(MAP)
	rm -rf $(OBJ_DIR)

$(ELF): $(OBJS) $(LDSCRIPT)
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) -Map $(MAP) -o $@ $(OBJS)

$(ROM): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(OBJ_DIR)/$(N2_DIR)/asm/%.o: $(N2_DIR)/asm/%.s asm/macros.inc asm/macros/function.inc
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/$(N2_DIR)/data/%.o: $(N2_DIR)/data/%.s $(BASEROM)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BASEROM):
	$(error $(BASEROM) is missing: the N-Tranced build needs the original ROM (Europe, sha1 ee9234ca15ba2caa71fad7d6af71e27b2030ae0b) in the project root)
