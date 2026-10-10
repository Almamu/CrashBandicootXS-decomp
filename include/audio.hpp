#ifndef GUARD_AUDIO_HPP
#define GUARD_AUDIO_HPP

/* The audio context as C++ (docs/cplusplus.md, "The audio context"):
 * class AudioContext, the music and sound-effect front end over GAX2
 * (src/audio/audio.cpp). gAudioContext (globals.h declares it as an
 * `AudioContext *` to C++) is the one instance: LevelState's constructor
 * makes it with `new AudioContext` (src/level/spawn_markers.cpp) and its
 * destructor deletes it (src/level/level_cutscene.cpp). It has no vtable.
 *
 * The C++ traits the C had: the destructor takes `__in_chrg` and frees
 * on bit 0 (DestroyAudioContext's `if (flags & 1) IwramFree(self)`), the
 * constructor returns `this` (InitAudioContext), and its one caller
 * allocated with IwramAlloc first: the class's own operator new and
 * delete, IWRAM's heap as ActorSelf's (actor_self.hpp). The state tests
 * every method repeats (`flag = 0; if (state == 1) flag = 1; if (flag)`)
 * are the inline IsPlaying/IsPaused/IsStopped: the copy of the flag into
 * a second register in StartSong and StopSong is the inline's return
 * value, which the C needed a register pin for.
 *
 * struct audio_context (below) is the field list, the class's base; no
 * C file is left that calls the methods by their C names.
 *
 * `#pragma interface`: no vtable to emit, and no out-of-line copies of
 * the inline methods. */
#pragma interface

extern "C" {
#include "core.h"
#include "audio.h"
#include "util.h"
}

/* The music/SFX-trigger "context" object `PlaySfx` and its neighbors take
 * as their first argument - `*gAudioContext` in the ROM, an
 * 8340-byte (0x2094) allocation made by `InitLevelState` (see
 * docs/rom_map.md's "Found the origin point" section). At +0x58 sits the
 * music player's GAX2 parameter block (`gax`, a `struct GaxSongHeader`,
 * <gax.h>: GAX2_new fills it, StartSong sets its work buffer (`gaxWork`,
 * 0x2000 bytes), `numSfx`, `sfxTypes` and `layout`, and hands it to
 * GAX2_init), followed by that work RAM.
 *
 * Two independent fade-envelope pairs are tracked, each ramping by a
 * fixed +-0x10 (Q8.8, ~0.06) per `UpdateAudio` tick once its direction
 * flag is armed:
 *   - `musicVolCurrent`/`musicVolTarget` (+0x18/+0x1c, mirrored into
 *     the parameter block's master `volume`, +0x68) - independent of the
 *     ducking pair below.
 *   - `duckVolCurrent`/`duckVolTarget` (+0x24/+0x28) - the music-ducking
 *     ramp `FadeOutMusic` (duck out, explicit target)/`FadeInMusic` (duck
 *     back in, restores `duckVolDefault`) drive; `duckVolDefault` (+0x20)
 *     is the last value explicitly set via `SetMusicVolume`.
 *
 * A second pair of 3-word records tracks a currently-playing/queued
 * "ambient" sound effect (distinct from the one-shot `PlaySfx` calls):
 * `activeSfx`/`pendingSfx`, each `{id, gRoomFrameCount-relative
 * deadline, volume}` - see `TickAmbientSfx`/`ResetAmbientSfx`/`StopAmbientSfx`/
 * `PlayAmbientSfx`. `0x63` (99) is the "none" sentinel for both ids.
 * `TickAmbientSfx` copies `pendingSfx` over `activeSfx` as a single 12-byte
 * struct assignment (not 3 separate field copies) - that's what gets it
 * to reproduce the ROM's `ldm/stm {r2,r3,r5}` block-move codegen. */
struct SfxRecord {
    u32 id;       // 0x63 (99) = none
    u32 deadline; // gRoomFrameCount-relative
    s32 volume;   // target the owning ambientSfxVolume fade ramps toward
};

struct audio_context {
    u32 unused_00;   // 0x00 - never read or written (InitAudioContext skips it too)
    u32 state;       // 0x04 - 0 = stopped, 1 = playing, 2 = paused
    u32 currentSong; // 0x08 - index into gSongTable (SONG_*); SONG_NONE = none
    u32 pendingSong; // 0x0c - queued song index, started once the duck-out fade completes
    // 0x10/0x14 - round-robin record of the last 2 PlaySfx ids (StopSfx stop-if-playing scan)
    u32 lastSfxId[2];
    // 0x18 - signed: UpdateAudio compares it with blt/bgt, not an unsigned bcc/bcs
    s32 musicVolCurrent;
    s32 musicVolTarget; // 0x1c
    s32 duckVolDefault; // 0x20
    s32 duckVolCurrent; // 0x24
    s32 duckVolTarget;  // 0x28
    s32 sfxVolume;      // 0x2c - PlaySfx's own volume multiplier
    // 0x30 - GAX2 low-pass filter amount, mirrored into the parameter block's `filter`
    // (gax.filter, +0x62) while playing (SetMusicFilter)
    u32 musicFilter;
    // 0x34 - the ambient-sfx channel's own current fade volume (ramps toward activeSfx.volume,
    // TickAmbientSfx - signed, compared with bgt/bge/ble)
    s32 ambientSfxVolume;
    struct SfxRecord activeSfx;  // 0x38
    struct SfxRecord pendingSfx; // 0x44
    u8 musicVolFadeUpArmed;      // 0x50
    u8 musicVolFadeDownArmed;    // 0x51
    u8 duckVolFadeUpArmed;       // 0x52
    u8 duckVolFadeDownArmed;     // 0x53
    // 0x54 - only ever cleared in this cluster (StartSong, on a successful song start)
    u8 field_54;
    struct GaxSongHeader gax; // 0x58 - the music player's GAX2 parameter block
    u8 gaxWork[0x2000];       // 0x94 - GAX2's work RAM (gax.workBuf)
};

COMPILE_TIME_ASSERT(audio_hpp, sizeof(struct audio_context) == 0x2094);

/* The methods keep their C names (cxx_symbols.txt), except the four whose
 * C name is noted. */
class AudioContext : public audio_context
{
public:
    void Update(); // UpdateAudio
    void StartSong(u32 songIndex);
    void PlaySfx(u32 id, u32 volumeParam);
    void TickAmbientSfx();
    void StopSfx(u32 id);
    void ResetAmbientSfx();
    void StopAmbientSfx();
    void PlayAmbientSfx(u32 id, u32 frameOffset, s32 volumeMul, bool force);
    u32 GetCurrentSong();
    s32 GetSfxVolume();
    s32 GetMusicVolume();
    void FadeOutMusic(u32 value);
    void FadeInMusic();
    void FadeOutMasterVolume(u32 value);
    void FadeInMasterVolume(u32 value);
    void SetMusicFilter(u32 value); // UNUSED
    void SetMusicVolume(u32 value);
    void SetSfxVolume(u32 value);
    void PlaySong(u32 id);
    void ResumeSong();
    void PauseSong();
    void StopSong();
    ~AudioContext();         // DestroyAudioContext
    AudioContext();          // InitAudioContext
    void DisableVCountIrq(); // DisableMusicVCountIrq: `this` is unused

    bool IsStopped()
    {
        return state == 0;
    }
    bool IsPlaying()
    {
        return state == 1;
    }
    bool IsPaused()
    {
        return state == 2;
    }

    static void *operator new(size_t size)
    {
        return IwramAlloc(size);
    }
    static void operator delete(void *p)
    {
        IwramFree((u8 *)p);
    }
};

COMPILE_TIME_ASSERT(audio_hpp, sizeof(AudioContext) == 0x2094);

#endif /* !GUARD_AUDIO_HPP */
