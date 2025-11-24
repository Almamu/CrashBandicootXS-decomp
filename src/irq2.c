#include "core.h"

extern void nullsub_10();
extern u32 IntrMain_Buffer;

u32 irq_setup() {
    u32* intrbuffer = &IntrMain_Buffer;
    u32 data = &nullsub_10;
    u32** dst1 = (u32** )0x03000A20;
    u32** dst2 = (u32** )0x030009E8;
    s32 count = 0xD;

    for (;count >= 0;) {
        *dst1++ = data;
        *dst2++ = data;
        count --;
    }
    
    INTR_VECTOR = intrbuffer;
    REG_IME = 1;
    
    return 0;
}
