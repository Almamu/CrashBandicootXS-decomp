/* An N-Tranced BG0 tile set (8bpp, a raw tagged asset), which
 * games/n2/levels.mk makes from the rooms' BG0 pictures (tools/levels.py
 * tileset). Between two room regions, as in the ROM. Its name is a
 * placeholder until its theme is known. See docs/n_tranced.md, "Levels". */

extern "C" {
#include "engine/gfx/tagged_asset.h"
}

#include "tilesets/gEgyptBg0Tiles.h"
extern const TAGGED_RAW_ASSET(TILES_SIZE_gEgyptBg0Tiles) gEgyptBg0Tiles = {
    TAGGED_RAW_HEADER(TILES_SIZE_gEgyptBg0Tiles),
    {
#include "tilesets/gEgyptBg0Tiles.bin.inc"
    },
};
