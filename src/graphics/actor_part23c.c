#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Position-easing helper, called from `sub_8030734`/`sub_8030834`
 * (actor_part21d.c/actor_part21e.c): advances the position
 * accumulators (`gUnknown_03001540`/`gUnknown_03001544`) by their
 * per-frame deltas (`gUnknown_03001558`/`gUnknown_0300155C`), then
 * computes the player's (`gUnknown_03000884`) signed distance from a
 * fixed keyframe-table-relative target point on each axis
 * (`self+0x1c`/`0x20` against `gUnknown_0300154C`/`gUnknown_03001550`
 * offset by `gStaticData_0817C3D8`'s box) and, per axis, nudges a
 * "shake"/camera-offset accumulator (`gUnknown_0300154C`/
 * `gUnknown_03001550`, via `ip`/`r8`) toward the target in small
 * discrete steps once the distance exceeds a `0x2CFF` threshold and a
 * finer `>>10` sub-threshold. Also clamps both accumulators against a
 * set of fixed ranges/bias points (`0xa000`/`0x4FFF`, `0xFFFFD300`/
 * `0x13FF`) and a final `0x180`/`-0x180`, `0x100`/`-0x100` hard clamp.
 *
 * Semantics are understood at the level above, but this is transcribed
 * as NAKED asm: the two accumulator addresses (`sb`/`r8`) and the
 * player-position pointer/table (`sl`, `ip`) all stay live across the
 * whole function's per-axis branching, the same many-high-register
 * allocation gcc-2.9 difficulty documented throughout this project.
 * Mechanical, byte-verified transcription. */
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001558;
extern s32 gUnknown_03001544;
extern s32 gUnknown_0300155C;
extern struct actor_self *gUnknown_03000884;
extern s32 gUnknown_0300154C;
extern const s16 gStaticData_0817C3D8[];
extern s32 gUnknown_03001550;

#if NON_MATCHING
/* Near miss (identical under both compilers): 19 halfwords off, all of
 * it the `&gUnknown_03001558`/`&gUnknown_0300155C` address copies
 * landing in `r4`/`r6` swapped - every instruction, branch and literal
 * is otherwise the ROM's. */
static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

static inline s32 CamX(void) { return gUnknown_0300154C - 0x1200; }
static inline s32 CamY(void) { return gUnknown_03001550 + 0x1800; }

static inline s32 ClampHi(s32 *p, s32 lim)
{
    s32 v = *p;
    if (v > lim)
        v = lim;
    *p = v;
    return v;
}

void sub_8030E08(void)
{
    s32 vx;
    s32 dx, dy, cx, cy, px, py;
    struct actor_self *pl;

    gUnknown_03001540 += gUnknown_03001558;
    gUnknown_03001544 += gUnknown_0300155C;

    pl = gUnknown_03000884;
    px = pl->x;
    cx = gUnknown_0300154C - 0x1200;
    dx = px - cx - (gStaticData_0817C3D8[0] + gStaticData_0817C3D8[3] / 2);
    py = pl->y;
    cy = gUnknown_03001550 + 0x1800;
    dy = py - cy - (gStaticData_0817C3D8[1] + gStaticData_0817C3D8[4] / 2);

    if (Abs(dx) <= 0x2CFF) {
        s32 s = dx >> 10;
        vx = gUnknown_03001558;
        if (s >= 0) {
            gUnknown_03001558 = vx;
            if (s != 0)
                gUnknown_03001558 = vx - 3;
        } else
            gUnknown_03001558 = vx + 3;
    }
    if (Abs(dy) <= 0x2CFF) {
        s32 v;
        if ((dy >> 10) >= 0) {
            v = gUnknown_0300155C;
            if ((dy >> 10) == 0)
                goto skip;
            v -= 2;
        } else
            v = gUnknown_0300155C + 2;
        gUnknown_0300155C = v;
    }
skip:

    if (gUnknown_0300154C <= 0x1400)
        gUnknown_03001558 += 6;
    if (gUnknown_0300154C > 0x4FFF)
        gUnknown_03001558 -= 6;
    if (gUnknown_03001550 <= -0x2D00)
        gUnknown_0300155C += 3;
    if (gUnknown_03001550 > 0x13FF)
        gUnknown_0300155C -= 3;

    {
        s32 *p = &gUnknown_03001558;
        s32 v = *p;
        if (v > 0x180)
            v = 0x180;
        *p = v;
        if (v < -0x180)
            v = -0x180;
        gUnknown_03001558 = v;
    }
    {
        s32 *p = &gUnknown_0300155C;
        s32 v = *p;
        if (v > 0x100)
            v = 0x100;
        *p = v;
        if (v < -0x100)
            v = -0x100;
        gUnknown_0300155C = v;
    }
}
#else
NAKED void sub_8030E08(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "ldr r1, 1f\n\t"
        "ldr r0, 2f\n\t"
        "mov sb, r0\n\t"
        "ldr r0, [r1]\n\t"
        "mov r2, sb\n\t"
        "ldr r2, [r2]\n\t"
        "mov ip, r2\n\t"
        "add r0, ip\n\t"
        "str r0, [r1]\n\t"
        "ldr r2, 3f\n\t"
        "ldr r3, 4f\n\t"
        "mov r8, r3\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r1, [r3]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r2]\n\t"
        "ldr r0, 5f\n\t"
        "ldr r5, [r0]\n\t"
        "ldr r3, [r5, #0x1c]\n\t"
        "ldr r0, 6f\n\t"
        "mov sl, r0\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, 7f\n\t"
        "add r0, r0, r1\n\t"
        "sub r3, r3, r0\n\t"
        "ldr r4, 8f\n\t"
        "mov r0, #0\n\t"
        "ldrsh r2, [r4, r0]\n\t"
        "mov r1, #6\n\t"
        "ldrsh r0, [r4, r1]\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "add r2, r2, r0\n\t"
        "sub r7, r3, r2\n\t"
        "ldr r3, [r5, #0x20]\n\t"
        "ldr r5, 9f\n\t"
        "ldr r0, [r5]\n\t"
        "mov r2, #0xc0\n\t"
        "lsl r2, r2, #5\n\t"
        "add r0, r0, r2\n\t"
        "sub r3, r3, r0\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r4, r0]\n\t"
        "mov r1, #8\n\t"
        "ldrsh r0, [r4, r1]\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "add r2, r2, r0\n\t"
        "sub r3, r3, r2\n\t"
        "asr r1, r7, #0x1f\n\t"
        "add r0, r7, #0\n\t"
        "eor r0, r1\n\t"
        "sub r0, r0, r1\n\t"
        "ldr r1, 10f\n\t"
        "mov r6, sb\n\t"
        "mov r4, r8\n\t"
        "mov r2, sl\n\t"
        "cmp r0, r1\n\t"
        "bgt 12f\n\t"
        "asr r0, r7, #0xa\n\t"
        "cmp r0, #0\n\t"
        "blt 11f\n\t"
        "mov r1, ip\n\t"
        "str r1, [r6]\n\t"
        "cmp r0, #0\n\t"
        "beq 12f\n\t"
        "mov r0, ip\n\t"
        "sub r0, #3\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001540\n"
    "2: .4byte gUnknown_03001558\n"
    "3: .4byte gUnknown_03001544\n"
    "4: .4byte gUnknown_0300155C\n"
    "5: .4byte gUnknown_03000884\n"
    "6: .4byte gUnknown_0300154C\n"
    "7: .4byte 0xFFFFEE00\n"
    "8: .4byte gStaticData_0817C3D8\n"
    "9: .4byte gUnknown_03001550\n"
    "10: .4byte 0x00002CFF\n"
    "11:\n\t"
        "mov r0, ip\n\t"
        "add r0, #3\n\t"
    "13:\n\t"
        "str r0, [r6]\n\t"
    "12:\n\t"
        "asr r0, r3, #0x1f\n\t"
        "add r1, r3, #0\n\t"
        "eor r1, r0\n\t"
        "sub r1, r1, r0\n\t"
        "ldr r0, 14f\n\t"
        "cmp r1, r0\n\t"
        "bgt 17f\n\t"
        "asr r1, r3, #0xa\n\t"
        "cmp r1, #0\n\t"
        "blt 15f\n\t"
        "ldr r0, [r4]\n\t"
        "cmp r1, #0\n\t"
        "beq 17f\n\t"
        "sub r0, #2\n\t"
        "b 16f\n\t"
        ".align 2, 0\n"
    "14: .4byte 0x00002CFF\n"
    "15:\n\t"
        "ldr r0, [r4]\n\t"
        "add r0, #2\n\t"
    "16:\n\t"
        "str r0, [r4]\n\t"
    "17:\n\t"
        "ldr r1, [r2]\n\t"
        "mov r0, #0xa0\n\t"
        "lsl r0, r0, #5\n\t"
        "cmp r1, r0\n\t"
        "bgt 18f\n\t"
        "ldr r0, [r6]\n\t"
        "add r0, #6\n\t"
        "str r0, [r6]\n\t"
    "18:\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, 25f\n\t"
        "cmp r1, r0\n\t"
        "ble 19f\n\t"
        "ldr r0, [r6]\n\t"
        "sub r0, #6\n\t"
        "str r0, [r6]\n\t"
    "19:\n\t"
        "ldr r1, [r5]\n\t"
        "ldr r0, 26f\n\t"
        "cmp r1, r0\n\t"
        "bgt 20f\n\t"
        "ldr r0, [r4]\n\t"
        "add r0, #3\n\t"
        "str r0, [r4]\n\t"
    "20:\n\t"
        "ldr r1, [r5]\n\t"
        "ldr r0, 27f\n\t"
        "cmp r1, r0\n\t"
        "ble 21f\n\t"
        "ldr r0, [r4]\n\t"
        "sub r0, #3\n\t"
        "str r0, [r4]\n\t"
    "21:\n\t"
        "add r2, r6, #0\n\t"
        "ldr r0, [r2]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #1\n\t"
        "cmp r0, r1\n\t"
        "ble 22f\n\t"
        "add r0, r1, #0\n\t"
    "22:\n\t"
        "str r0, [r2]\n\t"
        "ldr r1, 28f\n\t"
        "cmp r0, r1\n\t"
        "bge 23f\n\t"
        "add r0, r1, #0\n\t"
    "23:\n\t"
        "str r0, [r6]\n\t"
        "add r2, r4, #0\n\t"
        "ldr r0, [r2]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #1\n\t"
        "cmp r0, r1\n\t"
        "ble 24f\n\t"
        "add r0, r1, #0\n\t"
    "24:\n\t"
        "str r0, [r2]\n\t"
        "ldr r1, 29f\n\t"
        "cmp r0, r1\n\t"
        "bge 30f\n\t"
        "add r0, r1, #0\n\t"
    "30:\n\t"
        "str r0, [r4]\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "25: .4byte 0x00004FFF\n"
    "26: .4byte 0xFFFFD300\n"
    "27: .4byte 0x000013FF\n"
    "28: .4byte 0xFFFFFE80\n"
    "29: .4byte 0xFFFFFF00\n"
    );
}
#endif
