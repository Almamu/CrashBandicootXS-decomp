#ifndef GUARD_SPRITE_BANK_H
#define GUARD_SPRITE_BANK_H

#include "core.h"
#include "engine/gfx/hitbox_quad.h"

/*
 * The sprite-bank animation system: gSpriteBankTable and the tables it
 * points at (engine/src/.../data/sprite_banks_*.c, docs/data_map.md). An animated
 * part's +0x20 field points at one struct sprite_bank; +0x2D is the
 * animation index into its `anims`, +0x30 the step within that animation.
 *
 * Readers: GetSpriteTileBase (tileBase), GetSpriteFrame (anim -> seq -> frame),
 * DrawAffineSpritePieces/DrawSpritePieces (the pieces), GetSpriteAttackBox/GetSpriteBodyBox and
 * GetSpriteFrameAnchor-GetSpriteFrameBodyBox (the frame's boxes and anchor, picked by the
 * layout type), GetPaletteSlot (the OBJ palettes). Sprite's `bank`
 * (sprite_obj.hpp) is a `const struct sprite_bank *`; the copies the older
 * files read the records through (player.h's struct act_anim_record and
 * act_anim_bank, gobj_1a794.h's struct anim_rec and anim_table, the HUD's
 * and gfx_part.h's) are merged into these (#656). gfx.h's struct
 * piece_info is the frame header's view. The menus' copies
 * (level_menu.h's, level_select_parts.h, level_select.c) and the file-local ones
 * (level_state.cpp, dingodile.cpp, tiny.cpp, spawn_objects.cpp,
 * sprite_renderer.cpp's kf_record) use these types since #574 batch 9e.
 */

/* One OBJ piece's position relative to the part, in pixels. */
struct sprite_piece_pos {
    s16 x;
    s16 y;
};

/* A point relative to the part (GetSpriteFrameAnchor; the fallback when a frame has
 * none is gEmptySpritePoint). */
struct sprite_point {
    s16 x;
    s16 y;
};

/*
 * A piece byte: the low nibble indexes the OBJ shape/size tables
 * gObjPieceWidths (width) / gObjPieceHeights (height); the high
 * nibble of a frame's *first* piece is the frame's layout type, which says
 * which of the records below follow the 12-byte header (the readers switch
 * on it):
 *
 *   type 1          nothing                    struct sprite_frame
 *   type 2, 5       box[0]                     struct sprite_frame_1box
 *   type 6          box[0], anchor             struct sprite_frame_1box_anchor
 *   type 3          box[0..1]                  struct sprite_frame_2box
 *   type 4          box[0..2]                  struct sprite_frame_3box
 *   type 0          box[0..2], anchor          struct sprite_frame_3box_anchor
 */
#define SPRITE_PIECE(type, shape) (((type) << 4) | (shape))

/* `tiles`: bits 0-23 are the frame's byte offset into the sprite tile pool
 * (tileBase, gSpriteBank00Tiles), bits 24-31 the piece count. The
 * pieces' tiles are back to back from there, each piece's in 1D OBJ order. */
#define SPRITE_FRAME_TILES(offset, count) (((u32)(count) << 24) | (offset))

struct sprite_frame {
    const struct sprite_piece_pos *pos; /* 0x00 - [count] */
    const u8 *pieces;                   /* 0x04 - [count], SPRITE_PIECE */
    u32 tiles;                          /* 0x08 - SPRITE_FRAME_TILES */
};

/* The header of frame `name`, whose pieces are the arrays namePos and
 * namePieces, with its tiles at pool offset `offset`. */
#define SPRITE_FRAME(name, offset) \
    { name##Pos, name##Pieces, SPRITE_FRAME_TILES(offset, ARRAY_COUNT(name##Pos)) }

/*
 * Each frame's tile offset, SPRITE_TILES_BANKnn_FRAMEkkk, and each bank's
 * range of the pool, SPRITE_TILES_BANKnn(_SIZE), are generated from
 * games/xs/graphics/sprites/bankNN_<name>/ by tools/sprite_frames.py, in
 * sprites/sprite_frames.h (built under build/, included by
 * engine/src/.../data/sprite_banks_*.cpp and sprite_tiles.cpp).
 */

struct sprite_frame_1box {
    struct sprite_frame frame;
    struct hitbox_quad box[1]; /* 0x0C */
};

struct sprite_frame_1box_anchor {
    struct sprite_frame frame;
    struct hitbox_quad box[1];  /* 0x0C */
    struct sprite_point anchor; /* 0x14 */
};

struct sprite_frame_2box {
    struct sprite_frame frame;
    struct hitbox_quad box[2]; /* 0x0C, 0x14 */
};

struct sprite_frame_3box {
    struct sprite_frame frame;
    struct hitbox_quad box[3]; /* 0x0C, 0x14, 0x1C */
};

struct sprite_frame_3box_anchor {
    struct sprite_frame frame;
    struct hitbox_quad box[3];  /* 0x0C, 0x14, 0x1C */
    struct sprite_point anchor; /* 0x24 */
};

/* sprite_anim.flags */
#define SPRITE_ANIM_LOOP 2 /* GetSpriteFrame: without it the last step holds */

/* One animation: a sequence of frame indices into the bank's `frames`. */
struct sprite_anim {
    const u16 *seq;            /* 0x00 - [frameCount] frame indices */
    struct hitbox_quad box[2]; /* 0x04, 0x0C */
    /* 0x14 - index into the table's `palettes` (GetPaletteSlot/LockPalette) */
    u8 paletteId;
    u8 duration;   /* 0x15 - ticks per step */
    u8 frameCount; /* 0x16 - steps in `seq` */
    u8 flags;      /* 0x17 - SPRITE_ANIM_LOOP */
    u32 unk_18;    /* always 0 */
};

/* One sprite bank (an actor's whole animation set). */
struct sprite_bank {
    const struct sprite_anim *anims;          /* 0x00 */
    const struct sprite_frame *const *frames; /* 0x04 */
    u16 unk_08;                               /* always 0 */
    u16 animCount;                            /* 0x0A */
};

/* gSpriteBankTable, *gSpriteBankSet. */
struct sprite_bank_table {
    const struct sprite_bank *banks; /* 0x00 - [bankCount] */
    const u8 *tileBase;              /* 0x04 - sprite tile pool, GetSpriteTileBase */
    const u8 *palettes; /* 0x08 - the 16-colour OBJ palettes (32 bytes each) GetPaletteSlot
                         *        copies into the palette cache; InitLevelState/RunPauseMenu */
    u16 bankCount;      /* 0x0C */
    u16 paletteCount;   /* 0x0E - 125 */
};

#endif /* GUARD_SPRITE_BANK_H */
