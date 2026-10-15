extern "C" {
#include "gba/types.h"
#include "engine/gfx/sprite_bank.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x08492908-0x08492acb: sprite banks 47-48 of the sprite-bank
 * animation system (gSpriteBankTable, include/engine/gfx/sprite_bank.h). Linked in
 * ROM order between the incbin'd data by games/n2/ldscript.txt - see
 * docs/data.md and docs/data_map.md ("gSpriteBankTable").
 *
 * Extracted once from baserom_n2.gba by tools/sprite_banks.py --graphics-dir games/n2/graphics; this file is
 * the source now. Per bank: the animations (a frame-index sequence each),
 * the frame pointer array, the frames (header plus the boxes/anchor of
 * their layout type), then every frame's piece positions and piece bytes.
 * The pieces and the frames' tiles come from games/n2/graphics/sprites/bankNN_<name>/
 * (a PNG per frame and bank.json, see docs/graphics.md "Sprite banks"):
 * tools/sprite_frames.py generates the piece arrays (sprites/bankNN_pieces.inc),
 * and sprites/sprite_frames.h declares the frames and gives each one's
 * offset in the sprite tile pool (games/n2/src/engine/gfx/data/sprite_tiles.cpp),
 * SPRITE_TILES_BANKnn_FRAMEkkk. A frame's struct type has to match its
 * "layout" in bank.json.
 */

/* ---------------------------------------------------------------------- */
/* Bank 47: 1 animation, 3 frames, games/n2/graphics/sprites/bank47_ufo/. */

extern const u16 gSpriteBank47Anim00Seq[3];

extern const struct sprite_anim gSpriteBank47Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank47Anim00Seq,
        /* box */ { { -22, -20, 45, 41 }, { -22, -20, 45, 41 } },
        /* paletteId */ 89,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank47Anim00Seq[3] = {
    0, 1, 2,
};

extern const struct sprite_frame *const gSpriteBank47Frames[3] = {
    &gSpriteBank47Frame000,
    &gSpriteBank47Frame001,
    &gSpriteBank47Frame002,
};

const struct sprite_frame gSpriteBank47Frame000 = SPRITE_FRAME(gSpriteBank47Frame000, SPRITE_TILES_BANK47_FRAME000);
const struct sprite_frame gSpriteBank47Frame001 = SPRITE_FRAME(gSpriteBank47Frame001, SPRITE_TILES_BANK47_FRAME001);
const struct sprite_frame gSpriteBank47Frame002 = SPRITE_FRAME(gSpriteBank47Frame002, SPRITE_TILES_BANK47_FRAME002);

/* The piece arrays, from games/n2/graphics/sprites/bank47_ufo/bank.json. */
#include "sprites/bank47_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 48: 5 animations, 5 frames, games/n2/graphics/sprites/bank48_palm_trees/. */

extern const u16 gSpriteBank48Anim00Seq[1];
extern const u16 gSpriteBank48Anim01Seq[1];
extern const u16 gSpriteBank48Anim02Seq[1];
extern const u16 gSpriteBank48Anim03Seq[1];
extern const u16 gSpriteBank48Anim04Seq[1];

extern const struct sprite_anim gSpriteBank48Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank48Anim00Seq,
        /* box */ { { -17, -15, 35, 31 }, { -17, -15, 35, 31 } },
        /* paletteId */ 90,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank48Anim01Seq,
        /* box */ { { -37, -45, 75, 91 }, { -37, -45, 75, 91 } },
        /* paletteId */ 91,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank48Anim02Seq,
        /* box */ { { -11, -10, 23, 20 }, { -11, -10, 23, 20 } },
        /* paletteId */ 90,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank48Anim03Seq,
        /* box */ { { -13, -11, 27, 23 }, { -13, -11, 27, 23 } },
        /* paletteId */ 90,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank48Anim04Seq,
        /* box */ { { -37, -38, 75, 77 }, { -37, -38, 75, 77 } },
        /* paletteId */ 92,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank48Anim00Seq[1] = {
    0,
};
const u16 gSpriteBank48Anim01Seq[1] = {
    1,
};
const u16 gSpriteBank48Anim02Seq[1] = {
    2,
};
const u16 gSpriteBank48Anim03Seq[1] = {
    3,
};
const u16 gSpriteBank48Anim04Seq[1] = {
    4,
};

extern const struct sprite_frame *const gSpriteBank48Frames[5] = {
    &gSpriteBank48Frame000,
    &gSpriteBank48Frame001,
    &gSpriteBank48Frame002,
    &gSpriteBank48Frame003,
    &gSpriteBank48Frame004,
};

const struct sprite_frame gSpriteBank48Frame000 = SPRITE_FRAME(gSpriteBank48Frame000, SPRITE_TILES_BANK48_FRAME000);
const struct sprite_frame gSpriteBank48Frame001 = SPRITE_FRAME(gSpriteBank48Frame001, SPRITE_TILES_BANK48_FRAME001);
const struct sprite_frame gSpriteBank48Frame002 = SPRITE_FRAME(gSpriteBank48Frame002, SPRITE_TILES_BANK48_FRAME002);
const struct sprite_frame gSpriteBank48Frame003 = SPRITE_FRAME(gSpriteBank48Frame003, SPRITE_TILES_BANK48_FRAME003);
const struct sprite_frame gSpriteBank48Frame004 = SPRITE_FRAME(gSpriteBank48Frame004, SPRITE_TILES_BANK48_FRAME004);

/* The piece arrays, from games/n2/graphics/sprites/bank48_palm_trees/bank.json. */
#include "sprites/bank48_pieces.inc"
