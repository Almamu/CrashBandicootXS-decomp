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

struct AudioContext;
struct hud_counter;
struct oam_shadow_buffer;
struct palette_cache;
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

/* sym_iwram.txt */
extern u8 gDispcnt[2];                              /* the REG_DISPCNT shadow (CommitDispcnt), read and written bytewise */
extern struct palette_cache *gPaletteCache;
extern struct AudioContext *gAudioContext;
extern void *gSpriteRenderer;                       /* an empty 4-byte object (DrawSprite ignores it) */
extern struct sprite_bank_set *gSpriteBankSet;
extern void *gEntitySpawner;
extern struct vram_upload_cursor *gObjVramCursor;
extern struct oam_shadow_buffer *gOamBuffer;
extern void *gInput;                                /* UpdateKeys's object; it only reads gKeys */
extern struct hud_counter *gHud;
extern u8 gJetpackPlayerInactive;

/* src/data/boss_pictures_167ad4.c: a full turn in 256 steps, scaled by 0x100. */
extern const s16 gSineTable[256];

#endif /* GUARD_GLOBALS_H */
