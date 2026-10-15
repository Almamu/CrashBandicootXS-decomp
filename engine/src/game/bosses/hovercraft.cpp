#include "game/bosses/hovercraft.hpp"
#include "game/vehicles/vehicle.hpp"
#include "game/bosses/boss_actors.hpp"
#include "engine/audio/audio.hpp"
#include "game/level/level_state.hpp"
#include "engine/gfx/cell_anim.hpp"
#include "engine/actor/actor_bg.hpp"
#include "engine/actor/actor_spawns.hpp"

extern "C" {
#include <libgcc.h>
#include "engine/sys/system.h"
#include "engine/actor/actor.h"
#include "engine/gfx/gfx.h"
#include "game/level/level.h"
#include "globals.h"
#include "engine/util/math_util.h"
}

using Engine::Actor::ActorBg;
using Engine::Actor::ActorSpawns;
using Engine::Gfx::CellAnim;
using Game::Bosses::Hovercraft;
using Game::Bosses::HovercraftCannon;
using Game::Bosses::HovercraftLauncher;
using Game::Bosses::HovercraftSideGun;
using Game::Entities::ActorSelf;
using Game::Entities::AnimPart;

/* The hovercraft boss (#664 part 11h), ROM 0x08032AF8-0x08033804, between
 * hovercraft_fireball.cpp and hovercraft_state.cpp: a bare AnimPart
 * (anim) and variables stepped through the plain function table
 * stateFuncs, the airship's twin (airship*.cpp). Since #772 an all-static
 * class (include/game/bosses/hovercraft.hpp); its members keep their C names
 * (gHovercraftX, CreateHovercraft, ...) through cxx_symbols.txt. See
 * docs/matching/archive/issue-59-0x08031784-actor.md and
 * docs/matching/archive/issue-60-61-gap-31a6c-part2.md. */

/* The hit flash (StartHovercraftHitFlash arms it): for 12 frames, the
 * hovercraft's 16-colour BG palette (BG palette 1) flips between white
 * and its colours (gHovercraftPalette) every 4 frames.
 *
 * The 3 is a variable, loaded before the timer as in the ROM. The loop
 * is an indexed `for` over the palettes, with white stored as a
 * constant: that gives the ROM's flag address loaded for the loop, the
 * white hoisted into r3 and copied to r4, and the walking pointers. The
 * C walked the pointers itself, which needed two pins on the flag's
 * address and the white (it had six more) and the timer re-read through
 * a volatile cast (#662 round 2). */
void Hovercraft::UpdateHitFlash(void)
{
    u16 *dst;
    s32 i;

    if (hitFlashTimer == 0)
        return;

    hitFlashTimer += 1;

    /* `three` is needed: inlined, the code is 2 instructions shorter */
    s32 three = 3;

    if ((three & hitFlashTimer) == 0)
        hitFlashOn ^= 1;
    if (hitFlashTimer > 0xb)
        hitFlashTimer = 0;

    dst = BG_PALETTE(1);
    for (i = 0; i < 16; i++) {
        if (hitFlashOn != 0)
            dst[i] = RGB_WHITE;
        else
            dst[i] = palette[i];
    }
}

/* The hovercraft's frame: every 16th frame its palettes' colour 15 goes
 * white, and every 8th back (SetHovercraftFlashColor's body inlined
 * twice), then the hit flash and the state function
 * (gHovercraftStateFuncs, a plain function table). */
void Hovercraft::RunState(void)
{
    s32 counter = frameCount + 1;
    frameCount = counter;

    if ((counter & 0xf) == 0)
        ApplyFlashColor(1);
    else if ((counter & 7) == 0)
        ApplyFlashColor(0);

    UpdateHitFlash();
    stateFuncs[state]();
}

/* State 2: closes in. The Z speed ramps toward the phase's speed; in
 * phase 0 the hovercraft steers toward the player (gActorList) relative
 * to its box (gHovercraftBox), within fixed bounds; in phase 1 it sweeps
 * from side to side; later phases circle on the sine table with a
 * growing radius. Once closer than 40 px, it falls back (state 3). */
void Hovercraft::StateCloseIn(void)
{
    z += velZ;
    if (phase == HOVERCRAFT_PHASE_STEER) {
        if (velZ <= HOVERCRAFT_STEER_SPEED_Z)
            velZ = velZ + HOVERCRAFT_SPEED_Z_STEP;
        else
            velZ = velZ - HOVERCRAFT_SPEED_Z_STEP;
    } else if (phase == HOVERCRAFT_PHASE_SWEEP) {
        if (velZ <= HOVERCRAFT_SWEEP_SPEED_Z - 1)
            velZ = velZ + HOVERCRAFT_SPEED_Z_STEP;
        else if (velZ > HOVERCRAFT_SWEEP_SPEED_Z)
            velZ = velZ - HOVERCRAFT_SPEED_Z_STEP;
    } else {
        if (velZ <= HOVERCRAFT_CIRCLE_SPEED_Z - 1)
            velZ = velZ + HOVERCRAFT_SPEED_Z_STEP;
        else if (velZ > HOVERCRAFT_CIRCLE_SPEED_Z)
            velZ = velZ - HOVERCRAFT_SPEED_Z_STEP;
    }

    if (phase == HOVERCRAFT_PHASE_STEER) {
        ActorSelf *pl;
        s32 vx, vy, px, py, cx, cy;

        x += velX;
        y += velY;
        pl = gActorList;
        px = pl->x;
        /* `cx` is needed: inlined, the code is 1 instruction shorter */
        cx = screenX - INT_TO_Q8(18);
        velX -= (px - cx - (box.x + box.w / 2)) >> HOVERCRAFT_STEER_SHIFT;
        vx = velX;
        /* `py` is needed: inlined, the code is 2 instructions longer */
        py = pl->y;
        /* `cy` is needed: inlined, the code is 1 instruction longer */
        cy = screenY + INT_TO_Q8(24);
        vy = velY - ((py - cy - (box.y + box.h / 2)) >> HOVERCRAFT_STEER_SHIFT);
        velY = vy;

        LIMIT_MAX(vx, INT_TO_Q8(2));
        velX = vx;
        LIMIT_MIN(vx, -INT_TO_Q8(2));
        velX = vx;
        LIMIT_MAX(vy, INT_TO_Q8(2));
        velY = vy;
        LIMIT_MIN(vy, -INT_TO_Q8(2));
        velY = vy;

        if (screenX <= 0)
            velX = INT_TO_Q8(2);
        if (screenX > INT_TO_Q8(100) - 1)
            velX = -INT_TO_Q8(2);
        if (screenY <= -INT_TO_Q8(60))
            velY = INT_TO_Q8(2);
        if (screenY > INT_TO_Q8(44) - 1)
            velY = -INT_TO_Q8(2);
    } else if (phase == HOVERCRAFT_PHASE_SWEEP) {
        x += velX;
        if (x > INT_TO_Q8(256) - 1 && velX > 0)
            velX = -INT_TO_Q8(4);
        else if (x <= -INT_TO_Q8(256) && velX < 0)
            velX = INT_TO_Q8(4);
    } else {
        s32 a;

        if ((orbitRadius += Q8(1.5)) > INT_TO_Q8(128))
            orbitRadius = INT_TO_Q8(128);

        s32 *px = &x;
        const s16 *tbl = gSineTable;

        a = ((frameCount * 30) >> 4) & 0xff;
        *px = Q8_MUL(tbl[(a + 0x40) & 0xff], orbitRadius);
        y = Q8_MUL(tbl[a], orbitRadius);
    }

    if (distance <= INT_TO_Q8(40) - 1) {
        fireTimer = attack->timing[1].delay;
        volleyCount = 0;
        EnterState(HOVERCRAFT_STATE_FALL_BACK, 0);
        phase = HOVERCRAFT_PHASE_FIRST_LEG;
        velZ = HOVERCRAFT_FALL_BACK_SPEED_Z;
    }
}

/* State 3: falls back. Y settles to 0; for the first legs (phase <= 3)
 * the Z speed ramps toward 0xae and X sweeps between +-128 px, counting
 * legs; then Z ramps toward 0x1d4 and X back to 0. Once farther than
 * 128 px, it closes in again (state 2), sweeping (phase 1) while it
 * has more than two weapons left, circling (phase 2) after. */
void Hovercraft::StateFallBack(void)
{
    s32 curY;

    x += velX;
    curY = y += velY;
    z += velZ;

    if (curY > 0)
        velY = -INT_TO_Q8(1);
    else if (curY < 0)
        velY = INT_TO_Q8(1);
    else
        velY = 0;

    if (phase <= HOVERCRAFT_PHASE_LAST_LEG) {
        s32 v = velZ;

        if (v <= HOVERCRAFT_FALL_BACK_SPEED_Z - 1)
            velZ = v + HOVERCRAFT_SPEED_Z_STEP;
        else if (v > HOVERCRAFT_FALL_BACK_SPEED_Z)
            velZ = v - HOVERCRAFT_SPEED_Z_STEP;

        if (x > INT_TO_Q8(128) - 1 && velX > 0) {
            velX = -INT_TO_Q8(2);
            phase++;
        } else if (x <= -INT_TO_Q8(128) && velX < 0) {
            velX = INT_TO_Q8(2);
            phase++;
        }
    } else {
        s32 v = velZ;

        if (v <= HOVERCRAFT_PULL_AWAY_SPEED_Z)
            velZ = v + HOVERCRAFT_SPEED_Z_STEP;
        else
            velZ = v - HOVERCRAFT_SPEED_Z_STEP;

        if (x > 0)
            velX = -INT_TO_Q8(2);
        else if (x < 0)
            velX = INT_TO_Q8(2);
        else
            velX = 0;
    }

    if (phase > HOVERCRAFT_PHASE_LAST_LEG && distance > INT_TO_Q8(128)) {
        frameCount = 0;
        orbitRadius = 0;
        fireTimer = attack->timing[1].delay;
        volleyCount = 0;
        EnterState(HOVERCRAFT_STATE_CLOSE_IN, 0);
        if (partsLeft > 2) {
            phase = HOVERCRAFT_PHASE_SWEEP;
            velZ = HOVERCRAFT_SWEEP_SPEED_Z;
        } else {
            phase = HOVERCRAFT_PHASE_CIRCLE;
            velZ = HOVERCRAFT_CIRCLE_SPEED_Z;
        }
        velX = -INT_TO_Q8(10);
    }
}

/* State 5: the hovercraft, out of weapons, sinks: the Z speed ramps
 * toward 0x99 and Y bounces, and once it is under (Y 75 px) the level
 * goes on (ActorSpawns::Resume, FinishJetpackRun). Once it is closer than
 * 21 px, BG2 goes off for good (gHovercraftGone). */
void Hovercraft::StateFall(void)
{
    if (gone == 0) {
        s32 v = velZ;
        s32 d;

        if (v <= HOVERCRAFT_SINK_SPEED_Z - 1)
            velZ = v + HOVERCRAFT_SPEED_Z_STEP;
        else if (v > HOVERCRAFT_SINK_SPEED_Z)
            velZ = v - HOVERCRAFT_SPEED_Z_STEP;

        d = velY;
        if (d <= INT_TO_Q8(1) - 1)
            velY = d + INT_TO_Q8(1);
        else if (d > INT_TO_Q8(1))
            velY = d - INT_TO_Q8(1);

        z += velZ;
        y += velY;

        if (y > INT_TO_Q8(75)) {
            ActorSpawns::Resume();
            gActorList->AsJetpackPlayer()->FinishRun();
        }
    }

    if (distance <= INT_TO_Q8(21) - 1) {
        REG_DISPCNT &= ~DISPCNT_BG2_ON;
        gone = 1;
    }
}

/* Writes the picture's map (`tileRow`, one byte per tile) into BG2's
 * current page, centred, with the tile base added: DrawAirshipMap's twin
 * (airship_map.cpp). The bias is a plain `u8` narrowing of the `s32`
 * global, and `row` is declared before `i` so `i + 1` wins the r7/ip tie.
 * Needs old_agbcc, which is why this object is in OLD_AGBCC_OBJS
 * (docs/matching/archive/issue-58-61-naked-retry.md). */
void Hovercraft::DrawMap(void *tileRow)
{
    u16 *src = (u16 *)tileRow;
    u8 *row = (u8 *)(uintptr_t)((bg2Page + 0x18) << 11) + (VRAM + (0x20 - mapCols) / 4 * 2) +
              ((0x20 - mapRows) / 2 * 32 + 2);
    s32 i, j;

    for (i = 0; i < mapRows; i++) {
        for (j = 0; j < mapCols / 2; j++) {
            u8 bias = mapTileBase;
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
void Hovercraft::Create(s32 lvl)
{
    level = lvl;
    mapCols = BOSS_PICTURE_SIZE(picture)->cols;
    mapRows = BOSS_PICTURE_SIZE(picture)->rows;
    anim = new AnimPart(keyframes, mapFrames, 1);
    EnterState(HOVERCRAFT_STATE_INACTIVE, 0);
    LoadGraphics();
    bg2PageFlip = 0;
    partsLeft = 4;
}

/* Starts the fight: state 1 (approach) at (x * 5, y * 3, z), the attack
 * parameters of `kind` and the level (gHovercraftAttacks), BG2 on and
 * zoomed from the distance, the picture's map, and the four weapons on
 * the hovercraft: the cannon, the launcher and the two side guns.
 * SpawnAirship's twin. The zoom divide is an explicit `__divsi3` call
 * and the record lookup needs the `- -` form below. */
void Hovercraft::Spawn(s32 kind, s32 sx, s32 sy, s32 sz)
{
    s32 scale;

    velZ = HOVERCRAFT_APPROACH_SPEED_Z;
    EnterState(HOVERCRAFT_STATE_APPROACH, 0);
    x = sx * 5;
    y = sy * 3;
    z = sz;
    /* `a - -b` rather than `a + b`: the latter lets fold reassociate the
     * constant table base out of `&table[kind]`, while the ROM adds the
     * level offset to the finished record address. */
    attack = (const struct hovercraft_attack *)(level * (s32)sizeof(struct hovercraft_attack) -
                                                -(intptr_t)&attacks[kind]);
    fireTimer = attack->timing[0].burstDelay;
    hp = attack->hp;
    volleyCount = 0;
    REG_DISPCNT |= DISPCNT_BG2_ON;
    bg2PageFlip = 1;
    bg2Page = 0;
    phase = HOVERCRAFT_PHASE_STEER;
    frameCount = 0;
    orbitRadius = 0;
    partsLeft = 4;
    hitFlashTimer = 0;
    hitFlashOn = 0;
    distance = z - INT_TO_Q8(CellAnim::GetDistance());
    scale = __divsi3(ACTOR_PROJECTION, distance);
    screenX = Q12_MUL(x, scale);
    screenY = Q12_MUL(scale, y);
    ActorBg::SetLayerDepth(distance);

    AnimPart *self = anim;
    s32 t = Q8_TO_INT(self->animTime);

    DrawMap((void *)self->frameAddrs[self->anims[self->animIndex].frameIndex + t]);
    HovercraftCannon::Spawn(x + INT_TO_Q8(32), y + INT_TO_Q8(48), z - INT_TO_Q8(1));
    HovercraftLauncher::Spawn(x + INT_TO_Q8(30), y - INT_TO_Q8(48), z - INT_TO_Q8(1));
    HovercraftSideGun::Spawn(x - INT_TO_Q8(65), y + INT_TO_Q8(10), z - 1, 1);
    HovercraftSideGun::Spawn(x + INT_TO_Q8(132), y + INT_TO_Q8(10), z - 1, 0);
    ActorSpawns::Pause();
}

/* The hovercraft's frame (UpdateAirship's twin): RunHovercraftState, and
 * once it is active, its animation step, the BG2 zoom from its distance,
 * and the picture's map again when the animation moved on to another
 * frame. The divide is an explicit call to `__divsi3` (the ROM reloads
 * `gHovercraftDistance` after it, which `/`'s const libcall wouldn't),
 * and the tail reads the hovercraft through a fresh local. */
void Hovercraft::Update(void)
{
    /* `prev` is needed: inlined, the code is 25 instructions shorter */
    s32 prev = Q8_TO_INT(anim->animTime);
    AnimPart *self;

    RunState();
    if (state != HOVERCRAFT_STATE_INACTIVE) {
        s32 scale;

        self = anim;
        self->animTime += self->animTimer;
        self->animDone = 0;
        if (self->GetAnimFrameBaseOffset() >= self->anims[self->animIndex].loopThreshold) {
            ANIM_REWIND(self->animTime, self->anims[self->animIndex]);
            self->animDone = 1;
        }
        distance = z - INT_TO_Q8(CellAnim::GetDistance());
        scale = __divsi3(ACTOR_PROJECTION, distance);
        screenX = Q12_MUL(x, scale);
        screenY = Q12_MUL(scale, y);
        ActorBg::SetLayerDepth(distance);

        AnimPart *cur = anim;
        s32 t = Q8_TO_INT(cur->animTime);

        if (prev != t) {
            DrawMap((void *)cur->frameAddrs[cur->anims[cur->animIndex].frameIndex + t]);
            bg2PageFlip = 1;
        }
    }
}

/* Flips BG2's page when a new map was written (gHovercraftBg2PageFlip),
 * and sets BG2's affine matrix: a uniform scale from the distance, offset
 * by the BG's centre. UpdateAirshipBg2's twin. */
void Hovercraft::UpdateBg2(void)
{
    s32 scale;
    /* `dy` is needed: inlined, the code is 1 instruction longer */
    s32 dy;
    /* `dx` is needed: inlined, the code is 2 instructions longer */
    s32 dx;

    if (bg2PageFlip != 0) {
        if (bg2Page == 0)
            REG_BG2CNT =
                BGCNT_PRIORITY(1) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(24) | BGCNT_AFF256x256;
        else
            REG_BG2CNT =
                BGCNT_PRIORITY(1) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(25) | BGCNT_AFF256x256;
        bg2PageFlip = 0;
        bg2Page ^= 1;
    }

    /* `distance << 8`: Q8_DIV(distance, INT_TO_Q8(60))'s numerator, as
     * an explicit __divsi3 call (see Spawn) */
    scale = __divsi3(distance << 8, INT_TO_Q8(60));
    dy = screenX + ActorBg::GetCenterX();
    dx = screenY + ActorBg::GetCenterY();

    REG_BG2X = INT_TO_Q8(128) - Q8_MUL(dy, scale);
    REG_BG2Y = INT_TO_Q8(128) - Q8_MUL(dx, scale);
    REG_BG2PA = scale;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = scale;
}

/* Loads the hovercraft's graphics: its palette into BG palette 1, a blank
 * 8bpp tile (tile 0xFF of char block 2), both map pages (screen blocks 24
 * and 25) filled with that tile, its tiles (ConvertHovercraftTiles), and
 * once it is active, its map and BG2. LoadAirshipGraphics's twin, with
 * the tile clear as a signed-address loop with its zero hoisted into a
 * local. */
void Hovercraft::LoadGraphics(void)
{
    intptr_t i;
    intptr_t base;
    u32 zero;

    DmaCopy16(3, palette, BG_PLTT + PALETTE_SIZE_16, PALETTE_SIZE_16);
    base = BG_CHAR_ADDR(2) + 0xFF * TILE_SIZE_8BPP;
    zero = 0;
    for (i = base + 0x3c; i >= base; i -= 4)
        *(u32 *)i = zero;
    DmaFill16(3, 0xFFFF, (void *)BG_SCREEN_ADDR(24), 2 * BG_SCREEN_SIZE);
    ConvertTiles();
    if (state != HOVERCRAFT_STATE_INACTIVE) {
        AnimPart *self;

        bg2PageFlip = 1;
        bg2Page = 0;
        self = anim;

        s32 t = Q8_TO_INT(self->animTime);

        DrawMap((void *)self->frameAddrs[self->anims[self->animIndex].frameIndex + t]);
        REG_DISPCNT |= DISPCNT_BG2_ON;
        UpdateBg2();
    }
}

/* The one-row twin of `ConvertAirshipTiles` (airship_graphics.cpp): the
 * picture's 4bpp tiles (gHovercraftPalette's data after the palette) into
 * 8bpp tiles at the top of char block 2. The height is re-read after the
 * row-pointer store (gHovercraftMapFrames is a `u32` table, whose store
 * may alias heights[]; #662 round 3) and the second loop has its
 * own counter (sharing `k` makes the first loop's reversed counter start
 * from a constant instead of `sum`'s zero register). The row header is
 * written out step by step in the ROM's order, `d` being a copy of `dst`.
 * In the nibble loop the 0xf mask is copied from a function-scope
 * variable (`u32 m = mask;`), because that is what reproduces the ROM's
 * mask copy: cse1 can't see the constant at the ANDs, so the mask stays
 * their first operand and regmove copies the mask register rather than
 * the byte; the second byte gets its own local. The copy is redundant,
 * and was adopted by owner decision over the MATCH_CONST asm it replaced
 * (#662 round 8). ConvertAirshipTiles' comment has the full analysis
 * (cse1's operand order, loop.c's preheader, reload's REG_EQUIV; #662
 * rounds 2-8). */
static inline u32 MeterPx(u32 v)
{
    return v != 0 ? 0x10 | v : 0;
}

void Hovercraft::ConvertTiles(void)
{
    s32 heights[1];
    u32 stride;
    s32 sum = 0;
    s32 off = 0x204;
    s32 k;
    s32 row_i;
    u32 *dst;
    u32 mask = 0xf;

    stride = (u32)(mapCols * mapRows + 1) >> 1 << 2;
    for (k = 0; k < 1; k++) {
        s32 x = *(s32 *)(((u8 *)palette) + off);
        heights[k] = x;
        sum += x;
        off += 4;
        mapFrames[k] = (uintptr_t)((u8 *)palette + off);
        off += stride;
        off += heights[k] << 5;
    }
    mapTileBase = 0xFF - sum;
    dst = (u32 *)(((0xFF - sum) << 6) + BG_CHAR_ADDR(2));
    for (row_i = 0; row_i <= 0; row_i++) {
        u8 *src;
        /* `row` is needed: inlined, the code is 2 instructions longer */
        u8 *row;
        /* `hp` is needed: inlined, the instructions are reordered */
        s32 *hp;
        /* `n` is needed: inlined, the code is 2 instructions longer */
        s32 n;
        s32 j;
        u32 *d;

        row = (u8 *)mapFrames[row_i];
        hp = &heights[row_i];
        d = dst;
        src = row + stride;
        n = *hp;

        for (j = 0; j < n << 4; j++) {
            u32 m = mask;
            u32 b, c, p0, p1, p2, p3;

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
void Hovercraft::Destroy(void)
{
    delete anim;
}

/* UNUSED - no caller anywhere in the ROM (checked every src/ file, the
 * category vtables and every word-aligned Thumb pointer in baserom.gba).
 * Empty. With sub_80337FC and nullsub_35 it trails DestroyHovercraft the
 * way nullsub_30 trails DestroyAirship; no table slot names them, so all
 * three keep their placeholder names (docs/naming.md: nullsub_N for an
 * empty one, sub_XXXXXXXX when in doubt). */
void Hovercraft::nullsub_34(void)
{
}

/* UNUSED - no caller anywhere in the ROM (same checks as nullsub_34).
 * Returns 0. */
s32 Hovercraft::sub_80337FC(void)
{
    return 0;
}

/* UNUSED - no caller anywhere in the ROM (same checks as nullsub_34).
 * Empty. */
void Hovercraft::nullsub_35(void)
{
}
