#include "hud.hpp"
#include "frontend.hpp"
#include "spawners.hpp"
#include "audio.hpp"
#include "level_state.hpp"

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
#include "menus.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* UpdateGameFrame - the main per-frame game-loop driver at the head of
 * the UpdateGameFrame-MainLoop cluster (GitHub issue #34,
 * docs/matching.md). Called once per frame from `MainLoop`
 * (src/system/main_loop.cpp) on `gLevelState`, the
 * central per-level state object every other function in this
 * cluster (`EndBonusRound`/`SetCheckpointAtPlayer`, bonus_round.cpp; the
 * `+0x80`-`0xc4`/`+2` accessor family, level_state.cpp) also shares.
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
 * - `bonusPlatform = gemPlatform = 0` computes the 0x1b8 address
 *   first, and the `SetMaskLevel` argument starts at 2 and takes the
 *   tier only when it is <= 1.
 * - `gHud = new Hud`: the HUD's constructor (include/hud.hpp; the C
 *   needed a `void *` conversion of InitHud's result to load the global's
 *   address before the call, as the ROM does), and `delete gHud`.
 */
void LevelState::UpdateGameFrame()
{
    vu16 zero;
    s32 best;
    s32 status;
    struct entity_flags **bitmap;

    unk_68 = 0;
    ResetLives();
    ResetWumpa();
    ResetCrateCount();
    SetUnusedAssistDeaths(5);
    SetMaskAssistDeaths(5);
    SetCrateAssistDeaths(5);
    room.level = LEVEL_JUNGLE_JAM;
    room.checkpointFlags = 0;
    {
        struct dma_regs *dma;

        zero = 0;
        dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)&zero;
        dma->dst = (u32)this;
        dma->cnt = 0x81000034;
        dma->cnt;
    }
    MemCopy32(&saveData, &progress, sizeof(struct game_progress));
    gGameFrameLevelState = this;
    maskLevel = MASK_LEVEL_NONE;
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
            gLevelState->PlayCutscene(2);
        }
    }
    for (;;) {
        {
            u8 quit;

        restore:
            {
                s32 level = room.level;

                LIMIT_MAX(level, LEVEL_NEO_CORTEX);
                room.level = level;
            }
            quit = RunLevelSelect(&room.level);
            MemCopy32(&progress, &saveData, sizeof(struct game_progress));
            MemCopy32(&checkpointData, &progress, sizeof(struct game_progress));
            if (quit) {
                OpenSaveMenu();
                quit = RunSaveMenu(0, 0);
                CloseSaveMenu();
                if (quit)
                    room.checkpointFlags = 0;
                goto restore;
            }
        }
        ClearTimeTrial();

    start:
        room.roomIndex = 0;
        room.checkpointCrateCount = 0;
        room.checkpointFlags = 0;
        status = 1;
        crateTotal = CountLevelCrates(room.level);
        gHud = new Hud;
        gHud->SetCrateTotal(crateTotal);
        ClearBonusRoundDone();
        ClearInBonusRound();
        ClearGemPathDone();
        ClearInGemPath();
        ResetCrateCount();
        ClearSwitchPressed();
        pendingSwitchCrates = 0;
        gEntityFlags->list = 0;
        best = maskLevel;
        SetCheckpointAtPlayer(0);
        ArmStartSpawn();
        bitmap = &gEntityFlags;
        for (;;) {
            bonusPlatform = gemPlatform = 0;
            if (IsInBonusRound() || IsInGemPath()) {
                savedBitmap = *bitmap;
                *bitmap = new LevelEntityFlags;
                if (IsInBonusRound()) {
                    savedWumpa = GetWumpa();
                    savedLives = GetLives();
                    savedCrateCount = GetCrateCount();
                    ResetWumpa();
                    lives = 0;
                    ResetCrateCount();
                    room.EnterBonusRoom();
                    gHud->SetCrateTotal(CountRoomCrates(room.cat));
                } else {
                    savedCrateCount = GetCrateCount();
                    ResetCrateCount();
                    room.EnterGemPathRoom();
                    gHud->SetCrateTotal(CountRoomCrates(room.cat));
                }
                ArmStartSpawn();
            } else if (!(u8)room.SelectRoom()) {
                break;
            }
            gPaletteCache->FreeUnlockedSlots();
            gHud->ConfigureParts(0);
            gRoomFrameCount = 0;
            SetLevelBoss(0);
            room.PlayRoomMusic();
            mem_free_bytes(0xC0000000);
            switch (room.cat->kind) {
            case ROOM_KIND_ON_FOOT:
            case ROOM_KIND_UNDERWATER:
            case ROOM_KIND_HOVER:
                status = room.PlayRoom();
                break;
            case ROOM_KIND_CATEGORY:
                status = InitActorCategory(room.cat->param.catIndex);
                if (status == 0) {
                    AddPendingSwitchCrates(GetActorMissedNitros());
                    SetCheckpointAtPlayer(0);
                }
                break;
            }
            gAudioContext->ResetAmbientSfx();
            {
                s32 tier = maskLevel;
                s32 arg = MASK_LEVEL_TWO;

                if (tier <= MASK_LEVEL_ONE)
                    arg = tier;
                SetMaskLevel(arg);
            }
            mem_free_bytes(0xC0000000);
            if ((u8)room.IsInBonusRoom() && IsInBonusRound()) {
                delete (LevelEntityFlags *)gEntityFlags;
                gEntityFlags = savedBitmap;
                EndBonusRound(status == 0);
            }
            if ((u8)room.IsInGemPathRoom() && IsInGemPath()) {
                delete (LevelEntityFlags *)gEntityFlags;
                gEntityFlags = savedBitmap;
                EndGemPath(status == 0);
            }
            if (status == 2)
                break;
            if (status == 1 && GetLives() < 0)
                break;
            if (timeTrial && status == 1) {
                pendingSwitchCrates = 0;
                room.checkpointSwitchPressed = 0;
                room.roomIndex = 0;
                room.checkpointCrateCount = 0;
                ArmStartSpawn();
                ClearTimeTrial();
            }
            if (status == 0) {
                u8 done;

                if (!(u8)room.IsInBonusRoom() && !IsInBonusRound() && !(u8)room.IsInGemPathRoom() &&
                    !(done = IsInGemPath())) {
                    if (!(u8)room.NextRoom())
                        break;
                    ArmStartSpawn();
                    *(s32 *)*bitmap = done;
                }
            } else {
                RestoreCheckpoint();
            }
        }
        if (gHud != NULL)
            delete gHud;
        if (GetLives() < 0) {
            if (RunContinuePrompt())
                ResetLives();
            else
                break;
        }

        if (status == 2) {
            s32 tier = best;

            LIMIT_MAX(tier, maskLevel);
            maskLevel = tier;
        }
        if (status == 0) {
            switch (room.level) {
            case LEVEL_DINGODILE:
                if (!(u8)HasSuperBodySlam()) {
                    SetNewWorldOpened();
                    GiveSuperBodySlam();
                    ShowSuperBodySlamDialog();
                    PlayCutscene(4);
                }
                break;
            case LEVEL_N_GIN:
                if (!(u8)HasDoubleJump()) {
                    SetNewWorldOpened();
                    GiveDoubleJump();
                    ShowDoubleJumpDialog();
                    PlayCutscene(5);
                }
                break;
            case LEVEL_TINY:
                if (!(u8)HasTornadoSpin()) {
                    SetNewWorldOpened();
                    GiveTornadoSpin();
                    ShowTornadoSpinDialog();
                    PlayCutscene(6);
                }
                break;
            case LEVEL_NEO_CORTEX:
                if (!(u8)HasTurboRun()) {
                    SetNewWorldOpened();
                    GiveTurboRun();
                    ShowTurboRunDialog();
                }
                if (GetCompletionPercent(&progress) > 99) {
                    PlayCutscene(8);
                    room.level++;
                    room.roomIndex = 0;
                    goto start;
                }
                PlayCutscene(10);
                RunCredits();
                break;
            case LEVEL_MEGA_MIX:
                PlayCutscene(9);
                RunCredits();
                break;
            default:
                if (room.cat->kind == ROOM_KIND_CATEGORY)
                    ((union level_record *)GetCurrentLevelFlags())->w.cleared = 1;
                break;
            }
            if (timeTrial) {
                u32 t = tenths + seconds * 10 + minutes * 600;

                LIMIT_MAX(t, LEVEL_FLAG_TIME_MAX);
                if (t < ((union level_record *)GetCurrentLevelFlags())->w.time ||
                    (((union level_record *)GetCurrentLevelFlags())->low & LEVEL_FLAG_TIME_MASK) ==
                        0)
                    ((union level_record *)GetCurrentLevelFlags())->w.time = t;
            }
            MemCopy32(&saveData, &progress, sizeof(struct game_progress));
        }
    }
}
