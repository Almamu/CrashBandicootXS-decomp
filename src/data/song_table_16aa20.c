#include "gba/types.h"
#include "gax_songs.h"

/*
 * ROM 0x0816AA20: the song table, indexed by song id (sub_80017BC,
 * music_player.c; `AudioContext.currentSong`). Each entry is a song in
 * the GAX2 music block gGaxMusicData, which tools/gax_audio.py
 * builds from sound/; the offsets come from its generated gax_songs.h,
 * so a song can change size.
 */

extern const u8 gGaxMusicData[];

const void *const gSongTable[19] = {
    gGaxMusicData + GAX_SONG_JUNGLE,
    gGaxMusicData + GAX_SONG_UNDERWATER,
    gGaxMusicData + GAX_SONG_ARCTIC,
    gGaxMusicData + GAX_SONG_SEWERS,
    gGaxMusicData + GAX_SONG_FUTURE,
    gGaxMusicData + GAX_SONG_ROCKET_CRASH,
    gGaxMusicData + GAX_SONG_BONUS_ROUND,
    gGaxMusicData + GAX_SONG_DINGODILE,
    gGaxMusicData + GAX_SONG_N_GIN,
    gGaxMusicData + GAX_SONG_TINY,
    gGaxMusicData + GAX_SONG_NEO_CORTEX,
    gGaxMusicData + GAX_SONG_MAIN_MENU_EUROPE,
    gGaxMusicData + GAX_SONG_MAIN_MENU_JAPAN,
    gGaxMusicData + GAX_SONG_CUTSCENES,
    gGaxMusicData + GAX_SONG_CUTSCENES_SPOOKY,
    gGaxMusicData + GAX_SONG_INTRO,
    gGaxMusicData + GAX_SONG_WARP_ROOM,
    gGaxMusicData + GAX_SONG_CREDITS,
    gGaxMusicData + GAX_SONG_DRUMS,
};
