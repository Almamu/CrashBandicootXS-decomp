#include "core.h"
#include "audio.h"

/* `PlaySfx` sits right after the matched `sub_80017BC` (src/audio/
 * music_player.c) and before the matched functions this file holds. */

#if NON_MATCHING
extern u32 gUnknown_030007FC;
extern s32 sub_8038E74(u32 handle, s32 channel, s32 pitchOffset, s32 priority);
extern void sub_80390F8(s32 channel, u32 volume);

/* `PlaySfx(context, sfxId, volumeParam)` - see docs/audio.md's "Sound
 * effects" section (identified there as `sub_8001854`, called ~264
 * times across gameplay code). Looks up `sfxId` in the 99-entry
 * `gStaticData_0816AA6C` table, steals a mixing voice via
 * `sub_8038E74` (retrying once with the alternate slot on failure - a
 * two-channel round-robin toggled by `gUnknown_030007FC`), and plays
 * it at a volume scaled by both the table's own base volume and the
 * context's `sfxVolume`. Records which of the 2 round-robin slots
 * last played `sfxId` in `lastSfxId`, for `sub_80019A8`'s
 * stop-if-playing scan.
 *
 * Parked: every instruction matches except the prologue's register
 * save order. The ROM does `mov sb,r0` (cache `self`) immediately,
 * before `mov sl,r1` (cache `id`); this compiler always defers the
 * `self` save to just before `self->state` is first dereferenced
 * (the point `r0` actually gets clobbered) once `self` is bound to an
 * explicit high-register variable - tried an inline-asm register
 * "touch" barrier right after binding it (forces the save, but as a
 * literal extra instruction, not earlier scheduling), reordering the
 * declaration relative to other locals, and leaving `self` unbound
 * (register allocator then picks the right registers late instead of
 * early, and a different pair besides). Every combination reproduces
 * the ROM's exact instruction stream except this one prologue
 * ordering - same class of gap as sub_80014A4's loop-invariant-hoisting
 * entry in this doc. */
void PlaySfx(struct AudioContext *self, u32 id, u32 volumeParam)
{
    s32 isPlaying;
    register struct AudioContext *pself asm("r9") = self;

    isPlaying = 0;
    if (pself->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        u32 handle = gStaticData_0816AA6C[id].slotId;

        if (handle != 0) {
            u32 toggle = gUnknown_030007FC;
            u32 chanArg = gStaticData_0816AA6C[id].chanArg;
            s32 voice = sub_8038E74(handle, toggle, chanArg, -1);

            if (voice == -1) {
                toggle = gUnknown_030007FC ^ 1;
                gUnknown_030007FC = toggle;
                voice = sub_8038E74(handle, toggle, chanArg, -1);
            }
            if (voice != -1) {
                u32 baseVolume = gStaticData_0816AA6C[id].baseVolume;
                register struct AudioContext *p2 asm("r2") = pself;
                u32 volume = (baseVolume * p2->sfxVolume) * volumeParam >> 0x10;

                sub_80390F8(voice, volume);
                pself->lastSfxId[gUnknown_030007FC] = id;
                gUnknown_030007FC ^= 1;
            }
        }
    }
}
#endif /* NON_MATCHING */

extern u32 gUnknown_0300082C;
extern void sub_80390F8(s32 channel, u32 volume);
extern void sub_8038FD0(s32 channel);
extern s32 sub_8038E74(u32 handle, s32 channel, s32 pitchOffset, s32 priority);

/* Per-tick update of the "ambient" (looping/crossfaded, as opposed to
 * `PlaySfx`'s one-shot) sound-effect channel: ramps `field_34` (the
 * channel's live volume) toward `activeSfx.volume`, and once
 * `gUnknown_0300082C` passes `activeSfx.deadline`, fades the channel
 * out over several ticks before promoting the queued `pendingSfx`
 * record into `activeSfx` (a 12-byte struct copy - see
 * include/audio.h) and triggering it via `sub_8038E74`. */
void sub_800190C(struct AudioContext *self)
{
    u32 now;

    if (self->activeSfx.id == 0x63) {
        return;
    }
    now = gUnknown_0300082C;
    if (now >= self->activeSfx.deadline) {
        register s32 v asm("r0");

        self->activeSfx.volume = 0;
        v = self->field_34;
        v -= 0x10;
        self->field_34 = v;
        if (v > 0) {
            sub_80390F8(2, *(volatile s32 *)&self->field_34);
            return;
        }
        self->field_34 = 0;
        sub_8038FD0(2);
        self->activeSfx = self->pendingSfx;
        if (self->activeSfx.id != 0x63) {
            u32 handle;

            self->pendingSfx.id = 0x63;
            handle = gStaticData_0816AA6C[self->activeSfx.id].slotId;
            sub_8038E74(handle, 2, 0, -1);
        }
        sub_80390F8(2, self->field_34);
        return;
    }
    if (self->field_34 < self->activeSfx.volume) {
        self->field_34 += 0x10;
        if (self->field_34 > self->activeSfx.volume) {
            self->field_34 = self->activeSfx.volume;
        }
        sub_80390F8(2, self->field_34);
        return;
    }
    if (self->field_34 <= self->activeSfx.volume) {
        return;
    }
    self->field_34 -= 0x10;
    if (self->field_34 < self->activeSfx.volume) {
        self->field_34 = self->activeSfx.volume;
    }
    sub_80390F8(2, self->field_34);
}

/* Stop-if-playing companion to `PlaySfx`: mutes whichever of the 2
 * round-robin channel slots last played `id` (`lastSfxId[0]`/`[1]`),
 * via `sub_8038FD0(slotIndex)` (a GAX2 per-channel mute, see
 * docs/rom_map.md's "Resolved sub_80019A8" section). */
void sub_80019A8(struct AudioContext *self, u32 id)
{
    s32 i;

    for (i = 0; i <= 1; i++) {
        if (self->lastSfxId[i] == id) {
            sub_8038FD0(i);
        }
    }
}

/* Resets the ambient-sfx channel (both `activeSfx`/`pendingSfx`
 * records cleared to the `0x63` "none" sentinel) and mutes it. */
void sub_80019CC(struct AudioContext *self)
{
    self->pendingSfx.id = 0x63;
    self->activeSfx.id = 0x63;
    self->field_34 = 0;
    self->pendingSfx.volume = 0;
    self->activeSfx.volume = 0;
    sub_8038FD0(2);
}

/* Forces the ambient-sfx channel's current record to expire
 * immediately (deadline = now) and drops any queued pending record -
 * the next `sub_800190C` tick will fade it out and go idle. */
void sub_80019E8(struct AudioContext *self)
{
    self->pendingSfx.id = 0x63;
    self->activeSfx.deadline = gUnknown_0300082C;
}
asm(".align 2, 0");
