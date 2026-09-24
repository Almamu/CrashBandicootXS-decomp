#include "core.h"
#include "icon_manager.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"

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
 * - see `struct bg_package` (include/graphics_package.h), shared with
 * `LoadObjSpriteTiles` below and with `LoadGraphicsPackage`
 * (src/graphics/graphics_package_1e578.c). */
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

/* Loads BG2's tileset/palette/tilemap from `gStaticData_0817D0E4`'s
 * package (see `struct bg_package` above), remapping the tilemap's
 * per-tile palette-select nibble (bits 8-15 of each source halfword)
 * into a straight palette-index byte pair as it copies it to
 * `0x0600F000`, then sets up BG2 (REG_BG2CNT).
 *
 * Matched as NAKED, not real decompiled C. Every operation and every
 * register in the body was already confirmed to match the ROM exactly
 * via plain C (isolated compile, instruction-for-instruction, including
 * the `width`/`height` load order and the remap loop's `r6`/`r8`/`ip`/
 * `r4` register choices - `mapBuf` pinned to `r8`, `count` pinned to
 * `ip`, `mask` pinned to `r4` per matching_decomp_register_pinning), but
 * the ROM's prologue/epilogue push/pop **one extra callee-saved
 * register** (`r7`, via `mov r7, r8`/`push {r7}` at entry and the
 * matching pop at exit) that the function body never actually reads or
 * writes - a genuinely dead register slot the ROM's own compiler
 * apparently reserved during an early register-pressure-counting pass
 * and never ended up using, but still saved/restored anyway. Every
 * plain-C phrasing tried either reproduced this exact dead `r7` slot (by
 * leaving `mapBuf` unpinned, which let the allocator naturally shuttle
 * through `r7` again) at the cost of `dest`/`count` landing in the wrong
 * registers, or got `dest`/`count` right (by pinning `mapBuf` to `r8`)
 * at the cost of the shuttle register collapsing to `r6` and `r7`
 * dropping out of the push list entirely. An explicit dummy
 * `register u32 r7dummy asm("r7")` referenced via an empty
 * `asm volatile("" : "+r"(r7dummy))` barrier (the same "force a real
 * register variable, not just a clobber string" technique that
 * unblocks other registers in this project) was also tried and made no
 * difference - confirmed with an isolated compile that still dropped
 * `r7` from both the push and pop lists even with the dummy variable
 * live across the whole function. This is the same category of
 * gcc-2.9-allocator artifact already documented for `sub_801E644`
 * (docs/matching/issue-30-graphics-loading.md) and `sub_80240E4`
 * (src/system/game_loop8.c) - a live range the allocator reserves
 * during its first (pressure-counting) pass that ends up unused by the
 * time the second (assignment) pass runs, and which this compiler never
 * adds back into its own auto-generated push/pop list no matter how the
 * dead register is referenced from inline asm. Converted to NAKED and
 * transcribed instruction-for-instruction from the ROM disassembly
 * instead - confirmed byte-identical (modulo the unresolved `bl`
 * targets and the `gStaticData_0817D0E4` literal-pool address, both
 * inherent to an isolated, unlinked object) via a direct
 * `arm-none-eabi-objcopy --only-section=.text` + byte comparison against
 * the ROM's own bytes at `0x080355E0` before integrating - the same
 * escape hatch already used for `sub_80240E4`
 * (src/system/game_loop8.c) and `sub_801E688`
 * (src/graphics/graphics_package_1e688.c). See
 * docs/matching/issue-65-graphics-loading.md. */
NAKED void LoadBg2Background(u32 *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "ldr r4, =gStaticData_0817D0E4\n\t"
        "ldr r0, [r4, #8]\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "bl LoadTaggedAsset\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "ldr r1, =0x06008000\n\t"
        "bl LoadTaggedAsset\n\t"
        "ldr r1, [r4, #4]\n\t"
        "ldr r0, [r4]\n\t"
        "mul r0, r1, r0\n\t"
        "lsl r0, r0, #1\n\t"
        "bl sub_8026EC0\n\t"
        "mov r8, r0\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "mov r1, r8\n\t"
        "bl LoadTaggedAsset\n\t"
        "ldr r6, =0x0600F000\n\t"
        "mov r3, #0\n\t"
        "ldr r1, [r4, #4]\n\t"
        "ldr r0, [r4]\n\t"
        "mul r1, r0, r1\n\t"
        "cmp r3, r1\n\t"
        "bge 2f\n\t"
        "mov r4, #0xff\n\t"
        "mov ip, r1\n\t"
        "mov r2, r8\n\t"
        "1:\n\t"
        "add r1, r4, #0\n\t"
        "ldrh r0, [r2]\n\t"
        "and r1, r0\n\t"
        "add r0, r4, #0\n\t"
        "ldrh r7, [r2, #2]\n\t"
        "and r0, r7\n\t"
        "lsl r0, r0, #8\n\t"
        "orr r1, r0\n\t"
        "strh r1, [r6]\n\t"
        "add r6, #2\n\t"
        "add r2, #4\n\t"
        "add r3, #2\n\t"
        "cmp r3, ip\n\t"
        "blt 1b\n\t"
        "2:\n\t"
        "ldr r0, =0xFFFF0000\n\t"
        "and r5, r0\n\t"
        "mov r0, #8\n\t"
        "orr r5, r0\n\t"
        "mov r0, #0xf0\n\t"
        "lsl r0, r0, #5\n\t"
        "orr r5, r0\n\t"
        "mov r0, #0x80\n\t"
        "orr r5, r0\n\t"
        "mov r0, #1\n\t"
        "orr r5, r0\n\t"
        "ldr r0, =0x0400000C\n\t"
        "strh r5, [r0]\n\t"
        "mov r0, r8\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "bl sub_8026EB4\n\t"
        "3:\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}

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
