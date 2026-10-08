#ifndef GUARD_TAGGED_ASSET_H
#define GUARD_TAGGED_ASSET_H

#include "gba/types.h"

/*
 * A tag-0x00 ("raw") tagged asset, as LoadTaggedAsset
 * (src/system/asset.cpp) reads it: one header word whose bits 4-7 are
 * the format (0 = uncompressed, 1 = LZ77, 3 = run-length) and whose top
 * 24 bits are the payload size, then the payload itself. The LZ77 and RL
 * variants are the GBA BIOS stream formats; they are built from their own
 * sources and don't use this type.
 *
 * Every object gets its own anonymous struct type, sized to its payload:
 *
 *     const TAGGED_RAW_ASSET(0x1000) gFoo = {
 *         TAGGED_RAW_HEADER(0x1000),
 *         {
 *     #include "dir/foo.img.bin.inc"
 *         },
 *     };
 *
 * agbcc pads a struct to 4 bytes, so the payload size must be a multiple
 * of 4 (true of every tile set).
 */
#define TAGGED_RAW_ASSET(size) struct { u32 header; u8 data[size]; }
#define TAGGED_RAW_HEADER(size) ((u32)(size) << 8)

#endif // GUARD_TAGGED_ASSET_H
