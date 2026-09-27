#include "core.h"
#include "action_obj.h"

/* GitHub issue #17, ROM 0x080138E8-0x08013C60 (details in
 * docs/matching/issue-17-0x08012fbc-actor.md, "Third pass"). Two more
 * gStaticData_0816BF20 action-table handlers for the player/action object
 * (include/action_obj.h). Built with old_agbcc. */

asm(".set _call_via_r2, sub_803AD80\n"
    ".set _call_via_r3, sub_803AD84\n");

extern u32 gUnknown_030007E0;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *gUnknown_03001304;
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern u8 sub_8000760(void *pad);
extern u8 sub_800AAEC(struct act_part *part, s32 action);
extern u8 sub_80231BC(void *self);
extern u8 sub_80231C4(void *self);
extern void sub_8015508(struct act *self);
extern void sub_8015398(struct act *self);
extern void sub_801434C(struct act *self);
extern void sub_8015460(struct act *self);

/* Queues action `next` on the +0x31/+0x2F/+0x27 trio. As inline
 * parameters, old_agbcc materializes the values before the three stores;
 * `ActQueue27` stores a literal 1 for +0x2F, `ActTrio27` a caller value. */
static inline void ActQueue27(struct act *self, s32 cur, s32 next)
{
    self->next31 = cur;
    self->flag2F = 1;
    self->next27 = next;
}

static inline void ActTrio27(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->next31 = cur;
    self->flag2F = flag;
    self->next27 = next;
}

/* Picks the part animation from its state: with tag 6, animation 9 on
 * frame 3 or 8 past it (or once finished); otherwise, once finished, 0x19
 * plus part animation 7 if sub_80231BC allows it, else 0x18. */
void sub_80138E8(struct act *self)
{
    struct act_part *part = self->part;

    if (part->tag == 6)
    {
        s32 frame = part->frame;

        if (frame == 3)
            ACT_VCALL2(self, m50, part, 9);
        else if (frame > 3 || part->animDone)
            ACT_VCALL2(self, m50, part, 8);
    }
    else if (part->animDone)
    {
        if (sub_80231BC(gUnknown_030012C0))
        {
            ACT_VCALL1(self, m20, 0x19);
            ACT_VCALL2(self, m50, self->part, 7);
        }
        else
        {
            ACT_VCALL1(self, m20, 0x18);
        }
    }
}

/* The "player input/action handling" reader of docs/rom_map.md. Out of
 * contact it queues idle (5); otherwise the confirm edge
 * (sub_800AAEC(part, 0xB)) hands off to sub_8015508 and the alt edge
 * (sub_800AAEC(part, 0x10)) to sub_8015398. Then it counts the animation
 * (holding the part on frame 3 until +0x18 reaches +0x1C) and, once the
 * part's animation is done, dispatches on contact, the 0x100/0x200 held
 * bits, the D-pad and sub_800AAEC(part, 2).
 *
 * The +0x2F store of the alt path and the 0x1B path reuses the 1 already
 * in a register (the `held & 1` test's, then +0x29's); the two final
 * VCALL2+trio tails are written out twice, as the ROM cross-jumps them
 * from the method call on. */
void sub_8013994(struct act *self)
{
    u32 in = gUnknown_030007E0;

    {
        struct act_part *part = self->part;

        if (part->contact == 0)
        {
            ActSetNext(self, 5);
        }
        else if (INPUT_HELD(in) & 1)
        {
            if (sub_800AAEC(part, 0xB) == 1)
            {
                PlaySfx(gUnknown_030012BC, 0xC, 0x100);
                ActAndFlags0D(self->part, -2);
                ActAndFlags0D(self->part, -3);
                sub_8015508(self);
                return;
            }
        }
        else if (INPUT_PRESSED(in) & 2)
        {
            if (sub_800AAEC(part, 0x10) == 1)
            {
                sub_8015398(self);
                ActTrio27(self, 0, 1, 1);
                return;
            }
        }
    }

    if (++self->frame < self->frames)
    {
        struct act_part *part = self->part;
        s32 frame;
        s32 count;

        part->unk_34 = 0;
        frame = 3;
        count = part->bank->records[part->tag].frameCount;
        if (frame >= count)
            frame = count - 1;
        part->frame = frame;
        return;
    }
    {
        struct act_part *part = self->part;
        u8 contact;

        if (!part->animDone)
            return;
        contact = part->contact;
        if (contact == 0)
        {
            ACT_VCALL1(self, m20, 0x1A);
            ACT_VCALL2(self, m50, self->part, 0x1B);
            ActSetNext(self, 4);
            return;
        }
    }
    {
        u16 held = INPUT_HELD(in) & 0x100;

        if (held)
        {
            s32 zero = 0;

            ACT_VCALL1(self, m20, 0x14);
            ACT_VCALL2(self, m50, self->part, 0);
            self->frames = zero;
            ActQueue27(self, zero, 3);
            sub_801434C(self);
            return;
        }
        {
            u8 dir = sub_8000760(gUnknown_03001304);

            if (dir != 0 && sub_800AAEC(self->part, 2))
            {
                switch (dir)
                {
                case 3 ... 4:
                    if ((INPUT_HELD(in) & 0x200) && sub_80231C4(gUnknown_030012C0))
                    {
                        self->unk_29 = 1;
                        ACT_VCALL1(self, m20, 4);
                        ACT_VCALL2(self, m50, self->part, 0x18);
                        ActTrio27(self, held, 1, 0x1B);
                        return;
                    }
                    sub_8015460(self);
                    return;
                }
                ACT_VCALL1(self, m20, 0x12);
                ACT_VCALL2(self, m50, self->part, 2);
                ActQueue27(self, 0, 0);
                return;
            }
            else
            {
                u8 hit = sub_800AAEC(self->part, 2);

                if (hit == 1)
                {
                    ACT_VCALL1(self, m20, 0x12);
                    ACT_VCALL2(self, m50, self->part, 2);
                    ActTrio27(self, 0, hit, 0);
                    return;
                }
                ACT_VCALL1(self, m20, 0x11);
                ACT_VCALL2(self, m50, self->part, 4);
                ActQueue27(self, 0, 0);
            }
        }
    }
}
