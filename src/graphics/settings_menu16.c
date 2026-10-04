#include "core.h"
#include "actor.h"
#include "icon_manager.h"
#include "pause_screen_results.h"

extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Draws `label1`/`label2` (a small "N/M" fraction readout - a row's
 * count over its fixed total, e.g. the icon-row helpers in
 * sub_80057E0/sub_80058C0 pass each row's formatted count/total
 * scratch buffers) on the composite pause/options screen's results
 * icons: draws `label1` at `gUnknown_030012DC`'s current position
 * (slot 2), copies that position (x-2, y unchanged) into
 * `gUnknown_030012E0` and draws a literal `/` there (slot 4), then
 * repositions `gUnknown_030012DC` to (that x-5, that y+8) and draws
 * `label2` there (slot 2). `self` is unused - the ROM never reads it
 * either.
 *
 * Matched in the last-ten pass (docs/matching/last-ten-naked-retry.md).
 * The ROM's r7 is never a pseudo's register here (the function is one
 * basic block, and local-alloc never uses the frame pointer): it is
 * reload's register for the 0x110 posX offset. Every posX/posY access is
 * a plain field access, so each offset reaches reload as a constant: the
 * second half's 0x110 goes to r6 (reload_cse copies it from r7) and
 * 0x114 to r7 (move2add's `adds r7, #4`). The second half's reads go
 * through inline getters and its new position is passed straight to the
 * inline setter, which puts both loads ahead of the `*pdc` load. */
static inline u32 get_icon_mgr_posx(struct icon_manager *m)
{
    return m->posX;
}

static inline u32 get_icon_mgr_posy(struct icon_manager *m)
{
    return m->posY;
}

static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Calls the icon manager's `record->slots[slot]` method on `label` (a
 * gcc 2.x virtual call through libgcc's `_call_via_r2`). */
#define DRAW_ICON_SLOT(mgrExpr, slot, label)                                          \
    {                                                                                 \
        struct icon_manager *_m = (mgrExpr);                                          \
        struct icon_record *_r = _m->record;                                          \
        _call_via_r2((u8 *)_m + _r->slots[slot].offset, (label), _r->slots[slot].ptr); \
    }

void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2)
{
    struct icon_manager **pdc = &gUnknown_030012DC;
    struct icon_manager **pe0;

    DRAW_ICON_SLOT(*pdc, 2, label1);
    {
        struct icon_manager *d = *pdc;
        u32 x = d->posX;
        u32 y = d->posY;

        /* Assigned here, not at the top: that keeps the
         * &gUnknown_030012E0 load after the posX/posY loads. */
        pe0 = &gUnknown_030012E0;
        set_icon_mgr_pos(*pe0, x - 2, y);
    }
    DRAW_ICON_SLOT(*pe0, 4, (void *)0x2f);
    {
        struct icon_manager *e = *pe0;

        set_icon_mgr_pos(*pdc, get_icon_mgr_posx(e) - 5, get_icon_mgr_posy(e) + 8);
    }
    DRAW_ICON_SLOT(*pdc, 2, label2);
}
