#include "core.h"
#include "actor.h"
#include "icon_manager.h"
#include "pause_screen_results.h"

extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);

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
 * Still NAKED (transcribed from the ROM). The C draft under
 * NON_MATCHING is 73 halfwords off (16 bytes long) under either
 * compiler. The ROM keeps 0x130 (record) and 0x110 (posX) in r4/r7
 * across the calls but re-materializes 0x114 (posY) in the first
 * reposition, and later derives it as r7 + 4. The draft CSEs 0x114 as
 * well, which pushes label2 and the two icon-manager addresses into
 * r8-r10. */
#if NON_MATCHING
static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Calls the icon manager's `record->slots[slot]` method on `label` (a
 * gcc 2.x virtual call; sub_803AD80 is `_call_via_r2`). */
#define DRAW_ICON_SLOT(mgrExpr, slot, label)                                          \
    {                                                                                 \
        struct icon_manager *_m = (mgrExpr);                                          \
        struct icon_record *_r = _m->record;                                          \
        sub_803AD80((u8 *)_m + _r->slots[slot].offset, (label), _r->slots[slot].ptr); \
    }

void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2)
{
    u32 x, y;

    DRAW_ICON_SLOT(gUnknown_030012DC, 2, label1);
    x = gUnknown_030012DC->posX;
    y = gUnknown_030012DC->posY;
    set_icon_mgr_pos(gUnknown_030012E0, x - 2, y);
    DRAW_ICON_SLOT(gUnknown_030012E0, 4, (void *)0x2f);
    x = gUnknown_030012E0->posX;
    y = gUnknown_030012E0->posY;
    set_icon_mgr_pos(gUnknown_030012DC, x - 5, y + 8);
    DRAW_ICON_SLOT(gUnknown_030012DC, 2, label2);
}
#else
NAKED void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, r8\n\t"
    "push {r7}\n\t"
    "mov r8, r2\n\t"
    "ldr r5, 1f\n\t"
    "ldr r0, [r5]\n\t"
    "mov r4, #0x98\n\t"
    "lsl r4, r4, #1\n\t"
    "add r2, r0, r4\n\t"
    "ldr r3, [r2]\n\t"
    "mov r6, #0x20\n\t"
    "ldrsh r2, [r3, r6]\n\t"
    "add r0, r0, r2\n\t"
    "ldr r2, [r3, #0x24]\n\t"
    "bl sub_803AD80\n\t"
    "ldr r1, [r5]\n\t"
    "mov r7, #0x88\n\t"
    "lsl r7, r7, #1\n\t"
    "add r0, r1, r7\n\t"
    "ldr r2, [r0]\n\t"
    "mov r3, #0x8a\n\t"
    "lsl r3, r3, #1\n\t"
    "add r0, r1, r3\n\t"
    "ldr r3, [r0]\n\t"
    "ldr r6, 2f\n\t"
    "ldr r0, [r6]\n\t"
    "sub r2, #2\n\t"
    "add r1, r0, r7\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x8a\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "add r1, r0, r4\n\t"
    "ldr r2, [r1]\n\t"
    "mov r3, #0x30\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r2, #0x34]\n\t"
    "mov r1, #0x2f\n\t"
    "bl sub_803AD80\n\t"
    "ldr r1, [r6]\n\t"
    "add r6, r7, #0\n\t"
    "add r0, r1, r6\n\t"
    "ldr r2, [r0]\n\t"
    "add r7, #4\n\t"
    "add r0, r1, r7\n\t"
    "ldr r3, [r0]\n\t"
    "ldr r0, [r5]\n\t"
    "sub r2, #5\n\t"
    "add r3, #8\n\t"
    "add r1, r0, r6\n\t"
    "str r2, [r1]\n\t"
    "add r2, r7, #0\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "add r4, r0, r4\n\t"
    "ldr r2, [r4]\n\t"
    "mov r3, #0x20\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "mov r1, r8\n\t"
    "bl sub_803AD80\n\t"
    "pop {r3}\n\t"
    "mov r8, r3\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "1: .4byte gUnknown_030012DC\n"
    "2: .4byte gUnknown_030012E0\n"
    );
}
#endif
