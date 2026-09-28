#include "core.h"
#include "action_obj.h"

/* GitHub issue #17, ROM 0x08013C60-0x0801426C, formerly
 * asm/code_3_2_17_12af4.s (details in
 * docs/matching/issue-17-0x08012fbc-actor.md, "Second pass").
 *
 * Five more entries of the gStaticData_0816BF20 42-slot action table
 * (docs/rom_map.md), the same player/action object as
 * actor_part18.c/actor_part_138e8.c: `self+0xc` is its method table,
 * `self+0x10` its on-screen part, `self+0x18`/`+0x1c` an animation
 * counter/limit, and the +0x27..+0x32 bytes the shared "next action"
 * trio (actor_part18.c). Each handler snapshots the input word
 * gUnknown_030007E0 (the high half is the newly-pressed buttons) and
 * most also the D-pad direction sub_8000760 remaps. */

asm(".set _call_via_r2, sub_803AD80\n"
    ".set _call_via_r3, sub_803AD84\n");

extern u32 gUnknown_030007E0;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *gUnknown_03001304;
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern u8 sub_8000760(void *pad);
extern u8 sub_80231B4(void *self);
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
 * sub_8000760's argument before taking the input snapshot, hence the
 * `pad` local. */
void sub_8013C60(struct act *self)
{
    u32 in;
    u8 dir;

    {
        void *pad = gUnknown_03001304;

        in = gUnknown_030007E0;
        dir = sub_8000760(pad);
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
    if (sub_80231B4(gUnknown_030012C0) && (INPUT_PRESSED(in) & 2) && self->unk_26 == 0)
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

    in = gUnknown_030007E0;
    part = self->part;

    if ((part->contact & 8) && part->unk_64 > 0)
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
    if (sub_80231B4(gUnknown_030012C0) && (INPUT_PRESSED(in) & 2) && self->unk_26 == 0)
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
            self->unk_26 = 0xC;
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
        void *pad = gUnknown_03001304;

        in = gUnknown_030007E0;
        dir = sub_8000760(pad);
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
    if (sub_80231B4(gUnknown_030012C0) && (INPUT_PRESSED(in) & 2) && self->unk_26 == 0)
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

    in = gUnknown_030007E0;
    fire = INPUT_PRESSED(in) & 1;

    if (fire)
    {
        PlaySfx(gUnknown_030012BC, 0xC, 0x100);
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
 * NAKED: the C below (old_agbcc) is one instruction off. The facing
 * block reads `self->part` each time (GCSE turns the reloads into the
 * ROM's r2 copy) and spells its two bit tests differently so gcc doesn't
 * thread the second into the first; only the second branch's `adds r2,
 * #40` lands after the -0x11 mask instead of before it (writing the
 * pointer first moves it to r0) - see
 * docs/matching/issue-15-16-17-naked-retry-2.md. */
#if NON_MATCHING
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
        void *pad = gUnknown_03001304;

        in = gUnknown_030007E0;
        dir = sub_8000760(pad);
    }
    if ((INPUT_PRESSED(in) & 1) && sub_800AAEC(self->part, 0xB) == 1)
    {
        PlaySfx(gUnknown_030012BC, 0xC, 0x100);
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
        m = -0x11;
        m &= self->part->flags28;
        m |= 0x10;
        self->part->flags28 = m;
        self->flag2F = turned;
    }
turn_done:

    moved = 0;
    if (!turned)
    {
        switch (sub_8000760(gUnknown_03001304))
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
#else
NAKED void sub_8014084(struct act *self)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tsub sp, #4\n"
        "\tadds r4, r0, #0\n"
        "\tldr r0, _080140E8\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, _080140EC\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [sp]\n"
        "\tbl sub_8000760\n"
        "\tlsls r0, r0, #0x18\n"
        "\tlsrs r5, r0, #0x18\n"
        "\tmov r0, sp\n"
        "\tldrh r1, [r0, #2]\n"
        "\tmovs r0, #1\n"
        "\tands r0, r1\n"
        "\tcmp r0, #0\n"
        "\tbeq _080140F4\n"
        "\tldr r0, [r4, #0x10]\n"
        "\tmovs r1, #0xb\n"
        "\tbl sub_800AAEC\n"
        "\tlsls r0, r0, #0x18\n"
        "\tlsrs r0, r0, #0x18\n"
        "\tcmp r0, #1\n"
        "\tbne _080140F4\n"
        "\tldr r0, _080140F0\n"
        "\tldr r0, [r0]\n"
        "\tmovs r2, #0x80\n"
        "\tlsls r2, r2, #1\n"
        "\tmovs r1, #0xc\n"
        "\tbl PlaySfx\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tmovs r0, #2\n"
        "\trsbs r0, r0, #0\n"
        "\tldrb r2, [r1, #0xd]\n"
        "\tands r0, r2\n"
        "\tstrb r0, [r1, #0xd]\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tmovs r0, #3\n"
        "\trsbs r0, r0, #0\n"
        "\tldrb r2, [r1, #0xd]\n"
        "\tands r0, r2\n"
        "\tstrb r0, [r1, #0xd]\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_8015508\n"
        "\tb _08014264\n"
        "\t.align 2, 0\n"
        "_080140E8: .4byte gUnknown_03001304\n"
        "_080140EC: .4byte gUnknown_030007E0\n"
        "_080140F0: .4byte gUnknown_030012BC\n"
        "_080140F4:\n"
        "\tadds r0, r4, #0\n"
        "\tbl sub_8012A7C\n"
        "\tlsls r0, r0, #0x18\n"
        "\tcmp r0, #0\n"
        "\tbeq _08014102\n"
        "\tb _08014264\n"
        "_08014102:\n"
        "\tmovs r3, #0\n"
        "\tldr r0, [r4, #0x10]\n"
        "\tadds r1, r0, #0\n"
        "\tadds r1, #0x28\n"
        "\tldrb r1, [r1]\n"
        "\tlsls r1, r1, #0x1b\n"
        "\tadds r2, r0, #0\n"
        "\tcmp r1, #0\n"
        "\tbge _0801413A\n"
        "\tcmp r5, #4\n"
        "\tbeq _08014120\n"
        "\tcmp r5, #6\n"
        "\tbeq _08014120\n"
        "\tcmp r5, #8\n"
        "\tbne _0801413A\n"
        "_08014120:\n"
        "\tadds r0, r2, #0\n"
        "\tadds r0, #0x28\n"
        "\tmovs r1, #0x11\n"
        "\trsbs r1, r1, #0\n"
        "\tldrb r2, [r0]\n"
        "\tands r1, r2\n"
        "\tstrb r1, [r0]\n"
        "\tadds r1, r4, #0\n"
        "\tadds r1, #0x2f\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r1]\n"
        "\tmovs r3, #1\n"
        "\tb _0801416A\n"
        "_0801413A:\n"
        "\tadds r0, r2, #0\n"
        "\tadds r0, #0x28\n"
        "\tldrb r0, [r0]\n"
        "\tlsls r0, r0, #0x1b\n"
        "\tcmp r0, #0\n"
        "\tblt _0801416A\n"
        "\tcmp r5, #3\n"
        "\tbeq _08014152\n"
        "\tcmp r5, #5\n"
        "\tbeq _08014152\n"
        "\tcmp r5, #7\n"
        "\tbne _0801416A\n"
        "_08014152:\n"
        "\tmovs r3, #1\n"
        "\tadds r2, #0x28\n"
        "\tmovs r0, #0x11\n"
        "\trsbs r0, r0, #0\n"
        "\tldrb r1, [r2]\n"
        "\tands r0, r1\n"
        "\tmovs r1, #0x10\n"
        "\torrs r0, r1\n"
        "\tstrb r0, [r2]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x2f\n"
        "\tstrb r3, [r0]\n"
        "_0801416A:\n"
        "\tmovs r6, #0\n"
        "\tcmp r3, #0\n"
        "\tbne _080141C8\n"
        "\tldr r0, _0801421C\n"
        "\tldr r0, [r0]\n"
        "\tbl sub_8000760\n"
        "\tlsls r0, r0, #0x18\n"
        "\tlsrs r0, r0, #0x18\n"
        "\tcmp r0, #3\n"
        "\tblt _080141C8\n"
        "\tcmp r0, #4\n"
        "\tble _0801418C\n"
        "\tcmp r0, #8\n"
        "\tbgt _080141C8\n"
        "\tcmp r0, #7\n"
        "\tblt _080141C8\n"
        "_0801418C:\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x13\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r1, #0\n"
        "\tldrsh r0, [r2, r1]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #0x14\n"
        "\tbl sub_803AD84\n"
        "\tmovs r1, #3\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r6, [r0]\n"
        "\tadds r2, r4, #0\n"
        "\tadds r2, #0x2f\n"
        "\tmovs r0, #1\n"
        "\tstrb r0, [r2]\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x27\n"
        "\tstrb r1, [r0]\n"
        "\tmovs r6, #1\n"
        "_080141C8:\n"
        "\tmov r0, sp\n"
        "\tmovs r5, #0xc0\n"
        "\tlsls r5, r5, #1\n"
        "\tldrh r0, [r0]\n"
        "\tands r5, r0\n"
        "\tcmp r5, #0\n"
        "\tbne _08014264\n"
        "\tldr r0, [r4, #0x10]\n"
        "\tmovs r1, #2\n"
        "\tbl sub_800AAEC\n"
        "\tlsls r0, r0, #0x18\n"
        "\tlsrs r7, r0, #0x18\n"
        "\tcmp r7, #1\n"
        "\tbne _08014220\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x12\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r1, #0\n"
        "\tldrsh r0, [r2, r1]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #2\n"
        "\tbl sub_803AD84\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r5, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r7, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r5, [r0]\n"
        "\tb _08014264\n"
        "\t.align 2, 0\n"
        "_0801421C: .4byte gUnknown_03001304\n"
        "_08014220:\n"
        "\tcmp r6, #0\n"
        "\tbne _08014264\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmovs r2, #0x20\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r2, [r1, #0x24]\n"
        "\tmovs r1, #0x11\n"
        "\tbl sub_803AD80\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tadds r2, #0x50\n"
        "\tmovs r1, #0\n"
        "\tldrsh r0, [r2, r1]\n"
        "\tadds r0, r4, r0\n"
        "\tldr r1, [r4, #0x10]\n"
        "\tldr r3, [r2, #4]\n"
        "\tmovs r2, #4\n"
        "\tbl sub_803AD84\n"
        "\tadds r0, r4, #0\n"
        "\tadds r0, #0x31\n"
        "\tstrb r6, [r0]\n"
        "\tsubs r0, #2\n"
        "\tmovs r1, #1\n"
        "\tstrb r1, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r6, [r0]\n"
        "\tadds r0, #0xb\n"
        "\tstrb r6, [r0]\n"
        "\tsubs r0, #2\n"
        "\tstrb r1, [r0]\n"
        "\tsubs r0, #8\n"
        "\tstrb r6, [r0]\n"
        "_08014264:\n"
        "\tadd sp, #4\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        ".syntax divided\n");
}

#endif
asm(".align 2, 0");
