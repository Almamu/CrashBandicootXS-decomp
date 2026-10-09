#include "player.hpp"
#include "action_ctrl.hpp"
#include "swim_ctrl.hpp"
#include "input_ctrl.hpp"
#include "crate_list.hpp"
#include "bg_layer.hpp"
#include "level_state.hpp"
#include "hud.hpp"
#include "platform.hpp"
#include "crate.hpp"
#include "audio.hpp"
#include "key_input.hpp"
#include "camera.hpp"

extern "C" {
#include "core.h"
#include "actor.h"
#include "level_data.h"
#include "crates.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "hud.h"
#include "util.h"
#include "system.h"
#include "menus.h"
#include "player.h"
#include "actor_self.h"
#include "sprite_bank.h"
}

/* The room loop: LevelProgress::PlayRoom, the level-start dispatcher,
 * RunRoom, the room's lifecycle state machine it runs, and the
 * per-frame refresh UpdateRoomFrame and SetupRoomBlend RunRoom calls
 * (include/level_state.hpp). Built with old_agbcp (Makefile
 * OLD_AGBCC_OBJS) - see docs/matching/archive/game-loop-old-agbcc.md.
 * RunRoom and UpdateRoomFrame/SetupRoomBlend were run_room.cpp and
 * room_frame.cpp until #771. */

/* Level-start dispatcher, called once from `UpdateGameFrame` when the
 * level object's own `+0xdc->+8` state field is `2`. Allocates the whole
 * per-level object set (the part lists `gUpdateOnlyPartList`,
 * `gTouchableList`, `gCollidableList`, `gDecorationList` and
 * `gForegroundList`, the crate list `gCrateList`, the camera), takes the
 * level layers singleton (`LevelLayers::Get`), builds the player
 * (`gPlayer`) at the checkpoint, and gives it the room kind's controller
 * (`cat->kind`): the action controller on foot, the swim controller
 * (`gSwimCtrl`) underwater, the input controller on the hover levels.
 * Then it runs the room (`RunRoom`) and deletes everything again.
 *
 * C++ since the #664 cleanup, built with old_agbcp (agbcc as C, held by
 * 14 pins, a clobber and the `widget` vtable struct; agbcp orders the
 * flag byte's load before the constant). The locals are the ROM's: each
 * controller case reads `gPlayer` once into `pl` after the sprite bank,
 * and the constant stores (`tag`, `ctrlMode` 3) go through a `u8` local
 * so the constant is loaded before the field's address. */
s32 LevelProgress::PlayRoom()
{
    s32 mode;
    s32 result;

    CreateEntitySpawner();
    ClearRoomExit();

    gUpdateOnlyPartList = new PartList(0x20);
    gTouchableList = new PartList(0xc0);
    gCrateList = new CrateList(0xc0);
    gCollidableList = new PartList(0x80);
    gDecorationList = new PartList(0x40);
    gForegroundList = new PartList(0x40);
    gCamera = new Camera;

    {
        LevelLayers *layers = LevelLayers::Get();

        gLevelLayers = layers;
    }

    gPlayer = new Player(0xffff, 0, 0, 0);
    SetEntityPos(gPlayer, checkpointX, checkpointY);
    gPlayer->f.bytes.flags |= 0x10;
    gPlayer->mirrorFlags.mirrorX = checkpointFlags;

    mode = cat->kind;
    switch (mode) {
    case ROOM_KIND_ON_FOOT:
        {
            ActionCtrl *ctrl = new ActionCtrl;

            ctrl->SetAnimSet(&gActionCtrlMotionSet);
            gPlayer->ctrlMode = mode;
            {
                const struct sprite_bank *bank = (const struct sprite_bank *)SPRITE_BANK_BASE;
                Player *pl = gPlayer;

                pl->bank = bank;
                pl->mover = ctrl;
                ctrl->Attach(pl);
            }
            break;
        }
    case ROOM_KIND_UNDERWATER:
        {
            gSwimCtrl = new SwimCtrl;
            ((SwimCtrl *)gSwimCtrl)->SetAnimSet(&gSwimCtrlMotionSet);
            gPlayer->ctrlMode = mode;
            {
                const struct sprite_bank *bank =
                    (const struct sprite_bank *)(SPRITE_BANK_BASE + 0xc);
                Player *pl = gPlayer;

                pl->bank = bank;
                {
                    u8 anim = 0x1f;

                    pl->tag = anim;
                }
                pl->ResetFrameTimer();
                pl->ResetFrameIndex();
                pl->SetAnimDone(0);
            }
            {
                Player *pl = gPlayer;
                SwimCtrl *ctrl = (SwimCtrl *)gSwimCtrl;

                pl->mover = ctrl;
                ctrl->Attach(pl);
            }
            break;
        }
    case ROOM_KIND_HOVER:
        {
            InputCtrl *ctrl = new InputCtrl;

            ctrl->SetAnimSet(&gInputCtrlMotionSet);
            {
                Player *pl = gPlayer;
                u8 hover = 3;

                pl->ctrlMode = hover;
            }
            {
                const struct sprite_bank *bank =
                    (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x18);
                Player *pl = gPlayer;

                pl->bank = bank;
                pl->mover = ctrl;
                ctrl->Attach(pl);
            }
            break;
        }
    }

    result = RunRoom();

    delete gLevelLayers;
    delete gCamera;
    delete gPlayer;
    delete gForegroundList;
    delete gDecorationList;
    delete gCollidableList;
    delete gCrateList;
    delete gTouchableList;
    delete gUpdateOnlyPartList;

    DestroyEntitySpawner();

    return result;
}

/* GitHub issue #37 follow-up to `docs/matching/archive/issue-37-game-loop-2375c.md`
 * (which matched this function's only caller, `PlayRoom`, above,
 * but left this one "not yet confidently understood
 * branch-by-branch"). `this` (r7) is the level object `PlayRoom`
 * itself received; `gLevelState` is the separate "level" object
 * most of its own callees take. `gLevelTable` is the confirmed
 * 36-slot, 0x24-byte-stride per-level master table (see
 * `pause_menu_info.cpp`/`power_dialog_draw.cpp`/`level_query.cpp`'s own struct views
 * of it) - here indexed by `+0`, reading its `+0x1c` `isBoss`
 * byte (calls `CheckAllCratesBroken` if clear), then
 * `+0x14`/`+0x18` (`maskAssistDeaths`/`crateAssistDeaths`, fed to `SetMaskAssistDeaths`/`SetCrateAssistDeaths`)
 * and finally `+4`, the **state field the 6-case jump table below
 * dispatches on**.
 *
 * **The 6-case dispatch** (`state-1` clamped to `[0,5]`, anything else -
 * `state==0` or `state>6` - taking the `default` path below):
 *
 * - **state 1 or 6** (cases 0 and 5 share one code block): resets the
 *   `gPaletteCycles` "fx queue" (`ClearPaletteCycles`, the PaletteCycles
 *   struct `palette_cycle.cpp` documents) then fires it **twice** via
 *   `AddPaletteCycle(queue, (u16 *)0x05000000, gThemePaletteCycle1A, 0x10,
 *   9, 0)` and `AddPaletteCycle(queue, (u16 *)0x05000000, gStaticData_
 *   0816C830, 0x14, 9, 0)` - `(u16 *)0x05000000` is GBA palette RAM
 *   itself passed as the queue's `targets` argument, so this is a
 *   **palette color-cycle animation** (the only caller of
 *   `AddPaletteCycle`; `TickPaletteCycles` runs it). The second
 *   call's setup (`rate=0x14`, `direction=0`) is byte-identical to
 *   state 5's own call below and the two share its tail (`_08023BA6`)
 *   in the ROM itself - not a coincidence this transcription
 *   reproduces, see the NAKED note below.
 * - **state 2**: single call, `AddPaletteCycle(queue, (u16 *)0x05000000,
 *   gThemePaletteCycle2, 6, 5, 1)` (`direction=1`, the only case that
 *   sets it).
 * - **state 3**: single call, `AddPaletteCycle(queue, (u16 *)0x05000000,
 *   gThemePaletteCycle3, 0xa, 0x10, 0)`.
 * - **state 5**: single call, `AddPaletteCycle(queue, (u16 *)0x05000000,
 *   gThemePaletteCycle5, 0x14, 5, 0)` - shares its `rate=0x14; bl
 *   AddPaletteCycle` tail instruction-for-instruction with state 1/6's
 *   second call, per the note above.
 * - **default** (state 0, state 4, or anything `>6`): skips the reset
 *   and the `AddPaletteCycle` call entirely, just clears the queue's own
 *   `active` byte directly.
 *
 * All five non-default cases fall into one shared tail. **This closes
 * the "`gThemePaletteCycle1A`/`0816C830`/`0816C842`/`0816C862`" open
 * question from the issue doc**: these (plus `gThemePaletteCycle2`,
 * a fifth table the same neighborhood the jump-table trace hadn't
 * previously reached) are not per-level records with their own shape -
 * they're plain, tightly-packed `u16[]` "permutation index list"
 * arguments straight to `AddPaletteCycle`'s own `lists` parameter, each
 * one exactly `list_count * 2` bytes long and back-to-back with the
 * next table in ROM (`0816C814`: 5 entries/0xA bytes -> `0816C81E`;
 * `0816C81E`: 9/0x12 -> `0816C830`; `0816C830`: 9/0x12 -> `0816C842`;
 * `0816C842`: 0x10/0x20 -> `0816C862`; `0816C862`: 5 entries), matching
 * every call site's own `list_count` argument exactly. No further
 * struct needed.
 *
 * **Shared tail**: calls `SetupRoomBlend()`/`ResetObjBuffers()` (the
 * latter already matched in `room.cpp`), then re-reads the
 * current room's (`cat`) `kind`
 * (the same field `PlayRoom` dispatched its own widget-construction
 * switch on) - if it's `1`, re-stamps the player's `+0x2d` byte to
 * `0x1f`, refreshes its OAM entry (`ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/
 * `SetSpriteAnimDone`), and sets the 0x18-byte scratch block's `+0x14` to
 * `2`. Either way, recomputes the player's `+0x29` low nibble from
 * `GetSpriteAnimPaletteSlot(player)` (the "negative-constant bit-clear idiom",
 * `docs/matching.md`) and fires `LoadPaletteSlot` against the tile-asset
 * cache using a `player+0x20`-table lookup indexed by `player+0x2d*7`
 * (0x1c-byte stride), flushes the scratch block (`SnapCamera`) and
 * text-box singleton (`ResetLevelLayers`).
 *
 * If the widget kind is `0`: probes `IsInBonusRound`/`IsInBonusRoom` or
 * `IsInGemPath`/`IsInGemPathRoom` (level-object and self-based readiness
 * checks); on success, clears the player's busy bit 7 (`+0xc &=
 * 0x7f`), re-stamps `+0x2d` to `0x29`, refreshes the OAM entry again,
 * plays a sound effect (`gAudioContext` as the sample id, priority
 * `0x2c`) via `PlaySfx`, sets the player's controller's mode to `0x29`
 * (`mover->SetMode`, a virtual call), repeats the same `LoadPaletteSlot` tile-
 * cache call, and pings `gHud` (`ShowHudCounters`).
 *
 * Either way, this converges on culling the four object lists
 * (`CullPartList` on `gForegroundList`, `gTouchableList`,
 * `gCollidableList` and `gDecorationList`), a
 * `UpdateRoomFrame()` VRAM/OAM refresh, and the fade-cluster
 * `SetDispcntMode(0)`/`ShowObj`/`CommitDispcnt`/`CommitBlendRegs` reset
 * quartet, landing at the **wait loop** (`_08023E5A`/`_08023D7C`,
 * `docs/rom_map.md`'s "Traced the fade-to-black's trigger" section):
 * poll `IsRoomExitRequested` (the `gRoomExitRequested` readiness flag,
 * `room.cpp`) each iteration; while not ready and the player's
 * `+0xc` bit 0 is clear, run one more "outstanding work" pass
 * (`ResetObjBuffers`/`UpdateRoomFrame`, a `RunPauseMenu` input-driven mini-
 * dispatch that can early-exit this whole function with return value
 * `1` or `2` via `ResumeRoomAfterPause`'s level-end teardown, a `gUnknown_
 * 030007E0` input-flag-gated `ShowHudCounters` ping, `UpdatePartList` on
 * three ring-buffer managers, the player's `IsNearCamera` and `Update`
 * (virtual calls), `UpdateCrateList` on
 * `gCrateList`, `UpdateHudSlides`, and a `gLevelState+0x8c`-
 * gated `TickLevelClock` call) before looping back. Once ready, fires the
 * fade (`FadePaletteToBlack`) - the concrete trigger `rom_map.md` originally
 * traced this function down to find.
 *
 * **Post-fade** (`_08023E82` onward, converging at `_08023F92`): sets
 * the return value to `0`, then tries two `SetCheckpoint` "spawn"
 * dispatches gated by `IsInBonusRoom`/`IsInBonusRound`/`GetBonusPlatform` or
 * `IsInGemPathRoom`/`IsInGemPath` (both skip straight to the flush tail on
 * failure); falling through both, loops `gCrateList` counting
 * crates whose `GetClassId` (virtual) returns `3` *and* whose
 * `kind` (`+0x4e`) is `0xa` (the same physics-subsystem state tag
 * `gCrateHitResponse` indexes, `docs/matching/
 * issue-12-physics-collision.md`), then calls `AddPendingSwitchCrates(gUnknown_
 * 030012C0, count)`. **Final tail** (`_08023F92`, also every early-out
 * above): flushes all five hot IWRAM widget-manager globals
 * (`ClearPartList` on `030012E8`/`EC`/`F0`/`F8`/`F4`, `ResetCrateList` on
 * `0300130C`), resets the fade cluster's own bitfield accessors
 * (`HideBg0`/`HideBg1`/`HideBg2`/`HideBg3`/
 * `HideObj`/`WaitForVBlank`/`CommitDispcnt`), and returns whatever
 * `sl` was left holding (`1` by default, `2` from the wait-loop's
 * `RunPauseMenu`-driven early exit, or `0` once the post-fade branch
 * was reached) - the value `PlayRoom` itself stashes and returns.
 *
 * Was a NAKED transcription (from `asm/code_3_2_17_23a1c.s`); matches
 * as plain C under old_agbcc since the hard-register hold pass
 * (docs/matching/archive/hard-register-hold-retry.md), so this object is on the
 * Makefile's OLD_AGBCC_OBJS list (agbcc is 42 halfwords off). C++ since the #664 cleanup (old_agbcp): the
 * player's and the crates' virtual calls replace the hand-written slot
 * calls; the wait loop's flag test is written `(... & 1) == 0`, as `!`
 * on it gives an `eor` in C++. The shared `_08023BA6` tail is ordinary
 * cross-jumping. The one-byte `direction` stack argument is a BLKmode
 * struct (see `struct fx_direction`), and the post-fade player-position
 * copy is a `struct vec2` compound literal. */
/* The direction flag travels as a one-byte struct by value - the ROM
 * stores it into its stack slot with `strb`. The zero-length `pad`
 * makes the struct BLKmode, so the compound literal is stored straight
 * into the outgoing slot: the slot address (`add rN, sp, #4`) comes
 * before the constant, as in the ROM. As a QImode struct the value was
 * built in a register first. */
struct fx_direction {
    u8 value;
    u8 pad[0];
} __attribute__((packed));

/* codegen: AddPaletteCycle takes a u8 `direction` (gfx.h); passed as a
 * u8 it is stored to the stack slot as a word. docs/headers_plan.md */
extern void AddPaletteCycle_fx(PaletteCycles *self, u16 *targets, const u16 *lists, s32 rate,
                               s32 count, struct fx_direction direction) asm("AddPaletteCycle");

#define FX_CYCLE(lists, rate, count, dir) \
    AddPaletteCycle_fx(gPaletteCycles, PAL_RAM, (lists), (rate), (count), \
                (struct fx_direction){ (dir) })

#define PAL_RAM ((u16 *)PLTT)

static inline void SetPoint(struct vec2 *point, s32 x, s32 y)
{
    point->x = x;
    point->y = y;
}

static inline void RefreshPlayerTiles(void)
{
    PaletteCache *cache = gPaletteCache;
    Player *p = gPlayer;

    cache->LoadSlot(p->palette, p->bank->anims[p->tag].paletteId);
}

s32 LevelProgress::RunRoom()
{
    s32 ret = 1;
    s32 i;

    gPlayer->ResetForRoom();
    gCamera->target = gPlayer;
    gCamera->mode = ret;
    gLevelLayers->LoadRoom(cat);
    if (!gLevelTable[level].isBoss)
        gLevelState->CheckAllCratesBroken();
    if (gLevelState->IsSwitchPressed())
        UpdateCrates();
    gLevelState->SetMaskAssistDeaths(gLevelTable[level].maskAssistDeaths);
    gLevelState->SetCrateAssistDeaths(gLevelTable[level].crateAssistDeaths);

    switch (gLevelTable[level].theme) {
    case 2:
        gPaletteCycles->Clear();
        FX_CYCLE(gThemePaletteCycle2, 6, 5, 1);
        break;
    case 1:
    case 6:
        gPaletteCycles->Clear();
        FX_CYCLE(gThemePaletteCycle1A, 0x10, 9, 0);
        FX_CYCLE(gThemePaletteCycle1B, 0x14, 9, 0);
        break;
    case 3:
        gPaletteCycles->Clear();
        FX_CYCLE(gThemePaletteCycle3, 0xA, 0x10, 0);
        break;
    case 5:
        gPaletteCycles->Clear();
        FX_CYCLE(gThemePaletteCycle5, 0x14, 5, 0);
        break;
    default:
        gPaletteCycles->active = 0;
        break;
    }

    SetupRoomBlend();
    ResetObjBuffers();
    if (cat->kind == ROOM_KIND_UNDERWATER) {
        gPlayer->StartAnim(0x1F);
        gCamera->mode = 2;
    }
    gPlayer->palette = gPlayer->GetAnimPaletteSlot();
    RefreshPlayerTiles();
    gCamera->Snap();
    gLevelLayers->Reset();

    if (cat->kind == ROOM_KIND_ON_FOOT) {
        if ((gLevelState->IsInBonusRound() && (u8)IsInBonusRoom()) ||
            (gLevelState->IsInGemPath() && (u8)IsInGemPathRoom())) {
            gPlayer->f.bytes.flags &= 0x7F;
            gPlayer->StartAnim(0x29);
            gAudioContext->PlaySfx(SFX_WARP, 0x100);
            gPlayer->mover->SetMode(0x29);
            RefreshPlayerTiles();
            gHud->ShowCounters();
        }
    }
    gForegroundList->Cull();
    gTouchableList->Cull();
    gCollidableList->Cull();
    gDecorationList->Cull();
    UpdateRoomFrame();
    SetDispcntMode(0);
    SetObjMapping1D();
    ShowObj();
    CommitDispcnt();
    CommitBlendRegs();

    while (!IsRoomExitRequested() && (gPlayer->f.bytes.flags & 1) == 0) {
        ResetObjBuffers();
        UpdateRoomFrame();
        gInput->Update();
        if (!gPlayer->dead && (gKeys.half.pressed & 8)) {
            s32 r = RunPauseMenu();

            if (r == 0) {
                ResumeRoomAfterPause();
                gInput->Update();
            }
            if (r == 1) {
                ret = 1;
                goto fade;
            }
            if (r == 2) {
                ret = 2;
                goto fade;
            }
        }
        if (gKeys.all & 4)
            gHud->ShowCounters();
        gForegroundList->Update();
        gUpdateOnlyPartList->Update();
        if (gPlayer->IsNearCamera())
            gPlayer->Update();
        gCrateList->Update();
        gTouchableList->Update();
        gCollidableList->Update();
        gDecorationList->Update();
        gHud->UpdateSlides();
        if (gLevelState->timeTrial)
            gLevelState->TickLevelClock();
        gRoomFrameCount++;
    }
fade:
    FadePaletteToBlack();
    if (IsRoomExitRequested()) {
        ret = 0;
        if (!(u8)IsInBonusRoom() && gLevelState->IsInBonusRound()) {
            struct vec2 point;
            s32 x;

            x = *(s32 *)gLevelState->GetBonusPlatform() + -0x1E00;
            i = gPlayer->y + 0x1200;
            point.x = x;
            point.y = i;
            // `x` again: the level state goes through the same register
            x = (s32)gLevelState;
            {
                LevelState *state = (LevelState *)x;

                state->SetCheckpoint(((Platform *)state->GetBonusPlatform())->GetExitMirror(),
                                     &point.x);
            }
        } else if (!(u8)IsInGemPathRoom() && gLevelState->IsInGemPath()) {
            struct vec2 point;

            /* The ROM loads the two words into r0 and r1 through the
             * player pointer in r2. A compound literal is one DImode
             * pseudo (r0:r1) set word by word and born before the
             * global's address and the pointer are loaded, so both
             * conflict with it and take r2. Through a `pl` local, the
             * pointer dies in a DImode load and took r0 (`ldr r1, [r0,
             * #4]; ldr r0, [r0]`) unless r0/r1 were held (#662 round 3,
             * from the -dl dump). */
            point = (struct vec2){ gPlayer->x, gPlayer->y };
            gLevelState->SetCheckpoint(0, &point.x);
        } else {
            s32 count = 0;
            CrateList **list;

            i = 0;
            if (count < gCrateList->count) {
                list = &gCrateList;
                do {
                    Crate *e = (*list)->slots[i];

                    if (e->GetClassId() == 3 && e->kind == 0xA)
                        count++;
                    i++;
                } while (i < (*list)->count);
            }
            gLevelState->AddPendingSwitchCrates(count);
        }
    }
    gUpdateOnlyPartList->Clear();
    gCrateList->Reset();
    gTouchableList->Clear();
    gCollidableList->Clear();
    gDecorationList->Clear();
    gForegroundList->Clear();
    HideBg0();
    HideBg1();
    HideBg2();
    HideBg3();
    HideObj();
    WaitForVBlank();
    CommitDispcnt();
    return ret;
}

/* Runs the DMA3/`UploadPaletteCache`+`ResetLevelLayers` refresh pass over every
 * currently-active dual-array manager, then flushes the VRAM DMA
 * queue - only while `level` is `<= 0x1000` (always, for a level
 * index), otherwise this is a no-op. C++ since the #664 cleanup: the
 * player's IsOnScreen and Draw are virtual calls. */
void LevelProgress::UpdateRoomFrame()
{
    gPaletteCache->Upload();
    gCamera->Update();
    gLevelLayers->Scroll();
    gPaletteCycles->Tick();

    if (level <= 0x1000) {
        gHud->Update();
        gForegroundList->Draw();

        if (gPlayer->IsOnScreen())
            gPlayer->Draw();

        gCollidableList->Draw();
        gTouchableList->Draw();
        gCrateList->Draw();
        gDecorationList->Draw();

        gOamBuffer->HideUnused();
        WaitForVBlank();
        gOamBuffer->Commit();
        gLevelLayers->CommitScroll();
        FlushVramDmaQueue();
    }
}

/* Rebuilds the gBlendRegs BLDCNT/BLDALPHA shadow from the current room's
 * blend settings (`level_room.param.blend`) and
 * sets gLevelLayers->raiseObjPriority in an underwater room (kind 1). With
 * no blend effect, the shadow gets a fixed 16/16 alpha pattern. The effect
 * is tested as a halfword and stored from its low byte (the `(u8)` cast:
 * `ldrb`, where the bitfield store alone reads it with `ldrh`), as in the
 * ROM. */
void LevelProgress::SetupRoomBlend()
{
    union blend *b = &gBlendRegs.blend;

    b->raw = 0;
    gLevelLayers->raiseObjPriority = 0;
    if (cat->param.blend.effect != 0) {
        if (cat->kind == ROOM_KIND_UNDERWATER)
            gLevelLayers->raiseObjPriority = 1;
        b->bits.effect = (u8)cat->param.blend.effect;
        b->bits.eva = cat->param.blend.eva;
        b->bits.evb = cat->param.blend.evb;
        b->bits.bg3First = 1;
        b->bits.bg0Second = 1;
        b->bits.bg1Second = 1;
        b->bits.bg2Second = 1;
        b->bits.objSecond = 1;
    } else {
        b->bits.effect = 0;
        b->bits.eva = 0x10;
        b->bits.evb = 0x10;
        b->bits.bg3First = 1;
        b->bits.bg0Second = 1;
        b->bits.bg1Second = 1;
        b->bits.bg2Second = 1;
        b->bits.objSecond = 1;
    }
}
