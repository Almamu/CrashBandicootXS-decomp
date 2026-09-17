#include "core.h"
#include "icon_manager.h"

/* Sits between the parked MeasureText (asm/code_3_2_20_8994.s) and the
 * parked sub_8028A78 (asm/code_3_2_20_8a78.s) - UploadHudTile/
 * sub_8028A30/sub_8028A40, GitHub issue #46. Same `struct icon_manager`
 * as hud_icon_widget.c/hud_icon_widget2.c/hud_icon_widget3.c/
 * hud_icon_widget5.c. */

extern void LoadTaggedAsset(void *asset, void *dest);
extern u8 *gUnknown_030012B8;
extern void ***gUnknown_030012D0;
extern s32 sub_8006DF8(u8 *cache, s32 recordId);

/* Uploads `field_128`'s referenced tile data to the OBJ VRAM slot
 * selected by `field_108`, recording the resulting tile-count-derived
 * shift (`>>13` of the asset's own header word) into `field_12c`. */
void UploadHudTile(struct icon_manager *self)
{
    void *asset = self->field_128;

    self->field_12c = *(u32 *)asset >> 13;
    LoadTaggedAsset(asset, (void *)(0x06010000 + (self->field_108 << 5)));
}

/* Sets the low nibble of `oam_scratch[5]` from `val`'s low byte - a
 * priority/attribute nibble selector, exact meaning not established. */
void sub_8028A30(struct icon_manager *self, u8 val)
{
    u32 shifted;
    register u8 mask asm("r2");
    register u8 field asm("r3");

    shifted = val << 4;
    mask = 0xF;
    asm volatile("" : "+r"(mask));
    field = self->oam_scratch[5];
    mask &= field;
    mask |= shifted;
    self->oam_scratch[5] = mask;
}

/* Looks up a tile-cache slot for the byte at
 * `(**gUnknown_030012D0)[0x1A4]`'s own `+0x14` field (see
 * docs/rom_map.md's `gStaticData_084A5600` investigation) via
 * `sub_8006DF8`, and folds the result into the same `oam_scratch[5]`
 * nibble sub_8028A30 sets above. */
void sub_8028A40(struct icon_manager *self, u32 unused)
{
    u8 *cache = gUnknown_030012B8;
    void *rec = *(void **)((u8 *)(**gUnknown_030012D0) + (0xD2 << 1));
    u8 field = ((u8 *)rec)[0x14];
    s32 slot = sub_8006DF8(cache, field);
    u32 shifted = slot << 4;
    register u8 mask asm("r1");
    register u8 b asm("r2");

    mask = 0xF;
    asm volatile("" : "+r"(mask));
    b = self->oam_scratch[5];
    mask &= b;
    mask |= shifted;
    self->oam_scratch[5] = mask;
}
