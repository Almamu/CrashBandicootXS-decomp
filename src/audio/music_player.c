#include "core.h"
#include "audio.h"

/* The first matched code in the Shin'en GAX2 wrapper layer (the engine
 * itself is still raw in asm/code_3.s - see docs/audio.md). These two
 * functions drive music playback on the shared `AudioContext` object
 * (`*gAudioContext` in the ROM - see include/audio.h and
 * docs/rom_map.md's "Found the origin point" section): `UpdateAudio`
 * is the per-tick fade update, `StartSong` starts a song. `PlaySfx`
 * (the very next function in ROM order) stays raw here - see
 * src/audio/sfx_ambient.c's doc comment. */

extern void TickAmbientSfx(struct AudioContext *self);
extern u8 gGaxIrqEnabled;
extern void *gSongTable[19];
extern u8 gGaxMusicData[];
extern void FadeInMusic(struct AudioContext *self);
void StartSong(struct AudioContext *self, u32 songIndex);

/* Per-tick (called off `EnableMusicVCountIrq`'s installed callback, a few
 * functions after this chunk) update of both fade-envelope pairs:
 * `musicVolCurrent`/`Target` (independent background-volume ramp) and
 * `duckVolCurrent`/`Target` (the ducking ramp that also kicks off a
 * queued `pendingSong` once it finishes fading all the way down). Both
 * ramp by a fixed 0x10 (Q8.8) per call. */
void UpdateAudio(struct AudioContext *self)
{
    s32 isPlaying;

    isPlaying = 0;
    if (self->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        if (self->musicVolFadeUpArmed != 0) {
            if (self->musicVolCurrent >= self->musicVolTarget) {
                self->musicVolCurrent = self->musicVolTarget;
                self->musicVolFadeUpArmed = 0;
            } else {
                self->musicVolCurrent += 0x10;
            }
            {
                s32 v = self->musicVolCurrent;
                *(vu16 *)((u8 *)self + 0x68) = v;
            }
        }
        if (self->musicVolFadeDownArmed != 0) {
            if (self->musicVolCurrent <= self->musicVolTarget) {
                self->musicVolCurrent = self->musicVolTarget;
                self->musicVolFadeDownArmed = 0;
            } else {
                self->musicVolCurrent -= 0x10;
            }
            {
                s32 v = self->musicVolCurrent;
                *(vu16 *)((u8 *)self + 0x68) = v;
            }
        }
        TickAmbientSfx(self);
        if (self->duckVolFadeUpArmed != 0) {
            if (self->duckVolCurrent >= self->duckVolTarget) {
                self->duckVolCurrent = self->duckVolTarget;
                self->duckVolFadeUpArmed = 0;
            } else {
                self->duckVolCurrent += 0x10;
            }
            GAX_set_music_volume(-1, self->duckVolCurrent);
        }
        if (self->duckVolFadeDownArmed != 0) {
            if (self->duckVolCurrent <= self->duckVolTarget) {
                self->duckVolCurrent = self->duckVolTarget;
                self->duckVolFadeDownArmed = 0;
                if (self->pendingSong != 0x13) {
                    StartSong(self, self->pendingSong);
                    self->pendingSong = 0x13;
                }
            } else {
                self->duckVolCurrent -= 0x10;
            }
            GAX_set_music_volume(-1, self->duckVolCurrent);
        }
        GAX_play();
    }
}
asm(".align 2, 0");

/* Starts playing song `songIndex` (index into the 19-entry
 * `gSongTable` song-pointer table). First-time-only resets
 * the player state if it wasn't already stopped, then (re)initializes
 * the embedded GAX2 runtime player-state object at `self+0x58` -
 * genuinely nested engine-internal state `GAX2_new`/`GAX2_init`
 * own, not independently reverse-engineered, so those writes stay raw
 * offset casts (see docs/audio.md). On success, ducks the music back
 * in (`FadeInMusic`) and arms the per-tick GAX2 IRQ update
 * (`gGaxIrqEnabled`). */
void StartSong(struct AudioContext *self, u32 songIndex)
{
    {
        register s32 wasStopped asm("r1");
        register s32 zero asm("r4");

        wasStopped = 0;
        if (self->state == 0) {
            wasStopped = 1;
        }
        zero = wasStopped;
        if (zero == 0) {
            self->pendingSong = 0x13;
            self->currentSong = 0x13;
            self->state = zero;
            GAX_stop();
            gGaxIrqEnabled = zero;
        }
    }
    {
        u8 *gaxState = (u8 *)self + 0x58;

        GAX2_new(gaxState);
        *(void **)((u8 *)self + 0x58) = (u8 *)self + 0x94;
        *(u32 *)((u8 *)self + 0x5c) = 0x2000;
        *(void **)((u8 *)self + 0x88) = gSongTable[songIndex];
        {
            u16 *p = (u16 *)((u8 *)self + 0x66);
            u8 zero2 = 0;

            *p = 3;
            *(void **)((u8 *)self + 0x84) = gGaxMusicData;
            *((u8 *)self + 0x90) = zero2;
        }
        if (GAX2_init((struct GaxSongHeader *)gaxState)) {
            self->currentSong = songIndex;
            *((u8 *)self + 0x54) = 0;
            FadeInMusic(self);
            gGaxIrqEnabled = 1;
            self->state = 1;
        }
    }
}
