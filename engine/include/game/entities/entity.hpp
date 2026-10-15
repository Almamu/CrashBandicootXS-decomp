#ifndef GUARD_ENTITY_HPP
#define GUARD_ENTITY_HPP

/* The entity base class (#664, docs/cplusplus.md; src/game/entities/entity.cpp,
 * gEntityVtable). include/game/entities/sprite_obj.hpp builds the sprite classes on it.
 *
 * This header has no `#pragma interface`, unlike the other class
 * headers: its destructor is inline (the subclasses' destructors inline
 * it, as DestroySpriteObj and DestroyPeriodicSpawner show), and the ROM
 * still has an out-of-line copy, DestroyEntity, the last function of
 * entity.cpp. That is g++ 2.9's rule for a class with no `#pragma
 * interface`: the file that defines its first non-inline virtual method
 * (CheckPlayerContact, in entity.cpp) gets the vtable and, at its end,
 * the out-of-line copies of the inline virtual ones. The vtable it emits
 * is a weak symbol in a `.gnu.linkonce.d` section, which games/xs/ldscript.txt
 * places at gEntityVtable's ROM address (docs/cplusplus.md, "Emitting
 * the vtables").
 *
 * The class is in the namespace Game::Entities (#1033, docs/cplusplus.md
 * "Namespaces"), with the sprite classes: `_vt.Q34Game8Entities6Entity`.
 * The C-linkage SetEntityPixelPos and SetEntityPos (below) stay global. */

#include "game/level/entity_flags.hpp"

extern "C" {
#include "core.h"
#include "engine/util/math_util.h"
#include "engine/util/aabb.h"
#include "engine/gfx/hitbox_quad.h"
#include "engine/actor/actor.h"
#include "game/level/entity_bits.h"
}

/* union EntityFlags, the flags at +0x0C, is actor.h's. */

namespace Game
{
namespace Entities
{

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
#ifdef GAME_N2
    /* N-Tranced's entity has three more words here, which Reset sets to 1,
     * 0 and 0 (its ResetEntity, 0x0800805C); what they are isn't known
     * yet. So the rest is 12 bytes later: the flags at 0x18, the box at
     * 0x1C (its sizes u16, hitbox_quad.h) and the vtable pointer at 0x24. */
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
#endif
    union EntityFlags f; // 0x0C
    /* 0x10-0x17: a struct hitbox_quad, the box GetBounds returns. */
    s16 halfW; // 0x10 - -rawW / 2 (SetSize, Reset)
    s16 halfH; // 0x12 - -rawH / 2
#ifdef GAME_N2
    u16 rawW;
    u16 rawH;
#else
    u8 rawW; // 0x14
    u8 rawH; // 0x15
    u8 unused_16[2];
#endif
    // 0x18: the vtable pointer

    // OverlapsRect (6) and IsInsideRect (8) are yes/no tests that stay s32:
    // their callers' `(u8)` is the ROM's truncation, but as bools, Entity's,
    // Sprite's and Crate's versions change (#855).
    Entity();                         // InitEntity: inline in entity.cpp (see there)
    virtual s32 CheckPlayerContact(); // 1
#ifdef GAME_N2
    /* N-Tranced's slot 2, empty in Entity (0x08007F94): the slots below
     * are one later there (GetBounds 3, ..., the destructor 11) */
    virtual void UnkVirtual2();
#endif
    virtual const struct hitbox_quad *GetBounds();           // 2 - the hitbox record
    virtual void Update();                                   // 3
    virtual void Draw();                                     // 4
    virtual u8 IsOnScreen();                                 // 5
    virtual s32 OverlapsRect(struct aabb *region);           // 6
    virtual u8 IsNearCamera();                               // 7
    virtual s32 IsInsideRect(struct aabb *box);              // 8
    virtual s32 GetClassId();                                // 9
    static Entity *Create(u16 id, u16 x, u16 y, u16 unused); // CreateEntity
    void SetSize(s32 w, s32 h);
    void Reset();

    /* The inline methods. entity.cpp has their out-of-line copies at
     * its end, in the reverse of this order (see the top of this file),
     * for the C callers: DestroyEntity is the last. A method calls one
     * declared after it out of line (SetPixelPosVec, SetPosVec), as in
     * the ROM. */

    /* 10, DestroyEntity. g++ adds the `delete` when bit 0 of the flags is
     * set. */
    virtual ~Entity()
    {
    }
#ifdef GAME_N2
    /* N-Tranced's slot 12, empty in Entity (0x08007F4C) */
    virtual void UnkVirtual12();
#endif

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
        f.b.attacks = 1;
    }

    void ClearFlag1()
    {
        f.b.attacks = 0;
    }

    u8 GetFlag1()
    {
        return (f.flags >> ENTITY_BIT_ATTACKS) & 1;
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
        return (f.flags >> ENTITY_BIT_CONTACT) & 1;
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
            Game::Level::LevelEntityFlags *flags = gEntityFlags;
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
        return f.flags & ENTITY_FLAG_GONE;
    }

    u8 IsTouched()
    {
        return (f.flags >> ENTITY_BIT_TOUCHED) & 1;
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
        return (f.flags >> ENTITY_BIT_ALWAYS_ACTIVE) & 1;
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

LAYOUT_ASSERT(entity_hpp, sizeof(Entity) == 0x1C);
N2_LAYOUT_ASSERT(entity_hpp, sizeof(Entity) == 0x28);

} // namespace Entities
} // namespace Game

/* entity.cpp's out-of-line copies of SetPixelPos and SetPos (above),
 * under their C names (cxx_symbols.txt). The ROM calls them out of line
 * from other files (the level select's sprites, the HUD, the crates, the
 * rooms...), where a call to the inline method would be inlined, so those
 * files call these. */
extern "C" void SetEntityPixelPos(Game::Entities::Entity *self, s32 x, s32 y);
extern "C" void SetEntityPos(Game::Entities::Entity *self, s32 x, s32 y);

#endif /* !GUARD_ENTITY_HPP */
