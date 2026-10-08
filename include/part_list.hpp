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
 * objects.h's `struct collision_queue` stays the C view of the
 * CollisionQueue (struct player embeds it), checked below.
 * cxx_symbols.txt maps the methods to their C names. The HUD that owns the
 * HudParts is hud.hpp's Hud.
 *
 * No `#pragma interface`: g++ emits HudPart's vtable in
 * palette_cycle.cpp (see ctrl.hpp). */

#include "sprite_obj.hpp"

class Crate;

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
    void Add(Crate *neighbor, s32 kind, s32 code, s32 edge, s32 depth, struct vec2 pos, s32 hit,
             struct byte_arg p20,
             struct byte_arg p21); // AddCollisionCandidate
};

COMPILE_TIME_ASSERT(part_list_hpp, sizeof(CollisionQueue) == sizeof(struct collision_queue));
ASSERT_VIEW_FIELD(part_list_hpp, CollisionQueue, collision_queue, count);
ASSERT_VIEW_FIELD(part_list_hpp, CollisionQueue, collision_queue, posCommitted);
ASSERT_VIEW_FIELD(part_list_hpp, CollisionQueue, collision_queue, candidates);

/* Up to three palette colour cycles (gPaletteCycles, `new
 * PaletteCycles`, 0x48 bytes). run_room.cpp adds them (Add) with
 * `targets` = BG palette RAM and `lists` = the palette indices to cycle;
 * every `periods[i]` = 60 / rate frames, Tick rotates the colours of
 * `targets[i]` at the indices `lists[i]` holds by one place (when
 * `gRoomFrameCount % periods[i] == 0`), forwards or backwards by
 * `direction`. Add only appends at `count`, with no wraparound: the
 * caller empties the cycles (Clear) between sets. (docs/rom_map.md's "fx"
 * investigation first read the pair as a particle queue and `rate` as an
 * angle; `__divsi3` is plain division. docs/matching/
 * issue-45-hud-stat-widget-dispatcher.md, "Third pass", settled the
 * fields.) */
class PaletteCycles
{
public:
    u8 active; // 0x00
    u8 unk_01[3];
    s32 fields_e[3]; // 0x04 - only ever cleared (Add); never read
    u16 *targets[3]; // 0x10 - the colours each cycle rotates
    u16 *lists[3];   // 0x1C - the indices into targets[i], counts[i] of them
    s32 periods[3];  // 0x28 - 60 / rate: frames per step
    s32 counts[3];   // 0x34 - `lists[i]`'s length
    s32 count;       // 0x40 - the cycles in use (0-3)
    u8 direction;    // 0x44 - 0/1: which end of `lists[i]` the rotation starts from
    u8 unk_45[3];

    PaletteCycles();  // InitPaletteCycles
    ~PaletteCycles(); // DestroyPaletteCycles
    void Tick();      // TickPaletteCycles
    void Add(u16 *targets, u16 *lists, s32 rate, s32 listCount,
             struct byte_arg direction); // AddPaletteCycle
    void Clear();                        // ClearPaletteCycles
};

COMPILE_TIME_ASSERT(part_list_hpp, sizeof(PaletteCycles) == 0x48);

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
