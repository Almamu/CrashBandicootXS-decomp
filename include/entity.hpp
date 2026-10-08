#ifndef GUARD_ENTITY_HPP
#define GUARD_ENTITY_HPP

/* The entity base class (#664, docs/cplusplus.md; src/gfx/graphics.cpp,
 * gEntityVtable). include/sprite_obj.hpp builds the sprite classes on it.
 *
 * This header has no `#pragma interface`, unlike the other class
 * headers: its destructor is inline (the subclasses' destructors inline
 * it, as DestroySpriteObj and DestroyPeriodicSpawner show), and the ROM
 * still has an out-of-line copy, DestroyEntity, the last function of
 * graphics.cpp. That is g++ 2.9's rule for a class with no `#pragma
 * interface`: the file that defines its first non-inline virtual method
 * (CheckPlayerContact, in graphics.cpp) gets the vtable and, at its end,
 * the out-of-line copies of the inline virtual ones. The vtable it emits
 * is a weak symbol in a `.gnu.linkonce.d` section, which ldscript.txt
 * places at gEntityVtable's ROM address (docs/cplusplus.md, "Emitting
 * the vtables"). */

extern "C" {
#include "core.h"
#include "math_util.h"
#include "aabb.h"
#include "hitbox_quad.h"
#include "actor.h"
#include "entity_bits.h"
}

/* union EntityFlags, the flags at +0x0C, is actor.h's (struct player, the
 * C view of the player, has it too). */

/* The entity: a position, the spawn's id, a kind, the flags and a size;
 * then the vtable pointer. Each
 * virtual method's slot is its declaration order, from slot 1. */
class Entity
{
public:
    s32 x;   // 0x00 - Q8
    s32 y;   // 0x04 - Q8
    u16 id;  // 0x08 - the "gone" bit index; ENTITY_ID_NONE: none
    u8 kind; // 0x0A - the object kind sent to the player's hit method on contact
    u8 unused_0B;
    union EntityFlags f; // 0x0C
    /* 0x10-0x17: a struct hitbox_quad, the box GetBounds returns. */
    s16 halfW; // 0x10 - -rawW / 2 (SetSize, Reset)
    s16 halfH; // 0x12 - -rawH / 2
    u8 rawW;   // 0x14
    u8 rawH;   // 0x15
    u8 unused_16[2];
    // 0x18: the vtable pointer

    Entity();                                      // InitEntity: inline in graphics.cpp (see there)
    virtual s32 CheckPlayerContact();              // 1
    virtual const struct hitbox_quad *GetBounds(); // 2 - the hitbox record
    virtual void Update();                         // 3
    virtual void Draw();                           // 4
    virtual u8 IsOnScreen();                       // 5
    virtual s32 OverlapsRect(struct aabb *region); // 6
    virtual u8 IsNearCamera();                     // 7
    virtual s32 IsInsideRect(struct aabb *box);    // 8
    virtual s32 GetClassId();                      // 9
    static Entity *Create(u16 id, u16 x, u16 y, u16 unused); // CreateEntity
    void SetSize(s32 w, s32 h);
    void Reset();

    /* The inline methods. graphics.cpp has their out-of-line copies at
     * its end, in the reverse of this order (see the top of this file),
     * for the C callers: DestroyEntity is the last. A method calls one
     * declared after it out of line (SetPixelPosVec, SetPosVec), as in
     * the ROM. */

    /* 10, DestroyEntity. g++ adds the `delete` when bit 0 of the flags is
     * set. */
    virtual ~Entity()
    {
    }

    u16 GetId()
    {
        return id;
    }

    u8 GetKind()
    {
        return kind;
    }

    void SetKind(u8 value)
    {
        kind = value;
    }

    void SetPosVec(s32 *pos)
    {
        SetPos(pos[0], pos[1]);
    }

    void SetPos(s32 px, s32 py)
    {
        x = px;
        y = py;
    }

    void SetPixelPosVec(s32 *pos)
    {
        SetPixelPos(pos[0], pos[1]);
    }

    void SetPixelPos(s32 px, s32 py)
    {
        x = INT_TO_Q8(px);
        y = INT_TO_Q8(py);
    }

    s32 GetX()
    {
        return x;
    }

    s32 GetY()
    {
        return y;
    }

    s32 GetPixelX()
    {
        return Q8_TO_INT(x);
    }

    s32 GetPixelY()
    {
        return Q8_TO_INT(y);
    }

    void SetFlag1()
    {
        f.b.unk_1 = 1;
    }

    void ClearFlag1()
    {
        f.b.unk_1 = 0;
    }

    u8 GetFlag1()
    {
        return (f.flags >> 1) & 1;
    }

    void EnableContact()
    {
        f.b.visible = 1;
    }

    void DisableContact()
    {
        f.b.visible = 0;
    }

    u8 IsContactEnabled()
    {
        return (f.flags >> 2) & 1;
    }

    /* MarkEntityGone: the `gone` flag, and unless the id is
     * ENTITY_ID_NONE, its bit in the bitmap (gEntityFlags->bits0Copy).
     * The same code as entity_bits.h's ENTITY_MARK_GONE everywhere, but
     * with the bitmap indexed rather than addressed by byte offset: every
     * object compiles the same either way, and this one also gives
     * cortex.cpp's gem and shot updates the ROM's register allocation
     * (part 7i). */
    void MarkGone()
    {
        f.b.gone = 1;
        if (id != ENTITY_ID_NONE) {
            s32 i = id;
            struct entity_flags *flags = gEntityFlags;
            s32 word = i / 32;

            flags->bits0Copy[word] |= 1 << (i - word * 32);
        }
    }

    void ClearGone()
    {
        f.b.gone = 0;
    }

    u8 IsGone()
    {
        return f.flags & 1;
    }

    u8 IsTouched()
    {
        return (f.flags >> 3) & 1;
    }

    void SetTouched()
    {
        f.b.bit3 = 1;
    }

    void ClearTouched()
    {
        f.b.bit3 = 0;
    }

    u8 IsAlwaysActive()
    {
        return (f.flags >> 4) & 1;
    }

    void SetAlwaysActive()
    {
        f.b.active = 1;
    }

    void ClearAlwaysActive()
    {
        f.b.active = 0;
    }
};

COMPILE_TIME_ASSERT(entity_hpp, sizeof(Entity) == 0x1C);

/* graphics.cpp's out-of-line copies of SetPixelPos and SetPos (above),
 * under their C names (cxx_symbols.txt). The ROM calls them out of line
 * from other files (the level select's sprites, the HUD, the crates, the
 * rooms...), where a call to the inline method would be inlined, so those
 * files call these. */
extern "C" void SetEntityPixelPos(Entity *self, s32 x, s32 y);
extern "C" void SetEntityPos(Entity *self, s32 x, s32 y);

#endif /* !GUARD_ENTITY_HPP */
