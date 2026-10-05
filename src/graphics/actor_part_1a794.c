#include "core.h"
#include "gobj_1a794.h"

/* GitHub issue #25, ROM 0x0801A794-0x0801A878 (see include/gobj_1a794.h
 * and docs/matching/issue-25-level-objects.md). CreateDingodileShieldCtrl/DestroyDingodile/
 * CreateDingodile are constructor/destructor bodies of a subclass of the
 * CreateBossCtrl object family (actor_part27.c), method tables
 * gDingodileShieldVtable / gDingodileVtable; StartDingodileMotion is the same
 * mirror-gated velocity-record copy as SetMegaMixMotionXFromSet (actor_part27b.c) but
 * indexed straight into gDingodileMotionEntries.
 *
 * UNUSED - no caller anywhere in the ROM (checked asm/ .s files, src/ .c files
 * and the ROM for Thumb pointers): SetDingodileStep, SetDingodileNextState. */

/* The CreateBossCtrl-family object (actor_part27.c): only the fields touched
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

struct seq_obj *CreateDingodileShieldCtrl(struct seq_obj *self)
{
    CreateBossCtrl(self);
    self->vtable = gDingodileShieldVtable;
    return self;
}

/* `self` is unused. The pins keep `part` in r3 and `index` in r5 (copied
 * in that order), and the empty asm keeps `index` live to the end so the
 * second lookup doesn't clobber it in place (docs/workflow.md step 7). */
void StartDingodileMotion(void *self, struct gobj *partArg, s32 indexArg)
{
    register struct gobj *part asm("r3") = partArg;
    register s32 index asm("r5") = indexArg;
    struct vec3 *e = &gDingodileMotionRecords[gDingodileMotionEntries[index].a];

    if ((s32)(part->mirror << 27) < 0)
    {
        s32 x = -e->x;
        s32 z = -e->z;
        s32 y = e->y;

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
    else
    {
        s32 x = e->x;
        s32 y = e->y;
        s32 z = e->z;

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
    {
        struct vec3 *e2 = &gDingodileMotionRecords[gDingodileMotionEntries[index].b];
        s32 x = e2->x;
        s32 y = e2->y;
        s32 z = e2->z;

        part->speedY = x;
        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
    asm("" : : "r"(index));
}

void DestroyDingodile(struct seq_obj *self, s32 flags)
{
    self->vtable = gDingodileVtable;
    DestroyBossCtrl(self, flags);
}

struct seq_obj *CreateDingodile(struct seq_obj *self, u32 a, u32 b)
{
    CreateBossCtrl(self);
    self->vtable = gDingodileVtable;
    SpawnDingodileShieldOrRocket(self, 0, (u16)a, (u16)b, 0);
    return self;
}

void SetDingodileStep(struct seq_obj *self, s32 value)
{
    self->unk_1C = value;
}

void SetDingodileNextState(struct seq_obj *self, s32 value)
{
    self->unk_24 = value;
}
