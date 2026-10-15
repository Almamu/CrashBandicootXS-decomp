#ifndef GUARD_HITBOX_QUAD_H
#define GUARD_HITBOX_QUAD_H

#include "gba/types.h"

/* A sprite box, `{s16 offX, s16 offY, u8 w, u8 h}`: an offset from the
 * part's position and a size, in pixels. A sprite animation has two
 * (`struct sprite_anim.box`, sprite_bank.h: +0x4 is the box
 * GetSpriteHitbox/PlayerHasRoomForAnim/BreakCrateTouchedByPlayer read,
 * +0xc the one GetSpriteBounds reads) and a sprite frame one to three
 * (GetSpriteAttackBox/GetSpriteBodyBox add {offX, offY} to the part's
 * position and hand that and {w, h} to SetAabbPos/SetAabbSize as an
 * AABB). An entity's vtable slot 2 returns a pointer to one
 * (CheckEntityPlayerContact), and gEmptySpriteBox is the all-zero
 * fallback.
 *
 * sprite_flags.h's `struct part_box`, the `struct anim_box` of graphics.cpp (now entity.cpp) and
 * gobj_1a794.h (batch 8a) and sprite_bank.h's `struct sprite_box` (`x`/`y`
 * were `offX`/`offY`; #574, batch 9e) were copies.
 *
 * N-Tranced's are `{s16 offX, s16 offY, u16 w, u16 h}`, the same 8 bytes
 * with no padding: in its sprite banks' 2,903 boxes the height is at +6
 * and +5 (XS's h) is always 0, as is w's high byte (docs/n_tranced.md,
 * "Sprite banks"). */
struct hitbox_quad {
    s16 offX;
    s16 offY;
#ifdef GAME_N2
    u16 w;
    u16 h;
#else
    u8 w;
    u8 h;
#endif
};

#endif /* GUARD_HITBOX_QUAD_H */
