#include "core.h"
#include "gba/io_reg.h"

/* Hardware sound-register reset for GAX2's Direct Sound A output: briefly
 * re-arms then disarms DMA1CNT_H (a real hardware settle delay between the
 * two writes, not padding - GAS's default encoding of a textually-identical
 * "adds r3, r3, #0" picks the 2-operand immediate form (0x3300) instead of
 * the ROM's 3-operand form (bytes 0x1b, 0x1c), so it has to be forced via
 * raw .byte, exactly as already flagged in-source in the raw disassembly
 * this replaces), resets DMA1's word count/control, disables SOUNDCNT_X,
 * sets SOUNDCNT_H for Direct Sound A on timer0 (0x0B04), flushes FIFO_A (8
 * zero halfwords - same idiom as sub_8038C50 in gax_dma_control.c),
 * writes SOUNDBIAS_H, and points DMA1's destination register at FIFO_A. */
void sub_80384DC(void)
{
    vu16 *fifo;
    u16 zero;
    s32 i;

    REG_DMA1CNT_H = 0x8640;
    asm(".byte 0x1b, 0x1c\n\t"
        "mov r8, r8\n\t"
        "mov r8, r8\n\t"
        "mov r8, r8");
    REG_DMA1CNT_H = 0xc8 << 3;
    REG_DMA1CNT = 4;
    REG_SOUNDCNT_X = 0;
    REG_SOUNDCNT_H = 0x0B04;
    fifo = (vu16 *)REG_ADDR_FIFO_A;
    zero = 0;
    for (i = 7; i >= 0; i--) {
        *fifo = zero;
    }
    REG_SOUNDBIAS_H = 0x42;
    REG_DMA1DAD = REG_ADDR_FIFO_A;
}
