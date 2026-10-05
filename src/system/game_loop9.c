#include "core.h"

extern void *gObjVramCursor;
extern void *gOamBuffer;
extern void *gCamera;
extern void *gLevelLayers;
extern struct palette_cache *gPaletteCache;
extern u8 gRoomExitRequested;

extern void UploadPaletteCache(struct palette_cache *self);
extern void ResetObjVram(struct vram_upload_cursor *self);
extern void ResetOamBuffer(struct oam_shadow_buffer *arg0);
extern void SnapCamera(void *self);
extern void ResetLevelLayers(void *self);
extern void UpdateRoomFrame(void *self);
extern void SetDispcntMode(s32 val);
extern void ShowObj(void);
extern void CommitDispcnt(void);
extern void CommitBlendRegs(void);

void ClearRoomExit(void)
{
    gRoomExitRequested = 0;
}

void RequestRoomExit(void)
{
    gRoomExitRequested = 1;
}

u8 IsRoomExitRequested(void)
{
    return gRoomExitRequested;
}

/* Level-end teardown: DMA-copies the level object's first palette word
 * into BG palette RAM entry 0, clears its first color, then re-runs the
 * same VRAM/OAM/DMA refresh pass as `UpdateRoomFrame` and the four
 * `display.c` state resets. */
void ResumeRoomAfterPause(void *self)
{
    struct dma_regs *dma;
    u32 pltt;

    UploadPaletteCache(gPaletteCache);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = *(u32 *)(*(void **)((u8 *)self + 0x18));
    pltt = PLTT;
    dma->dst = pltt;
    dma->cnt = 0x80000100;
    (void)dma->cnt;
    *(vu16 *)pltt = 0;

    ResetObjVram(gObjVramCursor);
    ResetOamBuffer(gOamBuffer);
    SnapCamera(gCamera);
    ResetLevelLayers(gLevelLayers);
    UpdateRoomFrame(self);
    SetDispcntMode(0);
    ShowObj();
    CommitDispcnt();
    CommitBlendRegs();
}

/* Flushes the vram upload cursor (`gObjVramCursor`) and OAM shadow
 * buffer (`gOamBuffer`) - the tail end shared by both
 * `UpdateRoomFrame`'s "near start of level" path and `ResumeRoomAfterPause`'s
 * level-end teardown. */
void ResetObjBuffers(void)
{
    ResetObjVram(gObjVramCursor);
    ResetOamBuffer(gOamBuffer);
}
