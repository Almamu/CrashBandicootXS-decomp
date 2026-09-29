#include "core.h"
#include "actor_self.h"

/* Same singleton system as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern struct spawn_timing_table *sub_80338C4(void);
asm(".set __divsi3, sub_803ADB4");
extern void sub_802E674(s32 x, s32 y, s32 z, s32 dx, s32 dy);
extern void sub_802E504(s32 x, s32 y, s32 z);
extern struct actor_self *gUnknown_03000884;

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
    s32 spawnX;         // 0x58 - the constructor's `b`/`c` (sub_8033EF4)
    s32 spawnY;         // 0x5C
    u8 unk_60[4];
    s32 cooldown;       // 0x64
    s32 count;          // 0x68
    u8 dead;            // 0x6C
};

/* sub_80339DC: a proximity-triggered effect/hazard detector. Syncs
 * `self`'s position fields to the singleton's current position (plus a
 * fixed offset), and - while the `cooldown` slot is zero -
 * measures `self`'s distance to the player; in range, it spawns a pair
 * of effects at `self`'s position and cycles `count` against a
 * threshold from `sub_80338C4`'s table. Once `base.depth` passes
 * `0x4B00` it resets `self` to its idle animation state.
 *
 * Matched in a later pass (see docs/matching/issue-62-0x08033804-actor.md,
 * "Later pass: strag2 retry"): both divisions are plain `/` through the ROM's own
 * `__divsi3` (`sub_803ADB4`) - as a libcall they don't clobber memory, so
 * `base.z` stays CSE'd in `r6` across them - the divisor is -0x1AA
 * (not -0xAA), and the new cooldown value is stored at one shared
 * `store:` label from all three paths. */
void sub_80339DC(struct spawner *self)
{
    s32 slot;
    s32 next;

    self->base.x = sub_8033900() + 0x2000;
    self->base.y = sub_80338F4() + 0x3000;
    self->base.z = sub_80338E8() - 0x100;

    slot = self->cooldown;
    if (slot == 0) {
        struct actor_self *player = gUnknown_03000884;
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
                struct spawn_timing_table *table;
                s32 count;

                sub_802E674(self->base.x, self->base.y, self->base.z, dx, dy);
                sub_802E504(self->base.x, self->base.y, self->base.z);

                count = self->count + 1;
                self->count = count;
                table = sub_80338C4();
                if (count == table->timing[1].burst) {
                    self->count = slot;
                    table = sub_80338C4();
                    next = table->timing[1].burstDelay;
                } else {
                    table = sub_80338C4();
                    next = table->timing[1].delay;
                }
                goto store;
            }
        }
    } else {
        next = slot - 1;
    store:
        self->cooldown = next;
    }

    if (self->base.depth > 0x4B00) {
        self->base.state = 0;
        self->base.stateTime = 0;
        self->base.animIndex = 0;
        {
            u16 anim = self->base.anims[0].duration;
            u8 zero;

            /* separate byte zero: the ROM materializes its own movs for it */
            asm("" : "=r"(zero) : "0"(0));
            self->base.animTimer = anim;
            self->base.animDone = zero;
        }
        self->base.animTime = 0;
    }
}

asm(".align 2, 0");
