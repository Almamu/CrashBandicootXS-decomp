#include "core.h"

/* Same boss-weapon subsystem as actor_part20.c - see that file's header
 * comment and docs/matching/issue-58-0x08030334-actor.md. */

extern s32 gUnknown_0300153C;

/* Sets BG palette bank 1's last color (index 15) to either a near-white
 * flash color or a dim default, gated by bit 3 of `gUnknown_0300153C`
 * (a flags word driving this effect's per-frame look). */
void sub_803171C(void)
{
    vu16 *bank1 = (vu16 *)0x05000020;

    if (gUnknown_0300153C & 8) {
        bank1[0xf] = 0x7fff;
    } else {
        bank1[0xf] = 0x1f;
    }
}

extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);
extern s32 gUnknown_03001578;
extern u8 gStaticData_0817C378[];

/* While `gUnknown_03001578`'s DMA-refresh counter is armed, decrements
 * it and re-queues one "frame" of `gStaticData_0817C378`'s palette
 * animation strip into BG palette bank 1 - `sub_803ADB4` picks a
 * triangle-wave frame index (0-2, mirrored back down for 3-4) so the
 * animation ping-pongs. */
void sub_8031744(void)
{
    s32 v;

    if (gUnknown_03001578 == 0) {
        return;
    }
    gUnknown_03001578--;

    v = sub_803ADB4(gUnknown_03001578, 3);
    if (v > 2) {
        v = 5 - v;
    }

    QueueVramDmaTransfer(gStaticData_0817C378 + (v << 5), (void *)0x05000020, 0x20, 0x10);
}
