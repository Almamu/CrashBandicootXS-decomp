#include "core.h"
#include "gobj_1a794.h"

/* GitHub issue #25, ROM 0x0801A878-0x0801AB34: sub_801A878, the level
 * object spawner (`new` + inlined constructor sub_801B2E4, spawn-record
 * lookup through the level header at *gUnknown_030012B4, per-type mover
 * attachment). See include/gobj_1a794.h and
 * docs/matching/issue-25-level-objects.md.
 *
 * Parked: NAKED transcription of the ROM (byte-correct, not decompiled
 * C). The C reconstruction below (under NON_MATCHING) has the same
 * control flow, stack layout and nearly every instruction, but reload's
 * choice of scratch register for each use of the hi-register-resident
 * spawn record (`mov rN, r8`) and for several constants rotates
 * differently from the ROM's (gcc 2.x allocate_reload_reg's round-robin
 * last_spill_reg), leaving ~24 instructions in different low registers;
 * pinning individual sites just moves the rotation elsewhere. The C also
 * needs a hand-written outgoing-argument block for sub_801B7D8's
 * stack-passed byte (this compiler widens every stack argument to a word
 * store). */
#if NON_MATCHING

struct gobj *sub_801A878(u16 id, u16 x, u16 y, u16 index, s32 kind)
{
    struct gobj *obj;
    struct spawn_rec *rec;
    u32 type;
    struct mover_stack_args args;
    struct mover *m;

    obj = GobjInit(sub_8026EDC(0x80));
    obj->id = id;
    obj->x = x << 8;
    obj->y = y << 8;
    {
        u8 *lvl = *(u8 **)gUnknown_030012B4;
        u16 *offsets = *(u16 **)(lvl + 8);
        register u8 *recs asm("r0");

        rec = (struct spawn_rec *)(index * 2 + (u8 *)offsets);
        /* keep the offset-slot address in rec's own register, as the ROM
         * does (docs/workflow.md step 7) */
        asm("" : "+r"(rec));
        recs = *(u8 **)(lvl + 0xC);
        {
            register struct spawn_rec *ap asm("r4") = rec;
            register u32 sum asm("r5") = *(u16 *)ap;

            sum += (u32)recs;
            rec = (struct spawn_rec *)sum;
            type = ((struct spawn_rec *)sum)->type;
        }
    }
    switch (kind)
    {
    case 4:
        type = 2;
        break;
    case 5:
        type = 3;
        break;
    case 8:
        type = 7;
        break;
    case 3:
    case 9:
    case 10:
    case 11:
    case 12:
        type = 4;
        break;
    case 6:
        type = 6;
        break;
    }
    switch (type)
    {
    case 0:
        obj->type = 0;
        break;
    case 1:
        {
            register s32 one asm("r6") = 1;

            obj->type = one;
            {
                void *mem = sub_8026EDC(0x38);
                register s32 dX asm("r1") = rec->distX;
                register s32 dY asm("r2") = rec->distY;
                u32 fX = rec->dirX != 0;
                register u32 fY asm("r4") = rec->dirY != 0;

                *(volatile u8 *)&args.dirY = fY;
                *(volatile s32 *)&args.kind = one;
                m = MOVER_NEW(mem, dX, dY, fX);
            }
        }
        obj->mover = m;
        MOVER_CALL2(m, m18, obj);
        if (rec->flag)
        {
            u8 bit = 0x10;

            obj->flags = bit | obj->flags;
        }
        break;
    case 2:
        obj->type = 2;
        break;
    case 3:
        obj->type = 3;
        sub_801B2A8(obj, rec->flags & 1);
        break;
    case 4:
        obj->type = 4;
        break;
    case 5:
        obj->type = 5;
        {
            void *mem = sub_8026EDC(0x38);

            *(volatile u8 *)&args.dirY = 0;
            *(volatile s32 *)&args.kind = 5;
            m = MOVER_NEW(mem, 0, 0, 0);
        }
        obj->mover = m;
        MOVER_CALL2(m, m18, obj);
        break;
    case 6:
        obj->type = 6;
        if (sub_80233B4(gUnknown_030012C0) != 1)
        {
            void *mem = sub_8026EDC(0x38);

            *(volatile u8 *)&args.dirY = 0;
            *(volatile s32 *)&args.kind = 6;
            m = MOVER_NEW(mem, 0, 0, 0);
        }
        else
            m = sub_801961C(sub_8026EDC(0x38));
        obj->mover = m;
        MOVER_CALL2(m, m18, obj);
        break;
    case 7:
        obj->type = 7;
        {
            void *mem = sub_8026EDC(0x38);

            *(volatile u8 *)&args.dirY = 0;
            *(volatile s32 *)&args.kind = 7;
            m = MOVER_NEW(mem, 0, 0, 0);
        }
        obj->mover = m;
        MOVER_CALL2(m, m18, obj);
        break;
    }
    sub_8008E94(gUnknown_030012EC, obj);
    obj->anim = (void *)(**gUnknown_030012D0 + 0x1D4);
    obj->tag = kind;
    sub_80087C0(obj);
    sub_80087B4(obj);
    sub_800872C(obj, 0);
    {
        u8 *p = &obj->mirror;
        s32 v = ~0x10;

        v &= *p;
        /* two separate bit clears in the ROM - keep them from folding */
        asm("" : "+r"(v));
        v &= ~0x20;
        *p = v;
    }
    {
        register struct anim_rec *r asm("r1") = obj->anim->records;
        u32 id;
        register u8 *p asm("r2");
        register u32 low asm("r1");
        s32 mask;

        r += obj->tag;
        id = sub_8006DF8(gUnknown_030012B8, r->unk_14);
        p = &obj->slot;
        low = 0xF;
        /* hide 0xF from reload's cse, which would build ~0xF as 0xF - 0x1F */
        asm("" : "+r"(low));
        id &= low;
        mask = ~0xF;
        *p = (mask & *p) | id;
    }
    return obj;
}
#else
NAKED struct gobj *sub_801A878(u16 id, u16 x, u16 y, u16 index, s32 kind)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #8\n\t"
        "mov sb, r0\n\t"
        "add r5, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "mov r8, r3\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov sb, r0\n\t"
        "lsl r5, r5, #0x10\n\t"
        "lsr r5, r5, #0x10\n\t"
        "lsl r6, r6, #0x10\n\t"
        "lsr r6, r6, #0x10\n\t"
        "mov r1, r8\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "mov r8, r1\n\t"
        "mov r0, #0x80\n\t"
        "bl sub_8026EDC\n\t"
        "add r4, r0, #0\n\t"
        "bl sub_8009F90\n\t"
        "ldr r0, _0801A8F4\n\t"
        "str r0, [r4, #0x18]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801B2D8\n\t"
        "add r7, r4, #0\n\t"
        "mov r2, sb\n\t"
        "strh r2, [r7, #8]\n\t"
        "lsl r5, r5, #8\n\t"
        "str r5, [r7]\n\t"
        "lsl r6, r6, #8\n\t"
        "str r6, [r7, #4]\n\t"
        "ldr r0, _0801A8F8\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "mov r3, r8\n\t"
        "lsl r3, r3, #1\n\t"
        "mov r8, r3\n\t"
        "add r8, r0\n\t"
        "ldr r0, [r1, #0xc]\n\t"
        "mov r4, r8\n\t"
        "ldrh r5, [r4]\n\t"
        "add r5, r5, r0\n\t"
        "mov r8, r5\n\t"
        "ldr r2, [r5, #4]\n\t"
        "ldr r0, [sp, #0x24]\n\t"
        "sub r0, #3\n\t"
        "cmp r0, #9\n\t"
        "bhi _0801A93A\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, _0801A8FC\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n\t"
    "_0801A8F4:\n"
        ".4byte gStaticData_087E49DC\n\t"
    "_0801A8F8:\n"
        ".4byte gUnknown_030012B4\n\t"
    "_0801A8FC:\n"
        ".4byte _0801A900\n\t"
    "_0801A900:\n"
        ".4byte _0801A934\n\t"
        ".4byte _0801A928\n\t"
        ".4byte _0801A92C\n\t"
        ".4byte _0801A938\n\t"
        ".4byte _0801A93A\n\t"
        ".4byte _0801A930\n\t"
        ".4byte _0801A934\n\t"
        ".4byte _0801A934\n\t"
        ".4byte _0801A934\n\t"
        ".4byte _0801A934\n\t"
    "_0801A928:\n"
        "mov r2, #2\n\t"
        "b _0801A93A\n\t"
    "_0801A92C:\n"
        "mov r2, #3\n\t"
        "b _0801A93A\n\t"
    "_0801A930:\n"
        "mov r2, #7\n\t"
        "b _0801A93A\n\t"
    "_0801A934:\n"
        "mov r2, #4\n\t"
        "b _0801A93A\n\t"
    "_0801A938:\n"
        "mov r2, #6\n\t"
    "_0801A93A:\n"
        "cmp r2, #7\n\t"
        "bls _0801A940\n\t"
        "b _0801AA9C\n\t"
    "_0801A940:\n"
        "lsl r0, r2, #2\n\t"
        "ldr r1, _0801A94C\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n\t"
    "_0801A94C:\n"
        ".4byte _0801A950\n\t"
    "_0801A950:\n"
        ".4byte _0801A970\n\t"
        ".4byte _0801A976\n\t"
        ".4byte _0801A9D4\n\t"
        ".4byte _0801A9DA\n\t"
        ".4byte _0801A9EE\n\t"
        ".4byte _0801A9F4\n\t"
        ".4byte _0801AA1C\n\t"
        ".4byte _0801AA6C\n\t"
    "_0801A970:\n"
        "mov r0, #0\n\t"
        "str r0, [r7, #0x78]\n\t"
        "b _0801AA9C\n\t"
    "_0801A976:\n"
        "mov r6, #1\n\t"
        "str r6, [r7, #0x78]\n\t"
        "mov r0, #0x38\n\t"
        "bl sub_8026EDC\n\t"
        "mov r2, r8\n\t"
        "ldr r1, [r2, #8]\n\t"
        "ldr r2, [r2, #0xc]\n\t"
        "mov r3, r8\n\t"
        "mov r5, #0x10\n\t"
        "ldrsh r4, [r3, r5]\n\t"
        "neg r3, r4\n\t"
        "orr r3, r4\n\t"
        "lsr r3, r3, #0x1f\n\t"
        "mov sb, r3\n\t"
        "mov r3, r8\n\t"
        "mov r4, #0x12\n\t"
        "ldrsh r5, [r3, r4]\n\t"
        "neg r4, r5\n\t"
        "orr r4, r5\n\t"
        "lsr r4, r4, #0x1f\n\t"
        "mov r5, sp\n\t"
        "strb r4, [r5]\n\t"
        "str r6, [sp, #4]\n\t"
        "mov r3, sb\n\t"
        "bl sub_801B7D8\n\t"
        "add r2, r0, #0\n\t"
        "str r2, [r7, #0x44]\n\t"
        "ldr r1, [r2, #0xc]\n\t"
        "mov r5, #0x18\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r2, [r1, #0x1c]\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_803AD80\n\t"
        "mov r1, r8\n\t"
        "mov r2, #0x14\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "cmp r0, #0\n\t"
        "beq _0801AA9C\n\t"
        "mov r0, #0x10\n\t"
        "ldrb r3, [r7, #0xc]\n\t"
        "orr r0, r3\n\t"
        "strb r0, [r7, #0xc]\n\t"
        "b _0801AA9C\n\t"
    "_0801A9D4:\n"
        "mov r0, #2\n\t"
        "str r0, [r7, #0x78]\n\t"
        "b _0801AA9C\n\t"
    "_0801A9DA:\n"
        "mov r0, #3\n\t"
        "str r0, [r7, #0x78]\n\t"
        "mov r1, #1\n\t"
        "mov r4, r8\n\t"
        "ldrb r4, [r4]\n\t"
        "and r1, r4\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_801B2A8\n\t"
        "b _0801AA9C\n\t"
    "_0801A9EE:\n"
        "mov r0, #4\n\t"
        "str r0, [r7, #0x78]\n\t"
        "b _0801AA9C\n\t"
    "_0801A9F4:\n"
        "mov r4, #5\n\t"
        "str r4, [r7, #0x78]\n\t"
        "mov r0, #0x38\n\t"
        "bl sub_8026EDC\n\t"
        "mov r2, sp\n\t"
        "mov r1, #0\n\t"
        "strb r1, [r2]\n\t"
        "str r4, [sp, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0\n\t"
        "mov r3, #0\n\t"
        "bl sub_801B7D8\n\t"
        "add r2, r0, #0\n\t"
        "str r2, [r7, #0x44]\n\t"
        "ldr r1, [r2, #0xc]\n\t"
        "mov r5, #0x18\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "b _0801AA60\n\t"
    "_0801AA1C:\n"
        "mov r4, #6\n\t"
        "str r4, [r7, #0x78]\n\t"
        "ldr r0, _0801AA48\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80233B4\n\t"
        "cmp r0, #1\n\t"
        "beq _0801AA4C\n\t"
        "mov r0, #0x38\n\t"
        "bl sub_8026EDC\n\t"
        "mov r2, sp\n\t"
        "mov r1, #0\n\t"
        "strb r1, [r2]\n\t"
        "str r4, [sp, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0\n\t"
        "mov r3, #0\n\t"
        "bl sub_801B7D8\n\t"
        "b _0801AA56\n\t"
        ".align 2, 0\n\t"
    "_0801AA48:\n"
        ".4byte gUnknown_030012C0\n\t"
    "_0801AA4C:\n"
        "mov r0, #0x38\n\t"
        "bl sub_8026EDC\n\t"
        "bl sub_801961C\n\t"
    "_0801AA56:\n"
        "add r2, r0, #0\n\t"
        "str r2, [r7, #0x44]\n\t"
        "ldr r1, [r2, #0xc]\n\t"
        "mov r3, #0x18\n\t"
        "ldrsh r0, [r1, r3]\n\t"
    "_0801AA60:\n"
        "add r0, r2, r0\n\t"
        "ldr r2, [r1, #0x1c]\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_803AD80\n\t"
        "b _0801AA9C\n\t"
    "_0801AA6C:\n"
        "mov r4, #7\n\t"
        "str r4, [r7, #0x78]\n\t"
        "mov r0, #0x38\n\t"
        "bl sub_8026EDC\n\t"
        "mov r2, sp\n\t"
        "mov r1, #0\n\t"
        "strb r1, [r2]\n\t"
        "str r4, [sp, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0\n\t"
        "mov r3, #0\n\t"
        "bl sub_801B7D8\n\t"
        "add r2, r0, #0\n\t"
        "str r2, [r7, #0x44]\n\t"
        "ldr r1, [r2, #0xc]\n\t"
        "mov r4, #0x18\n\t"
        "ldrsh r0, [r1, r4]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r2, [r1, #0x1c]\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_803AD80\n\t"
    "_0801AA9C:\n"
        "ldr r0, _0801AB28\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8008E94\n\t"
        "ldr r0, _0801AB2C\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r5, #0xea\n\t"
        "lsl r5, r5, #1\n\t"
        "add r0, r0, r5\n\t"
        "str r0, [r7, #0x20]\n\t"
        "add r4, r7, #0\n\t"
        "add r4, #0x2d\n\t"
        "add r0, sp, #0x24\n\t"
        "ldrb r0, [r0]\n\t"
        "strb r0, [r4]\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r7, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x28\n\t"
        "mov r0, #0x11\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r2]\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x21\n\t"
        "neg r1, r1\n\t"
        "and r0, r1\n\t"
        "strb r0, [r2]\n\t"
        "ldr r0, [r7, #0x20]\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r2, [r4]\n\t"
        "lsl r0, r2, #3\n\t"
        "sub r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, _0801AB30\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r1, [r1, #0x14]\n\t"
        "bl sub_8006DF8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "add r0, r7, #0\n\t"
        "add sp, #8\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n\t"
    "_0801AB28:\n"
        ".4byte gUnknown_030012EC\n\t"
    "_0801AB2C:\n"
        ".4byte gUnknown_030012D0\n\t"
    "_0801AB30:\n"
        ".4byte gUnknown_030012B8\n\t"

    );
}
#endif
