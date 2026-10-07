#ifndef GUARD_GOBJ_1A794_H
#define GUARD_GOBJ_1A794_H

#include "mover_new.h"
#include <libgcc.h>
#include "util.h"
#include "crates.h"
#include "player.h"
#include "bosses.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "match.h"

/* The C views of the platforms (GitHub issue #25, ROM
 * 0x0801A794-0x0801B85C), shared by the C files that still use them (the
 * player, the crates, cortex.c, the vtable data) and by
 * src/bosses/dingodile_create.cpp. The platforms are C++ now:
 * include/platform.hpp's `Platform` and `PlatformMover`
 * (src/objects/platform*.cpp) are the definitions, and check their sizes
 * against these two structs.
 *
 * - `struct gobj`, a 0x80-byte level object: CreatePlatform's
 *   (gPlatformVtable) and, with the player's fields after it, the player
 *   (gPlayer, player.h's `struct player`). It is the C view of
 *   include/sprite_obj.hpp's GroundSprite (the classes are the
 *   definitions: Entity, Sprite, MovingSprite, GroundSprite; sprite_obj.hpp
 *   checks this struct's size against GroundSprite's) and of
 *   platform.hpp's Platform. Its `type` (+0x78) is the platform type;
 *   types 1/5/6/7 get a `struct mover` attached at +0x44.
 * - `struct mover`, a 0x38-byte controller (gPlatformMoverVtable; the C
 *   view of PlatformMover) that oscillates its owner back and forth over
 *   `rangeX`/`rangeY` pixels using the 12-byte velocity records of
 *   gPlatformMoverMotionRecords, and drags the player along while it is
 *   `active` (MovePlayerWithPlatform). cortex.c's Neo Cortex platform mover
 *   is built on it. */

struct vec_pair {
    u32 a;
    u32 b;
};

struct anim_rec {
    u8 unk_00[4];
    s16 offX; // 0x04 - a struct hitbox_quad (gfx.h)
    s16 offY; // 0x06
    u8 padX;  // 0x08
    u8 padY;  // 0x09
    u8 unk_0A[0xA];
    u8 paletteId; // 0x14 - GetPaletteSlot(gPaletteCache, paletteId) gives the OBJ palette slot
    u8 unk_15;
    u8 frames; // 0x16
    u8 unk_17[5];
};

struct anim_table {
    struct anim_rec *records;
};

struct gobj_vtable {
    u8 unk_00[0x10];
    // 0x10 - slot 2, returns the current frame's hitbox record
    // (GetSpriteObjHitbox; UpdateGroundSprite, ProbeGroundSpriteTerrain)
    struct actor_method m10;
    u8 unk_18[0x20];
    struct actor_method m38; // 0x38
    u8 unk_40[0x20];
    struct actor_method m60;          // 0x60
    struct actor_method m68;          // 0x68
    struct actor_method checkContact; // 0x70 - CheckPlayerContact (CollideMovingSprite)
};

struct mover;

struct gobj {
    s32 x;  // 0x00
    s32 y;  // 0x04
    u16 id; // 0x08
    u8 unk_0A[2];
    u8 flags;  // 0x0C
    u8 flags2; // 0x0D
    u8 unk_0E[0xA];
    struct gobj_vtable *vtable; // 0x18
    void *lastHitbox;           // 0x1C - the last m10 record (AnchorGroundSpriteHitbox)
    struct anim_table *anim;    // 0x20
    u8 dir;                     // 0x24
    // 0x25 - 1: x/y are screen coordinates (DrawSpriteAt skips WorldToScreen;
    //        always counts as on screen). GetSpriteScreenSpace/SetSpriteScreenSpace
    u8 screenSpace;
    u8 unk_26[2];
    u8 mirror; // 0x28 - bit 4: X mirrored, bit 5: Y mirrored
    u8 slot;   // 0x29 - low nibble: palette/tile slot
    u8 unk_2A[2];
    u8 animating; // 0x2C - nonzero while the keyframe timer runs (box_part.h)
    u8 tag;       // 0x2D
    u8 unk_2E[2];
    s32 frame;     // 0x30
    s32 stepTimer; // 0x34 - ticks spent on the current step (ResetSpriteFrameTimer)
    u8 animDone;   // 0x38 - set once a non-looping animation ends (SetSpriteAnimDone)
    u8 unk_39[3];
    u16 affine; // 0x3C - box_part.h's `affine` (ResetSpriteObj clears it)
    u8 unk_3E[2];
    s32 unk_40;              // 0x40 - ResetMovingSprite clears it; nothing reads it
    struct mover *mover;     // 0x44
    struct speed_ramp rampX; // 0x48 - speedX's ramp (ApplySpriteVelocity)
    struct speed_ramp rampY; // 0x54 - speedY's ramp
    s32 speedX;              // 0x60
    s32 speedY;              // 0x64
    // 0x68 - collision axes the terrain probe resolved (8: Y, standing; 4: X)
    u8 hitAxes;
    u8 probeTries; // 0x69
    u8 unk_6A[2];
    s32 prevX;   // 0x6C - previous position (Q8), cached by ApplySpriteVelocity
    s32 prevY;   // 0x70
    s32 hitMask; // 0x74 - probe axes hit this frame (OR-accumulated, see box_part.h)
    s32 type;    // 0x78
    u8 unk_7C[4];
};

struct mover_vtable {
    u8 unk_00[8];
    struct actor_method m08; // 0x08
    struct actor_method m10; // 0x10
    struct actor_method m18; // 0x18
    u8 unk_20[0x28];
    struct actor_method destroy; // 0x48 - the destructor (DestroyMovingSprite passes 3)
    u8 unk_50[0x10];
    struct actor_method m60; // 0x60
};

struct mover {
    u8 unk_00[4];
    struct {
        struct vec_pair *entries;
    } *set; // 0x04
    u8 unk_08[4];
    struct mover_vtable *vtable; // 0x0C
    s32 kind;                    // 0x10
    s32 timer;                   // 0x14
    s32 distX;                   // 0x18
    s32 distY;                   // 0x1C
    s32 lastX;                   // 0x20
    s32 lastY;                   // 0x24
    s32 rangeX;                  // 0x28
    s32 rangeY;                  // 0x2C
    u8 dirX;                     // 0x30
    u8 dirY;                     // 0x31
    u8 active;                   // 0x32
    u8 unk_33;
    u32 time; // 0x34
};

struct pos2 {
    s32 x;
    s32 y;
};

struct spawn_rec {
    u8 flags; // 0x00
    u8 unk_01[3];
    u32 type;  // 0x04
    s32 distX; // 0x08
    s32 distY; // 0x0C
    s16 dirX;  // 0x10
    s16 dirY;  // 0x12
    s16 flag;  // 0x14
};

extern s32 _call_via_r1(void *self, void *fn);
extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern s32 _call_via_r3(void *self, void *arg1, s32 arg2, void *fn);
extern void _call_via_r4(void *self, s32 a, s32 b, s32 c);

#endif /* GUARD_GOBJ_1A794_H */
