/* The pc platform's startup and interrupt entry, in place of the GBA's
 * crt0.s and intr_main.s (platform/gba/asm/).
 *
 * crt0's job on the GBA is the CPU modes and stacks, the IWRAM image's
 * copy and AgbMain. On the host the image's code and data are ordinary
 * host code and data, and the host memory (memory.c) starts zeroed, so
 * main only runs AgbMain. */

#include "gba/gba.h"
#include "engine/sys/irq.h"
#include "engine/sys/system.h"

/* What Irq::Setup stores in INTR_VECTOR: on the GBA the IWRAM copy of
 * IntrMain. Here only its address is used; the platform's frame loop
 * calls HostIntrMain (phase 2, #1093). */
u32 IntrMain_Buffer;

/* IntrMain's job: run the handler of each IRQ that is both enabled (IE)
 * and raised (IF), in IF's bit order, and acknowledge it. Nothing calls
 * it yet: raising VBlank and VCount once a frame is phase 2. */
void HostIntrMain(void);

void HostIntrMain(void)
{
    extern irq_handler_t gIntrTable[14];
    u16 pending;
    s32 i;

    if (REG_IME == 0) {
        return;
    }
    pending = REG_IE & REG_IF;
    for (i = 0; i < 14; i++) {
        if (pending & (1 << i)) {
            REG_IF &= ~(1 << i);
            gIntrTable[i]();
        }
    }
}

int main(void)
{
    return AgbMain();
}
