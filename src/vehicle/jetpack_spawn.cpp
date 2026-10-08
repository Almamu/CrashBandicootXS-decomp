#include "vehicle.hpp"
#include "boss_actors.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include "byte_arg.h"
#include "system.h"
#include "audio.h"
#include "actor.h"
#include "bosses.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* The jetpack levels' spawners and the jetpack player's constructor and
 * virtual methods (#664 part 11e, include/vehicle.hpp), ROM
 * 0x0802E0A4-0x0802F0DC, between yeti.c and jetpack_run.cpp:
 *
 * - The level's spawn dispatcher `CreateJetpackActor` (a 31-case `switch`
 *   over the spawn "kind", indexing the per-kind record table
 *   `gJetpackAnimTable`) and its helpers: `SpawnJetpackActor` picks a
 *   spawn record's kind byte and forwards to it, and the run of small
 *   spawners (`CreateJetpackCheckpointText`-`SpawnJetpackShot`) each build
 *   one object from a fixed record of the same table.
 * - `JetpackPlayer` (gJetpackPlayerVtable): its constructor, `Update`,
 *   `Draw`, `Damage`, the d-pad steering and three of its states. Its
 *   state lives in the gJetpack* globals.
 *
 * Built with old_agbcp (as the C was with old_agbcc). */

/* A level spawn record, as passed to `SpawnJetpackActor`. */
struct jetpack_spawn_rec {
    u8 kind[3]; // 0x00 - normal / alternate-mode / `alt`-gated kind
    u8 pad;
    s32 x; // 0x04 - tile units (<< 8 to Q8)
    s32 y; // 0x08
    s32 z; // 0x0C
};

/* Clamps a steering speed to +-0x240, keeping its sign. */
#define CLAMP_SPEED(v)                                                         \
    if (ABS_BRANCHLESS(v) > 0x240)                                             \
        (v) = (v) < 0 ? -0x240 : ((v) != 0 ? 0x240 : 0);                      \
    else (void)0

/* Once the current animation has played through, switches the yeti to
 * animation sequence 3 (unless it's already on it), restarting its timer
 * from that sequence's first frame. */
void YetiStateStop(void)
{
    ActorSelf *self = gYeti;

    if (self->animIndex != 3 && self->animDone != 0) {
        self->animIndex = 3;
        self->animTimer = self->anims[3].duration;
        self->animDone = 0;
        self->animTime = 0;
    }
}

/* Spawns the object a level spawn record describes: its kind comes from
 * byte 0, byte 1 in the alternate game mode (kind 0x17 there becomes
 * 0x14) or byte 2 when `alt` is set. Kind 0x1d only spawns while
 * `IsCrystalSaved` allows it; kinds 0, 0x3e and 0x20-0x25 never do. */
void *SpawnJetpackActor(struct jetpack_spawn_rec *rec, u8 alt, s32 dz)
{
    u8 kind = rec->kind[0];
    s32 x, y, z;

    if (gLevelState->timeTrial != 0) {
        kind = rec->kind[1];
        if (kind == 0x17)
            kind = 0x14;
    } else if (alt != 0) {
        kind = rec->kind[2];
    }
    if (kind == 0x1d && !(u8)IsCrystalSaved(gLevelState))
        return 0;
    if (kind == 0 || kind == 0x3e || (u8)(kind - 0x20) <= 5)
        return 0;
    x = INT_TO_Q8(rec->x);
    y = INT_TO_Q8(rec->y);
    z = INT_TO_Q8(rec->z) + dz;
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
void *CreateJetpackActor(u8 kind, s32 x, s32 y, s32 z, void *spawn)
{
    x += gJetpackAnimTable[kind].spawnX;
    y += gJetpackAnimTable[kind].spawnY;
    switch (kind) {
    case 1:
        return new JetpackPlane(&gJetpackAnimTable[kind], x, y, z, (struct spawn_arg *)spawn);
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return new JetpackBomber(&gJetpackAnimTable[kind], x, y, z);
    case 19:
        return new JetpackHealthCrate(&gJetpackAnimTable[kind], x, y, z);
    case 23:
        if ((u8)IsSpawnCollected(spawn))
            return new JetpackQuestionCrate(&gJetpackAnimTable[20], x, y, z, spawn);
        /* fallthrough */
    case 20:
    case 21:
    case 22:
        return new JetpackQuestionCrate(&gJetpackAnimTable[kind], x, y, z, spawn);
    case 24:
    case 25:
    case 26:
    case 29:
        return new JetpackTimeCrate(&gJetpackAnimTable[kind], x, y, z);
    case 27:
        return new JetpackParachuteNitro(&gJetpackAnimTable[kind], x, y, z);
    case 28:
        return new JetpackRocket(&gJetpackAnimTable[kind], x, y, z);
    case 31:
        new JetpackRing(&gJetpackAnimTable[43],
                        x - gJetpackAnimTable[kind].spawnX + gJetpackAnimTable[43].spawnX, y, z);
        return new JetpackRing(&gJetpackAnimTable[kind], x, y, z);
    }
    return 0;
}

/* Plays sfx 0x17 and spawns the checkpoint banner (record 46). */
void CreateJetpackCheckpointText(void)
{
    PlaySfx(gAudioContext, SFX_CHECKPOINT, 0x100);
    new JetpackCheckpointText(&gJetpackAnimTable[46], 0, 0, 0);
}

/* Plays sfx 4 and spawns an explosion (record 45) at (x, y, z). */
void CreateJetpackExplosion(s32 x, s32 y, s32 z)
{
    PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
    new JetpackExplosion(&gJetpackAnimTable[45], x, y, z);
}

/* Kind-44 constructor. */
void SpawnJetpackCollectedWumpa(s32 a, s32 b, s32 c)
{
    new JetpackCollectedWumpa(&gJetpackAnimTable[44], a, b, c);
}

/* A balloon of record `kind` holding crate `d` (the crates' constructors,
 * jetpack_crates.cpp). */
void *SpawnJetpackBalloon(u8 kind, s32 a, s32 b, s32 c, s32 d)
{
    return new JetpackBalloon(&gJetpackAnimTable[kind], a, b, c, (JetpackBalloonCrate *)d);
}

/* Kind-14 constructor. */
void SpawnHovercraftCannonFlash(s32 a, s32 b, s32 c)
{
    new HovercraftCannonFlash(&gJetpackAnimTable[14], a, b, c);
}

/* Kind-13 constructor. The side gun's `left` is a `bool`, which g++
 * passes as a byte on the stack (`add r2, sp, #4; strb`). */
void SpawnHovercraftSideGun(s32 a, s32 b, s32 c, bool left)
{
    new HovercraftSideGun(&gJetpackAnimTable[13], a, b, c, left);
}

/* Kind-12 constructor. */
void SpawnHovercraftLauncher(s32 a, s32 b, s32 c)
{
    new HovercraftLauncher(&gJetpackAnimTable[12], a, b, c);
}

/* Kind-11 constructor. */
void SpawnHovercraftCannon(s32 a, s32 b, s32 c)
{
    new HovercraftCannon(&gJetpackAnimTable[11], a, b, c);
}

/* Plays sfx 0x38 and spawns a kind-39 object. */
void SpawnHovercraftFireball(s32 x, s32 y, s32 z)
{
    PlaySfx(gAudioContext, SFX_FIREBALL_LAUNCH, 0x100);
    new HovercraftFireball(&gJetpackAnimTable[39], x, y, z);
}

/* Plays sfx 0x38 and spawns an airship fireball (record 38). */
void SpawnAirshipFireball(s32 x, s32 y, s32 z)
{
    PlaySfx(gAudioContext, SFX_FIREBALL_LAUNCH, 0x100);
    new AirshipFireball(&gJetpackAnimTable[38], x, y, z);
}

/* Plays sfx 0x30 and fires a cannonball (record 3) at (d, e). */
void SpawnJetpackCannonball(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    PlaySfx(gAudioContext, SFX_CANNONBALL_FIRE, 0x100);
    new JetpackCannonball(&gJetpackAnimTable[3], a, b, c, d, e);
}

/* Spawns the player's shot (record 2; JetpackPlayer::StateFly). */
void SpawnJetpackShot(s32 x, s32 y, s32 z, s32 velX, s32 velY)
{
    new JetpackShot(&gJetpackAnimTable[2], x, y, z, velX, velY);
}

/* Installs the level's per-kind table and builds the player from its
 * first record, making it the (self-linked) actor list's root. */
void CreateJetpackPlayer(struct anim_table_record *table, s32 z)
{
    JetpackPlayer *p;

    gJetpackAnimTable = table;
    gActorList = p = new JetpackPlayer(gJetpackAnimTable, z);
    p->prev = p;
    p->next = p;
}

/* InitJetpackPlayer: 100 hit points (0x78 when `IsActorMaskAssistDue`
 * says so), and a reset of all its global state. A nonzero start depth
 * starts it in state 7. */
JetpackPlayer::JetpackPlayer(const struct anim_table_record *rec, s32 z)
    : HpActor(rec, 0, z == 0 ? -0x9600 : 0, z, 100)
{
    AllocTiles();
    gJetpackPlayerVelX = 0;
    if (this->z != 0) {
        gJetpackPlayerVelY = 0;
        SetState(7, 0);
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
    if ((u8)IsActorMaskAssistDue())
        hp = 0x78;
    gJetpackPlayerMaxHp = hp;
    gJetpackBomberCount = 0;
    gJetpackBomberSfxTimer = 0;
}

/* Slot 2: the engine sound's throttle, the fire cooldown, the movement by
 * the steering speeds (clamped to the play area unless
 * `gJetpackPlayerInactive` is set), the depth, the animation, then the
 * current state's method from stateFuncs. */
void JetpackPlayer::Update()
{
    s32 nx, ny;

    if (gJetpackBomberCount != 0) {
        if (gJetpackBomberSfxTimer-- <= 0) {
            s32 vol;

            gJetpackBomberSfxTimer = 0x16;
            vol = gJetpackBomberCount * 48;
            LIMIT_MAX(vol, 0x100);
            PlaySfx(gAudioContext, SFX_JETPACK_BOMBER, vol);
        }
        gJetpackBomberCount = 0;
    }
    if (gJetpackShotCooldown != 0)
        gJetpackShotCooldown--;
    AnimatePalette();
    DispenseWumpa();
    nx = x += gJetpackPlayerVelX;
    ny = y += gJetpackPlayerVelY;
    if (gJetpackPlayerInactive == 0) {
        x = CLAMP_MIN(nx, -0x8000);
        x = CLAMP_MAX(x, 0x8000);
        y = CLAMP_MIN(ny, -0x4b00);
        y = CLAMP_MAX(y, 0x4b00);
    }
    if (gJetpackPlayerHalted == 0) {
        depth = 0x1c00;
        z = INT_TO_Q8(GetCellAnimDistance()) + depth;
    }
    {
        s32 d = (depth >> 1) & 0x7f80;

        sortKey = d | (((ABS_BRANCHLESS(y) + ABS_BRANCHLESS(x)) >> 11) & 0x7f);
    }
    stateTime++;
    animTime += (s16)animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        ANIM_REWIND(animTime, anims[animIndex]);
        animDone = 1;
    }
    UpdateActorBgScroll(x, y);
    (this->*stateFuncs[state])();
}

/* AnimPart's frame accessors (actor_anim.cpp), inlined. */
static inline u8 *CurFrame(AnimPart *self)
{
    s32 base = Q8_TO_INT(self->animTime);
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;
    s32 val = table[idx].frameIndex;

    val += base;
    return (u8 *)self->frameOffsets[val];
}

static inline s32 CurAttr(AnimPart *self)
{
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;

    return (s32)table[idx].attr << 16;
}

/* Slot 3: projects the position by depth (scaled and double-sized when
 * drawn behind the reference depth), culls against the screen, uploads
 * the frame's tiles into the other of the two VRAM buffers when the
 * frame changed, and queues the OAM entry. */
void JetpackPlayer::Draw()
{
    s32 scale;
    s32 attr1 = 0;
    u8 *frame;
    u32 w, h;
    s32 halfW, halfH;
    s32 sx, sy;

    frame = CurFrame(this);
    w = frame[0];
    halfW = w * 4;
    h = frame[1];
    halfH = h * 4;
    if (depth == record->baseDepth) {
        scale = 0x100;
        sy = Q8_TO_INT(y + GetActorBgCenterY());
        sx = Q8_TO_INT(x + GetActorBgCenterX());
    } else {
        s32 d = depth;
        s32 f;

        scale = Q8_DIV(d, record->baseDepth);
        f = 0x1c00000 / d;
        sy = Q8_TO_INT(Q12_MUL(y, f) + GetActorBgCenterY());
        sx = Q8_TO_INT(Q12_MUL(x, f) + GetActorBgCenterX());
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
        u32 attr = CurAttr(this);

        attr1 |= (sy & 0xff) | ((sx & 0x1ff) << 16) | attr | GetSpriteShapeSizeBits(frame);
        if (frame != gJetpackPlayerLastFrame) {
            gJetpackPlayerTileBuffer ^= 1;
            gUnpackRleSpriteFrameFunc((u16 *)gJetpackPlayerTiles[gJetpackPlayerTileBuffer],
                                      (struct rle_frame *)frame);
            gJetpackPlayerLastFrame = frame;
        }
        {
            /* computed first, into r0 */
            u32 tile = GET_TILE_NUM(gJetpackPlayerTiles[gJetpackPlayerTileBuffer]);

            QueueSpriteFrameOam(attr1, (palette << 12) | tile, scale);
        }
    }
}

/* Slot 4: ignored during the first 16 frames of states 2/3. Out of hit
 * points, the player enters state 4 (anim 3), input is locked and the
 * steering speeds are cut; otherwise SFX_UNKNOWN_42 plays. */
void JetpackPlayer::Damage(s32 dmg)
{
    if ((u32)(state - 2) <= 1 && stateTime <= 0x10)
        return;
    hp -= dmg;
    gJetpackFlashTimer = 0x12;
    if (hp <= 0) {
        hp = 0;
        PlaySfx(gAudioContext, SFX_JETPACK_PLAYER_DOWN, 0x100);
        SetState(4, 3);
        if (gLevelState->timeTrial == 0)
            LoseLife(gLevelState);
        gJetpackInputEnabled = 0;
        gJetpackPauseLocked = 1;
        gJetpackPlayerInactive = 1;
        SetCellAnimSpeed(0x1e);
        gJetpackPlayerVelY = 0;
        CLAMP_SPEED(gJetpackPlayerVelX);
        gJetpackPlayerVelX /= 2;
    } else {
        PlaySfx(gAudioContext, SFX_UNKNOWN_42, 0x100);
    }
}

/* The key word read as a whole (the ROM does a 32-bit load). */
static inline struct held_pressed_pair ReadKeys(void)
{
    return gKeys.half;
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

/* Vertical steering: up/down change `gJetpackPlayerVelY` by 0x40 while
 * input is enabled, otherwise it decays to zero; clamped to +-0x240. */
void JetpackPlayer::SteerY()
{
    if (gJetpackInputEnabled && (ReadKeys().held & DPAD_UP))
        gJetpackPlayerVelY -= 0x40;
    else if (gJetpackInputEnabled && (ReadKeys().held & DPAD_DOWN))
        gJetpackPlayerVelY += 0x40;
    else {
        DecaySpeed(&gJetpackPlayerVelY);
        if (ABS_BRANCHLESS(gJetpackPlayerVelY) <= 0x40)
            gJetpackPlayerVelY = 0;
    }
    CLAMP_SPEED(gJetpackPlayerVelY);
}

/* Horizontal steering, same shape with left/right and
 * `gJetpackPlayerVelX`. */
void JetpackPlayer::SteerX()
{
    if (gJetpackInputEnabled && (ReadKeys().held & DPAD_LEFT))
        gJetpackPlayerVelX -= 0x40;
    else if (gJetpackInputEnabled && (ReadKeys().held & DPAD_RIGHT))
        gJetpackPlayerVelX += 0x40;
    else {
        DecaySpeed(&gJetpackPlayerVelX);
        if (ABS_BRANCHLESS(gJetpackPlayerVelX) <= 0x40)
            gJetpackPlayerVelX = 0;
    }
    CLAMP_SPEED(gJetpackPlayerVelX);
}

/* State 1, flying: steering, then R/L enter the roll states 2/3 and A
 * fires a shot (sfx 0x24, `SpawnJetpackShot`) when the cooldown allows. */
void JetpackPlayer::StateFly()
{
    SteerY();
    SteerX();
    if (gJetpackInputEnabled) {
        struct held_pressed_pair keys = gKeys.half;

        if (keys.held & L_BUTTON) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            SetState(2, 1);
        } else if (keys.held & R_BUTTON) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            SetState(3, 2);
        } else if (gJetpackShotCooldown == 0 && (keys.held & 1)) {
            struct byte_arg one;
            s32 sx, sy;

            gJetpackShotCooldown = 0x12;
            one.v = 1;
            PlayAmbientSfx(gAudioContext, SFX_JETPACK_SHOOT, 1000, 0xa0, one);
            sx = x + 0x1200;
            sy = y - 0x1800;
            SpawnJetpackShot(sx, sy, z + 10, Q12_MUL(sx, 0x199), Q12_MUL(sy, 0x199));
        }
    }
}

/* State 2, rolling left: a quick leftward burst for 5 frames, then the
 * horizontal speed recovers; after 0x21 frames R/L may chain another
 * roll, and the animation's end returns to state 1. */
void JetpackPlayer::StateRollLeft()
{
    SteerY();
    if (stateTime <= 5) {
        gJetpackPlayerVelX += -0x100;
        LIMIT_MIN(gJetpackPlayerVelX, -0x500);
    } else {
        gJetpackPlayerVelX += 0x2d;
        if (ABS_BRANCHLESS(gJetpackPlayerVelX) <= 0x2d)
            gJetpackPlayerVelX = 0;
    }
    if (stateTime > 0x21) {
        SteerX();
        struct held_pressed_pair keys = gKeys.half;

        if (keys.held & L_BUTTON) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            SetState(2, 1);
        } else if (keys.held & R_BUTTON) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            SetState(3, 2);
        }
    }
    if (animDone)
        SetState(1, 0);
}

/* State 3, rolling right: the mirror of StateRollLeft. */
void JetpackPlayer::StateRollRight()
{
    SteerY();
    if (stateTime <= 5) {
        gJetpackPlayerVelX += 0x100;
        LIMIT_MAX(gJetpackPlayerVelX, 0x500);
    } else {
        gJetpackPlayerVelX -= 0x2d;
        if (ABS_BRANCHLESS(gJetpackPlayerVelX) <= 0x2d)
            gJetpackPlayerVelX = 0;
    }
    if (stateTime > 0x21) {
        SteerX();
        struct held_pressed_pair keys = gKeys.half;

        if (keys.held & L_BUTTON) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            SetState(2, 1);
        } else if (keys.held & R_BUTTON) {
            gJetpackFlashTimer = 0x12;
            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            SetState(3, 2);
        }
    }
    if (animDone)
        SetState(1, 0);
}
