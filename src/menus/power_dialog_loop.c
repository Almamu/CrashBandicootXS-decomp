#include "core.h"
#include "system.h"

/* The same small per-widget object `src/menus/power_dialog_draw.c` already
 * names `struct sub_8006700_actor` (redeclared locally here per this
 * project's minimal-local-type convention for a type already anchored
 * in another translation unit - see e.g. pause_menu_pages_init.c's own
 * `struct threshold_table_entry` comment). Steps `field_24`'s low 5
 * bits down to 0 (redrawing/committing every step via
 * DrawPowerDialog/CommitPowerDialogFrame/AnimatePowerDialog), then polls input
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
struct sub_8006700_actor {
    u8 unused_00[0x10];
    s32 field_10;
    void *field_14;
    void *field_18;
    u32 field_1c;
    u32 field_20;
    struct {
        u8 level:5;
        u8 rest:3;
    } __attribute__((packed)) field_24;
    u8 unused_25[3];
    /* Cleared as a halfword, then bit 6 of its low byte set. */
    union {
        u16 all;
        struct {
            u8 flags;
            u8 hi;
        } b;
    } field_28;
};

extern void DrawPowerDialog(struct sub_8006700_actor *arg0);
extern void CommitPowerDialogFrame(struct sub_8006700_actor *arg0);
extern void AnimatePowerDialog(struct sub_8006700_actor *arg0);
extern void *gInput;

struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gKeys;

void PowerDialogLoop(struct sub_8006700_actor *self)
{
    while (self->field_24.level != 0) {
        self->field_24.level--;
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
    while (self->field_24.level != 0x10) {
        self->field_24.level++;
        DrawPowerDialog(self);
        CommitPowerDialogFrame(self);
        AnimatePowerDialog(self);
    }
    self->field_28.all = 0;
    self->field_28.b.flags |= 0x40;
    CommitPowerDialogFrame(self);
}
