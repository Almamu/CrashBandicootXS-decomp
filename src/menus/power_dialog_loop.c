#include "core.h"
#include "system.h"
#include "menus.h"

extern void *gInput;

struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gKeys;

/* The power dialog's (`struct sub_8006700_actor`, menus.h) fade/confirm
 * driver. Steps `field_24`'s low 5 bits down to 0 (redrawing/committing
 * every step via DrawPowerDialog/CommitPowerDialogFrame/
 * AnimatePowerDialog), then polls input
 * (`UpdateKeys`/`gKeys.pressed`) redrawing every frame
 * until the confirm button is newly pressed, then steps `field_24`
 * back up to 0x10 the same way, and finally forces `field_28` to
 * `0x40` and re-applies.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): it loads the 0x1f
 * mask before the byte it tests, the old compiler's tell. `field_24`
 * is a packed `level:5` bitfield, which gives the ROM's read-modify-
 * write steps without pins. See
 * docs/matching/issue-4-6-8-naked-retry.md. */
void PowerDialogLoop(struct sub_8006700_actor *self)
{
    while (self->field_24.bits.level != 0) {
        self->field_24.bits.level--;
        DrawPowerDialog(self);
        CommitPowerDialogFrame(self);
        AnimatePowerDialog(self);
    }
    do {
        DrawPowerDialog(self);
        CommitPowerDialogFrame(self);
        AnimatePowerDialog(self);
        UpdateKeys(gInput);
    } while (!(gKeys.pressed & 8));
    while (self->field_24.bits.level != 0x10) {
        self->field_24.bits.level++;
        DrawPowerDialog(self);
        CommitPowerDialogFrame(self);
        AnimatePowerDialog(self);
    }
    self->field_28.all = 0;
    self->field_28.b.flags |= 0x40;
    CommitPowerDialogFrame(self);
}
