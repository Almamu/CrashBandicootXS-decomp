#include "core.h"
#include "icon_manager.h"
#include "vram_pool.h"

/* Same "fade overlay" self object as `actor_part87.c` (`sub_803472C`) -
 * redeclared locally here per this project's minimal-local-type
 * convention for a type already anchored in another translation unit
 * (see e.g. settings_menu10.c's own `struct sub_8006700_actor`
 * comment). Only the fields this file actually touches are named. */
struct fade_overlay {
    u8 unused_00[0xc];
    u16 dispcnt; /* 0x0c */
    u8 unused_0e[2];
    u32 unused_10;      /* 0x10 */
    u8 unused_14[4];
    struct icon_manager *icons; /* 0x18 */
};

extern struct vram_upload_cursor *gUnknown_030012FC;
extern void sub_8006C4C(struct vram_upload_cursor *self);
extern struct icon_manager *gUnknown_030012DC;
extern void *_call_via_r1(void *arg0, void *arg1);
extern s32 sub_8006C58(struct vram_upload_cursor *self, s32 size);
extern void sub_8006C30(struct vram_upload_cursor *self);
extern struct tile_asset_cache *gUnknown_030012B8;
extern void sub_8006EA8(struct tile_asset_cache *self);
extern s32 sub_8006D50(struct tile_asset_cache *self, s32 index);
extern void sub_8006DC8(struct tile_asset_cache *self);
extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006A90(struct oam_shadow_buffer *arg0);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void WaitForVBlank(void);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern u16 gStaticData_0817C512[];
extern u16 gStaticData_0817C532[];
extern u16 gStaticData_0817C552[];
extern u16 gStaticData_0817C572[];

/* The other half of the fade overlay's setup, called from
 * `sub_803472C` (actor_part87.c): flushes the shared VRAM upload cursor
 * twice, hooks `self->icons` up to the global text icon manager
 * (`gUnknown_030012DC`), fires its 7th OAM trampoline slot, clears its
 * `field_118` and re-derives the cursor's limit from `field_12c`, resets
 * the shared tile cache and pins its first four slots, seeds those four
 * slots with fixed 32-byte tile patterns from ROM data, flushes the
 * cache, sets the overlay's DISPCNT "OBJ enable" bit, and flushes the
 * OAM shadow buffer.
 *
 * Built with old_agbcc. The seeding loop is indexed through `i`: gcc
 * strength-reduces every access into its own pointer but keeps `i` as
 * the up-counting trip counter (r7) the ROM has. Written with explicit
 * pointer increments, `i` has nothing left to do but count, and gcc
 * reverses it into a down-counter. */
void sub_803487C(struct fade_overlay *self)
{
    struct icon_manager *icons;
    struct tile_asset_cache *cache;
    s32 i;

    gUnknown_030012FC->field_08 = 0;
    sub_8006C4C(gUnknown_030012FC);
    sub_8006C4C(gUnknown_030012FC);

    icons = gUnknown_030012DC;
    self->icons = icons;
    icons->field_108 = 0;
    {
        u8 *rec = (u8 *)icons->record + 0x40;
        _call_via_r1((u8 *)icons + *(s16 *)rec, *(void **)(rec + 4));
    }
    self->icons->field_118 = 0;
    sub_8006C58(gUnknown_030012FC, self->icons->field_12c << 5);
    sub_8006C30(gUnknown_030012FC);

    sub_8006EA8(gUnknown_030012B8);
    sub_8006D50(gUnknown_030012B8, 0);
    sub_8006D50(gUnknown_030012B8, 1);
    sub_8006D50(gUnknown_030012B8, 2);
    sub_8006D50(gUnknown_030012B8, 3);

    cache = gUnknown_030012B8;
    {
        u16 *destA = (u16 *)cache->slots[0];
        u16 *destB = (u16 *)cache->slots[2];

        for (i = 0; i < 16; i++) {
            destA[i] = gStaticData_0817C512[i];
            destA[i + 0x10] = gStaticData_0817C532[i];
            destB[i] = gStaticData_0817C552[i];
            destB[i + 0x10] = gStaticData_0817C572[i];
        }
    }
    sub_8006DC8(gUnknown_030012B8);

    ((u8 *)&self->dispcnt)[1] |= 0x10;

    sub_8006A90(gUnknown_03001300);
    sub_8006A48(gUnknown_03001300);
    WaitForVBlank();
    sub_8006AAC(gUnknown_03001300);
}
