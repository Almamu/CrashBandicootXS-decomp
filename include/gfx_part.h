#ifndef GUARD_GFX_PART_H
#define GUARD_GFX_PART_H

/* A CreateMovingSprite/CreateSpriteObj-built on-screen "part" object and its
 * animation bank, as driven by the per-frame helpers
 * ResetSpriteFrameTimer/ResetSpriteFrameIndex/SetSpriteAnimDone/GetSpriteAnimPaletteSlot. First written for
 * src/bosses/cortex.c (issue #23); also used by the
 * "trigger effect type N" spawners in
 * src/level/spawn_gems.c. A C view of include/sprite_obj.hpp's
 * MovingSprite, the definition (sprite_obj.hpp checks that this view
 * fits in it), for the C files; also described as `struct gobj`
 * (include/gobj_1a794.h), `struct box_part` (include/box_part.h) and
 * `struct settings_icon_actor` (include/pause_menu.h). */

struct anim_record {
    u8 unk_00[0x16];
    u8 frameCount; // 0x16
    u8 unk_17[5];
};

struct anim_bank {
    struct anim_record *records;
};

struct gfx_vec {
    s32 x;
    s32 y;
};

struct gfx_part {
    struct gfx_vec pos; // 0x00
    u16 id;             // 0x08
    u8 kind;            // 0x0A - object kind passed to the hit handlers (box_part.h `kind`);
                        //        the effect spawners store their type here
    u8 unk_0B;
    u8 gone:1; // 0x0C bit 0
    u8 flags_1:1;
    u8 hidden:1; // 0x0C bit 2
    u8 flags_3:1;
    u8 active:1; // 0x0C bit 4
    u8 flags_5:3;
    u8 unk_0D[0x13];
    struct anim_bank *bank; // 0x20
    u8 unk_24[4];
    u8 gfxMode:2; // 0x28 - OBJ mode (box_part.gfxMode, Get/SetSpriteGfxMode); 1: semi-transparent
    u8 unk_28_2:2;
    u8 flipX:1;
    u8 unk_28_5:3;
    u8 frameNibble:4; // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[2];
    u8 animating; // 0x2C - nonzero while the keyframe timer runs
    u8 tag;       // 0x2D
    u8 unk_2E[2];
    s32 frame;     // 0x30 - step within the animation
    s32 stepTimer; // 0x34 - reset to the animation's `duration` (GetSpriteFrame)
    u8 animDone;   // 0x38
    u8 unk_39[0xB];
    void *ctrl; // 0x44
    u8 unk_48[0x24];
    s32 prevX; // 0x6C - previous position (Q8), cached by ApplySpriteVelocity
    s32 prevY; // 0x70
};

/* The whole flags byte at +0x0C, for the spots that update it as one
 * byte through register pins (see UpdateUnusedOneShotAnimCtrl). */
#define PART_FLAGS(p) (*((u8 *)(p) + 0xC))

/* Bit `7 - (shift - 24)` of the flags byte at +0x28 (the OBJ mode, mirror
 * and 8bpp bits), tested as a sign test (`lsl #shift; cmp #0; bge`), as
 * the sprite piece loops do; a 1-bit field test compiles to `movs #0x20;
 * ands` instead. */
#define PART_FLAG_SET(part, shift) ((s32)(*((u8 *)(part) + 0x28) << (shift)) < 0)

#endif /* GUARD_GFX_PART_H */
