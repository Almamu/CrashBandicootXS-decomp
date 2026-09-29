#include "core.h"

/*
 * ROM 0x0816C5F0-0x0816C6A4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

struct xy_pair {
    s32 x;
    s32 y;
};

/* sub_801D828 (actor_part_1cee0.c): the four slots' offsets. */
const struct xy_pair gStaticData_0816C5F0[4] = {
    { 4, -4 },
    { -4, -4 },
    { 4, 4 },
    { -4, 4 },
};

/* Animation ids: sub_801DF0C (actor_part_1da38.c) by kind, sub_801DEA4
 * by world, sub_801E190 (actor_part_1dfec.c). */
const u32 gStaticData_0816C610[5] = {
    0, 1, 4, 3, 2,
};
const u32 gStaticData_0816C624[4] = {
    2, 3, 1, 4,
};
const u32 gStaticData_0816C634[4] = {
    0, 1, 2, 3,
};

/* The OBJ shape/size index as width and height in pixels, as s32s:
 * sub_801E688 and sub_801E788 (graphics_package_1e688.c). The same
 * sizes as gStaticData_0816B2E0/0816B2EC in another order (the shape
 * bits first). */
const s32 gStaticData_0816C644[12] = {
    8, 16, 32, 64, 16, 32, 32, 64, 8, 8, 16, 32,
};
const s32 gStaticData_0816C674[12] = {
    8, 16, 32, 64, 8, 8, 16, 32, 16, 32, 32, 64,
};
