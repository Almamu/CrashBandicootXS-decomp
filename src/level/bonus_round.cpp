#include "player.hpp"
#include "hud.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "actor.h"
#include <agb_syscall.h>
#include "hud.h"
#include "system.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "player.h"
}

/* Called at level start/checkpoint-restore: `arg1` selects whether to
 * accumulate this attempt's progress into the running totals
 * (`crateCount`/`wumpa`/`lives`, carrying every 100 wumpa into a life the
 * same way `TickLevelClock`'s odometer carries) or to just reset those
 * three fields back from their `savedCrateCount`/`savedWumpa`/`savedLives` "level start"
 * snapshot. Either way it re-syncs the player's stored position
 * (`checkpointX`/`checkpointY` -> `SetEntityPos`) and re-runs
 * `SetCheckpointAtPlayer`, then flushes `crateTotal` into the `gHud`
 * cache and clears the `+0xa4` busy flag.
 *
 * `wumpa`/`crateCount`/`lives` are accessed directly off `this` throughout
 * (no cached pointer) because those three offsets fit the Thumb
 * `ldr`/`str` immediate range (0-124); only the fields past that range
 * (`savedWumpa`-`crateTotal`, the checkpoint position, `checkpointFlags`) need an
 * explicit address computed into a local pointer - matching the ROM
 * exactly. Caching *all* of them in pointers (the earlier attempt here)
 * forced 3 extra always-live locals the natural allocator had to spill
 * into `r8`/`r9`/`sl`. */
void LevelState::EndBonusRound(u8 arg1)
{
    s32 *fieldbc;

    ClearSpawnAtStart();

    if (arg1 != 0) {
        s32 *fieldb4 = &savedCrateCount;

        crateCount += *fieldb4;

        {
            s32 *fieldb0 = &savedWumpa;

            wumpa += *fieldb0;
        }

        {
            s32 *fieldb8 = &savedLives;
            s32 *fieldd4 = &room.checkpointX;
            u8 *fielde0 = &room.checkpointFlags;
            s32 total;

            fieldbc = &crateTotal;

            if (wumpa > 0x63) {
                s32 carry = lives;
                s32 value = wumpa;

                do {
                    carry++;
                    value -= 100;
                } while (value > 0x63);

                wumpa = value;
                lives = carry;
            }

            total = lives + *fieldb8;
            LIMIT_MAX(total, 0x63);
            lives = total;

            SetBonusRoundDone();
            SetEntityPos(gPlayer, fieldd4[0], fieldd4[1]);
            SetCheckpointAtPlayer(*fielde0);
        }
    } else {
        crateCount = savedCrateCount;
        wumpa = savedWumpa;
        lives = savedLives;

        fieldbc = &crateTotal;
    }

    gHud->SetCrateTotal(*fieldbc);
    ClearInBonusRound();
}

/* One span of the entity bitmap copied to its checkpoint copy (CpuSet,
 * 0x40 words). The ROM loads the control word from the literal pool
 * fresh for each of the two calls (two `ldr r2, =0x04000040`): through
 * this inline the constant is the inlined body's, loaded where the call
 * is, where written twice in place gcc kept it in one callee-saved
 * register across the first call (the C needed an r2 pin; #750).
 * LevelState::SetCheckpoint (level_state.cpp) has the same helper. */
static inline void CopyBitmapSpan(void *dst, void *src)
{
    CpuSet(dst, src, CPU_SET_32BIT | 0x40);
}

/* `arg1` truncated to a byte, matching the ROM's own `lsls/lsrs #0x18`
 * parameter normalization. If `cat->kind == 3`, just
 * refreshes `checkpointCrateCount`/`checkpointSwitchPressed` (a cached frame count / a copy of the
 * `+0xa9` byte) and snapshots the first `0x68` bytes (`progress`) into
 * `checkpointData` (+0xe4). Otherwise it also stashes the camera's `{x, y}`
 * (`gPlayer`) into `checkpointX`/`checkpointY`, clears two flag
 * bytes, and syncs two spans of the `gEntityFlags` bitmap
 * (`+0x108`->`+8`, `+0x308`->`+0x208`) via the BIOS `CpuSet` wrapper -
 * reads like an end-of-level "freeze the HUD/save state" snapshot. */
void LevelState::SetCheckpointAtPlayer(u8 arg1)
{
    const struct level_room *level = room.cat;

    if (level->kind == ROOM_KIND_CATEGORY) {
        room.checkpointCrateCount = GetCrateCount();

        room.checkpointSwitchPressed = switchPressed;
        MemCopy32(&checkpointData, &progress, sizeof(GameProgress));
    } else {
        Player *player = gPlayer;
        s32 x = player->x;
        s32 y = player->y;
        LevelEntityFlags *flags;

        room.checkpointFlags = arg1;
        room.checkpointCrateCount = GetCrateCount();

        room.checkpointSwitchPressed = switchPressed;
        ClearSpawnAtStart();
        ResetDeaths();
        {
            /* Two-word field copy: a plain `room.checkpointX = x;
             * room.checkpointY = y;` re-derives the destination address from scratch
             * for the second store (`adds r0, #4` then `str r5,
             * [r0]`); the ROM instead keeps the base pointer from the
             * first store and uses its `[r0, #4]` immediate-offset
             * form. Reproduced with a local pointer and indexed
             * stores - same gotcha as `SetCrateGemPos`/`SetCheckpoint` in
             * docs/matching/archive/issue-37-game-loop-234e8.md. */
            s32 *dst = &room.checkpointX;

            dst[0] = x;
            dst[1] = y;
        }

        flags = gEntityFlags;
        CopyBitmapSpan(flags->bits0Copy, flags->bits0);
        CopyBitmapSpan(flags->bits1Copy, flags->bits1);

        MemCopy32(&checkpointData, &progress, sizeof(GameProgress));
    }
}
