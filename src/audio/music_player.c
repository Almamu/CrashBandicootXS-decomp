#include "core.h"
#include "audio.h"

/* The first matched code in the Shin'en GAX2 wrapper layer (the engine
 * itself is still raw in asm/code_3.s - see docs/audio.md). These two
 * functions drive music playback on the shared `AudioContext` object
 * (`*gUnknown_030012BC` in the ROM - see include/audio.h and
 * docs/rom_map.md's "Found the origin point" section): `sub_80016EC`
 * is the per-tick fade update, `sub_80017BC` starts a song. `PlaySfx`
 * (the very next function in ROM order) stays raw here - see
 * src/audio/sfx_ambient.c's doc comment. */

extern void sub_800190C(struct AudioContext *self);
extern void sub_8039064(s32 channel, u32 volume);
extern void sub_8038C88(void);
extern void sub_8039198(void);
extern u8 gUnknown_030007DD;
extern void sub_80381FC(void *gaxState);
extern u8 sub_8038538(void *gaxState);
extern void *gStaticData_0816AA20[19];
extern u8 gStaticData_0855BCB4[];
extern void sub_8001AD8(struct AudioContext *self);
void sub_80017BC(struct AudioContext *self, u32 songIndex);

/* Per-tick (called off `sub_8001C80`'s installed callback, a few
 * functions after this chunk) update of both fade-envelope pairs:
 * `musicVolCurrent`/`Target` (independent background-volume ramp) and
 * `duckVolCurrent`/`Target` (the ducking ramp that also kicks off a
 * queued `pendingSong` once it finishes fading all the way down). Both
 * ramp by a fixed 0x10 (Q8.8) per call. */
void sub_80016EC(struct AudioContext *self)
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
        sub_800190C(self);
        if (self->duckVolFadeUpArmed != 0) {
            if (self->duckVolCurrent >= self->duckVolTarget) {
                self->duckVolCurrent = self->duckVolTarget;
                self->duckVolFadeUpArmed = 0;
            } else {
                self->duckVolCurrent += 0x10;
            }
            sub_8039064(-1, self->duckVolCurrent);
        }
        if (self->duckVolFadeDownArmed != 0) {
            if (self->duckVolCurrent <= self->duckVolTarget) {
                self->duckVolCurrent = self->duckVolTarget;
                self->duckVolFadeDownArmed = 0;
                if (self->pendingSong != 0x13) {
                    sub_80017BC(self, self->pendingSong);
                    self->pendingSong = 0x13;
                }
            } else {
                self->duckVolCurrent -= 0x10;
            }
            sub_8039064(-1, self->duckVolCurrent);
        }
        sub_8038C88();
    }
}
asm(".align 2, 0");

/* Starts playing song `songIndex` (index into the 19-entry
 * `gStaticData_0816AA20` song-pointer table). First-time-only resets
 * the player state if it wasn't already stopped, then (re)initializes
 * the embedded GAX2 runtime player-state object at `self+0x58` -
 * genuinely nested engine-internal state `sub_80381FC`/`sub_8038538`
 * own, not independently reverse-engineered, so those writes stay raw
 * offset casts (see docs/audio.md). On success, ducks the music back
 * in (`sub_8001AD8`) and arms the per-tick GAX2 IRQ update
 * (`gUnknown_030007DD`). */
void sub_80017BC(struct AudioContext *self, u32 songIndex)
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
            sub_8039198();
            gUnknown_030007DD = zero;
        }
    }
    {
        u8 *gaxState = (u8 *)self + 0x58;

        sub_80381FC(gaxState);
        *(void **)((u8 *)self + 0x58) = (u8 *)self + 0x94;
        *(u32 *)((u8 *)self + 0x5c) = 0x2000;
        *(void **)((u8 *)self + 0x88) = gStaticData_0816AA20[songIndex];
        {
            u16 *p = (u16 *)((u8 *)self + 0x66);
            u8 zero2 = 0;

            *p = 3;
            *(void **)((u8 *)self + 0x84) = gStaticData_0855BCB4;
            *((u8 *)self + 0x90) = zero2;
        }
        if (sub_8038538(gaxState)) {
            self->currentSong = songIndex;
            *((u8 *)self + 0x54) = 0;
            sub_8001AD8(self);
            gUnknown_030007DD = 1;
            self->state = 1;
        }
    }
}
