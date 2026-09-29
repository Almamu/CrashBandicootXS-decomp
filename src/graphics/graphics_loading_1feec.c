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
 * Still NAKED (draft under NON_MATCHING): the plain-C version is 62 halfwords off under old_agbcc,
 * all register allocation. The ROM keeps arg3 in r4 and
 * &gUnknown_030012B4 in sb, and spills part+0x28 to a single stack slot;
 * the C spills the shared constant 1 and &gUnknown_030012B4 instead.
 * Mix-6 pass: extra references or `"+r"` on arg3 (before the anim store,
 * the rec2 load or the tail), and other flip spellings (`!`, `^= 1`,
 * ternary, if/else, a u8-parameter setter, u8/u32 temp) are all 62 hw
 * or worse. In the draft the part+0x28 pointer outranks arg3 for a low
 * callee-saved register, which is the reverse of the ROM.
 * Hard-register hold pass: the draft also keeps `arg3 << 16` (the
 * zero-extension's first half) alive, spilled, to rebuild `arg3 * 2`
 * for the second LEVEL_RECORD; the ROM zero-extends a copy in r4 and
 * doubles it in place. A `u32` copy of arg3 behind `asm("" : "+r")`
 * puts it in r4 but moves the zero-extension after the first call (120
 * hw). r4/r5 holds over parts of the part+0x28 range were worse (80+).
 * Inline-argument-order pass: in the ROM, part+0x28 is in r3 and is
 * caller-saved around sub_8008E94 (`str r3,[sp]` before the call).
 * Zero-length r4/r5 holds at every statement boundary put arg3/hdr+0x84
 * in r4/r5, but part+0x28 then goes to r8, not r3 (80+ hw).
 * Last-six pass (docs/matching/last-six-naked-retry.md): global.c only
 * caller-saves a pseudo when no callee-saved register is free when its
 * turn comes. In the draft part+0x28 (6 refs over 49 insns) is
 * allocated before arg3 (3/63), &gUnknown_030012B4 (3/94, then left
 * unallocated and rematerialized) and -0x11 (3/86), so it gets r5. An
 * escaped `&gUnknown_030012B4` local gets sl but is 88 hw.
 * Last-eight pass (docs/matching/last-eight-naked-retry.md): 12 hw, same
 * size. The second flip goes through `q2`, a stack-resident copy of
 * part+0x28 (an `"m"` asm operand), so the first flip's part+0x28 is
 * block-local (r3) and arg3/hdr+0x84 get r4/r5. Left: the
 * `str r3, [sp]` comes one insn before `adds r1, r7, #0` instead of
 * after it (the ROM's looks like a caller-save), the reload registers
 * for `q2` (r4/r3 swapped), and &gUnknown_030012B4 / -0x11 in sl/r9
 * where the ROM has r9/sl.
 * Last-nine pass (docs/matching/last-nine-naked-retry.md): 4 hw, same
 * size. An r9 hold over the second flip fixes sl/r9, and extra
 * references on rec2 and the reloaded q2 fix the r4/r3 swap. Left: the
 * `str r3, [sp]` placement and `mov r1, sb` (the draft reloads through
 * r3). Both fit a caller-save of one part+0x28 pseudo in r3 (the save
 * goes right before the call, the restore right before the next use),
 * which needs every callee-saved register taken first; a single-pointer
 * draft with r4 pins and r5 holds was 62-140 hw. */
#if NON_MATCHING
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
    register s32 h9 asm("r9");

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
    sub_8008E94(gUnknown_030012F0, (q2 = (struct popup_bits *)((u8 *)part + 0x28), part));
    /* No code: the "m" operand keeps `q2` in a stack slot, the ROM's
     * `str r3, [sp]` / `ldr r3, [sp]` pair around the call. */
    asm("" : : "m"(q2));
    SetPopupGfx(hdr, gStaticData_0816B98C);
    rec2 = LEVEL_RECORD(arg3);
    /* No code: four extra references lift rec2's allocation priority
     * above the reloaded q2 pointer's, so rec2 keeps r2 and q2 gets r3. */
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    /* No code: hold r9 from here through the flip. -0x11 is live over
     * this range but &gUnknown_030012B4 is not, so -0x11 skips r9 (it
     * gets sl) and the address, re-allocated after its r5 spill, gets
     * r9 as in the ROM. */
    asm("" : "=r"(h9));
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
    /* No code: end of the r9 hold. */
    asm("" : : "r"(h9));
    SetPartField0A(part, 1);
    SetPopupGfx(hdr, gStaticData_0816BAAC);
    SetPopupSpan(hdr, rec2->unk_08, rec2->unk_04, rec2->unk_0C);
    sub_800C6A8(hdr, 4);
}
#else
NAKED void sub_802062C(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tsub sp, #4\n"
        "\tadd r4, r3, #0\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r4, r4, #0x10\n"
        "\tlsr r4, r4, #0x10\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tadd r3, r4, #0\n"
        "\tbl sub_8009ED0\n"
        "\tadd r7, r0, #0\n"
        "\tldr r0, 2f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tmov r1, #0x8a\n"
        "\tlsl r1, r1, #1\n"
        "\tadd r0, r0, r1\n"
        "\tstr r0, [r7, #0x20]\n"
        "\tadd r0, r7, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r7, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r3, [r2]\n"
        "\tand r1, r3\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r6, r0, #0\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r5, #0x18\n"
        "\tldrsh r0, [r1, r5]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r7, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #0x17\n"
        "\tstr r0, [r6, #0x6c]\n"
        "\tstr r6, [r7, #0x44]\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r7, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r3, #1\n"
        "\tmov r8, r3\n"
        "\tstrb r3, [r7, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r5, [r7, #0xc]\n"
        "\tand r0, r5\n"
        "\tstrb r0, [r7, #0xc]\n"
        "\tldr r0, 3f @ =gUnknown_030012B4\n"
        "\tmov sb, r0\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tlsl r4, r4, #1\n"
        "\tadd r0, r4, r0\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r1, [r2]\n"
        "\tlsr r0, r1, #1\n"
        "\tmov r3, r8\n"
        "\teor r0, r3\n"
        "\tand r0, r3\n"
        "\tadd r3, r7, #0\n"
        "\tadd r3, #0x28\n"
        "\tmov r5, r8\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tmov sl, r1\n"
        "\tldrb r5, [r3]\n"
        "\tand r1, r5\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tmov r2, r8\n"
        "\tand r0, r2\n"
        "\tand r0, r2\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 4f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r7, #0\n"
        "\tstr r3, [sp]\n"
        "\tbl sub_8008E94\n"
        "\tldr r0, 5f @ =gStaticData_0816B98C\n"
        "\tadd r5, r6, #0\n"
        "\tadd r5, #0x84\n"
        "\tstr r0, [r5]\n"
        "\tmov r1, sb\n"
        "\tldr r0, [r1]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tadd r4, r4, r0\n"
        "\tldr r0, [r1, #0xc]\n"
        "\tldrh r4, [r4]\n"
        "\tadd r2, r4, r0\n"
        "\tldr r3, [sp]\n"
        "\tldrb r4, [r3]\n"
        "\tlsl r0, r4, #0x1b\n"
        "\tmov r1, #0\n"
        "\tcmp r0, #0\n"
        "\tblt 1f\n"
        "\tmov r1, #1\n"
        "1:\n"
        "\tmov r0, r8\n"
        "\tand r1, r0\n"
        "\tlsl r1, r1, #4\n"
        "\tmov r0, sl\n"
        "\tand r0, r4\n"
        "\torr r0, r1\n"
        "\tstrb r0, [r3]\n"
        "\tmov r1, #1\n"
        "\tstrb r1, [r7, #0xa]\n"
        "\tldr r0, 6f @ =gStaticData_0816BAAC\n"
        "\tstr r0, [r5]\n"
        "\tldr r0, [r2, #8]\n"
        "\tldr r1, [r2, #4]\n"
        "\tldr r2, [r2, #0xc]\n"
        "\tstr r0, [r6, #0x30]\n"
        "\tstr r1, [r6, #0x34]\n"
        "\tstr r2, [r6, #0x38]\n"
        "\tadd r0, r6, #0\n"
        "\tmov r1, #4\n"
        "\tbl sub_800C6A8\n"
        "\tadd sp, #4\n"
        "\tpop {r3, r4, r5}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tmov sl, r5\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "2: .4byte gUnknown_030012D0\n"
        "3: .4byte gUnknown_030012B4\n"
        "4: .4byte gUnknown_030012F0\n"
        "5: .4byte gStaticData_0816B98C\n"
        "6: .4byte gStaticData_0816BAAC\n"
    );
}
#endif

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
