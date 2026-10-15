#include "graphics.hpp"

/*
 * ROM 0x080C2758-0x080FF1B0: two zero-run-compressed OBJ frame sets (the
 * old "rotation strips A and B"). Linked in ROM order between games/xs/data/data.s
 * sections by games/xs/ldscript.txt - see docs/data.md ("Compressed sprite frames").
 *
 * Each frame is a {w, h, 0x30, 0} header and a stream of u16 run counts:
 * a zero run, then alternately a literal run (count + that many halfwords
 * of 4bpp tile data) and a zero run, until w*h tiles are filled. The IWRAM
 * decoder behind gUnpackRleSpriteFrameFunc (0x03000634) unpacks one frame into a
 * VRAM tile block (polar_player.cpp, jetpack_player_update.cpp,
 * company_logos.cpp). The frames are stored back to back; the frame
 * pointer tables point at their headers.
 *
 * The sets are classes of graphics.hpp, PolarPlayerFrameSet::frames
 * (categories 0-2's animation record 0, table_B gPolarPlayerFrames: Crash
 * riding the polar bear, games/xs/graphics/vehicles/polar/player.png) and
 * YetiFrameSet::frames (the CreateYeti singleton's, Yeti::frames in
 * yeti_frames.cpp: games/xs/graphics/vehicles/polar/yeti.png), defined by
 * tools/gfx_manifest.py from games/xs/graphics/manifest.json (docs/graphics.md,
 * "Graphics classes"). tools/rle_sprites.py writes their bytes and the
 * frames' offsets (vehicles/polar/player.h, yeti.h; graphics.mk).
 */
#include "polar_rle_sprites.inc"
