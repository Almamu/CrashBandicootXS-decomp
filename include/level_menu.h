#ifndef GUARD_LEVEL_MENU_H
#define GUARD_LEVEL_MENU_H

/* The menus' shared C types: the 0x40-byte UI sprite part as the C files
 * see it (`struct sprite`, the pause menu's icons). The save block
 * PackSaveData returns, read by the level select, the pause menu and the
 * power dialog, is level_state.h's `struct game_progress` (it was
 * `struct menu_save` here; #656, batch 8).
 *
 * The level select's own objects are C++ classes now (include/
 * level_select.hpp, #664 part 10): the screen, its page entries, the two
 * background layers and the cursor. level_select.c, level_select_widgets.c
 * and level_select_parts.h used to carry their own copies of these types;
 * they were merged here (#574, batch 9e), and moved to the classes. */

#include "menus.h"
#include "gfx.h"
#include "actor_self.h"
#include "sprite_bank.h"
#include "level_state.h"

/* Bits of a sprite's `+0x28` byte (see Sprite::MirrorBits in
 * include/sprite_obj.hpp). */
struct sprite_f28 {
    u8 mode:2;
    u8 unk_2:2;
    u8 flipX:1;
    u8 flipY:1;
    u8 unk_6:2;
} __attribute__((packed));

/* The sprite part's method table (at part+0x18); +0x50 (slot 10) is the
 * destructor. */
struct sprite_vtable {
    u8 unk_00[0x50];
    struct actor_method m50; // 0x50
};

/* The 0x40-byte animated sprite part `InitUiSpriteObj` constructs (class
 * UiSprite, sprite_obj.hpp), as the C files see it. Its animation set is a sprite bank (sprite_bank.h; the records were
 * `struct anim_record`/`anim_table`, whose `tileRecord`/`paletteId` is
 * `sprite_anim.paletteId`). */
struct sprite {
    s32 x;  // 0x00 - Q8
    s32 y;  // 0x04 - Q8
    u16 id; // 0x08
    u8 unk_0A[2];
    u8 flags; // 0x0C
    u8 unk_0D[0x0B];
    struct sprite_vtable *vtable; // 0x18
    u8 unk_1C[4];
    const struct sprite_bank *anim; // 0x20
    u8 unk_24[4];
    struct sprite_f28 f28; // 0x28
    u8 palette:4;          // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 animIndex; // 0x2D
    u8 unk_2E[2];
    s32 frame; // 0x30
    u8 unk_34[4];
    u8 animDone; // 0x38
    u8 unk_39[3];
    u16 scale; // 0x3C - Q8 affine scale, 0 = not affine (DrawAffineSpritePieces)
    u8 unk_3E[2];
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct sprite) == 0x40);

#endif /* GUARD_LEVEL_MENU_H */
