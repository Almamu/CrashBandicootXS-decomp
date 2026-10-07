#ifndef GUARD_CONSTANTS_LEVEL_FLAGS_H
#define GUARD_CONSTANTS_LEVEL_FLAGS_H

/*
 * The bits of a level's progress word, `level_state.levelFlags[level]`
 * (GetLevelFlags/GetCurrentLevelFlags return its address; the save menus
 * read the same words as `menu_save.levels[]`, `union level_record`).
 * It is saved with the rest of the attempt block (PackSaveData).
 *
 * - Bit 0, the crystal: set by player event 27 (the crystal pickup,
 *   PlayerHandleEvent) and, for a stage played in an actor category
 *   (room kind 3), when the level is won (UpdateGameFrame). SpawnCrystal
 *   doesn't spawn it again, IsCrystalSaved reads it and CountCrystals
 *   counts it.
 * - Bit 1, the crate gem: set when every crate is broken in an actor-
 *   category stage (AddBrokenCrate, PressSwitchCrate,
 *   CheckAllCratesBroken) and by player event 29, the gem SpawnCrateGem
 *   spawns otherwise.
 * - Bit 2, the gem-path gem: set by player event 30, the gem
 *   SpawnGemPathGem spawns.
 * - Bits 3-15, the best time-trial time in tenths of a second, 0 for
 *   none (UpdateGameFrame; struct level_save.time). The relic counters
 *   (CountSapphireRelics/CountGoldRelics/CountPlatinumRelics) compare it
 *   with the level's gLevelTable times.
 *
 * CountGems/CountClearGems count bits 1 and 2 (the clear gems); the
 * four colored gems are bits 0-3 of `level_state.flags` instead.
 */

#define LEVEL_FLAG_CRYSTAL (1 << 0)
#define LEVEL_FLAG_CRATE_GEM (1 << 1)
#define LEVEL_FLAG_GEM_PATH_GEM (1 << 2)

#define LEVEL_FLAG_TIME_SHIFT 3
#define LEVEL_FLAG_TIME_MASK 0xfff8 // bits 3-15 of the low halfword
#define LEVEL_FLAG_TIME_MAX 0x1fff  // the time is capped at this many tenths

#endif /* GUARD_CONSTANTS_LEVEL_FLAGS_H */
