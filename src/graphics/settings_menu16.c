#include "core.h"
#include "actor.h"
#include "icon_manager.h"
#include "pause_screen_results.h"

extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);

#if NON_MATCHING
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
 * NOT YET BYTE-MATCHING: semantics fully understood and cross-checked
 * against sub_80057E0/sub_80058C0's callers (docs/rom_map.md). Parked
 * the same class of gcc-2.9 register-allocation difficulty documented
 * for sub_8006600 (src/graphics/oam_count.c): the two long-lived
 * `&gUnknown_030012DC`/`&gUnknown_030012E0` address pointers and the
 * `label2` argument (all three genuinely live across the three
 * sub_803AD80 calls) always land in r8/r9/sl here, no matter how the
 * source is restructured (plain global references, cached address
 * locals, or explicit r5/r6/r8 register pins matching the ROM's own
 * choice - the last of which also introduced a stack spill the ROM
 * doesn't have). The ROM's own register choice for these three values
 * (r5/r6/r8) isn't reachable from plain C at this call depth. */
void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2)
{
    struct icon_manager **dcAddr = &gUnknown_030012DC;
    struct icon_manager **e0Addr;
    void *lbl2 = label2;
    struct icon_manager *mgr;
    struct icon_record *rec;
    s32 x, y;

    mgr = *dcAddr;
    rec = mgr->record;
    sub_803AD80((u8 *)mgr + rec->slots[2].offset, label1, rec->slots[2].ptr);

    mgr = *dcAddr;
    x = mgr->posX;
    y = mgr->posY;

    e0Addr = &gUnknown_030012E0;
    mgr = *e0Addr;
    mgr->posX = x - 2;
    mgr->posY = y;

    rec = mgr->record;
    sub_803AD80((u8 *)mgr + rec->slots[4].offset, (void *)0x2f, rec->slots[4].ptr);

    mgr = *e0Addr;
    x = mgr->posX;
    y = mgr->posY;

    mgr = *dcAddr;
    mgr->posX = x - 5;
    mgr->posY = y + 8;

    rec = mgr->record;
    sub_803AD80((u8 *)mgr + rec->slots[2].offset, lbl2, rec->slots[2].ptr);
}
#endif /* NON_MATCHING */
