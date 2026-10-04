#include "core.h"
#include "icon_manager.h"
#include "pause_options_screen.h"

extern s32 sub_8028A30(void *mgr, s32 arg1);
extern s32 GetUiText(s32 arg0);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern struct icon_manager *gUnknown_030012E0;
extern struct icon_manager *gUnknown_030012DC;

static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Calls `record->slots[n]` on an icon manager with `label` (slot 0
 * measures and returns the pixel width, slot 2 draws) - a gcc 2.x
 * virtual call through libgcc's `_call_via_r2`. A statement macro so
 * `this` is computed before the label argument, as in the ROM. */
#define ICON_TEXT_CALL(mgrExpr, n, label)                                       \
    ({                                                                          \
        struct icon_manager *_m = (mgrExpr);                                    \
        struct icon_slot *_s = &_m->record->slots[n];                           \
        _call_via_r2((u8 *)_m + _s->offset, (void *)(label), _s->ptr);           \
    })

/* Sits right after the screen-init BG-load/per-row-stats cluster
 * (`src/graphics/settings_menu2.o`, ROM `0x080047F8`-`0x08004914`) and
 * before the settings-row flag-test/wrapper cluster
 * (`src/graphics/settings_menu3.c`, ROM `0x08004A50` onward). Both
 * functions were NAKED transcriptions until the issue #4/#6/#8 retry
 * (docs/matching/issue-4-6-8-naked-retry.md); they match as plain C
 * under both compilers. */

/* `arg1`/`arg2` are plain coordinate values here (not pointers - the
 * ROM does raw integer arithmetic on them, `arg1+0x1d`/`arg2+0xc`),
 * used as the on-screen anchor for a centered numeric glyph (label
 * 0x25) into gUnknown_030012DC.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers, the draws written as `record->slots[n]` virtual calls
 * (`ICON_TEXT_CALL`). See docs/matching/issue-4-6-8-naked-retry.md. */
void sub_8004914(struct pause_options_screen *self, s32 arg1, s32 arg2, u8 arg3)
{
    s32 x = arg1 + 0x1d;
    s32 y = arg2 + 0xc;
    s32 w;

    if (arg3)
        sub_8028A30(gUnknown_030012DC, ((self->flags >> 2) & 1) ? 1 : 2);
    else
        sub_8028A30(gUnknown_030012DC, 0);
    w = ICON_TEXT_CALL(gUnknown_030012DC, 0, GetUiText(0x25));
    set_icon_mgr_pos(gUnknown_030012DC, x - w / 2, y);
    ICON_TEXT_CALL(gUnknown_030012DC, 2, GetUiText(0x25));
}

/* Draws a centered label (from the runtime string table via
 * GetUiText) into gUnknown_030012E0's icon pair - `self` is unused.
 * Matches sub_8006600's (src/graphics/oam_count.c) centered-icon shape
 * exactly, just for a single label rather than flanking a number.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers with the same `ICON_TEXT_CALL` virtual-call macro and no
 * pins. See docs/matching/issue-4-6-8-naked-retry.md. */
void sub_80049CC(struct pause_options_screen *self, s32 labelIndex)
{
    s32 w;

    sub_8028A30(gUnknown_030012E0, 0);
    w = ICON_TEXT_CALL(gUnknown_030012E0, 0, GetUiText(labelIndex));
    set_icon_mgr_pos(gUnknown_030012E0, (0xf0 - w) >> 1, 6);
    ICON_TEXT_CALL(gUnknown_030012E0, 2, GetUiText(labelIndex));
}
