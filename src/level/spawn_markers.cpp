#include "spawners.hpp"
#include "pickups.hpp"
#include "player.hpp"
#include "part_list.hpp"
#include "font.hpp"
#include "level_state.hpp"
#include "audio.hpp"

extern "C" {
#include "util.h"
#include "text.h"
#include "gfx.h"
#include "memory.h"
#include "system.h"
#include "level.h"
#include "globals.h"
}

/* The player positions and start markers (entity types 0x01-0x05), the
 * entity spawner's constructor and destructor calls and InitLevelState
 * (#664, include/spawners.hpp), ROM 0x08022188-0x08022354; split from
 * spawn_pickups.cpp (#770). Built with old_agbcp. */

/* Entity type 0x05: does nothing. Types 0x01/0x03 (SpawnPlayerPosition,
 * SpawnUnderwaterPlayerPosition) only move the player to the entity, and
 * the three pair up with the start markers of the room kinds by slot (0x00
 * normal, 0x02 underwater, 0x04 hover; the ROM holds these five functions
 * in descending type order); for the hover room this one is empty. No
 * level places types 0x01, 0x03 or 0x05 (docs/levels.md). */
void SpawnHoverPlayerPosition(void)
{
}

/* Entity type 0x04: the player start of the kind-2 (hover vehicle) room
 * 16, the only room that places it. Forwards to `SpawnStartMarker`
 * (type 0x00, the start of the kind-0 rooms). */
void SpawnHoverStartMarker(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    SpawnStartMarker(arg0, arg1, arg2, arg3);
}

/* Entity type 0x03: moves the player to the entity's position (Q8),
 * without a start marker; the underwater counterpart of type 0x01 (see
 * SpawnHoverPlayerPosition). Ignores `arg0`/`arg3` entirely, matching the
 * ROM (a leaf function, no `push`/`pop` at all). */
void SpawnUnderwaterPlayerPosition(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    gPlayer->x = INT_TO_Q8((s32)arg1);
    gPlayer->y = INT_TO_Q8((s32)arg2);
}

/* Entity type 0x02: the player start of the kind-1 (underwater) rooms;
 * every kind-1 room places one and no other room does. Same trampoline
 * as `SpawnHoverStartMarker` above. */
void SpawnUnderwaterStartMarker(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    SpawnStartMarker(arg0, arg1, arg2, arg3);
}

/* Entity type 0x01: the same as `SpawnUnderwaterPlayerPosition` above, for
 * the normal rooms (see SpawnHoverPlayerPosition). */
void SpawnPlayerPosition(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    gPlayer->x = INT_TO_Q8((s32)arg1);
    gPlayer->y = INT_TO_Q8((s32)arg2);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the spawn
 * table). The spawners in this stretch of ROM run in descending type order
 * (SpawnWumpa 0x06, SpawnHoverPlayerPosition 0x05 ... SpawnPlayerPosition
 * 0x01), so this empty stub sits where type 0x00's spawner would; the
 * table's type 0x00 is SpawnStartMarker (spawn_start_marker.cpp, 0x0801E990)
 * instead. */
void SpawnStartMarkerStub(void)
{
}

/* `delete gEntitySpawner`: the null test, then the destructor
 * (DestroyEntitySpawnerObj) with 3. */
void DestroyEntitySpawner(void)
{
    delete gEntitySpawner;
}

/* gEntitySpawner, over the 92 spawn functions of gEntitySpawnFuncs. */
void CreateEntitySpawner(void)
{
    gEntitySpawner = new EntitySpawner;
    gEntitySpawner->SetTable(gEntitySpawnFuncs, ENTITY_COUNT);
}

/* The level state's constructor (GetLevelState calls it once, at the top
 * of the game loop): builds the hot IWRAM globals - the audio context
 * (0x2094 bytes of IWRAM), the sprite renderer, the sprite bank set
 * (pointed at gSpriteBankTable), the palette cache (over the table's
 * palettes), the two fonts, the VRAM DMA queue, the OAM buffer, the OBJ
 * VRAM cursor, the key input, the entity flags and the palette cycles -
 * clears the display control and the level state's unused flags.
 *
 * It is LevelState's constructor (include/level_state.hpp). The globals
 * keep their C types (the C files use them), so the new objects of the
 * C++ classes are stored through their C views (the fonts are `Font *`s
 * to C++, text.h). The audio context is `new AudioContext` (its own
 * operator new, IwramAlloc: include/audio.hpp). */
LevelState::LevelState()
{
    {
        AudioContext **audio = &gAudioContext;

        *audio = new AudioContext;
        EnableMusicVCountIrq();
        (*audio)->SetSfxVolume(0xc0);
        (*audio)->SetMusicVolume(0xc0);
    }
    gSpriteRenderer = new SpriteRenderer;
    {
        SpriteBankSet *banks;

        gSpriteBankSet = (banks = new SpriteBankSet);
        banks->table = &gSpriteBankTable;
    }
    {
        PaletteCache *cache;

        gPaletteCache = (cache = new PaletteCache);
        cache->SetSource(gSpriteBankTable.paletteCount, gSpriteBankTable.palettes);
    }
    {
        Font **font = &gSmallFont;

        *font = new SmallFont;
        font = &gLargeFont;
        *font = new LargeFont;
    }
    AllocVramDmaQueue();
    gOamBuffer = new OamBuffer;
    gObjVramCursor = new ObjVramCursor(0);
    gInput = new KeyInput;
    gEntityFlags = new LevelEntityFlags;
    gPaletteCycles = new PaletteCycles;
    {
        u16 *dispcnt = (u16 *)gDispcnt;
        u16 zero = 0;

        *dispcnt = zero;
        SetObjMapping1D();
        CommitDispcnt();
        unusedFlags = zero;
    }
}
