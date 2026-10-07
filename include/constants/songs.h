#ifndef GUARD_CONSTANTS_SONGS_H
#define GUARD_CONSTANTS_SONGS_H

/*
 * Song IDs: the index into gSongTable (src/data/song_table_16aa20.c)
 * that PlaySong/StartSong take. Each name is the song's key in
 * sound/gax_manifest.json (the title the ROM stores with the song,
 * e.g. "jungle (c) Manfred Linzner"), and gSongTable pairs every
 * SONG_<NAME> with the generated GAX_SONG_<NAME> offset, so the two
 * can't drift apart. The comments say where the game plays each one.
 */

#define SONG_JUNGLE 0x0           // level theme (gThemeMusicCues)
#define SONG_UNDERWATER 0x1       // level theme
#define SONG_ARCTIC 0x2           // level theme
#define SONG_SEWERS 0x3           // level theme
#define SONG_FUTURE 0x4           // level theme
#define SONG_ROCKET_CRASH 0x5     // level theme
#define SONG_BONUS_ROUND 0x6      // bonus rooms (PlayRoomMusic)
#define SONG_DINGODILE 0x7        // boss theme
#define SONG_N_GIN 0x8            // boss theme
#define SONG_TINY 0x9             // boss theme
#define SONG_NEO_CORTEX 0xa       // boss theme
#define SONG_MAIN_MENU_EUROPE 0xb // the title screen (InitTitleScreen)
#define SONG_MAIN_MENU_JAPAN 0xc  // the title screen's cheat code (TitleScreenCheatInput)
#define SONG_CUTSCENES 0xd        // cutscene slides
#define SONG_CUTSCENES_SPOOKY 0xe // cutscene slides
#define SONG_INTRO 0xf            // cutscene slides, the power dialog
#define SONG_WARP_ROOM 0x10       // level select, save menu
#define SONG_CREDITS 0x11         // RunCredits
#define SONG_DRUMS 0x12           // maskLevel MASK_LEVEL_INVINCIBLE (PlayRoomMusic)

#define SONG_COUNT 19
#define SONG_NONE 0x13 // AudioContext.currentSong/pendingSong: no song (StopSong)

#endif /* GUARD_CONSTANTS_SONGS_H */
