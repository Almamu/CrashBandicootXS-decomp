#include "core.h"
#include "actor.h"

/* Continuation of actor_part28b.c (issue #18's chunk) - covers
 * `sub_8015350` through `sub_80156B4` (matched) and `sub_80156EC`
 * (parked, NON_MATCHING); non-adjacent to actor_part28b.c since the
 * parked `sub_80152F0` sits raw between them (asm/code_3_2_17_152f0.s).
 * Same "self" object family documented at the top of actor_part18.c/
 * actor_part28.c. */

extern void *gUnknown_030012BC;
extern void *gUnknown_030012D8;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);
extern void sub_80122CC(void *self);
extern void sub_8014B54(void *self);
extern void sub_80241A4(void);

/* Clears `self+0x33`, saves `self+8`'s previous value (truncated) into
 * `self+0x2d`, overwrites `self+8` with `arg1`, and clears
 * `self+0x2c`/`self+0x2b`. If `arg1` isn't `0xd`/`0xe`, also clears
 * `part+0x90` and the player's `+0x92`/`+0x94` (written twice - the ROM
 * really does re-derive the player pointer and store the same byte
 * there a second time). */
void sub_8015350(void *selfArg, s32 arg1)
{
    u8 *self = selfArg;
    s32 old;

    self[0x33] = 0;
    old = *(s32 *)(self + 8);
    self[0x2d] = (u8)old;
    *(s32 *)(self + 8) = arg1;
    self[0x2c] = 0;
    self[0x2b] = 0;

    if ((u32)(arg1 - 0xd) > 1) {
        struct actor *part = *(struct actor **)(self + 0x10);
        u8 *player;

        ((u8 *)part)[0x90] = 0;

        player = *(u8 * volatile *)&gUnknown_030012D8;
        player[0x92] = 0;
        player = *(u8 * volatile *)&gUnknown_030012D8;
        player[0x94] = 0;
        player = *(u8 * volatile *)&gUnknown_030012D8;
        player[0x94] = 0;
    }
}

/* While `self+0x26` is clear: plays a fixed cue, resets `self+0x18`/
 * `0x1c` to `0`/`0x18`, fires the mgr trampoline pair (actions `0x10`
 * then `0xd`), and clears `self+0x20`-`self+0x24`. */
void sub_8015398(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x26] == 0) {
        u8 *mgr;
        u8 *off;

        PlaySfx(gUnknown_030012BC, 0xa, 0x100);
        *(s32 *)(self + 0x18) = 0;
        *(s32 *)(self + 0x1c) = 0x18;

        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x10,
                    *(void **)(off + 4));
        mgr = *(u8 **)(self + 0xc);
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0xd, *(void **)(mgr + 0x24));

        self[0x21] = 0;
        self[0x20] = 0;
        self[0x22] = 0;
        self[0x23] = 0;
        self[0x24] = 0;
    }
}

/* Same shape as `sub_8015398`, different action codes (`0x1e`/`0x21`)
 * and the field-clear runs before the trampoline pair instead of
 * after. */
void sub_80153FC(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x26] == 0) {
        u8 *mgr;
        u8 *off;

        PlaySfx(gUnknown_030012BC, 0xa, 0x100);
        *(s32 *)(self + 0x18) = 0;
        *(s32 *)(self + 0x1c) = 0x18;

        self[0x21] = 0;
        self[0x20] = 0;
        self[0x22] = 0;
        self[0x23] = 0;
        self[0x24] = 0;

        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x1e,
                    *(void **)(off + 4));
        mgr = *(u8 **)(self + 0xc);
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x21, *(void **)(mgr + 0x24));
    }
}

/* Two near-identical arms keyed on `self+0x29`: both fire the mgr
 * trampoline pair and set the state/counter trio (`0x31`/`0x2f`), then
 * latch `self+0x31` again if `part+0x100` is set - only the
 * trampoline actions and the table-index (`self+0x27`, `0x1b` vs `1`)
 * differ between the two arms. */
void sub_8015460(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x29] != 0) {
        u8 *off;
        u8 *mgr;
        u8 *p31;

        *(s32 *)(self + 0x18) = 0;

        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x18,
                    *(void **)(off + 4));
        mgr = *(u8 **)(self + 0xc);
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)4, *(void **)(mgr + 0x24));

        {
            u8 val = 0x1b;

            p31 = self + 0x31;
            *p31 = 0;
            {
                u8 *p2f = self + 0x2f;
                u8 one = 1;

                *p2f = one;
                p2f -= 8;
                *p2f = val;

                if (((u8 *)*(void **)(self + 0x10))[0x100] != 0) {
                    *p31 = one;
                }
            }
        }
    } else {
        u8 *off;
        u8 *mgr;
        u8 *p31;

        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0xd,
                    *(void **)(off + 4));
        *(s32 *)(self + 0x18) = 0;
        mgr = *(u8 **)(self + 0xc);
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)3, *(void **)(mgr + 0x24));

        {
            u8 one = 1;
            u8 *p2f;

            p31 = self + 0x31;
            *p31 = 0;
            p2f = self + 0x2f;
            *p2f = one;
            p2f -= 8;
            *p2f = one;

            if (((u8 *)*(void **)(self + 0x10))[0x100] != 0) {
                *p31 = one;
            }
        }
    }
}

/* Fires the mgr trampoline pair with actions `0xb`/`0xb`, clears
 * `self+0x18`, sets the state/counter/table-index trio to `0`/`1`/`0xb`,
 * and clears `part+0x68`. */
void sub_8015508(void *selfArg)
{
    u8 *self = selfArg;
    register s32 zero asm("r5") = 0;
    register u8 idx asm("r2");
    u8 *mgr = *(u8 **)(self + 0xc);
    u8 *off;
    u8 *p28;

    sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0xb, *(void **)(mgr + 0x24));
    off = *(u8 **)(self + 0xc) + 0x50;
    sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0xb,
                *(void **)(off + 4));

    *(s32 *)(self + 0x18) = zero;
    idx = 0xb;
    self[0x32] = zero;
    self[0x30] = 1;
    p28 = self + 0x28;
    asm volatile("" : "+r"(p28));
    *p28 = idx;
    (*(u8 **)(self + 0x10))[0x68] = zero;
}

/* Same shape as `sub_8015508`, table-index `7` instead of `0xb`. */
void sub_8015558(void *selfArg)
{
    u8 *self = selfArg;
    register s32 zero asm("r5") = 0;
    register u8 idx asm("r2");
    u8 *mgr = *(u8 **)(self + 0xc);
    u8 *off;
    u8 *p28;

    sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0xb, *(void **)(mgr + 0x24));
    off = *(u8 **)(self + 0xc) + 0x50;
    sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0xb,
                *(void **)(off + 4));

    *(s32 *)(self + 0x18) = zero;
    idx = 7;
    self[0x32] = zero;
    self[0x30] = 1;
    p28 = self + 0x28;
    asm volatile("" : "+r"(p28));
    *p28 = idx;
    (*(u8 **)(self + 0x10))[0x68] = zero;
}

/* Single-instruction store: `self+0x10 = val`. */
void sub_80155A8(void *selfArg, void *val)
{
    *(void **)((u8 *)selfArg + 0x10) = val;
}

/* Trivial tail-call. */
void sub_80155AC(void *selfArg)
{
    sub_8014B54(selfArg);
}

/* While `part+0x38` is set: fires the mgr trampoline pair (actions
 * `0x20`/`0x1f`) and clears `self+0x18`/`0x1c`. */
void sub_80155B8(void *selfArg)
{
    u8 *self = selfArg;

    if (((u8 *)*(struct actor **)(self + 0x10))[0x38] != 0) {
        register s32 zero asm("r4") = 0;
        u8 *mgr = *(u8 **)(self + 0xc);
        u8 *off;

        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x20, *(void **)(mgr + 0x24));
        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x1f,
                    *(void **)(off + 4));

        *(s32 *)(self + 0x18) = zero;
        *(s32 *)(self + 0x1c) = zero;
    }
}

/* Bumps `self+0x18`; once it reaches `self+0x1c` (or `part+0x38` is
 * already set), stamps `self+0x26 = 0xc`, fires the mgr trampoline pair
 * (actions `0x20`/`0x1f`), and resets `self+0x18`/`0x1c` to `0`.
 * Always tail-calls `sub_80122CC`. */
void sub_80155F8(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x18) += 1;
    if (*(s32 *)(self + 0x18) >= *(s32 *)(self + 0x1c)
        || ((u8 *)*(struct actor **)(self + 0x10))[0x38] != 0) {
        register s32 zero asm("r4");
        u8 *p26 = self + 0x26;
        u8 *mgr;
        u8 *off;

        zero = 0;
        *p26 = 0xc;

        mgr = *(u8 **)(self + 0xc);
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x20, *(void **)(mgr + 0x24));
        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x1f,
                    *(void **)(off + 4));

        *(s32 *)(self + 0x18) = zero;
        *(s32 *)(self + 0x1c) = zero;
    }

    sub_80122CC(self);
}

/* Same shape as `sub_80155B8` - byte-identical ROM encoding at a
 * different address (no shared caller; kept as a separate copy rather
 * than a wrapper to match). */
void sub_8015650(void *selfArg)
{
    u8 *self = selfArg;

    if (((u8 *)*(struct actor **)(self + 0x10) + 0x38)[0] != 0) {
        register s32 zero asm("r4") = 0;
        u8 *mgr = *(u8 **)(self + 0xc);
        u8 *off;

        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x20, *(void **)(mgr + 0x24));
        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x1f,
                    *(void **)(off + 4));

        *(s32 *)(self + 0x18) = zero;
        *(s32 *)(self + 0x1c) = zero;
    }
}

/* While `part+0x38` is set: sets the player's `+0xc` bit `0x80` and
 * tail-calls `sub_80241A4`. */
void sub_8015690(void *selfArg)
{
    u8 *self = selfArg;

    if (((u8 *)*(struct actor **)(self + 0x10) + 0x38)[0] != 0) {
        register u8 *player asm("r1") = gUnknown_030012D8;
        register s32 bit asm("r0") = 0x80;
        register u8 old asm("r2") = player[0xc];

        bit |= old;
        player[0xc] = bit;
        sub_80241A4();
    }
}

/* While `part+0x38` is set: fires the mgr trampoline pair with actions
 * `0x11`/`4`. */
void sub_80156B4(void *selfArg)
{
    u8 *self = selfArg;

    if (((u8 *)*(struct actor **)(self + 0x10) + 0x38)[0] != 0) {
        u8 *mgr = *(u8 **)(self + 0xc);
        u8 *off;

        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x11, *(void **)(mgr + 0x24));
        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)4,
                    *(void **)(off + 4));
    }
}

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-18-0x08014f8c-actor.md,
 * "Parked, not matched: sub_80156EC" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_17_156ec.s) is used otherwise. Every load/store, branch
 * and call is confirmed correct; the residual gap is the `else` arm
 * recomputing `self` into a fresh register (an extra `push`/`pop`
 * this compiler insists on once the `mgr` local is redeclared inside
 * that arm) where the ROM reuses the same `self` register the whole
 * function already lives in - tried inlining the `mgr` expression
 * directly at the call site and sharing one `mgr` local across both
 * arms; both left either this extra register or the mgr-hoisting
 * mismatch documented in the same file's `sub_8014F8C`-style
 * anti-CSE notes. */
extern s32 sub_80231BC(void *self);

/* While `part+0x38` is set: when `sub_80231BC(gUnknown_030012C0)` is
 * true, fires the mgr trampoline pair with actions `0x19`/`7`;
 * otherwise fires only the first trampoline with action `0x18`. */
void sub_80156EC(void *selfArg)
{
    u8 *self = selfArg;
    extern void *gUnknown_030012C0;

    if (((u8 *)*(struct actor **)(self + 0x10) + 0x38)[0] != 0) {
        if ((u8)sub_80231BC(gUnknown_030012C0) != 0) {
            u8 *mgr = *(u8 **)(self + 0xc);
            u8 *off;

            sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x19, *(void **)(mgr + 0x24));
            off = *(u8 **)(self + 0xc) + 0x50;
            sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)7,
                        *(void **)(off + 4));
        } else {
            u8 *mgr2 = *(u8 **)(self + 0xc);

            sub_803AD80(self + *(s16 *)(mgr2 + 0x20), (void *)0x18, *(void **)(mgr2 + 0x24));
        }
    }
}
#endif /* NON_MATCHING */
