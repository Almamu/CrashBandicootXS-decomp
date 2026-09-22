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
    u8 unused_0e[4];
    u32 unused_10;
    u8 unused_14[4];
    struct icon_manager *icons; /* 0x18 */
};

extern struct vram_upload_cursor *gUnknown_030012FC;
extern void sub_8006C4C(struct vram_upload_cursor *self);
extern struct icon_manager *gUnknown_030012DC;
extern void *sub_803AD7C(void *arg0, void *arg1);
extern s32 sub_8006C58(struct vram_upload_cursor *self, s32 size);
extern void sub_8006C30(struct vram_upload_cursor *self);
extern struct tile_asset_cache *gUnknown_030012B8;
extern void sub_8006EA8(struct tile_asset_cache *self);
extern s32 sub_8006D50(struct tile_asset_cache *self, s32 index);
extern void sub_8006DC8(struct tile_asset_cache *self);
extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006A90(struct oam_shadow_buffer *arg0);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_80006A8(void);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern u16 gStaticData_0817C512[];
extern u16 gStaticData_0817C532[];
extern u16 gStaticData_0817C552[];
extern u16 gStaticData_0817C572[];

/* The other half of the fade overlay's setup, called from
 * `sub_803472C` (actor_part87.c): flushes/double-flushes the shared
 * VRAM upload cursor, hooks `self->icons` up to the global text icon
 * manager (`gUnknown_030012DC`), fires its 7th (index 6) OAM trampoline
 * slot (the same `icon_slot` shape `sub_8011A1C`/`actor_part39.c`
 * already established), clears its `field_118`/re-derives the cursor's
 * limit from `field_12c`, resets the shared tile cache and pins its
 * first four slots, hand-seeds those same four slots with four fixed
 * 32-byte tile patterns from ROM data, flushes the cache, sets the fade
 * overlay's DISPCNT "OBJ enable" bit, and finally flushes/double-syncs
 * the OAM shadow buffer. Returns `self`.
 *
 * Parked (`NON_MATCHING`), not matched: every field, call, and register
 * choice matches the ROM's own build exactly except the tile-cache
 * seeding loop's trip counter, which the ROM keeps in r7 for the whole
 * loop. That's this project's confirmed categorical gcc-2.9 r7-pin bug
 * (see graphics_package_1e688.c/oam_count.c/actor_part7.c and the many
 * other entries under docs/matching/naked-*.md) - an explicit
 * `register s32 counter asm("r7")` pin compiles the exact right
 * instructions but this compiler's push/pop-list computation never
 * includes r7 for it, no matter how the source is phrased (a barrier
 * spanning the `sub_8006DC8` call right after the loop, hoisting the
 * declaration to function scope, and narrowing every other pinned
 * local's scope were all tried here). The real ROM build's own r7 use
 * is otherwise unremarkable - a plain trip counter, not special in any
 * way - so this is the same bug, not a new variant of it. The real
 * bytes come from `asm/code_3_2_20_28568_c99c_31784_33ef4_3487c.s`
 * under a `.if NON_MATCHING == 0` guard, mirroring this function's own
 * `#if NON_MATCHING` guard - see docs/matching/issue-30-graphics-loading.md's
 * LoadGraphicsPackage entry for the established pattern. */
#if NON_MATCHING
struct fade_overlay *sub_803487C(struct fade_overlay *selfArg)
{
    /* Pinned to r8 (matching_decomp_register_pinning memory) - `self`
     * survives every call in this function, and with r0-r7 all claimed
     * by other locals at one point or another (`rec`, and the loop's six
     * running pointers below), r8 is the ROM's own choice too. */
    register struct fade_overlay *self asm("r8") = selfArg;
    s32 counter;
    struct icon_manager *icons;
    struct tile_asset_cache *cache;

    /* `gUnknown_030012FC` is re-read fresh at every use below rather
     * than cached in a local (matching_decomp_register_pinning memory)
     * - the ROM's own build only ever caches the global's *address* in
     * a register, re-dereferencing it after every call. */
    gUnknown_030012FC->field_08 = 0;
    sub_8006C4C(gUnknown_030012FC);
    sub_8006C4C(gUnknown_030012FC);

    icons = gUnknown_030012DC;
    self->icons = icons;
    icons->field_108 = 0;

    /* `rec` is advanced in place to point at `record->slots[6]`
     * (matching the ROM's own `adds r1, #0x40`) rather than indexing
     * `rec->slots[6]` fresh - see matching_decomp_register_pinning
     * memory. */
    {
        register u8 *rec asm("r1") = (u8 *)icons->record + 0x40;
        sub_803AD7C((u8 *)icons + *(s16 *)rec, *(void **)(rec + 4));
    }

    /* `self->icons` is re-read here rather than reusing the `icons`
     * local above - the ROM's own build re-derives it from `self` after
     * the `sub_803AD7C` call instead of keeping it live in a
     * callee-saved register. */
    self->icons->field_118 = 0;
    sub_8006C58(gUnknown_030012FC, self->icons->field_12c << 5);
    sub_8006C30(gUnknown_030012FC);

    /* `gUnknown_030012B8` is likewise re-read fresh at every use, same
     * reason as `gUnknown_030012FC` above. */
    sub_8006EA8(gUnknown_030012B8);
    sub_8006D50(gUnknown_030012B8, 0);
    sub_8006D50(gUnknown_030012B8, 1);
    sub_8006D50(gUnknown_030012B8, 2);
    sub_8006D50(gUnknown_030012B8, 3);

    cache = gUnknown_030012B8;

    /* Six running-pointer cursors plus the trip counter, explicitly
     * pinned to the ROM's own exact register choices
     * (matching_decomp_register_pinning memory) - this compiler's
     * unforced allocator reaches a different, merely-equivalent
     * permutation of the same seven registers otherwise. `destA`/`destB`
     * each cover two adjacent 32-byte slots at once via the `+0x10`
     * (halfword) stride to the second slot. */
    {
        register u16 *destA asm("r1") = (u16 *)cache->slots[0];
        register u16 *destB asm("r2") = (u16 *)cache->slots[2];
        register const u16 *srcA asm("r5") = gStaticData_0817C512;
        register const u16 *srcB asm("r6") = gStaticData_0817C532;
        register const u16 *srcC asm("r3") = gStaticData_0817C552;
        register const u16 *srcD asm("r4") = gStaticData_0817C572;

        counter = 0;
        do {
            destA[0] = srcA[0];
            destA[0x10] = srcB[0];
            destB[0] = srcC[0];
            destB[0x10] = srcD[0];
            destA++;
            destB++;
            srcA++;
            srcB++;
            srcC++;
            srcD++;
            counter++;
        } while (counter <= 0xf);

        sub_8006DC8(gUnknown_030012B8);
    }

    ((u8 *)&self->dispcnt)[1] |= 0x10;

    sub_8006A90(gUnknown_03001300);
    sub_8006A48(gUnknown_03001300);
    sub_80006A8();
    sub_8006AAC(gUnknown_03001300);

    return self;
}
#endif /* NON_MATCHING */
