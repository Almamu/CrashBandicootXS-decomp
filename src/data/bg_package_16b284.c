#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0816B284-0x0816B298. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_0861BB04[];
extern const u8 gStaticData_0861E5F8[];
extern const u8 gStaticData_0862FFF4[];

/* BG graphics package loaded by RunPauseMenu (settings_menu15.c). */
const struct bg_package gStaticData_0816B284 = {
    0x1e,
    0x14,
    (void *)gStaticData_0861BB04,
    (void *)gStaticData_0861E5F8,
    (void *)gStaticData_0862FFF4,
};
