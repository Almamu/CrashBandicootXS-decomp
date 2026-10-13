#include "frame_limiter.hpp"

extern "C" {
#include "core.h"
#include "system.h"
}

/* Sits right after the still-parked WaitForKeyPress (asm/code_3_1_5.s) and
 * before whatever's still raw in asm/code_3_1_6.s. */

extern "C" void LZ77UnCompVram(const void *src);
extern "C" void RLUnCompVram(const void *src);

/* Decompresses (or raw-DMA-copies) a "tagged" asset into `dest`. Bits 4-7
 * of the asset's first word, the BIOS's compression type (ASSET_TYPE_*),
 * select the format: RAW = uncompressed (DMA3, word transfers, size taken
 * from the header word's top 24 bits), LZ77, RL = run-length; anything
 * else (HUFFMAN, or 4-15) is a silent no-op. Written as a `switch`
 * (not an if/else chain) specifically to reproduce the ROM's exact
 * compare sequence (`cmp #1; beq; cmp #1; blo; cmp #3; beq; b`) - an
 * equivalent if/else chain compiles to a shorter, differently-ordered
 * set of comparisons instead. `case 0` is listed *before* `case 1` in
 * this switch (despite being checked *after* it, via the `blo`) because
 * agbcc lays out non-jump-table switch case bodies in source order, not
 * check order or case-value order - the ROM's own case-0 body sits
 * physically first, right after the compare chain, with case 1's body
 * coming after it. */
void LoadTaggedAsset(const void *asset, void *dest)
{
    u32 header = *(const u32 *)asset;
    u32 type = (header << 24) >> 28;

    switch (type) {
    case ASSET_TYPE_RAW:
        DmaCopy32(3, (const u8 *)asset + 4, dest, (*(const u32 *)asset >> 8) - 4);
        break;
    case ASSET_TYPE_LZ77:
        LZ77UnCompVram(asset);
        break;
    case ASSET_TYPE_RL:
        RLUnCompVram(asset);
        break;
    }
}

/* Loads a specific background's tile/tileset data (tagged asset at
 * `asset + 0x200`) into the start of `VRAM`, then DMAs the first 0x200
 * bytes of `asset` (a raw palette, 256 halfwords) straight into palette
 * RAM (`PLTT`). */
void LoadBackgroundTileAndPalette(const void *asset)
{
    LoadTaggedAsset((const u8 *)asset + 0x200, (void *)VRAM);
    FrameLimiter::WaitForVBlank();
    DmaCopy16(3, asset, PLTT, BG_PLTT_SIZE);
}
