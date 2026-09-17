#include "core.h"
#include "vram_pool.h"

extern void *gUnknown_03001318;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012D0;
extern struct tile_asset_cache *gUnknown_030012B8;

extern u8 sub_80232B8(void *self);
extern u8 sub_8023290(void *self);
extern u8 *sub_8023404(void *self);
extern void sub_801EB04(s32 a, u16 b, u16 c, u16 d);
extern void sub_8028474(void *state);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern u8 sub_8006DF8(struct tile_asset_cache *self, s32 recordId);
extern void sub_8006D08(struct tile_asset_cache *self, s32 slot, s32 recordId);
extern void sub_8006DA0(struct tile_asset_cache *self, s32 index);

#if NON_MATCHING
/* Record 47's periodic-trigger setter (docs/rom_map.md, "An
 * achievement/unlock-icon spawner family, tied to gStaticData_084A5600
 * record 47") - `sub_8022F2C` is its decrementer/consumer.
 *
 * NOT YET BYTE-MATCHING: every field offset, call, and argument is
 * confirmed correct, but the ROM keeps `&gUnknown_030012D0` and
 * `&gUnknown_030012B8` alive across the `sub_8006DF8` call in `r4`/
 * `r7` (only 4 low registers total, `r4`'s slot reused from the now-
 * dead `seconds` parameter) while this compiler's natural allocation
 * spills one of them to `r8` instead. Explicit register pins for all
 * of `self`/`seconds`/the two cached globals/`slot` (mirroring the
 * ROM's exact map) were tried and made things worse - the pinned
 * `slot` (`u8`, r6) picks up a spurious truncate-and-remask at every
 * read the ROM only does once at its assignment, and a stray stack
 * spill appears for the `0x234` offset constant - so parked with the
 * naturally-allocated (but `r8`-using) version instead of a version
 * that's both non-matching and has extra, harder-to-explain
 * mismatches. */
void sub_8022EA8(void *self, s32 seconds)
{
    void *p3, *header, *record;
    u8 recordId, slot;
    void *level;

    PlaySfx(gUnknown_030012BC, 0x18, 0x100);

    *(s32 *)((u8 *)self + 0xa0) += seconds * 60;

    p3 = *(void **)gUnknown_030012D0;
    header = *(void **)p3;
    record = *(void **)((u8 *)header + 0x234);
    recordId = *((u8 *)record + 0x30);
    slot = sub_8006DF8(gUnknown_030012B8, recordId);

    p3 = *(void **)gUnknown_030012D0;
    header = *(void **)p3;
    record = *(void **)((u8 *)header + 0x234);
    recordId = *((u8 *)record + 0x84);
    sub_8006D08(gUnknown_030012B8, slot, recordId);

    level = *(void **)((u8 *)self + 0xdc);
    if (*(s32 *)((u8 *)level + 8) == 3) {
        sub_8006DA0(gUnknown_030012B8, slot);
    }
}
#endif

#if NON_MATCHING
/* Countdown-gated periodic event trigger (docs/rom_map.md, "A per-level
 * completion-time cascade..."): decrements `self+0xa0`'s countdown and,
 * on reaching 0, fires record 47's spawn (`sub_8022EA8`'s sibling,
 * reusing `+0x30` for both the lookup and the slot-fill argument this
 * time). While the countdown is already 0, instead runs a cascading
 * digit-counter carry over `self+0x9c`/`0x98`/`0x94`/`0x90` (thresholds
 * `5`/`9`/`0x3b`/`0x63`) - shaped like a minutes:seconds:centiseconds
 * odometer, saturating (not wrapping) once the top field hits its cap.
 *
 * NOT YET BYTE-MATCHING: the countdown-carry half (the `else` branch)
 * already reproduces the ROM's exact branch topology; the trigger half
 * hits the identical `sub_8006DF8` cross-call register-spill issue
 * documented on `sub_8022EA8` above (same two cached globals, same
 * `r8` spill) - parked for the same reason, not re-explained twice. */
void sub_8022F2C(void *self)
{
    s32 countdown = *(s32 *)((u8 *)self + 0xa0);

    if (countdown != 0) {
        countdown--;
        *(s32 *)((u8 *)self + 0xa0) = countdown;

        if (countdown == 0) {
            void *p3, *header, *record;
            u8 recordId, slot;
            void *level;

            p3 = *(void **)gUnknown_030012D0;
            header = *(void **)p3;
            record = *(void **)((u8 *)header + 0x234);
            recordId = *((u8 *)record + 0x30);
            slot = sub_8006DF8(gUnknown_030012B8, recordId);

            p3 = *(void **)gUnknown_030012D0;
            header = *(void **)p3;
            record = *(void **)((u8 *)header + 0x234);
            recordId = *((u8 *)record + 0x30);
            sub_8006D08(gUnknown_030012B8, slot, recordId);

            level = *(void **)((u8 *)self + 0xdc);
            if (*(s32 *)((u8 *)level + 8) == 3) {
                sub_8006DA0(gUnknown_030012B8, slot);
            }
        }
    } else {
        s32 *c1 = (s32 *)((u8 *)self + 0x9c);

        if (*c1 != 5) {
            *c1 += 1;
        } else {
            s32 *c2 = (s32 *)((u8 *)self + 0x98);

            if (*c2 != 9) {
                *c2 += 1;
            } else {
                s32 *c3 = (s32 *)((u8 *)self + 0x94);

                if (*c3 != 0x3b) {
                    *c3 += 1;
                } else {
                    s32 *c4 = (s32 *)((u8 *)self + 0x90);

                    if (*c4 == 0x63) {
                        return;
                    }
                    *c4 += 1;
                    *c3 = countdown;
                }
                *c2 = 0;
            }
            *c1 = 0;
        }
    }
}
#endif

void sub_8022FEC(void *self)
{
    *(s32 *)((u8 *)self + 0x70) += 1;

    if (*(s32 *)((u8 *)self + 0x70) == *(s32 *)((u8 *)self + 0xbc)) {
        if (!sub_80232B8(self) && !sub_8023290(self)) {
            void *level = *(void **)((u8 *)self + 0xdc);

            if (*(s32 *)((u8 *)level + 8) == 3) {
                u8 *flags = sub_8023404(self);
                /* Register pins reproduce the ROM's "build the OR
                 * mask before loading the byte" order - a plain
                 * `*flags |= 2;` loads the byte first regardless of
                 * statement order (see docs/workflow.md step 7). */
                register s32 mask asm("r1") = 2;
                register s32 value asm("r2") = *flags;
                mask |= value;
                *flags = mask;
            } else {
                u16 b = *(u16 *)((u8 *)self + 0x1c0);
                u16 c = *(u16 *)((u8 *)self + 0x1c4);
                sub_801EB04(0xffff, b, c, 0);
            }
        }
    }

    if (*((u8 *)self + 0x8c) == 0) {
        sub_8028474(gUnknown_03001318);
    }
}

void sub_802306C(void *self)
{
    *((u8 *)self + 0xa9) = 1;

    if (sub_80232B8(self)) {
        *(s32 *)((u8 *)self + 0xb4) += *(s32 *)((u8 *)self + 0xac);
        return;
    }

    *(s32 *)((u8 *)self + 0x70) += *(s32 *)((u8 *)self + 0xac);

    if (*(s32 *)((u8 *)self + 0x70) != *(s32 *)((u8 *)self + 0xbc)) {
        return;
    }

    if (!sub_80232B8(self) && !sub_8023290(self)) {
        void *level = *(void **)((u8 *)self + 0xdc);

        if (*(s32 *)((u8 *)level + 8) == 3) {
            u8 *flags = sub_8023404(self);
            register s32 mask asm("r1") = 2;
            register s32 value asm("r2") = *flags;
            mask |= value;
            *flags = mask;
        } else {
            u16 b = *(u16 *)((u8 *)self + 0x1c0);
            u16 c = *(u16 *)((u8 *)self + 0x1c4);
            sub_801EB04(0xffff, b, c, 0);
        }
    }
}

void *sub_8023104(void *self)
{
    return *(void **)((u8 *)self + 0x1b8);
}

void sub_8023110(void *self, s32 value)
{
    *(s32 *)((u8 *)self + 0x88) = value;
}

void sub_8023118(void *self, s32 value)
{
    *(s32 *)((u8 *)self + 0x84) = value;
}

void sub_8023120(void *self, s32 value)
{
    *(s32 *)((u8 *)self + 0x80) = value;
}

s32 sub_8023128(void *self)
{
    return *(s32 *)((u8 *)self + 0x88);
}

s32 sub_8023130(void *self)
{
    return *(s32 *)((u8 *)self + 0x84);
}

s32 sub_8023138(void *self)
{
    return *(s32 *)((u8 *)self + 0x80);
}

void sub_8023140(void *self, s32 delta)
{
    *(s32 *)((u8 *)self + 0xac) += delta;
}

void sub_802314C(void *self, s32 mask)
{
    *(s32 *)((u8 *)self + 0xc0) |= mask;
}

s32 sub_8023158(void *self, s32 mask)
{
    s32 x = *(s32 *)((u8 *)self + 0xc0) & mask;
    return (u32)(-x | x) >> 31;
}

void sub_8023168(void *self)
{
    /* Register pins reproduce the ROM's exact accumulator/mask split -
     * see docs/workflow.md step 7 - a plain local otherwise lets gcc
     * reuse the still-live -0x11 constant to derive -0x41 via a single
     * SUB instead of a fresh mov+neg pair. */
    register s32 acc asm("r1") = -0x11;
    register s32 tmp asm("r2") = *((u8 *)self + 2);
    acc &= tmp;
    tmp = -0x41;
    acc &= tmp;
    tmp += 0x20;
    acc &= tmp;
    tmp = 0x7f;
    acc &= tmp;
    *((u8 *)self + 2) = acc;
}

/* Each of these four builds its OR mask into `r1` *before* loading the
 * byte into `r2` (the ROM's `movs r1,#N; ldrb r2,[r0,#2]` order) - a
 * plain `*flags |= N;` loads the byte first regardless of statement
 * order, so the mask/value roles are pinned explicitly (see
 * docs/workflow.md step 7, and the identical fix on `sub_8022FEC`/
 * `sub_802306C`'s `*flags |= 2;` above). */
void sub_8023184(void *self)
{
    register s32 mask asm("r1") = 0x40;
    register s32 value asm("r2") = *((u8 *)self + 2);
    mask |= value;
    *((u8 *)self + 2) = mask;
}

void sub_8023190(void *self)
{
    register s32 mask asm("r1") = 0x20;
    register s32 value asm("r2") = *((u8 *)self + 2);
    mask |= value;
    *((u8 *)self + 2) = mask;
}

void sub_802319C(void *self)
{
    register s32 mask asm("r1") = 0x10;
    register s32 value asm("r2") = *((u8 *)self + 2);
    mask |= value;
    *((u8 *)self + 2) = mask;
}

void sub_80231A8(void *self)
{
    register s32 mask asm("r1") = 0x80;
    register s32 value asm("r2") = *((u8 *)self + 2);
    mask |= value;
    *((u8 *)self + 2) = mask;
}

/* These three read a single flag bit back out of `self+2` as a plain
 * 0/1 value. Writing them as `(x >> n) & 1` compiles an extra `and`
 * this compiler doesn't need - the ROM instead isolates the bit by
 * shifting it up into the sign bit and shifting back down unsigned,
 * the same branchless idiom already used for `sub_8023158`'s
 * `!= 0` test (see docs/workflow.md step 7 / matching.md). */
s32 sub_80231B4(void *self)
{
    return (u32)(*((u8 *)self + 2) << 25) >> 31;
}

s32 sub_80231BC(void *self)
{
    return (u32)(*((u8 *)self + 2) << 26) >> 31;
}

s32 sub_80231C4(void *self)
{
    return (u32)(*((u8 *)self + 2) << 27) >> 31;
}
