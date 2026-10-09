#include "frontend.hpp"
#include "key_input.hpp"

extern "C" {
#include "gba/dma_macros.h"
#include "util.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* The starfield (Starfield, #664 part 10c, include/frontend.hpp): a BG0
 * "raw bitmap" behind the language select's, the credits' and the
 * company logos' text. The whole 240x160 screen is one run of 8x8 tiles
 * on BG0 (tile index == screen position, palette bank 15), and a 4bpp
 * shadow buffer (`tileBuffer`, 240*160/2 = 0x4B00 bytes) is drawn into
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
inline void Starfield::Plot(u32 x, s32 y, s32 val)
{
    if (x <= 0xef && y >= 0 && y <= 0x9f) {
        s32 addr = ((s32)x >> 3) << 6;
        s32 blockY = y >> 3;
        s32 shift;
        u16 *entry;

        addr += ((blockY << 4) - blockY) << 7;
        addr += (x & 7);
        addr += (y & 7) << 3;
        entry = (u16 *)(tileBuffer + ((addr >> 2) << 1));
        shift = (addr & 3) << 2;
        *entry = (*entry & ~(0xf << shift)) | (val << shift);
    }
}

/* BG0 at screen block 31, its whole screen one sequential run of tiles in
 * palette bank 15, a 3-step grey gradient in that bank, and the tile VRAM
 * and the shadow buffer cleared. */
Starfield::Starfield()
{
    struct dma_regs *dma;
    union bgcnt bg0cnt;
    s32 gradIdx;
    s32 gradCount;
    s32 row;
    s32 tileIdx;
    u32 tileBase;
    u32 mapBase;
    u16 *gradDst;
    u16 dmaFillSrc16;
    u32 dmaFillSrc32;
    u32 *dma2Src;
    u32 zero;

    particles = new StarParticle[0x80];

    {
        u16 *dispcntShadow = (u16 *)gDispcnt;
        zero = 0;
        *dispcntShadow = DISPCNT_OBJ_1D_MAP;
    }
    SetDispcntMode(0);

    /* Clears BG0HOFS/BG0VOFS together via one word store. */
    *(vu32 *)REG_ADDR_BG0HOFS = zero;

    bg0cnt.raw = 0;
    bg0cnt.bits.priority = 3;
    bg0cnt.bits.screenBase = 31;
    REG_BG0CNT = bg0cnt.raw;

    tileVramBase = VRAM;
    mapVramBase = BG_SCREEN_ADDR(31);

    ShowBg0();

    /* Clears BG palette entry 0 (the backdrop color). */
    *(vu16 *)BG_PLTT = zero;

    /* A 3-step white-to-black grey gradient into BG palette bank 15's
     * first 3 entries (palette index 240). */
    gradDst = (u16 *)(BG_PLTT + 240 * sizeof(u16));
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

    /* Lays out BG0's whole 240x160 (30x20 tiles) screen as one sequential
     * run of tile indices (palette bank 15): tile index N at screen
     * position N, so `tileBuffer` lines up 1:1 with the tile VRAM. */
    tileIdx = 0;
    row = 0;
    tileBase = tileVramBase;
    mapBase = mapVramBase;
    do {
        s32 nextRow = row + 1;
        u16 *rowPtr = (u16 *)((row << 6) + mapBase);
        s32 col = 0x1d;

        do {
            *rowPtr = tileIdx | 0xF000;
            tileIdx++;
            rowPtr++;
            col--;
        } while (col >= 0);
        row = nextRow;
    } while (row <= 0x13);

    /* Zero-fills the 19200-byte tile VRAM (a fixed-source 16-bit fill
     * from one stack halfword). */
    dmaFillSrc16 = 0;
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)&dmaFillSrc16;
    dma->dst = tileBase;
    dma->cnt = 0x81002580;
    dma->cnt;

    REG_BLDCNT = 0;
    CommitDispcnt();

    count = 0;

    tileBuffer = new u8[0x4B00];

    /* Zero-fills the new `tileBuffer` (a fixed-source 32-bit fill from
     * one stack word). */
    dmaFillSrc32 = 0;
    dma->src = (u32)dma2Src;
    dma->dst = (u32)tileBuffer;
    dma->cnt = 0x850012C0;
    dma->cnt;
}

/* Every frame: commits last frame's `tileBuffer` to the tile VRAM (DMA3,
 * 32-bit), clears it (a DMA3 fill), then, for each star, plots a 2-value
 * trail (nibble 1 at the old position, nibble 2 at the new one; the
 * nibble address is PlotPixel's, inlined twice), moving the star in
 * between and respawning it (SpawnStar) once it leaves the 24.8
 * fixed-point `[0, 0xEFFF]` x `[0, 0x9FFF]` (240x160 pixel) box. The x
 * bounds tests are unsigned (the ROM's `bhi`). */
void Starfield::Draw()
{
    struct dma_regs *dma;
    StarParticle *slot;
    s32 i;
    s32 n;
    u32 fillZero;

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)tileBuffer;
    dma->dst = tileVramBase;
    dma->cnt = 0x840012C0;
    dma->cnt;

    fillZero = 0;
    dma->src = (u32)&fillZero;
    dma->dst = (u32)tileBuffer;
    dma->cnt = 0x850012C0;
    dma->cnt;

    slot = particles;
    i = 0;
    n = count;
    if (i < n) {
        do {
            Plot(Q8_TO_INT(slot->x), Q8_TO_INT(slot->y), 1);
            {
                s32 newX, newY;

                newX = slot->x + slot->dx;
                slot->x = newX;
                newY = slot->y + slot->dy;
                slot->y = newY;

                if ((u32)newX > 0xEFFF || newY < 0 || newY > 0x9FFF) {
                    SpawnStar(i);
                }
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
    slot->x = 0x7800;
    slot->y = 0x5000;

    angle = (u16)RandRange(0x100);
    speed = (u16)RandRange(0x200) + 0x100;

    slot->dx = Q8_MUL(COS_Q8(angle), speed);
    slot->dy = Q8_MUL(SIN_Q8(angle), speed);

    slot->x += slot->dx * 5;
    slot->y += slot->dy * 5;
}

/* Plot, out of line.
 * UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba). */
void Starfield::PlotPixel(u32 x, s32 y, s32 val)
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
            s32 end = -1;

            do {
                SpawnStar(count++);
                n -= 1;
            } while (n != end);
        }
    }
}

/* Runs the starfield until A or START is pressed. */
void Starfield::WaitForButton()
{
    for (;;) {
        gInput->Update();
        if (gKeys.half.pressed & 9) {
            break;
        }
        WaitForVBlank();
        Update();
    }
}

Starfield::~Starfield()
{
    delete[] tileBuffer;
    delete[] particles;
}
