#include "obj_tile_allocator.hpp"
#include "vram_dma_queue.hpp"
#include "actor_palette_cycle.hpp"
#include "sprite_obj.hpp"
#include "hud.hpp"
#include "level_state.hpp"
#include "key_input.hpp"
#include "menus.hpp"
#include "vehicle.hpp"
#include "cell_anim.hpp"
#include "actor_bg.hpp"
#include "frame_limiter.hpp"
#include "fade.hpp"
#include "sprite_oam_queue.hpp"
#include "sprite_frame_cache.hpp"
#include "category_sprite_sheet.hpp"
#include "collected_spawns.hpp"
#include "actor_category_stats.hpp"
#include "actor_category.hpp"

extern "C" {
#include "core.h"
#include "actor_anim.h"
#include "gba/dma_macros.h"
#include "system.h"
#include "util.h"
#include "actor.h"
#include "bosses.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

using Engine::Sys::FrameLimiter;
using Engine::Sys::VramDmaQueue;

/* codegen: LevelState::SetCheckpointAtPlayer takes a flag (level_state.hpp); this
 * caller passes the state only and leaves r1 as it is. docs/headers.md */
extern void SetCheckpointAtPlayer_1(void *self) asm("SetCheckpointAtPlayer");

/*
 * `ActorCategory::Init` - the category (re)initialization + loading-screen
 * driver: stores the category argument into ActorCategory::current, resets the
 * running counters, decompresses the category sprite sheet, rebuilds the
 * tile-cache pins (ActorCategory::SetupVramPool) and hands off to
 * ActorCategory::Select, then runs a per-VBlank loop (polling input,
 * redrawing, flushing the VRAM DMA queue) until `ActorCategory::RunFrame` reports an
 * exit state. Returns the loop's exit code (0-2); 1 re-runs the outer
 * setup unless gLevelState's own state (GetLives, its +0x8c
 * byte) says to leave.
 *
 * Matches under old_agbcc (current agbcc is 3 halfwords off in the
 * option-screen block). What the old NAKED note called "four high-register
 * pins" is loop.c's own invariant hoisting; the shape that reproduces it:
 *  - `activeCount`/`variantCount` point at ActorCategoryStats::deaths/bossDeaths
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
 *  - `-ActorCategory::CanPause() < 0` gives the ROM's `neg; lsr #31` (`!= 0` adds
 *    an `orr`), and the new-press test is `(keys >> 16) & 8` so it shares
 *    the gKeys literal with the `& 4` word test.
 *  - The pause menu round trip is its own inline function. The C++ front
 *    end puts a deleted note after every expression statement, and loop.c
 *    counts notes in a register's lifetime; inlined bodies leave them out.
 *    Written in line, the OBJ_PLTT and 0x80000100 constants of its two
 *    DMAs live long enough to be hoisted out of the inner loop, which
 *    spills ActorCategory::descriptors and cross-jumps the `ret = 1` exits
 *    (docs/cplusplus.md, "Dead ends and gotchas").
 */

#define CUR_CATEGORY (descriptors[current])
#define PAUSED (gLevelState->timeTrial)

/* The pause menu over a category: saves the OBJ palette to a heap buffer,
 * frees the sprite caches, runs the menu, then rebuilds the VRAM pool,
 * restores the palette and redraws the background. Returns the menu's
 * result. */
inline s32 ActorCategory::OpenPauseMenu()
{
    void *buf = mem_alloc(OBJ_PLTT_SIZE, MEM_HEAP_IWRAM);
    s32 result;

    DmaCopy16(3, OBJ_PLTT, buf, OBJ_PLTT_SIZE);
    SpriteFrameCache::Destroy();
    SpriteOamQueue::Destroy();
    ObjTileAllocator::Destroy();
    result = RunPauseMenu();
    SetupVramPool();
    BrightnessFade::Start(0x80, 1, 1);
    DmaCopy16(3, buf, OBJ_PLTT, OBJ_PLTT_SIZE);
    mem_free(buf);
    CellAnim::ResetBg();
    if (CUR_CATEGORY.bgPicture != NULL)
        LoadBgPicture(CUR_CATEGORY.bgPicture);
    ReloadGraphics();
    return result;
}

s32 ActorCategory::Init(s32 category)
{
    s32 ret = ROOM_EXIT_RETRY;
    s32 *activeCount;
    s32 *variantCount;
    LevelState **state;
    u32 variant;
    s32 status;
    s32 result;
    u8 open;

    ActorCategoryStats::checkpointMissedNitros = 0;
    current = category;
    ActorCategoryStats::checkpoint = 0;
    ActorCategoryStats::deaths = 0;
    ActorCategoryStats::bossDeaths = 0;
    SetCheckpointAtPlayer_1(gLevelState);
    CategorySpriteSheet::Decompress(CUR_CATEGORY.sprite_sheet);
    SetupVramPool();
    CollectedSpawns::Clear();
    ActorPaletteCycle::Enable(CUR_CATEGORY.type == CATEGORY_TYPE_POLAR);
    ActorBg::InitScroll(CUR_CATEGORY.type);

    do {
        activeCount = &ActorCategoryStats::deaths;
        variantCount = &ActorCategoryStats::bossDeaths;
        ActorCategoryStats::missedNitros = ActorCategoryStats::checkpointMissedNitros;
        gLevelState->RestoreCheckpoint();
        if (*variantCount >= CUR_CATEGORY.retryBossDeaths)
            variant = CUR_CATEGORY.retryBossLevel;
        else
            variant = CUR_CATEGORY.bossLevel;
        DmaFill16(3, 0, PLTT, PLTT_SIZE);
        BrightnessFade::Start(0x80, 2, 1);
        CellAnim::Init(CUR_CATEGORY.type, CUR_CATEGORY.cellAnim, CUR_CATEGORY.cellAnimSize,
                       ActorCategoryStats::checkpoint);
        if (CUR_CATEGORY.bgPicture != NULL)
            LoadBgPicture(CUR_CATEGORY.bgPicture);
        ActorPaletteCycle::Restore();
        Select(CUR_CATEGORY.type, CUR_CATEGORY.spawnTable, CUR_CATEGORY.anim_table,
               *activeCount >= CUR_CATEGORY.bonusKindDeaths, variant,
               ActorCategoryStats::checkpoint);
        DmaCopy16(3, CUR_CATEGORY.palette, OBJ_PLTT, OBJ_PLTT_SIZE);

        state = &gLevelState;
        for (;;) {
            gInput->Update();
            CellAnim::Advance();
            status = RunFrame();
            if ((*state)->timeTrial != 0)
                (*state)->TickLevelClock();
            gObjVramCursor->Reset();
            gOamBuffer->Rewind();
            gHud->UpdateSlides();
            gHud->Update();
            SpriteOamQueue::Flush();
            FrameLimiter::WaitForVBlank();
            ActorBg::CommitScroll();
            gOamBuffer->Commit();
            VramDmaQueue::Flush();
            CellAnim::FlipPage();
            UpdateBg2();
            SpriteFrameCache::Age();

            if (status != CATEGORY_EXIT_NONE) {
                if (status == CATEGORY_EXIT_CLEARED) {
                    ret = ROOM_EXIT_CLEARED;
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
                    ret = ROOM_EXIT_RETRY;
                }
            } else {
                open = 0;
                if (BrightnessFade::IsActive() == 0 && ((gKeys.all >> 16) & START_BUTTON))
                    open = -CanPause() < 0;
                if (open) {
                    result = OpenPauseMenu();
                    if (result == PAUSE_ROW_TYPE_QUIT) {
                        ret = ROOM_EXIT_QUIT;
                        goto done;
                    }
                    if (result == PAUSE_ROW_TYPE_UNK3) {
                        ret = ROOM_EXIT_CLEARED;
                        goto done;
                    }
                    if (result == PAUSE_ROW_TYPE_RESTART) {
                        ret = ROOM_EXIT_RETRY;
                        goto done;
                    }
                }
                if (gKeys.all & SELECT_BUTTON)
                    gHud->ShowCounters();
                continue;
            }
            break;
        }
    done:
        DestroyActors();
        ActorCategoryAttemptEndStub();
    } while (ret == ROOM_EXIT_RETRY && gLevelState->GetLives() >= 0 && PAUSED == 0);

    ActorCategoryEndStub();
    SpriteFrameCache::Destroy();
    SpriteOamQueue::Destroy();
    ObjTileAllocator::Destroy();
    CategorySpriteSheet::Free();
    REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP;
    BACKDROP_COLOR = 0;
    return ret;
}
