#include "gba/types.h"

/*
 * gStaticData_085AA170 (ROM 0x085AA170, 0x3A61 bytes): the first intro Mode 4
 * bitmap, 240x160 8bpp, LZ77 compressed (38400 bytes decompressed).
 *
 * The first graphics asset built as a C const array (see docs/graphics.md,
 * "PNG -> C const array (grit)"). The bytes come from
 * graphics/intro/00_5aa170_bitmap.png via graphics.mk:
 *   grit -gb -gB8 -p! -ftb   (PNG -> linear 8bpp bitmap)
 *   gbagfx                   (byte-exact LZ77)
 *   tools/bin2c.py --lz      (initializer bytes, trimmed to the stream)
 * ldscript.txt links this object between data/data.s's .rodata and its
 * .rodata.085ADBD1 tail, i.e. at the asset's original ROM address.
 */
const u8 gStaticData_085AA170[] = {
#include "intro/00_5aa170_bitmap.img.bin.lz.inc"
};
