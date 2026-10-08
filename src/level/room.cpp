#include "sprite_obj.hpp"
#include "bg_layer.hpp"

extern "C" {
#include "core.h"
#include "util.h"
#include "gfx.h"
#include "level.h"
#include "level_state.h"
#include "globals.h"
}

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

/* Level-end teardown: DMA-copies the room's BG palette into BG palette
 * RAM, clears its first color, then re-runs the
 * same VRAM/OAM/DMA refresh pass as `UpdateRoomFrame` and the four
 * `display.cpp` state resets. */
void ResumeRoomAfterPause(struct level_progress *self)
{
    struct dma_regs *dma;
    u32 pltt;

    gPaletteCache->Upload();

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)self->cat->palette;
    pltt = PLTT;
    dma->dst = pltt;
    dma->cnt = 0x80000100;
    (void)dma->cnt;
    *(vu16 *)pltt = 0;

    gObjVramCursor->Reset();
    gOamBuffer->Reset();
    SnapCamera(gCamera);
    gLevelLayers->Reset();
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
    gObjVramCursor->Reset();
    gOamBuffer->Reset();
}
