#include "core.h"
#include "system.h"

/*
 * ROM 0x08167AD4-0x0816AA20. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The two boss pictures (docs/data.md "Boss pictures"): a palette, then
 * {s16 cols, s16 rows} and the frames, each a tile count, a `cols * rows`
 * map of u16 tile indices and the frame's new 4bpp tiles. All frames
 * share one tile pool (a frame's indices count on from the tiles of the
 * frames before it). The code finds the frames by walking their sizes
 * from the palette's label + 0x204, so each palette and its picture
 * must stay back to back.
 *
 * The frames and their tile counts are built from
 * graphics/boss_pictures/<addr>_frameN.png by grit and
 * tools/boss_pictures.py. The game converts the 4bpp tiles to 8bpp VRAM
 * tiles of BG palette 1 (the picture is drawn on an affine BG, which
 * only takes 8bpp tiles), and lays them out by the maps with
 * DrawAirshipMap. */
#define BOSS_FRAME(ncells, ntiles) struct { s32 tileCount; u16 map[ncells]; u8 tiles[(ntiles) * 32]; }

#include "boss_pictures/167cd4.h"
#include "boss_pictures/169ce8.h"

/* N. Gin's airship: its palette, 16 colours that LoadAirshipGraphics
 * (airship_load_graphics.c) DMAs to BG palette 1 (the rest is zero). */
const u16 gAirshipPalette[256] = {
    0x03E0, 0x30E7, 0x3549, 0x41AC, 0x46C5, 0x3222, 0x1DA0, 0x033F,
    0x02BF, 0x3AB9, 0x05F7, 0x0194, 0x5B3B, 0x29B0, 0x14BF, 0x7FFF,
};

#define AIRSHIP_CELLS (BOSS_PICTURE_167CD4_COLS * BOSS_PICTURE_167CD4_ROWS)

/* The airship, 4 frames (the propellers turn). CreateAirship
 * (airship.c) reads cols and rows, ConvertAirshipTiles (airship_graphics.c)
 * uploads the tiles. */
const struct {
    s16 cols, rows;
    BOSS_FRAME(AIRSHIP_CELLS, BOSS_PICTURE_167CD4_FRAME0_TILES) frame0;
    BOSS_FRAME(AIRSHIP_CELLS, BOSS_PICTURE_167CD4_FRAME1_TILES) frame1;
    BOSS_FRAME(AIRSHIP_CELLS, BOSS_PICTURE_167CD4_FRAME2_TILES) frame2;
    BOSS_FRAME(AIRSHIP_CELLS, BOSS_PICTURE_167CD4_FRAME3_TILES) frame3;
} gAirshipPicture = {
    BOSS_PICTURE_167CD4_COLS, BOSS_PICTURE_167CD4_ROWS,
#include "boss_pictures/167cd4.inc"
};

/* Cortex's hovercraft: its palette. LoadHovercraftGraphics (hovercraft.c) DMAs
 * the first 16 colours to BG palette 1 and UpdateHovercraftHitFlash restores them
 * from here; the other 240 entries are the 0x03E0 filler colour. */
const u16 gHovercraftPalette[256] = {
    0x03E0, 0x66F5, 0x5250, 0x41EF, 0x25AF, 0x7FFF, 0x1CE9, 0x2D04,
    0x3988, 0x0C45, 0x35DE, 0x003C, 0x14B5, 0x0936, 0x15F5, 0x16FF,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
    0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0, 0x03E0,
};

/* The hovercraft, 1 frame. CreateHovercraft (hovercraft.c) reads cols and
 * rows, ConvertHovercraftTiles uploads the tiles. */
const struct {
    s16 cols, rows;
    BOSS_FRAME(BOSS_PICTURE_169CE8_COLS * BOSS_PICTURE_169CE8_ROWS, BOSS_PICTURE_169CE8_FRAME0_TILES) frame0;
} gHovercraftPicture = {
    BOSS_PICTURE_169CE8_COLS, BOSS_PICTURE_169CE8_ROWS,
#include "boss_pictures/169ce8.inc"
};

/* GetDpadDirection (irq.c): the d-pad direction (0-8, 0 = none) of each
 * combination of the right/left/up/down bits. */
const u8 gDpadDirectionTable[16] = {
    0, 1, 2, 2, 3, 5, 7, 7, 4, 6, 8, 8, 4, 6, 8, 8,
};

/* The sine table: 256 steps of a full turn, scaled by 0x100. Read by
 * about twenty actor and game-loop functions (include/orbit_part.h's
 * `phase` indexes it). */
const s16 gSineTable[256] = {
    0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92,
    97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176,
    181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234,
    236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255,
    256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238,
    236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185,
    181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103,
    97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6,
    0, -6, -12, -18, -25, -31, -37, -43, -49, -56, -62, -68, -74, -80, -86, -92,
    -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176,
    -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234,
    -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255,
    -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238,
    -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185,
    -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103,
    -97, -92, -86, -80, -74, -68, -62, -56, -49, -43, -37, -31, -25, -18, -12, -6,
};
