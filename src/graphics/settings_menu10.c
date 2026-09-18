#include "core.h"

#if NON_MATCHING
/* Reconstructed (semantics understood) but NOT YET BYTE-MATCHING -
 * parked the same way `sub_8006600` (src/graphics/oam_count.c) and
 * `src/graphics/settings_menu9.c`'s parked quartet are. The loop/mask
 * register scaffolding never lands in the exact scratch registers
 * (`r7`/`r8`) the ROM's own allocator reaches for, no matter how the
 * source is rephrased - the same class of gcc-2.9 register-allocation
 * difficulty documented at length for `sub_8006600`. See
 * docs/matching/issue-8-0x080060ac-overlay-ui.md. */

/* The same small per-widget object `src/graphics/oam_count.c` already
 * names `struct sub_8006700_actor` (redeclared locally here per this
 * project's minimal-local-type convention for a type already anchored
 * in another translation unit - see e.g. settings_menu6.c's own
 * `struct threshold_table_entry` comment). Steps `field_24`'s low 5
 * bits down to 0 (redrawing/committing every step via
 * sub_8006600/sub_8006714/sub_8006700), then polls input
 * (`sub_80007AC`/`gUnknown_030007E0.pressed`) redrawing every frame
 * until the confirm button is newly pressed, then steps `field_24`
 * back up to 0x10 the same way, and finally forces `field_28` to
 * `0x40` and re-applies. */
struct sub_8006700_actor {
    u8 unused_00[0x10];
    s32 field_10;
    void *field_14;
    void *field_18;
    u32 field_1c;
    u32 field_20;
    u8 field_24;
    u8 unused_25[3];
    u16 field_28;
};

extern void sub_8006600(struct sub_8006700_actor *arg0);
extern void sub_8006714(struct sub_8006700_actor *arg0);
extern void sub_8006700(struct sub_8006700_actor *arg0);
extern void sub_80007AC(void *arg0);
extern void *gUnknown_03001304;

struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gUnknown_030007E0;

void sub_8006518(struct sub_8006700_actor *self)
{
    u8 byte;
    s32 low;

    if ((self->field_24 & 0x1f) != 0) {
        do {
            byte = self->field_24;
            low = (u32)(byte << 27) >> 27;
            low -= 1;
            low &= 0x1f;
            self->field_24 = (byte & -0x20) | low;
            sub_8006600(self);
            sub_8006714(self);
            sub_8006700(self);
        } while ((self->field_24 & 0x1f) != 0);
    }

    do {
        sub_8006600(self);
        sub_8006714(self);
        sub_8006700(self);
        sub_80007AC(gUnknown_03001304);
    } while (!(gUnknown_030007E0.pressed & 8));

    if ((self->field_24 & 0x1f) != 0x10) {
        do {
            byte = self->field_24;
            low = (u32)(byte << 27) >> 27;
            low += 1;
            low &= 0x1f;
            self->field_24 = (byte & -0x20) | low;
            sub_8006600(self);
            sub_8006714(self);
            sub_8006700(self);
        } while ((self->field_24 & 0x1f) != 0x10);
    }

    self->field_28 = 0x40;
    sub_8006714(self);
}
#endif /* NON_MATCHING */
