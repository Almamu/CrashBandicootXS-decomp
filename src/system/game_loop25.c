#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `sub_801089C` right
 * before this function is left untouched raw. */

extern u8 gStaticData_0816BBAE[];

/* Trivial byte-table lookup: `gStaticData_0816BBAE[idx]`. The first
 * parameter is unused in the ROM. */
u8 sub_8010908(void *arg0, u32 idx)
{
    return gStaticData_0816BBAE[idx];
}
