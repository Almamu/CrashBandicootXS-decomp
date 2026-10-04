#include "core.h"
#include "action_obj.h"

/* GitHub issue #17, ROM 0x08014674-0x08014F8C, formerly
 * asm/code_3_2_17_14674.s (details in
 * docs/matching/issue-17-0x08012fbc-actor.md, "Second pass").
 *
 * More gStaticData_0816BF20 action-table handlers for the player/action
 * object (include/action_obj.h). Built with old_agbcc. */

asm(".set _call_via_r2, sub_803AD80\n"
    ".set _call_via_r3, sub_803AD84\n");

extern u32 gUnknown_030007E0;
extern void *gEntityFlags;
extern void *gUnknown_030012B8;
extern void *gUnknown_030012BC;
extern struct act_part *gUnknown_030012D8;
extern void *gUnknown_03001304;
extern u8 gStaticData_0816B300[];
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern u8 sub_8000760(void *pad);
extern void sub_8015398(struct act *self);
extern void sub_8015780(struct act *self, s32 a, s32 b, s32 c, s32 d);
extern void sub_8006D08(void *cache, s32 slot, s32 kind);
extern void sub_80153FC(struct act *self);
extern u8 sub_80122CC(struct act *self);
extern void *sub_80083B8(struct act_part *part);
extern void sub_80087C0(struct act_part *p);
extern void sub_80087B4(struct act_part *p);
extern void sub_800872C(struct act_part *p, s32 arg1);

void sub_8014B54(struct act *self);

/* Queues action `next` on the +0x31/+0x2F/+0x27 trio (as ActSetNext) */
static inline void ActSetNext27(struct act *self, s32 next)
{
    self->next31 = 0;
    self->flag2F = 1;
    self->next27 = next;
}

/* The same with the +0x31 value as a parameter too: both are materialized
 * before the stores. */
static inline void ActQueue27(struct act *self, s32 cur, s32 next)
{
    self->next31 = cur;
    self->flag2F = 1;
    self->next27 = next;
}

static inline void SetTag(struct act_part *part, s32 tag)
{
    part->tag = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
}

static inline void ActTrio28(struct act *self, s32 a, s32 b, s32 c)
{
    self->next32 = a;
    self->flag30 = b;
    self->next28 = c;
}

static inline void ActHold27(struct act *self, u8 *slot, s32 next)
{
    self->next31 = 0;
    self->flag2F = next;
    *slot = next;
}

/* On contact (part+0x68 bit 3), picks the landing action from the part's
 * tag (0xD: the +0x29-gated landing, 0x18: re-arm +0x29) or state 0xE;
 * otherwise handles the fire/alt/shoulder inputs and, with the D-pad
 * idle, clears the +0x31/+0x2F/+0x27 trio.
 *
 * The tag test is a `switch` with a shared 0xD/0x18 case: the ROM's
 * `beq` for 0xD is threaded past the inner re-test of 0xD while the
 * 0x18 path keeps it, which an `||` test does not reproduce (see
 * docs/matching/mix-naked-retry-5.md). */
void sub_8014674(struct act *self)
{
    u8 hit = self->part->contact & 8;

    if (hit)
    {
        u8 tag;
        /* the ROM's r5 zero, reused by the state-0xE else trio; without
         * it that trio stores the `& 0x30` result register */
        u8 z;

        ACT_PART_FLAGS0D(self->part) |= 1;
        {
            u8 *p34 = &self->unk_34;

            z = 0;
            *p34 = z;
        }
        switch (tag = self->part->tag)
        {
        case 0xD:
        case 0x18:
            if (tag == 0xD)
            {
                if (self->unk_29)
                {
                    self->frame = 0;
                    ACT_VCALL2(self, m50, self->part, 0x18);
                    ACT_VCALL1(self, m20, 4);
                    ActQueue27(self, 0, 0x1B);
                }
                else
                {
                    self->frame = 0;
                    ACT_VCALL1(self, m20, 3);
                    {
                        u8 *slot = &self->next27;

                        if (*slot != 1)
                            ActHold27(self, slot, 1);
                    }
                }
                ActSetNext(self, 0);
            }
            else if (tag == 0x18)
            {
                ACT_VCALL1(self, m20, 4);
                self->next32 = 0;
                self->flag30 = 1;
                self->next28 = 0;
                self->unk_29 = 1;
                ActQueue27(self, 0, 0x1B);
            }
            break;
        default:
            if (self->state == 0xE)
            {
                if (gUnknown_030007E0 & 0x30)
                {
                    ActQueue27(self, 0, 1);
                }
                else
                {
                    self->next31 = z;
                    self->flag2F = 1;
                    self->next27 = z;
                }
                ActSetNext(self, 0);
                ACT_VCALL1(self, m20, 0xD);
            }
            else
            {
                ACT_VCALL1(self, m20, 0);
                self->next32 = z;
                self->flag30 = 1;
                self->next28 = 0;
                self->next31 = 0;
                self->flag2F = 1;
                self->next27 = 0;
            }
        }
        return;
    }
    {
        u32 in = gUnknown_030007E0;
        s32 fire;
        s32 one;
        u16 p;

        p = INPUT_PRESSED(in);
        one = 1;
        /* a fresh 1 for `fire` (the ROM's `movs r3, #1; ands r3, r1`),
         * not a copy of `one` */
        asm("" : "=r"(fire) : "0"(1));
        fire &= p;
        /* extra reference: keeps `one` in r6 and the input pointer in r7 */
        asm("" : : "r"(one));
        if (fire)
        {
            ACT_VCALL1(self, m20, 5);
            ACT_VCALL2(self, m50, self->part, 0x13);
            self->frame = hit;
            ActTrio28(self, hit, one, 7);
        }
        else
        {
            u16 alt;
            s32 t = 2;

            t &= p;
            alt = t;

            if (alt)
            {
                ActOrFlags0D(self->part, 1);
                self->unk_34 = fire;
                if (gUnknown_030007E0 & 0x30)
                {
                    self->next31 = fire;
                    self->flag2F = one;
                    self->next27 = one;
                }
                else
                {
                    self->next31 = 0;
                    self->flag2F = one;
                    self->next27 = 0;
                }
                if ((u32)(self->state - 0xD) > 1)
                    sub_8015398(self);
                else
                    ACT_VCALL1(self, m20, 0xD);
            }
            else if (INPUT_HELD(in) & 0x100)
            {
                ActOrFlags0D(self->part, 1);
                self->unk_34 = alt;
                ACT_VCALL1(self, m20, 0x10);
                ACT_VCALL2(self, m50, self->part, 3);
                self->frames = alt;
                self->next31 = alt;
                self->flag2F = one;
                self->next27 = alt;
            }
        }
    }
    {
        u8 dir = sub_8000760(gUnknown_03001304);

        if (dir == 0)
        {
            self->next31 = dir;
            self->flag2F = 1;
            self->next27 = dir;
        }
    }
}

void sub_8014940(struct act *self)
{
    struct act_part *part = self->part;

    if (part->tag == 0x2F && part->frame == 3 && part->unk_34 == 0)
        PlaySfx(gUnknown_030012BC, 0x2E, 0x100);
    part = self->part;
    if (part->animDone)
    {
        part->flags0C |= 1;
        {
            /* the "mark part gone" bitmap set of actor_part_188d0.c's
             * MARK_GONE_BITMAP, with the same load-bearing registers */
            register s32 none asm("r0") = 0xFFFF;
            register u32 cur asm("r4") = part->id;

            if (cur != none)
            {
                register s32 id asm("r3") = *(vu16 *)&part->id;
                register u8 *base asm("r2") = gEntityFlags;
                register s32 word asm("r0") = id;
                s32 off;
                u32 *slot;

                /* a signed shift: hidden from gcc's "a u16 is never
                 * negative" folding, which would make it lsr */
                asm("" : "+r"(word));
                word >>= 5;
                off = word * 4;
                slot = (u32 *)(base + 0x108);
                slot = (u32 *)((u8 *)slot + off);
                word = id - (word << 5);
                *slot |= 1 << word;
            }
        }
    }
}

void sub_80149BC(struct act *self)
{
    if (self->part->animDone)
    {
        *((u8 *)gUnknown_030012D8 + 0xC) |= 0x80;
        sub_8015780(self, 0, 0x12, 0, 0);
        self->next31 = 0;
        self->flag2F = 1;
        self->next27 = 0;
        self->next32 = 0;
        self->flag30 = 1;
        self->next28 = 0;
        sub_8006D08(gUnknown_030012B8, self->part->slotNibble,
                    self->part->bank->records[self->part->tag].unk_14);
    }
}

void sub_8014A3C(struct act *self)
{
    u8 dir = sub_8000760(gUnknown_03001304);
    u32 in = gUnknown_030007E0;

    if (dir != 0)
        switch (dir)
    {
    case 3 ... 8:
        ActSetNext27(self, 0x20);
        ACT_VCALL1(self, m20, 0x25);
        ACT_VCALL2(self, m50, self->part, 0x20);
        break;
    }
    if (INPUT_PRESSED(in) & 1)
    {
        PlaySfx(gUnknown_030012BC, 0xD, 0x100);
        sub_8014B54(self);
        return;
    }
    if (INPUT_PRESSED(in) & 2)
    {
        sub_80153FC(self);
        sub_80122CC(self);
    }
    else
    {
        sub_80122CC(self);
    }
}

void sub_8014AEC(struct act *self)
{
    u32 in = gUnknown_030007E0;
    s32 fire = INPUT_PRESSED(in) & 1;

    if (fire)
    {
        PlaySfx(gUnknown_030012BC, 0xD, 0x100);
        sub_8014B54(self);
        return;
    }
    if (INPUT_PRESSED(in) & 2)
    {
        sub_80153FC(self);
        sub_80122CC(self);
        self->next31 = fire;
        self->flag2F = 1;
        self->next27 = fire;
    }
}

/* Starts the jump: clears part+0x101, lifts the part by 6 px (0x600 Q8),
 * sets animations 0x1A/0x1B, holds the part on its last frame and queues
 * action 4.
 *
 * The 0x600 is a reload, and the ROM loads it into r3. When reload
 * picks a spill register for that insn, r2 and r3 are both free, and it
 * takes the lower one (r2). Every later reload then rotates through the
 * spill-register set {1,2,6} instead of the ROM's {1,3,6}. `hold` is a
 * register variable in r2, set and used only by empty asms (no code).
 * It keeps r2 live across the add, so reload spills r3 there instead
 * (docs/matching/late-naked-retry-3.md). */
void sub_8014B54(struct act *self)
{
    struct act_part *part;
    s32 count;
    register s32 hold asm("r2");

    self->part->unk_101 = 0;
    asm("" : "=r"(hold)); /* r2 live from here: no code */
    self->part->y += 0x600;
    asm("" : : "r"(hold)); /* ...to here, so the 0x600 reload takes r3 */
    ACT_VCALL1(self, m20, 0x1A);
    ACT_VCALL2(self, m50, self->part, 0x1B);
    part = self->part;
    count = part->bank->records[part->tag].frameCount;
    part->frame = count - 1;
    ActSetNext(self, 4);
}

/* Crouch/aim handler: fire jumps (sub_8014B54), alt hands off to
 * sub_80153FC, an idle D-pad plays animations 0x28/0x22, a sideways one
 * queues action 0x20 on the +0x31/+0x2F/+0x27 trio, and at the end of
 * the animation it replays 0x26/0x21 from frame 5.
 *
 * The alt and idle trios are written out twice (gcc cross-jumps them into
 * the ROM's one block), so each gets the fire test's 1 from CSE; the
 * method calls use ACT_CALL (include/action_obj.h). */
void sub_8014BCC(struct act *self)
{
    void *pad = gUnknown_03001304;
    u32 in = gUnknown_030007E0;
    s32 v = INPUT_PRESSED(in) & 1;

    if (v)
    {
        PlaySfx(gUnknown_030012BC, 0xD, 0x100);
        ActQueue27(self, 0, 0);
        sub_8014B54(self);
        return;
    }
    if (INPUT_PRESSED(in) & 2)
    {
        sub_80153FC(self);
        sub_80122CC(self);
        self->next31 = 0;
        self->flag2F = 1;
        self->next27 = 0;
        return;
    }
    v = sub_8000760(pad);
    if (v == 0)
    {
        ACT_CALL1(self, m20, 0x28);
        ACT_CALL2(self, m50, self->part, 0x22);
        self->next31 = 0;
        self->flag2F = 1;
        self->next27 = 0;
        return;
    }
    {
        u8 cur = self->next27;

        if (cur == 0)
        {
            switch (v)
            {
            case 3 ... 8:
                ActQueue27(self, cur, 0x20);
            }
        }
        if (self->part->animDone)
        {
            struct act_part *part;
            s32 zero = 0;
            s32 frame;
            s32 count;

            ACT_CALL1(self, m20, 0x26);
            ACT_CALL2(self, m50, self->part, 0x21);
            self->frame = zero;
            part = self->part;
            frame = 5;
            count = part->bank->records[part->tag].frameCount;
            if (frame >= count)
                frame = count - 1;
            part->frame = frame;
            ActQueue27(self, zero, 0x20);
        }
    }
    sub_80122CC(self);
}

/* Walk handler: retags a finished part (0x21), handles fire/alt like
 * sub_8014BCC, steps a 4-frame idle timer that picks animation 0x22/0x23
 * from the part's frame, and while sub_80122CC reports a step moves the
 * part by the sub_80083B8 record's (or gStaticData_0816B300's) X offset,
 * mirrored by part+0x28 bit 4.
 *
 * The idle dispatch is written out per case: the ROM's one shared
 * sub_803AD84 call and trio are gcc's cross-jumping of the identical
 * tails, and the 1 the trios store comes from the fire test's constant in
 * r7, which CSE only carries into single-predecessor blocks. The method
 * calls use ACT_CALL (see include/action_obj.h). The record kind is
 * switched on 0..6 with separate case bodies, which is what makes gcc
 * emit the ROM's jump table. */
void sub_8014D18(struct act *self)
{
    u8 dir = sub_8000760(gUnknown_03001304);
    u32 in = gUnknown_030007E0;
    struct act_part *part = self->part;
    s32 fire;
    u16 alt;

    if (part->animDone)
        SetTag(part, 0x21);
    fire = INPUT_PRESSED(in) & 1;
    if (fire)
    {
        PlaySfx(gUnknown_030012BC, 0xD, 0x100);
        ActQueue27(self, 0, 0);
        sub_8014B54(self);
        return;
    }
    alt = INPUT_PRESSED(in) & 2;
    if (alt)
    {
        sub_80153FC(self);
        sub_80122CC(self);
        self->next31 = fire;
        self->flag2F = 1;
        self->next27 = fire;
        return;
    }
    if (dir == 0)
    {
        if (++self->frame > 3)
        {
            s32 f;

            self->frame = alt;
            f = self->part->frame;
            if (f == 0)
            {
                ACT_CALL1(self, m20, 0x28);
                ACT_CALL2(self, m50, self->part, 0x22);
                self->next31 = alt;
                self->flag2F = 1;
                self->next27 = alt;
            }
            else if (f <= 4)
            {
                ACT_CALL1(self, m20, 0x28);
                ACT_CALL2(self, m50, self->part, 0x23);
                self->next31 = alt;
                self->flag2F = 1;
                self->next27 = alt;
            }
            else if (f > 9)
            {
                ACT_CALL1(self, m20, 0x28);
                ACT_CALL2(self, m50, self->part, 0x22);
                self->next31 = alt;
                self->flag2F = 1;
                self->next27 = alt;
            }
            else
            {
                self->next31 = alt;
                self->flag2F = 1;
                self->next27 = alt;
            }
        }
    }
    else
    {
        self->frame = alt;
        ActQueue27(self, alt, 0x20);
    }
    if (sub_80122CC(self))
    {
        u8 *info = sub_80083B8(self->part);
        s32 x;
        s32 y;

        switch (**(u8 **)(info + 4) >> 4)
        {
        case 0:
            info += 0x24;
            break;
        case 1:
            info = gStaticData_0816B300;
            break;
        case 2:
            info = gStaticData_0816B300;
            break;
        case 3:
            info = gStaticData_0816B300;
            break;
        case 4:
            info = gStaticData_0816B300;
            break;
        case 5:
            info = gStaticData_0816B300;
            break;
        case 6:
            info += 0x14;
            break;
        default:
            info = gStaticData_0816B300;
            break;
        }
        x = self->part->x >> 8;
        y = self->part->y;
        if ((s8)(self->part->flags28 << 3) < 0)
            x += *(s16 *)info;
        else
            x -= *(s16 *)info;
        self->part->x = x << 8;
        self->part->y = y;
    }
}

void sub_8014EE0(struct act *self)
{
    u32 in = gUnknown_030007E0;
    s32 fire = INPUT_PRESSED(in) & 1;
    u16 alt;

    if (fire)
    {
        PlaySfx(gUnknown_030012BC, 0xD, 0x100);
        ActSetNext27(self, 0);
        sub_8014B54(self);
        return;
    }
    alt = INPUT_PRESSED(in) & 2;
    if (alt)
    {
        sub_80153FC(self);
        sub_80122CC(self);
        self->next31 = fire;
        self->flag2F = 1;
        self->next27 = fire;
        return;
    }
    if (self->part->animDone)
    {
        ACT_VCALL1(self, m20, 0x20);
        ACT_VCALL2(self, m50, self->part, 0x1F);
        self->frame = alt;
        self->frames = alt;
    }
}
asm(".align 2, 0");
