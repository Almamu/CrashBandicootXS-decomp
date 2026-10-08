#include "core.h"
#include "math_util.h"
#include "match.h"
#include "actor.h"
#include "level_state.h"
#include <agb_syscall.h>
#include "hud.h"
#include "system.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "player.h"

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
 * `wumpa`/`crateCount`/`lives` are accessed directly off `self` throughout
 * (no cached pointer) because those three offsets fit the Thumb
 * `ldr`/`str` immediate range (0-124); only the fields past that range
 * (`savedWumpa`-`crateTotal`, the checkpoint position, `checkpointFlags`) need an
 * explicit address computed into a local pointer - matching the ROM
 * exactly. Caching *all* of them in pointers (the earlier attempt here)
 * forced 3 extra always-live locals the natural allocator had to spill
 * into `r8`/`r9`/`sl`. */
void EndBonusRound(struct level_state *self, u8 arg1)
{
    s32 *fieldbc;

    ClearSpawnAtStart(self);

    if (arg1 != 0) {
        s32 *fieldb4 = &self->savedCrateCount;

        self->crateCount += *fieldb4;

        {
            s32 *fieldb0 = &self->savedWumpa;

            self->wumpa += *fieldb0;
        }

        {
            s32 *fieldb8 = &self->savedLives;
            s32 *fieldd4 = &self->room.checkpointX;
            u8 *fielde0 = &self->room.checkpointFlags;
            s32 total;

            fieldbc = &self->crateTotal;

            if (self->wumpa > 0x63) {
                s32 carry = self->lives;
                s32 value = self->wumpa;

                do {
                    carry++;
                    value -= 100;
                } while (value > 0x63);

                self->wumpa = value;
                self->lives = carry;
            }

            total = self->lives + *fieldb8;
            LIMIT_MAX(total, 0x63);
            self->lives = total;

            SetBonusRoundDone(self);
            SetEntityPos((struct actor *)gPlayer, fieldd4[0], fieldd4[1]);
            SetCheckpointAtPlayer(self, *fielde0);
        }
    } else {
        self->crateCount = self->savedCrateCount;
        self->wumpa = self->savedWumpa;
        self->lives = self->savedLives;

        fieldbc = &self->crateTotal;
    }

    SetHudCrateTotal(gHud, *fieldbc);
    ClearInBonusRound(self);
}

/* `arg1` truncated to a byte, matching the ROM's own `lsls/lsrs #0x18`
 * parameter normalization. If `self->cat->kind == 3`, just
 * refreshes `checkpointCrateCount`/`checkpointSwitchPressed` (a cached frame count / a copy of the
 * `+0xa9` byte) and snapshots the first `0x68` bytes of `self` into
 * `self+0xe4`. Otherwise it also stashes the camera's `{x, y}`
 * (`gPlayer`) into `checkpointX`/`checkpointY`, clears two flag
 * bytes, and syncs two spans of the `gEntityFlags` bitmap
 * (`+0x108`->`+8`, `+0x308`->`+0x208`) via the BIOS `CpuSet` wrapper -
 * reads like an end-of-level "freeze the HUD/save state" snapshot. */
void SetCheckpointAtPlayer(struct level_state *self, u8 arg1)
{
    const struct level_room *level = self->room.cat;

    if (level->kind == ROOM_KIND_CATEGORY) {
        self->room.checkpointCrateCount = GetCrateCount(self);

        self->room.checkpointSwitchPressed = self->switchPressed;
        MemCopy32(&self->checkpointData, &self->progress, sizeof(struct game_progress));
    } else {
        struct player *player = gPlayer;
        s32 x = player->x;
        s32 y = player->y;
        struct entity_flags *flags;

        self->room.checkpointFlags = arg1;
        self->room.checkpointCrateCount = GetCrateCount(self);

        self->room.checkpointSwitchPressed = self->switchPressed;
        ClearSpawnAtStart(self);
        ResetDeaths(self);
        {
            /* Two-word field copy: a plain `self->0xd4 = x; self->0xd8
             * = y;` re-derives the destination address from scratch
             * for the second store (`adds r0, #4` then `str r5,
             * [r0]`); the ROM instead keeps the base pointer from the
             * first store and uses its `[r0, #4]` immediate-offset
             * form. Reproduced with a local pointer and indexed
             * stores - same gotcha as `SetCrateGemPos`/`SetCheckpoint` in
             * docs/matching/archive/issue-37-game-loop-234e8.md. */
            s32 *dst = &self->room.checkpointX;

            dst[0] = x;
            dst[1] = y;
        }

        flags = gEntityFlags;
        {
            /* The ROM loads the control word from the literal pool
             * fresh for each call (two `ldr r2, =0x04000040`). Written
             * as the same constant twice, gcc keeps it in one callee-
             * saved register across the first call (SpawnRoomEntities's
             * shape, room_entities.cpp), and no spelling of the
             * constant stops that; pinning the first call's copy to its
             * argument register r2 does (the second call then needs
             * nothing). SetCheckpoint (level_state.cpp) is the same. */
            void *a = flags->bits0Copy;
            void *b = flags->bits0;
            MATCH_HOLD_REG(u32, ctrl, r2) = CPU_SET_32BIT | 0x40;

            CpuSet(a, b, ctrl);
        }
        CpuSet(flags->bits1Copy, flags->bits1, CPU_SET_32BIT | 0x40);

        MemCopy32(&self->checkpointData, &self->progress, sizeof(struct game_progress));
    }
}
