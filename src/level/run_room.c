#include "core.h"
#include "match.h"
#include "hud.h"
#include "util.h"
#include "system.h"
#include "audio.h"
#include "menus.h"
#include "crates.h"
#include "player.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #37 follow-up to `docs/matching/archive/issue-37-game-loop-2375c.md`
 * (which matched this function's only caller, `PlayRoom`, in
 * `play_room.c`, but left this one "not yet confidently understood
 * branch-by-branch"). `self` (r7) is the level object `PlayRoom`
 * itself received; `gLevelState` is the separate "level" object
 * most of its own callees take. `gLevelTable` is the confirmed
 * 36-slot, 0x24-byte-stride per-level master table (see
 * `pause_menu_info.c`/`power_dialog_draw.c`/`level_query.c`'s own struct views
 * of it) - here indexed by `self+0`, reading its `+0x1c` `isBoss`
 * byte (calls `CheckAllCratesBroken` if clear), then
 * `+0x14`/`+0x18` (`maskAssistDeaths`/`crateAssistDeaths`, fed to `SetMaskAssistDeaths`/`SetCrateAssistDeaths`)
 * and finally `+4`, the **state field the 6-case jump table below
 * dispatches on**.
 *
 * **The 6-case dispatch** (`state-1` clamped to `[0,5]`, anything else -
 * `state==0` or `state>6` - taking the `default` path below):
 *
 * - **state 1 or 6** (cases 0 and 5 share one code block): resets the
 *   `gPaletteCycles` "fx queue" (`ClearPaletteCycles`, the `palette_cycler`
 *   struct `palette_cycle.c` documents) then fires it **twice** via
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
 * **Shared tail**: calls `SetupRoomBlend(self)`/`ResetObjBuffers()` (the
 * latter already matched in `room.c`), then re-reads the
 * current room's (`self->cat`) `kind`
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
 * `0x2c`) via `PlaySfx`, fires the `player+0x44`-table's trampoline
 * (`_call_via_r2`, mode `0x29`), repeats the same `LoadPaletteSlot` tile-
 * cache call, and pings `gHud` (`ShowHudCounters`).
 *
 * Either way, this converges on flushing the four HUD ring-buffer
 * managers (`CullPartList` on `030012F4`/`EC`/`F0`/`F8`), a
 * `UpdateRoomFrame(self)` VRAM/OAM refresh, and the fade-cluster
 * `SetDispcntMode(0)`/`ShowObj`/`CommitDispcnt`/`CommitBlendRegs` reset
 * quartet, landing at the **wait loop** (`_08023E5A`/`_08023D7C`,
 * `docs/rom_map.md`'s "Traced the fade-to-black's trigger" section):
 * poll `IsRoomExitRequested` (the `gRoomExitRequested` readiness flag,
 * `room.c`) each iteration; while not ready and the player's
 * `+0xc` bit 0 is clear, run one more "outstanding work" pass
 * (`ResetObjBuffers`/`UpdateRoomFrame`, a `RunPauseMenu` input-driven mini-
 * dispatch that can early-exit this whole function with return value
 * `1` or `2` via `ResumeRoomAfterPause`'s level-end teardown, a `gUnknown_
 * 030007E0` input-flag-gated `ShowHudCounters` ping, `UpdatePartList` on
 * three ring-buffer managers, `_call_via_r1` trampoline probes against
 * the player's own `+0x18`/`+0x38`-`/+0x18` tables, `UpdateCrateList` on
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
 * entries whose `_call_via_r1` trampoline probe returns `3` *and* whose
 * own `+0x4e` tag is `0xa` (the same physics-subsystem state tag
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
 * Makefile's OLD_AGBCC_OBJS list (it is the file's only function;
 * agbcc is 42 halfwords off). The shared `_08023BA6` tail is ordinary
 * cross-jumping. The one-byte `direction` stack argument is a BLKmode
 * struct (see `struct fx_direction`), and the post-fade player-position
 * copy holds r0/r1 while the player pointer is loaded. */
struct gl_point {
    s32 x;
    s32 y;
};

struct gl_vtable {
    u8 unk_00[0x18];
    struct actor_method m18; /* +0x18 */
    u8 unk_20[0x18];
    struct actor_method m38; /* +0x38 */
    u8 unk_40[8];
    struct actor_method m48; /* +0x48 */
};

struct gl_attach_vtable {
    u8 unk_00[0x20];
    struct actor_method attach; /* +0x20 */
};

struct gl_attach {
    u8 unk_00[0xC];
    struct gl_attach_vtable *vtable; /* +0x0C */
};

struct gl_entity {
    u8 unk_00[0x18];
    struct gl_vtable *vtable; /* +0x18 */
    u8 unk_1C[0x32];
    u8 tag; /* +0x4E */
};

union gl_input {
    u32 held;
    struct {
        u16 lo;
        u16 hi;
    } half;
};

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
extern void AddPaletteCycle_fx(struct palette_cycler *self, u16 *targets, const u16 *lists,
                               s32 rate, s32 count,
                               struct fx_direction direction) asm("AddPaletteCycle");

#define FX_CYCLE(lists, rate, count, dir) \
    AddPaletteCycle_fx(gPaletteCycles, PAL_RAM, (lists), (rate), (count), \
                (struct fx_direction){ (dir) })
extern s32 _call_via_r2(void *self, s32 arg, void *fn);
extern s32 _call_via_r1(void *self, void *fn);

#define PAL_RAM ((u16 *)PLTT)

/* obj->vtable->slot(obj), through `_call_via_r1`. */
#define PMF_CALL(obj, slot)                                                    \
    ({                                                                         \
        __typeof__(&(obj)->vtable->slot) _m = &(obj)->vtable->slot;            \
        _call_via_r1((u8 *)(obj) + _m->thisOffset, _m->fn);                     \
    })

static inline void SetPoint(struct gl_point *point, s32 x, s32 y)
{
    point->x = x;
    point->y = y;
}

static inline void SpawnNearPlayer(s32 x, s32 y)
{
    struct gl_point point;
    struct level_state *level;

    point.x = x;
    point.y = y;
    level = gLevelState;
    SetCheckpoint(level, sub_801B29C((struct gobj *)GetBonusPlatform(level)), &point.x);
}

static inline void RestartPlayerAnim(struct player *p, s32 anim)
{
    p->tag = anim;
    ResetSpriteFrameTimer(p);
    ResetSpriteFrameIndex(p);
    SetSpriteAnimDone(p, 0);
}

static inline void RefreshPlayerTiles(void)
{
    void *cache = gPaletteCache;
    struct player *p = gPlayer;

    LoadPaletteSlot(cache, p->slot, p->anim->records[p->tag].paletteId);
}

s32 RunRoom(struct level_progress *self)
{
    s32 ret = 1;
    s32 i;

    ResetPlayerForRoom(gPlayer);
    gCamera->target = (struct camera_target *)gPlayer;
    gCamera->mode = ret;
    LoadRoom(gLevelLayers, self->cat);
    if (!gLevelTable[self->level].isBoss)
        CheckAllCratesBroken(gLevelState);
    if (IsSwitchPressed(gLevelState))
        UpdateCrates();
    SetMaskAssistDeaths(gLevelState, gLevelTable[self->level].maskAssistDeaths);
    SetCrateAssistDeaths(gLevelState, gLevelTable[self->level].crateAssistDeaths);

    switch (gLevelTable[self->level].theme) {
    case 2:
        ClearPaletteCycles(gPaletteCycles);
        FX_CYCLE(gThemePaletteCycle2, 6, 5, 1);
        break;
    case 1:
    case 6:
        ClearPaletteCycles(gPaletteCycles);
        FX_CYCLE(gThemePaletteCycle1A, 0x10, 9, 0);
        FX_CYCLE(gThemePaletteCycle1B, 0x14, 9, 0);
        break;
    case 3:
        ClearPaletteCycles(gPaletteCycles);
        FX_CYCLE(gThemePaletteCycle3, 0xA, 0x10, 0);
        break;
    case 5:
        ClearPaletteCycles(gPaletteCycles);
        FX_CYCLE(gThemePaletteCycle5, 0x14, 5, 0);
        break;
    default:
        gPaletteCycles->active = 0;
        break;
    }

    SetupRoomBlend((struct level_ctx *)self);
    ResetObjBuffers();
    if (self->cat->kind == 1) {
        RestartPlayerAnim(gPlayer, 0x1F);
        gCamera->mode = 2;
    }
    gPlayer->slot = GetSpriteAnimPaletteSlot((struct actor *)gPlayer);
    RefreshPlayerTiles();
    SnapCamera(gCamera);
    ResetLevelLayers(gLevelLayers);

    if (self->cat->kind == 0) {
        if ((IsInBonusRound(gLevelState) && (u8)IsInBonusRoom(self)) ||
            (IsInGemPath(gLevelState) && (u8)IsInGemPathRoom(self))) {
            struct gl_attach *a;

            gPlayer->flags.all &= 0x7F;
            RestartPlayerAnim(gPlayer, 0x29);
            PlaySfx(gAudioContext, 0x2C, 0x100);
            a = gPlayer->ctrl;
            _call_via_r2((u8 *)a + a->vtable->attach.thisOffset, 0x29, a->vtable->attach.fn);
            RefreshPlayerTiles();
            ShowHudCounters(gHud);
        }
    }
    CullPartList(gUnknown_030012F4);
    CullPartList(gUnknown_030012EC);
    CullPartList(gCollidableList);
    CullPartList(gDecorationList);
    UpdateRoomFrame(self);
    SetDispcntMode(0);
    SetObjMapping1D();
    ShowObj();
    CommitDispcnt();
    CommitBlendRegs();

    while (!IsRoomExitRequested() && !(gPlayer->flags.all & 1)) {
        ResetObjBuffers();
        UpdateRoomFrame(self);
        UpdateKeys(gInput);
        if (!gPlayer->dead && (gKeys.half.pressed & 8)) {
            s32 r = RunPauseMenu();

            if (r == 0) {
                ResumeRoomAfterPause(self);
                UpdateKeys(gInput);
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
            ShowHudCounters(gHud);
        UpdatePartList(gUnknown_030012F4);
        UpdatePartList(gUpdateOnlyPartList);
        if ((u8)PMF_CALL(gPlayer, isNearCamera))
            PMF_CALL(gPlayer, update);
        UpdateCrateList(gCrateList);
        UpdatePartList(gUnknown_030012EC);
        UpdatePartList(gCollidableList);
        UpdatePartList(gDecorationList);
        UpdateHudSlides(gHud);
        if (gLevelState->timeTrial)
            TickLevelClock(gLevelState);
        gRoomFrameCount++;
    }
fade:
    FadePaletteToBlack();
    if (IsRoomExitRequested()) {
        ret = 0;
        if (!(u8)IsInBonusRoom(self) && IsInBonusRound(gLevelState)) {
            struct gl_point point;
            s32 x;

            x = *(s32 *)GetBonusPlatform(gLevelState) + -0x1E00;
            i = gPlayer->y + 0x1200;
            point.x = x;
            point.y = i;
            x = (s32)gLevelState;
            SetCheckpoint((void *)x, sub_801B29C((struct gobj *)GetBonusPlatform((void *)x)),
                          &point.x);
        } else if (!(u8)IsInGemPathRoom(self) && IsInGemPath(gLevelState)) {
            struct gl_point point;
            struct player *pl;
            MATCH_HOLD_REG(s32, hold, r0);
            MATCH_HOLD_REG(s32, hold1, r1);

            /* Hard-register hold (no code): with r0 and r1 live, the
             * global's address and the player pointer both land in r2,
             * as in the ROM. */
            MATCH_HOLD(hold);
            MATCH_HOLD(hold1);
            pl = gPlayer;
            /* End of the hold. */
            MATCH_USE(hold);
            MATCH_USE(hold1);
            /* copied as one 8-byte struct (ldr; ldr; str; str) */
            point = *(struct gl_point *)&pl->x;
            SetCheckpoint(gLevelState, 0, &point.x);
        } else {
            s32 count = 0;
            struct pool_manager **list;

            i = 0;
            if (count < gCrateList->activeCount) {
                list = &gCrateList;
                do {
                    struct gl_entity *e = (struct gl_entity *)(*list)->slotArray[i];

                    if (PMF_CALL(e, m48) == 3 && e->tag == 0xA)
                        count++;
                    i++;
                } while (i < (*list)->activeCount);
            }
            AddPendingSwitchCrates(gLevelState, count);
        }
    }
    ClearPartList(gUpdateOnlyPartList);
    ResetCrateList(gCrateList);
    ClearPartList(gUnknown_030012EC);
    ClearPartList(gCollidableList);
    ClearPartList(gDecorationList);
    ClearPartList(gUnknown_030012F4);
    HideBg0();
    HideBg1();
    HideBg2();
    HideBg3();
    HideObj();
    WaitForVBlank();
    CommitDispcnt();
    return ret;
}
