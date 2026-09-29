#include "core.h"
#include "action_obj.h"

/* Continuation of actor_part18.c's `gStaticData_0816BF20` action-table
 * entries. See actor_part18.c's own
 * top-of-file comment for the shared field-offset conventions
 * (`self+0xc`/`self+0x10`/`+0x27`.."+0x32" etc.) these functions use. */

extern u32 gUnknown_030007E0;
extern void *gUnknown_03001304;
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);
extern u8 sub_8000760(void *dummy);
extern void sub_8015780(void *self, s32 a, s32 b, s32 c, s32 d);

/* Same shape as `sub_801426C` (actor_part18.c) - resets the same
 * flag/counter/table-index trio via `sub_8015780` while `part+0x38` is
 * set. */
void sub_80144E0(struct act *self)
{
    struct act_part *part = self->part;

    if (part->animDone != 0) {
        sub_8015780(self, 0, 0x12, 0, 0);
        self->next31 = 0;
        self->flag2F = 1;
        self->next27 = 0;
        self->next32 = 0;
        self->flag30 = 1;
        self->next28 = 0;
    }
}

/* While `part+0x38` is set: computes `v = (gUnknown_030007E0 bit 0x100)
 * != 0`, forced to `1` when `sub_8000760`'s D-pad-remap result is `2` or
 * in `[7,8]`. If still clear, resets the same flag/counter/table-index
 * trio as `sub_801426C` via `sub_8015780`; otherwise fires the usual
 * base+offset+fn-pointer trampoline pair. */
void sub_8014524(struct act *self)
{
    struct act_part *part = self->part;

    if (part->animDone != 0) {
        void *dummy = gUnknown_03001304;
        u16 m = gUnknown_030007E0 & 0x100;
        u8 v = m != 0;
        s32 st = sub_8000760(dummy);

        switch (st) {
        case 2:
        case 7:
        case 8:
            v = 1;
            break;
        }

        if (v == 0) {
            sub_8015780(self, 0, 0x12, 0, v);
            self->next31 = v;
            self->flag2F = 1;
            self->next27 = v;
            self->next32 = v;
            self->flag30 = 1;
            self->next28 = v;
        } else {
            struct act_vtable *mgr = self->vt;
            struct act_method *off;
            sub_803AD80((u8 *)self + mgr->m20.thisOffset, (void *)0x10,
                        mgr->m20.fn);
            off = &self->vt->m50;
            sub_803AD84((u8 *)self + off->thisOffset, self->part,
                        (void *)3, off->fn);
            {
                u8 zero = 0;
                self->next31 = zero;
                self->flag2F = 1;
                self->next27 = zero;
            }
        }
    }
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");

extern void sub_8012D24(void *self);

/* Clears `self+0x18`. If `gUnknown_030007E0` bit `0x100` is set, fires
 * the usual base+offset+fn-pointer trampoline pair and clears
 * `self+0x1c` too. Otherwise, while `part+0x38` is set, resets the same
 * flag/counter/table-index trio as `sub_801426C` via `sub_8015780`
 * (storing the raw masked bit value, not a normalized boolean, since
 * the ROM reuses the same register for both the branch test and the
 * stores here - unlike `sub_8014524`'s `!= 0`-normalized version of the
 * same test), then tail-calls `sub_8012D24`.
 *
 * Formerly NAKED (docs/matching/issue-15-16-17-naked-retry-2.md): the
 * old gap - the masked bit landing in a scratch register before being
 * copied to the register `flag` keeps - goes away when the assignment
 * sits inside the test, `if ((flag = ...) != 0)`. Matches under both
 * compilers. */
asm(".set _call_via_r2, sub_803AD80\n"
    ".set _call_via_r3, sub_803AD84\n");

void sub_80145E4(struct act *self)
{
    u16 flag;

    self->frame = 0;
    if ((flag = gUnknown_030007E0 & 0x100) != 0)
    {
        ACT_CALL1(self, m20, 0x10);
        ACT_CALL2(self, m50, self->part, 3);
        self->frames = 0;
        return;
    }
    if (self->part->animDone)
    {
        sub_8015780(self, 0, 0x12, 0, flag);
        self->next31 = flag;
        self->flag2F = 1;
        self->next27 = flag;
        self->next32 = flag;
        self->flag30 = 1;
        self->next28 = flag;
    }
    sub_8012D24(self);
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
