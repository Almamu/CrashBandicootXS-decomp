#include "core.h"
#include "text_popup.h"

/* Text-popup variants with their own header constructors, ROM
 * 0x08021280-0x08021668. Built with old_agbcc; see include/text_popup.h. */

extern void *gUnknown_030012C0;
extern void *gUnknown_030012F4;

extern struct popup_hdr *sub_801A838(void *block, u16 arg1, u16 arg2);
extern void sub_8023318(void *self, struct popup_hdr *hdr);
extern struct popup_hdr *sub_80189EC(void);
extern struct popup_hdr *sub_80197DC(void);

/* Three-way spawner. While the level controller reports nothing pending
 * and the current level's table entry has no guard, spawns a 0x64x0x64
 * sub_80071E4 part tagged 0x12. Otherwise, unless gUnknown_030012D8's
 * +0x88 flag is set, hands sub_8023500 a point just above-left of a
 * sub_801A878 probe; with the flag set it spawns a 0x28x0x28 part.
 * Still NAKED: the plain-C version is 9 halfwords off under old_agbcc.
 * The ROM computes the point's x/y into fresh registers
 * (`subs r2, r1, #2`; `adds r3, r0, #0; subs r3, #30`) where the C
 * reuses their inputs, the same gap as sub_802209C
 * (graphics_loading_21d80.c). */
NAKED void sub_8021280(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #0xc\n\t"
        "add r6, r0, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r7, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "mov r8, r2\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r3, r3, #0x10\n\t"
        "mov sb, r3\n\t"
        "ldr r5, 1f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8023290\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80232B8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8023324\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r4, 2f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_802332C\n\t"
        "lsl r1, r0, #3\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "add r4, #4\n\t"
        "add r1, r1, r4\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "lsl r0, r6, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, r8\n\t"
        "mov r3, sb\n\t"
        "bl sub_80071E4\n\t"
        "add r4, r0, #0\n\t"
        "mov r1, #0x64\n\t"
        "mov r2, #0x64\n\t"
        "bl sub_80070EC\n\t"
        "mov r0, #0x12\n\t"
        "strb r0, [r4, #0xa]\n\t"
        "ldr r0, 4f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012C0\n"
    "2: .4byte gStaticData_0816C86C\n"
    "4: .4byte gUnknown_030012E8\n"
    "3:\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x88\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 6f\n\t"
        "lsl r0, r6, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #4\n\t"
        "str r1, [sp]\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, r8\n\t"
        "mov r3, sb\n\t"
        "bl sub_801A878\n\t"
        "ldr r1, [r0]\n\t"
        "asr r1, r1, #8\n\t"
        "sub r2, r1, #2\n\t"
        "ldr r0, [r0, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "add r3, r0, #0\n\t"
        "sub r3, #0x1e\n\t"
        "str r2, [sp, #4]\n\t"
        "str r3, [sp, #8]\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, sp, #4\n\t"
        "bl sub_8023500\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "5: .4byte gUnknown_030012D8\n"
    "8: .4byte gUnknown_030012C0\n"
    "6:\n\t"
        "lsl r0, r6, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, r8\n\t"
        "mov r3, sb\n\t"
        "bl sub_80071E4\n\t"
        "add r4, r0, #0\n\t"
        "mov r1, #0x28\n\t"
        "mov r2, #0x28\n\t"
        "bl sub_80070EC\n\t"
        "mov r0, #0x12\n\t"
        "strb r0, [r4, #0xa]\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
    "7:\n\t"
        "add sp, #0xc\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "9: .4byte gUnknown_030012E8\n"
    );
}

/* "Two-line text popup" variant with its own header: instead of
 * sub_800CA74 it builds one with sub_801A838 in a fresh 0x30-byte block
 * (from arg1/arg2), attaches the part to it once, and registers the
 * header with the level controller via sub_8023318. */
void sub_8021388(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x288);
    part->frameNibble = sub_800815C(part);
    part->base.flags |= 0x10;
    SetPartField0A(part, 1);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    hdr = sub_801A838(sub_8026EDC(0x30), arg1, arg2);
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    sub_8023318(gUnknown_030012C0, hdr);
}

/* "Two-line text popup" variant whose header comes from sub_80189EC
 * (after a 0x4c-byte sub_8026EDC reservation). Attaches the part once,
 * packs the collected bits, sets flag bit 4, and registers the part and
 * the header with the manager and the level controller. */
void sub_8021480(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x294);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x4c);
    hdr = sub_80189EC();
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    part->base.flags |= 0x10;
    sub_8008E94(gUnknown_030012F0, part);
    sub_8023318(gUnknown_030012C0, hdr);
}

/* "Two-line text popup" variant with the OAM-trio setup: animation 1 at
 * +0x27c, header from sub_80197DC (after a 0x24-byte sub_8026EDC
 * reservation), collected bits and flag bit 4, then registration with
 * gUnknown_030012F4's manager and the level controller. */
void sub_802155C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x27c);
    SetPartTag(part, 1);
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x24);
    hdr = sub_80197DC();
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    part->base.flags |= 0x10;
    sub_8008E94(gUnknown_030012F4, part);
    part->unk_2A[2] = 0;
    sub_8023318(gUnknown_030012C0, hdr);
}
