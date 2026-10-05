#include "core.h"
#include "action_obj.h"

/* GitHub issue #17, ROM 0x080134B8-0x080138E8 (details in
 * docs/matching/issue-17-0x08012fbc-actor.md, "Third pass"). Built with
 * old_agbcc. */

/* The spark object sub_8025BAC spawns, as far as it is used. */
struct spark
{
    u8 unk_00[0xC];
    u8 unk_0C_0:2;
    u8 unk_0C_2:1;
    u8 unk_0C_3:5;
    u8 unk_0D[0x13];
    struct act_anim_bank *bank; // 0x20
    u8 unk_24[4];
    u8 unk_28_0:2;         // 0x28
    u8 unk_28_2:6;
    u8 unk_29[4];
    u8 tag;                // 0x2D
    u8 unk_2E[2];
    s32 frame;             // 0x30
};

extern u32 gKeys;
extern void *gAudioContext;
extern struct act_part *gPlayer;
extern void *gEntitySpawner;
extern void *gUnknown_03001304;
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern u8 GetDpadDirection(void *pad);
extern void UpdatePlayerFacing(struct act *self);
extern void sub_801283C(struct act *self);
extern void sub_800B334(struct act_part *part);
extern void sub_8014F8C(struct act *self);
extern struct spark *sub_8025BAC(void *pool, s32 a, s32 b, s32 x, s32 y, s32 mirror);

/* Byte masks with the mask as an `s32` parameter: the AND stays in SImode
 * (a plain `*p & -0x11` is narrowed to 0xEF), so the -0x11 the ROM derives
 * from the 1 already in r7 (`subs r7, #0x12`) can be shared by both
 * sparks. */
static inline void AndByte(u8 *p, s32 clear)
{
    *p &= clear;
}

static inline void OrMaskByte(u8 *p, s32 clear, s32 set)
{
    *p = (*p & clear) | set;
}

/* The coordinates are inline parameters, so they are computed before the
 * pool pointer is loaded, as in the ROM. */
static inline struct spark *SpawnSpark(s32 x, s32 y, s32 mirror)
{
    return sub_8025BAC(gEntitySpawner, 0x29, 1, x, y, mirror);
}

/* The "next action" trio stores (include/action_obj.h's ActSetNext for
 * the other trio): inline parameters are materialized before the stores. */
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

static inline void ActSetNextP(struct act *self, u8 *slot, s32 next)
{
    self->next32 = 0;
    self->flag30 = 1;
    *slot = next;
}

static inline void ActSetContact(struct act_part *p, s32 v)
{
    p->contact = v;
}

/* The table's shared "bonus/score popup" handler (docs/rom_map.md, reused
 * by 6 of the 42 slots). The alt edge restarts the charge animation (as in
 * sub_8013228); out of contact it ticks the +0x25 countdown and otherwise
 * queues action 4 from state 0x1A. On contact bit 2 it lands the part (frame
 * 2, sub_800B334); on contact bit 3 in states 0x18/0x19 it spawns two
 * sparks at the player (+-0x14 px, the first mirrored) and plays 0x16/0x11;
 * state 0xE queues from the shoulder bits; the rest plays 0x17/0x16.
 *
 * The first spark's bit-2 clear is written twice: the second store folds
 * away, but its extra use of the -5 mask is what gives that mask sb (and
 * the -0x11 r8) as in the ROM. */
void sub_80134B8(struct act *self)
{
    void *pad = gUnknown_03001304;
    u32 in = gKeys;
    u8 contact = self->part->contact;
    u8 dir = GetDpadDirection(pad);
    s32 state = self->state;

    if (state != 0xE)
    {
        u8 busy = self->unk_26;

        if (busy == 0 && (INPUT_PRESSED(in) & 2) && (u32)(state - 0x18) > 1)
        {
            s32 frames;

            PlaySfx(gAudioContext, 0xA, 0x100);
            frames = 0x18;
            ACT_VCALL1(self, m20, 0xE);
            ACT_VCALL2(self, m50, self->part, 0x10);
            self->frame = busy;
            self->frames = frames;
            self->unk_21 = busy;
            self->charge = busy;
            self->unk_22 = busy;
            self->unk_23 = busy;
            self->unk_24[0] = busy;
            ((u8 *)gPlayer)[0x92] = busy;
        }
    }
    if (contact == 0)
    {
        u8 *timer;

        UpdatePlayerFacing(self);
        timer = &self->unk_24[1];
        if (*timer != 0)
        {
            (*timer)--;
            if (dir == 0)
                *timer = contact;
            return;
        }
        sub_801283C(self);
        if (self->state == 0x1A)
        {
            u8 *slot = &self->next28;

            if (*slot == 0)
                ActSetNextP(self, slot, 4);
        }
        return;
    }
    {
        u8 bit4 = contact & 4;

        if (bit4)
        {
            if (self->state != 0x1A)
            {
                ActOrFlags0D(self->part, 1);
                self->unk_34 = 0;
                if (self->state != 0xE)
                {
                    struct act_part *part;
                    s32 frame;
                    s32 count;

                    ACT_VCALL1(self, m20, 0x1A);
                    ACT_VCALL2(self, m50, self->part, 0x15);
                    part = self->part;
                    frame = 2;
                    count = part->bank->records[part->tag].frameCount;
                    if (frame >= count)
                        frame = count - 1;
                    part->frame = frame;
                }
                sub_800B334(self->part);
            }
            UpdatePlayerFacing(self);
            sub_801283C(self);
            ActSetContact(self->part, 0);
            return;
        }
        if (contact == 1 || contact == 2)
        {
            UpdatePlayerFacing(self);
            sub_801283C(self);
            self->part->contact = bit4;
            return;
        }
        if ((contact & 8) && self->part->unk_64 >= 0)
        {
            s32 st;

            ACT_PART_FLAGS0D(self->part) |= 1;
            self->unk_34 = bit4;
            st = self->state;
            if ((u32)(st - 0x18) <= 1)
            {
                struct spark *obj;
                s32 x;
                s32 y;
                s32 frame;
                s32 count;

                x = gPlayer->x;
                x >>= 8;
                x += 0x14;
                y = gPlayer->y;
                y >>= 8;
                y += 0xC;
                obj = SpawnSpark(x, y, 1);
                obj->unk_28_0 = 1;
                obj->unk_0C_2 = 0;
                obj->unk_0C_2 = 0;
                OrMaskByte((u8 *)obj + 0x28, -0x11, 0x10);
                frame = 3;
                count = obj->bank->records[obj->tag].frameCount;
                if (frame >= count)
                    frame = count - 1;
                obj->frame = frame;

                x = gPlayer->x;
                x >>= 8;
                x -= 0x14;
                y = gPlayer->y;
                y >>= 8;
                y += 0xC;
                obj = SpawnSpark(x, y, bit4);
                obj->unk_28_0 = 1;
                obj->unk_0C_2 = 0;
                AndByte((u8 *)obj + 0x28, -0x11);
                frame = 3;
                count = obj->bank->records[obj->tag].frameCount;
                if (frame >= count)
                    frame = count - 1;
                obj->frame = frame;

                if (self->state == 0x19)
                    sub_8014F8C(self);
                if (self->state != 0x1D)
                {
                    PlaySfx(gAudioContext, 0x19, 0x100);
                    ACT_VCALL1(self, m20, 0x16);
                    ACT_VCALL2(self, m50, self->part, 0x11);
                    self->next32 = bit4;
                    self->flag30 = 1;
                    self->next28 = bit4;
                }
                return;
            }
            if (st == 0xE)
            {
                u16 held = INPUT_HELD(in) & 0x30;

                if (held)
                    ActTrio27(self, bit4, 1, 1);
                else
                {
                    self->next31 = held;
                    self->flag2F = 1;
                    self->next27 = held;
                }
                ActSetNext(self, 0);
                if (self->unk_22)
                    ACT_VCALL1(self, m20, 0xF);
                else
                    ACT_VCALL1(self, m20, 0xD);
                return;
            }
            if (((u8 *)gPlayer)[0x100])
            {
                ACT_VCALL1(self, m20, 0x17);
                ACT_VCALL2(self, m50, self->part, 0x16);
            }
            else
            {
                ACT_VCALL1(self, m20, 0x17);
                ACT_VCALL2(self, m50, self->part, 0x16);
            }
            ActQueue27(self, 0, 0);
            ActSetNext(self, 0);
            return;
        }
    }
    ActQueue27(self, 0, 0);
}
asm(".align 2, 0");
