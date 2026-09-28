#include "core.h"
#include "actor_self.h"

/* Continues the `InitActorPart`/`gUnknown_0300148x`-`gUnknown_030014Bx`
 * cluster already established in `src/graphics/actor_part107.c`
 * (issue #50's leftover tail, `docs/matching/issue-50-actor-bc68.md`) -
 * same `self` object and global family (a countdown-timer/respawn
 * pair at `gUnknown_0300148C`/`gUnknown_0300149C`, a "camera catch-up"
 * budget at `gUnknown_030014A4`, and the shared reset/state-transition
 * idiom). Covers `0x0802B364`-`0x0802BC68`, right before
 * `actor_part107.c`'s own range - the start of this whole 40KB "actor
 * zone" a scoping investigation found still raw, before issue #52's
 * `actor_part19.c`. `docs/rom_map.md` had already read part of this
 * cluster from disassembly alone.
 *
 * Built with old_agbcc: `sub_802BAD0` (the `1` mask materialized before
 * the `ldrh` it is ANDed with) and `sub_802B5B4`/`sub_802B864` (operand
 * order of their loads and multiplies) only match under it, and every
 * other function here matches under both compilers - see
 * docs/matching/issue-51-54-naked-retry.md. */

extern void *gUnknown_030012C0;
extern void *gUnknown_030012BC;
extern void *gUnknown_03001490;
extern void *gUnknown_03001494;
extern s32 gUnknown_0300149C;
extern s32 gUnknown_030014A4;
extern u8 gUnknown_03001480;
extern u8 gUnknown_030014A0;
extern u8 gUnknown_030014A3;

struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gUnknown_030007E0;

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);
extern void sub_8029BAC(s32 arg0);
extern void sub_8023234(void *arg0);
extern void sub_802DFBC(void);
extern s32 sub_802D4B0(void *self);
extern void *sub_802AC28(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void *AllocVramTileBlock(s32 size);
extern s32 sub_8000E1C(s32 max);
extern s32 sub_80231EC(void *arg0, s32 arg1);
extern void sub_802BC68(void *selfArg);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);

extern u8 gStaticData_0817A728[];
extern u8 gStaticData_0817A748[];

extern s32 gUnknown_0300148C;
extern s32 gUnknown_03001498;
extern u8 gUnknown_030014A1;
extern struct actor_pmf gStaticData_0817A6B8[];
extern void (*gUnknown_03000874)(void *dst, u8 *frame);
extern void *gUnknown_030014B0[2];     // the two VRAM tile buffers
extern s32 gUnknown_030014A8;          // which buffer holds the current frame
extern u8 *gUnknown_030014AC;          // the frame last uploaded

/* The camera object `self+0x30` points at. */
struct cam_ref {
    u8 unk_00[0x10];
    s32 depth;      // 0x10 - the depth at which sprites draw unscaled
};

extern s32 GetAnimFrameBaseOffset(void *self);
extern u32 GetSpriteShapeSizeBits(u8 *frame);
extern void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 priority);
extern u8 sub_8029794(void);
extern s32 sub_8029B2C(void);
extern void sub_8029D8C(s32 x, s32 y);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern void *sub_802B1A8(s32 x, s32 y, s32 z, s32 tier);
extern void sub_802D3A8(void *obj, s32 x, s32 y, s32 z);
extern s32 sub_802D4EC(void *obj);

ACTOR_CALL_VIA_ALIASES
asm(".set __divsi3, sub_803ADB4");

static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

/* The per-frame update (`docs/rom_map.md`'s "sub_802B364", a slot of
 * the `gStaticData_087E4E54` method table): runs `sub_802BC68`, ticks
 * the two countdowns (`gUnknown_0300148C` expiring into state 10;
 * `gUnknown_0300149C` blinking `self+0x2C`), depth, animation, the
 * current state's handler from `gStaticData_0817A6B8` (a C++
 * pointer-to-member call), left/right steering while
 * `gUnknown_030014A3` is set, then drives - or first spawns - the
 * companion object in `gUnknown_03001494`. Same shape as
 * `actor_part128.c`'s `sub_802E84C`. */
void sub_802B364(struct actor_self *self)
{
    sub_802BC68(self);
    if (gUnknown_0300148C != 0 && --gUnknown_0300148C == 0) {
        gUnknown_030014A1 = 1;
        sub_8029BAC(0);
        ACTOR_SET_STATE(self, 10, 10);
    }
    if (gUnknown_0300149C != 0 && --gUnknown_0300149C != 0 && gUnknown_030014A0 == 0
        && *(s32 *)((u8 *)gUnknown_030012C0 + 0x78) != 3)
        self->unk_2C[0] = ((u32)gUnknown_0300149C >> 2) & 1;
    else
        self->unk_2C[0] = 1;
    if (gUnknown_030014A1 == 0) {
        self->depth = 0x2f00;
        self->z = (sub_8029B2C() << 8) - self->depth;
    }
    {
        s32 d = (self->depth >> 1) & 0x7f80;

        self->visible = d | (((Abs(self->y) + Abs(self->x)) >> 11) & 0x7f);
    }
    self->stateTime++;
    self->animTime += *(s16 *)&self->animTimer;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold) {
        self->animTime -= (self->anims[self->animIndex].loopThreshold
                           - self->anims[self->animIndex].loopBase) << 8;
        self->animDone = 1;
    }
    sub_8029D8C(self->x, self->y);
    ACTOR_PMF_CALL(self, gStaticData_0817A6B8);
    if (gUnknown_030014A3 != 0) {
        struct held_pressed_pair keys = gUnknown_030007E0;

        if (keys.held & 0x20) {
            if (gUnknown_03001498++ > 12)
                self->x += -0x380;
            else
                self->x += -0x2cd;
            if (self->x < -0x3200)
                self->x = -0x3200;
        } else {
            u16 right = keys.held & 0x10;

            if (right) {
                if (gUnknown_03001498++ > 12)
                    self->x += 0x380;
                else
                    self->x += 0x2cd;
                if (self->x > 0x3200)
                    self->x = 0x3200;
            } else {
                gUnknown_03001498 = right;
            }
        }
    }
    if (gUnknown_03001494 != NULL) {
        sub_802D3A8(gUnknown_03001494, self->x, self->y, self->z);
    } else {
        s32 tier = *(s32 *)((u8 *)gUnknown_030012C0 + 0x78);

        gUnknown_03001494 = sub_802B1A8(self->x, self->y, self->z, tier);
        if (sub_8029794())
            sub_802D4EC(gUnknown_03001494);
    }
}

/* The anim_part_instance accessors (actor_anim.c), inlined. */
static inline s32 CurAttr(struct actor_self *self)
{
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;

    return (s32)table[idx].attr << 16;
}

static inline u8 *CurFrame(struct actor_self *self)
{
    s32 t = self->animTime >> 8;

    return (u8 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t];
}

/* The sprite draw: projects the position by depth (scaled and
 * double-sized when drawn behind the camera's reference depth), culls
 * against the screen, uploads the frame's tiles into the other of the
 * two VRAM buffers when the frame changed, and queues the OAM entry.
 * The same code as `actor_part128.c`'s `sub_802E9FC` with a different
 * projection constant. */
void sub_802B5B4(struct actor_self *self)
{
    s32 scale;
    s32 attr1 = 0;
    u8 *frame;
    u32 w, h;
    s32 halfW, halfH;
    s32 sx, sy;

    frame = CurFrame(self);
    w = frame[0];
    halfW = w * 4;
    h = frame[1];
    halfH = h * 4;
    if (self->depth == (*(struct cam_ref **)&self->unk_2C[4])->depth) {
        scale = 0x100;
        sy = (self->y + sub_8029E98()) >> 8;
        sx = (self->x + sub_8029EB4()) >> 8;
    } else {
        s32 depth = self->depth;
        s32 f;

        scale = (depth << 8) / (*(struct cam_ref **)&self->unk_2C[4])->depth;
        f = 0x2f00000 / depth;
        sy = (((self->y * f) >> 12) + sub_8029E98()) >> 8;
        sx = (((self->x * f) >> 12) + sub_8029EB4()) >> 8;
        attr1 = 0x100;
        if (scale <= 0xff) {
            attr1 |= 0x200;
            halfW = w * 8;
            halfH = h * 8;
        }
    }
    sx -= halfW;
    sy -= halfH;
    if (sy <= 0x9f && sy + halfH * 2 >= 0 && sx <= 0xef && sx + halfW * 2 >= 0) {
        u32 attr = CurAttr(self);

        attr1 |= (sy & 0xff) | ((sx & 0x1ff) << 16) | attr | GetSpriteShapeSizeBits(frame);
        if (frame != gUnknown_030014AC) {
            gUnknown_030014A8 ^= 1;
            gUnknown_03000874(gUnknown_030014B0[gUnknown_030014A8], frame);
            gUnknown_030014AC = frame;
        }
        {
            register u32 tile asm("r0") = GET_TILE_NUM(gUnknown_030014B0[gUnknown_030014A8]);

            QueueSpriteFrameOam(attr1, tile | (self->unk_18 << 12), scale);
        }
    }
}

/* Once-only spawn/reset trigger (proximity-hazard family, established
 * `gUnknown_0300148x`/`gUnknown_030014Bx` cluster): if the shared
 * "used" respawn timer (`gUnknown_0300149C`) is already counting down,
 * reports "still used" (1) without doing anything. Otherwise, while
 * the current hazard tier (`gUnknown_030012C0->0x78`) is clear, plays
 * a cue, DMAs a gauge strip, resets `self` to state 6/table-index 5,
 * arms `gUnknown_03001480`, kicks the game-mode transition
 * (`sub_8023234`) if not already paused, clears
 * `gUnknown_030014A3`/arms `gUnknown_030014A0`, and fires
 * `sub_8029BAC(0)`/`sub_802DFBC()` - or, while a tier is already
 * active, arms a fixed `gUnknown_0300149C` countdown and forwards to
 * `sub_802D4B0` (the "remove a mask" helper, `actor_part58.c`)
 * instead. Either way reports "not yet used" (0).
 *
 * The ROM keeps the "already used" early return sharing the exact same
 * epilogue as the main fallthrough path's own final `return 0`, rather
 * than duplicating it - a single `return result;` at one shared `end`
 * label (reached by `goto` from the early-return case) reproduces that,
 * per the `goto`-shared-tail idiom in
 * `docs/matching/issue-52-gap-b364.md`. The three addresses this
 * function keeps alive throughout (`gUnknown_0300149C`, `gUnknown_
 * 03001494`, `&gUnknown_030012C0`) are each read once into their own
 * pointer local and reused from there, matching the ROM's own register
 * lifetime (never re-deriving an address it already has); `self`
 * itself is reused for the unrelated `1` constant once its own fields
 * are no longer needed (`one`, sharing r4 with the now-dead `self`). */
s32 sub_802B730(void *selfArg)
{
    u8 *self = selfArg;
    register s32 result asm("r0");
    register s32 *usedTimer asm("r1") = &gUnknown_0300149C;

    if (*usedTimer != 0) {
        result = 1;
        goto end;
    }

    {
        register void **effectAddr asm("r2") = &gUnknown_03001494;
        void **playerAddr = (void **)&gUnknown_030012C0;
        s32 tier = *(s32 *)((u8 *)(*playerAddr) + 0x78);

        if (tier == 0) {
            PlaySfx(gUnknown_030012BC, 0x1b, 0x100);
            QueueVramDmaTransfer(gStaticData_0817A728, (void *)0x05000200, 0x20, 0x10);
            {
                register s32 state asm("r0") = 6;
                register s32 idx asm("r1") = 5;

                *(s32 *)(self + 0x28) = state;
                *(s32 *)(self + 0x44) = tier;
                *(s32 *)(self + 0xc) = idx;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x3c);
                    register u8 zero asm("r6") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero;
                    *(s32 *)(self + 8) = tier;

                    {
                        u8 *reg1480 = &gUnknown_03001480;
                        register u8 one asm("r4") = 1;

                        *reg1480 = one;
                        {
                            u8 *player = *playerAddr;

                            if (player[0x8c] == 0) {
                                sub_8023234(player);
                            }
                        }
                        gUnknown_030014A3 = zero;
                        gUnknown_030014A0 = one;
                    }
                }
            }
            sub_8029BAC(0);
            sub_802DFBC();
        } else {
            *usedTimer = 0x4b;
            sub_802D4B0(*effectAddr);
        }
    }

    result = 0;
end:
    return result;
}

/* Same shape as `sub_802B730` (twin trigger, different reset target -
 * state 0xc/table-index 0xb): once-only spawn/reset gated the same way
 * on `gUnknown_0300149C`/hazard tier, or forwards to `sub_802D4B0`.
 *
 * Same `goto`-shared-tail idiom as `sub_802B730` above (single `return
 * result;` at one shared `end` label) and the same three
 * persistent-address-local shape, except here `effectAddr` itself
 * (`&gUnknown_03001494`, in r4) is the register reused for the
 * unrelated `0` constant once its own address is no longer needed on
 * the tier-clear path (the tier-active path never touches that reuse,
 * since it reads through the original `effectAddr` before this
 * function would ever take the tier-clear branch). */
s32 sub_802B7E0(void *selfArg)
{
    u8 *self = selfArg;
    register s32 result asm("r0");
    register s32 *usedTimer asm("r1") = &gUnknown_0300149C;

    if (*usedTimer != 0) {
        result = 1;
        goto end;
    }

    {
        register void **effectAddr asm("r4") = &gUnknown_03001494;
        void **playerAddr = (void **)&gUnknown_030012C0;
        s32 tier = *(s32 *)((u8 *)(*playerAddr) + 0x78);

        if (tier == 0) {
            register s32 state asm("r0") = 0xc;
            register s32 idx asm("r1") = 0xb;

            *(s32 *)(self + 0x28) = state;
            *(s32 *)(self + 0x44) = tier;
            *(s32 *)(self + 0xc) = idx;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x84);
                register u8 zero asm("r4") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero;
                *(s32 *)(self + 8) = tier;

                PlaySfx(gUnknown_030012BC, 0x33, 0x100);
                gUnknown_030014A3 = zero;
            }
            gUnknown_030014A0 = 1;
            sub_8029BAC(0);
            sub_802DFBC();
        } else {
            *usedTimer = 0x4b;
            sub_802D4B0(*effectAddr);
        }
    }

    result = 0;
end:
    return result;
}

/* Allocates the pair of VRAM tile blocks this cluster's gauge display
 * uses (`gUnknown_030014B0[0]`/`[1]`), each sized from the same
 * keyframe-table byte-pair lookup (`self`'s part table, indexed by
 * `self+0xc`, offset by `self+8`'s frame accumulator, into a *second*
 * pointer array at `self+4`) already established for `sub_802F338`
 * (`actor_part43b.c`, `docs/matching/issue-56-0x0802f0dc-actor.md`);
 * arms `gUnknown_030014A8`, clears `gUnknown_030014AC`. The ROM's
 * "multiply into a copy, copy again, then shift" sequence is simply
 * old_agbcc's code for `h * w * 32` - no register forcing needed. */
void sub_802B864(struct actor_self *self)
{
    u8 *f;

    f = CurFrame(self);
    gUnknown_030014B0[0] = AllocVramTileBlock(f[1] * f[0] * 32);
    f = CurFrame(self);
    gUnknown_030014B0[1] = AllocVramTileBlock(f[1] * f[0] * 32);
    gUnknown_030014A8 = 1;
    gUnknown_030014AC = 0;
}

/* Spawn-once trigger for a secondary effect object
 * (`gUnknown_03001490`, via `sub_802AC28`), then drains a "camera
 * catch-up" budget (`gUnknown_030014A4`) into `self+0x20` until it
 * crosses a fixed threshold, at which point it plays a cue, clamps
 * `self+0x20`, resets `self` to state 9/table-index 9, clears
 * `gUnknown_030014A0`, fires the spawned object's own `self+0x50`
 * trampoline (index 3) if still alive, clears `gUnknown_03001490`, and
 * fires `sub_8029BAC(0x19)`. */
void sub_802B8E8(struct actor_self *self)
{
    struct actor_self **spawnAddr = (struct actor_self **)&gUnknown_03001490;
    struct actor_self *spawn = *spawnAddr;
    s32 *budget;
    s32 y;

    if (spawn == NULL) {
        struct actor_self *o = sub_802AC28(2, self->x, 0x2800, self->z, 0);

        *spawnAddr = o;
        o->animIndex = 1;
        o->animTimer = o->anims[1].duration;
        o->animDone = 0;
        o->animTime = 0;
    }
    budget = &gUnknown_030014A4;
    y = self->y + *budget;
    self->y = y;
    *budget += 0x2d;
    if (y > 0x2800) {
        PlaySfx(gUnknown_030012BC, 0x35, 0x100);
        self->y = 0x2800;
        ACTOR_SET_STATE(self, 9, 9);
        gUnknown_030014A0 = 0;
        if (*spawnAddr != NULL) {
            ACTOR_VCALL(*spawnAddr, m08, 3);
        }
        *spawnAddr = NULL;
        sub_8029BAC(0x19);
    }
}

/* On the state-0x12 anim-frame edge, plays a "confirm" cue
 * (`sub_8029BAC(0x24)`) then either resets `self` to the idle
 * table-index 0 (`self+0xc == 0` case) or, gated on
 * `sub_8000E1C(3)`'s own result, either restores `self+0xc` or
 * transitions it to 1 - both cases refreshing the anim frame from the
 * (possibly restored) table index. Independently, on the
 * `gUnknown_030014A3` edge, fires up to two more `gUnknown_030007E0`
 * input-gated one-shot transitions (state 4/table-index 3 with a cue
 * and a `gUnknown_030014A4` reset, and state 2/table-index 0 with
 * `sub_8029BAC(0x38)`).
 *
 * The ROM keeps the "self+0xc == 0" reset case and the
 * `sub_8000E1C`-gated case's two outcomes sharing one physical tail
 * (the anim-frame refresh) rather than each duplicating it - a plain
 * nested if/else here reproduces the checks but not that exact tail
 * sharing (this compiler inlines the tail into each arm on its own
 * schedule instead of always jumping to one shared copy), so the
 * branch structure below is written with explicit `goto`s to the same
 * `tail`/`join` labels the ROM's own branches target, per the
 * `goto`-shared-tail idiom documented in
 * `docs/matching/issue-52-gap-b364.md`. */
void sub_802B990(void *selfArg)
{
    u8 *self = selfArg;
    register s32 index asm("r5");
    register u16 anim asm("r0");

    if (self[0x12] == 0)
        goto tail;

    sub_8029BAC(0x24);
    index = *(s32 *)(self + 0xc);
    if (index == 0)
        goto gated;

    {
        register s32 zero asm("r2") = 0;

        *(s32 *)(self + 0xc) = zero;
        {
            register u16 a asm("r0") = *(u16 *)(*(u8 **)self + 0);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = a;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = zero;
    }
    goto tail;

gated:
    if ((u16)sub_8000E1C(3) != 0)
        goto restore;

    *(s32 *)(self + 0xc) = 1;
    anim = *(u16 *)(*(u8 **)self + 0xc);
    goto join;

restore:
    *(s32 *)(self + 0xc) = index;
    anim = *(u16 *)(*(u8 **)self + 0);

join:
    {
        register u8 zero1 asm("r1") = 0;

        *(u16 *)(self + 0x10) = anim;
        self[0x12] = zero1;
    }
    *(s32 *)(self + 8) = index;

tail:
    if (gUnknown_030014A3 != 0) {
        register struct held_pressed_pair *addr asm("r5") = &gUnknown_030007E0;
        register s32 bit1 asm("r0") = 1;
        u16 pressed = addr->pressed;

        bit1 &= pressed;
        if (bit1 != 0) {
            register s32 state asm("r0") = 4;
            register s32 idx asm("r1") = 3;

            *(s32 *)(self + 0x28) = state;
            {
                register s32 zero asm("r2") = 0;

                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = idx;
                {
                    register u16 a asm("r0") = *(u16 *)(*(u8 **)self + 0x24);
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = a;
                    self[0x12] = zero1;
                }
                *(s32 *)(self + 8) = zero;
            }
            PlaySfx(gUnknown_030012BC, 0xd, 0x100);
            gUnknown_030014A4 = 0xFFFFF880;
        }
        if ((*(u32 *)addr & 2) != 0) {
            register s32 state asm("r0") = 2;

            *(s32 *)(self + 0x28) = state;
            {
                register s32 zero asm("r2") = 0;

                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = state;
                {
                    register u16 a asm("r0") = *(u16 *)(*(u8 **)self + 0x18);
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = a;
                    self[0x12] = zero1;
                }
                *(s32 *)(self + 8) = zero;
            }
            sub_8029BAC(0x38);
        }
    }
}

/* Camera catch-up accumulate/threshold reset: drains
 * `gUnknown_030014A4` into `self+0x20`, advances the budget by a fixed
 * step (clamped to 0x780), applies a stall clamp
 * (`gUnknown_030014A4 = 0xFFFFFC00`) once the frame counter passes a
 * threshold with no input, then resets `self` to state 1/table-index 4
 * once `self+0x20` crosses `0x2800`. */
void sub_802BA5C(struct actor_self *self)
{
    s32 *budget = &gUnknown_030014A4;

    self->y += *budget;
    *budget += 0x60;
    if (*budget > 0x780)
        *budget = 0x780;
    if (self->stateTime <= 10 && !(*(u32 *)&gUnknown_030007E0 & 1) && *budget < (s32)0xFFFFFC00)
        *budget = 0xFFFFFC00;
    if (self->y > 0x2800) {
        self->y = 0x2800;
        ACTOR_SET_STATE(self, 1, 4);
        sub_8029BAC(0x24);
    }
}

/* Two independent `gUnknown_030007E0` input-gated one-shot
 * transitions on `self`: bit 1 resets to state 1/table-index 0 (plain
 * anim-frame idle reset, `sub_8029BAC(0x24)`); bit 0 (of the high
 * halfword) transitions to state 4/table-index 3 with a cue and the
 * `gUnknown_030014A4` stall reset - same pair `sub_802B990` fires. */
void sub_802BAD0(struct actor_self *self)
{
    struct held_pressed_pair *input = &gUnknown_030007E0;
    u16 bit = *(u32 *)input & 2;

    if (bit == 0) {
        self->state = 1;
        self->stateTime = bit;
        self->animIndex = bit;
        self->animTimer = self->anims[0].duration;
        self->animDone = 0;
        self->animTime = bit;
        sub_8029BAC(0x24);
    }

    if (input->pressed & 1) {
        ACTOR_SET_STATE(self, 4, 3);
        PlaySfx(gUnknown_030012BC, 0xd, 0x100);
        gUnknown_030014A4 = 0xFFFFF880;
    }
}

/* Frame-counter threshold DMA driver: past `0x2c` frames, does the
 * full gauge-strip DMA plus state 6/table-index 5 reset (arming
 * `gUnknown_03001480` and kicking the mode transition, same shape as
 * `sub_802B730` above); otherwise DMAs one of two gauge-strip variants
 * every 4th frame without touching any state. */
void sub_802BB4C(void *selfArg)
{
    u8 *self = selfArg;
    s32 counter = *(s32 *)(self + 0x44);

    if (counter > 0x2c) {
        QueueVramDmaTransfer(gStaticData_0817A728, (void *)0x05000200, 0x20, 0x10);
        gUnknown_03001480 = 1;
        {
            register s32 state asm("r0") = 6;
            register s32 idx asm("r1") = 5;

            *(s32 *)(self + 0x28) = state;
            {
                register s32 zero asm("r2") = 0;

                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = idx;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x3c);
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero1;
                }
                *(s32 *)(self + 8) = zero;
            }
        }
        if (*(u8 *)((u8 *)gUnknown_030012C0 + 0x8c) == 0) {
            sub_8023234(gUnknown_030012C0);
        }
    } else if (counter & 4) {
        QueueVramDmaTransfer(gStaticData_0817A728, (void *)0x05000200, 0x20, 0x10);
    } else {
        QueueVramDmaTransfer(gStaticData_0817A748, (void *)0x05000200, 0x20, 0x10);
    }
}

/* On the state-0x12 anim edge, resets `gUnknown_030014A4`'s stall
 * clamp, and, once `self+0x20` crosses `0x2000`, spawns a secondary
 * effect object via `sub_802AC28` (stashed into `gUnknown_03001490`)
 * gated on that same threshold. Resets `self` to state 8/table-index
 * 7, arms `gUnknown_03001480`, and kicks the mode transition the same
 * way as `sub_802B730`/`sub_802BB4C`. */
void sub_802BBE4(struct actor_self *self)
{
    if (self->animDone) {
        gUnknown_030014A4 = 0xFFFFF980;
        if (self->y > 0x2000)
            gUnknown_03001490 = sub_802AC28(2, self->x, 0x2800, self->z, 0);
        ACTOR_SET_STATE(self, 8, 7);
        gUnknown_03001480 = 1;
        {
            u8 *player = gUnknown_030012C0;

            if (player[0x8c] == 0)
                sub_8023234(player);
        }
    }
}

asm(".align 2, 0");
