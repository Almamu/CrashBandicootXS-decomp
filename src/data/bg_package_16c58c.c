#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0816C58C-0x0816C5A0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_0861BD48[];
extern const u8 gStaticData_0862556C[];
extern const u8 gStaticData_0863053C[];

/* BG graphics package loaded by sub_801D7E0 (actor_part_1cee0.c). */
const struct bg_package gStaticData_0816C58C = {
    0x20,
    0x20,
    (void *)gStaticData_0861BD48,
    (void *)gStaticData_0862556C,
    (void *)gStaticData_0863053C,
};
