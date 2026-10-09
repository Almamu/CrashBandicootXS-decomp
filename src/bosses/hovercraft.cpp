#include "hovercraft.hpp"
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

/* The hovercraft boss (#664 part 11h), ROM 0x08032AF8-0x08033804, between
 * hovercraft_fireball.cpp and hovercraft_state.cpp: a bare AnimPart
 * (anim) and variables stepped through the plain function table
 * stateFuncs, the airship's twin (airship*.cpp). Since #772 an all-static
 * class (include/hovercraft.hpp); its members keep their C names
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
    const u16 *src;
    u16 *dst;
    s32 i;

    if (hitFlashTimer == 0)
        return;

    hitFlashTimer += 1;
    {
        s32 three = 3;

        if ((three & hitFlashTimer) == 0)
            hitFlashOn ^= 1;
    }
    if (hitFlashTimer > 0xb)
        hitFlashTimer = 0;

    src = palette;
    dst = (u16 *)(PLTT + 0x20);
    for (i = 0; i < 16; i++) {
        if (hitFlashOn != 0)
            dst[i] = RGB_WHITE;
        else
            dst[i] = src[i];
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
 * growing radius. Once close enough (0x27ff), it falls back (state 3). */
void Hovercraft::StateCloseIn(void)
{
    z += velZ;
    if (phase == 0) {
        if (velZ <= 0x98)
            velZ = velZ + 1;
        else
            velZ = velZ - 1;
    } else if (phase == 1) {
        if (velZ <= 0x3f)
            velZ = velZ + 1;
        else if (velZ > 0x40)
            velZ = velZ - 1;
    } else {
        if (velZ <= 0x69)
            velZ = velZ + 1;
        else if (velZ > 0x6a)
            velZ = velZ - 1;
    }

    if (phase == 0) {
        ActorSelf *pl;
        s32 vx, vy, px, py, cx, cy;

        x += velX;
        y += velY;
        pl = gActorList;
        px = pl->x;
        cx = screenX - 0x1200;
        velX -= (px - cx - (box.x + box.w / 2)) >> 12;
        vx = velX;
        py = pl->y;
        cy = screenY + 0x1800;
        vy = velY - ((py - cy - (box.y + box.h / 2)) >> 12);
        velY = vy;

        LIMIT_MAX(vx, 0x200);
        velX = vx;
        LIMIT_MIN(vx, -0x200);
        velX = vx;
        LIMIT_MAX(vy, 0x200);
        velY = vy;
        LIMIT_MIN(vy, -0x200);
        velY = vy;

        if (screenX <= 0)
            velX = 0x200;
        if (screenX > 0x63ff)
            velX = -0x200;
        if (screenY <= -0x3c00)
            velY = 0x200;
        if (screenY > 0x2bff)
            velY = -0x200;
    } else if (phase == 1) {
        x += velX;
        if (x > 0xffff && velX > 0)
            velX = -0x400;
        else if (x <= -0x10000 && velX < 0)
            velX = 0x400;
    } else {
        s32 a;

        if ((orbitRadius += 0x180) > 0x8000)
            orbitRadius = 0x8000;
        {
            s32 *px = &x;
            const s16 *tbl = gSineTable;

            a = ((frameCount * 30) >> 4) & 0xff;
            *px = Q8_MUL(tbl[(a + 0x40) & 0xff], orbitRadius);
            y = Q8_MUL(tbl[a], orbitRadius);
        }
    }

    if (distance <= 0x27ff) {
        fireTimer = attack->timing[1].delay;
        volleyCount = 0;
        EnterState(3, 0);
        phase = 0;
        velZ = 0xae;
    }
}

/* State 3: falls back. Y settles to 0; for the first legs (phase <= 3)
 * the Z speed ramps toward 0xae and X sweeps between +-0x8000, counting
 * legs; then Z ramps toward 0x1d4 and X back to 0. Once far enough
 * (0x8000), it closes in again (state 2), sweeping (phase 1) while it
 * has more than two weapons left, circling (phase 2) after. */
void Hovercraft::StateFallBack(void)
{
    s32 curY;

    x += velX;
    curY = y += velY;
    z += velZ;

    if (curY > 0)
        velY = -0x100;
    else if (curY < 0)
        velY = 0x100;
    else
        velY = 0;

    if (phase <= 3) {
        s32 v = velZ;

        if (v <= 0xad)
            velZ = v + 1;
        else if (v > 0xae)
            velZ = v - 1;

        if (x > 0x7fff && velX > 0) {
            velX = -0x200;
            phase++;
        } else if (x <= -0x8000 && velX < 0) {
            velX = 0x200;
            phase++;
        }
    } else {
        s32 v = velZ;

        if (v <= 0x1d4)
            velZ = v + 1;
        else
            velZ = v - 1;

        if (x > 0)
            velX = -0x200;
        else if (x < 0)
            velX = 0x200;
        else
            velX = 0;
    }

    if (phase > 3 && distance > 0x8000) {
        frameCount = 0;
        orbitRadius = 0;
        fireTimer = attack->timing[1].delay;
        volleyCount = 0;
        EnterState(2, 0);
        if (partsLeft > 2) {
            phase = 1;
            velZ = 0x40;
        } else {
            phase = 2;
            velZ = 0x6a;
        }
        velX = -0xa00;
    }
}

/* State 5: the hovercraft, out of weapons, sinks: the Z speed ramps
 * toward 0x99 and Y bounces, and once it is under (Y 0x4b00) the level
 * goes on (ResumeActorSpawns, FinishJetpackRun). Once it is close enough
 * (0x14ff), BG2 goes off for good (gHovercraftGone). */
void Hovercraft::StateFall(void)
{
    if (gone == 0) {
        s32 v = velZ;
        s32 d;

        if (v <= 0x98)
            velZ = v + 1;
        else if (v > 0x99)
            velZ = v - 1;

        d = velY;
        if (d <= 0xff)
            velY = d + 0x100;
        else if (d > 0x100)
            velY = d - 0x100;

        z += velZ;
        y += velY;

        if (y > 0x4b00) {
            ResumeActorSpawns();
            ((JetpackPlayer *)gActorList)->FinishRun();
        }
    }

    if (distance <= 0x14ff) {
        REG_DISPCNT &= 0xfbff;
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
    u8 *row = (u8 *)((bg2Page + 0x18) << 11) + (VRAM + (0x20 - mapCols) / 4 * 2) +
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
    anim = new AnimPart((struct anim_frame_record *)keyframes, mapFrames, 1);
    EnterState(0, 0);
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

    velZ = 0x66;
    EnterState(1, 0);
    x = sx * 5;
    y = sy * 3;
    z = sz;
    /* `a - -b` rather than `a + b`: the latter lets fold reassociate the
     * constant table base out of `&table[kind]`, while the ROM adds the
     * level offset to the finished record address. */
    attack = (const struct hovercraft_attack *)(level * (s32)sizeof(struct hovercraft_attack) -
                                                -(s32)&attacks[kind]);
    fireTimer = attack->timing[0].burstDelay;
    hp = attack->hp;
    volleyCount = 0;
    REG_DISPCNT |= DISPCNT_BG2_ON;
    bg2PageFlip = 1;
    bg2Page = 0;
    phase = 0;
    frameCount = 0;
    orbitRadius = 0;
    partsLeft = 4;
    hitFlashTimer = 0;
    hitFlashOn = 0;
    distance = z - INT_TO_Q8(GetCellAnimDistance());
    scale = __divsi3(0x1C00000, distance);
    screenX = Q12_MUL(x, scale);
    screenY = Q12_MUL(scale, y);
    SetActorBgLayerDepth(distance);
    {
        AnimPart *self = anim;
        s32 t = Q8_TO_INT(self->animTime);

        DrawMap((void *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
    }
    SpawnHovercraftCannon(x + 0x2000, y + 0x3000, z - 0x100);
    SpawnHovercraftLauncher(x + 0x1e00, y - 0x3000, z - 0x100);
    SpawnHovercraftSideGun(x - 0x4100, y + 0xa00, z - 1, 1);
    SpawnHovercraftSideGun(x + 0x8400, y + 0xa00, z - 1, 0);
    PauseActorSpawns();
}

/* The hovercraft's frame (UpdateAirship's twin): RunHovercraftState, and
 * once it is active, its animation step, the BG2 zoom from its distance,
 * and the picture's map again when the animation moved on to another
 * frame. The divide is an explicit call to `__divsi3` (the ROM reloads
 * `gHovercraftDistance` after it, which `/`'s const libcall wouldn't),
 * and the tail reads the hovercraft through a fresh local. */
void Hovercraft::Update(void)
{
    s32 prev = Q8_TO_INT(anim->animTime);
    AnimPart *self;

    RunState();
    if (state != 0) {
        s32 scale;

        self = anim;
        self->animTime += (s16)self->animTimer;
        self->animDone = 0;
        if (self->GetAnimFrameBaseOffset() >= self->anims[self->animIndex].loopThreshold) {
            ANIM_REWIND(self->animTime, self->anims[self->animIndex]);
            self->animDone = 1;
        }
        distance = z - INT_TO_Q8(GetCellAnimDistance());
        scale = __divsi3(0x1C00000, distance);
        screenX = Q12_MUL(x, scale);
        screenY = Q12_MUL(scale, y);
        SetActorBgLayerDepth(distance);
        {
            AnimPart *cur = anim;
            s32 t = Q8_TO_INT(cur->animTime);

            if (prev != t) {
                DrawMap((void *)cur->frameOffsets[cur->anims[cur->animIndex].frameIndex + t]);
                bg2PageFlip = 1;
            }
        }
    }
}

/* Flips BG2's page when a new map was written (gHovercraftBg2PageFlip),
 * and sets BG2's affine matrix: a uniform scale from the distance, offset
 * by the BG's centre. UpdateAirshipBg2's twin. */
void Hovercraft::UpdateBg2(void)
{
    s32 scale;
    s32 dy;
    s32 dx;

    if (bg2PageFlip != 0) {
        if (bg2Page == 0)
            REG_BG2CNT = 0x5809;
        else
            REG_BG2CNT = 0x5909;
        bg2PageFlip = 0;
        bg2Page ^= 1;
    }

    scale = __divsi3(distance << 8, 0x3c00);
    dy = screenX + GetActorBgCenterX();
    dx = screenY + GetActorBgCenterY();

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
void Hovercraft::LoadGraphics(void)
{
    s32 i;
    s32 base;
    u32 zero;

    DmaCopy16(3, palette, (void *)(PLTT + 0x20), 0x20);
    base = VRAM + 0xBFC0;
    zero = 0;
    for (i = base + 0x3c; i >= base; i -= 4)
        *(u32 *)i = zero;
    DmaFill16(3, 0xFFFF, (void *)(VRAM + 0xC000), 0x1000);
    ConvertTiles();
    if (state != 0) {
        AnimPart *self;

        bg2PageFlip = 1;
        bg2Page = 0;
        self = anim;
        {
            s32 t = Q8_TO_INT(self->animTime);

            DrawMap((void *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
        }
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
 * In the nibble loop the 0xf mask is an opaque value ANDed with each byte
 * (`m & b`), so gcc copies the mask rather than the byte, as the ROM
 * does, and the second byte gets its own local. ConvertAirshipTiles'
 * comment has why the mask needs the asm (cse1's operand order; #662
 * rounds 2 and 3) and the exact condition the ROM implies (round 4: the
 * mask set where cse1 can't see it but loop.c doesn't move it out of
 * the row loop, which only a guard duplicating the pixel loop's entry
 * test gives; the same guard variants are as far off here: 44 lines in
 * round 6, the loop test then reusing the guard's `n << 4`; round 6's
 * other variants are in ConvertAirshipTiles' comment; round 7's whole-ROM
 * tests of a regmove and a cse1 rule that free both twins, refuted by 7
 * and 4 other functions, are there too). */
static inline u32 MeterPx(u32 v)
{
    u32 r = 0;
    if (v != 0)
        r = 0x10 | v;
    return r;
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
    u32 *rows = mapFrames;
    u32 m;

    stride = (u32)(mapCols * mapRows + 1) >> 1 << 2;
    for (k = 0; k < 1; k++) {
        s32 x = *(s32 *)(((u8 *)palette) + off);
        heights[k] = x;
        sum += x;
        off += 4;
        rows[k] = (u32)((u8 *)palette + off);
        off += stride;
        off += heights[k] << 5;
    }
    mapTileBase = 0xFF - sum;
    dst = (u32 *)(((0xFF - sum) << 6) + (VRAM + 0x8000));
    for (row_i = 0; row_i <= 0; row_i++) {
        u8 *src;
        u8 *row;
        s32 *hp;
        s32 n;
        s32 j;
        u32 *d;

        row = (u8 *)mapFrames[row_i];
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
