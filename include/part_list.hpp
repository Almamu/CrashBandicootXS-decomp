#ifndef GUARD_PART_LIST_HPP
#define GUARD_PART_LIST_HPP

/* Part 7c of the C++ conversion (#664, docs/cplusplus.md): the rest of
 * the part list, the player's collision queue, the palette cycles and
 * the HUD part.
 *
 *   PartList        src/objects/part_list.cpp, part_list_cull.cpp,
 *                   part_collide.cpp (the class is in sprite_obj.hpp)
 *   CollisionQueue  src/objects/collision_queue.cpp
 *   PaletteCycles   src/gfx/palette_cycle.cpp
 *   HudPart         src/gfx/palette_cycle.cpp (gHudPartVtable)
 *
 * The C views stay for the C files, each checked against its class
 * below: objects.h's `struct collision_queue` and gfx.h's `struct
 * palette_cycler`. The C prototypes (objects.h, gfx.h) keep the C
 * names; cxx_symbols.txt maps the methods to them. The HUD that owns the
 * HudParts is hud.hpp's Hud.
 *
 * No `#pragma interface`: g++ emits HudPart's vtable in
 * palette_cycle.cpp (see ctrl.hpp). */

#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "objects.h"
#include "gfx.h"
#include "hud.h"
}

/* The player's collision queue (Player's `collisionQueue`,
 * +0x108; objects.h's `struct collision_queue` is its C view): the crate
 * collisions found during the frame, resolved once a frame. */
class CollisionQueue
{
public:
    s32 count;       // 0x00
    u8 posCommitted; // 0x04 - while set, ApplyCrateCollision leaves the player's position alone
    u8 unk_05[3];
    struct collision_candidate candidates[16]; // 0x08

    CollisionQueue();  // ResetCollisionQueue: empties it (Player's constructor)
    ~CollisionQueue(); // DestroyCollisionQueue
    void Resolve();    // ResolveCollisionCandidates
    void Add(struct crate *neighbor, s32 kind, s32 code, s32 edge, s32 depth, struct e08c_pos pos,
             s32 hit, struct byte_arg p20,
             struct byte_arg p21); // AddCollisionCandidate
};

COMPILE_TIME_ASSERT(part_list_hpp, sizeof(CollisionQueue) == sizeof(struct collision_queue));

/* Up to three palette colour cycles (gPaletteCycles; gfx.h's `struct
 * palette_cycler` is its C view, and has the full description): every
 * `periods[i]` frames, the colours of `targets[i]` at the indices
 * `lists[i]` holds rotate by one place, forwards or backwards by
 * `direction`. */
class PaletteCycles
{
public:
    u8 active; // 0x00
    u8 unk_01[3];
    s32 fields_e[3]; // 0x04 - only ever cleared (Add)
    u16 *targets[3]; // 0x10 - the colours each cycle rotates
    u16 *lists[3];   // 0x1C - the indices into targets[i], counts[i] of them
    s32 periods[3];  // 0x28 - 60 / rate: frames per step
    s32 counts[3];   // 0x34
    s32 count;       // 0x40 - the cycles in use (0-3)
    u8 direction;    // 0x44
    u8 unk_45[3];

    PaletteCycles();  // InitPaletteCycles
    ~PaletteCycles(); // DestroyPaletteCycles
    void Tick();      // TickPaletteCycles
    void Add(u16 *targets, u16 *lists, s32 rate, s32 listCount,
             struct byte_arg direction); // AddPaletteCycle
    void Clear();                        // ClearPaletteCycles
};

COMPILE_TIME_ASSERT(part_list_hpp, sizeof(PaletteCycles) == sizeof(struct palette_cycler));

/* One HUD digit or icon (gHudPartVtable, 0x40 bytes): a UiSprite whose `frame` is -1 while it is hidden. */
class HudPart : public UiSprite
{
public:
    HudPart();                 // InitHudPart
    virtual ~HudPart();        // DestroyHudPart
    void Draw(s32 dx, s32 dy); // DrawHudPart
};

COMPILE_TIME_ASSERT(part_list_hpp, sizeof(HudPart) == 0x40);

#endif /* !GUARD_PART_LIST_HPP */
