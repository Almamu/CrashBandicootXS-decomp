#include "core.h"
#include "text_popup.h"

/* "Two-line text popup" spawners, ROM 0x0801FEEC-0x08020E84 - the
 * continuation of graphics_loading_1ef0c.c. Built with old_agbcc; see
 * include/text_popup.h. */

extern u8 gStaticData_0816B98C[];
extern u8 gStaticData_0816BAAC[];
extern u8 gStaticData_0816BACC[];
extern u8 gStaticData_0816BAEC[];
extern u8 gStaticData_0816BB0C[];
extern u8 gStaticData_0816BB2C[];
extern u8 gStaticData_0816BB4C[];

/* Text popup, tag 9. Shows the header with gStaticData_0816B98C and
 * style 6, then copies the level record's +8/+0xc/+4 words into
 * header+0x3c/0x40/0x44. */
void sub_801FEEC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x6c);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 9;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    sub_800C6A8(hdr, 6);
    SetPopupBox(hdr, rec2->unk_08, rec2->unk_0C, rec2->unk_04);
}

/* Text popup, tag 0x19, anim +0x12c. Flips the part's flipX, sets
 * field_0A to 2, clears flag bit 6 and shows the header with style 1. */
void sub_8020010(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x12c);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x19;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    {
        s32 f = part->flipX;
        part->flipX = f == 0;
    }
    SetPartField0A(part, 2);
    AndPartFlags(part, ~0x40);
    sub_800C6A8(hdr, 1);
}

/* Text popup, tag 0x1b, anim +0x144. After registering the part it
 * switches the header's graphics to gStaticData_0816BACC, copies the
 * level record's +8/+4/+0xc fields into header+0x30/0x34/0x38 and shows
 * it with style 4. */
void sub_8020138(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x144);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x1b;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    SetPopupGfx(hdr, gStaticData_0816BACC);
    SetPopupSpan(hdr, rec2->unk_08, rec2->unk_04, rec2->unk_0C);
    sub_800C6A8(hdr, 4);
}

/* Text popup, tag 0x18, anim +0x120. After registering the part it
 * switches the header's graphics to gStaticData_0816BAEC, copies the
 * level record's +8/+0xc/+0x10 fields into header+0x30/0x34/0x38, passes
 * the record's +4 to sub_800C898 and shows the header with style 0xd. */
void sub_802026C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x120);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x18;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    SetPopupGfx(hdr, gStaticData_0816BAEC);
    SetPopupSpan(hdr, rec2->unk_08, rec2->unk_0C, rec2->unk_10);
    sub_800C898(hdr, rec2->unk_04);
    sub_800C6A8(hdr, 0xd);
}

/* Text popup, tag 0x1d, anim +0x15c, spawned 0x28 pixels above arg2.
 * After registering the part it switches the header's graphics to
 * gStaticData_0816BB4C, sets header+0x30/0x34/0x38 to {0x78, 0x5a,
 * record+0xc}, calls sub_800C898(hdr, 0x28) and shows it with style 0x12. */
void sub_80203A8(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u16 y = arg2 - 0x28;
    struct popup_part *part = sub_8009ED0(arg0, arg1, y, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x15c);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x1d;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    SetPopupGfx(hdr, gStaticData_0816BB4C);
    SetPopupSpan(hdr, 0x78, 0x5a, rec2->unk_0C);
    sub_800C898(hdr, 0x28);
    sub_800C6A8(hdr, 0x12);
}

/* Text popup, tag 0x1a, anim +0x138. After registering the part it
 * switches it to mode 0xa with flags bit 6 cleared, points the header at
 * gStaticData_0816BB2C, copies the level record's +4/+8/+0xc fields into
 * header+0x30/0x34/0x38 and shows it with style 4. */
void sub_80204EC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x138);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x1a;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    SetPartField0A(part, 0xa);
    AndPartFlags(part, ~0x40);
    SetPopupGfx(hdr, gStaticData_0816BB2C);
    SetPopupSpan(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
    sub_800C6A8(hdr, 4);
}

/* Text popup, tag 0x17, anim +0x114. Flips the part's flipX bit, points
 * the header at gStaticData_0816BAAC, copies the level record's
 * +8/+4/+0xc words into header+0x30/0x34/0x38 and shows it with style 4.
 * Register allocation took several passes (see
 * docs/matching/last-eleven-naked-retry.md and the passes it links).
 * The ROM keeps arg3 in r4, part+0x28 in r3 across sub_8008E94 through a
 * stack slot (`str r3, [sp]` after the argument setup, `ldr r3, [sp]`
 * before the second flip), &gEntityFlags in sb and -0x11 in sl.
 * The second flip reads part+0x28 back from `q2`, a stack-resident copy
 * (an `"m"` asm operand), so the first flip's pointer is block-local (r3)
 * and arg3/hdr+0x84 get r4/r5. */

/* The part's +0x28 bitfield byte seen through its own pointer. Padded
 * past a word so the fields are read with `ldrb` (a 4-byte struct is
 * read as a whole word). */
struct popup_bits
{
    u32 unk_28_0:4;
    u32 flipX:1;
    u32 unk_28_5:1;
    u32 unk_28_6:2;
    u8 unk_29[0x1f];
};

void sub_802062C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;
    struct popup_bits *q2;
    register s32 h3 asm("r3");

    part->anim = POPUP_ANIM(0x114);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x17;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    /* The `q2` store sits inside the second argument, after a copy of
     * `part` that the empty asm (no code) keeps as its own pseudo. That
     * copy is tied to r1, so the ROM's order comes out: `adds r1, r7, #0`,
     * then `str r3, [sp]`, then the call. With `(q2 = ..., part)` the
     * store comes before the r1 move. */
    sub_8008E94(gUnknown_030012F0, ({
        struct popup_part *t = part;

        asm("" : "+r"(t));
        q2 = (struct popup_bits *)((u8 *)part + 0x28);
        t;
    }));
    /* No code: the "m" operand keeps `q2` in a stack slot, the ROM's
     * `str r3, [sp]` / `ldr r3, [sp]` pair around the call. */
    asm("" : : "m"(q2));
    /* No code: hold r3 over the gfx store and the rec2 lookup. The ROM
     * keeps r3 free there, so reloading &gEntityFlags out of sb
     * uses r1 (`mov r1, sb`), not r3. */
    asm("" : "=r"(h3));
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    /* No code: end of the r3 hold. */
    asm("" : : "r"(h3));
    /* No code: four extra references lift rec2's allocation priority
     * above the reloaded q2 pointer's, so rec2 keeps r2 and q2 gets r3. */
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    {
        struct popup_bits *p = q2;
        s32 f;

        /* No code: one extra reference puts the reloaded q2 pointer
         * ahead of the flag byte, so the pointer gets r3 and the byte
         * r4. */
        asm("" : : "r"(p));
        f = p->flipX;
        p->flipX = f == 0;
    }
    SetPartField0A(part, 1);
    SetPopupGfx(hdr, gStaticData_0816BAAC);
    SetPopupSpan(hdr, rec2->unk_08, rec2->unk_04, rec2->unk_0C);
    sub_800C6A8(hdr, 4);
}

/* Text popup, tag 0x16, anim +0x108. After registering the part it shows
 * the header with style 9, passes the level record's +4/+8/+0xc fields
 * to sub_800C860 and sets header+0x3c/0x40/0x44 to {0x80, 0, 0x14}. */
void sub_8020788(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x108);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x16;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    sub_800C6A8(hdr, 9);
    sub_800C860(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
    SetPopupBox(hdr, 0x80, 0, 0x14);
}

/* Text popup, tag 0x14, anim +0xf0. After registering the part it points
 * the header at gStaticData_0816BB0C, shows it with style 2 and passes
 * the level record's +4 field to sub_800C898. */
void sub_80208C4(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xf0);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x14;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    SetPopupGfx(hdr, gStaticData_0816BB0C);
    sub_800C6A8(hdr, 2);
    sub_800C898(hdr, rec2->unk_04);
}

/* "Two-line text popup" spawner, tag 0x15. After the shared setup it
 * points the header at gStaticData_0816B98C, shows it with
 * sub_800C6A8(hdr, 2) and passes the level record's +4 word to
 * sub_800C898. */
void sub_80209EC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xfc);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x15;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    sub_800C6A8(hdr, 2);
    sub_800C898(hdr, rec2->unk_04);
}

/* "Two-line text popup" spawner, tag 0x13. The tail points the header at
 * gStaticData_0816B98C, switches the part's field_0A from 1 to 7, then
 * re-points the header at gStaticData_0816BB0C before showing it with
 * sub_800C6A8(hdr, 8). */
void sub_8020B0C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0xe4);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x13;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    part->base.field_0A = 7;
    hdr->gfx = gStaticData_0816BB0C;
    sub_800C6A8(hdr, 8);
}

/* "Two-line text popup" spawner, tag 6. The tail sets the part's field_0A
 * to 4, shows the header with sub_800C6A8(hdr, 0xb), then hands it the
 * level record's X bounds (+0x10..+0x18, sub_800C860) and Y bounds
 * (+0x4..+0xc, sub_800C87C). */
void sub_8020C18(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x48);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 6;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    part->base.field_0A = 4;
    sub_800C6A8(hdr, 0xb);
    sub_800C860(hdr, rec2->unk_10, rec2->unk_14, rec2->unk_18);
    sub_800C87C(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
}

/* "Two-line text popup" spawner, tag 0x12. The tail sets the part's
 * field_0A to 0xa and clears flag bit 6, re-points the header from
 * gStaticData_0816B98C to gStaticData_0816BB2C, copies the level
 * record's +4/+8/+0xc words into the header's +0x30 box, and shows it
 * with sub_800C6A8(hdr, 4). */
void sub_8020D4C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xd8);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 0x12;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    part->base.field_0A = 0xa;
    AndPartFlags(part, ~0x40);
    hdr->gfx = gStaticData_0816BB2C;
    SetPopupSpan(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
    sub_800C6A8(hdr, 4);
}
