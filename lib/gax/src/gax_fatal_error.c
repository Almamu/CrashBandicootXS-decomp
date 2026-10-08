#include "gax_internal.h"

/* One 4bpp pixel row (4 bytes) of tile `tile` in the font's char block 1. */
#define HALT_FONT_ROW(tile, row) (*(vu32 *)(BG_CHAR_ADDR(1) + (tile) * 32 + (row) * 4))
/* BG palette entry `n`. */
#define BG_PLTT_COLOR(n) (*(vu16 *)(BG_PLTT + (n) * 2))

/* GAX2's fatal-error screen: disables Timer0/1/2/3 direct-sound-output
 * ticking (GaxStopDma, still raw, has the same hardware-register
 * NOP-delay compiler quirk documented for GaxResetSoundHardware), zeroes the
 * whole BG VRAM, decompresses a font tileset (gGaxHaltFont, the
 * Huffman-compressed font in the IWRAM image - src/iwram/iwram_data.cpp)
 * via the HuffUnComp
 * SWI wrapper into BG char block 1, blanks its first tile (the "space"
 * glyph), draws the '.', ':' and '_' glyphs the font lacks, draws a fixed
 * header string plus the two caller-supplied message lines via
 * GaxDrawText (still raw - a word-wrap text/console-tile renderer),
 * resets the BG0/backdrop palette to black-on-white, enables BG0 only,
 * and finally spins forever - this is the end of the road, nothing
 * ever returns from here. */
void GaxFatalError(const char *msg1, const char *msg2)
{
    vu16 *dst;
    u16 zero;
    s32 i;

    REG_IME = 0;

    GaxStopDma(0);
    GaxStopDma(1);
    GaxStopDma(2);
    GaxStopDma(3);

    GaxZeroFill((void *)BG_VRAM, 0x10000);

    GaxHuffUnComp(gGaxHaltFont, (void *)BG_CHAR_ADDR(1));

    dst = (vu16 *)BG_CHAR_ADDR(1);
    zero = 0;
    for (i = 0xf; i >= 0; i--) {
        *dst = zero;
        dst++;
    }

    /* The font has no punctuation: draw the three glyphs GaxDrawText
     * maps '.', ':' and '_' to (tiles 37-39) a pixel row at a time. */
    HALT_FONT_ROW(37, 6) = 0x1000;  /* '.' */
    HALT_FONT_ROW(38, 2) = 0x10000; /* ':' */
    HALT_FONT_ROW(38, 5) = 0x10000;
    HALT_FONT_ROW(39, 7) = 0x01111110; /* '_' */

    GaxDrawText(0, 0, gGaxHaltBannerPtr);
    GaxDrawText(0, 5, gGaxHaltFunctionLabel);
    GaxDrawText(0xf, 5, msg1);
    GaxDrawText(0, 7, msg2);

    BG_PLTT_COLOR(1) = 0;      /* black text */
    BG_PLTT_COLOR(0) = 0x7FFF; /* on white */

    REG_BG0CNT = 4;
    REG_DISPCNT = 0x100;

    REG_BLDCNT = 0;
    REG_BLDALPHA = 0;
    REG_BLDY = 0;

    while (1) {
    }
}
