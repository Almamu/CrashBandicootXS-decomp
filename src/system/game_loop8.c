#include "core.h"
#include "actor.h"

extern void *gUnknown_030012D8;
extern void *gUnknown_030012D4;
extern void *gUnknown_030012C8;
extern void *gUnknown_03001300;
extern void *gUnknown_03001308;
extern void *gUnknown_03001318;
extern void *gUnknown_030012F0;
extern void *gUnknown_030012F4;
extern void *gUnknown_030012F8;
extern void *gUnknown_030012EC;
extern void *gUnknown_0300130C;
extern u8 gUnknown_03001280[4];
extern struct tile_asset_cache *gUnknown_030012B8;

extern void sub_8006DC8(struct tile_asset_cache *self);
extern void sub_8026E6C(void *self);
extern void sub_802692C(void *self);
extern void sub_8026F54(void *self);
extern void sub_80274EC(void *self);
extern void sub_8008DC0(struct dual_array_manager *manager);
extern void *sub_803AD7C(void *arg0, void *arg1);
extern void sub_800944C(void *managerArg);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_80006A8(void);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void sub_80268F8(void *self);
extern void FlushVramDmaQueue(void);

/* Runs the DMA3/`sub_8006DC8`+`sub_8026984` refresh pass over every
 * currently-active dual-array manager, then flushes the VRAM DMA
 * queue - only while `self->0x0` is still within the "near start of
 * level" range (`<= 0x1000`), otherwise this is a no-op. */
void sub_802400C(void *self)
{
    sub_8006DC8(gUnknown_030012B8);
    sub_8026E6C(gUnknown_030012D4);
    sub_802692C(gUnknown_03001308);
    sub_8026F54(gUnknown_030012C8);

    if (*(s32 *)self <= 0x1000) {
        sub_80274EC(gUnknown_03001318);
        sub_8008DC0(gUnknown_030012F4);

        {
            register struct actor *p asm("r0") = (struct actor *)gUnknown_030012D8;
            void *tbl = p->table;
            if ((u8)(s32)sub_803AD7C((u8 *)p + *(s16 *)((u8 *)tbl + 0x28),
                                      *(void **)((u8 *)tbl + 0x2c)) != 0) {
                register struct actor *p2 asm("r0") = (struct actor *)gUnknown_030012D8;
                void *tbl2 = p2->table;
                sub_803AD7C((u8 *)p2 + *(s16 *)((u8 *)tbl2 + 0x20),
                            *(void **)((u8 *)tbl2 + 0x24));
            }
        }

        sub_8008DC0(gUnknown_030012F0);
        sub_8008DC0(gUnknown_030012EC);
        sub_800944C(gUnknown_0300130C);
        sub_8008DC0(gUnknown_030012F8);

        sub_8006A48(gUnknown_03001300);
        sub_80006A8();
        sub_8006AAC(gUnknown_03001300);
        sub_80268F8(gUnknown_03001308);
        FlushVramDmaQueue();
    }
}

#if NON_MATCHING
/* Rebuilds the `gUnknown_03001280` `REG_BLDCNT`/`REG_BLDALPHA` shadow
 * word (see `src/graphics/aabb_util.c`'s `sub_8001624`, which commits
 * this same shadow to hardware) from `self->0x18`'s (a level object)
 * raster-mode fields, and sets `gUnknown_03001308`'s `+0x2b` flag when
 * that level's mode is 1. When the level has no raster mode at all
 * (`+0x10` halfword is 0), the blend word instead gets a fixed
 * "disabled" pattern. Real bytes for the default build in
 * `asm/code_3_2_17_240e4.s`.
 *
 * NOT YET BYTE-MATCHING: every field/mask/shift/branch is confirmed
 * correct, but this compiler's natural register allocation for the
 * dense byte-level bitfield packing below picks a different register
 * for nearly every intermediate than the ROM throughout (which keeps
 * `self->0x18` in r4, the shadow-word base in r6, and threads
 * mask/value pairs through r0-r3/r7 in a specific reused order) -
 * plausible but impractical to hand-pin every single one of the ~40
 * instructions involved; parked with the naturally-allocated version
 * instead. */
void sub_80240E4(void *self)
{
    u8 *bld = gUnknown_03001280;
    void *level3001308 = gUnknown_03001308;
    void *level = *(void **)((u8 *)self + 0x18);

    *(u32 *)bld = 0;
    *((u8 *)level3001308 + 0x2b) = 0;

    if (*(u16 *)((u8 *)level + 0x10) != 0) {
        if (*(s32 *)((u8 *)level + 8) == 1) {
            *((u8 *)level3001308 + 0x2b) = 1;
        }

        level = *(void **)((u8 *)self + 0x18);
        bld[0] = (bld[0] & 0x3f) | (*((u8 *)level + 0x10) << 6);
        bld[2] = (bld[2] & 0xe0) | (*((u8 *)level + 0x12) & 0x1f);
        bld[3] = (bld[3] & 0xe0) | (*((u8 *)level + 0x13) & 0x1f);
        bld[0] |= 8;
        bld[1] = bld[1] | 1 | 2 | 4 | 0x10;
    } else {
        bld[2] = (bld[2] & 0xe0) | 0x10;
        bld[3] = (bld[3] & 0xe0) | 0x10;
        bld[0] = (bld[0] & 0x3f) | 8;
        bld[1] = bld[1] | 1 | 2 | 4 | 0x10;
    }
}
#endif
