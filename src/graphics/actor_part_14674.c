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
extern void *gUnknown_030012B4;
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

static inline void SetTag(struct act_part *part, s32 tag)
{
    part->tag = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
}

/* On contact (part+0x68 bit 3), picks the landing action from the part's
 * tag (0xD: the +0x29-gated landing, 0x18: re-arm +0x29) or state 0xE;
 * otherwise handles the fire/alt/shoulder inputs and, with the D-pad
 * idle, clears the +0x31/+0x2F/+0x27 trio.
 *
 * NAKED: the C below has the ROM's blocks, but the ROM keeps the
 * constant 1 and the contact bit in callee-saved registers across the
 * whole function (r6/r5) and shares the +0x31/+0x2F/+0x27 stores
 * between paths differently - see docs/matching/issue-17-0x08012fbc-actor.md,
 * "Second pass". */
#if NON_MATCHING
void sub_8014674(struct act *self)
{
    struct act_part *part = self->part;
    u8 hit = part->contact & 8;

    if (hit)
    {
        u8 tag;

        ActOrFlags0D(part, 1);
        self->unk_34 = 0;
        part = self->part;
        tag = part->tag;
        if (tag == 0xD || tag == 0x18)
        {
            if (tag == 0xD)
            {
                if (self->unk_29)
                {
                    self->frame = 0;
                    ACT_VCALL2(self, m50, part, 0x18);
                    ACT_VCALL1(self, m20, 4);
                    self->next31 = 0;
                    self->flag2F = 1;
                    self->next27 = 0x1B;
                }
                else
                {
                    self->frame = 0;
                    ACT_VCALL1(self, m20, 3);
                    if (self->next27 != 1)
                    {
                        self->next31 = 0;
                        self->flag2F = 1;
                        self->next27 = 1;
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
                self->next31 = 0;
                self->flag2F = 1;
                self->next27 = 0x1B;
            }
        }
        else if (self->state == 0xE)
        {
            if (gUnknown_030007E0 & 0x30)
            {
                self->next31 = 0;
                self->flag2F = 1;
                self->next27 = 1;
            }
            else
            {
                self->next31 = 0;
                self->flag2F = 1;
                self->next27 = 0;
            }
            ActSetNext(self, 0);
            ACT_VCALL1(self, m20, 0xD);
        }
        else
        {
            ACT_VCALL1(self, m20, 0);
            self->next32 = 0;
            self->flag30 = 1;
            self->next28 = 0;
            self->next31 = 0;
            self->flag2F = 1;
            self->next27 = 0;
        }
        return;
    }
    {
        u32 in = gUnknown_030007E0;
        s32 fire = INPUT_PRESSED(in) & 1;

        if (fire)
        {
            ACT_VCALL1(self, m20, 5);
            ACT_VCALL2(self, m50, self->part, 0x13);
            self->frame = hit;
            self->next32 = hit;
            self->flag30 = 1;
            self->next28 = 7;
        }
        else
        {
            u16 alt = INPUT_PRESSED(in) & 2;

            if (alt)
            {
                ActOrFlags0D(part, 1);
                self->unk_34 = fire;
                if (gUnknown_030007E0 & 0x30)
                {
                    self->next31 = fire;
                    self->flag2F = 1;
                    self->next27 = 1;
                }
                else
                {
                    self->next31 = 0;
                    self->flag2F = 1;
                    self->next27 = 0;
                }
                if ((u32)(self->state - 0xD) <= 1)
                    ACT_VCALL1(self, m20, 0xD);
                else
                    sub_8015398(self);
            }
            else if (INPUT_HELD(in) & 0x100)
            {
                ActOrFlags0D(part, 1);
                self->unk_34 = alt;
                ACT_VCALL1(self, m20, 0x10);
                ACT_VCALL2(self, m50, self->part, 3);
                self->frames = alt;
                self->next31 = alt;
                self->flag2F = 1;
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
#else
NAKED void sub_8014674(struct act *self)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tsub sp, #4\n"
        "\tadds r4, r0, #0\n"
        "\tldr r2, [r4, #0x10]\n"
        "\tadds r1, r2, #0\n"
        "\tadds r1, #0x68\n"
        "\tmovs r0, #8\n"
        "\tldrb r1, [r1]\n"
        "\tands r0, r1\n"
        "\tlsls r0, r0, #0x18\n"
        "\tlsrs r5, r0, #0x18\n"
        "\tcmp r5, #0\n"
        "\tbne _08014690\n"
        "\tb _0801480A\n"
        "_08014690:\n"
        "\tmovs r0, #1\n"
        "\tldrb r1, [r2, #0xd]\n"
        "\torrs r0, r1\n"
        "\tstrb r0, [r2, #0xd]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x34\n"
        "\tmovs r5, #0\n"
        "\tstrb r5, [r0]\n"
        "\tldr r2, [r4, #0x10]\n"
        "\tadds r0, r2, #0\n"
        "\tadds r0, #0x2d\n"
        "\tldrb r0, [r0]\n"
        "\tcmp r0, #0xd\n"
        "\tbeq _080146B4\n"
        "\tcmp r0, #0x18\n"
        "\tbne _08014778\n"
        "\tcmp r0, #0xd\n"
        "\tbne _0801473E\n"
        "_080146B4:\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x29\n"
        "\tldrb r0, [r0]\n"
        "\tcmp r0, #0\n"
        "\tbeq _080146FC\n"
        "\tstr r5, [r4, #0x18]\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tadds r1, #0x50\n"
        "\tmovs r3, #0\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r3, [r1, #4]\n"
        "\tadds r1, r2, #0\n"
        "\tmovs r2, #0x18\n"
        "\tbl sub_803AD84\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #4\n"
        "\tbl sub_803AD80\n"
        "\tmovs r1, #0x1b\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r5, [r0]\n"
        "\tadds r2, r4, #0\n"
        "\tadds r2, #0x2f\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r2]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x27\n"
        "\tstrb r1, [r0]\n"
        "\tb _08014726\n"
        "_080146FC:\n"
        "\tstr r5, [r4, #0x18]\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r3, #0x20\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #3\n"
        "\tbl sub_803AD80\n"
        "\tadds r2, r4, #0\n"
        "\tadds r2, #0x27\n"
        "\tldrb r0, [r2]\n"
        "\tcmp r0, #1\n"
        "\tbeq _08014726\n"
        "\tmovs r0, #1\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x31\n"
        "\tstrb r5, [r1]\n"
        "\tsubs r1, #2\n"
        "\tstrb r0, [r1]\n"
        "\tstrb r0, [r2]\n"
        "_08014726:\n"
        "\tmovs r2, #0\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x32\n"
        "\tstrb r2, [r0]\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x30\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r1]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x28\n"
        "\tstrb r2, [r0]\n"
        "\tb _08014934\n"
        "_0801473E:\n"
        "\tcmp r0, #0x18\n"
        "\tbeq _08014744\n"
        "\tb _08014934\n"
        "_08014744:\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #4\n"
        "\tbl sub_803AD80\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x32\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tmovs r1, #1\n"
        "\tstrb r1, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r5, [r0]\n"
        "\tadds r0, #1\n"
        "\tstrb r1, [r0]\n"
        "\tmovs r2, #0x1b\n"
        "\tadds r0, #8\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r1, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r2, [r0]\n"
        "\tb _08014934\n"
        "_08014778:\n"
        "\tldr r0, [r4, #8]\n"
        "\tcmp r0, #0xe\n"
        "\tbne _080147DC\n"
        "\tldr r0, _0801479C\n"
        "\tldr r0, [r0]\n"
        "\tmovs r1, #0x30\n"
        "\tands r0, r1\n"
        "\tcmp r0, #0\n"
        "\tbeq _080147A0\n"
        "\tmovs r0, #1\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x31\n"
        "\tstrb r5, [r1]\n"
        "\tsubs r1, #2\n"
        "\tstrb r0, [r1]\n"
        "\tsubs r1, #8\n"
        "\tstrb r0, [r1]\n"
        "\tb _080147B4\n"
        "\t.align 2, 0\n"
        "_0801479C: .4byte gUnknown_030007E0\n"
        "_080147A0:\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r5, [r0]\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x2f\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r1]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x27\n"
        "\tstrb r5, [r0]\n"
        "_080147B4:\n"
        "\tmovs r2, #0\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x32\n"
        "\tstrb r2, [r0]\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x30\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r1]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x28\n"
        "\tstrb r2, [r0]\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r3, #0x20\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0xd\n"
        "\tbl sub_803AD80\n"
        "\tb _08014934\n"
        "_080147DC:\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0\n"
        "\tbl sub_803AD80\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x32\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tmovs r1, #1\n"
        "\tstrb r1, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r5, [r0]\n"
        "\tadds r0, #9\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r1, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r5, [r0]\n"
        "\tb _08014934\n"
        "_0801480A:\n"
        "\tldr r7, _08014858\n"
        "\tldr r0, [r7]\n"
        "\tstr r0, [sp]\n"
        "\tmov r0, sp\n"
        "\tldrh r1, [r0, #2]\n"
        "\tmovs r6, #1\n"
        "\tmovs r3, #1\n"
        "\tands r3, r1\n"
        "\tcmp r3, #0\n"
        "\tbeq _0801485C\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r3, #0x20\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #5\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r1, #0\n"
        "\tldrsh r0, [r2, r1]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #0x13\n"
        "\tbl sub_803AD84\n"
        "\tstr r5, [r4, #0x18]\n"
        "\tmovs r1, #7\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x32\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r6, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r1, [r0]\n"
        "\tb _08014910\n"
        "\t.align 2, 0\n"
        "_08014858: .4byte gUnknown_030007E0\n"
        "_0801485C:\n"
        "\tmovs r0, #2\n"
        "\tands r0, r1\n"
        "\tlsls r0, r0, #0x10\n"
        "\tlsrs r5, r0, #0x10\n"
        "\tcmp r5, #0\n"
        "\tbeq _080148C0\n"
        "\tmovs r0, #1\n"
        "\tldrb r1, [r2, #0xd]\n"
        "\torrs r0, r1\n"
        "\tstrb r0, [r2, #0xd]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x34\n"
        "\tstrb r3, [r0]\n"
        "\tldr r1, [r7]\n"
        "\tmovs r0, #0x30\n"
        "\tands r1, r0\n"
        "\tcmp r1, #0\n"
        "\tbeq _08014890\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r3, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r6, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r6, [r0]\n"
        "\tb _0801489E\n"
        "_08014890:\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r1, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r6, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r1, [r0]\n"
        "_0801489E:\n"
        "\tldr r0, [r4, #8]\n"
        "\tsubs r0, #0xd\n"
        "\tcmp r0, #1\n"
        "\tbls _080148AE\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_8015398\n"
        "\tb _08014910\n"
        "_080148AE:\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0xd\n"
        "\tbl sub_803AD80\n"
        "\tb _08014910\n"
        "_080148C0:\n"
        "\tmov r1, sp\n"
        "\tmovs r0, #0x80\n"
        "\tlsls r0, r0, #1\n"
        "\tldrh r1, [r1]\n"
        "\tands r0, r1\n"
        "\tcmp r0, #0\n"
        "\tbeq _08014910\n"
        "\tmovs r0, #1\n"
        "\tldrb r3, [r2, #0xd]\n"
        "\torrs r0, r3\n"
        "\tstrb r0, [r2, #0xd]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x34\n"
        "\tstrb r5, [r0]\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x10\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r3, #0\n"
        "\tldrsh r0, [r2, r3]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #3\n"
        "\tbl sub_803AD84\n"
        "\tstr r5, [r4, #0x1c]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r6, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r5, [r0]\n"
        "_08014910:\n"
        "\tldr r0, _0801493C\n"
        "\tldr r0, [r0]\n"
        "\tbl sub_8000760\n"
        "\tlsls r0, r0, #0x18\n"
        "\tlsrs r1, r0, #0x18\n"
        "\tcmp r1, #0\n"
        "\tbne _08014934\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r1, [r0]\n"
        "\tadds r2, r4, #0\n"
        "\tadds r2, #0x2f\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r2]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x27\n"
        "\tstrb r1, [r0]\n"
        "_08014934:\n"
        "\tadd sp, #4\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "_0801493C: .4byte gUnknown_03001304\n"
        ".syntax divided\n");
}
#endif

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
                register u8 *base asm("r2") = gUnknown_030012B4;
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
 * NAKED: the C below is off by one register - the 0x600 constant is a
 * reload the ROM puts in r3 where gcc picks r2 (its reload-register
 * rotation is one step apart), and pinning it shifts every later reload
 * instead - see docs/matching/issue-17-0x08012fbc-actor.md, "Second pass". */
#if NON_MATCHING
void sub_8014B54(struct act *self)
{
    struct act_part *part;
    s32 count;

    self->part->unk_101 = 0;
    self->part->y += 0x600;
    ACT_VCALL1(self, m20, 0x1A);
    ACT_VCALL2(self, m50, self->part, 0x1B);
    part = self->part;
    count = part->bank->records[part->tag].frameCount;
    part->frame = count - 1;
    ActSetNext(self, 4);
}
#else
NAKED void sub_8014B54(struct act *self)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, lr}\n"
        "\tadds r4, r0, #0\n"
        "\tldr r0, [r4, #0x10]\n"
        "\tldr r1, _08014BC8\n"
        "\tadds r0, r0, r1\n"
        "\tmovs r5, #0\n"
        "\tstrb r5, [r0]\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r0, [r1, #4]\n"
        "\tmovs r3, #0xc0\n"
        "\tlsls r3, r3, #3\n"
        "\tadds r0, r0, r3\n"
        "\tstr r0, [r1, #4]\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r6, #0x20\n"
        "\tldrsh r0, [r1, r6]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x1a\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r1, #0\n"
        "\tldrsh r0, [r2, r1]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #0x1b\n"
        "\tbl sub_803AD84\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r0, [r1, #0x20]\n"
        "\tadds r3, r1, #0\n"
        "\tadds r3, #0x2d\n"
        "\tldr r2, [r0]\n"
        "\tldrb r6, [r3]\n"
        "\tlsls r0, r6, #3\n"
        "\tsubs r0, r0, r6\n"
        "\tlsls r0, r0, #2\n"
        "\tadds r0, r0, r2\n"
        "\tldrb r0, [r0, #0x16]\n"
        "\tsubs r0, #1\n"
        "\tstr r0, [r1, #0x30]\n"
        "\tmovs r2, #4\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x32\n"
        "\tstrb r5, [r0]\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x30\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r1]\n"
        "\tadds r4, #0x28\n"
        "\tstrb r2, [r4]\n"
        "\tpop {r4, r5, r6}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "_08014BC8: .4byte 0x00000101\n"
        ".syntax divided\n");
}
#endif

/* Crouch/aim handler: fire jumps (sub_8014B54), alt hands off to
 * sub_80153FC, an idle D-pad plays animations 0x28/0x22, a sideways one
 * queues action 0x20 on the +0x31/+0x2F/+0x27 trio, and at the end of
 * the animation it replays 0x26/0x21 from frame 5.
 *
 * NAKED: the C below differs in register roles only - the ROM keeps the
 * constant 1 in r6 and the &self->next27 pointer in r8 across the calls,
 * where gcc gives the pointer r6 and the constant r7 - see
 * docs/matching/issue-17-0x08012fbc-actor.md, "Second pass". */
#if NON_MATCHING
void sub_8014BCC(struct act *self)
{
    void *pad = gUnknown_03001304;
    u32 in = gUnknown_030007E0;
    s32 v = INPUT_PRESSED(in) & 1;

    if (v)
    {
        PlaySfx(gUnknown_030012BC, 0xD, 0x100);
        ActSetNext27(self, 0);
        sub_8014B54(self);
        return;
    }
    if (INPUT_PRESSED(in) & 2)
    {
        sub_80153FC(self);
        sub_80122CC(self);
        goto idle;
    }
    v = sub_8000760(pad);
    if (v == 0)
    {
        ACT_VCALL1(self, m20, 0x28);
        ACT_VCALL2(self, m50, self->part, 0x22);
    idle:
        self->next31 = v;
        self->flag2F = 1;
        self->next27 = v;
        return;
    }
    {
        u8 *next = &self->next27;
        u8 cur;

        asm("" : "+r"(next));
        cur = *next;

        if (cur == 0)
        {
            switch (v)
            {
            case 3 ... 8:
                self->next31 = cur;
                self->flag2F = 1;
                *next = 0x20;
            }
        }
        if (self->part->animDone)
        {
            struct act_part *part;
            s32 frame;
            s32 count;

            ACT_VCALL1(self, m20, 0x26);
            ACT_VCALL2(self, m50, self->part, 0x21);
            self->frame = 0;
            part = self->part;
            frame = 5;
            count = part->bank->records[part->tag].frameCount;
            if (frame >= count)
                frame = count - 1;
            part->frame = frame;
            self->next31 = 0;
            self->flag2F = 1;
            *next = 0x20;
        }
    }
    sub_80122CC(self);
}
#else
NAKED void sub_8014BCC(struct act *self)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, r8\n"
        "\tpush {r7}\n"
        "\tsub sp, #4\n"
        "\tadds r4, r0, #0\n"
        "\tldr r0, _08014C14\n"
        "\tldr r2, [r0]\n"
        "\tldr r0, _08014C18\n"
        "\tldr r0, [r0]\n"
        "\tstr r0, [sp]\n"
        "\tmov r0, sp\n"
        "\tldrh r1, [r0, #2]\n"
        "\tmovs r6, #1\n"
        "\tmovs r5, #1\n"
        "\tands r5, r1\n"
        "\tcmp r5, #0\n"
        "\tbeq _08014C20\n"
        "\tldr r0, _08014C1C\n"
        "\tldr r0, [r0]\n"
        "\tmovs r2, #0x80\n"
        "\tlsls r2, r2, #1\n"
        "\tmovs r1, #0xd\n"
        "\tbl PlaySfx\n"
        "\tmovs r0, #0\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x31\n"
        "\tstrb r0, [r1]\n"
        "\tsubs r1, #2\n"
        "\tstrb r6, [r1]\n"
        "\tsubs r1, #8\n"
        "\tstrb r0, [r1]\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_8014B54\n"
        "\tb _08014D0A\n"
        "\t.align 2, 0\n"
        "_08014C14: .4byte gUnknown_03001304\n"
        "_08014C18: .4byte gUnknown_030007E0\n"
        "_08014C1C: .4byte gUnknown_030012BC\n"
        "_08014C20:\n"
        "\tmovs r0, #2\n"
        "\tands r0, r1\n"
        "\tcmp r0, #0\n"
        "\tbeq _08014C36\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_80153FC\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_80122CC\n"
        "\tb _08014C68\n"
        "_08014C36:\n"
        "\tadds r0, r2, #0\n"
        "\tbl sub_8000760\n"
        "\tlsls r0, r0, #0x18\n"
        "\tlsrs r5, r0, #0x18\n"
        "\tcmp r5, #0\n"
        "\tbne _08014C78\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x28\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r7, #0\n"
        "\tldrsh r0, [r2, r7]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #0x22\n"
        "\tbl sub_803AD84\n"
        "_08014C68:\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r6, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r5, [r0]\n"
        "\tb _08014D0A\n"
        "_08014C78:\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x27\n"
        "\tldrb r2, [r0]\n"
        "\tmov r8, r0\n"
        "\tcmp r2, #0\n"
        "\tbne _08014C9C\n"
        "\tcmp r5, #8\n"
        "\tbgt _08014C9C\n"
        "\tcmp r5, #3\n"
        "\tblt _08014C9C\n"
        "\tmovs r0, #0x20\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x31\n"
        "\tstrb r2, [r1]\n"
        "\tsubs r1, #2\n"
        "\tstrb r6, [r1]\n"
        "\tmov r1, r8\n"
        "\tstrb r0, [r1]\n"
        "_08014C9C:\n"
        "\tldr r0, [r4, #0x10]\n"
        "\tadds r0, #0x38\n"
        "\tldrb r0, [r0]\n"
        "\tcmp r0, #0\n"
        "\tbeq _08014D04\n"
        "\tmovs r6, #0\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x26\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r7, #0\n"
        "\tldrsh r0, [r2, r7]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #0x21\n"
        "\tbl sub_803AD84\n"
        "\tstr r6, [r4, #0x18]\n"
        "\tldr r3, [r4, #0x10]\n"
        "\tmovs r5, #5\n"
        "\tldr r0, [r3, #0x20]\n"
        "\tadds r2, r3, #0\n"
        "\tadds r2, #0x2d\n"
        "\tldr r1, [r0]\n"
        "\tldrb r7, [r2]\n"
        "\tlsls r0, r7, #3\n"
        "\tadds r2, r7, #0\n"
        "\tsubs r0, r0, r2\n"
        "\tlsls r0, r0, #2\n"
        "\tadds r0, r0, r1\n"
        "\tldrb r0, [r0, #0x16]\n"
        "\tcmp r5, r0\n"
        "\tblt _08014CEE\n"
        "\tsubs r5, r0, #1\n"
        "_08014CEE:\n"
        "\tstr r5, [r3, #0x30]\n"
        "\tmovs r1, #0x20\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r6, [r0]\n"
        "\tadds r2, r4, #0\n"
        "\tadds r2, #0x2f\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r2]\n"
        "\tmov r0, r8\n"
        "\tstrb r1, [r0]\n"
        "_08014D04:\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_80122CC\n"
        "_08014D0A:\n"
        "\tadd sp, #4\n"
        "\tpop {r3}\n"
        "\tmov r8, r3\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        ".syntax divided\n");
}
#endif

/* Walk handler: retags a finished part (0x21), handles fire/alt like
 * sub_8014BCC, steps a 4-frame idle timer that picks animation 0x22/0x23
 * from the part's frame, and while sub_80122CC reports a step moves the
 * part by the sub_80083B8 record's (or gStaticData_0816B300's) X offset,
 * mirrored by part+0x28 bit 4.
 *
 * NAKED: the C below differs in register roles (the fire/alt results
 * swap r5/r6, and the ROM keeps the constant 1 in r7) and in the idle
 * timer's animation dispatch, where the ROM shares one sub_803AD84 call
 * between the 0x22 and 0x23 paths - see
 * docs/matching/issue-17-0x08012fbc-actor.md, "Second pass". */
#if NON_MATCHING
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
    if (dir == 0)
    {
        if (++self->frame > 3)
        {
            s32 f;

            self->frame = alt;
            f = self->part->frame;
            if (f != 0)
            {
                if (f <= 4)
                {
                    ACT_VCALL1(self, m20, 0x28);
                    ACT_VCALL2(self, m50, self->part, 0x23);
                    goto queued;
                }
                if (f <= 9)
                    goto queued;
            }
            ACT_VCALL1(self, m20, 0x28);
            ACT_VCALL2(self, m50, self->part, 0x22);
        queued:
            self->next31 = alt;
            self->flag2F = 1;
            self->next27 = alt;
        }
    }
    else
    {
        self->frame = alt;
        self->next31 = alt;
        self->flag2F = 1;
        self->next27 = 0x20;
    }
    if (sub_80122CC(self))
    {
        u8 *info = sub_80083B8(self->part);
        s16 *off;
        s32 x;
        s32 y;

        switch (**(u8 **)(info + 4) >> 4)
        {
        case 0:
            off = (s16 *)(info + 0x24);
            break;
        case 6:
            off = (s16 *)(info + 0x14);
            break;
        default:
            off = (s16 *)gStaticData_0816B300;
            break;
        }
        part = self->part;
        x = part->x >> 8;
        y = part->y;
        if ((s8)(part->flags28 << 3) < 0)
            x += *off;
        else
            x -= *off;
        part->x = x << 8;
        part->y = y;
    }
}
#else
NAKED void sub_8014D18(struct act *self)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, r8\n"
        "\tpush {r7}\n"
        "\tsub sp, #4\n"
        "\tadds r4, r0, #0\n"
        "\tldr r0, _08014D94\n"
        "\tldr r0, [r0]\n"
        "\tbl sub_8000760\n"
        "\tlsls r0, r0, #0x18\n"
        "\tlsrs r0, r0, #0x18\n"
        "\tmov r8, r0\n"
        "\tldr r0, _08014D98\n"
        "\tldr r0, [r0]\n"
        "\tstr r0, [sp]\n"
        "\tldr r5, [r4, #0x10]\n"
        "\tadds r0, r5, #0\n"
        "\tadds r0, #0x38\n"
        "\tldrb r0, [r0]\n"
        "\tcmp r0, #0\n"
        "\tbeq _08014D5E\n"
        "\tmovs r0, #0x21\n"
        "\tadds r1, r5, #0\n"
        "\tadds r1, #0x2d\n"
        "\tstrb r0, [r1]\n"
        "\tadds r0, r5, #0\n"
        "\tbl sub_80087C0\n"
        "\tadds r0, r5, #0\n"
        "\tbl sub_80087B4\n"
        "\tadds r0, r5, #0\n"
        "\tmovs r1, #0\n"
        "\tbl sub_800872C\n"
        "_08014D5E:\n"
        "\tmov r0, sp\n"
        "\tldrh r1, [r0, #2]\n"
        "\tmovs r7, #1\n"
        "\tmovs r6, #1\n"
        "\tands r6, r1\n"
        "\tcmp r6, #0\n"
        "\tbeq _08014DA0\n"
        "\tldr r0, _08014D9C\n"
        "\tldr r0, [r0]\n"
        "\tmovs r2, #0x80\n"
        "\tlsls r2, r2, #1\n"
        "\tmovs r1, #0xd\n"
        "\tbl PlaySfx\n"
        "\tmovs r0, #0\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x31\n"
        "\tstrb r0, [r1]\n"
        "\tsubs r1, #2\n"
        "\tstrb r7, [r1]\n"
        "\tsubs r1, #8\n"
        "\tstrb r0, [r1]\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_8014B54\n"
        "\tb _08014ED4\n"
        "\t.align 2, 0\n"
        "_08014D94: .4byte gUnknown_03001304\n"
        "_08014D98: .4byte gUnknown_030007E0\n"
        "_08014D9C: .4byte gUnknown_030012BC\n"
        "_08014DA0:\n"
        "\tmovs r0, #2\n"
        "\tands r0, r1\n"
        "\tlsls r0, r0, #0x10\n"
        "\tlsrs r5, r0, #0x10\n"
        "\tcmp r5, #0\n"
        "\tbeq _08014DC8\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_80153FC\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_80122CC\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r6, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r7, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r6, [r0]\n"
        "\tb _08014ED4\n"
        "_08014DC8:\n"
        "\tmov r0, r8\n"
        "\tcmp r0, #0\n"
        "\tbne _08014E40\n"
        "\tldr r0, [r4, #0x18]\n"
        "\tadds r0, #1\n"
        "\tstr r0, [r4, #0x18]\n"
        "\tcmp r0, #3\n"
        "\tble _08014E52\n"
        "\tstr r5, [r4, #0x18]\n"
        "\tldr r0, [r4, #0x10]\n"
        "\tldr r0, [r0, #0x30]\n"
        "\tcmp r0, #0\n"
        "\tbeq _08014E0C\n"
        "\tcmp r0, #4\n"
        "\tbgt _08014E08\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x28\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r1, #0\n"
        "\tldrsh r0, [r2, r1]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #0x23\n"
        "\tb _08014E2C\n"
        "_08014E08:\n"
        "\tcmp r0, #9\n"
        "\tble _08014E30\n"
        "_08014E0C:\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x28\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r1, #0\n"
        "\tldrsh r0, [r2, r1]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #0x22\n"
        "_08014E2C:\n"
        "\tbl sub_803AD84\n"
        "_08014E30:\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r7, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r5, [r0]\n"
        "\tb _08014E52\n"
        "_08014E40:\n"
        "\tstr r5, [r4, #0x18]\n"
        "\tmovs r0, #0x20\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x31\n"
        "\tstrb r5, [r1]\n"
        "\tsubs r1, #2\n"
        "\tstrb r7, [r1]\n"
        "\tsubs r1, #8\n"
        "\tstrb r0, [r1]\n"
        "_08014E52:\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_80122CC\n"
        "\tlsls r0, r0, #0x18\n"
        "\tcmp r0, #0\n"
        "\tbeq _08014ED4\n"
        "\tldr r0, [r4, #0x10]\n"
        "\tbl sub_80083B8\n"
        "\tadds r2, r0, #0\n"
        "\tldr r0, [r2, #4]\n"
        "\tldrb r0, [r0]\n"
        "\tlsrs r0, r0, #4\n"
        "\tcmp r0, #6\n"
        "\tbhi _08014EA4\n"
        "\tlsls r0, r0, #2\n"
        "\tldr r1, _08014E7C\n"
        "\tadds r0, r0, r1\n"
        "\tldr r0, [r0]\n"
        "\tmov pc, r0\n"
        "\t.align 2, 0\n"
        "_08014E7C: .4byte _08014E80\n"
        "_08014E80:\n"
        "\t.4byte _08014E9C\n"
        "\t.4byte _08014EA4\n"
        "\t.4byte _08014EA4\n"
        "\t.4byte _08014EA4\n"
        "\t.4byte _08014EA4\n"
        "\t.4byte _08014EA4\n"
        "\t.4byte _08014EA0\n"
        "_08014E9C:\n"
        "\tadds r2, #0x24\n"
        "\tb _08014EA6\n"
        "_08014EA0:\n"
        "\tadds r2, #0x14\n"
        "\tb _08014EA6\n"
        "_08014EA4:\n"
        "\tldr r2, _08014EC4\n"
        "_08014EA6:\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r0, [r1]\n"
        "\tasrs r3, r0, #8\n"
        "\tldr r4, [r1, #4]\n"
        "\tadds r0, r1, #0\n"
        "\tadds r0, #0x28\n"
        "\tldrb r0, [r0]\n"
        "\tlsls r0, r0, #0x1b\n"
        "\tcmp r0, #0\n"
        "\tbge _08014EC8\n"
        "\tmovs r5, #0\n"
        "\tldrsh r0, [r2, r5]\n"
        "\tadds r3, r3, r0\n"
        "\tb _08014ECE\n"
        "\t.align 2, 0\n"
        "_08014EC4: .4byte gStaticData_0816B300\n"
        "_08014EC8:\n"
        "\tmovs r5, #0\n"
        "\tldrsh r0, [r2, r5]\n"
        "\tsubs r3, r3, r0\n"
        "_08014ECE:\n"
        "\tlsls r0, r3, #8\n"
        "\tstr r0, [r1]\n"
        "\tstr r4, [r1, #4]\n"
        "_08014ED4:\n"
        "\tadd sp, #4\n"
        "\tpop {r3}\n"
        "\tmov r8, r3\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        ".syntax divided\n");
}
#endif

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
