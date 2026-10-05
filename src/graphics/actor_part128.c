#include "core.h"
#include "actor_self.h"

/* Covers the 0x0802E0A4-0x0802F0DC gap between issue #54's chunk
 * (`actor_part61.c`, ending at `YetiStateCaught`/`sub_802E0A0`) and issue
 * #56's chunk (`actor_part43.c`, starting at `FinishJetpackRun`). Two things
 * live here:
 *
 * - The level's spawn dispatcher `CreateJetpackActor` (a 31-case `switch` over
 *   the spawn "kind", indexing the stride-40 per-kind record table
 *   `gJetpackAnimTable`) and its helpers: `SpawnJetpackActor` picks a spawn
 *   record's kind byte and forwards to it, and the run of small
 *   `new Foo(...)` constructors (`CreateJetpackCheckpointText`-`SpawnJetpackShot`) each
 *   allocate one object and hand it a fixed record of the same table.
 * - The player's vehicle object (method table gJetpackPlayerVtable,
 *   built by `CreateJetpackPlayer`/`InitJetpackPlayer`): its per-frame update
 *   (`UpdateJetpackPlayer`), sprite draw (`DrawJetpackPlayer`), damage handler
 *   (`DamageJetpackPlayer`), d-pad steering (`SteerJetpackPlayerY`/`SteerJetpackPlayerX`) and
 *   the per-state input steps (`JetpackPlayerStateFly`-`JetpackPlayerStateRollRight`). Its state
 *   lives in the `gJetpackBomberSfxTimer`-`gJetpackPlayerTiles` singletons.
 *
 * Built with old_agbcc: `DrawJetpackPlayer` only matches under it (current
 * agbcc loads its `attr` halfword straight into the callee-saved
 * register instead of via r0); every other function here compiles
 * identically under both. */

/* One record of the `gJetpackAnimTable` per-kind table (stride 40). */
struct kind_entry {
    u8 unk_00[0x20];
    s32 dx;         // 0x20 - added to the spawn X
    s32 dy;         // 0x24 - added to the spawn Y
};

/* A level spawn record, as passed to `SpawnJetpackActor`. */
struct spawn_rec {
    u8 kind[3];     // 0x00 - normal / alternate-mode / `alt`-gated kind
    u8 pad;
    s32 x;          // 0x04 - tile units (<< 8 to Q8)
    s32 y;          // 0x08
    s32 z;          // 0x0C
};

/* `actor_self` plus the hit-point word every class built here keeps at
 * +0x54. */
struct actor_hp {
    struct actor_self base;
    s32 hp;         // 0x54
};

/* The camera-ish object `DrawJetpackPlayer` reads through `self+0x30`. */
struct cam_ref {
    u8 unk_00[0x10];
    s32 depth;      // 0x10 - the depth at which sprites draw unscaled
};

struct keys_pair {
    u16 held;
    u16 pressed;
};

/* A one-byte by-value argument: the ROM stores it into its stack slot
 * with `strb` (a promoted `u8` would be stored with `str`). */
struct byte_arg {
    u8 v;
} __attribute__((packed));

extern u8 *mem_alloc(u32 size, s32 flags);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void InitActorPart(void *self, void *part, s32 b, s32 c, s32 d);
extern s32 GetAnimFrameBaseOffset(void *self);
extern u32 GetSpriteShapeSizeBits(u8 *frame);
extern void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 priority);
extern void PlayAmbientSfx(void *ctx, s32 id, s32 frame, s32 vol, struct byte_arg force);
extern u8 IsCrystalSaved(void *self);
extern void LoseLife(void *self);
extern u8 IsActorMaskAssistDue(void);
extern s32 GetCellAnimDistance(void);
extern void SetCellAnimSpeed(s32 a);
extern void UpdateActorBgScroll(s32 x, s32 y);
extern s32 GetActorBgCenterY(void);
extern s32 GetActorBgCenterX(void);
extern u8 IsSpawnCollected(void *spawn);
extern void AllocJetpackPlayerTiles(void *self);
extern void DispenseJetpackWumpa(void *self);
extern void AnimateJetpackPlayerPalette(void *self);
extern void SpawnAirship(s32 k, s32 x, s32 y, s32 z);
extern void SpawnHovercraft(s32 k, s32 x, s32 y, s32 z);
extern s32 CreateJetpackPlane(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z, void *spawn);
extern s32 CreateJetpackBomber(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 CreateJetpackHealthCrate(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 CreateJetpackQuestionCrate(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z, void *spawn);
extern s32 CreateJetpackTimeCrate(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 CreateJetpackParachuteNitro(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 CreateJetpackRocket(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 CreateJetpackRing(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 CreateJetpackBalloon(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c, s32 d);
extern s32 CreateJetpackCollectedWumpa(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern s32 CreateHovercraftCannonFlash(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern void *CreateHovercraftSideGun(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c, struct byte_arg d);
extern s32 CreateHovercraftLauncher(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern s32 CreateHovercraftCannon(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern void CreateHovercraftFireball(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern void CreateAirshipFireball(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern void CreateJetpackCannonball(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void CreateJetpackShot(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c, s32 d, s32 e);

extern struct actor_self *gActorList;
extern void (*gUnpackRleSpriteFrameFunc)(void *dst, u8 *frame);
extern struct keys_pair gKeys;
extern void *gAudioContext;
extern u8 *gLevelState;
extern void *gYeti;
extern struct kind_entry *gJetpackAnimTable;
extern s32 gJetpackBomberSfxTimer;
extern s32 gJetpackBomberCount;
extern s32 gJetpackPlayerMaxHp;
extern u8 gJetpackPauseLocked;
extern s32 gJetpackRingLastFrame;
extern s32 gJetpackRingChain;
extern s32 gJetpackFlashTimer;
extern s32 gJetpackWumpaDispenseTimer;
extern s32 gJetpackQueuedWumpa;
extern s32 gJetpackShotCooldown;
extern u8 gJetpackPlayerHalted;
extern u8 gJetpackFadeStarted;
extern u8 gJetpackPlayerInactive;
extern u8 gJetpackInputEnabled;
extern s32 gJetpackPlayerVelY;
extern s32 gJetpackPlayerVelX;
extern s32 gJetpackPlayerTileBuffer;
extern u8 *gJetpackPlayerLastFrame;
extern u8 *gJetpackPlayerTiles[2];
extern struct actor_pmf gJetpackPlayerStateFuncs[];
extern u8 gJetpackCheckpointTextVtable[];
extern u8 gJetpackExplosionVtable[];
extern u8 gJetpackPlayerVtable[];

/* `operator new`: the ROM materializes the size before the heap flags. */
static inline void *AllocActor(u32 size)
{
    return mem_alloc(size, 0x80000000);
}

/* The inlined base constructor of the hit-point classes: the hit-point
 * value is an argument, so it's materialized before the call. */
static inline void InitHpActor(struct actor_hp *obj, struct kind_entry *rec, s32 x, s32 y, s32 z, s32 hp)
{
    InitActorPart(obj, rec, x, y, z);
    obj->hp = hp;
}

/* Branchless `abs()` (`asrs`/`eors`/`subs`), as the ROM computes it. */
static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

/* Clamps a steering speed to +-0x240, keeping its sign. */
#define CLAMP_SPEED(v)                                                         \
    if (Abs(v) > 0x240)                                                        \
        (v) = (v) < 0 ? -0x240 : ((v) != 0 ? 0x240 : 0);                      \
    else (void)0

/* Once the current animation has played through, switches `self`
 * (`gYeti`) to animation sequence 3 (unless it's already
 * on it), restarting its timer from that sequence's first frame. */
void YetiStateStop(void)
{
    struct actor_self *self = gYeti;

    if (self->animIndex != 3 && self->animDone != 0) {
        self->animIndex = 3;
        {
            register u16 anim asm("r0") = self->anims[3].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            self->animTime = zero2;
        }
    }
}

s32 CreateJetpackActor(u8 kind, s32 x, s32 y, s32 z, void *spawn);

/* Spawns the object a level spawn record describes: its kind comes from
 * byte 0, byte 1 in the alternate game mode (kind 0x17 there becomes
 * 0x14) or byte 2 when `alt` is set. Kind 0x1d only spawns while
 * `IsCrystalSaved` allows it; kinds 0, 0x3e and 0x20-0x25 never do. */
s32 SpawnJetpackActor(struct spawn_rec *rec, u8 alt, s32 dz)
{
    u8 kind = rec->kind[0];
    s32 x, y, z;

    if (gLevelState[0x8c] != 0) {
        kind = rec->kind[1];
        if (kind == 0x17)
            kind = 0x14;
    } else if (alt != 0) {
        kind = rec->kind[2];
    }
    if (kind == 0x1d && !IsCrystalSaved(gLevelState))
        return 0;
    if (kind == 0 || kind == 0x3e || (u8)(kind - 0x20) <= 5)
        return 0;
    x = rec->x << 8;
    y = rec->y << 8;
    z = (rec->z << 8) + dz;
    if ((u8)(kind - 0x10) <= 2) {
        SpawnAirship(kind - 0x10, x, y, z);
    } else if (kind != 0xa) {
        return CreateJetpackActor(kind, x, y, z, rec);
    } else {
        SpawnHovercraft(0, x, y, z);
    }
    return 0;
}

/* The spawn dispatcher: offsets the position by the kind's record and
 * constructs the kind's object. Kind 23 turns into kind 20's object
 * when `IsSpawnCollected` says so; kind 31 spawns a kind-43 companion first. */
s32 CreateJetpackActor(u8 kind, s32 x, s32 y, s32 z, void *spawn)
{
    x += gJetpackAnimTable[kind].dx;
    y += gJetpackAnimTable[kind].dy;
    switch (kind) {
    case 1:
        return CreateJetpackPlane(AllocActor(0x80), &gJetpackAnimTable[kind], x, y, z, spawn);
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return CreateJetpackBomber(AllocActor(0x64), &gJetpackAnimTable[kind], x, y, z);
    case 19:
        return CreateJetpackHealthCrate(AllocActor(0x70), &gJetpackAnimTable[kind], x, y, z);
    case 23:
        if (IsSpawnCollected(spawn))
            return CreateJetpackQuestionCrate(AllocActor(0x74), &gJetpackAnimTable[20], x, y, z, spawn);
        /* fallthrough */
    case 20:
    case 21:
    case 22:
        return CreateJetpackQuestionCrate(AllocActor(0x74), &gJetpackAnimTable[kind], x, y, z, spawn);
    case 24:
    case 25:
    case 26:
    case 29:
        return CreateJetpackTimeCrate(AllocActor(0x70), &gJetpackAnimTable[kind], x, y, z);
    case 27:
        return CreateJetpackParachuteNitro(AllocActor(0x60), &gJetpackAnimTable[kind], x, y, z);
    case 28:
        return CreateJetpackRocket(AllocActor(0x68), &gJetpackAnimTable[kind], x, y, z);
    case 31:
        CreateJetpackRing(AllocActor(0x5c), &gJetpackAnimTable[43],
                    x - gJetpackAnimTable[kind].dx + gJetpackAnimTable[43].dx, y, z);
        return CreateJetpackRing(AllocActor(0x5c), &gJetpackAnimTable[kind], x, y, z);
    }
    return 0;
}

/* Plays sfx 0x17 and spawns a 1-hit-point kind-46 object at the origin. */
void CreateJetpackCheckpointText(void)
{
    struct actor_hp *obj;

    PlaySfx(gAudioContext, 0x17, 0x100);
    obj = AllocActor(0x58);
    InitHpActor(obj, &gJetpackAnimTable[46], 0, 0, 0, 1);
    obj->base.vtable = (struct actor_vtable *)gJetpackCheckpointTextVtable;
}

/* Plays sfx 4 and spawns a 1-hit-point kind-45 object at (x, y, z). */
void CreateJetpackExplosion(s32 x, s32 y, s32 z)
{
    struct actor_hp *obj;

    PlaySfx(gAudioContext, 4, 0x100);
    obj = AllocActor(0x58);
    InitHpActor(obj, &gJetpackAnimTable[45], x, y, z, 1);
    obj->base.vtable = (struct actor_vtable *)gJetpackExplosionVtable;
}

/* Kind-44 constructor. */
void SpawnJetpackCollectedWumpa(s32 a, s32 b, s32 c)
{
    CreateJetpackCollectedWumpa(AllocActor(0x64), &gJetpackAnimTable[44], a, b, c);
}

/* `CreateJetpackBalloon`-class constructor for any kind. */
s32 SpawnJetpackBalloon(u8 kind, s32 a, s32 b, s32 c, s32 d)
{
    return CreateJetpackBalloon(AllocActor(0x64), &gJetpackAnimTable[kind], a, b, c, d);
}

/* Kind-14 constructor. */
void SpawnHovercraftCannonFlash(s32 a, s32 b, s32 c)
{
    CreateHovercraftCannonFlash(AllocActor(0x5c), &gJetpackAnimTable[14], a, b, c);
}

/* Kind-13 constructor; the last argument is passed as a single byte. */
void SpawnHovercraftSideGun(s32 a, s32 b, s32 c, u8 d)
{
    struct byte_arg arg;

    arg.v = d;
    CreateHovercraftSideGun(AllocActor(0x70), &gJetpackAnimTable[13], a, b, c, arg);
}

/* Kind-12 constructor. */
void SpawnHovercraftLauncher(s32 a, s32 b, s32 c)
{
    CreateHovercraftLauncher(AllocActor(0x70), &gJetpackAnimTable[12], a, b, c);
}

/* Kind-11 constructor. */
void SpawnHovercraftCannon(s32 a, s32 b, s32 c)
{
    CreateHovercraftCannon(AllocActor(0x70), &gJetpackAnimTable[11], a, b, c);
}

/* Plays sfx 0x38 and spawns a kind-39 object. */
void SpawnHovercraftFireball(s32 x, s32 y, s32 z)
{
    PlaySfx(gAudioContext, 0x38, 0x100);
    CreateHovercraftFireball(AllocActor(0x6c), &gJetpackAnimTable[39], x, y, z);
}

/* Plays sfx 0x38 and spawns a kind-38 object. */
void SpawnAirshipFireball(s32 x, s32 y, s32 z)
{
    PlaySfx(gAudioContext, 0x38, 0x100);
    CreateAirshipFireball(AllocActor(0x6c), &gJetpackAnimTable[38], x, y, z);
}

/* Plays sfx 0x30 and spawns a kind-3 object. */
void SpawnJetpackCannonball(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    PlaySfx(gAudioContext, 0x30, 0x100);
    CreateJetpackCannonball(AllocActor(0x60), &gJetpackAnimTable[3], a, b, c, d, e);
}

/* Spawns a kind-2 object (the vehicle's shot - see `JetpackPlayerStateFly`). */
void SpawnJetpackShot(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    CreateJetpackShot(AllocActor(0x60), &gJetpackAnimTable[2], a, b, c, d, e);
}

struct actor_hp *InitJetpackPlayer(struct actor_hp *self, struct kind_entry *rec, s32 z);

/* Installs the level's per-kind table and builds the player vehicle
 * from its first record, making it the (self-linked) player object. */
void CreateJetpackPlayer(struct kind_entry *table, s32 z)
{
    struct actor_hp *p;

    gJetpackAnimTable = table;
    gActorList = &(p = InitJetpackPlayer(AllocActor(0x58), gJetpackAnimTable, z))->base;
    *(void **)&p->base.prev = p;
    *(void **)&p->base.next = p;
}

/* The player vehicle's constructor: 100 hit points (0x78 when
 * `IsActorMaskAssistDue` says so), and a reset of all its singleton state. A
 * nonzero start depth starts it in state 7. */
struct actor_hp *InitJetpackPlayer(struct actor_hp *self, struct kind_entry *rec, s32 z)
{
    s32 y = 0;

    if (z == 0)
        y = -0x9600;
    InitHpActor(self, rec, 0, y, z, 100);
    self->base.vtable = (struct actor_vtable *)gJetpackPlayerVtable;
    AllocJetpackPlayerTiles(self);
    gJetpackPlayerVelX = 0;
    if (self->base.z != 0) {
        gJetpackPlayerVelY = 0;
        ACTOR_SET_STATE(&self->base, 7, 0);
        SetCellAnimSpeed(0x28);
    } else {
        SetCellAnimSpeed(0x1e);
        gJetpackPlayerVelY = 0x180;
    }
    gJetpackInputEnabled = 0;
    gJetpackPlayerInactive = 1;
    gJetpackShotCooldown = 0;
    gJetpackFadeStarted = 0;
    gJetpackPlayerHalted = 0;
    gJetpackQueuedWumpa = 0;
    gJetpackWumpaDispenseTimer = 0;
    gJetpackFlashTimer = 0;
    gJetpackRingLastFrame = -0xbe;
    gJetpackRingChain = 0;
    gJetpackPauseLocked = 0;
    if (IsActorMaskAssistDue())
        self->hp = 0x78;
    gJetpackPlayerMaxHp = self->hp;
    gJetpackBomberCount = 0;
    gJetpackBomberSfxTimer = 0;
    return self;
}

/* The vehicle's per-frame update: engine-sound throttle, fire cooldown,
 * movement by the steering speeds (clamped to the play area unless
 * `gJetpackPlayerInactive` is set), depth, animation, then the current
 * state's handler from gJetpackPlayerStateFuncs. */
void UpdateJetpackPlayer(struct actor_hp *self)
{
    s32 x, y;

    if (gJetpackBomberCount != 0) {
        if (gJetpackBomberSfxTimer-- <= 0) {
            s32 vol;

            gJetpackBomberSfxTimer = 0x16;
            vol = gJetpackBomberCount * 48;
            if (vol > 0x100)
                vol = 0x100;
            PlaySfx(gAudioContext, 0x37, vol);
        }
        gJetpackBomberCount = 0;
    }
    if (gJetpackShotCooldown != 0)
        gJetpackShotCooldown--;
    AnimateJetpackPlayerPalette(self);
    DispenseJetpackWumpa(self);
    x = self->base.x += gJetpackPlayerVelX;
    y = self->base.y += gJetpackPlayerVelY;
    if (gJetpackPlayerInactive == 0) {
        self->base.x = x < -0x8000 ? -0x8000 : x;
        self->base.x = self->base.x > 0x8000 ? 0x8000 : self->base.x;
        self->base.y = y < -0x4b00 ? -0x4b00 : y;
        self->base.y = self->base.y > 0x4b00 ? 0x4b00 : self->base.y;
    }
    if (gJetpackPlayerHalted == 0) {
        self->base.depth = 0x1c00;
        self->base.z = (GetCellAnimDistance() << 8) + self->base.depth;
    }
    {
        s32 d = (self->base.depth >> 1) & 0x7f80;

        self->base.sortKey = d | (((Abs(self->base.y) + Abs(self->base.x)) >> 11) & 0x7f);
    }
    self->base.stateTime++;
    self->base.animTime += *(s16 *)&self->base.animTimer;
    self->base.animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->base.anims[self->base.animIndex].loopThreshold) {
        self->base.animTime -= (self->base.anims[self->base.animIndex].loopThreshold
                                - self->base.anims[self->base.animIndex].loopBase) << 8;
        self->base.animDone = 1;
    }
    UpdateActorBgScroll(self->base.x, self->base.y);
    ACTOR_PMF_CALL(&self->base, gJetpackPlayerStateFuncs);
}

asm(".align 2, 0");

/* The anim_part_instance accessors (actor_anim.c), inlined. */
static inline s32 AnimBase(struct actor_self *self)
{
    return self->animTime >> 8;
}

static inline u8 *CurFrame(struct actor_self *self)
{
    s32 base = AnimBase(self);
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;
    s32 val = table[idx].frameIndex;

    val += base;
    return (u8 *)self->frameOffsets[val];
}

static inline s32 CurAttr(struct actor_self *self)
{
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;

    return (s32)table[idx].attr << 16;
}

/* The vehicle's sprite draw: projects the position by depth (scaled
 * and double-sized when drawn behind the reference depth), culls
 * against the screen, uploads the frame's tiles into the other of the
 * two VRAM buffers when the frame changed, and queues the OAM entry. */
void DrawJetpackPlayer(struct actor_hp *self)
{
    s32 scale;
    s32 attr1 = 0;
    u8 *frame;
    u32 w, h;
    s32 halfW, halfH;
    s32 sx, sy;

    frame = CurFrame(&self->base);
    w = frame[0];
    halfW = w * 4;
    h = frame[1];
    halfH = h * 4;
    if (self->base.depth == (*(struct cam_ref **)&self->base.record)->depth) {
        scale = 0x100;
        sy = (self->base.y + GetActorBgCenterY()) >> 8;
        sx = (self->base.x + GetActorBgCenterX()) >> 8;
    } else {
        s32 depth = self->base.depth;
        s32 f;

        scale = (depth << 8) / (*(struct cam_ref **)&self->base.record)->depth;
        f = 0x1c00000 / depth;
        sy = (((self->base.y * f) >> 12) + GetActorBgCenterY()) >> 8;
        sx = (((self->base.x * f) >> 12) + GetActorBgCenterX()) >> 8;
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
        u32 attr = CurAttr(&self->base);

        attr1 |= (sy & 0xff) | ((sx & 0x1ff) << 16) | attr | GetSpriteShapeSizeBits(frame);
        if (frame != gJetpackPlayerLastFrame) {
            gJetpackPlayerTileBuffer ^= 1;
            gUnpackRleSpriteFrameFunc(gJetpackPlayerTiles[gJetpackPlayerTileBuffer], frame);
            gJetpackPlayerLastFrame = frame;
        }
        {
            /* the ROM computes the tile number in r0 */
            register u32 tile asm("r0") = GET_TILE_NUM(gJetpackPlayerTiles[gJetpackPlayerTileBuffer]);

            QueueSpriteFrameOam(attr1, tile | (self->base.palette << 12), scale);
        }
    }
}

/* Damage handler: ignored during the first 16 frames of states 2/3.
 * Out of hit points, the vehicle enters state 4 (anim 3), input is
 * locked and the steering speeds are cut; otherwise sfx 0x42 plays. */
void DamageJetpackPlayer(struct actor_hp *self, s32 dmg)
{
    if ((u32)(self->base.state - 2) <= 1 && self->base.stateTime <= 0x10)
        return;
    self->hp -= dmg;
    gJetpackFlashTimer = 0x12;
    if (self->hp <= 0) {
        self->hp = 0;
        PlaySfx(gAudioContext, 0x3a, 0x100);
        ACTOR_SET_STATE(&self->base, 4, 3);
        if (gLevelState[0x8c] == 0)
            LoseLife(gLevelState);
        gJetpackInputEnabled = 0;
        gJetpackPauseLocked = 1;
        gJetpackPlayerInactive = 1;
        SetCellAnimSpeed(0x1e);
        gJetpackPlayerVelY = 0;
        CLAMP_SPEED(gJetpackPlayerVelX);
        gJetpackPlayerVelX /= 2;
    } else {
        PlaySfx(gAudioContext, 0x42, 0x100);
    }
}

/* The key word read as a whole (the ROM does a 32-bit load). */
static inline struct keys_pair ReadKeys(void)
{
    return gKeys;
}

/* Moves a steering speed 0x40 toward zero. */
static inline void DecaySpeed(s32 *p)
{
    s32 v = *p;

    if (v >= 0) {
        if (v != 0)
            v -= 0x40;
    } else {
        v += 0x40;
    }
    *p = v;
}

/* Vertical steering (a C++ method; `this` is unused): up/down change
 * `gJetpackPlayerVelY` by 0x40 while input is enabled, otherwise it
 * decays to zero; clamped to +-0x240. */
void SteerJetpackPlayerY(void *self)
{
    if (gJetpackInputEnabled && (ReadKeys().held & 0x40))
        gJetpackPlayerVelY -= 0x40;
    else if (gJetpackInputEnabled && (ReadKeys().held & 0x80))
        gJetpackPlayerVelY += 0x40;
    else {
        DecaySpeed(&gJetpackPlayerVelY);
        if (Abs(gJetpackPlayerVelY) <= 0x40)
            gJetpackPlayerVelY = 0;
    }
    CLAMP_SPEED(gJetpackPlayerVelY);
}

/* Horizontal steering, same shape with left/right and
 * `gJetpackPlayerVelX`. */
void SteerJetpackPlayerX(void *self)
{
    if (gJetpackInputEnabled && (ReadKeys().held & 0x20))
        gJetpackPlayerVelX -= 0x40;
    else if (gJetpackInputEnabled && (ReadKeys().held & 0x10))
        gJetpackPlayerVelX += 0x40;
    else {
        DecaySpeed(&gJetpackPlayerVelX);
        if (Abs(gJetpackPlayerVelX) <= 0x40)
            gJetpackPlayerVelX = 0;
    }
    CLAMP_SPEED(gJetpackPlayerVelX);
}

/* Normal-state step: steering, then R/L enter the roll states 2/3 and
 * A fires a shot (sfx 0x24, `SpawnJetpackShot`) when the cooldown allows. */
void JetpackPlayerStateFly(struct actor_hp *self)
{
    SteerJetpackPlayerY(self);
    SteerJetpackPlayerX(self);
    if (gJetpackInputEnabled) {
        struct keys_pair keys = gKeys;

        if (keys.held & 0x200) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 2, 1);
        } else if (keys.held & 0x100) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 3, 2);
        } else if (gJetpackShotCooldown == 0 && (keys.held & 1)) {
            struct byte_arg one;
            s32 x, y;

            gJetpackShotCooldown = 0x12;
            one.v = 1;
            PlayAmbientSfx(gAudioContext, 0x24, 1000, 0xa0, one);
            x = self->base.x + 0x1200;
            y = self->base.y - 0x1800;
            SpawnJetpackShot(x, y, self->base.z + 10, (x * 0x199) >> 12, (y * 0x199) >> 12);
        }
    }
}

/* Roll state 2: a quick leftward burst for 5 frames, then the horizontal
 * speed recovers; after 0x21 frames R/L may chain another roll, and the
 * animation's end returns to state 1. */
void JetpackPlayerStateRollLeft(struct actor_hp *self)
{
    SteerJetpackPlayerY(self);
    if (self->base.stateTime <= 5) {
        gJetpackPlayerVelX += -0x100;
        if (gJetpackPlayerVelX < -0x500)
            gJetpackPlayerVelX = -0x500;
    } else {
        gJetpackPlayerVelX += 0x2d;
        if (Abs(gJetpackPlayerVelX) <= 0x2d)
            gJetpackPlayerVelX = 0;
    }
    if (self->base.stateTime > 0x21) {
        struct keys_pair keys;

        SteerJetpackPlayerX(self);
        keys = gKeys;
        if (keys.held & 0x200) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 2, 1);
        } else if (keys.held & 0x100) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 3, 2);
        }
    }
    if (self->base.animDone) {
        ACTOR_SET_STATE(&self->base, 1, 0);
    }
}

/* Roll state 3: the rightward mirror of `JetpackPlayerStateRollLeft`. */
void JetpackPlayerStateRollRight(struct actor_hp *self)
{
    SteerJetpackPlayerY(self);
    if (self->base.stateTime <= 5) {
        gJetpackPlayerVelX += 0x100;
        if (gJetpackPlayerVelX > 0x500)
            gJetpackPlayerVelX = 0x500;
    } else {
        gJetpackPlayerVelX -= 0x2d;
        if (Abs(gJetpackPlayerVelX) <= 0x2d)
            gJetpackPlayerVelX = 0;
    }
    if (self->base.stateTime > 0x21) {
        struct keys_pair keys;

        SteerJetpackPlayerX(self);
        keys = gKeys;
        if (keys.held & 0x200) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 2, 1);
        } else if (keys.held & 0x100) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 3, 2);
        }
    }
    if (self->base.animDone) {
        ACTOR_SET_STATE(&self->base, 1, 0);
    }
}

asm(".align 2, 0");
