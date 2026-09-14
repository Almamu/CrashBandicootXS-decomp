#include "core.h"
#include "irq.h"

struct irq_unknown1 {
    s32 unknown[8];
    char pad2[48];
}; // 0x80

extern irq_handler_t* gUnknown_030009E8[5];
extern irq_handler_t* gUnknown_03000A20[5];
extern struct irq_unknown1 gUnknown_03000A60;
void sub_8000720(void); // vblank handler?
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

void sub_8000670(s32 arg0) {
    gUnknown_03000A60.unknown[arg0] = 0;
}

s32 sub_8000680(s32 arg0) {
    s32 index = 0;

    while (index <= 7) {
        if (gUnknown_03000A60.unknown[index] == 0) {
            gUnknown_03000A60.unknown[index] = arg0;
            return index;
        }

        index ++;
    }

    return -1;
}

extern u8 gUnknown_030007DC;
extern u32 gUnknown_030007D8;
extern u32 gUnknown_03000A58;
extern u32 gUnknown_03000A5C;
extern void sub_0803A960(void);

/* arg0 is unused - the ROM never reads r0 past the prologue. */
void sub_80006A8(void *arg0)
{
    u32 *p1;
    u32 *p2;
    u32 *p3;
    u32 v1;
    u32 v2;

    if (gUnknown_030007DC != 0) {
        p1 = &gUnknown_030007D8;
        p2 = &gUnknown_03000A58;
        p3 = &gUnknown_03000A5C;
        v1 = *p1;
        v2 = *p2;
        while (v1 < v2) {
            sub_0803A960();
            v1 = *p1;
            v2 = *p2;
        }
        *p2 = v2 + *p3;
    } else {
        sub_0803A960();
    }
}

void sub_80006EC(void)
{
    gUnknown_030007DC = 0;
}

void sub_80006F8(u32 arg0)
{
    gUnknown_03000A5C = arg0;
    gUnknown_03000A58 = gUnknown_030007D8 + arg0;
    gUnknown_030007DC = 1;
}

extern u8 gUnknown_030007DD;
extern void sub_8038B68(void);
extern void sub_803AD78(void);

void sub_8000720(void)
{
    s32 *p;
    s32 i;

    if (gUnknown_030007DD != 0) {
        sub_8038B68();
    }
    p = gUnknown_03000A60.unknown;
    i = 7;
    do {
        if (*p != 0) {
            sub_803AD78();
        }
        p++;
        i--;
    } while (i >= 0);
    gUnknown_030007D8++;
}

extern u16 gUnknown_030007E0;
extern u8 gStaticData_0816A810[];

u8 sub_8000760(void)
{
    u8 idx = 0;
    if (gUnknown_030007E0 & 0x10) idx |= 8;
    if (gUnknown_030007E0 & 0x20) idx |= 4;
    if (gUnknown_030007E0 & 0x80) idx |= 2;
    if (gUnknown_030007E0 & 0x40) idx |= 1;
    return gStaticData_0816A810[idx];
}

/* Reads the raw (active-low) hardware key register, inverts it to
 * active-high, records newly-pressed bits into gUnknown_030007E2 (the
 * u16 right after gUnknown_030007E0 - read/written through pointer
 * arithmetic off gUnknown_030007E0 rather than its own extern: agbcc
 * doesn't know the two globals are adjacent and emits a second,
 * non-matching literal-pool load/store pair otherwise), updates
 * gUnknown_030007E0 to the new state, then returns 1 if the low 4 bits
 * (A/B/Select/Start) are all held - a "soft reset" combo check. All
 * four register pins below are plain caller-saved scratch (r0-r3), so
 * none of them carry the r4-r7 save/restore hazard: `addr`/`prevKeys`
 * (r2/r3) match the ROM's choice for the address/reload pair, and
 * `keysR1`/`mask` (r1/r0) match its choice for the closing mask-and-
 * compare (gcc's own unpinned allocator picks a fresh register for the
 * AND result instead of reusing r1 in place, and compares against a
 * fresh immediate instead of reusing r0's already-loaded 0xF). The
 * inline `add %0,%1,#0` anchors a copy of `keys` into a scratch value
 * gcc would otherwise schedule after the `prevKeys` reload instead of
 * before it, despite neither having a data dependency on the other. */
s32 sub_80007AC(void)
{
    u16 keys;
    u16 keysCopy;
    register u16 *addr asm("r2");
    register u16 prevKeys asm("r3");
    register u16 keysR1 asm("r1");
    register s32 mask asm("r0");

    keys = (u16)~REG_KEYINPUT;
    addr = &gUnknown_030007E0;
    asm volatile("add %0, %1, #0" : "=r"(keysCopy) : "r"(keys));
    prevKeys = *addr;
    *(u16 *)((u8 *)addr + 2) = keysCopy & ~prevKeys;
    *addr = keys;
    keysR1 = keys;
    mask = 0xF;
    keysR1 &= mask;
    if (mask == keysR1) {
        return 1;
    }
    return 0;
}

void sub_80007DC(void)
{
    register u16 *addr asm("r2");
    register u16 zero asm("r1");

    addr = &gUnknown_030007E0;
    zero = 0;
    *addr = zero;
    *(u16 *)((u8 *)addr + 2) = zero;
}

__asm__(".align 2,0");