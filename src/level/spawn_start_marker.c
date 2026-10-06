#include "core.h"
#include "actor.h"
#include "level_state.h"
#include "level_data.h"
#include "util.h"
#include "audio.h"
#include "level.h"
#include "globals.h"

extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* Sound-trigger dispatch/position writer - the last of the
 * `LoadGraphicsPackage` cluster's scratch-buffer-style helper family
 * (issue #30). Two independent, unrelated halves:
 *
 * 1. If `GetSpawnAtStart(gLevelState)` (the player's `+0xa8` flag)
 *    is set: looks up a per-`z` flags byte via the same
 *    `gEntityFlags -> *rec` entity parameter table
 *    (`paramOffsets[]`/`params`, the room's `struct level_entity_list`)
 *    `SpawnBasicCrate` (spawn_crates.c) already reads, folds
 *    its bit 1 into the player's `+0x28` bitfield's bit 4, then
 *    unconditionally writes the incoming `x`/`y` (Q8.8, shifted from
 *    the raw `u16` args) into the player's own `x`/`y` fields - the
 *    same unconditional write `sub_80221A4`/`sub_80221D4`
 *    (spawn_pickups.c) already do elsewhere in this cluster.
 *
 * 2. Unless the level state's `timeTrial` flag is set: fires the
 *    player's `table+0x68` trampoline (via `_call_via_r4`, action
 *    `0x1a`) and plays SFX `0x100` through `gAudioContext`,
 *    unless a budget/reentrancy guard trips first - either the
 *    player's spawn counter (`GetDeaths`, `+0x7c`) has room against
 *    its cap (`GetMaskAssistDeaths`, `+0x84`), or (when it doesn't) all three
 *    of `GetLives` (`+0x74`), `IsInBonusRound` (`+0xa4`) and the
 *    level state's `maskLevel` field agree it's still safe to fire.
 *
 * Was a NAKED asm transcription for a long time - see
 * docs/matching/naked-sub_801e990-matched.md for the full derivation
 * history, including the register-choice gap that blocked a real match
 * (the `+0x28` write's address/value register split) and how it closed:
 * the r3-pinned local had to model the *address of the global*
 * (`&gPlayer`, a `struct player **`) with `+0x28` computed as
 * a single dereference-and-add into r1, rather than modeling the
 * *dereferenced value* itself and copying it into r1 afterward - the
 * latter is semantically equivalent but makes gcc materialize the
 * value in a different temp register first, needing an extra `mov`
 * the ROM doesn't have. */
void SpawnStartMarker(u32 arg0, u16 x, u16 y, u16 z)
{
    if (GetSpawnAtStart(gLevelState)) {
        register const struct level_entity_list *rec asm("r2");
        register u16 *arrayBase asm("r0");
        register s32 addr asm("r1");
        register u8 *tmp asm("r0");
        register struct player **d8ptr asm("r3");

        rec = gEntityFlags->list;
        arrayBase = (u16 *)rec->paramOffsets;
        addr = (z << 1) + (s32)arrayBase;
        {
            s32 base = (s32)rec->params;
            addr = *(u16 *)addr;
            tmp = (u8 *)(addr + base);
        }

        {
            register u8 byte asm("r0") = *tmp;
            register s32 shiftedByte asm("r2");
            register s32 one asm("r0");
            register u8 *addr28 asm("r1");
            register s32 mask asm("r0");
            register u8 byte2 asm("r4");

            shiftedByte = byte >> 1;
            one = 1;
            d8ptr = &gPlayer;
            addr28 = (u8 *)*d8ptr + 0x28;
            shiftedByte &= one;
            shiftedByte <<= 4;
            asm volatile("sub %0, %0, #0x12" : "+r"(one));
            mask = one;
            byte2 = *addr28;
            mask &= byte2;
            mask |= shiftedByte;
            *addr28 = mask;
        }

        {
            register u8 *obj2 asm("r1") = (u8 *)*d8ptr;
            *(u32 *)obj2 = x << 8;
            *(u32 *)(obj2 + 4) = y << 8;
        }
    }

    if (gLevelState->timeTrial != 0) {
        goto end;
    }
    {
        s32 spawnCount = GetDeaths(gLevelState);
        s32 cap = GetMaskAssistDeaths(gLevelState);
        if (spawnCount >= cap) {
            goto fire;
        }
        if (GetLives(gLevelState) != 0) {
            goto end;
        }
        if (IsInBonusRound(gLevelState) != 0) {
            goto end;
        }
        if (gLevelState->maskLevel != 0) {
            goto end;
        }
    }
fire:
    {
        register u8 *d8obj asm("r0");
        register u8 *entry asm("r1");
        register s32 fnOffset asm("r2");
        register void *fn asm("r0");
        register u32 dead asm("r4");

        d8obj = (u8 *)gPlayer;
        entry = *(u8 **)(d8obj + 0x18);
        entry = entry + 0x68;
        asm volatile("mov r3, #0\n\tldrsh %0, [%1, r3]" : "=r"(fnOffset) : "r"(entry) : "r3");
        fn = d8obj + fnOffset;
        dead = *(u32 volatile *)(entry + 4);
        (void)dead;
        _call_via_r4(fn, 0, 0x1a, 0);
        PlaySfx(gAudioContext, 1, 0x100);
    }
end:;
}
