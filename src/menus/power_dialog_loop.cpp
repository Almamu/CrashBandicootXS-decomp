#include "menus.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "globals.h"
}

/* PowerDialog::Loop (menus.hpp; C++ since the #664 cleanup), the power
 * dialog's fade and confirm loop: steps BLDY's level down to 0 (drawing,
 * committing and animating every step), redraws every frame until START
 * is newly pressed, steps the level back up to 0x10, and finally sets
 * REG_DISPCNT's shadow to 0x40 and commits it.
 *
 * Built with old_agbcp (Makefile OLD_AGBCC_OBJS; old_agbcc as C): it
 * loads the 0x1f mask before the byte it tests, the old compiler's
 * tell. */
void PowerDialog::Loop()
{
    while (bldy.evy != 0) {
        bldy.evy--;
        Draw();
        CommitFrame();
        Animate();
    }
    do {
        Draw();
        CommitFrame();
        Animate();
        UpdateKeys(gInput);
    } while (!(gKeys.half.pressed & START_BUTTON));
    while (bldy.evy != 0x10) {
        bldy.evy++;
        Draw();
        CommitFrame();
        Animate();
    }
    dispcnt.raw = 0;
    dispcnt.bits.objMap1D = 1;
    CommitFrame();
}
