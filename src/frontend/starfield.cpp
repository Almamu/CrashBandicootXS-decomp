#include "display.hpp"
#include "frontend.hpp"
#include "key_input.hpp"
#include "frame_limiter.hpp"

extern "C" {
#include "gba/dma_macros.h"
#include "util.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

using Engine::Sys::Display;
using Engine::Sys::FrameLimiter;

/* The starfield (Starfield, #664 part 10c, include/frontend.hpp): a BG0
 * "raw bitmap" behind the language select's, the credits' and the
 * company logos' text. The whole screen is one run of 8x8 tiles on BG0
 * (tile index == screen position, palette bank 15), and a 4bpp shadow
 * buffer (`tileBuffer`, STARFIELD_FRAME_SIZE = 240*160/2 = 0x4B00 bytes
 * on the GBA) is drawn into
 * with nibble writes each frame, then DMA'd into the tile VRAM.
 *
 * Up to 128 stars start at the screen centre with a random direction and
 * speed (SpawnStar); Draw moves them outwards, plotting a short trail.
 *
 * old_agbcp (OLD_AGBCC_OBJS): the C was built with agbcc and needed 16
 * pins and 5 asm blocks; the C++ below matches as written under
 * old_agbcp. */

/* Writes the 4-bit nibble `val` into `tileBuffer` at pixel (x, y), doing
 * nothing off screen. The x test is unsigned (`bhi`), its shift signed
 * (`asrs`). Draw has it inline twice; PlotPixel (unused) out of line. */
inline void Starfield::Plot(screen_ux_t x, screen_y_t y, s32 val)
{
    if (x <= DISPLAY_WIDTH - 1 && y >= 0 && y <= DISPLAY_HEIGHT - 1) {
        s32 addr = ((s32)x >> 3) << 6;
        s32 blockY = y >> 3;
        s32 shift;
        u16 *entry;

        /* A row of tiles, 64 pixels each: gcc strength-reduces it to the
         * ROM's `((blockY << 4) - blockY) << 7` (30 tiles). */
        addr += blockY * DISPLAY_WIDTH_TILES * 64;
        addr += (x & 7);
        addr += (y & 7) << 3;
        entry = &tileBuffer[addr >> 2];
        shift = (addr & 3) << 2;
        *entry = (*entry & ~(0xf << shift)) | (val << shift);
    }
}

/* BG0 at screen block 31, its whole screen one sequential run of tiles in
 * palette bank 15, a 3-step grey gradient in that bank, and the tile VRAM
 * and the shadow buffer cleared. */
Starfield::Starfield()
{
    union bgcnt bg0cnt;
    s32 gradIdx;
    s32 gradCount;
    s32 row;
    s32 tileIdx;
    /* `tileBase` is needed: inlined, the code is 2 instructions shorter */
    u32 tileBase;
    u16 *gradDst;
    u16 dmaFillSrc16;
    u32 dmaFillSrc32;
    /* `dma2Src` is needed: inlined, the instructions are reordered */
    u32 *dma2Src;
    u32 zero;

    particles = new StarParticle[0x80];

    u16 *dispcntShadow = &Display::dispcnt.raw;
    zero = 0;
    *dispcntShadow = DISPCNT_OBJ_1D_MAP;
    Display::SetMode(DISPCNT_MODE_0);

    /* Clears BG0HOFS/BG0VOFS together via one word store. */
    *(vu32 *)REG_ADDR_BG0HOFS = zero;

    bg0cnt.raw = 0;
    bg0cnt.bits.priority = 3;
    bg0cnt.bits.screenBase = 31;
    REG_BG0CNT = bg0cnt.raw;

    tileVramBase = VRAM;
    mapVramBase = BG_SCREEN_ADDR(31);

    Display::ShowBg0();

    /* Clears BG palette entry 0 (the backdrop color). */
    BACKDROP_COLOR = zero;

    /* A 3-step white-to-black grey gradient into BG palette bank 15's
     * first 3 entries (palette index 240). */
    gradDst = BG_PALETTE(15);
    dma2Src = &dmaFillSrc32;
    gradIdx = 0;
    gradCount = 2;
    do {
        s32 half = gradIdx / 2;
        u16 color = RGB16(half, half, half);
        *gradDst = color;
        gradDst++;
        gradIdx += 0x1f;
        gradCount--;
    } while (gradCount >= 0);

    /* Lays out BG0's whole screen (30x20 tiles on the GBA) as one sequential
     * run of tile indices (palette bank 15): tile index N at screen
     * position N, so `tileBuffer` lines up 1:1 with the tile VRAM. */
    tileIdx = 0;
    row = 0;
    tileBase = tileVramBase;
    do {
        /* `nextRow` is needed: inlined, the instructions differ */
        s32 nextRow = row + 1;
        u16 *rowPtr = (u16 *)((row << 6) + mapVramBase);
        s32 col = DISPLAY_WIDTH_TILES - 1;

        do {
            *rowPtr = tileIdx | 0xF000;
            tileIdx++;
            rowPtr++;
            col--;
        } while (col >= 0);
        row = nextRow;
    } while (row <= DISPLAY_HEIGHT_TILES - 1);

    /* Zero-fills the tile VRAM (19200 bytes on the GBA; a fixed-source
     * 16-bit fill from one stack halfword). */
    dmaFillSrc16 = 0;
    DmaFill16From(3, &dmaFillSrc16, tileBase, STARFIELD_FRAME_SIZE);

    REG_BLDCNT = 0;
    Display::CommitDispcnt();

    count = 0;

    tileBuffer = new u16[STARFIELD_FRAME_SIZE / 2];

    /* Zero-fills the new `tileBuffer` (a fixed-source 32-bit fill from
     * one stack word). */
    dmaFillSrc32 = 0;
    DmaFill32From(3, dma2Src, tileBuffer, STARFIELD_FRAME_SIZE);
}

/* Every frame: commits last frame's `tileBuffer` to the tile VRAM (DMA3,
 * 32-bit), clears it (a DMA3 fill), then, for each star, plots a 2-value
 * trail (nibble 1 at the old position, nibble 2 at the new one; the
 * nibble address is PlotPixel's, inlined twice), moving the star in
 * between and respawning it (SpawnStar) once it leaves the 24.8
 * fixed-point screen box (`[0, 0xEFFF]` x `[0, 0x9FFF]` on the GBA). The x
 * bounds tests are unsigned (the ROM's `bhi`). */
void Starfield::Draw()
{
    StarParticle *slot;
    s32 i;
    s32 n;

    DmaCopy32(3, tileBuffer, tileVramBase, STARFIELD_FRAME_SIZE);
    DmaFill32(3, 0, tileBuffer, STARFIELD_FRAME_SIZE);

    slot = particles;
    i = 0;
    n = count;
    if (i < n) {
        do {
            Plot(Q8_TO_INT(slot->x), Q8_TO_INT(slot->y), 1);

            screen_x_q8_t newX;
            screen_y_q8_t newY;

            newX = slot->x + slot->dx;
            slot->x = newX;
            newY = slot->y + slot->dy;
            slot->y = newY;

            if ((u32)newX > INT_TO_Q8(DISPLAY_WIDTH) - 1 || newY < 0 ||
                newY > INT_TO_Q8(DISPLAY_HEIGHT) - 1) {
                SpawnStar(i);
            }
            Plot(Q8_TO_INT(slot->x), Q8_TO_INT(slot->y), 2);
            slot++;
            i++;
            n = count;
        } while (i < n);
    }
}

/* Starts star `idx` at the screen centre with a random direction (an
 * angle into gSineTable) and speed, five steps out. An `idx` past the
 * last star hangs (the ROM's own trap). */
void Starfield::SpawnStar(s32 idx)
{
    StarParticle *slot;
    s32 angle;
    s32 speed;

    if (idx > 0x7f) {
        for (;;) {
        }
    }

    slot = &particles[idx];
    slot->x = INT_TO_Q8(DISPLAY_CENTER_X);
    slot->y = INT_TO_Q8(DISPLAY_CENTER_Y);

    angle = RandRange(0x100);
    speed = RandRange(INT_TO_Q8(2)) + INT_TO_Q8(1); /* 1 to 3 px a frame */

    slot->dx = Q8_MUL(COS_Q8(angle), speed);
    slot->dy = Q8_MUL(SIN_Q8(angle), speed);

    slot->x += slot->dx * 5;
    slot->y += slot->dy * 5;
}

/* Plot, out of line.
 * UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba). */
void Starfield::PlotPixel(screen_ux_t x, screen_y_t y, s32 val)
{
    Plot(x, y, val);
}

/* Draws, then spawns up to 8 more stars while there are fewer than 128. */
void Starfield::Update()
{
    Draw();

    if (count <= 0x7f) {
        s32 n = 0x80 - count;

        LIMIT_MAX(n, 8);
        n -= 1;

        if (n != -1) {
            do {
                SpawnStar(count++);
                n -= 1;
            } while (n != -1);
        }
    }
}

/* Runs the starfield until A or START is pressed. */
void Starfield::WaitForButton()
{
    for (;;) {
        gInput->Update();
        if (gKeys.half.pressed & (A_BUTTON | START_BUTTON)) {
            break;
        }
        FrameLimiter::WaitForVBlank();
        Update();
    }
}

Starfield::~Starfield()
{
    delete[] tileBuffer;
    delete[] particles;
}
