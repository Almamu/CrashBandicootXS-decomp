#include "core.h"
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include "hud.h"
#include "save.h"
#include "frontend.h"
#include "util.h"
#include "system.h"
#include "audio.h"
#include "menus.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

/* UpdateGameFrame - the main per-frame game-loop driver at the head of
 * the UpdateGameFrame-MainLoop cluster (GitHub issue #34,
 * docs/matching.md). Called once per frame from `MainLoop`
 * (src/system/main_loop.c) with `self` = `gLevelState`, the
 * central per-level state object every other function in this
 * cluster (`EndBonusRound`/`SetCheckpointAtPlayer`, bonus_round.c; the
 * `self+0x80`-`0xc4`/`+2` accessor family, level_state.c) also shares.
 *
 * Shape: a level-load loop (`InitTitleScreen` / `RunTitleScreen`, the
 * map screen `RunCredits` on result 2), then the level loop. Each pass
 * restores the per-attempt block from `saveData` (repeating while
 * `RunLevelSelect` asks to), runs the attempt loop (`PlayRoom` or
 * `InitActorCategory` per frame, the two bitmap ping-pongs through
 * `gEntityFlags`, the checkpoint/respawn handling), and after the
 * attempt dispatches on the level number (20-24 are the special
 * levels) and records a time-trial best time.
 *
 * Real C under old_agbcc (docs/matching/archive/big-naked-retry.md):
 * - The level loop and the attempt loop are real `for (;;)` loops.
 *   gcc rolls each one's first exit test to the end, which gives the
 *   ROM's `b` into the middle of the loop. The restore step is a `goto`
 *   loop, so it is not rotated and keeps using the hoisted `&level`.
 * - `bitmap` is assigned right before the attempt loop, which gives
 *   the ROM's `mov sb, r4` copy of `&gEntityFlags`.
 * - The best-time "unset" test reads the record as a halfword
 *   (`raw & 0xfff8`), as the ROM does, while the compare and the store
 *   go through the 13-bit field.
 * - `self->bonusPlatform = self->gemPlatform = 0` computes the 0x1b8 address
 *   first, and the `SetMaskLevel` argument starts at 2 and takes the
 *   tier only when it is <= 1.
 * - `gHud = (void *)InitHud(...)`: through the `void *` conversion the
 *   store loads the global's address before the call, as in the ROM;
 *   storing the typed result directly loads it after.
 */
/* The per-level state object (`gLevelState`) as UpdateGameFrame
 * uses it. The first 0x68 bytes are the per-attempt block that the
 * frame loop snapshots into `checkpointData`/`saveData` and restores from. */
union level_best_time {
    struct {
        u32 flag:1;  // LEVEL_FLAG_CRYSTAL
        u32 unk_1:2; // LEVEL_FLAG_CRATE_GEM, LEVEL_FLAG_GEM_PATH_GEM
        u32 time:13; // tenths of a second, capped at LEVEL_FLAG_TIME_MAX
        u32 unk_16:16;
    } f;
    u16 raw;
};

void UpdateGameFrame(struct level_state *self)
{
    vu16 zero;
    s32 best;
    s32 status;
    struct entity_flags **bitmap;

    self->unk_68 = 0;
    ResetLives(self);
    ResetWumpa(self);
    ResetCrateCount(self);
    SetUnusedAssistDeaths(self, 5);
    SetMaskAssistDeaths(self, 5);
    SetCrateAssistDeaths(self, 5);
    self->level = 0;
    self->checkpointFlags = 0;
    {
        struct dma_regs *dma;

        zero = 0;
        dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)&zero;
        dma->dst = (u32)self;
        dma->cnt = 0x81000034;
        dma->cnt;
    }
    MemCopy32(self->saveData, self, 0x68);
    gGameFrameLevelState = self;
    self->maskLevel = MASK_LEVEL_NONE;
    {
        void *gfx;
        s32 result;

    load:
        gfx = InitTitleScreen(OperatorNew(0x220));
        result = RunTitleScreen(gfx);
        if (gfx != NULL)
            DestroyTitleScreen(gfx, 3);
        if (result == 2) {
            RunCredits();
            goto load;
        }
        if (result != 0) {
            OpenSaveMenu();
            RunSaveMenu(1, 0);
            CloseSaveMenu();
        } else {
            PlayCutscene(gLevelState, 2);
        }
    }
    for (;;) {
        {
            u8 quit;

        restore:
            {
                s32 level = self->level;

                if (level > 23)
                    level = 23;
                self->level = level;
            }
            quit = RunLevelSelect(&self->level);
            MemCopy32(self, self->saveData, 0x68);
            MemCopy32(self->checkpointData, self, 0x68);
            if (quit) {
                OpenSaveMenu();
                quit = RunSaveMenu(0, 0);
                CloseSaveMenu();
                if (quit)
                    self->checkpointFlags = 0;
                goto restore;
            }
        }
        ClearTimeTrial(self);

    start:
        self->roomIndex = 0;
        self->checkpointCrateCount = 0;
        self->checkpointFlags = 0;
        status = 1;
        self->crateTotal = CountLevelCrates(self->level);
        gHud = (void *)InitHud(OperatorNew(0x68));
        SetHudCrateTotal(gHud, self->crateTotal);
        ClearBonusRoundDone(self);
        ClearInBonusRound(self);
        ClearGemPathDone(self);
        ClearInGemPath(self);
        ResetCrateCount(self);
        ClearSwitchPressed(self);
        self->pendingSwitchCrates = 0;
        gEntityFlags->list = NULL;
        best = self->maskLevel;
        SetCheckpointAtPlayer(self, 0);
        ArmStartSpawn(self);
        bitmap = &gEntityFlags;
        for (;;) {
            self->bonusPlatform = self->gemPlatform = 0;
            if (IsInBonusRound(self) || IsInGemPath(self)) {
                self->savedBitmap = *bitmap;
                *bitmap = InitEntityFlags(OperatorNew(0x408));
                if (IsInBonusRound(self)) {
                    self->savedWumpa = GetWumpa(self);
                    self->savedLives = GetLives(self);
                    self->savedCrateCount = GetCrateCount(self);
                    ResetWumpa(self);
                    self->lives = 0;
                    ResetCrateCount(self);
                    EnterBonusRoom((struct level_progress *)&self->level);
                    SetHudCrateTotal(gHud, CountRoomCrates(self->cat));
                } else {
                    self->savedCrateCount = GetCrateCount(self);
                    ResetCrateCount(self);
                    EnterGemPathRoom((struct level_progress *)&self->level);
                    SetHudCrateTotal(gHud, CountRoomCrates(self->cat));
                }
                ArmStartSpawn(self);
            } else if (!(u8)SelectRoom((struct level_progress *)&self->level)) {
                break;
            }
            FreeUnlockedPaletteSlots(gPaletteCache);
            ConfigureHudParts(gHud, 0);
            gRoomFrameCount = 0;
            SetLevelBoss(self, 0);
            PlayRoomMusic((struct level_progress *)&self->level);
            mem_free_bytes(0xC0000000);
            switch (self->cat->kind) {
            case 0:
            case 1:
            case 2:
                status = PlayRoom((struct level_progress *)&self->level);
                break;
            case 3:
                status = InitActorCategory(self->cat->catIndex);
                if (status == 0) {
                    AddPendingSwitchCrates(self, GetActorMissedNitros());
                    SetCheckpointAtPlayer(self, 0);
                }
                break;
            }
            ResetAmbientSfx(gAudioContext);
            {
                s32 tier = self->maskLevel;
                s32 arg = MASK_LEVEL_TWO;

                if (tier <= MASK_LEVEL_ONE)
                    arg = tier;
                SetMaskLevel(self, arg);
            }
            mem_free_bytes(0xC0000000);
            if ((u8)IsInBonusRoom((struct level_progress *)&self->level) && IsInBonusRound(self)) {
                if (gEntityFlags != NULL)
                    DestroyEntityFlags(gEntityFlags, 3);
                gEntityFlags = self->savedBitmap;
                EndBonusRound(self, status == 0);
            }
            if ((u8)IsInGemPathRoom((struct level_progress *)&self->level) && IsInGemPath(self)) {
                if (gEntityFlags != NULL)
                    DestroyEntityFlags(gEntityFlags, 3);
                gEntityFlags = self->savedBitmap;
                EndGemPath(self, status == 0);
            }
            if (status == 2)
                break;
            if (status == 1 && GetLives(self) < 0)
                break;
            if (self->timeTrial && status == 1) {
                self->pendingSwitchCrates = 0;
                self->checkpointSwitchPressed = 0;
                self->roomIndex = 0;
                self->checkpointCrateCount = 0;
                ArmStartSpawn(self);
                ClearTimeTrial(self);
            }
            if (status == 0) {
                u8 done;

                if (!(u8)IsInBonusRoom((struct level_progress *)&self->level) &&
                    !IsInBonusRound(self) &&
                    !(u8)IsInGemPathRoom((struct level_progress *)&self->level) &&
                    !(done = IsInGemPath(self))) {
                    if (!(u8)NextRoom((struct level_progress *)&self->level))
                        break;
                    ArmStartSpawn(self);
                    *(s32 *)*bitmap = done;
                }
            } else {
                RestoreCheckpoint(self);
            }
        }
        if (gHud != NULL)
            DestroyHud(gHud, 3);
        if (GetLives(self) < 0) {
            if (RunContinuePrompt())
                ResetLives(self);
            else
                break;
        }

        if (status == 2) {
            s32 tier = best;

            if (tier > self->maskLevel)
                tier = self->maskLevel;
            self->maskLevel = tier;
        }
        if (status == 0) {
            switch (self->level) {
            case 20:
                if (!(u8)HasSuperBodySlam(self)) {
                    SetNewWorldOpened();
                    GiveSuperBodySlam(self);
                    ShowSuperBodySlamDialog();
                    PlayCutscene(self, 4);
                }
                break;
            case 21:
                if (!(u8)HasDoubleJump(self)) {
                    SetNewWorldOpened();
                    GiveDoubleJump(self);
                    ShowDoubleJumpDialog();
                    PlayCutscene(self, 5);
                }
                break;
            case 22:
                if (!(u8)HasTornadoSpin(self)) {
                    SetNewWorldOpened();
                    GiveTornadoSpin(self);
                    ShowTornadoSpinDialog();
                    PlayCutscene(self, 6);
                }
                break;
            case 23:
                if (!(u8)HasTurboRun(self)) {
                    SetNewWorldOpened();
                    GiveTurboRun(self);
                    ShowTurboRunDialog();
                }
                if (GetCompletionPercent(self) > 99) {
                    PlayCutscene(self, 8);
                    self->level++;
                    self->roomIndex = 0;
                    goto start;
                }
                PlayCutscene(self, 10);
                RunCredits();
                break;
            case 24:
                PlayCutscene(self, 9);
                RunCredits();
                break;
            default:
                if (self->cat->kind == 3)
                    ((union level_best_time *)GetCurrentLevelFlags(self))->f.flag = 1;
                break;
            }
            if (self->timeTrial) {
                u32 t = self->tenths + self->seconds * 10 + self->minutes * 600;

                if (t > LEVEL_FLAG_TIME_MAX)
                    t = LEVEL_FLAG_TIME_MAX;
                if (t < ((union level_best_time *)GetCurrentLevelFlags(self))->f.time ||
                    (((union level_best_time *)GetCurrentLevelFlags(self))->raw &
                     LEVEL_FLAG_TIME_MASK) == 0)
                    ((union level_best_time *)GetCurrentLevelFlags(self))->f.time = t;
            }
            MemCopy32(self->saveData, self, 0x68);
        }
    }
}
