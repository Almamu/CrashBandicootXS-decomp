#include "core.h"
#include "system.h"
#include "menus.h"
#include "globals.h"


/* The power dialog's (`struct power_dialog`, menus.h) fade/confirm
 * driver. Steps `bldy`'s low 5 bits down to 0 (redrawing/committing
 * every step via DrawPowerDialog/CommitPowerDialogFrame/
 * AnimatePowerDialog), then polls input
 * (`UpdateKeys`/`gKeys.half.pressed`) redrawing every frame
 * until the confirm button is newly pressed, then steps `bldy`
 * back up to 0x10 the same way, and finally forces `dispcnt` to
 * `0x40` and re-applies.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): it loads the 0x1f
 * mask before the byte it tests, the old compiler's tell. `bldy`
 * is a packed `level:5` bitfield, which gives the ROM's read-modify-
 * write steps without pins. See
 * docs/matching/archive/issue-4-6-8-naked-retry.md. */
void PowerDialogLoop(struct power_dialog *self)
{
    while (self->bldy.bits.level != 0) {
        self->bldy.bits.level--;
        DrawPowerDialog(self);
        CommitPowerDialogFrame(self);
        AnimatePowerDialog(self);
    }
    do {
        DrawPowerDialog(self);
        CommitPowerDialogFrame(self);
        AnimatePowerDialog(self);
        UpdateKeys(gInput);
    } while (!(gKeys.half.pressed & START_BUTTON));
    while (self->bldy.bits.level != 0x10) {
        self->bldy.bits.level++;
        DrawPowerDialog(self);
        CommitPowerDialogFrame(self);
        AnimatePowerDialog(self);
    }
    self->dispcnt.all = 0;
    self->dispcnt.b.flags |= 0x40;
    CommitPowerDialogFrame(self);
}
