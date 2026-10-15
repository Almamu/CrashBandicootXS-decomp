/* The pc platform's PPU: the GBA's picture, drawn a scanline at a time
 * from the display registers, VRAM, palette RAM and OAM the game writes
 * (gHostIo, gHostVram, gHostPltt, gHostOam). irq.c's frame loop calls
 * HostPpuLine for each visible line as the display reaches it and
 * HostPpuFrame at line 160, so a register the game changes in the middle
 * of a frame (an IRQ handler) changes the lines after it, as on the GBA.
 *
 * What it draws (docs/platforms.md has the survey of what the game uses):
 * - modes 0 to 5: text BGs (4 and 8 bpp, flips, the four screen sizes,
 *   scrolling), affine BGs (BG2PA-PD, BG2X/Y and BG3's, wrap-around or
 *   transparent outside, the reference point latched at the frame's start,
 *   stepped by PB/PD each line and reloaded when the game rewrites it), the
 *   bitmap modes 3, 4 and 5 with their frame select;
 * - OBJs: regular and affine, double size, 1D and 2D mapping, 4 and 8 bpp,
 *   flips, priorities, semi-transparent ones, OBJ-window ones, the
 *   per-line sprite budget (1210 cycles, 954 with H-blank interval free)
 *   and tile numbers below 512 hidden in the bitmap modes;
 * - windows 0 and 1, the OBJ window and the outside, mosaic (BGs and
 *   OBJs), BLDCNT's alpha blending, brightening and darkening, forced
 *   blank (white) and the backdrop.
 *
 * Colours are 8-bit RGB: each 5-bit channel widened as c << 3 | c >> 2,
 * and the colour effects computed on those 8-bit channels with the GBA's
 * formulas (I1*EVA/16 + I2*EVB/16 capped at 255, I + (255-I)*EVY/16,
 * I - I*EVY/16). That is what mGBA does, so its screenshots and these
 * compare pixel for pixel (tools/boottest.py `screens`); the GBA does the
 * same on the 5-bit channels, which can round a blended pixel apart. Also
 * mGBA's: darkening rounds green's and blue's step up and red's down
 * (mGBA works on the three channels packed in one word, and only red's
 * fraction is dropped before the subtraction): a darkened grey comes out
 * a step short on green and blue. Without that, 14 frames of a fade in
 * the boot test differ from mGBA's by that one step.
 * Where the hardware's behaviour is subtle, it does what mGBA 0.10 does (a
 * semi-transparent OBJ with no second target under it takes BLDCNT's
 * brightness effect; a transparent pixel of a higher-priority OBJ passes
 * its priority on to the OBJ pixel under it; the sprite budget). Written
 * from GBATEK's description of the hardware; no emulator's code is in it.
 *
 * Not modelled: the GBA's 3-line delay when a BG is switched on in the
 * middle of a frame (the game switches them in VBlank), and the
 * OAM/VRAM access timing. */

#include <stdio.h>
#include <string.h>

#include "gba/gba.h"
#include "host_platform.h"

#define WIDTH  HOST_SCREEN_WIDTH
#define HEIGHT HOST_SCREEN_HEIGHT

/* DISPCNT's bits the game's io_reg.h doesn't name. */
#define DISPCNT_FRAME_SELECT 0x0010 /* modes 4 and 5: the second frame */
#define DISPCNT_HBLANK_FREE  0x0020 /* OBJs aren't drawn in H-blank */

/* The picture the display finished last, and the one being drawn. */
u32 gHostScreen[HOST_SCREEN_WIDTH * HOST_SCREEN_HEIGHT];
static u32 sDraw[HOST_SCREEN_WIDTH * HOST_SCREEN_HEIGHT];

/* The affine BGs' (BG2, BG3) internal reference points, and the register
 * values they were last loaded from. */
static s32 sRefX[2];
static s32 sRefY[2];
static u32 sRegX[2];
static u32 sRegY[2];

/* What the frame used, for --ppu-stats (main.c): HOST_PPU_* bits per
 * line, counted. */
static u32 sStats[HOST_PPU_STAT_COUNT];
static u32 sStatLines;

/* The layers. */
enum {
    LAYER_BG0,
    LAYER_BG1,
    LAYER_BG2,
    LAYER_BG3,
    LAYER_OBJ,
    LAYER_BD
};

/* A BG line: the 15-bit colour of each pixel, or TRANSPARENT. */
#define TRANSPARENT 0x8000

/* The OBJ line: per pixel the colour (or TRANSPARENT), the priority, and
 * the flags below; and the OBJ window. */
#define OBJ_SEMI      1
#define OBJ_UNWRITTEN 0x80

struct ObjPixel {
    u16 color;
    u8 priority;
    u8 flags;
};

static u16 sBgLine[4][WIDTH];
static struct ObjPixel sObjLine[WIDTH];
static u8 sObjWindow[WIDTH];

/* The OBJ sizes by shape and size; shape 3 is prohibited (nothing). */
static const u8 sObjSize[4][4][2] = {
    { { 8, 8 }, { 16, 16 }, { 32, 32 }, { 64, 64 } },
    { { 16, 8 }, { 32, 8 }, { 32, 16 }, { 64, 32 } },
    { { 8, 16 }, { 8, 32 }, { 16, 32 }, { 32, 64 } },
    { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
};

static inline u16 Io16(u32 offset)
{
    return *(const u16 *)(gHostIo + offset);
}

static inline u32 Io32(u32 offset)
{
    return *(const u32 *)(gHostIo + offset);
}

static inline u16 Vram16(u32 addr)
{
    return *(const u16 *)(gHostVram + addr);
}

static inline u16 Pltt(u32 index)
{
    return *(const u16 *)(gHostPltt + 2 * index) & 0x7FFF;
}

/* A 15-bit BGR colour as 8-bit XRGB. */
static inline u32 Rgb8(u32 c)
{
    u32 r = c & 0x1F;
    u32 g = (c >> 5) & 0x1F;
    u32 b = (c >> 10) & 0x1F;

    r = r << 3 | r >> 2;
    g = g << 3 | g >> 2;
    b = b << 3 | b >> 2;
    return r << 16 | g << 8 | b;
}

static inline u32 Blend(u32 a, u32 eva, u32 b, u32 evb)
{
    u32 out = 0;
    s32 shift;

    for (shift = 0; shift <= 16; shift += 8) {
        u32 c = (((a >> shift) & 0xFF) * eva + ((b >> shift) & 0xFF) * evb) / 16;

        out |= (c > 0xFF ? 0xFF : c) << shift;
    }
    return out;
}

static inline u32 Brighten(u32 a, u32 evy)
{
    u32 out = 0;
    s32 shift;

    for (shift = 0; shift <= 16; shift += 8) {
        u32 c = (a >> shift) & 0xFF;

        out |= (c + (0xFF - c) * evy / 16) << shift;
    }
    return out;
}

/* Red's darkening rounds down, green's and blue's up: mGBA's, whose
 * arithmetic keeps those two channels' fractions (see the top). */
static inline u32 Darken(u32 a, u32 evy)
{
    u32 r = (a >> 16) & 0xFF;
    u32 g = (a >> 8) & 0xFF;
    u32 b = a & 0xFF;

    r -= r * evy / 16;
    g -= (g * evy + 15) / 16;
    b -= (b * evy + 15) / 16;
    return r << 16 | g << 8 | b;
}

/* A 28-bit signed reference point register. */
static inline s32 RefPoint(u32 reg)
{
    return (s32)(reg << 4) >> 4;
}

/* The display starts a frame: the affine reference points come from
 * BG2X/Y and BG3X/Y (the GBA reloads them at VBlank and on each write;
 * the game writes them in VBlank, before this). */
static void StartFrame(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        sRegX[i] = Io32(REG_OFFSET_BG2X + 0x10 * i);
        sRegY[i] = Io32(REG_OFFSET_BG2Y + 0x10 * i);
        sRefX[i] = RefPoint(sRegX[i]);
        sRefY[i] = RefPoint(sRegY[i]);
    }
}

static void TextBg(s32 bg, u32 y, u32 mosaic)
{
    u16 cnt = Io16(REG_OFFSET_BG0CNT + 2 * bg);
    u32 charBase = ((cnt >> 2) & 3) * 0x4000;
    u32 screenBase = ((cnt >> 8) & 0x1F) * 0x800;
    u32 size = cnt >> 14;
    s32 is8bpp = (cnt >> 7) & 1;
    u32 hofs = Io16(REG_OFFSET_BG0HOFS + 4 * bg) & 0x1FF;
    u32 vofs = Io16(REG_OFFSET_BG0VOFS + 4 * bg) & 0x1FF;
    u32 widthMask = (size & 1) ? 0x1FF : 0xFF;
    u32 heightMask = (size & 2) ? 0x1FF : 0xFF;
    u32 mosaicH = 1;
    u32 ty;
    u32 x;
    u16 *out = sBgLine[bg];

    if (mosaic && (cnt & BGCNT_MOSAIC)) {
        u32 mosaicV = ((Io16(REG_OFFSET_MOSAIC) >> 4) & 0xF) + 1;

        mosaicH = (Io16(REG_OFFSET_MOSAIC) & 0xF) + 1;
        y -= y % mosaicV;
    }
    ty = (y + vofs) & heightMask;
    for (x = 0; x < WIDTH; x++) {
        u32 sx = x - x % mosaicH;
        u32 tx = (sx + hofs) & widthMask;
        u32 block = 0;
        u32 entry;
        u32 tile;
        u32 px;
        u32 py;
        u32 addr;
        u32 index;

        if (tx >= 256) {
            block = 1;
        }
        if (ty >= 256) {
            block += size == 3 ? 2 : 1;
        }
        entry =
            Vram16((screenBase + block * 0x800 + ((ty & 0xFF) >> 3) * 64 + ((tx & 0xFF) >> 3) * 2) &
                   0xFFFF);
        tile = entry & 0x3FF;
        px = tx & 7;
        py = ty & 7;
        if (entry & 0x400) {
            px = 7 - px;
        }
        if (entry & 0x800) {
            py = 7 - py;
        }
        if (is8bpp) {
            addr = charBase + tile * 64 + py * 8 + px;
            index = addr < 0x10000 ? gHostVram[addr] : 0;
            out[x] = index ? Pltt(index) : TRANSPARENT;
        } else {
            addr = charBase + tile * 32 + py * 4 + px / 2;
            index = addr < 0x10000 ? (gHostVram[addr] >> ((px & 1) * 4)) & 0xF : 0;
            out[x] = index ? Pltt((entry >> 12) * 16 + index) : TRANSPARENT;
        }
    }
}

/* An affine BG (mode 1's BG2, mode 2's BG2 and BG3) or a bitmap BG (modes
 * 3-5, BG2): the texture coordinates step by PA/PC along the line from the
 * reference point. */
static void AffineBg(s32 bg, u32 y, u32 mode, u32 dispcnt, u32 mosaic)
{
    s32 i = bg - 2;
    u16 cnt = Io16(REG_OFFSET_BG0CNT + 2 * bg);
    s32 pa = (s16)Io16(REG_OFFSET_BG2PA + 0x10 * i);
    s32 pc = (s16)Io16(REG_OFFSET_BG2PC + 0x10 * i);
    s32 px = sRefX[i];
    s32 py = sRefY[i];
    u32 mosaicH = 1;
    u32 x;
    u16 *out = sBgLine[bg];

    if (mosaic && (cnt & BGCNT_MOSAIC)) {
        u32 mosaicV = ((Io16(REG_OFFSET_MOSAIC) >> 4) & 0xF) + 1;
        s32 back = y % mosaicV;

        mosaicH = (Io16(REG_OFFSET_MOSAIC) & 0xF) + 1;
        /* The reference point of the mosaic block's first line. */
        px -= back * (s16)Io16(REG_OFFSET_BG2PB + 0x10 * i);
        py -= back * (s16)Io16(REG_OFFSET_BG2PD + 0x10 * i);
    }
    for (x = 0; x < WIDTH; x++) {
        u32 sx = x - x % mosaicH;
        s32 tx = (px + (s32)sx * pa) >> 8;
        s32 ty = (py + (s32)sx * pc) >> 8;
        u16 color = TRANSPARENT;

        if (mode <= 2) {
            u32 size = 128 << (cnt >> 14);
            u32 charBase = ((cnt >> 2) & 3) * 0x4000;
            u32 screenBase = ((cnt >> 8) & 0x1F) * 0x800;
            u32 tile;
            u32 addr;
            u32 index;

            if (cnt & BGCNT_WRAP) {
                tx &= size - 1;
                ty &= size - 1;
            } else if (tx < 0 || ty < 0 || tx >= (s32)size || ty >= (s32)size) {
                out[x] = TRANSPARENT;
                continue;
            }
            tile = gHostVram[(screenBase + (ty >> 3) * (size >> 3) + (tx >> 3)) & 0xFFFF];
            addr = charBase + tile * 64 + (ty & 7) * 8 + (tx & 7);
            index = addr < 0x10000 ? gHostVram[addr] : 0;
            if (index) {
                color = Pltt(index);
            }
        } else if (mode == 3) {
            if (tx >= 0 && ty >= 0 && tx < WIDTH && ty < HEIGHT) {
                color = Vram16((ty * WIDTH + tx) * 2) & 0x7FFF;
            }
        } else if (mode == 4) {
            u32 base = (dispcnt & DISPCNT_FRAME_SELECT) ? 0xA000 : 0;

            if (tx >= 0 && ty >= 0 && tx < WIDTH && ty < HEIGHT) {
                u32 index = gHostVram[base + ty * WIDTH + tx];

                if (index) {
                    color = Pltt(index);
                }
            }
        } else if (mode == 5) {
            u32 base = (dispcnt & DISPCNT_FRAME_SELECT) ? 0xA000 : 0;

            if (tx >= 0 && ty >= 0 && tx < 160 && ty < 128) {
                color = Vram16(base + (ty * 160 + tx) * 2) & 0x7FFF;
            }
        }
        out[x] = color;
    }
}

/* One OBJ's pixel at texture (lx, ly). The OBJ tiles start at VRAM
 * 0x10000; in 1D mapping an OBJ's tiles follow each other, in 2D each
 * tile row is 32 tiles apart (and an 8-bpp OBJ's tile number is even). */
static u32 ObjTexel(u32 attr0, u32 attr2, u32 width, u32 lx, u32 ly, u32 map1D)
{
    u32 tile = attr2 & 0x3FF;
    u32 addr;

    if (attr0 & (1 << 13)) {
        if (map1D) {
            addr = tile * 32 + (ly >> 3) * width * 8 + (lx >> 3) * 64 + (ly & 7) * 8 + (lx & 7);
        } else {
            tile &= ~1u;
            addr = ((tile * 32) & 0x7C00) + (ly >> 3) * 0x400 + (ly & 7) * 8 +
                   (((tile * 32) + (lx >> 3) * 64 + (lx & 7)) & 0x3FF);
        }
        return gHostVram[0x10000 + (addr & 0x7FFF)];
    }
    if (map1D) {
        addr = tile * 32 + (ly >> 3) * width * 4 + (lx >> 3) * 32 + (ly & 7) * 4 + (lx & 7) / 2;
    } else {
        addr = ((tile * 32) & 0x7C00) + (ly >> 3) * 0x400 + (ly & 7) * 4 +
               (((tile * 32) + (lx >> 3) * 32 + (lx & 7) / 2) & 0x3FF);
    }
    return (gHostVram[0x10000 + (addr & 0x7FFF)] >> ((lx & 1) * 4)) & 0xF;
}

static void PutObjPixel(u32 x, u32 index, u32 attr0, u32 attr2, u32 priority, u32 flags)
{
    struct ObjPixel *p = &sObjLine[x];

    if (((attr0 >> 10) & 3) == 2) {
        if (index) {
            sObjWindow[x] = 1;
        }
        return;
    }
    if (priority >= p->priority) {
        return;
    }
    if (index) {
        p->color =
            (attr0 & (1 << 13)) ? Pltt(0x100 + index) : Pltt(0x100 + (attr2 >> 12) * 16 + index);
        p->priority = priority;
        p->flags = flags;
    } else if (!(p->flags & OBJ_UNWRITTEN)) {
        /* A transparent pixel still passes its priority (and its blending)
         * on to the OBJ pixel under it. */
        p->priority = priority;
        p->flags = flags;
    }
}

static void ObjLine(u32 y, u32 dispcnt)
{
    u16 mosaicReg = Io16(REG_OFFSET_MOSAIC);
    u32 mosaicH = ((mosaicReg >> 8) & 0xF) + 1;
    u32 mosaicV = ((mosaicReg >> 12) & 0xF) + 1;
    u32 map1D = (dispcnt & DISPCNT_OBJ_1D_MAP) != 0;
    u32 mode = dispcnt & 7;
    s32 cycles = (dispcnt & DISPCNT_HBLANK_FREE) ? 954 : 1210;
    const u16 *oam = (const u16 *)gHostOam;
    s32 i;

    for (i = 0; i < 128; i++) {
        u32 attr0 = oam[4 * i];
        u32 attr1 = oam[4 * i + 1];
        u32 attr2 = oam[4 * i + 2];
        u32 affine = (attr0 >> 8) & 1;
        u32 shape = attr0 >> 14;
        u32 sizeIndex = attr1 >> 14;
        u32 width = sObjSize[shape][sizeIndex][0];
        u32 height = sObjSize[shape][sizeIndex][1];
        u32 boxW = width;
        u32 boxH = height;
        u32 objY = attr0 & 0xFF;
        s32 objX = (s32)((attr1 & 0x1FF) << 23) >> 23;
        u32 priority = (attr2 >> 10) & 3;
        u32 objMode = (attr0 >> 10) & 3;
        u32 flags = objMode == 1 ? OBJ_SEMI : 0;
        u32 mosaic = (attr0 >> 12) & 1;
        u32 inY;
        s32 x;

        if (!affine && (attr0 & (1 << 9))) {
            continue; /* hidden */
        }
        if (affine) {
            boxW <<= (attr0 >> 9) & 1;
            boxH <<= (attr0 >> 9) & 1;
        }
        /* Off the screen altogether: no time spent on it. */
        if (objY >= HEIGHT && objY + boxH < 228) {
            continue;
        }
        if ((u32)(attr1 & 0x1FF) >= WIDTH && (attr1 & 0x1FF) + boxW < 512) {
            continue;
        }
        inY = (y - objY) & 0xFF;
        if (inY >= boxH) {
            continue;
        }
        if (mosaic && mosaicV > 1) {
            /* The mosaic block's first line, within the OBJ. */
            u32 my = y - y % mosaicV;
            u32 top = (u32)(my - objY) & 0xFF;

            if (top < boxH) {
                inY = top;
            } else if (objY < HEIGHT) {
                inY = 0;
            }
        }
        if (mode >= 3 && (attr2 & 0x3FF) < 512) {
            goto spent;
        }
        if (affine) {
            u32 m = ((attr1 >> 9) & 0x1F) * 16;
            s32 pa = (s16)oam[m + 3];
            s32 pb = (s16)oam[m + 7];
            s32 pc = (s16)oam[m + 11];
            s32 pd = (s16)oam[m + 15];
            s32 dy = (s32)inY - (s32)(boxH / 2);
            s32 lx = 0;
            s32 ly = 0;

            for (x = 0; x < (s32)boxW; x++) {
                s32 sx = objX + x;
                s32 dx = x - (s32)(boxW / 2);

                /* With mosaic, a texel is held for the mosaic block. */
                if (x == 0 || !mosaic || mosaicH == 1 || (sx >= 0 && (u32)sx % mosaicH == 0)) {
                    lx = ((pa * dx + pb * dy) >> 8) + (s32)(width / 2);
                    ly = ((pc * dx + pd * dy) >> 8) + (s32)(height / 2);
                }
                if (sx < 0 || sx >= WIDTH) {
                    continue;
                }
                if (lx < 0 || ly < 0 || lx >= (s32)width || ly >= (s32)height) {
                    continue;
                }
                PutObjPixel(sx, ObjTexel(attr0, attr2, width, lx, ly, map1D), attr0, attr2,
                            priority, flags);
            }
        } else {
            u32 ly = (attr1 & (1 << 13)) ? height - 1 - inY : inY;

            for (x = 0; x < (s32)width; x++) {
                s32 sx = objX + x;
                s32 lx;

                if (sx < 0 || sx >= WIDTH) {
                    continue;
                }
                lx = x;
                if (mosaic && mosaicH > 1) {
                    lx -= sx % mosaicH;
                    if (lx < 0) {
                        lx = 0;
                    }
                }
                if (attr1 & (1 << 12)) {
                    lx = width - 1 - lx;
                }
                PutObjPixel(sx, ObjTexel(attr0, attr2, width, lx, ly, map1D), attr0, attr2,
                            priority, flags);
            }
        }
    spent:
        cycles -= affine ? 10 + 2 * boxW : boxW;
        if (cycles <= 0) {
            break;
        }
    }
}

/* Is x inside a window's horizontal span (WINxH: X1 in the high byte,
 * X2 in the low; X1 > X2 wraps around). */
static inline s32 InSpan(u32 v, u32 reg, u32 limit)
{
    u32 a = reg >> 8;
    u32 b = reg & 0xFF;

    if (b > limit || (a > b && b == 0)) {
        b = limit;
    }
    if (a <= b) {
        return v >= a && v < b;
    }
    return v >= a || v < b;
}

void HostPpuLine(u32 y)
{
    u32 dispcnt = Io16(REG_OFFSET_DISPCNT);
    u32 mode = dispcnt & 7;
    u32 bldcnt = Io16(REG_OFFSET_BLDCNT);
    u32 bldalpha = Io16(REG_OFFSET_BLDALPHA);
    u32 effect = (bldcnt >> 6) & 3;
    u32 eva = bldalpha & 0x1F;
    u32 evb = (bldalpha >> 8) & 0x1F;
    u32 evy = Io16(REG_OFFSET_BLDY) & 0x1F;
    u32 windows = dispcnt & (DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON);
    u32 bgOn = 0;
    u32 bgPrio[4];
    u32 target2Any;
    u32 *row = &sDraw[y * WIDTH];
    u32 backdrop;
    u32 x;
    s32 i;

    if (y == 0) {
        StartFrame();
    }
    /* A reference point the game rewrote since the last line reloads. */
    for (i = 0; i < 2; i++) {
        u32 rx = Io32(REG_OFFSET_BG2X + 0x10 * i);
        u32 ry = Io32(REG_OFFSET_BG2Y + 0x10 * i);

        if (rx != sRegX[i]) {
            sRegX[i] = rx;
            sRefX[i] = RefPoint(rx);
        }
        if (ry != sRegY[i]) {
            sRegY[i] = ry;
            sRefY[i] = RefPoint(ry);
        }
    }
    if (eva > 16) {
        eva = 16;
    }
    if (evb > 16) {
        evb = 16;
    }
    if (evy > 16) {
        evy = 16;
    }

    if (dispcnt & DISPCNT_FORCED_BLANK) {
        for (x = 0; x < WIDTH; x++) {
            row[x] = 0xFFFFFF;
        }
        sStats[HOST_PPU_FORCED_BLANK]++;
        goto next;
    }

    /* The BGs this mode has. */
    switch (mode) {
    case 0:
        bgOn = 0xF;
        break;
    case 1:
        bgOn = 0x7;
        break;
    case 2:
        bgOn = 0xC;
        break;
    default:
        bgOn = 0x4;
        break;
    }
    bgOn &= dispcnt >> 8;
    for (i = 0; i < 4; i++) {
        bgPrio[i] = Io16(REG_OFFSET_BG0CNT + 2 * i) & 3;
        if (!(bgOn & (1 << i))) {
            continue;
        }
        if (mode == 0 || (mode == 1 && i < 2)) {
            TextBg(i, y, 1);
        } else {
            AffineBg(i, y, mode, dispcnt, 1);
        }
    }

    for (x = 0; x < WIDTH; x++) {
        sObjLine[x].color = TRANSPARENT;
        sObjLine[x].priority = 4;
        sObjLine[x].flags = OBJ_UNWRITTEN;
        sObjWindow[x] = 0;
    }
    if (dispcnt & DISPCNT_OBJ_ON) {
        ObjLine(y, dispcnt);
    }

    /* Some layer is a second target: a semi-transparent OBJ with none
     * under it takes the brightness effect instead (mGBA's REBLEND). */
    target2Any = (bldcnt >> 13) & 1;
    for (i = 0; i < 4; i++) {
        if ((bldcnt & (0x100 << i)) && (dispcnt & (DISPCNT_BG0_ON << i))) {
            target2Any = 1;
        }
    }

    backdrop = Rgb8(Pltt(0));
    for (x = 0; x < WIDTH; x++) {
        u32 control = 0x3F;
        u32 layers[2] = { LAYER_BD, LAYER_BD };
        u32 colors[2];
        u32 found = 0;
        u32 p;
        u32 top;
        u32 out;
        s32 semi = 0;

        if (windows) {
            u32 winin = Io16(REG_OFFSET_WININ);
            u32 winout = Io16(REG_OFFSET_WINOUT);

            if ((dispcnt & DISPCNT_WIN0_ON) && InSpan(x, Io16(REG_OFFSET_WIN0H), WIDTH) &&
                InSpan(y, Io16(REG_OFFSET_WIN0V), HEIGHT)) {
                control = winin & 0x3F;
            } else if ((dispcnt & DISPCNT_WIN1_ON) && InSpan(x, Io16(REG_OFFSET_WIN1H), WIDTH) &&
                       InSpan(y, Io16(REG_OFFSET_WIN1V), HEIGHT)) {
                control = (winin >> 8) & 0x3F;
            } else if ((dispcnt & DISPCNT_OBJWIN_ON) && sObjWindow[x]) {
                control = (winout >> 8) & 0x3F;
            } else {
                control = winout & 0x3F;
            }
        }

        /* The top two layers: by priority, an OBJ before the BGs of its
         * priority, then BG0 to BG3. */
        for (p = 0; p < 4 && found < 2; p++) {
            if ((control & 0x10) && sObjLine[x].color != TRANSPARENT && sObjLine[x].priority == p) {
                colors[found] = sObjLine[x].color;
                if (found == 0) {
                    semi = sObjLine[x].flags & OBJ_SEMI;
                }
                layers[found++] = LAYER_OBJ;
            }
            for (i = 0; i < 4 && found < 2; i++) {
                if ((bgOn & control & (1 << i)) && bgPrio[i] == p && sBgLine[i][x] != TRANSPARENT) {
                    colors[found] = sBgLine[i][x];
                    layers[found++] = i;
                }
            }
        }
        top = layers[0];
        out = top == LAYER_BD ? backdrop : Rgb8(colors[0]);

        if (semi && (bldcnt & (0x100 << layers[1]))) {
            /* A semi-transparent OBJ blends with the second target under it,
             * whatever BLDCNT's effect and the window say. */
            u32 under = layers[1] == LAYER_BD ? backdrop : Rgb8(colors[1]);

            out = Blend(out, eva, under, evb);
        } else if (!(control & 0x20)) {
            /* the window turns the effects off */
        } else if (semi && target2Any) {
            if (effect == 2) {
                out = Brighten(out, evy);
            } else if (effect == 3) {
                out = Darken(out, evy);
            }
        } else if (bldcnt & (1 << top)) {
            if (effect == 1 && top != LAYER_BD && (bldcnt & (0x100 << layers[1]))) {
                u32 under = layers[1] == LAYER_BD ? backdrop : Rgb8(colors[1]);

                out = Blend(out, eva, under, evb);
            } else if (effect == 2) {
                out = Brighten(out, evy);
            } else if (effect == 3) {
                out = Darken(out, evy);
            }
        }
        row[x] = out;
    }

    if (gHostOptions.ppuStats) {
        u32 bits = 1u << mode;

        for (i = 0; i < 4; i++) {
            u16 cnt = Io16(REG_OFFSET_BG0CNT + 2 * i);

            if (!(bgOn & (1 << i))) {
                continue;
            }
            if (mode == 0 || (mode == 1 && i < 2)) {
                bits |= (cnt & BGCNT_256COLOR) ? HOST_PPU_BIT(HOST_PPU_TEXT_8BPP)
                                               : HOST_PPU_BIT(HOST_PPU_TEXT_4BPP);
                bits |= HOST_PPU_BIT(HOST_PPU_TEXT_SIZE0 + (cnt >> 14));
            } else if (mode <= 2) {
                bits |=
                    HOST_PPU_BIT((cnt & BGCNT_WRAP) ? HOST_PPU_AFFINE_WRAP : HOST_PPU_AFFINE_CLIP);
            }
            if (cnt & BGCNT_MOSAIC) {
                bits |= HOST_PPU_BIT(HOST_PPU_BG_MOSAIC);
            }
        }
        if (dispcnt & DISPCNT_WIN0_ON) {
            bits |= HOST_PPU_BIT(HOST_PPU_WIN0);
        }
        if (dispcnt & DISPCNT_WIN1_ON) {
            bits |= HOST_PPU_BIT(HOST_PPU_WIN1);
        }
        if (dispcnt & DISPCNT_OBJWIN_ON) {
            bits |= HOST_PPU_BIT(HOST_PPU_OBJWIN);
        }
        if (effect != 0) {
            bits |= HOST_PPU_BIT(HOST_PPU_BLEND_ALPHA + effect - 1);
        }
        if (dispcnt & DISPCNT_OBJ_ON) {
            const u16 *oam = (const u16 *)gHostOam;

            bits |=
                HOST_PPU_BIT((dispcnt & DISPCNT_OBJ_1D_MAP) ? HOST_PPU_OBJ_1D : HOST_PPU_OBJ_2D);
            for (i = 0; i < 128; i++) {
                u32 attr0 = oam[4 * i];
                u32 h = sObjSize[attr0 >> 14][oam[4 * i + 1] >> 14][1];

                if (!(attr0 & 0x100) && (attr0 & 0x200)) {
                    continue;
                }
                if ((attr0 & 0x300) == 0x300) {
                    h *= 2;
                }
                if (((y - (attr0 & 0xFF)) & 0xFF) >= h) {
                    continue;
                }
                bits |= HOST_PPU_BIT(
                    (attr0 & 0x100) ? ((attr0 & 0x200) ? HOST_PPU_OBJ_DOUBLE : HOST_PPU_OBJ_AFFINE)
                                    : HOST_PPU_OBJ_REGULAR);
                if (((attr0 >> 10) & 3) == 1) {
                    bits |= HOST_PPU_BIT(HOST_PPU_OBJ_SEMI);
                }
                if (((attr0 >> 10) & 3) == 2) {
                    bits |= HOST_PPU_BIT(HOST_PPU_OBJ_WINDOW);
                }
                if (attr0 & 0x1000) {
                    bits |= HOST_PPU_BIT(HOST_PPU_OBJ_MOSAIC);
                }
                if (attr0 & 0x2000) {
                    bits |= HOST_PPU_BIT(HOST_PPU_OBJ_8BPP);
                }
            }
        }
        for (i = 0; i < HOST_PPU_STAT_COUNT; i++) {
            if (bits & HOST_PPU_BIT(i)) {
                sStats[i]++;
            }
        }
    }

next:
    sStatLines++;
    /* The affine BGs' reference points step to the next line. */
    for (i = 0; i < 2; i++) {
        sRefX[i] += (s16)Io16(REG_OFFSET_BG2PB + 0x10 * i);
        sRefY[i] += (s16)Io16(REG_OFFSET_BG2PD + 0x10 * i);
    }
}

void HostPpuFrame(void)
{
    memcpy(gHostScreen, sDraw, sizeof(gHostScreen));
}

static const char *const sStatNames[HOST_PPU_STAT_COUNT] = {
    "mode 0",
    "mode 1",
    "mode 2",
    "mode 3",
    "mode 4",
    "mode 5",
    "forced blank",
    "text BG 4bpp",
    "text BG 8bpp",
    "text BG 256x256",
    "text BG 512x256",
    "text BG 256x512",
    "text BG 512x512",
    "affine BG, wrap",
    "affine BG, clipped",
    "BG mosaic",
    "window 0",
    "window 1",
    "OBJ window",
    "alpha blending",
    "brighten",
    "darken",
    "OBJ 1D",
    "OBJ 2D",
    "OBJ regular",
    "OBJ affine",
    "OBJ affine double size",
    "OBJ semi-transparent",
    "OBJ window OBJ",
    "OBJ mosaic",
    "OBJ 8bpp",
};

void HostPpuPrintStats(void)
{
    s32 i;

    printf("ppu-stats: %u lines\n", sStatLines);
    for (i = 0; i < HOST_PPU_STAT_COUNT; i++) {
        printf("ppu-stats: %-24s %u\n", sStatNames[i], sStats[i]);
    }
}
