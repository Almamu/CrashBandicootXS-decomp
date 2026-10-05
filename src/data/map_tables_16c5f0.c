#include "core.h"

/*
 * ROM 0x0816C5F0-0x0816C6A4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

struct xy_pair {
    s32 x;
    s32 y;
};

/* InitZoomBg (actor_part_1cee0.c): the four slots' offsets. */
const struct xy_pair gZoomBgSlotOffsets[4] = {
    { 4, -4 },
    { -4, -4 },
    { 4, 4 },
    { -4, 4 },
};

/* Animation ids: SetLevelSelectEntryBox (actor_part_1da38.c) by kind, SetLevelSelectEntryLevel
 * by world, UpdateLevelSelectCursor (actor_part_1dfec.c). */
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
 * FitScaledSprite and DrawScaledSprite (graphics_package.c). The same
 * sizes as gObjPieceWidths/0816B2EC in another order (the shape
 * bits first). */
const s32 gObjSizeWidths[12] = {
    8, 16, 32, 64, 16, 32, 32, 64, 8, 8, 16, 32,
};
const s32 gObjSizeHeights[12] = {
    8, 16, 32, 64, 8, 8, 16, 32, 16, 32, 32, 64,
};
