#include "core.h"
#include "gobj_1a794.h"

/* GitHub issue #25, ROM 0x0801A794-0x0801A878 (see include/gobj_1a794.h
 * and docs/matching/issue-25-level-objects.md). sub_801A794/sub_801A824/
 * sub_801A838 are constructor/destructor bodies of a subclass of the
 * sub_8017A8C object family (actor_part27.c), method tables
 * gStaticData_087E490C / gStaticData_087E4974; sub_801A7AC is the same
 * mirror-gated velocity-record copy as sub_8017F14 (actor_part27b.c) but
 * indexed straight into gStaticData_0816C418.
 *
 * UNUSED - no caller anywhere in the ROM (checked asm/ .s files, src/ .c files
 * and the ROM for Thumb pointers): sub_801A870, sub_801A874. */

/* The sub_8017A8C-family object (actor_part27.c): only the fields touched
 * here. */
struct seq_obj
{
    u8 unk_00[0xC];
    void *vtable;       // 0x0C
    u8 unk_10[0xC];
    s32 unk_1C;         // 0x1C
    u8 unk_20[4];
    s32 unk_24;         // 0x24
};

struct seq_obj *sub_801A794(struct seq_obj *self)
{
    sub_8017A8C(self);
    self->vtable = gStaticData_087E490C;
    return self;
}

/* `self` is unused. The pins keep `part` in r3 and `index` in r5 (copied
 * in that order), and the empty asm keeps `index` live to the end so the
 * second lookup doesn't clobber it in place (docs/workflow.md step 7). */
void sub_801A7AC(void *self, struct gobj *partArg, s32 indexArg)
{
    register struct gobj *part asm("r3") = partArg;
    register s32 index asm("r5") = indexArg;
    struct vec3 *e = &gStaticData_0816C3B8[gStaticData_0816C418[index].a];

    if ((s32)(part->mirror << 27) < 0)
    {
        s32 x = -e->x;
        s32 z = -e->z;
        s32 y = e->y;

        part->speedX = x;
        part->velA.x = x;
        part->velA.y = y;
        part->velA.z = z;
    }
    else
    {
        s32 x = e->x;
        s32 y = e->y;
        s32 z = e->z;

        part->speedX = x;
        part->velA.x = x;
        part->velA.y = y;
        part->velA.z = z;
    }
    {
        struct vec3 *e2 = &gStaticData_0816C3B8[gStaticData_0816C418[index].b];
        s32 x = e2->x;
        s32 y = e2->y;
        s32 z = e2->z;

        part->speedY = x;
        part->velB.x = x;
        part->velB.y = y;
        part->velB.z = z;
    }
    asm("" : : "r"(index));
}

void sub_801A824(struct seq_obj *self, s32 flags)
{
    self->vtable = gStaticData_087E4974;
    sub_8017A78(self, flags);
}

struct seq_obj *sub_801A838(struct seq_obj *self, u32 a, u32 b)
{
    sub_8017A8C(self);
    self->vtable = gStaticData_087E4974;
    sub_8019EBC(self, 0, (u16)a, (u16)b, 0);
    return self;
}

void sub_801A870(struct seq_obj *self, s32 value)
{
    self->unk_1C = value;
}

void sub_801A874(struct seq_obj *self, s32 value)
{
    self->unk_24 = value;
}
