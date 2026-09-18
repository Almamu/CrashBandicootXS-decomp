#include "core.h"
#include "actor.h"
#include "icon_manager.h"
#include "pause_screen_results.h"

extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern s32 sub_8026F38(s32 arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern struct icon_manager *gUnknown_030012DC;

/* Shows (`sub_8008890(icon, 0, 0)`) whichever of `icons8c[0..3]` has a
 * matching bit set in `self->field_10`'s byte at offset 2 (a flag byte
 * on the row-stats handle sub_8004860/sub_80048E0 - src/graphics/
 * settings_menu2.c - already fill; bits 0x20/0x80/0x40/0x10, one per
 * slot). If *none* of the four bits were set, draws a fallback
 * centered label (text id 0x3a) at a fixed position instead. */
void sub_800570C(struct pause_screen_results *self)
{
    s32 none = 1;

    {
        register u8 *p asm("r1") = (u8 *)self->field_10 + 2;
        register s32 mask asm("r0") = 0x20;
        register u8 byte asm("r1");
        byte = *p;
        mask &= byte;
        if (mask) {
            sub_8008890(self->icons8c[0], 0, 0);
            none = 0;
        }
    }
    {
        register u8 *p asm("r1") = (u8 *)self->field_10 + 2;
        register s32 mask asm("r0") = 0x80;
        register u8 byte asm("r1");
        byte = *p;
        mask &= byte;
        if (mask) {
            sub_8008890(self->icons8c[1], 0, 0);
            none = 0;
        }
    }
    {
        register u8 *p asm("r1") = (u8 *)self->field_10 + 2;
        register s32 mask asm("r0") = 0x40;
        register u8 byte asm("r1");
        byte = *p;
        mask &= byte;
        if (mask) {
            sub_8008890(self->icons8c[2], 0, 0);
            none = 0;
        }
    }
    {
        register u8 *p asm("r1") = (u8 *)self->field_10 + 2;
        register s32 mask asm("r0") = 0x10;
        register u8 byte asm("r1");
        byte = *p;
        mask &= byte;
        if (mask) {
            sub_8008890(self->icons8c[3], 0, 0);
            none = 0;
        }
    }

    if (none) {
        s32 label = sub_8026F38(0x3a);
        struct icon_record *rec = gUnknown_030012DC->record;
        u32 width = sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[0].offset, (void *)label, rec->slots[0].ptr);
        s32 halfX = 0xc2 - (width >> 1);
        struct icon_manager *mgr = gUnknown_030012DC;
        s32 y = 0x64;

        mgr->posX = halfX;
        mgr->posY = y;

        rec = gUnknown_030012DC->record;
        sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, (void *)label, rec->slots[2].ptr);
    }
}
