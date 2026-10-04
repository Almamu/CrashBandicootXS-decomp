#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern void *gAudioContext;

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

extern void sub_8033804(void);
extern void sub_803388C(void);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);

/* Applies `dmg` damage to `hp`, and once it drops to zero (or
 * below), marks `self` dead (`dead = 1`), fires the singleton's own
 * death transition (`sub_803388C`), switches `self` to its death
 * state/anim (state 2, table-index 2, anim frame from `self`'s part
 * table at `+0x18`), and plays the death sound; otherwise just plays a
 * hit sound. The `*(T *)&self->...` stores keep gcc from treating them as
 * struct-member accesses, which changes where the byte zero is built. */
void DamageHovercraftCannon(struct spawner *self, s32 dmg)
{

    sub_8033804();
    self->hp -= dmg;

    if (self->hp <= 0) {
        u8 *deadFlag = &self->dead;
        register s32 zero asm("r4") = 0;

        *deadFlag = 1;
        sub_803388C();
        {
            register s32 stateVal asm("r0") = 2;

            self->base.state = stateVal;
            self->base.stateTime = zero;
            self->base.animIndex = stateVal;
            {
                register u16 anim asm("r0") = self->base.anims[2].duration;
                register u8 zero2 asm("r1") = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero2;
            }
            self->base.animTime = zero;
        }
        PlaySfx(gAudioContext, 4, 0x100);
    } else {
        PlaySfx(gAudioContext, 0x45, 0x100);
    }
}
