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

/* codegen: SetCheckpointAtPlayer takes (state, flag) (level.h); this
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
 *  - `activeCount`/`variantCount` point at gActorCategoryDeaths/03001388
 *    and are (re)assigned at the top of the outer loop. They end up
 *    spilled, and every use rematerializes the address, which is what
 *    puts the ROM's reload registers (r3/r7 in the prologue, r5 in the
 *    `unknown_28` test, r0/r1/r3/r5 in the exit stores) where they are.
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
 */

#define CUR_CATEGORY (gActorCategories[gActorCategory])
#define PAUSED (gLevelState->timeTrial)

s32 InitActorCategory(s32 category)
{
    s32 ret = 1;
    s32 *activeCount;
    s32 *variantCount;
    struct level_state **state;
    struct dma_regs *dma;
    vu16 zero;
    u32 variant;
    s32 status;
    s32 result;
    u8 open;
    void *buf;

    gActorCheckpointMissedNitros = 0;
    gActorCategory = category;
    gActorCheckpoint = 0;
    gActorCategoryDeaths = 0;
    gUnknown_03001388 = 0;
    SetCheckpointAtPlayer_1(gLevelState);
    DecompressCategorySpriteSheet(CUR_CATEGORY.sprite_sheet);
    SetupActorVramPool();
    ClearCollectedSpawns();
    EnableActorPaletteCycle(CUR_CATEGORY.type == 0);
    InitActorBgScroll(CUR_CATEGORY.type);

    do {
        activeCount = &gActorCategoryDeaths;
        variantCount = &gUnknown_03001388;
        gActorMissedNitros = gActorCheckpointMissedNitros;
        RestoreCheckpoint(gLevelState);
        if (*variantCount >= (s32)CUR_CATEGORY.unknown_28)
            variant = CUR_CATEGORY.unknown_30;
        else
            variant = CUR_CATEGORY.position_offset_flag;
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
            LoadBgPicture(CUR_CATEGORY.bgPicture);
        RestoreActorPaletteCycle();
        SelectActorCategory(CUR_CATEGORY.type, CUR_CATEGORY.spawnTable, CUR_CATEGORY.anim_table,
                            *activeCount >= (s32)CUR_CATEGORY.active_count_threshold, variant,
                            gActorCheckpoint);
        dma->src = (u32)CUR_CATEGORY.palette;
        dma->dst = OBJ_PLTT;
        dma->cnt = 0x80000100;
        dma->cnt;

        state = &gLevelState;
        for (;;) {
            UpdateKeys(gInput);
            AdvanceCellAnim();
            status = RunActorCategoryFrame();
            if ((*state)->timeTrial != 0)
                TickLevelClock(*state);
            ResetObjVram(gObjVramCursor);
            RewindOamBuffer(gOamBuffer);
            UpdateHudSlides(gHud);
            UpdateHud(gHud);
            FlushSpriteFrameOamQueue();
            WaitForVBlank();
            CommitActorBgScroll();
            CommitOamBuffer(gOamBuffer);
            FlushVramDmaQueue();
            FlipCellAnimPage();
            UpdateActorCategoryBg2();
            AgeSpriteFrameCache();

            if (status != 0) {
                if (status == 1) {
                    ret = 0;
                } else if (status == 2) {
                    if ((*state)->timeTrial != 0)
                        goto again;
                    goto both;
                } else if (status == 3) {
                    if ((*state)->timeTrial != 0)
                        goto again;
                    if (CUR_CATEGORY.type != 0) {
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
                    open = -(u8)CanPauseActorCategory() < 0;
                if (open) {
                    buf = mem_alloc(0x200, 0x80000000);
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
                        LoadBgPicture(CUR_CATEGORY.bgPicture);
                    ReloadActorCategoryGraphics();
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
                    ShowHudCounters(gHud);
                continue;
            }
            break;
        }
    done:
        DestroyAllActors();
        nullsub_5();
    } while (ret == 1 && GetLives(gLevelState) >= 0 && PAUSED == 0);

    nullsub_6();
    FreeSpriteFrameCache();
    FreeSpriteFrameOamQueue();
    FreeObjTileFreeList();
    FreeCategorySpriteSheet();
    REG_DISPCNT = 0x41;
    *(vu16 *)PLTT = 0;
    return ret;
}

asm(".align 2, 0");
