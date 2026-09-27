#include "core.h"
#include "gfx_part.h"

/* 0x0801EA5C-0x0801EF0C (GitHub issue #30), formerly
 * asm/code_3_2_17_1e990.s: six of the "trigger effect type N" spawners
 * reached through the trigger dispatch table at gStaticData_0816C6C0
 * (sub_801EB04 is also called directly by game_loop2.c). Each one
 * spawns a sub_8008434 part with a fixed bank offset, tag and type byte
 * (+0x0A) and registers it with the gUnknown_030012EC manager, unless
 * the level's "already collected" bit for it is set:
 *
 * - sub_801EA5C/sub_801EB04/sub_801EBF0 test bits 0/1/2 of the byte
 *   sub_8023404(gUnknown_030012C0) points at; sub_801EB04 also spawns a
 *   second (0x2B) effect through sub_8025BAC.
 * - sub_801EC9C/sub_801ED6C/sub_801EE3C first ask sub_80233B4 whether
 *   the level is in mode 1, and if so hand over to sub_8018D70
 *   (actor_part_188d0.c) with kind 0/1/2 instead; otherwise they test
 *   bits 0/2/1 of gUnknown_030012C0+2.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the ROM materializes
 * each bit mask before loading the byte it is ANDed with, old_agbcc's
 * tell. Under it all six are plain C with no pins (the current agbcc
 * misses all six - likely also what parked the trigger_effect.c
 * siblings, issue #31). See docs/matching/issue-30-graphics-loading.md,
 * "Tenth pass". */

extern void *gUnknown_030012C0;
extern u8 ***gUnknown_030012D0;
extern void *gUnknown_030012E4;
extern void *gUnknown_030012EC;

extern u8 *sub_8023404(void *self);
extern s32 sub_80233B4(void *self);
extern struct gfx_part *sub_8008434(u16 a0, u16 a1, u16 a2, u16 a3);
extern void sub_80087C0(struct gfx_part *part);
extern void sub_80087B4(struct gfx_part *part);
extern void sub_800872C(struct gfx_part *part, s32 val);
extern s32 sub_800815C(struct gfx_part *part);
extern void sub_8008E94(void *manager, struct gfx_part *part);
extern struct gfx_part *sub_8025BAC(void *pool, s32 arg1, s32 kind, s32 x, s32 y, s32 arg5);
extern void sub_8018D70(u32 a0, u16 a1, u16 a2, u16 a3, s32 kind);

/* The `tag`/`type` locals are not just naming: the ROM loads both
 * constants into callee-saved registers before the sub_8008434 call and
 * stores them from there afterwards, which is how this compiler treats a
 * variable set before the call (a literal would be loaded at the store). */
void sub_801EA5C(u32 a0, u16 a1, u16 a2, u16 a3)
{
    s32 bit = *sub_8023404(gUnknown_030012C0) & 1;

    if (bit == 0)
    {
        u8 type = 0x1B;
        struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x1BC);
        part->tag = bit;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        part->frameNibble = sub_800815C(part);
        part->unk_0A = type;
        sub_8008E94(gUnknown_030012EC, part);
    }
}

void sub_801EB04(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = *sub_8023404(gUnknown_030012C0) & 2;

    if (bit == 0)
    {
        u8 tag = 1;
        u8 type = 0x1D;
        struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
        part->tag = tag;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        part->frameNibble = sub_800815C(part);
        part->unk_0A = type;
        sub_8008E94(gUnknown_030012EC, part);

        {
            struct gfx_part *p = sub_8025BAC(gUnknown_030012E4, 0x2B, 2, a1, a2, bit);
            p->unk_28_0 = 1;
            p->hidden = 0;
        }
    }
}

void sub_801EBF0(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if ((*sub_8023404(gUnknown_030012C0) & 4) == 0)
    {
        u8 tag = 1;
        u8 type = 0x1E;
        struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
        part->tag = tag;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        part->frameNibble = sub_800815C(part);
        part->unk_0A = type;
        sub_8008E94(gUnknown_030012EC, part);
    }
}

void sub_801EC9C(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (sub_80233B4(gUnknown_030012C0) != 1)
    {
        if ((((u8 *)gUnknown_030012C0)[2] & 1) == 0)
        {
            u8 tag = 3;
            u8 type = 0x1F;
            struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

            part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
            part->tag = tag;
            sub_80087C0(part);
            sub_80087B4(part);
            sub_800872C(part, 0);
            part->frameNibble = sub_800815C(part);
            part->unk_0A = type;
            sub_8008E94(gUnknown_030012EC, part);
        }
    }
    else
    {
        sub_8018D70(a0, a1, a2, a3, 0);
    }
}

void sub_801ED6C(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (sub_80233B4(gUnknown_030012C0) != 1)
    {
        if ((((u8 *)gUnknown_030012C0)[2] & 4) == 0)
        {
            u8 tag = 2;
            u8 type = 0x20;
            struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

            part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
            part->tag = tag;
            sub_80087C0(part);
            sub_80087B4(part);
            sub_800872C(part, 0);
            part->frameNibble = sub_800815C(part);
            part->unk_0A = type;
            sub_8008E94(gUnknown_030012EC, part);
        }
    }
    else
    {
        sub_8018D70(a0, a1, a2, a3, 1);
    }
}

void sub_801EE3C(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (sub_80233B4(gUnknown_030012C0) != 1)
    {
        u8 bit = ((u8 *)gUnknown_030012C0)[2] & 2;

        if (bit == 0)
        {
            u8 type = 0x22;
            struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

            part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
            part->tag = bit;
            sub_80087C0(part);
            sub_80087B4(part);
            sub_800872C(part, 0);
            part->frameNibble = sub_800815C(part);
            part->unk_0A = type;
            sub_8008E94(gUnknown_030012EC, part);
        }
    }
    else
    {
        sub_8018D70(a0, a1, a2, a3, 2);
    }
}
