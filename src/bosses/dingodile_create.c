#include "core.h"
#include "gobj_1a794.h"
#include "bosses.h"

/* GitHub issue #25, ROM 0x0801A794-0x0801A878 (see include/gobj_1a794.h
 * and docs/matching/issue-25-level-objects.md). CreateDingodileShieldCtrl/DestroyDingodile/
 * CreateDingodile are constructor/destructor bodies of a subclass of the
 * CreateBossCtrl object family (input_ctrl_queue.c), method tables
 * gDingodileShieldVtable / gDingodileVtable; StartDingodileMotion is the same
 * mirror-gated velocity-record copy as SetMegaMixMotionXFromSet (mega_mix.c) but
 * indexed straight into gDingodileMotionEntries.
 *
 * UNUSED - no caller anywhere in the ROM (checked asm/ .s files, src/ .c files
 * and the ROM for Thumb pointers): SetDingodileStep, SetDingodileNextState. */

/* The CreateBossCtrl-family object (input_ctrl_queue.c): only the fields touched
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
    CreateBossCtrl((struct boss_ctrl *)self);
    self->vtable = (void *)gDingodileShieldVtable;
    return self;
}

/* `self` is unused. The pins keep `part` in r3 and `index` in r5 (copied
 * in that order), and the empty asm keeps `index` live to the end so the
 * second lookup doesn't clobber it in place (docs/workflow.md step 7). */
void StartDingodileMotion(void *self, struct gobj *partArg, s32 indexArg)
{
    register struct gobj *part asm("r3") = partArg;
    register s32 index asm("r5") = indexArg;
    const struct speed_ramp *e = &gDingodileMotionRecords[gDingodileMotionEntries[index][0]];

    if ((s32)(part->mirror << 27) < 0)
    {
        s32 x = -e->start;
        s32 z = -e->target;
        s32 y = e->step;

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
    else
    {
        s32 x = e->start;
        s32 y = e->step;
        s32 z = e->target;

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
    {
        const struct speed_ramp *e2 = &gDingodileMotionRecords[gDingodileMotionEntries[index][1]];
        s32 x = e2->start;
        s32 y = e2->step;
        s32 z = e2->target;

        part->speedY = x;
        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
    asm("" : : "r"(index));
}

void DestroyDingodile(struct seq_obj *self, s32 flags)
{
    self->vtable = (void *)gDingodileVtable;
    DestroyBossCtrl((struct boss_ctrl *)self, flags);
}

struct seq_obj *CreateDingodile(struct seq_obj *self, u32 a, u32 b)
{
    CreateBossCtrl((struct boss_ctrl *)self);
    self->vtable = (void *)gDingodileVtable;
    SpawnDingodileShieldOrRocket((struct dingodile_boss *)self, 0, (u16)a, (u16)b, 0);
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
