#include "core.h"
#include "action_obj.h"

/* This file (and actor_part18b.c, its non-adjacent continuation) covers
 * part of `gStaticData_0816BF20`, the 42-slot per-level action dispatch
 * table documented in docs/rom_map.md ("`gStaticData_0816BF20` is a
 * 42-slot, fully-populated action dispatch table") - `self` is the
 * player/action object those table entries are invoked on, not the
 * small (0x1c-byte) `struct actor` from include/actor.h. `self+0xc` is
 * a per-category table of `{s16 offset; void *fn}` pairs (at least two
 * entries known so far, `+0x20`/`+0x24` and `+0x50`/`+0x54`) fed
 * through the `_call_via_r2`/`_call_via_r3` trampolines together with
 * `self+offset` and `self+0x10` (a "part" sub-object) - the same
 * base+offset+fn-pointer convention already named in actor_part17.c's
 * doc comments. The `+0x27`/`+0x28`/`+0x29`/`+0x2f`/`+0x30`/`+0x31`/
 * `+0x32` bytes are a state/flag/table-index trio pair this whole
 * action-table family shares; none of the three objects' full shapes
 * are pinned down yet, so every access here stays a raw offset rather
 * than a guessed struct. */

extern u32 gKeys;
extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);
extern u8 sub_800AAEC(void *self, s32 action);
extern void sub_801434C(void *self);
extern void sub_8015508(void *self);
extern void sub_8015780(void *self, s32 a, s32 b, s32 c, s32 d);

/* Clears `part+0x38`'s "busy" flag by resetting the shared
 * flag/counter/table-index trio (`+0x31`/`+0x2f`/`+0x27` and
 * `+0x32`/`+0x30`/`+0x28`) via `sub_8015780`, but only while that flag
 * is actually set. */
void sub_801426C(void *selfArg)
{
    u8 *self = selfArg;
    u8 *part = *(u8 **)(self + 0x10);

    if (part[0x38] != 0) {
        sub_8015780(self, 0, 0x12, 0, 0);
        self[0x31] = 0;
        self[0x2f] = 1;
        self[0x27] = 0;
        self[0x32] = 0;
        self[0x30] = 1;
        self[0x28] = 0;
    }
}

/* On the "confirm" input edge (checked via `sub_800AAEC(part, 0xb)`),
 * plays a sound, clears two `part+0xd` bits (the runtime `& -2`/`& -3`
 * negation rather than a folded mask - see docs/matching.md), and hands
 * off to `sub_8015508`. Otherwise, while `part+0x38` is set, fires the
 * usual base+offset+fn-pointer trampoline pair and tail-calls
 * `sub_801434C` (below). */
void sub_80142B0(void *selfArg)
{
    struct act *self = selfArg;
    u32 snap = *(u32 *)&gKeys;

    if ((*(u16 *)((u8 *)&snap + 2) & 1) != 0
        && sub_800AAEC(self->part, 0xb) == 1) {
        PlaySfx(gUnknown_030012BC, 0xc, 0x100);

        {
            register u8 *part asm("r1") = (u8 *)self->part;
            register s32 mask asm("r0") = 2;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }
        {
            register u8 *part asm("r1") = (u8 *)self->part;
            register s32 mask asm("r0") = 3;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }

        sub_8015508(self);
        return;
    }

    if (self->part->animDone != 0) {
        struct act_vtable *mgr = self->vt;
        _call_via_r2((u8 *)self + mgr->m20.thisOffset, (void *)0x14,
                    mgr->m20.fn);
        {
            struct act_method *off = &self->vt->m50;
            _call_via_r3((u8 *)self + off->thisOffset, self->part,
                        (void *)0, off->fn);
        }
        sub_801434C(self);
    }
}

extern u8 GetDpadDirection(void *dummy);
extern u8 sub_8012A7C(void *self);
extern void sub_80122CC(void *self);
extern void *gUnknown_03001304;

/* Queues action `next` on the +0x31/+0x2F/+0x27 trio; as inline
 * parameters, `cur`/`next` are materialized before the stores. */
static inline void ActQueue27(struct act *self, s32 cur, s32 next)
{
    self->next31 = cur;
    self->flag2F = 1;
    self->next27 = next;
}

/* The shared handler `sub_80142B0` tail-calls: same "confirm" edge check
 * (short-circuits before reaching `sub_8012A7C` when it fires), then
 * (once `sub_8012A7C(self)` is clear) dispatches on `GetDpadDirection`'s
 * D-pad-remap result - `1` fires one trampoline pair, `0`/`2` fires
 * another - before falling into a shared tail that, when the input
 * snapshot's `0x180` bits are clear and `sub_800AAEC(part, 2)` just
 * fired, runs a third trampoline pair and finishes with
 * `sub_80122CC`.
 *
 * Formerly NAKED; matches under old_agbcc (this whole file is built with
 * it, see docs/matching/issue-15-16-17-naked-retry-2.md): the case 0/2
 * trio goes through ActQueue27 so its 0 is materialized before the
 * stores, and the case 1 trio is written out in both branches, with the
 * calls in the do/while ACT_VCALL form, so gcc cross-jumps the shared
 * `bl` of the second method call as the ROM does (the if/else ACT_CALL
 * form there changes the whole block). */
void sub_801434C(void *selfArg)
{
    struct act *self = selfArg;
    u32 in = gKeys;
    u8 busy;
    u8 dir;
    u8 hit;
    u32 held;

    if ((INPUT_PRESSED(in) & 1) && sub_800AAEC(self->part, 0xB) == 1)
    {
        PlaySfx(gUnknown_030012BC, 0xC, 0x100);
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        sub_8015508(self);
        return;
    }
    busy = sub_8012A7C(self);
    if (busy != 0)
        return;
    dir = GetDpadDirection(gUnknown_03001304);
    switch (dir)
    {
    case 0:
    case 2:
        ACT_CALL1(self, m20, 0x1B);
        ACT_CALL2(self, m50, self->part, 1);
        ActQueue27(self, 0, 0);
        break;
    case 1:
        if (sub_800AAEC(self->part, 2) == 1)
        {
            ACT_VCALL1(self, m20, 0x15);
            ACT_VCALL2(self, m50, self->part, 2);
            self->next31 = busy;
            self->flag2F = dir;
            self->next27 = busy;
            break;
        }
        ACT_VCALL1(self, m20, 0x11);
        ACT_VCALL2(self, m50, self->part, 4);
        self->next31 = busy;
        self->flag2F = dir;
        self->next27 = busy;
        break;
    }
    held = INPUT_HELD(in) & 0x180;
    if (held == 0 && (hit = sub_800AAEC(self->part, 2)) == 1)
    {
        ACT_CALL1(self, m20, 0x12);
        ACT_CALL2(self, m50, self->part, 2);
        self->next31 = held;
        self->flag2F = hit;
        self->next27 = held;
    }
    sub_80122CC(self);
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
