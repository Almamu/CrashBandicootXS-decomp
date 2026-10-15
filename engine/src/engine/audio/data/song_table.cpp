#include "engine/audio/audio.hpp"

extern "C" {
#include "core.h"
#include "gax_songs.h"
}

/*
 * ROM 0x0816AA20: the song table, indexed by song id (StartSong,
 * audio.cpp; `AudioContext::currentSong`). Each entry is a song in
 * the GAX2 music block gGaxMusicData, which tools/gax_audio.py
 * builds from games/xs/sound/; the offsets come from its generated gax_songs.h,
 * so a song can change size. The table itself is generated too
 * (`gax_audio.py --table`, song_table.inc) from the manifest's
 * song_table, the order of the SONG_* ids, so every song has its entry.
 */

#include "song_table.inc"
