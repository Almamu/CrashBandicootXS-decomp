#include "core.h"
#include "actor_self.h"

/* Same large per-instance "self" object family as ctrl.c/
 * action_ctrl_states.c/polar_player_actions.c/airship_fireball.c (state at `self+0x28`,
 * table-index at `self+0xc`, an anim-frame halfword/byte pair at
 * `self+0x10`/`self+0x12`, an accumulator at `self+8`, a "part table"
 * pointer at `self+0`), part of a second boss-weapon "spawn/pre-
 * attack" singleton whose own flags/counters live at
 * `gJetpackBomberCount`-`gJetpackPlayerTiles` - a different singleton
 * cluster than issue #58's `gAirship` one and issue #62's
 * `gHovercraft` one. See docs/matching/issue-56-0x0802f0dc-actor.md
 * and docs/status/actor.md. */

extern void SetCellAnimSpeed(s32 arg0);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void FreezeLevelClock(void *arg0, s32 arg1);
extern s32 GetActorCategoryFrameCount(void);
extern s32 __divsi3(s32 arg0, s32 arg1);
extern s32 AddLife(void *self);
extern void *gAudioContext;
extern void *gLevelState;
extern u8 gJetpackPlayerHalted;
extern u8 gJetpackPlayerInactive;
extern u8 gJetpackInputEnabled;
extern s32 gJetpackPlayerVelY;
extern s32 gJetpackPlayerVelX;
extern s32 gJetpackPlayerMaxHp;
extern s32 gJetpackRingLastFrame;
extern s32 gJetpackRingChain;
extern s32 gJetpackWumpaDispenseTimer;
extern s32 gJetpackQueuedWumpa;

struct actor_hp {
    struct actor_self base;
    s32 hp;             // 0x54 - refilled by PassJetpackRing, capped at gJetpackPlayerMaxHp
};

/* Constructor/reset: while the singleton flag (`gJetpackPlayerInactive`) is
 * off, resets `self` to state 5/table-index 4 (idle-ish), plays a cue,
 * and - only if the current game-mode flag at `gLevelState+0x8c`
 * is set - fires an extra one-shot effect via `FreezeLevelClock`. */
void FinishJetpackRun(void *selfArg)
{
    register struct actor_self *self asm("r4") = selfArg;
    register s32 zero asm("r5") = gJetpackPlayerInactive;

    if (zero == 0) {
        gJetpackInputEnabled = zero;
        gJetpackPlayerHalted = 1;
        gJetpackPlayerInactive = 1;
        SetCellAnimSpeed(0x3c);
        gJetpackPlayerVelY = zero;
        gJetpackPlayerVelX = zero;
        {
            register s32 five asm("r0") = 5;
            register s32 four asm("r1") = 4;

            self->state = five;
            self->stateTime = zero;
            self->animIndex = four;
        }
        {
            register u16 anim asm("r0") = self->anims[4].duration;
            register u8 zero2 asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero2;
        }
        self->animTime = zero;
        PlaySfx(gAudioContext, 0x3b, 0x100);
        if (*((u8 *)gLevelState + 0x8c) != 0) {
            FreezeLevelClock(gLevelState, 0x2710);
        }
    }
}

/* State-machine update for the same singleton (issue #56): while `self`
 * is in one of the "active" states (5-state-history via
 * `self+0x28` == 1/6/2/3), resets `self`'s table index/anim to the idle
 * frame if it wasn't already, latches the target position at `self+0x1c`/
 * `self+0x20` from the two arguments, and re-arms state 6 (playing a cue
 * only on the *first* transition into it). While the current game-mode
 * flag at `gLevelState+0x8c` is clear and the `gJetpackRingLastFrame`
 * frame-timer has advanced far enough (>0x14 frames since the last pass),
 * drives a 5-case round-robin (`gJetpackRingChain`, wrapping 0-4) once
 * every >0xbe-frame window: cases 0-2 feed the `gJetpackQueuedWumpa` reward
 * accumulator (by 1/5/0x14) while not paused, case 3 advances `self+0x54`'s
 * own accumulator (clamped to `gJetpackPlayerMaxHp`) while the other
 * singleton flag is clear, and case 4 fires a one-shot effect plus a cue.
 * Every path through the round-robin (taken or not) re-samples the
 * frame timer and advances the round-robin index. */
void PassJetpackRing(void *selfArg, s32 xArg, s32 yArg)
{
    register struct actor_hp *self asm("r5") = selfArg;
    register s32 x asm("r3") = xArg;
    register s32 y asm("r4") = yArg;
    s32 state = self->base.state;
    u8 paused;

    if (state != 1 && state != 6 && state != 2 && state != 3) {
        return;
    }

    if (self->base.animIndex != 5) {
        self->base.animIndex = 5;
        {
            register u16 anim asm("r0") = self->base.anims[5].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
            self->base.animTime = zero2;
        }
    }

    self->base.x = x;
    self->base.y = y;

    if (self->base.state != 6) {
        SetCellAnimSpeed(0x50);
    }
    self->base.state = 6;

    {
        register s32 *p1508 asm("r2") = &gJetpackPlayerVelY;
        register s32 *p150c asm("r1") = &gJetpackPlayerVelX;
        register s32 zero asm("r0") = 0;

        *p150c = zero;
        *p1508 = zero;
        self->base.stateTime = zero;
    }

    paused = *((u8 *)gLevelState + 0x8c);
    if (paused != 0) {
        return;
    }

    if (GetActorCategoryFrameCount() - gJetpackRingLastFrame <= 0x14) {
        return;
    }

    if (GetActorCategoryFrameCount() - gJetpackRingLastFrame > 0xbe) {
        gJetpackRingChain = paused;
    }

    switch (gJetpackRingChain) {
    case 0:
        if (*((u8 *)gLevelState + 0x8c) == 0) {
            if (gJetpackQueuedWumpa == 0) {
                gJetpackWumpaDispenseTimer = 0xf;
            }
            gJetpackQueuedWumpa += 1;
        }
        break;
    case 1:
        if (*((u8 *)gLevelState + 0x8c) == 0) {
            if (gJetpackQueuedWumpa == 0) {
                gJetpackWumpaDispenseTimer = 0xf;
            }
            gJetpackQueuedWumpa += 5;
        }
        break;
    case 2:
        if (*((u8 *)gLevelState + 0x8c) == 0) {
            if (gJetpackQueuedWumpa == 0) {
                gJetpackWumpaDispenseTimer = 0xf;
            }
            gJetpackQueuedWumpa += 0x14;
        }
        break;
    case 3:
        if (gJetpackPlayerInactive == 0) {
            register s32 *maxPtr asm("r4") = &gJetpackPlayerMaxHp;
            register s32 max asm("r1") = *maxPtr;
            register s32 mul asm("r0") = 0x14;
            s32 v = self->hp + __divsi3(max * mul, 0x64);

            self->hp = v;
            {
                register s32 cap asm("r4") = *maxPtr;

                if (v > cap) {
                    self->hp = cap;
                }
            }
        }
        break;
    case 4:
        if (*((u8 *)gLevelState + 0x8c) == 0) {
            AddLife(gLevelState);
            PlaySfx(gAudioContext, 7, 0x100);
        }
        break;
    }

    gJetpackRingLastFrame = GetActorCategoryFrameCount();
    gJetpackRingChain++;
    if (gJetpackRingChain == 5) {
        gJetpackRingChain = 0;
    }
}

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md.
 *
 * Computes two `self`-keyframe-driven sizes (byte0*byte1, scaled by
 * 32) via `AllocVramTileBlock`, storing them into the `gJetpackPlayerTiles`
 * pair, then arms `gJetpackPlayerTileBuffer`/clears `gJetpackPlayerLastFrame`. Both
 * keyframe-size sub-blocks are literally identical computations,
 * matching the ROM's own duplication.
 *
 * The ROM computes `byte0*byte1` into one register, copies it to a
 * second, *then* shifts (`adds r2,r3,#0; muls r2,r1,r2; adds r0,r2,#0;
 * lsls r0,r0,#5`) - this compiler's dead-store elimination always
 * collapses a plain `(rec[1] * rec[0]) << 5` into a shorter
 * compute-and-shift-in-place sequence, so the extra copy is
 * materialized via an opaque `asm volatile` matching the ROM's exact
 * register roles (same class of gap as `DrawPolarCollectedWumpa`/`DrawJetpackCheckpointText`,
 * issue #52/#71). The two blocks' index/address computation
 * (`table + idx*3*4 + 2`) also needed its own register roles pinned to
 * match: `table` loaded early into r3, the `+2` index constant
 * materialized via an opaque `mov #2` immediately before the `ldrsh`
 * (a bare C-level `register`-pinned local has no effect here, since
 * gcc constant-folds the literal and freely picks its own register),
 * and the final byte-load pair (`rec[0]`/`rec[1]`) pinned per-block to
 * the exact registers the ROM's `ldrb` pair uses. */
extern void *AllocVramTileBlock(s32 size);
extern void *gJetpackPlayerTiles[2];
extern s32 gJetpackPlayerTileBuffer;
extern s32 gJetpackPlayerLastFrame;

void AllocJetpackPlayerTiles(void *selfArg)
{
    struct actor_self *self = selfArg;

    {
        s32 accum = self->animTime >> 8;
        s32 idx = self->animIndex;
        u8 *table = (u8 *)self->anims;
        s16 off = *(s16 *)(table + idx * 3 * 4 + 2);
        s32 pos = off + accum;
        u8 **table2 = (u8 **)self->frameOffsets;
        u8 *rec = table2[pos];
        register s32 b0 asm("r3") = rec[0];
        register s32 b1 asm("r1") = rec[1];
        register s32 temp asm("r2");
        register s32 size asm("r0");

        asm volatile(
            "add %0, %2, #0\n\t"
            "mul %0, %1, %0\n\t"
            "add %3, %0, #0\n\t"
            "lsl %3, %3, #5\n\t"
            : "=r"(temp), "+r"(b1), "+r"(b0), "=r"(size)
        );
        gJetpackPlayerTiles[0] = AllocVramTileBlock(size);
    }
    {
        s32 accum = self->animTime >> 8;
        s32 idx = self->animIndex;
        register u8 *table asm("r3") = (u8 *)self->anims;
        register s32 shiftResult asm("r0") = idx * 3 * 4;
        register u8 *addr2 asm("r0");
        register s32 twoIdx asm("r3");
        register s32 off asm("r0");
        s32 pos;
        u8 **table2;
        u8 *rec;

        asm volatile("add %0, %0, %1" : "+r"(shiftResult) : "r"(table));
        addr2 = (u8 *)shiftResult;
        asm volatile("mov %0, #2\n\tldrsh %1, [%2, %0]" : "=r"(twoIdx), "=r"(off) : "r"(addr2));
        pos = off + accum;
        table2 = (u8 **)self->frameOffsets;
        rec = table2[pos];
        {
            register s32 b0 asm("r2") = rec[0];
            register s32 b1 asm("r3") = rec[1];
            register s32 temp asm("r1");
            register s32 size asm("r0");

            asm volatile(
                "add %0, %2, #0\n\t"
                "mul %0, %1, %0\n\t"
                "add %3, %0, #0\n\t"
                "lsl %3, %3, #5\n\t"
                : "=r"(temp), "+r"(b1), "+r"(b0), "=r"(size)
            );
            gJetpackPlayerTiles[1] = AllocVramTileBlock(size);
        }
    }

    gJetpackPlayerTileBuffer = 1;
    gJetpackPlayerLastFrame = 0;
}

asm(".align 2, 0");
