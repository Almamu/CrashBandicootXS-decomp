#include "engine/save/save_data.hpp"
#include "engine/sys/irq.hpp"

extern "C" {
#include "core.h"
#include "engine/sys/system.h"
}

using Engine::Sys::Irq;


s32 AgbMain(void)
{
    // setup cart
    REG_WAITCNT = WAITCNT_WS0_N_3 | WAITCNT_WS0_S_1 | WAITCNT_PREFETCH_ENABLE;
    // setup display configuration, also updates REG_ADDR_BLDALPHA to 0
    *(vu32 *)REG_ADDR_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_BLDY = 0x10;
    REG_DISPCNT = DISPCNT_MODE_0;

    // the stack at the top of IWRAM, above the IWRAM heap: the ROM's 0x400
    // bytes, and what a larger save needs (SAVE_STACK_GROWTH, 0 in the ROM)
    if (mem_heap_init(0x400 + SAVE_STACK_GROWTH) != 0 || Irq::Setup() != 0) {
        return -1;
    }

    Irq::EnableVBlankHandler();

    if (MainLoop() != 0) {
        return -1;
    }

    Irq::Disable();
    mem_heap_shutdown();

    return 0;
}
