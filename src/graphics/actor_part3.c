#include "core.h"
#include "actor.h"

extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void *gUnknown_03001308;

/* Same shape as sub_8006FE4 (graphics.c) - `part+0x25 == 1` is a fast
 * "always visible" override; otherwise `part+0xd` bit 2 gates an
 * on-screen check via `sub_803AD80`, using a 4-word "region" of
 * `{gUnknown_03001308's sub-object's two Q8 fields, 240<<8, 160<<8}`
 * (the GBA's screen width/height) and the same
 * `table+N`/`table+N+4` offset/pointer slot pair convention
 * sub_8006FE4 reads at `table+0x40`, here at `table+0x30`. */
s32 sub_8007F78(struct actor *part)
{
    register s32 result asm("r3") = 0;

    if (*((u8 *)part + 0x25) == 1) {
        return 1;
    }

    {
        register u32 flags asm("r1");
        register s32 bit2 asm("r0");

        flags = *((u8 *)part + 0xd);
        bit2 = (flags >> 2) & 1;
        if (!bit2) {
            s32 buf[4];
            register void *subObj asm("r0");
            void *table;

            subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
            {
                s32 field0 = *(s32 *)subObj << 8;
                s32 field4 = *(s32 *)((u8 *)subObj + 4) << 8;

                buf[0] = field0;
                buf[1] = field4;
            }
            {
                s32 width = 0xf0 << 8;
                s32 height = 0xa0 << 8;

                buf[2] = width;
                buf[3] = height;
            }

            table = part->table;
            result = (u8)sub_803AD80((u8 *)part + *(s16 *)((u8 *)table + 0x30), buf, *(void **)((u8 *)table + 0x34));
        }
    }
    return result;
}

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void sub_8007B00(void *dest, void *part);

/* Same `part+0x25`/`part+0xd` bit-2 fast-path shape as sub_8007F78
 * above, but the real check is an AABB-overlap test: `part`'s own box
 * (via the parked sub_8007B00) against `region`'s `{s32 x, y, w, h}`
 * (kept raw - `region`'s own type isn't established). */
s32 sub_8007FD8(struct actor *part, void *region)
{
    register s32 earlyResult asm("r3") = 0;

    if (*((u8 *)part + 0x25) == 1) {
        return 1;
    }

    {
        register u32 flags asm("r1");
        register s32 bit2 asm("r0");

        flags = *((u8 *)part + 0xd);
        bit2 = (flags >> 2) & 1;
        if (bit2) {
            return earlyResult;
        }
    }

    {
        struct aabb box;
        s32 x1, y1, x2, y2;
        s32 result;

        sub_8007B00(&box, part);

        x1 = box.field_0 << 8;
        y1 = box.field_4 << 8;
        x2 = x1 + (box.field_8 << 8);
        y2 = y1 + (box.field_c << 8);

        result = 0;
        if (x2 > *(s32 *)region) {
            if (x1 < *(s32 *)region + *(s32 *)((u8 *)region + 8)) {
                if (y2 > *(s32 *)((u8 *)region + 4)) {
                    if (y1 < *(s32 *)((u8 *)region + 4) + *(s32 *)((u8 *)region + 0xc)) {
                        result = 1;
                    }
                }
            }
        }
        earlyResult = result;
        return earlyResult;
    }
}
asm(".align 2, 0");

/* Advances `part`'s per-keyframe animation timer by one tick, only
 * when `part+0x2c` is nonzero. `part+0x34` counts up each tick against
 * the current keyframe record's `+0x15` duration; once it reaches that
 * duration, `part+0x34` resets and `part+0x30` (the frame index)
 * advances. If `part+0x30` then reaches the record's `+0x16` frame
 * count, both counters reset and - unless the record's `+0x17` flags
 * byte has bit 1 set (a "loop" flag, working theory) - `part+0x38`
 * gets marked done. Record fields kept raw, same convention as the
 * keyframe tables read elsewhere in this ROM region. See the (now
 * removed) NON_MATCHING C draft in git history for the full commented
 * C reconstruction. Written as NAKED asm here instead: the ROM keeps
 * `part` itself in `ip` for the whole function (never a normal
 * callee-saved register) and shares its keyframe-table pointer/index
 * address across both halves, which every C-level attempt to reproduce
 * (an outer-scope local, a register pin, or both) instead made gcc stop
 * using `ip` for `part` altogether - see docs/matching.md's "Parked,
 * not matched: sub_8008044" for the full account. A transcription of
 * the ROM's own confirmed-correct instructions, same technique as
 * `sub_8006600`/`sub_80073DC`/`sub_8007B00` above and this project's
 * other hard-compiler-limitation cases (see src/system/link_cable.c/
 * src/audio/gax_swi.c). */
NAKED void sub_8008044(struct actor *part)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "mov ip, r0\n\t"
        "add r0, #0x2c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "mov r0, ip\n\t"
        "ldr r4, [r0, #0x34]\n\t"
        "ldr r3, [r0, #0x20]\n\t"
        "mov r1, ip\n\t"
        "add r1, #0x2d\n\t"
        "ldr r2, [r3]\n\t"
        "ldrb r5, [r1]\n\t"
        "lsl r0, r5, #3\n\t"
        "sub r0, r0, r5\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r2\n\t"
        "add r2, r1, #0\n\t"
        "ldrb r0, [r0, #0x15]\n\t"
        "cmp r4, r0\n\t"
        "bge 1f\n\t"
        "add r0, r4, #1\n\t"
        "mov r1, ip\n\t"
        "str r0, [r1, #0x34]\n\t"
        "b 2f\n\t"
    "1:\n\t"
        "mov r0, #0\n\t"
        "mov r4, ip\n\t"
        "str r0, [r4, #0x34]\n\t"
        "ldr r0, [r4, #0x30]\n\t"
        "add r0, #1\n\t"
        "str r0, [r4, #0x30]\n\t"
    "2:\n\t"
        "mov r5, ip\n\t"
        "ldr r1, [r5, #0x30]\n\t"
        "ldr r3, [r3]\n\t"
        "ldrb r4, [r2]\n\t"
        "lsl r0, r4, #3\n\t"
        "sub r0, r0, r4\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r3\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "cmp r1, r0\n\t"
        "blt 3f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r5, #0x30]\n\t"
        "str r0, [r5, #0x34]\n\t"
        "ldrb r5, [r2]\n\t"
        "lsl r0, r5, #3\n\t"
        "sub r0, r0, r5\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r3\n\t"
        "mov r1, #2\n\t"
        "ldrb r0, [r0, #0x17]\n\t"
        "and r1, r0\n\t"
        "cmp r1, #0\n\t"
        "bne 3f\n\t"
        "mov r1, #1\n\t"
        "mov r0, ip\n\t"
        "add r0, #0x38\n\t"
        "strb r1, [r0]\n\t"
    "3:\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
