#include "core.h"

extern void *gUnknown_030012FC;
extern void *gUnknown_03001300;
extern void *gUnknown_030012D4;
extern void *gUnknown_03001308;
extern struct tile_asset_cache *gUnknown_030012B8;
extern u8 gUnknown_03000830;

extern void sub_8006DC8(struct tile_asset_cache *self);
extern void sub_8006C4C(struct vram_upload_cursor *self);
extern void sub_8006A90(struct oam_shadow_buffer *arg0);
extern void sub_8026DFC(void *self);
extern void sub_8026984(void *self);
extern void sub_802400C(void *self);
extern void sub_8001524(s32 val);
extern void sub_80015E0(void);
extern void sub_8001614(void);
extern void sub_8001624(void);

void sub_8024198(void)
{
    gUnknown_03000830 = 0;
}

void sub_80241A4(void)
{
    gUnknown_03000830 = 1;
}

u8 sub_80241B0(void)
{
    return gUnknown_03000830;
}

/* Level-end teardown: DMA-copies the level object's first palette word
 * into BG palette RAM entry 0, clears its first color, then re-runs the
 * same VRAM/OAM/DMA refresh pass as `sub_802400C` and the four
 * `fade_screen_mode2.c` state resets. */
void sub_80241BC(void *self)
{
    struct dma_regs *dma;
    u32 pltt;

    sub_8006DC8(gUnknown_030012B8);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = *(u32 *)(*(void **)((u8 *)self + 0x18));
    pltt = PLTT;
    dma->dst = pltt;
    dma->cnt = 0x80000100;
    (void)dma->cnt;
    *(vu16 *)pltt = 0;

    sub_8006C4C(gUnknown_030012FC);
    sub_8006A90(gUnknown_03001300);
    sub_8026DFC(gUnknown_030012D4);
    sub_8026984(gUnknown_03001308);
    sub_802400C(self);
    sub_8001524(0);
    sub_80015E0();
    sub_8001614();
    sub_8001624();
}

/* Flushes the vram upload cursor (`gUnknown_030012FC`) and OAM shadow
 * buffer (`gUnknown_03001300`) - the tail end shared by both
 * `sub_802400C`'s "near start of level" path and `sub_80241BC`'s
 * level-end teardown. */
void sub_802423C(void)
{
    sub_8006C4C(gUnknown_030012FC);
    sub_8006A90(gUnknown_03001300);
}
