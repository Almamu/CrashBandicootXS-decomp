#include "core.h"

extern s32 sub_80001CC(s32);                               /* extern */
extern void sub_8000518();                                    /* extern */
extern void sub_80005D0();                                    /* extern */
extern u32 irq_setup();                                  /* extern */
extern void sub_8000620();                                    /* extern */
extern s32 sub_8026EEC();                                  /* extern */

s32 AgbMain(void) {
    // setup cart
    REG_WAITCNT = WAITCNT_WS0_N_3 | WAITCNT_WS0_S_1 | WAITCNT_PREFETCH_ENABLE;
    // setup display configuration, also updates REG_ADDR_BLDALPHA to 0
    *(vu32 *) REG_ADDR_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_BLDY = 0x10;
    REG_DISPCNT = DISPCNT_MODE_0;

    if (sub_80001CC(0x400) != 0 || irq_setup() != 0) {
        return -1;
    }
    
    sub_8000620();
    
    if (sub_8026EEC() != 0) {
        return -1;
    }
    
    sub_80005D0();
    sub_8000518();
    
    return 0;
}
