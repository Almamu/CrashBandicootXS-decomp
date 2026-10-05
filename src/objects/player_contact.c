#include "core.h"
#include "actor.h"
#include "level_state.h"
#include "aabb.h"
#include "player.h"
#include "objects.h"

/* The one player-object field (gPlayer, a `struct gobj`)
 * this file reads. */
struct player_view
{
    u8 unk_00[0x8c];
    u32 unk_8C;                     // 0x8c - a gRoomFrameCount deadline
};

extern struct level_state *gLevelState;
extern void *gPlayer;
extern u32 gRoomFrameCount;
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* Tests `part` for a collision-grid hit against the player
 * (`gPlayer`), gated by a mix of flag bits and a periodic
 * "fast path" check against `gRoomFrameCount` (the same ~128-frame
 * counter documented in docs/rom_map.md): if `part->flags` bit 2 is
 * set and the player's `unk_8C` field is ahead of the frame counter
 * and `gLevelState`'s mode (`maskLevel`) is 3, or independently if
 * `part`'s `+0xd` byte bit 3 is set and the mode is 3, builds `part`'s
 * primary AABB via `GetSpriteAttackBox` and tests it against the player via
 * `PlayerTouchesBox`; on a hit, calls `ResolvePlayerContact` and returns. If the
 * primary AABB has no region (`w` zero) or the hit test missed,
 * falls back to the secondary AABB via `GetSpriteBodyBox` and repeats the
 * same hit test. */
void CheckPlayerContact(void *partArg)
{
    register struct actor *part asm("r4") = partArg;
    register u8 flagsByte asm("r1") = part->flags;
    register s32 flagsShifted asm("r0") = flagsByte >> 2;
    register s32 mask asm("r5") = 1;
    register s32 flagsBit asm("r0");

    flagsBit = flagsShifted & mask;
    if (flagsBit) {
        s32 fast;
        struct player_view *player = gPlayer;

        fast = 0;
        if (player->unk_8C > gRoomFrameCount) {
            fast = 1;
        }
        if (fast != 0) {
            if (gLevelState->maskLevel != 3) {
                goto gate2;
            }
        }
        {
            register u8 fieldD asm("r1") = *((u8 *)part + 0xd);
            register s32 shifted asm("r0") = fieldD >> 3;
            register s32 bit asm("r0");

            bit = shifted & mask;
            if (!bit) {
                goto doCheck;
            }
        }
    }
gate2:
    {
        register u8 fieldD asm("r1") = *((u8 *)part + 0xd);
        register s32 shifted asm("r0") = fieldD >> 3;
        register s32 one asm("r1") = 1;
        register s32 bit asm("r0");

        bit = shifted & one;
        if (!bit) {
            return;
        }
    }
    if (gLevelState->maskLevel != 3) {
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

/* ROM 0x08009D5C - fires a `part->table+0x68`-driven trampoline (the
 * "dead read" idiom already established for
 * `CollidePartWithPlayer`/`CollidePartWithObject`/`CollideCrateGridPartWithObject`) based on
 * `gLevelState`'s mode (`+0x78`): mode 0 fires it on the player
 * with arguments `(0, part->field_0A, 0)`; modes 1-2 fire it on the
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
    register struct actor *part asm("r5") = partArg;
    s32 mode;
    register void *addr asm("r0");
    register s32 arg1 asm("r1");
    register s32 arg2 asm("r2");
    register void *deadRead asm("r4");

    {
        register s32 flagBit asm("r0") = 8;
        register u8 curFlags asm("r1") = part->flags;
        register s32 newFlags asm("r0");

        newFlags = flagBit | curFlags;
        part->flags = newFlags;
    }
    mode = gLevelState->maskLevel;

    switch (mode) {
    case 0:
        goto mode0;
    case 1:
    case 2:
        goto mode1or2;
    case 3:
        goto checkMode3;
    default:
        return;
    }

mode0:
    {
        register struct actor *player asm("r0") = gPlayer;
        u8 *rec = (u8 *)player->table + 0x68;
        s16 offset = *(s16 *)rec;
        addr = (u8 *)player + offset;
        arg2 = part->field_0A;
        deadRead = *(void *volatile *)(rec + 4);
        arg1 = 0;
        goto tail;
    }

mode1or2:
    {
        struct actor *player = gPlayer;
        u8 *rec = (u8 *)player->table + 0x68;
        s16 offset = *(s16 *)rec;
        void *addr0 = (u8 *)player + offset;
        u8 someByte = part->field_0A;
        register void *deadRead0 asm("r4") = *(void *volatile *)(rec + 4);
        (void)deadRead0;

        _call_via_r4(addr0, 0, someByte, 0);

        {
            u8 *rec2 = (u8 *)part->table + 0x68;
            s16 offset2 = *(s16 *)rec2;
            addr = (u8 *)part + offset2;
            deadRead = *(void *volatile *)(rec2 + 4);
            arg1 = 1;
            arg2 = 1;
        }
        goto tail;
    }

tail:
    {
        register s32 arg3 asm("r3") = 0;
        _call_via_r4(addr, arg1, arg2, arg3);
    }
    return;

checkMode3:
    if (mode == 3) {
        u8 *rec = (u8 *)part->table + 0x68;
        s16 offset = *(s16 *)rec;
        void *addr3 = (u8 *)part + offset;
        register void *deadRead3 asm("r4") = *(void *volatile *)(rec + 4);
        (void)deadRead3;
        _call_via_r4(addr3, 1, 1, 0);
    }
}
asm(".align 2, 0");
