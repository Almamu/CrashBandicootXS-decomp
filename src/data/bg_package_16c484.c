#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0816C484-0x0816C498. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_0861BADC[];
extern const u8 gStaticData_0861C30C[];
extern const u8 gStaticData_0862FB24[];

/* BG0 graphics package shared by sub_80047F8 (settings_menu2.c),
 * sub_80374D0 (counter_selector_setup.c), sub_801BAF0
 * (actor_part_1b85c.c) and settings_menu13.c. */
const struct bg_package gStaticData_0816C484 = {
    0x20,
    0x14,
    (void *)gStaticData_0861BADC,
    (void *)gStaticData_0861C30C,
    (void *)gStaticData_0862FB24,
};
