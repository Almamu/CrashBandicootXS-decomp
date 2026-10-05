#include "core.h"
#include "action_obj.h"

/* Part of GitHub issue #16's remainder (0x08011BD4-0x08012D24) - see
 * actor_part79.c's top-of-file comment for the shared field-offset
 * conventions (`self+0xc`/`self+0x10`/`+0x27`..`+0x32`) this "child
 * object" family uses (include/action_obj.h's `struct act`). Not ROM-adjacent to actor_part79.c's functions
 * (raw `UpdateActionCtrl`/`TryActionCtrlDoubleJump`/`HandleActionCtrlAirInput` sit in between, see
 * asm/code_3_2_17_12420.s), hence its own file. */

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);

/* If `part+0x68` bit 3 is set, returns 0 (busy). Otherwise, on
 * `part+0x69 > 2`, fires the `+0x20`/`+0x24` trampoline pair (id
 * `0x1a`) then the `+0x50`/`+0x54` pair (id `0x1b`) with `part` as its
 * argument; on `part+0x69 <= 2`, fires only the `+0x20`/`+0x24` pair
 * (id `0x1c`). Either way, sets the second half of the shared
 * state/flag/table-index trio (`self+0x32`=0/`+0x30`=1/`+0x28`=4) and
 * returns 1. */
u8 CheckActionCtrlLeftGround(void *selfArg)
{
    struct act *self = selfArg;
    u8 *part = (u8 *)self->part;
    register u8 *p asm("r1") = part + 0x68;
    register s32 mask asm("r0") = 8;

    mask &= *p;
    if (mask == 0) {
        if (part[0x69] > 2) {
            struct act_vtable *mgr = self->vt;
            _call_via_r2((u8 *)self + mgr->m20.thisOffset, (void *)0x1a, mgr->m20.fn);
            {
                struct act_method *off = &self->vt->m50;
                _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0x1b, off->fn);
            }
        } else {
            struct act_vtable *mgr = self->vt;
            _call_via_r2((u8 *)self + mgr->m20.thisOffset, (void *)0x1c, mgr->m20.fn);
        }

        {
            register s32 four asm("r2") = 4;
            self->next32 = 0;
            self->flag30 = 1;
            self->next28 = four;
        }
        return 1;
    }
    return 0;
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
