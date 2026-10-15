#ifndef GUARD_SPRITE_PIECE_FLAGS_H
#define GUARD_SPRITE_PIECE_FLAGS_H

/* A helper of the sprite pieces' draw loops (DrawPieces and
 * DrawAffinePieces, engine/src/engine/gfx/sprite_renderer.cpp). The C view this header was named after,
 * `struct gfx_part` (a moving sprite, as engine/include/game/entities/sprite_obj.hpp's
 * MovingSprite has it) and its `struct anim_bank`/`anim_record` records,
 * went with their last users, and its `struct gfx_vec` is aabb.h's
 * `struct vec2` (#656). */

/* Bit `7 - (shift - 24)` of Sprite::mirror, the flags byte at +0x28 (the
 * OBJ mode, mirror and 8bpp bits), tested as a sign test (`lsl #shift;
 * cmp #0; bge`), as the sprite piece loops do; a 1-bit field test
 * compiles to `movs #0x20; ands` instead. */
#define PART_FLAG_SET(part, shift) ((s32)((part)->mirror << (shift)) < 0)

#endif /* GUARD_SPRITE_PIECE_FLAGS_H */
