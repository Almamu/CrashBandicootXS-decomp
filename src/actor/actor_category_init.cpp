#include "sprite_obj.hpp"
#include "hud.hpp"
#include "level_state.hpp"
#include "key_input.hpp"

extern "C" {
#include "core.h"
#include "actor_anim.h"
#include "gba/dma_macros.h"
#include "system.h"
#include "hud.h"
#include "util.h"
#include "menus.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* codegen: LevelState::SetCheckpointAtPlayer takes a flag (level_state.hpp); this
 * caller passes the state only and leaves r1 as it is. docs/headers_plan.md */
extern void SetCheckpointAtPlayer_1(void *self) asm("SetCheckpointAtPlayer");

/*
 * `InitActorCategory` - the category (re)initialization + loading-screen
 * driver: stores the category argument into gActorCategory, resets the
 * running counters, decompresses the category sprite sheet, rebuilds the
 * tile-cache pins (SetupActorVramPool) and hands off to
 * SelectActorCategory, then runs a per-VBlank loop (polling input,
 * redrawing, flushing the VRAM DMA queue) until `RunActorCategoryFrame` reports an
 * exit state. Returns the loop's exit code (0-2); 1 re-runs the outer
 * setup unless gLevelState's own state (GetLives, its +0x8c
 * byte) says to leave.
 *
 * Matches under old_agbcc (current agbcc is 3 halfwords off in the
 * option-screen block). What the old NAKED note called "four high-register
 * pins" is loop.c's own invariant hoisting; the shape that reproduces it:
 *  - `activeCount`/`variantCount` point at gActorCategoryDeaths/gActorCategoryBossDeaths
 *    and are (re)assigned at the top of the outer loop. They end up
 *    spilled, and every use rematerializes the address, which is what
 *    puts the ROM's reload registers (r3/r7 in the prologue, r5 in the
 *    `retryBossDeaths` test, r0/r1/r3/r5 in the exit stores) where they are.
 *    With plain globals the reload rotation shifts by one and jump2
 *    cross-jumps the two `ret = 1` exits together.
 *  - `state` (&gLevelState) is assigned right before the inner
 *    loop, so its load precedes the hoisted gOamBuffer load in the
 *    preheader, as in the ROM (sb before the sl/r8 copies).
 *  - The exit-state tests are an if/else chain (a switch builds a
 *    balanced compare tree); the "option screen" branch ends in
 *    `continue` so the inner loop is not rotated.
 *  - `-CanPauseActorCategory() < 0` gives the ROM's `neg; lsr #31` (`!= 0` adds
 *    an `orr`), and the new-press test is `(keys >> 16) & 8` so it shares
 *    the gKeys literal with the `& 4` word test.
 *  - `zero` is volatile, as in the DmaFill16 idiom (address before the
 *    `strh`).
 *  - The pause menu round trip is its own inline function. The C++ front
 *    end puts a deleted note after every expression statement, and loop.c
 *    counts notes in a register's lifetime; inlined bodies leave them out.
 *    Written in line, the OBJ_PLTT and 0x80000100 constants of its two
 *    DMAs live long enough to be hoisted out of the inner loop, which
 *    spills gActorCategories and cross-jumps the `ret = 1` exits
 *    (docs/cplusplus.md, "Dead ends and gotchas").
 */

#define CUR_CATEGORY (gActorCategories[gActorCategory])
#define PAUSED (gLevelState->timeTrial)

/* The pause menu over a category: saves the OBJ palette to a heap buffer,
 * frees the sprite caches, runs the menu, then rebuilds the VRAM pool,
 * restores the palette and redraws the background. Returns the menu's
 * result. */
static inline s32 RunCategoryPauseMenu(struct dma_regs *dma)
{
    void *buf = mem_alloc(OBJ_PLTT_SIZE, MEM_HEAP_IWRAM);
    s32 result;

    dma->src = OBJ_PLTT;
    dma->dst = (u32)buf;
    dma->cnt = 0x80000100;
    dma->cnt;
    FreeSpriteFrameCache();
    FreeSpriteFrameOamQueue();
    FreeObjTileFreeList();
    result = RunPauseMenu();
    SetupActorVramPool();
    FadeBrightness(0x80, 1, 1);
    dma->src = (u32)buf;
    dma->dst = OBJ_PLTT;
    dma->cnt = 0x80000100;
    dma->cnt;
    mem_free(buf);
    ResetCellAnimBg();
    if (CUR_CATEGORY.bgPicture != NULL)
        LoadBgPicture((u8 *)CUR_CATEGORY.bgPicture);
    ReloadActorCategoryGraphics();
    return result;
}

s32 InitActorCategory(s32 category)
{
    s32 ret = 1;
    s32 *activeCount;
    s32 *variantCount;
    LevelState **state;
    struct dma_regs *dma;
    vu16 zero;
    u32 variant;
    s32 status;
    s32 result;
    u8 open;

    gActorCheckpointMissedNitros = 0;
    gActorCategory = category;
    gActorCheckpoint = 0;
    gActorCategoryDeaths = 0;
    gActorCategoryBossDeaths = 0;
    SetCheckpointAtPlayer_1(gLevelState);
    DecompressCategorySpriteSheet(CUR_CATEGORY.sprite_sheet);
    SetupActorVramPool();
    ClearCollectedSpawns();
    EnableActorPaletteCycle(CUR_CATEGORY.type == CATEGORY_TYPE_POLAR);
    InitActorBgScroll(CUR_CATEGORY.type);

    do {
        activeCount = &gActorCategoryDeaths;
        variantCount = &gActorCategoryBossDeaths;
        gActorMissedNitros = gActorCheckpointMissedNitros;
        gLevelState->RestoreCheckpoint();
        if (*variantCount >= (s32)CUR_CATEGORY.retryBossDeaths)
            variant = CUR_CATEGORY.retryBossLevel;
        else
            variant = CUR_CATEGORY.bossLevel;
        zero = 0;
        dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)&zero;
        dma->dst = PLTT;
        dma->cnt = 0x81000200;
        dma->cnt;
        FadeBrightness(0x80, 2, 1);
        InitCellAnim(CUR_CATEGORY.type, CUR_CATEGORY.cellAnim, CUR_CATEGORY.cellAnimSize,
                     gActorCheckpoint);
        if (CUR_CATEGORY.bgPicture != NULL)
            LoadBgPicture((u8 *)CUR_CATEGORY.bgPicture);
        RestoreActorPaletteCycle();
        SelectActorCategory(CUR_CATEGORY.type, CUR_CATEGORY.spawnTable, CUR_CATEGORY.anim_table,
                            *activeCount >= (s32)CUR_CATEGORY.bonusKindDeaths, variant,
                            gActorCheckpoint);
        dma->src = (u32)CUR_CATEGORY.palette;
        dma->dst = OBJ_PLTT;
        dma->cnt = 0x80000100;
        dma->cnt;

        state = &gLevelState;
        for (;;) {
            gInput->Update();
            AdvanceCellAnim();
            status = RunActorCategoryFrame();
            if ((*state)->timeTrial != 0)
                (*state)->TickLevelClock();
            gObjVramCursor->Reset();
            gOamBuffer->Rewind();
            gHud->UpdateSlides();
            gHud->Update();
            FlushSpriteFrameOamQueue();
            WaitForVBlank();
            CommitActorBgScroll();
            gOamBuffer->Commit();
            FlushVramDmaQueue();
            FlipCellAnimPage();
            UpdateActorCategoryBg2();
            AgeSpriteFrameCache();

            if (status != CATEGORY_EXIT_NONE) {
                if (status == CATEGORY_EXIT_CLEARED) {
                    ret = 0;
                } else if (status == CATEGORY_EXIT_BOSS_DEATH) {
                    if ((*state)->timeTrial != 0)
                        goto again;
                    goto both;
                } else if (status == CATEGORY_EXIT_DEATH) {
                    if ((*state)->timeTrial != 0)
                        goto again;
                    if (CUR_CATEGORY.type != CATEGORY_TYPE_POLAR) {
                    both:
                        (*variantCount)++;
                    }
                    (*activeCount)++;
                again:
                    ret = 1;
                }
            } else {
                open = 0;
                if ((u8)IsBrightnessFadeActive() == 0 && ((gKeys.all >> 16) & 8))
                    open = -CanPauseActorCategory() < 0;
                if (open) {
                    result = RunCategoryPauseMenu(dma);
                    if (result == 2) {
                        ret = 2;
                        goto done;
                    }
                    if (result == 3) {
                        ret = 0;
                        goto done;
                    }
                    if (result == 1) {
                        ret = 1;
                        goto done;
                    }
                }
                if (gKeys.all & 4)
                    gHud->ShowCounters();
                continue;
            }
            break;
        }
    done:
        DestroyAllActors();
        ActorCategoryAttemptEndStub();
    } while (ret == 1 && gLevelState->GetLives() >= 0 && PAUSED == 0);

    ActorCategoryEndStub();
    FreeSpriteFrameCache();
    FreeSpriteFrameOamQueue();
    FreeObjTileFreeList();
    FreeCategorySpriteSheet();
    REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP;
    *(vu16 *)PLTT = 0;
    return ret;
}
