#ifndef GUARD_CONSTANTS_PACKED_STATS_H
#define GUARD_CONSTANTS_PACKED_STATS_H

/*
 * The packed halfword at the start of the progress block
 * (`game_progress.packedStats`: `level_state.progress`, and `saveData`
 * in the save), which
 * PackSaveData writes from `lives`, `maskLevel` and `wumpa` and
 * UnpackSaveData reads back:
 *
 *   bits 0-6   lives      (0-99)
 *   bits 7-8   maskLevel  (MASK_LEVEL_*)
 *   bits 9-15  wumpa      (0-99)
 *
 * The getters are spelled as shifts, the way the ROM extracts the
 * fields (a mask would compile differently). UnpackSaveData reads the
 * wumpa count from the high byte instead (`byte >> 1`).
 */

#define PACKED_STATS_LIVES_MASK 0x7f
#define PACKED_STATS_MASK_LEVEL_SHIFT 7
#define PACKED_STATS_MASK_LEVEL_MASK (3 << 7)
#define PACKED_STATS_WUMPA_SHIFT 9

#define PACKED_STATS_LIVES(h) ((u32)((h) << 25) >> 25)
#define PACKED_STATS_MASK_LEVEL(h) ((u32)((h) << 23) >> 30)

#endif /* GUARD_CONSTANTS_PACKED_STATS_H */
