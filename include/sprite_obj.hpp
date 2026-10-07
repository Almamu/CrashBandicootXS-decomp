#ifndef GUARD_SPRITE_OBJ_HPP
#define GUARD_SPRITE_OBJ_HPP

/* The sprite objects as the C++ controllers see them (#664,
 * docs/cplusplus.md): the classes behind gEntityVtable and the sprite
 * objects built on it. Their own code is still C (actor.h's `struct
 * actor`, gobj_1a794.h's `struct gobj`), so these are views for the C++
 * objects that use them: the same layout as the C structs. Entity
 * declares all of gEntityVtable's slots, since PeriodicSpawner
 * (include/enemy_ctrl.hpp) derives from it; SpriteObj adds none of its
 * own yet. The rest of the family becomes C++ in a later part of #664.
 *
 * `#pragma interface`: no vtable is emitted for these (see ctrl.hpp). */
#pragma interface

extern "C" {
#include "core.h"
#include "objects.h"
#include "gobj_1a794.h"
#include "entity_bits.h"
}

class Ctrl;

/* struct actor's header, the 0x1C-byte base class of gEntityVtable: its
 * own fields, then the vtable pointer. Its methods are still C, in
 * src/gfx/graphics.c (InitEntity, IsEntityOnScreen, ...); a sprite object
 * overrides them (IsSpriteObjOnScreen, ...). */
class Entity
{
public:
    s32 x;  // 0x00
    s32 y;  // 0x04
    u16 id; // 0x08 - the "gone" bit index; ENTITY_ID_NONE: none
    u8 kind;
    u8 unused_0B;
    union {
        u8 flags; // 0x0C
        struct {
            u8 gone:1;
            u8 unk_1:1;
            u8 visible:1;
            u8 bit3:1;   // the effect part was touched; SetTargetAnim clears it
            u8 active:1; // always active (updated off screen too)
            u8 unk_5:1;
            u8 vulnerable:1; // the player's attacks hit it
            u8 unk_7:1;
            u8 unk_0D_0:2; // 0x0D
            u8 blink:1;    // hidden this frame (a blinking part)
            u8 unk_0D_3:5;
        } b; // (ARM structs are 4-byte sized: the union spans 0x0C-0x0F)
    } f;
    s16 halfW; // 0x10
    s16 halfH; // 0x12
    u8 rawW;   // 0x14
    u8 rawH;   // 0x15
    u8 unused_16[2];
    // 0x18: the vtable pointer

    Entity();                                   // InitEntity
    virtual void CheckPlayerContact();          // 1
    virtual void GetBounds();                   // 2
    virtual void Update();                      // 3
    virtual void Draw();                        // 4
    virtual u8 IsOnScreen();                    // 5
    virtual s32 OverlapsRect();                 // 6
    virtual u8 IsNearCamera();                  // 7
    virtual s32 IsInsideRect(struct aabb *box); // 8
    virtual s32 GetClassId();                   // 9

    /* 10, DestroyEntity: inline, as the subclasses' destructors have it
     * (DestroyPeriodicSpawner). g++ adds the `delete` when bit 0 of the
     * flags is set. */
    virtual ~Entity()
    {
    }

    /* MarkEntityGone inlined: the `gone` flag, and unless the id is
     * ENTITY_ID_NONE, its bit in the bitmap. */
    void MarkGone()
    {
        ENTITY_MARK_GONE(f.b.gone, id);
    }
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(Entity) == 0x1C);

/* struct gobj (gobj_1a794.h): an animated sprite object. The fields
 * after the Entity header, with struct gobj's names. */
class SpriteObj : public Entity
{
public:
    void *lastHitbox;        // 0x1C
    struct anim_table *anim; // 0x20
    u8 dir;                  // 0x24
    u8 screenSpace;          // 0x25
    u8 unk_26[2];
    union {
        u8 mirror; // 0x28 - bit 4: X mirrored, bit 5: Y mirrored
        struct MirrorBits {
            u8 gfxMode:2;
            u8 unk_2:2;
            s32 flipX:1; // signed: test it with `< 0` (`lsl #27`, a sign test)
            u32 flipY:1;
            u8 priority:2;
        } __attribute__((packed)) mirrorBits;
    } __attribute__((packed)); // one byte, not the 4 of an ARM struct
    u8 palette:4;              // 0x29 - low nibble: the OBJ palette slot
    u8 unk_29_4:4;
    u8 unk_2A[2];
    u8 animating; // 0x2C
    u8 tag;       // 0x2D - the animation (SetTargetAnim)
    u8 unk_2E[2];
    s32 frame;     // 0x30
    s32 stepTimer; // 0x34
    u8 animDone;   // 0x38 - set once a non-looping animation ends
    u8 unk_39[3];
    u16 affine; // 0x3C
    u8 unk_3E[2];
    s32 unk_40;              // 0x40
    Ctrl *mover;             // 0x44 - its controller
    struct speed_ramp rampX; // 0x48 - speedX's ramp
    struct speed_ramp rampY; // 0x54 - speedY's ramp
    s32 speedX;              // 0x60
    s32 speedY;              // 0x64
    u8 hitAxes;              // 0x68
    u8 probeTries;           // 0x69
    u8 unk_6A[2];
    s32 prevX;   // 0x6C
    s32 prevY;   // 0x70
    s32 hitMask; // 0x74
    s32 type;    // 0x78
    u8 unk_7C[4];
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(SpriteObj) == sizeof(struct gobj));

#endif /* !GUARD_SPRITE_OBJ_HPP */
