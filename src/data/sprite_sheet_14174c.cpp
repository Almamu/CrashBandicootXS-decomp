extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x0814174C-0x08151AC4: gJetpackSpriteSheet, the frames of the actor
 * category descriptors 3-6 (docs/graphics.md, "One PNG per frame, no sidecar").
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between data/data.s sections by ldscript.txt, see
 * docs/data.md.
 */

/*
 * The framed OBJ sprite sheet of actor categories 3-6 (207,124 bytes
 * unpacked): 4bpp frames, each behind a 4-byte header. Built by
 * tools/framed_gfx.py from the per-entity frame PNGs of
 * graphics/unknown/01_14174c/.
 */
extern const u8 gJetpackSpriteSheet[] __attribute__((aligned(4))) = {
#include "vehicles/jetpack/sprite_sheet.bin.lz.inc"
};
