#include "core.h"
#include "actor_anim.h"
#include "gba/dma_macros.h"
#include "memory.h"

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
 *  - `activeCount`/`variantCount` point at gUnknown_03001384/03001388
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
 *  - `-sub_802A5AC() < 0` gives the ROM's `neg; lsr #31` (`!= 0` adds
 *    an `orr`), and the new-press test is `(keys >> 16) & 8` so it shares
 *    the gKeys literal with the `& 4` word test.
 *  - `zero` is volatile, as in the DmaFill16 idiom (address before the
 *    `strh`).
 */

extern s32 gUnknown_03001390;
extern s32 gActorCategory;
extern s32 gActorCheckpoint;
extern s32 gUnknown_03001384;
extern s32 gUnknown_03001388;
extern s32 gUnknown_0300138C;
extern u8 *gLevelState;
extern void *gOamBuffer;
extern void *gInput;
extern void *gObjVramCursor;
extern void *gHud;
extern u32 gKeys;

extern void SetCheckpointAtPlayer(void *arg0);
extern void DecompressCategorySpriteSheet(const u8 *sheet);
extern void SetupActorVramPool(void);
extern void ClearCollectedSpawns(void);
extern void EnableActorPaletteCycle(s32 flag);
extern void sub_8029C30(s32 kind);
extern s32 GetLives(void *state);
extern void RestoreCheckpoint(void *arg0);
extern void FadeBrightness(s32 a, s32 b, s32 c);
extern void InitCellAnim(s32 arg0, void *arg1, u32 arg2, s32 arg3);
extern void LoadBgPicture(void);
extern void RestoreActorPaletteCycle(void);
extern void SelectActorCategory(s32 type, void *subEffectTable, void *animTable, s32 activeFlag, s32 variant, s32 tick);
extern void UpdateKeys(void *arg0);
extern void AdvanceCellAnim(void);
extern s32 RunActorCategoryFrame(void);
extern void TickLevelClock(void *arg0);
extern void ResetObjVram(void *self);
extern void RewindOamBuffer(void *arg0);
extern void UpdateHudSlides(void *state);
extern void UpdateHud(void *self);
extern void FlushSpriteFrameOamQueue(void);
extern void WaitForVBlank(void);
extern void sub_8029E50(void);
extern void CommitOamBuffer(void *arg0);
extern void FlushVramDmaQueue(void);
extern void FlipCellAnimPage(void);
extern void sub_802A650(void);
extern void AgeSpriteFrameCache(void);
extern u8 IsBrightnessFadeActive(void);
extern u8 sub_802A5AC(void);
extern void FreeSpriteFrameCache(void);
extern void FreeSpriteFrameOamQueue(void);
extern void FreeObjTileFreeList(void);
extern s32 RunPauseMenu(void);
extern void ResetCellAnimBg(void);
extern void sub_802A5C4(void);
extern void ShowHudCounters(void *arg0);
extern void FreeCategorySpriteSheet(void);
extern void nullsub_5(void);
extern void nullsub_6(void);
extern void DestroyAllActors(void);

#define CUR_CATEGORY (gActorCategories[gActorCategory])
#define PAUSED (gLevelState[0x8c])

s32 InitActorCategory(s32 category)
{
    s32 ret = 1;
    s32 *activeCount;
    s32 *variantCount;
    u8 **state;
    struct dma_regs *dma;
    vu16 zero;
    u32 variant;
    s32 status;
    s32 result;
    u8 open;
    void *buf;

    gUnknown_03001390 = 0;
    gActorCategory = category;
    gActorCheckpoint = 0;
    gUnknown_03001384 = 0;
    gUnknown_03001388 = 0;
    SetCheckpointAtPlayer(gLevelState);
    DecompressCategorySpriteSheet(CUR_CATEGORY.sprite_sheet);
    SetupActorVramPool();
    ClearCollectedSpawns();
    EnableActorPaletteCycle(CUR_CATEGORY.type == 0);
    sub_8029C30(CUR_CATEGORY.type);

    do {
        activeCount = &gUnknown_03001384;
        variantCount = &gUnknown_03001388;
        gUnknown_0300138C = gUnknown_03001390;
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
            LoadBgPicture();
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
            if ((*state)[0x8c] != 0)
                TickLevelClock(*state);
            ResetObjVram(gObjVramCursor);
            RewindOamBuffer(gOamBuffer);
            UpdateHudSlides(gHud);
            UpdateHud(gHud);
            FlushSpriteFrameOamQueue();
            WaitForVBlank();
            sub_8029E50();
            CommitOamBuffer(gOamBuffer);
            FlushVramDmaQueue();
            FlipCellAnimPage();
            sub_802A650();
            AgeSpriteFrameCache();

            if (status != 0) {
                if (status == 1) {
                    ret = 0;
                } else if (status == 2) {
                    if ((*state)[0x8c] != 0)
                        goto again;
                    goto both;
                } else if (status == 3) {
                    if ((*state)[0x8c] != 0)
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
                if (IsBrightnessFadeActive() == 0 && ((gKeys >> 16) & 8))
                    open = -sub_802A5AC() < 0;
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
                        LoadBgPicture();
                    sub_802A5C4();
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
                if (gKeys & 4)
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
