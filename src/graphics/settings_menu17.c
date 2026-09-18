#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "pause_screen_results.h"

extern s32 sub_8026F38(s32 arg0);
extern s32 sub_802332C(void *arg0);
extern void *gUnknown_030012C0;
extern u8 gStaticData_0816C86C[];
extern s32 sub_800697C(void *arg0);
extern s32 sub_80060AC(s32 value, void *dest);
extern void sub_80060F8(s32 arg0, s32 arg1, u8 *out);
extern s32 sub_8001AC0(void *arg0);
extern s32 sub_8001ABC(void *arg0);
extern struct AudioContext *gUnknown_030012BC;
extern void sub_8005A78(struct pause_screen_results *self);
extern void sub_8005AE8(struct pause_screen_results *self);
extern void sub_8005B80(struct pause_screen_results *self);
extern void sub_8005C58(struct pause_screen_results *self);
extern void sub_8005D44(struct pause_screen_results *self);

/* The composite pause/options screen's "results" sub-region
 * constructor: resolves the current level's name/index label
 * (`field_70`/`field_74`/`buf78` - " N" for levels 0-0x13, blank for
 * anything past that), formats the row-stats completion percentage
 * (`buf41`), formats the two BG scroll-speed settings (`sub_8001AC0`/
 * `sub_8001ABC` on the shared AudioContext, each scaled `(v+0xc)*20/
 * 256` into `field_60`/`field_64`, then " <NN%>"-formatted into
 * `buf57`/`buf4f`), then builds the five icon-widget sub-groups in
 * order (`sub_8005A78`/`AE8`/`B80`/`C58`/`D44`, all already matched or
 * parked - src/graphics/settings_menu6.c). */
void sub_800599C(struct pause_screen_results *self)
{
    s32 levelIdx = sub_802332C(gUnknown_030012C0);
    u32 labelId = *(u32 *)(gStaticData_0816C86C + levelIdx * 0x24);

    self->field_70 = (void *)sub_8026F38(labelId);

    if (levelIdx <= 0x13) {
        self->field_74 = (void *)sub_8026F38(0);
        self->buf78[0] = ' ';
        sub_80060AC(levelIdx + 1, &self->buf78[1]);
    } else {
        self->field_74 = 0;
    }

    {
        s32 count = sub_80060AC(sub_800697C(self->field_10), self->buf41);
        self->buf41[count] = '%';
        self->buf41[count + 1] = 0;
    }

    self->field_60 = (sub_8001AC0(gUnknown_030012BC) + 0xc) * 20 / 256;
    self->field_64 = (sub_8001ABC(gUnknown_030012BC) + 0xc) * 20 / 256;

    sub_80060F8((s32)self, self->field_60, self->buf57);
    sub_80060F8((s32)self, self->field_64, self->buf4f);

    sub_8005A78(self);
    sub_8005AE8(self);
    sub_8005B80(self);
    sub_8005C58(self);
    sub_8005D44(self);
}
