#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "core.h"
#include "byte_arg.h"
#include <gax.h>

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

struct AudioContext {
    u32 field_00;    // 0x00 - not touched by this function cluster
    u32 state;       // 0x04 - 0 = stopped, 1 = playing, 2 = paused
    u32 currentSong; // 0x08 - index into gSongTable (19 songs); 0x13 = none
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
    u8 pad_55[3];             // 0x55-0x57
    struct GaxSongHeader gax; // 0x58 - the music player's GAX2 parameter block
    u8 gaxWork[0x2000];       // 0x94 - GAX2's work RAM (gax.workBuf)
};

COMPILE_TIME_ASSERT(audio_h, sizeof(struct AudioContext) == 0x2094);

/* One record of the 99-entry sound-effect trigger table at ROM
 * `0x0816AA6C` (`sound/sfx_table.json`) - see docs/audio.md's "Sound
 * effects" section. Indexed by the id `PlaySfx`/`PlayAmbientSfx` take. */
struct SfxTableEntry {
    /* instrument index into the sound-effect data set
     * (gGaxSfxData, docs/audio.md) - 0 = unused slot */
    u32 slotId;
    /* passed through as GAX_fx_ex's priority arg (only
     * PlaySfx reads this; PlayAmbientSfx hardcodes 0) */
    u32 chanArg;
    /* multiplied by the caller's volume param and AudioContext.sfxVolume, then >>16 */
    u32 baseVolume;
};

extern struct SfxTableEntry gSfxTable[99];

/* The GAX2 music block (data/data.s, built by tools/gax_audio.py) and
 * the song table pointing into it (src/data/song_table_16aa20.c). */
extern const u8 gGaxMusicData[];
extern const void *const gSongTable[19];

/* src/iwram/iwram_data.c */
/* VBlankHandler (src/system/irq.c) calls GAX_irq while this is set. */
extern u8 gGaxIrqEnabled;
/* PlaySfx's two-voice round robin. */
extern u32 gSfxVoiceToggle;

/* src/audio/audio.c */
extern void UpdateAudio(struct AudioContext *self);
extern void StartSong(struct AudioContext *self, u32 songIndex);
extern void PlaySfx(struct AudioContext *self, u32 id, u32 volumeParam);
extern void TickAmbientSfx(struct AudioContext *self);
extern void StopSfx(struct AudioContext *self, u32 id);
extern void ResetAmbientSfx(struct AudioContext *self);
extern void StopAmbientSfx(struct AudioContext *self);
extern void PlayAmbientSfx(struct AudioContext *self, u32 id, u32 frameOffset, s32 volumeMul,
                           struct byte_arg force);
extern u32 GetCurrentSong(struct AudioContext *self);
extern s32 GetSfxVolume(struct AudioContext *self);
extern s32 GetMusicVolume(struct AudioContext *self);
extern void FadeOutMusic(struct AudioContext *self, u32 value);
extern void FadeInMusic(struct AudioContext *self);
extern void FadeOutMasterVolume(struct AudioContext *self, u32 value);
extern void FadeInMasterVolume(struct AudioContext *self, u32 value);
extern void SetMusicFilter(struct AudioContext *self, u32 value);
extern void SetMusicVolume(struct AudioContext *self, u32 value);
extern void SetSfxVolume(struct AudioContext *self, u32 value);
extern void PlaySong(struct AudioContext *self, u32 id);
extern void ResumeSong(struct AudioContext *self);
extern void PauseSong(struct AudioContext *self);
extern void StopSong(struct AudioContext *self);
extern void DestroyAudioContext(struct AudioContext *self, u32 flags);
extern struct AudioContext *InitAudioContext(struct AudioContext *self);
extern void DisableMusicVCountIrq(struct AudioContext *self);
extern void EnableMusicVCountIrq(void);
extern void MusicVCountIrqHandler(void);

#endif /* __AUDIO_H__ */
