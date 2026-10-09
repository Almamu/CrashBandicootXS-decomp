#include "core.h"
#include "menus.h"
#include "gfx.h"

/*
 * ROM 0x0816C5F0-0x0816C6A4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* InitZoomBg (zoom_bg.cpp): the four slots' offsets. */
const struct vec2 gZoomBgSlotOffsets[4] = {
    { 4, -4 },
    { -4, -4 },
    { 4, 4 },
    { -4, 4 },
};

/* Animation ids: SetLevelSelectEntryBox (level_select_entry.cpp) by kind, SetLevelSelectEntryLevel
 * by world, UpdateLevelSelectCursor (level_select_cursor.cpp). */
const u32 gLevelSelectEntryBoxAnims[5] = {
    0, 1, 4, 3, 2,
};
const u32 gLevelSelectEntryWorldAnims[4] = {
    2, 3, 1, 4,
};
const u32 gLevelSelectCursorAnims[4] = {
    0, 1, 2, 3,
};

/* The OBJ shape/size index as width and height in pixels, as s32s:
 * FitScaledSprite and DrawScaledSprite (graphics_package.cpp). The same
 * sizes as gObjPieceWidths/0816B2EC in another order (the shape
 * bits first). */
const s32 gObjSizeWidths[12] = {
    8, 16, 32, 64, 16, 32, 32, 64, 8, 8, 16, 32,
};
const s32 gObjSizeHeights[12] = {
    8, 16, 32, 64, 8, 8, 16, 32, 16, 32, 32, 64,
};
