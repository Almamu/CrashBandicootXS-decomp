#include "core.h"
#include "actor.h"

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern void *sub_80083B8(void *part);
extern u8 gStaticData_0816B2F8[];

/* A third AABB-for-keyframe builder (see sub_8007B00/sub_8007B98 in
 * src/graphics/actor_part.c), this time selecting its 6-byte
 * `{s16 x, s16 y, u8 w, u8 h}` record via a `sub_80083B8(part)`-derived
 * "info" struct rather than `part`'s own keyframe table pointer:
 * `info+4` points to a byte whose upper nibble (0-15, but only 0-6
 * handled - anything above 6 and unhandled 1/2/6 fall through to the
 * same default) selects one of `info+0x14`, `info+0xc`, or the fixed
 * fallback table `gStaticData_0816B2F8`. */
void *sub_8007C30(void *dest, void *pt)
{
    register void *part asm("r6") = pt;
    struct aabb buf_;
    void *info;
    void *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = sub_80083B8(part);
    type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    switch (type) {
    case 0:
    case 3:
    case 4:
        rec = (u8 *)info + 0x14;
        break;
    case 1:
    case 2:
    case 6:
        rec = gStaticData_0816B2F8;
        break;
    case 5:
        rec = (u8 *)info + 0xc;
        break;
    default:
        rec = gStaticData_0816B2F8;
        break;
    }

    x = *(s32 *)part >> 8;
    offX = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)part + 4) >> 8;
    offY = *(s16 *)((u8 *)rec + 2);
    w = *((u8 *)rec + 4);
    h = *((u8 *)rec + 5);

    offX = offX + x;
    offY = offY + y;
    sub_803AFE4(&buf_, offX, offY);
    sub_803AFDC(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        register s32 flags asm("r1");
        register s32 shifted asm("r0");

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.field_0 = (*(s32 *)part >> 8) * 2 - (buf_.field_0 + buf_.field_8);
        }
        {
            register s32 addr asm("r3") = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r" (shifted), "+r" (addr));
        }
        if (shifted < 0) {
            buf_.field_4 = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf_.field_4 + buf_.field_c);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

/* Same shape as sub_8007C30 above, with a simpler switch: only
 * `info+0xc` or the `gStaticData_0816B2F8` fallback are ever selected
 * (cases 0/2/3/4/6 to `info+0xc`; cases 1/5 and the out-of-range
 * default all to the fallback). */
void *sub_8007CF8(void *dest, void *pt)
{
    register void *part asm("r6") = pt;
    struct aabb buf_;
    void *info;
    void *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = sub_80083B8(part);
    type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    switch (type) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
        rec = (u8 *)info + 0xc;
        break;
    case 1:
    case 5:
        rec = gStaticData_0816B2F8;
        break;
    default:
        rec = gStaticData_0816B2F8;
        break;
    }

    x = *(s32 *)part >> 8;
    offX = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)part + 4) >> 8;
    offY = *(s16 *)((u8 *)rec + 2);
    w = *((u8 *)rec + 4);
    h = *((u8 *)rec + 5);

    offX = offX + x;
    offY = offY + y;
    sub_803AFE4(&buf_, offX, offY);
    sub_803AFDC(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        register s32 flags asm("r1");
        register s32 shifted asm("r0");

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.field_0 = (*(s32 *)part >> 8) * 2 - (buf_.field_0 + buf_.field_8);
        }
        {
            register s32 addr asm("r3") = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r" (shifted), "+r" (addr));
        }
        if (shifted < 0) {
            buf_.field_4 = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf_.field_4 + buf_.field_c);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

extern void *sub_8007B98(void *dest, void *part);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void *sub_8025BAC(void *pool, s32 arg1, s32 kind, s32 x, s32 y, s32 arg5);
extern struct actor *gUnknown_030012D8;
extern void *gUnknown_030012E4;
extern void *gUnknown_030012B4;

/* `part` (a `struct actor`, same layout used throughout this ROM
 * region) collides with the player (`gUnknown_030012D8`, tested via
 * two `sub_8007B98` AABBs and `sub_8001688`) and, if so, plays a sound
 * at the player's position (the `table+0x68` offset/dead-read idiom
 * matches sub_8007048's `sub_803AD88` call exactly, just keyed off
 * `part->field_0A` instead of `self->field_0A`) and marks itself
 * "collected" (`gUnknown_030012B4` bitmap, same convention as
 * sub_80072D8). `part->field_0A - 0x1b` (0-7) then selects a "kind" to
 * spawn via `sub_8025BAC` at `part`'s own position - case 1 and any
 * out-of-range value spawn nothing. If something spawned, its
 * `+0x28`/`+0xc` flag bytes get tagged - kept as raw offsets since the
 * spawned object's own type isn't established yet.
 *
 * Written as NAKED asm, not plain C: every instruction's operation,
 * operand, and order was already confirmed against the ROM by the
 * earlier plain-C reconstruction (the paragraph above is that
 * derivation) - the only gap was one systematic register choice, the
 * cached address of the `gUnknown_030012D8` global landing in r6
 * instead of the ROM's r7, cascading into nearly every later register
 * number since it's read from repeatedly across several basic blocks
 * (both collision-AABB calls, the sound-position lookup). This is
 * this project's well-documented "explicit `register T x asm("r7")`
 * pin compiles correct instructions/order but silently drops r7 from
 * the prologue/epilogue push/pop list" gcc-2.9 bug - and here it's
 * worse than usual: pinning the cached-address local directly to r7
 * crashes the compiler outright (`internal error--unrecognizable
 * insn`) instead of just mis-scheduling. Register-pinning archaeology
 * is a dead end for this specific function, confirmed multiple ways -
 * full NAKED transcription instead, like this project's other
 * hard-compiler-limitation cases (`src/system/link_cable.c`'s several
 * NAKED functions, `src/audio/gax_swi.c`'s `sub_80392C4`). See
 * docs/matching/naked-sub_8007dbc.md for the conversion write-up. */
NAKED void sub_8007DBC(struct actor *part)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #0x28\n\t"
        "add r5, r0, #0\n\t"
        "ldrb r0, [r5, #0xc]\n\t"
        "lsl r1, r0, #0x18\n\t"
        "lsr r0, r1, #0x1b\n\t"
        "mov r6, #1\n\t"
        "and r0, r6\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "b 24f\n\t"
    "1:\n\t"
        "lsr r0, r1, #0x1a\n\t"
        "and r0, r6\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "b 24f\n\t"
    "2:\n\t"
        "add r0, sp, #8\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_8007B98\n\t"
        "ldr r7, 6f\n\t"
        "ldr r1, [r7]\n\t"
        "ldrb r2, [r1, #0xc]\n\t"
        "lsr r0, r2, #7\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "b 24f\n\t"
    "3:\n\t"
        "add r4, sp, #0x18\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8007B98\n\t"
        "add r0, r4, #0\n\t"
        "add r1, sp, #8\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "b 24f\n\t"
    "4:\n\t"
        "mov r0, #8\n\t"
        "ldrb r3, [r5, #0xc]\n\t"
        "orr r0, r3\n\t"
        "strb r0, [r5, #0xc]\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r4, #0\n\t"
        "ldrsh r2, [r1, r4]\n\t"
        "add r0, r0, r2\n\t"
        "ldrb r2, [r5, #0xa]\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "ldrb r0, [r5, #0xc]\n\t"
        "orr r0, r6\n\t"
        "strb r0, [r5, #0xc]\n\t"
        "ldr r0, 7f\n\t"
        "ldrh r1, [r5, #8]\n\t"
        "cmp r1, r0\n\t"
        "beq 5f\n\t"
        "ldrh r3, [r5, #8]\n\t"
        "ldr r0, 8f\n\t"
        "ldr r2, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r1, r0, #2\n\t"
        "mov r4, #0x84\n\t"
        "lsl r4, r4, #1\n\t"
        "add r2, r2, r4\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r3, r0\n\t"
        "mov r1, #1\n\t"
        "lsl r1, r0\n\t"
        "ldr r0, [r2]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2]\n\t"
    "5:\n\t"
        "mov r3, #0\n\t"
        "ldrb r0, [r5, #0xa]\n\t"
        "sub r0, #0x1b\n\t"
        "cmp r0, #7\n\t"
        "bhi 23f\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, 9f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "6: .4byte gUnknown_030012D8\n"
    "7: .4byte 0x0000FFFF\n"
    "8: .4byte gUnknown_030012B4\n"
    "9: .4byte 10f\n"
    "10: .4byte 21f\n\t"
        ".4byte 23f\n\t"
        ".4byte 11f\n\t"
        ".4byte 11f\n\t"
        ".4byte 15f\n\t"
        ".4byte 19f\n\t"
        ".4byte 13f\n\t"
        ".4byte 17f\n"
    "11:\n\t"
        "ldr r3, [r5]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r1, [r5, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 12f\n\t"
        "ldr r0, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp, #4]\n\t"
        "mov r1, #0x2b\n\t"
        "mov r2, #1\n\t"
        "b 22f\n\t"
        ".align 2, 0\n"
    "12: .4byte gUnknown_030012E4\n"
    "13:\n\t"
        "ldr r3, [r5]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r1, [r5, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 14f\n\t"
        "ldr r0, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp, #4]\n\t"
        "mov r1, #0x2b\n\t"
        "mov r2, #6\n\t"
        "b 22f\n\t"
        ".align 2, 0\n"
    "14: .4byte gUnknown_030012E4\n"
    "15:\n\t"
        "ldr r3, [r5]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r1, [r5, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp, #4]\n\t"
        "mov r1, #0x2b\n\t"
        "mov r2, #5\n\t"
        "b 22f\n\t"
        ".align 2, 0\n"
    "16: .4byte gUnknown_030012E4\n"
    "17:\n\t"
        "ldr r3, [r5]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r1, [r5, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 18f\n\t"
        "ldr r0, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp, #4]\n\t"
        "mov r1, #0x2b\n\t"
        "mov r2, #0\n\t"
        "b 22f\n\t"
        ".align 2, 0\n"
    "18: .4byte gUnknown_030012E4\n"
    "19:\n\t"
        "ldr r3, [r5]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r1, [r5, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp, #4]\n\t"
        "mov r1, #0x2b\n\t"
        "mov r2, #3\n\t"
        "b 22f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012E4\n"
    "21:\n\t"
        "ldr r3, [r5]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r1, [r5, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 25f\n\t"
        "ldr r0, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp, #4]\n\t"
        "mov r1, #0x2b\n\t"
        "mov r2, #4\n\t"
    "22:\n\t"
        "bl sub_8025BAC\n\t"
        "add r3, r0, #0\n\t"
    "23:\n\t"
        "cmp r3, #0\n\t"
        "beq 24f\n\t"
        "add r2, r3, #0\n\t"
        "add r2, #0x28\n\t"
        "mov r1, #1\n\t"
        "mov r0, #4\n\t"
        "neg r0, r0\n\t"
        "ldrb r4, [r2]\n\t"
        "and r0, r4\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r2]\n\t"
        "mov r0, #5\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r3, #0xc]\n\t"
        "and r0, r1\n\t"
        "strb r0, [r3, #0xc]\n\t"
    "24:\n\t"
        "mov r0, #0\n\t"
        "add sp, #0x28\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "25: .4byte gUnknown_030012E4\n"
    );
}

