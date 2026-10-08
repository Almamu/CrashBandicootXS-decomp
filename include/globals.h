#ifndef GUARD_GLOBALS_H
#define GUARD_GLOBALS_H

/* The globals that three or more subsystems use (docs/headers_plan.md,
 * "Who owns a symbol", rule 3): the sym_iwram.txt singletons the game
 * builds at boot or per level (the audio context, the input object, the
 * OAM shadow buffer, the palette cache, ...), the key state and frame
 * counter of src/iwram/iwram_data.c, and the sine table.
 *
 * Each global has the type of its definition or, for the linker-script
 * symbols, the type of the object stored there. A .c file that needs a
 * different type for codegen keeps it as an asm-label alias with a
 * `codegen:` comment (docs/headers_plan.md). */

#include "core.h"
#include "vtable.h"

struct AudioContext;
struct actor_self;
struct camera;
struct entity_flags;
struct hud_counter;
struct level_layers;
struct level_state;
struct oam_shadow_buffer;
struct palette_cache;
struct part_list;
struct player;
struct pool_manager;
struct sprite_bank_table;
struct vram_upload_cursor;

/* The input word: the held keys in the low half, the keys newly pressed
 * this frame in the high half (UpdateKeys, irq.c). The action
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

/* The sprite bank set (`gSpriteBankSet`, 4 bytes, built by
 * InitLevelState): it holds the sprite bank table the level's sprites
 * come from (&gSpriteBankTable). Its users take the first bank's animations
 * (`table->banks`) as the base of their byte offsets. */
struct sprite_bank_set {
    const struct sprite_bank_table *table;
};

/* The first byte of the set's sprite banks (`table->banks`, 12 bytes per
 * bank). It is read through the table's first word, so that this header
 * doesn't need sprite_bank.h (its `struct sprite_frame` clashes with
 * actor_anim.h's). */
#define SPRITE_BANK_BASE (*(u8 *const *)gSpriteBankSet->table)

/* src/iwram/iwram_data.c */
extern union key_state gKeys;
extern u32 gRoomFrameCount;
/* The circular actor list's root (actor_self.prev/next): the C++ files see
 * it as its class, ActorSelf (actor_self.hpp), the C files as its C view. */
#ifdef __cplusplus
extern class ActorSelf *gActorList;
#else
extern struct actor_self *gActorList;
#endif

/* sym_iwram.txt */
extern u8 gDispcnt[2]; /* the REG_DISPCNT shadow (CommitDispcnt), read and written bytewise */
extern struct palette_cache *gPaletteCache;
extern struct AudioContext *gAudioContext;
extern void *gSpriteRenderer; /* an empty 4-byte object (DrawSprite ignores it) */
extern struct sprite_bank_set *gSpriteBankSet;
/* The entity spawner (CreateEntitySpawner): the C++ files see it as its
 * class, EntitySpawner (spawners.hpp), the C files as level.h's struct
 * entity_spawner. */
#ifdef __cplusplus
extern class EntitySpawner *gEntitySpawner;
#else
extern struct entity_spawner *gEntitySpawner;
#endif
extern struct vram_upload_cursor *gObjVramCursor;
extern struct oam_shadow_buffer *gOamBuffer;
extern void *gInput; /* UpdateKeys's object; it only reads gKeys */
extern struct hud_counter *gHud;
extern u8 gJetpackPlayerInactive;

/* sym_iwram.txt: the level's objects. game_frame.c builds the level
 * state and the entity flags; PlayRoom (play_room.c) builds the rest per
 * room. The structs are in level_state.h (struct level_state), level.h
 * (struct level_layers, entity_flags, camera), box_part.h (struct
 * part_list) and crates.h (struct pool_manager, the crate list). */
extern struct entity_flags *gEntityFlags;
extern struct level_state *gLevelState;
extern struct camera *gCamera;
/* What the player's body touches (CollidePlayerWithObjects runs
 * CollidePartsOfClass on it): the pickups, gems, crystals, platforms and
 * Tiny's hop pads. */
extern struct part_list *gTouchableList;
/* What the player's attack box hits (CollidePartList), such as the
 * enemies and bosses. */
extern struct part_list *gCollidableList;
/* Updated before the player and drawn first, so in front of everything
 * (the lowest OAM entries); the player never collides with it: the Cortex
 * boss and its cannon and shots, and wumpa flying to the HUD (CreateWumpa
 * with `special`). */
extern struct part_list *gForegroundList;
extern struct level_layers *gLevelLayers;
extern struct pool_manager *gCrateList;

/* sym_iwram.txt: the player object, built by PlayRoom. The C++ files see
 * it as its class, Player (player.hpp), the C files as its C view, struct
 * player (player.h). */
#ifdef __cplusplus
extern class Player *gPlayer;
#else
extern struct player *gPlayer;
#endif

/* src/data/entity_vtables_7e3bec.c: the actor base's method table
 * (DestroyActor/UpdateActor/DrawActor), which every actor-zone destructor
 * stores back before it frees the object. */
extern const struct vtable_slot gActorVtable[4];

/* src/data/boss_pictures_167ad4.c: a full turn in 256 steps, scaled by 0x100. */
extern const s16 gSineTable[256];

#endif /* GUARD_GLOBALS_H */
