#include "core.h"
#include "audio.h"

/* `PlaySfx` sits right after the matched `StartSong` (src/audio/
 * music_player.c) and before the other functions this file holds. */

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
