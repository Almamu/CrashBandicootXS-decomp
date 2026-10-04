#include "core.h"
#include "actor.h"
#include "pause_screen_results.h"

extern s32 FormatDecimal(s32 value, void *dest);
extern void *gAudioContext;
extern void SetMusicVolume(void *self, u32 value);
extern void SetSfxVolume(void *self, u32 value);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

/* One entry of the per-row record array at `field_14` (8-byte stride;
 * `type` 4/5 are the two editable-percentage rows). */
struct pause_row {
    void *label;
    s32 type;
};

#define ROW_TYPE(self) (((struct pause_row *)(self)->field_14)[(self)->field_18].type)

/* Writes " <NN%>" into `buf`: the digits of `value` land at `buf + 2`
 * via FormatDecimal, which returns how many it wrote. */
static inline void format_pct(u8 *buf, s32 value)
{
    s32 len;

    buf[0] = ' ';
    buf[1] = '<';
    len = FormatDecimal(value, &buf[2]);
    buf[len + 2] = '%';
    buf[len + 3] = '>';
    buf[len + 4] = '\0';
}

/* One of a matched pair (PauseMenuVolumeUp increments the other way): if the
 * current row is an "editable count" row (type 4 or 5) and its count
 * (`musicVolume`/`soundVolume` respectively, 0-20 in steps of 5%) is
 * non-zero, decrements it, formats the " <NN%>" scratch string into
 * `musicVolumeText`/`soundVolumeText`, and pushes the new level (`(count << 8 | 1) / 20`)
 * through the matching AudioContext setter (`SetMusicVolume`/`SetSfxVolume`
 * - see src/audio/audio_context.c). Only the type-5/`soundVolume` branch
 * also plays the standard SFX cue. */
void PauseMenuVolumeDown(struct pause_screen_results *self)
{
    s32 count;

    switch (ROW_TYPE(self)) {
    case 4:
        count = self->musicVolume;
        if (count != 0) {
            count--;
            self->musicVolume = count;
            format_pct(self->musicVolumeText, count * 5);
            SetMusicVolume(gAudioContext, ((self->musicVolume << 8) + 1) / 20);
        }
        break;
    case 5:
        count = self->soundVolume;
        if (count != 0) {
            count--;
            self->soundVolume = count;
            format_pct(self->soundVolumeText, count * 5);
            SetSfxVolume(gAudioContext, ((self->soundVolume << 8) + 1) / 20);
            PlaySfx(gAudioContext, 0xe, 0x100);
        }
        break;
    }
}

/* Counterpart to PauseMenuVolumeDown above: increments (capped at 0x14)
 * instead of decrementing. */
void PauseMenuVolumeUp(struct pause_screen_results *self)
{
    s32 count;

    switch (ROW_TYPE(self)) {
    case 4:
        count = self->musicVolume;
        if (count <= 0x13) {
            count++;
            self->musicVolume = count;
            format_pct(self->musicVolumeText, count * 5);
            SetMusicVolume(gAudioContext, ((self->musicVolume << 8) + 1) / 20);
        }
        break;
    case 5:
        count = self->soundVolume;
        if (count <= 0x13) {
            count++;
            self->soundVolume = count;
            format_pct(self->soundVolumeText, count * 5);
            SetSfxVolume(gAudioContext, ((self->soundVolume << 8) + 1) / 20);
            PlaySfx(gAudioContext, 0xe, 0x100);
        }
        break;
    }
}
