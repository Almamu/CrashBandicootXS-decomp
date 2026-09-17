#include "core.h"
#include "vram_pool.h"
#include "gba/dma_macros.h"
#include "memory.h"

struct counter_widget {
    u32 field_0;
    u8 field_4;
    u8 pad_5[3];
    s32 field_8;
    u8 field_c;
    u8 field_d;
    u8 pad_e[2];
    void *field_10;
};

extern struct oam_shadow_buffer *gUnknown_03001300;
extern struct tile_asset_cache *gUnknown_030012B8;

extern void sub_80006A8(void);
extern void sub_8006EA8(struct tile_asset_cache *self);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void sub_8006DC8(struct tile_asset_cache *arg0);
extern void FlushVramDmaQueue(void);

extern u8 gStaticData_0816C484[];

extern void *sub_8026EDC(s32 size);
extern void sub_80346FC(void *self, s32 arg1);
extern void sub_8026ED0(void *self);
extern void *sub_8034374(void *arg0);
extern void LoadGraphicsPackage(void *buf, void *asset);
extern void *sub_801E644(void *buf, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 sub_801E640(void *buf);
extern void sub_800132C(u8 flags, s32 frameDelay, u8 sync);
extern void sub_8001604(void);
extern void sub_80015E0(void);
extern void sub_8001524(s32 val);
extern void sub_8001614(void);
extern struct counter_widget *gUnknown_030008CC;

/* Left raw (asm/code_3_2_20a.s, alongside sub_80372BC) rather than
 * matched here - it fully decodes (initializes gUnknown_030012B8's tile
 * cache with 4 fixed OBJ tiles, then copies a few icon_manager fields
 * from gUnknown_030012DC's instance into gUnknown_030012E0's), but hits
 * the same class of gcc-2.9 register-allocation difficulty already
 * documented for sub_8006600 (src/graphics/oam_count.c) - the compiler
 * keeps reaching for r8/r9/sl instead of the ROM's plain r4-r7 reuse no
 * matter how the source is rephrased (indexed vs pointer-increment copy
 * loop, address-of-global caching, ...). Parking it properly (the
 * SUB_8006600-style register-pin macros) would need more of that same
 * heavy, per-call-site engineering than this pass has budget for. */
extern void sub_8037388(void *unused);

/* Resets `self`'s two byte flags, requests a BG tile/map graphics
 * package, and sets BG0's control register from it - a shared "load my
 * background" helper for the widget above. */
void sub_80374D0(struct counter_widget *self)
{
    u8 buf[0x10];
    u32 zero = 0;
    s32 a;
    register s32 b asm("r2");
    register s32 mask asm("r1");

    *(u16 *)&self->field_c = zero;
    a = 0x40;
    a |= self->field_c;
    a &= -8;
    a |= 1;
    self->field_c = a;
    b = 1;
    b |= self->field_d;
    mask = -3;
    b &= mask;
    b |= 0x10;
    self->field_d = b;

    sub_801E644(buf, 2, 0x1e, 1, 3);
    LoadGraphicsPackage(buf, gStaticData_0816C484);
    REG_BG0CNT = sub_801E640(buf);
    *(vu32 *)REG_ADDR_BG0HOFS = zero;
}

s32 sub_8037534(s32 *arg0)
{
    if ((*arg0 >> 2) & 1) {
        return 1;
    }
    return 2;
}

void sub_8037548(struct counter_widget *self)
{
    REG_DISPCNT = *(u16 *)&self->field_c;
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    sub_8006DC8(gUnknown_030012B8);
    sub_8006AAC(gUnknown_03001300);
    FlushVramDmaQueue();
}

void sub_8037578(void *self, u32 flags)
{
    void *field10 = *(void **)((u8 *)self + 0x10);

    if (field10 != NULL) {
        sub_80346FC(field10, 3);
    }
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Loads the widget's graphics, sets up its (16-tile) map/palette upload
 * request, and kicks off the fade/screen machinery. Returns `self`. */
void *sub_80375A0(struct counter_widget *self)
{
    sub_8006EA8(gUnknown_030012B8);
    sub_8037388(self);
    sub_80374D0(self);
    self->field_10 = sub_8034374(sub_8026EDC(0x14));
    sub_800132C(0x80, 1, 0);
    sub_8001604();
    sub_80015E0();
    sub_8001524(1);
    sub_8001614();
    return self;
}

void sub_80375EC(void)
{
    sub_800132C(0, 1, 0);
    if (gUnknown_030008CC != NULL) {
        sub_8037578(gUnknown_030008CC, 3);
    }
    gUnknown_030008CC = NULL;
    sub_8006EA8(gUnknown_030012B8);
}

void sub_8037620(void)
{
    sub_8006EA8(gUnknown_030012B8);
    gUnknown_030008CC = sub_80375A0(sub_8026EDC(0x14));
}
