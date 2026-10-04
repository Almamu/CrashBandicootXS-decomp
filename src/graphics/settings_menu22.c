#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "pause_screen_results.h"
#include "memory.h"

/* sub_80057E0 + sub_80058C0: mutually address-adjacent, bracketed by
 * the already-matched sub_800570C (settings_menu18.c) before and
 * sub_800599C (settings_menu19.c) after - own object file for the
 * same reason settings_menu20.c documents. See
 * docs/matching/issue-7-0x08004d74-overlay-ui.md.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): its mask-before-ldrb
 * order shows in sub_80057E0's flag tests. */

extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern struct icon_pos gStaticData_0816B21C[];
extern void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern struct icon_manager *gUnknown_030012DC;

static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Draws `label` at the icon manager's current position through its
 * `record->slots[2]` method (a gcc 2.x virtual call; _call_via_r2 is
 * `_call_via_r2`). Kept as a block macro rather than an inline
 * function: the method's `this` must be computed before the label
 * argument, as in the ROM. */
#define DRAW_ICON_TEXT(mgrExpr, label)                                          \
    {                                                                           \
        struct icon_manager *_m = (mgrExpr);                                    \
        struct icon_record *_r = _m->record;                                    \
        _call_via_r2((u8 *)_m + _r->slots[2].offset, (label), _r->slots[2].ptr); \
    }

/* Shows whichever of `icons9c[1..4]` has a matching bit set in
 * `self->field_10`'s flag byte (bits 1/4/8/2 - a different bit set
 * than sub_800570C's, same handle), always shows `icons9c[0]`
 * unconditionally, then draws a fixed "x/28"-shaped fraction readout:
 * first `gStaticData_0816B21C[0]`'s position (offset by -0x14/-4) with
 * `self->buf2f` at a fixed slot, then repositions to (0xb4, 0x80) and
 * calls `sub_8005E5C` with `self->buf32`/`self->buf49` (the count/total
 * buffers `sub_8005B80` - src/graphics/settings_menu6.c - already fills
 * for this same icon row). */
void sub_80057E0(struct pause_screen_results *self)
{
    if (((u8 *)self->field_10)[2] & 1)
        sub_8008890(self->icons9c[1], 0, 0);
    if (((u8 *)self->field_10)[2] & 4)
        sub_8008890(self->icons9c[2], 0, 0);
    if (((u8 *)self->field_10)[2] & 8)
        sub_8008890(self->icons9c[3], 0, 0);
    if (((u8 *)self->field_10)[2] & 2)
        sub_8008890(self->icons9c[4], 0, 0);
    sub_8008890(self->icons9c[0], 0, 0);

    set_icon_mgr_pos(gUnknown_030012DC, gStaticData_0816B21C[0].x - 0x14, gStaticData_0816B21C[0].y - 4);
    DRAW_ICON_TEXT(gUnknown_030012DC, self->buf2f);
    set_icon_mgr_pos(gUnknown_030012DC, 0xb4, 0x80);
    sub_8005E5C(self, self->buf32, self->buf49);
}

extern struct icon_pos gStaticData_0816B258[];

/* Same shape as sub_80057E0 above for the `iconsB0[3]` row: shows all
 * three icons unconditionally (no per-bit gating this time), then
 * draws three fixed "x/20"-shaped fraction readouts at
 * `gStaticData_0816B258[2]/[1]/[0]`'s positions (offset -4/+0xe, same
 * pattern as sub_80057E0's single readout) with `self->buf38`/
 * `buf3b`/`buf3e`, then a final one at (0xb4, 0x80) via `sub_8005E5C`
 * with `self->buf35`/`buf4c` (the total/threshold buffers
 * `sub_8005C58` - src/graphics/settings_menu6.c - fills for this
 * row). */
void sub_80058C0(struct pause_screen_results *self)
{
    s32 i;

    for (i = 0; i <= 2; i++)
        sub_8008890(self->iconsB0[i], 0, 0);

    set_icon_mgr_pos(gUnknown_030012DC, gStaticData_0816B258[2].x - 4, gStaticData_0816B258[2].y + 0xe);
    DRAW_ICON_TEXT(gUnknown_030012DC, self->buf38);
    set_icon_mgr_pos(gUnknown_030012DC, gStaticData_0816B258[1].x - 4, gStaticData_0816B258[1].y + 0xe);
    DRAW_ICON_TEXT(gUnknown_030012DC, self->buf3b);
    set_icon_mgr_pos(gUnknown_030012DC, gStaticData_0816B258[0].x - 4, gStaticData_0816B258[0].y + 0xe);
    DRAW_ICON_TEXT(gUnknown_030012DC, self->buf3e);
    set_icon_mgr_pos(gUnknown_030012DC, 0xb4, 0x80);
    sub_8005E5C(self, self->buf35, self->buf4c);
}
