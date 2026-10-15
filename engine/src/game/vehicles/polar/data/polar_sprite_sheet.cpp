extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x080B2120-0x080C0C38: gPolarSpriteSheet, the frames of the actor
 * category descriptors 0-2 (docs/graphics.md, "One PNG per frame, no sidecar").
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between games/xs/data/data.s sections by games/xs/ldscript.txt, see
 * docs/data.md.
 */

/*
 * The framed OBJ sprite sheet of actor categories 0-2 (213,064 bytes
 * unpacked): 4bpp frames, each behind a 4-byte header. Built by
 * tools/framed_gfx.py from the per-entity frame PNGs of
 * games/xs/graphics/vehicles/polar/sprite_sheet/.
 */
extern const u8 gPolarSpriteSheet[] __attribute__((aligned(4))) = {
#include "vehicles/polar/sprite_sheet.bin.lz.inc"
};
