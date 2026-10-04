#include "core.h"
#include "audio.h"

/* `PlayAmbientSfx` sits right after the matched `StopAmbientSfx`
 * (src/audio/sfx_ambient.c) and before the matched functions this
 * file holds - the rest of the `AudioContext` accessor/state-machine
 * cluster (play/pause/stop, the two fade-envelope arm/setter pairs,
 * the constructor). */

extern u32 gUnknown_0300082C;
extern s32 GAX_fx_ex(u32 handle, s32 channel, s32 pitchOffset, s32 priority);
extern void GAX_set_fx_volume(s32 channel, u32 volume);

/* Requests the ambient-sfx channel play `id` for `frameOffset` frames
 * (relative to `gUnknown_0300082C`) at a volume derived from
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
            self->activeSfx.deadline = gUnknown_0300082C + frameOffset;
            self->activeSfx.volume = volume;
        } else if (volume >= self->activeSfx.volume) {
            if (cur == id) {
                self->activeSfx.deadline = gUnknown_0300082C + frameOffset;
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
                now = gUnknown_0300082C;
                self->pendingSfx.deadline = now + frameOffset;
                self->pendingSfx.volume = volume;
                self->activeSfx.deadline = now;
            }
        }
    }
}

extern void GAX_set_music_volume(s32 channel, u32 volume);
extern void GAX_stop(void);
extern u8 gGaxIrqEnabled;
extern void GAX_resume(void);
extern void GAX_play(void);
extern void GAX_pause(void);
extern void WaitForVBlank(void);
extern void sub_80016D0(u8 *address);
extern void IrqRestoreHandler(s32 interruptIndex);
extern void sub_80017BC(struct AudioContext *self, u32 songIndex);

/* currentSong getter. */
u32 sub_8001AB8(struct AudioContext *self)
{
    return self->currentSong;
}

/* sfxVolume getter. */
u32 sub_8001ABC(struct AudioContext *self)
{
    return self->sfxVolume;
}

/* duckVolDefault getter. */
u32 sub_8001AC0(struct AudioContext *self)
{
    return self->duckVolDefault;
}

/* Arms a duck-out: sets the ducking fade target and the "fade down"
 * direction flag. */
void sub_8001AC4(struct AudioContext *self, u32 value)
{
    self->duckVolTarget = value;
    self->duckVolFadeUpArmed = 0;
    self->duckVolFadeDownArmed = 1;
}

/* Arms a duck-in back to `duckVolDefault` (the last value explicitly
 * set via `sub_8001B30`). */
void sub_8001AD8(struct AudioContext *self)
{
    self->duckVolTarget = self->duckVolDefault;
    self->duckVolFadeUpArmed = 1;
    self->duckVolFadeDownArmed = 0;
}

/* Arms a fade-out on the independent `musicVolCurrent`/`Target` pair. */
void sub_8001AEC(struct AudioContext *self, u32 value)
{
    self->musicVolTarget = value;
    self->musicVolFadeUpArmed = 0;
    self->musicVolFadeDownArmed = 1;
}

/* Arms a fade-in on the independent `musicVolCurrent`/`Target` pair. */
void sub_8001B00(struct AudioContext *self, u32 value)
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
 * `duckVolDefault` a later `sub_8001AD8` duck-in restores to. */
void sub_8001B30(struct AudioContext *self, u32 value)
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
void sub_8001B50(struct AudioContext *self, u32 value)
{
    self->sfxVolume = value;
}

/* Requests song `id` to play: if nothing is playing yet, starts it
 * immediately (`sub_80017BC`); otherwise, if it's not already the
 * current or already-queued song, queues it as `pendingSong` and arms
 * a duck-out (`sub_8001AC4`) - `sub_80016EC`'s per-tick update starts
 * the queued song once the duck-out fade completes. */
void sub_8001B54(struct AudioContext *self, u32 id)
{
    s32 isPlaying = 0;

    if (self->state == 1) {
        isPlaying = 1;
    }
    if (!isPlaying) {
        sub_80017BC(self, id);
        return;
    }
    if (id == self->currentSong) {
        return;
    }
    if (id == self->pendingSong) {
        return;
    }
    self->pendingSong = id;
    sub_8001AC4(self, 0);
}

/* Resumes a paused song. */
void sub_8001B88(struct AudioContext *self)
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
void sub_8001BAC(struct AudioContext *self)
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
void sub_8001BD4(struct AudioContext *self)
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
void sub_8001C04(struct AudioContext *self, u32 flags)
{
    sub_8001BD4(self);
    gGaxIrqEnabled = 0;
    if (flags & 1) {
        sub_80016D0((u8 *)self);
    }
}

/* Constructor: resets every field to its idle default (both volume
 * pairs to `0x100` = 1.0 in Q8.8, both song slots to the `0x13` "none"
 * sentinel, every fade-direction flag cleared) and returns `self`. */
struct AudioContext *sub_8001C2C(struct AudioContext *self)
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
void sub_8001C64(void)
{
    register vu8 *dispstat asm("r1") = (vu8 *)REG_ADDR_DISPSTAT;
    u8 tmp = DISPSTAT_VCOUNT_INTR;

    *dispstat &= ~tmp;
    IrqRestoreHandler(INTR_INDEX_VCOUNT);
}
asm(".align 2, 0");
