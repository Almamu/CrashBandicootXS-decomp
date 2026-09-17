#include "core.h"
#include "memory.h"

/* Continues the same player/action-object action-table family already
 * documented in actor_part17.c/actor_part18.c/actor_part18b.c - `self`
 * is the same large per-instance object those files use (state at
 * `+0x28`, a table-index field at `+0xc`, an anim-frame halfword/byte
 * pair at `+0x10`/`+0x12`, a counter at `+0x44`, an accumulator at `+8`
 * that doubles as `struct anim_part_instance.field_08` for
 * `GetAnimFrameBaseOffset`, and a "part table" pointer at `+0`, the
 * same convention actor_part18.c documents at `self+0x10` for its own
 * object), plus a `+0x50`-rooted `{s16 offset; void *fn}` trampoline
 * record fed through `sub_803AD80`/`sub_803AD84` (the same convention
 * already named in actor_part10.c/actor_part11.c for a sibling "part"
 * object, just at a different fixed offset here) and a `+0x48`/`+0x4c`
 * circular doubly-linked-list pair (confirmed by `sub_802C19C`'s own
 * unlink sequence below) rooted at the player-pointer global
 * `gUnknown_03000884`. As with the other actor_part1[78].c files, none
 * of these objects' full shapes are pinned down yet, so every access
 * stays a raw offset with a doc comment rather than a guessed struct -
 * see docs/rom_map.md's "gUnknown_030014xx tier-threshold actor
 * family" and "type-byte event dispatch" sections for the semantics
 * behind the individual functions below. */

extern s32 gUnknown_030014A4;
extern u8 gUnknown_030014A3;
extern u8 gUnknown_030014A0;
extern u8 gUnknown_030014A1;
extern s32 gUnknown_0300148C;
extern void *gUnknown_03001494;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *gUnknown_03000884;
extern s32 gUnknown_03001488;
extern s32 gUnknown_03001484;
extern s32 gUnknown_0300149C;
extern void *gUnknown_030014B0[2];
extern void *gUnknown_03001490;

extern u8 gStaticData_087E4E54[];
extern u8 gStaticData_087E4DF4[];
extern u8 gStaticData_087E4E74[];
extern u8 gStaticData_0817A6B8[];
extern u8 gStaticData_0817A768[];

extern void sub_8029BAC(s32 arg0);
extern void sub_802DFBC(void);
extern void sub_802D490(void *arg0);
extern s32 sub_802D4EC(void *arg0);
extern s32 GetAnimFrameBaseOffset(void *self);
extern u8 *GetAnimFrameData(void *self);
extern void SetupSpriteFrameOam(u8 *frame, u32 arg1, u32 arg2, s32 priority);
extern s32 sub_803B060(void *self);
extern u8 gStaticData_087E4E94[];
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *addr, void *arg1, void *tableEntry, void *fn);
extern u8 sub_802A6EC(void *self);
extern void sub_802A7B8(void *self);
extern u8 sub_802DD9C(void *self);
extern void sub_8022FEC(void *self);
extern s32 sub_8023464(void *self);
extern void sub_8023430(void *self);
extern void sub_8028C48(void *arg0);
extern void sub_802AAB4(s32 arg0);
extern void sub_802B730(void *arg0);
extern void sub_8029720(void);
extern void *sub_802AC28(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void sub_802C7A8(void *self);

/* Accumulates `gUnknown_030014A4` into `self+0x20`, then drains
 * `gUnknown_030014A4` toward a fixed ceiling (`0x780`) - the same
 * "lazy-singleton accumulator" shape documented in docs/rom_map.md for
 * `sub_802B8E8`. Once `self+0x20` crosses a threshold (`0x2800`),
 * clamps it and fires the state-1/table-index-4 transition (anim frame
 * taken from `self`'s own part-table pointer at `+0x30`). */
void sub_802BED8(void *selfArg)
{
    u8 *self = selfArg;
    s32 total = *(s32 *)(self + 0x20) + gUnknown_030014A4;

    *(s32 *)(self + 0x20) = total;
    gUnknown_030014A4 += 0x60;
    if (gUnknown_030014A4 > 0x780) {
        gUnknown_030014A4 = 0x780;
    }

    if (total > 0x2800) {
        *(s32 *)(self + 0x20) = 0x2800;
        {
            register u8 *addr asm("r1") = &gUnknown_030014A3;
            register u8 val asm("r0") = 1;
            *addr = val;
        }
        sub_8029BAC(0x24);
        {
            register s32 stateVal asm("r0") = 1;
            register s32 idxVal asm("r1") = 4;
            *(s32 *)(self + 0x28) = stateVal;
            {
                register s32 zero asm("r2") = 0;
                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = idxVal;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x30);
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero2;
                }
                *(s32 *)(self + 8) = zero;
            }
        }
    }
}

/* On the "confirm" input edge (`self+0x12` set), plays a sound, resets
 * `gUnknown_030014A4` to a large negative "cooldown" value, and fires
 * the state-11/table-index-7 transition (anim frame from `self`'s
 * part-table pointer at `+0x54`). While `self+0x20` (the accumulator
 * `sub_802BED8` above drives) exceeds a threshold, additionally spawns
 * an effect object via `sub_802AC28` and stashes it into
 * `gUnknown_03001490`. */
void sub_802BF30(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x12] != 0) {
        PlaySfx(gUnknown_030012BC, 0x3c, 0x100);
        gUnknown_030014A4 = 0xFFFFF980;
        {
            register s32 stateVal asm("r0") = 0xb;
            register s32 idxVal asm("r1") = 7;

            *(s32 *)(self + 0x28) = stateVal;
            {
                register s32 zero asm("r5") = 0;

                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = idxVal;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x54);
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero2;
                }
                *(s32 *)(self + 8) = zero;

                if (*(s32 *)(self + 0x20) > 0x2000) {
                    gUnknown_03001490 = sub_802AC28(2, *(s32 *)(self + 0x1c), 0x2800,
                                                     *(s32 *)(self + 0x24), zero);
                }
            }
        }
    }
}

/* On the "confirm" input edge, sets `gUnknown_030014A3`/state-1/
 * table-index-0 (anim frame from `self`'s own part-table pointer at
 * `+0`) and fires `sub_8029BAC(0x24)` - the state-transition counterpart
 * to `sub_802BED8`, entered directly rather than through the
 * accumulator threshold. */
void sub_802BFA0(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;

    if (self[0x12] != 0) {
        {
            register u8 *addr asm("r1") = &gUnknown_030014A3;
            register u8 val asm("r0") = 1;
            *addr = val;
        }
        {
            register s32 stateVal asm("r0") = 1;
            register s32 zero asm("r2") = 0;

            *(s32 *)(self + 0x28) = stateVal;
            *(s32 *)(self + 0x44) = zero;
            *(s32 *)(self + 0xc) = zero;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
                register u8 zero2 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero2;
            }
            *(s32 *)(self + 8) = zero;
        }
        sub_8029BAC(0x24);
    }
}

/* Once-only latch (`gUnknown_030014A0`): arms a countdown
 * (`gUnknown_0300148C = 0x16`), runs `sub_8029BAC(0x24)`, clamps
 * `gUnknown_030014A4` to non-negative, then calls `sub_802DFBC` and
 * marks both `gUnknown_030014A0` and `gUnknown_030014A3`. */
void sub_802BFD4(void)
{
    if (gUnknown_030014A0 == 0) {
        gUnknown_0300148C = 0x16;
        sub_8029BAC(0x24);
        if (gUnknown_030014A4 < 0) {
            gUnknown_030014A4 = 0;
        }
        sub_802DFBC();
        gUnknown_030014A0 = 1;
        gUnknown_030014A3 = 0;
    }
}

/* Resets the `gUnknown_030014A3`/`030014A1`/`030014A0` latch trio, runs
 * `sub_802D490` on `gUnknown_03001494`, and fires the state-7/table-
 * index-6 transition (anim frame from `self`'s part-table pointer at
 * `+0x48`) plus a sound cue. */
void sub_802C018(void *selfArg)
{
    u8 *self = selfArg;

    gUnknown_030014A3 = 0;
    gUnknown_030014A1 = 1;
    sub_802D490(gUnknown_03001494);
    gUnknown_030014A0 = 1;

    {
        register s32 stateVal asm("r0") = 7;
        register s32 idxVal asm("r1") = 6;

        *(s32 *)(self + 0x28) = stateVal;
        {
            register s32 zero asm("r2") = 0;

            *(s32 *)(self + 0x44) = zero;
            *(s32 *)(self + 0xc) = idxVal;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x48);
                register u8 zero2 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero2;
            }
            *(s32 *)(self + 8) = zero;
        }
    }

    PlaySfx(gUnknown_030012BC, 0x41, 0x100);
}

/* Accumulator: while `gUnknown_030012C0+0x8c` is clear, arms
 * `gUnknown_03001484` (once, on the first accumulation) and adds
 * `delta` into `gUnknown_03001488`. Ignores its own first (player-
 * pointer) argument entirely - see docs/rom_map.md's correction on
 * this function. */
void sub_802C078(void *arg0, s32 delta)
{
    if (*((u8 *)gUnknown_030012C0 + 0x8c) == 0) {
        if (gUnknown_03001488 == 0) {
            gUnknown_03001484 = 0xf;
        }
        gUnknown_03001488 += delta;
    }
}

/* Trivial forwarder - ignores its own argument and calls
 * `sub_8023464(gUnknown_030012C0)`, per docs/rom_map.md's correction
 * (the ROM's own tail-call epilogue clobbers r0/the call's result, so
 * this is void, not passed through as a return value). */
void sub_802C0A8(void *arg0)
{
    sub_8023464(gUnknown_030012C0);
}

/* Only runs while `self+0x28` (state) is 1-3: sets table-index 2, anim
 * frame from `self`'s part-table pointer at `+0x18`, and - once the
 * frame counter reaches the entry's threshold (the same `+4`-halfword-
 * of-a-0xc-stride-table shape as `sub_802C270` below) - resets the
 * `+8` accumulator. Stashes `arg1` into `self+0x1c`, plays a
 * state-keyed sound cue (0x5a for state 2, 0x55 for state 1), and
 * transitions to state 3. */
void sub_802C0BC(void *selfArg, s32 arg1param)
{
    register u8 *self asm("r4") = selfArg;
    register s32 arg1 asm("r5") = arg1param;

    if ((u32)(*(s32 *)(self + 0x28) - 1) <= 2) {
        s32 frame;

        *(s32 *)(self + 0xc) = 2;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x18);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }

        frame = GetAnimFrameBaseOffset(self);
        {
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

        *(s32 *)(self + 0x1c) = arg1;

        {
            s32 switchState = *(s32 *)(self + 0x28);

            if (switchState == 2) {
                sub_8029BAC(0x5a);
            } else if (switchState == 1) {
                sub_8029BAC(0x55);
            }
        }

        *(s32 *)(self + 0x28) = 3;
        *(s32 *)(self + 0x44) = 0;
        gUnknown_030014A3 = 0;
    }
}

/* Lock-timer setter: while `sub_802D4EC(gUnknown_03001494)` returns 3,
 * arms `gUnknown_0300149C = 500` - the same lock/active flag
 * `sub_802B7E0` gates on, per docs/rom_map.md. */
void sub_802C128(void *arg0)
{
    if (sub_802D4EC(gUnknown_03001494) == 3) {
        gUnknown_0300149C = 500;
    }
}

/* While `self+0x28` (state) is 1-3 and `gUnknown_0300149C` (the same
 * lock/active flag `sub_802B7E0` gates on, per docs/rom_map.md) is
 * clear: transitions to state 5/table-index 3 (anim frame from
 * `self`'s part-table pointer at `+0x24`), resets
 * `gUnknown_030014A4` to `-0x780`, and fires `sub_8029BAC(0x1c)`. */
void sub_802C14C(void *selfArg)
{
    register u8 *self asm("r2") = selfArg;

    if ((u32)(*(s32 *)(self + 0x28) - 1) <= 2) {
        register s32 flag asm("r3") = gUnknown_0300149C;

        if (flag == 0) {
            gUnknown_030014A3 = flag;
            {
                register s32 stateVal asm("r0") = 5;
                register s32 idxVal asm("r1") = 3;

                *(s32 *)(self + 0x28) = stateVal;
                *(s32 *)(self + 0x44) = flag;
                *(s32 *)(self + 0xc) = idxVal;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x24);
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero2;
                }
                *(s32 *)(self + 8) = flag;
            }
            gUnknown_030014A4 = 0xFFFFF880;
            sub_8029BAC(0x1c);
        }
    }
}

/* Teardown, gated by `arg1` bit 0: temporarily swaps `self+0x50`'s
 * vtable to `gStaticData_087E4E54` to run `gUnknown_03001488` drain
 * calls into `sub_8023430(gUnknown_030012C0)`, runs two
 * `sub_8028C48` cleanup calls on `gUnknown_030014B0[0]`/`[1]`, sets
 * `self+0x50` to the "dead" vtable `gStaticData_087E4DF4`, unlinks
 * `self` from the circular `+0x48`(next)/`+0x4c`(prev) list, and frees
 * `self` when `arg1 & 1`. */
void sub_802C19C(void *selfArg, u32 arg1param)
{
    register u8 *self asm("r5") = selfArg;
    u32 arg1 = arg1param;

    *(u8 **)(self + 0x50) = gStaticData_087E4E54;

    if (gUnknown_03001488 != 0) {
        do {
            sub_8023430(gUnknown_030012C0);
            gUnknown_03001488 -= 1;
        } while (gUnknown_03001488 != 0);
    }

    sub_8028C48(gUnknown_030014B0[0]);
    sub_8028C48(gUnknown_030014B0[1]);

    *(u8 **)(self + 0x50) = gStaticData_087E4DF4;

    {
        u8 *prev = *(u8 **)(self + 0x4c);
        u8 *next = *(u8 **)(self + 0x48);
        *(u8 **)(prev + 0x48) = next;
    }
    {
        u8 *next = *(u8 **)(self + 0x48);
        u8 *prev = *(u8 **)(self + 0x4c);
        *(u8 **)(next + 0x4c) = prev;
    }

    if (arg1 & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");
