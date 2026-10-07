#ifndef GUARD_BOX_PART_H
#define GUARD_BOX_PART_H

#include "aabb.h"
#include "gfx.h"

/* The collision/animation view of a `part` object (the same object as
 * include/gfx_part.h's `struct gfx_part` / include/gobj_1a794.h's
 * `struct gobj` / include/actor.h's `struct actor`), as read by the
 * early actor/collision core in src/objects/sprite.c, sprite_anim.c,
 * sprite_obj.c, part_collide.c and player_anim_room.c. Only the fields
 * those functions touch are named.
 *
 * The mirror bits at 0x28 are `u32` bitfields on purpose: that is what
 * makes the compiler test them with `lsl #27`/`lsl #26` + a sign test,
 * as the ROM does (a `u8` container gives `movs #0x10; ands`). Being
 * bitfields, the bytes at 0x28/0x29 have no address: the setters that
 * update them as a whole byte (SetSpriteFlipX, SetSpritePalette, ...)
 * go through `(u8 *)part + 0x28`, and ResetSpriteObj clears both through
 * gobj_1a794.h's byte-wide `mirror`/`slot`. */

/* One KEYFRAME_SIZE-byte keyframe record: an animation of the part's
 * sprite bank (the same record as sprite_bank.h's `struct sprite_anim`).
 * The {offX, offY, w, h} collision box (struct hitbox_quad, gfx.h) is
 * `box[0]` (+0x4) or `box[1]` (+0xc) depending on which table the part
 * uses. */
struct keyframe {
    const u16 *seq;            // 0x00 - frame indices into the bank's frames
    struct hitbox_quad box[2]; // 0x04, 0x0C
    u8 paletteId;              // 0x14 - GetPaletteSlot record id
    u8 duration;               // 0x15 - ticks per step
    u8 steps;                  // 0x16 - steps before the keyframe ends
    u8 flags;                  // 0x17 - bit 1: loops (animDone is not set)
    u8 unk_18[4];
};

#define KEYFRAME_SIZE 0x1c

/* A gcc 2.x method-table record: `this` adjustment plus code pointer.
 * A part's method table (`vtable`) is indexed by byte offset - see
 * PART_METHOD. */
struct part_method {
    s16 thisOffset;
    u8 unk_02[2];
    void *fn;
};

#define PART_METHOD(obj, off) ((struct part_method *)((obj)->vtable + (off)))

struct box_part {
    s32 x;   // 0x00 - Q8 fixed-point
    s32 y;   // 0x04 - Q8 fixed-point
    u16 id;  // 0x08 - bit index in the "gone" bitmap, 0xFFFF for none
    u8 kind; // 0x0A
    u8 unk_0B;
    u8 flags;  // 0x0C - bit 0 gone, bit 2 visible, bit 3 touched (PART_FLAG_*)
    u8 flags2; // 0x0D - bit 3: solid (pushes the player out)
    u8 unk_0E[0xa];
    u8 *vtable; // 0x18 - method table, see PART_METHOD
    u8 unk_1C[4];
    struct keyframe **keyframes; // 0x20
    u8 moveAxes;                 // 0x24 - bits 0-1: X probe mode, bits 2-3: Y probe mode
    // 0x25 - 1: x/y are screen coordinates (DrawSpriteAt skips WorldToScreen;
    //        always counts as on screen)
    u8 screenSpace;
    u8 unk_26[2];
    u32 gfxMode:2;   // 0x28 - bits 0-1 (Get/SetSpriteGfxMode)
    u32 mosaic:1;    //        bit 2
    u32 colorMode:1; //        bit 3
    u32 mirrorX:1;   //        bit 4
    u32 mirrorY:1;   //        bit 5
    u32 priority:2;  //        bits 6-7 - OBJ priority (Get/SetSpritePriority)
    u32 palette:4;   // 0x29 - low nibble: OBJ palette slot
    u32 unk_29_4:4;
    u8 unk_2A[2];
    u8 animating; // 0x2C - nonzero while the keyframe timer runs
    u8 frame;     // 0x2D - current keyframe index
    u8 unk_2E[2];
    s32 tick;    // 0x30 - per-keyframe step counter
    s32 timer;   // 0x34 - ticks spent on the current step
    u8 animDone; // 0x38
    u8 unk_39[3];
    u16 affine; // 0x3C - nonzero: DrawSpriteWithOffset draws through DrawAffineSpritePieces
    u8 unk_3E[0xf];
    u8 physMode; // 0x4D - low 7 bits 1: skipped by the physics AABB tests
    u8 state;    // 0x4E - PlayerAnimWouldTouchCrate skips 5 and 0xA
    u8 unk_4F[0x15];
    s32 speedY;    // 0x64 - struct gobj.speedY (> 0: falling, so a touch stomps)
    u8 hitAxes;    // 0x68 - collision axes ProbeGroundSpriteTerrain resolved (bit 3: Y)
    u8 probeTries; // 0x69 - ProbeHitboxEdgeTerrain's retry counter
    u8 unk_6A[0xa];
    u32 hitMask; // 0x74 - probe axes ProbeGroundSpriteTerrain hit this call
};

/* box_part.flags (the byte at 0x0C every part view shares; crate.h's
 * struct phys_flag_bits is the bitfield view of the same byte). */
#define PART_FLAG_GONE    1 // removed (MarkEntityGone); the part and crate lists drop it
#define PART_FLAG_TOUCHED 8 // hit by another object (CollidePartWithObject); IsEntityTouched

/* box_part.moveAxes (player.h's `dir`, the same byte): the direction
 * bits, 1 right, 2 left, 4 up, 8 down. The X pair is also the X probe
 * mode, the Y pair the Y probe mode (ProbeGroundSpriteTerrain). */
#define PART_DIR_X_MASK 3
#define PART_DIR_Y_MASK 0xc

/* The part list the per-frame collision passes walk (UpdatePartList
 * compacts `items` and fills `visible`; CollidePartList walks `visible`). */
struct part_list {
    s32 capacity;              // 0x00
    s32 count;                 // 0x04
    s32 visibleCount;          // 0x08
    struct box_part **items;   // 0x0C
    struct box_part **visible; // 0x10
};

#endif /* GUARD_BOX_PART_H */
