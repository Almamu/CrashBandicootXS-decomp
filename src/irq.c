#include "core.h"
#include "irq.h"

struct irq_unknown1 {
    s32 unknown[8];
    char pad2[48];
}; // 0x80

extern irq_handler_t* gUnknown_030009E8[5];
extern irq_handler_t* gUnknown_03000A20[5];
extern struct irq_unknown1 gUnknown_03000A60;
extern irq_handler_t sub_8000720; // vblank handler?
void irq_empty_handler();
extern u32 IntrMain_Buffer;

// TODO: PROPERLY UNDERSTAND WHAT THIS FUNCTION IS USED FOR, MIGHT BE A CALLBACK INIT FUNCTION FOR SOME TYPE OF MESSAGE
// irq_handler_clear_index maybe?
void sub_8000544(s32 interruptIndex) {
    gUnknown_030009E8[interruptIndex] = &irq_empty_handler;
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
    
    gUnknown_03000A20[interruptIndex] = &irq_empty_handler;
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
    irq_handler_t* fn = &irq_empty_handler;
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

void irq_empty_handler() {}

__asm__(".align 2,0");

void sub_8000620(void) {
    irq_handler_t* fn = &sub_8000720;
    struct irq_unknown1* base = &gUnknown_03000A60;
    s32 unknown = 0;
    s32* current = &base->unknown[7];
    // this does not look right, but matches generated assembly
    u8 tmp;
    register u8* value asm("r1");
    
    do {
        *current-- = unknown;
    } while ((s32) current >= (s32) &base->unknown[0]);
    
    sub_80005A0(INTR_INDEX_VBLANK, fn);

    value = (vu8* )REG_ADDR_DISPSTAT;
    tmp = DISPSTAT_VBLANK_INTR;
    *value = tmp | *value;
}

// lcd_deactivate or something like that?
void sub_8000654(void) {
    register vu8* dispstat asm("r1") = REG_ADDR_DISPSTAT;
    u8 tmp = DISPSTAT_VBLANK_INTR;
    
    *dispstat &= ~tmp;
    
    sub_8000558(INTR_INDEX_VBLANK);
}
