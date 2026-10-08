#ifndef __PAUSE_MENU_H__
#define __PAUSE_MENU_H__

#include "menus.h"
#include "graphics_package.h"
#include "objects.h" /* GetSpriteAnimPaletteSlot */

/* A small `struct actor`-derived on-screen icon: the first 0x1c bytes
 * are a plain `struct actor` (see actor.h), then a second keyframe-
 * table pointer at +0x20 and a frame index at +0x2d - both already
 * established by the already-matched GetSpriteAnimPaletteSlot/SpriteHitboxOverlaps
 * (src/objects/sprite_obj.cpp), which read this exact same object
 * through raw offsets. +0x29's low nibble and +0x3c are new fields this
 * chunk's functions write but don't otherwise interpret. Allocated with
 * `OperatorNew(0x40)` - bigger than plain `struct actor` (0x1c), so it
 * has more trailing fields this chunk's functions never touch. */
struct settings_icon_actor {
    struct actor base; /* 0x00-0x1b */
    u8 unused_1c[0x20 - 0x1c];
    void **anim; /* 0x20 - the sprite bank (sprite_bank.h), see GetSpriteAnimPaletteSlot */
    u8 unused_24[0x29 - 0x24];
    u8 palette; /* 0x29 - low nibble: the OBJ palette slot (GetSpriteAnimPaletteSlot) */
    u8 unused_2a[0x2d - 0x2a];
    u8 frameIndex; /* 0x2d - current keyframe index, see GetSpriteAnimPaletteSlot */
    u8 unused_2e[0x38 - 0x2e];
    u8 animDone; /* 0x38 - the animation reached its end (SetSpriteAnimDone) */
    u8 unused_39[0x3c - 0x39];
    /* 0x3c - Q8 affine scale, 0 = not affine (DrawAffineSpritePieces);
     * InitPauseGemsPage/InitPauseRelicsPage draw their icons at half size (0x80) */
    u16 scale;
};

#endif /* __PAUSE_MENU_H__ */
