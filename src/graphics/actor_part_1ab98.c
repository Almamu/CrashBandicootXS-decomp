#include "core.h"
#include "gobj_1a794.h"

/* GitHub issue #25, ROM 0x0801AB98-0x0801B208: sub_801AB98, the
 * player-vs-object collision resolver (see include/gobj_1a794.h and
 * docs/matching/issue-25-level-objects.md for what it computes).
 *
 * Parked: NAKED transcription of the ROM (byte-correct, not decompiled
 * C). The C reconstruction below (under NON_MATCHING) reproduces the
 * control flow, the 0x44-byte frame and every stack-slot assignment, but
 * reload's round-robin scratch-register choice (last_spill_reg) is two
 * steps out of phase with the ROM's from the first constant store on -
 * the ROM loads the anim-record tag byte through a reload register (r3),
 * which this source's equivalent load does not need - so most
 * `movs rN, #c; str rN, [sp, #x]` pairs and hi-register base copies land
 * in different low registers. */
#if NON_MATCHING

void sub_801AB98(struct gobj *selfArg, void *unused)
{
    struct gobj *self = selfArg;
    struct aabb a;
    struct aabb b;
    struct pos2 pos;
    struct pos2 *pp;
    s32 ox;
    s32 oy;
    s32 hdir;
    s32 vdir;
    s32 side;
    s32 above;
    register s32 px asm("r5");
    s32 py;
    s32 tx;
    s32 ty;
    struct anim_box *box;
    s32 result;
    s32 flags;

    sub_8007B98(&a, self);
    {
        register s32 t asm("r0") = gUnknown_030012D8->x;

        px = t >> 8;
    }
    py = gUnknown_030012D8->y >> 8;
    sub_8007B98(&b, gUnknown_030012D8);
    {
        struct gobj *q = gUnknown_030012D8;
        struct anim_table *anim = q->anim;
        register u32 tag asm("r3") = q->tag;

        box = (struct anim_box *)&anim->records[tag].offX;
    }
    if (sub_8001688(&a, &b))
    {
        result = 0;
        above = 0;
        if (b.y < a.y)
            above = 1;
        tx = sub_8009EC4(gUnknown_030012D8);
        ty = sub_8009EBC(gUnknown_030012D8);
        side = 2;
        if (px > tx)
            side = 1;
        if ((gUnknown_030012D8->x >> 8) < (self->x >> 8))
        {
            hdir = 1;
            ox = b.x + b.w - a.x + 1;
        }
        else
        {
            hdir = 2;
            ox = a.x + a.w - b.x + 1;
        }
        if ((gUnknown_030012D8->y >> 8) > (self->y >> 8))
        {
            vdir = 4;
            oy = a.y + a.h - b.y;
        }
        else
        {
            vdir = 8;
            oy = b.y + b.h - a.y;
        }
        if (self->type != 1 && self->type != 5 && self->type != 6)
        {
            if (ty == py)
            {
                if (tx == px)
                {
                    result = hdir;
                    if (oy <= 2)
                        result = 8;
                    goto classified;
                }
                if (sub_8009EBC(self) == (self->y >> 8) && oy > 2)
                {
                    result = hdir;
                    goto classified;
                }
            }
            if (tx == px && sub_8009EC4(self) == (self->x >> 8))
                result = vdir;
        }
    classified:
        if (ty <= py && above)
        {
            if (result == 0)
            {
                ty += box->offY + box->padY;
                if (ty > a.y + a.h)
                    result = hdir;
                else
                {
                    s32 r;

                    if (side == 1)
                    {
                        if (b.x + b.w >= a.x && ox > 2)
                            result = 8;
                    }
                    else
                    {
                        if (b.x <= a.x + a.w && ox > 2)
                            result = 8;
                    }
                    if (result == 0)
                    {
                        py += box->offY + box->padY;
                        if (hdir == 1)
                        {
                            tx += box->offX + box->padX;
                            r = sub_800FDC8(tx, ty, b.x + b.w, py, a.x);
                        }
                        else
                        {
                            tx += box->offX;
                            r = sub_800FDC8(tx, ty, b.x, py, a.x + a.w);
                        }
                        if ((r < 0 && above && oy <= 1) || (r > 0 && r <= a.y))
                            result = 8;
                        else
                            result = hdir;
                    }
                }
            }
        }
        else if (result == 0)
        {
            ty += box->offY;
            if (ty < a.y)
                result = hdir;
            else
            {
                s32 r;

                if (side == 1)
                {
                    if (b.x + b.w >= a.x && ox > 3)
                        result = 4;
                }
                else
                {
                    if (b.x <= a.x + a.w && ox > 3)
                        result = 4;
                }
                if (result == 0)
                {
                    py += box->offY;
                    if (hdir == 1)
                    {
                        tx += box->offX + box->padX;
                        r = sub_800FDC8(tx, ty, b.x + b.w, py, a.x);
                    }
                    else
                    {
                        tx += box->offX;
                        r = sub_800FDC8(tx, ty, b.x, py, a.x + a.w);
                    }
                    if ((r < 0 && !above && ox > 3) || (r > 0 && r >= a.y + a.h))
                        result = 4;
                    else
                        result = hdir;
                }
            }
        }

        pos.x = gUnknown_030012D8->x;
        pp = &pos;
        pp->y = gUnknown_030012D8->y;
        if (ox < 0)
            ox = 0;
        if (oy < 0)
            oy = 0;
        flags = 0;
        switch (result)
        {
        case 4:
            {
                struct gobj *q = gUnknown_030012D8;

                if (!(q->unk_68 & 8))
                {
                    OBJ_CALL68(q, 0, 0xC, 4);
                    pp->y += oy << 8;
                }
            }
            break;
        case 8:
            pp->y = (pp->y - ((oy - 1) << 8)) & ~0xFF;
            break;
        case 1:
        case 2:
            if (hdir == 2)
                pos.x += ox << 8;
            else if (hdir == 1)
                pos.x -= ox << 8;
            break;
        }
        if (result == 8 || oy <= 1)
        {
            struct gobj *q = gUnknown_030012D8;

            if (!(q->dir & 4) && above)
            {
                q->carried = self;
                q->unk_68 = 8;
                pp->y = gUnknown_030012D8->y - ((oy - 1) << 8);
                flags = 0;
                pos.x = gUnknown_030012D8->x;
            }
        }
        sub_8007398(gUnknown_030012D8, pos.x, pp->y);
        if (flags)
        {
            OBJ_CALL68(gUnknown_030012D8, 0, 0xC, flags);
            gUnknown_030012D8->unk_74 |= flags;
        }
        if (result == 8)
        {
            s32 type = self->type;

            if (type == 1 || type == 5 || type == 6)
                self->mover->active = 1;
            else
            {
                struct gobj *q = gUnknown_030012D8;
                s32 d = (self->x >> 8) - (q->x >> 8);
                s32 sign;

                ABS32(d, sign);
                if (d <= 7)
                {
                    switch (type)
                    {
                    case 2:
                        OBJ_CALL68(q, 0, 0x11, 0);
                        break;
                    case 3:
                        if (!sub_80232A0(gUnknown_030012C0) && !((u8 *)gUnknown_030012C0)[0x8C])
                            OBJ_CALL68(gUnknown_030012D8, 0, 0xF, 0);
                        break;
                    case 4:
                        if (!sub_8023278(gUnknown_030012C0) && !((u8 *)gUnknown_030012C0)[0x8C])
                            OBJ_CALL68(gUnknown_030012D8, 0, 0x10, 0);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        a.y -= 4;
        a.h += 4;
        switch (self->type)
        {
        case 0:
        case 7:
            if (sub_8001688(&a, &b))
            {
                struct gobj *q = gUnknown_030012D8;

                q->carried = self;
                q->unk_68 = 8;
            }
            break;
        case 2:
            if (sub_8001688(&a, &b))
            {
                s32 d = (self->x >> 8) - (gUnknown_030012D8->x >> 8);
                s32 sign;

                ABS32(d, sign);
                if (d <= 7)
                    OBJ_CALL68(gUnknown_030012D8, 0, 0x11, 0);
            }
            break;
        case 3:
            if (!sub_80232A0(gUnknown_030012C0) && !((u8 *)gUnknown_030012C0)[0x8C]
                && sub_8001688(&a, &b))
            {
                s32 d = (self->x >> 8) - (gUnknown_030012D8->x >> 8);
                s32 sign;

                ABS32(d, sign);
                if (d <= 7)
                    OBJ_CALL68(gUnknown_030012D8, 0, 0xF, 0);
            }
            break;
        case 4:
            if (!sub_8023278(gUnknown_030012C0) && !((u8 *)gUnknown_030012C0)[0x8C]
                && sub_8001688(&a, &b))
            {
                s32 d = (self->x >> 8) - (gUnknown_030012D8->x >> 8);
                s32 sign;

                ABS32(d, sign);
                if (d <= 7)
                    OBJ_CALL68(gUnknown_030012D8, 0, 0x10, 0);
            }
            break;
        case 1:
        case 5:
        case 6:
            if (!sub_8001688(&a, &b))
                self->mover->active = 0;
            break;
        }
    }
}
#else
NAKED void sub_801AB98(struct gobj *selfArg, void *unused)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x44\n\t"
        "mov sb, r0\n\t"
        "add r0, sp, #4\n\t"
        "mov r1, sb\n\t"
        "bl sub_8007B98\n\t"
        "ldr r0, _0801AC3C\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "asr r5, r0, #8\n\t"
        "ldr r0, [r1, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "mov sl, r0\n\t"
        "add r4, sp, #0x14\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8007B98\n\t"
        "ldr r1, _0801AC3C\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r2, [r0, #0x20]\n\t"
        "add r0, #0x2d\n\t"
        "ldrb r3, [r0]\n\t"
        "lsl r1, r3, #3\n\t"
        "sub r1, r1, r3\n\t"
        "lsl r1, r1, #2\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, r0, r1\n\t"
        "add r7, r0, #4\n\t"
        "add r0, sp, #4\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _0801ABEA\n\t"
        "b _0801B078\n\t"
    "_0801ABEA:\n"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp, #0x40]\n\t"
        "ldr r1, [sp, #0x18]\n\t"
        "ldr r0, [sp, #8]\n\t"
        "cmp r1, r0\n\t"
        "bge _0801ABFE\n\t"
        "mov r2, #1\n\t"
        "str r2, [sp, #0x40]\n\t"
    "_0801ABFE:\n"
        "ldr r3, _0801AC3C\n\t"
        "ldr r0, [r3]\n\t"
        "bl sub_8009EC4\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, _0801AC3C\n\t"
        "ldr r0, [r1]\n\t"
        "bl sub_8009EBC\n\t"
        "add r6, r0, #0\n\t"
        "mov r2, #2\n\t"
        "str r2, [sp, #0x3c]\n\t"
        "cmp r5, r4\n\t"
        "ble _0801AC1E\n\t"
        "mov r3, #1\n\t"
        "str r3, [sp, #0x3c]\n\t"
    "_0801AC1E:\n"
        "ldr r1, _0801AC3C\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r1, [r0]\n\t"
        "asr r1, r1, #8\n\t"
        "mov r2, sb\n\t"
        "ldr r0, [r2]\n\t"
        "asr r0, r0, #8\n\t"
        "cmp r1, r0\n\t"
        "bge _0801AC40\n\t"
        "mov r3, #1\n\t"
        "str r3, [sp, #0x34]\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "ldr r1, [sp, #0x1c]\n\t"
        "ldr r2, [sp, #4]\n\t"
        "b _0801AC4A\n\t"
        ".align 2, 0\n\t"
    "_0801AC3C:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801AC40:\n"
        "mov r0, #2\n\t"
        "str r0, [sp, #0x34]\n\t"
        "ldr r0, [sp, #4]\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "ldr r2, [sp, #0x14]\n\t"
    "_0801AC4A:\n"
        "add r0, r0, r1\n\t"
        "sub r0, r0, r2\n\t"
        "add r0, #1\n\t"
        "str r0, [sp, #0x2c]\n\t"
        "ldr r0, _0801AC70\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "mov r2, sb\n\t"
        "ldr r0, [r2, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "cmp r1, r0\n\t"
        "ble _0801AC74\n\t"
        "mov r3, #4\n\t"
        "str r3, [sp, #0x38]\n\t"
        "ldr r0, [sp, #8]\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "b _0801AC7E\n\t"
        ".align 2, 0\n\t"
    "_0801AC70:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801AC74:\n"
        "mov r0, #8\n\t"
        "str r0, [sp, #0x38]\n\t"
        "ldr r0, [sp, #0x18]\n\t"
        "ldr r1, [sp, #0x20]\n\t"
        "ldr r2, [sp, #8]\n\t"
    "_0801AC7E:\n"
        "add r0, r0, r1\n\t"
        "sub r0, r0, r2\n\t"
        "str r0, [sp, #0x30]\n\t"
        "mov r1, sb\n\t"
        "ldr r0, [r1, #0x78]\n\t"
        "cmp r0, #1\n\t"
        "beq _0801ACE0\n\t"
        "cmp r0, #5\n\t"
        "beq _0801ACE0\n\t"
        "cmp r0, #6\n\t"
        "beq _0801ACE0\n\t"
        "cmp r6, sl\n\t"
        "bne _0801ACC8\n\t"
        "cmp r4, r5\n\t"
        "bne _0801ACAC\n\t"
        "ldr r2, [sp, #0x34]\n\t"
        "mov r8, r2\n\t"
        "ldr r3, [sp, #0x30]\n\t"
        "cmp r3, #2\n\t"
        "bgt _0801ACE0\n\t"
        "mov r5, #8\n\t"
        "mov r8, r5\n\t"
        "b _0801ACE0\n\t"
    "_0801ACAC:\n"
        "mov r0, sb\n\t"
        "bl sub_8009EBC\n\t"
        "mov r2, sb\n\t"
        "ldr r1, [r2, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "cmp r0, r1\n\t"
        "bne _0801ACC8\n\t"
        "ldr r3, [sp, #0x30]\n\t"
        "cmp r3, #2\n\t"
        "ble _0801ACC8\n\t"
        "ldr r5, [sp, #0x34]\n\t"
        "mov r8, r5\n\t"
        "b _0801ACE0\n\t"
    "_0801ACC8:\n"
        "cmp r4, r5\n\t"
        "bne _0801ACE0\n\t"
        "mov r0, sb\n\t"
        "bl sub_8009EC4\n\t"
        "mov r2, sb\n\t"
        "ldr r1, [r2]\n\t"
        "asr r1, r1, #8\n\t"
        "cmp r0, r1\n\t"
        "bne _0801ACE0\n\t"
        "ldr r3, [sp, #0x38]\n\t"
        "mov r8, r3\n\t"
    "_0801ACE0:\n"
        "cmp r6, sl\n\t"
        "bgt _0801ADA8\n\t"
        "ldr r5, [sp, #0x40]\n\t"
        "cmp r5, #0\n\t"
        "beq _0801ADA8\n\t"
        "mov r0, r8\n\t"
        "cmp r0, #0\n\t"
        "beq _0801ACF2\n\t"
        "b _0801AE60\n\t"
    "_0801ACF2:\n"
        "mov r1, #2\n\t"
        "ldrsh r0, [r7, r1]\n\t"
        "ldrb r2, [r7, #5]\n\t"
        "add r0, r0, r2\n\t"
        "add r6, r6, r0\n\t"
        "ldr r0, [sp, #8]\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "add r0, r0, r1\n\t"
        "add r3, r2, #0\n\t"
        "cmp r6, r0\n\t"
        "ble _0801AD0A\n\t"
        "b _0801AE5C\n\t"
    "_0801AD0A:\n"
        "ldr r2, [sp, #0x3c]\n\t"
        "cmp r2, #1\n\t"
        "bne _0801AD2A\n\t"
        "ldr r1, [sp, #0x14]\n\t"
        "ldr r0, [sp, #0x1c]\n\t"
        "add r0, r1, r0\n\t"
        "add r2, r1, #0\n\t"
        "ldr r1, [sp, #4]\n\t"
        "cmp r0, r1\n\t"
        "blt _0801AD40\n\t"
        "ldr r5, [sp, #0x2c]\n\t"
        "cmp r5, #2\n\t"
        "ble _0801AD40\n\t"
        "mov r0, #8\n\t"
        "mov r8, r0\n\t"
        "b _0801AE60\n\t"
    "_0801AD2A:\n"
        "ldr r1, [sp, #4]\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "add r0, r1, r0\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "cmp r2, r0\n\t"
        "bgt _0801AD40\n\t"
        "ldr r5, [sp, #0x2c]\n\t"
        "cmp r5, #2\n\t"
        "ble _0801AD40\n\t"
        "mov r0, #8\n\t"
        "mov r8, r0\n\t"
    "_0801AD40:\n"
        "mov r5, r8\n\t"
        "cmp r5, #0\n\t"
        "beq _0801AD48\n\t"
        "b _0801AE60\n\t"
    "_0801AD48:\n"
        "mov r5, #2\n\t"
        "ldrsh r0, [r7, r5]\n\t"
        "add r0, r0, r3\n\t"
        "add sl, r0\n\t"
        "ldr r0, [sp, #0x34]\n\t"
        "cmp r0, #1\n\t"
        "bne _0801AD6E\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r7, r3]\n\t"
        "ldrb r7, [r7, #4]\n\t"
        "add r0, r7, r0\n\t"
        "add r4, r4, r0\n\t"
        "ldr r0, [sp, #0x1c]\n\t"
        "add r5, r2, r0\n\t"
        "str r1, [sp]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r5, #0\n\t"
        "b _0801AD80\n\t"
    "_0801AD6E:\n"
        "mov r5, #0\n\t"
        "ldrsh r0, [r7, r5]\n\t"
        "add r4, r4, r0\n\t"
        "add r5, r2, #0\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "add r0, r1, r0\n\t"
        "str r0, [sp]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
    "_0801AD80:\n"
        "mov r3, sl\n\t"
        "bl sub_800FDC8\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "bge _0801AD98\n\t"
        "ldr r0, [sp, #0x40]\n\t"
        "cmp r0, #0\n\t"
        "beq _0801AD98\n\t"
        "ldr r1, [sp, #0x30]\n\t"
        "cmp r1, #1\n\t"
        "ble _0801ADA2\n\t"
    "_0801AD98:\n"
        "cmp r2, #0\n\t"
        "ble _0801AE56\n\t"
        "ldr r0, [sp, #8]\n\t"
        "cmp r2, r0\n\t"
        "bgt _0801AE56\n\t"
    "_0801ADA2:\n"
        "mov r2, #8\n\t"
        "mov r8, r2\n\t"
        "b _0801AE60\n\t"
    "_0801ADA8:\n"
        "mov r0, r8\n\t"
        "cmp r0, #0\n\t"
        "bne _0801AE60\n\t"
        "mov r1, #2\n\t"
        "ldrsh r0, [r7, r1]\n\t"
        "add r6, r6, r0\n\t"
        "ldr r0, [sp, #8]\n\t"
        "cmp r6, r0\n\t"
        "blt _0801AE5C\n\t"
        "ldr r2, [sp, #0x3c]\n\t"
        "cmp r2, #1\n\t"
        "bne _0801ADD8\n\t"
        "ldr r1, [sp, #0x14]\n\t"
        "ldr r0, [sp, #0x1c]\n\t"
        "add r0, r1, r0\n\t"
        "add r2, r1, #0\n\t"
        "ldr r1, [sp, #4]\n\t"
        "cmp r0, r1\n\t"
        "blt _0801ADEE\n\t"
        "ldr r3, [sp, #0x2c]\n\t"
        "cmp r3, #3\n\t"
        "ble _0801ADEE\n\t"
        "mov r5, #4\n\t"
        "b _0801AE5E\n\t"
    "_0801ADD8:\n"
        "ldr r1, [sp, #4]\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "add r0, r1, r0\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "cmp r2, r0\n\t"
        "bgt _0801ADEE\n\t"
        "ldr r0, [sp, #0x2c]\n\t"
        "cmp r0, #3\n\t"
        "ble _0801ADEE\n\t"
        "mov r3, #4\n\t"
        "mov r8, r3\n\t"
    "_0801ADEE:\n"
        "mov r5, r8\n\t"
        "cmp r5, #0\n\t"
        "bne _0801AE60\n\t"
        "mov r3, #2\n\t"
        "ldrsh r0, [r7, r3]\n\t"
        "add sl, r0\n\t"
        "ldr r5, [sp, #0x34]\n\t"
        "cmp r5, #1\n\t"
        "bne _0801AE18\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r7, r3]\n\t"
        "ldrb r7, [r7, #4]\n\t"
        "add r0, r7, r0\n\t"
        "add r4, r4, r0\n\t"
        "ldr r0, [sp, #0x1c]\n\t"
        "add r5, r2, r0\n\t"
        "str r1, [sp]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r5, #0\n\t"
        "b _0801AE2A\n\t"
    "_0801AE18:\n"
        "mov r5, #0\n\t"
        "ldrsh r0, [r7, r5]\n\t"
        "add r4, r4, r0\n\t"
        "add r5, r2, #0\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "add r0, r1, r0\n\t"
        "str r0, [sp]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
    "_0801AE2A:\n"
        "mov r3, sl\n\t"
        "bl sub_800FDC8\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "bge _0801AE42\n\t"
        "ldr r0, [sp, #0x40]\n\t"
        "cmp r0, #0\n\t"
        "bne _0801AE42\n\t"
        "ldr r1, [sp, #0x2c]\n\t"
        "cmp r1, #3\n\t"
        "bgt _0801AE50\n\t"
    "_0801AE42:\n"
        "cmp r2, #0\n\t"
        "ble _0801AE56\n\t"
        "ldr r0, [sp, #8]\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "add r0, r0, r1\n\t"
        "cmp r2, r0\n\t"
        "blt _0801AE56\n\t"
    "_0801AE50:\n"
        "mov r2, #4\n\t"
        "mov r8, r2\n\t"
        "b _0801AE60\n\t"
    "_0801AE56:\n"
        "ldr r3, [sp, #0x34]\n\t"
        "mov r8, r3\n\t"
        "b _0801AE60\n\t"
    "_0801AE5C:\n"
        "ldr r5, [sp, #0x34]\n\t"
    "_0801AE5E:\n"
        "mov r8, r5\n\t"
    "_0801AE60:\n"
        "ldr r2, _0801AE98\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, [r1]\n\t"
        "str r0, [sp, #0x24]\n\t"
        "ldr r0, [r1, #4]\n\t"
        "add r1, sp, #0x24\n\t"
        "str r0, [r1, #4]\n\t"
        "mov sl, r2\n\t"
        "add r7, r1, #0\n\t"
        "ldr r0, [sp, #0x2c]\n\t"
        "cmp r0, #0\n\t"
        "bge _0801AE7C\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp, #0x2c]\n\t"
    "_0801AE7C:\n"
        "ldr r2, [sp, #0x30]\n\t"
        "cmp r2, #0\n\t"
        "bge _0801AE86\n\t"
        "mov r3, #0\n\t"
        "str r3, [sp, #0x30]\n\t"
    "_0801AE86:\n"
        "mov r5, #0\n\t"
        "mov r0, r8\n\t"
        "cmp r0, #8\n\t"
        "bhi _0801AF3C\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, _0801AE9C\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n\t"
    "_0801AE98:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801AE9C:\n"
        ".4byte _0801AEA0\n\t"
    "_0801AEA0:\n"
        ".4byte _0801AF32\n\t"
        ".4byte _0801AF14\n\t"
        ".4byte _0801AF14\n\t"
        ".4byte _0801AF32\n\t"
        ".4byte _0801AEC4\n\t"
        ".4byte _0801AF32\n\t"
        ".4byte _0801AF32\n\t"
        ".4byte _0801AF32\n\t"
        ".4byte _0801AEFC\n\t"
    "_0801AEC4:\n"
        "ldr r0, _0801AEF8\n\t"
        "ldr r2, [r0]\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x68\n\t"
        "mov r0, #8\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne _0801AF32\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0xc\n\t"
        "mov r3, #4\n\t"
        "bl sub_803AD88\n\t"
        "ldr r1, [sp, #0x30]\n\t"
        "lsl r0, r1, #8\n\t"
        "ldr r1, [r7, #4]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r7, #4]\n\t"
        "b _0801AF32\n\t"
        ".align 2, 0\n\t"
    "_0801AEF8:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801AEFC:\n"
        "ldr r0, [sp, #0x30]\n\t"
        "sub r0, #1\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r7, #4]\n\t"
        "sub r1, r1, r0\n\t"
        "ldr r0, _0801AF10\n\t"
        "and r1, r0\n\t"
        "str r1, [r7, #4]\n\t"
        "b _0801AF32\n\t"
        ".align 2, 0\n\t"
    "_0801AF10:\n"
        ".4byte 0xFFFFFF00\n\t"
    "_0801AF14:\n"
        "ldr r5, [sp, #0x34]\n\t"
        "cmp r5, #2\n\t"
        "bne _0801AF24\n\t"
        "ldr r2, [sp, #0x2c]\n\t"
        "lsl r0, r2, #8\n\t"
        "ldr r1, [sp, #0x24]\n\t"
        "add r0, r0, r1\n\t"
        "b _0801AF30\n\t"
    "_0801AF24:\n"
        "cmp r5, #1\n\t"
        "bne _0801AF32\n\t"
        "ldr r3, [sp, #0x2c]\n\t"
        "lsl r1, r3, #8\n\t"
        "ldr r0, [sp, #0x24]\n\t"
        "sub r0, r0, r1\n\t"
    "_0801AF30:\n"
        "str r0, [sp, #0x24]\n\t"
    "_0801AF32:\n"
        "ldr r0, _0801AFD0\n\t"
        "mov sl, r0\n\t"
        "mov r1, r8\n\t"
        "cmp r1, #8\n\t"
        "beq _0801AF42\n\t"
    "_0801AF3C:\n"
        "ldr r2, [sp, #0x30]\n\t"
        "cmp r2, #1\n\t"
        "bgt _0801AF7C\n\t"
    "_0801AF42:\n"
        "mov r3, sl\n\t"
        "ldr r2, [r3]\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x24\n\t"
        "mov r0, #4\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne _0801AF7C\n\t"
        "ldr r0, [sp, #0x40]\n\t"
        "cmp r0, #0\n\t"
        "beq _0801AF7C\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0xac\n\t"
        "mov r1, sb\n\t"
        "str r1, [r0]\n\t"
        "mov r1, #8\n\t"
        "sub r0, #0x44\n\t"
        "strb r1, [r0]\n\t"
        "ldr r2, [r3]\n\t"
        "ldr r1, [r2, #4]\n\t"
        "ldr r0, [sp, #0x30]\n\t"
        "sub r0, #1\n\t"
        "lsl r0, r0, #8\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r7, #4]\n\t"
        "mov r5, #0\n\t"
        "ldr r0, [r2]\n\t"
        "str r0, [sp, #0x24]\n\t"
    "_0801AF7C:\n"
        "mov r6, sl\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [sp, #0x24]\n\t"
        "ldr r2, [r7, #4]\n\t"
        "bl sub_8007398\n\t"
        "cmp r5, #0\n\t"
        "beq _0801AFAC\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0xc\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_803AD88\n\t"
        "ldr r1, [r6]\n\t"
        "ldr r0, [r1, #0x74]\n\t"
        "orr r0, r5\n\t"
        "str r0, [r1, #0x74]\n\t"
    "_0801AFAC:\n"
        "mov r5, r8\n\t"
        "cmp r5, #8\n\t"
        "beq _0801AFB4\n\t"
        "b _0801B1F8\n\t"
    "_0801AFB4:\n"
        "mov r0, sb\n\t"
        "ldr r2, [r0, #0x78]\n\t"
        "cmp r2, #1\n\t"
        "beq _0801AFC4\n\t"
        "cmp r2, #5\n\t"
        "beq _0801AFC4\n\t"
        "cmp r2, #6\n\t"
        "bne _0801AFD4\n\t"
    "_0801AFC4:\n"
        "mov r1, sb\n\t"
        "ldr r0, [r1, #0x44]\n\t"
        "add r0, #0x32\n\t"
        "mov r1, #1\n\t"
        "b _0801B1F6\n\t"
        ".align 2, 0\n\t"
    "_0801AFD0:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801AFD4:\n"
        "mov r3, sb\n\t"
        "ldr r0, [r3]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r3, [r6]\n\t"
        "ldr r1, [r3]\n\t"
        "asr r1, r1, #8\n\t"
        "sub r0, r0, r1\n\t"
        "asr r1, r0, #0x1f\n\t"
        "eor r0, r1\n\t"
        "sub r0, r0, r1\n\t"
        "cmp r0, #7\n\t"
        "ble _0801AFEE\n\t"
        "b _0801B1F8\n\t"
    "_0801AFEE:\n"
        "cmp r2, #3\n\t"
        "beq _0801B014\n\t"
        "cmp r2, #3\n\t"
        "bgt _0801AFFC\n\t"
        "cmp r2, #2\n\t"
        "beq _0801B002\n\t"
        "b _0801B1F8\n\t"
    "_0801AFFC:\n"
        "cmp r2, #4\n\t"
        "beq _0801B048\n\t"
        "b _0801B1F8\n\t"
    "_0801B002:\n"
        "ldr r1, [r3, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r5, #0\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r3, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0x11\n\t"
        "b _0801B1D0\n\t"
    "_0801B014:\n"
        "ldr r4, _0801B044\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_80232A0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _0801B024\n\t"
        "b _0801B1F8\n\t"
    "_0801B024:\n"
        "ldr r0, [r4]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq _0801B030\n\t"
        "b _0801B1F8\n\t"
    "_0801B030:\n"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0xf\n\t"
        "b _0801B1D0\n\t"
        ".align 2, 0\n\t"
    "_0801B044:\n"
        ".4byte gUnknown_030012C0\n\t"
    "_0801B048:\n"
        "ldr r4, _0801B074\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8023278\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _0801B058\n\t"
        "b _0801B1F8\n\t"
    "_0801B058:\n"
        "ldr r0, [r4]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq _0801B064\n\t"
        "b _0801B1F8\n\t"
    "_0801B064:\n"
        "mov r5, sl\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "b _0801B1CA\n\t"
        ".align 2, 0\n\t"
    "_0801B074:\n"
        ".4byte gUnknown_030012C0\n\t"
    "_0801B078:\n"
        "ldr r0, [sp, #8]\n\t"
        "sub r0, #4\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "add r0, #4\n\t"
        "str r0, [sp, #0x10]\n\t"
        "mov r5, sb\n\t"
        "ldr r0, [r5, #0x78]\n\t"
        "cmp r0, #7\n\t"
        "bls _0801B08E\n\t"
        "b _0801B1F8\n\t"
    "_0801B08E:\n"
        "lsl r0, r0, #2\n\t"
        "ldr r1, _0801B098\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n\t"
    "_0801B098:\n"
        ".4byte _0801B09C\n\t"
    "_0801B09C:\n"
        ".4byte _0801B0BC\n\t"
        ".4byte _0801B1E0\n\t"
        ".4byte _0801B0E4\n\t"
        ".4byte _0801B124\n\t"
        ".4byte _0801B180\n\t"
        ".4byte _0801B1E0\n\t"
        ".4byte _0801B1E0\n\t"
        ".4byte _0801B0BC\n\t"
    "_0801B0BC:\n"
        "add r0, sp, #4\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _0801B0CC\n\t"
        "b _0801B1F8\n\t"
    "_0801B0CC:\n"
        "ldr r0, _0801B0E0\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0xac\n\t"
        "mov r2, sb\n\t"
        "str r2, [r1]\n\t"
        "mov r1, #8\n\t"
        "add r0, #0x68\n\t"
        "b _0801B1F6\n\t"
        ".align 2, 0\n\t"
    "_0801B0E0:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801B0E4:\n"
        "add r0, sp, #4\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _0801B0F4\n\t"
        "b _0801B1F8\n\t"
    "_0801B0F4:\n"
        "mov r3, sb\n\t"
        "ldr r1, [r3]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, _0801B120\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r2]\n\t"
        "asr r0, r0, #8\n\t"
        "sub r1, r1, r0\n\t"
        "asr r0, r1, #0x1f\n\t"
        "eor r1, r0\n\t"
        "sub r1, r1, r0\n\t"
        "cmp r1, #7\n\t"
        "bgt _0801B1F8\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r5, #0\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0x11\n\t"
        "b _0801B1D0\n\t"
        ".align 2, 0\n\t"
    "_0801B120:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801B124:\n"
        "ldr r5, _0801B178\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80232A0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _0801B1F8\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne _0801B1F8\n\t"
        "add r0, sp, #4\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _0801B1F8\n\t"
        "mov r0, sb\n\t"
        "ldr r1, [r0]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, _0801B17C\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r2]\n\t"
        "asr r0, r0, #8\n\t"
        "sub r1, r1, r0\n\t"
        "asr r0, r1, #0x1f\n\t"
        "eor r1, r0\n\t"
        "sub r1, r1, r0\n\t"
        "cmp r1, #7\n\t"
        "bgt _0801B1F8\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0xf\n\t"
        "b _0801B1D0\n\t"
        ".align 2, 0\n\t"
    "_0801B178:\n"
        ".4byte gUnknown_030012C0\n\t"
    "_0801B17C:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801B180:\n"
        "ldr r5, _0801B1D8\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8023278\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _0801B1F8\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne _0801B1F8\n\t"
        "add r0, sp, #4\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _0801B1F8\n\t"
        "mov r5, sb\n\t"
        "ldr r1, [r5]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, _0801B1DC\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r2]\n\t"
        "asr r0, r0, #8\n\t"
        "sub r1, r1, r0\n\t"
        "asr r0, r1, #0x1f\n\t"
        "eor r1, r0\n\t"
        "sub r1, r1, r0\n\t"
        "cmp r1, #7\n\t"
        "bgt _0801B1F8\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
    "_0801B1CA:\n"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0x10\n\t"
    "_0801B1D0:\n"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "b _0801B1F8\n\t"
        ".align 2, 0\n\t"
    "_0801B1D8:\n"
        ".4byte gUnknown_030012C0\n\t"
    "_0801B1DC:\n"
        ".4byte gUnknown_030012D8\n\t"
    "_0801B1E0:\n"
        "add r0, sp, #4\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r1, r0, #0x18\n\t"
        "cmp r1, #0\n\t"
        "bne _0801B1F8\n\t"
        "mov r5, sb\n\t"
        "ldr r0, [r5, #0x44]\n\t"
        "add r0, #0x32\n\t"
    "_0801B1F6:\n"
        "strb r1, [r0]\n\t"
    "_0801B1F8:\n"
        "add sp, #0x44\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"

    );
}
#endif
