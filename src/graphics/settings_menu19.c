#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "pause_screen_results.h"

extern s32 GetUiText(s32 arg0);
extern s32 GetCurrentLevel(void *arg0);
extern void *gLevelState;
extern u8 gLevelTable[];
extern s32 GetCompletionPercent(void *arg0);
extern s32 FormatDecimal(s32 value, void *dest);
extern void FormatVolumePercent(s32 arg0, s32 arg1, u8 *out);
extern s32 GetMusicVolume(void *arg0);
extern s32 GetSfxVolume(void *arg0);
extern struct AudioContext *gAudioContext;
extern void InitPauseCrystalsPage(struct pause_screen_results *self);
extern void InitPausePowersPage(struct pause_screen_results *self);
extern void InitPauseGemsPage(struct pause_screen_results *self);
extern void InitPauseRelicsPage(struct pause_screen_results *self);
extern void InitPauseTimeTrialPage(struct pause_screen_results *self);

/* The composite pause/options screen's "results" sub-region
 * constructor: resolves the current level's name/index label
 * (`field_70`/`field_74`/`buf78` - " N" for levels 0-0x13, blank for
 * anything past that), formats the row-stats completion percentage
 * (`buf41`), formats the two BG scroll-speed settings (`GetMusicVolume`/
 * `GetSfxVolume` on the shared AudioContext, each scaled `(v+0xc)*20/
 * 256` into `musicVolume`/`soundVolume`, then " <NN%>"-formatted into
 * `musicVolumeText`/`soundVolumeText`), then builds the five icon-widget sub-groups in
 * order (`InitPauseCrystalsPage`/`AE8`/`B80`/`C58`/`D44`, all already matched or
 * parked - src/graphics/settings_menu6.c). */
void InitPauseMenuInfo(struct pause_screen_results *self)
{
    s32 levelIdx = GetCurrentLevel(gLevelState);
    u32 labelId = *(u32 *)(gLevelTable + levelIdx * 0x24);

    self->field_70 = (void *)GetUiText(labelId);

    if (levelIdx <= 0x13) {
        self->field_74 = (void *)GetUiText(0);
        self->buf78[0] = ' ';
        FormatDecimal(levelIdx + 1, &self->buf78[1]);
    } else {
        self->field_74 = 0;
    }

    {
        s32 count = FormatDecimal(GetCompletionPercent(self->field_10), self->buf41);
        self->buf41[count] = '%';
        self->buf41[count + 1] = 0;
    }

    self->musicVolume = (GetMusicVolume(gAudioContext) + 0xc) * 20 / 256;
    self->soundVolume = (GetSfxVolume(gAudioContext) + 0xc) * 20 / 256;

    FormatVolumePercent((s32)self, self->musicVolume, self->musicVolumeText);
    FormatVolumePercent((s32)self, self->soundVolume, self->soundVolumeText);

    InitPauseCrystalsPage(self);
    InitPausePowersPage(self);
    InitPauseGemsPage(self);
    InitPauseRelicsPage(self);
    InitPauseTimeTrialPage(self);
}
