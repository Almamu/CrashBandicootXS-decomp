#include "core.h"
#include "gba/io_reg.h"

extern u8 gUnknown_030013BA;
extern u8 gUnknown_030013B9;

/* Toggles the BG0 tile-set half used for the console/text plane
 * (`gUnknown_030013B9`), committing the choice to `REG_BG0CNT`, once
 * `gUnknown_030013BA` signals the previous DMA is done. Written as two
 * full, duplicated `REG_BG0CNT = ...` branches rather than a ternary -
 * this compiler emits a separate `REG_BG0CNT` literal-pool reference per
 * branch for the duplicated-store form (matching the ROM's own two
 * `.4byte 0x04000008` pool entries), while a ternary/single-store
 * collapses it to one shared pool entry and a shorter, differently
 * shaped sequence (see docs/workflow.md step 3/7). The trailing toggle
 * pins its accumulator to `r0` - this compiler otherwise canonicalizes
 * `1 ^ gUnknown_030013B9`/`gUnknown_030013B9 ^= 1` the same way
 * regardless of source operand order, loading the memory operand first;
 * the ROM loads the constant `1` first instead, so the pin forces that
 * exact order (see docs/workflow.md step 3). */
void sub_8029ADC(void)
{
    if (gUnknown_030013BA != 0) {
        register u8 toggled asm("r0");

        if (gUnknown_030013B9 != 0) {
            REG_BG0CNT = 0x5C02;
        } else {
            REG_BG0CNT = 0x5E02;
        }
        gUnknown_030013BA = 0;
        toggled = 1;
        toggled ^= gUnknown_030013B9;
        gUnknown_030013B9 = toggled;
    }
}
