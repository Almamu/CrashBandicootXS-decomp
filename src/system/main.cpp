extern "C" {
#include "core.h"
#include "system.h"
}


s32 AgbMain(void)
{
    // setup cart
    REG_WAITCNT = WAITCNT_WS0_N_3 | WAITCNT_WS0_S_1 | WAITCNT_PREFETCH_ENABLE;
    // setup display configuration, also updates REG_ADDR_BLDALPHA to 0
    *(vu32 *)REG_ADDR_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_BLDY = 0x10;
    REG_DISPCNT = DISPCNT_MODE_0;

    if (mem_heap_init(0x400) != 0 || IrqSetup() != 0) {
        return -1;
    }

    EnableVBlankHandler();

    if (MainLoop() != 0) {
        return -1;
    }

    IrqDisable();
    mem_heap_shutdown();

    return 0;
}
