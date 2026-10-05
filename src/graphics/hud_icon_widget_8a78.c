#include "core.h"
#include "bitmap_font.h"

extern void CpuSet(void *src, void *dst, s32 control);
extern u8 gFontVtable[];

/* Constructor variant used for a widget that's never assigned its own
 * data tables past `record` (the line-height/space-width/glyph fields
 * stay whatever the caller already set) - just the OAM-scratch zero and
 * cursor/margin reset shared with InitSmallFont/B
 * (src/graphics/hud_icon_widget_85c4.c).
 *
 * The `posX`/`posY`/`marginX`/`tileCount` zero-init needed the same
 * inline-asm address anchor as InitSmallFont/B - see that
 * function's own comment for the full account of why. Unlike A/B, there
 * is no charLookup-building loop here, so this one reaches a full
 * byte-exact match. */
struct bitmap_font *InitFont(struct bitmap_font *selfArg)
{
    register struct bitmap_font *self asm("r4") = selfArg;
    s32 zero;
    struct icon_record **recordAddr;

    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(recordAddr) : "r"(self) : "r0");
    *recordAddr = (struct icon_record *)gFontVtable;

    /* `zero`'s own store (`str r1, [sp]` right before the call) reuses
     * this same zeroed r1 too, instead of materializing a fresh 0 -
     * folded into this same asm block so the compiler can't tell them
     * apart and reload. */
    asm volatile(
        "mov r1, #0x88\n\tlsl r1, r1, #1\n\tadd r2, %1, r1\n\t"
        "add r1, r1, #4\n\tadd r0, %1, r1\n\t"
        "mov r1, #0\n\tstr r1, [r0]\n\tstr r1, [r2]\n\t"
        "mov r2, #0x8c\n\tlsl r2, r2, #1\n\tadd r0, %1, r2\n\t"
        "str r1, [r0]\n\t"
        "add r2, r2, #0x14\n\tadd r0, %1, r2\n\t"
        "str r1, [r0]\n\t"
        "str r1, %0"
        : "=m"(zero)
        : "r"(self)
        : "r0", "r1", "r2", "memory"
    );

    CpuSet(&zero, self, CPU_SET_SRC_FIXED | CPU_SET_32BIT | 2);
    return self;
}
