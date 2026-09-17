#include "core.h"

/* This file (and actor_part18b.c, its non-adjacent continuation) covers
 * part of `gStaticData_0816BF20`, the 42-slot per-level action dispatch
 * table documented in docs/rom_map.md ("`gStaticData_0816BF20` is a
 * 42-slot, fully-populated action dispatch table") - `self` is the
 * player/action object those table entries are invoked on, not the
 * small (0x1c-byte) `struct actor` from include/actor.h. `self+0xc` is
 * a per-category table of `{s16 offset; void *fn}` pairs (at least two
 * entries known so far, `+0x20`/`+0x24` and `+0x50`/`+0x54`) fed
 * through the `sub_803AD80`/`sub_803AD84` trampolines together with
 * `self+offset` and `self+0x10` (a "part" sub-object) - the same
 * base+offset+fn-pointer convention already named in actor_part17.c's
 * doc comments. The `+0x27`/`+0x28`/`+0x29`/`+0x2f`/`+0x30`/`+0x31`/
 * `+0x32` bytes are a state/flag/table-index trio pair this whole
 * action-table family shares; none of the three objects' full shapes
 * are pinned down yet, so every access here stays a raw offset rather
 * than a guessed struct. */

extern u32 gUnknown_030007E0;
extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);
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
 * `sub_801434C` (still raw/parked - see docs/matching.md, "Parked, not
 * matched: sub_801434C" - real bytes live in asm/code_3_2_17_1434c.s,
 * linked right after this object). */
void sub_80142B0(void *selfArg)
{
    u8 *self = selfArg;
    u32 snap = *(u32 *)&gUnknown_030007E0;

    if ((*(u16 *)((u8 *)&snap + 2) & 1) != 0
        && sub_800AAEC(*(void **)(self + 0x10), 0xb) == 1) {
        PlaySfx(gUnknown_030012BC, 0xc, 0x100);

        {
            register u8 *part asm("r1") = *(u8 **)(self + 0x10);
            register s32 mask asm("r0") = 2;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }
        {
            register u8 *part asm("r1") = *(u8 **)(self + 0x10);
            register s32 mask asm("r0") = 3;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }

        sub_8015508(self);
        return;
    }

    if ((*(u8 **)(self + 0x10))[0x38] != 0) {
        u8 *mgr = *(u8 **)(self + 0xc);
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x14,
                    *(void **)(mgr + 0x24));
        {
            u8 *off = *(u8 **)(self + 0xc) + 0x50;
            sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10),
                        (void *)0, *(void **)(off + 4));
        }
        sub_801434C(self);
    }
}

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching.md, "Parked, not matched:
 * sub_801434C" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_17_1434c.s) is used otherwise. Every load/store, branch
 * and call is confirmed correct and in the right case order (matching
 * the ROM's case-0/2-before-case-1 layout). The residual gap is
 * entirely instruction-*scheduling*: two small blocks (the `case 0`/
 * `case 2` field-write trio's value-vs-address evaluation order, and
 * the closing `masked = *(u16 *)&snap & 0x180` block's address/
 * constant/load ordering and register reuse) compile correctly but in
 * a different relative order than the ROM for a handful of mutually-
 * independent instructions - this compiler's own scheduler, not
 * anything under this file's control via plain C. Tried: reordering the
 * source statements (no effect - the scheduler reorders independent
 * instructions regardless of source order); splitting the constant/
 * load into separate statements or a separate pointer local (same);
 * pinning the constant/load/result to specific registers via
 * `register ... asm("rN")` (fixes the register choice but not the
 * instruction order, and loses the `u16` truncation semantics on
 * later reads - a worse mismatch than the one being chased). */
extern u8 sub_8000760(void *dummy);
extern u8 sub_8012A7C(void *self);
extern void sub_80122CC(void *self);
extern void *gUnknown_03001304;

/* The shared handler `sub_80142B0` tail-calls: same "confirm" edge check
 * (short-circuits before reaching `sub_8012A7C` when it fires), then
 * (once `sub_8012A7C(self)` is clear) dispatches on `sub_8000760`'s
 * D-pad-remap result - `1` fires one trampoline pair, `0`/`2` fires
 * another - before falling into a shared tail that, when the input
 * snapshot's `0x180` bits are clear and `sub_800AAEC(part, 2)` just
 * fired, runs a third trampoline pair and finishes with
 * `sub_80122CC`. */
void sub_801434C(void *selfArg)
{
    u8 *self = selfArg;
    u32 snap = *(u32 *)&gUnknown_030007E0;
    u8 v6;
    u8 v5;

    if ((*(u16 *)((u8 *)&snap + 2) & 1) != 0
        && sub_800AAEC(*(void **)(self + 0x10), 0xb) == 1) {
        PlaySfx(gUnknown_030012BC, 0xc, 0x100);

        {
            register u8 *part asm("r1") = *(u8 **)(self + 0x10);
            register s32 mask asm("r0") = 2;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }
        {
            register u8 *part asm("r1") = *(u8 **)(self + 0x10);
            register s32 mask asm("r0") = 3;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }

        sub_8015508(self);
        return;
    }

    v6 = sub_8012A7C(self);
    if (v6 != 0) {
        return;
    }

    v5 = sub_8000760(gUnknown_03001304);
    switch (v5) {
    case 0:
    case 2: {
        u8 *mgr = *(u8 **)(self + 0xc);
        u8 *off;
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x1b,
                    *(void **)(mgr + 0x24));
        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10),
                    (void *)1, *(void **)(off + 4));
        {
            u8 zero = 0;
            self[0x31] = zero;
            self[0x2f] = 1;
            self[0x27] = zero;
        }
        break;
    }
    case 1: {
        /* Both arms build the same sub_803AD84 call (addr/part/fn all
         * pinned to r0/r1/r3, the arg3 constant to r2) and converge on a
         * single shared `bl` - matching the ROM's own tail-sharing
         * (`b _0801446E` / fallthrough into it) rather than emitting two
         * separate calls. Needs the four call-argument locals pinned to
         * their AAPCS registers; without the pins gcc computes them into
         * whatever registers are free and adds a final register-shuffle
         * before the shared call that the ROM doesn't have. */
        register void *addr asm("r0");
        register void *part asm("r1");
        register s32 arg3 asm("r2");
        register void *fn asm("r3");

        if (sub_800AAEC(*(void **)(self + 0x10), 2) == 1) {
            u8 *mgr = *(u8 **)(self + 0xc);
            u8 *off;
            sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x15,
                        *(void **)(mgr + 0x24));
            off = *(u8 **)(self + 0xc) + 0x50;
            addr = self + *(s16 *)off;
            part = *(void **)(self + 0x10);
            fn = *(void **)(off + 4);
            arg3 = 2;
            goto sub_801434C_case1_call;
        }
        {
            u8 *mgr = *(u8 **)(self + 0xc);
            u8 *off;
            sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x11,
                        *(void **)(mgr + 0x24));
            off = *(u8 **)(self + 0xc) + 0x50;
            addr = self + *(s16 *)off;
            part = *(void **)(self + 0x10);
            fn = *(void **)(off + 4);
            arg3 = 4;
        }
sub_801434C_case1_call:
        sub_803AD84(addr, part, (void *)arg3, fn);
        self[0x31] = v6;
        self[0x2f] = v5;
        self[0x27] = v6;
        break;
    }
    }

    {
        u16 masked = *(u16 *)&snap & 0x180;

        if (masked == 0) {
            u8 aaec2 = sub_800AAEC(*(void **)(self + 0x10), 2);

            if (aaec2 == 1) {
                u8 *mgr = *(u8 **)(self + 0xc);
                u8 *off;
                sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x12,
                            *(void **)(mgr + 0x24));
                off = *(u8 **)(self + 0xc) + 0x50;
                sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10),
                            (void *)2, *(void **)(off + 4));
                self[0x31] = masked;
                self[0x2f] = aaec2;
                self[0x27] = masked;
            }
        }
    }
    sub_80122CC(self);
}
#endif /* NON_MATCHING */
