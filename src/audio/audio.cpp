#include "audio.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "system.h"
#include "util.h"
#include "globals.h"
}

/* The Shin'en GAX2 wrapper layer (the engine itself is lib/gax, see
 * docs/audio.md): class AudioContext (include/audio.hpp), the music and
 * sound-effect front end `*gAudioContext` is (docs/rom_map.md's "Found
 * the origin point" section). */

/* Per-tick (called off `EnableMusicVCountIrq`'s installed callback, at
 * the end of this file) update of both fade-envelope pairs:
 * `musicVolCurrent`/`Target` (independent background-volume ramp) and
 * `duckVolCurrent`/`Target` (the ducking ramp that also kicks off a
 * queued `pendingSong` once it finishes fading all the way down). Both
 * ramp by a fixed 0x10 (Q8.8) per call. */
void AudioContext::Update()
{
    if (IsPlaying()) {
        if (musicVolFadeUpArmed != 0) {
            if (musicVolCurrent >= musicVolTarget) {
                musicVolCurrent = musicVolTarget;
                musicVolFadeUpArmed = 0;
            } else {
                musicVolCurrent += 0x10;
            }
            gax.volume = musicVolCurrent;
        }
        if (musicVolFadeDownArmed != 0) {
            if (musicVolCurrent <= musicVolTarget) {
                musicVolCurrent = musicVolTarget;
                musicVolFadeDownArmed = 0;
            } else {
                musicVolCurrent -= 0x10;
            }
            gax.volume = musicVolCurrent;
        }
        TickAmbientSfx();
        if (duckVolFadeUpArmed != 0) {
            if (duckVolCurrent >= duckVolTarget) {
                duckVolCurrent = duckVolTarget;
                duckVolFadeUpArmed = 0;
            } else {
                duckVolCurrent += 0x10;
            }
            GAX_set_music_volume(-1, duckVolCurrent);
        }
        if (duckVolFadeDownArmed != 0) {
            if (duckVolCurrent <= duckVolTarget) {
                duckVolCurrent = duckVolTarget;
                duckVolFadeDownArmed = 0;
                if (pendingSong != SONG_NONE) {
                    StartSong(pendingSong);
                    pendingSong = SONG_NONE;
                }
            } else {
                duckVolCurrent -= 0x10;
            }
            GAX_set_music_volume(-1, duckVolCurrent);
        }
        GAX_play();
    }
}

/* Starts playing song `songIndex` (index into the 19-entry
 * `gSongTable` song-pointer table). First-time-only resets
 * the player state if it wasn't already stopped, then (re)initializes
 * the embedded GAX2 parameter block `gax` (GAX2_new's defaults, then the
 * work buffer `gaxWork`, 3 SFX voices, the SFX handler types and the
 * song's layout) and starts it with GAX2_init. On success, ducks the music back
 * in (`FadeInMusic`) and arms the per-tick GAX2 IRQ update
 * (`gGaxIrqEnabled`). */
void AudioContext::StartSong(u32 songIndex)
{
    if (!IsStopped()) {
        pendingSong = SONG_NONE;
        currentSong = SONG_NONE;
        state = 0;
        GAX_stop();
        gGaxIrqEnabled = 0;
    }
    GAX2_new(&gax);
    gax.workBuf = gaxWork;
    gax.workSize = sizeof(gaxWork);
    gax.layout = (struct GaxHandlerLayout *)gSongTable[songIndex];
    gax.numSfx = 3;
    gax.sfxTypes = (struct GaxHandlerType **)gGaxMusicData;
    gax.showErrors = 0;
    if (GAX2_init(&gax)) {
        currentSong = songIndex;
        field_54 = 0;
        FadeInMusic();
        gGaxIrqEnabled = 1;
        state = 1;
    }
}

/* `PlaySfx(context, sfxId, volumeParam)` - see docs/audio.md's "Sound
 * effects" section (called ~264 times across gameplay code). Looks up
 * `sfxId` in the 99-entry `gSfxTable` table, steals a mixing voice via
 * `GAX_fx_ex` (retrying once with the alternate slot on failure - a
 * two-channel round-robin toggled by `gSfxVoiceToggle`), and plays
 * it at a volume scaled by both the table's own base volume and the
 * context's `sfxVolume`. Records which of the 2 round-robin slots
 * last played `sfxId` in `lastSfxId`, for `StopSfx`'s
 * stop-if-playing scan. */
void AudioContext::PlaySfx(u32 id, u32 volumeParam)
{
    if (IsPlaying()) {
        u32 handle = gSfxTable[id].slotId;

        if (handle != 0) {
            u32 toggle = gSfxVoiceToggle;
            u32 chanArg = gSfxTable[id].chanArg;
            s32 voice = GAX_fx_ex(handle, toggle, chanArg, -1);

            if (voice == -1) {
                toggle = gSfxVoiceToggle ^ 1;
                gSfxVoiceToggle = toggle;
                voice = GAX_fx_ex(handle, toggle, chanArg, -1);
            }
            if (voice != -1) {
                u32 baseVolume = gSfxTable[id].baseVolume;
                u32 volume = Q16_TO_INT((baseVolume * sfxVolume) * volumeParam);

                GAX_set_fx_volume(voice, volume);
                lastSfxId[gSfxVoiceToggle] = id;
                gSfxVoiceToggle ^= 1;
            }
        }
    }
}

/* Per-tick update of the "ambient" (looping/crossfaded, as opposed to
 * `PlaySfx`'s one-shot) sound-effect channel: ramps `ambientSfxVolume` (the
 * channel's live volume) toward `activeSfx.volume`, and once
 * `gRoomFrameCount` passes `activeSfx.deadline`, fades the channel
 * out over several ticks before promoting the queued `pendingSfx`
 * record into `activeSfx` (a 12-byte struct copy - see
 * include/audio.h) and triggering it via `GAX_fx_ex`. */
void AudioContext::TickAmbientSfx()
{
    u32 now;

    if (activeSfx.id == 0x63) {
        return;
    }
    now = gRoomFrameCount;
    if (now >= activeSfx.deadline) {
        activeSfx.volume = 0;
        ambientSfxVolume -= 0x10;
        if (ambientSfxVolume <= 0) {
            ambientSfxVolume = 0;
            GAX_stop_fx(2);
            activeSfx = pendingSfx;
            if (activeSfx.id != 0x63) {
                u32 handle;

                pendingSfx.id = 0x63;
                handle = gSfxTable[activeSfx.id].slotId;
                GAX_fx_ex(handle, 2, 0, -1);
            }
        }
        GAX_set_fx_volume(2, ambientSfxVolume);
        return;
    }
    if (ambientSfxVolume < activeSfx.volume) {
        ambientSfxVolume += 0x10;
        LIMIT_MAX(ambientSfxVolume, activeSfx.volume);
        GAX_set_fx_volume(2, ambientSfxVolume);
        return;
    }
    if (ambientSfxVolume <= activeSfx.volume) {
        return;
    }
    ambientSfxVolume -= 0x10;
    LIMIT_MIN(ambientSfxVolume, activeSfx.volume);
    GAX_set_fx_volume(2, ambientSfxVolume);
}

/* Stop-if-playing companion to `PlaySfx`: mutes whichever of the 2
 * round-robin channel slots last played `id` (`lastSfxId[0]`/`[1]`),
 * via `GAX_stop_fx(slotIndex)` (a GAX2 per-channel mute, see
 * docs/rom_map.md's "Resolved StopSfx" section). */
void AudioContext::StopSfx(u32 id)
{
    s32 i;

    for (i = 0; i <= 1; i++) {
        if (lastSfxId[i] == id) {
            GAX_stop_fx(i);
        }
    }
}

/* Resets the ambient-sfx channel (both `activeSfx`/`pendingSfx`
 * records cleared to the `0x63` "none" sentinel) and mutes it. */
void AudioContext::ResetAmbientSfx()
{
    pendingSfx.id = 0x63;
    activeSfx.id = 0x63;
    ambientSfxVolume = 0;
    pendingSfx.volume = 0;
    activeSfx.volume = 0;
    GAX_stop_fx(2);
}

/* Forces the ambient-sfx channel's current record to expire
 * immediately (deadline = now) and drops any queued pending record -
 * the next `TickAmbientSfx` tick will fade it out and go idle. */
void AudioContext::StopAmbientSfx()
{
    pendingSfx.id = 0x63;
    activeSfx.deadline = gRoomFrameCount;
}

/* Requests the ambient-sfx channel play `id` for `frameOffset` frames
 * (relative to `gRoomFrameCount`) at a volume derived from
 * `volumeMul`/the table's own base volume/`sfxVolume`, same formula as
 * `PlaySfx`. If nothing is currently active, starts it immediately;
 * if something louder-or-equal is already active with the same `id`,
 * refreshes its deadline/volume (optionally retriggering the voice via
 * `force`); if a different, louder request comes in, queues it as
 * `pendingSfx` and forces the current record to expire on the very
 * next tick instead of playing over it.
 *
 * `force` is a `bool` passed on the stack (the ROM's `add rX, sp, #0x14;
 * ldrb`: docs/cplusplus.md's gotchas), the C's one-byte struct. */
void AudioContext::PlayAmbientSfx(u32 id, u32 frameOffset, s32 volumeMul, bool force)
{
    u8 forceFlag = force;
    u32 handle = gSfxTable[id].slotId;

    if (handle != 0 && volumeMul > 0) {
        s32 volume = Q16_TO_INT((gSfxTable[id].baseVolume * volumeMul) * sfxVolume);
        u32 cur = activeSfx.id;

        if (cur == 0x63) {
            GAX_fx_ex(handle, 2, 0, -1);
            GAX_set_fx_volume(2, ambientSfxVolume);
            activeSfx.id = id;
            activeSfx.deadline = gRoomFrameCount + frameOffset;
            activeSfx.volume = volume;
        } else if (volume >= activeSfx.volume) {
            if (cur == id) {
                activeSfx.deadline = gRoomFrameCount + frameOffset;
                activeSfx.volume = volume;
                if (forceFlag) {
                    GAX_fx_ex(handle, 2, 0, -1);
                    GAX_set_fx_volume(2, ambientSfxVolume);
                }
                pendingSfx.id = 0x63;
                pendingSfx.volume = 0;
            } else {
                u32 now;

                pendingSfx.id = id;
                now = gRoomFrameCount;
                pendingSfx.deadline = now + frameOffset;
                pendingSfx.volume = volume;
                activeSfx.deadline = now;
            }
        }
    }
}

/* currentSong getter. */
u32 AudioContext::GetCurrentSong()
{
    return currentSong;
}

/* sfxVolume getter. */
s32 AudioContext::GetSfxVolume()
{
    return sfxVolume;
}

/* duckVolDefault getter. */
s32 AudioContext::GetMusicVolume()
{
    return duckVolDefault;
}

/* Arms a duck-out: sets the ducking fade target and the "fade down"
 * direction flag. */
void AudioContext::FadeOutMusic(u32 value)
{
    duckVolTarget = value;
    duckVolFadeUpArmed = 0;
    duckVolFadeDownArmed = 1;
}

/* Arms a duck-in back to `duckVolDefault` (the last value explicitly
 * set via `SetMusicVolume`). */
void AudioContext::FadeInMusic()
{
    duckVolTarget = duckVolDefault;
    duckVolFadeUpArmed = 1;
    duckVolFadeDownArmed = 0;
}

/* Arms a fade-out on the independent `musicVolCurrent`/`Target` pair. */
void AudioContext::FadeOutMasterVolume(u32 value)
{
    musicVolTarget = value;
    musicVolFadeUpArmed = 0;
    musicVolFadeDownArmed = 1;
}

/* Arms a fade-in on the independent `musicVolCurrent`/`Target` pair. */
void AudioContext::FadeInMasterVolume(u32 value)
{
    musicVolTarget = value;
    musicVolFadeUpArmed = 1;
    musicVolFadeDownArmed = 0;
}

/* UNUSED - no caller anywhere in the ROM (no `bl` in expected/*.s, no
 * pointer to it in baserom.gba). Sets the music's GAX2 low-pass filter
 * amount: `musicFilter`, and while a song is playing the parameter
 * block's `filter` (`gax.filter`), which GAX_play
 * copies to the player each frame. It has no audible effect in this
 * game: StartSong never sets GaxSongHeader.flags bit 2, so GAX2_init
 * doesn't load the filter routine (gGaxArmFilter). */
void AudioContext::SetMusicFilter(u32 value)
{
    musicFilter = value;
    if (IsPlaying()) {
        gax.filter = value;
    }
}

/* Immediate (non-fading) music-volume setter - also becomes the new
 * `duckVolDefault` a later `FadeInMusic` duck-in restores to. */
void AudioContext::SetMusicVolume(u32 value)
{
    duckVolCurrent = value;
    duckVolDefault = value;
    if (IsPlaying()) {
        GAX_set_music_volume(-1, value);
    }
}

/* sfxVolume setter. */
void AudioContext::SetSfxVolume(u32 value)
{
    sfxVolume = value;
}

/* Requests song `id` to play: if nothing is playing yet, starts it
 * immediately (`StartSong`); otherwise, if it's not already the
 * current or already-queued song, queues it as `pendingSong` and arms
 * a duck-out (`FadeOutMusic`) - `Update`'s per-tick update starts
 * the queued song once the duck-out fade completes. */
void AudioContext::PlaySong(u32 id)
{
    if (!IsPlaying()) {
        StartSong(id);
        return;
    }
    if (id == currentSong) {
        return;
    }
    if (id == pendingSong) {
        return;
    }
    pendingSong = id;
    FadeOutMusic(0);
}

/* Resumes a paused song. */
void AudioContext::ResumeSong()
{
    if (IsPaused()) {
        WaitForVBlank();
        GAX_resume();
        state = 1;
    }
}

/* Pauses a playing song. */
void AudioContext::PauseSong()
{
    if (IsPlaying()) {
        state = 2;
        WaitForVBlank();
        GAX_play();
        GAX_pause();
    }
}

/* Stops the currently playing/paused song (a no-op if already
 * stopped) and disarms the per-tick GAX2 IRQ update. */
void AudioContext::StopSong()
{
    if (!IsStopped()) {
        pendingSong = SONG_NONE;
        currentSong = SONG_NONE;
        state = 0;
        GAX_stop();
        gGaxIrqEnabled = 0;
    }
}

/* Stops the song; the deleting destructor (`__in_chrg` bit 0) then
 * frees the context with the class's operator delete (IwramFree). */
AudioContext::~AudioContext()
{
    StopSong();
    gGaxIrqEnabled = 0;
}

/* Constructor: resets every field to its idle default (both volume
 * pairs to `0x100` = 1.0 in Q8.8, both song slots to the `SONG_NONE` (0x13)
 * sentinel, every fade-direction flag cleared). */
AudioContext::AudioContext()
{
    state = 0;
    pendingSong = SONG_NONE;
    currentSong = SONG_NONE;
    musicVolCurrent = 0x100;
    duckVolCurrent = 0x100;
    duckVolDefault = 0x100;
    sfxVolume = 0x100;
    ambientSfxVolume = 0;
    activeSfx.id = 0x63;
    musicVolFadeDownArmed = 0;
    musicVolFadeUpArmed = 0;
    duckVolFadeDownArmed = 0;
    duckVolFadeUpArmed = 0;
    field_54 = 0;
    musicFilter = 0;
}

/* Disables the GBA's V-Count interrupt - a counterpart to
 * `DisableVBlankHandler` (VBlank) in src/system/irq.cpp. `this` is unused;
 * LevelState's destructor calls it on gAudioContext. */
void AudioContext::DisableVCountIrq()
{
    u8 tmp = DISPSTAT_VCOUNT_INTR;

    *(u8 *)REG_ADDR_DISPSTAT &= ~tmp;
    IrqRestoreHandler(INTR_INDEX_VCOUNT);
}

/* Installs `MusicVCountIrqHandler` as the VCount-IRQ handler and arms VCount IRQs
 * with a fixed trigger line (`0x35`) - the music player's per-tick fade
 * update (`AudioContext::Update`) runs off this VCount interrupt rather
 * than VBlank. */
void EnableMusicVCountIrq(void)
{
    u8 *p;

    IrqSetHandler(INTR_INDEX_VCOUNT, MusicVCountIrqHandler);
    p = (u8 *)REG_ADDR_DISPSTAT;
    p[1] = 0x35;
    *p |= DISPSTAT_VCOUNT_INTR;
}

/* The VCount-IRQ handler installed by `EnableMusicVCountIrq` above: just forwards
 * into the music player's per-tick fade update. */
void MusicVCountIrqHandler(void)
{
    gAudioContext->Update();
}
