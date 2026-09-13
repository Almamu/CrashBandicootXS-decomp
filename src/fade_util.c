#include "core.h"

/* Sits right after sub_8001254 (ROM 0x08001254, in src/line_util2.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

extern void sub_8000670(s32 arg0);
extern s32 gUnknown_030007F8;
extern s32 gUnknown_030007F4;

struct unk_030007E8 {
    s32 field_0;
    s32 field_4;
    u8 field_8;
};

extern struct unk_030007E8 gUnknown_030007E8;

/* A per-frame screen-brightness fade tick: every `gUnknown_030007E8`.
 * `field_0` frames, writes the next brightness step to `BLDY`
 * (`0x04000054`), counting either up or down depending on
 * `field_8`'s top bit (fading in vs. out). After 17 steps (a full
 * fade), resets both counters, briefly disables interrupts
 * (`0x04000208`, `REG_IE`) while resetting `field_0` to `-1` and
 * calling `sub_8000670` with `field_4` (presumably to kick off
 * whatever comes after the fade), then re-enables interrupts.
 * `mask`/`flag8` are pinned to r0/r1 to match the ROM's exact register
 * choice for the `& 0x80` check - the natural (unpinned) allocation
 * puts the loaded byte in r0 and the constant in r1 instead, one
 * register off. */
void sub_80012AC(void)
{
    s32 counter;

    counter = gUnknown_030007F8 + 1;
    gUnknown_030007F8 = counter;
    if (counter == gUnknown_030007E8.field_0) {
        s32 val;
        register u8 flag8 asm("r1");
        register s32 mask asm("r0");

        gUnknown_030007F8 = 0;
        mask = 0x80;
        flag8 = gUnknown_030007E8.field_8;
        if (mask & flag8) {
            *(vu16 *)0x04000054 = 16 - gUnknown_030007F4;
        } else {
            *(vu16 *)0x04000054 = gUnknown_030007F4;
        }
        val = gUnknown_030007F4 + 1;
        gUnknown_030007F4 = val;
        if (val == 0x11) {
            gUnknown_030007F4 = 0;
            gUnknown_030007F8 = 0;
            *(vu16 *)0x04000208 = 0;
            gUnknown_030007E8.field_0 = -1;
            sub_8000670(gUnknown_030007E8.field_4);
            *(vu16 *)0x04000208 = 1;
        }
    }
}
