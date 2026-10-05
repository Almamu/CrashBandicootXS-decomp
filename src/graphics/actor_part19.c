#include "core.h"
#include "memory.h"
#include "actor_self.h"

/* Continues the same player/action-object action-table family already
 * documented in actor_part17.c/actor_part18.c/actor_part18b.c - `self`
 * is the same large per-instance object those files use (state at
 * `+0x28`, a table-index field at `+0xc`, an anim-frame halfword/byte
 * pair at `+0x10`/`+0x12`, a counter at `+0x44`, an accumulator at `+8`
 * that doubles as `struct anim_part_instance.field_08` for
 * `GetAnimFrameBaseOffset`, and a "part table" pointer at `+0`, the
 * same convention actor_part18.c documents at `animTimer` for its own
 * object), plus a `+0x50`-rooted `{s16 offset; void *fn}` trampoline
 * record fed through `_call_via_r2`/`_call_via_r3` (the same convention
 * already named in actor_part10.c/actor_part11.c for a sibling "part"
 * object, just at a different fixed offset here) and a `+0x48`/`+0x4c`
 * circular doubly-linked-list pair (confirmed by `DestroyPolarPlayer`'s own
 * unlink sequence below) rooted at the player-pointer global
 * `gActorList`. `self` is `struct actor_self` (actor_self.h);
 * the functions not yet converted still use raw offsets into it - see
 * docs/rom_map.md's "gUnknown_030014xx tier-threshold actor
 * family" and "type-byte event dispatch" sections for the semantics
 * behind the individual functions below. */

extern s32 gUnknown_030014A4;
extern u8 gUnknown_030014A3;
extern u8 gUnknown_030014A0;
extern u8 gUnknown_030014A1;
extern s32 gUnknown_0300148C;
extern void *gPolarAkuAku;
extern void *gAudioContext;
extern void *gLevelState;
extern void *gActorList;
extern s32 gPolarQueuedWumpa;
extern s32 gUnknown_03001484;
extern s32 gUnknown_0300149C;
extern void *gPolarPlayerTiles[2];
extern void *gUnknown_03001490;

extern u8 gPolarPlayerVtable[];
extern u8 gActorVtable[];
extern u8 gPolarCollectedWumpaVtable[];
extern u8 gPolarPlayerStateFuncs[];
extern u8 gPolarNitroCrateBox[];

extern void SetCellAnimSpeed(s32 arg0);
extern void StopYeti(void);
extern void sub_802D490(void *arg0);
extern s32 AddPolarAkuAkuMask(void *arg0);
extern s32 GetAnimFrameBaseOffset(void *self);
extern u8 *GetAnimFrameData(void *self);
extern void SetupSpriteFrameOam(u8 *frame, u32 arg1, u32 arg2, s32 priority);
extern s32 GetAnimFrameAttr(void *self);
extern u8 gPolarWumpaVtable[];
extern s32 __divsi3(s32 arg0, s32 arg1);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *addr, void *arg1, void *tableEntry, void *fn);
extern u8 IsTouchingPlayer(void *self);
extern void UpdateActor(void *self);
extern u8 sub_802DD9C(void *self);
extern void AddBrokenCrate(void *self);
extern s32 AddLife(void *self);
extern void CollectWumpa(void *self);
extern void FreeVramTileBlock(void *arg0);
extern void MarkSpawnCollected(s32 arg0);
extern void HurtPolarPlayer(void *arg0);
extern void sub_8029720(void);
extern void *CreateActor(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void DetonateNearbyPolarNitros(void *self);

/* Accumulates `gUnknown_030014A4` into `y`, then drains
 * `gUnknown_030014A4` toward a fixed ceiling (`0x780`) - the same
 * "lazy-singleton accumulator" shape documented in docs/rom_map.md for
 * `sub_802B8E8`. Once `y` crosses a threshold (`0x2800`),
 * clamps it and fires the state-1/table-index-4 transition (anim frame
 * taken from `self`'s own part-table pointer at `+0x30`). */
void sub_802BED8(void *selfArg)
{
    struct actor_self *self = selfArg;
    s32 total = self->y + gUnknown_030014A4;

    self->y = total;
    gUnknown_030014A4 += 0x60;
    if (gUnknown_030014A4 > 0x780) {
        gUnknown_030014A4 = 0x780;
    }

    if (total > 0x2800) {
        self->y = 0x2800;
        {
            register u8 *addr asm("r1") = &gUnknown_030014A3;
            register u8 val asm("r0") = 1;
            *addr = val;
        }
        SetCellAnimSpeed(0x24);
        {
            register s32 stateVal asm("r0") = 1;
            register s32 idxVal asm("r1") = 4;
            self->state = stateVal;
            {
                register s32 zero asm("r2") = 0;
                self->stateTime = zero;
                self->animIndex = idxVal;
                {
                    /* Stored through `*(T *)&field` casts: plain member
                     * stores let gcc move the zero load (docs/workflow.md
                     * step 7). */
                    register u16 anim asm("r0") = *(u16 *)&self->anims[4].duration;
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero2;
                }
                self->animTime = zero;
            }
        }
    }
}

/* On the "confirm" input edge (`animDone` set), plays a sound, resets
 * `gUnknown_030014A4` to a large negative "cooldown" value, and fires
 * the state-11/table-index-7 transition (anim frame from `self`'s
 * part-table pointer at `+0x54`). While `y` (the accumulator
 * `sub_802BED8` above drives) exceeds a threshold, additionally spawns
 * an effect object via `CreateActor` and stashes it into
 * `gUnknown_03001490`. */
void sub_802BF30(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animDone != 0) {
        PlaySfx(gAudioContext, 0x3c, 0x100);
        gUnknown_030014A4 = 0xFFFFF980;
        {
            register s32 stateVal asm("r0") = 0xb;
            register s32 idxVal asm("r1") = 7;

            self->state = stateVal;
            {
                register s32 zero asm("r5") = 0;

                self->stateTime = zero;
                self->animIndex = idxVal;
                {
                    /* Casts as in sub_802BED8. */
                    register u16 anim asm("r0") = *(u16 *)&self->anims[7].duration;
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero2;
                }
                self->animTime = zero;

                if (self->y > 0x2000) {
                    gUnknown_03001490 = CreateActor(2, self->x, 0x2800,
                                                     self->z, zero);
                }
            }
        }
    }
}

/* On the "confirm" input edge, sets `gUnknown_030014A3`/state-1/
 * table-index-0 (anim frame from `self`'s own part-table pointer at
 * `+0`) and fires `SetCellAnimSpeed(0x24)` - the state-transition counterpart
 * to `sub_802BED8`, entered directly rather than through the
 * accumulator threshold. */
void sub_802BFA0(void *selfArg)
{
    register struct actor_self *self asm("r3") = selfArg;

    if (self->animDone != 0) {
        {
            register u8 *addr asm("r1") = &gUnknown_030014A3;
            register u8 val asm("r0") = 1;
            *addr = val;
        }
        {
            register s32 stateVal asm("r0") = 1;
            register s32 zero asm("r2") = 0;

            self->state = stateVal;
            self->stateTime = zero;
            self->animIndex = zero;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[0].duration;
                register u8 zero2 asm("r1") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero2;
            }
            self->animTime = zero;
        }
        SetCellAnimSpeed(0x24);
    }
}

/* Once-only latch (`gUnknown_030014A0`): arms a countdown
 * (`gUnknown_0300148C = 0x16`), runs `SetCellAnimSpeed(0x24)`, clamps
 * `gUnknown_030014A4` to non-negative, then calls `StopYeti` and
 * marks both `gUnknown_030014A0` and `gUnknown_030014A3`. */
void sub_802BFD4(void)
{
    if (gUnknown_030014A0 == 0) {
        gUnknown_0300148C = 0x16;
        SetCellAnimSpeed(0x24);
        if (gUnknown_030014A4 < 0) {
            gUnknown_030014A4 = 0;
        }
        StopYeti();
        gUnknown_030014A0 = 1;
        gUnknown_030014A3 = 0;
    }
}

/* Resets the `gUnknown_030014A3`/`030014A1`/`030014A0` latch trio, runs
 * `sub_802D490` on `gPolarAkuAku`, and fires the state-7/table-
 * index-6 transition (anim frame from `self`'s part-table pointer at
 * `+0x48`) plus a sound cue. */
void CatchPolarPlayer(void *selfArg)
{
    struct actor_self *self = selfArg;

    gUnknown_030014A3 = 0;
    gUnknown_030014A1 = 1;
    sub_802D490(gPolarAkuAku);
    gUnknown_030014A0 = 1;

    {
        register s32 stateVal asm("r0") = 7;
        register s32 idxVal asm("r1") = 6;

        self->state = stateVal;
        {
            register s32 zero asm("r2") = 0;

            self->stateTime = zero;
            self->animIndex = idxVal;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[6].duration;
                register u8 zero2 asm("r1") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero2;
            }
            self->animTime = zero;
        }
    }

    PlaySfx(gAudioContext, 0x41, 0x100);
}

/* Accumulator: while `gLevelState+0x8c` is clear, arms
 * `gUnknown_03001484` (once, on the first accumulation) and adds
 * `delta` into `gPolarQueuedWumpa`. Ignores its own first (player-
 * pointer) argument entirely - see docs/rom_map.md's correction on
 * this function. */
void QueuePolarWumpa(void *arg0, s32 delta)
{
    if (*((u8 *)gLevelState + 0x8c) == 0) {
        if (gPolarQueuedWumpa == 0) {
            gUnknown_03001484 = 0xf;
        }
        gPolarQueuedWumpa += delta;
    }
}

/* Trivial forwarder - ignores its own argument and calls
 * `AddLife(gLevelState)`, per docs/rom_map.md's correction
 * (the ROM's own tail-call epilogue clobbers r0/the call's result, so
 * this is void, not passed through as a return value). */
void sub_802C0A8(void *arg0)
{
    AddLife(gLevelState);
}

/* Only runs while `state` is 1-3: sets table-index 2, anim
 * frame from `self`'s part-table pointer at `+0x18`, and - once the
 * frame counter reaches the entry's threshold (the same `+4`-halfword-
 * of-a-0xc-stride-table shape as `UpdatePolarCollectedWumpa` below) - resets the
 * `+8` accumulator. Stashes `arg1` into `x`, plays a
 * state-keyed sound cue (0x5a for state 2, 0x55 for state 1), and
 * transitions to state 3. */
void sub_802C0BC(void *selfArg, s32 arg1param)
{
    register struct actor_self *self asm("r4") = selfArg;
    register s32 arg1 asm("r5") = arg1param;

    if ((u32)(self->state - 1) <= 2) {
        s32 frame;

        self->animIndex = 2;
        {
            /* Casts as in sub_802BED8. */
            register u16 anim asm("r0") = *(u16 *)&self->anims[2].duration;
            register u8 zero1 asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
        }

        frame = GetAnimFrameBaseOffset(self);
        {
            register s32 idx asm("r2") = self->animIndex;
            register u8 *table asm("r3") = (u8 *)self->anims;
            register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
            register s32 four asm("r2");
            register s32 val asm("r1");

            asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
            four = 4;
            val = *(s16 *)(entryPtr + four);

            if (frame >= val) {
                self->animTime = 0;
            }
        }

        self->x = arg1;

        {
            s32 switchState = self->state;

            if (switchState == 2) {
                SetCellAnimSpeed(0x5a);
            } else if (switchState == 1) {
                SetCellAnimSpeed(0x55);
            }
        }

        self->state = 3;
        self->stateTime = 0;
        gUnknown_030014A3 = 0;
    }
}

/* Lock-timer setter: while `AddPolarAkuAkuMask(gPolarAkuAku)` returns 3,
 * arms `gUnknown_0300149C = 500` - the same lock/active flag
 * `ShockPolarPlayer` gates on, per docs/rom_map.md. */
void sub_802C128(void *arg0)
{
    if (AddPolarAkuAkuMask(gPolarAkuAku) == 3) {
        gUnknown_0300149C = 500;
    }
}

/* While `state` is 1-3 and `gUnknown_0300149C` (the same
 * lock/active flag `ShockPolarPlayer` gates on, per docs/rom_map.md) is
 * clear: transitions to state 5/table-index 3 (anim frame from
 * `self`'s part-table pointer at `+0x24`), resets
 * `gUnknown_030014A4` to `-0x780`, and fires `SetCellAnimSpeed(0x1c)`. */
void sub_802C14C(void *selfArg)
{
    register struct actor_self *self asm("r2") = selfArg;

    if ((u32)(self->state - 1) <= 2) {
        register s32 flag asm("r3") = gUnknown_0300149C;

        if (flag == 0) {
            gUnknown_030014A3 = flag;
            {
                register s32 stateVal asm("r0") = 5;
                register s32 idxVal asm("r1") = 3;

                self->state = stateVal;
                self->stateTime = flag;
                self->animIndex = idxVal;
                {
                    /* Casts as in sub_802BED8. */
                    register u16 anim asm("r0") = *(u16 *)&self->anims[3].duration;
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero2;
                }
                self->animTime = flag;
            }
            gUnknown_030014A4 = 0xFFFFF880;
            SetCellAnimSpeed(0x1c);
        }
    }
}

/* Teardown, gated by `arg1` bit 0: temporarily swaps `vtable`'s
 * vtable to `gPolarPlayerVtable` to run `gPolarQueuedWumpa` drain
 * calls into `CollectWumpa(gLevelState)`, runs two
 * `FreeVramTileBlock` cleanup calls on `gPolarPlayerTiles[0]`/`[1]`, sets
 * `vtable` to the "dead" vtable `gActorVtable`, unlinks
 * `self` from the circular `+0x48`(prev)/`+0x4c`(next) list, and frees
 * `self` when `arg1 & 1`. */
void DestroyPolarPlayer(void *selfArg, u32 arg1param)
{
    register u8 *self asm("r5") = selfArg;
    u32 arg1 = arg1param;

    *(u8 **)(self + 0x50) = gPolarPlayerVtable;

    if (gPolarQueuedWumpa != 0) {
        do {
            CollectWumpa(gLevelState);
            gPolarQueuedWumpa -= 1;
        } while (gPolarQueuedWumpa != 0);
    }

    FreeVramTileBlock(gPolarPlayerTiles[0]);
    FreeVramTileBlock(gPolarPlayerTiles[1]);

    *(u8 **)(self + 0x50) = gActorVtable;

    {
        u8 *next = *(u8 **)(self + 0x4c);
        u8 *prev = *(u8 **)(self + 0x48);
        *(u8 **)(next + 0x48) = prev;
    }
    {
        u8 *prev = *(u8 **)(self + 0x48);
        u8 *next = *(u8 **)(self + 0x4c);
        *(u8 **)(prev + 0x4c) = next;
    }

    if (arg1 & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");
