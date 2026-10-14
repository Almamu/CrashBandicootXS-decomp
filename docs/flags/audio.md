# `audio_context::state`: the song's playback state (#843)

`state` (+0x04 of `struct audio_context`, the AudioContext class in
include/engine/audio/audio.hpp) is what the GAX song player is doing. The names sit
next to the struct. `IsStopped`, `IsPlaying` and `IsPaused` test it.

| Value | Name | Set by | Meaning |
|---|---|---|---|
| 0 | `AUDIO_STATE_STOPPED` | the constructor; StopSong and StartSong (before a new song) when not already stopped | no song: `currentSong` and `pendingSong` are `SONG_NONE`, GAX is stopped and the per-tick IRQ update (`gGaxIrqEnabled`) is off |
| 1 | `AUDIO_STATE_PLAYING` | StartSong, when GAX2_init succeeds; ResumeSong, from `PAUSED` | the song plays; the IRQ update is on |
| 2 | `AUDIO_STATE_PAUSED` | PauseSong, from `PLAYING` | GAX_pause'd; ResumeSong goes back to `PLAYING` |

The save code (save_data.cpp) stops a playing song around its flash
reads and writes, and plays it again afterwards (`IsPlaying()`).
