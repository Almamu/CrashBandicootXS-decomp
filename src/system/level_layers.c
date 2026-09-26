#include "core.h"
#include "gba/dma_macros.h"

/* GitHub issue #43: the level-layers singleton (`gUnknown_0300084C`,
 * 0x2C bytes, created on first use by `sub_80268AC`) - the object
 * `gUnknown_03001308` also points at: the camera (`camera_follow.c`)
 * clamps into its scroll fields via `sub_80268D0`, and
 * `game_loop44.c`/`game_loop45.c` already reach its terrain tile cache
 * at `+0x20`.
 *
 * It owns BG layer 0 (`+0x10`, a 0x60-byte tile-slot-pooled layer, see
 * `tile_slot_pool.c`), three BG-scroll layers for BG1-3 (`+0x14`-`+0x1C`,
 * `sub_8025D74`), the terrain tile cache (`+0x20`, 0x1064 bytes) and an
 * optional level asset (`+0x24`, heap-owned when `+0x28` is set).
 *
 * - `sub_80267A0` - constructor; `sub_80268AC` - get-or-create.
 * - `sub_802680C(self, flags)` - destructor: frees the asset, destroys
 *   each layer through its method table (`destroy`, called with 3), the
 *   tile cache via `sub_8025444(.., 3)`, clears the singleton pointer,
 *   and frees `self` when `flags & 1` - the same flags convention as
 *   `sub_8025444` and `sub_8026418`.
 * - `sub_80266BC(self, args)` - level load: unpacks or references the
 *   asset, feeds each layer and the tile cache its data from the level
 *   descriptor, sets the scroll limits to layer 0's size minus the
 *   240x160 screen, and DMA3-copies the BG palette.
 * - `sub_80268D0(self, x, y)` - takes a Q8 camera position, clamps it to
 *   `[0, maxScroll]` in pixels and stores it as the scroll position.
 * - `sub_802692C`/`sub_8026984` - call one method of layer 0 with the
 *   scroll position, then the same method of each enabled BG1-3 layer
 *   with layer 0's resulting position. `sub_80268F8` calls
 *   `sub_8025F24` on layer 0 and each enabled layer.
 * - `sub_80269DC`/`sub_80269F8` - identical predicates: 0 if `arg1`,
 *   `*arg2` and `arg3` are all nonzero, else 1. `sub_8026A14` returns 0.
 *
 * The method-table entries are a 16-bit `this` adjustment plus a function
 * pointer, called through `sub_803AD80`. Function names stay
 * `sub_XXXXXXXX` (docs/naming.md). Only `sub_80268D0` needed a matching
 * tweak (separate temps per axis). See
 * docs/matching/issue-43-level-layers.md.
 *
 * Real bytes formerly the whole of `asm/code_3_2_17_266bc.s`. */

struct layer_method
{
    s16 thisOffset; // 0x0 - added to the object pointer before the call
    u8 unk_2[2];    // 0x2
    void *fn;       // 0x4
};

struct layer_vtable
{
    u8 unk_00[8];              // 0x00
    struct layer_method destroy; // 0x08 - called with 3 by sub_802680C
    struct layer_method method_10; // 0x10 - sub_8026984
    struct layer_method method_18; // 0x18 - sub_802692C
};

struct layer
{
    s32 x;                      // 0x00
    s32 y;                      // 0x04
    u8 unk_08[8];               // 0x08
    s32 width;                  // 0x10 - pixels (layer 0 only, see sub_80266BC)
    s32 height;                 // 0x14
    u8 unk_18[0x10];            // 0x18
    u8 enabled;                 // 0x28
    u8 unk_29[7];               // 0x29
    struct layer_vtable *vtable; // 0x30
};

struct tile_cache;

struct level_desc
{
    void *layerData[3];  // 0x00 - for layers[0..2]
    void *layer0Data;    // 0x0C
    void *tileData;      // 0x10
    u32 *asset;          // 0x14 - first word >> 8 is the unpacked size
    u8 assetPacked;      // 0x18
    u8 unk_19[3];        // 0x19
    void *unk_1C;        // 0x1C - passed to sub_80255D4
    void *unk_20;        // 0x20 - passed to sub_80255D4
};

struct level_load_args
{
    u32 palette;             // 0x0 - DMA source for the BG palette
    struct level_desc *desc; // 0x4
};

struct level_layers
{
    s32 maxScrollX;            // 0x00 - pixels
    s32 maxScrollY;            // 0x04
    s32 scrollX;               // 0x08 - pixels
    s32 scrollY;               // 0x0C
    struct layer *layer0;      // 0x10 - 0x60 bytes, sub_8026448
    struct layer *layers[3];   // 0x14 - 0x5C bytes each, sub_8025D74(.., 1..3)
    struct tile_cache *tiles;  // 0x20 - 0x1064 bytes
    void *asset;               // 0x24
    u8 assetOwned;             // 0x28
    u8 unk_29;                 // 0x29
    u8 unk_2A;                 // 0x2A
    u8 unk_2B;                 // 0x2B
};

extern struct level_layers *gUnknown_0300084C;
extern void *gUnknown_030012B4;

extern void *sub_8026EC0(u32 size);
extern void *sub_8026EDC(u32 size);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *ptr);
extern void LoadTaggedAsset(void *asset, void *dest);
extern void sub_80260D4(struct layer *layer, void *data);
extern void sub_80254F8(struct tile_cache *self, void *source);
extern void sub_80015D0(void);
extern void sub_80015C0(void);
extern void sub_80015B0(void);
extern void sub_80015A0(void);
extern void sub_80255D4(void *self, void *arg1, void *arg2, s32 arg3, s32 arg4);
extern struct layer *sub_8026448(void *mem, s32 arg1);
extern struct tile_cache *nullsub_4(void *mem);
extern struct layer *sub_8025D74(void *mem, s32 bgIndex);
extern s32 sub_803AD80(void *self, void *arg1, void *fn);
extern void sub_8025444(struct tile_cache *self, u32 flags);
extern void sub_8025F24(struct layer *layer);

void sub_80266BC(struct level_layers *self, struct level_load_args *args)
{
    struct dma_regs *dma;
    u32 pltt;
    struct level_desc *desc = args->desc;

    if (desc->assetPacked == 0)
    {
        self->asset = desc->asset;
        self->assetOwned = 0;
    }
    else
    {
        self->asset = sub_8026EC0(*desc->asset >> 8);
        LoadTaggedAsset(args->desc->asset, self->asset);
        self->assetOwned = 1;
    }

    sub_80260D4(self->layer0, args->desc->layer0Data);
    sub_80254F8(self->tiles, args->desc->tileData);
    self->maxScrollX = self->layer0->width - DISPLAY_WIDTH;
    self->maxScrollY = self->layer0->height - DISPLAY_HEIGHT;
    sub_80015D0();

    sub_80260D4(self->layers[0], args->desc->layerData[0]);
    if (self->layers[0]->enabled)
        sub_80015C0();
    sub_80260D4(self->layers[1], args->desc->layerData[1]);
    if (self->layers[1]->enabled)
        sub_80015B0();
    sub_80260D4(self->layers[2], args->desc->layerData[2]);
    if (self->layers[2]->enabled)
        sub_80015A0();

    sub_80255D4(gUnknown_030012B4, args->desc->unk_1C, args->desc->unk_20, 0, 0);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = args->palette;
    pltt = PLTT;
    dma->dst = pltt;
    dma->cnt = 0x80000100;
    (void)dma->cnt;
    *(vu16 *)pltt = 0;
}

struct level_layers *sub_80267A0(struct level_layers *self)
{
    self->layer0 = sub_8026448(sub_8026EDC(0x60), 0);
    self->tiles = nullsub_4(sub_8026EDC(0x1064));
    self->layers[0] = sub_8025D74(sub_8026EDC(0x5C), 1);
    self->layers[1] = sub_8025D74(sub_8026EDC(0x5C), 2);
    self->layers[2] = sub_8025D74(sub_8026EDC(0x5C), 3);
    self->unk_29 = 0;
    self->asset = NULL;
    self->assetOwned = 0;
    self->unk_2B = 0;
    self->unk_2A = 0;
    return self;
}

#define DESTROY_LAYER(layer) \
    if ((layer) != NULL) \
        sub_803AD80((u8 *)(layer) + (layer)->vtable->destroy.thisOffset, (void *)3, (layer)->vtable->destroy.fn)

void sub_802680C(struct level_layers *self, u32 flags)
{
    if ((self->assetOwned != 0 || self->asset != NULL) && self->asset != NULL)
        sub_8026EB4(self->asset);

    DESTROY_LAYER(self->layer0);
    if (self->tiles != NULL)
        sub_8025444(self->tiles, 3);
    DESTROY_LAYER(self->layers[0]);
    DESTROY_LAYER(self->layers[1]);
    DESTROY_LAYER(self->layers[2]);

    gUnknown_0300084C = NULL;
    if (flags & 1)
        sub_8026ED0(self);
}

struct level_layers *sub_80268AC(void)
{
    if (gUnknown_0300084C == NULL)
        gUnknown_0300084C = sub_80267A0(sub_8026EDC(sizeof(struct level_layers)));
    return gUnknown_0300084C;
}

void sub_80268D0(struct level_layers *self, s32 x, s32 y)
{
    s32 sx, sy;

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    x >>= 8;
    y >>= 8;

    sx = self->maxScrollX;
    if (sx > x)
        sx = x;
    self->scrollX = sx;

    sy = self->maxScrollY;
    if (sy > y)
        sy = y;
    self->scrollY = sy;
}

void sub_80268F8(struct level_layers *self)
{
    s32 i;

    sub_8025F24(self->layer0);
    for (i = 0; i < 3; i++)
    {
        struct layer *layer = self->layers[i];
        if (layer->enabled)
            sub_8025F24(layer);
    }
}

void sub_802692C(struct level_layers *self)
{
    s32 pos[2];
    s32 i;

    sub_803AD80((u8 *)self->layer0 + self->layer0->vtable->method_18.thisOffset, &self->scrollX,
                self->layer0->vtable->method_18.fn);
    pos[0] = self->layer0->x;
    pos[1] = self->layer0->y;
    for (i = 0; i < 3; i++)
    {
        struct layer *layer = self->layers[i];
        if (layer->enabled)
            sub_803AD80((u8 *)layer + layer->vtable->method_18.thisOffset, pos, layer->vtable->method_18.fn);
    }
}

void sub_8026984(struct level_layers *self)
{
    s32 pos[2];
    s32 i;

    sub_803AD80((u8 *)self->layer0 + self->layer0->vtable->method_10.thisOffset, &self->scrollX,
                self->layer0->vtable->method_10.fn);
    pos[0] = self->layer0->x;
    pos[1] = self->layer0->y;
    for (i = 0; i < 3; i++)
    {
        struct layer *layer = self->layers[i];
        if (layer->enabled)
            sub_803AD80((u8 *)layer + layer->vtable->method_10.thisOffset, pos, layer->vtable->method_10.fn);
    }
}

s32 sub_80269DC(void *self, s32 arg1, s32 *arg2, s32 arg3)
{
    s32 result = 1;

    if (arg1 != 0 && *arg2 != 0 && arg3 != 0)
        result = 0;
    return result;
}

s32 sub_80269F8(void *self, s32 arg1, s32 *arg2, s32 arg3)
{
    s32 result = 1;

    if (arg1 != 0 && *arg2 != 0 && arg3 != 0)
        result = 0;
    return result;
}

s32 sub_8026A14(void)
{
    return 0;
}
