#include "core.h"
#include "graphics_package.h"

/* GitHub issue #30's `LoadGraphicsPackage` itself - the "front door" of the
 * 0x0801E578-0x0801FA3C cluster docs/rom_map.md anchored on this function.
 * Loads a palette, a tileset, and a tilemap for one BG background, using
 * the same 5-field `struct bg_package` (include/graphics_package.h)
 * `LoadBg2Background`/`LoadObjSpriteTiles` (src/graphics/level_graphics.c)
 * already established, plus the small 0x10-byte scratch "self" buffer
 * `sub_801E640`/`sub_801E644`/`sub_801E8F8`/`sub_801E964`/`sub_801E96C`
 * (src/graphics/graphics_package_1e640.c, _1e8f8.c, _1e964.c) build up
 * field-by-field before calling here - see those files' docs for what's
 * known about each field:
 *  - `self+0x00`: a tile-bank index (used here as the tileset's VRAM char
 *    base, `<<14` i.e. one 0x4000-byte char-block per unit).
 *  - `self+0x04`: a tilemap screen-block index (used here as the
 *    destination tilemap's VRAM base, `<<11` i.e. one 0x800-byte
 *    screen-block per unit).
 *  - `self+0x08`: a palette-bank index, used both for the palette's own
 *    destination (`<<5`, one 0x20-byte 16-color bank per unit) and as the
 *    per-tile palette-select nibble merged into every copied tilemap entry
 *    (`<<12`, bits 12-15 of a BG tilemap halfword).
 *  - `self+0x0c`: the packed BG control byte `sub_801E644` already builds
 *    (priority/char-base/screen-base/color-mode) - this function only
 *    touches its top bit (bit 7, the 256-color/16-color mode select), set
 *    when the palette asset's own declared color count (its header word's
 *    top 24 bits) is bigger than 0x20.
 *
 * Parked (`NON_MATCHING`), not matched: every operation, and almost every
 * register choice, now matches the ROM exactly after heavy register
 * pinning (`self`->r5, `pkg`->r6, `mapBuf`->r8, `src`->r7, the packed
 * palette-bank mask->ip, `width`->r4, `height`->sl, the row stride->sb,
 * the per-row dest pointer->r0, the inner-loop src/dest/count triple->
 * r2/r1/r3) plus several instruction-order fixes for this compiler's
 * "skips an apparently-redundant copy" habit (matching_decomp_register_
 * pinning memory - see the `asm("":"+r"(...))` barriers below). The one
 * gap that resisted every phrasing tried: this function needs r6 free for
 * one more scratch temp (the loaded tilemap halfword, right before it's
 * ORed with the palette-bank mask) *inside* the same window `pkg`'s own
 * r6 binding is still technically in scope for - reusing r6 there (to
 * match the ROM's own `ldrh r6,...`) makes gcc's allocator, for reasons
 * that didn't yield to further restructuring, stop treating `src`'s r7 as
 * needing a callee-save push/pop at all, even though the function body
 * still writes and later reads it - i.e. it's either byte-exact bar one
 * dropped push/pop pair (semantically self-consistent, but not
 * ABI-correct - a real bug, not just a cosmetic mismatch), or ABI-correct
 * with r6 landing on a different scratch temp than the ROM picked. This
 * is the same category of first-pass-vs-second-pass register-pressure
 * artifact already documented for `LoadBg2Background` right next door in
 * spirit (src/graphics/level_graphics.c) and `sub_801E644` (this same
 * cluster) - see docs/matching/issue-30-graphics-loading.md. */
#if NON_MATCHING
extern void LoadTaggedAsset(void *asset, void *dest);
extern void *sub_8026EC0(u32 size);
extern void sub_8026EB4(void *ptr);

void LoadGraphicsPackage(u8 *selfArg, struct bg_package *pkgArg)
{
    /* Register-pinned to match the ROM's own allocation for this large,
     * register-starved function (matching_decomp_register_pinning
     * memory) - see the file comment above for the one gap that's left. */
    register u8 *self asm("r5") = selfArg;
    register struct bg_package *pkg asm("r6") = pkgArg;
    register void *mapBuf asm("r8");
    register u16 *src asm("r7");
    register u32 paletteBankMask asm("ip");
    register s32 width asm("r4");
    register s32 height asm("sl");
    register s32 stride asm("sb");
    /* Pinned to r0 to match the ROM keeping this value untouched in r0
     * across the loop-guard test below. */
    register u16 *destRow asm("r0");
    /* `volatile` forces every access through a real stack slot rather
     * than staying in a register - matching the ROM's own `str r1,[sp]`/
     * `ldr r0,[sp]` round-trip below, needed because every other live
     * value at that point in the loop (`self`-turned-row-counter,
     * `pkg`-turned-scratch, `mapBuf`, `src`, `paletteBankMask`, `width`,
     * `height`, `stride`) is already pinned to one of the twelve
     * available registers with nothing spare. */
    u16 * volatile nextDest;
    register s32 row asm("r5");

    if ((*(u32 *)pkg->paletteAsset >> 8) <= 0x20) {
        /* Constant materialized into its own register and barriered
         * before the memory read, matching the ROM's own
         * immediate-then-load-then-combine instruction order. */
        s32 mask = 0x7f;
        asm("" : "+r"(mask));
        self[0xc] = mask & self[0xc];
    } else {
        s32 mask = 0x80;
        asm("" : "+r"(mask));
        self[0xc] = mask | self[0xc];
    }

    LoadTaggedAsset(pkg->paletteAsset,
                     (void *)(0x05000000 + (*(u32 *)(self + 8) << 5)));
    LoadTaggedAsset(pkg->tileAsset,
                     (void *)(0x06000000 + (*(u32 *)(self + 0) << 14)));

    mapBuf = sub_8026EC0((*(u32 *)pkg->mapAsset >> 9) << 1);
    LoadTaggedAsset(pkg->mapAsset, mapBuf);

    paletteBankMask = *(u32 *)(self + 8) << 12;
    src = mapBuf;
    destRow = (u16 *)(0x06000000 + (*(u32 *)(self + 4) << 11));
    height = pkg->height;
    if (0 < height) {
        width = pkg->width;
        stride = width * 2;
        row = 0;
        do {
            u16 *dest;

            /* Computed eagerly (rather than at the bottom of the loop)
             * to match the ROM's own instruction order - and stashed to
             * a real stack slot rather than a register (see the
             * `volatile nextDest` comment above). */
            nextDest = (u16 *)((u8 *)destRow + 0x40);
            row++;
            dest = destRow;

            if (width > 0) {
                register u16 *s asm("r2") = src;
                register u16 *d asm("r1") = dest;
                register s32 count asm("r3") = width;
                do {
                    /* `tmp` pinned to r6, matching the ROM's own
                     * `ldrh r6,...` - see the file comment above for why
                     * this specific pin trades away r7's callee-save
                     * push/pop instead of matching it too. */
                    u32 mask = paletteBankMask;
                    register u16 tmp asm("r6");
                    asm("" : "+r"(mask));
                    tmp = *s;
                    *d = mask | tmp;
                    s++;
                    d++;
                    count--;
                } while (count != 0);
            }

            src = (u16 *)((u8 *)src + stride);
            destRow = nextDest;
        } while (row < height);
    }

    if (mapBuf != 0) {
        sub_8026EB4(mapBuf);
    }
}
#endif /* NON_MATCHING */
