#include "core.h"
#include "icon_manager.h"
#include "gba/dma_macros.h"

/* GitHub issue #65's chunk (0x080354E0-0x08037110) starts here, right at
 * the 40.4 KB actor-per-type-behavior zone's own end (docs/rom_map.md's
 * `0x0802B348`-`0x080354E0` entry) - `LoadLevelGraphics`/
 * `LoadBg2Background`/`LoadObjSpriteTiles` are already named and were
 * already high-confidence `graphics_loading` per docs/rom_map.md's own
 * table before this chunk (`0x080354E0`-`0x08035780`ish). */

extern struct icon_manager *gUnknown_030012DC;
extern struct oam_shadow_buffer *gUnknown_03001300;
extern struct AudioContext *gUnknown_030012BC;

extern void sub_8006A90(struct oam_shadow_buffer *arg0);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_80006A8(void);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void sub_8028A30(struct icon_manager *self, u8 val);
extern void *sub_803AD7C(void *arg0, void *fn);
extern void *sub_8026EDC(s32 size);
extern void *sub_8034374(void *arg0);
extern void sub_8001604(void);
extern void sub_80015E0(void);
extern void sub_8001524(s32 val);
extern void sub_8001614(void);
extern void sub_80017BC(struct AudioContext *self, u32 songIndex);

extern u8 gStaticData_0817D034[0x20];
extern u8 gStaticData_0817D054[0x20];
extern u8 gStaticData_0817D074[0x70];

/* Loaded onto BG2, via the 5-field package struct at `gStaticData_0817D0E4`
 * - see `struct bg_package`, shared with `LoadObjSpriteTiles` below. */
struct bg_package {
    u32 width;
    u32 height;
    void *paletteAsset;
    void *tileAsset;
    void *mapAsset;
};

extern struct bg_package gStaticData_0817D0E4;
extern void *gUnknown_030008BC[4];

extern void *sub_8026EC0(u32 size);
extern void sub_8026EB4(void *ptr);
extern void LoadTaggedAsset(void *asset, void *dest);

void LoadBg2Background(u32 *self);
void LoadObjSpriteTiles(u32 *self);

/* The 0x220-byte per-level scratch object `UpdateGameFrame` allocates
 * (`sub_8026EDC(0x220)`) and passes here - most of its fields are still
 * touched only by this chunk's not-yet-matched neighbors
 * (`sub_8035780`/`sub_8035E14`/`sub_8036154`/...), so it stays a raw
 * `u32 *` scratch buffer here rather than a named struct (see
 * `matching_decomp_prefer_structs`: fine to fall back to raw offsets
 * when the full shape isn't known yet) - only the three fields this
 * function itself touches (offsets 0/4/0xc/0x208) are given meaning. */
void *LoadLevelGraphics(u32 *self)
{
    struct dma_regs *dma;
    struct icon_manager *iconManager;
    struct icon_slot *slot;
    u32 fieldValue;

    self[3] = (u32)gUnknown_030012DC;
    sub_8006A90(gUnknown_03001300);
    sub_8006A48(gUnknown_03001300);
    sub_80006A8();
    sub_8006AAC(gUnknown_03001300);

    *(vu32 *)REG_ADDR_BLDCNT = 0xff;
    REG_BLDY = 0x10;
    REG_DISPCNT = 0;

    sub_8028A30((struct icon_manager *)self[3], 0xe);

    iconManager = (struct icon_manager *)self[3];
    fieldValue = 0x200;
    iconManager->field_108 = fieldValue;
    slot = &iconManager->record->slots[6];
    sub_803AD7C((u8 *)iconManager + slot->offset, slot->ptr);

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)gStaticData_0817D034;
    dma->dst = 0x050003A0;
    dma->cnt = 0x80000010;
    dma->cnt;
    dma->src = (u32)gStaticData_0817D054;
    dma->dst = 0x050003C0;
    dma->cnt = 0x80000010;
    dma->cnt;
    dma->src = (u32)gStaticData_0817D074;
    dma->dst = 0x050003E0;
    dma->cnt = 0x80000010;
    dma->cnt;

    LoadBg2Background(self);
    LoadObjSpriteTiles(self);

    {
        u32 *dest = &self[0x82];
        *dest = (u32)sub_8034374(sub_8026EDC(0x14));
    }

    sub_8001604();
    sub_80015E0();
    sub_8001524(1);
    sub_8001614();

    self[0] = 0;
    self[1] = 0;

    sub_80017BC(gUnknown_030012BC, 0xb);

    return self;
}

#if NON_MATCHING
/* Loads BG2's tileset/palette/tilemap from `gStaticData_0817D0E4`'s
 * package (see `struct bg_package` above), remapping the tilemap's
 * per-tile palette-select nibble (bits 8-15 of each source halfword)
 * into a straight palette-index byte pair as it copies it to
 * `0x0600F000`, then sets up BG2 (REG_BG2CNT).
 *
 * Parked rather than matched: every operation and every register in the
 * body itself now matches the ROM exactly (confirmed via isolated
 * compile, instruction-for-instruction, including the `width`/`height`
 * load order and the remap loop's `r6`/`r8`/`ip`/`r4` register choices -
 * `mapBuf` pinned to `r8`, `count` pinned to `ip`, `mask` pinned to `r4`
 * per matching_decomp_register_pinning), but the ROM's prologue/epilogue
 * push/pop **one extra callee-saved register** (`r7`, via `mov r7, r8`/
 * `push {r7}` at entry and the matching pop at exit) that the body never
 * actually reads or writes - every phrasing tried either reproduces this
 * exact dead `r7` slot (by leaving `mapBuf` unpinned) at the cost of
 * `dest`/`count` landing in the wrong registers, or gets `dest`/`count`
 * right (by pinning `mapBuf` to `r8`) at the cost of the shuttle
 * register collapsing to `r6` and `r7` dropping out of the push list
 * entirely. This is the same category of gcc-2.9-allocator artifact
 * already documented for `sub_801E644`
 * (docs/matching/issue-30-graphics-loading.md) - a live range the
 * allocator reserves during its first (pressure-counting) pass that
 * ends up unused by the time the second (assignment) pass runs. See
 * docs/matching/issue-65-graphics-loading.md. */
void LoadBg2Background(u32 *self)
{
    struct bg_package *pkg = &gStaticData_0817D0E4;
    /* Register-pinned to match the ROM's own register choices for the
     * remap loop below (`mapBuf` -> `r8` - the ROM shuttles it through
     * `r8` across the whole function via the `mov r7,r8`/`push {r7}`
     * prologue trick, `dest` -> `r6`, `count` -> `ip`/`r12`) - see
     * matching_decomp_register_pinning memory. */
    register u16 *mapBuf asm("r8");
    vu16 *dest;
    s32 i;
    register s32 count asm("ip");
    /* Deliberately left uninitialized: the ROM builds this value with an
     * `ands r5, =0xFFFF0000` against whatever was already in the
     * register, immediately followed by the four `orrs` below that fill
     * in every bit the final `REG_BG2CNT` write actually reads - the
     * negative-constant bit-clear idiom (docs/matching.md), applied to
     * an unread starting value rather than an existing one. */
    u32 bg2cnt;

    LoadTaggedAsset(pkg->paletteAsset, (void *)0x05000000);
    LoadTaggedAsset(pkg->tileAsset, (void *)0x06008000);

    mapBuf = sub_8026EC0(pkg->height * pkg->width * 2);
    LoadTaggedAsset(pkg->mapAsset, mapBuf);

    dest = (vu16 *)0x0600F000;
    i = 0;
    count = pkg->height * pkg->width;
    if (i < count) {
        /* Register-pinned to match the ROM's own reuse of `r4` (this
         * function's `pkg` pointer, dead by this point) for the mask -
         * see matching_decomp_register_pinning memory. */
        register u32 mask asm("r4") = 0xff;
        u16 *src = mapBuf;

        do {
            u16 lo = mask & *src;
            u16 hi = (mask & src[1]) << 8;
            *dest = lo | hi;
            dest++;
            src += 2;
            i += 2;
        } while (i < count);
    }

    bg2cnt &= -0x10000;
    bg2cnt |= 8;
    bg2cnt |= 0xf0 << 5;
    bg2cnt |= 0x80;
    bg2cnt |= 1;
    REG_BG2CNT = bg2cnt;

    if (mapBuf != NULL) {
        sub_8026EB4(mapBuf);
    }
}
#endif /* NON_MATCHING */

#if NON_MATCHING
/* Uploads the 4 obj-sprite `struct bg_package` entries in
 * `gUnknown_030008BC` (each package's `width`/`height` describe the
 * tilemap, not the object's own screen size) into OBJ VRAM
 * (`0x06010000` on) and OBJ palette RAM (`0x05000200` on, one 16-color
 * bank - 0x20 bytes - per package), remapping each package's tilemap
 * into a straight tile copy the same way `LoadBg2Background` remaps
 * BG2's (here: DMA-copying each referenced tile out of the raw tileset
 * buffer, tile-index byte selecting which 0x20-byte 4bpp tile).
 *
 * Parked, not matched: the overall shape (4-pass loop, palette DMA then
 * tile-buffer DMA then per-tile remap DMA) is confirmed against the ROM
 * and this reconstruction is semantically faithful, but it hasn't had
 * the same per-register tuning pass `LoadBg2Background` above got -
 * isolated compiles put several locals (the `struct bg_package **`
 * array-walk pointer, the per-pass palette/tile-VRAM cursors) in
 * different registers than the ROM's own `sl`/`sb`/`r8` allocation.
 * Left for a follow-up pass rather than force a low-confidence match -
 * see docs/matching/issue-65-graphics-loading.md. */
void LoadObjSpriteTiles(u32 *self)
{
    struct bg_package **pkgPtr = (struct bg_package **)gUnknown_030008BC;
    struct bg_package **nextPkgPtr;
    void *paletteDest = (void *)0x05000200;
    void *tileDest = (void *)0x06010000;
    struct dma_regs *dma;
    void *paletteBuf;
    void *tileBuf;
    u16 *mapBuf;
    s32 count;
    s32 pass;

    for (pass = 0; pass <= 3; pass++) {
        struct bg_package *pkg = *pkgPtr;

        paletteBuf = sub_8026EC0(*(u32 *)pkg->paletteAsset >> 8);
        LoadTaggedAsset(pkg->paletteAsset, paletteBuf);
        dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)paletteBuf;
        dma->dst = (u32)paletteDest;
        dma->cnt = 0x80000010;
        dma->cnt;
        paletteDest = (u8 *)paletteDest + 0x20;
        if (paletteBuf != NULL) {
            sub_8026EB4(paletteBuf);
        }

        tileBuf = sub_8026EC0(*(u32 *)pkg->tileAsset >> 8);
        LoadTaggedAsset(pkg->tileAsset, tileBuf);

        count = pkg->width * pkg->height;
        mapBuf = sub_8026EC0(count * 2);
        nextPkgPtr = pkgPtr + 1;
        LoadTaggedAsset((*pkgPtr)->mapAsset, mapBuf);
        pkgPtr = nextPkgPtr;

        if (count > 0) {
            s32 i = count;
            u8 *src = (u8 *)mapBuf;

            dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
            do {
                dma->src = (u32)tileBuf + ((*(u16 *)src & 0xff) << 5);
                dma->dst = (u32)tileDest;
                dma->cnt = 0x80000010;
                dma->cnt;
                tileDest = (u8 *)tileDest + 0x20;
                src += 2;
                i--;
            } while (i != 0);
        }

        if (mapBuf != NULL) {
            sub_8026EB4(mapBuf);
        }
        if (tileBuf != NULL) {
            sub_8026EB4(tileBuf);
        }
    }
}
#endif /* NON_MATCHING */
