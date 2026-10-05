#include "core.h"
#include "audio.h"
#include "irq.h"

/* The first matched code in the Shin'en GAX2 wrapper layer (the engine
 * itself is still raw in asm/code_3.s - see docs/audio.md). These two
 * functions drive music playback on the shared `AudioContext` object
 * (`*gAudioContext` in the ROM - see include/audio.h and
 * docs/rom_map.md's "Found the origin point" section): `UpdateAudio`
 * is the per-tick fade update, `StartSong` starts a song. `PlaySfx`
 * (the very next function in ROM order) stays raw here - see
 * src/audio/audio.c's doc comment. */

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

/* `PlaySfx` sits right after the matched `StartSong` (src/audio/
 * audio.c) and before the other functions this file holds. */

extern u32 gSfxVoiceToggle;

/* `PlaySfx(context, sfxId, volumeParam)` - see docs/audio.md's "Sound
 * effects" section (identified there as `sub_8001854`, called ~264
 * times across gameplay code). Looks up `sfxId` in the 99-entry
 * `gSfxTable` table, steals a mixing voice via
 * `GAX_fx_ex` (retrying once with the alternate slot on failure - a
 * two-channel round-robin toggled by `gSfxVoiceToggle`), and plays
 * it at a volume scaled by both the table's own base volume and the
 * context's `sfxVolume`. Records which of the 2 round-robin slots
 * last played `sfxId` in `lastSfxId`, for `StopSfx`'s
 * stop-if-playing scan.
 *
 * Matched in the near-miss polish pass (docs/matching/
 * near-miss-polish.md). The older draft pinned `self` to r9, which
 * made its save a body statement placed after the other parameter
 * copies. Unpinned, the instruction stream is the ROM's, but global-alloc
 * ranks `self` > `id` > `&gSfxVoiceToggle` where the ROM has the
 * address first (r8, then r9, then sl). The empty
 * `asm("" : : "r"(&gSfxVoiceToggle))` emits nothing; it adds one
 * reference to the address pseudo, lifting its priority to the top. */
void PlaySfx(struct AudioContext *self, u32 id, u32 volumeParam)
{
    s32 isPlaying;
    struct AudioContext *pself = self;

    isPlaying = 0;
    if (pself->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        u32 handle = gSfxTable[id].slotId;

        if (handle != 0) {
            u32 toggle = gSfxVoiceToggle;
            u32 chanArg = gSfxTable[id].chanArg;
            s32 voice = GAX_fx_ex(handle, toggle, chanArg, -1);

            /* No code: one extra use of the address for global-alloc. */
            asm("" : : "r"(&gSfxVoiceToggle));
            if (voice == -1) {
                toggle = gSfxVoiceToggle ^ 1;
                gSfxVoiceToggle = toggle;
                voice = GAX_fx_ex(handle, toggle, chanArg, -1);
            }
            if (voice != -1) {
                u32 baseVolume = gSfxTable[id].baseVolume;
                register struct AudioContext *p2 asm("r2") = pself;
                u32 volume = (baseVolume * p2->sfxVolume) * volumeParam >> 0x10;

                GAX_set_fx_volume(voice, volume);
                pself->lastSfxId[gSfxVoiceToggle] = id;
                gSfxVoiceToggle ^= 1;
            }
        }
    }
}

extern u32 gRoomFrameCount;

/* Per-tick update of the "ambient" (looping/crossfaded, as opposed to
 * `PlaySfx`'s one-shot) sound-effect channel: ramps `ambientSfxVolume` (the
 * channel's live volume) toward `activeSfx.volume`, and once
 * `gRoomFrameCount` passes `activeSfx.deadline`, fades the channel
 * out over several ticks before promoting the queued `pendingSfx`
 * record into `activeSfx` (a 12-byte struct copy - see
 * include/audio.h) and triggering it via `GAX_fx_ex`. */
void TickAmbientSfx(struct AudioContext *self)
{
    u32 now;

    if (self->activeSfx.id == 0x63) {
        return;
    }
    now = gRoomFrameCount;
    if (now >= self->activeSfx.deadline) {
        register s32 v asm("r0");

        self->activeSfx.volume = 0;
        v = self->ambientSfxVolume;
        v -= 0x10;
        self->ambientSfxVolume = v;
        if (v > 0) {
            GAX_set_fx_volume(2, *(volatile s32 *)&self->ambientSfxVolume);
            return;
        }
        self->ambientSfxVolume = 0;
        GAX_stop_fx(2);
        self->activeSfx = self->pendingSfx;
        if (self->activeSfx.id != 0x63) {
            u32 handle;

            self->pendingSfx.id = 0x63;
            handle = gSfxTable[self->activeSfx.id].slotId;
            GAX_fx_ex(handle, 2, 0, -1);
        }
        GAX_set_fx_volume(2, self->ambientSfxVolume);
        return;
    }
    if (self->ambientSfxVolume < self->activeSfx.volume) {
        self->ambientSfxVolume += 0x10;
        if (self->ambientSfxVolume > self->activeSfx.volume) {
            self->ambientSfxVolume = self->activeSfx.volume;
        }
        GAX_set_fx_volume(2, self->ambientSfxVolume);
        return;
    }
    if (self->ambientSfxVolume <= self->activeSfx.volume) {
        return;
    }
    self->ambientSfxVolume -= 0x10;
    if (self->ambientSfxVolume < self->activeSfx.volume) {
        self->ambientSfxVolume = self->activeSfx.volume;
    }
    GAX_set_fx_volume(2, self->ambientSfxVolume);
}

/* Stop-if-playing companion to `PlaySfx`: mutes whichever of the 2
 * round-robin channel slots last played `id` (`lastSfxId[0]`/`[1]`),
 * via `GAX_stop_fx(slotIndex)` (a GAX2 per-channel mute, see
 * docs/rom_map.md's "Resolved StopSfx" section). */
void StopSfx(struct AudioContext *self, u32 id)
{
    s32 i;

    for (i = 0; i <= 1; i++) {
        if (self->lastSfxId[i] == id) {
            GAX_stop_fx(i);
        }
    }
}

/* Resets the ambient-sfx channel (both `activeSfx`/`pendingSfx`
 * records cleared to the `0x63` "none" sentinel) and mutes it. */
void ResetAmbientSfx(struct AudioContext *self)
{
    self->pendingSfx.id = 0x63;
    self->activeSfx.id = 0x63;
    self->ambientSfxVolume = 0;
    self->pendingSfx.volume = 0;
    self->activeSfx.volume = 0;
    GAX_stop_fx(2);
}

/* Forces the ambient-sfx channel's current record to expire
 * immediately (deadline = now) and drops any queued pending record -
 * the next `TickAmbientSfx` tick will fade it out and go idle. */
void StopAmbientSfx(struct AudioContext *self)
{
    self->pendingSfx.id = 0x63;
    self->activeSfx.deadline = gRoomFrameCount;
}
asm(".align 2, 0");

/* `PlayAmbientSfx` sits right after the matched `StopAmbientSfx`
 * (src/audio/audio.c) and before the matched functions this
 * file holds - the rest of the `AudioContext` accessor/state-machine
 * cluster (play/pause/stop, the two fade-envelope arm/setter pairs,
 * the constructor). */

/* Requests the ambient-sfx channel play `id` for `frameOffset` frames
 * (relative to `gRoomFrameCount`) at a volume derived from
 * `volumeMul`/the table's own base volume/`sfxVolume`, same formula as
 * `PlaySfx`. If nothing is currently active, starts it immediately;
 * if something louder-or-equal is already active with the same `id`,
 * refreshes its deadline/volume (optionally retriggering the voice via
 * `forceFlag`); if a different, louder request comes in, queues it as
 * `pendingSfx` and forces the current record to expire on the very
 * next tick instead of playing over it.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The fifth argument is a one-byte struct passed by value
 * (the ROM's `add rX,sp,#0x14; ldrb` read), and the volume read is
 * `gSfxTable[id].baseVolume`, whose `base+8+offset` address
 * is simply what gcc emits for a non-zero field offset - the earlier
 * note blamed a CSE decision that is not there. */
struct sfx_byte_arg {
    u8 v;
} __attribute__((packed));

void PlayAmbientSfx(struct AudioContext *self, u32 id, u32 frameOffset, s32 volumeMul, struct sfx_byte_arg force)
{
    u8 forceFlag = force.v;
    u32 handle = gSfxTable[id].slotId;

    if (handle != 0 && volumeMul > 0) {
        s32 volume = (gSfxTable[id].baseVolume * volumeMul) * self->sfxVolume >> 16;
        u32 cur = self->activeSfx.id;

        if (cur == 0x63) {
            GAX_fx_ex(handle, 2, 0, -1);
            GAX_set_fx_volume(2, self->ambientSfxVolume);
            self->activeSfx.id = id;
            self->activeSfx.deadline = gRoomFrameCount + frameOffset;
            self->activeSfx.volume = volume;
        } else if (volume >= self->activeSfx.volume) {
            if (cur == id) {
                self->activeSfx.deadline = gRoomFrameCount + frameOffset;
                self->activeSfx.volume = volume;
                if (forceFlag) {
                    GAX_fx_ex(handle, 2, 0, -1);
                    GAX_set_fx_volume(2, self->ambientSfxVolume);
                }
                self->pendingSfx.id = 0x63;
                self->pendingSfx.volume = 0;
            } else {
                u32 now;

                self->pendingSfx.id = id;
                now = gRoomFrameCount;
                self->pendingSfx.deadline = now + frameOffset;
                self->pendingSfx.volume = volume;
                self->activeSfx.deadline = now;
            }
        }
    }
}

extern void WaitForVBlank(void);
extern void IwramFree(u8 *address);
extern void IrqRestoreHandler(s32 interruptIndex);
extern void StartSong(struct AudioContext *self, u32 songIndex);

/* currentSong getter. */
u32 GetCurrentSong(struct AudioContext *self)
{
    return self->currentSong;
}

/* sfxVolume getter. */
u32 GetSfxVolume(struct AudioContext *self)
{
    return self->sfxVolume;
}

/* duckVolDefault getter. */
u32 GetMusicVolume(struct AudioContext *self)
{
    return self->duckVolDefault;
}

/* Arms a duck-out: sets the ducking fade target and the "fade down"
 * direction flag. */
void FadeOutMusic(struct AudioContext *self, u32 value)
{
    self->duckVolTarget = value;
    self->duckVolFadeUpArmed = 0;
    self->duckVolFadeDownArmed = 1;
}

/* Arms a duck-in back to `duckVolDefault` (the last value explicitly
 * set via `SetMusicVolume`). */
void FadeInMusic(struct AudioContext *self)
{
    self->duckVolTarget = self->duckVolDefault;
    self->duckVolFadeUpArmed = 1;
    self->duckVolFadeDownArmed = 0;
}

/* Arms a fade-out on the independent `musicVolCurrent`/`Target` pair. */
void FadeOutMasterVolume(struct AudioContext *self, u32 value)
{
    self->musicVolTarget = value;
    self->musicVolFadeUpArmed = 0;
    self->musicVolFadeDownArmed = 1;
}

/* Arms a fade-in on the independent `musicVolCurrent`/`Target` pair. */
void FadeInMasterVolume(struct AudioContext *self, u32 value)
{
    self->musicVolTarget = value;
    self->musicVolFadeUpArmed = 1;
    self->musicVolFadeDownArmed = 0;
}

/* Setter for `field_30`, hardware-mirrored (truncated) into the
 * embedded GAX object's own field at `self+0x62` while a song is
 * playing - see include/audio.h. */
void sub_8001B14(struct AudioContext *self, u32 value)
{
    s32 isPlaying;

    self->field_30 = value;
    isPlaying = 0;
    if (self->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        *(vu16 *)((u8 *)self + 0x62) = value;
    }
}

/* Immediate (non-fading) music-volume setter - also becomes the new
 * `duckVolDefault` a later `FadeInMusic` duck-in restores to. */
void SetMusicVolume(struct AudioContext *self, u32 value)
{
    s32 isPlaying;

    self->duckVolCurrent = value;
    self->duckVolDefault = value;
    isPlaying = 0;
    if (self->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        GAX_set_music_volume(-1, value);
    }
}

/* sfxVolume setter. */
void SetSfxVolume(struct AudioContext *self, u32 value)
{
    self->sfxVolume = value;
}

/* Requests song `id` to play: if nothing is playing yet, starts it
 * immediately (`StartSong`); otherwise, if it's not already the
 * current or already-queued song, queues it as `pendingSong` and arms
 * a duck-out (`FadeOutMusic`) - `UpdateAudio`'s per-tick update starts
 * the queued song once the duck-out fade completes. */
void PlaySong(struct AudioContext *self, u32 id)
{
    s32 isPlaying = 0;

    if (self->state == 1) {
        isPlaying = 1;
    }
    if (!isPlaying) {
        StartSong(self, id);
        return;
    }
    if (id == self->currentSong) {
        return;
    }
    if (id == self->pendingSong) {
        return;
    }
    self->pendingSong = id;
    FadeOutMusic(self, 0);
}

/* Resumes a paused song. */
void ResumeSong(struct AudioContext *self)
{
    s32 isPaused;

    isPaused = 0;
    if (self->state == 2) {
        isPaused = 1;
    }
    if (isPaused) {
        WaitForVBlank();
        GAX_resume();
        self->state = 1;
    }
}

/* Pauses a playing song. */
void PauseSong(struct AudioContext *self)
{
    s32 isPlaying;

    isPlaying = 0;
    if (self->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        self->state = 2;
        WaitForVBlank();
        GAX_play();
        GAX_pause();
    }
}

/* Stops the currently playing/paused song (a no-op if already
 * stopped) and disarms the per-tick GAX2 IRQ update. */
void StopSong(struct AudioContext *self)
{
    register s32 isStopped asm("r2");
    register s32 zero asm("r4");

    isStopped = 0;
    if (self->state == 0) {
        isStopped = 1;
    }
    zero = isStopped;
    if (zero == 0) {
        self->pendingSong = 0x13;
        self->currentSong = 0x13;
        self->state = zero;
        GAX_stop();
        gGaxIrqEnabled = zero;
    }
}

/* Stops the song, and if `flags` bit 0 is set, also frees `self`. */
void DestroyAudioContext(struct AudioContext *self, u32 flags)
{
    StopSong(self);
    gGaxIrqEnabled = 0;
    if (flags & 1) {
        IwramFree((u8 *)self);
    }
}

/* Constructor: resets every field to its idle default (both volume
 * pairs to `0x100` = 1.0 in Q8.8, both song slots to the `0x13` "none"
 * sentinel, every fade-direction flag cleared) and returns `self`. */
struct AudioContext *InitAudioContext(struct AudioContext *self)
{
    self->state = 0;
    self->pendingSong = 0x13;
    self->currentSong = 0x13;
    self->musicVolCurrent = 0x100;
    self->duckVolCurrent = 0x100;
    self->duckVolDefault = 0x100;
    self->sfxVolume = 0x100;
    self->ambientSfxVolume = 0;
    self->activeSfx.id = 0x63;
    self->musicVolFadeDownArmed = 0;
    self->musicVolFadeUpArmed = 0;
    self->duckVolFadeDownArmed = 0;
    self->duckVolFadeUpArmed = 0;
    self->field_54 = 0;
    self->field_30 = 0;
    return self;
}

/* Disables the GBA's V-Count interrupt - a counterpart to
 * `DisableVBlankHandler` (VBlank) in src/system/irq.c. */
void DisableMusicVCountIrq(void)
{
    register vu8 *dispstat asm("r1") = (vu8 *)REG_ADDR_DISPSTAT;
    u8 tmp = DISPSTAT_VCOUNT_INTR;

    *dispstat &= ~tmp;
    IrqRestoreHandler(INTR_INDEX_VCOUNT);
}
asm(".align 2, 0");

extern void IrqSetHandler(s32 interruptIndex, irq_handler_t *fn);
extern struct AudioContext *gAudioContext;
extern void UpdateAudio(struct AudioContext *self);

void MusicVCountIrqHandler(void);

/* Installs `MusicVCountIrqHandler` as the VCount-IRQ handler and arms VCount IRQs
 * with a fixed trigger line (`0x35`) - the music player's per-tick fade
 * update (`UpdateAudio`, `src/audio/audio.c`) runs off this
 * VCount interrupt rather than VBlank. See that file's header comment,
 * which already anticipated this function. */
void EnableMusicVCountIrq(void)
{
    register vu8 *p asm("r1");
    register u8 v asm("r0");
    register u8 loaded asm("r2");

    IrqSetHandler(INTR_INDEX_VCOUNT, MusicVCountIrqHandler);
    p = (vu8 *)REG_ADDR_DISPSTAT;
    p[1] = 0x35;
    v = DISPSTAT_VCOUNT_INTR;
    loaded = *p;
    v |= loaded;
    *p = v;
}

/* The VCount-IRQ handler installed by `EnableMusicVCountIrq` above: just forwards
 * into the music player's per-tick fade update. */
void MusicVCountIrqHandler(void)
{
    UpdateAudio(gAudioContext);
}
