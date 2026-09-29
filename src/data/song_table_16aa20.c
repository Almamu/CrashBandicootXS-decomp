#include "gba/types.h"
#include "gax_songs.h"

/*
 * ROM 0x0816AA20: the song table, indexed by song id (sub_80017BC,
 * music_player.c; `AudioContext.currentSong`). Each entry is a song in
 * the GAX2 music block gStaticData_0855BCB4, which tools/gax_audio.py
 * builds from sound/; the offsets come from its generated gax_songs.h,
 * so a song can change size.
 */

extern const u8 gStaticData_0855BCB4[];

const void *const gStaticData_0816AA20[19] = {
    gStaticData_0855BCB4 + GAX_SONG_JUNGLE,
    gStaticData_0855BCB4 + GAX_SONG_UNDERWATER,
    gStaticData_0855BCB4 + GAX_SONG_ARCTIC,
    gStaticData_0855BCB4 + GAX_SONG_SEWERS,
    gStaticData_0855BCB4 + GAX_SONG_FUTURE,
    gStaticData_0855BCB4 + GAX_SONG_ROCKET_CRASH,
    gStaticData_0855BCB4 + GAX_SONG_BONUS_ROUND,
    gStaticData_0855BCB4 + GAX_SONG_DINGODILE,
    gStaticData_0855BCB4 + GAX_SONG_N_GIN,
    gStaticData_0855BCB4 + GAX_SONG_TINY,
    gStaticData_0855BCB4 + GAX_SONG_NEO_CORTEX,
    gStaticData_0855BCB4 + GAX_SONG_MAIN_MENU_EUROPE,
    gStaticData_0855BCB4 + GAX_SONG_MAIN_MENU_JAPAN,
    gStaticData_0855BCB4 + GAX_SONG_CUTSCENES,
    gStaticData_0855BCB4 + GAX_SONG_CUTSCENES_SPOOKY,
    gStaticData_0855BCB4 + GAX_SONG_INTRO,
    gStaticData_0855BCB4 + GAX_SONG_WARP_ROOM,
    gStaticData_0855BCB4 + GAX_SONG_CREDITS,
    gStaticData_0855BCB4 + GAX_SONG_DRUMS,
};
