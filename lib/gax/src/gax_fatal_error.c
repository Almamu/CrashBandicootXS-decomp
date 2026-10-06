#include "gax_internal.h"

/* GAX2's fatal-error screen: disables Timer0/1/2/3 direct-sound-output
 * ticking (GaxStopDma, still raw, has the same hardware-register
 * NOP-delay compiler quirk documented for GaxResetSoundHardware), zeroes the
 * whole BG VRAM, decompresses a font tileset (gGaxHaltFont, the
 * Huffman-compressed font in the IWRAM image - src/iwram/iwram_data.c)
 * via the HuffUnComp
 * SWI wrapper into BG char block 1, blanks its first tile (the "space"
 * glyph), pokes a handful of raw tilemap entries (0x060044B8/
 * 0x060044C8/0x060044FC - not modeled as named screen-block macros
 * since they don't fall on a screen-block boundary), draws a fixed
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

    *(vu32 *)0x060044B8 = 0x1000;
    *(vu32 *)0x060044C8 = 0x10000;
    *(vu32 *)0x060044D4 = 0x10000;
    *(vu32 *)0x060044FC = 0x01111110;

    GaxDrawText(0, 0, gGaxHaltBannerPtr);
    GaxDrawText(0, 5, gGaxHaltFunctionLabel);
    GaxDrawText(0xf, 5, msg1);
    GaxDrawText(0, 7, msg2);

    *(vu16 *)0x05000002 = 0;
    *(vu16 *)0x05000000 = 0x7FFF;

    REG_BG0CNT = 4;
    REG_DISPCNT = 0x100;

    REG_BLDCNT = 0;
    REG_BLDALPHA = 0;
    REG_BLDY = 0;

    while (1) {}
}
