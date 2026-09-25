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

/* Record 47's periodic-trigger setter (docs/rom_map.md, "An
 * achievement/unlock-icon spawner family, tied to gStaticData_084A5600
 * record 47") - `sub_8022F2C` is its decrementer/consumer.
 *
 * The ROM keeps `&gUnknown_030012D0` and `&gUnknown_030012B8` alive
 * across the `sub_8006DF8` call in `r4`/`r7` (only 4 low registers
 * total, `r4`'s slot reused from the now-dead `seconds` parameter).
 * Blanket register pins for all of `self`/`seconds`/the two cached
 * globals/`slot` (mirroring the ROM's map directly) made things worse
 * - a pinned `slot` picked up a spurious truncate-and-remask on every
 * read, and a stray stack spill appeared for the `0x234` offset
 * constant. What actually closes it: 1) a `base` local snapshotting
 * `gUnknown_030012B8`'s value *before* the first chase (not inline in
 * the call), so its evaluation lands in `r0` early exactly like the
 * ROM's `ldr r0,[r7]` and the chase is forced into `r1`; 2) a single
 * `register s32 off asm("r2")` pin for the `0x8d << 2` (`0x234`) field
 * offset in the *first* chase only, matching the ROM's `movs
 * r2,#0x8d; lsls r2,r2,#2` - this also stops gcc from caching that
 * constant in a register across the `sub_8006DF8` call, which is what
 * was pushing something else into `r8`; 3) fresh, differently-named
 * locals (`p3b`/`headerb`/`recordb`) for the *second* chase instead of
 * reusing `p3`/`header`/`record` - reusing the same C variable names
 * across both chases made gcc "stick" the second chase's registers to
 * the first's choice instead of letting `r0`/`r1` fall out naturally
 * (`r0` is free again there since the first call's result is already
 * in `slot`/r6). No explicit pin is needed for `self`, `cache1`
 * (`&gUnknown_030012B8`), or `slot` - they land in `r5`/`r7`/`r6`
 * purely from the resulting register pressure, matching the ROM
 * exactly. */
void sub_8022EA8(void *self, s32 seconds)
{
    register s32 off asm("r2");
    struct tile_asset_cache *base;
    void *p3, *header, *record;
    void *p3b, *headerb, *recordb;
    u8 recordId, slot;
    void *level;

    PlaySfx(gUnknown_030012BC, 0x18, 0x100);

    *(s32 *)((u8 *)self + 0xa0) += seconds * 60;

    base = gUnknown_030012B8;
    p3 = *(void **)gUnknown_030012D0;
    header = *(void **)p3;
    off = 0x8d << 2;
    record = *(void **)((u8 *)header + off);
    recordId = *((u8 *)record + 0x30);
    slot = sub_8006DF8(base, recordId);

    p3b = *(void **)gUnknown_030012D0;
    headerb = *(void **)p3b;
    recordb = *(void **)((u8 *)headerb + 0x234);
    recordId = *((u8 *)recordb + 0x84);
    sub_8006D08(gUnknown_030012B8, slot, recordId);

    level = *(void **)((u8 *)self + 0xdc);
    if (*(s32 *)((u8 *)level + 8) == 3) {
        sub_8006DA0(gUnknown_030012B8, slot);
    }
}

/* Countdown-gated periodic event trigger (docs/rom_map.md, "A per-level
 * completion-time cascade..."): decrements `self+0xa0`'s countdown and,
 * on reaching 0, fires record 47's spawn (`sub_8022EA8`'s sibling,
 * reusing `+0x30` for both the lookup and the slot-fill argument this
 * time). While the countdown is already 0, instead runs a cascading
 * digit-counter carry over `self+0x9c`/`0x98`/`0x94`/`0x90` (thresholds
 * `5`/`9`/`0x3b`/`0x63`) - shaped like a minutes:seconds:centiseconds
 * odometer, saturating (not wrapping) once the top field hits its cap.
 *
 * The trigger half uses the same `sub_8022EA8` register-pinning recipe
 * (see its comment above) for the `sub_8006DF8`/`sub_8006D08` cross-
 * call pair. Two more pins close the rest: `addr`/`countdown` pinned
 * to `r1`/`r3` reproduce the ROM's exact front-of-function map (the
 * countdown pointer and its loaded value), and that same `addr`
 * register variable is *reused* (reassigned, not redeclared) for the
 * digit-cascade's own address-chasing in the `else` branch, which is
 * what makes gcc emit the ROM's `subs r1,#4` chain-decrement instead
 * of recomputing `self+0x98`/`self+0x94` fresh from `self` each time.
 * `newCountdown` is pinned to `r0` because otherwise gcc decrements
 * `countdown`'s own register (`r3`) in place - functionally fine since
 * the two branches are mutually exclusive, but a different instruction
 * encoding (`subs r3,#1` vs the ROM's `subs r0,r3,#1`) than the ROM's.
 * The digit-cascade's four levels are each written test-true-first
 * (`if (val == N) { nested / return } else { val + 1 }`) rather than
 * test-false-first (`if (val != N) { val + 1 } else { nested }`) -
 * despite being logically identical, this flips which branch gcc lays
 * out inline vs at the end of the function, and only the true-first
 * form reproduces the ROM's block order (all four "plain increment"
 * cases grouped at the tail via fall-through). Each level also caches
 * its loaded field value in a named local (`val1`/`val2`/`val3`)
 * rather than re-reading `*addr` for the increment - without that, gcc
 * re-emits a redundant `ldr` in the `else` (increment) arm instead of
 * reusing the register the comparison already loaded. */
void sub_8022F2C(void *self)
{
    register s32 *addr asm("r1") = (s32 *)((u8 *)self + 0xa0);
    register s32 countdown asm("r3") = *addr;

    if (countdown != 0) {
        register s32 newCountdown asm("r0") = countdown - 1;
        *addr = newCountdown;

        if (newCountdown == 0) {
            register s32 off asm("r2");
            struct tile_asset_cache *base;
            void *p3, *header, *record;
            void *p3b, *headerb, *recordb;
            u8 recordId, slot;
            void *level;

            base = gUnknown_030012B8;
            p3 = *(void **)gUnknown_030012D0;
            header = *(void **)p3;
            off = 0x8d << 2;
            record = *(void **)((u8 *)header + off);
            recordId = *((u8 *)record + 0x30);
            slot = sub_8006DF8(base, recordId);

            p3b = *(void **)gUnknown_030012D0;
            headerb = *(void **)p3b;
            recordb = *(void **)((u8 *)headerb + 0x234);
            recordId = *((u8 *)recordb + 0x30);
            sub_8006D08(gUnknown_030012B8, slot, recordId);

            level = *(void **)((u8 *)self + 0xdc);
            if (*(s32 *)((u8 *)level + 8) == 3) {
                sub_8006DA0(gUnknown_030012B8, slot);
            }
        }
    } else {
        s32 *save1;
        s32 val1;

        addr = (s32 *)((u8 *)self + 0x9c);
        val1 = *addr;
        save1 = addr;

        if (val1 == 5) {
            s32 *save2;
            s32 val2;

            addr -= 1;
            val2 = *addr;
            save2 = addr;

            if (val2 == 9) {
                s32 val3;

                addr -= 1;
                val3 = *addr;

                if (val3 == 0x3b) {
                    s32 *c4 = (s32 *)((u8 *)self + 0x90);

                    if (*c4 == 0x63) {
                        return;
                    }
                    *c4 += 1;
                    *addr = countdown;
                } else {
                    *addr = val3 + 1;
                }
                *save2 = 0;
            } else {
                *addr = val2 + 1;
            }
            *save1 = 0;
        } else {
            *addr = val1 + 1;
        }
    }
}

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

/* GitHub issues #35/#36: 0x080231CC-0x08023488, the remainder of the
 * UpdateGameFrame-MainLoop cluster's "level" object accessor family
 * (`gUnknown_030012C0`) - fully contiguous with the functions above (no
 * ldscript.txt change needed, this is still the same self type and
 * still the same object file). See docs/matching/issue-35-36-0x080231cc-game-loop.md
 * for the full write-up. Bit-7 getter for the `self+2` flags byte this
 * file's own family already covers bits 4-6 of. */
s32 sub_80231CC(void *self)
{
    return *((u8 *)self + 2) >> 7;
}

/* self+0x70/self+0xbc form a counter/threshold pair (`sub_8022FEC`/
 * `sub_802306C` above, `sub_8023484` below); this resets the counter. */
void sub_80231D4(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x70) = 0;
}

void sub_80231DC(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x6c) = 0;
}

void sub_80231E4(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x74) = 5;
}

struct AudioContext;
extern void sub_80017BC(struct AudioContext *self, u32 songIndex);
extern void sub_8024498(void *self);

/* `self+0x78` is a small "last state" latch: state `3` always fires a
 * jingle (`sub_80017BC(gUnknown_030012BC, 0x12)`) and skips the rest;
 * any other state re-fires `sub_8024498(self+0xc4)` once, the first
 * time it's seen (`self+0x78 == 3` guards a one-shot transition out of
 * state 3). Either way `self+0x78` ends up holding `state`. */
void sub_80231EC(void *selfArg, s32 stateArg)
{
    /* Register-pinned so the ROM's own `adds r4,r0,#0` (self) /
     * `adds r5,r1,#0` (state) copy order is reproduced - a plain pair
     * of locals lets this compiler swap the order since `state` is
     * referenced first, in the `if` condition below. */
    register u8 *self asm("r4") = selfArg;
    register s32 state asm("r5") = stateArg;

    if (state == 3) {
        sub_80017BC(gUnknown_030012BC, 0x12);
    } else if (*(s32 *)(self + 0x78) == 3) {
        *(s32 *)(self + 0x78) = state;
        sub_8024498(self + 0xc4);
    }
    *(s32 *)(self + 0x78) = state;
}

/* Plain setter for the same `self+0x74` field `sub_80231E4` above
 * hardcodes to `5`. */
void sub_8023220(void *selfArg, s32 value)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x74) = value;
}

/* Advances the `self+0x78` latch by one via `sub_80231EC`. */
void sub_8023224(void *selfArg)
{
    u8 *self = selfArg;
    s32 next = *(s32 *)(self + 0x78) + 1;
    sub_80231EC(self, next);
}

extern void sub_80284A4(void *state);

/* While `self+0x8c` is clear: decrements `self+0x74` and pings
 * `gUnknown_03001318` (`sub_80284A4`) once it reaches (or passes) zero. */
void sub_8023234(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x8c] == 0) {
        s32 v = *(s32 *)(self + 0x74) - 1;
        *(s32 *)(self + 0x74) = v;

        if (v >= 0) {
            sub_80284A4(gUnknown_03001318);
        }
    }
}

/* Getter for `self+0x6c`, the counter `sub_80231DC` clears. */
s32 sub_802325C(void *self)
{
    return *(s32 *)((u8 *)self + 0x6c);
}

/* Getters for the "seconds"/"minutes" tier of the digit-cascade odometer
 * `sub_8022F2C` above already documents (`self+0x9c`/`0x98`/`0x94`/
 * `0x90`, thresholds 5/9/0x3b/0x63) - this trio covers its bottom three
 * tiers. */
s32 sub_8023260(void *self)
{
    return *(s32 *)((u8 *)self + 0x98);
}

s32 sub_8023268(void *self)
{
    return *(s32 *)((u8 *)self + 0x94);
}

s32 sub_8023270(void *self)
{
    return *(s32 *)((u8 *)self + 0x90);
}

/* `self+0xa4`-`self+0xa9`: a bank of six busy/status-flag bytes, each
 * with a getter and a clear (some also a set-to-1) - same shape as the
 * `self+2` bitfield family above, just laid out as whole bytes instead
 * of packed bits. */
u8 sub_8023278(void *self)
{
    return *((u8 *)self + 0xa7);
}

void sub_8023280(void *selfArg)
{
    u8 *self = selfArg;
    self[0xa7] = 0;
}

void sub_8023288(void *selfArg)
{
    u8 *self = selfArg;
    self[0xa7] = 1;
}

u8 sub_8023290(void *self)
{
    return *((u8 *)self + 0xa6);
}

void sub_8023298(void *selfArg)
{
    u8 *self = selfArg;
    self[0xa6] = 0;
}

u8 sub_80232A0(void *self)
{
    return *((u8 *)self + 0xa5);
}

void sub_80232A8(void *selfArg)
{
    u8 *self = selfArg;
    self[0xa5] = 0;
}

void sub_80232B0(void *selfArg)
{
    u8 *self = selfArg;
    self[0xa5] = 1;
}

u8 sub_80232B8(void *self)
{
    return *((u8 *)self + 0xa4);
}

void sub_80232C0(void *selfArg)
{
    u8 *self = selfArg;
    self[0xa4] = 0;
}

u8 sub_80232C8(void *self)
{
    return *((u8 *)self + 0xa9);
}

void sub_80232D0(void *selfArg)
{
    u8 *self = selfArg;
    self[0xa9] = 0;
}

void sub_80232D8(void *selfArg)
{
    u8 *self = selfArg;
    self[0x8c] = 0;
}

/* `self+0x7c`: a plain free-running counter, get/increment/reset trio. */
s32 sub_80232E0(void *self)
{
    return *(s32 *)((u8 *)self + 0x7c);
}

void sub_80232E4(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x7c) = *(s32 *)(self + 0x7c) + 1;
}

void sub_80232EC(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x7c) = 0;
}

u8 sub_80232F4(void *self)
{
    return *((u8 *)self + 0xa8);
}

void sub_80232FC(void *selfArg)
{
    u8 *self = selfArg;
    self[0xa8] = 0;
}

/* "Start" helper: resets `self+0x7c`'s counter then sets `self+0xa8`'s
 * flag. */
void sub_8023304(void *selfArg)
{
    u8 *self = selfArg;
    sub_80232EC(self);
    self[0xa8] = 1;
}

/* Plain setter for a fourth word-sized field at `self+0x1c8`, right
 * after the `self+0x1c0`/`0x1c4` pair `sub_8023484` below reads. */
void sub_8023318(void *selfArg, s32 value)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x1c8) = value;
}

/* Plain getter/getter/setter trio for `self+0xc8`/`self+0xc4` - the
 * latter is the "current index" field `sub_8023378`/`sub_80233B4`/
 * `sub_8023404`/`sub_8023418` below all read. */
s32 sub_8023324(void *self)
{
    return *(s32 *)((u8 *)self + 0xc8);
}

s32 sub_802332C(void *self)
{
    return *(s32 *)((u8 *)self + 0xc4);
}

void sub_8023334(void *selfArg, s32 value)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0xc4) = value;
}

extern s32 sub_8024428(s32 idx);
extern s32 sub_8024434(s32 idx);
extern s32 sub_8024440(s32 idx);
extern s32 sub_802444C(s32 idx);
extern s32 sub_8024458(s32 idx);

/* Five thin two-argument wrappers that drop `self` entirely and forward
 * straight to one of `sub_8024428`/`34`/`40`/`4C`/`58` (the medal
 * "flag index" wrappers, `game_loop18.c`). */
s32 sub_802333C(void *self, s32 idx)
{
    return sub_8024428(idx);
}

s32 sub_8023348(void *self, s32 idx)
{
    return sub_8024434(idx);
}

s32 sub_8023354(void *self, s32 idx)
{
    return sub_8024440(idx);
}

s32 sub_8023360(void *self, s32 idx)
{
    return sub_802444C(idx);
}

s32 sub_802336C(void *self, s32 idx)
{
    return sub_8024458(idx);
}

extern s32 sub_8033880(void);

/* Dispatches on `self+0xc4`'s "current index" field: index `0x15` fires
 * the actor-part singleton lifetime counter (`sub_8033880`,
 * `actor_part28.c`); indices `0x14`/`0x16`/`0x17` instead compute
 * `3 - (*(self+0x1c8))->0x10` (the fourth word-field `sub_8023318`
 * above sets, apparently itself a pointer to a small record); anything
 * else returns `0`. */
s32 sub_8023378(void *selfArg)
{
    u8 *self = selfArg;
    s32 idx = *(s32 *)(self + 0xc4);

    switch (idx) {
    case 0x15:
        return sub_8033880();
    case 0x14: {
        void *p = *(void **)(self + 0x1c8);
        return 3 - *(s32 *)((u8 *)p + 0x10);
    }
    case 0x16: {
        void *p = *(void **)(self + 0x1c8);
        return 3 - *(s32 *)((u8 *)p + 0x10);
    }
    case 0x17: {
        void *p = *(void **)(self + 0x1c8);
        return 3 - *(s32 *)((u8 *)p + 0x10);
    }
    default:
        return 0;
    }
}

/* Same `self+0xc4` "current index" field, mapped through a 5-entry
 * table (`0x14`-`0x18`) to `{9, 8, 6, 7}` minus a shared `6` - index
 * `0x18` and anything outside `[0x14, 0x18]` both skip the shared
 * subtraction and return `-1` directly (the ROM's own `_080233F0`
 * case-4 slot points straight at `_080233F8`'s `bx lr`, bypassing
 * `_080233F6`'s `subs r0,#6` cases 0-3 share - a plain
 * `return 9 - 6;`-style fold collapses that shared instruction away,
 * so the subtraction has to stay a genuine runtime step). */
s32 sub_80233B4(void *selfArg)
{
    u8 *self = selfArg;
    s32 idx = *(s32 *)(self + 0xc4);
    s32 result;

    switch (idx - 0x14) {
    case 0:
        result = 9;
        break;
    case 1:
        result = 8;
        break;
    case 2:
        result = 6;
        break;
    case 3:
        result = 7;
        break;
    case 4:
    default:
        return -1;
    }
    return result - 6;
}

/* Address-of-slot helper: `self` is treated as the base of a packed
 * array of 4-byte records starting at offset `+4`, indexed by `idx`. */
u8 *sub_80233FC(void *selfArg, s32 idx)
{
    return (u8 *)selfArg + (idx * 4 + 4);
}

/* Resolves the "current index" field (`self+0xc4`) into its own slot
 * address via `sub_80233FC` - the address this file's `sub_8022FEC`/
 * `sub_802306C`/`sub_8023484` all call "flags" and OR a bit into. */
u8 *sub_8023404(void *selfArg)
{
    u8 *self = selfArg;
    s32 idx = *(s32 *)(self + 0xc4);
    return sub_80233FC(self, idx);
}

/* Getter for `self+0x70`, the counter `sub_80231D4` resets. */
s32 sub_8023414(void *self)
{
    return *(s32 *)((u8 *)self + 0x70);
}

/* Bit-0 getter on the record at `self + idx*4 + 0x150` (`idx` from
 * `self+0xc4`, the same "current index" field). */
s32 sub_8023418(void *selfArg)
{
    u8 *self = selfArg;
    s32 idx = *(s32 *)(self + 0xc4);
    u8 *addr = self + idx * 4 + 0x150;

    return (u32)(*addr << 31) >> 31;
}

extern void sub_80284D4(void *state);

/* `self+0x6c`/`self+0x74` centisecond/second odometer pair (distinct
 * from the `sub_8022F2C` cascade above): increments `self+0x6c`, and on
 * wrap past `0x63` resets it, bumps `self+0x74` (saturating at `0x62`),
 * and pings `gUnknown_03001318` via `sub_80284A4`; either way, always
 * pings it again via `sub_80284D4`. */
void sub_8023430(void *selfArg)
{
    u8 *self = selfArg;
    s32 v = *(s32 *)(self + 0x6c) + 1;

    *(s32 *)(self + 0x6c) = v;
    if (v > 0x63) {
        *(s32 *)(self + 0x6c) = 0;
        if (*(s32 *)(self + 0x74) <= 0x62) {
            *(s32 *)(self + 0x74) += 1;
        }
        sub_80284A4(gUnknown_03001318);
    }
    sub_80284D4(gUnknown_03001318);
}

/* Just the "bump `self+0x74`, ping `sub_80284A4`" half of
 * `sub_8023430` above, standalone. */
void sub_8023464(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0x74) <= 0x62) {
        *(s32 *)(self + 0x74) += 1;
    }
    sub_80284A4(gUnknown_03001318);
}

/* GitHub issue #37: closes the loop on the `self+0x1c0`/`0x1c4`
 * counter-notification chain (`docs/rom_map.md`'s "Coverage check and
 * eight more small reads" section) - the consumer/trigger side of the
 * 15-slot table's `sub_802209C` writer, forwarding into `sub_801EB04`
 * alongside `sub_802306C`/`sub_8022FEC`'s own threshold-cross paths
 * above (identical shape: gated by the same `self+0x70 == self+0xbc`
 * counter/threshold pair, `sub_80232B8`/`sub_8023290` readiness checks,
 * then either OR a bit into `sub_8023404`'s slot or forward
 * `self+0x1c0`/`0x1c4` to `sub_801EB04`). Only caller is
 * `sub_8023A1C`'s dispatch opener (`game_loop56.c`), which passes
 * `*gUnknown_030012C0` as `self`. */
void sub_8023484(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;

    if (*(s32 *)(self + 0x70) == *(s32 *)(self + 0xbc)
        && !sub_80232B8(self) && !sub_8023290(self)) {
        void *level = *(void **)(self + 0xdc);

        if (*(s32 *)((u8 *)level + 8) == 3) {
            u8 *flags = sub_8023404(self);
            register s32 mask asm("r1") = 2;
            register s32 value asm("r2") = *flags;
            mask |= value;
            *flags = mask;
        } else {
            /* Register-pinned to reproduce the ROM's exact map: `magic`
             * (the 3rd `sub_801EB04` argument's true value) loaded into
             * r0 early rather than right before the call, and `off`
             * kept in r3 across its own +4 increment instead of being
             * recomputed from scratch for the second field - see
             * docs/workflow.md step 7 / matching_decomp_register_pinning
             * memory. */
            register s32 magic asm("r0") = 0xffff;
            register s32 off asm("r3") = 0xe0 << 1;
            register u16 *addr1 asm("r1") = (u16 *)(self + off);
            u16 b = *addr1;
            register u16 *addr2 asm("r2");
            u16 c;
            asm volatile("" : "+r"(b));
            off += 4;
            asm volatile("" : "+r"(off));
            addr2 = (u16 *)(self + off);
            c = *addr2;
            sub_801EB04(magic, b, c, 0);
        }
    }
}
