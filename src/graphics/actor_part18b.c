#include "core.h"
#include "action_obj.h"

/* Continuation of actor_part18.c's `gActionCtrlStateTable` action-table
 * entries. See actor_part18.c's own
 * top-of-file comment for the shared field-offset conventions
 * (`self+0xc`/`self+0x10`/`+0x27`.."+0x32" etc.) these functions use. */

extern u32 gKeys;
extern void *gInput;
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);
extern u8 GetDpadDirection(void *dummy);
extern void SetActionCtrlModeAnim(void *self, s32 a, s32 b, s32 c, s32 d);

/* Same shape as `ActionCtrlStateStandUp` (actor_part18.c) - resets the same
 * flag/counter/table-index trio via `SetActionCtrlModeAnim` while `part+0x38` is
 * set. */
void ActionCtrlStateCrawlStandUp(struct act *self)
{
    struct act_part *part = self->part;

    if (part->animDone != 0) {
        SetActionCtrlModeAnim(self, 0, 0x12, 0, 0);
        self->next31 = 0;
        self->flag2F = 1;
        self->next27 = 0;
        self->next32 = 0;
        self->flag30 = 1;
        self->next28 = 0;
    }
}

/* While `part+0x38` is set: computes `v = (gKeys bit 0x100)
 * != 0`, forced to `1` when `GetDpadDirection`'s D-pad-remap result is `2` or
 * in `[7,8]`. If still clear, resets the same flag/counter/table-index
 * trio as `ActionCtrlStateStandUp` via `SetActionCtrlModeAnim`; otherwise fires the usual
 * base+offset+fn-pointer trampoline pair. */
void ActionCtrlStateBodySlamLand(struct act *self)
{
    struct act_part *part = self->part;

    if (part->animDone != 0) {
        void *dummy = gInput;
        u16 m = gKeys & 0x100;
        u8 v = m != 0;
        s32 st = GetDpadDirection(dummy);

        switch (st) {
        case 2:
        case 7:
        case 8:
            v = 1;
            break;
        }

        if (v == 0) {
            SetActionCtrlModeAnim(self, 0, 0x12, 0, v);
            self->next31 = v;
            self->flag2F = 1;
            self->next27 = v;
            self->next32 = v;
            self->flag30 = 1;
            self->next28 = v;
        } else {
            struct act_vtable *mgr = self->vt;
            struct act_method *off;
            _call_via_r2((u8 *)self + mgr->m20.thisOffset, (void *)0x10,
                        mgr->m20.fn);
            off = &self->vt->m50;
            _call_via_r3((u8 *)self + off->thisOffset, self->part,
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

extern void ActionCtrlStateIdle(void *self);

/* Clears `self+0x18`. If `gKeys` bit `0x100` is set, fires
 * the usual base+offset+fn-pointer trampoline pair and clears
 * `self+0x1c` too. Otherwise, while `part+0x38` is set, resets the same
 * flag/counter/table-index trio as `ActionCtrlStateStandUp` via `SetActionCtrlModeAnim`
 * (storing the raw masked bit value, not a normalized boolean, since
 * the ROM reuses the same register for both the branch test and the
 * stores here - unlike `ActionCtrlStateBodySlamLand`'s `!= 0`-normalized version of the
 * same test), then tail-calls `ActionCtrlStateIdle`.
 *
 * Formerly NAKED (docs/matching/issue-15-16-17-naked-retry-2.md): the
 * old gap - the masked bit landing in a scratch register before being
 * copied to the register `flag` keeps - goes away when the assignment
 * sits inside the test, `if ((flag = ...) != 0)`. Matches under both
 * compilers. */

void ActionCtrlStateLand(struct act *self)
{
    u16 flag;

    self->frame = 0;
    if ((flag = gKeys & 0x100) != 0)
    {
        ACT_CALL1(self, m20, 0x10);
        ACT_CALL2(self, m50, self->part, 3);
        self->frames = 0;
        return;
    }
    if (self->part->animDone)
    {
        SetActionCtrlModeAnim(self, 0, 0x12, 0, flag);
        self->next31 = flag;
        self->flag2F = 1;
        self->next27 = flag;
        self->next32 = flag;
        self->flag30 = 1;
        self->next28 = flag;
    }
    ActionCtrlStateIdle(self);
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
