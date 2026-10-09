#ifndef GUARD_ENTITY_FLAGS_HPP
#define GUARD_ENTITY_FLAGS_HPP

/* The room's entity flags (gEntityFlags) and the unused 1024-bit bitmap
 * whose code sits just before them in the ROM (#762, docs/cplusplus.md).
 * Neither class has a vtable or an inline method. */

extern "C" {
#include "core.h"
#include "level_data.h"
}

/* The entity flags (`gEntityFlags`, 0x408 bytes; src/level/entity_flags.cpp,
 * room_entities.cpp): the room's entity list and two pairs of bitmaps,
 * one bit per entity id. SpawnRoomEntities skips the entities set in
 * `bits0` and copies `bits0`/`bits1` to `bits0Copy`/`bits1Copy` when the
 * room loads; MarkEntityGone and its inline copies (entity_bits.h,
 * Entity::MarkGone) set an entity's bit in `bits0Copy` when it is
 * collected, broken or killed, and SetCheckpointAtPlayer copies the two
 * back. InitLevelState (spawn_markers.cpp) and UpdateGameFrame make it.
 * The names are room_entities.cpp's (`struct lk_self`); dingodile.c
 * called the object `struct entity_flags` (`bits0Copy` was `bitmap`, the
 * list `struct collect_info`), level_state.cpp `struct collision_map`
 * (`seen`), the enemy spawners' text_popup.h a `struct
 * level_record_table **`. The methods keep their C names
 * (cxx_symbols.txt). */
class LevelEntityFlags
{
public:
    const struct level_entity_list *list; // 0x000 - the room's entities and their parameters
    s32 pos;                              // 0x004 - SpawnRoomEntities's position argument >> 8
    u32 bits0[64];                        // 0x008 - committed "gone"
    u32 bits0Copy[64];                    // 0x108 - live "gone"
    u32 bits1[64];                        // 0x208 - committed "activated"
    u32 bits1Copy[64];                    // 0x308 - live "activated"

    LevelEntityFlags();                                           // InitEntityFlags
    ~LevelEntityFlags();                                          // DestroyEntityFlags
    s32 CountCrateEntities(const struct level_entity_list *list); // CountCrateEntities
    void SetGone(s32 n);                                          // SetEntityIdGone (UNUSED)
    s32 IsGone(s32 n);                                            // IsEntityIdGone
    s32 IsActivated(s32 n);                                       // IsEntityIdActivated
    void SetActivated(s32 n);                                     // SetEntityIdActivated
    void MarkActivated(s32 n);                                    // MarkEntityIdActivated
    void SetPos(s32 val);                                         // SetEntityFlagsPos (UNUSED)
    /* SpawnRoomEntities (room_entities.cpp): the room load's entity pass */
    void SpawnRoomEntities(const struct level_entity_list *list,
                           const struct level_link_list *links, s32 posArg, s32 unused);
};

COMPILE_TIME_ASSERT(entity_flags_hpp, sizeof(LevelEntityFlags) == 0x408);

/* A 32-word (1024-bit) bitmap (src/level/entity_bitmap.cpp, ROM
 * 0x08025554-0x080255D4). UNUSED: nothing in the ROM calls its code or
 * makes one; LevelEntityFlags::SetGone uses the same bit idiom. */
class Bitmap
{
public:
    u32 bits[32];

    Bitmap();             // InitBitmap (UNUSED)
    s32 Set(s32 n);       // SetBitmapBit (UNUSED)
    void ClearBit(s32 n); // ClearBitmapBit (UNUSED)
    void Clear();         // ClearBitmap (UNUSED)
};

COMPILE_TIME_ASSERT(entity_flags_hpp, sizeof(Bitmap) == 0x80);

#endif /* !GUARD_ENTITY_FLAGS_HPP */
