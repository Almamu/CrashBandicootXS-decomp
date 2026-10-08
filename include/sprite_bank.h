#ifndef GUARD_SPRITE_BANK_H
#define GUARD_SPRITE_BANK_H

#include "core.h"
#include "hitbox_quad.h"

/*
 * The sprite-bank animation system: gSpriteBankTable and the tables it
 * points at (src/data/sprite_banks_*.c, docs/data_map.md). An animated
 * part's +0x20 field points at one struct sprite_bank; +0x2D is the
 * animation index into its `anims`, +0x30 the step within that animation.
 *
 * Readers: GetSpriteTileBase (tileBase), GetSpriteFrame (anim -> seq -> frame),
 * DrawAffineSpritePieces/DrawSpritePieces (the pieces), GetSpriteAttackBox/GetSpriteBodyBox and
 * GetSpriteFrameAnchor-GetSpriteFrameBodyBox (the frame's boxes and anchor, picked by the
 * layout type), GetPaletteSlot (the OBJ palettes). Older files read the
 * same records through views with only the fields they use (struct
 * anim_record/anim_bank in gfx_part.h, struct act_anim_record in
 * player.h, struct anim_rec/anim_table in gobj_1a794.h, struct
 * piece_info in gfx.h). The menus' copies (level_menu.h,
 * level_select_parts.h, level_select.c) and the file-local ones
 * (time_trial.cpp, dingodile.cpp, tiny_update.cpp, spawn_objects.cpp,
 * affine_sprite_pieces.cpp's kf_record) use these types since #574 batch 9e.
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
 * pieces' tiles are back to back from there. */
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
 * Where each bank's tiles start in the sprite tile pool: the offset of the
 * bank's array in src/data/sprite_tiles_2bf120.c from gSpriteBank00Tiles
 * (graphics/sprites/bankNN_*.png, TILE_BYTES_bankNN_* in graphics.mk).
 * Frames give their tile offset relative to these. Resizing a bank's PNG
 * moves every later bank, so these have to follow.
 */
#define SPRITE_TILES_BANK00 0x000000
#define SPRITE_TILES_BANK01 0x03cfe0
#define SPRITE_TILES_BANK02 0x068560
#define SPRITE_TILES_BANK03 0x06ffc0
#define SPRITE_TILES_BANK04 0x076720
#define SPRITE_TILES_BANK05 0x07e640
#define SPRITE_TILES_BANK06 0x081a00
#define SPRITE_TILES_BANK07 0x082dc0
#define SPRITE_TILES_BANK08 0x089e80
#define SPRITE_TILES_BANK09 0x08d540
#define SPRITE_TILES_BANK10 0x08e9e0
#define SPRITE_TILES_BANK11 0x094aa0
#define SPRITE_TILES_BANK12 0x09a500
#define SPRITE_TILES_BANK13 0x0a51a0
#define SPRITE_TILES_BANK14 0x0a7280
#define SPRITE_TILES_BANK15 0x0abfc0
#define SPRITE_TILES_BANK16 0x0b1e00
#define SPRITE_TILES_BANK17 0x0b7740
#define SPRITE_TILES_BANK18 0x0b9ce0
#define SPRITE_TILES_BANK19 0x0bd7c0
#define SPRITE_TILES_BANK20 0x0c1680
#define SPRITE_TILES_BANK21 0x0c3a60
#define SPRITE_TILES_BANK22 0x0c5260
#define SPRITE_TILES_BANK23 0x0cace0
#define SPRITE_TILES_BANK24 0x0deaa0
#define SPRITE_TILES_BANK25 0x0ec000
#define SPRITE_TILES_BANK26 0x0f3880
#define SPRITE_TILES_BANK27 0x0f7300
#define SPRITE_TILES_BANK28 0x0f9560
#define SPRITE_TILES_BANK29 0x0f9860
#define SPRITE_TILES_BANK30 0x1057e0
#define SPRITE_TILES_BANK31 0x11eca0
#define SPRITE_TILES_BANK32 0x133240
#define SPRITE_TILES_BANK33 0x133f00
#define SPRITE_TILES_BANK34 0x135b00
#define SPRITE_TILES_BANK35 0x136ae0
#define SPRITE_TILES_BANK36 0x138120
#define SPRITE_TILES_BANK37 0x1389c0
#define SPRITE_TILES_BANK38 0x1393c0
#define SPRITE_TILES_BANK39 0x13e8c0
#define SPRITE_TILES_BANK40 0x144f40
#define SPRITE_TILES_BANK41 0x147f60
#define SPRITE_TILES_BANK42 0x148a60
#define SPRITE_TILES_BANK43 0x148e40
#define SPRITE_TILES_BANK44 0x14cbe0
#define SPRITE_TILES_BANK45 0x14d240
#define SPRITE_TILES_BANK46 0x150640
#define SPRITE_TILES_BANK47 0x150780
#define SPRITE_TILES_BANK48 0x152640
#define SPRITE_TILES_BANK49 0x15a640
#define SPRITE_TILES_BANK50 0x15a9c0
#define SPRITE_TILES_BANK51 0x15d060
#define SPRITE_TILES_BANK52 0x15dda0
#define SPRITE_TILES_BANK53 0x15dfa0
#define SPRITE_TILES_BANK54 0x16e360
#define SPRITE_TILES_BANK55 0x1ac080

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
