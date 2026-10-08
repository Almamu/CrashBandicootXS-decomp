#include "hud.hpp"
#include "frontend.hpp"
#include "level_state.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "level_state.h"
#include "actor.h"
#include <agb_syscall.h>
#include "hud.h"
#include "frontend.h"
#include "system.h"
#include "bosses.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "player.h"
#include "sprite_bank.h"
}

/* The level state's accessors and helpers (C linkage; they take level_state.h's
 * struct level_state). C++ since the #664 cleanup, for GetLevelState's
 * `new LevelState` (level_state.hpp) and ShowCompanyLogos' `new
 * CompanyLogos`. */

/* Record 47's periodic-trigger setter (docs/rom_map.md, "An
 * achievement/unlock-icon spawner family, tied to gSpriteBankTable
 * record 47") - `TickLevelClock` is its decrementer/consumer. It gets a
 * palette slot for sprite bank 47's animation 1 palette and loads
 * animation 4's palette into it.
 *
 * Bank 47 is read through SpriteBankSet::Anims (sprite_obj.hpp) both
 * times: the ROM rebuilds the bank's offset after the `GetPaletteSlot`
 * call instead of keeping it in a register across it. The second
 * palette id is its own statement, so it is read before
 * `gPaletteCache`, as in the ROM. */
void FreezeLevelClock(struct level_state *self, s32 seconds)
{
    u8 recordId, slot;

    gAudioContext->PlaySfx(SFX_CLOCK, 0x100);

    self->countdown += seconds * 60;

    slot = gPaletteCache->GetSlot(gSpriteBankSet->Anims(47)[1].paletteId);

    recordId = gSpriteBankSet->Anims(47)[4].paletteId;
    gPaletteCache->LoadSlot(slot, recordId);

    if (self->room.cat->kind == ROOM_KIND_CATEGORY) {
        gPaletteCache->UploadSlot(slot);
    }
}

/* Countdown-gated periodic event trigger (docs/rom_map.md, "A per-level
 * completion-time cascade..."): decrements `self+0xa0`'s countdown and,
 * on reaching 0, fires record 47's spawn (`FreezeLevelClock`'s sibling,
 * using animation 1's palette for both the lookup and the slot fill
 * this time). While the countdown is already 0, instead runs a cascading
 * digit-counter carry over `self+0x9c`/`0x98`/`0x94`/`0x90` (thresholds
 * `5`/`9`/`0x3b`/`0x63`) - shaped like a minutes:seconds:centiseconds
 * odometer, saturating (not wrapping) once the top field hits its cap.
 *
 * The trigger half reads bank 47 the way `FreezeLevelClock` does (see
 * its comment above). The countdown pointer `addr` is *reused* (reassigned, not
 * redeclared) for the digit-cascade's own address-chasing in the `else`
 * branch, which is what makes gcc emit the ROM's `subs r1,#4`
 * chain-decrement instead of recomputing `self+0x98`/`self+0x94` fresh
 * from `self` each time. `newCountdown` is its own local: decrementing
 * `countdown` in place gives `subs r3,#1` where the ROM has
 * `subs r0,r3,#1`.
 * The digit-cascade's four levels are each written test-true-first
 * (`if (val == N) { nested / return } else { val + 1 }`) rather than
 * test-false-first (`if (val != N) { val + 1 } else { nested }`) -
 * despite being logically identical, this flips which branch gcc lays
 * out inline vs at the end of the function, and only the true-first
 * form reproduces the ROM's block order (all four "plain increment"
 * cases grouped at the tail via fall-through). Each level also caches
 * its loaded field value in a named local (`val1`/`val2`/`val3`)
 * rather than re-reading `*addr` for the increment - without that, gcc
 * re-emits a redundant `ldr` in the `else` (increment) arm instead of
 * reusing the register the comparison already loaded. */
void TickLevelClock(struct level_state *self)
{
    s32 *addr = &self->countdown;
    s32 countdown = *addr;

    if (countdown != 0) {
        s32 newCountdown = countdown - 1;
        *addr = newCountdown;

        if (newCountdown == 0) {
            u8 recordId, slot;

            slot = gPaletteCache->GetSlot(gSpriteBankSet->Anims(47)[1].paletteId);

            recordId = gSpriteBankSet->Anims(47)[1].paletteId;
            gPaletteCache->LoadSlot(slot, recordId);

            if (self->room.cat->kind == ROOM_KIND_CATEGORY) {
                gPaletteCache->UploadSlot(slot);
            }
        }
    } else {
        s32 *save1;
        s32 val1;

        addr = &self->frames;
        val1 = *addr;
        save1 = addr;

        if (val1 == 5) {
            s32 *save2;
            s32 val2;

            addr -= 1;
            val2 = *addr;
            save2 = addr;

            if (val2 == 9) {
                s32 val3;

                addr -= 1;
                val3 = *addr;

                if (val3 == 0x3b) {
                    s32 *c4 = &self->minutes;

                    if (*c4 == 0x63) {
                        return;
                    }
                    *c4 += 1;
                    *addr = countdown;
                } else {
                    *addr = val3 + 1;
                }
                *save2 = 0;
            } else {
                *addr = val2 + 1;
            }
            *save1 = 0;
        } else {
            *addr = val1 + 1;
        }
    }
}

void AddBrokenCrate(struct level_state *self)
{
    self->crateCount += 1;

    if (self->crateCount == self->crateTotal) {
        if (!IsInBonusRound(self) && !IsInGemPath(self)) {
            const struct level_room *level = self->room.cat;

            if (level->kind == ROOM_KIND_CATEGORY) {
                u8 *flags = GetCurrentLevelFlags(self);
                /* Separate `mask`/`value` locals reproduce the ROM's
                 * "build the OR mask before loading the byte" order - a plain
                 * `*flags |= LEVEL_FLAG_CRATE_GEM;` loads the byte first regardless of
                 * statement order (see docs/workflow.md step 7). */
                s32 mask = LEVEL_FLAG_CRATE_GEM;
                s32 value = *flags;
                mask |= value;
                *flags = mask;
            } else {
                u16 b = *(u16 *)&self->crateGemX;
                u16 c = *(u16 *)&self->crateGemY;
                SpawnCrateGem(0xffff, b, c, 0);
            }
        }
    }

    if (self->timeTrial == 0) {
        gHud->ShowCrates();
    }
}

void PressSwitchCrate(struct level_state *self)
{
    self->switchPressed = 1;

    if (IsInBonusRound(self)) {
        self->savedCrateCount += self->pendingSwitchCrates;
        return;
    }

    self->crateCount += self->pendingSwitchCrates;

    if (self->crateCount != self->crateTotal) {
        return;
    }

    if (!IsInBonusRound(self) && !IsInGemPath(self)) {
        const struct level_room *level = self->room.cat;

        if (level->kind == ROOM_KIND_CATEGORY) {
            u8 *flags = GetCurrentLevelFlags(self);
            s32 mask = LEVEL_FLAG_CRATE_GEM;
            s32 value = *flags;
            mask |= value;
            *flags = mask;
        } else {
            u16 b = *(u16 *)&self->crateGemX;
            u16 c = *(u16 *)&self->crateGemY;
            SpawnCrateGem(0xffff, b, c, 0);
        }
    }
}

void *GetBonusPlatform(struct level_state *self)
{
    return self->bonusPlatform;
}

void SetCrateAssistDeaths(struct level_state *self, s32 value)
{
    self->crateAssistDeaths = value;
}

void SetMaskAssistDeaths(struct level_state *self, s32 value)
{
    self->maskAssistDeaths = value;
}

/* Setter of `unusedAssistDeaths`, a third assist-deaths threshold: the
 * field sits next to the other two (in the reverse of this accessor
 * order, as theirs do), and UpdateGameFrame sets all three to the same
 * default 5 together. The level table has no value for it and only the
 * unused GetUnusedAssistDeaths reads it, so which assist it was meant
 * for is unknown. */
void SetUnusedAssistDeaths(struct level_state *self, s32 value)
{
    self->unusedAssistDeaths = value;
}

s32 GetCrateAssistDeaths(struct level_state *self)
{
    return self->crateAssistDeaths;
}

s32 GetMaskAssistDeaths(struct level_state *self)
{
    return self->maskAssistDeaths;
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/). Getter
 * of `unusedAssistDeaths` (SetUnusedAssistDeaths). */
s32 GetUnusedAssistDeaths(struct level_state *self)
{
    return self->unusedAssistDeaths;
}

void AddPendingSwitchCrates(struct level_state *self, s32 delta)
{
    self->pendingSwitchCrates += delta;
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/), nor is
 * TestUnusedFlags: ORs `mask` into `unusedFlags`, which InitLevelState zeroes and
 * nothing else touches. */
void SetUnusedFlags(struct level_state *self, s32 mask)
{
    self->unusedFlags |= mask;
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/). Whether
 * any bit of `mask` is set in `unusedFlags`. */
s32 TestUnusedFlags(struct level_state *self, s32 mask)
{
    s32 x = self->unusedFlags & mask;
    return (u32)(-x | x) >> 31;
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the
 * data tables for ClearPowers). Clears all four power bits (`flags`
 * bits 4-7) that GiveTurboRun/GiveSuperBodySlam/GiveTornadoSpin/
 * GiveDoubleJump set. */
void ClearPowers(struct level_state *self)
{
    /* The step-by-step accumulator/mask split reproduces the ROM's
     * sequence (a fresh mov+neg pair for -0x41, then `tmp += 0x20`); a
     * plain `&= ~0xf0` compiles to a different one. */
    s32 acc = -0x11;
    s32 tmp = self->progress.flags;
    acc &= tmp;
    tmp = -0x41;
    acc &= tmp;
    tmp += 0x20;
    acc &= tmp;
    tmp = 0x7f;
    acc &= tmp;
    self->progress.flags = acc;
}

/* Each of these four builds its OR mask into `r1` *before* loading the
 * byte into `r2` (the ROM's `movs r1,#N; ldrb r2,[r0,#2]` order) - a
 * plain `*flags |= N;` loads the byte first regardless of statement
 * order, so the mask and the value are separate locals (see
 * docs/workflow.md step 7, and the identical fix on `AddBrokenCrate`/
 * `PressSwitchCrate`'s `*flags |= 2;` above). */
void GiveTornadoSpin(struct level_state *self)
{
    s32 mask = 0x40;
    s32 value = self->progress.flags;
    mask |= value;
    self->progress.flags = mask;
}

void GiveSuperBodySlam(struct level_state *self)
{
    s32 mask = 0x20;
    s32 value = self->progress.flags;
    mask |= value;
    self->progress.flags = mask;
}

void GiveTurboRun(struct level_state *self)
{
    s32 mask = 0x10;
    s32 value = self->progress.flags;
    mask |= value;
    self->progress.flags = mask;
}

void GiveDoubleJump(struct level_state *self)
{
    s32 mask = 0x80;
    s32 value = self->progress.flags;
    mask |= value;
    self->progress.flags = mask;
}

/* These three read a single flag bit back out of `self+2` as a plain
 * 0/1 value. Writing them as `(x >> n) & 1` compiles an extra `and`
 * this compiler doesn't need - the ROM instead isolates the bit by
 * shifting it up into the sign bit and shifting back down unsigned,
 * the same branchless idiom already used for `TestUnusedFlags`'s
 * `!= 0` test (see docs/workflow.md step 7 / matching.md). */
s32 HasTornadoSpin(struct level_state *self)
{
    return (u32)(self->progress.flags << 25) >> 31;
}

s32 HasSuperBodySlam(struct level_state *self)
{
    return (u32)(self->progress.flags << 26) >> 31;
}

s32 HasTurboRun(struct level_state *self)
{
    return (u32)(self->progress.flags << 27) >> 31;
}

/* GitHub issues #35/#36: 0x080231CC-0x08023488, the remainder of the
 * UpdateGameFrame-MainLoop cluster's "level" object accessor family
 * (`gLevelState`) - fully contiguous with the functions above (no
 * ldscript.txt change needed, this is still the same self type and
 * still the same object file). See docs/matching/archive/issue-35-36-0x080231cc-game-loop.md
 * for the full write-up. Bit-7 getter for the `self+2` flags byte this
 * file's own family already covers bits 4-6 of. */
s32 HasDoubleJump(struct level_state *self)
{
    return self->progress.flags >> 7;
}

/* self+0x70/self+0xbc form a counter/threshold pair (`AddBrokenCrate`/
 * `PressSwitchCrate` above, `CheckAllCratesBroken` below); this resets the counter. */
void ResetCrateCount(struct level_state *self)
{
    self->crateCount = 0;
}

void ResetWumpa(struct level_state *self)
{
    self->wumpa = 0;
}

void ResetLives(struct level_state *self)
{
    self->lives = 5;
}

/* Sets the Aku Aku mask level (`maskLevel`, +0x78, 0-3): level `3` (the
 * invincibility mask) always fires a jingle (`StartSong(
 * gAudioContext, SONG_DRUMS)`) and skips the rest; leaving level 3 re-fires
 * `PlayRoomMusic(&self->room)` once. Either way `maskLevel` ends up
 * holding `state`. */
void SetMaskLevel(void *selfArg, s32 stateArg)
{
    struct level_state *self = (struct level_state *)selfArg;
    s32 state = stateArg;

    if (state == MASK_LEVEL_INVINCIBLE) {
        gAudioContext->StartSong(SONG_DRUMS);
    } else if (self->maskLevel == MASK_LEVEL_INVINCIBLE) {
        self->maskLevel = state;
        PlayRoomMusic(&self->room);
    }
    self->maskLevel = state;
}

/* Plain setter for the same `self+0x74` field `ResetLives` above
 * hardcodes to `5`. */
void SetLives(struct level_state *self, s32 value)
{
    self->lives = value;
}

/* Advances the `self+0x78` latch by one via `SetMaskLevel`. */
void RaiseMaskLevel(struct level_state *self)
{
    s32 next = self->maskLevel + 1;
    SetMaskLevel(self, next);
}

/* Outside time trials (`timeTrial`, +0x8c): loses a life (`lives`,
 * +0x74) and, while any are left, pings `gHud`
 * (`ShowHudLives`). */
void LoseLife(struct level_state *self)
{
    if (self->timeTrial == 0) {
        s32 v = self->lives - 1;
        self->lives = v;

        if (v >= 0) {
            gHud->ShowLives();
        }
    }
}

/* Getter for `self+0x6c`, the counter `ResetWumpa` clears. */
s32 GetWumpa(struct level_state *self)
{
    return self->wumpa;
}

/* Getters for the "seconds"/"minutes" tier of the digit-cascade odometer
 * `TickLevelClock` above already documents (`self+0x9c`/`0x98`/`0x94`/
 * `0x90`, thresholds 5/9/0x3b/0x63) - this trio covers its bottom three
 * tiers. */
s32 GetClockTenths(struct level_state *self)
{
    return self->tenths;
}

s32 GetClockSeconds(struct level_state *self)
{
    return self->seconds;
}

s32 GetClockMinutes(struct level_state *self)
{
    return self->minutes;
}

/* `self+0xa4`-`self+0xa9`: six flag bytes (bonus round, gem path,
 * spawn at the start marker, switch crate), each with a getter and a
 * clear (some also a set-to-1). */
u8 IsGemPathDone(struct level_state *self)
{
    return self->gemPathDone;
}

void ClearGemPathDone(struct level_state *self)
{
    self->gemPathDone = 0;
}

void SetGemPathDone(struct level_state *self)
{
    self->gemPathDone = 1;
}

u8 IsInGemPath(struct level_state *self)
{
    return self->inGemPath;
}

void ClearInGemPath(struct level_state *self)
{
    self->inGemPath = 0;
}

u8 IsBonusRoundDone(struct level_state *self)
{
    return self->bonusRoundDone;
}

void ClearBonusRoundDone(struct level_state *self)
{
    self->bonusRoundDone = 0;
}

void SetBonusRoundDone(struct level_state *self)
{
    self->bonusRoundDone = 1;
}

u8 IsInBonusRound(struct level_state *self)
{
    return self->inBonusRound;
}

void ClearInBonusRound(struct level_state *self)
{
    self->inBonusRound = 0;
}

u8 IsSwitchPressed(struct level_state *self)
{
    return self->switchPressed;
}

void ClearSwitchPressed(struct level_state *self)
{
    self->switchPressed = 0;
}

void ClearTimeTrial(struct level_state *self)
{
    self->timeTrial = 0;
}

/* `deaths` (+0x7c): maskless hits since the last checkpoint
 * (AddDeath is only called from PlayerHandleEvent). */
s32 GetDeaths(struct level_state *self)
{
    return self->deaths;
}

void AddDeath(struct level_state *self)
{
    self->deaths = self->deaths + 1;
}

void ResetDeaths(struct level_state *self)
{
    self->deaths = 0;
}

u8 GetSpawnAtStart(struct level_state *self)
{
    return self->spawnAtStart;
}

void ClearSpawnAtStart(struct level_state *self)
{
    self->spawnAtStart = 0;
}

/* Resets `deaths` and arms `spawnAtStart`, so the player is placed on
 * the room's start marker. */
void ArmStartSpawn(struct level_state *self)
{
    ResetDeaths(self);
    self->spawnAtStart = 1;
}

/* Sets the boss's controller (`self->boss`; the spawners' BossCtrl,
 * spawn_bosses.cpp), right after the `self+0x1c0`/`0x1c4` pair
 * `CheckAllCratesBroken` below reads. */
void SetLevelBoss(struct level_state *self, void *value)
{
    self->boss = (struct level_state_1c8 *)value;
}

/* Plain getter/getter/setter trio for `room.roomIndex` (`self+0xc8`, the
 * current room's index in the level's room list; SpawnRoomExit tests it
 * for the first room) and `room.level` (`self+0xc4`) - the
 * latter is the "current index" field `GetBossHealth`/`GetBossIndex`/
 * `GetCurrentLevelFlags`/`IsCrystalSaved` below all read. */
s32 GetRoomIndex(struct level_state *self)
{
    return self->room.roomIndex;
}

s32 GetCurrentLevel(struct level_state *self)
{
    return self->room.level;
}

void SetCurrentLevel(struct level_state *self, s32 value)
{
    self->room.level = value;
}

/* Five thin two-argument wrappers that drop `self` entirely and forward
 * straight to one of `LevelHasYellowGemEntity`/`34`/`40`/`4C`/`58` (the medal
 * "flag index" wrappers, `level_query.cpp`). */
s32 LevelHasYellowGem(void *self, s32 idx)
{
    return LevelHasYellowGemEntity(idx);
}

s32 LevelHasBlueGem(void *self, s32 idx)
{
    return LevelHasBlueGemEntity(idx);
}

s32 LevelHasGreenGem(void *self, s32 idx)
{
    return LevelHasGreenGemEntity(idx);
}

s32 LevelHasRedGem(void *self, s32 idx)
{
    return LevelHasRedGemEntity(idx);
}

s32 LevelHasGemPathGem(void *self, s32 idx)
{
    return LevelHasGemPathGemEntity(idx);
}

/* Dispatches on the "current index" field `room.level` (`self+0xc4`): index `0x15` fires
 * the actor-part singleton lifetime counter (`GetHovercraftPartsLeft`,
 * `hovercraft_parts.cpp`); indices `0x14`/`0x16`/`0x17` instead compute
 * `3 - (*(self+0x1c8))->0x10` (the fourth word-field `SetLevelBoss`
 * above sets, apparently itself a pointer to a small record); anything
 * else returns `0`. */
s32 GetBossHealth(struct level_state *self)
{
    s32 idx = self->room.level;

    switch (idx) {
    case LEVEL_N_GIN:
        return GetHovercraftPartsLeft();
    case LEVEL_DINGODILE:
        {
            struct level_state_1c8 *p = self->boss;
            return 3 - p->hits;
        }
    case LEVEL_TINY:
        {
            struct level_state_1c8 *p = self->boss;
            return 3 - p->hits;
        }
    case LEVEL_NEO_CORTEX:
        {
            struct level_state_1c8 *p = self->boss;
            return 3 - p->hits;
        }
    default:
        return 0;
    }
}

/* Same "current index" field (`room.level`), the level, mapped through a
 * 5-entry table (the boss levels) to the boss's HUD icon animation
 * (`{9, 8, 6, 7}`) minus a shared `BOSS_HUD_ANIM_BASE`, giving BOSS_* -
 * LEVEL_MEGA_MIX and any other level both skip the shared
 * subtraction and return BOSS_NONE directly (the ROM's own `_080233F0`
 * case-4 slot points straight at `_080233F8`'s `bx lr`, bypassing
 * `_080233F6`'s `subs r0,#6` cases 0-3 share - a plain
 * `return 9 - 6;`-style fold collapses that shared instruction away,
 * so the subtraction has to stay a genuine runtime step). */
s32 GetBossIndex(struct level_state *self)
{
    s32 idx = self->room.level;
    s32 result;

    switch (idx) {
    case LEVEL_DINGODILE:
        result = BOSS_HUD_ANIM_BASE + BOSS_DINGODILE;
        break;
    case LEVEL_N_GIN:
        result = BOSS_HUD_ANIM_BASE + BOSS_N_GIN;
        break;
    case LEVEL_TINY:
        result = BOSS_HUD_ANIM_BASE + BOSS_TINY;
        break;
    case LEVEL_NEO_CORTEX:
        result = BOSS_HUD_ANIM_BASE + BOSS_NEO_CORTEX;
        break;
    case LEVEL_MEGA_MIX:
    default:
        return BOSS_NONE;
    }
    return result - BOSS_HUD_ANIM_BASE;
}

/* Address-of-slot helper: level `idx`'s word in `progress.levels`. */
u8 *GetLevelFlags(struct level_state *self, s32 idx)
{
    return (u8 *)&self->progress.levels[idx];
}

/* Resolves the "current index" field (`room.level`, `self+0xc4`) into its own slot
 * address via `GetLevelFlags` - the address this file's `AddBrokenCrate`/
 * `PressSwitchCrate`/`CheckAllCratesBroken` all call "flags" and OR a bit into. */
u8 *GetCurrentLevelFlags(struct level_state *self)
{
    s32 idx = self->room.level;
    return GetLevelFlags(self, idx);
}

/* Getter for `self+0x70`, the counter `ResetCrateCount` resets. */
s32 GetCrateCount(struct level_state *self)
{
    return self->crateCount;
}

/* Bit-0 getter on the current level's word in the `saveData` copy of
 * the progress block (`idx` from `room.level`). Spelled as
 * `self + idx * 4 + offset`: taking `&...levels[idx]` inside the
 * snapshot adds the constant first, which the ROM doesn't. */
s32 IsCrystalSaved(struct level_state *self)
{
    s32 idx = self->room.level;
    u8 *addr = (u8 *)self + idx * 4 + offsetof(struct level_state, saveData.levels);

    return (u32)(*addr << 31) >> 31;
}

/* Collects one wumpa fruit (`wumpa`, +0x6c): at 100 it wraps to 0 and
 * adds a life (`lives`, +0x74, capped at 99) with a ping to
 * `gHud` via `ShowHudLives`; either way, always pings it
 * again via `ShowHudWumpa`. */
void CollectWumpa(struct level_state *self)
{
    s32 v = self->wumpa + 1;

    self->wumpa = v;
    if (v > 0x63) {
        self->wumpa = 0;
        if (self->lives <= 0x62) {
            self->lives += 1;
        }
        gHud->ShowLives();
    }
    gHud->ShowWumpa();
}

/* Just the "add a life (`lives`), ping `ShowHudLives`" half of
 * `CollectWumpa` above, standalone. */
void AddLife(struct level_state *self)
{
    if (self->lives <= 0x62) {
        self->lives += 1;
    }
    gHud->ShowLives();
}

/* GitHub issue #37: closes the loop on the `self+0x1c0`/`0x1c4`
 * counter-notification chain (`docs/rom_map.md`'s "Coverage check and
 * eight more small reads" section) - the consumer/trigger side of the
 * 15-slot table's `SpawnCrateGemMarker` writer, forwarding into `SpawnCrateGem`
 * alongside `PressSwitchCrate`/`AddBrokenCrate`'s own threshold-cross paths
 * above (identical shape: gated by the same `self+0x70 == self+0xbc`
 * counter/threshold pair, `IsInBonusRound`/`IsInGemPath` readiness checks,
 * then either OR a bit into `GetCurrentLevelFlags`'s slot or forward
 * `self+0x1c0`/`0x1c4` to `SpawnCrateGem`). Only caller is
 * `RunRoom`'s dispatch opener (`run_room.cpp`), which passes
 * `*gLevelState` as `self`. */
void CheckAllCratesBroken(struct level_state *self)
{
    if (self->crateCount == self->crateTotal && !IsInBonusRound(self) && !IsInGemPath(self)) {
        const struct level_room *level = self->room.cat;

        if (level->kind == ROOM_KIND_CATEGORY) {
            u8 *flags = GetCurrentLevelFlags(self);
            s32 mask = LEVEL_FLAG_CRATE_GEM;
            s32 value = *flags;
            mask |= value;
            *flags = mask;
        } else {
            SpawnCrateGem(0xffff, self->crateGemX, self->crateGemY, 0);
        }
    }
}

/* Sets `self->0x1bc` (a Q-format camera/position field paired with the
 * `SetCrateGemPos` two-word setter below). */
void SetGemPlatform(struct level_state *self, void *value)
{
    self->gemPlatform = value;
}

/* Sets `self->0x1b8`, the companion field to `SetGemPlatform` above. */
void SetBonusPlatform(struct level_state *self, void *value)
{
    self->bonusPlatform = value;
}

/* Copies a `{x, y}` pair into `self->0x1c0`/`0x1c4`. */
void SetCrateGemPos(struct level_state *self, s32 *point)
{
    s32 *dst = &self->crateGemX;
    s32 y = point[1];
    s32 x = point[0];

    dst[0] = x;
    dst[1] = y;
}

/* Sets `self->0xa6` to 1 unless `IsInGemPath` says it's already set. */
void RequestGemPath(struct level_state *self)
{
    if (!IsInGemPath(self)) {
        self->inGemPath = 1;
    }
}

/* Sets `self->0xa4` to 1 unless `IsInBonusRound` says it's already set. */
void RequestBonusRound(struct level_state *self)
{
    if (!IsInBonusRound(self)) {
        self->inBonusRound = 1;
    }
}

/* Restores `self->0x70`/`0xa9` from their `0xcc`/`0xd0` shadow copies,
 * then copies the `0xe4`-`0x14b` snapshot block back over `self`'s own
 * first `0x68` bytes - the inverse direction of `SetCheckpoint`/
 * `SetCheckpointAtPlayer`'s "stash a snapshot at +0xe4" below. */
void RestoreCheckpoint(struct level_state *self)
{
    u8 tmp;

    self->crateCount = self->room.checkpointCrateCount;
    tmp = self->room.checkpointSwitchPressed;
    self->switchPressed = tmp;
    MemCopy32(&self->progress, &self->checkpointData, sizeof(struct game_progress));
}

/* Re-arms a level/checkpoint transition: stores `flag` at `self->0xe0`,
 * refreshes the `0xcc`/`0xd0` shadow pair (crate count and switch
 * flag), clears `spawnAtStart` and `deaths`, snapshots `pair`
 * into `self->0xd4`/`0xd8`, flushes two spans of the
 * `gEntityFlags` bitmap via the `CpuSet` wrapper, then stashes the
 * `0xe4`-byte snapshot block (see `RestoreCheckpoint` above). */
void SetCheckpoint(void *selfArg, s32 flag, s32 *pair)
{
    struct level_state *self = (struct level_state *)selfArg;
    struct entity_flags *flags;
    u8 tmp;

    self->room.checkpointFlags = flag;
    self->room.checkpointCrateCount = GetCrateCount(self);
    tmp = self->switchPressed;
    self->room.checkpointSwitchPressed = tmp;
    ClearSpawnAtStart(self);
    ResetDeaths(self);
    {
        s32 *dst = &self->room.checkpointX;
        s32 px = pair[0];
        s32 py = pair[1];
        dst[0] = px;
        dst[1] = py;
    }

    flags = gEntityFlags;
    {
        void *a = flags->bits0Copy;
        void *b = flags->bits0;
        // the ROM loads the control word fresh for each call: the r2 pin
        // (SetCheckpointAtPlayer, bonus_round.c, has the reason)
        MATCH_HOLD_REG(u32, ctrl, r2) = CPU_SET_32BIT | 0x40;
        CpuSet(a, b, ctrl);
    }
    CpuSet(flags->bits1Copy, flags->bits1, CPU_SET_32BIT | 0x40);

    MemCopy32(&self->checkpointData, &self->progress, sizeof(struct game_progress));
}

/* When `flag` is set, accumulates `self->0xb4` into `self->0x70`,
 * re-arms the start spawn, flushes the tile record cache
 * (`gHud`) using `self->0xbc`, re-syncs the player's
 * stored position (`gPlayer`) from `self->0xd4`/`0xd8`, and
 * re-runs `SetCheckpointAtPlayer`; otherwise just calls `ResetCrateCount`. */
void EndGemPath(struct level_state *self, u8 flag)
{
    if (flag != 0) {
        self->crateCount += self->savedCrateCount;
        ClearInGemPath(self);
        ClearSpawnAtStart(self);
        SetGemPathDone(self);
        gHud->SetCrateTotal(self->crateTotal);
        {
            Player *player = gPlayer;
            s32 *p = &self->room.checkpointX;
            SetEntityPos(player, p[0], p[1]);
        }
        SetCheckpointAtPlayer(self, self->room.checkpointFlags);
    } else {
        ResetCrateCount(self);
    }
}

void PlayNewGameCutscene(void *self)
{
    PlayCutscene(self, 2);
}

void PlayIntroCutscene(void *self)
{
    PlayCutscene(self, 1);
    gAudioContext->StopSfx(SFX_SPACE_STATION_AMBIENCE);
}

/* The company logos (CompanyLogos, frontend.hpp; 0x44c bytes): made, run
 * and deleted. The constructor is empty (InitCompanyLogos,
 * language_select.cpp), and g++'s `new` keeps the block in r0 across its
 * call, which the C could only write as an asm `bl` with a pinned r0. */
void ShowCompanyLogos(void *unused)
{
    CompanyLogos *logos = new CompanyLogos;

    logos->Run();
    delete logos;
}

void PlayBootCutscene(void *self)
{
    PlayCutscene(self, 0);
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/). */
void nullsub_24(void)
{
}

/* Unpacks the packed halfword of `saveData` (see `PackSaveData`'s
 * inverse below) into `lives`/`wumpa`/`maskLevel`, but first refreshes
 * the snapshot itself: copies `src` into `self`'s progress block, then
 * re-copies that into `saveData`. `wumpa` is read off `self` (the ROM's
 * `ldrb [self, 0x14d]`), the other two through `snap`. */
void UnpackSaveData(struct level_state *self, const struct game_progress *src)
{
    struct game_progress *snap = &self->saveData;

    MemCopy32(&self->progress, src, sizeof(struct game_progress));
    MemCopy32(snap, &self->progress, sizeof(struct game_progress));

    self->lives = snap->lives;
    self->wumpa = self->saveData.wumpa;
    self->maskLevel = snap->maskLevel;
}

/* Packs `lives`/`wumpa`/`maskLevel` back into `saveData`'s halfword -
 * the inverse of `UnpackSaveData` above - and returns `&self->saveData`.
 * As there, `wumpa` goes through `self` (`strb [self, 0x14d]`). */
struct game_progress *PackSaveData(void *selfArg)
{
    struct level_state *self = (struct level_state *)selfArg;
    s32 lives = self->lives;
    struct game_progress *snap = &self->saveData;

    snap->lives = lives;
    self->saveData.wumpa = self->wumpa;
    snap->maskLevel = self->maskLevel;
    return snap;
}

/* Makes `gLevelStateSingleton` (a LevelState, level_state.hpp; its
 * constructor is InitLevelState) the first time it's needed, then
 * returns it. Its own
 * file: ROM-adjacent to `PlayRoom` (now matched, `play_room.cpp`)
 * and the still-raw `RunRoom` on both sides
 * (asm/code_3_2_17_236ec.s before it, `PlayRoom`/
 * asm/code_3_2_17_23a1c.s after), so it can't share an object file
 * with either matched neighbor without splitting the ROM-contiguous
 * layout. */
struct level_state *GetLevelState(void)
{
    if (gLevelStateSingleton == NULL) {
        gLevelStateSingleton = new LevelState;
    }
    return gLevelStateSingleton;
}
