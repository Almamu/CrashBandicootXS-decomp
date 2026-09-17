#include "core.h"

/* Sits right after the still-raw remainder of asm/code_3_1_6.s'
 * SIO/link-cable and overlay_ui functions and before the small
 * fade/screen-mode utility cluster this file documents - see
 * docs/rom_map.md "A fourth thing in this file". The parked
 * `sub_80014A4` right before this function stays raw in
 * asm/code_3_1_7.s (guarded by `.if NON_MATCHING == 0`), since this
 * cluster's parked functions interleave with the matched ones -
 * see docs/matching.md for the full split. */

/* `gUnknown_030007E8.field_0 != -1` - the same "idle" sentinel
 * documented on the struct in fade_util.c, exposed here as a plain
 * s32 read (this file doesn't share that struct definition, per this
 * project's per-file raw-offset convention). */
extern s32 gUnknown_030007E8;

s32 sub_8001510(void)
{
    return gUnknown_030007E8 != -1;
}
