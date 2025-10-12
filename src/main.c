#include "core.h"

extern s32 sub_80001CC(s32);                               /* extern */
extern int sub_8000518();                                    /* extern */
extern int sub_80005D0();                                    /* extern */
extern s32 sub_80005DC();                                  /* extern */
extern int sub_8000620();                                    /* extern */
extern s32 sub_8026EEC();                                  /* extern */

s32 AgbMain(void) {
    s16* temp_r1;

    *(s16* )0x04000204 = 0x4014;
    *(s32* )0x04000050 = 0xFF;
    temp_r1 = (s32* )0x04000050 + 4;
    *temp_r1 = 0x10;
    *(temp_r1 - 0x54) = 0;
    if ((sub_80001CC(0x400) == 0) && (sub_80005DC() == 0)) {
        sub_8000620();
        if (sub_8026EEC() == 0) {
            sub_80005D0();
            sub_8000518();
            return 0;
        }
    }
    return -1;
}
