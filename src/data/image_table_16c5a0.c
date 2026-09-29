#include "core.h"

/*
 * ROM 0x0816C5A0-0x0816C5F0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_08637A70[];
extern const u8 gStaticData_086382C8[];
extern const u8 gStaticData_08638D68[];
extern const u8 gStaticData_0863945C[];
extern const u8 gStaticData_08639D18[];
extern const u8 gStaticData_0863A60C[];
extern const u8 gStaticData_0863AD90[];
extern const u8 gStaticData_0863B668[];
extern const u8 gStaticData_0863BDD4[];
extern const u8 gStaticData_0863C5E4[];
extern const u8 gStaticData_0863CF98[];
extern const u8 gStaticData_0863D01C[];
extern const u8 gStaticData_0863D0A0[];
extern const u8 gStaticData_0863D124[];
extern const u8 gStaticData_0863D1A8[];
extern const u8 gStaticData_0863D22C[];
extern const u8 gStaticData_0863D2B0[];
extern const u8 gStaticData_0863D334[];
extern const u8 gStaticData_0863D3B8[];
extern const u8 gStaticData_0863D43C[];

/* Ten {palette, tiles} tagged-asset pairs, indexed by image number:
 * sub_801DAD8 (actor_part_1da38.c, `struct image_pair`) loads both
 * through LoadTaggedAsset. */
const u8 *const gStaticData_0816C5A0[10][2] = {
    { gStaticData_0863CF98, gStaticData_08637A70 },
    { gStaticData_0863D01C, gStaticData_086382C8 },
    { gStaticData_0863D124, gStaticData_0863945C },
    { gStaticData_0863D22C, gStaticData_0863A60C },
    { gStaticData_0863D0A0, gStaticData_08638D68 },
    { gStaticData_0863D1A8, gStaticData_08639D18 },
    { gStaticData_0863D43C, gStaticData_0863C5E4 },
    { gStaticData_0863D334, gStaticData_0863B668 },
    { gStaticData_0863D3B8, gStaticData_0863BDD4 },
    { gStaticData_0863D2B0, gStaticData_0863AD90 },
};
