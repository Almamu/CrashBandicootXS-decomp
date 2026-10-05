#include "core.h"
#include "actor_self.h"

/* Continues the `InitActorPart`/`gUnknown_0300148x`-`gUnknown_030014Bx`
 * cluster already established in `src/graphics/actor_part107.c`
 * (issue #50's leftover tail, `docs/matching/issue-50-actor-bc68.md`) -
 * same `self` object and global family (a countdown-timer/respawn
 * pair at `gPolarFinishTimer`/`gPolarInvulnTimer`, a "camera catch-up"
 * budget at `gPolarPlayerVelY`, and the shared reset/state-transition
 * idiom). Covers `0x0802B364`-`0x0802BC68`, right before
 * `actor_part107.c`'s own range - the start of this whole 40KB "actor
 * zone" a scoping investigation found still raw, before issue #52's
 * `actor_part19.c`. `docs/rom_map.md` had already read part of this
 * cluster from disassembly alone.
 *
 * Built with old_agbcc: `PolarPlayerStateDash` (the `1` mask materialized before
 * the `ldrh` it is ANDed with) and `DrawPolarPlayer`/`AllocPolarPlayerTiles` (operand
 * order of their loads and multiplies) only match under it, and every
 * other function here matches under both compilers - see
 * docs/matching/issue-51-54-naked-retry.md. */

/* The gLevelState fields read here. */
struct game_state {
    u8 unk_00[0x78];
    s32 maskLevel;      // 0x78 - the Aku Aku mask level (0-3)
    u8 unk_7C[0x10];
    u8 timeTrial;       // 0x8C
};

extern struct game_state *gLevelState;
extern void *gAudioContext;
extern void *gRiderlessPolar;
extern void *gPolarAkuAku;
extern s32 gPolarInvulnTimer;
extern s32 gPolarPlayerVelY;
extern u8 gPolarPauseLocked;
extern u8 gPolarPlayerInactive;
extern u8 gPolarSteerEnabled;

struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gKeys;

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);
extern void SetCellAnimSpeed(s32 arg0);
extern void LoseLife(void *arg0);
extern void StopYeti(void);
extern s32 RemovePolarAkuAkuMask(void *self);
extern void *CreateActor(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void *AllocVramTileBlock(s32 size);
extern s32 RandRange(s32 max);
extern s32 SetMaskLevel(void *arg0, s32 arg1);
extern void DispensePolarWumpa(void *selfArg);
extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);

extern u8 gPolarPlayerShockPalette[];
extern u8 gPolarPlayerShockBlinkPalette[];

extern s32 gPolarFinishTimer;
extern s32 gPolarSteerTime;
extern u8 gPolarPlayerHalted;
extern struct actor_pmf gPolarPlayerStateFuncs[];
extern void (*gUnpackRleSpriteFrameFunc)(void *dst, u8 *frame);
extern void *gPolarPlayerTiles[2];     // the two VRAM tile buffers
extern s32 gPolarPlayerTileBuffer;          // which buffer holds the current frame
extern u8 *gPolarPlayerLastFrame;          // the frame last uploaded

/* The camera object `self+0x30` points at. */
struct cam_ref {
    u8 unk_00[0x10];
    s32 depth;      // 0x10 - the depth at which sprites draw unscaled
};

extern s32 GetAnimFrameBaseOffset(void *self);
extern u32 GetSpriteShapeSizeBits(u8 *frame);
extern void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 priority);
extern u8 IsActorMaskAssistDue(void);
extern s32 GetCellAnimDistance(void);
extern void UpdateActorBgScroll(s32 x, s32 y);
extern s32 GetActorBgCenterY(void);
extern s32 GetActorBgCenterX(void);
extern void *SpawnPolarAkuAku(s32 x, s32 y, s32 z, s32 tier);
extern void MovePolarAkuAku(void *obj, s32 x, s32 y, s32 z);
extern s32 AddPolarAkuAkuMask(void *obj);

static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

/* The per-frame update (`docs/rom_map.md`'s "UpdatePolarPlayer", a slot of
 * the `gPolarPlayerVtable` method table): runs `DispensePolarWumpa`, ticks
 * the two countdowns (`gPolarFinishTimer` expiring into state 10;
 * `gPolarInvulnTimer` blinking `self+0x2C`), depth, animation, the
 * current state's handler from `gPolarPlayerStateFuncs` (a C++
 * pointer-to-member call), left/right steering while
 * `gPolarSteerEnabled` is set, then drives - or first spawns - the
 * companion object in `gPolarAkuAku`. Same shape as
 * `actor_part128.c`'s `UpdateJetpackPlayer`. */
void UpdatePolarPlayer(struct actor_self *self)
{
    DispensePolarWumpa(self);
    if (gPolarFinishTimer != 0 && --gPolarFinishTimer == 0) {
        gPolarPlayerHalted = 1;
        SetCellAnimSpeed(0);
        ACTOR_SET_STATE(self, 10, 10);
    }
    if (gPolarInvulnTimer != 0 && --gPolarInvulnTimer != 0 && gPolarPlayerInactive == 0
        && gLevelState->maskLevel != 3)
        self->visible = ((u32)gPolarInvulnTimer >> 2) & 1;
    else
        self->visible = 1;
    if (gPolarPlayerHalted == 0) {
        self->depth = 0x2f00;
        self->z = (GetCellAnimDistance() << 8) - self->depth;
    }
    {
        s32 d = (self->depth >> 1) & 0x7f80;

        self->sortKey = d | (((Abs(self->y) + Abs(self->x)) >> 11) & 0x7f);
    }
    self->stateTime++;
    self->animTime += *(s16 *)&self->animTimer;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold) {
        self->animTime -= (self->anims[self->animIndex].loopThreshold
                           - self->anims[self->animIndex].loopBase) << 8;
        self->animDone = 1;
    }
    UpdateActorBgScroll(self->x, self->y);
    ACTOR_PMF_CALL(self, gPolarPlayerStateFuncs);
    if (gPolarSteerEnabled != 0) {
        struct held_pressed_pair keys = gKeys;

        if (keys.held & 0x20) {
            if (gPolarSteerTime++ > 12)
                self->x += -0x380;
            else
                self->x += -0x2cd;
            if (self->x < -0x3200)
                self->x = -0x3200;
        } else {
            u16 right = keys.held & 0x10;

            if (right) {
                if (gPolarSteerTime++ > 12)
                    self->x += 0x380;
                else
                    self->x += 0x2cd;
                if (self->x > 0x3200)
                    self->x = 0x3200;
            } else {
                gPolarSteerTime = right;
            }
        }
    }
    if (gPolarAkuAku != NULL) {
        MovePolarAkuAku(gPolarAkuAku, self->x, self->y, self->z);
    } else {
        s32 tier = gLevelState->maskLevel;

        gPolarAkuAku = SpawnPolarAkuAku(self->x, self->y, self->z, tier);
        if (IsActorMaskAssistDue())
            AddPolarAkuAkuMask(gPolarAkuAku);
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
 * The same code as `actor_part128.c`'s `DrawJetpackPlayer` with a different
 * projection constant. */
void DrawPolarPlayer(struct actor_self *self)
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
    if (self->depth == (*(struct cam_ref **)&self->record)->depth) {
        scale = 0x100;
        sy = (self->y + GetActorBgCenterY()) >> 8;
        sx = (self->x + GetActorBgCenterX()) >> 8;
    } else {
        s32 depth = self->depth;
        s32 f;

        scale = (depth << 8) / (*(struct cam_ref **)&self->record)->depth;
        f = 0x2f00000 / depth;
        sy = (((self->y * f) >> 12) + GetActorBgCenterY()) >> 8;
        sx = (((self->x * f) >> 12) + GetActorBgCenterX()) >> 8;
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
        if (frame != gPolarPlayerLastFrame) {
            gPolarPlayerTileBuffer ^= 1;
            gUnpackRleSpriteFrameFunc(gPolarPlayerTiles[gPolarPlayerTileBuffer], frame);
            gPolarPlayerLastFrame = frame;
        }
        {
            register u32 tile asm("r0") = GET_TILE_NUM(gPolarPlayerTiles[gPolarPlayerTileBuffer]);

            QueueSpriteFrameOam(attr1, tile | (self->palette << 12), scale);
        }
    }
}

/* Once-only spawn/reset trigger (proximity-hazard family, established
 * `gUnknown_0300148x`/`gUnknown_030014Bx` cluster): if the shared
 * "used" respawn timer (`gPolarInvulnTimer`) is already counting down,
 * reports "still used" (1) without doing anything. Otherwise, while
 * the current mask level (`maskLevel`) (`gLevelState->0x78`) is clear, plays
 * a cue, DMAs a gauge strip, resets `self` to state 6/table-index 5,
 * arms `gPolarPauseLocked`, kicks the game-mode transition
 * (`LoseLife`) if not already paused, clears
 * `gPolarSteerEnabled`/arms `gPolarPlayerInactive`, and fires
 * `SetCellAnimSpeed(0)`/`StopYeti()` - or, while a tier is already
 * active, arms a fixed `gPolarInvulnTimer` countdown and forwards to
 * `RemovePolarAkuAkuMask` (the "remove a mask" helper, `actor_part58.c`)
 * instead. Either way reports "not yet used" (0).
 *
 * The ROM keeps the "already used" early return sharing the exact same
 * epilogue as the main fallthrough path's own final `return 0`, rather
 * than duplicating it - a single `return result;` at one shared `end`
 * label (reached by `goto` from the early-return case) reproduces that,
 * per the `goto`-shared-tail idiom in
 * `docs/matching/issue-52-gap-b364.md`. The three addresses this
 * function keeps alive throughout (`gPolarInvulnTimer`, `gUnknown_
 * 03001494`, `&gLevelState`) are each read once into their own
 * pointer local and reused from there, matching the ROM's own register
 * lifetime (never re-deriving an address it already has); `self`
 * itself is reused for the unrelated `1` constant once its own fields
 * are no longer needed (`one`, sharing r4 with the now-dead `self`). */
s32 HurtPolarPlayer(void *selfArg)
{
    struct actor_self *self = selfArg;
    register s32 result asm("r0");
    register s32 *usedTimer asm("r1") = &gPolarInvulnTimer;

    if (*usedTimer != 0) {
        result = 1;
        goto end;
    }

    {
        register void **effectAddr asm("r2") = &gPolarAkuAku;
        void **playerAddr = (void **)&gLevelState;
        s32 tier = ((struct game_state *)*playerAddr)->maskLevel;

        if (tier == 0) {
            PlaySfx(gAudioContext, 0x1b, 0x100);
            QueueVramDmaTransfer(gPolarPlayerShockPalette, (void *)OBJ_PLTT, 0x20, 0x10);
            {
                register s32 state asm("r0") = 6;
                register s32 idx asm("r1") = 5;

                self->state = state;
                self->stateTime = tier;
                self->animIndex = idx;
                {
                    register u16 anim asm("r0") = self->anims[5].duration;
                    register u8 zero asm("r6") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero;
                    self->animTime = tier;

                    {
                        u8 *reg1480 = &gPolarPauseLocked;
                        register u8 one asm("r4") = 1;

                        *reg1480 = one;
                        {
                            struct game_state *player = *playerAddr;

                            if (player->timeTrial == 0) {
                                LoseLife(player);
                            }
                        }
                        gPolarSteerEnabled = zero;
                        gPolarPlayerInactive = one;
                    }
                }
            }
            SetCellAnimSpeed(0);
            StopYeti();
        } else {
            *usedTimer = 0x4b;
            RemovePolarAkuAkuMask(*effectAddr);
        }
    }

    result = 0;
end:
    return result;
}

/* Same shape as `HurtPolarPlayer` (twin trigger, different reset target -
 * state 0xc/table-index 0xb): once-only spawn/reset gated the same way
 * on `gPolarInvulnTimer`/mask level (`maskLevel`), or forwards to `RemovePolarAkuAkuMask`.
 *
 * Same `goto`-shared-tail idiom as `HurtPolarPlayer` above (single `return
 * result;` at one shared `end` label) and the same three
 * persistent-address-local shape, except here `effectAddr` itself
 * (`&gPolarAkuAku`, in r4) is the register reused for the
 * unrelated `0` constant once its own address is no longer needed on
 * the tier-clear path (the tier-active path never touches that reuse,
 * since it reads through the original `effectAddr` before this
 * function would ever take the tier-clear branch). */
s32 ShockPolarPlayer(void *selfArg)
{
    struct actor_self *self = selfArg;
    register s32 result asm("r0");
    register s32 *usedTimer asm("r1") = &gPolarInvulnTimer;

    if (*usedTimer != 0) {
        result = 1;
        goto end;
    }

    {
        register void **effectAddr asm("r4") = &gPolarAkuAku;
        void **playerAddr = (void **)&gLevelState;
        s32 tier = ((struct game_state *)*playerAddr)->maskLevel;

        if (tier == 0) {
            register s32 state asm("r0") = 0xc;
            register s32 idx asm("r1") = 0xb;

            self->state = state;
            self->stateTime = tier;
            self->animIndex = idx;
            {
                register u16 anim asm("r0") = self->anims[11].duration;
                register u8 zero asm("r4") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero;
                self->animTime = tier;

                PlaySfx(gAudioContext, 0x33, 0x100);
                gPolarSteerEnabled = zero;
            }
            gPolarPlayerInactive = 1;
            SetCellAnimSpeed(0);
            StopYeti();
        } else {
            *usedTimer = 0x4b;
            RemovePolarAkuAkuMask(*effectAddr);
        }
    }

    result = 0;
end:
    return result;
}

/* Allocates the pair of VRAM tile blocks this cluster's gauge display
 * uses (`gPolarPlayerTiles[0]`/`[1]`), each sized from the same
 * keyframe-table byte-pair lookup (`self`'s part table, indexed by
 * `self+0xc`, offset by `self+8`'s frame accumulator, into a *second*
 * pointer array at `self+4`) already established for `AllocJetpackPlayerTiles`
 * (`actor_part43b.c`, `docs/matching/issue-56-0x0802f0dc-actor.md`);
 * arms `gPolarPlayerTileBuffer`, clears `gPolarPlayerLastFrame`. The ROM's
 * "multiply into a copy, copy again, then shift" sequence is simply
 * old_agbcc's code for `h * w * 32` - no register forcing needed. */
void AllocPolarPlayerTiles(struct actor_self *self)
{
    u8 *f;

    f = CurFrame(self);
    gPolarPlayerTiles[0] = AllocVramTileBlock(f[1] * f[0] * 32);
    f = CurFrame(self);
    gPolarPlayerTiles[1] = AllocVramTileBlock(f[1] * f[0] * 32);
    gPolarPlayerTileBuffer = 1;
    gPolarPlayerLastFrame = 0;
}

/* Spawn-once trigger for a secondary effect object
 * (`gRiderlessPolar`, via `CreateActor`), then drains a "camera
 * catch-up" budget (`gPolarPlayerVelY`) into `self+0x20` until it
 * crosses a fixed threshold, at which point it plays a cue, clamps
 * `self+0x20`, resets `self` to state 9/table-index 9, clears
 * `gPolarPlayerInactive`, fires the spawned object's own `self+0x50`
 * trampoline (index 3) if still alive, clears `gRiderlessPolar`, and
 * fires `SetCellAnimSpeed(0x19)`. */
void PolarPlayerStateMount(struct actor_self *self)
{
    struct actor_self **spawnAddr = (struct actor_self **)&gRiderlessPolar;
    struct actor_self *spawn = *spawnAddr;
    s32 *budget;
    s32 y;

    if (spawn == NULL) {
        struct actor_self *o = CreateActor(2, self->x, 0x2800, self->z, 0);

        *spawnAddr = o;
        o->animIndex = 1;
        o->animTimer = o->anims[1].duration;
        o->animDone = 0;
        o->animTime = 0;
    }
    budget = &gPolarPlayerVelY;
    y = self->y + *budget;
    self->y = y;
    *budget += 0x2d;
    if (y > 0x2800) {
        PlaySfx(gAudioContext, 0x35, 0x100);
        self->y = 0x2800;
        ACTOR_SET_STATE(self, 9, 9);
        gPolarPlayerInactive = 0;
        if (*spawnAddr != NULL) {
            ACTOR_VCALL(*spawnAddr, destroy, 3);
        }
        *spawnAddr = NULL;
        SetCellAnimSpeed(0x19);
    }
}

/* On the state-0x12 anim-frame edge, plays a "confirm" cue
 * (`SetCellAnimSpeed(0x24)`) then either resets `self` to the idle
 * table-index 0 (`self+0xc == 0` case) or, gated on
 * `RandRange(3)`'s own result, either restores `self+0xc` or
 * transitions it to 1 - both cases refreshing the anim frame from the
 * (possibly restored) table index. Independently, on the
 * `gPolarSteerEnabled` edge, fires up to two more `gKeys`
 * input-gated one-shot transitions (state 4/table-index 3 with a cue
 * and a `gPolarPlayerVelY` reset, and state 2/table-index 0 with
 * `SetCellAnimSpeed(0x38)`).
 *
 * The ROM keeps the "self+0xc == 0" reset case and the
 * `RandRange`-gated case's two outcomes sharing one physical tail
 * (the anim-frame refresh) rather than each duplicating it - a plain
 * nested if/else here reproduces the checks but not that exact tail
 * sharing (this compiler inlines the tail into each arm on its own
 * schedule instead of always jumping to one shared copy), so the
 * branch structure below is written with explicit `goto`s to the same
 * `tail`/`join` labels the ROM's own branches target, per the
 * `goto`-shared-tail idiom documented in
 * `docs/matching/issue-52-gap-b364.md`. */
void PolarPlayerStateRun(void *selfArg)
{
    struct actor_self *self = selfArg;
    register s32 index asm("r5");
    register u16 anim asm("r0");

    if (self->animDone == 0)
        goto tail;

    SetCellAnimSpeed(0x24);
    index = self->animIndex;
    if (index == 0)
        goto gated;

    {
        register s32 zero asm("r2") = 0;

        self->animIndex = zero;
        {
            register u16 a asm("r0") = self->anims[0].duration;
            register u8 zero1 asm("r1") = 0;

            *(u16 *)&self->animTimer = a;
            *(u8 *)&self->animDone = zero1;
        }
        self->animTime = zero;
    }
    goto tail;

gated:
    if ((u16)RandRange(3) != 0)
        goto restore;

    self->animIndex = 1;
    anim = self->anims[1].duration;
    goto join;

restore:
    self->animIndex = index;
    anim = self->anims[0].duration;

join:
    {
        register u8 zero1 asm("r1") = 0;

        *(u16 *)&self->animTimer = anim;
        *(u8 *)&self->animDone = zero1;
    }
    self->animTime = index;

tail:
    if (gPolarSteerEnabled != 0) {
        register struct held_pressed_pair *addr asm("r5") = &gKeys;
        register s32 bit1 asm("r0") = 1;
        u16 pressed = addr->pressed;

        bit1 &= pressed;
        if (bit1 != 0) {
            register s32 state asm("r0") = 4;
            register s32 idx asm("r1") = 3;

            self->state = state;
            {
                register s32 zero asm("r2") = 0;

                self->stateTime = zero;
                self->animIndex = idx;
                {
                    register u16 a asm("r0") = self->anims[3].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = a;
                    *(u8 *)&self->animDone = zero1;
                }
                self->animTime = zero;
            }
            PlaySfx(gAudioContext, 0xd, 0x100);
            gPolarPlayerVelY = 0xFFFFF880;
        }
        if ((*(u32 *)addr & 2) != 0) {
            register s32 state asm("r0") = 2;

            self->state = state;
            {
                register s32 zero asm("r2") = 0;

                self->stateTime = zero;
                self->animIndex = state;
                {
                    register u16 a asm("r0") = self->anims[2].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = a;
                    *(u8 *)&self->animDone = zero1;
                }
                self->animTime = zero;
            }
            SetCellAnimSpeed(0x38);
        }
    }
}

/* Camera catch-up accumulate/threshold reset: drains
 * `gPolarPlayerVelY` into `self+0x20`, advances the budget by a fixed
 * step (clamped to 0x780), applies a stall clamp
 * (`gPolarPlayerVelY = 0xFFFFFC00`) once the frame counter passes a
 * threshold with no input, then resets `self` to state 1/table-index 4
 * once `self+0x20` crosses `0x2800`. */
void PolarPlayerStateJump(struct actor_self *self)
{
    s32 *budget = &gPolarPlayerVelY;

    self->y += *budget;
    *budget += 0x60;
    if (*budget > 0x780)
        *budget = 0x780;
    if (self->stateTime <= 10 && !(*(u32 *)&gKeys & 1) && *budget < (s32)0xFFFFFC00)
        *budget = 0xFFFFFC00;
    if (self->y > 0x2800) {
        self->y = 0x2800;
        ACTOR_SET_STATE(self, 1, 4);
        SetCellAnimSpeed(0x24);
    }
}

/* Two independent `gKeys` input-gated one-shot
 * transitions on `self`: bit 1 resets to state 1/table-index 0 (plain
 * anim-frame idle reset, `SetCellAnimSpeed(0x24)`); bit 0 (of the high
 * halfword) transitions to state 4/table-index 3 with a cue and the
 * `gPolarPlayerVelY` stall reset - same pair `PolarPlayerStateRun` fires. */
void PolarPlayerStateDash(struct actor_self *self)
{
    struct held_pressed_pair *input = &gKeys;
    u16 bit = *(u32 *)input & 2;

    if (bit == 0) {
        self->state = 1;
        self->stateTime = bit;
        self->animIndex = bit;
        self->animTimer = self->anims[0].duration;
        self->animDone = 0;
        self->animTime = bit;
        SetCellAnimSpeed(0x24);
    }

    if (input->pressed & 1) {
        ACTOR_SET_STATE(self, 4, 3);
        PlaySfx(gAudioContext, 0xd, 0x100);
        gPolarPlayerVelY = 0xFFFFF880;
    }
}

/* Frame-counter threshold DMA driver: past `0x2c` frames, does the
 * full gauge-strip DMA plus state 6/table-index 5 reset (arming
 * `gPolarPauseLocked` and kicking the mode transition, same shape as
 * `HurtPolarPlayer` above); otherwise DMAs one of two gauge-strip variants
 * every 4th frame without touching any state. */
void PolarPlayerStateShocked(void *selfArg)
{
    struct actor_self *self = selfArg;
    s32 counter = self->stateTime;

    if (counter > 0x2c) {
        QueueVramDmaTransfer(gPolarPlayerShockPalette, (void *)OBJ_PLTT, 0x20, 0x10);
        gPolarPauseLocked = 1;
        {
            register s32 state asm("r0") = 6;
            register s32 idx asm("r1") = 5;

            self->state = state;
            {
                register s32 zero asm("r2") = 0;

                self->stateTime = zero;
                self->animIndex = idx;
                {
                    register u16 anim asm("r0") = self->anims[5].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero1;
                }
                self->animTime = zero;
            }
        }
        if (gLevelState->timeTrial == 0) {
            LoseLife(gLevelState);
        }
    } else if (counter & 4) {
        QueueVramDmaTransfer(gPolarPlayerShockPalette, (void *)OBJ_PLTT, 0x20, 0x10);
    } else {
        QueueVramDmaTransfer(gPolarPlayerShockBlinkPalette, (void *)OBJ_PLTT, 0x20, 0x10);
    }
}

/* On the state-0x12 anim edge, resets `gPolarPlayerVelY`'s stall
 * clamp, and, once `self+0x20` crosses `0x2000`, spawns a secondary
 * effect object via `CreateActor` (stashed into `gRiderlessPolar`)
 * gated on that same threshold. Resets `self` to state 8/table-index
 * 7, arms `gPolarPauseLocked`, and kicks the mode transition the same
 * way as `HurtPolarPlayer`/`PolarPlayerStateShocked`. */
void PolarPlayerStateCaught(struct actor_self *self)
{
    if (self->animDone) {
        gPolarPlayerVelY = 0xFFFFF980;
        if (self->y > 0x2000)
            gRiderlessPolar = CreateActor(2, self->x, 0x2800, self->z, 0);
        ACTOR_SET_STATE(self, 8, 7);
        gPolarPauseLocked = 1;
        {
            struct game_state *player = gLevelState;

            if (player->timeTrial == 0)
                LoseLife(player);
        }
    }
}

asm(".align 2, 0");
