#include "core.h"
#include "actor.h"

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void *sub_8007C30(void *dest, void *pt);
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void sub_8008A40(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, s32 unused, void *compareViewport);
extern void sub_8009868(void *manager, s32 arg1);
extern void sub_8008D30(void *manager, s32 arg1);
extern void sub_80106DC(void);
extern void *gUnknown_030012F0;
extern void *gUnknown_0300130C;
extern void *gUnknown_030012EC;

/* GitHub issue #9/#10's own "most tractable next target" flagged by
 * issue-9-10-0x0800a884-graphics.md: a two-flag-gated teardown/
 * notification step on `self`, the same still-unnamed "big object"
 * (at least 0x108 bytes) `actor_part15.c`/`actor_part77.c` already
 * work on, guarded by `self+0x105` (a "torn down already" latch -
 * whatever set it, it's read once here into `wasCleared`, confirmed 0
 * on every path that reaches either branch below, and both branches
 * reuse that already-loaded 0 for their own writes rather than
 * reloading a fresh constant, matching the ROM's own `r6` reuse).
 *
 * `self+0xc` bit 1: relocates `self`'s primary AABB
 * (`sub_8007C30`, `actor_part9.c`'s own copy of the same helper) onto
 * a second stack slot (`sub_800014C`, a plain `memcpy`) before
 * unpacking it back out into scalars for `sub_8008A40` - the exact
 * "relocate then unpack" idiom `sub_8008A40`'s own doc comment
 * (`actor_part7.c`) already documents from its callers' side, done
 * here explicitly in the caller instead of inline in the callee.
 *
 * `self+0xc` bit 7: clears `self+0x108`/`self+0x10c` (the same fields
 * `sub_8010E2C` clears elsewhere in this object family, just written
 * directly here instead of through that helper) and fires three
 * teardown/notification calls: `sub_8009868` (NAKED-parked,
 * `actor_part11d.c`) against `gUnknown_0300130C`'s manager with
 * selector `3`, `sub_8008D30` (`actor_part10.c`) against
 * `gUnknown_030012EC`'s manager with selector `4`, and `sub_80106DC`
 * (`game_loop23.c`) with no arguments. `sub_8009868`'s own body never
 * reads its second argument (confirmed by inspection - the ROM caller
 * still loads it into r1 before the call regardless), so this call
 * passes it through anyway to reproduce that dead-argument load.
 *
 * PARKED (NON_MATCHING) - one call's argument-marshalling order.
 * Everything else in this function is confirmed instruction-for-
 * instruction against the ROM: both bit tests use the established
 * "byte loads into r1, shifted result lands in r0" idiom
 * (`sub_800A884`'s own doc comment, `actor_part78.c`), the
 * `self+0x108`/`self+0x10c` clear shares one base-address computation
 * exactly like the ROM's `adds r0, r5, r1` / `str r6, [r0]` / `strb
 * r6, [r0, #4]` pair (needed `clearedFields` pinned to `r0` - left
 * unpinned it reused `self`'s own `r5` in place instead of a fresh
 * register), and the leading `sub_8007C30`/`sub_800014C` pair matches
 * byte-for-byte using a raw `u8 boxCopy[0x10]` buffer read back via
 * pointer casts rather than a `struct aabb boxCopy` (a named-struct
 * local let this compiler cache `&boxCopy` in a register and reuse it
 * for every field read afterward - even `volatile`-qualifying the
 * struct didn't stop it - where the ROM re-issues four independent
 * `ldr rX, [sp, #imm]` instructions with no cached base at all; the
 * raw-array/pointer-cast form closed that gap on its own).
 *
 * The one gap that resisted every technique tried: the ROM evaluates
 * `sub_8008A40`'s 7 arguments in the order `unused`(`self[0x24]`,
 * stack slot 2), `compareViewport`(`self`, stack slot 3),
 * `boxH`(`boxCopy.field_c`, stack slot 1), `boxX`/`boxY`/`boxW`
 * (`r1`/`r2`/`r3`), `manager`(`r0`, deferred last since `r0` is reused
 * as scratch throughout the rest) - each stack-bound value's compute
 * is immediately followed by its own `str` to the outgoing-argument
 * slot. Every plain-C phrasing tried (the values inlined directly in
 * the call; each precomputed into its own named local first, in ROM
 * order; a `asm volatile("" ::: "memory")` scheduling barrier between
 * each) instead has this compiler batch *all* outgoing-stack-argument
 * stores together immediately before the `bl`, after first computing
 * every value (register-bound or not) - the compute order can be
 * nudged by C source order, but the store timing for stack arguments
 * apparently can't, from any C-level phrasing tried. This is the same
 * class of gap already documented and accepted as unclosable in this
 * exact source file's `sub_8008AD8`/`sub_8008D80` doc comment above
 * (`actor_part7.c`) - "this compiler has no way to express 'this
 * scalar parameter is already sitting in the right stack position'" -
 * and the same class `PlaySfx`'s own doc comment describes as "gcc
 * 2.9's own instruction scheduler picks a fixed policy... that doesn't
 * appear to be steerable from C source at all" (issue #3 write-up).
 * Real bytes stay in the new `asm/code_3_2_16_ab9c.s` (trimmed from
 * `asm/code_3_2_16.s`, which now ends after `sub_800AAEC`, with the
 * remainder - `sub_800AC2C` onward - moved to the new
 * `asm/code_3_2_16_ac2c.s`). See
 * docs/matching/issue-9-10-0x0800ab9c-graphics.md. */
#if NON_MATCHING
void sub_800AB9C(void *selfArg)
{
    register u8 *self asm("r5") = selfArg;
    register u32 wasCleared asm("r6") = self[0x105];

    if (wasCleared != 0) {
        return;
    }

    {
        register u8 flagByte asm("r1") = self[0xc];
        register u32 bit1 asm("r0") = flagByte >> 1;
        register u32 mask asm("r1") = 1;
        register u32 result asm("r0") = bit1 & mask;

        if (result) {
            struct aabb box;
            u8 boxCopy[0x10];
            void *manager;

            sub_8007C30(&box, self);
            manager = gUnknown_030012F0;
            sub_800014C(boxCopy, &box, sizeof(box));
            sub_8008A40(manager, *(s32 *)(boxCopy + 0), *(s32 *)(boxCopy + 4), *(s32 *)(boxCopy + 8), *(s32 *)(boxCopy + 0xc), self[0x24], self);
        }
    }

    {
        register u8 flagByte asm("r1") = self[0xc];
        register u32 bit7 asm("r0") = flagByte >> 7;

        if (bit7) {
            register u8 *clearedFields asm("r0") = self + 0x108;

            *(u32 *)clearedFields = wasCleared;
            clearedFields[4] = wasCleared;

            sub_8009868(gUnknown_0300130C, 3);
            sub_8008D30(gUnknown_030012EC, 4);
            sub_80106DC();
        }
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
