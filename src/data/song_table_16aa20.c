#include "core.h"
#include "audio.h"
#include "gax_songs.h"

/*
 * ROM 0x0816AA20: the song table, indexed by song id (StartSong,
 * audio.cpp; `AudioContext::currentSong`). Each entry is a song in
 * the GAX2 music block gGaxMusicData, which tools/gax_audio.py
 * builds from sound/; the offsets come from its generated gax_songs.h,
 * so a song can change size.
 */

const void *const gSongTable[SONG_COUNT] = {
    [SONG_JUNGLE] = gGaxMusicData + GAX_SONG_JUNGLE,
    [SONG_UNDERWATER] = gGaxMusicData + GAX_SONG_UNDERWATER,
    [SONG_ARCTIC] = gGaxMusicData + GAX_SONG_ARCTIC,
    [SONG_SEWERS] = gGaxMusicData + GAX_SONG_SEWERS,
    [SONG_FUTURE] = gGaxMusicData + GAX_SONG_FUTURE,
    [SONG_ROCKET_CRASH] = gGaxMusicData + GAX_SONG_ROCKET_CRASH,
    [SONG_BONUS_ROUND] = gGaxMusicData + GAX_SONG_BONUS_ROUND,
    [SONG_DINGODILE] = gGaxMusicData + GAX_SONG_DINGODILE,
    [SONG_N_GIN] = gGaxMusicData + GAX_SONG_N_GIN,
    [SONG_TINY] = gGaxMusicData + GAX_SONG_TINY,
    [SONG_NEO_CORTEX] = gGaxMusicData + GAX_SONG_NEO_CORTEX,
    [SONG_MAIN_MENU_EUROPE] = gGaxMusicData + GAX_SONG_MAIN_MENU_EUROPE,
    [SONG_MAIN_MENU_JAPAN] = gGaxMusicData + GAX_SONG_MAIN_MENU_JAPAN,
    [SONG_CUTSCENES] = gGaxMusicData + GAX_SONG_CUTSCENES,
    [SONG_CUTSCENES_SPOOKY] = gGaxMusicData + GAX_SONG_CUTSCENES_SPOOKY,
    [SONG_INTRO] = gGaxMusicData + GAX_SONG_INTRO,
    [SONG_WARP_ROOM] = gGaxMusicData + GAX_SONG_WARP_ROOM,
    [SONG_CREDITS] = gGaxMusicData + GAX_SONG_CREDITS,
    [SONG_DRUMS] = gGaxMusicData + GAX_SONG_DRUMS,
};
