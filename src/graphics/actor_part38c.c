#include "core.h"
#include "vtable.h"
#include "actor.h"
#include "action_obj.h"

/* Continuation of actor_part38b.c (issue #18's chunk) - covers
 * `sub_8015350` through `sub_80156EC`. Same "self" object family
 * documented at the top of actor_part18.c/actor_part28.c. */

extern void *gAudioContext;
extern void *gUnknown_030012D8;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);
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
        struct vtable_slot *mgr;
        u8 *off;

        PlaySfx(gAudioContext, 0xa, 0x100);
        *(s32 *)(self + 0x18) = 0;
        *(s32 *)(self + 0x1c) = 0x18;

        off = *(u8 **)(self + 0xc) + 0x50;
        _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x10,
                    *(void **)(off + 4));
        mgr = *(struct vtable_slot **)(self + 0xc);
        _call_via_r2(self + mgr[4].delta, (void *)0xd, mgr[4].fn);

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
        struct vtable_slot *mgr;
        u8 *off;

        PlaySfx(gAudioContext, 0xa, 0x100);
        *(s32 *)(self + 0x18) = 0;
        *(s32 *)(self + 0x1c) = 0x18;

        self[0x21] = 0;
        self[0x20] = 0;
        self[0x22] = 0;
        self[0x23] = 0;
        self[0x24] = 0;

        off = *(u8 **)(self + 0xc) + 0x50;
        _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x1e,
                    *(void **)(off + 4));
        mgr = *(struct vtable_slot **)(self + 0xc);
        _call_via_r2(self + mgr[4].delta, (void *)0x21, mgr[4].fn);
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
        struct vtable_slot *mgr;
        u8 *p31;

        *(s32 *)(self + 0x18) = 0;

        off = *(u8 **)(self + 0xc) + 0x50;
        _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x18,
                    *(void **)(off + 4));
        mgr = *(struct vtable_slot **)(self + 0xc);
        _call_via_r2(self + mgr[4].delta, (void *)4, mgr[4].fn);

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
        struct vtable_slot *mgr;
        u8 *p31;

        off = *(u8 **)(self + 0xc) + 0x50;
        _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0xd,
                    *(void **)(off + 4));
        *(s32 *)(self + 0x18) = 0;
        mgr = *(struct vtable_slot **)(self + 0xc);
        _call_via_r2(self + mgr[4].delta, (void *)3, mgr[4].fn);

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
    struct vtable_slot *mgr = *(struct vtable_slot **)(self + 0xc);
    u8 *off;
    u8 *p28;

    _call_via_r2(self + mgr[4].delta, (void *)0xb, mgr[4].fn);
    off = *(u8 **)(self + 0xc) + 0x50;
    _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0xb,
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
    struct vtable_slot *mgr = *(struct vtable_slot **)(self + 0xc);
    u8 *off;
    u8 *p28;

    _call_via_r2(self + mgr[4].delta, (void *)0xb, mgr[4].fn);
    off = *(u8 **)(self + 0xc) + 0x50;
    _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0xb,
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
        struct vtable_slot *mgr = *(struct vtable_slot **)(self + 0xc);
        u8 *off;

        _call_via_r2(self + mgr[4].delta, (void *)0x20, mgr[4].fn);
        off = *(u8 **)(self + 0xc) + 0x50;
        _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x1f,
                    *(void **)(off + 4));

        *(s32 *)(self + 0x18) = zero;
        *(s32 *)(self + 0x1c) = zero;
    }
}

/* Bumps `frame`; once it reaches `frames` (or the part's `animDone` is
 * already set), stamps `unk_26 = 0xc`, fires the `m20`/`m50` methods
 * (actions `0x20`/`0x1f`), and resets `frame`/`frames` to `0`.
 * Always tail-calls `sub_80122CC`. */
void sub_80155F8(void *selfArg)
{
    struct act *self = selfArg;

    self->frame += 1;
    if (self->frame >= self->frames || self->part->animDone != 0) {
        register s32 zero asm("r4");
        u8 *p26 = &self->unk_26;
        struct act_method *m;

        zero = 0;
        *p26 = 0xc;

        m = &self->vt->m20;
        _call_via_r2((u8 *)self + m->thisOffset, (void *)0x20, m->fn);
        m = &self->vt->m50;
        _call_via_r3((u8 *)self + m->thisOffset, self->part, (void *)0x1f, m->fn);

        self->frame = zero;
        self->frames = zero;
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
        struct vtable_slot *mgr = *(struct vtable_slot **)(self + 0xc);
        u8 *off;

        _call_via_r2(self + mgr[4].delta, (void *)0x20, mgr[4].fn);
        off = *(u8 **)(self + 0xc) + 0x50;
        _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x1f,
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
        struct vtable_slot *mgr = *(struct vtable_slot **)(self + 0xc);
        u8 *off;

        _call_via_r2(self + mgr[4].delta, (void *)0x11, mgr[4].fn);
        off = *(u8 **)(self + 0xc) + 0x50;
        _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)4,
                    *(void **)(off + 4));
    }
}

extern s32 sub_80231BC(void *self);
extern void *gLevelState;

/* While `part+0x38` is set: when `sub_80231BC(gLevelState)` is
 * true, fires the mgr trampoline pair with actions `0x19`/`7`;
 * otherwise fires only the first trampoline with action `0x18`.
 *
 * Takes `self` as a `u8 *` parameter directly: the old `u8 *self =
 * selfArg;` copy survived GCSE's copy propagation into the `else` arm,
 * which is what forced the extra `push {r5}`/`adds r5, r4, #0` - see
 * docs/matching/issue-18-0x08014f8c-actor.md, "Later pass: strag2 retry". */
void sub_80156EC(u8 *self)
{
    if (((u8 *)*(struct actor **)(self + 0x10) + 0x38)[0] != 0) {
        if ((u8)sub_80231BC(gLevelState)) {
            struct vtable_slot *mgr = *(struct vtable_slot **)(self + 0xc);
            u8 *off;

            _call_via_r2(self + mgr[4].delta, (void *)0x19, mgr[4].fn);
            off = *(u8 **)(self + 0xc) + 0x50;
            _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)7,
                        *(void **)(off + 4));
        } else {
            struct vtable_slot *mgr = *(struct vtable_slot **)(self + 0xc);

            _call_via_r2(self + mgr[4].delta, (void *)0x18, mgr[4].fn);
        }
    }
}

/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
