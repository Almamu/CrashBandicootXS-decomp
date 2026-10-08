#include "hud.hpp"
#include "frontend.hpp"
#include "level_state.hpp"
#include "player.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "level_state.h"
#include "actor.h"
#include <agb_syscall.h>
#include "hud.h"
#include "frontend.h"
#include "system.h"
#include "audio.h"
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
 * The ROM keeps `&gSpriteBankSet` and `&gPaletteCache` alive
 * across the `GetPaletteSlot` call in `r4`/`r7` (only 4 low registers
 * total, `r4`'s slot reused from the now-dead `seconds` parameter).
 * Blanket register pins for all of `self`/`seconds`/the two cached
 * globals/`slot` (mirroring the ROM's map directly) made things worse
 * - a pinned `slot` picked up a spurious truncate-and-remask on every
 * read, and a stray stack spill appeared for the `0x234` offset
 * constant. What actually closes it: 1) a `base` local snapshotting
 * `gPaletteCache`'s value *before* the first chase (not inline in
 * the call), so its evaluation lands in `r0` early exactly like the
 * ROM's `ldr r0,[r7]` and the chase is forced into `r1`; 2) a single
 * `MATCH_HOLD_REG(s32, off, r2)` pin for the `0x8d << 2` (`0x234`) field
 * offset in the *first* chase only, matching the ROM's `movs
 * r2,#0x8d; lsls r2,r2,#2` - this also stops gcc from caching that
 * constant in a register across the `GetPaletteSlot` call, which is what
 * was pushing something else into `r8`; 3) fresh, differently-named
 * locals (`p3b`/`headerb`/`recordb`) for the *second* chase instead of
 * reusing `p3`/`header`/`record` - reusing the same C variable names
 * across both chases made gcc "stick" the second chase's registers to
 * the first's choice instead of letting `r0`/`r1` fall out naturally
 * (`r0` is free again there since the first call's result is already
 * in `slot`/r6). No explicit pin is needed for `self`, `cache1`
 * (`&gPaletteCache`), or `slot` - they land in `r5`/`r7`/`r6`
 * purely from the resulting register pressure, matching the ROM
 * exactly. */
void FreezeLevelClock(struct level_state *self, s32 seconds)
{
    MATCH_HOLD_REG(s32, off, r2);
    PaletteCache *base;
    const struct sprite_bank_table *p3, *p3b;
    const struct sprite_bank *header, *headerb;
    const struct sprite_anim *record, *recordb;
    u8 recordId, slot;
    const struct level_room *level;

    PlaySfx(gAudioContext, SFX_CLOCK, 0x100);

    self->countdown += seconds * 60;

    base = gPaletteCache;
    p3 = gSpriteBankSet->table;
    header = p3->banks;
    off = 0x8d << 2; /* &banks[47] */
    record = *(const struct sprite_anim **)((u8 *)header + off);
    recordId = record[1].paletteId;
    slot = base->GetSlot(recordId);

    p3b = gSpriteBankSet->table;
    headerb = p3b->banks;
    recordb = headerb[47].anims;
    recordId = recordb[4].paletteId;
    gPaletteCache->LoadSlot(slot, recordId);

    level = self->room.cat;
    if (level->kind == ROOM_KIND_CATEGORY) {
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
 * The trigger half uses the same `FreezeLevelClock` register-pinning recipe
 * (see its comment above) for the `GetPaletteSlot`/`LoadPaletteSlot` cross-
 * call pair. Two more pins close the rest: `addr`/`countdown` pinned
 * to `r1`/`r3` reproduce the ROM's exact front-of-function map (the
 * countdown pointer and its loaded value), and that same `addr`
 * register variable is *reused* (reassigned, not redeclared) for the
 * digit-cascade's own address-chasing in the `else` branch, which is
 * what makes gcc emit the ROM's `subs r1,#4` chain-decrement instead
 * of recomputing `self+0x98`/`self+0x94` fresh from `self` each time.
 * `newCountdown` is pinned to `r0` because otherwise gcc decrements
 * `countdown`'s own register (`r3`) in place - functionally fine since
 * the two branches are mutually exclusive, but a different instruction
 * encoding (`subs r3,#1` vs the ROM's `subs r0,r3,#1`) than the ROM's.
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
    MATCH_HOLD_REG(s32 *, addr, r1) = &self->countdown;
    MATCH_HOLD_REG(s32, countdown, r3) = *addr;

    if (countdown != 0) {
        MATCH_HOLD_REG(s32, newCountdown, r0) = countdown - 1;
        *addr = newCountdown;

        if (newCountdown == 0) {
            MATCH_HOLD_REG(s32, off, r2);
            PaletteCache *base;
            const struct sprite_bank_table *p3, *p3b;
            const struct sprite_bank *header, *headerb;
            const struct sprite_anim *record, *recordb;
            u8 recordId, slot;
            const struct level_room *level;

            base = gPaletteCache;
            p3 = gSpriteBankSet->table;
            header = p3->banks;
            off = 0x8d << 2; /* &banks[47] */
            record = *(const struct sprite_anim **)((u8 *)header + off);
            recordId = record[1].paletteId;
            slot = base->GetSlot(recordId);

            p3b = gSpriteBankSet->table;
            headerb = p3b->banks;
            recordb = headerb[47].anims;
            recordId = recordb[1].paletteId;
            gPaletteCache->LoadSlot(slot, recordId);

            level = self->room.cat;
            if (level->kind == ROOM_KIND_CATEGORY) {
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
                /* Register pins reproduce the ROM's "build the OR
                 * mask before loading the byte" order - a plain
                 * `*flags |= LEVEL_FLAG_CRATE_GEM;` loads the byte first regardless of
                 * statement order (see docs/workflow.md step 7). */
                MATCH_HOLD_REG(s32, mask, r1) = LEVEL_FLAG_CRATE_GEM;
                MATCH_HOLD_REG(s32, value, r2) = *flags;
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
            MATCH_HOLD_REG(s32, mask, r1) = LEVEL_FLAG_CRATE_GEM;
            MATCH_HOLD_REG(s32, value, r2) = *flags;
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
    /* Register pins reproduce the ROM's exact accumulator/mask split -
     * see docs/workflow.md step 7 - a plain local otherwise lets gcc
     * reuse the still-live -0x11 constant to derive -0x41 via a single
     * SUB instead of a fresh mov+neg pair. */
    MATCH_HOLD_REG(s32, acc, r1) = -0x11;
    MATCH_HOLD_REG(s32, tmp, r2) = self->progress.flags;
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
 * order, so the mask/value roles are pinned explicitly (see
 * docs/workflow.md step 7, and the identical fix on `AddBrokenCrate`/
 * `PressSwitchCrate`'s `*flags |= 2;` above). */
void GiveTornadoSpin(struct level_state *self)
{
    MATCH_HOLD_REG(s32, mask, r1) = 0x40;
    MATCH_HOLD_REG(s32, value, r2) = self->progress.flags;
    mask |= value;
    self->progress.flags = mask;
}

void GiveSuperBodySlam(struct level_state *self)
{
    MATCH_HOLD_REG(s32, mask, r1) = 0x20;
    MATCH_HOLD_REG(s32, value, r2) = self->progress.flags;
    mask |= value;
    self->progress.flags = mask;
}

void GiveTurboRun(struct level_state *self)
{
    MATCH_HOLD_REG(s32, mask, r1) = 0x10;
    MATCH_HOLD_REG(s32, value, r2) = self->progress.flags;
    mask |= value;
    self->progress.flags = mask;
}

void GiveDoubleJump(struct level_state *self)
{
    MATCH_HOLD_REG(s32, mask, r1) = 0x80;
    MATCH_HOLD_REG(s32, value, r2) = self->progress.flags;
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

struct AudioContext;

/* Sets the Aku Aku mask level (`maskLevel`, +0x78, 0-3): level `3` (the
 * invincibility mask) always fires a jingle (`StartSong(
 * gAudioContext, SONG_DRUMS)`) and skips the rest; leaving level 3 re-fires
 * `PlayRoomMusic(&self->room)` once. Either way `maskLevel` ends up
 * holding `state`. */
void SetMaskLevel(void *selfArg, s32 stateArg)
{
    /* Register-pinned so the ROM's own `adds r4,r0,#0` (self) /
     * `adds r5,r1,#0` (state) copy order is reproduced - a plain pair
     * of locals lets this compiler swap the order since `state` is
     * referenced first, in the `if` condition below. */
    MATCH_HOLD_REG(struct level_state *, self, r4) = (struct level_state *)selfArg;
    MATCH_HOLD_REG(s32, state, r5) = stateArg;

    if (state == MASK_LEVEL_INVINCIBLE) {
        StartSong(gAudioContext, SONG_DRUMS);
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
void CheckAllCratesBroken(void *selfArg)
{
    MATCH_HOLD_REG(struct level_state *, self, r4) = (struct level_state *)selfArg;

    if (self->crateCount == self->crateTotal && !IsInBonusRound(self) && !IsInGemPath(self)) {
        const struct level_room *level = self->room.cat;

        if (level->kind == ROOM_KIND_CATEGORY) {
            u8 *flags = GetCurrentLevelFlags(self);
            MATCH_HOLD_REG(s32, mask, r1) = LEVEL_FLAG_CRATE_GEM;
            MATCH_HOLD_REG(s32, value, r2) = *flags;
            mask |= value;
            *flags = mask;
        } else {
            /* Register-pinned to reproduce the ROM's exact map: `magic`
             * (the 3rd `SpawnCrateGem` argument's true value) loaded into
             * r0 early rather than right before the call, and `off`
             * kept in r3 across its own +4 increment instead of being
             * recomputed from scratch for the second field - see
             * docs/workflow.md step 7 / matching_decomp_register_pinning
             * memory. */
            MATCH_HOLD_REG(s32, magic, r0) = 0xffff;
            MATCH_HOLD_REG(s32, off, r3) = 0xe0 << 1;
            MATCH_HOLD_REG(u16 *, addr1, r1) = (u16 *)((u8 *)self + off); /* &self->crateGemX */
            u16 b = *addr1;
            MATCH_HOLD_REG(u16 *, addr2, r2);
            u16 c;
            MATCH_KEEP_VOLATILE(b);
            off += 4;
            MATCH_KEEP_VOLATILE(off);
            addr2 = (u16 *)((u8 *)self + off); /* &self->crateGemY */
            c = *addr2;
            SpawnCrateGem(magic, b, c, 0);
        }
    }
}

struct AudioContext;

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
void SetCheckpoint(void *selfArg, s32 flag, s32 *pairArg)
{
    MATCH_HOLD_REG(struct level_state *, self, r5) = (struct level_state *)selfArg;
    MATCH_HOLD_REG(s32 *, pair, r4) = pairArg;
    u8 tmp;

    self->room.checkpointFlags = flag;
    self->room.checkpointCrateCount = GetCrateCount(self);
    tmp = self->switchPressed;
    self->room.checkpointSwitchPressed = tmp;
    ClearSpawnAtStart(self);
    ResetDeaths(self);
    {
        MATCH_HOLD_REG(s32 *, dst, r2) = &self->room.checkpointX;
        MATCH_HOLD_REG(s32, px, r0) = pair[0];
        MATCH_HOLD_REG(s32, py, r1) = pair[1];
        dst[0] = px;
        dst[1] = py;
    }

    pair = (s32 *)gEntityFlags;
    {
        void *a = (u8 *)pair + 0x108;
        void *b = (u8 *)pair + 8;
        MATCH_HOLD_REG(u32, ctrl, r2) = CPU_SET_32BIT | 0x40;
        CpuSet(a, b, ctrl);
    }
    {
        void *a = (u8 *)pair + 0x308;
        void *b = (u8 *)pair + 0x208;
        MATCH_HOLD_REG(u32, ctrl, r2) = CPU_SET_32BIT | 0x40;
        CpuSet(a, b, ctrl);
    }

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
    StopSfx(gAudioContext, SFX_SPACE_STATION_AMBIENCE);
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

/* Unpacks the packed halfword at `self->0x14c`/`0x14d` (see
 * `PackSaveData`'s inverse below) into `self->0x74`/`0x6c`/`0x78`, but
 * first refreshes the snapshot itself: copies `src` into `self`'s first
 * `0x68` bytes, then re-copies `self` into the `0x14c`-based snapshot
 * block. */
void UnpackSaveData(struct level_state *self, const struct game_progress *src)
{
    MATCH_HOLD_REG(u8 *, snap, r5) = self->saveData.packedStats;
    /* Register pins reproduce the ROM's exact "freshly loaded value in
     * one register, shifted result in another" shape for both the byte
     * and halfword extracts below (see docs/workflow.md step 7). */
    MATCH_HOLD_REG(u8, raw, r1);
    MATCH_HOLD_REG(s32, val, r0);
    MATCH_HOLD_REG(u16, packed, r5);

    MemCopy32(&self->progress, src, sizeof(struct game_progress));
    MemCopy32(snap, &self->progress, sizeof(struct game_progress));

    raw = *snap;
    val = PACKED_STATS_LIVES(raw);
    self->lives = val;

    self->wumpa = self->saveData.packedStats[1] >> 1;

    packed = *(u16 *)snap;
    val = PACKED_STATS_MASK_LEVEL(packed);
    self->maskLevel = val;
}

/* Packs `self->0x74`/`0x6c`/`0x78` back into the halfword at
 * `self->0x14c`/`0x14d` - the inverse of `UnpackSaveData` above - and
 * returns the `self->0x14c` snapshot, `&self->saveData`.
 *
 * Register-pinned to reproduce three ROM-specific shapes plain C
 * phrasing alone didn't reach (see docs/workflow.md step 7):
 *  - `self` stays live in r3 across the whole function (natural
 *    codegen instead folds `self` into each field access as an
 *    immediate-offset addressing mode).
 *  - `self->0x14d` is addressed via register+register indexing (a
 *    literal `0x14D` loaded once into r5, added to r3 right at the
 *    `ldrb`/`strb`) rather than through a precomputed pointer -
 *    reproduced with two opaque `asm volatile` accesses.
 *  - the first field's `~0x7f` mask is materialized as a full 32-bit
 *    `0x80; neg` pair (the same "freshly loaded value and its
 *    transformed result in different registers" idiom as
 *    `UnpackSaveData`) rather than narrowed to an 8-bit `#0x80` AND the
 *    way this compiler's optimizer does when it can prove the masked
 *    operand is byte-ranged - reproduced with an opaque `asm volatile`
 *    for just that mask. Because the return value (`self+0x14c`) ends
 *    up already sitting in r0 at the end, the epilogue's LR-restore
 *    register naturally lands on r1 instead of r0, matching the ROM's
 *    `pop {r1}; bx r1` without any extra hint. */
struct game_progress *PackSaveData(void *selfArg)
{
    MATCH_HOLD_REG(struct level_state *, self, r3) = (struct level_state *)selfArg;
    MATCH_HOLD_REG(struct game_progress *, snap, r0);
    u8 byte0;
    u16 packed;
    MATCH_HOLD_REG(s32, t, r2);

    t = self->lives;
    snap = &self->saveData;
    t &= PACKED_STATS_LIVES_MASK;
    {
        MATCH_HOLD_REG(s32, mask, r1);
        asm volatile("mov %0, #0x80\n\tneg %0, %0" : "=r"(mask));
        mask &= snap->packedStats[0];
        byte0 = mask | t;
    }
    snap->packedStats[0] = byte0;

    {
        s32 field6c = self->wumpa;
        MATCH_HOLD_REG(u32, off, r5) = offsetof(struct level_state, saveData.packedStats[1]);
        s32 shifted = field6c << 1;
        MATCH_HOLD_REG(s32, one, r1) = 1;
        MATCH_HOLD_REG(u32, raw, r4);
        asm volatile("ldrb %0, [%1, %2]" : "=r"(raw) : "r"(off), "r"(self));
        one &= raw;
        one |= shifted;
        asm volatile("strb %0, [%1, %2]" : : "r"(one), "r"(off), "r"(self));
    }

    {
        MATCH_HOLD_REG(s32, shifted, r2) = (self->maskLevel & 3) << PACKED_STATS_MASK_LEVEL_SHIFT;
        MATCH_HOLD_REG(s32, mask, r1) = ~PACKED_STATS_MASK_LEVEL_MASK;
        MATCH_HOLD_REG(u16, loaded, r5) = *(u16 *)snap->packedStats;
        mask &= loaded;
        packed = mask | shifted;
    }
    *(u16 *)snap->packedStats = packed;

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
