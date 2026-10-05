#include "core.h"
#include "action_obj.h"

/* GitHub issue #17, ROM 0x08013C60-0x0801426C, formerly
 * asm/code_3_2_17_12af4.s (details in
 * docs/matching/issue-17-0x08012fbc-actor.md, "Second pass").
 *
 * Five more entries of the gActionCtrlStateTable 42-slot action table
 * (docs/rom_map.md), the same player/action object as
 * actor_part18.c/actor_part_138e8.c: `self+0xc` is its method table,
 * `self+0x10` its on-screen part, `self+0x18`/`+0x1c` an animation
 * counter/limit, and the +0x27..+0x32 bytes the shared "next action"
 * trio (actor_part18.c). Each handler snapshots the input word
 * gKeys (the high half is the newly-pressed buttons) and
 * most also the D-pad direction GetDpadDirection remaps. */

extern u32 gKeys;
extern void *gAudioContext;
extern void *gLevelState;
extern void *gInput;
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern u8 GetDpadDirection(void *pad);
extern u8 HasTornadoSpin(void *self);
extern u8 sub_800AAEC(struct act_part *part, s32 action);
extern u8 sub_8012A7C(struct act *self);
extern void sub_80152F0(struct act *self, u8 dir);
extern void sub_8015238(struct act *self, u8 dir, u32 in);
extern void sub_8015038(struct act *self, s32 id, s32 param);
extern void sub_80151C8(struct act *self);
extern void sub_80134B8(struct act *self);
extern void sub_8015508(struct act *self);

/* ActSetNext for the "fire" paths below. There the ROM loads a fresh 1 for
 * +0x30; plain C reuses the 1 of the preceding `pressed & 1` test, which
 * gcc then keeps in a callee-saved register across the method call (an
 * extra push). The barrier hides the constant, and the pins give its
 * store the ROM's registers. */
static inline void ActSetNextB(struct act *self, s32 next)
{
    register s32 one asm("r0");
    register u8 *flag asm("r1");

    self->next32 = 0;
    flag = &self->flag30;
    one = 1;
    asm("" : "+r"(one));
    *flag = one;
    self->next28 = next;
}

/* Charge-attack step: queues idle (5) once the part leaves contact, jumps
 * to action 7 on fire during contact bit 3, counts alt presses into
 * +0x20 (max 3), and at the animation's end either releases the charge
 * (sub_8015038) or hands off to sub_8015238. The handlers below load
 * GetDpadDirection's argument before taking the input snapshot, hence the
 * `pad` local. */
void sub_8013C60(struct act *self)
{
    u32 in;
    u8 dir;

    {
        void *pad = gInput;

        in = gKeys;
        dir = GetDpadDirection(pad);
    }
    if (self->part->contact == 0)
    {
        ActSetNext(self, 5);
    }
    if ((INPUT_PRESSED(in) & 1) && (self->part->contact & 8))
    {
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        ACT_VCALL1(self, m20, 0xE);
        ActSetNextB(self, 7);
        self->unk_22 = 0;
        self->unk_23 = 0;
        self->part->contact = 0;
        return;
    }
    if (HasTornadoSpin(gLevelState) && (INPUT_PRESSED(in) & 2) && self->spinCooldown == 0)
    {
        if (++self->charge > 3)
            self->charge = 3;
    }
    sub_80152F0(self, dir);
    if (++self->frame >= self->frames || self->part->animDone)
    {
        if (self->charge)
            sub_8015038(self, 0xF, 0xD);
        else
            sub_8015238(self, dir, in);
    }
}

void sub_8013D94(struct act *self)
{
    u32 in;
    struct act_part *part;

    in = gKeys;
    part = self->part;

    if ((part->contact & 8) && part->speedY > 0)
    {
        ActOrFlags0D(part, 1);
        self->unk_34 = 0;
        ACT_VCALL1(self, m20, 0xD);
        self->next32 = 0;
        self->flag30 = 1;
        self->next28 = 0;
        sub_8013C60(self);
        return;
    }
    if (HasTornadoSpin(gLevelState) && (INPUT_PRESSED(in) & 2) && self->spinCooldown == 0)
    {
        if (++self->charge > 3)
            self->charge = 3;
    }
    {
        s32 frame = ++self->frame;
        s32 frames = self->frames;

        part = self->part;
        if (frame < frames && !part->animDone)
            goto done;
    }
    {
        u8 charge;

        ActOrFlags0D(part, 1);
        charge = self->charge;
        if (charge)
        {
            sub_8015038(self, 0xE, 0xE);
        }
        else
        {
            self->spinCooldown = 0xC;
            ACT_VCALL1(self, m20, 0x1A);
            ACT_VCALL2(self, m50, self->part, 0x15);
            self->frame = charge;
            self->frames = charge;
        }
    }
done:
    sub_80134B8(self);
}

void sub_8013EAC(struct act *self)
{
    u32 in;
    u8 dir;

    {
        void *pad = gInput;

        in = gKeys;
        dir = GetDpadDirection(pad);
    }
    if (self->part->contact == 0)
    {
        if (self->unk_22)
        {
            sub_80151C8(self);
        }
        else
        {
            ActSetNext(self, 5);
        }
    }
    if ((INPUT_PRESSED(in) & 1) && (self->part->contact & 8))
    {
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        ACT_VCALL1(self, m20, 0xE);
        ActSetNextB(self, 7);
        self->unk_23 = 0;
        self->part->contact = 0;
        return;
    }
    if (HasTornadoSpin(gLevelState) && (INPUT_PRESSED(in) & 2) && self->spinCooldown == 0)
    {
        if (++self->charge > 3)
            self->charge = 3;
    }
    sub_80152F0(self, dir);
    if (++self->frame >= self->frames || self->part->animDone)
        sub_8015038(self, 0xF, 0xD);
}

void sub_8013FD4(struct act *self)
{
    u32 in;
    s32 fire;

    in = gKeys;
    fire = INPUT_PRESSED(in) & 1;

    if (fire)
    {
        PlaySfx(gAudioContext, 0xC, 0x100);
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        sub_8015508(self);
        return;
    }
    if (self->part->contact == 8 && (u8)(self->next28 - 4) <= 1)
    {
        self->next32 = fire;
        self->flag30 = 1;
        self->next28 = fire;
    }
    if (self->part->animDone)
    {
        ACT_VCALL1(self, m20, 0x11);
        ACT_VCALL2(self, m50, self->part, 4);
    }
}

/* On the "confirm" edge (sub_800AAEC(part, 0xB)) hands off to
 * sub_8015508 like sub_80142B0 (actor_part18.c); otherwise, unless
 * sub_8012A7C reports busy, turns the part to face the D-pad direction
 * (setting +0x2F), starts a walk (action 3) on a horizontal direction,
 * and - with neither shoulder button held - either starts action 2 on a
 * sub_800AAEC(part, 2) hit or falls back to idle.
 *
 * old_agbcc. The facing block reads `self->part` each time (GCSE turns
 * the reloads into the ROM's r2 copy) and spells its two bit tests
 * differently so gcc doesn't thread the second into the first. The
 * second branch writes through a scoped `volatile u8 *` so its `adds r2,
 * #40` stays in place ahead of the -0x11 mask - see
 * docs/matching/issue-15-16-17-naked-retry-2.md. */
static inline void ActQueue27(struct act *self, s32 cur, s32 next)
{
    self->next31 = cur;
    self->flag2F = 1;
    self->next27 = next;
}

void sub_8014084(struct act *self)
{
    u32 in;
    u8 dir;
    s32 turned;
    s32 moved;

    {
        void *pad = gInput;

        in = gKeys;
        dir = GetDpadDirection(pad);
    }
    if ((INPUT_PRESSED(in) & 1) && sub_800AAEC(self->part, 0xB) == 1)
    {
        PlaySfx(gAudioContext, 0xC, 0x100);
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        sub_8015508(self);
        return;
    }
    if (sub_8012A7C(self))
        return;

    turned = 0;
    if ((s32)(self->part->flags28 << 27) < 0 && (dir == 4 || dir == 6 || dir == 8))
    {
        u8 *p = &self->part->flags28;
        s32 m = -0x11;

        m &= *p;
        *p = m;
        self->flag2F = 1;
        turned = 1;
        goto turn_done;
    }
    if ((s8)(self->part->flags28 << 3) >= 0 && (dir == 3 || dir == 5 || dir == 7))
    {
        s32 m;

        turned = 1;
        {
            /* volatile: keeps the `+0x28` address in the part copy's
             * register and computed ahead of the -0x11 mask, as in the
             * ROM (a plain pointer lands in a fresh register) */
            volatile u8 *p = &self->part->flags28;

            m = -0x11;
            m &= *p;
            m |= 0x10;
            *p = m;
        }
        self->flag2F = turned;
    }
turn_done:

    moved = 0;
    if (!turned)
    {
        switch (GetDpadDirection(gInput))
        {
        case 3:
        case 4:
        case 7:
        case 8:
            ACT_VCALL1(self, m20, 0x13);
            ACT_VCALL2(self, m50, self->part, 0x14);
            ActQueue27(self, moved, 3);
            moved = 1;
            break;
        }
    }

    {
        s32 held = INPUT_HELD(in) & 0x180;

        if (held == 0)
        {
            u8 hit = sub_800AAEC(self->part, 2);

            if (hit == 1)
            {
                ACT_VCALL1(self, m20, 0x12);
                ACT_VCALL2(self, m50, self->part, 2);
                self->next31 = held;
                self->flag2F = hit;
                self->next27 = held;
            }
            else if (!moved)
            {
                ACT_VCALL1(self, m20, 0x11);
                ACT_VCALL2(self, m50, self->part, 4);
                self->next31 = moved;
                self->flag2F = 1;
                self->next27 = moved;
                self->next32 = moved;
                self->flag30 = 1;
                self->next28 = moved;
            }
        }
    }
}
asm(".align 2, 0");
