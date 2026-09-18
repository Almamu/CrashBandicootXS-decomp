#include "core.h"
#include "actor.h"

extern void sub_8007174(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);
extern void sub_80073DC(void *unused, void *part, s32 *posPtr);
extern void sub_8026ED0(void *arg0);

/* `part+0x25` selects whether (x, y) are already screen-relative
 * (nonzero - used as-is) or need the camera-relative conversion
 * sub_8007174 applies (zero - the common case). Either way, the
 * resolved {x, y} pair is forwarded to sub_80073DC (parked as
 * NON_MATCHING in src/graphics/graphics.c) to build/queue this part's
 * OAM entries. */
void sub_8007A48(void *self, void *part, s32 x, s32 y)
{
    s32 pos[2];

    if (*((u8 *)part + 0x25) == 0) {
        sub_8007174(part, x, y, &pos[0], &pos[1]);
    } else {
        pos[0] = x;
        pos[1] = y;
    }
    sub_80073DC(self, part, pos);
}
asm(".align 2, 0");

/* `part`'s own leading {x, y} pair (the same Q8 fixed-point position
 * fields struct actor has at 0x00/0x04) becomes the explicit position
 * passed to sub_8007A48 - confirms `part` embeds a struct-actor-shaped
 * position at its own start. */
void sub_8007A84(void *self, void *part)
{
    sub_8007A48(self, part, *(s32 *)part >> 8, *(s32 *)((u8 *)part + 4) >> 8);
}
asm(".align 2, 0");

void sub_8007A98(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        sub_8026ED0(arg0);
    }
}
asm(".align 2, 0");

void nullsub_2(void)
{
}
asm(".align 2, 0");

/* Initializes/clears several `part`-object fields also seen used in
 * sub_80073DC/sub_8007634: 0x20/0x30/0x34 (position-interpolation
 * state), 0x28-0x29 (the flags byte pair packed into attr1/attr2),
 * 0x2d (keyframe counter), 0x3c (Q8 "scale" factor), and 0x25 (the
 * screen-vs-camera-relative flag sub_8007A48 tests). `part+0xd` is a
 * second, separate flags byte from `part+0xc`.
 *
 * Register pins throughout match the ROM's own choices for the two
 * bit-clear sequences (constant computed before the byte load, result
 * landing in the constant's own register - the same accumulator
 * pattern documented at length for `struct actor`'s flags field in
 * graphics.c) and for the final `0x2c` store (the ROM computes that
 * address into a *fresh* register rather than reusing `part`'s, even
 * though `part` is dead afterward - plain C let the allocator reuse
 * it instead). The running `p` pointer (advanced by `+8` then `+0xb`
 * rather than recomputed from `part` each time) and the shared `zero`
 * local (reused across differently-sized stores instead of
 * rematerializing the constant) both mirror the ROM's own address/
 * value reuse - see docs/matching.md, "Matching decompilation". */
void sub_8007AB4(void *arg0)
{
    register void *part asm("r3") = arg0;
    register s32 result asm("r0");
    register s32 tmp asm("r1");

    result = 0x7f;
    tmp = *((u8 *)part + 0xc);
    result &= tmp;
    tmp = -0x41;
    result &= tmp;
    *((u8 *)part + 0xc) = result;

    {
        u8 *p = (u8 *)part + 0x25;
        s32 zero = 0;
        *p = zero;
        *(s32 *)((u8 *)part + 0x20) = zero;
        p += 8;
        *p = zero;
        *(s32 *)((u8 *)part + 0x30) = zero;
        *(s32 *)((u8 *)part + 0x34) = zero;
        *(u16 *)((u8 *)part + 0x28) = zero;
        p += 0xb;
        *p = zero;
    }

    {
        register s32 result2 asm("r0");
        register s32 tmp2 asm("r4");

        result2 = -5;
        tmp2 = *((u8 *)part + 0xd);
        result2 &= tmp2;
        *((u8 *)part + 0xd) = result2;
    }

    *((u8 *)part + 0x24) = 0;
    *(u16 *)((u8 *)part + 0x3c) = 0;
    {
        register u8 *p2 asm("r1") = (u8 *)part + 0x2c;
        *p2 = 1;
    }
}

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);

/* Confirmed by matching sub_8007B00 below (even though that function
 * itself is parked, this shape isn't in doubt - every field read
 * compiled byte-identically to the ROM). */
struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

/* Builds an AABB (via the shared sub_803AFE4/sub_803AFDC primitive -
 * see docs/matching.md) for `part`'s current animation keyframe: the
 * keyframe table pointer at `part+0x20` indexed by the counter at
 * `part+0x2d` (0x1c bytes per record), whose own `+0xc`/`+0xe`/`+0x10`/
 * `+0x11` fields are {s16 xOffset, s16 yOffset, u8 w, u8 h} - the same
 * offset/text-table convention seen elsewhere, just with the AABB
 * dimensions instead of a text pointer. `part+0x28` bits 4/5 mirror
 * the AABB horizontally/vertically around `part`'s own position. See
 * the (now removed) NON_MATCHING C draft in git history for the full
 * commented C reconstruction. Written as NAKED asm here instead: `part`
 * needed r7 (this compiler's plain, unpinned allocator put it in r6
 * every attempt), and an explicit `register void *part asm("r7")` pin
 * is categorically unsafe in this toolchain (see
 * `matching_decomp_register_pinning` memory, point 10 - never makes it
 * into the compiled prologue's push list). A transcription of the ROM's
 * own confirmed-correct instructions, same technique as
 * `sub_8006600`/`sub_80073DC` above and this project's other hard-
 * compiler-limitation cases (see src/system/link_cable.c/
 * src/audio/gax_swi.c). */
NAKED void sub_8007B00(void *dest, void *part)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #0x10\n\t"
        "mov r8, r0\n\t"
        "add r7, r1, #0\n\t"
        "ldr r1, [r7, #0x20]\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x2d\n\t"
        "ldrb r3, [r2]\n\t"
        "lsl r0, r3, #3\n\t"
        "sub r0, r0, r3\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, [r1]\n\t"
        "add r1, r1, r0\n\t"
        "add r3, r1, #0\n\t"
        "add r3, #0xc\n\t"
        "ldr r4, [r7]\n\t"
        "asr r4, r4, #8\n\t"
        "mov r5, #0xc\n\t"
        "ldrsh r1, [r1, r5]\n\t"
        "ldr r0, [r7, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "mov r5, #2\n\t"
        "ldrsh r2, [r3, r5]\n\t"
        "ldrb r5, [r3, #4]\n\t"
        "ldrb r6, [r3, #5]\n\t"
        "add r1, r1, r4\n\t"
        "add r2, r2, r0\n\t"
        "mov r0, sp\n\t"
        "bl sub_803AFE4\n\t"
        "mov r0, sp\n\t"
        "add r1, r5, #0\n\t"
        "add r2, r6, #0\n\t"
        "bl sub_803AFDC\n\t"
        "add r3, r7, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r1, [r3]\n\t"
        "lsl r0, r1, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 1f\n\t"
        "ldr r0, [r7]\n\t"
        "asr r0, r0, #8\n\t"
        "lsl r0, r0, #1\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r2, [sp, #8]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp]\n\t"
    "1:\n\t"
        "ldrb r3, [r3]\n\t"
        "lsl r0, r3, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 2f\n\t"
        "ldr r0, [r7, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "lsl r0, r0, #1\n\t"
        "ldr r1, [sp, #4]\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #4]\n\t"
    "2:\n\t"
        "mov r0, r8\n\t"
        "mov r1, sp\n\t"
        "ldm r1!, {r2, r3, r4}\n\t"
        "stm r0!, {r2, r3, r4}\n\t"
        "ldr r1, [r1]\n\t"
        "str r1, [r0]\n\t"
        "mov r0, r8\n\t"
        "add sp, #0x10\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}

/* Same AABB-for-keyframe shape as sub_8007B00 above, for a second,
 * differently-laid-out keyframe table (offX/offY/w/h sit at rec+4/+6/+8/+9
 * here, not rec+0xc/+0xe/+0x10/+0x11) - reusing the shared `struct aabb`.
 * Also returns `dest` back to the caller (the ROM reloads r8 into r0
 * right before the epilogue), unlike sub_8007B00 which is void. See the
 * (now removed) NON_MATCHING C draft in git history for the full
 * commented C reconstruction - every instruction's operation, operand,
 * and order matched the ROM exactly except a recurring "which anonymous
 * scratch register" choice (about 10 of this function's ~73
 * instructions), which no C-level rephrasing closed (see
 * docs/matching.md's "Parked, not matched: sub_8007B98" for the full
 * account of what was tried). Written as NAKED asm here instead, same
 * technique as `sub_8007B00` above. */
NAKED void *sub_8007B98(void *dest, void *part)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #0x10\n\t"
        "mov r8, r0\n\t"
        "add r7, r1, #0\n\t"
        "ldr r1, [r7, #0x20]\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x2d\n\t"
        "ldrb r3, [r2]\n\t"
        "lsl r0, r3, #3\n\t"
        "sub r0, r0, r3\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, [r1]\n\t"
        "add r1, r1, r0\n\t"
        "add r3, r1, #4\n\t"
        "ldr r4, [r7]\n\t"
        "asr r4, r4, #8\n\t"
        "mov r5, #4\n\t"
        "ldrsh r1, [r1, r5]\n\t"
        "ldr r0, [r7, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "mov r5, #2\n\t"
        "ldrsh r2, [r3, r5]\n\t"
        "ldrb r5, [r3, #4]\n\t"
        "ldrb r6, [r3, #5]\n\t"
        "add r1, r1, r4\n\t"
        "add r2, r2, r0\n\t"
        "mov r0, sp\n\t"
        "bl sub_803AFE4\n\t"
        "mov r0, sp\n\t"
        "add r1, r5, #0\n\t"
        "add r2, r6, #0\n\t"
        "bl sub_803AFDC\n\t"
        "add r3, r7, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r1, [r3]\n\t"
        "lsl r0, r1, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 1f\n\t"
        "ldr r0, [r7]\n\t"
        "asr r0, r0, #8\n\t"
        "lsl r0, r0, #1\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r2, [sp, #8]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp]\n\t"
    "1:\n\t"
        "ldrb r3, [r3]\n\t"
        "lsl r0, r3, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 2f\n\t"
        "ldr r0, [r7, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "lsl r0, r0, #1\n\t"
        "ldr r1, [sp, #4]\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #4]\n\t"
    "2:\n\t"
        "mov r0, r8\n\t"
        "mov r1, sp\n\t"
        "ldm r1!, {r2, r3, r4}\n\t"
        "stm r0!, {r2, r3, r4}\n\t"
        "ldr r1, [r1]\n\t"
        "str r1, [r0]\n\t"
        "mov r0, r8\n\t"
        "add sp, #0x10\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}
asm(".align 2, 0");
