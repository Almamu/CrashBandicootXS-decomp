#include "core.h"
#include "gba/io_reg.h"
#include "gba/defines.h"

extern void sub_80391E8(u32 idx);
extern void sub_8037F3C(void *arg0, s32 size);
extern void sub_80392C4(void *src, void *dst);
extern void sub_8039214(u32 col, u32 row, const u8 *str);
extern u8 gUnknown_030008D0[];
extern u8 *gStaticData_085A62C8;
extern u8 gStaticData_085A62CC[];

/* GAX2's fatal-error screen: disables Timer0/1/2/3 direct-sound-output
 * ticking (sub_80391E8, still raw, has the same hardware-register
 * NOP-delay compiler quirk documented for sub_80384DC), zeroes the
 * whole BG VRAM, decompresses a font tileset (gUnknown_030008D0, a
 * fixed IWRAM scratch address - see sym_iwram.txt) via the HuffUnComp
 * SWI wrapper into BG char block 1, blanks its first tile (the "space"
 * glyph), pokes a handful of raw tilemap entries (0x060044B8/
 * 0x060044C8/0x060044FC - not modeled as named screen-block macros
 * since they don't fall on a screen-block boundary), draws a fixed
 * header string plus the two caller-supplied message lines via
 * sub_8039214 (still raw - a word-wrap text/console-tile renderer),
 * resets the BG0/backdrop palette to black-on-white, enables BG0 only,
 * and finally spins forever - this is the end of the road, nothing
 * ever returns from here. */
void sub_80392E0(const u8 *msg1, const u8 *msg2)
{
    vu16 *dst;
    u16 zero;
    s32 i;

    REG_IME = 0;

    sub_80391E8(0);
    sub_80391E8(1);
    sub_80391E8(2);
    sub_80391E8(3);

    sub_8037F3C((void *)BG_VRAM, 0x10000);

    sub_80392C4(gUnknown_030008D0, (void *)BG_CHAR_ADDR(1));

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

    sub_8039214(0, 0, gStaticData_085A62C8);
    sub_8039214(0, 5, gStaticData_085A62CC);
    sub_8039214(0xf, 5, msg1);
    sub_8039214(0, 7, msg2);

    *(vu16 *)0x05000002 = 0;
    *(vu16 *)0x05000000 = 0x7FFF;

    REG_BG0CNT = 4;
    REG_DISPCNT = 0x100;

    REG_BLDCNT = 0;
    REG_BLDALPHA = 0;
    REG_BLDY = 0;

    while (1) {}
}
