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

/* `DamageHovercraftCannon`'s gated twin: only applies damage while `self` is in
 * state 1. On death, uses table-index 3 and the anim frame from
 * `self`'s part table `+0x24` field (instead of `DamageHovercraftCannon`'s
 * table-index 2/`+0x18`), and reuses the just-checked `state` value
 * (always 1 here) for the death-flag store, matching the ROM's literal
 * register reuse. The `*(T *)&self->...` stores keep gcc from treating them
 * as struct-member accesses, which changes where the byte zero is built. */
void DamageHovercraftLauncher(struct spawner *self, s32 dmg)
{
    s32 state = self->base.state;

    if (state == 1) {
        sub_8033804();
        self->hp -= dmg;

        if (self->hp <= 0) {
            u8 *deadFlag = &self->dead;
            register s32 zero asm("r4") = 0;

            *deadFlag = state;
            sub_803388C();
            {
                register s32 stateVal asm("r0") = 2;
                register s32 three asm("r1") = 3;

                self->base.state = stateVal;
                self->base.stateTime = zero;
                self->base.animIndex = three;
            }
            {
                register u16 anim asm("r0") = self->base.anims[3].duration;
                register u8 zero2 asm("r1") = 0;

                *(u16 *)&self->base.animTimer = anim;
                *(u8 *)&self->base.animDone = zero2;
            }
            self->base.animTime = zero;
            PlaySfx(gAudioContext, 4, 0x100);
        } else {
            PlaySfx(gAudioContext, 0x45, 0x100);
        }
    }
}
