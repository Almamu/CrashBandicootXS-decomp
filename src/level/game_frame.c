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
 * Real C under old_agbcc (docs/matching/big-naked-retry.md):
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
 * - `InitHud` returns a typed pointer, so the store into
 *   `gHud` loads the global's address before the call.
 */
/* The per-level state object (`gLevelState`) as UpdateGameFrame
 * uses it. The first 0x68 bytes are the per-attempt block that the
 * frame loop snapshots into `checkpointData`/`saveData` and restores from. */
union level_best_time
{
    struct
    {
        u32 flag:1;
        u32 unk_1:2;
        u32 time:13;    // tenths of a second, capped at 0x1fff
        u32 unk_16:16;
    } f;
    u16 raw;
};

struct level_category
{
    u8 unk_00[8];
    s32 kind;           // 0x08 - 0-2 plain level, 3 actor-category level
    u8 unk_0c[4];
    u16 category;       // 0x10
};

struct level_state
{
    u8 attempt[0x68];               // 0x000
    s32 unk_68;                     // 0x068
    u8 unk_6c[0x74 - 0x6c];
    s32 lives;                     // 0x074
    s32 maskLevel;                   // 0x078
    u8 unk_7c[0x8c - 0x7c];
    u8 timeTrial;                   // 0x08c
    u8 unk_8d[3];
    s32 minutes;                    // 0x090
    s32 seconds;                    // 0x094
    s32 tenths;                     // 0x098
    u8 unk_9c[0xac - 0x9c];
    s32 pendingSwitchCrates;                     // 0x0ac
    s32 savedWumpa;                     // 0x0b0
    s32 savedCrateCount;                     // 0x0b4
    s32 savedLives;                     // 0x0b8
    s32 crateTotal;                     // 0x0bc
    u8 unk_c0[4];
    s32 level;                      // 0x0c4 - also the head of the progress record
    s32 unk_c8;                     // 0x0c8
    s32 checkpointCrateCount;                     // 0x0cc
    u8 checkpointSwitchPressed;                      // 0x0d0
    u8 unk_d1[0xdc - 0xd1];
    struct level_category *cat;     // 0x0dc
    u8 unk_e0;                      // 0x0e0
    u8 unk_e1[3];
    u8 checkpointData[0x68];                // 0x0e4
    u8 saveData[0x68];               // 0x14c
    void *savedBitmap;              // 0x1b4
    s32 bonusPlatform;                    // 0x1b8
    s32 gemPlatform;                    // 0x1bc
};

extern void *gEntityFlags;
extern void *gPaletteCache;
extern void *gAudioContext;
extern struct level_state *gLevelState;
extern struct level_state *gUnknown_030012C4;
extern void *gHud;
extern s32 gRoomFrameCount;

extern void ResetLives(struct level_state *self);
extern void ResetWumpa(struct level_state *self);
extern void ResetCrateCount(struct level_state *self);
extern void sub_8023120(struct level_state *self, s32 n);
extern void SetMaskAssistDeaths(struct level_state *self, s32 n);
extern void SetCrateAssistDeaths(struct level_state *self, s32 n);
extern void *OperatorNew(u32 size);
extern void PlayCutscene(struct level_state *self, s32 screen);
extern u8 HasSuperBodySlam(struct level_state *self);
extern u8 HasDoubleJump(struct level_state *self);
extern u8 HasTornadoSpin(struct level_state *self);
extern u8 HasTurboRun(struct level_state *self);
extern void GiveSuperBodySlam(struct level_state *self);
extern void GiveDoubleJump(struct level_state *self);
extern void GiveTornadoSpin(struct level_state *self);
extern void GiveTurboRun(struct level_state *self);
extern s32 GetCompletionPercent(struct level_state *self);
extern union level_best_time *GetCurrentLevelFlags(struct level_state *self);
extern void ClearTimeTrial(struct level_state *self);
extern s32 CountLevelCrates(s32 level);
extern void ClearBonusRoundDone(struct level_state *self);
extern void ClearInBonusRound(struct level_state *self);
extern void ClearGemPathDone(struct level_state *self);
extern void ClearInGemPath(struct level_state *self);
extern void ClearSwitchPressed(struct level_state *self);
extern void SetCheckpointAtPlayer(struct level_state *self, u8 arg1);
extern void ArmStartSpawn(struct level_state *self);
extern void FreeUnlockedPaletteSlots(void *cache);
extern void SetLevelBoss(struct level_state *self, s32 arg1);
extern void PlayRoomMusic(s32 *progress);
extern s32 PlayRoom(s32 *progress);
extern void AddPendingSwitchCrates(struct level_state *self, s32 arg1);
extern void SetMaskLevel(struct level_state *self, s32 tier);
extern u8 IsInBonusRoom(s32 *progress);
extern u8 IsInGemPathRoom(s32 *progress);
extern u8 NextRoom(s32 *progress);
extern u8 SelectRoom(s32 *progress);
extern void EnterBonusRoom(s32 *progress);
extern void EnterGemPathRoom(s32 *progress);
extern u8 IsInBonusRound(struct level_state *self);
extern u8 IsInGemPath(struct level_state *self);
extern void DestroyEntityFlags(void *bitmap, s32 arg1);
extern void *InitEntityFlags(void *mem);
extern void EndBonusRound(struct level_state *self, u8 arg1);
extern void EndGemPath(struct level_state *self, u8 arg1);
extern void RestoreCheckpoint(struct level_state *self);
extern s32 GetWumpa(struct level_state *self);
extern s32 GetCrateCount(struct level_state *self);
extern s32 CountRoomCrates(struct level_category *cat);

void UpdateGameFrame(struct level_state *self)
{
    vu16 zero;
    s32 best;
    s32 status;
    void **bitmap;

    self->unk_68 = 0;
    ResetLives(self);
    ResetWumpa(self);
    ResetCrateCount(self);
    sub_8023120(self, 5);
    SetMaskAssistDeaths(self, 5);
    SetCrateAssistDeaths(self, 5);
    self->level = 0;
    self->unk_e0 = 0;
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
    gUnknown_030012C4 = self;
    self->maskLevel = 0;
    {
        void *gfx;
        s32 result;

    load:
        gfx = InitTitleScreen(OperatorNew(0x220));
        result = RunTitleScreen(gfx);
        if (gfx != NULL)
            DestroyTitleScreen(gfx, 3);
        if (result == 2)
        {
            RunCredits();
            goto load;
        }
        if (result != 0)
        {
            OpenSaveMenu();
            RunSaveMenu(1, 0);
            CloseSaveMenu();
        }
        else
        {
            PlayCutscene(gLevelState, 2);
        }
    }
    for (;;)
    {
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
            if (quit)
            {
                OpenSaveMenu();
                quit = RunSaveMenu(0, 0);
                CloseSaveMenu();
                if (quit)
                    self->unk_e0 = 0;
                goto restore;
            }
        }
        ClearTimeTrial(self);

    start:
        self->unk_c8 = 0;
        self->checkpointCrateCount = 0;
        self->unk_e0 = 0;
        status = 1;
        self->crateTotal = CountLevelCrates(self->level);
        gHud = InitHud(OperatorNew(0x68));
        SetHudCrateTotal(gHud, self->crateTotal);
        ClearBonusRoundDone(self);
        ClearInBonusRound(self);
        ClearGemPathDone(self);
        ClearInGemPath(self);
        ResetCrateCount(self);
        ClearSwitchPressed(self);
        self->pendingSwitchCrates = 0;
        *(s32 *)gEntityFlags = 0;
        best = self->maskLevel;
        SetCheckpointAtPlayer(self, 0);
        ArmStartSpawn(self);
        bitmap = &gEntityFlags;
        for (;;)
        {
            self->bonusPlatform = self->gemPlatform = 0;
            if (IsInBonusRound(self) || IsInGemPath(self))
            {
                self->savedBitmap = *bitmap;
                *bitmap = InitEntityFlags(OperatorNew(0x408));
                if (IsInBonusRound(self))
                {
                    self->savedWumpa = GetWumpa(self);
                    self->savedLives = GetLives(self);
                    self->savedCrateCount = GetCrateCount(self);
                    ResetWumpa(self);
                    self->lives = 0;
                    ResetCrateCount(self);
                    EnterBonusRoom(&self->level);
                    SetHudCrateTotal(gHud, CountRoomCrates(self->cat));
                }
                else
                {
                    self->savedCrateCount = GetCrateCount(self);
                    ResetCrateCount(self);
                    EnterGemPathRoom(&self->level);
                    SetHudCrateTotal(gHud, CountRoomCrates(self->cat));
                }
                ArmStartSpawn(self);
            }
            else if (!SelectRoom(&self->level))
            {
                break;
            }
            FreeUnlockedPaletteSlots(gPaletteCache);
            ConfigureHudParts(gHud, 0);
            gRoomFrameCount = 0;
            SetLevelBoss(self, 0);
            PlayRoomMusic(&self->level);
            mem_free_bytes(0xC0000000);
            switch (self->cat->kind)
            {
            case 0:
            case 1:
            case 2:
                status = PlayRoom(&self->level);
                break;
            case 3:
                status = InitActorCategory(self->cat->category);
                if (status == 0)
                {
                    AddPendingSwitchCrates(self, GetActorMissedNitros());
                    SetCheckpointAtPlayer(self, 0);
                }
                break;
            }
            ResetAmbientSfx(gAudioContext);
            {
                s32 tier = self->maskLevel;
                s32 arg = 2;

                if (tier <= 1)
                    arg = tier;
                SetMaskLevel(self, arg);
            }
            mem_free_bytes(0xC0000000);
            if (IsInBonusRoom(&self->level) && IsInBonusRound(self))
            {
                if (gEntityFlags != NULL)
                    DestroyEntityFlags(gEntityFlags, 3);
                gEntityFlags = self->savedBitmap;
                EndBonusRound(self, status == 0);
            }
            if (IsInGemPathRoom(&self->level) && IsInGemPath(self))
            {
                if (gEntityFlags != NULL)
                    DestroyEntityFlags(gEntityFlags, 3);
                gEntityFlags = self->savedBitmap;
                EndGemPath(self, status == 0);
            }
            if (status == 2)
                break;
            if (status == 1 && GetLives(self) < 0)
                break;
            if (self->timeTrial && status == 1)
            {
                self->pendingSwitchCrates = 0;
                self->checkpointSwitchPressed = 0;
                self->unk_c8 = 0;
                self->checkpointCrateCount = 0;
                ArmStartSpawn(self);
                ClearTimeTrial(self);
            }
            if (status == 0)
            {
                u8 done;

                if (!IsInBonusRoom(&self->level) && !IsInBonusRound(self) && !IsInGemPathRoom(&self->level)
                    && !(done = IsInGemPath(self)))
                {
                    if (!NextRoom(&self->level))
                        break;
                    ArmStartSpawn(self);
                    *(s32 *)*bitmap = done;
                }
            }
            else
            {
                RestoreCheckpoint(self);
            }
        }
        if (gHud != NULL)
            DestroyHud(gHud, 3);
        if (GetLives(self) < 0)
        {
            if (RunContinuePrompt())
                ResetLives(self);
            else
                break;
        }

        if (status == 2)
        {
            s32 tier = best;

            if (tier > self->maskLevel)
                tier = self->maskLevel;
            self->maskLevel = tier;
        }
        if (status == 0)
        {
            switch (self->level)
            {
            case 20:
                if (!HasSuperBodySlam(self))
                {
                    SetNewWorldOpened();
                    GiveSuperBodySlam(self);
                    ShowSuperBodySlamDialog();
                    PlayCutscene(self, 4);
                }
                break;
            case 21:
                if (!HasDoubleJump(self))
                {
                    SetNewWorldOpened();
                    GiveDoubleJump(self);
                    ShowDoubleJumpDialog();
                    PlayCutscene(self, 5);
                }
                break;
            case 22:
                if (!HasTornadoSpin(self))
                {
                    SetNewWorldOpened();
                    GiveTornadoSpin(self);
                    ShowTornadoSpinDialog();
                    PlayCutscene(self, 6);
                }
                break;
            case 23:
                if (!HasTurboRun(self))
                {
                    SetNewWorldOpened();
                    GiveTurboRun(self);
                    ShowTurboRunDialog();
                }
                if (GetCompletionPercent(self) > 99)
                {
                    PlayCutscene(self, 8);
                    self->level++;
                    self->unk_c8 = 0;
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
                    GetCurrentLevelFlags(self)->f.flag = 1;
                break;
            }
            if (self->timeTrial)
            {
                u32 t = self->tenths + self->seconds * 10 + self->minutes * 600;

                if (t > 0x1fff)
                    t = 0x1fff;
                if (t < GetCurrentLevelFlags(self)->f.time || (GetCurrentLevelFlags(self)->raw & 0xfff8) == 0)
                    GetCurrentLevelFlags(self)->f.time = t;
            }
            MemCopy32(self->saveData, self, 0x68);
        }
    }
}
