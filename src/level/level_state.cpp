#include "hud.hpp"
#include "frontend.hpp"
#include "level_state.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "boss_ctrl.hpp"

extern "C" {
#include "core.h"
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

/* The level state's accessors and helpers: LevelState's methods
 * (level_state.hpp; #750), under their C names (cxx_symbols.txt), with
 * the free functions nullsub_24 and GetLevelState. C++ since the #664
 * cleanup, for GetLevelState's `new LevelState` and ShowCompanyLogos'
 * `new CompanyLogos`; the methods were C functions taking the level
 * state as `self` until #750, and compile to the same code. */

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
void LevelState::FreezeLevelClock(s32 secs)
{
    u8 recordId, slot;

    gAudioContext->PlaySfx(SFX_CLOCK, 0x100);

    countdown += secs * 60;

    slot = gPaletteCache->GetSlot(gSpriteBankSet->Anims(47)[1].paletteId);

    recordId = gSpriteBankSet->Anims(47)[4].paletteId;
    gPaletteCache->LoadSlot(slot, recordId);

    if (room.cat->kind == ROOM_KIND_CATEGORY) {
        gPaletteCache->UploadSlot(slot);
    }
}

/* Countdown-gated periodic event trigger (docs/rom_map.md, "A per-level
 * completion-time cascade..."): decrements `+0xa0`'s countdown and,
 * on reaching 0, fires record 47's spawn (`FreezeLevelClock`'s sibling,
 * using animation 1's palette for both the lookup and the slot fill
 * this time). While the countdown is already 0, instead runs a cascading
 * digit-counter carry over `+0x9c`/`0x98`/`0x94`/`0x90` (thresholds
 * `5`/`9`/`0x3b`/`0x63`) - shaped like a minutes:seconds:centiseconds
 * odometer, saturating (not wrapping) once the top field hits its cap.
 *
 * The trigger half reads bank 47 the way `FreezeLevelClock` does (see
 * its comment above). The countdown pointer `addr` is *reused* (reassigned, not
 * redeclared) for the digit-cascade's own address-chasing in the `else`
 * branch, which is what makes gcc emit the ROM's `subs r1,#4`
 * chain-decrement instead of recomputing `+0x98`/`+0x94` fresh
 * from `this` each time. `newCountdown` is its own local: decrementing
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
void LevelState::TickLevelClock()
{
    s32 *addr = &countdown;
    s32 cur = *addr;

    if (cur != 0) {
        s32 newCountdown = cur - 1;
        *addr = newCountdown;

        if (newCountdown == 0) {
            u8 recordId, slot;

            slot = gPaletteCache->GetSlot(gSpriteBankSet->Anims(47)[1].paletteId);

            recordId = gSpriteBankSet->Anims(47)[1].paletteId;
            gPaletteCache->LoadSlot(slot, recordId);

            if (room.cat->kind == ROOM_KIND_CATEGORY) {
                gPaletteCache->UploadSlot(slot);
            }
        }
    } else {
        s32 *save1;
        s32 val1;

        addr = &frames;
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
                    s32 *c4 = &minutes;

                    if (*c4 == 0x63) {
                        return;
                    }
                    *c4 += 1;
                    *addr = cur;
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

void LevelState::AddBrokenCrate()
{
    crateCount += 1;

    if (crateCount == crateTotal) {
        if (!IsInBonusRound() && !IsInGemPath()) {
            const struct level_room *level = room.cat;

            if (level->kind == ROOM_KIND_CATEGORY) {
                u8 *flags = GetCurrentLevelFlags();
                /* Separate `mask`/`value` locals reproduce the ROM's
                 * "build the OR mask before loading the byte" order - a plain
                 * `*flags |= LEVEL_FLAG_CRATE_GEM;` loads the byte first regardless of
                 * statement order (see docs/workflow.md step 7). */
                s32 mask = LEVEL_FLAG_CRATE_GEM;
                s32 value = *flags;
                mask |= value;
                *flags = mask;
            } else {
                /* `u16` copies: the ROM loads both halves before the
                 * `0xffff`; passing the fields directly loads the constant
                 * first, CheckAllCratesBroken's order. */
                u16 b = crateGemX;
                u16 c = crateGemY;
                SpawnCrateGem(0xffff, b, c, 0);
            }
        }
    }

    if (timeTrial == 0) {
        gHud->ShowCrates();
    }
}

void LevelState::PressSwitchCrate()
{
    switchPressed = 1;

    if (IsInBonusRound()) {
        savedCrateCount += pendingSwitchCrates;
        return;
    }

    crateCount += pendingSwitchCrates;

    if (crateCount != crateTotal) {
        return;
    }

    if (!IsInBonusRound() && !IsInGemPath()) {
        const struct level_room *level = room.cat;

        if (level->kind == ROOM_KIND_CATEGORY) {
            u8 *flags = GetCurrentLevelFlags();
            s32 mask = LEVEL_FLAG_CRATE_GEM;
            s32 value = *flags;
            mask |= value;
            *flags = mask;
        } else {
            u16 b = crateGemX;
            u16 c = crateGemY;
            SpawnCrateGem(0xffff, b, c, 0);
        }
    }
}

void *LevelState::GetBonusPlatform()
{
    return bonusPlatform;
}

void LevelState::SetCrateAssistDeaths(s32 value)
{
    crateAssistDeaths = value;
}

void LevelState::SetMaskAssistDeaths(s32 value)
{
    maskAssistDeaths = value;
}

/* Setter of `unusedAssistDeaths`, a third assist-deaths threshold: the
 * field sits next to the other two (in the reverse of this accessor
 * order, as theirs do), and UpdateGameFrame sets all three to the same
 * default 5 together. The level table has no value for it and only the
 * unused GetUnusedAssistDeaths reads it, so which assist it was meant
 * for is unknown. */
void LevelState::SetUnusedAssistDeaths(s32 value)
{
    unusedAssistDeaths = value;
}

s32 LevelState::GetCrateAssistDeaths()
{
    return crateAssistDeaths;
}

s32 LevelState::GetMaskAssistDeaths()
{
    return maskAssistDeaths;
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/). Getter
 * of `unusedAssistDeaths` (SetUnusedAssistDeaths). */
s32 LevelState::GetUnusedAssistDeaths()
{
    return unusedAssistDeaths;
}

void LevelState::AddPendingSwitchCrates(s32 delta)
{
    pendingSwitchCrates += delta;
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/), nor is
 * TestUnusedFlags: ORs `mask` into `unusedFlags`, which InitLevelState zeroes and
 * nothing else touches. */
void LevelState::SetUnusedFlags(s32 mask)
{
    unusedFlags |= mask;
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/). Whether
 * any bit of `mask` is set in `unusedFlags`. */
s32 LevelState::TestUnusedFlags(s32 mask)
{
    s32 x = unusedFlags & mask;
    return (u32)(-x | x) >> 31;
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the
 * data tables for ClearPowers). Clears all four power bits (`flags`
 * bits 4-7) that GiveTurboRun/GiveSuperBodySlam/GiveTornadoSpin/
 * GiveDoubleJump set. */
void LevelState::ClearPowers()
{
    /* The step-by-step accumulator/mask split reproduces the ROM's
     * sequence (a fresh mov+neg pair for -0x41, then `tmp += 0x20`); a
     * plain `&= ~0xf0` compiles to a different one. */
    s32 acc = -0x11;
    s32 tmp = progress.flags;
    acc &= tmp;
    tmp = -0x41;
    acc &= tmp;
    tmp += 0x20;
    acc &= tmp;
    tmp = 0x7f;
    acc &= tmp;
    progress.flags = acc;
}

/* Each of these four builds its OR mask into `r1` *before* loading the
 * byte into `r2` (the ROM's `movs r1,#N; ldrb r2,[r0,#2]` order) - a
 * plain `*flags |= N;` loads the byte first regardless of statement
 * order, so the mask and the value are separate locals (see
 * docs/workflow.md step 7, and the identical fix on `AddBrokenCrate`/
 * `PressSwitchCrate`'s `*flags |= 2;` above). */
void LevelState::GiveTornadoSpin()
{
    s32 mask = 0x40;
    s32 value = progress.flags;
    mask |= value;
    progress.flags = mask;
}

void LevelState::GiveSuperBodySlam()
{
    s32 mask = 0x20;
    s32 value = progress.flags;
    mask |= value;
    progress.flags = mask;
}

void LevelState::GiveTurboRun()
{
    s32 mask = 0x10;
    s32 value = progress.flags;
    mask |= value;
    progress.flags = mask;
}

void LevelState::GiveDoubleJump()
{
    s32 mask = 0x80;
    s32 value = progress.flags;
    mask |= value;
    progress.flags = mask;
}

/* These three read a single flag bit back out of `+2` as a plain
 * 0/1 value. Writing them as `(x >> n) & 1` compiles an extra `and`
 * this compiler doesn't need - the ROM instead isolates the bit by
 * shifting it up into the sign bit and shifting back down unsigned,
 * the same branchless idiom already used for `TestUnusedFlags`'s
 * `!= 0` test (see docs/workflow.md step 7 / matching.md). */
s32 LevelState::HasTornadoSpin()
{
    return (u32)(progress.flags << 25) >> 31;
}

s32 LevelState::HasSuperBodySlam()
{
    return (u32)(progress.flags << 26) >> 31;
}

s32 LevelState::HasTurboRun()
{
    return (u32)(progress.flags << 27) >> 31;
}

/* GitHub issues #35/#36: 0x080231CC-0x08023488, the remainder of the
 * UpdateGameFrame-MainLoop cluster's "level" object accessor family
 * (`gLevelState`) - fully contiguous with the functions above (no
 * ldscript.txt change needed, this is still the same class and
 * still the same object file). See docs/matching/archive/issue-35-36-0x080231cc-game-loop.md
 * for the full write-up. Bit-7 getter for the `+2` flags byte this
 * file's own family already covers bits 4-6 of. */
s32 LevelState::HasDoubleJump()
{
    return progress.flags >> 7;
}

/* +0x70/+0xbc form a counter/threshold pair (`AddBrokenCrate`/
 * `PressSwitchCrate` above, `CheckAllCratesBroken` below); this resets the counter. */
void LevelState::ResetCrateCount()
{
    crateCount = 0;
}

void LevelState::ResetWumpa()
{
    wumpa = 0;
}

void LevelState::ResetLives()
{
    lives = 5;
}

/* Sets the Aku Aku mask level (`maskLevel`, +0x78, 0-3): level `3` (the
 * invincibility mask) always fires a jingle (`StartSong(
 * gAudioContext, SONG_DRUMS)`) and skips the rest; leaving level 3 re-fires
 * `room.PlayRoomMusic()` once. Either way `maskLevel` ends up
 * holding `state`. */
void LevelState::SetMaskLevel(s32 state)
{
    if (state == MASK_LEVEL_INVINCIBLE) {
        gAudioContext->StartSong(SONG_DRUMS);
    } else if (maskLevel == MASK_LEVEL_INVINCIBLE) {
        maskLevel = state;
        room.PlayRoomMusic();
    }
    maskLevel = state;
}

/* Plain setter for the same `+0x74` field `ResetLives` above
 * hardcodes to `5`. */
void LevelState::SetLives(s32 value)
{
    lives = value;
}

/* Advances the `+0x78` latch by one via `SetMaskLevel`. */
void LevelState::RaiseMaskLevel()
{
    s32 next = maskLevel + 1;
    SetMaskLevel(next);
}

/* Outside time trials (`timeTrial`, +0x8c): loses a life (`lives`,
 * +0x74) and, while any are left, pings `gHud`
 * (`ShowHudLives`). */
void LevelState::LoseLife()
{
    if (timeTrial == 0) {
        s32 v = lives - 1;
        lives = v;

        if (v >= 0) {
            gHud->ShowLives();
        }
    }
}

/* Getter for `+0x6c`, the counter `ResetWumpa` clears. */
s32 LevelState::GetWumpa()
{
    return wumpa;
}

/* Getters for the "seconds"/"minutes" tier of the digit-cascade odometer
 * `TickLevelClock` above already documents (`+0x9c`/`0x98`/`0x94`/
 * `0x90`, thresholds 5/9/0x3b/0x63) - this trio covers its bottom three
 * tiers. */
s32 LevelState::GetClockTenths()
{
    return tenths;
}

s32 LevelState::GetClockSeconds()
{
    return seconds;
}

s32 LevelState::GetClockMinutes()
{
    return minutes;
}

/* `+0xa4`-`+0xa9`: six flag bytes (bonus round, gem path,
 * spawn at the start marker, switch crate), each with a getter and a
 * clear (some also a set-to-1). */
u8 LevelState::IsGemPathDone()
{
    return gemPathDone;
}

void LevelState::ClearGemPathDone()
{
    gemPathDone = 0;
}

void LevelState::SetGemPathDone()
{
    gemPathDone = 1;
}

u8 LevelState::IsInGemPath()
{
    return inGemPath;
}

void LevelState::ClearInGemPath()
{
    inGemPath = 0;
}

u8 LevelState::IsBonusRoundDone()
{
    return bonusRoundDone;
}

void LevelState::ClearBonusRoundDone()
{
    bonusRoundDone = 0;
}

void LevelState::SetBonusRoundDone()
{
    bonusRoundDone = 1;
}

u8 LevelState::IsInBonusRound()
{
    return inBonusRound;
}

void LevelState::ClearInBonusRound()
{
    inBonusRound = 0;
}

u8 LevelState::IsSwitchPressed()
{
    return switchPressed;
}

void LevelState::ClearSwitchPressed()
{
    switchPressed = 0;
}

void LevelState::ClearTimeTrial()
{
    timeTrial = 0;
}

/* `deaths` (+0x7c): maskless hits since the last checkpoint
 * (AddDeath is only called from PlayerHandleEvent). */
s32 LevelState::GetDeaths()
{
    return deaths;
}

void LevelState::AddDeath()
{
    deaths = deaths + 1;
}

void LevelState::ResetDeaths()
{
    deaths = 0;
}

u8 LevelState::GetSpawnAtStart()
{
    return spawnAtStart;
}

void LevelState::ClearSpawnAtStart()
{
    spawnAtStart = 0;
}

/* Resets `deaths` and arms `spawnAtStart`, so the player is placed on
 * the room's start marker. */
void LevelState::ArmStartSpawn()
{
    ResetDeaths();
    spawnAtStart = 1;
}

/* Sets the boss's controller (`boss`; the spawners' BossCtrl,
 * spawn_bosses.cpp), right after the `+0x1c0`/`0x1c4` pair
 * `CheckAllCratesBroken` below reads. */
void LevelState::SetLevelBoss(void *value)
{
    boss = (BossCtrl *)value;
}

/* Plain getter/getter/setter trio for `room.roomIndex` (`+0xc8`, the
 * current room's index in the level's room list; SpawnRoomExit tests it
 * for the first room) and `room.level` (`+0xc4`) - the
 * latter is the "current index" field `GetBossHealth`/`GetBossIndex`/
 * `GetCurrentLevelFlags`/`IsCrystalSaved` below all read. */
s32 LevelState::GetRoomIndex()
{
    return room.roomIndex;
}

s32 LevelState::GetCurrentLevel()
{
    return room.level;
}

void LevelState::SetCurrentLevel(s32 value)
{
    room.level = value;
}

/* Five thin two-argument wrappers that drop `this` entirely and forward
 * straight to one of `LevelHasYellowGemEntity`/`34`/`40`/`4C`/`58` (the medal
 * "flag index" wrappers, `level_query.cpp`). */
s32 LevelState::LevelHasYellowGem(s32 idx)
{
    return LevelHasYellowGemEntity(idx);
}

s32 LevelState::LevelHasBlueGem(s32 idx)
{
    return LevelHasBlueGemEntity(idx);
}

s32 LevelState::LevelHasGreenGem(s32 idx)
{
    return LevelHasGreenGemEntity(idx);
}

s32 LevelState::LevelHasRedGem(s32 idx)
{
    return LevelHasRedGemEntity(idx);
}

s32 LevelState::LevelHasGemPathGem(s32 idx)
{
    return LevelHasGemPathGemEntity(idx);
}

/* Dispatches on the "current index" field `room.level` (`+0xc4`): index `0x15` fires
 * the actor-part singleton lifetime counter (`GetHovercraftPartsLeft`,
 * `hovercraft_parts.cpp`); indices `0x14`/`0x16`/`0x17` instead compute
 * `3 - boss->counter` (the boss controller `SetLevelBoss` above
 * sets; its counter is the hits taken); anything else returns `0`. */
s32 LevelState::GetBossHealth()
{
    s32 idx = room.level;

    switch (idx) {
    case LEVEL_N_GIN:
        return GetHovercraftPartsLeft();
    case LEVEL_DINGODILE:
        {
            BossCtrl *p = boss;
            return 3 - p->counter;
        }
    case LEVEL_TINY:
        {
            BossCtrl *p = boss;
            return 3 - p->counter;
        }
    case LEVEL_NEO_CORTEX:
        {
            BossCtrl *p = boss;
            return 3 - p->counter;
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
s32 LevelState::GetBossIndex()
{
    s32 idx = room.level;
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
u8 *LevelState::GetLevelFlags(s32 idx)
{
    return (u8 *)&progress.levels[idx];
}

/* Resolves the "current index" field (`room.level`, `+0xc4`) into its own slot
 * address via `GetLevelFlags` - the address this file's `AddBrokenCrate`/
 * `PressSwitchCrate`/`CheckAllCratesBroken` all call "flags" and OR a bit into. */
u8 *LevelState::GetCurrentLevelFlags()
{
    s32 idx = room.level;
    return GetLevelFlags(idx);
}

/* Getter for `+0x70`, the counter `ResetCrateCount` resets. */
s32 LevelState::GetCrateCount()
{
    return crateCount;
}

/* Bit-0 getter on the current level's word in the `saveData` copy of
 * the progress block (`idx` from `room.level`). Spelled as
 * `this + idx * 4 + offset`: taking `&...levels[idx]` inside the
 * snapshot adds the constant first, which the ROM doesn't. */
s32 LevelState::IsCrystalSaved()
{
    s32 idx = room.level;
    u8 *addr = (u8 *)this + idx * 4 + offsetof(LevelState, saveData.levels);

    return (u32)(*addr << 31) >> 31;
}

/* Collects one wumpa fruit (`wumpa`, +0x6c): at 100 it wraps to 0 and
 * adds a life (`lives`, +0x74, capped at 99) with a ping to
 * `gHud` via `ShowHudLives`; either way, always pings it
 * again via `ShowHudWumpa`. */
void LevelState::CollectWumpa()
{
    s32 v = wumpa + 1;

    wumpa = v;
    if (v > 0x63) {
        wumpa = 0;
        if (lives <= 0x62) {
            lives += 1;
        }
        gHud->ShowLives();
    }
    gHud->ShowWumpa();
}

/* Just the "add a life (`lives`), ping `ShowHudLives`" half of
 * `CollectWumpa` above, standalone. */
void LevelState::AddLife()
{
    if (lives <= 0x62) {
        lives += 1;
    }
    gHud->ShowLives();
}

/* GitHub issue #37: closes the loop on the `+0x1c0`/`0x1c4`
 * counter-notification chain (`docs/rom_map.md`'s "Coverage check and
 * eight more small reads" section) - the consumer/trigger side of the
 * 15-slot table's `SpawnCrateGemMarker` writer, forwarding into `SpawnCrateGem`
 * alongside `PressSwitchCrate`/`AddBrokenCrate`'s own threshold-cross paths
 * above (identical shape: gated by the same `+0x70 == +0xbc`
 * counter/threshold pair, `IsInBonusRound`/`IsInGemPath` readiness checks,
 * then either OR a bit into `GetCurrentLevelFlags`'s slot or forward
 * `+0x1c0`/`0x1c4` to `SpawnCrateGem`). Only caller is
 * `RunRoom`'s dispatch opener (`run_room.cpp`), which passes
 * `*gLevelState` as `this`. */
void LevelState::CheckAllCratesBroken()
{
    if (crateCount == crateTotal && !IsInBonusRound() && !IsInGemPath()) {
        const struct level_room *level = room.cat;

        if (level->kind == ROOM_KIND_CATEGORY) {
            u8 *flags = GetCurrentLevelFlags();
            s32 mask = LEVEL_FLAG_CRATE_GEM;
            s32 value = *flags;
            mask |= value;
            *flags = mask;
        } else {
            SpawnCrateGem(0xffff, crateGemX, crateGemY, 0);
        }
    }
}

/* Sets `+0x1bc` (a Q-format camera/position field paired with the
 * `SetCrateGemPos` two-word setter below). */
void LevelState::SetGemPlatform(void *value)
{
    gemPlatform = value;
}

/* Sets `+0x1b8`, the companion field to `SetGemPlatform` above. */
void LevelState::SetBonusPlatform(void *value)
{
    bonusPlatform = value;
}

/* Copies a `{x, y}` pair into `+0x1c0`/`0x1c4`. */
void LevelState::SetCrateGemPos(s32 *point)
{
    s32 *dst = &crateGemX;
    s32 y = point[1];
    s32 x = point[0];

    dst[0] = x;
    dst[1] = y;
}

/* Sets `+0xa6` to 1 unless `IsInGemPath` says it's already set. */
void LevelState::RequestGemPath()
{
    if (!IsInGemPath()) {
        inGemPath = 1;
    }
}

/* Sets `+0xa4` to 1 unless `IsInBonusRound` says it's already set. */
void LevelState::RequestBonusRound()
{
    if (!IsInBonusRound()) {
        inBonusRound = 1;
    }
}

/* Restores `+0x70`/`0xa9` from their `0xcc`/`0xd0` shadow copies,
 * then copies the `0xe4`-`0x14b` snapshot block back over `this`'s own
 * first `0x68` bytes - the inverse direction of `SetCheckpoint`/
 * `SetCheckpointAtPlayer`'s "stash a snapshot at +0xe4" below. */
void LevelState::RestoreCheckpoint()
{
    u8 tmp;

    crateCount = room.checkpointCrateCount;
    tmp = room.checkpointSwitchPressed;
    switchPressed = tmp;
    MemCopy32(&progress, &checkpointData, sizeof(struct game_progress));
}

/* One span of the entity bitmap copied to its checkpoint copy: see
 * LevelState::SetCheckpointAtPlayer (bonus_round.cpp), which has the same
 * helper. Through it the control word is loaded fresh for each call, as
 * in the ROM (the C needed an r2 pin). */
static inline void CopyBitmapSpan(void *dst, void *src)
{
    CpuSet(dst, src, CPU_SET_32BIT | 0x40);
}

/* Re-arms a level/checkpoint transition: stores `flag` at `+0xe0`,
 * refreshes the `0xcc`/`0xd0` shadow pair (crate count and switch
 * flag), clears `spawnAtStart` and `deaths`, snapshots `pair`
 * into `+0xd4`/`0xd8`, flushes two spans of the
 * `gEntityFlags` bitmap via the `CpuSet` wrapper, then stashes the
 * `0xe4`-byte snapshot block (see `RestoreCheckpoint` above). */
void LevelState::SetCheckpoint(s32 flag, s32 *pair)
{
    struct entity_flags *flags;
    u8 tmp;

    room.checkpointFlags = flag;
    room.checkpointCrateCount = GetCrateCount();
    tmp = switchPressed;
    room.checkpointSwitchPressed = tmp;
    ClearSpawnAtStart();
    ResetDeaths();
    {
        s32 *dst = &room.checkpointX;
        s32 px = pair[0];
        s32 py = pair[1];
        dst[0] = px;
        dst[1] = py;
    }

    flags = gEntityFlags;
    CopyBitmapSpan(flags->bits0Copy, flags->bits0);
    CopyBitmapSpan(flags->bits1Copy, flags->bits1);

    MemCopy32(&checkpointData, &progress, sizeof(struct game_progress));
}

/* When `flag` is set, accumulates `+0xb4` into `+0x70`,
 * re-arms the start spawn, flushes the tile record cache
 * (`gHud`) using `+0xbc`, re-syncs the player's
 * stored position (`gPlayer`) from `+0xd4`/`0xd8`, and
 * re-runs `SetCheckpointAtPlayer`; otherwise just calls `ResetCrateCount`. */
void LevelState::EndGemPath(u8 flag)
{
    if (flag != 0) {
        crateCount += savedCrateCount;
        ClearInGemPath();
        ClearSpawnAtStart();
        SetGemPathDone();
        gHud->SetCrateTotal(crateTotal);
        {
            Player *player = gPlayer;
            s32 *p = &room.checkpointX;
            SetEntityPos(player, p[0], p[1]);
        }
        SetCheckpointAtPlayer(room.checkpointFlags);
    } else {
        ResetCrateCount();
    }
}

void LevelState::PlayNewGameCutscene()
{
    PlayCutscene(2);
}

void LevelState::PlayIntroCutscene()
{
    PlayCutscene(1);
    gAudioContext->StopSfx(SFX_SPACE_STATION_AMBIENCE);
}

/* The company logos (CompanyLogos, frontend.hpp; 0x44c bytes): made, run
 * and deleted. The constructor is empty (InitCompanyLogos,
 * company_logos_ctor.cpp), and g++'s `new` keeps the block in r0 across its
 * call, which the C could only write as an asm `bl` with a pinned r0. */
void LevelState::ShowCompanyLogos()
{
    CompanyLogos *logos = new CompanyLogos;

    logos->Run();
    delete logos;
}

void LevelState::PlayBootCutscene()
{
    PlayCutscene(0);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/, expected/
 * and every word-aligned Thumb pointer in baserom.gba). Empty; keeps the
 * nullsub_N name (docs/naming.md): no call, table slot or neighbour
 * shows what it stood for. */
void nullsub_24(void)
{
}

/* Unpacks the packed halfword of `saveData` (see `PackSaveData`'s
 * inverse below) into `lives`/`wumpa`/`maskLevel`, but first refreshes
 * the snapshot itself: copies `src` into `this`'s progress block, then
 * re-copies that into `saveData`. `wumpa` is read off `this` (the ROM's
 * `ldrb [this, 0x14d]`), the other two through `snap`. */
void LevelState::UnpackSaveData(const struct game_progress *src)
{
    struct game_progress *snap = &saveData;

    MemCopy32(&progress, src, sizeof(struct game_progress));
    MemCopy32(snap, &progress, sizeof(struct game_progress));

    lives = snap->lives;
    wumpa = saveData.wumpa;
    maskLevel = snap->maskLevel;
}

/* Packs `lives`/`wumpa`/`maskLevel` back into `saveData`'s halfword -
 * the inverse of `UnpackSaveData` above - and returns `&saveData`.
 * As there, `wumpa` goes through `this` (`strb [this, 0x14d]`). */
struct game_progress *LevelState::PackSaveData()
{
    s32 l = lives;
    struct game_progress *snap = &saveData;

    snap->lives = l;
    saveData.wumpa = wumpa;
    snap->maskLevel = maskLevel;
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
LevelState *GetLevelState(void)
{
    if (gLevelStateSingleton == NULL) {
        gLevelStateSingleton = new LevelState;
    }
    return gLevelStateSingleton;
}
