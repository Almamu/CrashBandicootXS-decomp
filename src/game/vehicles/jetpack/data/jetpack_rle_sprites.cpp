#include "graphics.hpp"

/*
 * ROM 0x0815A050-0x08167AD4: a zero-run-compressed OBJ frame set (the old
 * "rotation strip C"), same format as polar_rle_sprites.cpp. Linked in ROM
 * order between data/data.s sections by ldscript.txt - see docs/data.md
 * ("Compressed sprite frames").
 *
 * The set is JetpackPlayerFrameSet::frames of graphics.hpp (categories
 * 3-6's animation record 0, table_B gJetpackPlayerFrames: 80 frames of 8x8
 * tiles, graphics/vehicles/jetpack/player.png), defined by
 * tools/gfx_manifest.py from graphics/manifest.json (docs/graphics.md,
 * "Graphics classes").
 */
#include "jetpack_rle_sprites.inc"
