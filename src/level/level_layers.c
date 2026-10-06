#include "core.h"
#include "gba/dma_macros.h"
#include "system.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #43: the level-layers singleton (`gLevelLayersSingleton`,
 * 0x2C bytes, created on first use by `GetLevelLayers`) - the object
 * `gLevelLayers` also points at: the camera (`camera.c`)
 * clamps into its scroll fields via `SetLevelScroll`, and
 * `terrain.c` already reach its terrain tile cache
 * at `+0x20`.
 *
 * It owns BG layer 0 (`+0x10`, a 0x60-byte tile-slot-pooled layer, see
 * `tile_slot_pool.c`), three BG-scroll layers for BG1-3 (`+0x14`-`+0x1C`,
 * `InitBgLayer`), the terrain tile cache (`+0x20`, 0x1064 bytes) and an
 * optional level asset (`+0x24`, heap-owned when `+0x28` is set).
 *
 * - `InitLevelLayers` - constructor; `GetLevelLayers` - get-or-create.
 * - `DestroyLevelLayers(self, flags)` - destructor: frees the asset, destroys
 *   each layer through its method table (`destroy`, called with 3), the
 *   tile cache via `DestroyTileCache(.., 3)`, clears the singleton pointer,
 *   and frees `self` when `flags & 1` - the same flags convention as
 *   `DestroyTileCache` and `DestroyPooledBgLayer`.
 * - `LoadRoom(self, args)` - level load: unpacks or references the
 *   asset, feeds each layer and the tile cache its data from the level
 *   descriptor, sets the scroll limits to layer 0's size minus the
 *   240x160 screen, and DMA3-copies the BG palette.
 * - `SetLevelScroll(self, x, y)` - takes a Q8 camera position, clamps it to
 *   `[0, maxScroll]` in pixels and stores it as the scroll position.
 * - `ScrollLevelLayers`/`ResetLevelLayers` - call one method of layer 0 with the
 *   scroll position, then the same method of each enabled BG1-3 layer
 *   with layer 0's resulting position. `CommitLevelScroll` calls
 *   `CommitBgLayerScroll` on layer 0 and each enabled layer.
 * - `sub_80269DC`/`sub_80269F8` - identical predicates: 0 if `arg1`,
 *   `*arg2` and `arg3` are all nonzero, else 1. `sub_8026A14` returns 0.
 *
 * The method-table entries are a 16-bit `this` adjustment plus a function
 * pointer, called through `_call_via_r2`. Function names stay
 * `sub_XXXXXXXX` (docs/naming.md). Only `SetLevelScroll` needed a matching
 * tweak (separate temps per axis). See
 * docs/matching/archive/issue-43-level-layers.md.
 *
 * Real bytes formerly the whole of `asm/code_3_2_17_266bc.s`. */

struct tile_cache;

extern s32 _call_via_r2(void *self, void *arg1, void *fn);

void LoadRoom(struct level_layers *self, const struct level_room *args)
{
    struct dma_regs *dma;
    u32 pltt;
    const struct level_desc *desc = args->desc;

    if (desc->assetPacked == 0)
    {
        self->asset = (void *)desc->asset;
        self->assetOwned = 0;
    }
    else
    {
        self->asset = OperatorNewArray(*(const u32 *)desc->asset >> 8);
        LoadTaggedAsset(args->desc->asset, self->asset);
        self->assetOwned = 1;
    }

    LoadBgLayer(self->layer0, args->desc->layer0);
    SetCollisionSource(self->tiles, (struct level_layer_desc *)args->desc->collision);
    self->maxScrollX = self->layer0->widthPx - DISPLAY_WIDTH;
    self->maxScrollY = self->layer0->heightPx - DISPLAY_HEIGHT;
    ShowBg0();

    LoadBgLayer(self->layers[0], args->desc->layers[0]);
    if (self->layers[0]->enabled)
        ShowBg1();
    LoadBgLayer(self->layers[1], args->desc->layers[1]);
    if (self->layers[1]->enabled)
        ShowBg2();
    LoadBgLayer(self->layers[2], args->desc->layers[2]);
    if (self->layers[2]->enabled)
        ShowBg3();

    SpawnRoomEntities(gEntityFlags, args->desc->entities, args->desc->links, 0, 0);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)args->palette;
    pltt = PLTT;
    dma->dst = pltt;
    dma->cnt = 0x80000100;
    (void)dma->cnt;
    *(vu16 *)pltt = 0;
}

struct level_layers *InitLevelLayers(struct level_layers *self)
{
    self->layer0 = (struct bg_scroll_layer *)InitPooledBgLayer(OperatorNew(0x60), 0);
    self->tiles = nullsub_4(OperatorNew(0x1064));
    self->layers[0] = InitBgLayer(OperatorNew(0x5C), 1);
    self->layers[1] = InitBgLayer(OperatorNew(0x5C), 2);
    self->layers[2] = InitBgLayer(OperatorNew(0x5C), 3);
    self->kind = 0;
    self->asset = NULL;
    self->assetOwned = 0;
    self->unk_2B = 0;
    self->probeFlag = 0;
    return self;
}

#define DESTROY_LAYER(layer) \
    if ((layer) != NULL) \
        _call_via_r2((u8 *)(layer) + (layer)->vtable->destroy.thisOffset, (void *)3, (layer)->vtable->destroy.fn)

void DestroyLevelLayers(struct level_layers *self, u32 flags)
{
    if ((self->assetOwned != 0 || self->asset != NULL) && self->asset != NULL)
        OperatorDeleteArray(self->asset);

    DESTROY_LAYER(self->layer0);
    if (self->tiles != NULL)
        DestroyTileCache(self->tiles, 3);
    DESTROY_LAYER(self->layers[0]);
    DESTROY_LAYER(self->layers[1]);
    DESTROY_LAYER(self->layers[2]);

    gLevelLayersSingleton = NULL;
    if (flags & 1)
        OperatorDelete(self);
}

struct level_layers *GetLevelLayers(void)
{
    if (gLevelLayersSingleton == NULL)
        gLevelLayersSingleton = InitLevelLayers(OperatorNew(sizeof(struct level_layers)));
    return gLevelLayersSingleton;
}

void SetLevelScroll(struct level_layers *self, s32 x, s32 y)
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

void CommitLevelScroll(struct level_layers *self)
{
    s32 i;

    CommitBgLayerScroll(self->layer0);
    for (i = 0; i < 3; i++)
    {
        struct bg_scroll_layer *layer = self->layers[i];
        if (layer->enabled)
            CommitBgLayerScroll(layer);
    }
}

void ScrollLevelLayers(struct level_layers *self)
{
    s32 pos[2];
    s32 i;

    _call_via_r2((u8 *)self->layer0 + self->layer0->vtable->method_18.thisOffset, &self->scrollX,
                self->layer0->vtable->method_18.fn);
    pos[0] = self->layer0->x;
    pos[1] = self->layer0->y;
    for (i = 0; i < 3; i++)
    {
        struct bg_scroll_layer *layer = self->layers[i];
        if (layer->enabled)
            _call_via_r2((u8 *)layer + layer->vtable->method_18.thisOffset, pos, layer->vtable->method_18.fn);
    }
}

void ResetLevelLayers(struct level_layers *self)
{
    s32 pos[2];
    s32 i;

    _call_via_r2((u8 *)self->layer0 + self->layer0->vtable->reset.thisOffset, &self->scrollX,
                self->layer0->vtable->reset.fn);
    pos[0] = self->layer0->x;
    pos[1] = self->layer0->y;
    for (i = 0; i < 3; i++)
    {
        struct bg_scroll_layer *layer = self->layers[i];
        if (layer->enabled)
            _call_via_r2((u8 *)layer + layer->vtable->reset.thisOffset, pos, layer->vtable->reset.fn);
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
