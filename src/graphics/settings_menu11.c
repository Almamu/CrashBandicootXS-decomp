#include "core.h"
#include "icon_manager.h"

/* The three functions below are companion "draw a label centered on an
 * icon widget" steps. Once NAKED transcriptions; they match as plain C
 * under both compilers once the icon-manager draws are written as the
 * gcc 2.x virtual calls they are (`sub_803AD80` is `_call_via_r2`),
 * with `this` computed before the label argument - see
 * docs/matching/issue-4-6-8-naked-retry.md. */

/* Same "results" sub-region self object `settings_menu6.c`'s
 * `struct pause_screen_results` documents (`field_6c`/`field_bc`/
 * `timeBuf` all line up) - the medal-icon-widget's (`sub_8005D44`)
 * companion label draw: formats `self->timeBuf` (already filled in by
 * sub_8005D44) centered on the medal icon via the shared
 * `gUnknown_030012DC` icon manager, using the same fixed
 * `gStaticData_0816B27C` position pair sub_8005D44 itself positions
 * the icon with. */
struct pause_screen_results {
    u8 unused_00[0x2c];
    u8 buf2c[0x46 - 0x2c];
    u8 buf46[0x6c - 0x46];
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

static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* `record->slots[0]` measures `label` (returns its pixel width) and
 * `record->slots[2]` draws it at the manager's position - gcc 2.x
 * virtual calls (sub_803AD80 is `_call_via_r2`). Block/statement macros
 * so `this` is computed before the label argument, as in the ROM. */
#define MEASURE_ICON_TEXT(mgrExpr, label)                                       \
    ({                                                                          \
        struct icon_manager *_m = (mgrExpr);                                    \
        struct icon_record *_r = _m->record;                                    \
        sub_803AD80((u8 *)_m + _r->slots[0].offset, (s32)(label), _r->slots[0].ptr); \
    })
#define DRAW_ICON_TEXT(mgrExpr, label)                                          \
    {                                                                           \
        struct icon_manager *_m = (mgrExpr);                                    \
        struct icon_record *_r = _m->record;                                    \
        sub_803AD80((u8 *)_m + _r->slots[2].offset, (s32)(label), _r->slots[2].ptr); \
    }

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
    u32 w;

    if (self->field_6c)
        sub_8008890(self->field_bc, 0, 0);
    w = MEASURE_ICON_TEXT(gUnknown_030012DC, self->timeBuf);
    set_icon_mgr_pos(gUnknown_030012DC, gStaticData_0816B27C.x - (w >> 1) - 2, gStaticData_0816B27C.y - 0x23);
    DRAW_ICON_TEXT(gUnknown_030012DC, self->timeBuf);
}

/* Same self object, `sub_8005A78`'s (the `field_88` icon widget)
 * companion label draw - the "results count" pair (`buf2c`/`buf46`,
 * already formatted by `sub_8005A78` itself) centered on that icon at
 * the fixed `gStaticData_0816B1E4` position, via `sub_8005E5C`
 * (src/graphics/settings_menu16.c) that actually draws the two small
 * strings. */
extern struct icon_pos gStaticData_0816B1E4;
extern void sub_8005E5C(struct pause_screen_results *self, void *buf1, void *buf2);

void sub_800619C(struct pause_screen_results *self)
{
    sub_8008890(self->field_88, 0, 0);
    set_icon_mgr_pos(gUnknown_030012DC, gStaticData_0816B1E4.x - 0x2c, gStaticData_0816B1E4.y - 8);
    sub_8005E5C(self, self->buf2c, self->buf46);
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
    s32 label = sub_8026F38((s32)gStaticData_0816B1D0[self->field_24]);
    u32 w = MEASURE_ICON_TEXT(gUnknown_030012DC, label);

    set_icon_mgr_pos(gUnknown_030012DC, 0xc2 - (w >> 1), 0x2c);
    DRAW_ICON_TEXT(gUnknown_030012DC, label);
}
