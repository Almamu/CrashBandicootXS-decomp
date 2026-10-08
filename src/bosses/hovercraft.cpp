#include "boss_actors.hpp"
#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "match.h"
#include <libgcc.h>
#include "system.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
}

/* The hovercraft boss (#664 part 11h): the jetpack ring's and the
 * collected wumpa's methods (include/vehicle.hpp), the hovercraft's
 * fireball (include/boss_actors.hpp), and the hovercraft itself, a bare
 * AnimPart (gHovercraft) and globals stepped through the plain function
 * table gHovercraftStateFuncs, the airship's twin (airship*.cpp). Its
 * functions keep their C names. See
 * docs/matching/archive/issue-59-0x08031784-actor.md and
 * docs/matching/archive/issue-60-61-gap-31a6c-part2.md. */

/* 1 hit point, not cued yet. */
JetpackRing::JetpackRing(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 1)
{
    cued = 0;
}

/* Rings can't be shot. */
s32 JetpackRing::IsUnshootable()
{
    return 1;
}

/* Flies on towards the wumpa counter; once there (out of the positive
 * quadrant past 0x1000), the collect sound and deletes itself, otherwise
 * the animation step. */
void JetpackCollectedWumpa::Update()
{
    s32 one = 1;
    s32 nx, ny;

    sortKey = one;
    nx = x += velX;
    ny = y += velY;
    if (nx <= 0x1000 || ny <= 0x1000) {
        gAudioContext->PlaySfx(SFX_HUD_COLLECT, 0x100);
        delete this;
        return;
    }
    animTime += (s16)animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        ANIM_REWIND(animTime, anims[animIndex]);
        animDone = one;
    }
}

/* Draws the current frame at the position, centred on the frame's width
 * and height, unless it is entirely off screen. Same shape as
 * ActorSelf::Draw with the scale doubling fixed off: `scaled` starts at
 * 0 (halving nothing) and becomes the 0x100 OBJ-affine bit once the
 * sprite is known to be visible. The third OAM word takes the palette as
 * its priority nibble, plus 0x800 when the sort key is behind the BG. */
void JetpackCollectedWumpa::Draw()
{
    s32 sx;
    s32 sy;
    u8 *frame;
    u32 scaled;
    s32 halfW;
    s32 halfH;
    u32 attr;
    u32 attr2;
    u16 attr2Out;
    s32 oamPriority = 0x180;
    u32 highBit = 0x800;

    {
        s32 qx = x, qy = y;

        sx = Q8_TO_INT(qx);
        sy = Q8_TO_INT(qy);
    }
    frame = GetAnimFrameData();
    scaled = 0;
    halfW = scaled ? frame[0] << 3 : frame[0] << 2;
    halfH = scaled ? frame[1] << 3 : frame[1] << 2;
    sx -= halfW;
    sy -= halfH;
    if (sy > 0x9f)
        return;
    if (sy + halfH * 2 < 0)
        return;
    if (sx > 0xef)
        return;
    if (sx + halfW * 2 < 0)
        return;

    scaled |= 0x100;
    attr = (sy & 0xff) | ((sx & 0x1ff) << 16) | GetAnimFrameAttr() | scaled;
    sx = palette;
    attr2 = sx << 12;
    if (sortKey & SORT_KEY_FLAG_BEHIND_BG)
        attr2Out = attr2 | highBit;
    else
        attr2Out = attr2;
    SetupSpriteFrameOam(frame, attr, attr2Out, oamPriority);
}

/* Adds the fruit it carries. */
JetpackCollectedWumpa::~JetpackCollectedWumpa()
{
    s32 i;

    for (i = 0; i < reward; i++)
        gLevelState->CollectWumpa();
}

/* PolarCollectedWumpa's constructor with 1 hit point: from the BG's
 * centre, the velocity that reaches the wumpa counter (0x1000, 0x1000) in
 * a number of frames proportional to the distance. */
JetpackCollectedWumpa::JetpackCollectedWumpa(const struct anim_table_record *rec, s32 x, s32 y,
                                             s32 reward)
    : HpActor(rec, x, y, 1, 1)
{
    s32 sum;
    s32 q;
    s32 nx;

    this->reward = reward;
    this->y += GetActorBgCenterY();
    nx = this->x + GetActorBgCenterX();
    this->x = nx;

    {
        s32 a1 = nx - 0x1000;
        s32 a2 = ABS_BRANCHLESS(a1);
        s32 ny = this->y;
        s32 b1 = ny - 0x1000;
        s32 b2 = ABS_BRANCHLESS(b1);

        sum = a2 + b2;
        if (sum < 0)
            sum += 0x7ff;
        q = sum >> 0xb;

        velX = __divsi3(0x1000 - nx, q);
        velY = __divsi3(0x1000 - ny, q);
    }
}

/* Collected wumpas can't be shot. */
s32 JetpackCollectedWumpa::IsUnshootable()
{
    return 1;
}

/* Takes `amount` off the hit points; at zero, the explosion palette, a
 * sound, and state 1 (exploding) with animation 1. */
void HovercraftFireball::Damage(s32 amount)
{
    s32 health = hp - amount;

    hp = health;
    if (health > 0)
        return;

    palette = 4;
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    SetState(1, 1);
}

/* The state method, then deletes itself once the explosion has played
 * through, or else the common update. */
void HovercraftFireball::Update()
{
    (this->*stateFuncs[state])();

    if (state == 1 && animDone != 0)
        delete this;
    else
        ActorSelf::Update();
}

/* 2 hit points, and the airship fireball's orbit set up (never used). */
HovercraftFireball::HovercraftFireball(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 2)
{
    centerX = x;
    centerY = y;
    radius = 0;
    velZ = 0x95;
    exploding = 0;
}

void HovercraftFireball::StateExplode()
{
    exploding = 1;
}

/* Flies on at its Z speed, which decays by 5 down to 0x14; on touching
 * the player, hurts it (6) and explodes as in Damage. */
void HovercraftFireball::StateFly()
{
    s32 sum = z;
    s32 delta = velZ;

    sum += delta;
    z = sum;
    delta -= 5;
    velZ = delta;
    if (delta <= 0x13)
        velZ = 0x14;

    if ((u8)IsTouchingPlayer(this)) {
        ((HpActor *)gActorList)->Damage(6);
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(1, 1);
    }
}

/* Update's state dispatch, without the rest. */
void HovercraftFireball::RunState()
{
    (this->*stateFuncs[state])();
}

/* Exploding fireballs can't be shot. */
s32 HovercraftFireball::IsUnshootable()
{
    return exploding;
}

/* The hit flash (StartHovercraftHitFlash arms it): for 12 frames, the
 * hovercraft's 16-colour BG palette (BG palette 1) flips between white
 * and its colours (gHovercraftPalette) every 4 frames.
 *
 * The timer is read again for the `& 3` through a volatile cast (as in
 * the C: otherwise the incremented value is reused), and the 3 is a
 * variable, loaded before the timer as in the ROM. Kept from the C: the
 * loop's two pins (it had six more). Unpinned, the flag's address is
 * hoisted out of the toggle (the ROM loads it there, and again for the
 * loop) and the white isn't loaded through r3. */
void UpdateHovercraftHitFlash(void)
{
    if (gHovercraftHitFlashTimer == 0)
        return;

    gHovercraftHitFlashTimer += 1;
    {
        s32 three = 3;
        s32 cur = *(vu16 *)&gHovercraftHitFlashTimer;

        if ((three & cur) == 0)
            gHovercraftHitFlashOn ^= 1;
    }

    if (gHovercraftHitFlashTimer > 0xb)
        gHovercraftHitFlashTimer = 0;

    {
        MATCH_HOLD_REG(u8 *, flagAddr, r5) = &gHovercraftHitFlashOn;
        MATCH_HOLD_REG(u16, white, r4) = 0x7fff;
        const u16 *src = gHovercraftPalette;
        vu16 *dst = (vu16 *)(PLTT + 0x20);
        vu16 *end = dst + 15;

        do {
            if (*flagAddr != 0)
                *dst = white;
            else
                *dst = *src;
            src++;
            dst++;
        } while ((s32)dst <= (s32)end);
    }
}

/* SetHovercraftFlashColor's two stores (hovercraft_parts.cpp). */
static inline void CommitFlashColor(u16 *p, u16 val)
{
    p[15] = val;
    gFlashObjPalette[15] = val;
}

/* The hovercraft's frame: every 16th frame its palettes' colour 15 goes
 * white, and every 8th back (SetHovercraftFlashColor's body twice), then
 * the hit flash and the state function (gHovercraftStateFuncs, a plain
 * function table).
 *
 * Kept: the colour's r1 pin (the C had two more, and two `MATCH_KEEP`s).
 * The ROM loads the white into r2 and copies it to r1; unpinned, it is
 * loaded into r1 directly, with every spelling tried (the colour a
 * `u16` or an `s32` local, an argument of the inline). */
void RunHovercraftState(void)
{
    s32 counter = gHovercraftFrameCount + 1;
    gHovercraftFrameCount = counter;

    if ((counter & 0xf) == 0) {
        if (gHovercraftFlashColorSaved == 0) {
            gHovercraftFlashSavedColor = gFlashBgPalette[15];
            gHovercraftFlashColorSaved = 1;
        }
        {
            u16 *p = gFlashBgPalette;
            MATCH_HOLD_REG(u16, val, r1) = 0x7FFF;

            CommitFlashColor(p, val);
        }
    } else if ((counter & 7) == 0) {
        if (gHovercraftFlashColorSaved == 0) {
            gHovercraftFlashSavedColor = gFlashBgPalette[15];
            gHovercraftFlashColorSaved = 1;
        }
        {
            u16 *p = gFlashBgPalette;

            CommitFlashColor(p, gHovercraftFlashSavedColor);
        }
    }

    UpdateHovercraftHitFlash();
    gHovercraftStateFuncs[gHovercraftState]();
}

/* State 2: closes in. The Z speed ramps toward the phase's speed; in
 * phase 0 the hovercraft steers toward the player (gActorList) relative
 * to its box (gHovercraftBox), within fixed bounds; in phase 1 it sweeps
 * from side to side; later phases circle on the sine table with a
 * growing radius. Once close enough (0x27ff), it falls back (state 3). */
void HovercraftStateCloseIn(void)
{
    gHovercraftZ += gHovercraftVelZ;
    if (gHovercraftPhase == 0) {
        if (gHovercraftVelZ <= 0x98)
            gHovercraftVelZ = gHovercraftVelZ + 1;
        else
            gHovercraftVelZ = gHovercraftVelZ - 1;
    } else if (gHovercraftPhase == 1) {
        if (gHovercraftVelZ <= 0x3f)
            gHovercraftVelZ = gHovercraftVelZ + 1;
        else if (gHovercraftVelZ > 0x40)
            gHovercraftVelZ = gHovercraftVelZ - 1;
    } else {
        if (gHovercraftVelZ <= 0x69)
            gHovercraftVelZ = gHovercraftVelZ + 1;
        else if (gHovercraftVelZ > 0x6a)
            gHovercraftVelZ = gHovercraftVelZ - 1;
    }

    if (gHovercraftPhase == 0) {
        ActorSelf *pl;
        s32 vx, vy, px, py, cx, cy;

        gHovercraftX += gHovercraftVelX;
        gHovercraftY += gHovercraftVelY;
        pl = gActorList;
        px = pl->x;
        cx = gHovercraftScreenX - 0x1200;
        gHovercraftVelX -= (px - cx - (gHovercraftBox.x + gHovercraftBox.w / 2)) >> 12;
        vx = gHovercraftVelX;
        py = pl->y;
        cy = gHovercraftScreenY + 0x1800;
        vy = gHovercraftVelY - ((py - cy - (gHovercraftBox.y + gHovercraftBox.h / 2)) >> 12);
        gHovercraftVelY = vy;

        LIMIT_MAX(vx, 0x200);
        gHovercraftVelX = vx;
        LIMIT_MIN(vx, -0x200);
        gHovercraftVelX = vx;
        LIMIT_MAX(vy, 0x200);
        gHovercraftVelY = vy;
        LIMIT_MIN(vy, -0x200);
        gHovercraftVelY = vy;

        if (gHovercraftScreenX <= 0)
            gHovercraftVelX = 0x200;
        if (gHovercraftScreenX > 0x63ff)
            gHovercraftVelX = -0x200;
        if (gHovercraftScreenY <= -0x3c00)
            gHovercraftVelY = 0x200;
        if (gHovercraftScreenY > 0x2bff)
            gHovercraftVelY = -0x200;
    } else if (gHovercraftPhase == 1) {
        gHovercraftX += gHovercraftVelX;
        if (gHovercraftX > 0xffff && gHovercraftVelX > 0)
            gHovercraftVelX = -0x400;
        else if (gHovercraftX <= -0x10000 && gHovercraftVelX < 0)
            gHovercraftVelX = 0x400;
    } else {
        s32 a;

        if ((gHovercraftOrbitRadius += 0x180) > 0x8000)
            gHovercraftOrbitRadius = 0x8000;
        {
            s32 *px = &gHovercraftX;
            const s16 *tbl = gSineTable;

            a = ((gHovercraftFrameCount * 30) >> 4) & 0xff;
            *px = Q8_MUL(tbl[(a + 0x40) & 0xff], gHovercraftOrbitRadius);
            gHovercraftY = Q8_MUL(tbl[a], gHovercraftOrbitRadius);
        }
    }

    if (gHovercraftDistance <= 0x27ff) {
        gHovercraftFireTimer = gHovercraftAttack->timing[1].delay;
        gHovercraftVolleyCount = 0;
        EnterHovercraftState(3, 0);
        gHovercraftPhase = 0;
        gHovercraftVelZ = 0xae;
    }
}

/* State 3: falls back. Y settles to 0; for the first legs (phase <= 3)
 * the Z speed ramps toward 0xae and X sweeps between +-0x8000, counting
 * legs; then Z ramps toward 0x1d4 and X back to 0. Once far enough
 * (0x8000), it closes in again (state 2), sweeping (phase 1) while it
 * has more than two weapons left, circling (phase 2) after. */
void HovercraftStateFallBack(void)
{
    s32 y;

    gHovercraftX += gHovercraftVelX;
    y = gHovercraftY += gHovercraftVelY;
    gHovercraftZ += gHovercraftVelZ;

    if (y > 0)
        gHovercraftVelY = -0x100;
    else if (y < 0)
        gHovercraftVelY = 0x100;
    else
        gHovercraftVelY = 0;

    if (gHovercraftPhase <= 3) {
        s32 v = gHovercraftVelZ;

        if (v <= 0xad)
            gHovercraftVelZ = v + 1;
        else if (v > 0xae)
            gHovercraftVelZ = v - 1;

        if (gHovercraftX > 0x7fff && gHovercraftVelX > 0) {
            gHovercraftVelX = -0x200;
            gHovercraftPhase++;
        } else if (gHovercraftX <= -0x8000 && gHovercraftVelX < 0) {
            gHovercraftVelX = 0x200;
            gHovercraftPhase++;
        }
    } else {
        s32 v = gHovercraftVelZ;

        if (v <= 0x1d4)
            gHovercraftVelZ = v + 1;
        else
            gHovercraftVelZ = v - 1;

        if (gHovercraftX > 0)
            gHovercraftVelX = -0x200;
        else if (gHovercraftX < 0)
            gHovercraftVelX = 0x200;
        else
            gHovercraftVelX = 0;
    }

    if (gHovercraftPhase > 3 && gHovercraftDistance > 0x8000) {
        gHovercraftFrameCount = 0;
        gHovercraftOrbitRadius = 0;
        gHovercraftFireTimer = gHovercraftAttack->timing[1].delay;
        gHovercraftVolleyCount = 0;
        EnterHovercraftState(2, 0);
        if (gHovercraftPartsLeft > 2) {
            gHovercraftPhase = 1;
            gHovercraftVelZ = 0x40;
        } else {
            gHovercraftPhase = 2;
            gHovercraftVelZ = 0x6a;
        }
        gHovercraftVelX = -0xa00;
    }
}

/* State 5: the hovercraft, out of weapons, sinks: the Z speed ramps
 * toward 0x99 and Y bounces, and once it is under (Y 0x4b00) the level
 * goes on (ResumeActorSpawns, FinishJetpackRun). Once it is close enough
 * (0x14ff), BG2 goes off for good (gHovercraftGone). */
void HovercraftStateFall(void)
{
    if (gHovercraftGone == 0) {
        s32 v = gHovercraftVelZ;
        s32 d;

        if (v <= 0x98)
            gHovercraftVelZ = v + 1;
        else if (v > 0x99)
            gHovercraftVelZ = v - 1;

        d = gHovercraftVelY;
        if (d <= 0xff)
            gHovercraftVelY = d + 0x100;
        else if (d > 0x100)
            gHovercraftVelY = d - 0x100;

        gHovercraftZ += gHovercraftVelZ;
        gHovercraftY += gHovercraftVelY;

        if (gHovercraftY > 0x4b00) {
            ResumeActorSpawns();
            ((JetpackPlayer *)gActorList)->FinishRun();
        }
    }

    if (gHovercraftDistance <= 0x14ff) {
        REG_DISPCNT &= 0xfbff;
        gHovercraftGone = 1;
    }
}

/* Writes the picture's map (`tileRow`, one byte per tile) into BG2's
 * current page, centred, with the tile base added: DrawAirshipMap's twin
 * (airship_map.cpp). The bias is a plain `u8` narrowing of the `s32`
 * global, and `row` is declared before `i` so `i + 1` wins the r7/ip tie.
 * Needs old_agbcc, which is why this object is in OLD_AGBCC_OBJS
 * (docs/matching/archive/issue-58-61-naked-retry.md). */
void DrawHovercraftMap(void *tileRow)
{
    u16 *src = (u16 *)tileRow;
    u8 *row = (u8 *)((gHovercraftBg2Page + 0x18) << 11) +
              (VRAM + (0x20 - gHovercraftMapCols) / 4 * 2) +
              ((0x20 - gHovercraftMapRows) / 2 * 32 + 2);
    s32 i, j;

    for (i = 0; i < gHovercraftMapRows; i++) {
        for (j = 0; j < gHovercraftMapCols / 2; j++) {
            u8 bias = gHovercraftMapTileBase;
            u16 lo = *src++ + bias;
            u16 hi = *src++ + bias;
            ((u16 *)row)[j] = lo | (hi << 8);
        }
        row += 0x20;
    }
}

/* Sets the hovercraft up for level `level`: the picture's size, its
 * animation (`new AnimPart`, an IWRAM allocation; the ROM takes
 * &gHovercraft before the allocation, as g++ does), state 0 (inactive),
 * its graphics, and four weapons. CreateAirship's twin. */
void CreateHovercraft(s32 level)
{
    gHovercraftLevel = level;
    gHovercraftMapCols = BOSS_PICTURE_SIZE(gHovercraftPicture)->cols;
    gHovercraftMapRows = BOSS_PICTURE_SIZE(gHovercraftPicture)->rows;
    gHovercraft = new AnimPart((struct anim_frame_record *)gHovercraftKeyframes,
                               (u32 *)gHovercraftMapFrames, 1);
    EnterHovercraftState(0, 0);
    LoadHovercraftGraphics();
    gHovercraftBg2PageFlip = 0;
    gHovercraftPartsLeft = 4;
}

/* Starts the fight: state 1 (approach) at (x * 5, y * 3, z), the attack
 * parameters of `kind` and the level (gHovercraftAttacks), BG2 on and
 * zoomed from the distance, the picture's map, and the four weapons on
 * the hovercraft: the cannon, the launcher and the two side guns.
 * SpawnAirship's twin. The zoom divide is an explicit `__divsi3` call
 * and the record lookup needs the `- -` form below. */
void SpawnHovercraft(s32 kind, s32 x, s32 y, s32 z)
{
    s32 scale;

    gHovercraftVelZ = 0x66;
    EnterHovercraftState(1, 0);
    gHovercraftX = x * 5;
    gHovercraftY = y * 3;
    gHovercraftZ = z;
    /* `a - -b` rather than `a + b`: the latter lets fold reassociate the
     * constant table base out of `&table[kind]`, while the ROM adds the
     * level offset to the finished record address. */
    gHovercraftAttack =
        (const struct hovercraft_attack *)(gHovercraftLevel *
                                               (s32)sizeof(struct hovercraft_attack) -
                                           -(s32)&gHovercraftAttacks[kind]);
    gHovercraftFireTimer = gHovercraftAttack->timing[0].burstDelay;
    gHovercraftHp = gHovercraftAttack->hp;
    gHovercraftVolleyCount = 0;
    REG_DISPCNT |= DISPCNT_BG2_ON;
    gHovercraftBg2PageFlip = 1;
    gHovercraftBg2Page = 0;
    gHovercraftPhase = 0;
    gHovercraftFrameCount = 0;
    gHovercraftOrbitRadius = 0;
    gHovercraftPartsLeft = 4;
    gHovercraftHitFlashTimer = 0;
    gHovercraftHitFlashOn = 0;
    gHovercraftDistance = gHovercraftZ - INT_TO_Q8(GetCellAnimDistance());
    scale = __divsi3(0x1C00000, gHovercraftDistance);
    gHovercraftScreenX = Q12_MUL(gHovercraftX, scale);
    gHovercraftScreenY = Q12_MUL(scale, gHovercraftY);
    SetActorBgLayerDepth(gHovercraftDistance);
    {
        AnimPart *self = gHovercraft;
        s32 t = Q8_TO_INT(self->animTime);

        DrawHovercraftMap((void *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
    }
    SpawnHovercraftCannon(gHovercraftX + 0x2000, gHovercraftY + 0x3000, gHovercraftZ - 0x100);
    SpawnHovercraftLauncher(gHovercraftX + 0x1e00, gHovercraftY - 0x3000, gHovercraftZ - 0x100);
    SpawnHovercraftSideGun(gHovercraftX - 0x4100, gHovercraftY + 0xa00, gHovercraftZ - 1, 1);
    SpawnHovercraftSideGun(gHovercraftX + 0x8400, gHovercraftY + 0xa00, gHovercraftZ - 1, 0);
    PauseActorSpawns();
}

/* The hovercraft's frame (UpdateAirship's twin): RunHovercraftState, and
 * once it is active, its animation step, the BG2 zoom from its distance,
 * and the picture's map again when the animation moved on to another
 * frame. The divide is an explicit call to `__divsi3` (the ROM reloads
 * `gHovercraftDistance` after it, which `/`'s const libcall wouldn't),
 * and the tail reads the hovercraft through a fresh local. */
void UpdateHovercraft(void)
{
    s32 prev = Q8_TO_INT(gHovercraft->animTime);
    AnimPart *self;

    RunHovercraftState();
    if (gHovercraftState != 0) {
        s32 scale;

        self = gHovercraft;
        self->animTime += (s16)self->animTimer;
        self->animDone = 0;
        if (self->GetAnimFrameBaseOffset() >= self->anims[self->animIndex].loopThreshold) {
            ANIM_REWIND(self->animTime, self->anims[self->animIndex]);
            self->animDone = 1;
        }
        gHovercraftDistance = gHovercraftZ - INT_TO_Q8(GetCellAnimDistance());
        scale = __divsi3(0x1C00000, gHovercraftDistance);
        gHovercraftScreenX = Q12_MUL(gHovercraftX, scale);
        gHovercraftScreenY = Q12_MUL(scale, gHovercraftY);
        SetActorBgLayerDepth(gHovercraftDistance);
        {
            AnimPart *cur = gHovercraft;
            s32 t = Q8_TO_INT(cur->animTime);

            if (prev != t) {
                DrawHovercraftMap(
                    (void *)cur->frameOffsets[cur->anims[cur->animIndex].frameIndex + t]);
                gHovercraftBg2PageFlip = 1;
            }
        }
    }
}

/* Flips BG2's page when a new map was written (gHovercraftBg2PageFlip),
 * and sets BG2's affine matrix: a uniform scale from the distance, offset
 * by the BG's centre. UpdateAirshipBg2's twin. */
void UpdateHovercraftBg2(void)
{
    s32 scale;
    s32 dy;
    s32 dx;

    if (gHovercraftBg2PageFlip != 0) {
        if (gHovercraftBg2Page == 0)
            REG_BG2CNT = 0x5809;
        else
            REG_BG2CNT = 0x5909;
        gHovercraftBg2PageFlip = 0;
        gHovercraftBg2Page ^= 1;
    }

    scale = __divsi3(gHovercraftDistance << 8, 0x3c00);
    dy = gHovercraftScreenX + GetActorBgCenterX();
    dx = gHovercraftScreenY + GetActorBgCenterY();

    REG_BG2X = 0x8000 - Q8_MUL(dy, scale);
    REG_BG2Y = 0x8000 - Q8_MUL(dx, scale);
    REG_BG2PA = scale;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = scale;
}

/* Loads the hovercraft's graphics: its palette into BG palette 1, a blank
 * tile and a blank char block 3, its tiles (ConvertHovercraftTiles), and
 * once it is active, its map and BG2. LoadAirshipGraphics's twin, with
 * the tile clear as a signed-address loop with its zero hoisted into a
 * local. */
void LoadHovercraftGraphics(void)
{
    s32 i;
    s32 base;
    u32 zero;

    DmaCopy16(3, gHovercraftPalette, (void *)(PLTT + 0x20), 0x20);
    base = VRAM + 0xBFC0;
    zero = 0;
    for (i = base + 0x3c; i >= base; i -= 4)
        *(u32 *)i = zero;
    DmaFill16(3, 0xFFFF, (void *)(VRAM + 0xC000), 0x1000);
    ConvertHovercraftTiles();
    if (gHovercraftState != 0) {
        AnimPart *self;

        gHovercraftBg2PageFlip = 1;
        gHovercraftBg2Page = 0;
        self = gHovercraft;
        {
            s32 t = Q8_TO_INT(self->animTime);

            DrawHovercraftMap(
                (void *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
        }
        REG_DISPCNT |= DISPCNT_BG2_ON;
        UpdateHovercraftBg2();
    }
}

/* The one-row twin of `ConvertAirshipTiles` (airship_graphics.cpp): the
 * picture's 4bpp tiles (gHovercraftPalette's data after the palette) into
 * 8bpp tiles at the top of char block 2. The height is re-read after the
 * row-pointer store (the `MATCH_KEEP_MEM`) and the second loop has its
 * own counter (sharing `k` makes the first loop's reversed counter start
 * from a constant instead of `sum`'s zero register). The row header is
 * written out step by step in the ROM's order, `d` being a copy of `dst`.
 * In the nibble loop the 0xf mask is an opaque value ANDed with each byte
 * (`m & b`), so gcc copies the mask rather than the byte, as the ROM
 * does, and the second byte gets its own local. */
static inline u32 MeterPx(u32 v)
{
    u32 r = 0;
    if (v != 0)
        r = 0x10 | v;
    return r;
}

void ConvertHovercraftTiles(void)
{
    s32 heights[1];
    u32 stride;
    s32 sum = 0;
    s32 off = 0x204;
    s32 k;
    s32 row_i;
    u32 *dst;
    u8 **rows = (u8 **)gHovercraftMapFrames;
    u32 m;

    stride = (u32)(gHovercraftMapCols * gHovercraftMapRows + 1) >> 1 << 2;
    for (k = 0; k < 1; k++) {
        s32 x = *(s32 *)(((u8 *)gHovercraftPalette) + off);
        heights[k] = x;
        sum += x;
        off += 4;
        rows[k] = ((u8 *)gHovercraftPalette) + off;
        /* forces the height to be re-read (the ROM's `ldm r1!`) */
        MATCH_KEEP_MEM(heights[k]);
        off += stride;
        off += heights[k] << 5;
    }
    gHovercraftMapTileBase = 0xFF - sum;
    dst = (u32 *)(((0xFF - sum) << 6) + (VRAM + 0x8000));
    for (row_i = 0; row_i <= 0; row_i++) {
        u8 *src;
        u8 *row;
        s32 *hp;
        s32 n;
        s32 j;
        u32 *d;

        row = (u8 *)gHovercraftMapFrames[row_i];
        hp = &heights[row_i];
        d = dst;
        src = row + stride;
        n = *hp;

        for (j = 0; j < n << 4; j++) {
            u32 b, c, p0, p1, p2, p3;

            /* the 0xf mask without a constant-set register: the mask is
             * the AND's first operand, as in the ROM */
            MATCH_CONST(m, 0xf);
            b = *src;
            p0 = m & b;
            p0 = MeterPx(p0);
            p1 = (b >> 4) & m;
            src++;
            p1 = MeterPx(p1);
            c = *src;
            p2 = m & c;
            p2 = MeterPx(p2);
            p3 = (c >> 4) & m;
            src++;
            p3 = MeterPx(p3);
            *d++ = p0 | (p1 << 8) | (p2 << 16) | (p3 << 24);
        }
        dst = d;
    }
}

/* Frees the hovercraft's animation (`delete` of an AnimPart, which has
 * no destructor: a plain mem_free, AnimPart's operator delete). */
void DestroyHovercraft(void)
{
    delete gHovercraft;
}

/* UNUSED - no caller anywhere in the ROM (checked every src/ file, the
 * category vtables and every word-aligned Thumb pointer in baserom.gba).
 * Empty. With sub_80337FC and nullsub_35 it trails DestroyHovercraft the
 * way nullsub_30 trails DestroyAirship; no table slot names them, so all
 * three keep their placeholder names (docs/naming.md: nullsub_N for an
 * empty one, sub_XXXXXXXX when in doubt). */
void nullsub_34(void)
{
}

/* UNUSED - no caller anywhere in the ROM (same checks as nullsub_34).
 * Returns 0. */
s32 sub_80337FC(void)
{
    return 0;
}

/* UNUSED - no caller anywhere in the ROM (same checks as nullsub_34).
 * Empty. */
void nullsub_35(void)
{
}
