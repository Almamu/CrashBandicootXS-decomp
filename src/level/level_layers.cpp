#include "bg_layer.hpp"
#include "entity_flags.hpp"

extern "C" {
#include "math_util.h"
#include "gba/dma_macros.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
}

/* GitHub issue #43: the level-layers singleton (LevelLayers,
 * include/bg_layer.hpp; `gLevelLayersSingleton`, 0x2C bytes, created on
 * first use by `Get`) - the object `gLevelLayers` also points at: the
 * camera (`camera.cpp`) clamps into its scroll fields via `SetScroll`, and
 * `terrain.cpp` reaches its terrain tile cache at `+0x20`.
 *
 * It owns BG layer 0 (`+0x10`, a PooledBgLayer), three BgLayers for
 * BG1-3 (`+0x14`-`+0x1C`), the terrain tile cache (`+0x20`, 0x1064 bytes,
 * tile_cache.cpp) and an optional level asset (`+0x24`, heap-owned when
 * `+0x28` is set).
 *
 * - The constructor and `Get` (get-or-create); the destructor frees the
 *   asset, deletes each layer (a virtual `delete`) and the tile cache,
 *   and clears the singleton pointer. It has no vtable: g++ calls it
 *   directly, and it frees `this` itself when told to.
 * - `LoadRoom(args)` - level load: unpacks or references the asset,
 *   feeds each layer and the tile cache its data from the level
 *   descriptor, sets the scroll limits to layer 0's size minus the
 *   240x160 screen, and DMA3-copies the BG palette.
 * - `SetScroll(x, y)` - takes a Q8 camera position, clamps it to
 *   `[0, maxScroll]` in pixels and stores it as the scroll position.
 * - `Scroll`/`Reset` - call one virtual method of layer 0 with the scroll
 *   position, then the same method of each enabled BG1-3 layer with
 *   layer 0's resulting position. `CommitScroll` commits layer 0's and
 *   each enabled layer's scroll registers.
 * - `sub_80269DC`/`sub_80269F8` - identical predicates: 0 if `arg1`,
 *   `*arg2` and `arg3` are all nonzero, else 1. `sub_8026A14` returns 0.
 *   All three are UNUSED: no caller anywhere in the ROM (checked src/,
 *   asm/, the method tables and every word-aligned Thumb pointer in
 *   baserom.gba). They keep their `sub_XXXXXXXX` names (docs/naming.md):
 *   with no caller and no table slot, nothing shows what the arguments
 *   are or what the predicates answer.
 *
 * Only `SetScroll`
 * needed a matching tweak (separate temps per axis); the C's hand-written
 * method-table calls are virtual calls. See
 * docs/matching/archive/issue-43-level-layers.md.
 *
 * Real bytes formerly the whole of `asm/code_3_2_17_266bc.s`. */

void LevelLayers::LoadRoom(const struct level_room *args)
{
    struct dma_regs *dma;
    u32 pltt;
    const struct level_desc *desc = args->desc;

    if (desc->assetPacked == 0) {
        asset = (void *)desc->asset;
        assetOwned = 0;
    } else {
        asset = new u8[*(const u32 *)desc->asset >> 8];
        LoadTaggedAsset(args->desc->asset, asset);
        assetOwned = 1;
    }

    layer0->Load(args->desc->layer0);
    tiles->SetSource((struct level_layer_desc *)args->desc->collision);
    maxScrollX = layer0->widthPx - DISPLAY_WIDTH;
    maxScrollY = layer0->heightPx - DISPLAY_HEIGHT;
    ShowBg0();

    layers[0]->Load(args->desc->layers[0]);
    if (layers[0]->enabled)
        ShowBg1();
    layers[1]->Load(args->desc->layers[1]);
    if (layers[1]->enabled)
        ShowBg2();
    layers[2]->Load(args->desc->layers[2]);
    if (layers[2]->enabled)
        ShowBg3();

    gEntityFlags->SpawnRoomEntities(args->desc->entities, args->desc->links, 0, 0);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)args->palette;
    pltt = PLTT;
    dma->dst = pltt;
    dma->cnt = 0x80000100;
    (void)dma->cnt;
    *(vu16 *)pltt = 0;
}

LevelLayers::LevelLayers()
{
    layer0 = new PooledBgLayer(0);
    tiles = new TileCache;
    layers[0] = new BgLayer(1);
    layers[1] = new BgLayer(2);
    layers[2] = new BgLayer(3);
    kind = 0;
    asset = 0;
    assetOwned = 0;
    raiseObjPriority = 0;
    probeFlag = 0;
}

LevelLayers::~LevelLayers()
{
    if (assetOwned != 0 || asset != NULL)
        delete[] (u8 *)asset;

    delete layer0;
    delete tiles;
    delete layers[0];
    delete layers[1];
    delete layers[2];

    gLevelLayersSingleton = 0;
}

LevelLayers *LevelLayers::Get()
{
    if (gLevelLayersSingleton == NULL)
        gLevelLayersSingleton = new LevelLayers;
    return gLevelLayersSingleton;
}

void LevelLayers::SetScroll(s32 x, s32 y)
{
    s32 sx, sy;

    LIMIT_MIN(x, 0);
    LIMIT_MIN(y, 0);
    x = Q8_TO_INT(x);
    y = Q8_TO_INT(y);

    sx = maxScrollX;
    LIMIT_MAX(sx, x);
    scrollX = sx;

    sy = maxScrollY;
    LIMIT_MAX(sy, y);
    scrollY = sy;
}

void LevelLayers::CommitScroll()
{
    s32 i;

    layer0->CommitScroll();
    for (i = 0; i < 3; i++) {
        BgLayer *layer = layers[i];
        if (layer->enabled)
            layer->CommitScroll();
    }
}

void LevelLayers::Scroll()
{
    s32 pos[2];
    s32 i;

    layer0->Scroll(&scrollX);
    pos[0] = layer0->x;
    pos[1] = layer0->y;
    for (i = 0; i < 3; i++) {
        BgLayer *layer = layers[i];
        if (layer->enabled)
            layer->Scroll(pos);
    }
}

void LevelLayers::Reset()
{
    s32 pos[2];
    s32 i;

    layer0->Reset(&scrollX);
    pos[0] = layer0->x;
    pos[1] = layer0->y;
    for (i = 0; i < 3; i++) {
        BgLayer *layer = layers[i];
        if (layer->enabled)
            layer->Reset(pos);
    }
}

s32 LevelLayers::sub_80269DC(s32 arg1, s32 *arg2, s32 arg3)
{
    s32 result = 1;

    if (arg1 != 0 && *arg2 != 0 && arg3 != 0)
        result = 0;
    return result;
}

s32 LevelLayers::sub_80269F8(s32 arg1, s32 *arg2, s32 arg3)
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
