/* An N-Tranced BG0 tile set (8bpp, a raw tagged asset), which
 * games/n2/levels.mk makes from the rooms' BG0 pictures (tools/levels.py
 * tileset). Between two room regions, as in the ROM. Its name is a
 * placeholder until its theme is known. See docs/n_tranced.md, "Levels". */

extern "C" {
#include "engine/gfx/tagged_asset.h"
}

#include "tilesets/gStaticData_081E5394.h"
extern const TAGGED_RAW_ASSET(TILES_SIZE_gStaticData_081E5394) gStaticData_081E5394 = {
    TAGGED_RAW_HEADER(TILES_SIZE_gStaticData_081E5394),
    {
#include "tilesets/gStaticData_081E5394.bin.inc"
    },
};
