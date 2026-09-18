#include "core.h"
#include "icon_manager.h"

#if NON_MATCHING
/* The three functions below (sub_8006124, sub_800619C, sub_80061E8) -
 * plus sub_8006518, src/graphics/settings_menu10.c, further down this
 * ROM region past the still-raw sub_80062A8/sub_80063D8 - are
 * reconstructed (semantics understood, cross-checked against the
 * icon-manager conventions `src/graphics/oam_count.c` (`sub_8006600`,
 * parked) and `src/graphics/settings_menu6.c` (`sub_8005A78`, matched,
 * and its parked siblings) already establish) but NOT YET
 * BYTE-MATCHING - parked the same way those are. All three hit the
 * same class of gcc-2.9 register-allocation difficulty already
 * documented at length for `sub_8006600`/`sub_8005AE8` and friends:
 * the loop/self pointer and the icon-manager position-store's mask
 * register never land in the exact scratch register the ROM's own
 * allocator reaches for, no matter how the source is rephrased -
 * closing that gap would need the same kind of heavy per-call-site
 * register-pin macro work already invested in `sub_8006600`, more than
 * this pass had budget for across three more functions on top of it. */

/* Same "results" sub-region self object `settings_menu6.c`'s
 * `struct pause_screen_results` documents (`field_6c`/`field_bc`/
 * `timeBuf` all line up) - the medal-icon-widget's (`sub_8005D44`)
 * companion label draw: formats `self->timeBuf` (already filled in by
 * sub_8005D44) centered on the medal icon via the shared
 * `gUnknown_030012DC` icon manager, using the same fixed
 * `gStaticData_0816B27C` position pair sub_8005D44 itself positions
 * the icon with. */
struct pause_screen_results {
    u8 unused_00[0x6c];
    u8 field_6c;
    u8 unused_6d[0x7c - 0x6d];
    u8 timeBuf[0xc];
    void *field_88;
    u8 unused_8c[0xbc - 0x8c];
    void *field_bc;
};

extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern struct icon_manager *gUnknown_030012DC;

/* A fixed {x, y} screen-position pair, as consumed by sub_803AD80's
 * callers here - same shape settings_menu6.c's own `struct icon_pos`
 * documents (kept as a separate local type per this project's
 * minimal-local-type convention). */
struct icon_pos {
    s32 x;
    s32 y;
};
extern struct icon_pos gStaticData_0816B27C;

void sub_8006124(struct pause_screen_results *self)
{
    struct icon_manager *mgr;
    struct icon_record *record;
    s32 width;

    if (self->field_6c != 0) {
        sub_8008890(self->field_bc, 0, 0);
    }

    mgr = gUnknown_030012DC;
    record = mgr->record;
    width = sub_803AD80((u8 *)mgr + record->slots[0].offset, (s32)self->timeBuf, record->slots[0].ptr);

    mgr = gUnknown_030012DC;
    mgr->posX = gStaticData_0816B27C.x - (width >> 1) - 2;
    mgr->posY = gStaticData_0816B27C.y - 0x23;

    record = mgr->record;
    sub_803AD80((u8 *)mgr + record->slots[2].offset, (s32)self->timeBuf, record->slots[2].ptr);
}

/* Same self object, `sub_8005A78`'s (the `field_88` icon widget)
 * companion label draw - the "results count" pair (`buf2c`/`buf46`,
 * already formatted by `sub_8005A78` itself) centered on that icon at
 * the fixed `gStaticData_0816B1E4` position, via a still-raw sibling
 * (`sub_8005E5C`, GitHub issue #7) that actually draws the two small
 * strings. */
extern struct icon_pos gStaticData_0816B1E4;
extern void sub_8005E5C(struct pause_screen_results *self, void *buf1, void *buf2);

void sub_800619C(struct pause_screen_results *self)
{
    struct icon_manager *mgr;

    sub_8008890(self->field_88, 0, 0);

    mgr = gUnknown_030012DC;
    mgr->posX = gStaticData_0816B1E4.x - 0x2c;
    mgr->posY = gStaticData_0816B1E4.y - 8;

    sub_8005E5C(self, (u8 *)self + 0x2c, (u8 *)self + 0x46);
}

/* A different, still-unreconciled self object (only `field_24`, a
 * plain `s32` category index, is touched here) - draws a fixed-position
 * category/header label at (0xc2, 0x2c) via the same icon manager,
 * picking its source character from a lookup table
 * (`gStaticData_0816B1D0[self->field_24]`) fed through `sub_8026F38`
 * (the same "char code -> something sub_803AD80 can draw" conversion
 * `sub_8006600`/`sub_8005A78` already use for fixed digits like
 * `0x2e`/`0x14`). */
struct pause_screen_category_state {
    u8 unused_00[0x24];
    s32 field_24;
};

extern s32 sub_8026F38(s32 arg0);
extern void *gStaticData_0816B1D0[];

void sub_80061E8(struct pause_screen_category_state *self)
{
    struct icon_manager *mgr;
    struct icon_record *record;
    s32 charWidth;
    s32 width;

    charWidth = sub_8026F38((s32)gStaticData_0816B1D0[self->field_24]);

    mgr = gUnknown_030012DC;
    record = mgr->record;
    width = sub_803AD80((u8 *)mgr + record->slots[0].offset, charWidth, record->slots[0].ptr);

    mgr->posX = 0xc2 - (width >> 1);
    mgr->posY = 0x2c;

    record = mgr->record;
    sub_803AD80((u8 *)mgr + record->slots[2].offset, charWidth, record->slots[2].ptr);
}

#endif /* NON_MATCHING */
