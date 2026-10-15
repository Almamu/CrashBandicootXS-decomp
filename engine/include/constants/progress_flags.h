#ifndef GUARD_CONSTANTS_PROGRESS_FLAGS_H
#define GUARD_CONSTANTS_PROGRESS_FLAGS_H

/*
 * The bits of the progress block's flags byte, `GameProgress::flags`
 * (+0x02; game_progress.hpp, docs/flags/progress.md): the four coloured
 * gems and the four powers. Saved with the rest of the block
 * (PackSaveData); the level's own bits are `levels[]`'s (level_flags.h).
 *
 * - Bits 0-3, the coloured gems: Player::HandleEvent sets them on
 *   EVENT_RED_GEM, EVENT_YELLOW_GEM, EVENT_GREEN_GEM and EVENT_BLUE_GEM;
 *   the gem spawners (SpawnRedGem, Platform::SpawnGemRed, ...) test their
 *   own; CountGems counts them; the pause menu's gems page and the level
 *   select's gem icon show them.
 * - Bits 4-7, the powers: LevelState::GiveTurboRun, GiveSuperBodySlam,
 *   GiveTornadoSpin and GiveDoubleJump set them, HasTurboRun, ... read
 *   them, ClearPowers (UNUSED) clears them; the pause menu's powers page
 *   shows them and GetCompletionPercent counts them. The boss that gives
 *   a power also opens the next level-select page
 *   (LevelSelect::IsNextWorldOpen): page 1's next with the super body
 *   slam, page 2's with the double jump, page 3's with the tornado spin
 *   (levels.json's "worlds" "opened_by", docs/levels.md "Level-select
 *   pages").
 *
 * `PROGRESS_BIT_*` is the bit's number, for the code that shifts it out
 * (`TEST_BIT_SHR`, `TEST_BIT_SIGN`, core.h), `PROGRESS_FLAG_*` its mask.
 */

#define PROGRESS_BIT_RED_GEM 0
#define PROGRESS_BIT_YELLOW_GEM 1
#define PROGRESS_BIT_GREEN_GEM 2
#define PROGRESS_BIT_BLUE_GEM 3
#define PROGRESS_BIT_TURBO_RUN 4
#define PROGRESS_BIT_SUPER_BODY_SLAM 5
#define PROGRESS_BIT_TORNADO_SPIN 6
#define PROGRESS_BIT_DOUBLE_JUMP 7

#define PROGRESS_FLAG_RED_GEM (1 << PROGRESS_BIT_RED_GEM)
#define PROGRESS_FLAG_YELLOW_GEM (1 << PROGRESS_BIT_YELLOW_GEM)
#define PROGRESS_FLAG_GREEN_GEM (1 << PROGRESS_BIT_GREEN_GEM)
#define PROGRESS_FLAG_BLUE_GEM (1 << PROGRESS_BIT_BLUE_GEM)
#define PROGRESS_FLAG_TURBO_RUN (1 << PROGRESS_BIT_TURBO_RUN)
#define PROGRESS_FLAG_SUPER_BODY_SLAM (1 << PROGRESS_BIT_SUPER_BODY_SLAM)
#define PROGRESS_FLAG_TORNADO_SPIN (1 << PROGRESS_BIT_TORNADO_SPIN)
#define PROGRESS_FLAG_DOUBLE_JUMP (1 << PROGRESS_BIT_DOUBLE_JUMP)

#endif /* GUARD_CONSTANTS_PROGRESS_FLAGS_H */
