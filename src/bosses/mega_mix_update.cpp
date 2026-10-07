#include "boss_ctrl.hpp"

extern "C" {
#include "match.h"
#include "aabb.h"
#include "crates.h"
#include "crate.h"
#include "globals.h"
#include "math_util.h"
}

/* MegaMixCtrl::Update (UpdateMegaMix): the update of Mega-Mix, the boss
 * that entity type 0x49 spawns (SpawnMegaMix, sprite bank 30). Only room
 * 37 places one, at its left end among 34 nitro and 21 TNT crates; room
 * 37 is the only room of level 24, the last of the five one-room boss
 * levels (rooms 37-40, each placing a single boss spawner: Mega-Mix,
 * Dingodile, Tiny, Neo Cortex), and "mega-mix" is the one boss name of
 * the level-name list that none of the others take. Bank 30 renders as a
 * running fusion of the bosses (Dingodile's tail, Tiny's orange body,
 * N. Gin's helmet), and the game's ending has Mega-Mix chase Crash down
 * the space station's hallway, as this class does in a space-themed
 * room.
 *
 * GitHub issue #22, ROM 0x08017AB0-0x08017ECC, between
 * input_ctrl_queue.cpp and mega_mix.cpp (MegaMixCtrl's other methods).
 * Built with old_agbcp (Makefile OLD_AGBCC_OBJS), like tiny_update.cpp
 * and cortex.c after it (docs/matching/archive/issue-22-0x08018008-hopper.md).
 *
 * The first frame (`stamp` -1) starts X motion record 1. Then, by `state`:
 *
 * - 0: waits while the player is dead, then starts running towards him.
 * - 1: runs after the player. While the player is dead (or his
 *   controller is in mode 0x1E) it stops (anim 2, mode 0, X record 0).
 *   `latch` follows whether the part is on screen, and `stamp` is the
 *   frame it last changed; 60 frames after a change it restarts the X
 *   motion (record 1 on screen, 2 off screen, 3 when mirrored). It turns
 *   to face the player, and within range (0x27FF by 0x31FF in Q8) it
 *   grabs him (X record 0, mode 2, anim 1). Out of range it hits what
 *   its hitbox touches (CollidePartList) and blows up or breaks every
 *   crate it reaches.
 * - 2: the grab: on frame 8 of the anim, a player still in range takes
 *   a hit (his HandleEvent). Once the anim is done it runs again (or
 *   stops, if the player is dead). */

/* The player's `dead` byte (+0x104). The 0x104 is loaded into a spill
 * register that rotates through r1-r3; for this one read the ROM's is r3,
 * one step from what g++ picks (the same in C), so it is pinned there. */
static inline u8 PlayerDeadByte(struct player *pl)
{
    MATCH_HOLD_REG(s32, off, r3) = 0x104;

    MATCH_KEEP(off);
    return *((u8 *)pl + off);
}

/* Runs towards the player: faces him (X record 3 when mirrored, else 1),
 * anim 0, mode 1. */
static inline void Run(MegaMixCtrl *self, SpriteObj *part)
{
    if ((s8)(part->mirror << 3) < 0)
        self->StartTargetMotionXFromSet(part, 3);
    else
        self->SetMotionXFromSet(part, 1);
    self->SetTargetAnim(part, 0);
    self->SetMode(1);
}

/* Stops: anim 2, mode 0, X record 0. */
static inline void Stop(MegaMixCtrl *self, SpriteObj *part)
{
    self->SetTargetAnim(part, 2);
    self->SetMode(0);
    self->StartTargetMotionXFromSet(part, 0);
}

void MegaMixCtrl::Update(SpriteObj *part)
{
    if (stamp == -1) {
        SetMotionXFromSet(part, 1);
        stamp = 0;
    }

    switch (state) {
    case 0:
        if (gPlayer->dead != 0)
            return;
    run:
        Run(this, part);
        return;
    case 1:
        {
            s32 i;
            s32 px;

            if (gPlayer->dead != 0 || ((Ctrl *)gPlayer->ctrl)->state == 0x1E)
                Stop(this, part);
            if (part->IsOnScreen() && latch == 0) {
                stamp = gRoomFrameCount;
                latch = 1;
            } else {
                u8 hit = part->IsOnScreen();

                if (hit == 0 && latch != 0) {
                    stamp = gRoomFrameCount;
                    latch = hit;
                }
            }
            if (stamp != 0 && gRoomFrameCount - stamp > 0x3C) {
                stamp = 0;
                if ((s8)(part->mirror << 3) >= 0) {
                    if (latch != 0)
                        SetMotionXFromSet(part, 1);
                    else
                        SetMotionXFromSet(part, 2);
                } else
                    SetMotionXFromSet(part, 3);
            }
            if (gPlayer->x < part->x) {
                u8 f = part->mirror;

                if ((s8)(f << 3) >= 0) {
                    s32 m = -0x11;

                    m &= f;
                    m |= 0x10;
                    part->mirror = m;
                    StartTargetMotionXFromSet(part, 3);
                }
            }
            /* read into a local first: in place, gcc loads it after the sum */
            px = gPlayer->x;
            if (px > part->x + 0xA00) {
                u8 f = part->mirror;

                if ((s8)(f << 3) < 0) {
                    s32 m = -0x11;

                    m &= f;
                    part->mirror = m;
                    StartTargetMotionXFromSet(part, 1);
                }
            }
            {
                struct player *pl = gPlayer;

                if (ABS_BRANCHLESS(pl->x - part->x) <= 0x27FF &&
                    ABS_BRANCHLESS(pl->y - part->y) <= 0x31FF) {
                    SetMotionXFromSet(part, 0);
                    SetMode(2);
                    SetTargetAnim(part, 1);
                    return;
                }
            }
            /* The hitbox goes to CollidePartList by value, straight from
             * GetSpriteHitbox: a local copy is copied again (memcpy). */
            CollidePartList(gCollidableList, GetSpriteHitbox((struct box_part *)part), 0,
                            (struct box_part *)part);
            /* a guarded do-while: a `for` shares the list pointer between the
             * entry test and the body, where the ROM reloads it */
            i = 0;
            if (i < gCrateList->activeCount) {
                do {
                    struct crate *e = (struct crate *)gCrateList->slotArray[i];

                    if (((Entity *)e)->GetClassId() == 3) {
                        /* ExplodeCrate gets a copy of `e` made here, in its
                         * own register */
                        struct crate *c = e;

                        if (ABS_BRANCHLESS(Q8_TO_INT(e->x) - Q8_TO_INT(part->x)) <= 0x27 &&
                            ABS_BRANCHLESS(Q8_TO_INT(e->y) - Q8_TO_INT(part->y)) <= 0x3B &&
                            (e->state & 0x7F) == 0) {
                            s32 kind = e->kind;

                            /* TNT and nitro blow up */
                            if (kind == 0xE || kind == 0x13 || kind == 0x14 || kind == 0x15 ||
                                kind == 0xA)
                                ExplodeCrate(c, 0);
                            else if (IsCrateKindBreakable(e, kind))
                                BreakCrate(e, 1);
                        }
                    }
                    i++;
                } while (i < gCrateList->activeCount);
            }
            return;
        }
    case 2:
        if (part->frame == 8 && part->stepTimer == 0) {
            struct player *pl = gPlayer;

            /* out of range: run again (the same code as state 0's, which
             * the ROM shares) */
            if (ABS_BRANCHLESS(pl->x - part->x) > 0x27FF ||
                ABS_BRANCHLESS(pl->y - part->y) > 0x31FF)
                goto run;
            /* the player's HandleEvent (gPlayerVtable; the player is
             * still C) */
            {
                const struct actor_method *m = &pl->vtable->handleEvent;
                void *t = (u8 *)pl + m->thisOffset;

                ((void (*)(void *, s32, s32, s32))m->fn)(t, 0, EVENT_HIT, 0);
            }
            return;
        }
        if (!part->animDone)
            return;
        if (PlayerDeadByte(gPlayer) == 0)
            Run(this, part);
        else
            Stop(this, part);
        return;
    }
}
