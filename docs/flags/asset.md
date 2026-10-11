# A tagged asset's type (#843)

LoadTaggedAsset (src/system/asset.cpp) loads a BG palette, tile set or
map that starts with the GBA BIOS's decompression header: bits 0-3
reserved, bits 4-7 the compression type, bits 8-31 the size. It switches
on bits 4-7, `(header << 24) >> 28`. The names, `ASSET_TYPE_*`, are in
include/system.h, next to the prototype.

| Value | Name | LoadTaggedAsset |
|---|---|---|
| 0 | `ASSET_TYPE_RAW` | no compression: DMA3 copies `size - 4` bytes (the header's size counts itself) in words |
| 1 | `ASSET_TYPE_LZ77` | LZ77UnCompVram |
| 2 | `ASSET_TYPE_HUFFMAN` | the BIOS's Huffman type; no case, nothing is loaded |
| 3 | `ASSET_TYPE_RL` | RLUnCompVram |

Anything above 3 also loads nothing. The size (`header >> 8`) stays a
plain shift: BgSetup::Load compares a palette's size with 0x20 to pick
the BG's colour mode (docs/flags/display.md), and allocates the map from
it.
