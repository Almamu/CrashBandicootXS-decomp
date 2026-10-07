#ifndef GUARD_CONSTANTS_BOSSES_H
#define GUARD_CONSTANTS_BOSSES_H

/*
 * GetBossIndex: the boss of the current level (one per world's boss
 * level, LEVEL_DINGODILE..LEVEL_NEO_CORTEX), or BOSS_NONE. The HUD shows
 * the boss's icon, animation BOSS_HUD_ANIM_BASE + boss (InitHud).
 * SpawnRedGem and friends hand over to SpawnCortexBossGem in
 * BOSS_NEO_CORTEX's level.
 */
#define BOSS_NONE -1
#define BOSS_TINY 0
#define BOSS_NEO_CORTEX 1
#define BOSS_N_GIN 2
#define BOSS_DINGODILE 3
#define BOSS_HUD_ANIM_BASE 6

#endif /* GUARD_CONSTANTS_BOSSES_H */
