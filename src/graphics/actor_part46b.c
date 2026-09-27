#include "core.h"
#include "actor_self.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment, docs/matching/issue-56-0x0802f0dc-actor.md and
 * docs/matching/pmf-dispatch-retry.md. */

extern void sub_802E674(s32 x, s32 y, s32 z, s32 dx, s32 dy);
extern u8 sub_802A6EC(void *self);
extern void sub_802A7B8(void *self);
extern struct actor_pmf gStaticData_0817C260[];
extern struct actor_self *gUnknown_03000884;

ACTOR_CALL_VIA_ALIASES

/* sub_803ADB4 is libgcc's `__divsi3`, reached from plain `/`. */
asm(".set __divsi3, sub_803ADB4\n");

struct actor_fa38 {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 cooldown;       // 0x58
    s32 hits;           // 0x5C
    s32 velX;           // 0x60
    s32 velY;           // 0x64
    s32 velZ;           // 0x68
    u8 unk_6C[0x10];
    u8 unk_7C;          // 0x7C
};

/* Per-frame update: flags `self` as "deep" past a depth threshold,
 * integrates its velocity (Q4), runs the per-state member-pointer
 * dispatch `(this->*gStaticData_0817C260[this->state])()`, and while
 * animation 3 plays and the cooldown has run out, pushes the player
 * away (sub_802E674) when it is close in front - every third hit takes
 * a long cooldown. Then the usual player-contact damage exchange, and
 * finally "destroy" once state 3 rises past a height, else the
 * standard sub_802A7B8 step. */
void sub_802FA38(struct actor_fa38 *self)
{
    if (self->base.depth > 0x1B00) {
        self->base.unk_2C[0] = 1;
    } else {
        self->base.unk_2C[0] = 0;
    }
    self->base.x += self->velX >> 4;
    self->base.y += self->velY >> 4;
    self->base.z += self->velZ >> 4;

    ACTOR_PMF_CALL(&self->base, gStaticData_0817C260);

    if (self->base.animIndex == 3) {
        s32 cooldown = self->cooldown;

        if (cooldown == 0) {
            struct actor_self *player = gUnknown_03000884;
            s32 angle = (player->z - (self->base.z - 10)) / -0x1AA;

            if (angle > 0 && self->base.depth <= 0x8BFF) {
                s32 scale = 0x1000 / angle;
                s32 rawDx = (player->x - self->base.x) * scale;
                s32 dx = rawDx >> 12;
                s32 rawDy = (player->y - self->base.y) * scale;
                s32 dy = rawDy >> 12;
                s32 signDx = rawDx >> 31;
                s32 absDx = (dx ^ signDx) - signDx;
                s32 signDy = rawDy >> 31;
                s32 absDy = (dy ^ signDy) - signDy;

                if (absDx + absDy <= 0x5FF) {
                    sub_802E674(self->base.x, self->base.y, self->base.z - 10, dx, dy);
                    if (++self->hits == 3) {
                        self->hits = cooldown;
                        self->cooldown = 0x3C;
                    } else {
                        self->cooldown = 0x14;
                    }
                }
            }
        } else {
            self->cooldown = cooldown - 1;
        }
    }

    if (self->unk_7C == 0 && sub_802A6EC(self)) {
        ACTOR_VCALL(gUnknown_03000884, m20, 6);
        ACTOR_VCALL(&self->base, m20, 4);
    }

    if (self->base.state == 3 && self->base.y > 0xE100) {
        if (self != NULL) {
            ACTOR_VCALL(&self->base, m08, 3);
        }
    } else {
        sub_802A7B8(self);
    }
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");
