#include "core.h"
#include "util.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

extern void *gCamera;
extern void *gLevelLayers;

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
