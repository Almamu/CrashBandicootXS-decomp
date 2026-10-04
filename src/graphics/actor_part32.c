#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

/* This family's fields after the common `actor_self` prefix. The
 * animation-reset block stores through `*(T *)&self->field` casts:
 * plain member stores let gcc move the zero load (docs/workflow.md
 * step 7). */
struct health_actor {
    struct actor_self base;
    s32 health;                 // 0x54
    s32 unk_58;                 // 0x58 - constructor arg `b`
    s32 unk_5c;                 // 0x5c - constructor arg `c`
    u8 unk_60[4];
    s32 unk_64;                 // 0x64
    s32 unk_68;                 // 0x68
    u8 dead;                    // 0x6c
};

extern u8 gHovercraftCannonVtable[];

extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern void sub_8029E28(s32 arg0);
extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);
extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern void *sub_80338C4(void);

/* Constructor: forwards to `InitActorPart`, then sets `self`'s health
 * (`+0x54=15`), event/trampoline table (`+0x50=&gHovercraftCannonVtable`),
 * and caches its own `b`/`c` constructor args at `+0x58`/`+0x5c`;
 * finally resets state (`+0x28=0`) and the death flag (`+0x6c=0`).
 * Returns `self`. */
void *CreateHovercraftCannon(void *selfArg, s32 a, s32 b, s32 cParam, s32 d)
{
    struct health_actor *self = selfArg;
    register s32 bReg asm("r6") = b;
    register s32 c asm("r8") = cParam;
    register s32 dReg asm("r0") = d;
    register s32 health asm("r5") = 15;

    InitActorPart(self, a, b, cParam, dReg);
    self->health = health;
    self->base.vtable = (struct actor_vtable *)gHovercraftCannonVtable;
    self->unk_58 = bReg;
    self->unk_5c = c;
    self->base.state = 0;
    self->dead = 0;

    return self;
}

/* Plays a fixed sound cue (`sub_8029E28(0x400)`), then - if `self`'s
 * `+0x12` flag is set - fires the `vtable` event table's slot-3
 * trampoline at `self` offset by the table's `+8` halfword. */
void sub_8033BFC(void *selfArg)
{
    struct health_actor *self = selfArg;

    sub_8029E28(0x400);

    if (self->base.animDone != 0 && self != NULL) {
        register u8 *table asm("r1") = (u8 *)self->base.vtable;
        register u8 *addr asm("r0");
        void *fn;

        {
            register s32 eight asm("r2") = 8;

            addr = (u8 *)self + *(s16 *)((u8 *)table + eight);
        }
        fn = *(void **)(table + 0xc);

        _call_via_r2(addr, (void *)3, fn);
    }
}

/* Sets `self`'s position fields (`+0x1c`/`+0x20`/`+0x24`) from the
 * singleton's own position plus a fixed offset, and - while `depth`
 * is still under its `0x4AFF` threshold - switches `self` to state 1/
 * table-index 1, seeding `+0x64`/`+0x68` from `gUnknown_030015DC`'s
 * table and resetting the anim-frame pair from `self`'s part table's
 * `+0xc` field. */
void sub_8033C28(void *selfArg)
{
    struct health_actor *self = selfArg;

    self->base.x = sub_8033900() + 0x2000;
    self->base.y = sub_80338F4() + 0x3000;
    self->base.z = sub_80338E8() - 0x100;

    if (self->base.depth <= 0x4AFF) {
        s32 *table = sub_80338C4();

        self->unk_64 = table[4];
        {
            register s32 zero asm("r2") = 0;

            self->unk_68 = zero;
            {
                register s32 one asm("r0") = 1;

                self->base.state = one;
                self->base.stateTime = zero;
                self->base.animIndex = one;
                {
                    register u16 anim asm("r0") = *(u16 *)&self->base.anims[1].duration;
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)&self->base.animTimer = anim;
                    *(u8 *)&self->base.animDone = zero2;
                }
                self->base.animTime = zero;
            }
        }
    }
}
