#include "core.h"
#include "match.h"
#include "vtable.h"
#include "action_obj.h"
#include "system.h"
#include "audio.h"
#include "player.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* Part of GitHub issue #16's remainder (0x08011BD4-0x08012D24) - the
 * "child object" family docs/rom_map.md's "Undifferentiated core"
 * investigation and docs/matching/archive/issue-16-actor-11b0c.md both already
 * identified but left raw pending a dedicated pass (`self+0xc`/`+0x10`
 * hold pointers to further sub-records, distinct from `struct actor`).
 * `self+0xc` is a per-category table of `{s16 offset; void *fn}` pairs
 * (at least `+0x20`/`+0x24` and `+0x50`/`+0x54` entries known so far)
 * fed through the `_call_via_r2`/`_call_via_r3` trampolines together
 * with `self+offset` and `self+0x10` (a "part" sub-object) - the same
 * convention already named in action_ctrl_states.c's doc comments, which
 * this file's functions are siblings of (not ROM-adjacent to them,
 * hence a separate file per the one-file-per-contiguous-region rule).
 * The `+0x27`-`+0x32` bytes are the same shared state/flag/table-index
 * trio pair action_ctrl_states.c documents. `self` is the action
 * controller (`struct act`, action_obj.h); its `part` is the player
 * (`struct player`). */

struct AudioContext;
struct palette_cache;

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);

/* Plays a sound, fires the `+0x50`/`+0x54` trampoline pair with `id`
 * (the death animation, 0x1c/0x2a-0x2f from ActionCtrlHandleEvent) as
 * its argument, then the `+0x20`/`+0x24` pair with id `0x1d`,
 * resets both halves of the state/flag/table-index trio (`+0x31`/
 * `+0x2f`/`+0x27` and `+0x32`/`+0x30`/`+0x28`) via a single walked
 * pointer, runs `ApplyActionCtrlMotion`, clears/sets a few more `part` bytes
 * (`+0x100`/`+0x102`/`+0x103`/`+0x104`, and clears bits `0x20`/`0x10`
 * of `part+0xc`), then calls `LoseLife` and looks up a byte from the
 * per-tag 28-byte-record table (`part+0x20 -> *ptr + tag*0x1C`, the
 * same dereference chain docs/rom_map.md's "eight more core reads"
 * documented from three other call sites) to feed `LoadPaletteSlot`. */
void KillPlayer(struct act *self, s32 id)
{
    {
        MATCH_HOLD_REG(void *, a0, r0) = gAudioContext;
        MATCH_HOLD_REG(s32, a2, r2) = 0x100;
        MATCH_HOLD_REG(s32, a1, r1) = 0x1b;
        PlaySfx(a0, a1, a2);
    }

    {
        struct act_method *off = &self->vt->m50;
        _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)id, off->fn);
    }
    {
        struct vtable_slot *mgr = (struct vtable_slot *)self->vt;
        _call_via_r2((u8 *)self + mgr[4].delta, (void *)0x1d, mgr[4].fn);
    }

    {
        MATCH_HOLD_REG(s32, zero, r4) = 0;
        MATCH_HOLD_REG(s32, one, r5);

        {
            MATCH_HOLD_REG(u8 *, w, r0) = &self->motionXKeepSpeed;
            *w = zero;
            w -= 2;
            one = 1;
            *w = one;
            w -= 8;
            *w = zero;
            w += 0xb;
            *w = zero;
            w -= 2;
            *w = one;
            w -= 8;
            *w = zero;
        }

        ApplyActionCtrlMotion(self);

        self->part->slippery = zero;
        self->part->pushLeft = zero;
        self->part->pushRight = zero;

        {
            MATCH_HOLD_REG(struct player *, p, r1) = self->part;
            MATCH_HOLD_REG(s32, mask, r0) = 0x7f;
            mask &= p->flags.all;
            p->flags.all = mask;
        }
        {
            MATCH_HOLD_REG(struct player *, p, r1) = self->part;
            MATCH_HOLD_REG(s32, mask, r0) = 0x41;
            mask = -mask;
            mask &= p->flags.all;
            p->flags.all = mask;
        }

        {
            /* Plain `addr[0x104] = one;` lets the compiler constant-fold
             * the 0x104 offset into whichever scratch register it
             * likes (r2), always the opposite of the ROM's r1 - an
             * inline-asm anchor (matching_decomp_register_pinning) is
             * the only way found to pin the folded constant's own
             * register. */
            MATCH_HOLD_REG(u8 *, addr, r0) = (u8 *)self->part;
            MATCH_HOLD_REG(s32, v, r1);
            asm volatile("mov %0, #0x82\n\tlsl %0, %0, #1" : "=r"(v));
            addr += v;
            *addr = one;
        }
    }

    LoseLife(gLevelState);

    {
        MATCH_HOLD_REG(struct palette_cache *, cache, r0) = gPaletteCache;
        MATCH_HOLD_REG(struct player *, p, r3) = self->part;
        MATCH_HOLD_REG(u32, nibble, r1) = p->slot;
        MATCH_HOLD_REG(struct act_anim_bank *, xptr, r2) = p->anim;
        MATCH_HOLD_REG(u8 *, tagp, r3) = &p->tag;

        {
            MATCH_HOLD_REG(struct act_anim_record *, base, r4) = xptr->records;
            MATCH_HOLD_REG(u8, tag, r5) = *tagp;
            MATCH_HOLD_REG(s32, record, r2) = tag * 0x1c;
            record += (s32)base;
            LoadPaletteSlot(cache, nibble, ((struct act_anim_record *)record)->paletteId);
        }
    }
}

/* UpdateActionCtrl calls this when the player's `slippery` flag changes.
 * If the player (`gPlayer`) is now on slippery ground (`slippery`, +0x100):
 * on tag `0x12` (only if `unk_60` is nonzero) or tag `0xd`/`0x18`,
 * re-tags the player as `0x25` (type `0x12`) or `0x26` (the other two,
 * re-reading the global fresh first) and fires the standard
 * `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone(..., 0)` teardown trio.
 * (0x25/0x26 are the skid anims, ActionCtrlSetTargetAnim's replacements).
 * Otherwise (flag clear), on player type `0x25`/`0x26`, stops the skid
 * sound (0x36) and goes back to the idle anim via `SetActionCtrlModeAnim`. */
void UpdateActionCtrlSkidAnim(struct act *selfArg)
{
    /* The copy emits nothing, but without it the `.s` label numbers of
     * UpdatePlayerFacing shift by one. */
    struct act *self = selfArg;
    struct player *player = gPlayer;
    MATCH_HOLD_REG(s32, flag, r5) = player->slippery;

    if (flag == 0)
        goto flag_zero;

    {
        MATCH_HOLD_REG(u8 *, typeAddr, r3) = &player->tag;
        s32 type = *typeAddr;
        MATCH_HOLD_REG(s32, type2, r2) = type;

        if (type == 0x12)
            goto case_12;
        if (type > 0x12)
            goto check_18;
        if (type == 0xd)
            goto case_set_26;
        goto end;
    check_18:
        if (type2 == 0x18)
            goto case_set_26;
        goto end;

    case_12:
        if (player->speedX == 0)
            goto end;
        *typeAddr = 0x25;
        goto common;
    }

case_set_26:
    {
        player = gPlayer;
        {
            MATCH_HOLD_REG(s32, v, r0) = 0x26;
            player->tag = v;
        }
    }

common:
    ResetSpriteFrameTimer(player);
    ResetSpriteFrameIndex(player);
    SetSpriteAnimDone(player, 0);
    goto end;

flag_zero:
    {
        s32 type2 = player->tag;
        if (type2 == 0x25)
            goto do_call;
        if (type2 != 0x26)
            goto end;
    do_call:
        StopSfx(gAudioContext, 0x36);
        SetActionCtrlModeAnim(self, 0, 0x12, 0, flag);
    }
end:
    return;
}

/* Reads D-pad input (unused directly, but forces the same call/reload
 * shape as the ROM) and dispatches on `self+8`'s type via a 39-entry
 * jump table (values 0-0x26; anything above returns 0 directly). 16 of
 * the 39 values (0, 3-5, 7, 9, 11, 13-15, 20, 26, 32-33, 37-38) run a
 * shared body: clear `part+0x28` bit `0x20`, then on bit `0x10` set,
 * either re-clear it (D-pad remap `4`/`6`/`8`) or set it while also
 * setting bit `0x10` back (D-pad remap `3`/`5`/`7`), in both cases also
 * setting the state/flag pair `self+0x2f`=1/`self+0x29`=0 and
 * returning 1; every other value/path returns 0. */
s32 UpdatePlayerFacing(struct act *self)
{
    MATCH_HOLD_REG(s32, dpad, r3) = GetDpadDirection(gInput);
    MATCH_HOLD_REG(s32, result, r2) = 0;
    s32 type = self->state;

    if ((u32)type > 0x26)
        goto end;

    // clang-format off
    switch (type) {
    case 0: case 3: case 4: case 5: case 7: case 9: case 11: case 13:
    case 14: case 15: case 20: case 26: case 32: case 33: case 37: case 38:
        goto do_it;
    default:
        goto end;
    }
    // clang-format on

do_it:
    {
        MATCH_HOLD_REG(u8 *, p, r1) = &self->part->mirror.all;
        MATCH_HOLD_REG(s32, mask, r0) = -0x21;
        MATCH_HOLD_REG(s32, val, r5) = *p;
        mask &= val;
        *p = mask;
    }

    if (self->part->mirror.all << 27 >= 0)
        goto check_2nd;
    if (dpad == 4 || dpad == 6 || dpad == 8)
        goto branch1;
    goto check_2nd;

branch1:
    {
        MATCH_HOLD_REG(struct player *, part, r1) = self->part;
        MATCH_HOLD_REG(s32, zero, r2) = 0;
        MATCH_HOLD_REG(u8 *, p, r1) = &part->mirror.all;
        {
            MATCH_HOLD_REG(s32, mask, r0) = -0x11;
            mask &= *p;
            *p = mask;
        }
        {
            MATCH_HOLD_REG(u8 *, addr1, r1) = &self->motionXPending;
            MATCH_HOLD_REG(s32, one, r0) = 1;
            *addr1 = one;
        }
        {
            MATCH_HOLD_REG(u8 *, addr2, r0) = &self->turboRun;
            *addr2 = zero;
        }
    }
    goto ret1;

check_2nd:
    {
        MATCH_HOLD_REG(struct player *, part, r0) = self->part;
        MATCH_HOLD_REG(u8 *, addr, r1) = &part->mirror.all;
        if (*addr << 27 < 0)
            goto end;
        if (dpad == 3 || dpad == 5 || dpad == 7)
            goto branch2;
        goto end;

    branch2:
        {
            MATCH_HOLD_REG(s32, one, r3) = 1;
            MATCH_HOLD_REG(u8 *, p, r2) = &part->mirror.all;
            MATCH_HOLD_REG(s32, mask, r0) = -0x11;
            MATCH_HOLD_REG(s32, val, r5) = *p;
            mask &= val;
            mask |= 0x10;
            *p = mask;
            {
                MATCH_HOLD_REG(u8 *, addr, r0) = &self->motionXPending;
                MATCH_HOLD_REG(s32, zero, r1) = 0;
                *addr = one;
                addr -= 6; /* turboRun */
                *addr = zero;
            }
        }
        goto ret1;
    }

ret1:
    result = 1;
end:
    return result;
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
