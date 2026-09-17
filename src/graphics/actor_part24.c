#include "core.h"

/* Same boss-weapon subsystem as actor_part20.c/actor_part23.c - see
 * actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md. */

extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 sub_8029EB4(void);
extern s32 sub_8029E98(void);
extern u8 gUnknown_03001524;
extern s32 gUnknown_03001520;
extern s32 gUnknown_03001554;
extern s32 gUnknown_0300154C;
extern s32 gUnknown_03001550;

/* If `gUnknown_03001524` (an "apply now" latch) is set, toggles
 * `BG2CNT` between two palette/priority presets (tracked by
 * `gUnknown_03001520`) and clears the latch. Either way, recomputes the
 * BG2 affine matrix (a uniform `scale` from `gUnknown_03001554` via
 * `sub_803ADB4`, offset by the screen-projection helpers
 * `sub_8029EB4`/`sub_8029E98`) so the effect stays centered while
 * zooming. */
void sub_80312C4(void)
{
    if (gUnknown_03001524 != 0) {
        if (gUnknown_03001520 == 0) {
            REG_BG2CNT = 0x5809;
        } else {
            REG_BG2CNT = 0x5909;
        }
        gUnknown_03001524 = 0;
        gUnknown_03001520 ^= 1;
    }

    {
        s32 scale = sub_803ADB4(gUnknown_03001554 << 8, 0x3c00);
        s32 dy = gUnknown_0300154C + sub_8029EB4();
        s32 dx = gUnknown_03001550 + sub_8029E98();

        REG_BG2X = 0x8000 - ((dy * scale) >> 8);
        REG_BG2Y = 0x8000 - ((dx * scale) >> 8);

        REG_BG2PA = scale;
        REG_BG2PB = 0;
        REG_BG2PC = 0;
        REG_BG2PD = scale;
    }
}
