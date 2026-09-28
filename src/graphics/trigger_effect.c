#include "core.h"
#include "gfx_part.h"

/* 0x08020E84-0x08021280 (GitHub issue #31): four of the "trigger effect
 * type N" spawners reached through the 15-slot dispatch table at
 * gStaticData_0816C7D8 (docs/rom_map.md, "A family of 'trigger effect
 * type N' functions") - the same family as sub_801EA5C-sub_801EE3C
 * (src/graphics/graphics_loading_1ea5c.c).
 *
 * Each one tests one "collected" bit of the level progress record
 * (gUnknown_030012C0 + 2). If it is set, the effect only plays a sound:
 * sub_801A878 with a per-slot id, or the shared id 0xC when
 * sub_8023278 says so or the record's +0x8C byte is set, handed to
 * sub_80234E8. Otherwise it spawns the full visual effect: a
 * sub_8008434 part on anim bank offset 0x180, with a per-slot tag,
 * built through the sub_80087C0/sub_80087B4/sub_800872C OAM trio, and
 * registered with the gUnknown_030012EC manager.
 *
 *   function     bit  sound  tag
 *   sub_8020E84   1    0xB    7
 *   sub_8020F7C   2    0x3    5
 *   sub_802107C   4    0xA    6
 *   sub_802117C   8    0x9    8
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the ROM materializes
 * the bit mask before loading the byte it is ANDed with, old_agbcc's
 * tell. Under it all four are plain C with no pins; they had been
 * parked as NAKED after drafts under the current agbcc stalled on
 * register allocation (see docs/matching/issue-31-trigger-effect-type-n.md,
 * "Old-compiler pass"). The two sub_801A878 calls are written
 * separately, one per sound id: the ROM repeats the a0 truncation in
 * both arms and shares the rest of the call, which is gcc's
 * cross-jumping of two call sites (a single call with an `id` variable
 * truncates a0 once, after the join). */

struct level_progress
{
    u8 unk_00[2];
    u8 collected;   // 0x02 - one bit per trigger effect slot
    u8 unk_03[0x89];
    u8 unk_8C;      // 0x8C
};

extern struct level_progress *gUnknown_030012C0;
extern u8 ***gUnknown_030012D0;
extern void *gUnknown_030012EC;

extern u8 sub_8023278(struct level_progress *self);
extern void *sub_801A878(u16 x, u16 y, u16 w, u16 h, s32 id);
extern void sub_80234E8(struct level_progress *self, void *handle);
extern struct gfx_part *sub_8008434(u16 a0, u16 a1, u16 a2, u16 a3);
extern void sub_80087C0(struct gfx_part *part);
extern void sub_80087B4(struct gfx_part *part);
extern void sub_800872C(struct gfx_part *part, s32 val);
extern s32 sub_800815C(struct gfx_part *part);
extern void sub_8008E94(void *manager, struct gfx_part *part);

/* The `tag` locals are set before the sub_8008434 call on purpose: the
 * ROM loads the constant into a callee-saved register up front and
 * stores it from there afterwards. */
void sub_8020E84(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = gUnknown_030012C0->collected & 1;

    if (bit)
    {
        void *snd;

        if (sub_8023278(gUnknown_030012C0) || gUnknown_030012C0->unk_8C)
            snd = sub_801A878(a0, a1, a2, a3, 0xC);
        else
            snd = sub_801A878(a0, a1, a2, a3, 0xB);
        sub_80234E8(gUnknown_030012C0, snd);
    }
    else
    {
        u8 tag = 7;
        struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
        part->tag = tag;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        part->frameNibble = sub_800815C(part);
        part->unk_0A = bit;
        sub_8008E94(gUnknown_030012EC, part);
        part->hidden = 0;
    }
}

void sub_8020F7C(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = gUnknown_030012C0->collected & 2;

    if (bit)
    {
        void *snd;

        if (sub_8023278(gUnknown_030012C0) || gUnknown_030012C0->unk_8C)
            snd = sub_801A878(a0, a1, a2, a3, 0xC);
        else
            snd = sub_801A878(a0, a1, a2, a3, 0x3);
        sub_80234E8(gUnknown_030012C0, snd);
    }
    else
    {
        u8 tag = 5;
        struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
        part->tag = tag;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        part->frameNibble = sub_800815C(part);
        part->unk_0A = bit;
        sub_8008E94(gUnknown_030012EC, part);
        part->hidden = 0;
    }
}

void sub_802107C(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = gUnknown_030012C0->collected & 4;

    if (bit)
    {
        void *snd;

        if (sub_8023278(gUnknown_030012C0) || gUnknown_030012C0->unk_8C)
            snd = sub_801A878(a0, a1, a2, a3, 0xC);
        else
            snd = sub_801A878(a0, a1, a2, a3, 0xA);
        sub_80234E8(gUnknown_030012C0, snd);
    }
    else
    {
        u8 tag = 6;
        struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
        part->tag = tag;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        part->frameNibble = sub_800815C(part);
        part->unk_0A = bit;
        sub_8008E94(gUnknown_030012EC, part);
        part->hidden = 0;
    }
}

/* Mask and tag are the same constant here, so the ROM keeps one copy of
 * it in a callee-saved register for both uses. */
void sub_802117C(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = gUnknown_030012C0->collected & 8;

    if (bit)
    {
        void *snd;

        if (sub_8023278(gUnknown_030012C0) || gUnknown_030012C0->unk_8C)
            snd = sub_801A878(a0, a1, a2, a3, 0xC);
        else
            snd = sub_801A878(a0, a1, a2, a3, 0x9);
        sub_80234E8(gUnknown_030012C0, snd);
    }
    else
    {
        u8 tag = 8;
        struct gfx_part *part = sub_8008434(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gUnknown_030012D0 + 0x180);
        part->tag = tag;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        part->frameNibble = sub_800815C(part);
        part->unk_0A = bit;
        sub_8008E94(gUnknown_030012EC, part);
        part->hidden = 0;
    }
}
