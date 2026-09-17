#include "core.h"
#include "pause_options_screen.h"

extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern void sub_8003698(struct pause_options_screen *self, s32 rowIndex);
extern void sub_80033E8(struct pause_options_screen *self, u32 flags);
extern u8 sub_8002CE8(void *handle, s32 rowIndex);
extern void sub_8002C14(void *handle, s32 rowIndex, void *buf);
extern void sub_8002C40(void *handle, s32 rowIndex, void *buf);
extern void sub_8002C6C(void *handle, s32 rowIndex);
extern s32 sub_8002BA4(void *arg0);

/* State 7's input handler: confirm/cancel-combo commits row `field_24`
 * (sub_8003698, src/graphics/settings_menu8b.c) and returns to state 0
 * if it was already the "current" row (`field_10==0`), else re-enters
 * state 5 to reselect; cancel (bit 1) re-enters state 5 too; L/R toggle
 * `field_10` between 0/1. */
void sub_800376C(struct pause_options_screen *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 0) {
            sub_8003698(self, self->field_24);
            self->state = 0;
            self->field_10 = 4;
        } else {
            self->state = 5;
            self->field_10 = self->field_24;
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        }
        return;
    }
    if (flags & 2) {
        self->state = 5;
        self->field_10 = self->field_24;
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        return;
    }
    if (flags & 0x40) {
        if (self->field_10 == 1) {
            self->field_10 = 0;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
        return;
    }
    if (flags & 0x80) {
        if (self->field_10 == 0) {
            self->field_10 = 1;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
    }
}

/* State 5's input handler: confirm/cancel-combo either resets to state
 * 0 (maxed out) or, if row `field_10` isn't already selected
 * (sub_8002CE8), enters state 9 to edit it, else commits it directly
 * (sub_8003698) and returns to state 0; cancel (bit 1) resets to state
 * 0; otherwise falls through to the shared L/R cursor mover. */
void sub_8003824(struct pause_options_screen *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 4) {
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
            self->state = 0;
            self->field_10 = 2;
            return;
        }
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        if (!sub_8002CE8(self->field_8c, self->field_10)) {
            self->state = 9;
            self->field_24 = self->field_10;
            self->field_10 = 0;
        } else {
            sub_8003698(self, self->field_10);
            self->state = 0;
            self->field_10 = 4;
        }
        return;
    }
    if (flags & 2) {
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        self->state = 0;
        self->field_10 = 2;
        return;
    }
    sub_80033E8(self, flags);
}

/* State 6's input handler - same shape as sub_8003824 above, a
 * different row-selection sub-menu (state 7 on confirm-when-unselected,
 * field_10 target value 3 rather than 2). */
void sub_80038D0(struct pause_options_screen *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 4) {
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
            self->state = 0;
            self->field_10 = 3;
            return;
        }
        if (sub_8002CE8(self->field_8c, self->field_10)) {
            PlaySfx(gUnknown_030012BC, 0x48, 0x100);
            return;
        }
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        self->state = 7;
        self->field_24 = self->field_10;
        self->field_10 = 0;
        return;
    }
    if (flags & 2) {
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        self->state = 0;
        self->field_10 = 3;
        return;
    }
    sub_80033E8(self, flags);
}

/* State 9's input handler: confirm/cancel-combo commits row `field_24`
 * unconditionally (sub_8002C14+sub_8002C6C+optional sub_8002C40) then
 * settles at state 0; cancel (bit 1) re-enters state 6; L/R toggle
 * `field_10` between 0/1. */
void sub_800397C(struct pause_options_screen *self, u32 flags)
{
    u8 buf[0x70];

    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 0) {
            s32 rowIndex = self->field_24;
            void *handle = self->field_8c;

            sub_8002C14(handle, rowIndex, buf);
            handle = self->field_8c;
            sub_8002C6C(handle, rowIndex);
            handle = self->field_8c;
            if (sub_8002BA4(handle)) {
                handle = self->field_8c;
                sub_8002C40(handle, rowIndex, buf);
            }
            self->state = 0;
            self->field_10 = 4;
        } else {
            self->state = 6;
            self->field_10 = self->field_24;
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        }
        return;
    }
    if (flags & 2) {
        self->state = 6;
        self->field_10 = self->field_24;
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        return;
    }
    if (flags & 0x40) {
        if (self->field_10 == 1) {
            self->field_10 = 0;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
        return;
    }
    if (flags & 0x80) {
        if (self->field_10 == 0) {
            self->field_10 = 1;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
    }
}

#include "icon_manager.h"

extern void *gUnknown_030012DC;
extern s32 sub_8028A30(void *mgr, s32 arg1);
extern s32 sub_8004A50(struct pause_options_screen *self);
extern s32 sub_8026F38(s32 arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_8003F30(struct pause_options_screen *self, s32 label1, s32 label2, s32 rowIdx, u8 flag);
extern s32 gStaticData_0816B1BC[];

/* Draws the 5-entry state-select sub-menu label list (sub_80032E8's
 * `field_10` states, gStaticData_0816B1BC's label table) into
 * gUnknown_030012DC, highlighting whichever row matches `field_10`,
 * then draws a final fixed label via the still-raw sub_8003F30. Same
 * measure-then-draw icon shape as sub_80049CC
 * (src/graphics/settings_menu.c, parked) - see that function's doc
 * comment for the class of gcc register-allocation quirk this may hit
 * too. */
#if NON_MATCHING
/* Reconstructed (semantics fully understood) but NOT YET
 * BYTE-MATCHING: the same measure-then-draw icon shape as
 * sub_80049CC (src/graphics/settings_menu.c, parked) hits the same
 * unresolved class of gcc-2.9 register-allocation nondeterminism
 * documented there - attempts here to pin a cached `&gUnknown_030012DC`
 * address (matching the technique that worked for sub_800306C/
 * sub_800312C/sub_80031E4 elsewhere in this chunk) ran into the pinned
 * loop counter colliding with a compiler-hoisted constant in the same
 * register, and further pin juggling didn't converge. Real bytes stay
 * in asm/code_3_1_10_3_3a60.s, wrapped `.if NON_MATCHING == 0`. */
void sub_8003A60(struct pause_options_screen *self)
{
    s32 i;
    s32 y = 0x64;

    for (i = 0; i <= 4; i++) {
        struct icon_manager *mgr = gUnknown_030012DC;
        struct icon_record *rec;
        s16 off;
        s32 label = gStaticData_0816B1BC[i];
        s32 str;
        s32 width;

        if (self->field_10 == i) {
            sub_8028A30(mgr, sub_8004A50(self));
        } else {
            sub_8028A30(mgr, 0);
        }

        mgr = gUnknown_030012DC;
        rec = mgr->record;
        off = rec->slots[0].offset;
        str = sub_8026F38(label);
        width = sub_803AD80((u8 *)mgr + off, (void *)str, rec->slots[0].ptr);

        mgr = gUnknown_030012DC;
        mgr->posX = (0xf0 - width) >> 1;
        mgr->posY = y;
        rec = mgr->record;
        off = rec->slots[2].offset;
        str = sub_8026F38(label);
        sub_803AD80((u8 *)mgr + off, (void *)str, rec->slots[2].ptr);

        y += 0xa;
    }

    sub_8003F30(self, 0x5a, 0x21, 0, 0);
}
#endif /* NON_MATCHING */
