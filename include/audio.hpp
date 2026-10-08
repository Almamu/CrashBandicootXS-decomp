#ifndef GUARD_AUDIO_HPP
#define GUARD_AUDIO_HPP

/* The audio context as C++ (docs/cplusplus.md, "The audio context"):
 * class AudioContext, the music and sound-effect front end over GAX2
 * (src/audio/audio.cpp). gAudioContext (globals.h declares it as an
 * `AudioContext *` to C++) is the one instance: LevelState's constructor
 * makes it with `new AudioContext` (src/level/spawn_pickups.cpp) and its
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
 * struct audio_context (audio.h) is the field list, the class's base (not
 * a C view: no C file uses it); no C file is left that calls the methods
 * by their C names.
 *
 * `#pragma interface`: no vtable to emit, and no out-of-line copies of
 * the inline methods. */
#pragma interface

extern "C" {
#include "core.h"
#include "audio.h"
#include "util.h"
}

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
