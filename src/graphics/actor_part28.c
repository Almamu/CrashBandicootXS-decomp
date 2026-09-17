#include "core.h"

/* A second per-instance "self" object family sharing the exact same
 * layout convention already documented for the boss-weapon cluster
 * (actor_part20.c-actor_part26.c, docs/matching/issue-58-0x08030334-actor.md):
 * state at `+0x28`, table-index/"kind" at `+0xc`, an anim-frame
 * halfword/byte pair at `+0x10`/`+0x12`, an accumulator at `+8`, a
 * "part table" pointer at `+0`, and an event/trampoline table pointer
 * at `+0x50` - plus a health-like countdown at `+0x54` and a death/
 * "dead" byte flag at `+0x6c`. These functions also drive a *singleton*
 * object reached through the global pointer `gUnknown_030015AC` (not a
 * per-instance `self`) - see docs/rom_map.md, "Follow-up reads
 * `sub_80339DC`'s helper cluster and `sub_8033470`" and "`sub_80331BC`
 * closes a long-open question: the missing singleton constructor". Most
 * of `gUnknown_030015AC`'s own accessors (`sub_8033880`-`sub_803390C`)
 * are trivial one-line getters for its fields; `sub_803390C`/
 * `sub_803395C` are the same state-transition/animation-frame-reset
 * sequence already documented for the boss cluster's
 * `sub_8030530`/`sub_8030C98`/`sub_803146C`. See
 * docs/matching/issue-62-0x08033804-actor.md. */

extern void *gUnknown_030015AC;
extern s32 gUnknown_030015B0;
extern s32 gUnknown_030015B4;
extern s32 gUnknown_030015B8;
extern s32 gUnknown_030015BC;
extern s32 gUnknown_030015C8;
extern s32 gUnknown_030015CC;
extern s32 gUnknown_030015D0;
extern s32 gUnknown_030015D4;
extern void *gUnknown_030015D8;
extern void *gUnknown_030015DC;
extern s32 gUnknown_030015F8;
extern s16 gUnknown_030015FC;
extern u8 gUnknown_030015FE;
extern u8 gUnknown_030015FF;
extern u16 gUnknown_03001590;
extern s32 gUnknown_03001594;
extern void *gUnknown_030008B4;
extern void *gUnknown_030008B8;
extern void *gUnknown_030012BC;

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 GetAnimFrameBaseOffset(void *self);

/* One-shot latch: if neither `gUnknown_030015FE` nor `gUnknown_030015FC`
 * has been set yet, arms both. */
void sub_8033804(void)
{
    if (gUnknown_030015FE == 0 && gUnknown_030015FC == 0) {
        gUnknown_030015FC = 1;
        gUnknown_030015FE = 1;
    }
}

/* Speed-override toggle for a P1/P2-mirrored object pair
 * (`gUnknown_030008B4`/`gUnknown_030008B8`, each a pointer to an object
 * with a speed-like `u16` at `+0x1e`). The first call caches the
 * current speed into `gUnknown_03001590`; from then on, `flag` picks
 * between a fixed max speed (`0x7FFF`) and the cached value, applying
 * it to both objects. */
void sub_8033828(u8 flag)
{
    register u16 val asm("r1");

    if (gUnknown_03001594 == 0) {
        gUnknown_03001590 = *(u16 *)((u8 *)gUnknown_030008B4 + 0x1e);
        gUnknown_03001594 = 1;
    }

    if (flag != 0) {
        register u8 *p asm("r0") = gUnknown_030008B4;

        val = 0x7FFF;
        *(u16 *)(p + 0x1e) = val;
    } else {
        register u8 *p asm("r2") = gUnknown_030008B4;

        val = gUnknown_03001590;
        *(u16 *)(p + 0x1e) = val;
    }

    *(u16 *)((u8 *)gUnknown_030008B8 + 0x1e) = val;
}

/* Constant getter - returns the singleton's lifetime counter
 * (`gUnknown_030015F8`). */
s32 sub_8033880(void)
{
    return gUnknown_030015F8;
}

extern void sub_803390C(s32 a0, s32 a1);

/* The singleton's death/reset transition: plays the death sound, then
 * decrements the lifetime counter `gUnknown_030015F8`, and once it
 * reaches zero clears `gUnknown_030015FF` and fires the state-5/
 * table-index-0 transition via `sub_803390C`. */
void sub_803388C(void)
{
    PlaySfx(gUnknown_030012BC, 4, 0x100);

    gUnknown_030015F8 -= 1;
    if (gUnknown_030015F8 == 0) {
        gUnknown_030015FF = gUnknown_030015F8;
        sub_803390C(5, 0);
    }
}

/* Constant getter - returns `gUnknown_030015DC` (a pointer to a small
 * per-state lookup table used by several functions in this cluster). */
void *sub_80338C4(void)
{
    return gUnknown_030015DC;
}

/* Constant getter - returns `gUnknown_030015B0` (the singleton's
 * current animation "kind" index). */
s32 sub_80338D0(void)
{
    return gUnknown_030015B0;
}

/* Constant getter - returns `gUnknown_030015D8` (set to the singleton
 * `self` pointer by its constructor, `sub_80331BC`). */
void *sub_80338DC(void)
{
    return gUnknown_030015D8;
}

/* Constant getter - returns the singleton's Z position field
 * (`gUnknown_030015BC`). */
s32 sub_80338E8(void)
{
    return gUnknown_030015BC;
}

/* Constant getter - returns the singleton's Y position field
 * (`gUnknown_030015B8`). */
s32 sub_80338F4(void)
{
    return gUnknown_030015B8;
}

/* Constant getter - returns the singleton's X position field
 * (`gUnknown_030015B4`). */
s32 sub_8033900(void)
{
    return gUnknown_030015B4;
}

/* State-transition setter for the singleton (`gUnknown_030015AC`):
 * selects animation "kind" `a0`, sets the table index to `a1`, resets
 * the anim-frame halfword/byte pair from the new table entry's first
 * field, and - once the current animation frame reaches the new
 * entry's duration (its `+4` halfword) - clears the accumulator at
 * `+8`. Same idiom as the boss cluster's `sub_8030530`/`sub_8030C98`. */
void sub_803390C(s32 a0, s32 a1)
{
    u8 *self;

    gUnknown_030015B0 = a0;
    self = gUnknown_030015AC;
    *(s32 *)(self + 0xc) = a1;

    {
        register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + a1 * 12);
        register u8 zero asm("r1") = 0;

        *(u16 *)(self + 0x10) = anim;
        self[0x12] = zero;
    }

    {
        s32 frame = GetAnimFrameBaseOffset(self);
        register s32 idx asm("r2") = *(s32 *)(self + 0xc);
        register u8 *table asm("r3") = *(u8 **)self;
        register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
        register s32 four asm("r2");
        register s32 val asm("r1");

        asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
        four = 4;
        val = *(s16 *)(entryPtr + four);

        if (frame >= val) {
            *(s32 *)(self + 8) = 0;
        }
    }
}

/* No-op stub. */
void nullsub_36(void)
{
}

/* State-transition setter for the singleton, gated by a depth
 * accumulator: advances `gUnknown_030015BC` by `gUnknown_030015D4`,
 * and - only while `gUnknown_030015C8` is still under its `0x81FF`
 * threshold - resets `gUnknown_030015CC`/`gUnknown_030015D0`, selects
 * animation "kind" 2, and runs the same table-index-0 anim-frame-reset
 * sequence as `sub_803390C`. */
void sub_803395C(void)
{
    u8 *self;

    gUnknown_030015BC += gUnknown_030015D4;

    if (gUnknown_030015C8 <= 0x81FF) {
        register s32 *pCC asm("r1") = &gUnknown_030015CC;
        register s32 *pD0 asm("r0") = &gUnknown_030015D0;
        register s32 zeroD0 asm("r5") = 0;

        *pD0 = zeroD0;
        *pCC = zeroD0;
        {
            register s32 two asm("r1") = 2;
            register s32 *pB0 asm("r0") = &gUnknown_030015B0;

            *pB0 = two;
        }

        self = gUnknown_030015AC;
        *(s32 *)(self + 0xc) = zeroD0;

        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
            register u8 zero asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero;
        }

        {
            s32 frame = GetAnimFrameBaseOffset(self);
            register s32 idx asm("r2") = *(s32 *)(self + 0xc);
            register u8 *table asm("r3") = *(u8 **)self;
            register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
            register s32 four asm("r2");
            register s32 val asm("r1");

            asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
            four = 4;
            val = *(s16 *)(entryPtr + four);

            if (frame >= val) {
                *(s32 *)(self + 8) = zeroD0;
            }
        }
    }
}

/* No-op stub. */
void nullsub_37(void)
{
}

asm(".align 2, 0");
