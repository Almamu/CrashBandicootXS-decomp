#include "core.h"

/* Sits right after the still-parked sub_80010E0 (asm/code_3_1_5.s) and
 * before whatever's still raw in asm/code_3_1_6.s. */

extern void LZ77UnCompWrapper(void *src);
extern void RLUnCompWrapper(void *src);

/* Decompresses (or raw-DMA-copies) a "tagged" asset into `dest`. The
 * asset's first word's high nibble (bits 28-31) selects the format:
 * 0 = uncompressed (DMA3, word transfers, size taken from the
 * remaining 24 bits of the header word), 1 = LZ77, 3 = run-length;
 * anything else (2, or 4-15) is a silent no-op. Written as a `switch`
 * (not an if/else chain) specifically to reproduce the ROM's exact
 * compare sequence (`cmp #1; beq; cmp #1; blo; cmp #3; beq; b`) - an
 * equivalent if/else chain compiles to a shorter, differently-ordered
 * set of comparisons instead. `case 0` is listed *before* `case 1` in
 * this switch (despite being checked *after* it, via the `blo`) because
 * agbcc lays out non-jump-table switch case bodies in source order, not
 * check order or case-value order - the ROM's own case-0 body sits
 * physically first, right after the compare chain, with case 1's body
 * coming after it. */
void LoadTaggedAsset(void *asset, void *dest)
{
    u32 header = *(u32 *)asset;
    u32 type = (header << 24) >> 28;

    switch (type) {
    case 0: {
        vu32 *dma = (vu32 *)0x040000D4;
        u32 size;
        dma[0] = (u32)asset + 4;
        dma[1] = (u32)dest;
        size = *(u32 *)asset;
        dma[2] = ((size >> 8) - 4) >> 2 | 0x84000000;
        size = dma[2];
        break;
    }
    case 1:
        LZ77UnCompWrapper(asset);
        break;
    case 3:
        RLUnCompWrapper(asset);
        break;
    }
}

extern void sub_80006A8(void);

/* Loads a specific background's tile/tileset data (tagged asset at
 * `asset + 0x200`) into VRAM at `0x06000000`, then DMAs the first 0x200
 * bytes of `asset` (a raw palette, 256 halfwords) straight into palette
 * RAM at `0x05000000`. */
void sub_80011C0(void *asset)
{
    vu32 *dma;
    u32 val;

    LoadTaggedAsset((u8 *)asset + 0x200, (void *)0x06000000);
    sub_80006A8();
    dma = (vu32 *)0x040000D4;
    dma[0] = (u32)asset;
    dma[1] = 0x05000000;
    dma[2] = 0x80000100;
    val = dma[2];
}

/* Trailing padding (see matching_decomp_alignment_fix memory). */
asm(".align 2, 0");
