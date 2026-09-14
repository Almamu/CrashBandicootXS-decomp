#include "core.h"
#include "memory.h"

s32 mem_heap_init(u32);
extern void sub_8000518();                                    /* extern */
extern void IrqDisable();                                    /* extern */
extern u32 IrqSetup();                                  /* extern */
extern void sub_8000620();                                    /* extern */
extern s32 MainLoop();                                  /* extern */


s32 AgbMain(void) {
    // setup cart
    REG_WAITCNT = WAITCNT_WS0_N_3 | WAITCNT_WS0_S_1 | WAITCNT_PREFETCH_ENABLE;
    // setup display configuration, also updates REG_ADDR_BLDALPHA to 0
    *(vu32 *) REG_ADDR_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_BLDY = 0x10;
    REG_DISPCNT = DISPCNT_MODE_0;

    if (mem_heap_init(0x400) != 0 || IrqSetup() != 0) {
        return -1;
    }
    
    sub_8000620();
    
    if (MainLoop() != 0) {
        return -1;
    }
    
    IrqDisable();
    sub_8000518();
    
    return 0;
}
