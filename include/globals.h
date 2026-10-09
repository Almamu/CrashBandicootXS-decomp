#ifndef GUARD_GLOBALS_H
#define GUARD_GLOBALS_H

/* The globals that three or more subsystems use (docs/headers_plan.md,
 * "Who owns a symbol", rule 3): the sym_iwram.txt singletons the game
 * builds at boot or per level (the audio context, the input object, the
 * OAM shadow buffer, the palette cache, ...), the key state and frame
 * counter of src/iwram/iwram_data.cpp, and the sine table.
 *
 * Each global has the type of its definition or, for the linker-script
 * symbols, the type of the object stored there. A .c file that needs a
 * different type for codegen keeps it as an asm-label alias with a
 * `codegen:` comment (docs/headers_plan.md). */

#include "core.h"
#include "vtable.h"

struct entity_flags;
struct level_state;
struct sprite_bank_table;

/* The input word: the held keys in the low half, the keys newly pressed
 * this frame in the high half (UpdateKeys, key_input.cpp). The action
 * controller's handlers read it whole (`all`); the menus read the
 * halves. */
struct held_pressed_pair {
    u16 held;
    u16 pressed;
};

union key_state {
    struct held_pressed_pair half;
    u32 all;
};

/* The first byte of the sprite bank set's banks (`gSpriteBankSet`, class
 * SpriteBankSet, sprite_obj.hpp: `table->banks`, 12 bytes per bank), the
 * base its users take their byte offsets from. It is read through the table's first word, so that this header
 * doesn't need sprite_bank.h (its `struct sprite_frame` clashes with
 * actor_anim.h's). */
#define SPRITE_BANK_BASE (*(u8 *const *)gSpriteBankSet->table)

/* src/iwram/iwram_data.cpp */
extern union key_state gKeys;
extern u32 gRoomFrameCount;
/* The circular actor list's root (ActorSelf's prev/next, actor_self.hpp).
 * No C file uses it. */
#ifdef __cplusplus
extern class ActorSelf *gActorList;
#endif

/* sym_iwram.txt */
extern u8 gDispcnt[2]; /* the REG_DISPCNT shadow (CommitDispcnt), read and written bytewise */
#ifdef __cplusplus
extern class PaletteCache *gPaletteCache;
#endif
/* class AudioContext (audio.hpp) to the C++ files. */
#ifdef __cplusplus
extern class AudioContext *gAudioContext;
#endif
/* An empty 4-byte object (DrawSprite ignores it): class SpriteRenderer
 * (sprite_obj.hpp) to the C++ files. */
#ifdef __cplusplus
extern class SpriteRenderer *gSpriteRenderer;
#endif
/* The sprite bank set (built by InitLevelState). No C file uses it. */
#ifdef __cplusplus
extern class SpriteBankSet *gSpriteBankSet;
#endif
/* The entity spawner (CreateEntitySpawner): an EntitySpawner
 * (spawners.hpp). */
#ifdef __cplusplus
extern class EntitySpawner *gEntitySpawner;
#endif
#ifdef __cplusplus
extern class ObjVramCursor *gObjVramCursor;
#endif
#ifdef __cplusplus
extern class OamBuffer *gOamBuffer;
#endif
/* The key input object (new KeyInput, spawn_markers.cpp): a KeyInput
 * (key_input.hpp), whose methods use gKeys, not the object. No C file
 * uses it. */
#ifdef __cplusplus
extern class KeyInput *gInput;
#endif
/* The HUD (game_frame.cpp builds it): a Hud (hud.hpp). No C file uses
 * it. */
#ifdef __cplusplus
extern class Hud *gHud;
#endif
extern u8 gJetpackPlayerInactive;

/* sym_iwram.txt: the level's objects. game_frame.cpp builds the level
 * state and the entity flags; PlayRoom (play_room.cpp) builds the rest per
 * room. The level state is a C++ class, LevelState (level_state.hpp;
 * C sees an incomplete struct level_state); the structs are in level.h
 * (struct entity_flags, camera). The level layers (LevelLayers,
 * bg_layer.hpp), the part lists (PartList, sprite_obj.hpp) and the crate
 * list (CrateList, crate_list.hpp) have no C view and no C user: only the
 * C++ files see them. */
extern struct entity_flags *gEntityFlags;
#ifdef __cplusplus
extern class LevelState *gLevelState;
#else
extern struct level_state *gLevelState;
#endif
#ifdef __cplusplus
/* The camera (PlayRoom builds it): a Camera (camera.hpp). */
extern class Camera *gCamera;
/* What the player's body touches (CollidePlayerWithObjects runs
 * CollidePartsOfClass on it): the pickups, gems, crystals, platforms and
 * Tiny's hop pads. */
extern class PartList *gTouchableList;
/* What the player's attack box hits (PartList::Collide), such as the
 * enemies and bosses. */
extern class PartList *gCollidableList;
/* Updated before the player and drawn first, so in front of everything
 * (the lowest OAM entries); the player never collides with it: the Cortex
 * boss and its cannon and shots, and wumpa flying to the HUD (CreateWumpa
 * with `special`). */
extern class PartList *gForegroundList;
extern class LevelLayers *gLevelLayers;
extern class CrateList *gCrateList;
#endif

/* sym_iwram.txt: the player object, built by PlayRoom: a Player
 * (player.hpp). No C file uses it. */
#ifdef __cplusplus
extern class Player *gPlayer;
#endif

/* src/data/boss_pictures_167ad4.c: a full turn in 256 steps, scaled by 0x100. */
extern const s16 gSineTable[256];

#endif /* GUARD_GLOBALS_H */
