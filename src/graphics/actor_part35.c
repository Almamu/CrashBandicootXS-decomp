#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern s32 GetHovercraftX(void);
extern s32 GetHovercraftY(void);
extern s32 GetHovercraftZ(void);
extern struct spawn_timing_table *sub_80338C4(void);
extern s32 GetHovercraftState(void);
extern s32 RandRange(s32 arg0);
extern void CreateJetpackActor(s32 kind, s32 x, s32 y, s32 z, s32 arg4);
extern struct actor_self *gActorList;

/* The singleton's per-spawner timing table (`sub_80338C4`): after each
 * spawn a spawner waits `delay` frames, except every `burst`-th spawn,
 * which resets its count and waits `burstDelay` instead. One record per
 * spawner kind (actor_part67.c reads [0], actor_part29.c [1],
 * actor_part35.c [2]). */
struct spawn_timing {
    s32 delay;
    s32 burst;
    s32 burstDelay;
};

struct spawn_timing_table {
    s32 unk_00;
    struct spawn_timing timing[3];  // 0x04
};

/* A spawner object of the singleton system (actor_part28.c):
 * `actor_self` plus a hit-point word, its spawn cooldown/count and a
 * "dead" flag. */
struct spawner {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 spawnX;         // 0x58 - the constructor's `b`/`c` (CreateHovercraftLauncher)
    s32 spawnY;         // 0x5C
    u8 unk_60[4];
    s32 cooldown;       // 0x64
    s32 count;          // 0x68
    u8 dead;            // 0x6C
};

/* HovercraftLauncherStateLaunch: `HovercraftCannonStateFire`'s sibling. Sets `self`'s position fields
 * from the singleton's own position plus a different fixed offset,
 * and - while `cooldown` is zero - measures `self`'s
 * distance to the player the same way; in range, it picks one of three
 * spawn "kinds" (5/6/8, via `RandRange(3)`) and calls `CreateJetpackActor`
 * at `self`'s position, then cycles `count` against a threshold
 * from `sub_80338C4`'s table. Once `base.depth` passes `0x4B00` and the
 * singleton's own "kind" (`GetHovercraftState`) is 3, resets `self` back to
 * its idle animation state.
 *
 * Matched in a later pass with the same shape as `HovercraftCannonStateFire` (see
 * docs/matching/issue-62-0x08033804-actor.md, "Later pass: strag2 retry"): no
 * register pins at all - the old `r7` blocker came from a wrong
 * source shape, not from a register the allocator couldn't reach. */
void HovercraftLauncherStateLaunch(struct spawner *self)
{
    s32 slot;
    s32 next;

    self->base.x = GetHovercraftX() + 0x1E00;
    self->base.y = GetHovercraftY() - 0x3000;
    self->base.z = GetHovercraftZ() - 0x100;

    slot = self->cooldown;
    if (slot == 0) {
        struct actor_self *player = gActorList;
        s32 angle = (player->z - self->base.z) / -0x1AA;

        if (angle > 0) {
            s32 scale = 0x1000 / angle;
            s32 rawDx = (player->x - self->base.x) * scale;
            s32 dx = rawDx >> 12;
            s32 rawDy = (player->y - self->base.y) * scale;
            s32 dy = rawDy >> 12;
            s32 signDx = rawDx >> 31;
            s32 absDx = (dx ^ signDx) - signDx;
            s32 signDy = rawDy >> 31;
            s32 absDy = (dy ^ signDy) - signDy;

            if (absDx + absDy <= 0xFFF) {
                s32 kind = (u16)RandRange(3);
                struct spawn_timing_table *table;
                s32 count;

                if (kind == 0) {
                    CreateJetpackActor(5, self->base.x, self->base.y, self->base.z, slot);
                } else if (kind == 1) {
                    CreateJetpackActor(6, self->base.x, self->base.y, self->base.z, slot);
                } else {
                    CreateJetpackActor(8, self->base.x, self->base.y, self->base.z, slot);
                }

                count = self->count + 1;
                self->count = count;
                table = sub_80338C4();
                if (count == table->timing[2].burst) {
                    self->count = 0;
                    table = sub_80338C4();
                    next = table->timing[2].burstDelay;
                } else {
                    table = sub_80338C4();
                    next = table->timing[2].delay;
                }
                goto store;
            }
        }
    } else {
        next = slot - 1;
    store:
        self->cooldown = next;
    }

    if (self->base.depth > 0x4B00 && GetHovercraftState() == 3) {
        s32 zero32;
        s32 state;

        /* The ROM materializes the 0 and then the 2 before the stores
         * (`movs r2, #0; movs r0, #2`); the "=r"/"0" escapes keep both
         * as registers, and the volatile one stops the 2 from being
         * sunk to its store. */
        asm("" : "=r"(zero32) : "0"(0));
        asm volatile("" : "=r"(state) : "0"(2));
        self->base.state = zero32;
        self->base.stateTime = zero32;
        self->base.animIndex = state;
        {
            u16 anim = self->base.anims[2].duration;
            u8 zero;

            /* separate byte zero: the ROM materializes its own movs for it */
            asm("" : "=r"(zero) : "0"(0));
            self->base.animTimer = anim;
            self->base.animDone = zero;
        }
        self->base.animTime = zero32;
    }
}

asm(".align 2, 0");
