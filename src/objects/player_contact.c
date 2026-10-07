#include "core.h"
#include "match.h"
#include "actor.h"
#include "level_state.h"
#include "aabb.h"
#include "player.h"
#include "objects.h"
#include "globals.h"
#include "box_part.h"

extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* Tests `part` for a collision-grid hit against the player
 * (`gPlayer`), gated by a mix of flag bits and a periodic
 * "fast path" check against `gRoomFrameCount` (the same ~128-frame
 * counter documented in docs/rom_map.md): if `part->flags` bit 2 is
 * set and the player's `deadline` is ahead of the frame counter
 * and `gLevelState`'s mode (`maskLevel`) is 3, or independently if
 * `part->flags2` bit 3 (solid) is set and the mode is 3, builds `part`'s
 * primary AABB via `GetSpriteAttackBox` and tests it against the player via
 * `PlayerTouchesBox`; on a hit, calls `ResolvePlayerContact` and returns. If the
 * primary AABB has no region (`w` zero) or the hit test missed,
 * falls back to the secondary AABB via `GetSpriteBodyBox` and repeats the
 * same hit test. */
void CheckPlayerContact(void *partArg)
{
    MATCH_HOLD_REG(struct box_part *, part, r4) = partArg;
    MATCH_HOLD_REG(u8, flagsByte, r1) = part->flags;
    MATCH_HOLD_REG(s32, flagsShifted, r0) = flagsByte >> 2;
    MATCH_HOLD_REG(s32, mask, r5) = 1;
    MATCH_HOLD_REG(s32, flagsBit, r0);

    flagsBit = flagsShifted & mask;
    if (flagsBit) {
        s32 fast;
        struct player *player = gPlayer;

        fast = 0;
        if (player->deadline > gRoomFrameCount) {
            fast = 1;
        }
        if (fast != 0) {
            if (gLevelState->maskLevel != MASK_LEVEL_INVINCIBLE) {
                goto gate2;
            }
        }
        {
            MATCH_HOLD_REG(u8, flags2, r1) = part->flags2;
            MATCH_HOLD_REG(s32, shifted, r0) = flags2 >> 3;
            MATCH_HOLD_REG(s32, bit, r0);

            bit = shifted & mask;
            if (!bit) {
                goto doCheck;
            }
        }
    }
gate2:
    {
        MATCH_HOLD_REG(u8, flags2, r1) = part->flags2;
        MATCH_HOLD_REG(s32, shifted, r0) = flags2 >> 3;
        MATCH_HOLD_REG(s32, one, r1) = 1;
        MATCH_HOLD_REG(s32, bit, r0);

        bit = shifted & one;
        if (!bit) {
            return;
        }
    }
    if (gLevelState->maskLevel != MASK_LEVEL_INVINCIBLE) {
        return;
    }
doCheck:
    {
        struct aabb box;
        GetSpriteAttackBox(&box, part);
        if (box.w != 0) {
            if (PlayerTouchesBox(gPlayer, &box)) {
                ResolvePlayerContact(part);
                return;
            }
        }
    }
    {
        struct aabb box2;
        GetSpriteBodyBox(&box2, part);
        if (*(s32 volatile *)&box2.w != 0) {
            if (PlayerTouchesBox(gPlayer, &box2)) {
                ResolvePlayerContact(part);
            }
        }
    }
}

/* ROM 0x08009D5C - fires the method table +0x68 hit method (the
 * "dead read" idiom already established for
 * `CollidePartWithPlayer`/`CollidePartWithObject`/`CollideCrateGridPartWithObject`) based on
 * `gLevelState->maskLevel`: mode 0 fires it on the player
 * with arguments `(0, part->kind, 0)`; modes 1-2 fire it on the
 * player with the same arguments, then again on `part` itself with
 * `(1, 1, 0)`; mode 3 fires it on `part` alone with `(1, 1, 0)`; any
 * other mode does nothing. Always sets `part->flags` bit 3 first.
 *
 * A `switch` on `mode` reproduces the ROM's exact 3-way dispatch (`cmp
 * #2,bgt` / `cmp #1,bge` / `cmp #0,beq`, which no amount of rewriting
 * an equivalent `if`/`else if` chain reproduces - this compiler always
 * normalizes `x >= 1` down to `x > 0` for a plain comparison chain,
 * but a `switch` lowers differently and keeps the literal `#1`/`bge`
 * form), and explicit `goto`s into a shared tail (`r0`/`r1`/`r2`/`r4`
 * pinned to their ABI registers) reproduce the mode-0/mode-1-2 call
 * sharing. The mode-3 case's own `cmp;beq;b` branch shape (jumping
 * into the call code, then separately jumping back to the epilogue,
 * rather than the more compact `cmp;bne` skip-and-fall-through this
 * compiler prefers whenever the call body is placed right after the
 * switch dispatch) only falls out once the `checkMode3:`/`if (mode ==
 * 3)` block is moved to be the *last* thing in the function, after
 * `mode0`/`mode1or2`/`tail` - with the call body no longer adjacent to
 * its own dispatch test, this compiler's block-layout pass can't
 * collapse the two paths into one, and emits the ROM's real
 * three-instruction form. Source order also had to swap `mode0`
 * before `mode1or2` (ROM's actual address order) and `player` needed
 * an explicit `r0` register pin in the `mode0` block specifically -
 * without it, this compiler picks `r2` for the reused player pointer
 * there (it doesn't need the same hint in `mode1or2`, where the ABI
 * call to `_call_via_r4` already forces player's address into `r0`).
 * Matched. */
void ResolvePlayerContact(void *partArg)
{
    MATCH_HOLD_REG(struct box_part *, part, r5) = partArg;
    s32 mode;
    MATCH_HOLD_REG(void *, addr, r0);
    MATCH_HOLD_REG(s32, arg1, r1);
    MATCH_HOLD_REG(s32, arg2, r2);
    MATCH_HOLD_REG(void *, deadRead, r4);

    {
        MATCH_HOLD_REG(s32, flagBit, r0) = 8;
        MATCH_HOLD_REG(u8, curFlags, r1) = part->flags;
        MATCH_HOLD_REG(s32, newFlags, r0);

        newFlags = flagBit | curFlags;
        part->flags = newFlags;
    }
    mode = gLevelState->maskLevel;

    switch (mode) {
    case MASK_LEVEL_NONE:
        goto mode0;
    case MASK_LEVEL_ONE:
    case MASK_LEVEL_TWO:
        goto mode1or2;
    case MASK_LEVEL_INVINCIBLE:
        goto checkMode3;
    default:
        return;
    }

mode0:
    {
        MATCH_HOLD_REG(struct player *, player, r0) = gPlayer;
        const struct actor_method *rec = &player->vtable->handleEvent;
        s16 offset = rec->thisOffset;
        addr = (u8 *)player + offset;
        arg2 = part->kind;
        deadRead = *(void *const volatile *)&rec->fn;
        arg1 = 0;
        goto tail;
    }

mode1or2:
    {
        struct player *player = gPlayer;
        const struct actor_method *rec = &player->vtable->handleEvent;
        s16 offset = rec->thisOffset;
        void *addr0 = (u8 *)player + offset;
        u8 someByte = part->kind;
        MATCH_HOLD_REG(void *, deadRead0, r4) = *(void *const volatile *)&rec->fn;
        (void)deadRead0;

        _call_via_r4(addr0, 0, someByte, 0);

        {
            struct part_method *rec2 = PART_METHOD(part, 0x68);
            s16 offset2 = rec2->thisOffset;
            addr = (u8 *)part + offset2;
            deadRead = *(void *volatile *)&rec2->fn;
            arg1 = 1;
            arg2 = EVENT_HIT;
        }
        goto tail;
    }

tail:
    {
        MATCH_HOLD_REG(s32, arg3, r3) = 0;
        _call_via_r4(addr, arg1, arg2, arg3);
    }
    return;

checkMode3:
    if (mode == MASK_LEVEL_INVINCIBLE) {
        struct part_method *rec = PART_METHOD(part, 0x68);
        s16 offset = rec->thisOffset;
        void *addr3 = (u8 *)part + offset;
        MATCH_HOLD_REG(void *, deadRead3, r4) = *(void *volatile *)&rec->fn;
        (void)deadRead3;
        _call_via_r4(addr3, 1, EVENT_HIT, 0);
    }
}
