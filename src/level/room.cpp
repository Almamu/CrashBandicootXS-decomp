#include "display.hpp"
#include "sprite_obj.hpp"
#include "bg_layer.hpp"
#include "level_state.hpp"
#include "camera.hpp"
#include "room_exit.hpp"

extern "C" {
#include "core.h"
#include "util.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* The room exit flag (RoomExit, room_exit.hpp). */
void RoomExit::Clear()
{
    requested = 0;
}

void RoomExit::Request()
{
    requested = 1;
}

u8 RoomExit::IsRequested()
{
    return requested;
}

/* Level-end teardown: DMA-copies the room's BG palette into BG palette
 * RAM, clears its first color, then re-runs the
 * same VRAM/OAM/DMA refresh pass as `UpdateRoomFrame` and the four
 * `display.cpp` state resets. */
void LevelProgress::ResumeRoomAfterPause()
{
    gPaletteCache->Upload();

    DmaCopy16(3, cat->palette, PLTT, BG_PLTT_SIZE);
    BACKDROP_COLOR = 0;

    gObjVramCursor->Reset();
    gOamBuffer->Reset();
    gCamera->Snap();
    gLevelLayers->Reset();
    UpdateRoomFrame();
    Display::SetMode(DISPCNT_MODE_0);
    Display::ShowObj();
    Display::CommitDispcnt();
    Display::CommitBlendRegs();
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
