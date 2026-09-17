#include "core.h"
#include "icon_manager.h"

/* NOT YET BYTE-MATCHING - see docs/matching/issue-46-hud-icon-widget.md
 * for the full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_2_20_8a78.s) is used otherwise. */
#if NON_MATCHING

extern void sub_803A94C(void *src, void *dst, s32 control);
extern u8 gStaticData_087E4DAC[];

/* Constructor variant used for a widget that's never assigned its own
 * data tables past `record` (the line-height/space-width/glyph fields
 * stay whatever the caller already set) - just the OAM-scratch zero and
 * cursor/margin reset shared with InitHudIconWidgetA/B
 * (asm/code_3_2_20_85c4.s).
 *
 * Residual gap: same `posX`/`posY` zero-init address/store-order
 * mismatch as InitHudIconWidgetA/B - see that function's own comment
 * for what was tried. */
struct icon_manager *sub_8028A78(struct icon_manager *self)
{
    s32 zero;

    self->record = (struct icon_record *)gStaticData_087E4DAC;
    self->posX = 0;
    self->posY = 0;
    self->field_118 = 0;
    self->field_12c = 0;
    zero = 0;
    sub_803A94C(&zero, self, 0x05000002);
    return self;
}

#endif /* NON_MATCHING */
