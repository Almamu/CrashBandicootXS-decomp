#include "hud.hpp"
#include "frontend.hpp"
#include "spawners.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
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
}

/* UpdateGameFrame - the main per-frame game-loop driver at the head of
 * the UpdateGameFrame-MainLoop cluster (GitHub issue #34,
 * docs/matching.md). Called once per frame from `MainLoop`
 * (src/system/main_loop.c) with `self` = `gLevelState`, the
 * central per-level state object every other function in this
 * cluster (`EndBonusRound`/`SetCheckpointAtPlayer`, bonus_round.c; the
 * `self+0x80`-`0xc4`/`+2` accessor family, level_state.cpp) also shares.
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
 * C++ since the #664 cleanup (`new TitleScreen`, `new LevelEntityFlags`
 * and their `delete`s, where the C called the constructors on
 * OperatorNew and the destructors with flags 3), built with old_agbcp as
 * the C was with old_agbcc (docs/matching/archive/big-naked-retry.md):
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
 * - `gHud = new Hud`: the HUD's constructor (include/hud.hpp; the C
 *   needed a `void *` conversion of InitHud's result to load the global's
 *   address before the call, as the ROM does), and `delete gHud`.
 */
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
    self->room.level = LEVEL_JUNGLE_JAM;
    self->room.checkpointFlags = 0;
    {
        struct dma_regs *dma;

        zero = 0;
        dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)&zero;
        dma->dst = (u32)self;
        dma->cnt = 0x81000034;
        dma->cnt;
    }
    MemCopy32(&self->saveData, &self->progress, sizeof(struct game_progress));
    gGameFrameLevelState = self;
    self->maskLevel = MASK_LEVEL_NONE;
    {
        TitleScreen *gfx;
        s32 result;

    load:
        gfx = new TitleScreen;
        result = gfx->Run();
        delete gfx;
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
                s32 level = self->room.level;

                LIMIT_MAX(level, LEVEL_NEO_CORTEX);
                self->room.level = level;
            }
            quit = RunLevelSelect(&self->room.level);
            MemCopy32(&self->progress, &self->saveData, sizeof(struct game_progress));
            MemCopy32(&self->checkpointData, &self->progress, sizeof(struct game_progress));
            if (quit) {
                OpenSaveMenu();
                quit = RunSaveMenu(0, 0);
                CloseSaveMenu();
                if (quit)
                    self->room.checkpointFlags = 0;
                goto restore;
            }
        }
        ClearTimeTrial(self);

    start:
        self->room.roomIndex = 0;
        self->room.checkpointCrateCount = 0;
        self->room.checkpointFlags = 0;
        status = 1;
        self->crateTotal = CountLevelCrates(self->room.level);
        gHud = new Hud;
        gHud->SetCrateTotal(self->crateTotal);
        ClearBonusRoundDone(self);
        ClearInBonusRound(self);
        ClearGemPathDone(self);
        ClearInGemPath(self);
        ResetCrateCount(self);
        ClearSwitchPressed(self);
        self->pendingSwitchCrates = 0;
        gEntityFlags->list = 0;
        best = self->maskLevel;
        SetCheckpointAtPlayer(self, 0);
        ArmStartSpawn(self);
        bitmap = &gEntityFlags;
        for (;;) {
            self->bonusPlatform = self->gemPlatform = 0;
            if (IsInBonusRound(self) || IsInGemPath(self)) {
                self->savedBitmap = *bitmap;
                *bitmap = new LevelEntityFlags;
                if (IsInBonusRound(self)) {
                    self->savedWumpa = GetWumpa(self);
                    self->savedLives = GetLives(self);
                    self->savedCrateCount = GetCrateCount(self);
                    ResetWumpa(self);
                    self->lives = 0;
                    ResetCrateCount(self);
                    EnterBonusRoom(&self->room);
                    gHud->SetCrateTotal(CountRoomCrates(self->room.cat));
                } else {
                    self->savedCrateCount = GetCrateCount(self);
                    ResetCrateCount(self);
                    EnterGemPathRoom(&self->room);
                    gHud->SetCrateTotal(CountRoomCrates(self->room.cat));
                }
                ArmStartSpawn(self);
            } else if (!(u8)SelectRoom(&self->room)) {
                break;
            }
            gPaletteCache->FreeUnlockedSlots();
            gHud->ConfigureParts(0);
            gRoomFrameCount = 0;
            SetLevelBoss(self, 0);
            PlayRoomMusic(&self->room);
            mem_free_bytes(0xC0000000);
            switch (self->room.cat->kind) {
            case ROOM_KIND_ON_FOOT:
            case ROOM_KIND_UNDERWATER:
            case ROOM_KIND_HOVER:
                status = PlayRoom(&self->room);
                break;
            case ROOM_KIND_CATEGORY:
                status = InitActorCategory(self->room.cat->param.catIndex);
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
            if ((u8)IsInBonusRoom(&self->room) && IsInBonusRound(self)) {
                delete (LevelEntityFlags *)gEntityFlags;
                gEntityFlags = self->savedBitmap;
                EndBonusRound(self, status == 0);
            }
            if ((u8)IsInGemPathRoom(&self->room) && IsInGemPath(self)) {
                delete (LevelEntityFlags *)gEntityFlags;
                gEntityFlags = self->savedBitmap;
                EndGemPath(self, status == 0);
            }
            if (status == 2)
                break;
            if (status == 1 && GetLives(self) < 0)
                break;
            if (self->timeTrial && status == 1) {
                self->pendingSwitchCrates = 0;
                self->room.checkpointSwitchPressed = 0;
                self->room.roomIndex = 0;
                self->room.checkpointCrateCount = 0;
                ArmStartSpawn(self);
                ClearTimeTrial(self);
            }
            if (status == 0) {
                u8 done;

                if (!(u8)IsInBonusRoom(&self->room) && !IsInBonusRound(self) &&
                    !(u8)IsInGemPathRoom(&self->room) && !(done = IsInGemPath(self))) {
                    if (!(u8)NextRoom(&self->room))
                        break;
                    ArmStartSpawn(self);
                    *(s32 *)*bitmap = done;
                }
            } else {
                RestoreCheckpoint(self);
            }
        }
        if (gHud != NULL)
            delete gHud;
        if (GetLives(self) < 0) {
            if (RunContinuePrompt())
                ResetLives(self);
            else
                break;
        }

        if (status == 2) {
            s32 tier = best;

            LIMIT_MAX(tier, self->maskLevel);
            self->maskLevel = tier;
        }
        if (status == 0) {
            switch (self->room.level) {
            case LEVEL_DINGODILE:
                if (!(u8)HasSuperBodySlam(self)) {
                    SetNewWorldOpened();
                    GiveSuperBodySlam(self);
                    ShowSuperBodySlamDialog();
                    PlayCutscene(self, 4);
                }
                break;
            case LEVEL_N_GIN:
                if (!(u8)HasDoubleJump(self)) {
                    SetNewWorldOpened();
                    GiveDoubleJump(self);
                    ShowDoubleJumpDialog();
                    PlayCutscene(self, 5);
                }
                break;
            case LEVEL_TINY:
                if (!(u8)HasTornadoSpin(self)) {
                    SetNewWorldOpened();
                    GiveTornadoSpin(self);
                    ShowTornadoSpinDialog();
                    PlayCutscene(self, 6);
                }
                break;
            case LEVEL_NEO_CORTEX:
                if (!(u8)HasTurboRun(self)) {
                    SetNewWorldOpened();
                    GiveTurboRun(self);
                    ShowTurboRunDialog();
                }
                if (GetCompletionPercent(&self->progress) > 99) {
                    PlayCutscene(self, 8);
                    self->room.level++;
                    self->room.roomIndex = 0;
                    goto start;
                }
                PlayCutscene(self, 10);
                RunCredits();
                break;
            case LEVEL_MEGA_MIX:
                PlayCutscene(self, 9);
                RunCredits();
                break;
            default:
                if (self->room.cat->kind == ROOM_KIND_CATEGORY)
                    ((union level_record *)GetCurrentLevelFlags(self))->w.cleared = 1;
                break;
            }
            if (self->timeTrial) {
                u32 t = self->tenths + self->seconds * 10 + self->minutes * 600;

                LIMIT_MAX(t, LEVEL_FLAG_TIME_MAX);
                if (t < ((union level_record *)GetCurrentLevelFlags(self))->w.time ||
                    (((union level_record *)GetCurrentLevelFlags(self))->low &
                     LEVEL_FLAG_TIME_MASK) == 0)
                    ((union level_record *)GetCurrentLevelFlags(self))->w.time = t;
            }
            MemCopy32(&self->saveData, &self->progress, sizeof(struct game_progress));
        }
    }
}
