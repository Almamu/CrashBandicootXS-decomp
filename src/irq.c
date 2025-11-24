#include "core.h"

extern void* gUnknown_030009E8[5];
extern void* gUnknown_03000A20[5];
extern void nullsub_10();

// TODO: PROPERLY UNDERSTAND WHAT THIS FUNCTION IS USED FOR, MIGHT BE A CALLBACK INIT FUNCTION FOR SOME TYPE OF MESSAGE
// irq_handler_clear_index maybe?
void sub_8000544(s32 arg0) {
    gUnknown_030009E8[arg0] = &nullsub_10;
}

// TODO: PROPERLY NAME THIS FUNCTION, SEEMS TO BE DISABLING SPECIFIC INTERRUPT HANDLING
// irq_disable_index maybe?
void sub_8000558(s32 interruptIndex) {
    u32* tmp = gUnknown_030009E8[interruptIndex] = gUnknown_03000A20[interruptIndex];
    
    if (tmp == NULL) {
        u16 previousIMEvalue = REG_IME;
        REG_IME = 0; // disable IME
        REG_IE &= ~(1 << interruptIndex); // disable specific interrupt
        REG_IME = previousIMEvalue; // bring back previous IME status
    }
    
    gUnknown_03000A20[interruptIndex] = &nullsub_10;
}

// TODO: PROPERLY NAME THIS FUNCTION
// irq_enable_index or maybe irq_set_index
void sub_80005A0(s32 interruptIndex, s32 arg1) {
    gUnknown_03000A20[interruptIndex] = gUnknown_030009E8[interruptIndex];
    gUnknown_030009E8[interruptIndex] = arg1;
    REG_IE |= 1 << interruptIndex;
}
