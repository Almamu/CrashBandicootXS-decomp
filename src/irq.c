#include "core.h"
#include "irq.h"

extern irq_handler_t* gUnknown_030009E8[5];
extern irq_handler_t* gUnknown_03000A20[5];
void nullsub_10();
extern u32 IntrMain_Buffer;

// TODO: PROPERLY UNDERSTAND WHAT THIS FUNCTION IS USED FOR, MIGHT BE A CALLBACK INIT FUNCTION FOR SOME TYPE OF MESSAGE
// irq_handler_clear_index maybe?
void sub_8000544(s32 interruptIndex) {
    gUnknown_030009E8[interruptIndex] = &nullsub_10;
}

// TODO: PROPERLY NAME THIS FUNCTION, SEEMS TO BE DISABLING SPECIFIC INTERRUPT HANDLING
// irq_disable_index maybe?
void sub_8000558(s32 interruptIndex) {
    irq_handler_t* tmp = gUnknown_030009E8[interruptIndex] = gUnknown_03000A20[interruptIndex];
    
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
void sub_80005A0(s32 interruptIndex, irq_handler_t* fn) {
    gUnknown_03000A20[interruptIndex] = gUnknown_030009E8[interruptIndex];
    gUnknown_030009E8[interruptIndex] = fn;
    REG_IE |= 1 << interruptIndex;
}


void irq_disable(void) {
    REG_IME = 0;
}

u32 irq_setup() {
    u32* intrbuffer = &IntrMain_Buffer;
    irq_handler_t* fn = &nullsub_10;
    irq_handler_t** dst1 = &gUnknown_03000A20;
    irq_handler_t** dst2 = &gUnknown_030009E8;
    s32 count;

    for (count = 0xD; count >= 0; count --) {
        *dst1++ = fn;
        *dst2++ = fn;
    }
    
    INTR_VECTOR = intrbuffer;
    REG_IME = 1;
    
    return 0;
}

void nullsub_10() {}

__asm__(".align 2,0");