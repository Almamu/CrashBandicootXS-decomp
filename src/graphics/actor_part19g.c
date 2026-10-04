#include "core.h"
#include "actor_self.h"

/* Continuation of actor_part19.c's player/action-object family, right
 * after the parked `sub_802C3E8` (see actor_part19c2.c) - same `self`
 * object (`struct actor_self`) and conventions documented there. The
 * animation-reset blocks (the `anim`/`zero1`/`zero2` register trios)
 * store through `*(T *)&self->field` casts: plain member stores let
 * gcc move the zero loads (docs/workflow.md step 7). */

/* Anonymous 12-byte (3-word) copy unit - see actor_part19c.c's own
 * copy of this comment for why this shape (rather than three separate
 * `s32` field copies) is needed to reproduce the ROM's `ldm`/`stm`
 * lowering for `self+0x38`'s refresh from `gStaticData_0817A768`. */
struct vec3_words {
    s32 a, b, c;
};

/* `sub_802C614`'s class adds one field after the common prefix: an
 * object it hands to `sub_802AAB4`'s 15-entry list. */
struct listed_actor {
    struct actor_self base;
    void *unk_54;               // 0x54
};

extern void *gAudioContext;
extern void *gLevelState;
extern void *gUnknown_03000884;

extern u8 gStaticData_087E4E94[];
extern u8 gStaticData_0817A768[];

extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern u8 sub_802A6EC(void *self);
extern void sub_802A7B8(void *self);
extern u8 sub_802DD9C(void *self);
extern void sub_8022FEC(void *self);
extern void sub_802AAB4(void *arg0);
extern void sub_802B730(void *arg0);
extern void sub_8029720(void);
extern void sub_802C7A8(void *self);
extern void sub_802C078(void *arg0, s32 delta);
extern void sub_802C128(void *arg0);
extern void sub_802C0A8(void *arg0);
extern void sub_802C4C8(void *selfArg);

/* On proximity (`sub_802A6EC`), accumulates `1` into the shared
 * `gUnknown_03000884`-targeted accumulator via `sub_802C078` then fires
 * the `vtable` trampoline (behind this family's `if (self)` guard);
 * otherwise tail-calls `sub_802A7B8(self)`. */
void sub_802C464(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (sub_802A6EC(self)) {
        sub_802C078(gUnknown_03000884, 1);
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->m08.thisOffset, (void *)3, table->m08.fn);
        }
    } else {
        sub_802A7B8(self);
    }
}

/* Thin `InitActorPart`-based constructor: forwards its own `a`/`b`/`c`
 * parameters straight through (untouched, same registers) plus the
 * caller's last stack argument, then sets `vtable` to
 * `gStaticData_087E4E94` - one of the "spawn effect type N" family
 * documented in docs/rom_map.md. */
void *sub_802C4A4(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitActorPart(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gStaticData_087E4E94;
    return self;
}

/* Shared cleanup/tail step for this actor family (per docs/rom_map.md):
 * once (state != 0x12 and `sub_802DD9C`'s overlap test passes),
 * transitions to the shared "used" state 0x12 (anim frame from
 * `self`'s part-table pointer at `+0xd8`) with a sound cue and the
 * lap-counter tie `sub_8022FEC`. Either way, fires the `vtable`
 * trampoline once state is (already, or now) 0x12 and `animDone` is
 * set; otherwise tail-calls `sub_802A7B8`. */
void sub_802C4C8(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12) {
        if (sub_802DD9C(self)) {
            PlaySfx(gAudioContext, 3, 0x100);
            sub_8022FEC(gLevelState);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
        }
    }

    if (self->animIndex == 0x12 && self->animDone != 0) {
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->m08.thisOffset, (void *)3, table->m08.fn);
        }
        return;
    }

    sub_802A7B8(self);
}

/* Extends the shared "type-byte event dispatch" family
 * (`sub_8031D7C`/etc., per docs/rom_map.md) to value range `0x1c`-
 * `0x1f`, reading the type byte through one extra pointer indirection
 * (`self+0x30`). Ties into the wraparound-lap-counter system via
 * `sub_8022FEC` and dispatches accumulator/lock-timer calls
 * (`sub_802C078`/`sub_802C128`) before tail-calling the shared cleanup
 * `sub_802C4C8`. */
void sub_802C540(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12 && sub_802A6EC(self)) {
        s32 typeByte;

        sub_8022FEC(gLevelState);
        typeByte = **(u8 **)((u8 *)self + 0x30);

        if (typeByte == 0x1d) {
            goto case_1d;
        }
        if (typeByte > 0x1d) {
            goto gt_1d_dispatch;
        }
        if (typeByte == 0x1c) {
            goto case_1c;
        }
        goto state_block;

    gt_1d_dispatch:
        if (typeByte == 0x1e) {
            goto case_1e;
        }
        if (typeByte == 0x1f) {
            goto case_1f;
        }
        goto state_block;

    case_1c:
        PlaySfx(gAudioContext, 3, 0x100);
        sub_802C078(gUnknown_03000884, 1);
        goto state_block;

    case_1d:
        PlaySfx(gAudioContext, 3, 0x100);
        sub_802C078(gUnknown_03000884, 3);
        goto state_block;

    case_1e:
        PlaySfx(gAudioContext, 3, 0x100);
        sub_802C078(gUnknown_03000884, 5);
        goto state_block;

    case_1f:
        sub_802C128(gUnknown_03000884);

    state_block:
        self->animIndex = 0x12;
        {
            register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            *(s32 *)&self->animTime = zero2;
        }
    }

    sub_802C4C8(self);
}

/* Extends the lap-counter/proximity-dispatch family: on proximity
 * (`sub_802A6EC`), plays a sound, ties the lap counter, forwards the
 * global player pointer to `sub_802C0A8` and `unk_54` to
 * `sub_802AAB4`, then (whether or not that first branch fired) on
 * `sub_802DD9C`'s overlap test transitions to the shared "used" state
 * a second time with its own sound cue - both branches finish with the
 * same state-0x12 transition block before tail-calling `sub_802C4C8`. */
void sub_802C614(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12) {
        if (sub_802A6EC(self)) {
            PlaySfx(gAudioContext, 7, 0x100);
            sub_8022FEC(gLevelState);
            sub_802C0A8(gUnknown_03000884);
            sub_802AAB4(((struct listed_actor *)self)->unk_54);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
            self->unk_18 = 1;
        }

        if (self->animIndex != 0x12 && sub_802DD9C(self)) {
            PlaySfx(gAudioContext, 3, 0x100);
            sub_8022FEC(gLevelState);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
            self->unk_18 = 1;
        }
    }

    sub_802C4C8(self);
}

/* Once already in the "used" state (0x12): refreshes `self+0x38`
 * (a 12-byte AABB, from `gStaticData_0817A768`) and, once
 * `stateTime` reaches `0x14`, calls `sub_802C7A8` (still raw - see
 * docs/matching.md). Otherwise, while `depth` (a lap/lifetime
 * counter) exceeds `0xa000`, calls `sub_8029720` and fires the
 * `vtable` trampoline; else on proximity or overlap, plays a sound,
 * ties the lap counter and the homing-chase helper `sub_802B730`,
 * and transitions to the "used" state. Tail-calls `sub_802C4C8`. */
void sub_802C6C0(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex == 0x12) {
        goto usedState;
    }

    if (self->depth > 0xa000) {
        sub_8029720();
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->m08.thisOffset, (void *)3, table->m08.fn);
        }
        return;
    } else {
        register u32 raw asm("r0") = sub_802A6EC(self);
        register u32 found asm("r5");

        raw = raw << 24;
        found = raw >> 24;

        if (found) {
            PlaySfx(gAudioContext, 4, 0x100);
            sub_8022FEC(gLevelState);
            sub_802B730(gUnknown_03000884);
            {
                register s32 zero2 asm("r2") = 0;

                self->stateTime = zero2;
                self->animIndex = 0x12;
                {
                    register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero1;
                }
                *(s32 *)&self->animTime = zero2;
            }
        } else if (sub_802DD9C(self)) {
            PlaySfx(gAudioContext, 4, 0x100);
            sub_8022FEC(gLevelState);
            {
                register s32 zero2 asm("r5") = found;

                self->stateTime = zero2;
                self->animIndex = 0x12;
                {
                    register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero1;
                }
                *(s32 *)&self->animTime = zero2;
            }
        }
    }
    goto tail;

usedState:
    *(struct vec3_words *)((u8 *)self + 0x38) = *(struct vec3_words *)gStaticData_0817A768;

    if (self->stateTime == 0x14) {
        sub_802C7A8(self);
    }

tail:
    sub_802C4C8(self);
}

asm(".align 2, 0");
