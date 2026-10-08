#ifndef GUARD_GFX_PART_H
#define GUARD_GFX_PART_H

/* A helper of the sprite pieces' draw loops (src/gfx/sprite_pieces.cpp,
 * affine_sprite_pieces.cpp). The C view this header was named after,
 * `struct gfx_part` (a moving sprite, as include/sprite_obj.hpp's
 * MovingSprite has it) and its `struct anim_bank`/`anim_record` records,
 * went with their last users, and its `struct gfx_vec` is aabb.h's
 * `struct vec2` (#656). */

/* Bit `7 - (shift - 24)` of the flags byte at +0x28 (the OBJ mode, mirror
 * and 8bpp bits), tested as a sign test (`lsl #shift; cmp #0; bge`), as
 * the sprite piece loops do; a 1-bit field test compiles to `movs #0x20;
 * ands` instead. */
#define PART_FLAG_SET(part, shift) ((s32)(*((u8 *)(part) + 0x28) << (shift)) < 0)

#endif /* GUARD_GFX_PART_H */
